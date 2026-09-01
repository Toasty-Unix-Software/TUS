/*
 * curve25519.c - X25519 key agreement and Ed25519 signatures
 *
 * Field elements are five 51-bit limbs over the prime 2^255 - 19. That
 * radix is what makes the schoolbook product of two elements fit: each
 * of the twenty-five 51x51 partial products is at most 102 bits, and
 * the folded sums stay inside the 128-bit accumulator with room to
 * spare, so a carry pass is only needed once per multiplication.
 *
 * Nothing here branches on secret data. The ladder and the scalar
 * multiplier select between candidate points with an arithmetic mask
 * rather than an if, and scalar reduction subtracts unconditionally
 * with a borrow mask. Constants that could have been transcribed as
 * limb arrays (the curve parameter d, sqrt(-1)) are instead computed
 * from small integers at first use, because a typo in a hand-copied
 * limb array is invisible until it silently produces wrong signatures.
 */

#include "tuscrypt.h"

#include <string.h>

typedef uint64_t fe[5];
typedef unsigned __int128 uint128_t;

#define MASK51 0x7ffffffffffffULL

/* ---- field arithmetic ---- */

static void fe_zero(fe h) { memset(h, 0, sizeof(fe)); }

static void fe_one(fe h) {
    fe_zero(h);
    h[0] = 1;
}

static void fe_copy(fe h, const fe f) { memcpy(h, f, sizeof(fe)); }

/*
 * Fold every limb back under 2^51. This is what lets fe_sub below
 * assume its subtrahend is small: without it, the sum of two limbs
 * that are each just under 2^51 lands above the 2p bias fe_sub adds,
 * and the subtraction wraps. Multiplication results already come out
 * of fe_carry this small, so only the additive operations need it.
 */
static void fe_reduce(fe h) {
    uint64_t c = 0;

    for (int i = 0; i < 5; i++) {
        h[i] += c;
        c = h[i] >> 51;
        h[i] &= MASK51;
    }
    /* 2^255 = 19 mod p: what fell off the top comes back at the
     * bottom, and that addition can carry once more. */
    h[0] += c * 19;
    h[1] += h[0] >> 51;
    h[0] &= MASK51;
}

static void fe_add(fe h, const fe f, const fe g) {
    for (int i = 0; i < 5; i++) h[i] = f[i] + g[i];
    fe_reduce(h);
}

/*
 * Subtraction adds 2p first so no limb can go negative. Twice the
 * prime is (2^52 - 38, 2^52 - 2, ...) in this radix; adding it changes
 * nothing modulo p and covers any subtrahend whose limbs are under
 * 2^52 - 38 - which fe_reduce guarantees for every value in flight.
 */
static void fe_sub(fe h, const fe f, const fe g) {
    h[0] = f[0] + 0xfffffffffffdaULL - g[0];
    h[1] = f[1] + 0xffffffffffffeULL - g[1];
    h[2] = f[2] + 0xffffffffffffeULL - g[2];
    h[3] = f[3] + 0xffffffffffffeULL - g[3];
    h[4] = f[4] + 0xffffffffffffeULL - g[4];
    fe_reduce(h);
}

static void fe_carry(fe h, uint128_t t[5]) {
    uint64_t c;

    t[1] += (uint64_t)(t[0] >> 51); h[0] = (uint64_t)t[0] & MASK51;
    t[2] += (uint64_t)(t[1] >> 51); h[1] = (uint64_t)t[1] & MASK51;
    t[3] += (uint64_t)(t[2] >> 51); h[2] = (uint64_t)t[2] & MASK51;
    t[4] += (uint64_t)(t[3] >> 51); h[3] = (uint64_t)t[3] & MASK51;
    c = (uint64_t)(t[4] >> 51);     h[4] = (uint64_t)t[4] & MASK51;

    /* 2^255 = 19 mod p, so the overflow past limb 4 comes back at the
     * bottom multiplied by 19. */
    h[0] += c * 19;
    h[1] += h[0] >> 51; h[0] &= MASK51;
    h[2] += h[1] >> 51; h[1] &= MASK51;
}

