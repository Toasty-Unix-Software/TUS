/*
 * sshtrans.c - version exchange, the binary packet protocol, and the
 * curve25519-sha256 key exchange
 *
 * The packet layer has two shapes. Before the first NEWKEYS it is
 * plaintext: a 4-byte length, a padding length, the payload, and
 * padding out to a multiple of 8. After NEWKEYS it is
 * chacha20-poly1305@openssh.com, which differs in three ways worth
 * naming, because each one is a place an implementation quietly goes
 * wrong:
 *
 *   - The length field is encrypted under its own key, with its own
 *     chacha instance. That is what lets a receiver learn how much to
 *     read without having authenticated anything yet.
 *   - The nonce is the packet sequence number, not a random value or
 *     a counter of its own, so both ends must agree on the sequence
 *     number exactly - including the reset that strict KEX performs.
 *   - Padding is computed over the packet *without* the length field,
 *     since that field is no longer part of the encrypted block.
 *
 * A receiver must not act on the length until the tag verifies. It
 * has to allocate on it, though, which is why the length is bounded
 * before anything is read.
 */

#include "ssh.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "tuscrypt.h"

#define SSH_PACKET_BLOCK 8
#define SSH_TAG_LEN      POLY1305_TAG_SIZE

void ssh_init(struct ssh *s, int fd, int server) {
    memset(s, 0, sizeof(*s));
    s->fd = fd;
    s->server = server;
    sshbuf_init(&s->in);
    sshbuf_init(&s->out);
    sshbuf_init(&s->pkt);
}

void ssh_close(struct ssh *s) {
    sshbuf_free(&s->in);
    sshbuf_free(&s->out);
    sshbuf_free(&s->pkt);
    crypto_wipe(s->key_in, sizeof(s->key_in));
    crypto_wipe(s->key_out, sizeof(s->key_out));
    if (s->fd >= 0) close(s->fd);
    s->fd = -1;
}

int ssh_fail(struct ssh *s, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(s->err, sizeof(s->err), fmt, ap);
    va_end(ap);
    return -1;
}

/* ---- raw socket I/O ---- */

static int write_all(int fd, const void *data, size_t len) {
    const uint8_t *p = data;
    while (len > 0) {
        long n = write(fd, p, len);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0) return -1;
        p += n;
        len -= (size_t)n;
    }
    return 0;
}

int ssh_flush(struct ssh *s) {
    if (s->out.len == 0) return 0;
    if (write_all(s->fd, s->out.data, s->out.len) != 0) {
        return ssh_fail(s, "write: %s", strerror(errno));
    }
    sshbuf_reset(&s->out);
    return 0;
}

long ssh_read_more(struct ssh *s) {
    uint8_t buf[4096];

    long n = read(s->fd, buf, sizeof(buf));
    if (n < 0) {
        if (errno == EINTR) return -2; /* caller retries */
        ssh_fail(s, "read: %s", strerror(errno));
        return -1;
    }
    if (n == 0) {
        s->eof = 1;
        return 0;
    }
    sshbuf_put(&s->in, buf, (size_t)n);
    if (s->in.error) {
        ssh_fail(s, "input buffer overflow");
        return -1;
    }
    return n;
}

/* ---- chacha20-poly1305@openssh.com ---- */

/*
 * The key is 64 bytes: the first 32 encrypt the packet body (K_2),
 * the second 32 encrypt the length field (K_1). They are separate
 * keys so that a peer who can see lengths still cannot see contents.
 */
static void chachapoly_seq(uint8_t nonce[8], uint32_t seq) {
    /* The nonce is a 64-bit big-endian sequence number. Ours is 32
     * bits because the connection rekeys long before it wraps. */
    memset(nonce, 0, 8);
    nonce[4] = (uint8_t)(seq >> 24);
    nonce[5] = (uint8_t)(seq >> 16);
    nonce[6] = (uint8_t)(seq >> 8);
    nonce[7] = (uint8_t)seq;
}

static void chachapoly_length(const uint8_t key[64], uint32_t seq,
                              const uint8_t in[4], uint8_t out[4]) {
    struct chacha20_ctx ctx;
    uint8_t nonce[8];

    chachapoly_seq(nonce, seq);
    chacha20_init(&ctx, key + 32, nonce, 0);
    chacha20_xor(&ctx, in, out, 4);
    crypto_wipe(&ctx, sizeof(ctx));
}

