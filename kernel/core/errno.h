/*
 * errno.h - kernel error codes (POSIX-style)
 *
 * Syscalls and VFS operations return a negative errno on failure,
 * mirroring the UNIX convention. Values match the classic errno.h.
 */

#ifndef TUS_CORE_ERRNO_H
#define TUS_CORE_ERRNO_H

#define EPERM   1  /* operation not permitted */
#define ENOENT  2  /* no such file or directory */
#define ESRCH   3  /* no such process (kill/waitpid target) */
#define EINTR   4  /* interrupted by a signal before completing */
#define EIO     5  /* I/O error */
#define EBADF   9  /* bad file descriptor */
#define EFAULT  14 /* bad address (user pointer outside user space) */
#define EAGAIN  11 /* resource temporarily unavailable */
#define ENOMEM  12 /* out of memory */
#define EBUSY   16 /* resource busy (e.g. the pointer is already grabbed) */
#define EEXIST  17 /* file exists */
#define ENOTDIR 20 /* not a directory */
#define EISDIR  21 /* is a directory */
#define EINVAL  22 /* invalid argument */
#define ERANGE  34 /* result too large for the buffer given */
#define EACCES  13 /* permission denied */
#define ENOTTY  25 /* inappropriate ioctl for device */
#define ENOSPC  28 /* no space left on device */
#define ESPIPE  29 /* illegal seek (pipe, socket or directory) */
#define ENODEV  19 /* no such device (e.g. no framebuffer for highX) */
#define ENOEXEC 8  /* exec format error */
#define EPIPE   32 /* broken pipe: write to a pipe with no readers */
#define ENAMETOOLONG 36 /* path longer than the buffer allows */
#define ENOSYS  38 /* function not implemented */
#define ENOTEMPTY 39 /* rename()/rmdir() target directory has entries */
#define EMFILE  24 /* too many open: every terminal session is taken */

/* Socket errors (Linux numbering, which is what musl maps back to
 * strerror strings). */
#define ENOTSOCK        88 /* fd is not a socket */
#define EDESTADDRREQ    89 /* destination address required */
#define EOPNOTSUPP      95 /* operation not supported on this socket */
#define EAFNOSUPPORT    97 /* address family not supported */
#define EADDRINUSE      98 /* address already bound */
#define ECONNRESET     104 /* connection reset by peer */
#define EISCONN        106 /* socket is already connected */
#define ENOTCONN       107 /* socket is not connected */
#define ECONNREFUSED   111 /* nothing listening on that address */
#define EPROTONOSUPPORT 93 /* protocol not supported */
#define ENETDOWN       100 /* the interface is not up */
#define ENETUNREACH    101 /* no route to that network */
#define ETIMEDOUT      110 /* the peer never answered */
#define EHOSTUNREACH   113 /* nobody answered ARP for the next hop */
#define EMSGSIZE        90 /* datagram larger than the MTU allows */
#define ECHILD          10 /* no such child to wait for */

#endif /* TUS_CORE_ERRNO_H */
