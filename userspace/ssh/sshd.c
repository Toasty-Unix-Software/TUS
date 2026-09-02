/*
 * sshd - the TUS SSH server
 *
 *   sshd [-p port]
 *
 * Reuses the transport, key formats and channel code the client
 * (ssh_client.c) already has - sshtrans.c and sshchan.c are written
 * generically over struct ssh's `server` flag, so ssh_kex() and
 * ssh_chan_pump() work unmodified on this side too. This file is only
 * the parts that genuinely differ for a server: listening/accepting,
 * host key management, authentication, and starting a shell.
 *
 * Concurrency: accept(), fork() (TUS now has a real one - see
 * kernel/sched/sched.c's sched_fork()), handle the connection in the
 * child, loop. The listening parent reaps finished children right
 * after each accept() so they do not sit as zombies in TUS's small
 * (TASK_MAX = 16) task table.
 *
 * Host key: generated once, the first time sshd runs with none on
 * disk, by calling the same ed25519_keypair()/ssh_key_write_*
 * functions ssh-keygen.c's `main()` calls - not by spawning that
 * binary. There is nothing IPC would buy here: it is the same
 * process's own library code either way, and skipping the spawn
 * avoids needing to parse a child's exit status to know whether key
 * generation actually succeeded.
 *
 * Authentication is password-only, checked the same way login.c
 * checks a console login: crypt() against /etc/shadow. Public-key
 * auth is not attempted - a real limitation, not an oversight (the
 * server side of "publickey" needs authorized_keys support this pass
 * does not add).
 *
 * A session gets /bin/ksh with its stdio wired to a pair of pipes -
 * plain fork()+dup2()+execve(), the standard POSIX shape, now that
 * TUS has a real fork() (the dup2-then-SYS_SPAWN-then-restore idiom
 * older TUS code uses, e.g. tsh's exec_pipeline(), predates that and
 * is not needed here). What this does NOT do: real pty semantics
 * (job control, window resize, a controlling terminal) - TUS has no
 * pty driver, and building one from scratch just for this is out of
 * scope for a first working version (same spirit as Clint's own
 * documented "what it doesn't do" list). A pty-req is acknowledged
 * (real clients send one whether or not the session needs it) but
 * otherwise ignored; the shell simply reads/writes its pipes, which
 * is what makes both `ssh host command` and a basic interactive
 * session work without a pty. exit-status is not forwarded to the
 * client either, for the same "not chasing this the first time
 * around" reason - ssh_chan_pump()'s own EOF/CLOSE handling ends the
 * session correctly either way, just without a meaningful exit code
 * on the client side.
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <netinet/in.h>

#include "ssh.h"
#include "sshchan.h"
#include "tuscrypt.h"

#define SSHD_DEFAULT_PORT 22
#define HOST_KEY_PATH "/etc/ssh/ssh_host_ed25519_key"
#define AUTH_RETRIES 6

static int verbose;

static void vsay(const char *fmt, ...) {
    if (!verbose) return;
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "sshd: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

/* ---- host key ---- */

