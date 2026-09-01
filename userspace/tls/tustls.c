/*
 * tustls.c - see tustls.h
 *
 * Two decisions are worth spelling out.
 *
 * The transport is the caller's. Mbed TLS's own net layer wants
 * select() and getaddrinfo(); TUS resolves names through the kernel
 * and has a plain blocking socket, so the socket is opened here and
 * handed to the library through mbedtls_ssl_set_bio(). That is also
 * what keeps MBEDTLS_NET_C out of the build.
 *
 * The trust store is loaded once and shared. Parsing a few hundred CA
 * certificates costs real time and a few hundred kilobytes; doing it
 * per connection would make every page load pay for it again.
 */

#include "tustls.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <netinet/in.h>

#include <mbedtls/debug.h>
#include <mbedtls/error.h>
#include <mbedtls/ssl.h>
#include <mbedtls/x509_crt.h>
#include <psa/crypto.h>

#include "tusnetutil.h"

struct tls_conn {
    int fd;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    const char *warning;
    char version[32];
    char ciphersuite[64];
};

static char g_error[256] = "no error";
static mbedtls_x509_crt g_ca;
static int g_ca_loaded;   /* 1 loaded, -1 tried and failed */
static int g_psa_ready;

static void fail(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_error, sizeof(g_error), fmt, ap);
    va_end(ap);
}

static void fail_mbed(const char *what, int rc) {
    char buf[128];
    mbedtls_strerror(rc, buf, sizeof(buf));
    snprintf(g_error, sizeof(g_error), "%s: %s (-0x%04x)", what, buf,
             (unsigned)-rc);
}

const char *tls_last_error(void) { return g_error; }

/* ---- the trust store ---- */

/*
 * The bundle is read with plain file calls rather than
 * mbedtls_x509_crt_parse_file(), because MBEDTLS_FS_IO is switched
 * off: the library never touches a filesystem, and everything it
 * parses arrives as bytes.
 */
static int load_ca_bundle(void) {
    if (g_ca_loaded != 0) {
        return g_ca_loaded == 1 ? 0 : -1;
    }
    g_ca_loaded = -1;

    FILE *f = fopen(TLS_CA_BUNDLE, "rb");
    if (f == NULL) {
        fail("no CA bundle at %s", TLS_CA_BUNDLE);
        return -1;
    }

    size_t cap = 262144, len = 0;
    char *pem = malloc(cap);
    if (pem == NULL) {
        fclose(f);
        fail("out of memory reading the CA bundle");
        return -1;
    }
    for (;;) {
        if (len + 4096 + 1 > cap) {
            char *bigger = realloc(pem, cap * 2);
            if (bigger == NULL) {
                free(pem);
                fclose(f);
                fail("out of memory reading the CA bundle");
                return -1;
            }
            pem = bigger;
            cap *= 2;
        }
        size_t n = fread(pem + len, 1, 4096, f);
        if (n == 0) break;
        len += n;
    }
    fclose(f);

    /* mbedtls_x509_crt_parse() wants PEM terminated by a NUL, and
     * counts that NUL in the length it is given. */
    pem[len++] = '\0';

    mbedtls_x509_crt_init(&g_ca);
    int rc = mbedtls_x509_crt_parse(&g_ca, (const unsigned char *)pem, len);
    free(pem);

    /* A positive return means some certificates failed to parse but
     * others are usable, which is normal for a distribution bundle
     * carrying formats this build does not enable. */
    if (rc < 0) {
        char why[128];
        mbedtls_strerror(rc, why, sizeof(why));
        /* The byte count is the thing worth knowing: a bundle that
         * arrives short reads as a malformed one. */
        fail("parsing %s (%lu bytes read): %s", TLS_CA_BUNDLE,
             (unsigned long)len, why);
        mbedtls_x509_crt_free(&g_ca);
        return -1;
    }
    g_ca_loaded = 1;
    return 0;
}

/* ---- the socket underneath ---- */