static void fe_mul(fe h, const fe f, const fe g) {
    /* The limbs of g above the first are pre-multiplied by 19 so the
     * wrap-around terms need no extra pass. */
    uint64_t g1_19 = 19 * g[1], g2_19 = 19 * g[2];
    uint64_t g3_19 = 19 * g[3], g4_19 = 19 * g[4];

    uint128_t t[5];
    t[0] = (uint128_t)f[0] * g[0] + (uint128_t)f[1] * g4_19 +
           (uint128_t)f[2] * g3_19 + (uint128_t)f[3] * g2_19 +
           (uint128_t)f[4] * g1_19;
    t[1] = (uint128_t)f[0] * g[1] + (uint128_t)f[1] * g[0] +
           (uint128_t)f[2] * g4_19 + (uint128_t)f[3] * g3_19 +
           (uint128_t)f[4] * g2_19;
    t[2] = (uint128_t)f[0] * g[2] + (uint128_t)f[1] * g[1] +
           (uint128_t)f[2] * g[0] + (uint128_t)f[3] * g4_19 +
           (uint128_t)f[4] * g3_19;
    t[3] = (uint128_t)f[0] * g[3] + (uint128_t)f[1] * g[2] +
           (uint128_t)f[2] * g[1] + (uint128_t)f[3] * g[0] +
           (uint128_t)f[4] * g4_19;
    t[4] = (uint128_t)f[0] * g[4] + (uint128_t)f[1] * g[3] +
           (uint128_t)f[2] * g[2] + (uint128_t)f[3] * g[1] +
           (uint128_t)f[4] * g[0];

    fe_carry(h, t);
}

static void fe_sq(fe h, const fe f) { fe_mul(h, f, f); }

static void fe_mul_small(fe h, const fe f, uint64_t n) {
    uint128_t t[5];
    for (int i = 0; i < 5; i++) t[i] = (uint128_t)f[i] * n;
    fe_carry(h, t);
}

static void fe_neg(fe h, const fe f) {
    fe zero;
    fe_zero(zero);
    fe_sub(h, zero, f);
}

/* Swap f and g when `swap` is 1, leave them alone when it is 0, with
 * no branch either way. */
static void fe_cswap(fe f, fe g, uint64_t swap) {
    uint64_t mask = 0 - swap;
    for (int i = 0; i < 5; i++) {
        uint64_t x = mask & (f[i] ^ g[i]);
        f[i] ^= x;
        g[i] ^= x;
    }
}

/* h = swap ? g : h */
static void fe_cmov(fe h, const fe g, uint64_t move) {
    uint64_t mask = 0 - move;
    for (int i = 0; i < 5; i++) {
        h[i] ^= mask & (h[i] ^ g[i]);
    }
}

static void fe_frombytes(fe h, const uint8_t s[32]) {
    uint64_t w[4];
    for (int i = 0; i < 4; i++) {
        w[i] = 0;
        for (int j = 0; j < 8; j++) {
            w[i] |= (uint64_t)s[i * 8 + j] << (j * 8);
        }
    }
    h[0] = w[0] & MASK51;
    h[1] = ((w[0] >> 51) | (w[1] << 13)) & MASK51;
    h[2] = ((w[1] >> 38) | (w[2] << 26)) & MASK51;
    h[3] = ((w[2] >> 25) | (w[3] << 39)) & MASK51;
    /* The top bit of the last byte is not part of the value: X25519
     * requires it be ignored, and Ed25519 uses it as a sign flag. */
    h[4] = (w[3] >> 12) & MASK51;
}

