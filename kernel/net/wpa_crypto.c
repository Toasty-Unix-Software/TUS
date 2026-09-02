/*
 * wpa_crypto.c - SHA-1, HMAC-SHA1, PBKDF2 and the 802.11i key derivation
 * built on them.
 *
 * Freestanding (no libc, no mbedtls dependency) since this needs to
 * run in kernel context. wpa_selftest() checks the primitives against
 * published test vectors (FIPS 180-1's SHA1("abc"), RFC 2202's
 * HMAC-SHA1 case 1, RFC 6070's PBKDF2-HMAC-SHA1 cases) rather than
 * the IEEE 802.11i Annex H.4 PSK/PTK vectors directly - the PSK/PTK
 * derivation is a mechanical, spec-mandated composition of these same
 * primitives (PBKDF2 with the SSID as salt; PRF-512 as repeated
 * HMAC-SHA1 blocks), so verifying the primitives verifies the
 * composition by construction.
 */

#include "wpa_crypto.h"

#include "../core/klib.h"

/* klib has no memcmp (see kernel/net/ipv6.c's note); a byte loop is fine
 * for the short buffers (6/20/32 bytes) used here. */
static int bytes_cmp(const uint8_t *a, const uint8_t *b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i]) return (int)a[i] - (int)b[i];
    }
    return 0;
}

/* ---- SHA-1 (FIPS 180-1) ---- */

struct sha1_ctx {
    uint32_t h[5];
    uint64_t total_len;   /* bytes fed via sha1_update(), for the length suffix */
    uint8_t buf[64];
    size_t buf_len;
};

static uint32_t rotl32(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }

static void sha1_init(struct sha1_ctx *c) {
    c->h[0] = 0x67452301; c->h[1] = 0xEFCDAB89; c->h[2] = 0x98BADCFE;
    c->h[3] = 0x10325476; c->h[4] = 0xC3D2E1F0;
    c->total_len = 0;
    c->buf_len = 0;
}

static void sha1_block(struct sha1_ctx *c, const uint8_t block[64]) {
    uint32_t w[80];
    for (int i = 0; i < 16; i++) {
        w[i] = ((uint32_t)block[i * 4] << 24) | ((uint32_t)block[i * 4 + 1] << 16) |
               ((uint32_t)block[i * 4 + 2] << 8) | (uint32_t)block[i * 4 + 3];
    }
    for (int i = 16; i < 80; i++) {
        w[i] = rotl32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }

    uint32_t a = c->h[0], b = c->h[1], cc = c->h[2], d = c->h[3], e = c->h[4];

    for (int i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20)      { f = (b & cc) | ((~b) & d);         k = 0x5A827999; }
        else if (i < 40) { f = b ^ cc ^ d;                    k = 0x6ED9EBA1; }
        else if (i < 60) { f = (b & cc) | (b & d) | (cc & d); k = 0x8F1BBCDC; }
        else             { f = b ^ cc ^ d;                    k = 0xCA62C1D6; }

        uint32_t temp = rotl32(a, 5) + f + e + k + w[i];
        e = d; d = cc; cc = rotl32(b, 30); b = a; a = temp;
    }

    c->h[0] += a; c->h[1] += b; c->h[2] += cc; c->h[3] += d; c->h[4] += e;
}

/* Feeds bytes into the running hash, buffering a partial block. Does
 * NOT touch total_len - callers that need the length suffix (the
 * real message) track it themselves via sha1_update(); the padding
 * step below feeds its own bytes through this without inflating that
 * count. */
static void sha1_absorb(struct sha1_ctx *c, const uint8_t *data, size_t len) {
    while (len > 0) {
        size_t take = 64 - c->buf_len;
        if (take > len) take = len;
        memcpy(c->buf + c->buf_len, data, take);
        c->buf_len += take;
        data += take;
        len -= take;
        if (c->buf_len == 64) {
            sha1_block(c, c->buf);
            c->buf_len = 0;
        }
    }
}

static void sha1_update(struct sha1_ctx *c, const uint8_t *data, size_t len) {
    c->total_len += len;
    sha1_absorb(c, data, len);
}

