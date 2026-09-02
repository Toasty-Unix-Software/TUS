/*
 * vfs.c - Virtual File System implementation
 *
 * The tree lives entirely in kernel memory (ramfs). Path resolution
 * splits on '/' and walks child lists by name. File writes grow the
 * backing buffer with krealloc(). Device reads/writes are forwarded to
 * the node's file_ops.
 *
 * The fd table is a fixed array; entries 0..2 are the standard
 * descriptors pre-opened on /dev/tty0.
 */

#include "vfs.h"

#include <stdbool.h>

#include "devices.h"
#include "procfs.h"
#include "pty.h"
#include "../fs/wrf.h"
#include "../arch/x86_64/io.h"
#include "../arch/x86_64/spectre.h"
#include "../core/errno.h"
#include "../core/klib.h"
#include "../mm/kmalloc.h"
#include "../net/socket.h"
#include "../sched/sched.h"

/* An open file description: a reference to a filesystem node, a pipe
 * or an AF_UNIX socket, plus the current position. Exactly one of the
 * three is non-NULL. Multiple fd slots may point at the same vfs_file
 * (dup/dup2 share the position, like POSIX), so it is refcounted; the
 * last close frees it. */
struct vfs_file {
    struct vfs_node *node;
    struct vfs_pipe *pipe;
    struct unix_sock *sock;
    struct inet_sock *inet_sock;
    int socket_domain;
    size_t pos;
    int flags;
    int refs;
    struct vfs_node *readdir_node;
    size_t readdir_index;
};

/* Pipe: a fixed 32 KiB ring buffer. Readers and writers block in a
 * hlt() loop while the buffer is empty/full; the 100 Hz PIT tick
 * preempts the waiter so the peer task gets CPU time. EOF is simply
 * "empty and no write end open"; -EPIPE is "write end open but no
 * read end". refs_r/refs_w count the open descriptors per end (one
 * per vfs_file referencing this pipe). */
#define PIPE_BUF_SIZE 32768

struct vfs_pipe {
    uint8_t buf[PIPE_BUF_SIZE];
    size_t head;  /* read position */
    size_t count; /* bytes buffered */
    int refs_r;
    int refs_w;
};

static struct vfs_node *g_root;

/* The fd table lives in the CURRENT task (struct task::fds). This
 * indirection is what makes redirection and pipelines possible: the
 * shell sets up its own slots, spawns a child (which inherits a
 * refcounted copy), then restores its own slots - the child keeps
 * its copy, so `cmd > file` and `a | b` just work. */
static struct vfs_file **fd_table(void) {
    struct task *cur = sched_current();
    return cur != NULL ? cur->fds : NULL;
}

/* ---- path helpers ---- */

/* Split "a/b/c" into parent path and final name. Returns 0 on success. */
static int path_split(const char *path, char *dir_out, size_t dir_size,
                      char *name_out, size_t name_size) {
    const char *slash = NULL;
    const char *p;
    for (p = path; *p != '\0'; p++) {
        if (*p == '/') {
            slash = p;
        }
    }
    if (slash == NULL) {
        /* No directory part at all: a bare name like "motd" means
         * "in the current directory" - "." rather than a hardcoded
         * "/", so vfs_lookup() resolves it against the caller's cwd
         * exactly like every other relative path (see vfs_path_
         * resolve()). Before per-task cwd existed every task's
         * effective directory was implicitly root, so this changes
         * nothing for any existing caller - "." and "/" resolve
         * identically until something actually calls chdir(). */
        size_t nlen = strlen(path);
        if (nlen >= name_size) {
            return -1;
        }
        memcpy(name_out, path, nlen + 1);
        dir_out[0] = '.';
        dir_out[1] = '\0';
        return 0;
    }

    size_t dlen = (size_t)(slash - path);
    if (dlen == 0) {
        /* Leading slash only (e.g. "/dev"): dir is root, name is
         * everything after the slash. Do NOT copy the slash into
         * the name - lookup splits on '/' and compares components. */
        dir_out[0] = '/';
        dir_out[1] = '\0';
    } else {
        if (dlen >= dir_size) {
            return -1;
        }
        memcpy(dir_out, path, dlen);
        dir_out[dlen] = '\0';
    }

    size_t nlen = strlen(slash + 1);
    if (nlen == 0 || nlen >= name_size) {
        return -1;
    }
    memcpy(name_out, slash + 1, nlen + 1);
    return 0;
}

/* Look up a single name inside a directory. */
static struct vfs_node *dir_find(struct vfs_node *dir, const char *name) {
    for (struct vfs_node *n = dir->child; n != NULL; n = n->sibling) {
        if (strcmp(n->name, name) == 0) {
            return n;
        }
    }
    return NULL;
}

/* Attach a freshly allocated node to a directory. */
static struct vfs_node *dir_attach(struct vfs_node *dir, struct vfs_node *node) {
    node->parent = dir;
    node->sibling = dir->child;
    dir->child = node;
    return node;
}

/* See vfs.h: moved from kernel/shell/cmd_fs.c's own path_resolve(),
 * which now calls this with the console/terminal-session cwd - the
 * algorithm is unchanged, only where the cwd string comes from. */