/* The Poly1305 key is the first 32 bytes of block zero of the body
 * keystream; the body itself starts at block one. */
static void chachapoly_polykey(const uint8_t key[64], uint32_t seq,
                               uint8_t out[32]) {
    struct chacha20_ctx ctx;
    uint8_t nonce[8], zeros[32];

    chachapoly_seq(nonce, seq);
    memset(zeros, 0, sizeof(zeros));
    chacha20_init(&ctx, key, nonce, 0);
    chacha20_xor(&ctx, zeros, out, 32);
    crypto_wipe(&ctx, sizeof(ctx));
}

static void chachapoly_body(const uint8_t key[64], uint32_t seq,
                            const uint8_t *in, uint8_t *out, size_t len) {
    struct chacha20_ctx ctx;
    uint8_t nonce[8];

    chachapoly_seq(nonce, seq);
    chacha20_init(&ctx, key, nonce, 1);
    chacha20_xor(&ctx, in, out, len);
    crypto_wipe(&ctx, sizeof(ctx));
}

/* ---- sending ---- */

void ssh_packet_put(struct ssh *s, const struct sshbuf *payload) {
    size_t plen = payload->len;

    /* Padding fills out the part of the packet the cipher covers. For
     * an AEAD the length field is outside that part; for plaintext it
     * is inside it. */
    size_t aligned = s->encrypt_out ? 1 + plen : 4 + 1 + plen;
    size_t pad = SSH_PACKET_BLOCK - (aligned % SSH_PACKET_BLOCK);
    if (pad < 4) pad += SSH_PACKET_BLOCK;

    uint32_t pktlen = (uint32_t)(1 + plen + pad);

    uint8_t padding[SSH_PACKET_BLOCK * 2];
    if (s->encrypt_out) {
        crypto_random(padding, pad);
    } else {
        /* Before the keys exist the padding is visible and carries no
         * secret, and a fixed pattern keeps the handshake
         * reproducible when something has to be debugged. */
        memset(padding, 0, pad);
    }

    if (!s->encrypt_out) {
        sshbuf_put_u32(&s->out, pktlen);
        sshbuf_put_u8(&s->out, (uint8_t)pad);
        sshbuf_put(&s->out, payload->data, plen);
        sshbuf_put(&s->out, padding, pad);
        s->seq_out++;
        return;
    }

    /* Assemble the plaintext body, then encrypt in place. */
    struct sshbuf body;
    sshbuf_init(&body);
    sshbuf_put_u8(&body, (uint8_t)pad);
    sshbuf_put(&body, payload->data, plen);
    sshbuf_put(&body, padding, pad);

    uint8_t lenbuf[4] = { (uint8_t)(pktlen >> 24), (uint8_t)(pktlen >> 16),
                          (uint8_t)(pktlen >> 8), (uint8_t)pktlen };
    uint8_t enc_len[4];
    chachapoly_length(s->key_out, s->seq_out, lenbuf, enc_len);
    chachapoly_body(s->key_out, s->seq_out, body.data, body.data, body.len);

    /* The tag covers the encrypted length and the encrypted body,
     * which is what stops a peer from splicing packets together. */
    uint8_t polykey[32], tag[SSH_TAG_LEN];
    struct sshbuf maced;
    sshbuf_init(&maced);
    sshbuf_put(&maced, enc_len, 4);
    sshbuf_put(&maced, body.data, body.len);

    chachapoly_polykey(s->key_out, s->seq_out, polykey);
    poly1305(polykey, maced.data, maced.len, tag);

    sshbuf_put(&s->out, maced.data, maced.len);
    sshbuf_put(&s->out, tag, sizeof(tag));

    crypto_wipe(polykey, sizeof(polykey));
    sshbuf_free(&body);
    sshbuf_free(&maced);
    s->seq_out++;
}

int ssh_packet_send(struct ssh *s, const struct sshbuf *payload) {
    ssh_packet_put(s, payload);
    if (s->out.error) return ssh_fail(s, "output buffer overflow");
    return ssh_flush(s);
}

int ssh_send_disconnect(struct ssh *s, uint32_t reason, const char *text) {
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_DISCONNECT);
    sshbuf_put_u32(&p, reason);
    sshbuf_put_cstring(&p, text);
    sshbuf_put_cstring(&p, "");
    int r = ssh_packet_send(s, &p);
    sshbuf_free(&p);
    return r;
}

