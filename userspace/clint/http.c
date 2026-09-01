/*
 * http.c - see http.h
 *
 * HTTP/1.1 with Connection: close, which is the version of the
 * protocol that fits in a page of code and still talks to every
 * server: one request, one response, the socket ends the body. The
 * two things that cannot be skipped are chunked transfer encoding
 * (servers send it whether or not you asked) and redirects (most
 * sites are one hop from where you typed).
 *
 * Compression is asked for, and unpacked here. Clint carries an
 * inflate implementation for PNG anyway (see inflate.h), and a page
 * that arrives as a quarter of the bytes arrives in a quarter of the
 * time - which over TUS's TCP is the difference a reader notices.
 */

#include "http.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <netinet/in.h>

#include "inflate.h"
#include "tusnetutil.h"
#include "tustls.h"

/* Large enough for a short video, which is the biggest thing a
 * browser is asked to hold in one piece; past it the reply is refused
 * rather than swallowed. */
#define HTTP_MAX_BODY     (48u * 1024 * 1024)
#define HTTP_MAX_REDIRECTS 5

static char g_error[256] = "no error";

static void fail(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_error, sizeof(g_error), fmt, ap);
    va_end(ap);
}

const char *http_last_error(void) { return g_error; }

/* ---- URLs ---- */

static int default_port(const char *scheme) {
    return strcmp(scheme, "https") == 0 ? 443 : 80;
}

/*
 * Enough of RFC 3986 for a browser's address bar: absolute URLs,
 * scheme-relative ("//host/path"), root-relative ("/path") and plain
 * relative ("page.html", "../up.html"), each resolved against the
 * page it was found on.
 */
int url_parse(struct url *out, const char *text, const struct url *base) {
    memset(out, 0, sizeof(*out));

    while (*text == ' ' || *text == '\t') text++;

    const char *rest = text;
    const char *colon = strstr(text, "://");

    if (colon != NULL && colon - text < (long)sizeof(out->scheme)) {
        size_t n = (size_t)(colon - text);
        memcpy(out->scheme, text, n);
        out->scheme[n] = '\0';
        rest = colon + 3;
    } else if (text[0] == '/' && text[1] == '/') {
        snprintf(out->scheme, sizeof(out->scheme), "%s",
                 base ? base->scheme : "http");
        rest = text + 2;
    } else if (base != NULL) {
        /* Relative: everything but the path comes from the base. */
        snprintf(out->scheme, sizeof(out->scheme), "%s", base->scheme);
        snprintf(out->host, sizeof(out->host), "%s", base->host);
        out->port = base->port;

        if (text[0] == '/') {
            snprintf(out->path, sizeof(out->path), "%s", text);
        } else if (text[0] == '#' || text[0] == '\0') {
            snprintf(out->path, sizeof(out->path), "%s", base->path);
        } else {
            /* Relative to the base's directory, then fold away the
             * "." and ".." segments the same way a server would. */
            char dir[sizeof(base->path)];
            snprintf(dir, sizeof(dir), "%s", base->path);
            char *slash = strrchr(dir, '/');
            if (slash != NULL) {
                slash[1] = '\0';
            } else {
                dir[0] = '/';
                dir[1] = '\0';
            }
            snprintf(out->path, sizeof(out->path), "%s%s", dir, text);
        }
        goto normalise;
    } else {
        /* No scheme and nothing to be relative to: assume the user
         * typed a host name. */
        snprintf(out->scheme, sizeof(out->scheme), "http");
    }

    {
        /* host[:port] up to the first '/', '?' or '#'. */
        size_t n = strcspn(rest, "/?#");
        if (n == 0 || n >= sizeof(out->host)) {
            fail("no host in the address");
            return -1;
        }
        memcpy(out->host, rest, n);
        out->host[n] = '\0';
        rest += n;

        char *port = strchr(out->host, ':');
        if (port != NULL) {
            *port = '\0';
            out->port = atoi(port + 1);
        }
        snprintf(out->path, sizeof(out->path), "%s", *rest ? rest : "/");
    }

normalise:
    if (out->port <= 0) {
        out->port = default_port(out->scheme);
    }
    if (out->path[0] != '/') {
        char tmp[sizeof(out->path)];
        snprintf(tmp, sizeof(tmp), "/%s", out->path);
        memcpy(out->path, tmp, sizeof(out->path));
    }

    /* A fragment is for the browser, never for the server. */
    char *hash = strchr(out->path, '#');
    if (hash != NULL) *hash = '\0';

    /* Fold "a/./b" and "a/../b" so the request line is what the
     * server expects to see. */
    {
        char folded[sizeof(out->path)];
        const char *p = out->path;
        size_t len = 0;
        folded[len++] = '/';
        while (*p != '\0') {
            if (*p == '/') { p++; continue; }
            const char *seg = p;
            size_t n = strcspn(p, "/");
            p += n;
            if (n == 1 && seg[0] == '.') continue;
            if (n == 2 && seg[0] == '.' && seg[1] == '.') {
                if (len > 1) {
                    len--; /* step over the trailing slash */
                    while (len > 1 && folded[len - 1] != '/') len--;
                }
                continue;
            }
            if (len + n + 1 >= sizeof(folded)) break;
            memcpy(folded + len, seg, n);
            len += n;
            if (*p == '/') folded[len++] = '/';
        }
        folded[len] = '\0';
        memcpy(out->path, folded, sizeof(out->path));
    }

    if (out->host[0] == '\0') {
        fail("no host in the address");
        return -1;
    }
    return 0;
}