void vfs_path_resolve(const char *cwd, const char *in, char *out, size_t outsz) {
    char tmp[VFS_PATH_MAX + 32];

    if (in[0] == '/') {
        strncpy(tmp, in, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';
    } else {
        size_t cl = strlen(cwd);
        size_t il = strlen(in);
        if (cl + 1 + il >= sizeof(tmp)) {
            strncpy(out, "/", outsz); /* absurdly long: fall back to root */
            out[outsz - 1] = '\0';
            return;
        }
        memcpy(tmp, cwd, cl);
        tmp[cl] = '/';
        memcpy(tmp + cl + 1, in, il + 1);
    }

    /* Split on '/', honoring "." and "..". */
    const char *segs[32];
    size_t lens[32];
    int n = 0;

    const char *p = tmp;
    while (*p != '\0') {
        while (*p == '/') {
            p++;
        }
        if (*p == '\0') {
            break;
        }
        const char *s = p;
        while (*p != '\0' && *p != '/') {
            p++;
        }
        size_t len = (size_t)(p - s);

        if (len == 1 && s[0] == '.') {
            continue;
        }
        if (len == 2 && s[0] == '.' && s[1] == '.') {
            if (n > 0) {
                n--; /* walk up one level */
            }
            continue;
        }
        if (n < 32) {
            segs[n] = s;
            lens[n] = len;
            n++;
        }
    }

    /* Rebuild the normalized absolute path. */
    char *w = out;
    size_t left = outsz;
    *w++ = '/';
    left--;
    for (int i = 0; i < n && left > 1; i++) {
        if (i > 0) {
            *w++ = '/';
            left--;
        }
        size_t l = lens[i] < left - 1 ? lens[i] : left - 1;
        memcpy(w, segs[i], l);
        w += l;
        left -= l;
    }
    *w = '\0';
}

struct vfs_node *vfs_lookup(const char *path) {
    char resolved[VFS_PATH_MAX];
    if (path[0] != '/') {
        /* Relative: resolve against the calling task's cwd. Ring-0
         * kernel callers (rootfs mounting at boot, device creation)
         * always pass a leading-slash absolute path already, so this
         * only ever fires for a genuine ring-3 relative path or the
         * "." a fixed-up path_split() (below) now hands back for a
         * bare filename with no directory component. */
        struct task *cur = sched_current();
        const char *cwd = (cur != NULL) ? cur->cwd : "/";
        vfs_path_resolve(cwd, path, resolved, sizeof(resolved));
        path = resolved;
    }

    struct vfs_node *cur = g_root;

    const char *p = path;
    while (*p == '/') {
        p++;
    }
    if (*p == '\0') {
        return g_root;
    }

    while (*p != '\0') {
        const char *end = p;
        while (*end != '\0' && *end != '/') {
            end++;
        }
        size_t len = (size_t)(end - p);
        if (len >= VFS_NAME_MAX) {
            return NULL;
        }

        if (cur->type != VFS_DIR) {
            return NULL; /* path component through a non-directory */
        }
        char name[VFS_NAME_MAX];
        memcpy(name, p, len);
        name[len] = '\0';
        cur = dir_find(cur, name);
        if (cur == NULL) {
            return NULL;
        }

        p = end;
        while (*p == '/') {
            p++;
        }
    }
    return cur;
}

/* ---- node creation ---- */

struct vfs_node *vfs_create_dir(const char *path) {
    char dir_path[256];
    char name[VFS_NAME_MAX];
    if (path_split(path, dir_path, sizeof(dir_path), name, sizeof(name)) != 0) {
        return NULL;
    }
    struct vfs_node *parent = vfs_lookup(dir_path);
    if (parent == NULL || parent->type != VFS_DIR || dir_find(parent, name) != NULL) {
        return NULL;
    }

    struct vfs_node *node = kmalloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, sizeof(*node));
    node->type = VFS_DIR;
    node->mode = 0755;
    memcpy(node->name, name, strlen(name) + 1);
    dir_attach(parent, node);
    /* WRF (kernel/fs/wrf.c): a directory created under a WRF-backed
     * parent gets its own on-disk inode and directory entry, so a
     * `mkdir` under /home survives a reboot. A no-op for every ramfs
     * directory (parent->wrf_ino == 0), which is every directory
     * outside a WRF mount. */
    if (parent->wrf_ino != 0) {
        wrf_notify_create(parent, node);
    }
    return node;
}

struct vfs_node *vfs_create_file(const char *path) {
    char dir_path[256];
    char name[VFS_NAME_MAX];
    if (path_split(path, dir_path, sizeof(dir_path), name, sizeof(name)) != 0) {
        return NULL;
    }
    struct vfs_node *parent = vfs_lookup(dir_path);
    if (parent == NULL || parent->type != VFS_DIR || dir_find(parent, name) != NULL) {
        return NULL;
    }

    struct vfs_node *node = kmalloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, sizeof(*node));
    node->type = VFS_FILE;
    node->mode = 0644;
    memcpy(node->name, name, strlen(name) + 1);
    dir_attach(parent, node);
    /* See the matching comment in vfs_create_dir() just above. */
    if (parent->wrf_ino != 0) {
        wrf_notify_create(parent, node);
    }
    return node;
}

struct vfs_node *vfs_create_device(const char *path,
                                   const struct file_ops *ops, void *priv) {
    char dir_path[256];
    char name[VFS_NAME_MAX];
    if (path_split(path, dir_path, sizeof(dir_path), name, sizeof(name)) != 0) {
        return NULL;
    }
    struct vfs_node *parent = vfs_lookup(dir_path);
    if (parent == NULL || parent->type != VFS_DIR || dir_find(parent, name) != NULL) {
        return NULL;
    }

    struct vfs_node *node = kmalloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, sizeof(*node));
    node->type = VFS_DEVICE;
    node->mode = 0600;
    node->ops = ops;
    node->priv = priv;
    memcpy(node->name, name, strlen(name) + 1);
    return dir_attach(parent, node);
}

struct vfs_node *vfs_create_socket(const char *path, void *sock) {
    char dir_path[256];
    char name[VFS_NAME_MAX];
    if (path_split(path, dir_path, sizeof(dir_path), name, sizeof(name)) != 0) {
        return NULL;
    }
    struct vfs_node *parent = vfs_lookup(dir_path);
    if (parent == NULL || parent->type != VFS_DIR || dir_find(parent, name) != NULL) {
        return NULL;
    }

    struct vfs_node *node = kmalloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    memset(node, 0, sizeof(*node));
    node->type = VFS_SOCKET;
    node->mode = 0666; /* srw-rw-rw-, the usual mode for a socket */
    node->priv = sock;
    memcpy(node->name, name, strlen(name) + 1);
    return dir_attach(parent, node);
}

int vfs_remove(const char *path) {
    char dir_path[256];
    char name[VFS_NAME_MAX];
    if (path_split(path, dir_path, sizeof(dir_path), name, sizeof(name)) != 0) {
        return -1;
    }
    struct vfs_node *parent = vfs_lookup(dir_path);
    if (parent == NULL || parent->type != VFS_DIR) {
        return -1;
    }

    struct vfs_node **link = &parent->child;
    while (*link != NULL) {
        if (strcmp((*link)->name, name) == 0) {
            struct vfs_node *victim = *link;
            if (victim->type == VFS_DIR && victim->child != NULL) {
                return -1; /* directory not empty */
            }
            *link = victim->sibling;
            /* WRF (kernel/fs/wrf.c): the directory entry is gone
             * either way an unlink can end - the name is never
             * reachable again even if the node itself lingers for an
             * open fd below. A no-op unless both parent and victim
             * are WRF-backed. */
            wrf_notify_detach(parent, victim);
            if (victim->open_refs > 0) {
                /* Still open somewhere (musl's tmpfile() does
                 * open+unlink+keep using the fd): drop it from the
                 * namespace now, same as a real unlink, but leave it
                 * alive for file_unref() to free on the last close -
                 * where its WRF inode and blocks (if any) are freed
                 * too, for the same reason. */
                victim->unlinked = true;
                return 0;
            }
            wrf_notify_free(victim);
            if (victim->type == VFS_FILE) {
                kfree(victim->data);
            }
            kfree(victim);
            return 0;
        }
        link = &(*link)->sibling;
    }
    return -1;
}

/* ---- fd table ---- */

static void file_ref(struct vfs_file *f) {
    f->refs++;
    if (f->pipe != NULL) {
        if (f->flags & O_WRONLY) {
            f->pipe->refs_w++;
        } else {
            f->pipe->refs_r++;
        }
    }
    if (f->sock != NULL) {
        unix_sock_ref(f->sock);
    }
}