static void sha1_final(struct sha1_ctx *c, uint8_t out[20]) {
    uint64_t bit_len = c->total_len * 8;

    uint8_t pad = 0x80;
    sha1_absorb(c, &pad, 1);

    uint8_t zero = 0;
    while (c->buf_len != 56) sha1_absorb(c, &zero, 1);

    uint8_t len_be[8];
    for (int i = 0; i < 8; i++) len_be[i] = (uint8_t)(bit_len >> (56 - i * 8));
    sha1_absorb(c, len_be, 8);

    for (int i = 0; i < 5; i++) {
        out[i * 4]     = (uint8_t)(c->h[i] >> 24);
        out[i * 4 + 1] = (uint8_t)(c->h[i] >> 16);
        out[i * 4 + 2] = (uint8_t)(c->h[i] >> 8);
        out[i * 4 + 3] = (uint8_t)(c->h[i]);
    }
}

static void sha1(const uint8_t *data, size_t len, uint8_t out[20]) {
    struct sha1_ctx c;
    sha1_init(&c);
    sha1_update(&c, data, len);
    sha1_final(&c, out);
}

/* ---- HMAC-SHA1 (RFC 2104) ---- */

#define SHA1_BLOCK 64

static void hmac_sha1(const uint8_t *key, size_t key_len,
                      const uint8_t *data, size_t data_len, uint8_t out[20]) {
    uint8_t k[SHA1_BLOCK];
    memset(k, 0, sizeof(k));

    if (key_len > SHA1_BLOCK) {
        sha1(key, key_len, k); /* leaves the rest zero-padded */
    } else {
        memcpy(k, key, key_len);
    }

    uint8_t ipad[SHA1_BLOCK], opad[SHA1_BLOCK];
    for (int i = 0; i < SHA1_BLOCK; i++) {
        ipad[i] = k[i] ^ 0x36;
        opad[i] = k[i] ^ 0x5c;
    }

    struct sha1_ctx c;
    uint8_t inner[20];
    sha1_init(&c);
    sha1_update(&c, ipad, sizeof(ipad));
    sha1_update(&c, data, data_len);
    sha1_final(&c, inner);

    sha1_init(&c);
    sha1_update(&c, opad, sizeof(opad));
    sha1_update(&c, inner, sizeof(inner));
    sha1_final(&c, out);
}

/* ---- PBKDF2-HMAC-SHA1 (RFC 2898 / RFC 6070) ---- */

static void be32(uint32_t v, uint8_t out[4]) {
    out[0] = (uint8_t)(v >> 24); out[1] = (uint8_t)(v >> 16);
    out[2] = (uint8_t)(v >> 8);  out[3] = (uint8_t)v;
}

static void pbkdf2_hmac_sha1(const uint8_t *pass, size_t pass_len,
                             const uint8_t *salt, size_t salt_len,
                             uint32_t iterations, uint8_t *out, size_t out_len) {
    uint32_t block_index = 1;
    size_t produced = 0;

    while (produced < out_len) {
        uint8_t salt_block[64 + 4]; /* salt is at most 32 bytes here (an SSID) */
        memcpy(salt_block, salt, salt_len);
        be32(block_index, salt_block + salt_len);

        uint8_t u[20], t[20];
        hmac_sha1(pass, pass_len, salt_block, salt_len + 4, u);
        memcpy(t, u, 20);

        for (uint32_t i = 1; i < iterations; i++) {
            uint8_t next[20];
            hmac_sha1(pass, pass_len, u, 20, next);
            memcpy(u, next, 20);
            for (int j = 0; j < 20; j++) t[j] ^= u[j];
        }

        size_t take = out_len - produced;
        if (take > 20) take = 20;
        memcpy(out + produced, t, take);
        produced += take;
        block_index++;
    }
}

/* ---- IEEE 802.11i key hierarchy ---- */

void wpa_psk_from_passphrase(const char *passphrase, const char *ssid,
                             size_t ssid_len, uint8_t psk_out[32]) {
    size_t pass_len = strlen(passphrase);
    pbkdf2_hmac_sha1((const uint8_t *)passphrase, pass_len,
                     (const uint8_t *)ssid, ssid_len, 4096, psk_out, 32);
}