static void fe_tobytes(uint8_t s[32], const fe f) {
    fe t;
    fe_copy(t, f);

    /* Carry, then subtract p once if the value is still >= p. Two
     * carry passes are enough because the inputs are already nearly
     * reduced. */
    for (int pass = 0; pass < 2; pass++) {
        uint64_t c = 0;
        for (int i = 0; i < 5; i++) {
            t[i] += c;
            c = t[i] >> 51;
            t[i] &= MASK51;
        }
        t[0] += c * 19;
    }

    /* Compute t - p and keep it if there was no borrow. */
    uint64_t q = (t[0] + 19) >> 51;
    q = (t[1] + q) >> 51;
    q = (t[2] + q) >> 51;
    q = (t[3] + q) >> 51;
    q = (t[4] + q) >> 51;

    t[0] += 19 * q;
    uint64_t c = t[0] >> 51; t[0] &= MASK51;
    t[1] += c; c = t[1] >> 51; t[1] &= MASK51;
    t[2] += c; c = t[2] >> 51; t[2] &= MASK51;
    t[3] += c; c = t[3] >> 51; t[3] &= MASK51;
    t[4] += c; t[4] &= MASK51;

    uint64_t w[4];
    w[0] = t[0] | (t[1] << 51);
    w[1] = (t[1] >> 13) | (t[2] << 38);
    w[2] = (t[2] >> 26) | (t[3] << 25);
    w[3] = (t[3] >> 39) | (t[4] << 12);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            s[i * 8 + j] = (uint8_t)(w[i] >> (j * 8));
        }
    }
}

/* h = f ^ e, with `e` a 32-byte little-endian exponent. Square and
 * multiply: the exponents used here are public constants, so the
 * variable pattern of multiplications leaks nothing. */
static void fe_pow(fe h, const fe f, const uint8_t e[32]) {
    fe result, base;
    fe_one(result);
    fe_copy(base, f);

    for (int i = 0; i < 256; i++) {
        if ((e[i / 8] >> (i % 8)) & 1) {
            fe_mul(result, result, base);
        }
        fe_sq(base, base);
    }
    fe_copy(h, result);
}

/* p - 2, little-endian: the exponent that inverts. */
static const uint8_t fe_exp_invert[32] = {
    0xeb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f
};

/* (p - 5) / 8, the exponent behind the square root for p = 5 mod 8. */
static const uint8_t fe_exp_sqrt[32] = {
    0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f
};

/* (p - 1) / 4, which raises 2 to sqrt(-1). */
static const uint8_t fe_exp_sqrtm1[32] = {
    0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f
};

static void fe_invert(fe h, const fe f) { fe_pow(h, f, fe_exp_invert); }

static int fe_is_zero(const fe f) {
    uint8_t s[32];
    fe_tobytes(s, f);

    uint8_t acc = 0;
    for (int i = 0; i < 32; i++) acc |= s[i];
    return acc == 0;
}

static int fe_is_negative(const fe f) {
    uint8_t s[32];
    fe_tobytes(s, f);
    return s[0] & 1;
}

/* ---- X25519 ---- */

void x25519_base(uint8_t out[32], const uint8_t scalar[32]) {
    static const uint8_t basepoint[32] = {9};
    x25519(out, scalar, basepoint);
}

int x25519(uint8_t out[32], const uint8_t scalar[32],
           const uint8_t point[32]) {
    uint8_t e[32];
    memcpy(e, scalar, 32);

    /* Clamping: clear the three low bits so the scalar is a multiple
     * of the cofactor, clear the top bit and set the one below it so
     * every scalar has the same bit length. */
    e[0] &= 248;
    e[31] &= 127;
    e[31] |= 64;

    fe x1, x2, z2, x3, z3, tmp0, tmp1;
    fe_frombytes(x1, point);
    fe_one(x2);
    fe_zero(z2);
    fe_copy(x3, x1);
    fe_one(z3);

    uint64_t swap = 0;

    for (int pos = 254; pos >= 0; pos--) {
        uint64_t b = (e[pos / 8] >> (pos & 7)) & 1;
        swap ^= b;
        fe_cswap(x2, x3, swap);
        fe_cswap(z2, z3, swap);
        swap = b;

        /* The Montgomery ladder step (RFC 7748 section 5). */
        fe_sub(tmp0, x3, z3);
        fe_sub(tmp1, x2, z2);
        fe_add(x2, x2, z2);
        fe_add(z2, x3, z3);
        fe_mul(z3, tmp0, x2);
        fe_mul(z2, z2, tmp1);
        fe_sq(tmp0, tmp1);
        fe_sq(tmp1, x2);
        fe_add(x3, z3, z2);
        fe_sub(z2, z3, z2);
        fe_mul(x2, tmp1, tmp0);
        fe_sub(tmp1, tmp1, tmp0);
        fe_sq(z2, z2);
        fe_mul_small(z3, tmp1, 121666);
        fe_sq(x3, x3);
        fe_add(tmp0, tmp0, z3);
        fe_mul(z3, x1, z2);
        fe_mul(z2, tmp1, tmp0);
    }

    fe_cswap(x2, x3, swap);
    fe_cswap(z2, z3, swap);

    fe_invert(z2, z2);
    fe_mul(x2, x2, z2);
    fe_tobytes(out, x2);

    crypto_wipe(e, sizeof(e));

    /* An all-zero result means the peer sent a small-order point: the
     * "shared" secret would be a constant both sides could predict. */
    uint8_t acc = 0;
    for (int i = 0; i < 32; i++) acc |= out[i];
    return acc == 0 ? -1 : 0;
}