static void file_unref(struct vfs_file *f) {
    if (f->pipe != NULL) {
        if (f->flags & O_WRONLY) {
            f->pipe->refs_w--;
        } else {
            f->pipe->refs_r--;
        }
    }
    if (f->sock != NULL) {
        unix_sock_unref(f->sock);
    }
    if (--f->refs == 0) {
        /* A pipe lives as long as either end is referenced. */
        if (f->pipe != NULL && f->pipe->refs_r == 0 && f->pipe->refs_w == 0) {
            kfree(f->pipe);
        }
        if (f->node != NULL && --f->node->open_refs == 0) {
            if (f->node->type == VFS_DEVICE && f->node->ops != NULL &&
                f->node->ops->close != NULL) {
                f->node->ops->close(f->node->priv);
            }
            if (f->node->unlinked) {
            /* Last fd on a node vfs_remove() already detached from
             * the tree (see there): free it now, like a real unlink
             * of an open file does on close. wrf_notify_free() is the
             * deferred half of the WRF cleanup vfs_remove() started
             * (its dirent removal already ran); a no-op if the node
             * was never WRF-backed. */
                wrf_notify_free(f->node);
                kfree(f->node->data);
                kfree(f->node);
            }
        }
        kfree(f);
    }
}

/* Allocate a fresh slot (3..15; 0..2 are the standard descriptors,
 * replaced only via dup2, never by open) and take ownership. */
static long fd_alloc(struct vfs_file *f) {
    struct vfs_file **tbl = fd_table();
    if (tbl == NULL) {
        return -EBADF;
    }
    for (int i = 3; i < VFS_MAX_FDS; i++) {
        if (tbl[i] == NULL) {
            tbl[i] = f;
            return i;
        }
    }
    return -ENOMEM; /* handled by caller mapping */
}

static struct vfs_file *fd_get(long fd) {
    struct vfs_file **tbl = fd_table();
    if (tbl == NULL || fd < 0 || fd >= VFS_MAX_FDS) {
        return NULL;
    }
    /* fd comes straight from a syscall argument: a mispredicted branch
     * on the bounds check above could let the CPU speculatively read
     * tbl[fd] with an out-of-range fd before the check retires
     * (Spectre v1 / CVE-2017-5753). The barrier forces the check to
     * resolve first. */
    spectre_v1_barrier();
    return tbl[fd];
}

/* ---- permission checks ----
 *
 * node->mode/uid/gid have always been tracked (ls -l reads them, and
 * the rootfs tar sets doas/passwd to 4555) but nothing ever checked
 * them against who was asking - the comment on struct vfs_node said
 * so outright: "no kernel privilege model exists yet". Any task could
 * read or write any file, /etc/shadow included, regardless of mode
 * bits. This is that model, applied the same place Unix always has:
 * open (O_CREAT needs write on the parent directory, not the file -
 * it doesn't exist yet), unlink and mkdir (write on the parent
 * directory - removing or adding an entry changes the directory, not
 * the file), and chmod (owner or root only, or any task could hand
 * itself access to anything by just rewriting its mode bits first). */
#define VFS_R_OK 4
#define VFS_W_OK 2

/* Root (euid 0) and any call made directly by ring-0 kernel code (the
 * console shell's built-ins, the boot-time rootfs mount before the
 * scheduler exists yet) bypass every check - the same trust boundary
 * access_ok()'s from_user flag draws for raw pointers. A real,
 * non-root task is checked against its *effective* ids, not its real
 * ones, so a setuid program (doas, passwd) gets the access its owner
 * bit grants rather than the caller's own. */
static bool vfs_access_ok(const struct vfs_node *node, uint32_t want) {
    struct task *cur = sched_current();
    if (cur == NULL || cur->euid == 0) {
        return true;
    }
    uint32_t bits;
    if (cur->euid == node->uid) {
        bits = (node->mode >> 6) & 7;
    } else if (cur->egid == node->gid) {
        bits = (node->mode >> 3) & 7;
    } else {
        bits = node->mode & 7;
    }
    return (bits & want) == want;
}

/* The parent directory of `path`, for the checks that land on it
 * (O_CREAT, unlink, mkdir) rather than on the entry itself. NULL if
 * the path has no resolvable parent. */
static struct vfs_node *vfs_parent_of(const char *path) {
    char dir_path[256];
    char name[VFS_NAME_MAX];
    if (path_split(path, dir_path, sizeof(dir_path), name, sizeof(name)) != 0) {
        return NULL;
    }
    return vfs_lookup(dir_path);
}

/* ---- fd-based API ---- */

long vfs_open(const char *path, int flags) {
    return vfs_open_mode(path, flags, 0644);
}

/* open(path, flags, mode): the classic 3-arg POSIX form. mode only
 * matters when O_CREAT actually creates the file - vfs_create_file()
 * itself always starts a node at 0644, so a create with an executable
 * mode (a linker writing its output, say) needs the requested bits
 * applied here afterward, not silently dropped. */
long vfs_open_mode(const char *path, int flags, uint32_t mode) {
    if (path == NULL) {
        return -EINVAL;
    }
    if (strcmp(path, "/dev/ptmx") == 0) {
        struct vfs_node *m = pty_alloc_master();
        if (m == NULL) {
            return -EMFILE; /* all PTY_COUNT slots taken */
        }
        struct vfs_file *f = kmalloc(sizeof(*f));
        if (f == NULL) {
            return -ENOMEM;
        }
        f->node = m;
        f->pipe = NULL;
        f->sock = NULL;
        f->inet_sock = NULL;
        f->socket_domain = 0;
        f->pos = 0;
        f->readdir_node = NULL;
        f->readdir_index = 0;
        f->flags = flags;
        f->refs = 1;
        m->open_refs++;
        return fd_alloc(f);
    }

    struct vfs_node *node = vfs_lookup(path);

    if (node == NULL) {
        if (flags & O_CREAT) {
            struct vfs_node *parent = vfs_parent_of(path);
            if (parent != NULL && !vfs_access_ok(parent, VFS_W_OK)) {
                return -EACCES;
            }
            node = vfs_create_file(path);
            if (node == NULL) {
                return -ENOENT;
            }
            node->mode = mode;
            struct task *cur = sched_current();
            if (cur != NULL) {
                node->uid = cur->euid;
                node->gid = cur->egid;
            }
        } else {
            return -ENOENT;
        }
    } else {
        bool wants_write = (flags & (O_WRONLY | O_RDWR)) != 0;
        bool wants_read = !(flags & O_WRONLY);
        if ((wants_read && !vfs_access_ok(node, VFS_R_OK)) ||
            (wants_write && !vfs_access_ok(node, VFS_W_OK))) {
            return -EACCES;
        }
    }

    if (node->type == VFS_DIR && (flags & (O_WRONLY | O_RDWR))) {
        return -EISDIR;
    }
    if (node->type == VFS_FILE && (flags & O_TRUNC)) {
        if (node->wrf_ino != 0) {
            wrf_truncate(node, 0);
        } else {
            node->size = 0;
        }
    }

    struct vfs_file *f = kmalloc(sizeof(*f));
    if (f == NULL) {
        return -ENOMEM;
    }
    f->node = node;
    f->pipe = NULL;
    f->sock = NULL;
    f->inet_sock = NULL;
    f->socket_domain = 0;
    f->pos = 0;
    f->readdir_node = NULL;
    f->readdir_index = 0;
    f->flags = flags;
    f->refs = 1;
    if (flags & O_APPEND) {
        f->pos = node->size; /* append: start at the end */
    }
    node->open_refs++;

    return fd_alloc(f);
}

