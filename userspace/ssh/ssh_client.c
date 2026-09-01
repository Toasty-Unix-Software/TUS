/*
 * ssh - the TUS SSH client
 *
 *   ssh [-p port] [-i identity] [-t] [-T] [user@]host [command...]
 *
 * Host keys are checked against ~/.ssh/known_hosts. An unknown host
 * is offered to the user when there is a terminal to ask on, and
 * accepted with a warning when there is not - but a host key that
 * has *changed* is always fatal, because that is the one case where
 * continuing could hand a password to the wrong machine.
 *
 * Authentication tries public key first (~/.ssh/id_ed25519, or the
 * file named by -i) and falls back to a password prompt, which is the
 * order that avoids typing a password the server was never going to
 * need.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include <netinet/in.h>

#ifdef TUS_HOST_BUILD
#include <arpa/inet.h>
#include <netdb.h>
#else
#include "tusnetutil.h"
#endif

#include "ssh.h"
#include "sshchan.h"
#include "tuscrypt.h"

#define SSH_DEFAULT_PORT 22

static int verbose;

static void vsay(const char *fmt, ...) {
    if (!verbose) return;
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "ssh: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

/* ---- connecting ---- */

static int tcp_connect(const char *host, int port) {
#ifdef TUS_HOST_BUILD
    char portstr[16];
    struct addrinfo hints, *res, *ai;

    snprintf(portstr, sizeof(portstr), "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, portstr, &hints, &res) != 0) {
        fprintf(stderr, "ssh: cannot resolve %s\n", host);
        return -1;
    }
    int fd = -1;
    for (ai = res; ai; ai = ai->ai_next) {
        fd = socket(ai->ai_family, ai->ai_socktype, 0);
        if (fd < 0) continue;
        if (connect(fd, ai->ai_addr, ai->ai_addrlen) == 0) break;
        close(fd);
        fd = -1;
    }
    freeaddrinfo(res);
    if (fd < 0) fprintf(stderr, "ssh: cannot connect to %s: %s\n", host,
                        strerror(errno));
    return fd;
#else
    uint32_t addr = host_resolve(host);
    if (!addr) {
        fprintf(stderr, "ssh: cannot resolve %s\n", host);
        return -1;
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = (uint16_t)((port << 8) | (port >> 8));
    sa.sin_addr.s_addr = addr;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stderr, "ssh: socket: %s\n", strerror(errno));
        return -1;
    }
    if (connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != 0) {
        fprintf(stderr, "ssh: cannot connect to %s: %s\n", host,
                strerror(errno));
        close(fd);
        return -1;
    }
    return fd;
#endif
}

/* ---- known_hosts ---- */

/* "SHA256:" plus the unpadded base64 of the key blob's hash, the same
 * fingerprint OpenSSH prints. */
static void key_fingerprint(const struct ssh_key *k, char *out, size_t size) {
    struct sshbuf blob;
    uint8_t digest[SHA256_DIGEST_SIZE];
    char b64[64];

    sshbuf_init(&blob);
    ssh_key_blob(k, &blob);
    sha256(blob.data, blob.len, digest);
    sshbuf_free(&blob);

    sshbuf_b64_encode(digest, sizeof(digest), b64, sizeof(b64));
    char *pad = strchr(b64, '=');
    if (pad) *pad = '\0';
    snprintf(out, size, "SHA256:%s", b64);
}

/* The name a host is filed under: "host" on port 22, "[host]:port"
 * anywhere else. */
static void host_spec(char *out, size_t size, const char *host, int port) {
    if (port == SSH_DEFAULT_PORT) {
        snprintf(out, size, "%s", host);
    } else {
        snprintf(out, size, "[%s]:%d", host, port);
    }
}