/* ---- receiving ---- */

/*
 * The whole encrypted region has to be a multiple of the cipher block
 * size, and which bytes that region covers depends on the cipher. In
 * plaintext the length field is inside it, so packet_length is 4
 * short of a multiple of 8; under the AEAD the length field is
 * outside it, so packet_length is a multiple of 8 exactly. Getting
 * this backwards rejects every packet a real peer sends.
 */
static int packet_length_sane(struct ssh *s, uint32_t pktlen) {
    if (pktlen < SSH_PACKET_BLOCK || pktlen > SSH_MAX_PAYLOAD + 256) {
        return ssh_fail(s, "bad packet length %u", pktlen);
    }

    uint32_t covered = s->decrypt_in ? pktlen : pktlen + 4;
    if (covered % SSH_PACKET_BLOCK != 0) {
        return ssh_fail(s, "packet length %u is not a multiple of %d", pktlen,
                        SSH_PACKET_BLOCK);
    }
    return 0;
}

int ssh_packet_next(struct ssh *s, uint8_t *type) {
    const uint8_t *raw = s->in.data;
    size_t have = s->in.len;

    if (!s->decrypt_in) {
        if (have < 4) return 0;

        uint32_t pktlen = ((uint32_t)raw[0] << 24) | ((uint32_t)raw[1] << 16) |
                          ((uint32_t)raw[2] << 8) | raw[3];
        if (packet_length_sane(s, pktlen) != 0) return -1;
        if (have < 4 + pktlen) return 0;

        uint8_t pad = raw[4];
        if (pad < 4 || (size_t)pad + 1 > pktlen) {
            return ssh_fail(s, "bad padding length %u", pad);
        }

        size_t plen = pktlen - 1 - pad;
        sshbuf_reset(&s->pkt);
        sshbuf_put(&s->pkt, raw + 5, plen);
        sshbuf_consume(&s->in, 4 + pktlen);
        s->seq_in++;

        if (plen == 0) return ssh_fail(s, "empty packet");
        if (sshbuf_get_u8(&s->pkt, type) != 0) return -1;
        return 1;
    }

    if (have < 4) return 0;

    uint8_t lenbuf[4];
    chachapoly_length(s->key_in, s->seq_in, raw, lenbuf);
    uint32_t pktlen = ((uint32_t)lenbuf[0] << 24) | ((uint32_t)lenbuf[1] << 16) |
                      ((uint32_t)lenbuf[2] << 8) | lenbuf[3];
    if (packet_length_sane(s, pktlen) != 0) return -1;
    if (have < 4 + pktlen + SSH_TAG_LEN) return 0;

    uint8_t polykey[32], tag[SSH_TAG_LEN];
    chachapoly_polykey(s->key_in, s->seq_in, polykey);
    poly1305(polykey, raw, 4 + pktlen, tag);
    crypto_wipe(polykey, sizeof(polykey));

    if (crypto_verify(tag, raw + 4 + pktlen, SSH_TAG_LEN) != 0) {
        return ssh_fail(s, "packet authentication failed");
    }

    sshbuf_reset(&s->pkt);
    if (sshbuf_reserve(&s->pkt, pktlen) != 0) {
        return ssh_fail(s, "out of memory for a %u byte packet", pktlen);
    }
    chachapoly_body(s->key_in, s->seq_in, raw + 4, s->pkt.data, pktlen);
    s->pkt.len = pktlen;

    uint8_t pad = s->pkt.data[0];
    if (pad < 4 || (size_t)pad + 1 > pktlen) {
        return ssh_fail(s, "bad padding length %u", pad);
    }
    s->pkt.len = pktlen - pad; /* drop the padding, keep padlen byte */
    s->pkt.off = 1;            /* and step over the padding length   */

    sshbuf_consume(&s->in, 4 + pktlen + SSH_TAG_LEN);
    s->seq_in++;

    if (sshbuf_remaining(&s->pkt) == 0) return ssh_fail(s, "empty packet");
    if (sshbuf_get_u8(&s->pkt, type) != 0) return -1;
    return 1;
}

/* Messages the transport answers on its own. Returns 1 when the
 * caller should not see this packet. */