long vfs_close(long fd) {
    struct vfs_file **tbl = fd_table();
    if (tbl == NULL || fd < 0 || fd >= VFS_MAX_FDS || tbl[fd] == NULL) {
        return -EBADF;
    }
    struct vfs_file *f = tbl[fd];
    tbl[fd] = NULL;
    file_unref(f);
    return 0;
}

long vfs_dup(long oldfd) {
    return vfs_fcntl_dupfd(oldfd, 0);
}

/* fcntl(fd, F_DUPFD/F_DUPFD_CLOEXEC, minfd): dup(2) that lands on the
 * lowest free descriptor >= minfd instead of always the lowest free
 * one - what ksh93 (kernel/vfs/vfs.c's own comment on vfs_dup applies
 * equally here) and most real shells use to move an fd out of the
 * way (out of 0-9, conventionally) while setting up redirections.
 * TUS tracks no close-on-exec flag (a spawned/forked task's fd table
 * is already an all-or-nothing copy - see struct task::fds), so
 * F_DUPFD and F_DUPFD_CLOEXEC behave identically here; the caller
 * (sys_fcntl, kernel/syscall/syscall.c) maps both commands to this
 * one function. */
long vfs_fcntl_dupfd(long oldfd, long minfd) {
    struct vfs_file **tbl = fd_table();
    if (tbl == NULL || oldfd < 0 || oldfd >= VFS_MAX_FDS ||
        tbl[oldfd] == NULL) {
        return -EBADF;
    }
    if (minfd < 0) {
        return -EINVAL;
    }
    for (long i = minfd; i < VFS_MAX_FDS; i++) {
        if (tbl[i] == NULL) {
            tbl[i] = tbl[oldfd];
            file_ref(tbl[oldfd]);
            return i;
        }
    }
    return -EMFILE; /* no free descriptor >= minfd - real dup(2) ENOMEM
                      * is for kernel memory, EMFILE is "too many open" */
}

long vfs_dup2(long oldfd, long newfd) {
    struct vfs_file **tbl = fd_table();
    if (tbl == NULL || oldfd < 0 || oldfd >= VFS_MAX_FDS ||
        tbl[oldfd] == NULL) {
        return -EBADF;
    }
    if (newfd < 0 || newfd >= VFS_MAX_FDS) {
        return -EBADF;
    }
    if (oldfd == newfd) {
        return newfd;
    }
    if (tbl[newfd] != NULL) {
        file_unref(tbl[newfd]); /* close the target first */
    }
    tbl[newfd] = tbl[oldfd];
    file_ref(tbl[oldfd]);
    return newfd;
}

long vfs_pipe(int fds[2]) {
    struct vfs_file **tbl = fd_table();
    if (tbl == NULL) {
        return -EBADF;
    }
    struct vfs_pipe *p = kmalloc(sizeof(*p));
    if (p == NULL) {
        return -ENOMEM;
    }
    memset(p, 0, sizeof(*p));
    p->refs_r = 1;
    p->refs_w = 1;

    struct vfs_file *r = kmalloc(sizeof(*r));
    struct vfs_file *w = kmalloc(sizeof(*w));
    if (r == NULL || w == NULL) {
        kfree(r);
        kfree(w);
        kfree(p);
        return -ENOMEM;
    }
    r->node = NULL;
    r->pipe = p;
    r->sock = NULL;
    r->inet_sock = NULL;
    r->socket_domain = 0;
    r->pos = 0;
    r->readdir_node = NULL;
    r->readdir_index = 0;
    r->flags = O_RDONLY;
    r->refs = 1;
    w->node = NULL;
    w->pipe = p;
    w->sock = NULL;
    w->inet_sock = NULL;
    w->socket_domain = 0;
    w->pos = 0;
    w->readdir_node = NULL;
    w->readdir_index = 0;
    w->flags = O_WRONLY;
    w->refs = 1;

    long a = fd_alloc(r);
    long b = fd_alloc(w);
    if (a < 0 || b < 0) {
        if (a >= 0) {
            tbl[a] = NULL;
        }
        if (b >= 0) {
            tbl[b] = NULL;
        }
        kfree(r);
        kfree(w);
        kfree(p);
        return -ENOMEM;
    }
    fds[0] = (int)a;
    fds[1] = (int)b;
    return 0;
}

/* ---- sockets ----
 *
 * socket.c never sees struct vfs_file; it hands a socket here and gets
 * an fd back. The caller's reference is transferred to the new fd. */

long vfs_sock_install(struct unix_sock *s) {
    if (s == NULL) {
        return -EINVAL;
    }
    struct vfs_file *f = kmalloc(sizeof(*f));
    if (f == NULL) {
        unix_sock_unref(s);
        return -ENOMEM;
    }
    f->node = NULL;
    f->pipe = NULL;
    f->sock = s;
    f->inet_sock = NULL;
    f->socket_domain = AF_UNIX;
    f->pos = 0;
    f->readdir_node = NULL;
    f->readdir_index = 0;
    f->flags = O_RDWR;
    f->refs = 1;

    long fd = fd_alloc(f);
    if (fd < 0) {
        kfree(f);
        unix_sock_unref(s);
        return fd;
    }
    return fd;
}

long vfs_inet_sock_install(struct inet_sock *s) {
    if (s == NULL) {
        return -EINVAL;
    }
    struct vfs_file *f = kmalloc(sizeof(*f));
    if (f == NULL) {
        inet_sock_unref(s);
        return -ENOMEM;
    }
    f->node = NULL;
    f->pipe = NULL;
    f->sock = NULL;
    f->inet_sock = s;
    f->socket_domain = AF_INET;
    f->pos = 0;
    f->readdir_node = NULL;
    f->readdir_index = 0;
    f->flags = O_RDWR;
    f->refs = 1;

    long fd = fd_alloc(f);
    if (fd < 0) {
        kfree(f);
        inet_sock_unref(s);
        return fd;
    }
    return fd;
}

struct unix_sock *vfs_fd_sock(long fd) {
    struct vfs_file *f = fd_get(fd);
    return (f != NULL && f->socket_domain == AF_UNIX) ? f->sock : NULL;
}

