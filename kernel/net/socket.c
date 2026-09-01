/*
 * socket.c - Unix domain socket implementation
 *
 * Each socket owns a 4 KiB ring buffer that it RECEIVES into; writing
 * copies into the peer's buffer. Two paired sockets therefore give one
 * ring per direction, which is what makes the channel bidirectional
 * where a pipe is not.
 *
 * Blocking follows the pipe model in vfs.c: a reader with an empty
 * buffer (or a writer with a full peer buffer) spins on hlt(), and the
 * 100 Hz PIT tick preempts it so the peer task gets CPU time. Every
 * loop re-reads the peer pointer, because the peer can be closed by
 * another task while we wait.
 *
 * Connection setup mirrors UNIX: connect() builds the server-side
 * socket itself, links it to the client and pushes it onto the
 * listener's backlog, then returns immediately. accept() only has to
 * pop that queue - so a single-threaded program can connect to its own
 * listener without deadlocking, exactly like on Linux.
 */

#include "socket.h"

#include "../arch/x86_64/io.h"
#include "../core/errno.h"
#include "../core/klib.h"
#include "../mm/kmalloc.h"
#include "../vfs/vfs.h"

/* Receive buffer per direction (same size as a pipe). */
#define UNIX_BUF_SIZE 4096

/* Hard cap on a listener's pending-connection queue. */
#define UNIX_BACKLOG_MAX 8

/* Socket lifecycle states. */
#define USOCK_NEW       0 /* created, no address, not connected */
#define USOCK_BOUND     1 /* bound to a path, not listening yet */
#define USOCK_LISTENING 2 /* accepting connections into the backlog */
#define USOCK_CONNECTED 3 /* paired with a peer */

struct unix_sock {
    int state;
    int refs; /* one per fd table slot (plus one while queued) */

    /* Half-close flags set by shutdown(). */
    int shut_rd;
    int shut_wr;

    /* Receive ring: bytes the peer has written and we have not read. */
    uint8_t buf[UNIX_BUF_SIZE];
    size_t head;  /* read position */
    size_t count; /* bytes buffered */

    /* Connected peer, or NULL once it is gone (read then sees EOF). */
    struct unix_sock *peer;

    /* Bound address ("" when unnamed). The VFS node is looked up by
     * this path rather than cached, so unlinking the node can never
     * leave a dangling pointer here. */
    char path[UNIX_PATH_MAX];

    /* Listener state: connections built by connect(), waiting for
     * accept(). Each entry holds one reference, handed to the fd. */
    struct unix_sock *backlog[UNIX_BACKLOG_MAX];
    int backlog_n;
    int backlog_max;
};

/* ---- allocation ---- */

static struct unix_sock *sock_alloc(void) {
    struct unix_sock *s = kmalloc(sizeof(*s));
    if (s == NULL) {
        return NULL;
    }
    memset(s, 0, sizeof(*s));
    s->state = USOCK_NEW;
    s->refs = 1;
    return s;
}

void unix_sock_ref(struct unix_sock *s) {
    if (s != NULL) {
        s->refs++;
    }
}

void unix_sock_unref(struct unix_sock *s) {
    if (s == NULL || --s->refs > 0) {
        return;
    }

    /* Release the name. The node itself stays in the filesystem (UNIX
     * leaves it behind for unlink()); clearing priv is what turns a
     * later connect() into -ECONNREFUSED. */
    if (s->path[0] != '\0') {
        struct vfs_node *n = vfs_lookup(s->path);
        if (n != NULL && n->type == VFS_SOCKET && n->priv == s) {
            n->priv = NULL;
        }
    }

    /* The peer must not be left pointing at freed memory; it sees the
     * broken link as EOF on read and -EPIPE on write. */
    if (s->peer != NULL) {
        s->peer->peer = NULL;
        s->peer = NULL;
    }

    /* Connections that were queued but never accepted die with the
     * listener; their clients observe the same broken link. */
    for (int i = 0; i < s->backlog_n; i++) {
        unix_sock_unref(s->backlog[i]);
    }
    s->backlog_n = 0;

    kfree(s);
}

/* ---- creation ---- */

