/*
 * sshbuf.h - the SSH wire format, as a growable buffer
 *
 * Every structure the protocol sends is built out of five primitives
 * (RFC 4251 section 5): bytes, 32-bit and 64-bit big-endian integers,
 * strings with a length prefix, and mpints. Rather than scatter
 * length checks through the protocol code, all of it goes through one
 * buffer type that carries a sticky error flag: a truncated packet
 * makes the first getter fail and every getter after it fail too, so
 * a caller can parse a whole message and check once at the end.
 *
 * The getters hand back pointers *into* the buffer rather than
 * copies. They stay valid until the buffer is reset or written to
 * again, which is long enough for a message handler and short enough
 * that nothing is tempted to hold on to one.
 */

#ifndef SSHBUF_H
#define SSHBUF_H

#include <stddef.h>
#include <stdint.h>

struct sshbuf {
    uint8_t *data;
    size_t len;   /* bytes written                       */
    size_t cap;   /* bytes allocated                     */
    size_t off;   /* read cursor, <= len                 */
    int error;    /* sticky: an allocation or a short read failed */
};

void sshbuf_init(struct sshbuf *b);
void sshbuf_free(struct sshbuf *b);

/* Drop the contents and rewind. The allocation is kept: the packet
 * loop reuses the same buffers for every message. */
void sshbuf_reset(struct sshbuf *b);

/* Forget the bytes already read, moving the rest to the front. The
 * socket reader uses it to consume one packet and keep the remainder
 * of a short read for the next call. */
void sshbuf_consume(struct sshbuf *b, size_t n);

int sshbuf_reserve(struct sshbuf *b, size_t extra);

/* ---- writing ---- */

void sshbuf_put(struct sshbuf *b, const void *data, size_t len);
void sshbuf_put_u8(struct sshbuf *b, uint8_t v);
void sshbuf_put_u32(struct sshbuf *b, uint32_t v);
void sshbuf_put_u64(struct sshbuf *b, uint64_t v);
void sshbuf_put_bool(struct sshbuf *b, int v);
void sshbuf_put_string(struct sshbuf *b, const void *data, size_t len);
void sshbuf_put_cstring(struct sshbuf *b, const char *s);

/*
 * An mpint is a signed two's-complement big-endian integer with no
 * leading zero bytes - except that a positive value whose top bit is
 * set gets one, so it does not read as negative. Every mpint SSH
 * sends here is a positive fixed-width number (the X25519 shared
 * secret), so that is the only case handled.
 */
void sshbuf_put_mpint(struct sshbuf *b, const uint8_t *bytes, size_t len);

/* Length-prefix another buffer's contents, for nesting a signature
 * blob or a key blob inside a message. */
void sshbuf_put_stringb(struct sshbuf *b, const struct sshbuf *v);

/* ---- reading ---- */

int sshbuf_get(struct sshbuf *b, void *out, size_t len);
int sshbuf_get_u8(struct sshbuf *b, uint8_t *v);
int sshbuf_get_u32(struct sshbuf *b, uint32_t *v);
int sshbuf_get_u64(struct sshbuf *b, uint64_t *v);
int sshbuf_get_bool(struct sshbuf *b, int *v);

/* Both hand back a pointer into the buffer. get_cstring additionally
 * refuses embedded NULs, so the result is safe to treat as C text. */
int sshbuf_get_string(struct sshbuf *b, const uint8_t **data, size_t *len);
int sshbuf_get_cstring(struct sshbuf *b, const char **s, size_t *len);

/* Skip a string without looking at it. */
int sshbuf_skip_string(struct sshbuf *b);

static inline size_t sshbuf_remaining(const struct sshbuf *b) {
    return b->len - b->off;
}
static inline const uint8_t *sshbuf_ptr(const struct sshbuf *b) {
    return b->data + b->off;
}

/* ---- name lists ---- */

/*
 * A name-list is a string holding comma-separated names. Negotiation
 * is "the first name on the client's list that the server also
 * offers", so the lookup runs over the client list in order.
 */
int sshbuf_namelist_has(const char *list, size_t list_len, const char *name);
const char *sshbuf_namelist_first_match(const char *client, size_t client_len,
                                        const char *server, size_t server_len,
                                        char *out, size_t out_size);

/* ---- base64, for key files and known_hosts ---- */

size_t sshbuf_b64_encode(const uint8_t *in, size_t in_len, char *out,
                         size_t out_size);
long sshbuf_b64_decode(const char *in, size_t in_len, uint8_t *out,
                       size_t out_size);

#endif /* SSHBUF_H */