long vfs_fd_flags(long fd) {
    struct vfs_file *f = fd_get(fd);
    return f != NULL ? (long)f->flags : -EBADF;
}

long vfs_fd_set_flags(long fd, long flags) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return -EBADF;
    }
    /* The access mode is fixed at open time; only the status flags
     * move. Keeping the mode bits is what makes a read on an fd that
     * was opened write-only still fail after an F_SETFL. */
    f->flags = (f->flags & 3) | (int)(flags & ~3);
    return 0;
}

struct inet_sock *vfs_fd_inet_sock(long fd) {
    struct vfs_file *f = fd_get(fd);
    return (f != NULL && f->socket_domain == AF_INET) ? f->inet_sock : NULL;
}

/* ---- readiness (poll/select) ---- */

short vfs_poll(long fd, short events) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return POLLNVAL;
    }

    short ready;
    if (f->sock != NULL) {
        ready = unix_sock_poll(f->sock);
    } else if (f->inet_sock != NULL) {
        ready = inet_sock_poll(f->inet_sock);
    } else if (f->pipe != NULL) {
        struct vfs_pipe *p = f->pipe;
        if ((f->flags & 3) == O_WRONLY) {
            /* Write end: ready while the ring has room; a pipe with no
             * readers left reports an error rather than writability,
             * because a write would return -EPIPE. */
            ready = p->refs_r == 0 ? POLLERR
                                   : (p->count < PIPE_BUF_SIZE ? POLLOUT : 0);
        } else {
            /* Read end: buffered data, or EOF once every writer is
             * gone (a read would return 0 without blocking). */
            ready = p->count > 0 ? POLLIN : 0;
            if (p->refs_w == 0) {
                ready |= POLLIN | POLLHUP;
            }
        }
    } else {
        struct vfs_node *node = f->node;
        switch (node->type) {
        case VFS_DEVICE:
            /* A device that can make a reader block must say so; the
             * rest are always ready (see struct file_ops). */
            ready = (node->ops != NULL && node->ops->poll != NULL)
                        ? node->ops->poll(node->priv)
                        : (POLLIN | POLLOUT);
            break;
        case VFS_DIR:
            ready = POLLIN; /* readdir never blocks */
            break;
        case VFS_SOCKET:
            /* The filesystem entry is an address, not an endpoint:
             * there is nothing to read from or write to. */
            ready = 0;
            break;
        default:
            ready = POLLIN | POLLOUT; /* regular files never block */
            break;
        }
    }

    /* poll(2): error conditions are reported whether or not they were
     * requested; ordinary readiness is masked by what the caller asked
     * for. */
    return ready & (events | POLLERR | POLLHUP | POLLNVAL);
}

/* Close every fd in an arbitrary table - the shared body of
 * vfs_close_all() below. Split out so that killing a task OTHER than
 * the current one (kernel/sched/sched.c:task_kill) can close its
 * table directly, the same way task_exit closes its own. */
void vfs_close_all_table(struct vfs_file **tbl) {
    if (tbl == NULL) {
        return;
    }
    for (int i = 0; i < VFS_MAX_FDS; i++) {
        if (tbl[i] != NULL) {
            file_unref(tbl[i]);
            tbl[i] = NULL;
        }
    }
}

/* Close every fd of the current task (called from task_exit; the
 * exiting task's whole table goes away, which is what lets a pipe
 * reader observe EOF once its writer exits). */
void vfs_close_all(void) {
    vfs_close_all_table(fd_table());
}

/* Inherit a parent's fd table at task creation (refcounted copy). */
void vfs_fd_inherit(struct vfs_file **dst, struct vfs_file **src) {
    for (int i = 0; i < VFS_MAX_FDS; i++) {
        dst[i] = NULL;
        if (src[i] != NULL) {
            dst[i] = src[i];
            file_ref(src[i]);
        }
    }
}

/* ---- pipes ---- */

static long pipe_read(struct vfs_pipe *p, void *buf, size_t count) {
    while (p->count == 0) {
        if (p->refs_w == 0) {
            return 0; /* EOF: every write end is closed */
        }
        hlt(); /* empty: wait for the writer (the tick preempts us) */
    }
    size_t n = count < p->count ? count : p->count;
    size_t first = PIPE_BUF_SIZE - p->head;
    if (n > first) {
        memcpy(buf, p->buf + p->head, first);
        memcpy((uint8_t *)buf + first, p->buf, n - first);
    } else {
        memcpy(buf, p->buf + p->head, n);
    }
    p->head = (p->head + n) % PIPE_BUF_SIZE;
    p->count -= n;
    return (long)n;
}

static long pipe_write(struct vfs_pipe *p, const void *buf, size_t count) {
    size_t total = 0;
    while (total < count) {
        if (p->refs_r == 0) {
            return total > 0 ? (long)total : -EPIPE;
        }
        if (p->count == PIPE_BUF_SIZE) {
            hlt(); /* full: wait for the reader to drain */
            continue;
        }
        size_t free = PIPE_BUF_SIZE - p->count;
        size_t n = count - total < free ? count - total : free;
        size_t wpos = (p->head + p->count) % PIPE_BUF_SIZE;
        size_t first = PIPE_BUF_SIZE - wpos;
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

static long file_read(struct vfs_file *f, void *buf, size_t count) {
    if (f->node->wrf_ino != 0) {
        long n = wrf_file_read(f->node, buf, count, f->pos);
        if (n > 0) {
            f->pos += (size_t)n;
        }
        return n;
    }
    if (f->pos >= f->node->size) {
        return 0; /* EOF */
    }
    size_t avail = f->node->size - f->pos;
    if (count > avail) {
        count = avail;
    }
    memcpy(buf, f->node->data + f->pos, count);
    f->pos += count;
    return (long)count;
}

static long file_write(struct vfs_file *f, const void *buf, size_t count) {
    if (f->node->wrf_ino != 0) {
        long n = wrf_file_write(f->node, buf, count, f->pos);
        if (n > 0) {
            f->pos += (size_t)n;
        }
        return n;
    }
    size_t need = f->pos + count;
    if (need > f->node->capacity) {
        size_t newcap = f->node->capacity ? f->node->capacity : 64;
        while (newcap < need) {
            newcap *= 2;
        }
        uint8_t *fresh = krealloc(f->node->data, newcap);
        if (fresh == NULL) {
            return -ENOMEM;
        }
        f->node->data = fresh;
        f->node->capacity = newcap;
    }
    memcpy(f->node->data + f->pos, buf, count);
    f->pos += count;
    if (f->pos > f->node->size) {
        f->node->size = f->pos;
    }
    return (long)count;
}

long vfs_ftruncate(long fd, long length) {
    struct vfs_file *f = fd_get(fd);
    /* node == NULL covers both pipes and sockets: neither has a
     * length to truncate. */
    if (f == NULL || f->node == NULL || f->node->type != VFS_FILE) {
        return -EBADF;
    }
    if (length < 0) {
        return -EINVAL;
    }
    struct vfs_node *node = f->node;
    if (node->wrf_ino != 0) {
        return wrf_truncate(node, (size_t)length);
    }
    if ((size_t)length < node->size) {
        node->size = (size_t)length;
        return 0;
    }
    /* Growing: extend with zero bytes (kilo truncates to the new
     * length, then rewrites the whole file from offset 0). */
    size_t need = (size_t)length;
    if (need > node->capacity) {
        size_t newcap = node->capacity ? node->capacity : 64;
        while (newcap < need) {
            newcap *= 2;
        }
        uint8_t *fresh = krealloc(node->data, newcap);
        if (fresh == NULL) {
            return -ENOMEM;
        }
        node->data = fresh;
        node->capacity = newcap;
    }
    if (need > node->size) {
        memset(node->data + node->size, 0, need - node->size);
        node->size = need;
    }
    return 0;
}

long vfs_read(long fd, void *buf, size_t count) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return -EBADF;
    }
    if (buf == NULL) {
        return -EINVAL;
    }
    if ((f->flags & 3) == O_WRONLY) {
        return -EBADF;
    }
    if (f->sock != NULL) {
        return unix_sock_read(f->sock, buf, count);
    }
    if (f->inet_sock != NULL) {
        return inet_sock_read(f->inet_sock, buf, count);
    }
    if (f->pipe != NULL) {
        return pipe_read(f->pipe, buf, count);
    }

    struct vfs_node *node = f->node;
    switch (node->type) {
    case VFS_FILE:
        return file_read(f, buf, count);
    case VFS_DEVICE:
        if (node->ops != NULL && node->ops->read != NULL) {
            long n = node->ops->read(node->priv, buf, count, f->pos);
            if (n > 0) {
                f->pos += (size_t)n;
            }
            return n;
        }
        return -EIO;
    default:
        return -EISDIR;
    }
}