struct unix_sock *unix_sock_create(int domain, int type, int protocol,
                                   long *err) {
    if (domain != AF_UNIX) {
        *err = -EAFNOSUPPORT;
        return NULL;
    }
    /* SOCK_CLOEXEC is meaningless here (TUS has no close-on-exec flag
     * at all), so it is accepted and dropped. SOCK_NONBLOCK is
     * refused rather than faked: every operation below blocks. */
    if (type & SOCK_NONBLOCK) {
        *err = -EINVAL;
        return NULL;
    }
    type &= ~SOCK_CLOEXEC;
    if (type != SOCK_STREAM) {
        *err = -EPROTONOSUPPORT;
        return NULL;
    }
    if (protocol != 0) {
        *err = -EPROTONOSUPPORT;
        return NULL;
    }

    struct unix_sock *s = sock_alloc();
    if (s == NULL) {
        *err = -ENOMEM;
        return NULL;
    }
    return s;
}

/* ---- naming ---- */

long unix_sock_bind(struct unix_sock *s, const char *path) {
    if (s == NULL || path == NULL) {
        return -EINVAL;
    }
    if (s->state != USOCK_NEW) {
        return -EINVAL; /* already bound or connected */
    }
    size_t len = strlen(path);
    if (len == 0) {
        return -EINVAL;
    }
    if (len >= UNIX_PATH_MAX) {
        return -ENAMETOOLONG;
    }
    if (vfs_lookup(path) != NULL) {
        return -EADDRINUSE; /* the path must not exist yet, like UNIX */
    }
    if (vfs_create_socket(path, s) == NULL) {
        return -ENOENT; /* parent directory missing */
    }
    memcpy(s->path, path, len + 1);
    s->state = USOCK_BOUND;
    return 0;
}

long unix_sock_listen(struct unix_sock *s, int backlog) {
    if (s == NULL) {
        return -EINVAL;
    }
    if (s->state == USOCK_CONNECTED) {
        return -EISCONN;
    }
    if (s->state != USOCK_BOUND && s->state != USOCK_LISTENING) {
        return -EDESTADDRREQ; /* listen() before bind() */
    }
    if (backlog < 1) {
        backlog = 1;
    }
    if (backlog > UNIX_BACKLOG_MAX) {
        backlog = UNIX_BACKLOG_MAX;
    }
    s->backlog_max = backlog;
    s->state = USOCK_LISTENING;
    return 0;
}

long unix_sock_getname(struct unix_sock *s, char *out, size_t size) {
    if (s == NULL || out == NULL || size == 0) {
        return -EINVAL;
    }
    size_t len = strlen(s->path);
    if (len >= size) {
        len = size - 1;
    }
    memcpy(out, s->path, len);
    out[len] = '\0';
    return (long)len;
}

/* ---- connection setup ---- */

/* Cross-link two fresh sockets into a connected pair. */
static void sock_link(struct unix_sock *a, struct unix_sock *b) {
    a->peer = b;
    b->peer = a;
    a->state = USOCK_CONNECTED;
    b->state = USOCK_CONNECTED;
}

long unix_sock_connect(struct unix_sock *s, const char *path) {
    if (s == NULL || path == NULL) {
        return -EINVAL;
    }
    if (s->state == USOCK_CONNECTED) {
        return -EISCONN;
    }
    if (s->state == USOCK_LISTENING) {
        return -EOPNOTSUPP;
    }
    if (strlen(path) >= UNIX_PATH_MAX) {
        return -ENAMETOOLONG;
    }

    /* The listener is re-resolved on every attempt: while we wait for
     * a backlog slot the server may exit, which unbinds the node. */
    for (;;) {
        struct vfs_node *n = vfs_lookup(path);
        if (n == NULL) {
            return -ENOENT;
        }
        if (n->type != VFS_SOCKET) {
            return -ECONNREFUSED;
        }
        struct unix_sock *l = (struct unix_sock *)n->priv;
        if (l == NULL || l->state != USOCK_LISTENING) {
            return -ECONNREFUSED;
        }
        if (l->backlog_n < l->backlog_max) {
            /* Build the server-side endpoint here and queue it. The
             * reference it carries is handed to accept()'s fd. */
            struct unix_sock *srv = sock_alloc();
            if (srv == NULL) {
                return -ENOMEM;
            }
            sock_link(s, srv);
            l->backlog[l->backlog_n++] = srv;
            return 0;
        }
        hlt(); /* backlog full: wait for the server to accept */
    }
}

struct unix_sock *unix_sock_accept(struct unix_sock *s, long *err) {
    if (s == NULL) {
        *err = -EINVAL;
        return NULL;
    }
    if (s->state != USOCK_LISTENING) {
        *err = -EINVAL;
        return NULL;
    }
    while (s->backlog_n == 0) {
        hlt(); /* no pending connection: wait for a client */
    }