/* 0 known and matching, 1 not present, -1 present with a different key. */
static int known_hosts_check(const char *path, const char *spec,
                             const struct ssh_key *k) {
    FILE *f = fopen(path, "r");
    if (!f) return 1;

    char line[1024];
    int result = 1;
    size_t spec_len = strlen(spec);

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        /* The first field is a comma-separated list of host patterns.
         * Only literal names are matched: wildcards and the hashed
         * form are things this client does not write. */
        size_t field = strcspn(line, " \t");
        int matched = 0;
        for (size_t i = 0; i < field;) {
            size_t j = i;
            while (j < field && line[j] != ',') j++;
            if (j - i == spec_len && memcmp(line + i, spec, spec_len) == 0) {
                matched = 1;
                break;
            }
            i = j + 1;
        }
        if (!matched) continue;

        struct ssh_key found;
        if (ssh_key_parse_pub_line(&found, line + field) != 0) continue;

        if (memcmp(found.pub, k->pub, 32) == 0) {
            result = 0;
            break;
        }
        result = -1;
        break;
    }
    fclose(f);
    return result;
}

static int known_hosts_add(const char *path, const char *spec,
                           const struct ssh_key *k) {
    char keyline[512];
    if (ssh_key_write_pub_line(k, NULL, keyline, sizeof(keyline)) != 0) {
        return -1;
    }

    /* Make sure ~/.ssh exists, with the permissions ssh expects. */
    char dir[512];
    snprintf(dir, sizeof(dir), "%s", path);
    char *slash = strrchr(dir, '/');
    if (slash) {
        *slash = '\0';
        mkdir(dir, 0700);
    }

    FILE *f = fopen(path, "a");
    if (!f) return -1;
    fprintf(f, "%s %s", spec, keyline);
    fclose(f);
    return 0;
}

struct host_check {
    const char *spec;
    const char *path;
    int accepted_new;
};

static int verify_host_key(void *ctx, const struct ssh_key *k) {
    struct host_check *hc = ctx;
    char fp[128];

    key_fingerprint(k, fp, sizeof(fp));

    switch (known_hosts_check(hc->path, hc->spec, k)) {
    case 0:
        vsay("host key for %s matches known_hosts", hc->spec);
        return 0;

    case -1:
        fprintf(stderr,
                "\n@@@ WARNING: THE HOST KEY FOR %s HAS CHANGED @@@\n"
                "The key offered is %s.\n"
                "Someone could be listening on this connection, or the host\n"
                "was reinstalled. Remove the old line from %s if you are\n"
                "certain it is the latter.\n\n",
                hc->spec, fp, hc->path);
        return -1;

    default:
        break;
    }

    fprintf(stderr, "The authenticity of host %s cannot be established.\n"
                    "ED25519 key fingerprint is %s.\n",
            hc->spec, fp);

    if (isatty(STDIN_FILENO)) {
        fprintf(stderr, "Continue connecting (yes/no)? ");
        fflush(stderr);

        char answer[16];
        if (!fgets(answer, sizeof(answer), stdin) ||
            (strncmp(answer, "yes", 3) != 0 && strncmp(answer, "y\n", 2) != 0)) {
            fprintf(stderr, "Host key rejected.\n");
            return -1;
        }
    } else {
        fprintf(stderr, "No terminal to ask on: accepting the key this "
                        "once.\n");
    }

    if (known_hosts_add(hc->path, hc->spec, k) == 0) {
        fprintf(stderr, "Added %s to %s.\n", hc->spec, hc->path);
    }
    hc->accepted_new = 1;
    return 0;
}

/* ---- authentication ---- */

static int expect_service_accept(struct ssh *s) {
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_SERVICE_REQUEST);
    sshbuf_put_cstring(&p, "ssh-userauth");
    int rc = ssh_packet_send(s, &p);
    sshbuf_free(&p);
    if (rc != 0) return -1;

    uint8_t type;
    if (ssh_packet_recv(s, &type) != 0) return -1;
    if (type != SSH_MSG_SERVICE_ACCEPT) {
        return ssh_fail(s, "server refused the userauth service");
    }
    return 0;
}

/*
 * Read one authentication reply. Returns 1 on success, 0 on failure
 * (with the methods that may still be tried copied into `methods`),
 * 2 when the server sent PK_OK, -1 on error.
 */