static int packet_is_housekeeping(struct ssh *s, uint8_t type) {
    switch (type) {
    case SSH_MSG_GLOBAL_REQUEST: {
        /* OpenSSH sends hostkeys-00@openssh.com the moment
         * authentication succeeds, which lands in the middle of
         * whatever the caller was waiting for. Neither end here
         * implements any global request, so the answer is always the
         * same one. */
        int want_reply = 0;
        if (sshbuf_skip_string(&s->pkt) != 0 ||
            sshbuf_get_bool(&s->pkt, &want_reply) != 0) {
            ssh_fail(s, "malformed GLOBAL_REQUEST");
            return -1;
        }
        if (want_reply) {
            struct sshbuf p;
            sshbuf_init(&p);
            sshbuf_put_u8(&p, SSH_MSG_REQUEST_FAILURE);
            int rc = ssh_packet_send(s, &p);
            sshbuf_free(&p);
            if (rc != 0) return -1;
        }
        return 1;
    }

    case SSH_MSG_IGNORE:
    case SSH_MSG_DEBUG:
    case SSH_MSG_UNIMPLEMENTED:
    case SSH_MSG_EXT_INFO:
        /* During strict KEX these are forbidden, precisely because
         * they are the packets an attacker can insert without being
         * able to decrypt anything. */
        if (s->strict_kex && !s->decrypt_in) {
            ssh_fail(s, "message %u during strict key exchange", type);
            return -1;
        }
        return 1;
    default:
        return 0;
    }
}

int ssh_packet_recv(struct ssh *s, uint8_t *type) {
    for (;;) {
        int r = ssh_packet_next(s, type);
        if (r < 0) return -1;
        if (r == 0) {
            long n = ssh_read_more(s);
            if (n == -2) continue;
            if (n < 0) return -1;
            if (n == 0) return ssh_fail(s, "connection closed by peer");
            continue;
        }

        if (*type == SSH_MSG_DISCONNECT) {
            uint32_t reason = 0;
            const char *text = NULL;
            size_t text_len = 0;
            sshbuf_get_u32(&s->pkt, &reason);
            sshbuf_get_cstring(&s->pkt, &text, &text_len);
            return ssh_fail(s, "peer disconnected (%u): %.*s", reason,
                            (int)text_len, text ? text : "");
        }

        int hk = packet_is_housekeeping(s, *type);
        if (hk < 0) return -1;
        if (hk == 0) return 0;
    }
}

/* ---- version exchange ---- */

/*
 * Both ends send "SSH-2.0-<software>\r\n". A server may send any
 * number of lines before its version line, which is where a login
 * banner goes, so lines are read until one begins with "SSH-".
 */
int ssh_exchange_versions(struct ssh *s, const char *software) {
    snprintf(s->v_local, sizeof(s->v_local), "SSH-2.0-%s", software);

    char line[300];
    int n = snprintf(line, sizeof(line), "%s\r\n", s->v_local);
    if (write_all(s->fd, line, (size_t)n) != 0) {
        return ssh_fail(s, "write: %s", strerror(errno));
    }

    for (int attempts = 0; attempts < 64; attempts++) {
        /* Find a complete line in what has already been read. */
        uint8_t *nl = s->in.len ? memchr(s->in.data, '\n', s->in.len) : NULL;
        if (!nl) {
            long got = ssh_read_more(s);
            if (got == -2) continue;
            if (got < 0) return -1;
            if (got == 0) return ssh_fail(s, "peer sent no version string");
            attempts--; /* reading is progress, not an attempt */
            continue;
        }

        size_t line_len = (size_t)(nl - s->in.data);
        size_t consumed = line_len + 1;
        if (line_len > 0 && s->in.data[line_len - 1] == '\r') line_len--;

        if (line_len >= sizeof(s->v_peer)) {
            return ssh_fail(s, "peer version string is too long");
        }

        int is_version = line_len >= 4 && memcmp(s->in.data, "SSH-", 4) == 0;
        if (is_version) {
            memcpy(s->v_peer, s->in.data, line_len);
            s->v_peer[line_len] = '\0';
            sshbuf_consume(&s->in, consumed);

            if (memcmp(s->v_peer, "SSH-2.0-", 8) != 0 &&
                memcmp(s->v_peer, "SSH-1.99-", 9) != 0) {
                return ssh_fail(s, "unsupported protocol version: %s",
                                s->v_peer);
            }
            return 0;
        }
        sshbuf_consume(&s->in, consumed); /* a banner line: ignore it */
    }
    return ssh_fail(s, "peer sent too many banner lines");
}