    struct unix_sock *srv = s->backlog[0];
    for (int i = 1; i < s->backlog_n; i++) {
        s->backlog[i - 1] = s->backlog[i];
    }
    s->backlog_n--;
    return srv; /* reference transferred to the caller */
}

long unix_sock_pair(struct unix_sock **a, struct unix_sock **b) {
    struct unix_sock *x = sock_alloc();
    struct unix_sock *y = sock_alloc();
    if (x == NULL || y == NULL) {
        kfree(x);
        kfree(y);
        return -ENOMEM;
    }
    sock_link(x, y);
    *a = x;
    *b = y;
    return 0;
}

/* ---- data transfer ---- */

long unix_sock_read(struct unix_sock *s, void *buf, size_t count) {
    if (s == NULL || buf == NULL) {
        return -EINVAL;
    }
    if (s->state != USOCK_CONNECTED) {
        return -ENOTCONN;
    }
    if (count == 0) {
        return 0;
    }

    while (s->count == 0) {
        if (s->shut_rd) {
            return 0; /* this end was shut down for reading */
        }
        if (s->peer == NULL || s->peer->shut_wr) {
            return 0; /* EOF: the writer is gone */
        }
        hlt();
    }

    size_t n = count < s->count ? count : s->count;
    size_t first = UNIX_BUF_SIZE - s->head;
    if (n > first) {
        memcpy(buf, s->buf + s->head, first);
        memcpy((uint8_t *)buf + first, s->buf, n - first);
    } else {
        memcpy(buf, s->buf + s->head, n);
    }
    s->head = (s->head + n) % UNIX_BUF_SIZE;
    s->count -= n;
    return (long)n;
}

long unix_sock_write(struct unix_sock *s, const void *buf, size_t count) {
    if (s == NULL || buf == NULL) {
        return -EINVAL;
    }
    if (s->state != USOCK_CONNECTED) {
        return -ENOTCONN;
    }
    if (s->shut_wr) {
        return -EPIPE;
    }

    size_t total = 0;
    while (total < count) {
        struct unix_sock *p = s->peer;
        if (p == NULL || p->shut_rd) {
            /* Partial writes are reported; a write that moved nothing
             * is the broken-pipe case. */
            return total > 0 ? (long)total : -EPIPE;
        }
        if (p->count == UNIX_BUF_SIZE) {
            hlt(); /* peer buffer full: wait for it to drain */
            continue;
        }
        size_t space = UNIX_BUF_SIZE - p->count;
        size_t n = count - total < space ? count - total : space;
        size_t wpos = (p->head + p->count) % UNIX_BUF_SIZE;
        size_t first = UNIX_BUF_SIZE - wpos;
        if (n > first) {
            memcpy(p->buf + wpos, (const uint8_t *)buf + total, first);
            memcpy(p->buf, (const uint8_t *)buf + total + first, n - first);
        } else {
            memcpy(p->buf + wpos, (const uint8_t *)buf + total, n);
        }
        p->count += n;
        total += n;
    }
    return (long)total;
}

long unix_sock_shutdown(struct unix_sock *s, int how) {
    if (s == NULL) {
        return -EINVAL;
    }
    if (s->state != USOCK_CONNECTED) {
        return -ENOTCONN;
    }
    if (how != SHUT_RD && how != SHUT_WR && how != SHUT_RDWR) {
        return -EINVAL;
    }
    if (how == SHUT_RD || how == SHUT_RDWR) {
        s->shut_rd = 1;
    }
    if (how == SHUT_WR || how == SHUT_RDWR) {
        s->shut_wr = 1;
    }
    return 0;
}

/* ---- readiness ---- */

short unix_sock_poll(struct unix_sock *s) {
    if (s == NULL) {
        return POLLNVAL;
    }
    if (s->state == USOCK_LISTENING) {
        /* A pending connection is what makes a listener "readable". */
        return s->backlog_n > 0 ? POLLIN : 0;
    }
    if (s->state != USOCK_CONNECTED) {
        /* Created or merely bound: nothing can ever arrive on this fd,
         * so report hang-up instead of making the caller wait forever.
         * read/write still fail with -ENOTCONN, which is what Linux
         * does for an unconnected socket too. */
        return POLLHUP;
    }

    short r = 0;
    /* Readable when data is buffered, or when a read would return the
     * EOF/0 that a dead or half-closed peer produces. */
    if (s->count > 0 || s->shut_rd) {
        r |= POLLIN;
    }
    if (s->peer == NULL) {
        r |= POLLIN | POLLHUP;
    } else if (s->peer->shut_wr) {
        r |= POLLIN;
    }
    /* Writable when the peer still reads and its buffer has room. */
    if (!s->shut_wr && s->peer != NULL && !s->peer->shut_rd &&
        s->peer->count < UNIX_BUF_SIZE) {
        r |= POLLOUT;
    }
    return r;
}