static int auth_reply(struct ssh *s, char *methods, size_t methods_size) {
    for (;;) {
        uint8_t type;
        if (ssh_packet_recv(s, &type) != 0) return -1;

        switch (type) {
        case SSH_MSG_USERAUTH_SUCCESS:
            return 1;

        case SSH_MSG_USERAUTH_FAILURE: {
            const char *list;
            size_t list_len;
            int partial = 0;
            if (sshbuf_get_cstring(&s->pkt, &list, &list_len) != 0) {
                return ssh_fail(s, "malformed USERAUTH_FAILURE");
            }
            sshbuf_get_bool(&s->pkt, &partial);
            if (methods) {
                size_t n = list_len < methods_size - 1 ? list_len
                                                       : methods_size - 1;
                memcpy(methods, list, n);
                methods[n] = '\0';
            }
            return 0;
        }

        case SSH_MSG_USERAUTH_BANNER: {
            const char *text;
            size_t text_len;
            if (sshbuf_get_cstring(&s->pkt, &text, &text_len) == 0) {
                fwrite(text, 1, text_len, stderr);
            }
            continue; /* a banner is not a reply */
        }

        case SSH_MSG_USERAUTH_PK_OK:
            return 2;

        default:
            return ssh_fail(s, "unexpected message %u during authentication",
                            type);
        }
    }
}

/* The "none" method: nobody expects it to succeed, but the failure
 * reply is what names the methods the server will actually accept. */
static int auth_none(struct ssh *s, const char *user, char *methods,
                     size_t methods_size) {
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_USERAUTH_REQUEST);
    sshbuf_put_cstring(&p, user);
    sshbuf_put_cstring(&p, "ssh-connection");
    sshbuf_put_cstring(&p, "none");
    int rc = ssh_packet_send(s, &p);
    sshbuf_free(&p);
    if (rc != 0) return -1;

    return auth_reply(s, methods, methods_size);
}

/*
 * The signed blob is the session id followed by the request itself,
 * which is what binds a signature to this connection: replaying it
 * anywhere else fails, because no other connection has this session
 * id.
 */
static void publickey_request(struct sshbuf *p, const char *user,
                              const struct sshbuf *blob, int with_signature) {
    sshbuf_put_u8(p, SSH_MSG_USERAUTH_REQUEST);
    sshbuf_put_cstring(p, user);
    sshbuf_put_cstring(p, "ssh-connection");
    sshbuf_put_cstring(p, "publickey");
    sshbuf_put_bool(p, with_signature);
    sshbuf_put_cstring(p, SSH_ED25519_NAME);
    sshbuf_put_stringb(p, blob);
}

static int auth_publickey(struct ssh *s, const char *user,
                          const struct ssh_key *k) {
    struct sshbuf blob, p, signed_data, sig;
    int rc;

    sshbuf_init(&blob);
    sshbuf_init(&p);
    sshbuf_init(&signed_data);
    sshbuf_init(&sig);

    ssh_key_blob(k, &blob);

    /* Ask first whether the key is acceptable, so that an unusable
     * key costs a round trip rather than a signature. */
    publickey_request(&p, user, &blob, 0);
    rc = ssh_packet_send(s, &p);
    if (rc != 0) goto out;

    rc = auth_reply(s, NULL, 0);
    if (rc <= 0) goto out; /* 0 means the server will not take this key */

    sshbuf_put_string(&signed_data, s->session_id, sizeof(s->session_id));
    publickey_request(&signed_data, user, &blob, 1);
    ssh_key_sign(k, signed_data.data, signed_data.len, &sig);

    sshbuf_reset(&p);
    publickey_request(&p, user, &blob, 1);
    sshbuf_put_stringb(&p, &sig);
    rc = ssh_packet_send(s, &p);
    if (rc != 0) goto out;

    rc = auth_reply(s, NULL, 0);

out:
    sshbuf_free(&blob);
    sshbuf_free(&p);
    sshbuf_free(&signed_data);
    sshbuf_free(&sig);
    return rc;
}

/* Read a line from the terminal with the echo turned off. */
static int read_password(const char *prompt, char *out, size_t size) {
    int fd = open("/dev/tty", O_RDWR);
    FILE *in = fd >= 0 ? fdopen(fd, "r+") : stdin;
    struct termios old, raw;
    int restore = 0;

    fprintf(in == stdin ? stderr : in, "%s", prompt);
    fflush(in == stdin ? stderr : in);

    if (tcgetattr(fileno(in), &old) == 0) {
        raw = old;
        raw.c_lflag &= (tcflag_t)~ECHO;
        /* TCSAFLUSH here is on purpose: anything typed before the
         * prompt appeared was not typed as a password, and echoing it
         * back later would put it on the screen. */
        if (tcsetattr(fileno(in), TCSAFLUSH, &raw) == 0) restore = 1;
    }

    int ok = fgets(out, (int)size, in) != NULL;

    if (restore) tcsetattr(fileno(in), TCSADRAIN, &old);
    fprintf(in == stdin ? stderr : in, "\n");
    if (in != stdin) fclose(in);

    if (!ok) return -1;
    out[strcspn(out, "\r\n")] = '\0';
    return 0;
}

