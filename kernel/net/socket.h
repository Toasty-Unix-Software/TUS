/*
 * socket.h - Unix domain sockets (AF_UNIX, SOCK_STREAM)
 *
 * A local IPC endpoint pair. Like a pipe, but bidirectional and
 * addressable: a server binds a socket to a filesystem path, listens,
 * and accepts connections; clients connect to that path. Both ends
 * then read()/write() through their fd exactly like a pipe.
 *
 * Every socket owns the buffer it RECEIVES into; a write copies into
 * the peer's receive buffer. That is what makes the channel
 * bidirectional with one 4 KiB ring per direction.
 *
 * Bound sockets appear in the VFS as VFS_SOCKET nodes (`ls -l` shows
 * them with an 's' type character). Following UNIX, closing a bound
 * socket does NOT remove the node - the path lingers until someone
 * unlink()s it, and a connect() to a stale path fails with
 * -ECONNREFUSED.
 *
 * Scope: SOCK_STREAM only, blocking only. There is no SOCK_DGRAM, no
 * fd passing (SCM_RIGHTS) and no O_NONBLOCK; each of those is
 * rejected at the ABI rather than silently faked.
 */

#ifndef TUS_NET_SOCKET_H
#define TUS_NET_SOCKET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Address family and socket type (Linux values; musl agrees). */
#define AF_UNIX     1
#define AF_LOCAL    1
#define AF_INET     2
#define SOCK_STREAM 1
#define SOCK_DGRAM  2

/* Bits musl may OR into the `type` argument of socket(). */
#define SOCK_CLOEXEC  0x80000
#define SOCK_NONBLOCK 0x800

/* shutdown() directions. */
#define SHUT_RD   0
#define SHUT_WR   1
#define SHUT_RDWR 2

/* Protocol numbers */
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

/* struct sockaddr_un carries 108 path bytes (musl's include/sys/un.h). */
#define UNIX_PATH_MAX 108

/* Userspace address layout, byte-identical to musl's sockaddr_un. */
struct sockaddr_un {
    uint16_t sun_family;
    char sun_path[UNIX_PATH_MAX];
};

/* sockaddr_in for AF_INET (IPv4) */
struct sockaddr_in {
    uint16_t sin_family;
    uint16_t sin_port;
    uint32_t sin_addr;
    uint8_t sin_zero[8];
};

struct unix_sock;
struct inet_sock;

/* Generic socket creation for both AF_UNIX and AF_INET. Returns a
 * pointer that can be cast to the appropriate socket type, or NULL
 * with *err set to a negative errno. */
void *socket_create(int domain, int type, int protocol, long *err);

/* socket(domain, type, protocol). Returns the new socket with one
 * reference, or NULL with *err set to a negative errno. */
struct unix_sock *unix_sock_create(int domain, int type, int protocol,
                                   long *err);

/* AF_INET socket operations. The protocol state lives in the TCP and
 * UDP PCBs; struct inet_sock is only what an fd points at. */
struct inet_sock *inet_sock_create(int type, int protocol, long *err);
long inet_sock_bind(struct inet_sock *s, uint32_t ip, uint16_t port);
long inet_sock_connect(struct inet_sock *s, uint32_t dst_ip, uint16_t dst_port);
long inet_sock_listen(struct inet_sock *s, int backlog);
struct inet_sock *inet_sock_accept(struct inet_sock *s, long *err);

/* read()/write() on a connected socket. */
long inet_sock_read(struct inet_sock *s, void *buf, size_t len);
long inet_sock_write(struct inet_sock *s, const void *buf, size_t len);

/* Datagram forms; on a stream socket these fall back to read/write. */
long inet_sock_sendto(struct inet_sock *s, const void *buf, size_t len,
                      uint32_t dst_ip, uint16_t dst_port);
long inet_sock_recvfrom(struct inet_sock *s, void *buf, size_t len,
                        uint32_t *src_ip, uint16_t *src_port,
                        uint32_t timeout_ms);

long inet_sock_shutdown(struct inet_sock *s, int how);

/* getsockname()/getpeername(): `peer` selects which end. */
long inet_sock_getname(struct inet_sock *s, uint32_t *ip, uint16_t *port,
                       bool peer);

short inet_sock_poll(struct inet_sock *s);
void inet_sock_set_nonblock(struct inet_sock *s, bool on);
bool inet_sock_is_stream(struct inet_sock *s);

void inet_sock_ref(struct inet_sock *s);
void inet_sock_unref(struct inet_sock *s);

/* bind(): claim `path` in the VFS namespace for this socket. */
long unix_sock_bind(struct unix_sock *s, const char *path);

/* listen(): mark a bound socket as accepting, sizing its backlog. */
long unix_sock_listen(struct unix_sock *s, int backlog);

/* accept(): pop the oldest queued connection, blocking until one
 * arrives. Returns the server-side socket (one reference) or NULL
 * with *err set. */
struct unix_sock *unix_sock_accept(struct unix_sock *s, long *err);

/* connect(): attach to the listener bound at `path`. Like UNIX, this
 * returns as soon as the connection is queued on the listener's
 * backlog - the server need not have called accept() yet. */
long unix_sock_connect(struct unix_sock *s, const char *path);

/* socketpair(): two already-connected, unnamed sockets. */
long unix_sock_pair(struct unix_sock **a, struct unix_sock **b);

/* read/write on a connected socket (what recv/send become). */
long unix_sock_read(struct unix_sock *s, void *buf, size_t count);
long unix_sock_write(struct unix_sock *s, const void *buf, size_t count);

/* shutdown(how): stop reading, writing or both on this end. */
long unix_sock_shutdown(struct unix_sock *s, int how);

/* getsockname(): copy the bound path (empty string when unbound). */
long unix_sock_getname(struct unix_sock *s, char *out, size_t size);

/* Readiness for poll()/select(); returns POLL* bits (see vfs.h). */
short unix_sock_poll(struct unix_sock *s);

/* Reference counting (one reference per fd table slot). */
void unix_sock_ref(struct unix_sock *s);
void unix_sock_unref(struct unix_sock *s);

#endif /* TUS_NET_SOCKET_H */
