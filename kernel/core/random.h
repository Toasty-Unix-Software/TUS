/*
 * random.h - the kernel's random number source
 *
 * A ChaCha20 stream keyed from whatever entropy the machine offers:
 * RDSEED/RDRAND when the CPU has them, otherwise timing jitter between
 * the timestamp counter and the PIT. Everything that needs
 * unpredictable bytes - TCP sequence numbers, ssh key exchange, host
 * key generation - draws from here, and from /dev/urandom in
 * userspace.
 */

#ifndef TUS_CORE_RANDOM_H
#define TUS_CORE_RANDOM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Gather entropy and key the generator. Called once from _start(). */
void random_init(void);

/* Fill `buf` with `len` random bytes. */
void random_bytes(void *buf, size_t len);

/* Stir external entropy (an interrupt's timing, a packet's arrival)
 * into the pool. Cheap enough to call from an interrupt handler. */
void random_add_entropy(const void *data, size_t len);

/* True when the CPU provided hardware entropy at seeding time. */
bool random_has_hardware(void);

#endif /* TUS_CORE_RANDOM_H */
