/*
 * cap.h - a small POSIX-capabilities-style bitmask
 *
 * TUS's privilege model was previously all-or-nothing: euid == 0 or
 * not (see sched.h's comment on struct task). That is still true for
 * everything not listed here - this adds a narrow set of capability
 * bits that let a *non-root* task be granted one specific privileged
 * operation (by a root task, via SYS_CAPSET) instead of needing full
 * root. Root (euid == 0) implicitly holds every bit; has_cap() is the
 * only thing callers should use to check.
 */

#ifndef TUS_SCHED_CAP_H
#define TUS_SCHED_CAP_H

#include <stdint.h>
#include "sched.h"

#define CAP_NET_ADMIN   (1u << 0) /* reconfigure network interfaces */
#define CAP_NET_RAW     (1u << 1) /* raw sockets (reserved: TUS has no
                                    * SOCK_RAW yet, but the bit exists
                                    * so a future raw-socket syscall
                                    * has something real to check) */
#define CAP_SETUID      (1u << 2) /* change uid/gid away from root
                                    * without actually being root
                                    * (reserved for a future
                                    * fine-grained setuid policy) */

#define CAP_ALL_KNOWN   (CAP_NET_ADMIN | CAP_NET_RAW | CAP_SETUID)

static inline int has_cap(const struct task *t, uint32_t cap) {
    if (t == NULL) {
        return 0;
    }
    if (t->euid == 0) {
        return 1; /* root implicitly has every capability */
    }
    return (t->caps & cap) != 0;
}

#endif /* TUS_SCHED_CAP_H */