/* ---- key exchange ---- */

#define KEX_NAME     "curve25519-sha256"
#define KEX_NAME_ALT "curve25519-sha256@libssh.org"
#define CIPHER_NAME  "chacha20-poly1305@openssh.com"
#define STRICT_C     "kex-strict-c-v00@openssh.com"
#define STRICT_S     "kex-strict-s-v00@openssh.com"

static void kexinit_build(struct ssh *s, struct sshbuf *p) {
    uint8_t cookie[16];
    crypto_random(cookie, sizeof(cookie));

    char kex[256];
    snprintf(kex, sizeof(kex), "%s,%s,%s", KEX_NAME, KEX_NAME_ALT,
             s->server ? STRICT_S : STRICT_C);

    sshbuf_put_u8(p, SSH_MSG_KEXINIT);
    sshbuf_put(p, cookie, sizeof(cookie));
    sshbuf_put_cstring(p, kex);
    sshbuf_put_cstring(p, SSH_ED25519_NAME);
    sshbuf_put_cstring(p, CIPHER_NAME); /* client to server */
    sshbuf_put_cstring(p, CIPHER_NAME); /* server to client */
    sshbuf_put_cstring(p, "hmac-sha2-256"); /* unused: the cipher is an AEAD */
    sshbuf_put_cstring(p, "hmac-sha2-256");
    sshbuf_put_cstring(p, "none");
    sshbuf_put_cstring(p, "none");
    sshbuf_put_cstring(p, "");
    sshbuf_put_cstring(p, "");
    sshbuf_put_bool(p, 0); /* no guessed KEX packet follows */
    sshbuf_put_u32(p, 0);
}

/*
 * Check that the peer's KEXINIT offers what we require. The lists we
 * send hold exactly one usable name per slot, so "negotiation" here
 * is only ever a membership test - but it has to be done on the
 * peer's list, not assumed.
 */
static int kexinit_check(struct ssh *s, struct sshbuf *peer) {
    static const char *slots[] = {
        "key exchange", "host key", "cipher client to server",
        "cipher server to client",
    };
    static const char *needed[] = {
        NULL, SSH_ED25519_NAME, CIPHER_NAME, CIPHER_NAME,
    };

    if (sshbuf_get(peer, NULL, 16) != 0) { /* the cookie */
        return ssh_fail(s, "truncated KEXINIT");
    }

    for (int i = 0; i < 4; i++) {
        const char *list;
        size_t list_len;
        if (sshbuf_get_cstring(peer, &list, &list_len) != 0) {
            return ssh_fail(s, "truncated KEXINIT");
        }
        if (i == 0) {
            if (!sshbuf_namelist_has(list, list_len, KEX_NAME) &&
                !sshbuf_namelist_has(list, list_len, KEX_NAME_ALT)) {
                return ssh_fail(s, "peer offers no %s", slots[i]);
            }
            /* Both ends must ask for strict KEX before either can
             * rely on it. */
            s->strict_kex = sshbuf_namelist_has(
                list, list_len, s->server ? STRICT_C : STRICT_S);
        } else if (!sshbuf_namelist_has(list, list_len, needed[i])) {
            return ssh_fail(s, "peer offers no %s we support (%s)", slots[i],
                            needed[i]);
        }
    }

    /* MAC, compression and language lists. Compression must include
     * "none": we do not implement any. */
    for (int i = 0; i < 6; i++) {
        const char *list;
        size_t list_len;
        if (sshbuf_get_cstring(peer, &list, &list_len) != 0) {
            return ssh_fail(s, "truncated KEXINIT");
        }
        if ((i == 2 || i == 3) && !sshbuf_namelist_has(list, list_len, "none")) {
            return ssh_fail(s, "peer requires compression");
        }
    }

    int follows = 0;
    sshbuf_get_bool(peer, &follows);
    if (follows) {
        /* The peer guessed a KEX packet. Its guess cannot match, since
         * a matching guess would have to name our algorithms, and we
         * have not told it what those are yet. */
        return ssh_fail(s, "peer sent a guessed key exchange packet");
    }
    return 0;
}

