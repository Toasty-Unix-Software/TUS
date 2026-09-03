/*
 * vfs.h - Virtual File System
 *
 * A small in-memory UNIX-like filesystem ("ramfs"): a tree of nodes
 * rooted at '/'. Nodes are directories, regular files (data held in
 * kernel memory) or device nodes (backed by a driver's file_ops).
 *
 * Access goes through file descriptors, exactly like a real UNIX
 * kernel: open() returns an fd, read/write/ioctl operate on it.
 * fds 0, 1 and 2 are pre-opened on /dev/tty0 (stdin/stdout/stderr).
 *
 * NOTE: paths are absolute ("/dev/fb0"); the shell (cmd_fs.c)
 * resolves relative paths against its working directory before
 * calling into the VFS.
 */

#ifndef TUS_VFS_VFS_H
#define TUS_VFS_VFS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Node types. */
#define VFS_DIR    1
#define VFS_FILE   2
#define VFS_DEVICE 3
#define VFS_SOCKET 4 /* bound AF_UNIX socket (see kernel/net/socket.c) */
#define VFS_PIPE   5 /* anonymous pipe() fd - see vfs_fstat()'s comment */

/* poll()/select() event bits (Linux values, matching musl's poll.h).
 * vfs_poll() reports these for any fd; POLLERR/POLLHUP/POLLNVAL are
 * always reported even when the caller did not ask for them. */
#define POLLIN   0x001
#define POLLPRI  0x002
#define POLLOUT  0x004
#define POLLERR  0x008
#define POLLHUP  0x010
#define POLLNVAL 0x020

/* open() flags (POSIX-ish subset). */
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR   2
#define O_CREAT  0x40
#define O_TRUNC  0x200
#define O_APPEND 0x400

/* lseek() whence values (POSIX). */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define VFS_NAME_MAX 64
/* 16 was enough when the busiest program was a shell with a pipeline.
 * sshd holds a listening socket, a connection, three pipes per session
 * and does it for several sessions at once. */
#define VFS_MAX_FDS  64

/* Directory entry as returned by readdir(). */
struct vfs_dirent {
    char name[VFS_NAME_MAX];
    uint32_t type;
    uint32_t size;
    uint32_t mode; /* permission bits (for ls -l) */
};

/* An open file description (opaque; see vfs.c). Each fd table slot
 * points at one; dup/dup2 share the same one (and therefore the same
 * file position, like POSIX). Pipes are represented by a vfs_file
 * whose `pipe` field is set instead of `node`. */
struct vfs_file;

/* Device driver interface. `pos` is the fd's current position.
 * `poll` reports POLL* readiness bits; a device that leaves it NULL is
 * treated as always ready for both reading and writing (true for the
 * framebuffer, /dev/null and /dev/zero, false for anything that can
 * make a reader block - those must implement it). */
struct file_ops {
    long (*read)(void *priv, void *buf, size_t count, size_t pos);
    long (*write)(void *priv, const void *buf, size_t count, size_t pos);
    int  (*ioctl)(void *priv, uint64_t request, void *arg);
    short (*poll)(void *priv);
    /* Optional: the last fd referencing this device node just closed.
     * Only pty.c uses this today, to free a master/slave slot back to
     * the pool; every other device leaves it NULL. */
    void (*close)(void *priv);
};

struct vfs_node {
    char name[VFS_NAME_MAX];
    uint32_t type;

    /* Permission bits (rwxrwxrwx + setuid/setgid/sticky, tar-style).
     * Files created by the kernel default to 0644, directories and
     * devices to 0755/0600; the rootfs tar supplies the real modes
     * (e.g. 4555 for doas and passwd). The x bit gates exec, the
     * setuid bit is reported by ls -l and honoured by userspace
     * tools (no kernel privilege model exists yet). */
    uint32_t mode;
    uint32_t uid;
    uint32_t gid;

    /* Regular file contents. */
    uint8_t *data;
    size_t size;
    size_t capacity;

    /* Device backing. */
    const struct file_ops *ops;
    void *priv;

    struct vfs_node *parent;
    struct vfs_node *sibling; /* next entry in the parent */
    struct vfs_node *child;   /* first entry inside a directory */