void url_format(const struct url *u, char *out, size_t size) {
    if (u->port == default_port(u->scheme)) {
        snprintf(out, size, "%s://%s%s", u->scheme, u->host, u->path);
    } else {
        snprintf(out, size, "%s://%s:%d%s", u->scheme, u->host, u->port,
                 u->path);
    }
}

/* ---- one transport, two kinds of socket ---- */

struct stream {
    int fd;                  /* plain http */
    struct tls_conn *tls;    /* https */
};

static int stream_open(struct stream *s, const struct url *u) {
    memset(s, 0, sizeof(*s));
    s->fd = -1;

    if (strcmp(u->scheme, "https") == 0) {
        s->tls = tls_connect(u->host, u->port, 0);
        if (s->tls == NULL) {
            fail("%s", tls_last_error());
            return -1;
        }
        return 0;
    }
    if (strcmp(u->scheme, "http") != 0) {
        fail("%s:// is not something Clint can fetch", u->scheme);
        return -1;
    }

    uint32_t addr = host_resolve(u->host);
    if (addr == 0) {
        fail("cannot resolve %s", u->host);
        return -1;
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = (uint16_t)((u->port << 8) | (u->port >> 8));
    sa.sin_addr.s_addr = addr;

    s->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s->fd < 0) {
        fail("socket: %s", strerror(errno));
        return -1;
    }
    if (connect(s->fd, (struct sockaddr *)&sa, sizeof(sa)) != 0) {
        fail("cannot connect to %s:%d: %s", u->host, u->port, strerror(errno));
        close(s->fd);
        s->fd = -1;
        return -1;
    }
    return 0;
}

static long stream_read(struct stream *s, void *buf, size_t len) {
    if (s->tls != NULL) return tls_read(s->tls, buf, len);
    for (;;) {
        long n = read(s->fd, buf, len);
        if (n < 0 && errno == EINTR) continue;
        return n;
    }
}

static int stream_write(struct stream *s, const void *buf, size_t len) {
    if (s->tls != NULL) return tls_write(s->tls, buf, len) < 0 ? -1 : 0;

    const char *p = buf;
    while (len > 0) {
        long n = write(s->fd, p, len);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        p += n;
        len -= (size_t)n;
    }
    return 0;
}

static void stream_close(struct stream *s) {
    if (s->tls != NULL) tls_close(s->tls);
    if (s->fd >= 0) close(s->fd);
    s->tls = NULL;
    s->fd = -1;
}

/* ---- a growable byte buffer ---- */

struct buf {
    char *data;
    size_t len, cap;
};

static int buf_put(struct buf *b, const void *data, size_t len) {
    if (b->len + len + 1 > b->cap) {
        size_t want = b->cap ? b->cap : 8192;
        while (want < b->len + len + 1) want *= 2;
        if (want > HTTP_MAX_BODY) {
            fail("the page is larger than Clint will load");
            return -1;
        }
        char *p = realloc(b->data, want);
        if (p == NULL) {
            fail("out of memory");
            return -1;
        }
        b->data = p;
        b->cap = want;
    }
    memcpy(b->data + b->len, data, len);
    b->len += len;
    b->data[b->len] = '\0';
    return 0;
}

/* ---- the response ---- */

