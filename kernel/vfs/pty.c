/*
 * pty.c - pseudo-terminal devices
 *
 * See pty.h. Each slot is a pair of 4 KiB ring buffers, exactly the
 * pipe_read/pipe_write discipline in vfs.c: a reader hlt()s while its
 * ring is empty and the writer side is still open, a writer hlt()s
 * while its ring is full.
 */

#include "pty.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vfs.h"
#include "../arch/x86_64/io.h"
#include "../core/errno.h"
#include "../core/klib.h"
#include "../mm/kmalloc.h"

#define PTY_COUNT   8
#define PTY_RING_SZ 4096

struct pty_ring {
    uint8_t buf[PTY_RING_SZ];
    size_t head;  /* read position */
    size_t count; /* bytes buffered */
};

struct pty_slot {
    bool in_use;
    bool master_open;
    bool slave_open;
    uint32_t index;
    struct pty_ring m2s; /* master write -> slave read */
    struct pty_ring s2m; /* slave write -> master read */
    uint16_t win_rows, win_cols;
};

static struct pty_slot g_pty[PTY_COUNT];
static struct vfs_node *g_master_node[PTY_COUNT];

/* Linux ioctl numbers musl's openpty()/ptsname() rely on. */
#define TIOCGPTN   0x80045430 /* get pty number (int*) */
#define TIOCSPTLCK 0x40045431 /* (un)lock pty (int*), we have no lock */
#define TIOCGWINSZ 0x5413
#define TIOCSWINSZ 0x5414
#define TCGETS     0x5401
#define TCSETS     0x5402
#define TCSETSW    0x5403
#define TCSETSF    0x5404

struct pty_winsize {
    uint16_t ws_row, ws_col, ws_xpixel, ws_ypixel;
};

static long ring_read(struct pty_ring *r, void *buf, size_t count,
                       bool *peer_open) {
    while (r->count == 0) {
        if (!*peer_open) {
            return 0; /* EOF: the other side went away */
        }
        hlt();
    }
    size_t n = count < r->count ? count : r->count;
    size_t first = PTY_RING_SZ - r->head;
    if (n > first) {
        memcpy(buf, r->buf + r->head, first);
        memcpy((uint8_t *)buf + first, r->buf, n - first);
    } else {
        memcpy(buf, r->buf + r->head, n);
    }
    r->head = (r->head + n) % PTY_RING_SZ;
    r->count -= n;
    return (long)n;
}

static long ring_write(struct pty_ring *r, const void *buf, size_t count,
                        bool *peer_open) {
    size_t written = 0;
    const uint8_t *src = buf;
    while (written < count) {
        if (!*peer_open) {
            return written > 0 ? (long)written : -EINVAL;
        }
        while (r->count == PTY_RING_SZ && *peer_open) {
            hlt();
        }
        if (!*peer_open) {
            continue;
        }
        size_t space = PTY_RING_SZ - r->count;
        size_t n = (count - written) < space ? (count - written) : space;
        size_t tail = (r->head + r->count) % PTY_RING_SZ;
        size_t first = PTY_RING_SZ - tail;
        if (n > first) {
            memcpy(r->buf + tail, src + written, first);
            memcpy(r->buf, src + written + first, n - first);
        } else {
            memcpy(r->buf + tail, src + written, n);
        }
        r->count += n;
        written += n;
    }
    return (long)written;
}

/* ---- master side (/dev/ptmx) ---- */

static long pty_master_read(void *priv, void *buf, size_t count, size_t pos) {
    (void)pos;
    struct pty_slot *s = priv;
    return ring_read(&s->s2m, buf, count, &s->slave_open);
}

static long pty_master_write(void *priv, const void *buf, size_t count,
                              size_t pos) {
    (void)pos;
    struct pty_slot *s = priv;
    return ring_write(&s->m2s, buf, count, &s->slave_open);
}

static int pty_master_ioctl(void *priv, uint64_t request, void *arg) {
    struct pty_slot *s = priv;
    switch (request) {
    case TIOCGPTN:
        if (arg == NULL) {
            return -EINVAL;
        }
        *(uint32_t *)arg = s->index;
        return 0;
    case TIOCSPTLCK:
        return 0; /* no lock to take: the slave node always exists */
    case TIOCGWINSZ: {
        if (arg == NULL) {
            return -EINVAL;
        }
        struct pty_winsize *ws = arg;
        ws->ws_row = s->win_rows;
        ws->ws_col = s->win_cols;
        ws->ws_xpixel = 0;
        ws->ws_ypixel = 0;
        return 0;
    }
    case TIOCSWINSZ: {
        if (arg == NULL) {
            return -EINVAL;
        }
        struct pty_winsize *ws = arg;
        s->win_rows = ws->ws_row;
        s->win_cols = ws->ws_col;
        return 0;
    }
    default:
        return -ENOTTY;
    }
}