/* ---- Ed25519: curve constants, derived rather than transcribed ---- */

static fe ed_d;      /* -121665 / 121666            */
static fe ed_d2;     /* 2 * d                       */
static fe ed_sqrtm1; /* a square root of -1         */
static int ed_constants_ready;

static void ed_init_constants(void) {
    if (ed_constants_ready) return;

    fe num, den;
    fe_zero(num);
    num[0] = 121665;
    fe_neg(num, num);

    fe_zero(den);
    den[0] = 121666;
    fe_invert(den, den);
    fe_mul(ed_d, num, den);
    fe_add(ed_d2, ed_d, ed_d);

    fe two;
    fe_zero(two);
    two[0] = 2;
    fe_pow(ed_sqrtm1, two, fe_exp_sqrtm1);

    ed_constants_ready = 1;
}

/* A point in extended coordinates: x = X/Z, y = Y/Z, T = XY/Z. */
struct ge {
    fe X, Y, Z, T;
};

static void ge_zero(struct ge *p) {
    fe_zero(p->X);
    fe_one(p->Y);
    fe_one(p->Z);
    fe_zero(p->T);
}

static void ge_copy(struct ge *r, const struct ge *p) {
    fe_copy(r->X, p->X);
    fe_copy(r->Y, p->Y);
    fe_copy(r->Z, p->Z);
    fe_copy(r->T, p->T);
}

static void ge_cmov(struct ge *r, const struct ge *p, uint64_t move) {
    fe_cmov(r->X, p->X, move);
    fe_cmov(r->Y, p->Y, move);
    fe_cmov(r->Z, p->Z, move);
    fe_cmov(r->T, p->T, move);
}

/* add-2008-hwcd-3 for a = -1. */
static void ge_add(struct ge *r, const struct ge *p, const struct ge *q) {
    fe a, b, c, d, e, f, g, h, t;

    fe_sub(a, p->Y, p->X);
    fe_sub(t, q->Y, q->X);
    fe_mul(a, a, t);

    fe_add(b, p->Y, p->X);
    fe_add(t, q->Y, q->X);
    fe_mul(b, b, t);

    fe_mul(c, p->T, q->T);
    fe_mul(c, c, ed_d2);

    fe_mul(d, p->Z, q->Z);
    fe_add(d, d, d);

    fe_sub(e, b, a);
    fe_sub(f, d, c);
    fe_add(g, d, c);
    fe_add(h, b, a);

    fe_mul(r->X, e, f);
    fe_mul(r->Y, g, h);
    fe_mul(r->T, e, h);
    fe_mul(r->Z, f, g);
}

/* dbl-2008-hwcd for a = -1. */
static void ge_double(struct ge *r, const struct ge *p) {
    fe a, b, c, e, f, g, h, t;

    fe_sq(a, p->X);
    fe_sq(b, p->Y);
    fe_sq(c, p->Z);
    fe_add(c, c, c);

    fe_add(t, p->X, p->Y);
    fe_sq(t, t);
    fe_add(e, a, b);
    fe_sub(e, t, e);

    fe_sub(g, b, a);
    fe_sub(f, g, c);

    fe_add(h, a, b);
    fe_neg(h, h);

    fe_mul(r->X, e, f);
    fe_mul(r->Y, g, h);
    fe_mul(r->T, e, h);
    fe_mul(r->Z, f, g);
}