/* Case-insensitive prefix test, for header names. */
static int header_is(const char *line, const char *name) {
    size_t n = strlen(name);
    for (size_t i = 0; i < n; i++) {
        char a = line[i], b = name[i];
        if (a >= 'A' && a <= 'Z') a = (char)(a - 'A' + 'a');
        if (b >= 'A' && b <= 'Z') b = (char)(b - 'A' + 'a');
        if (a != b) return 0;
    }
    return line[n] == ':';
}

static const char *header_value(const char *line, const char *name) {
    const char *v = line + strlen(name) + 1;
    while (*v == ' ' || *v == '\t') v++;
    return v;
}

/*
 * Chunked bodies: each chunk is a hex length, CRLF, that many bytes,
 * CRLF, ending with a zero-length chunk. Servers send this whenever
 * they do not know the length in advance, which for anything
 * generated is most of the time.
 */
static int dechunk(const char *in, size_t in_len, struct buf *out) {
    size_t at = 0;
    for (;;) {
        /* the chunk header line */
        size_t start = at;
        while (at < in_len && in[at] != '\n') at++;
        if (at >= in_len) return 0; /* truncated: keep what we have */

        char size_text[32];
        size_t n = at - start;
        if (n >= sizeof(size_text)) return -1;
        memcpy(size_text, in + start, n);
        size_text[n] = '\0';
        at++;

        unsigned long chunk = strtoul(size_text, NULL, 16);
        if (chunk == 0) return 0;
        if (at + chunk > in_len) chunk = in_len - at;

        if (buf_put(out, in + at, chunk) != 0) return -1;
        at += chunk;

        /* the CRLF that follows the data */
        while (at < in_len && in[at] != '\n') at++;
        if (at < in_len) at++;
    }
}

