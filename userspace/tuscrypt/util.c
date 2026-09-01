/*
 * util.c - the small pieces every other file leans on
 */

#include "tuscrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int crypto_verify(const void *a, const void *b, size_t len) {
    const uint8_t *x = (const uint8_t *)a;
    const uint8_t *y = (const uint8_t *)b;
    uint8_t diff = 0;

    /* No early exit: the loop must take the same time whether the
     * first byte differs or only the last one does. */
    for (size_t i = 0; i < len; i++) {
        diff |= (uint8_t)(x[i] ^ y[i]);
    }
    return diff != 0;
}

void crypto_wipe(void *buf, size_t len) {
    /* A volatile pointer keeps the compiler from deciding the stores
     * are dead - which it is otherwise entitled to do for a buffer
     * that is never read again. */
    volatile uint8_t *p = (volatile uint8_t *)buf;
    while (len--) *p++ = 0;
}

#ifndef TUSCRYPT_HOST_TEST

/* getrandom(2), which TUS routes to the kernel's ChaCha20 pool. musl
 * has no wrapper for it on this ABI, so the syscall is made directly. */
static long tus_getrandom(void *buf, size_t len) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = (long)buf;
    register long rsi __asm__("rsi") = (long)len;
    register long rdx __asm__("rdx") = 0;

    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"(48L)
                     : "rcx", "r11", "memory");
    return ret;
}

void crypto_random(void *buf, size_t len) {
    long n = tus_getrandom(buf, len);
    if (n != (long)len) {
        /* There is no way to carry on safely: a key made from
         * uninitialised stack is worse than no key at all. */
        fprintf(stderr, "crypto: the kernel gave no entropy; refusing "
                        "to generate anything\n");
        _exit(1);
    }
}

#else  /* the host test harness reads /dev/urandom instead */

void crypto_random(void *buf, size_t len) {
    FILE *f = fopen("/dev/urandom", "rb");
    if (!f || fread(buf, 1, len, f) != len) {
        fprintf(stderr, "crypto: /dev/urandom failed\n");
        exit(1);
    }
    fclose(f);
}

#endif
