/*
 * inflate.c - see inflate.h
 *
 * A straight reading of RFC 1951. Codes are decoded canonically, one
 * bit at a time, against a table of "how many codes of each length"
 * and "which symbols, in order" - the same shape zlib's own reference
 * decoder uses. A table-driven decoder would be faster; this one is
 * short enough to check by eye, and the streams Clint meets are a
 * page or an image, not a filesystem.
 *
 * The window is the output buffer itself: a back reference copies
 * from what has already been written, byte by byte, because the
 * lengths may overlap the distance (that is how a run of one byte is
 * encoded) and memcpy would be wrong.
 */

#include "inflate.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char g_error[128] = "no error";

const char *inflate_last_error(void) { return g_error; }

static int fail(const char *why) {
    size_t n = strlen(why);
    if (n >= sizeof(g_error)) n = sizeof(g_error) - 1;
    memcpy(g_error, why, n);
    g_error[n] = '\0';
    return -1;
}

/* ---- the bit reader ---- */

struct bits {
    const uint8_t *in;
    size_t len;
    size_t at;
    uint32_t buf;    /* bits read but not yet consumed */
    int cnt;
};

/* DEFLATE is little-endian within a byte, except for the Huffman
 * codes, which are read most significant bit first - handled in the
 * decoder rather than here. */
static int bits_get(struct bits *b, int n) {
    while (b->cnt < n) {
        if (b->at >= b->len) return -1;
        b->buf |= (uint32_t)b->in[b->at++] << b->cnt;
        b->cnt += 8;
    }
    int v = (int)(b->buf & ((1u << n) - 1u));
    b->buf >>= n;
    b->cnt -= n;
    return v;
}

/* ---- the output buffer ---- */

struct out {
    uint8_t *data;
    size_t len, cap, limit;
};

static int out_room(struct out *o, size_t extra) {
    if (o->len + extra <= o->cap) return 0;
    size_t want = o->cap ? o->cap : 16384;
    while (want < o->len + extra) want *= 2;
    if (want > o->limit) want = o->limit;
    if (o->len + extra > want) return fail("compressed stream is too large");
    uint8_t *p = realloc(o->data, want);
    if (p == NULL) return fail("out of memory");
    o->data = p;
    o->cap = want;
    return 0;
}

static int out_byte(struct out *o, uint8_t v) {
    if (out_room(o, 1) != 0) return -1;
    o->data[o->len++] = v;
    return 0;
}

/* ---- canonical Huffman ---- */

#define MAX_SYMBOLS 288

struct huff {
    uint16_t count[16];            /* codes of each length, 1..15 */
    uint16_t symbol[MAX_SYMBOLS];  /* symbols in canonical order */
};

static int huff_build(struct huff *h, const uint8_t *lens, int n) {
    memset(h->count, 0, sizeof(h->count));
    for (int i = 0; i < n; i++) h->count[lens[i]]++;
    h->count[0] = 0;

    /* An over-subscribed table means the data is not DEFLATE at all. */
    int left = 1;
    for (int len = 1; len <= 15; len++) {
        left <<= 1;
        left -= h->count[len];
        if (left < 0) return fail("bad Huffman table");
    }

    uint16_t offs[16];
    offs[1] = 0;
    for (int len = 1; len < 15; len++) {
        offs[len + 1] = (uint16_t)(offs[len] + h->count[len]);
    }
    for (int i = 0; i < n; i++) {
        if (lens[i] != 0) h->symbol[offs[lens[i]]++] = (uint16_t)i;
    }
    return 0;
}

static int huff_decode(struct bits *b, const struct huff *h) {
    int code = 0, first = 0, index = 0;
    for (int len = 1; len <= 15; len++) {
        int bit = bits_get(b, 1);
        if (bit < 0) return fail("compressed stream ends inside a code");
        code |= bit;
        int count = h->count[len];
        if (code - first < count) return h->symbol[index + (code - first)];
        index += count;
        first = (first + count) << 1;
        code <<= 1;
    }
    return fail("no Huffman code matches");
}

/* ---- the blocks ---- */