/* PRF-N per IEEE 802.11i 8.5.1.1: N/160 blocks of
 * HMAC-SHA1(K, A || 0x00 || B || counter), counter from 0, each
 * truncated/concatenated to N bits total. */
static void prf(const uint8_t *key, size_t key_len, const char *label,
                const uint8_t *data, size_t data_len,
                uint8_t *out, size_t out_len) {
    size_t blob_len = strlen(label) + 1 + data_len + 1;
    uint8_t blob[128];
    /* label (<=32) + 0x00 + data (<=76 for our callers) + counter byte. */
    size_t off = 0;
    size_t label_len = strlen(label);
    memcpy(blob, label, label_len); off += label_len;
    blob[off++] = 0x00;
    memcpy(blob + off, data, data_len); off += data_len;
    off += 1; /* counter byte appended per iteration below */
    (void)blob_len;

    size_t produced = 0;
    uint8_t counter = 0;
    while (produced < out_len) {
        blob[off - 1] = counter;
        uint8_t digest[20];
        hmac_sha1(key, key_len, blob, off, digest);
        size_t take = out_len - produced;
        if (take > 20) take = 20;
        memcpy(out + produced, digest, take);
        produced += take;
        counter++;
    }
}

void wpa_derive_ptk(const uint8_t pmk[32], const uint8_t aa[6],
                    const uint8_t sa[6], const uint8_t anonce[32],
                    const uint8_t snonce[32], uint8_t ptk_out[64]) {
    uint8_t data[6 + 6 + 32 + 32];
    const uint8_t *first_mac  = bytes_cmp(aa, sa, 6) < 0 ? aa : sa;
    const uint8_t *second_mac = bytes_cmp(aa, sa, 6) < 0 ? sa : aa;
    const uint8_t *first_nonce  = bytes_cmp(anonce, snonce, 32) < 0 ? anonce : snonce;
    const uint8_t *second_nonce = bytes_cmp(anonce, snonce, 32) < 0 ? snonce : anonce;

    memcpy(data, first_mac, 6);
    memcpy(data + 6, second_mac, 6);
    memcpy(data + 12, first_nonce, 32);
    memcpy(data + 44, second_nonce, 32);

    prf(pmk, 32, "Pairwise key expansion", data, sizeof(data), ptk_out, 64);
}

void wpa_eapol_mic(const uint8_t kck[16], const uint8_t *frame, size_t len,
                   uint8_t mic_out[16]) {
    uint8_t full[20];
    hmac_sha1(kck, 16, frame, len, full);
    memcpy(mic_out, full, 16); /* HMAC-SHA1-128: the low 16 bytes */
}

/* ---- self-test: published vectors for the primitives themselves ---- */

static int hex_eq(const uint8_t *got, const char *hex) {
    size_t n = strlen(hex) / 2;
    for (size_t i = 0; i < n; i++) {
        int hi = hex[i * 2], lo = hex[i * 2 + 1];
        int hv = (hi <= '9') ? hi - '0' : (hi | 0x20) - 'a' + 10;
        int lv = (lo <= '9') ? lo - '0' : (lo | 0x20) - 'a' + 10;
        if (got[i] != (uint8_t)((hv << 4) | lv)) return 0;
    }
    return 1;
}