static int ensure_host_key(struct ssh_key *k) {
    int r = ssh_key_read_private_file(k, HOST_KEY_PATH);
    if (r == 0) {
        vsay("loaded host key %s", HOST_KEY_PATH);
        return 0;
    }
    if (r != -1) {
        /* -2 permissions too open, -3 encrypted: a key IS there, just
         * unusable. Generating a new one over it would be surprising
         * (and, for -2, wrong: fixing the mode is the right answer,
         * not replacing the key). */
        fprintf(stderr, "sshd: %s exists but cannot be used (%s)\n",
                HOST_KEY_PATH, r == -2 ? "permissions are too open"
                                       : "it is encrypted");
        return -1;
    }

    /* This is the fallback path, not the primary one: tusinstall
     * (userspace/tusinstall.c) bakes a real host key into an
     * installed image's own rootfs.img at install time, the same way
     * it bakes in the root password and any user account, because
     * that is the only place anything survives a reboot on TUS -
     * vfs_create_file() (kernel/vfs/vfs.c) backs every new file with
     * an in-memory buffer, and rootfs.c reparses the same static
     * rootfs.img into fresh RAM on every single boot. A key written
     * here is real for this boot and gone the instant the machine
     * restarts (a live CD boot, "disk : none" in the banner, has no
     * persistent storage to put one on at all) - say so instead of
     * quietly generating a new identity every reboot. */
    fprintf(stderr, "sshd: no host key on this filesystem; generating one "
                    "for THIS BOOT ONLY at %s (it will not survive a "
                    "reboot - see tusinstall for the one that does)\n",
            HOST_KEY_PATH);

    char dir[512];
    snprintf(dir, sizeof(dir), "%s", HOST_KEY_PATH);
    char *slash = strrchr(dir, '/');
    if (slash) {
        *slash = '\0';
        mkdir(dir, 0755);
    }

    uint8_t seed[ED25519_SEED_SIZE];
    crypto_random(seed, sizeof(seed));
    memset(k, 0, sizeof(*k));
    ed25519_keypair(k->pub, k->priv, seed);
    k->have_priv = 1;
    crypto_wipe(seed, sizeof(seed));

    char hostname[64] = "tus";
    gethostname(hostname, sizeof(hostname) - 1);
    char comment[96];
    snprintf(comment, sizeof(comment), "root@%s", hostname);

    if (ssh_key_write_private_file(k, comment, HOST_KEY_PATH) != 0) {
        fprintf(stderr, "sshd: cannot write %s\n", HOST_KEY_PATH);
        return -1;
    }
    char pub_path[600], line[512];
    snprintf(pub_path, sizeof(pub_path), "%s.pub", HOST_KEY_PATH);
    if (ssh_key_write_pub_line(k, comment, line, sizeof(line)) == 0) {
        FILE *f = fopen(pub_path, "w");
        if (f) {
            fputs(line, f);
            fclose(f);
        }
    }
    return 0;
}

/* ---- authentication (same check as login.c's authenticate()) ---- */

static const char *shadow_hash(const char *user) {
    FILE *f = fopen("/etc/passwd", "r");
    if (f == NULL) {
        return NULL;
    }
    /* Confirm the account exists before trusting anything in shadow -
     * mirrors login.c's own two-file lookup shape closely enough that
     * an unknown user and a passwordless one fail the same way. */
    int found = 0;
    char line[512];
    while (fgets(line, sizeof(line), f) != NULL) {
        char *c = strchr(line, ':');
        if (c != NULL) {
            *c = '\0';
            if (strcmp(line, user) == 0) {
                found = 1;
                break;
            }
        }
    }
    fclose(f);
    if (!found) {
        return NULL;
    }

    f = fopen("/etc/shadow", "r");
    if (f == NULL) {
        return NULL;
    }
    static char buf[512];
    const char *hit = NULL;
    while (fgets(buf, sizeof(buf), f) != NULL) {
        char *c1 = strchr(buf, ':');
        if (c1 == NULL) continue;
        *c1 = '\0';
        if (strcmp(buf, user) == 0) {
            char *h = c1 + 1;
            char *c2 = strchr(h, ':');
            if (c2 != NULL) *c2 = '\0';
            hit = h;
            break;
        }
    }
    fclose(f);
    return hit;
}

static int check_password(const char *user, const char *pass) {
    const char *hash = shadow_hash(user);
    if (hash == NULL || hash[0] == '\0' || hash[0] == '!') {
        return 0;
    }
    char *c = crypt(pass, hash);
    return c != NULL && strcmp(c, hash) == 0;
}

/* ---- the userauth service request/accept handshake ---- */

static int accept_service_request(struct ssh *s) {
    uint8_t type;
    if (ssh_packet_recv(s, &type) != 0) return -1;
    if (type != SSH_MSG_SERVICE_REQUEST) {
        return ssh_fail(s, "expected SERVICE_REQUEST, got message %u", type);
    }
    const char *name;
    size_t name_len;
    if (sshbuf_get_cstring(&s->pkt, &name, &name_len) != 0) {
        return ssh_fail(s, "malformed SERVICE_REQUEST");
    }
    if (name_len != 12 || memcmp(name, "ssh-userauth", 12) != 0) {
        return ssh_fail(s, "unexpected service %.*s", (int)name_len, name);
    }

    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_SERVICE_ACCEPT);
    sshbuf_put_cstring(&p, "ssh-userauth");
    int rc = ssh_packet_send(s, &p);
    sshbuf_free(&p);
    return rc;
}

static int send_auth_failure(struct ssh *s) {
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_USERAUTH_FAILURE);
    sshbuf_put_cstring(&p, "password");
    sshbuf_put_bool(&p, 0);
    int rc = ssh_packet_send(s, &p);
    sshbuf_free(&p);
    return rc;
}

