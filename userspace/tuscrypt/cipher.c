/*
 * cipher.c - ChaCha20, Poly1305 and AES-CTR
 *
 * ChaCha20 and Poly1305 are RFC 8439's, except that ssh uses the
 * original 64-bit nonce / 64-bit counter split rather than the RFC's
 * 96/32 one, so chacha20_init() takes an eight-byte nonce.
 *
 * Poly1305 uses five 26-bit limbs so that every partial product fits a
 * 64-bit register with room for the sum - the arrangement that lets
 * the whole thing run without a 128-bit type or a single branch on
 * secret data.
 *
 * AES is the plain byte-oriented construction, not a T-table one: a
 * table indexed by a secret byte is a cache-timing side channel, and
 * TUS has no use for the speed it would buy.
 */

#include "tuscrypt.h"

#include <string.h>

/* ---- ChaCha20 ---- */

#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))

#define QUARTERROUND(a, b, c, d)       \
    a += b; d ^= a; d = ROTL32(d, 16); \
    c += d; b ^= c; b = ROTL32(b, 12); \
    a += b; d ^= a; d = ROTL32(d, 8);  \
    c += d; b ^= c; b = ROTL32(b, 7)

static uint32_t load_le32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void store_le32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

void chacha20_init(struct chacha20_ctx *ctx, const uint8_t key[32],
                   const uint8_t nonce[8], uint32_t counter) {
    /* "expand 32-byte k" */
    ctx->state[0] = 0x61707865;
    ctx->state[1] = 0x3320646e;
    ctx->state[2] = 0x79622d32;
    ctx->state[3] = 0x6b206574;

    for (int i = 0; i < 8; i++) {
        ctx->state[4 + i] = load_le32(key + i * 4);
    }
    ctx->state[12] = counter;
    ctx->state[13] = 0;
    ctx->state[14] = load_le32(nonce);
    ctx->state[15] = load_le32(nonce + 4);
}

static void chacha20_block(const uint32_t in[16], uint8_t out[64]) {
    uint32_t x[16];

    for (int i = 0; i < 16; i++) x[i] = in[i];

    for (int i = 0; i < 10; i++) {
        QUARTERROUND(x[0], x[4], x[8],  x[12]);
        QUARTERROUND(x[1], x[5], x[9],  x[13]);
        QUARTERROUND(x[2], x[6], x[10], x[14]);
        QUARTERROUND(x[3], x[7], x[11], x[15]);
        QUARTERROUND(x[0], x[5], x[10], x[15]);
        QUARTERROUND(x[1], x[6], x[11], x[12]);
        QUARTERROUND(x[2], x[7], x[8],  x[13]);
        QUARTERROUND(x[3], x[4], x[9],  x[14]);
    }
    for (int i = 0; i < 16; i++) store_le32(out + i * 4, x[i] + in[i]);
}

void chacha20_xor(struct chacha20_ctx *ctx, const uint8_t *in, uint8_t *out,
                  size_t len) {
    uint8_t block[64];

    while (len > 0) {
        chacha20_block(ctx->state, block);

        /* The counter is 64 bits wide in ssh's variant. */
        if (++ctx->state[12] == 0) ctx->state[13]++;

        size_t n = len < 64 ? len : 64;
        for (size_t i = 0; i < n; i++) out[i] = in[i] ^ block[i];
        in += n;
        out += n;
        len -= n;
    }
    crypto_wipe(block, sizeof(block));
}

/* ---- Poly1305 ---- */