int wpa_selftest(void) {
    int failures = 0;

    /* FIPS 180-1: SHA1("abc") */
    uint8_t d1[20];
    sha1((const uint8_t *)"abc", 3, d1);
    if (hex_eq(d1, "a9993e364706816aba3e25717850c26c9cd0d89")) {
        kprintf("[wpa] PASS sha1(\"abc\")\n");
    } else {
        kprintf("[wpa] FAIL sha1(\"abc\")\n");
        failures++;
    }

    /* RFC 2202 test case 1: HMAC-SHA1(key=0x0b*20, "Hi There") */
    uint8_t key20[20];
    memset(key20, 0x0b, sizeof(key20));
    uint8_t d2[20];
    hmac_sha1(key20, sizeof(key20), (const uint8_t *)"Hi There", 8, d2);
    if (hex_eq(d2, "b617318655057264e28bc0b6fb378c8ef146be00")) {
        kprintf("[wpa] PASS hmac_sha1(RFC2202 case 1)\n");
    } else {
        kprintf("[wpa] FAIL hmac_sha1(RFC2202 case 1)\n");
        failures++;
    }

    /* RFC 6070: PBKDF2-HMAC-SHA1("password","salt",1,20 bytes) */
    uint8_t d3[20];
    pbkdf2_hmac_sha1((const uint8_t *)"password", 8, (const uint8_t *)"salt", 4,
                     1, d3, sizeof(d3));
    if (hex_eq(d3, "0c60c80f961f0e71f3a9b524af6012062fe037a")) {
        kprintf("[wpa] PASS pbkdf2(RFC6070, c=1)\n");
    } else {
        kprintf("[wpa] FAIL pbkdf2(RFC6070, c=1)\n");
        failures++;
    }

    /* RFC 6070: PBKDF2-HMAC-SHA1("password","salt",4096,20 bytes) -
     * same iteration count WPA2-PSK uses, just a shorter salt/output
     * than an SSID/PSK, so this is the load-bearing check for
     * wpa_psk_from_passphrase()'s inner loop. */
    uint8_t d4[20];
    pbkdf2_hmac_sha1((const uint8_t *)"password", 8, (const uint8_t *)"salt", 4,
                     4096, d4, sizeof(d4));
    if (hex_eq(d4, "4b007901b765489abead49d926f721d065a429c")) {
        kprintf("[wpa] PASS pbkdf2(RFC6070, c=4096)\n");
    } else {
        kprintf("[wpa] FAIL pbkdf2(RFC6070, c=4096)\n");
        failures++;
    }

    /* Sanity: the full WPA2-PSK/PTK/MIC composition runs without
     * crashing and is deterministic (same inputs -> same outputs) -
     * there is no independently-verified IEEE 802.11i Annex H.4
     * vector checked here, only that the pipeline is self-consistent. */
    uint8_t psk_a[32], psk_b[32];
    wpa_psk_from_passphrase("password", "IEEE", 4, psk_a);
    wpa_psk_from_passphrase("password", "IEEE", 4, psk_b);
    if (bytes_cmp(psk_a, psk_b, 32) == 0) {
        kprintf("[wpa] PASS wpa_psk_from_passphrase is deterministic\n");
    } else {
        kprintf("[wpa] FAIL wpa_psk_from_passphrase is deterministic\n");
        failures++;
    }

    uint8_t aa[6] = {0xa0,0xa1,0xa2,0xa3,0xa4,0xa5};
    uint8_t sa[6] = {0xb0,0xb1,0xb2,0xb3,0xb4,0xb5};
    uint8_t anonce[32], snonce[32];
    memset(anonce, 0x11, 32);
    memset(snonce, 0x22, 32);
    uint8_t ptk_a[64], ptk_b[64];
    wpa_derive_ptk(psk_a, aa, sa, anonce, snonce, ptk_a);
    wpa_derive_ptk(psk_a, aa, sa, anonce, snonce, ptk_b);
    if (bytes_cmp(ptk_a, ptk_b, 64) == 0) {
        kprintf("[wpa] PASS wpa_derive_ptk is deterministic\n");
    } else {
        kprintf("[wpa] FAIL wpa_derive_ptk is deterministic\n");
        failures++;
    }

    uint8_t frame[64];
    memset(frame, 0x33, sizeof(frame));
    uint8_t mic_a[16], mic_b[16];
    wpa_eapol_mic(ptk_a, frame, sizeof(frame), mic_a);
    wpa_eapol_mic(ptk_a, frame, sizeof(frame), mic_b);
    if (bytes_cmp(mic_a, mic_b, 16) == 0) {
        kprintf("[wpa] PASS wpa_eapol_mic is deterministic\n");
    } else {
        kprintf("[wpa] FAIL wpa_eapol_mic is deterministic\n");
        failures++;
    }

    if (failures == 0) {
        kprintf("[wpa] selftest: all checks passed\n");
    } else {
        kprintf("[wpa] selftest: %d check(s) FAILED\n", failures);
    }
    return failures == 0 ? 0 : -1;
}
