/*
 * tus_psa_config.h - the same subtractions on the crypto side
 *
 * Mbed TLS 4 keeps its crypto in a separate tree (TF-PSA-Crypto) with
 * its own configuration, and the options that matter to an operating
 * system - files, clocks, CPU features - appear in both. This is the
 * crypto half; see tus_mbedtls_config.h for why each one goes.
 *
 * The one addition rather than subtraction is the random number
 * generator. Mbed TLS would otherwise build an entropy pool out of
 * platform sources it cannot find here; TUS already has a seeded
 * ChaCha20 CSPRNG in the kernel, reached through getrandom(2), so the
 * library is told to ask for randomness instead of collecting it.
 * mbedtls_psa_external_get_random() lives in tus_entropy.c.
 */

#ifndef TUS_PSA_CONFIG_H
#define TUS_PSA_CONFIG_H

#undef MBEDTLS_FS_IO
#undef MBEDTLS_PSA_ITS_FILE_C
#undef MBEDTLS_PSA_CRYPTO_STORAGE_C

#undef MBEDTLS_HAVE_TIME
#undef MBEDTLS_HAVE_TIME_DATE

#undef MBEDTLS_THREADING_C
#undef MBEDTLS_THREADING_PTHREAD

#undef MBEDTLS_SELF_TEST

#undef MBEDTLS_AESNI_C
#undef MBEDTLS_AESCE_C
#undef MBEDTLS_HAVE_ASM
#undef MBEDTLS_PADLOCK_C

/* The kernel's CSPRNG is the entropy source. */
#define MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG

#endif /* TUS_PSA_CONFIG_H */
