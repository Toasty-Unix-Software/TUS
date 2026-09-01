/*
 * hostname.h - the machine's name
 *
 * A single kernel-global buffer, exactly like keymap.c's current
 * layout name: uname()'s nodename and sethostname() both read/write
 * this one place, and /etc/hostname (main.c, loaded the same way as
 * /etc/keymap) seeds it at boot so a change made with `hostname`
 * survives a reboot without a rebuild.
 */

#ifndef TUS_CORE_HOSTNAME_H
#define TUS_CORE_HOSTNAME_H

#include <stddef.h>

#define HOSTNAME_MAX 64 /* matches Linux HOST_NAME_MAX, and struct
                          * utsname's 65-byte fields (64 + NUL) */

/* Never NULL, never unterminated: "tus" until something sets it. */
const char *hostname_get(void);

/* Copies at most HOSTNAME_MAX bytes of `name` (NUL or `len`-bounded,
 * whichever is shorter) in; always NUL-terminates. Returns 0, or
 * -EINVAL if `len` is 0 or exceeds HOSTNAME_MAX (matching Linux's own
 * sethostname() contract, which is what musl's wrapper expects). */
int hostname_set(const char *name, size_t len);

#endif /* TUS_CORE_HOSTNAME_H */
