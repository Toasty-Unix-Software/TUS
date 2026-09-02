/*
 * pty.h - pseudo-terminal devices (/dev/ptmx, /dev/pts/N)
 *
 * A fixed pool of pty pairs. Opening /dev/ptmx hands back a master
 * fd bound to a free slot; the slave side is a pre-created device
 * node at /dev/pts/<slot>. Two ring buffers move bytes between them
 * (master write -> slave read, slave write -> master read), the same
 * blocking discipline as a pipe (kernel/vfs/vfs.c pipe_read/write).
 *
 * This is a real byte-stream PTY, independent of the SYS_TERM
 * session mechanism in kernel/term/term.c (which binds a terminal to
 * a task, not a file descriptor). ksh's `pty` builtin and anything
 * else that expects POSIX openpty()/ptsname() semantics goes through
 * this instead.
 */

#ifndef TUS_VFS_PTY_H
#define TUS_VFS_PTY_H

/* Register /dev/ptmx and the /dev/pts/N slave nodes. Called from
 * devices_init(). */
void pty_init(void);

/* vfs_open_mode() special-cases "/dev/ptmx": returns the vfs_node for
 * a freshly allocated master slot, or NULL if the pool is exhausted
 * (-EMFILE to the caller). */
struct vfs_node *pty_alloc_master(void);

#endif /* TUS_VFS_PTY_H */