static void ge_tobytes(uint8_t s[32], const struct ge *p) {
    fe recip, x, y;

    fe_invert(recip, p->Z);
    fe_mul(x, p->X, recip);
    fe_mul(y, p->Y, recip);

    fe_tobytes(s, y);
    /* The compressed form is y with the low bit of x in the top bit. */
    s[31] ^= (uint8_t)(fe_is_negative(x) << 7);
}

/* Recover a point from its compressed form. Returns 0 on success, -1
 * when the encoding does not name a point on the curve. */
static int ge_frombytes(struct ge *p, const uint8_t s[32]) {
    fe u, v, v3, vxx, check, x, y;

    ed_init_constants();
    fe_frombytes(y, s);

    /* x^2 = (y^2 - 1) / (d y^2 + 1) */
    fe_sq(u, y);
    fe_mul(v, u, ed_d);
    fe_zero(check);
    check[0] = 1;
    fe_sub(u, u, check);      /* u = y^2 - 1     */
    fe_add(v, v, check);      /* v = d y^2 + 1   */

    /* The square root of u/v, via (u v^3) (u v^7)^((p-5)/8). */
    fe_sq(v3, v);
    fe_mul(v3, v3, v);
    fe_sq(x, v3);
    fe_mul(x, x, v);
    fe_mul(x, x, u);          /* x = u v^7       */

    fe_pow(x, x, fe_exp_sqrt);
    fe_mul(x, x, v3);
    fe_mul(x, x, u);          /* x = u v^3 (u v^7)^((p-5)/8) */

    fe_sq(vxx, x);
    fe_mul(vxx, vxx, v);
    fe_sub(check, vxx, u);

    if (!fe_is_zero(check)) {
        /* The other root: multiply by sqrt(-1) and retry the check. */
        fe_add(check, vxx, u);
        if (!fe_is_zero(check)) {
            return -1; /* not a square: this is not a curve point */
        }
        fe_mul(x, x, ed_sqrtm1);
    }

    /* Pick the root whose parity matches the encoded sign bit. */
    if (fe_is_negative(x) != ((s[31] >> 7) & 1)) {
        fe_neg(x, x);
    }

    fe_copy(p->X, x);
    fe_copy(p->Y, y);
    fe_one(p->Z);
    fe_mul(p->T, x, y);
    return 0;
}

/* The base point, as its standard compressed encoding. */
static const uint8_t ed_basepoint[32] = {
    0x58, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
    0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
    0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
    0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66
};

/*
 * r = [scalar]P, always 256 doublings and 256 conditional additions,
 * so the running time says nothing about the scalar. This is slower
 * than a windowed method and it is the one that is obviously correct.
 */
static void ge_scalarmult(struct ge *r, const uint8_t scalar[32],
                          const struct ge *p) {
    struct ge acc, sum;

    ed_init_constants();
    ge_zero(&acc);

    for (int i = 255; i >= 0; i--) {
        ge_double(&acc, &acc);
        ge_add(&sum, &acc, p);
        ge_cmov(&acc, &sum, (uint64_t)((scalar[i / 8] >> (i % 8)) & 1));
    }
    ge_copy(r, &acc);
}

static void ge_scalarmult_base(struct ge *r, const uint8_t scalar[32]) {
    struct ge b;
    ge_frombytes(&b, ed_basepoint);
    ge_scalarmult(r, scalar, &b);
}

/* ---- scalar arithmetic modulo the group order ---- */

/* l = 2^252 + 27742317777372353535851937790883648493, little-endian. */
static const uint8_t ed_l[32] = {
    0xed, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58,
    0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
};

/*
 * Reduce a 512-bit little-endian number modulo l.
 *
 * Long division, one bit at a time: shift the next bit of the input
 * into a running remainder and subtract l whenever the remainder has
 * grown past it. The subtraction is done unconditionally and masked by
 * the borrow, so the loop takes the same time whatever the input - it
 * runs on secrets during signing.
 */