static int auth_password(struct ssh *s, const char *user, const char *host) {
    char prompt[600], password[256];

    snprintf(prompt, sizeof(prompt), "%s@%s's password: ", user, host);
    if (read_password(prompt, password, sizeof(password)) != 0) {
        /* No terminal to ask on, or the input ended. Saying so beats
         * failing with an empty message, which is what a bare -1
         * here used to produce. */
        return ssh_fail(s, "cannot read a password (no terminal?)");
    }

    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_USERAUTH_REQUEST);
    sshbuf_put_cstring(&p, user);
    sshbuf_put_cstring(&p, "ssh-connection");
    sshbuf_put_cstring(&p, "password");
    sshbuf_put_bool(&p, 0);
    sshbuf_put_cstring(&p, password);
    int rc = ssh_packet_send(s, &p);

    crypto_wipe(password, sizeof(password));
    crypto_wipe(p.data, p.cap);
    sshbuf_free(&p);
    if (rc != 0) return -1;

    return auth_reply(s, NULL, 0);
}

/* ---- the session ---- */

static struct termios saved_tty;
static int tty_raw_active;

static void tty_restore(void) {
    if (tty_raw_active) {
        tcsetattr(STDIN_FILENO, TCSADRAIN, &saved_tty);
        tty_raw_active = 0;
    }
}

/*
 * With a remote pty the local terminal must stop interpreting
 * anything: control characters, echo and line editing all belong to
 * the far end now.
 */
static int tty_raw(void) {
    struct termios raw;

    if (tcgetattr(STDIN_FILENO, &saved_tty) != 0) return -1;
    raw = saved_tty;
    raw.c_iflag &= (tcflag_t) ~(IXON | ICRNL | INPCK | ISTRIP | BRKINT);
    raw.c_oflag &= (tcflag_t) ~OPOST;
    raw.c_lflag &= (tcflag_t) ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    /* TCSADRAIN, not TCSAFLUSH: anything already typed is the user's
     * input for the remote shell, and flushing it here throws away
     * every keystroke that arrived while the session was starting. */
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &raw) != 0) return -1;
    tty_raw_active = 1;
    return 0;
}

static int open_session(struct ssh *s, struct ssh_channel *c) {
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_OPEN);
    sshbuf_put_cstring(&p, "session");
    sshbuf_put_u32(&p, c->local_id);
    sshbuf_put_u32(&p, c->local_window);
    sshbuf_put_u32(&p, SSH_CHAN_MAX_PACKET);
    int rc = ssh_packet_send(s, &p);
    sshbuf_free(&p);
    if (rc != 0) return -1;

    uint8_t type;
    if (ssh_packet_recv(s, &type) != 0) return -1;

    if (type == SSH_MSG_CHANNEL_OPEN_FAILURE) {
        uint32_t id, reason = 0;
        const char *text = NULL;
        size_t text_len = 0;
        sshbuf_get_u32(&s->pkt, &id);
        sshbuf_get_u32(&s->pkt, &reason);
        sshbuf_get_cstring(&s->pkt, &text, &text_len);
        return ssh_fail(s, "session refused (%u): %.*s", reason, (int)text_len,
                        text ? text : "");
    }
    if (type != SSH_MSG_CHANNEL_OPEN_CONFIRMATION) {
        return ssh_fail(s, "expected a channel confirmation, got message %u",
                        type);
    }

    uint32_t local_id;
    if (sshbuf_get_u32(&s->pkt, &local_id) != 0 ||
        sshbuf_get_u32(&s->pkt, &c->remote_id) != 0 ||
        sshbuf_get_u32(&s->pkt, &c->remote_window) != 0 ||
        sshbuf_get_u32(&s->pkt, &c->remote_max_packet) != 0) {
        return ssh_fail(s, "malformed channel confirmation");
    }
    if (c->remote_max_packet == 0 ||
        c->remote_max_packet > SSH_CHAN_MAX_PACKET) {
        c->remote_max_packet = SSH_CHAN_MAX_PACKET;
    }
    c->open = 1;
    return 0;
}