/* Returns 1 authenticated (with *user filled in), 0 the peer ran out
 * of attempts or asked for something unsupported, -1 on error. */
static int authenticate(struct ssh *s, char *user, size_t user_size) {
    for (int tries = 0; tries < AUTH_RETRIES; tries++) {
        uint8_t type;
        if (ssh_packet_recv(s, &type) != 0) return -1;
        if (type != SSH_MSG_USERAUTH_REQUEST) {
            return ssh_fail(s, "expected USERAUTH_REQUEST, got message %u",
                            type);
        }

        const char *u, *svc, *method;
        size_t u_len, svc_len, method_len;
        if (sshbuf_get_cstring(&s->pkt, &u, &u_len) != 0 ||
            sshbuf_get_cstring(&s->pkt, &svc, &svc_len) != 0 ||
            sshbuf_get_cstring(&s->pkt, &method, &method_len) != 0) {
            return ssh_fail(s, "malformed USERAUTH_REQUEST");
        }
        if (svc_len != 14 || memcmp(svc, "ssh-connection", 14) != 0) {
            return ssh_fail(s, "unexpected service %.*s", (int)svc_len, svc);
        }

        if (method_len == 8 && memcmp(method, "password", 8) == 0) {
            int change_pw = 0;
            const char *pass;
            size_t pass_len;
            if (sshbuf_get_bool(&s->pkt, &change_pw) != 0 ||
                sshbuf_get_cstring(&s->pkt, &pass, &pass_len) != 0) {
                return ssh_fail(s, "malformed password request");
            }
            /* A password-change request is not something this server
             * implements; treat it the same as a wrong password. */
            char pw[256];
            size_t n = pass_len < sizeof(pw) - 1 ? pass_len : sizeof(pw) - 1;
            memcpy(pw, pass, n);
            pw[n] = '\0';

            size_t un = u_len < user_size - 1 ? u_len : user_size - 1;
            memcpy(user, u, un);
            user[un] = '\0';

            int ok = !change_pw && check_password(user, pw);
            crypto_wipe(pw, sizeof(pw));

            if (ok) {
                struct sshbuf p;
                sshbuf_init(&p);
                sshbuf_put_u8(&p, SSH_MSG_USERAUTH_SUCCESS);
                int rc = ssh_packet_send(s, &p);
                sshbuf_free(&p);
                return rc == 0 ? 1 : -1;
            }
            vsay("password authentication failed for %s", user);
            if (send_auth_failure(s) != 0) return -1;
            continue;
        }

        /* "none" (the client's probe for what is accepted) and
         * anything else this server does not implement (publickey,
         * keyboard-interactive) all get the same answer: not this
         * time, try password. */
        if (send_auth_failure(s) != 0) return -1;
    }
    return 0;
}

/* ---- the session channel ---- */

static int accept_session_channel(struct ssh *s, struct ssh_channel *c) {
    uint8_t type;
    if (ssh_packet_recv(s, &type) != 0) return -1;
    if (type != SSH_MSG_CHANNEL_OPEN) {
        return ssh_fail(s, "expected CHANNEL_OPEN, got message %u", type);
    }
    const char *chan_type;
    size_t chan_type_len;
    uint32_t remote_id, remote_window, remote_max_packet;
    if (sshbuf_get_cstring(&s->pkt, &chan_type, &chan_type_len) != 0 ||
        sshbuf_get_u32(&s->pkt, &remote_id) != 0 ||
        sshbuf_get_u32(&s->pkt, &remote_window) != 0 ||
        sshbuf_get_u32(&s->pkt, &remote_max_packet) != 0) {
        return ssh_fail(s, "malformed CHANNEL_OPEN");
    }

    ssh_chan_init(c, 0);
    c->remote_id = remote_id;
    c->remote_window = remote_window;
    c->remote_max_packet = remote_max_packet > SSH_CHAN_MAX_PACKET
                               ? SSH_CHAN_MAX_PACKET
                               : remote_max_packet;

    struct sshbuf p;
    sshbuf_init(&p);
    if (chan_type_len != 7 || memcmp(chan_type, "session", 7) != 0) {
        sshbuf_put_u8(&p, SSH_MSG_CHANNEL_OPEN_FAILURE);
        sshbuf_put_u32(&p, remote_id);
        sshbuf_put_u32(&p, 3); /* SSH_OPEN_UNKNOWN_CHANNEL_TYPE */
        sshbuf_put_cstring(&p, "only session channels are supported");
        sshbuf_put_cstring(&p, "");
        ssh_packet_send(s, &p);
        sshbuf_free(&p);
        return -1; /* nothing more to do for this connection either way */
    }

    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_OPEN_CONFIRMATION);
    sshbuf_put_u32(&p, remote_id);
    sshbuf_put_u32(&p, c->local_id);
    sshbuf_put_u32(&p, c->local_window);
    sshbuf_put_u32(&p, SSH_CHAN_MAX_PACKET);
    int rc = ssh_packet_send(s, &p);
    sshbuf_free(&p);
    if (rc != 0) return -1;
    c->open = 1;
    return 0;
}

