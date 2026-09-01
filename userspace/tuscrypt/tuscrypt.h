/*
 * tuscrypt.h - the cryptography TUS's ssh and git need, and no more
 *
 * One library, four jobs:
 *
 *   hashes      SHA-1 (git object names), SHA-256 (ssh key exchange
 *               and MACs), SHA-512 (inside Ed25519)
 *   ciphers     ChaCha20-Poly1305 and AES-CTR, the two ssh transports
 *               worth supporting
 *   curve25519  X25519 for key agreement, Ed25519 for host and user
 *               keys
 *   entropy     the kernel's CSPRNG, reached through getrandom(2)
 *
 * Everything is constant-time where a timing difference would leak a
 * secret: the field arithmetic has no data-dependent branches, and
 * comparisons of secrets go through crypto_verify().
 */

#ifndef TUSCRYPT_H
#define TUSCRYPT_H

#include <stddef.h>
#include <stdint.h>

/* ---- SHA-1 ---- */

#define SHA1_DIGEST_SIZE 20
#define SHA1_BLOCK_SIZE  64

struct sha1_ctx {
    uint32_t state[5];
    uint64_t count;
    uint8_t buf[SHA1_BLOCK_SIZE];
    size_t buf_len;
};

void sha1_init(struct sha1_ctx *ctx);
void sha1_update(struct sha1_ctx *ctx, const void *data, size_t len);
void sha1_final(struct sha1_ctx *ctx, uint8_t out[SHA1_DIGEST_SIZE]);
void sha1(const void *data, size_t len, uint8_t out[SHA1_DIGEST_SIZE]);

/* ---- SHA-256 ---- */

#define SHA256_DIGEST_SIZE 32
#define SHA256_BLOCK_SIZE  64

struct sha256_ctx {
    uint32_t state[8];
    uint64_t count;
    uint8_t buf[SHA256_BLOCK_SIZE];
    size_t buf_len;
};

void sha256_init(struct sha256_ctx *ctx);
void sha256_update(struct sha256_ctx *ctx, const void *data, size_t len);
void sha256_final(struct sha256_ctx *ctx, uint8_t out[SHA256_DIGEST_SIZE]);
void sha256(const void *data, size_t len, uint8_t out[SHA256_DIGEST_SIZE]);

/* ---- SHA-512 ---- */

#define SHA512_DIGEST_SIZE 64
#define SHA512_BLOCK_SIZE  128

struct sha512_ctx {
    uint64_t state[8];
    uint64_t count_lo, count_hi;
    uint8_t buf[SHA512_BLOCK_SIZE];
    size_t buf_len;
};

void sha512_init(struct sha512_ctx *ctx);
void sha512_update(struct sha512_ctx *ctx, const void *data, size_t len);
void sha512_final(struct sha512_ctx *ctx, uint8_t out[SHA512_DIGEST_SIZE]);
void sha512(const void *data, size_t len, uint8_t out[SHA512_DIGEST_SIZE]);

/* ---- HMAC ---- */

struct hmac_sha256_ctx {
    struct sha256_ctx inner;
    struct sha256_ctx outer;
};

void hmac_sha256_init(struct hmac_sha256_ctx *ctx, const void *key,
                      size_t key_len);
void hmac_sha256_update(struct hmac_sha256_ctx *ctx, const void *data,
                        size_t len);
void hmac_sha256_final(struct hmac_sha256_ctx *ctx,
                       uint8_t out[SHA256_DIGEST_SIZE]);
void hmac_sha256(const void *key, size_t key_len, const void *data,
                 size_t len, uint8_t out[SHA256_DIGEST_SIZE]);

void hmac_sha1(const void *key, size_t key_len, const void *data,
               size_t len, uint8_t out[SHA1_DIGEST_SIZE]);

/* ---- ChaCha20 and Poly1305 ---- */

#define CHACHA20_KEY_SIZE   32
#define CHACHA20_NONCE_SIZE 8    /* ssh uses the 64-bit nonce form */
#define POLY1305_TAG_SIZE   16

struct chacha20_ctx {
    uint32_t state[16];
};

/* `counter` is the initial block counter: ssh's construction uses 0
 * for the length field's own keystream and 1 for the payload. */
void chacha20_init(struct chacha20_ctx *ctx, const uint8_t key[32],
                   const uint8_t nonce[8], uint32_t counter);
void chacha20_xor(struct chacha20_ctx *ctx, const uint8_t *in, uint8_t *out,
                  size_t len);

void poly1305(const uint8_t key[32], const void *data, size_t len,
              uint8_t tag[POLY1305_TAG_SIZE]);

/* ---- AES-CTR ---- */

struct aes_ctx {
    uint32_t round_key[60];
    int rounds;
};

/* key_bits is 128, 192 or 256. */
int aes_init(struct aes_ctx *ctx, const uint8_t *key, int key_bits);
void aes_encrypt_block(const struct aes_ctx *ctx, const uint8_t in[16],
                       uint8_t out[16]);

struct aes_ctr_ctx {
    struct aes_ctx aes;
    uint8_t counter[16];
    uint8_t keystream[16];
    int offset;
};

int aes_ctr_init(struct aes_ctr_ctx *ctx, const uint8_t *key, int key_bits,
                 const uint8_t iv[16]);
void aes_ctr_xor(struct aes_ctr_ctx *ctx, const uint8_t *in, uint8_t *out,
                 size_t len);

/* ---- X25519 ---- */

#define X25519_KEY_SIZE 32

/* Multiply the base point by `scalar`, giving a public key. */
void x25519_base(uint8_t out[32], const uint8_t scalar[32]);

/* Multiply `point` by `scalar`. Returns 0 on success, -1 when the
 * result is all zeroes (a small-order point: the shared secret would
 * be predictable, so the caller must abort the exchange). */
int x25519(uint8_t out[32], const uint8_t scalar[32],
           const uint8_t point[32]);

/* ---- Ed25519 ---- */

#define ED25519_SEED_SIZE       32
#define ED25519_PUBLIC_KEY_SIZE 32
#define ED25519_SIGNATURE_SIZE  64

/* Expand a 32-byte seed into a key pair. OpenSSH stores the private
 * key as seed||public, which is what `private_key` receives. */
void ed25519_keypair(uint8_t public_key[32], uint8_t private_key[64],
                     const uint8_t seed[32]);

void ed25519_sign(uint8_t signature[64], const void *message, size_t len,
                  const uint8_t public_key[32],
                  const uint8_t private_key[64]);

/* 0 when the signature is good, -1 otherwise. */
int ed25519_verify(const uint8_t signature[64], const void *message,
                   size_t len, const uint8_t public_key[32]);

/* ---- helpers ---- */

/* Constant-time comparison: 0 when equal, non-zero otherwise. Never
 * compare a MAC with memcmp - the early exit tells an attacker how
 * many leading bytes were right. */
int crypto_verify(const void *a, const void *b, size_t len);

/* Overwrite a buffer in a way the compiler may not elide. */
void crypto_wipe(void *buf, size_t len);

/* Fill `buf` from the kernel's CSPRNG. Aborts the process on failure:
 * there is no safe way for a caller to continue without entropy. */
void crypto_random(void *buf, size_t len);

#endif /* TUSCRYPT_H */