    /* How many open vfs_files point at this node, and whether it has
     * been unlink()ed while some of them still do (musl's tmpfile()
     * relies on this: open, unlink, keep using the fd - the classic
     * "delete on last close" idiom). vfs_remove() only frees the node
     * once both open_refs is 0 and it has actually been unlinked. */
    int open_refs;
    bool unlinked;

    /* Nonzero when this node is backed by a mounted WRF filesystem
     * (kernel/fs/wrf.c) instead of living purely in kernel memory: the
     * node's inode number on that disk. A directory with this set
     * persists new children (see dir_attach() and vfs_remove() in
     * vfs.c); a file with this set routes its actual data through
     * wrf_file_read()/wrf_file_write() instead of `data`/`capacity`
     * above, which stay unused for it. Zero (the memset() default
     * every node is born with) means "an ordinary ramfs node," true
     * for everything outside a WRF mount. */
    uint32_t wrf_ino;
};

/* Build the root node. The directory tree is mounted afterwards
 * from rootfs.img (vfs_mount_rootfs). */
void vfs_init(void);

/* Register the built-in device nodes and wire stdin/stdout/stderr.
 * Must run after the rootfs mount (the base directories come from
 * the image; they are recreated here only as a fallback). */
void vfs_devices_init(void);

/* ---- node creation ---- */

struct vfs_node *vfs_create_dir(const char *path);
struct vfs_node *vfs_create_file(const char *path);
struct vfs_node *vfs_create_device(const char *path,
                                   const struct file_ops *ops, void *priv);

/* Create the filesystem entry for a bound AF_UNIX socket. `sock` is
 * the struct unix_sock the node points at; it is cleared when that
 * socket is closed, so a connect() to a stale path is refused. */
struct vfs_node *vfs_create_socket(const char *path, void *sock);

int vfs_remove(const char *path);

/* ---- fd-based API (what the syscalls call) ---- */

/* Create a pipe: fds[0] is the read end, fds[1] the write end. Reads
 * block (hlt) until data arrives or the last write end is closed
 * (EOF); writes block while the 4 KiB buffer is full and return
 * -EPIPE once no read end is open. The fds live in the calling
 * task's table and are inherited by its children. */
long vfs_pipe(int fds[2]);

/* POSIX dup: duplicate `oldfd` onto the lowest free slot. */
long vfs_dup(long oldfd);

/* fcntl(fd, F_DUPFD/F_DUPFD_CLOEXEC, minfd): dup(2) landing on the
 * lowest free descriptor >= minfd. See the comment on its definition
 * in vfs.c. */
long vfs_fcntl_dupfd(long oldfd, long minfd);

/* POSIX dup2: make `newfd` a copy of `oldfd`, closing any file
 * currently on `newfd`. Returns newfd (>= 0) or a negative errno. */
long vfs_dup2(long oldfd, long newfd);

/* Copy a whole fd table into another (called at task creation so a
 * child inherits its parent's open files, refcounted). */
void vfs_fd_inherit(struct vfs_file **dst, struct vfs_file **src);

/* ---- sockets ----
 *
 * struct unix_sock is opaque here; socket.c owns it. These two calls
 * are the only bridge between the socket layer and the fd table, so
 * struct vfs_file stays private to vfs.c. */

#define AF_UNIX 1
#define AF_INET 2

struct unix_sock;
struct inet_sock;

/* Install an already-created socket into the current task's fd table,
 * taking ownership of the caller's reference. Returns the fd or a
 * negative errno (the reference is released on failure). */
long vfs_sock_install(struct unix_sock *s);
long vfs_inet_sock_install(struct inet_sock *s);

/* The open-file flags behind an fd (F_GETFL), or a negative errno. */
long vfs_fd_flags(long fd);

/* Replace the changeable flags (F_SETFL); only O_NONBLOCK and O_APPEND
 * are settable after open, as POSIX requires. */
long vfs_fd_set_flags(long fd, long flags);

/* The socket behind an fd, or NULL when the fd is not a socket. */
struct unix_sock *vfs_fd_sock(long fd);
struct inet_sock *vfs_fd_inet_sock(long fd);

/* Readiness of `fd` for the requested `events`, as POLL* bits. Returns
 * POLLNVAL for a closed fd; POLLERR/POLLHUP are reported regardless of
 * what was requested, exactly like poll(2). */
short vfs_poll(long fd, short events);

