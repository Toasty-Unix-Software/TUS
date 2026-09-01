/*
 * tustls.h - a TLS client connection, in four calls
 *
 * Mbed TLS is a large library with a small job to do here: turn a TCP
 * socket into one that encrypts. Everything a caller needs is
 * connect, read, write, close - the same shape as the socket
 * underneath - so the configuration, the trust store and the
 * handshake stay on this side of the interface.
 *
 * Two things about TLS on TUS are worth knowing before trusting it:
 *
 *   The clock. TUS's time() counts seconds since boot, so a
 *   certificate's notBefore and notAfter cannot be checked against
 *   anything. The chain, the signatures and the host name are all
 *   verified; expiry is not. tls_conn_warning() says so for a caller
 *   that wants to tell the user.
 *
 *   The trust store. Verification needs CA certificates, read from
 *   TLS_CA_BUNDLE at startup. Without that file there is nothing to
 *   verify against, and tls_connect() fails rather than connecting
 *   insecurely - unless the caller passes TLS_INSECURE and means it.
 */

#ifndef TUSTLS_H
#define TUSTLS_H

#include <stddef.h>

#define TLS_CA_BUNDLE "/etc/ssl/ca-bundle.crt"

/* tls_connect() flags. */
#define TLS_INSECURE 0x1u /* connect even if the certificate is bad */

struct tls_conn;

/*
 * Connect to host:port and complete a TLS handshake. Returns NULL on
 * failure, with a reason in tls_last_error().
 */
struct tls_conn *tls_connect(const char *host, int port, unsigned flags);

/* Both return the byte count, 0 at end of stream, -1 on error. */
long tls_read(struct tls_conn *c, void *buf, size_t len);
long tls_write(struct tls_conn *c, const void *buf, size_t len);

void tls_close(struct tls_conn *c);

/* The last failure, as text. Never NULL. */
const char *tls_last_error(void);

/*
 * What could not be checked about this connection, or NULL when
 * everything that can be checked was. Present so a browser can show
 * an honest padlock instead of a confident one.
 */
const char *tls_conn_warning(const struct tls_conn *c);

/* The negotiated protocol version and ciphersuite, for display. */
const char *tls_conn_version(const struct tls_conn *c);
const char *tls_conn_ciphersuite(const struct tls_conn *c);

#endif /* TUSTLS_H */