static int tcp_connect(const char *host, int port) {
    uint32_t addr = host_resolve(host);
    if (addr == 0) {
        fail("cannot resolve %s", host);
        return -1;
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = (uint16_t)((port << 8) | (port >> 8));
    sa.sin_addr.s_addr = addr;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fail("socket: %s", strerror(errno));
        return -1;
    }
    if (connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != 0) {
        fail("connect to %s:%d: %s", host, port, strerror(errno));
        close(fd);
        return -1;
    }
    return fd;
}

/*
 * The two callbacks Mbed TLS drives the socket through.
 *
 * A callback has to answer with a negative error code, and the usual
 * ones (MBEDTLS_ERR_NET_*) live in the net layer this build leaves
 * out. Their values belong to the library's error space either way,
 * so reusing them keeps mbedtls_strerror() able to name them.
 *
 * Returning zero from the read callback is not an error: the library
 * reads that as the peer having closed, which is what it means.
 */
#define TLS_ERR_SEND (-0x004E)
#define TLS_ERR_RECV (-0x004C)

static int bio_send(void *ctx, const unsigned char *buf, size_t len) {
    int fd = *(int *)ctx;
    for (;;) {
        long n = write(fd, buf, len);
        if (n >= 0) return (int)n;
        if (errno == EINTR) continue;
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return MBEDTLS_ERR_SSL_WANT_WRITE;
        }
        return TLS_ERR_SEND;
    }
}

static int bio_recv(void *ctx, unsigned char *buf, size_t len) {
    int fd = *(int *)ctx;
    for (;;) {
        long n = read(fd, buf, len);
        if (n >= 0) return (int)n;
        if (errno == EINTR) continue;
        /* Nothing ready yet is not a failure: say so in the library's
         * own words and it will come back. */
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return MBEDTLS_ERR_SSL_WANT_READ;
        }
        return TLS_ERR_RECV;
    }
}

/* ---- connecting ---- */

struct tls_conn *tls_connect(const char *host, int port, unsigned flags) {
    if (!g_psa_ready) {
        if (psa_crypto_init() != PSA_SUCCESS) {
            fail("the crypto library will not start");
            return NULL;
        }
        g_psa_ready = 1;
    }

    int verify = (flags & TLS_INSECURE) == 0;
    if (verify && load_ca_bundle() != 0) {
        /* g_error already says why. Refusing here is deliberate: a
         * connection that cannot be verified should not quietly look
         * like one that was. */
        return NULL;
    }

    struct tls_conn *c = calloc(1, sizeof(*c));
    if (c == NULL) {
        fail("out of memory");
        return NULL;
    }
    c->fd = -1;
    mbedtls_ssl_init(&c->ssl);
    mbedtls_ssl_config_init(&c->conf);

    int rc = mbedtls_ssl_config_defaults(&c->conf, MBEDTLS_SSL_IS_CLIENT,
                                         MBEDTLS_SSL_TRANSPORT_STREAM,
                                         MBEDTLS_SSL_PRESET_DEFAULT);
    if (rc != 0) {
        fail_mbed("ssl_config_defaults", rc);
        goto fail;
    }