static int fetch_once(const struct url *u, const char *post_body,
                      struct http_response *out, char *location,
                      size_t location_size) {
    struct stream s;
    if (stream_open(&s, u) != 0) return -1;

    out->secure = s.tls != NULL;
    out->tls_warning = s.tls != NULL ? tls_conn_warning(s.tls) : NULL;

    char req[2048];
    int n;
    if (post_body != NULL) {
        n = snprintf(req, sizeof(req),
                     "POST %s HTTP/1.1\r\n"
                     "Host: %s\r\n"
                     "User-Agent: Clint/1.0 (TUS)\r\n"
                     "Accept: text/html,text/plain,*/*\r\n"
                     "Accept-Encoding: gzip, identity\r\n"
                     "Content-Type: application/x-www-form-urlencoded\r\n"
                     "Content-Length: %lu\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     u->path, u->host, (unsigned long)strlen(post_body));
    } else {
        n = snprintf(req, sizeof(req),
                     "GET %s HTTP/1.1\r\n"
                     "Host: %s\r\n"
                     "User-Agent: Clint/1.0 (TUS)\r\n"
                     "Accept: text/html,text/plain,*/*\r\n"
                     "Accept-Encoding: gzip, identity\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     u->path, u->host);
    }
    if (n < 0 || stream_write(&s, req, (size_t)n) != 0 ||
        (post_body != NULL &&
         stream_write(&s, post_body, strlen(post_body)) != 0)) {
        fail("cannot send the request");
        stream_close(&s);
        return -1;
    }

    struct buf raw = { NULL, 0, 0 };
    char chunk[8192];
    for (;;) {
        long got = stream_read(&s, chunk, sizeof(chunk));
        if (got == 0) break;
        if (got < 0) {
            /* A server that hangs up without close_notify is common
             * enough that a body already received still counts. */
            if (raw.len == 0) {
                /* The transport knows why; repeating "it failed" here
                 * would throw that away. */
                fail("no reply from %s: %s", u->host,
                     s.tls != NULL ? tls_last_error() : strerror(errno));
                free(raw.data);
                stream_close(&s);
                return -1;
            }
            break;
        }
        if (buf_put(&raw, chunk, (size_t)got) != 0) {
            free(raw.data);
            stream_close(&s);
            return -1;
        }
        if (raw.len > HTTP_MAX_BODY) {
            fail("the reply is larger than Clint will hold (%u MB)",
                 (unsigned)(HTTP_MAX_BODY / (1024 * 1024)));
            free(raw.data);
            stream_close(&s);
            return -1;
        }
    }
    stream_close(&s);

    if (raw.len == 0) {
        fail("the server sent nothing");
        free(raw.data);
        return -1;
    }

    /* Split at the blank line. */
    char *body = strstr(raw.data, "\r\n\r\n");
    size_t skip = 4;
    if (body == NULL) {
        body = strstr(raw.data, "\n\n");
        skip = 2;
    }
    if (body == NULL) {
        fail("the reply has no headers");
        free(raw.data);
        return -1;
    }
    *body = '\0';
    size_t body_at = (size_t)(body - raw.data) + skip;
    size_t body_len = raw.len - body_at;

    /* the status line */
    int status = 0;
    const char *space = strchr(raw.data, ' ');
    if (space != NULL) status = atoi(space + 1);
    out->status = status;

    int chunked = 0, gzipped = 0;
    location[0] = '\0';
    out->content_type[0] = '\0';

    char *line = raw.data;
    while (line != NULL) {
        char *end = strchr(line, '\n');
        if (end != NULL) *end = '\0';
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\r') line[len - 1] = '\0';

        if (header_is(line, "content-type")) {
            snprintf(out->content_type, sizeof(out->content_type), "%s",
                     header_value(line, "content-type"));
        } else if (header_is(line, "location")) {
            snprintf(location, location_size, "%s",
                     header_value(line, "location"));
        } else if (header_is(line, "transfer-encoding")) {
            chunked = strstr(header_value(line, "transfer-encoding"),
                             "chunked") != NULL;
        } else if (header_is(line, "content-encoding")) {
            gzipped = strstr(header_value(line, "content-encoding"),
                             "gzip") != NULL;
        }
        line = end != NULL ? end + 1 : NULL;
    }

    if (chunked) {
        struct buf plain = { NULL, 0, 0 };
        if (dechunk(raw.data + body_at, body_len, &plain) != 0) {
            free(plain.data);
            free(raw.data);
            return -1;
        }
        out->body = plain.data ? plain.data : strdup("");
        out->body_len = plain.len;
    } else {
        out->body = malloc(body_len + 1);
        if (out->body == NULL) {
            fail("out of memory");
            free(raw.data);
            return -1;
        }
        memcpy(out->body, raw.data + body_at, body_len);
        out->body[body_len] = '\0';
        out->body_len = body_len;
    }

    free(raw.data);

    /* Compression is undone after the chunking, because that is the
     * order it was applied in: the chunks carry the compressed
     * stream, not the other way round. */
    if (gzipped && out->body_len > 0) {
        void *plain = NULL;
        size_t plain_len = 0;
        if (inflate_gzip(out->body, out->body_len, HTTP_MAX_BODY, &plain,
                         &plain_len) != 0) {
            fail("the reply is compressed in a way Clint cannot read: %s",
                 inflate_last_error());
            return -1;
        }
        free(out->body);
        out->body = (char *)plain;
        out->body_len = plain_len;
    }

    return 0;
}

static int http_send(const struct url *u, const char *body,
                     struct http_response *out) {
    struct url at = *u;

    memset(out, 0, sizeof(*out));

    for (int hop = 0; hop <= HTTP_MAX_REDIRECTS; hop++) {
        char location[1024];

        free(out->body);
        out->body = NULL;
        if (fetch_once(&at, body, out, location, sizeof(location)) != 0) {
            return -1;
        }
        out->final_url = at;

        int redirect = out->status == 301 || out->status == 302 ||
                       out->status == 303 || out->status == 307 ||
                       out->status == 308;
        if (!redirect || location[0] == '\0') {
            return 0;
        }

        /* 303 says so outright, and 301/302 after a POST are what
         * every browser turns into a GET; only 307 and 308 promise to
         * repeat the request as it was. */
        if (out->status != 307 && out->status != 308) body = NULL;

        struct url next;
        if (url_parse(&next, location, &at) != 0) {
            return 0; /* a redirect we cannot follow: show what we got */
        }
        at = next;
        free(out->body);
        out->body = NULL;
    }

    fail("too many redirects");
    return -1;
}

int http_get(const struct url *u, struct http_response *out) {
    return http_send(u, NULL, out);
}

int http_post(const struct url *u, const char *body,
              struct http_response *out) {
    return http_send(u, body != NULL ? body : "", out);
}

void http_response_free(struct http_response *r) {
    if (r == NULL) return;
    free(r->body);
    r->body = NULL;
    r->body_len = 0;
}