/* Send a channel request and wait for its success or failure. */
static int channel_request(struct ssh *s, struct ssh_channel *c,
                           struct sshbuf *p) {
    if (ssh_packet_send(s, p) != 0) return -1;

    for (;;) {
        uint8_t type;
        if (ssh_packet_recv(s, &type) != 0) return -1;
        if (type == SSH_MSG_CHANNEL_SUCCESS) return 0;
        if (type == SSH_MSG_CHANNEL_FAILURE) return 1;

        /* The server may interleave window adjustments and even
         * output before it answers. */
        if (ssh_chan_dispatch(s, c, type, STDOUT_FILENO, STDERR_FILENO) != 0) {
            return -1;
        }
    }
}

static int request_pty(struct ssh *s, struct ssh_channel *c) {
    struct winsize ws;
    unsigned cols = 80, rows = 24;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col && ws.ws_row) {
        cols = ws.ws_col;
        rows = ws.ws_row;
    }

    const char *term = getenv("TERM");
    if (!term || !*term) term = "vt100";

    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_REQUEST);
    sshbuf_put_u32(&p, c->remote_id);
    sshbuf_put_cstring(&p, "pty-req");
    sshbuf_put_bool(&p, 1);
    sshbuf_put_cstring(&p, term);
    sshbuf_put_u32(&p, cols);
    sshbuf_put_u32(&p, rows);
    sshbuf_put_u32(&p, 0); /* pixel width and height: not known */
    sshbuf_put_u32(&p, 0);
    /* An empty mode list means "use your defaults", which is right
     * here: the local terminal is in raw mode, so the remote pty is
     * the only one doing any processing. */
    sshbuf_put_string(&p, "\0", 1);

    int rc = channel_request(s, c, &p);
    sshbuf_free(&p);
    return rc;
}

static int start_command(struct ssh *s, struct ssh_channel *c,
                         const char *command) {
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_REQUEST);
    sshbuf_put_u32(&p, c->remote_id);
    if (command) {
        sshbuf_put_cstring(&p, "exec");
        sshbuf_put_bool(&p, 1);
        sshbuf_put_cstring(&p, command);
    } else {
        sshbuf_put_cstring(&p, "shell");
        sshbuf_put_bool(&p, 1);
    }
    int rc = channel_request(s, c, &p);
    sshbuf_free(&p);
    return rc;
}

/* ---- main ---- */

static void usage(void) {
    fprintf(stderr,
            "usage: ssh [-p port] [-i identity] [-t] [-T] [-v] "
            "[user@]host [command ...]\n");
    exit(2);
}