long vfs_pread(long fd, void *buf, size_t count, size_t offset) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return -EBADF;
    }
    if (buf == NULL) {
        return -EINVAL;
    }
    if (f->node == NULL || f->node->type != VFS_FILE) {
        return -EISDIR; /* positioned reads only make sense for files */
    }
    if (f->node->wrf_ino != 0) {
        return wrf_file_read(f->node, buf, count, offset);
    }
    if (offset >= f->node->size) {
        return 0; /* EOF */
    }
    size_t avail = f->node->size - offset;
    if (count > avail) {
        count = avail;
    }
    memcpy(buf, f->node->data + offset, count);
    return (long)count;
}

/* lseek: the position lives in the open file description, so a dup'd
 * descriptor shares it exactly as POSIX requires. Only regular files
 * have a meaningful position - a pipe or a socket is a stream someone
 * else is writing into, and a directory is walked with readdir. */
long vfs_lseek(long fd, long offset, int whence) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return -EBADF;
    }
    /* Regular files, and the devices that are really a span of
     * bytes: a disk (/dev/hda) and the images the bootloader loaded
     * (/dev/kernel, /dev/rootfs). A device with no size is a stream
     * and stays unseekable. */
    if (f->node == NULL ||
        (f->node->type != VFS_FILE &&
         !(f->node->type == VFS_DEVICE && f->node->size > 0))) {
        return -ESPIPE;
    }

    long base;
    switch (whence) {
    case SEEK_SET:
        base = 0;
        break;
    case SEEK_CUR:
        base = (long)f->pos;
        break;
    case SEEK_END:
        base = (long)f->node->size;
        break;
    default:
        return -EINVAL;
    }

    long pos = base + offset;
    if (pos < 0) {
        return -EINVAL;
    }
    f->pos = (size_t)pos;
    return pos;
}

/* fstat(fd): the node's size/mode/type, just enough for musl's
 * fstat() - TUS has no fstatat/stat, so musl always falls back to
 * statx() with AT_EMPTY_PATH, which syscall.c's sys_statx() turns
 * into this call. Returns 0 on success, or a negative errno. */
long vfs_fstat(long fd, size_t *out_size, uint32_t *out_mode, int *out_type, uint64_t *out_ino) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return -EBADF;
    }
    if (f->node == NULL) {
        /* Not every open fd has a vfs_node - a pipe() end is a
         * vfs_pipe instead (see struct vfs_file). Real fstat() on a
         * pipe succeeds (S_ISFIFO, size 0), and something depending
         * on that is not a hypothetical: ksh's interactive/script
         * mode (as opposed to `ksh -c`) calls fstat(0) during its own
         * stdin setup and panics if it fails - found piping a real
         * shell session over sshd's pipe-connected stdio, where fd 0
         * legitimately is a pipe end. */
        if (f->pipe != NULL) {
            *out_size = 0;
            *out_mode = 0600;
            *out_type = VFS_PIPE;
            *out_ino = (uint64_t)(uintptr_t)f->pipe;
            return 0;
        }
        return -EBADF;
    }
    *out_size = f->node->size;
    *out_mode = f->node->mode;
    *out_type = (int)f->node->type;
    /* A stable per-node identity: every vfs_node is allocated once at
     * boot/mount and lives for the system's life (no remounting), so
     * its own address already has exactly the identity semantics
     * stat's st_ino needs - same node, same value; different nodes,
     * different values. Without this every node reported ino 0 (the
     * struct was memset and never filled in), so any two files were
     * indistinguishable by (st_dev, st_ino) - which is exactly the
     * pair libast's sfio uses to recognize /dev/null as a write sink,
     * and it was matching the console tty against it too, silently
     * discarding every builtin's stdout (found porting ksh: `echo`
     * ran and returned 0 but never actually wrote anything). */
    *out_ino = (uint64_t)(uintptr_t)f->node;
    return 0;
}

/* Same fields as vfs_fstat, looked up by path rather than an open fd -
 * for statx() calls that are not the AT_EMPTY_PATH/fd case (a bare
 * stat(), or fstatat() with a real path). Like every other path-taking
 * syscall in TUS (vfs_open_mode, vfs_mkdir, ...), a relative path is
 * resolved against the calling task's cwd inside vfs_lookup() itself
 * (see vfs_path_resolve()). */
long vfs_stat_path(const char *path, size_t *out_size, uint32_t *out_mode, int *out_type, uint64_t *out_ino) {
    struct vfs_node *node = vfs_lookup(path);
    if (node == NULL) {
        return -ENOENT;
    }
    *out_size = node->size;
    *out_mode = node->mode;
    *out_type = (int)node->type;
    *out_ino = (uint64_t)(uintptr_t)node; /* see vfs_fstat()'s comment */
    return 0;
}