void poly1305(const uint8_t key[32], const void *data, size_t len,
              uint8_t tag[POLY1305_TAG_SIZE]) {
    const uint8_t *m = (const uint8_t *)data;

    /* r is clamped: the top four bits of every fourth byte and the low
     * two bits of three others are cleared, which is what keeps the
     * partial products inside 64 bits. */
    uint32_t r0 = (load_le32(key + 0)) & 0x3ffffff;
    uint32_t r1 = (load_le32(key + 3) >> 2) & 0x3ffff03;
    uint32_t r2 = (load_le32(key + 6) >> 4) & 0x3ffc0ff;
    uint32_t r3 = (load_le32(key + 9) >> 6) & 0x3f03fff;
    uint32_t r4 = (load_le32(key + 12) >> 8) & 0x00fffff;

    uint32_t s1 = r1 * 5, s2 = r2 * 5, s3 = r3 * 5, s4 = r4 * 5;
    uint32_t h0 = 0, h1 = 0, h2 = 0, h3 = 0, h4 = 0;

    uint8_t block[16];

    while (len > 0) {
        size_t n = len < 16 ? len : 16;
        uint32_t hibit;

        if (n == 16) {
            memcpy(block, m, 16);
            hibit = 1u << 24;
        } else {
            /* The final short block is padded with a 0x01 byte and
             * then zeroes, so its high bit comes from the padding
             * rather than from the fixed 2^128 term. */
            memset(block, 0, sizeof(block));
            memcpy(block, m, n);
            block[n] = 1;
            hibit = 0;
        }

        uint32_t t0 = load_le32(block + 0);
        uint32_t t1 = load_le32(block + 4);
        uint32_t t2 = load_le32(block + 8);
        uint32_t t3 = load_le32(block + 12);

        h0 += t0 & 0x3ffffff;
        h1 += ((t0 >> 26) | (t1 << 6)) & 0x3ffffff;
        h2 += ((t1 >> 20) | (t2 << 12)) & 0x3ffffff;
        h3 += ((t2 >> 14) | (t3 << 18)) & 0x3ffffff;
        h4 += (t3 >> 8) | hibit;

        uint64_t d0 = (uint64_t)h0 * r0 + (uint64_t)h1 * s4 +
                      (uint64_t)h2 * s3 + (uint64_t)h3 * s2 +
                      (uint64_t)h4 * s1;
        uint64_t d1 = (uint64_t)h0 * r1 + (uint64_t)h1 * r0 +
                      (uint64_t)h2 * s4 + (uint64_t)h3 * s3 +
                      (uint64_t)h4 * s2;
        uint64_t d2 = (uint64_t)h0 * r2 + (uint64_t)h1 * r1 +
                      (uint64_t)h2 * r0 + (uint64_t)h3 * s4 +
                      (uint64_t)h4 * s3;
        uint64_t d3 = (uint64_t)h0 * r3 + (uint64_t)h1 * r2 +
                      (uint64_t)h2 * r1 + (uint64_t)h3 * r0 +
                      (uint64_t)h4 * s4;
        uint64_t d4 = (uint64_t)h0 * r4 + (uint64_t)h1 * r3 +
                      (uint64_t)h2 * r2 + (uint64_t)h3 * r1 +
                      (uint64_t)h4 * r0;

        uint32_t c;
        c = (uint32_t)(d0 >> 26); h0 = (uint32_t)d0 & 0x3ffffff;
        d1 += c; c = (uint32_t)(d1 >> 26); h1 = (uint32_t)d1 & 0x3ffffff;
        d2 += c; c = (uint32_t)(d2 >> 26); h2 = (uint32_t)d2 & 0x3ffffff;
        d3 += c; c = (uint32_t)(d3 >> 26); h3 = (uint32_t)d3 & 0x3ffffff;
        d4 += c; c = (uint32_t)(d4 >> 26); h4 = (uint32_t)d4 & 0x3ffffff;
        h0 += c * 5; c = h0 >> 26; h0 &= 0x3ffffff;
        h1 += c;

        m += n;
        len -= n;
    }

    /* Fully carry h. */
    uint32_t c;
    c = h1 >> 26; h1 &= 0x3ffffff;
    h2 += c; c = h2 >> 26; h2 &= 0x3ffffff;
    h3 += c; c = h3 >> 26; h3 &= 0x3ffffff;
    h4 += c; c = h4 >> 26; h4 &= 0x3ffffff;
    h0 += c * 5; c = h0 >> 26; h0 &= 0x3ffffff;
    h1 += c;

    /* Compute h + -p and pick it if there was no borrow, without
     * branching on the result. */
    uint32_t g0 = h0 + 5; c = g0 >> 26; g0 &= 0x3ffffff;
    uint32_t g1 = h1 + c; c = g1 >> 26; g1 &= 0x3ffffff;
    uint32_t g2 = h2 + c; c = g2 >> 26; g2 &= 0x3ffffff;
    uint32_t g3 = h3 + c; c = g3 >> 26; g3 &= 0x3ffffff;
    uint32_t g4 = h4 + c - (1u << 26);

    uint32_t mask = (g4 >> 31) - 1;   /* all ones when g >= 0 */
    g0 &= mask; g1 &= mask; g2 &= mask; g3 &= mask; g4 &= mask;
    mask = ~mask;
    h0 = (h0 & mask) | g0;
    h1 = (h1 & mask) | g1;
    h2 = (h2 & mask) | g2;
    h3 = (h3 & mask) | g3;
    h4 = (h4 & mask) | g4;

    /* Serialise the 130-bit accumulator into 128 bits. */
    uint32_t f0 = (h0      ) | (h1 << 26);
    uint32_t f1 = (h1 >>  6) | (h2 << 20);
    uint32_t f2 = (h2 >> 12) | (h3 << 14);
    uint32_t f3 = (h3 >> 18) | (h4 <<  8);

    uint64_t acc;
    acc = (uint64_t)f0 + load_le32(key + 16);
    f0 = (uint32_t)acc;
    acc = (uint64_t)f1 + load_le32(key + 20) + (acc >> 32);
    f1 = (uint32_t)acc;
    acc = (uint64_t)f2 + load_le32(key + 24) + (acc >> 32);
    f2 = (uint32_t)acc;
    acc = (uint64_t)f3 + load_le32(key + 28) + (acc >> 32);
    f3 = (uint32_t)acc;

    store_le32(tag + 0, f0);
    store_le32(tag + 4, f1);
    store_le32(tag + 8, f2);
    store_le32(tag + 12, f3);

    crypto_wipe(block, sizeof(block));
}

/* ---- AES ---- */