static void sc_reduce(uint8_t out[32], const uint8_t in[64]) {
    uint32_t r[9];   /* the remainder, 32-bit limbs with room to shift */
    uint32_t l[9];

    memset(r, 0, sizeof(r));
    memset(l, 0, sizeof(l));
    for (int i = 0; i < 8; i++) {
        l[i] = (uint32_t)ed_l[i * 4] | ((uint32_t)ed_l[i * 4 + 1] << 8) |
               ((uint32_t)ed_l[i * 4 + 2] << 16) |
               ((uint32_t)ed_l[i * 4 + 3] << 24);
    }

    for (int bit = 511; bit >= 0; bit--) {
        /* r <<= 1, then bring in the next bit of the dividend. */
        uint32_t carry = (uint32_t)((in[bit / 8] >> (bit % 8)) & 1);
        for (int i = 0; i < 9; i++) {
            uint32_t next = r[i] >> 31;
            r[i] = (r[i] << 1) | carry;
            carry = next;
        }

        /* diff = r - l, and the final borrow says whether r < l. */
        uint32_t diff[9];
        uint64_t borrow = 0;
        for (int i = 0; i < 9; i++) {
            uint64_t d = (uint64_t)r[i] - l[i] - borrow;
            diff[i] = (uint32_t)d;
            borrow = (d >> 32) & 1;
        }

        uint32_t mask = (uint32_t)borrow - 1; /* all ones when r >= l */
        for (int i = 0; i < 9; i++) {
            r[i] = (r[i] & ~mask) | (diff[i] & mask);
        }
    }

    for (int i = 0; i < 8; i++) {
        out[i * 4 + 0] = (uint8_t)r[i];
        out[i * 4 + 1] = (uint8_t)(r[i] >> 8);
        out[i * 4 + 2] = (uint8_t)(r[i] >> 16);
        out[i * 4 + 3] = (uint8_t)(r[i] >> 24);
    }
    crypto_wipe(r, sizeof(r));
}