int main(int argc, char **argv) {
    const char *identity = NULL;
    int port = SSH_DEFAULT_PORT;
    int want_pty = -1; /* -1 decides from whether stdin is a terminal */
    int i;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-' || argv[i][1] == '\0') break;
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            identity = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0) {
            want_pty = 1;
        } else if (strcmp(argv[i], "-T") == 0) {
            want_pty = 0;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else {
            usage();
        }
    }
    if (i >= argc || port <= 0 || port > 65535) usage();

    /* [user@]host */
    char target[256];
    snprintf(target, sizeof(target), "%s", argv[i++]);
    const char *user;
    char *at = strchr(target, '@');
    const char *host;
    if (at) {
        *at = '\0';
        user = target;
        host = at + 1;
    } else {
        user = getenv("USER");
        if (!user || !*user) user = "root";
        host = target;
    }

    /* Everything left over is one remote command, joined with spaces
     * exactly as ssh does - the remote shell re-splits it. */
    char *command = NULL;
    if (i < argc) {
        size_t len = 0;
        for (int j = i; j < argc; j++) len += strlen(argv[j]) + 1;
        command = malloc(len + 1);
        if (!command) return 1;
        command[0] = '\0';
        for (int j = i; j < argc; j++) {
            if (j > i) strcat(command, " ");
            strcat(command, argv[j]);
        }
    }

    if (want_pty < 0) want_pty = command == NULL && isatty(STDIN_FILENO);

    const char *home = getenv("HOME");
    if (!home || !*home) home = "/root";

    char known_hosts[512], default_key[512];
    snprintf(known_hosts, sizeof(known_hosts), "%s/.ssh/known_hosts", home);
    snprintf(default_key, sizeof(default_key), "%s/.ssh/id_ed25519", home);

    int fd = tcp_connect(host, port);
    if (fd < 0) return 255;

    struct ssh s;
    ssh_init(&s, fd, 0);

    if (ssh_exchange_versions(&s, "TUS_1.0") != 0) {
        fprintf(stderr, "ssh: %s\n", s.err);
        ssh_close(&s);
        return 255;
    }
    vsay("remote software is %s", s.v_peer);

    char spec[300];
    host_spec(spec, sizeof(spec), host, port);
    struct host_check hc = { spec, known_hosts, 0 };

    if (ssh_kex(&s, NULL, verify_host_key, &hc) != 0) {
        fprintf(stderr, "ssh: %s\n", s.err);
        ssh_close(&s);
        return 255;
    }
    vsay("key exchange complete");

    if (expect_service_accept(&s) != 0) {
        fprintf(stderr, "ssh: %s\n", s.err);
        ssh_close(&s);
        return 255;
    }

    char methods[512] = "";
    int authed = auth_none(&s, user, methods, sizeof(methods));
    if (authed < 0) {
        fprintf(stderr, "ssh: %s\n", s.err);
        ssh_close(&s);
        return 255;
    }
    vsay("server accepts: %s", methods);

    if (!authed && sshbuf_namelist_has(methods, strlen(methods), "publickey")) {
        struct ssh_key key;
        const char *path = identity ? identity : default_key;
        int r = ssh_key_read_private_file(&key, path);

        if (r == 0) {
            vsay("offering %s", path);
            authed = auth_publickey(&s, user, &key);
            crypto_wipe(&key, sizeof(key));
            if (authed < 0) {
                fprintf(stderr, "ssh: %s\n", s.err);
                ssh_close(&s);
                return 255;
            }
        } else if (identity) {
            fprintf(stderr, "ssh: cannot use identity %s%s\n", path,
                    r == -2 ? " (permissions are too open)"
                            : r == -3 ? " (it is encrypted)" : "");
        }
    }

    for (int tries = 0; !authed && tries < 3; tries++) {
        if (!sshbuf_namelist_has(methods, strlen(methods), "password")) break;

        authed = auth_password(&s, user, host);
        if (authed < 0) {
            fprintf(stderr, "ssh: %s\n", s.err);
            ssh_close(&s);
            return 255;
        }
        if (!authed) fprintf(stderr, "Permission denied, please try again.\n");
    }

    if (!authed) {
        fprintf(stderr, "ssh: %s@%s: permission denied\n", user, host);
        ssh_send_disconnect(&s, SSH_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE,
                            "no more authentication methods");
        ssh_close(&s);
        return 255;
    }
    vsay("authenticated");

    struct ssh_channel c;
    ssh_chan_init(&c, 0);
    if (open_session(&s, &c) != 0) {
        fprintf(stderr, "ssh: %s\n", s.err);
        ssh_close(&s);
        return 255;
    }

    if (want_pty) {
        if (request_pty(&s, &c) != 0) {
            fprintf(stderr, "ssh: the server refused a pty\n");
            want_pty = 0;
        }
    }
    if (start_command(&s, &c, command) != 0) {
        fprintf(stderr, "ssh: the server refused to start %s\n",
                command ? "the command" : "a shell");
        ssh_close(&s);
        return 255;
    }

    if (want_pty && tty_raw() == 0) atexit(tty_restore);

    int rc = ssh_chan_pump(&s, &c, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
    tty_restore();

    if (rc != 0) {
        fprintf(stderr, "ssh: %s\n", s.err);
        ssh_close(&s);
        return 255;
    }

    int status = c.have_exit_status ? c.exit_status : 0;
    ssh_close(&s);
    free(command);
    return status;
}