    if (verify) {
        mbedtls_ssl_conf_authmode(&c->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
        mbedtls_ssl_conf_ca_chain(&c->conf, &g_ca, NULL);
    } else {
        mbedtls_ssl_conf_authmode(&c->conf, MBEDTLS_SSL_VERIFY_NONE);
        c->warning = "the certificate was not checked at all";
    }

    rc = mbedtls_ssl_setup(&c->ssl, &c->conf);
    if (rc != 0) {
        fail_mbed("ssl_setup", rc);
        goto fail;
    }

    /* The name goes into SNI and into the certificate check; a server
     * on a shared address answers with the wrong certificate without
     * it. */
    rc = mbedtls_ssl_set_hostname(&c->ssl, host);
    if (rc != 0) {
        fail_mbed("set_hostname", rc);
        goto fail;
    }

    c->fd = tcp_connect(host, port);
    if (c->fd < 0) {
        goto fail;
    }
    mbedtls_ssl_set_bio(&c->ssl, &c->fd, bio_send, bio_recv, NULL);

    for (;;) {
        rc = mbedtls_ssl_handshake(&c->ssl);
        if (rc == 0) break;
        if (rc == MBEDTLS_ERR_SSL_WANT_READ || rc == MBEDTLS_ERR_SSL_WANT_WRITE) {
            continue; /* a blocking socket: just go round again */
        }
        if (rc == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED) {
            char why[256];
            uint32_t bad = mbedtls_ssl_get_verify_result(&c->ssl);
            mbedtls_x509_crt_verify_info(why, sizeof(why), "", bad);
            why[strcspn(why, "\n")] = '\0';
            fail("the server's certificate was rejected: %s", why);
        } else {
            fail_mbed("handshake", rc);
        }
        goto fail;
    }

    snprintf(c->version, sizeof(c->version), "%s",
             mbedtls_ssl_get_version(&c->ssl));
    snprintf(c->ciphersuite, sizeof(c->ciphersuite), "%s",
             mbedtls_ssl_get_ciphersuite(&c->ssl));

    /* Whatever was verified, the dates were not: see tustls.h. */
    if (c->warning == NULL) {
        c->warning = "the certificate's dates were not checked "
                     "(TUS has no real-time clock)";
    }
    return c;

fail:
    tls_close(c);
    return NULL;
}

long tls_read(struct tls_conn *c, void *buf, size_t len) {
    for (;;) {
        int rc = mbedtls_ssl_read(&c->ssl, buf, len);
        if (rc >= 0) return rc;
        if (rc == MBEDTLS_ERR_SSL_WANT_READ || rc == MBEDTLS_ERR_SSL_WANT_WRITE) {
            continue;
        }
        /*
         * TLS 1.3 servers send a session ticket after the handshake,
         * and Mbed TLS reports it to the application rather than
         * swallowing it. Nothing here resumes sessions, so it is a
         * notification to read past, not an error - and treating it
         * as one means every TLS 1.3 page fails with no reply.
         */
        if (rc == MBEDTLS_ERR_SSL_RECEIVED_NEW_SESSION_TICKET) {
            continue;
        }
        /* Both of these are the end of the stream, not a failure:
         * one is a polite close, the other a socket that simply
         * stopped. An HTTP/1.0 response can end either way. */
        if (rc == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY ||
            rc == MBEDTLS_ERR_SSL_CONN_EOF) {
            return 0;
        }
        fail_mbed("read", rc);
        return -1;
    }
}

long tls_write(struct tls_conn *c, const void *buf, size_t len) {
    size_t done = 0;
    while (done < len) {
        int rc = mbedtls_ssl_write(&c->ssl, (const unsigned char *)buf + done,
                                   len - done);
        if (rc > 0) {
            done += (size_t)rc;
            continue;
        }
        if (rc == MBEDTLS_ERR_SSL_WANT_READ || rc == MBEDTLS_ERR_SSL_WANT_WRITE) {
            continue;
        }
        fail_mbed("write", rc);
        return -1;
    }
    return (long)done;
}

void tls_close(struct tls_conn *c) {
    if (c == NULL) return;
    if (c->fd >= 0) {
        mbedtls_ssl_close_notify(&c->ssl);
        close(c->fd);
    }
    mbedtls_ssl_free(&c->ssl);
    mbedtls_ssl_config_free(&c->conf);
    free(c);
}

const char *tls_conn_warning(const struct tls_conn *c) {
    return c != NULL ? c->warning : NULL;
}

const char *tls_conn_version(const struct tls_conn *c) {
    return c != NULL ? c->version : "";
}

const char *tls_conn_ciphersuite(const struct tls_conn *c) {
    return c != NULL ? c->ciphersuite : "";
}