/* ---- AF_INET (IPv4) sockets ----
 *
 * A thin shim: the TCP and UDP protocol control blocks hold all the
 * state, and struct inet_sock is what the fd table points at. The only
 * thing it owns is the reference count and the blocking flag.
 */

#include "ip.h"
#include "netif.h"
#include "tcp.h"
#include "udp.h"

struct inet_sock {
    int domain;
    int type;
    int protocol;
    int refs;
    bool nonblock;

    union {
        struct tcp_pcb *tcp;
        struct udp_pcb *udp;
    } pcb;
};

struct inet_sock *inet_sock_create(int type, int protocol, long *err) {
    int base_type = type & ~(SOCK_CLOEXEC | SOCK_NONBLOCK);

    if (base_type == SOCK_STREAM && protocol != 0 && protocol != IPPROTO_TCP) {
        *err = -EPROTONOSUPPORT;
        return NULL;
    }
    if (base_type == SOCK_DGRAM && protocol != 0 && protocol != IPPROTO_UDP) {
        *err = -EPROTONOSUPPORT;
        return NULL;
    }
    if (base_type != SOCK_STREAM && base_type != SOCK_DGRAM) {
        *err = -EPROTONOSUPPORT;
        return NULL;
    }

    struct inet_sock *s = kmalloc(sizeof(*s));
    if (s == NULL) {
        *err = -ENOMEM;
        return NULL;
    }
    memset(s, 0, sizeof(*s));

    s->domain = AF_INET;
    s->type = base_type;
    s->protocol = protocol ? protocol
                           : (base_type == SOCK_STREAM ? IPPROTO_TCP
                                                       : IPPROTO_UDP);
    s->refs = 1;
    s->nonblock = (type & SOCK_NONBLOCK) != 0;

    if (base_type == SOCK_STREAM) {
        s->pcb.tcp = tcp_pcb_new();
        if (!s->pcb.tcp) {
            kfree(s);
            *err = -ENOMEM;
            return NULL;
        }
    } else {
        s->pcb.udp = udp_pcb_new();
        if (!s->pcb.udp) {
            kfree(s);
            *err = -ENOMEM;
            return NULL;
        }
    }
    return s;
}

/* Wrap a PCB that TCP handed back from accept(). */
static struct inet_sock *inet_sock_from_pcb(struct tcp_pcb *pcb) {
    struct inet_sock *s = kmalloc(sizeof(*s));
    if (!s) return NULL;

    memset(s, 0, sizeof(*s));
    s->domain = AF_INET;
    s->type = SOCK_STREAM;
    s->protocol = IPPROTO_TCP;
    s->refs = 1;
    s->pcb.tcp = pcb;
    return s;
}

long inet_sock_bind(struct inet_sock *s, uint32_t ip, uint16_t port) {
    if (!s) return -EINVAL;
    if (s->type == SOCK_STREAM) return tcp_bind(s->pcb.tcp, ip, port);
    return udp_bind(s->pcb.udp, ip, port);
}

long inet_sock_connect(struct inet_sock *s, uint32_t dst_ip, uint16_t dst_port) {
    if (!s) return -EINVAL;
    if (s->type == SOCK_STREAM) return tcp_connect(s->pcb.tcp, dst_ip, dst_port);
    return udp_connect(s->pcb.udp, dst_ip, dst_port);
}

long inet_sock_listen(struct inet_sock *s, int backlog) {
    if (!s || s->type != SOCK_STREAM) return -EOPNOTSUPP;
    return tcp_listen(s->pcb.tcp, backlog);
}

struct inet_sock *inet_sock_accept(struct inet_sock *s, long *err) {
    if (!s || s->type != SOCK_STREAM) {
        *err = -EOPNOTSUPP;
        return NULL;
    }

    struct tcp_pcb *child = tcp_accept(s->pcb.tcp, err, !s->nonblock);
    if (!child) return NULL;