/* Reconstruct the absolute path of a node by walking parent pointers
 * to the root. fchdir() has a directory fd but no path string - a
 * vfs_file only remembers the node it was opened on, never the path
 * that reached it - so this rebuilds one from the tree itself, the
 * only place that information still exists. */
static int vfs_node_path(struct vfs_node *node, char *out, size_t outsz) {
    if (node == g_root) {
        if (outsz < 2) {
            return -1;
        }
        out[0] = '/';
        out[1] = '\0';
        return 0;
    }
    const struct vfs_node *chain[64];
    int n = 0;
    const struct vfs_node *cur = node;
    while (cur != NULL && cur != g_root && n < 64) {
        chain[n++] = cur;
        cur = cur->parent;
    }
    if (cur != g_root) {
        return -1; /* deeper than 64 levels, or detached from the tree */
    }
    char *w = out;
    size_t left = outsz;
    for (int i = n - 1; i >= 0 && left > 1; i--) {
        size_t l = strlen(chain[i]->name);
        if (l + 1 >= left) {
            return -1;
        }
        *w++ = '/';
        left--;
        memcpy(w, chain[i]->name, l);
        w += l;
        left -= l;
    }
    if (left < 1) {
        return -1;
    }
    *w = '\0';
    return 0;
}

/* fchdir(fd): resolve the directory fd back to an absolute path via
 * vfs_node_path() above. The caller (syscall.c) copies `out` into the
 * calling task's cwd, same as chdir() does. */
long vfs_fchdir(long fd, char *out, size_t outsz) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL || f->node == NULL) {
        return -EBADF;
    }
    if (f->node->type != VFS_DIR) {
        return -ENOTDIR;
    }
    if (vfs_node_path(f->node, out, outsz) != 0) {
        return -ENAMETOOLONG;
    }
    return 0;
}

/* access(path, mode): F_OK just needs the lookup to succeed; R_OK/
 * W_OK/X_OK reuse vfs_access_ok() (the same permission check open(),
 * unlink(), mkdir() and chmod() already enforce), which - like every
 * check in this file - bypasses entirely for root/ring-0 callers. */
long vfs_access_path(const char *path, int mode) {
    struct vfs_node *node = vfs_lookup(path);
    if (node == NULL) {
        return -ENOENT;
    }
    if (mode != 0 && !vfs_access_ok(node, (uint32_t)mode)) {
        return -EACCES;
    }
    return 0;
}

/* rename(old, new): see vfs.h. Both paths are resolved (and their
 * parents' write permission checked) before anything is unlinked, so
 * a rename that would fail never leaves the tree half-changed. */
long vfs_rename(const char *oldpath, const char *newpath) {
    if (oldpath == NULL || newpath == NULL) {
        return -EINVAL;
    }
    struct vfs_node *node = vfs_lookup(oldpath);
    if (node == NULL) {
        return -ENOENT;
    }

    char old_dir[VFS_PATH_MAX];
    char old_name[VFS_NAME_MAX];
    if (path_split(oldpath, old_dir, sizeof(old_dir), old_name, sizeof(old_name)) != 0) {
        return -EINVAL;
    }
    struct vfs_node *old_parent = vfs_lookup(old_dir);
    if (old_parent == NULL || old_parent->type != VFS_DIR) {
        return -ENOENT;
    }
    if (!vfs_access_ok(old_parent, VFS_W_OK)) {
        return -EACCES;
    }

    char new_dir[VFS_PATH_MAX];
    char new_name[VFS_NAME_MAX];
    if (path_split(newpath, new_dir, sizeof(new_dir), new_name, sizeof(new_name)) != 0) {
        return -EINVAL;
    }
    struct vfs_node *new_parent = vfs_lookup(new_dir);
    if (new_parent == NULL || new_parent->type != VFS_DIR) {
        return -ENOENT;
    }
    if (!vfs_access_ok(new_parent, VFS_W_OK)) {
        return -EACCES;
    }

    struct vfs_node *existing = dir_find(new_parent, new_name);
    if (existing != NULL && existing != node) {
        if (existing->type == VFS_DIR && existing->child != NULL) {
            return -ENOTEMPTY;
        }
        /* Detach and free the destination the same way vfs_remove()
         * does - an open fd on it survives via the same unlinked
         * flag, freed on last close. */
        struct vfs_node **link = &new_parent->child;
        while (*link != NULL) {
            if (*link == existing) {
                *link = existing->sibling;
                break;
            }
            link = &(*link)->sibling;
        }
        /* WRF: same two-step cleanup vfs_remove() does - drop the
         * directory entry now, free the inode/blocks now if nothing
         * has it open, or leave that to file_unref() on last close. */
        wrf_notify_detach(new_parent, existing);
        if (existing->open_refs > 0) {
            existing->unlinked = true;
        } else {
            wrf_notify_free(existing);
            if (existing->type == VFS_FILE) {
                kfree(existing->data);
            }
            kfree(existing);
        }
    }

    /* Detach the source node from its old parent, rename it, and
     * attach it under the destination parent - the same node, so any
     * fd already open on it keeps working. */
    struct vfs_node **link = &old_parent->child;
    while (*link != NULL) {
        if (*link == node) {
            *link = node->sibling;
            break;
        }
        link = &(*link)->sibling;
    }
    /* WRF: remove the old directory entry (the node's inode is not
     * freed - it is moving, not being deleted) while node->name is
     * still the OLD name this dirent was filed under. */
    wrf_notify_detach(old_parent, node);

    size_t nlen = strlen(new_name);
    if (nlen >= VFS_NAME_MAX) {
        return -ENAMETOOLONG;
    }
    memcpy(node->name, new_name, nlen + 1);
    dir_attach(new_parent, node);
    /* WRF: add a directory entry for the (unchanged) inode under its
     * new name/parent. Only fires when node was already WRF-backed;
     * moving a plain ramfs node into a WRF directory (or vice versa)
     * still works in memory but is not itself persisted - a known v1
     * limitation, not a correctness bug (see wrf_notify_attach()'s
     * no-op guard). */
    if (new_parent->wrf_ino != 0 && node->wrf_ino != 0) {
        wrf_notify_attach(new_parent, node);
    }
    return 0;
}

long vfs_write(long fd, const void *buf, size_t count) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return -EBADF;
    }
    if (buf == NULL) {
        return -EINVAL;
    }
    if ((f->flags & 3) == O_RDONLY) {
        return -EBADF;
    }
    if (f->sock != NULL) {
        return unix_sock_write(f->sock, buf, count);
    }
    if (f->inet_sock != NULL) {
        return inet_sock_write(f->inet_sock, buf, count);
    }
    if (f->pipe != NULL) {
        return pipe_write(f->pipe, buf, count);
    }

    struct vfs_node *node = f->node;
    switch (node->type) {
    case VFS_FILE:
        return file_write(f, buf, count);
    case VFS_DEVICE:
        if (node->ops != NULL && node->ops->write != NULL) {
            long n = node->ops->write(node->priv, buf, count, f->pos);
            if (n > 0) {
                f->pos += (size_t)n;
            }
            return n;
        }
        return -EIO;
    default:
        return -EISDIR;
    }
}

