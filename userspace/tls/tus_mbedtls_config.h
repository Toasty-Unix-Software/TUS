/*
 * tus_mbedtls_config.h - what TUS takes away from Mbed TLS's defaults
 *
 * This is a user config: Mbed TLS reads its own mbedtls_config.h
 * first and then this, so everything here is a subtraction. Writing a
 * whole configuration from scratch would mean re-deciding hundreds of
 * options that upstream already has right; what actually differs is
 * the handful of things the operating system underneath cannot
 * provide.
 *
 *   files      TUS has open() and read(), but nothing here needs to
 *              load a certificate from disk: Clint carries its trust
 *              anchors compiled in.
 *   sockets    Mbed TLS's own net layer wants select() and
 *              getaddrinfo(); TUS has neither in the shape it
 *              expects, so the transport is supplied by the caller
 *              through mbedtls_ssl_set_bio() instead.
 *   time       TUS's time() counts seconds since boot, not since
 *              1970. A certificate's validity dates cannot be checked
 *              against a clock that says it is January 1970 every
 *              time the machine starts, so date checking is off and
 *              the caller has to know that.
 *   threads    TUS userspace is single-threaded.
 */

#ifndef TUS_MBEDTLS_CONFIG_H
#define TUS_MBEDTLS_CONFIG_H

/* No filesystem access from inside the library. */
#undef MBEDTLS_FS_IO
#undef MBEDTLS_PSA_ITS_FILE_C
#undef MBEDTLS_PSA_CRYPTO_STORAGE_C

/* The caller owns the socket. */
#undef MBEDTLS_NET_C
#undef MBEDTLS_TIMING_C

/*
 * Without a real-time clock there is no honest way to check notBefore
 * and notAfter, so X.509 stops pretending to. Everything else about a
 * certificate - the chain, the signatures, the name - is still
 * verified; expiry is the one property TUS cannot judge, and
 * MBEDTLS_X509_BADCERT_EXPIRED will simply never be raised.
 */
#undef MBEDTLS_HAVE_TIME
#undef MBEDTLS_HAVE_TIME_DATE

/* One thread, so no locking is needed and none is compiled. */
#undef MBEDTLS_THREADING_C
#undef MBEDTLS_THREADING_PTHREAD

/* Nothing calls the self-tests, and they are a large slice of the
 * binary. */
#undef MBEDTLS_SELF_TEST

/*
 * TUS userspace is built with -mgeneral-regs-only: the compiler may
 * not emit SSE, and the kernel does not save those registers across a
 * context switch. AES-NI and the hand-written x86-64 assembly both
 * assume otherwise, so the portable C implementations are used
 * instead.
 */
#undef MBEDTLS_AESNI_C
#undef MBEDTLS_AESCE_C
#undef MBEDTLS_HAVE_ASM
#undef MBEDTLS_PADLOCK_C

#endif /* TUS_MBEDTLS_CONFIG_H */