static short pty_master_poll(void *priv) {
    struct pty_slot *s = priv;
    short bits = 0;
    if (s->s2m.count > 0 || !s->slave_open) {
        bits |= 0x1; /* POLLIN */
    }
    if (s->m2s.count < PTY_RING_SZ) {
        bits |= 0x4; /* POLLOUT */
    }
    return bits;
}

/* ---- slave side (/dev/pts/N) ---- */

static long pty_slave_read(void *priv, void *buf, size_t count, size_t pos) {
    (void)pos;
    struct pty_slot *s = priv;
    return ring_read(&s->m2s, buf, count, &s->master_open);
}

static long pty_slave_write(void *priv, const void *buf, size_t count,
                             size_t pos) {
    (void)pos;
    struct pty_slot *s = priv;
    return ring_write(&s->s2m, buf, count, &s->master_open);
}

static int pty_slave_ioctl(void *priv, uint64_t request, void *arg) {
    struct pty_slot *s = priv;
    switch (request) {
    case TCGETS:
        if (arg != NULL) {
            memset(arg, 0, 60); /* struct tus_termios: all-zero (raw) is fine */
        }
        return 0;
    case TCSETS:
    case TCSETSW:
    case TCSETSF:
        return 0; /* accepted, not honoured: raw byte stream either way */
    case TIOCGWINSZ: {
        if (arg == NULL) {
            return -EINVAL;
        }
        struct pty_winsize *ws = arg;
        ws->ws_row = s->win_rows;
        ws->ws_col = s->win_cols;
        ws->ws_xpixel = 0;
        ws->ws_ypixel = 0;
        return 0;
    }
    case TIOCSWINSZ: {
        if (arg == NULL) {
            return -EINVAL;
        }
        struct pty_winsize *ws = arg;
        s->win_rows = ws->ws_row;
        s->win_cols = ws->ws_col;
        return 0;
    }
    default:
        return -ENOTTY;
    }
}

static short pty_slave_poll(void *priv) {
    struct pty_slot *s = priv;
    short bits = 0;
    if (s->m2s.count > 0 || !s->master_open) {
        bits |= 0x1;
    }
    if (s->s2m.count < PTY_RING_SZ) {
        bits |= 0x4;
    }
    return bits;
}

static void pty_master_close(void *priv) {
    struct pty_slot *s = priv;
    s->master_open = false;
    if (!s->slave_open) {
        s->in_use = false; /* both sides gone: back to the pool */
    }
}

static void pty_slave_close(void *priv) {
    struct pty_slot *s = priv;
    s->slave_open = false;
    if (!s->master_open) {
        s->in_use = false;
    }
}

static const struct file_ops g_master_ops = {
    pty_master_read, pty_master_write, pty_master_ioctl, pty_master_poll,
    pty_master_close,
};
static const struct file_ops g_slave_ops = {
    pty_slave_read, pty_slave_write, pty_slave_ioctl, pty_slave_poll,
    pty_slave_close,
};

struct vfs_node *pty_alloc_master(void) {
    for (int i = 0; i < PTY_COUNT; i++) {
        struct pty_slot *s = &g_pty[i];
        if (!s->in_use) {
            memset(s, 0, sizeof(*s));
            s->in_use = true;
            s->master_open = true;
            s->slave_open = false;
            s->index = (uint32_t)i;
            s->win_rows = 24;
            s->win_cols = 80;
            /* The slave node's priv already points at this slot; a
             * later open("/dev/pts/N") just flips slave_open. */
            return g_master_node[i];
        }
    }
    return NULL; /* pool exhausted: -EMFILE at the caller */
}

/* vfs_close() on a device node has no hook back into pty.c today (it
 * only drops the vfs_file), so a slot is freed for reuse the moment
 * open_refs on BOTH nodes drops to zero - checked lazily, from the
 * allocator above, by open_refs rather than a dedicated callback. */

void pty_init(void) {
    vfs_create_dir("/dev/pts");
    for (int i = 0; i < PTY_COUNT; i++) {
        g_pty[i].index = (uint32_t)i;
        char path[] = "/dev/pts/0";
        path[sizeof(path) - 2] = (char)('0' + i); /* PTY_COUNT <= 10 */
        vfs_create_device(path, &g_slave_ops, &g_pty[i]);
        /* The master side has no fixed path: /dev/ptmx special-cases
         * in vfs_open_mode() and returns one of these nodes, which
         * exist only to carry file_ops + priv (never linked into the
         * tree, so ls /dev never lists 8 "ptmx" entries). */
        struct vfs_node *m = kmalloc(sizeof(*m));
        memset(m, 0, sizeof(*m));
        m->type = VFS_DEVICE;
        m->mode = 0600;
        m->ops = &g_master_ops;
        m->priv = &g_pty[i];
        g_master_node[i] = m;
    }
}