long vfs_ioctl(long fd, uint64_t request, void *arg) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return -EBADF;
    }
    if (f->node == NULL || f->node->type != VFS_DEVICE ||
        f->node->ops == NULL || f->node->ops->ioctl == NULL) {
        return -ENOTTY;
    }
    return f->node->ops->ioctl(f->node->priv, request, arg);
}

long vfs_readdir(long fd, void *buf, size_t count) {
    struct vfs_file *f = fd_get(fd);
    if (f == NULL) {
        return -EBADF;
    }
    if (f->node == NULL || f->node->type != VFS_DIR) {
        return -ENOTDIR;
    }
    if (buf == NULL) {
        return -EINVAL;
    }

    size_t written = 0;
    struct vfs_node *n;
    if (f->pos == 0) {
        n = f->node->child;
    } else if (f->pos == f->readdir_index) {
        n = f->readdir_node;
    } else {
        n = f->node->child;
        size_t index = 0;
        while (n != NULL && index < f->pos) {
            n = n->sibling;
            index++;
        }
    }

    while (n != NULL && written + sizeof(struct vfs_dirent) <= count) {
        struct vfs_dirent *d = (struct vfs_dirent *)((uint8_t *)buf + written);
        memset(d, 0, sizeof(*d));
        memcpy(d->name, n->name, strlen(n->name) + 1);
        d->type = n->type;
        d->size = (uint32_t)n->size;
        d->mode = n->mode;
        written += sizeof(struct vfs_dirent);
        f->pos++;
        n = n->sibling;
    }
    f->readdir_node = n;
    f->readdir_index = f->pos;
    return (long)written;
}

long vfs_mkdir(const char *path, uint32_t mode) {
    if (path == NULL) {
        return -EINVAL;
    }
    struct vfs_node *existing = vfs_lookup(path);
    if (existing != NULL) {
        return -EEXIST;
    }
    struct vfs_node *parent = vfs_parent_of(path);
    if (parent != NULL && !vfs_access_ok(parent, VFS_W_OK)) {
        return -EACCES;
    }
    struct vfs_node *dir = vfs_create_dir(path);
    if (dir == NULL) {
        return -ENOENT;
    }
    if (mode != 0) {
        dir->mode = mode;
    }
    struct task *cur = sched_current();
    if (cur != NULL) {
        dir->uid = cur->euid;
        dir->gid = cur->egid;
    }
    return 0;
}

long vfs_unlink(const char *path) {
    if (path == NULL) {
        return -EINVAL;
    }
    if (vfs_lookup(path) == NULL) {
        return -ENOENT;
    }
    struct vfs_node *parent = vfs_parent_of(path);
    if (parent != NULL && !vfs_access_ok(parent, VFS_W_OK)) {
        return -EACCES;
    }
    if (vfs_remove(path) != 0) {
        return -EISDIR; /* non-empty directory or removal failure */
    }
    return 0;
}

long vfs_chmod(const char *path, uint32_t mode) {
    if (path == NULL) {
        return -EINVAL;
    }
    struct vfs_node *node = vfs_lookup(path);
    if (node == NULL) {
        return -ENOENT;
    }
    struct task *cur = sched_current();
    if (cur != NULL && cur->euid != 0 && cur->euid != node->uid) {
        return -EPERM; /* only the owner (or root) may change a mode */
    }
    node->mode = mode;
    return 0;
}

/* ---- tree construction ---- */

/* Ensure a base directory exists. This is the safety net for a boot
 * without a rootfs module: normally /dev, /tmp, /etc and /boot all
 * come from rootfs.img (see kernel/vfs/rootfs.c), and this does
 * nothing. Only when the module is missing are the directories
 * recreated here so the system can still boot (serial-only). */
static void ensure_dir(const char *path) {
    if (vfs_lookup(path) == NULL) {
        vfs_create_dir(path);
    }
}

void vfs_init(void) {
    g_root = kmalloc(sizeof(*g_root));
    if (g_root == NULL) {
        return; /* out of memory at boot: VFS stays unusable */
    }
    memset(g_root, 0, sizeof(*g_root));
    g_root->type = VFS_DIR;
    memcpy(g_root->name, "/", 2);
}

/* Second init stage, run AFTER the rootfs module is mounted: the
 * directory tree (/dev, /tmp, /etc, /boot) comes from rootfs.img,
 * then the built-in device nodes are registered and the standard
 * descriptors are wired to the console. */
void vfs_devices_init(void) {
    /* Fallback only: normally these directories already exist
     * because rootfs.img provided them. */
    ensure_dir("/dev");
    ensure_dir("/tmp");
    ensure_dir("/etc");
    ensure_dir("/boot");

    /* /tmp is the one directory every account, not just root, is
     * expected to create and remove its own files in - real Unix
     * ships it 01777 (world rwx + the sticky bit, so nobody can
     * delete someone else's file there) for exactly that reason.
     * Every OTHER directory gets the generic 0755 a fresh vfs_node
     * starts at (see vfs_create_dir()), owned by root, which is
     * correct there but wrong for /tmp specifically - and since /tmp
     * is normally populated from the rootfs.img tar rather than
     * ensure_dir() above, whatever mode happened to survive tar
     * packing (a plain `mkdir -p` on the build host, with no
     * guaranteed umask) is not something to rely on. Fix it up here,
     * unconditionally, regardless of which path created the node.
     * Nothing enforced this at all until vfs_access_ok() started
     * actually gating non-root writes - before that every session
     * ran as root and a wrong mode on /tmp was invisible. */
    struct vfs_node *tmp_dir = vfs_lookup("/tmp");
    if (tmp_dir != NULL) {
        tmp_dir->mode = 01777;
    }

    /* Populate /dev with the built-in devices. */
    devices_init();

    /* A handful of synthetic /proc files for ported Linux software
     * that reads them directly (see procfs.c). */
    procfs_init();

    /* Standard descriptors: stdin/stdout/stderr on the console. */
    struct vfs_node *tty0 = vfs_lookup("/dev/tty0");
    if (tty0 != NULL) {
        for (int i = 0; i < 3; i++) {
            struct vfs_file *f = kmalloc(sizeof(*f));
            if (f == NULL) {
                continue;
            }
            f->node = tty0;
            f->pipe = NULL;
            f->sock = NULL;
            f->inet_sock = NULL;
            f->socket_domain = 0;
            f->pos = 0;
            f->readdir_node = NULL;
            f->readdir_index = 0;
            f->flags = O_RDWR;
            f->refs = 1;
            struct vfs_file **tbl = fd_table();
            if (tbl == NULL) {
                kfree(f);
                continue;
            }
            tbl[i] = f;
        }
    }
}