/* Wait for the channel request that actually starts work: "shell" or
 * "exec". Anything else in between (pty-req, env, window-change) is
 * acknowledged if a reply was requested and otherwise ignored - see
 * the file comment for why a pty-req in particular is not acted on. */
static int wait_for_start(struct ssh *s, struct ssh_channel *c, int *is_exec,
                          char *command, size_t command_size) {
    for (;;) {
        uint8_t type;
        if (ssh_packet_recv(s, &type) != 0) return -1;
        if (type != SSH_MSG_CHANNEL_REQUEST) {
            /* Window adjustments etc. can arrive here too. */
            if (ssh_chan_dispatch(s, c, type, -1, -1) != 0) return -1;
            continue;
        }

        uint32_t id;
        const char *req;
        size_t req_len;
        int want_reply = 0;
        if (sshbuf_get_u32(&s->pkt, &id) != 0 ||
            sshbuf_get_cstring(&s->pkt, &req, &req_len) != 0 ||
            sshbuf_get_bool(&s->pkt, &want_reply) != 0) {
            return ssh_fail(s, "malformed CHANNEL_REQUEST");
        }

        int starts = 0;
        if (req_len == 5 && memcmp(req, "shell", 5) == 0) {
            *is_exec = 0;
            command[0] = '\0';
            starts = 1;
        } else if (req_len == 4 && memcmp(req, "exec", 4) == 0) {
            const char *cmd;
            size_t cmd_len;
            if (sshbuf_get_cstring(&s->pkt, &cmd, &cmd_len) != 0) {
                return ssh_fail(s, "malformed exec request");
            }
            size_t n = cmd_len < command_size - 1 ? cmd_len : command_size - 1;
            memcpy(command, cmd, n);
            command[n] = '\0';
            *is_exec = 1;
            starts = 1;
        }
        /* pty-req, env, shell without exec first, window-change,
         * signal: none of these need any action beyond the reply
         * every channel request gets when want_reply is set. */

        if (want_reply) {
            struct sshbuf p;
            sshbuf_init(&p);
            sshbuf_put_u8(&p, SSH_MSG_CHANNEL_SUCCESS);
            sshbuf_put_u32(&p, c->remote_id);
            int rc = ssh_packet_send(s, &p);
            sshbuf_free(&p);
            if (rc != 0) return -1;
        }
        if (starts) return 0;
    }
}

/* fork()+dup2()+execve() /bin/ksh with its stdio wired to the two
 * pipes; returns the child's pid, or -1. */
static int spawn_shell(int listen_fd, int client_fd, int to_shell_r,
                       int from_shell_w, int is_exec, const char *command) {
    int pid = fork();
    if (pid < 0) return -1;
    if (pid != 0) return pid;

    dup2(to_shell_r, 0);
    dup2(from_shell_w, 1);
    dup2(from_shell_w, 2);
    /* Nothing past fd 2 belongs in the shell's table - the listening
     * socket least of all (a child holding it open would keep the
     * port bound even after sshd itself exits). */
    close(listen_fd);
    close(client_fd);
    close(to_shell_r);
    close(from_shell_w);

    /* ksh is a tpm-installed package, not part of the base image;
     * fall back to the kernel's own tsh if it was never installed. */
    const char *shell = "/bin/ksh";
    if (access(shell, X_OK) != 0) {
        shell = "/bin/tsh";
    }

    char *argv[8];
    int n = 0;
    argv[n++] = (char *)shell;
    if (is_exec) {
        argv[n++] = "-c";
        argv[n++] = (char *)command;
    }
    argv[n] = NULL;
    static char *const envp[] = { "PATH=/bin:/usr/bin", "HOME=/",
                                  "USER=root", NULL };
    execve(shell, argv, envp);
    _exit(127);
}