static const uint16_t LEN_BASE[29] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59,
    67, 83, 99, 115, 131, 163, 195, 227, 258
};
static const uint8_t LEN_EXTRA[29] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4,
    5, 5, 5, 5, 0
};
static const uint16_t DIST_BASE[30] = {
    1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513,
    769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
};
static const uint8_t DIST_EXTRA[30] = {
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10,
    11, 11, 12, 12, 13, 13
};

static int block_stored(struct bits *b, struct out *o) {
    b->buf = 0;
    b->cnt = 0;   /* stored blocks start on a byte boundary */
    if (b->at + 4 > b->len) return fail("truncated stored block");
    unsigned len = (unsigned)b->in[b->at] | ((unsigned)b->in[b->at + 1] << 8);
    b->at += 4;   /* LEN and its complement */
    if (b->at + len > b->len) return fail("truncated stored block");
    if (out_room(o, len) != 0) return -1;
    memcpy(o->data + o->len, b->in + b->at, len);
    o->len += len;
    b->at += len;
    return 0;
}

static int block_codes(struct bits *b, struct out *o, const struct huff *lit,
                       const struct huff *dist) {
    for (;;) {
        int sym = huff_decode(b, lit);
        if (sym < 0) return -1;

        if (sym < 256) {
            if (out_byte(o, (uint8_t)sym) != 0) return -1;
            continue;
        }
        if (sym == 256) return 0;   /* end of block */

        sym -= 257;
        if (sym >= 29) return fail("bad length code");
        int extra = bits_get(b, LEN_EXTRA[sym]);
        if (extra < 0) return fail("truncated length");
        int len = LEN_BASE[sym] + extra;

        int dsym = huff_decode(b, dist);
        if (dsym < 0) return -1;
        if (dsym >= 30) return fail("bad distance code");
        extra = bits_get(b, DIST_EXTRA[dsym]);
        if (extra < 0) return fail("truncated distance");
        size_t back = (size_t)DIST_BASE[dsym] + (size_t)extra;
        if (back > o->len) return fail("back reference before the start");

        if (out_room(o, (size_t)len) != 0) return -1;
        size_t from = o->len - back;
        for (int i = 0; i < len; i++) {
            o->data[o->len] = o->data[from + (size_t)i];
            o->len++;
        }
    }
}

static void fixed_tables(struct huff *lit, struct huff *dist) {
    uint8_t lens[MAX_SYMBOLS];
    int i = 0;
    for (; i < 144; i++) lens[i] = 8;
    for (; i < 256; i++) lens[i] = 9;
    for (; i < 280; i++) lens[i] = 7;
    for (; i < 288; i++) lens[i] = 8;
    huff_build(lit, lens, 288);

    for (i = 0; i < 30; i++) lens[i] = 5;
    huff_build(dist, lens, 30);
}

/* The lengths of the two code tables are themselves Huffman coded,
 * with the lengths of *that* table written out in a fixed order. */
static const uint8_t CLEN_ORDER[19] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

static int dynamic_tables(struct bits *b, struct huff *lit, struct huff *dist) {
    int nlen = bits_get(b, 5);
    int ndist = bits_get(b, 5);
    int ncode = bits_get(b, 4);
    if (nlen < 0 || ndist < 0 || ncode < 0) return fail("truncated header");
    nlen += 257;
    ndist += 1;
    ncode += 4;
    if (nlen > 286 || ndist > 30) return fail("too many codes");

    uint8_t clens[19];
    memset(clens, 0, sizeof(clens));
    for (int i = 0; i < ncode; i++) {
        int v = bits_get(b, 3);
        if (v < 0) return fail("truncated code lengths");
        clens[CLEN_ORDER[i]] = (uint8_t)v;
    }

    struct huff clen;
    if (huff_build(&clen, clens, 19) != 0) return -1;

    uint8_t lens[MAX_SYMBOLS + 30];
    memset(lens, 0, sizeof(lens));
    int at = 0;
    while (at < nlen + ndist) {
        int sym = huff_decode(b, &clen);
        if (sym < 0) return -1;

        if (sym < 16) {
            lens[at++] = (uint8_t)sym;
            continue;
        }

        int repeat, value = 0;
        if (sym == 16) {
            if (at == 0) return fail("nothing to repeat");
            value = lens[at - 1];
            repeat = bits_get(b, 2);
            if (repeat < 0) return fail("truncated repeat");
            repeat += 3;
        } else if (sym == 17) {
            repeat = bits_get(b, 3);
            if (repeat < 0) return fail("truncated repeat");
            repeat += 3;
        } else {
            repeat = bits_get(b, 7);
            if (repeat < 0) return fail("truncated repeat");
            repeat += 11;
        }
        if (at + repeat > nlen + ndist) return fail("repeat runs past the end");
        while (repeat-- > 0) lens[at++] = (uint8_t)value;
    }

    if (huff_build(lit, lens, nlen) != 0) return -1;
    if (huff_build(dist, lens + nlen, ndist) != 0) return -1;
    return 0;
}

