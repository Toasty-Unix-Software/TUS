/*
 * tus_entropy.c - the randomness Mbed TLS asks TUS for
 *
 * Built with MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG, the library does not
 * gather entropy itself: it calls this one function whenever it needs
 * random bytes, for key generation, for nonces, for blinding. The
 * answer comes from the kernel's ChaCha20 pool through getrandom(2),
 * which is the same source ssh keys are made from.
 *
 * There is no fallback. A TLS handshake that continues with
 * predictable randomness is worse than one that fails, so a short
 * read is an error and the caller aborts the connection.
 */

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include <psa/crypto.h>

/* getrandom(buf, len, flags). musl has no wrapper for a call TUS
 * numbers its own way, so the syscall is made directly. */
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

psa_status_t mbedtls_psa_external_get_random(
    mbedtls_psa_external_random_context_t *context,
    uint8_t *output, size_t output_size, size_t *output_length) {
    (void)context;

    size_t done = 0;
    while (done < output_size) {
        long n = tus_getrandom(output + done, output_size - done);
        if (n <= 0) {
            *output_length = 0;
            return PSA_ERROR_INSUFFICIENT_ENTROPY;
        }
        done += (size_t)n;
    }

    *output_length = done;
    return PSA_SUCCESS;
}