long vfs_open(const char *path, int flags);

/* open(path, flags, mode): applies mode when O_CREAT actually creates
 * the file (vfs_open() itself always creates at 0644). */
long vfs_open_mode(const char *path, int flags, uint32_t mode);
long vfs_close(long fd);
long vfs_read(long fd, void *buf, size_t count);
long vfs_write(long fd, const void *buf, size_t count);
long vfs_ioctl(long fd, uint64_t request, void *arg);
long vfs_ftruncate(long fd, long length);
long vfs_readdir(long fd, void *buf, size_t count);
long vfs_mkdir(const char *path, uint32_t mode);
long vfs_unlink(const char *path);

/* Close every fd in the CURRENT task's table (called from task_exit). */
void vfs_close_all(void);

/* Close every fd in an explicit table (a task other than the current
 * one - see task_kill() in kernel/sched/sched.c). */
void vfs_close_all_table(struct vfs_file **tbl);

/* Read from a file at an explicit offset, without moving the fd's
 * position (used by the ELF loader). Returns bytes read or -errno. */
long vfs_pread(long fd, void *buf, size_t count, size_t offset);

/* lseek(fd, offset, whence): move the read/write position of a
 * regular file. SEEK_SET/CUR/END (0/1/2). Pipes, sockets and
 * directories are not seekable (-ESPIPE); seeking past the end is
 * allowed, as on UNIX, and reads there return EOF. Returns the new
 * position or a negative errno. */
long vfs_lseek(long fd, long offset, int whence);

/* fstat(fd): fills out_size, out_mode and out_type (VFS_FILE, VFS_DIR
 * or VFS_DEVICE) from the fd's node. Returns 0, or a negative errno. */
long vfs_fstat(long fd, size_t *out_size, uint32_t *out_mode, int *out_type, uint64_t *out_ino);
long vfs_stat_path(const char *path, size_t *out_size, uint32_t *out_mode, int *out_type, uint64_t *out_ino);


/* Change the permission bits of a node (chmod). */
long vfs_chmod(const char *path, uint32_t mode);
long vfs_chown(const char *path, uint32_t uid, uint32_t gid);

/* Return the node for a path, or NULL. A path that does not start
 * with '/' is resolved against the calling task's cwd first (see
 * vfs_path_resolve() below) - the single choke point every other
 * path-taking function in this file goes through vfs_lookup() for,
 * so relative-path support applies everywhere at once. */
struct vfs_node *vfs_lookup(const char *path);

/* Longest normalized path handed to a path-taking syscall (matches
 * the existing PATH_BUF convention in kernel/shell/cmd_fs.c and the
 * dir_path[256] locals throughout this file). */
#define VFS_PATH_MAX 256

/* Resolve `in` (absolute or relative) against `cwd` into `out`:
 * collapses duplicate slashes, honors "." and ".." (".." at the root
 * stays at the root). `out` always receives a normalized absolute
 * path. This is kernel/shell/cmd_fs.c's own path_resolve(), moved
 * here and parameterized on the cwd to resolve against (that file's
 * cwd_buf(), a per-terminal-session string) so vfs_lookup() can reuse
 * the exact same algorithm against a task's cwd field instead. */
void vfs_path_resolve(const char *cwd, const char *in, char *out, size_t outsz);

/* access(path, mode): mode is F_OK(0) or an OR of R_OK(4)/W_OK(2)/
 * X_OK(1) - the same numeric values POSIX defines, reused directly as
 * the `want` bits vfs_access_ok() already checks internally. */
#define VFS_X_OK 1
long vfs_access_path(const char *path, int mode);

/* fchdir(fd): rebuilds an absolute path from a directory fd (a
 * vfs_file only remembers its node, not the path that opened it) so
 * the syscall can copy it into the calling task's cwd the same way
 * chdir() does. Returns 0 and fills `out`, or a negative errno. */
long vfs_fchdir(long fd, char *out, size_t outsz);

/* rename(old, new): relinks the existing node under the destination's
 * parent with the destination's name - the same node, so open fds
 * survive, matching every other TUS node-table operation. Replaces an
 * existing destination file the same way vfs_remove() would; refuses
 * to replace a non-empty destination directory. */
long vfs_rename(const char *oldpath, const char *newpath);

#endif /* TUS_VFS_VFS_H */
