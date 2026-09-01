/*
 * hash.c - SHA-1, SHA-256 and SHA-512, plus HMAC over the first two
 *
 * Straight from FIPS 180-4. All three follow the same shape: a
 * streaming context buffers up to one block, the compression function
 * runs on full blocks, and the final call appends the padding and the
 * length.
 *
 * SHA-1 is here for git, which names every object by it, and nothing
 * else. It is not used anywhere a collision would matter to security.
 */

#include "tuscrypt.h"

#include <string.h>

/* ---- shared helpers ---- */

static uint32_t load_be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static void store_be32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v >> 24);
    p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8);
    p[3] = (uint8_t)v;
}

static uint64_t load_be64(const uint8_t *p) {
    return ((uint64_t)load_be32(p) << 32) | load_be32(p + 4);
}

static void store_be64(uint8_t *p, uint64_t v) {
    store_be32(p, (uint32_t)(v >> 32));
    store_be32(p + 4, (uint32_t)v);
}

static uint32_t rotr32(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
static uint32_t rotl32(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }
static uint64_t rotr64(uint64_t x, int n) { return (x >> n) | (x << (64 - n)); }

/* ---- SHA-1 ---- */

static void sha1_compress(uint32_t state[5], const uint8_t block[64]) {
    uint32_t w[80];

    for (int i = 0; i < 16; i++) w[i] = load_be32(block + i * 4);
    for (int i = 16; i < 80; i++) {
        w[i] = rotl32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }

    uint32_t a = state[0], b = state[1], c = state[2];
    uint32_t d = state[3], e = state[4];

    for (int i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20)      { f = (b & c) | (~b & d);          k = 0x5a827999; }
        else if (i < 40) { f = b ^ c ^ d;                   k = 0x6ed9eba1; }
        else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8f1bbcdc; }
        else             { f = b ^ c ^ d;                   k = 0xca62c1d6; }

        uint32_t tmp = rotl32(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = rotl32(b, 30);
        b = a;
        a = tmp;
    }

    state[0] += a; state[1] += b; state[2] += c; state[3] += d; state[4] += e;
}

void sha1_init(struct sha1_ctx *ctx) {
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xc3d2e1f0;
    ctx->count = 0;
    ctx->buf_len = 0;
}

void sha1_update(struct sha1_ctx *ctx, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    ctx->count += len;

    if (ctx->buf_len > 0) {
        size_t need = SHA1_BLOCK_SIZE - ctx->buf_len;
        size_t take = len < need ? len : need;
        memcpy(ctx->buf + ctx->buf_len, p, take);
        ctx->buf_len += take;
        p += take;
        len -= take;

        if (ctx->buf_len < SHA1_BLOCK_SIZE) return;
        sha1_compress(ctx->state, ctx->buf);
        ctx->buf_len = 0;
    }

    while (len >= SHA1_BLOCK_SIZE) {
        sha1_compress(ctx->state, p);
        p += SHA1_BLOCK_SIZE;
        len -= SHA1_BLOCK_SIZE;
    }
    if (len > 0) {
        memcpy(ctx->buf, p, len);
        ctx->buf_len = len;
    }
}

void sha1_final(struct sha1_ctx *ctx, uint8_t out[SHA1_DIGEST_SIZE]) {
    uint64_t bits = ctx->count * 8;
    uint8_t pad[SHA1_BLOCK_SIZE * 2];
    size_t pad_len = (ctx->buf_len < 56) ? (56 - ctx->buf_len)
                                         : (120 - ctx->buf_len);

    memset(pad, 0, sizeof(pad));
    pad[0] = 0x80;
    store_be64(pad + pad_len, bits);
    sha1_update(ctx, pad, pad_len + 8);

    for (int i = 0; i < 5; i++) store_be32(out + i * 4, ctx->state[i]);
    crypto_wipe(ctx, sizeof(*ctx));
}

void sha1(const void *data, size_t len, uint8_t out[SHA1_DIGEST_SIZE]) {
    struct sha1_ctx ctx;
    sha1_init(&ctx);
    sha1_update(&ctx, data, len);
    sha1_final(&ctx, out);
}

/* ---- SHA-256 ---- */