static const uint8_t aes_sbox[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

static const uint8_t aes_rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

static uint32_t aes_sub_word(uint32_t w) {
    return ((uint32_t)aes_sbox[(w >> 24) & 0xff] << 24) |
           ((uint32_t)aes_sbox[(w >> 16) & 0xff] << 16) |
           ((uint32_t)aes_sbox[(w >> 8) & 0xff] << 8) |
           ((uint32_t)aes_sbox[w & 0xff]);
}

static uint32_t aes_rot_word(uint32_t w) { return (w << 8) | (w >> 24); }

int aes_init(struct aes_ctx *ctx, const uint8_t *key, int key_bits) {
    int nk;

    switch (key_bits) {
    case 128: nk = 4; ctx->rounds = 10; break;
    case 192: nk = 6; ctx->rounds = 12; break;
    case 256: nk = 8; ctx->rounds = 14; break;
    default: return -1;
    }

    int total = 4 * (ctx->rounds + 1);
    for (int i = 0; i < nk; i++) {
        ctx->round_key[i] = ((uint32_t)key[4 * i] << 24) |
                            ((uint32_t)key[4 * i + 1] << 16) |
                            ((uint32_t)key[4 * i + 2] << 8) |
                            ((uint32_t)key[4 * i + 3]);
    }

    for (int i = nk; i < total; i++) {
        uint32_t tmp = ctx->round_key[i - 1];

        if (i % nk == 0) {
            tmp = aes_sub_word(aes_rot_word(tmp)) ^
                  ((uint32_t)aes_rcon[i / nk] << 24);
        } else if (nk > 6 && i % nk == 4) {
            tmp = aes_sub_word(tmp);
        }
        ctx->round_key[i] = ctx->round_key[i - nk] ^ tmp;
    }
    return 0;
}

/* Multiply by x in GF(2^8), reducing by the AES polynomial. */
static uint8_t xtime(uint8_t b) {
    return (uint8_t)((b << 1) ^ (((b >> 7) & 1) * 0x1b));
}

static void add_round_key(uint8_t state[16], const uint32_t *rk) {
    for (int c = 0; c < 4; c++) {
        state[4 * c + 0] ^= (uint8_t)(rk[c] >> 24);
        state[4 * c + 1] ^= (uint8_t)(rk[c] >> 16);
        state[4 * c + 2] ^= (uint8_t)(rk[c] >> 8);
        state[4 * c + 3] ^= (uint8_t)(rk[c]);
    }
}

void aes_encrypt_block(const struct aes_ctx *ctx, const uint8_t in[16],
                       uint8_t out[16]) {
    uint8_t s[16];
    memcpy(s, in, 16);

    add_round_key(s, &ctx->round_key[0]);

    for (int round = 1; round <= ctx->rounds; round++) {
        for (int i = 0; i < 16; i++) s[i] = aes_sbox[s[i]];

        /* ShiftRows, on the column-major state AES defines. */
        uint8_t t;
        t = s[1];  s[1]  = s[5];  s[5]  = s[9];  s[9]  = s[13]; s[13] = t;
        t = s[2];  s[2]  = s[10]; s[10] = t;
        t = s[6];  s[6]  = s[14]; s[14] = t;
        t = s[15]; s[15] = s[11]; s[11] = s[7];  s[7]  = s[3];  s[3]  = t;

        if (round < ctx->rounds) {
            for (int c = 0; c < 4; c++) {
                uint8_t *col = &s[4 * c];
                uint8_t a0 = col[0], a1 = col[1], a2 = col[2], a3 = col[3];
                uint8_t all = (uint8_t)(a0 ^ a1 ^ a2 ^ a3);
                col[0] ^= all ^ xtime((uint8_t)(a0 ^ a1));
                col[1] ^= all ^ xtime((uint8_t)(a1 ^ a2));
                col[2] ^= all ^ xtime((uint8_t)(a2 ^ a3));
                col[3] ^= all ^ xtime((uint8_t)(a3 ^ a0));
            }
        }
        add_round_key(s, &ctx->round_key[4 * round]);
    }

    memcpy(out, s, 16);
    crypto_wipe(s, sizeof(s));
}

int aes_ctr_init(struct aes_ctr_ctx *ctx, const uint8_t *key, int key_bits,
                 const uint8_t iv[16]) {
    if (aes_init(&ctx->aes, key, key_bits) != 0) return -1;
    memcpy(ctx->counter, iv, 16);
    ctx->offset = 16; /* forces a fresh keystream block on first use */
    return 0;
}

void aes_ctr_xor(struct aes_ctr_ctx *ctx, const uint8_t *in, uint8_t *out,
                 size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (ctx->offset == 16) {
            aes_encrypt_block(&ctx->aes, ctx->counter, ctx->keystream);
            ctx->offset = 0;

            /* Increment the 128-bit counter, big-endian as ssh wants. */
            for (int b = 15; b >= 0; b--) {
                if (++ctx->counter[b] != 0) break;
            }
        }
        out[i] = in[i] ^ ctx->keystream[ctx->offset++];
    }
}