/* K_x = HASH(K || H || X || session_id), extended a block at a time. */
static void derive_key(uint8_t *out, size_t out_len, const struct sshbuf *k_mpint,
                       const uint8_t h[32], char which,
                       const uint8_t session_id[32]) {
    struct sha256_ctx ctx;
    uint8_t block[32];

    sha256_init(&ctx);
    sha256_update(&ctx, k_mpint->data, k_mpint->len);
    sha256_update(&ctx, h, 32);
    sha256_update(&ctx, &which, 1);
    sha256_update(&ctx, session_id, 32);
    sha256_final(&ctx, block);

    size_t done = 0;
    for (;;) {
        size_t take = out_len - done < 32 ? out_len - done : 32;
        memcpy(out + done, block, take);
        done += take;
        if (done == out_len) break;

        /* Each further block hashes everything produced so far. */
        struct sha256_ctx more;
        sha256_init(&more);
        sha256_update(&more, k_mpint->data, k_mpint->len);
        sha256_update(&more, h, 32);
        sha256_update(&more, out, done);
        sha256_final(&more, block);
    }
    crypto_wipe(block, sizeof(block));
}

int ssh_kex(struct ssh *s, const struct ssh_key *hostkey,
            int (*verify)(void *ctx, const struct ssh_key *k), void *ctx) {
    struct sshbuf i_local, i_peer, p, k_mpint;
    uint8_t eph_priv[32], eph_pub[32], shared[32], h[32];
    int rc = -1;

    sshbuf_init(&i_local);
    sshbuf_init(&i_peer);
    sshbuf_init(&p);
    sshbuf_init(&k_mpint);

    kexinit_build(s, &i_local);
    if (ssh_packet_send(s, &i_local) != 0) goto out;

    uint8_t type;
    if (ssh_packet_recv(s, &type) != 0) goto out;
    if (type != SSH_MSG_KEXINIT) {
        ssh_fail(s, "expected KEXINIT, got message %u", type);
        goto out;
    }

    /* The exchange hash covers the peer's KEXINIT payload including
     * its message type byte, so rebuild it from the start. */
    sshbuf_put_u8(&i_peer, SSH_MSG_KEXINIT);
    sshbuf_put(&i_peer, sshbuf_ptr(&s->pkt), sshbuf_remaining(&s->pkt));
    if (kexinit_check(s, &s->pkt) != 0) goto out;

    crypto_random(eph_priv, sizeof(eph_priv));
    x25519_base(eph_pub, eph_priv);

    const uint8_t *q_c, *q_s, *k_s = NULL, *sig = NULL;
    size_t q_c_len = 32, q_s_len = 32, k_s_len = 0, sig_len = 0;
    struct sshbuf hostkey_blob;
    sshbuf_init(&hostkey_blob);
    struct ssh_key peer_key;

    if (!s->server) {
        sshbuf_reset(&p);
        sshbuf_put_u8(&p, SSH_MSG_KEX_ECDH_INIT);
        sshbuf_put_string(&p, eph_pub, 32);
        if (ssh_packet_send(s, &p) != 0) goto out2;

        if (ssh_packet_recv(s, &type) != 0) goto out2;
        if (type != SSH_MSG_KEX_ECDH_REPLY) {
            ssh_fail(s, "expected KEX_ECDH_REPLY, got message %u", type);
            goto out2;
        }
        if (sshbuf_get_string(&s->pkt, &k_s, &k_s_len) != 0 ||
            sshbuf_get_string(&s->pkt, &q_s, &q_s_len) != 0 ||
            sshbuf_get_string(&s->pkt, &sig, &sig_len) != 0) {
            ssh_fail(s, "malformed KEX_ECDH_REPLY");
            goto out2;
        }
        if (q_s_len != 32) {
            ssh_fail(s, "server sent a %zu byte ephemeral key", q_s_len);
            goto out2;
        }
        q_c = eph_pub;

        if (ssh_key_from_blob(&peer_key, k_s, k_s_len) != 0) {
            ssh_fail(s, "server host key is not %s", SSH_ED25519_NAME);
            goto out2;
        }
        if (x25519(shared, eph_priv, q_s) != 0) {
            ssh_fail(s, "server sent a small-order ephemeral key");
            goto out2;
        }
    } else {
        if (ssh_packet_recv(s, &type) != 0) goto out2;
        if (type != SSH_MSG_KEX_ECDH_INIT) {
            ssh_fail(s, "expected KEX_ECDH_INIT, got message %u", type);
            goto out2;
        }
        if (sshbuf_get_string(&s->pkt, &q_c, &q_c_len) != 0 || q_c_len != 32) {
            ssh_fail(s, "malformed KEX_ECDH_INIT");
            goto out2;
        }
        q_s = eph_pub;

        ssh_key_blob(hostkey, &hostkey_blob);
        k_s = hostkey_blob.data;
        k_s_len = hostkey_blob.len;

        if (x25519(shared, eph_priv, q_c) != 0) {
            ssh_fail(s, "client sent a small-order ephemeral key");
            goto out2;
        }
    }

    /* H = HASH(V_C || V_S || I_C || I_S || K_S || Q_C || Q_S || K),
     * every field length-prefixed, K as an mpint. */
    sshbuf_put_mpint(&k_mpint, shared, 32);
    {
        struct sshbuf hb;
        sshbuf_init(&hb);
        const struct sshbuf *i_c = s->server ? &i_peer : &i_local;
        const struct sshbuf *i_s = s->server ? &i_local : &i_peer;
        sshbuf_put_cstring(&hb, s->server ? s->v_peer : s->v_local);
        sshbuf_put_cstring(&hb, s->server ? s->v_local : s->v_peer);
        sshbuf_put_stringb(&hb, i_c);
        sshbuf_put_stringb(&hb, i_s);
        sshbuf_put_string(&hb, k_s, k_s_len);
        sshbuf_put_string(&hb, q_c, q_c_len);
        sshbuf_put_string(&hb, q_s, q_s_len);
        sshbuf_put(&hb, k_mpint.data, k_mpint.len);
        if (hb.error) {
            sshbuf_free(&hb);
            ssh_fail(s, "out of memory building the exchange hash");
            goto out2;
        }
        sha256(hb.data, hb.len, h);
        sshbuf_free(&hb);
    }

    if (!s->server) {
        if (ssh_key_verify(&peer_key, sig, sig_len, h, sizeof(h)) != 0) {
            ssh_fail(s, "the server's signature over the exchange hash is bad");
            goto out2;
        }
        if (verify && verify(ctx, &peer_key) != 0) {
            ssh_fail(s, "host key rejected");
            goto out2;
        }
    } else {
        struct sshbuf sigb;
        sshbuf_init(&sigb);
        ssh_key_sign(hostkey, h, sizeof(h), &sigb);

        sshbuf_reset(&p);
        sshbuf_put_u8(&p, SSH_MSG_KEX_ECDH_REPLY);
        sshbuf_put_string(&p, k_s, k_s_len);
        sshbuf_put_string(&p, eph_pub, 32);
        sshbuf_put_stringb(&p, &sigb);
        int sent = ssh_packet_send(s, &p);
        sshbuf_free(&sigb);
        if (sent != 0) goto out2;
    }

    /* The first exchange hash becomes the session identifier, and
     * stays that way through any later rekey. */
    if (!s->have_session_id) {
        memcpy(s->session_id, h, 32);
        s->have_session_id = 1;
    }

    sshbuf_reset(&p);
    sshbuf_put_u8(&p, SSH_MSG_NEWKEYS);
    if (ssh_packet_send(s, &p) != 0) goto out2;

    if (ssh_packet_recv(s, &type) != 0) goto out2;
    if (type != SSH_MSG_NEWKEYS) {
        ssh_fail(s, "expected NEWKEYS, got message %u", type);
        goto out2;
    }

    /* C and D are the two directions' encryption keys. chacha20-
     * poly1305 needs no IV and computes its own MAC key, so A, B, E
     * and F have nothing to carry. */
    derive_key(s->key_out, 64, &k_mpint, h, s->server ? 'D' : 'C',
               s->session_id);
    derive_key(s->key_in, 64, &k_mpint, h, s->server ? 'C' : 'D',
               s->session_id);
    s->encrypt_out = s->decrypt_in = 1;

    if (s->strict_kex) {
        s->seq_in = s->seq_out = 0;
    }
    rc = 0;

out2:
    sshbuf_free(&hostkey_blob);
out:
    crypto_wipe(eph_priv, sizeof(eph_priv));
    crypto_wipe(shared, sizeof(shared));
    sshbuf_free(&i_local);
    sshbuf_free(&i_peer);
    sshbuf_free(&p);
    sshbuf_free(&k_mpint);
    return rc;
}