    struct inet_sock *ns = inet_sock_from_pcb(child);
    if (!ns) {
        tcp_close(child);
        *err = -ENOMEM;
        return NULL;
    }
    *err = 0;
    return ns;
}

long inet_sock_read(struct inet_sock *s, void *buf, size_t len) {
    if (!s) return -ENOTSOCK;
    if (s->type == SOCK_STREAM) {
        return tcp_recv(s->pcb.tcp, buf, len, !s->nonblock);
    }
    return udp_recvfrom(s->pcb.udp, buf, len, NULL, NULL,
                        s->nonblock ? 0 : 30000);
}

long inet_sock_write(struct inet_sock *s, const void *buf, size_t len) {
    if (!s) return -ENOTSOCK;
    if (s->type == SOCK_STREAM) {
        return tcp_send(s->pcb.tcp, buf, len, !s->nonblock);
    }
    if (s->pcb.udp->remote_ip == 0) return -EDESTADDRREQ;
    return udp_sendto(s->pcb.udp, buf, len, s->pcb.udp->remote_ip,
                      s->pcb.udp->remote_port);
}

long inet_sock_sendto(struct inet_sock *s, const void *buf, size_t len,
                      uint32_t dst_ip, uint16_t dst_port) {
    if (!s) return -ENOTSOCK;
    if (s->type != SOCK_DGRAM) return inet_sock_write(s, buf, len);
    if (dst_ip == 0) return inet_sock_write(s, buf, len);
    return udp_sendto(s->pcb.udp, buf, len, dst_ip, dst_port);
}

long inet_sock_recvfrom(struct inet_sock *s, void *buf, size_t len,
                        uint32_t *src_ip, uint16_t *src_port,
                        uint32_t timeout_ms) {
    if (!s) return -ENOTSOCK;
    if (s->type != SOCK_DGRAM) {
        if (src_ip) *src_ip = 0;
        if (src_port) *src_port = 0;
        return inet_sock_read(s, buf, len);
    }
    return udp_recvfrom(s->pcb.udp, buf, len, src_ip, src_port,
                        s->nonblock ? 0 : timeout_ms);
}

long inet_sock_shutdown(struct inet_sock *s, int how) {
    if (!s) return -ENOTSOCK;
    if (s->type != SOCK_STREAM) return 0;
    if (how == SHUT_WR || how == SHUT_RDWR) {
        return tcp_shutdown_write(s->pcb.tcp);
    }
    return 0;
}

long inet_sock_getname(struct inet_sock *s, uint32_t *ip, uint16_t *port,
                       bool peer) {
    if (!s) return -ENOTSOCK;

    if (s->type == SOCK_STREAM) {
        struct tcp_pcb *pcb = s->pcb.tcp;
        *ip = peer ? pcb->remote_ip : (pcb->local_ip ? pcb->local_ip : g_netif.ip);
        *port = peer ? pcb->remote_port : pcb->local_port;
    } else {
        struct udp_pcb *pcb = s->pcb.udp;
        *ip = peer ? pcb->remote_ip : (pcb->local_ip ? pcb->local_ip : g_netif.ip);
        *port = peer ? pcb->remote_port : pcb->local_port;
    }
    return 0;
}

short inet_sock_poll(struct inet_sock *s) {
    if (!s) return POLLERR;
    if (s->type == SOCK_STREAM) return tcp_poll(s->pcb.tcp);
    return udp_poll(s->pcb.udp);
}

void inet_sock_set_nonblock(struct inet_sock *s, bool on) {
    if (s) s->nonblock = on;
}

bool inet_sock_is_stream(struct inet_sock *s) {
    return s != NULL && s->type == SOCK_STREAM;
}

void inet_sock_ref(struct inet_sock *s) {
    if (s) s->refs++;
}

void inet_sock_unref(struct inet_sock *s) {
    if (!s || --s->refs > 0) return;

    if (s->type == SOCK_STREAM) {
        tcp_close(s->pcb.tcp);
    } else {
        udp_pcb_free(s->pcb.udp);
    }
    kfree(s);
}

void *socket_create(int domain, int type, int protocol, long *err) {
    if (domain == AF_UNIX) {
        return unix_sock_create(domain, type, protocol, err);
    } else if (domain == AF_INET) {
        return inet_sock_create(type, protocol, err);
    }
    *err = -EAFNOSUPPORT;
    return NULL;
}