/* ---- one connection ---- */

static void handle_connection(int listen_fd, int client_fd,
                              const struct ssh_key *hostkey) {
    struct ssh s;
    ssh_init(&s, client_fd, 1);

    if (ssh_exchange_versions(&s, "TUS_1.0") != 0) {
        vsay("%s", s.err);
        goto done;
    }
    if (ssh_kex(&s, hostkey, NULL, NULL) != 0) {
        vsay("%s", s.err);
        goto done;
    }
    if (accept_service_request(&s) != 0) {
        vsay("%s", s.err);
        goto done;
    }

    char user[64];
    int authed = authenticate(&s, user, sizeof(user));
    if (authed < 0) {
        vsay("%s", s.err);
        goto done;
    }
    if (authed == 0) {
        ssh_send_disconnect(&s, SSH_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE,
                            "too many authentication failures");
        goto done;
    }
    vsay("%s authenticated", user);

    struct ssh_channel c;
    if (accept_session_channel(&s, &c) != 0) {
        vsay("%s", s.err);
        goto done;
    }

    int is_exec = 0;
    char command[1024];
    if (wait_for_start(&s, &c, &is_exec, command, sizeof(command)) != 0) {
        vsay("%s", s.err);
        goto done;
    }

    int to_shell[2], from_shell[2];
    if (pipe(to_shell) != 0 || pipe(from_shell) != 0) {
        ssh_fail(&s, "pipe: %s", strerror(errno));
        goto done;
    }

    int shell_pid = spawn_shell(listen_fd, client_fd, to_shell[0],
                                from_shell[1], is_exec, command);
    close(to_shell[0]);
    close(from_shell[1]);
    if (shell_pid < 0) {
        close(to_shell[1]);
        close(from_shell[0]);
        ssh_fail(&s, "fork: %s", strerror(errno));
        goto done;
    }

    ssh_chan_pump(&s, &c, from_shell[0], to_shell[1], -1);

    close(to_shell[1]);
    close(from_shell[0]);
    /* Reap the shell directly - it is this connection's own child,
     * not the listener's, so the listener's post-accept() reap loop
     * never sees it. */
    int status;
    waitpid(shell_pid, &status, 0);

done:
    ssh_close(&s);
}

/* ---- main ---- */

static void usage(void) {
    fprintf(stderr, "usage: sshd [-p port] [-v]\n");
    exit(2);
}

int main(int argc, char **argv) {
    int port = SSHD_DEFAULT_PORT;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else {
            usage();
        }
    }
    if (port <= 0 || port > 65535) usage();

    struct ssh_key hostkey;
    if (ensure_host_key(&hostkey) != 0) {
        return 1;
    }

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        fprintf(stderr, "sshd: socket: %s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = (uint16_t)((port << 8) | (port >> 8));
    sa.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_fd, (struct sockaddr *)&sa, sizeof(sa)) != 0) {
        fprintf(stderr, "sshd: bind: %s\n", strerror(errno));
        return 1;
    }
    if (listen(listen_fd, 16) != 0) {
        fprintf(stderr, "sshd: listen: %s\n", strerror(errno));
        return 1;
    }

    fprintf(stderr, "sshd: listening on port %d\n", port);

    for (;;) {
        /* Reap whatever finished since the last accept() - the only
         * point a blocked listener gets to run any code at all, so
         * this is also the only place zombies from EARLIER
         * connections get cleared (see the file comment: TASK_MAX is
         * only 16). */
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0) {
        }

        int client_fd = accept(listen_fd, NULL, NULL);
        if (client_fd < 0) {
            if (errno == EINTR) continue;
            fprintf(stderr, "sshd: accept: %s\n", strerror(errno));
            continue;
        }

        int pid = fork();
        if (pid < 0) {
            fprintf(stderr, "sshd: fork: %s\n", strerror(errno));
            close(client_fd);
            continue;
        }
        if (pid == 0) {
            /* The child owns this connection; the listening socket is
             * only there so spawn_shell() can close it in the
             * grandchild too. */
            handle_connection(listen_fd, client_fd, &hostkey);
            close(client_fd);
            _exit(0);
        }
        close(client_fd);
    }
}