int inflate_raw(const void *in, size_t in_len, size_t limit, void **out,
                size_t *out_len) {
    struct bits b = { (const uint8_t *)in, in_len, 0, 0, 0 };
    struct out o = { NULL, 0, 0, limit ? limit : (size_t)1 << 30 };

    for (;;) {
        int last = bits_get(&b, 1);
        int type = bits_get(&b, 2);
        if (last < 0 || type < 0) {
            fail("compressed stream ends without a final block");
            free(o.data);
            return -1;
        }

        int rc;
        if (type == 0) {
            rc = block_stored(&b, &o);
        } else if (type == 1) {
            struct huff lit, dist;
            fixed_tables(&lit, &dist);
            rc = block_codes(&b, &o, &lit, &dist);
        } else if (type == 2) {
            struct huff lit, dist;
            rc = dynamic_tables(&b, &lit, &dist);
            if (rc == 0) rc = block_codes(&b, &o, &lit, &dist);
        } else {
            rc = fail("reserved block type");
        }
        if (rc != 0) {
            free(o.data);
            return -1;
        }
        if (last) break;
    }

    /* A caller that wants to treat the result as text needs the room
     * for a terminator; giving it one costs a byte. */
    if (out_room(&o, 1) != 0) {
        free(o.data);
        return -1;
    }
    o.data[o.len] = '\0';

    *out = o.data;
    *out_len = o.len;
    return 0;
}

int inflate_zlib(const void *in, size_t in_len, size_t limit, void **out,
                 size_t *out_len) {
    const uint8_t *p = (const uint8_t *)in;
    if (in_len < 2) return fail("not a zlib stream");
    if ((p[0] & 0x0F) != 8) return fail("not deflate-compressed");
    if (((p[0] << 8) | p[1]) % 31 != 0) return fail("bad zlib header");
    if ((p[1] & 0x20) != 0) return fail("zlib preset dictionary");
    return inflate_raw(p + 2, in_len - 2, limit, out, out_len);
}

int inflate_gzip(const void *in, size_t in_len, size_t limit, void **out,
                 size_t *out_len) {
    const uint8_t *p = (const uint8_t *)in;
    if (in_len < 18) return fail("not a gzip stream");
    if (p[0] != 0x1F || p[1] != 0x8B || p[2] != 8) return fail("not gzip");

    uint8_t flags = p[3];
    size_t at = 10;
    if ((flags & 0x04) != 0) {   /* FEXTRA */
        if (at + 2 > in_len) return fail("truncated gzip header");
        size_t xlen = (size_t)p[at] | ((size_t)p[at + 1] << 8);
        at += 2 + xlen;
    }
    if ((flags & 0x08) != 0) {   /* FNAME */
        while (at < in_len && p[at] != 0) at++;
        at++;
    }
    if ((flags & 0x10) != 0) {   /* FCOMMENT */
        while (at < in_len && p[at] != 0) at++;
        at++;
    }
    if ((flags & 0x02) != 0) at += 2;  /* FHCRC */
    if (at >= in_len) return fail("truncated gzip header");

    return inflate_raw(p + at, in_len - at, limit, out, out_len);
}