static const uint32_t sha256_k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void sha256_compress(uint32_t state[8], const uint8_t block[64]) {
    uint32_t w[64];

    for (int i = 0; i < 16; i++) w[i] = load_be32(block + i * 4);
    for (int i = 16; i < 64; i++) {
        uint32_t s0 = rotr32(w[i - 15], 7) ^ rotr32(w[i - 15], 18) ^
                      (w[i - 15] >> 3);
        uint32_t s1 = rotr32(w[i - 2], 17) ^ rotr32(w[i - 2], 19) ^
                      (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t e = state[4], f = state[5], g = state[6], h = state[7];

    for (int i = 0; i < 64; i++) {
        uint32_t s1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25);
        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t t1 = h + s1 + ch + sha256_k[i] + w[i];
        uint32_t s0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t t2 = s0 + maj;

        h = g; g = f; f = e; e = d + t1;
        d = c; c = b; b = a; a = t1 + t2;
    }

    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

void sha256_init(struct sha256_ctx *ctx) {
    static const uint32_t iv[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    memcpy(ctx->state, iv, sizeof(iv));
    ctx->count = 0;
    ctx->buf_len = 0;
}

void sha256_update(struct sha256_ctx *ctx, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    ctx->count += len;

    if (ctx->buf_len > 0) {
        size_t need = SHA256_BLOCK_SIZE - ctx->buf_len;
        size_t take = len < need ? len : need;
        memcpy(ctx->buf + ctx->buf_len, p, take);
        ctx->buf_len += take;
        p += take;
        len -= take;

        if (ctx->buf_len < SHA256_BLOCK_SIZE) return;
        sha256_compress(ctx->state, ctx->buf);
        ctx->buf_len = 0;
    }

    while (len >= SHA256_BLOCK_SIZE) {
        sha256_compress(ctx->state, p);
        p += SHA256_BLOCK_SIZE;
        len -= SHA256_BLOCK_SIZE;
    }
    if (len > 0) {
        memcpy(ctx->buf, p, len);
        ctx->buf_len = len;
    }
}

void sha256_final(struct sha256_ctx *ctx, uint8_t out[SHA256_DIGEST_SIZE]) {
    uint64_t bits = ctx->count * 8;
    uint8_t pad[SHA256_BLOCK_SIZE * 2];
    size_t pad_len = (ctx->buf_len < 56) ? (56 - ctx->buf_len)
                                         : (120 - ctx->buf_len);

    memset(pad, 0, sizeof(pad));
    pad[0] = 0x80;
    store_be64(pad + pad_len, bits);
    sha256_update(ctx, pad, pad_len + 8);

    for (int i = 0; i < 8; i++) store_be32(out + i * 4, ctx->state[i]);
    crypto_wipe(ctx, sizeof(*ctx));
}

void sha256(const void *data, size_t len, uint8_t out[SHA256_DIGEST_SIZE]) {
    struct sha256_ctx ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, len);
    sha256_final(&ctx, out);
}

/* ---- SHA-512 ---- */

static const uint64_t sha512_k[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
    0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
    0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
    0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
    0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
    0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
    0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
    0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
    0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
    0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
    0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

static void sha512_compress(uint64_t state[8], const uint8_t block[128]) {
    uint64_t w[80];

    for (int i = 0; i < 16; i++) w[i] = load_be64(block + i * 8);
    for (int i = 16; i < 80; i++) {
        uint64_t s0 = rotr64(w[i - 15], 1) ^ rotr64(w[i - 15], 8) ^
                      (w[i - 15] >> 7);
        uint64_t s1 = rotr64(w[i - 2], 19) ^ rotr64(w[i - 2], 61) ^
                      (w[i - 2] >> 6);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    uint64_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint64_t e = state[4], f = state[5], g = state[6], h = state[7];

    for (int i = 0; i < 80; i++) {
        uint64_t s1 = rotr64(e, 14) ^ rotr64(e, 18) ^ rotr64(e, 41);
        uint64_t ch = (e & f) ^ (~e & g);
        uint64_t t1 = h + s1 + ch + sha512_k[i] + w[i];
        uint64_t s0 = rotr64(a, 28) ^ rotr64(a, 34) ^ rotr64(a, 39);
        uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint64_t t2 = s0 + maj;

        h = g; g = f; f = e; e = d + t1;
        d = c; c = b; b = a; a = t1 + t2;
    }

    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

void sha512_init(struct sha512_ctx *ctx) {
    static const uint64_t iv[8] = {
        0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL,
        0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
        0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
    };
    memcpy(ctx->state, iv, sizeof(iv));
    ctx->count_lo = ctx->count_hi = 0;
    ctx->buf_len = 0;
}

void sha512_update(struct sha512_ctx *ctx, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;

    uint64_t before = ctx->count_lo;
    ctx->count_lo += len;
    if (ctx->count_lo < before) ctx->count_hi++;

    if (ctx->buf_len > 0) {
        size_t need = SHA512_BLOCK_SIZE - ctx->buf_len;
        size_t take = len < need ? len : need;
        memcpy(ctx->buf + ctx->buf_len, p, take);
        ctx->buf_len += take;
        p += take;
        len -= take;

        if (ctx->buf_len < SHA512_BLOCK_SIZE) return;
        sha512_compress(ctx->state, ctx->buf);
        ctx->buf_len = 0;
    }

    while (len >= SHA512_BLOCK_SIZE) {
        sha512_compress(ctx->state, p);
        p += SHA512_BLOCK_SIZE;
        len -= SHA512_BLOCK_SIZE;
    }
    if (len > 0) {
        memcpy(ctx->buf, p, len);
        ctx->buf_len = len;
    }
}

void sha512_final(struct sha512_ctx *ctx, uint8_t out[SHA512_DIGEST_SIZE]) {
    /* The length is a 128-bit field; only the low 64 bits of the byte
     * count can ever be non-zero for anything TUS hashes, but the
     * high half is carried properly all the same. */
    uint64_t bits_lo = ctx->count_lo << 3;
    uint64_t bits_hi = (ctx->count_hi << 3) | (ctx->count_lo >> 61);

    uint8_t pad[SHA512_BLOCK_SIZE * 2];
    size_t pad_len = (ctx->buf_len < 112) ? (112 - ctx->buf_len)
                                          : (240 - ctx->buf_len);

    memset(pad, 0, sizeof(pad));
    pad[0] = 0x80;
    store_be64(pad + pad_len, bits_hi);
    store_be64(pad + pad_len + 8, bits_lo);
    sha512_update(ctx, pad, pad_len + 16);

    for (int i = 0; i < 8; i++) store_be64(out + i * 8, ctx->state[i]);
    crypto_wipe(ctx, sizeof(*ctx));
}

void sha512(const void *data, size_t len, uint8_t out[SHA512_DIGEST_SIZE]) {
    struct sha512_ctx ctx;
    sha512_init(&ctx);
    sha512_update(&ctx, data, len);
    sha512_final(&ctx, out);
}

/* ---- HMAC (RFC 2104) ---- */

void hmac_sha256_init(struct hmac_sha256_ctx *ctx, const void *key,
                      size_t key_len) {
    uint8_t k[SHA256_BLOCK_SIZE];
    uint8_t pad[SHA256_BLOCK_SIZE];

    memset(k, 0, sizeof(k));
    if (key_len > SHA256_BLOCK_SIZE) {
        sha256(key, key_len, k);
    } else {
        memcpy(k, key, key_len);
    }

    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) pad[i] = k[i] ^ 0x36;
    sha256_init(&ctx->inner);
    sha256_update(&ctx->inner, pad, sizeof(pad));

    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) pad[i] = k[i] ^ 0x5c;
    sha256_init(&ctx->outer);
    sha256_update(&ctx->outer, pad, sizeof(pad));

    crypto_wipe(k, sizeof(k));
    crypto_wipe(pad, sizeof(pad));
}

void hmac_sha256_update(struct hmac_sha256_ctx *ctx, const void *data,
                        size_t len) {
    sha256_update(&ctx->inner, data, len);
}

void hmac_sha256_final(struct hmac_sha256_ctx *ctx,
                       uint8_t out[SHA256_DIGEST_SIZE]) {
    uint8_t inner[SHA256_DIGEST_SIZE];
    sha256_final(&ctx->inner, inner);
    sha256_update(&ctx->outer, inner, sizeof(inner));
    sha256_final(&ctx->outer, out);
    crypto_wipe(inner, sizeof(inner));
}

void hmac_sha256(const void *key, size_t key_len, const void *data,
                 size_t len, uint8_t out[SHA256_DIGEST_SIZE]) {
    struct hmac_sha256_ctx ctx;
    hmac_sha256_init(&ctx, key, key_len);
    hmac_sha256_update(&ctx, data, len);
    hmac_sha256_final(&ctx, out);
}

void hmac_sha1(const void *key, size_t key_len, const void *data,
               size_t len, uint8_t out[SHA1_DIGEST_SIZE]) {
    uint8_t k[SHA1_BLOCK_SIZE];
    uint8_t pad[SHA1_BLOCK_SIZE];
    uint8_t inner[SHA1_DIGEST_SIZE];
    struct sha1_ctx ctx;

    memset(k, 0, sizeof(k));
    if (key_len > SHA1_BLOCK_SIZE) {
        sha1(key, key_len, k);
    } else {
        memcpy(k, key, key_len);
    }

    for (int i = 0; i < SHA1_BLOCK_SIZE; i++) pad[i] = k[i] ^ 0x36;
    sha1_init(&ctx);
    sha1_update(&ctx, pad, sizeof(pad));
    sha1_update(&ctx, data, len);
    sha1_final(&ctx, inner);

    for (int i = 0; i < SHA1_BLOCK_SIZE; i++) pad[i] = k[i] ^ 0x5c;
    sha1_init(&ctx);
    sha1_update(&ctx, pad, sizeof(pad));
    sha1_update(&ctx, inner, sizeof(inner));
    sha1_final(&ctx, out);

    crypto_wipe(k, sizeof(k));
    crypto_wipe(pad, sizeof(pad));
    crypto_wipe(inner, sizeof(inner));
}