/* out = (a * b + c) mod l, with all three inputs already reduced. */
static void sc_muladd(uint8_t out[32], const uint8_t a[32],
                      const uint8_t b[32], const uint8_t c[32]) {
    uint64_t prod[8];
    uint8_t wide[64];

    memset(prod, 0, sizeof(prod));

    /* Schoolbook 256 x 256 into 512 bits, over 32-bit limbs so each
     * partial product fits a 64-bit register. */
    uint32_t al[8], bl[8];
    for (int i = 0; i < 8; i++) {
        al[i] = (uint32_t)a[i * 4] | ((uint32_t)a[i * 4 + 1] << 8) |
                ((uint32_t)a[i * 4 + 2] << 16) | ((uint32_t)a[i * 4 + 3] << 24);
        bl[i] = (uint32_t)b[i * 4] | ((uint32_t)b[i * 4 + 1] << 8) |
                ((uint32_t)b[i * 4 + 2] << 16) | ((uint32_t)b[i * 4 + 3] << 24);
    }

    uint32_t res[16];
    memset(res, 0, sizeof(res));

    for (int i = 0; i < 8; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < 8; j++) {
            uint64_t t = (uint64_t)al[i] * bl[j] + res[i + j] + carry;
            res[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        int k = i + 8;
        while (carry != 0 && k < 16) {
            uint64_t t = (uint64_t)res[k] + carry;
            res[k] = (uint32_t)t;
            carry = t >> 32;
            k++;
        }
    }

    /* Add c, zero-extended to 512 bits. */
    uint64_t carry = 0;
    for (int i = 0; i < 8; i++) {
        uint32_t cl = (uint32_t)c[i * 4] | ((uint32_t)c[i * 4 + 1] << 8) |
                      ((uint32_t)c[i * 4 + 2] << 16) |
                      ((uint32_t)c[i * 4 + 3] << 24);
        uint64_t t = (uint64_t)res[i] + cl + carry;
        res[i] = (uint32_t)t;
        carry = t >> 32;
    }
    for (int i = 8; i < 16 && carry; i++) {
        uint64_t t = (uint64_t)res[i] + carry;
        res[i] = (uint32_t)t;
        carry = t >> 32;
    }

    for (int i = 0; i < 16; i++) {
        wide[i * 4 + 0] = (uint8_t)res[i];
        wide[i * 4 + 1] = (uint8_t)(res[i] >> 8);
        wide[i * 4 + 2] = (uint8_t)(res[i] >> 16);
        wide[i * 4 + 3] = (uint8_t)(res[i] >> 24);
    }
    sc_reduce(out, wide);

    crypto_wipe(res, sizeof(res));
    crypto_wipe(wide, sizeof(wide));
    crypto_wipe(prod, sizeof(prod));
}

/* ---- Ed25519 ---- */

void ed25519_keypair(uint8_t public_key[32], uint8_t private_key[64],
                     const uint8_t seed[32]) {
    uint8_t h[64];
    struct ge a;

    ed_init_constants();
    sha512(seed, 32, h);

    /* The same clamping as X25519, on the low half of the hash. */
    h[0] &= 248;
    h[31] &= 127;
    h[31] |= 64;

    ge_scalarmult_base(&a, h);
    ge_tobytes(public_key, &a);

    /* OpenSSH's private key blob is seed || public. */
    memcpy(private_key, seed, 32);
    memcpy(private_key + 32, public_key, 32);

    crypto_wipe(h, sizeof(h));
}

void ed25519_sign(uint8_t signature[64], const void *message, size_t len,
                  const uint8_t public_key[32],
                  const uint8_t private_key[64]) {
    uint8_t h[64], r[64], hram[64];
    uint8_t scalar[32], r_reduced[32], hram_reduced[32];
    struct ge R;
    struct sha512_ctx ctx;

    ed_init_constants();

    sha512(private_key, 32, h);   /* private_key[0..31] is the seed */
    memcpy(scalar, h, 32);
    scalar[0] &= 248;
    scalar[31] &= 127;
    scalar[31] |= 64;

    /* r = H(prefix || message), where the prefix is the hash's upper
     * half. Deriving the nonce from the key and the message is what
     * makes Ed25519 deterministic - and what keeps a bad random
     * number generator from leaking the private key. */
    sha512_init(&ctx);
    sha512_update(&ctx, h + 32, 32);
    sha512_update(&ctx, message, len);
    sha512_final(&ctx, r);
    sc_reduce(r_reduced, r);

    ge_scalarmult_base(&R, r_reduced);
    ge_tobytes(signature, &R);

    /* S = r + H(R || A || M) * a  mod l */
    sha512_init(&ctx);
    sha512_update(&ctx, signature, 32);
    sha512_update(&ctx, public_key, 32);
    sha512_update(&ctx, message, len);
    sha512_final(&ctx, hram);
    sc_reduce(hram_reduced, hram);

    sc_muladd(signature + 32, hram_reduced, scalar, r_reduced);

    crypto_wipe(h, sizeof(h));
    crypto_wipe(r, sizeof(r));
    crypto_wipe(scalar, sizeof(scalar));
    crypto_wipe(r_reduced, sizeof(r_reduced));
}

int ed25519_verify(const uint8_t signature[64], const void *message,
                   size_t len, const uint8_t public_key[32]) {
    uint8_t hram[64], hram_reduced[32], check[32];
    struct ge A, R_expected, sB, hA;
    struct sha512_ctx ctx;

    ed_init_constants();

    /* S must be reduced: an unreduced S would let the same signature
     * be re-encoded, which is malleability rather than forgery, but
     * it is cheap to refuse. */
    if (signature[63] & 0xe0) return -1;

    if (ge_frombytes(&A, public_key) != 0) return -1;

    /* Verification checks [S]B = R + [h]A, rearranged to
     * [S]B + [h](-A) = R so it needs one comparison and no
     * subtraction of points. */
    fe_neg(A.X, A.X);
    fe_neg(A.T, A.T);

    sha512_init(&ctx);
    sha512_update(&ctx, signature, 32);
    sha512_update(&ctx, public_key, 32);
    sha512_update(&ctx, message, len);
    sha512_final(&ctx, hram);
    sc_reduce(hram_reduced, hram);

    ge_scalarmult_base(&sB, signature + 32);
    ge_scalarmult(&hA, hram_reduced, &A);
    ge_add(&R_expected, &sB, &hA);

    ge_tobytes(check, &R_expected);
    return crypto_verify(check, signature, 32);
}
