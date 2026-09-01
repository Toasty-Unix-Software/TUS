/*
 * sshbuf.c - see sshbuf.h
 *
 * The error flag is what makes this safe to use without checking
 * every call. A write that cannot grow the buffer sets it and then
 * discards data; a read past the end sets it and leaves the output
 * zeroed. Either way the caller sees a consistent, empty-ish result
 * and one flag to test, instead of a half-parsed message.
 */

#include "sshbuf.h"

#include <stdlib.h>
#include <string.h>

#define SSHBUF_MAX (1u << 24) /* 16 MiB: far past any packet we send */

void sshbuf_init(struct sshbuf *b) {
    b->data = NULL;
    b->len = b->cap = b->off = 0;
    b->error = 0;
}

void sshbuf_free(struct sshbuf *b) {
    if (b->data) {
        /* Buffers hold key material and plaintext; do not leave it in
         * the heap for the next allocation to inherit. */
        memset(b->data, 0, b->cap);
        free(b->data);
    }
    sshbuf_init(b);
}

void sshbuf_reset(struct sshbuf *b) {
    b->len = b->off = 0;
    b->error = 0;
}

void sshbuf_consume(struct sshbuf *b, size_t n) {
    if (n > b->len) n = b->len;
    memmove(b->data, b->data + n, b->len - n);
    b->len -= n;
    b->off = b->off > n ? b->off - n : 0;
}

int sshbuf_reserve(struct sshbuf *b, size_t extra) {
    if (b->error) return -1;
    if (b->len + extra <= b->cap) return 0;
    if (extra > SSHBUF_MAX || b->len + extra > SSHBUF_MAX) {
        b->error = 1;
        return -1;
    }

    size_t want = b->cap ? b->cap : 256;
    while (want < b->len + extra) want *= 2;

    uint8_t *p = realloc(b->data, want);
    if (!p) {
        b->error = 1;
        return -1;
    }
    b->data = p;
    b->cap = want;
    return 0;
}

/* ---- writing ---- */

void sshbuf_put(struct sshbuf *b, const void *data, size_t len) {
    if (sshbuf_reserve(b, len) != 0) return;
    memcpy(b->data + b->len, data, len);
    b->len += len;
}

void sshbuf_put_u8(struct sshbuf *b, uint8_t v) { sshbuf_put(b, &v, 1); }

void sshbuf_put_u32(struct sshbuf *b, uint32_t v) {
    uint8_t s[4] = { (uint8_t)(v >> 24), (uint8_t)(v >> 16),
                     (uint8_t)(v >> 8), (uint8_t)v };
    sshbuf_put(b, s, 4);
}

void sshbuf_put_u64(struct sshbuf *b, uint64_t v) {
    uint8_t s[8];
    for (int i = 0; i < 8; i++) s[i] = (uint8_t)(v >> (56 - 8 * i));
    sshbuf_put(b, s, 8);
}

void sshbuf_put_bool(struct sshbuf *b, int v) {
    sshbuf_put_u8(b, v ? 1 : 0);
}

void sshbuf_put_string(struct sshbuf *b, const void *data, size_t len) {
    sshbuf_put_u32(b, (uint32_t)len);
    sshbuf_put(b, data, len);
}

void sshbuf_put_cstring(struct sshbuf *b, const char *s) {
    sshbuf_put_string(b, s, s ? strlen(s) : 0);
}

void sshbuf_put_stringb(struct sshbuf *b, const struct sshbuf *v) {
    sshbuf_put_string(b, v->data, v->len);
}

void sshbuf_put_mpint(struct sshbuf *b, const uint8_t *bytes, size_t len) {
    size_t i = 0;
    while (i < len && bytes[i] == 0) i++; /* no leading zero bytes */

    if (i == len) {
        sshbuf_put_u32(b, 0); /* zero is the empty string */
        return;
    }

    int pad = (bytes[i] & 0x80) != 0; /* would read as negative */
    sshbuf_put_u32(b, (uint32_t)(len - i + (size_t)pad));
    if (pad) sshbuf_put_u8(b, 0);
    sshbuf_put(b, bytes + i, len - i);
}

/* ---- reading ---- */

int sshbuf_get(struct sshbuf *b, void *out, size_t len) {
    if (b->error || sshbuf_remaining(b) < len) {
        b->error = 1;
        if (out) memset(out, 0, len);
        return -1;
    }
    if (out) memcpy(out, b->data + b->off, len);
    b->off += len;
    return 0;
}

int sshbuf_get_u8(struct sshbuf *b, uint8_t *v) { return sshbuf_get(b, v, 1); }

int sshbuf_get_u32(struct sshbuf *b, uint32_t *v) {
    uint8_t s[4];
    if (sshbuf_get(b, s, 4) != 0) {
        *v = 0;
        return -1;
    }
    *v = ((uint32_t)s[0] << 24) | ((uint32_t)s[1] << 16) |
         ((uint32_t)s[2] << 8) | s[3];
    return 0;
}

int sshbuf_get_u64(struct sshbuf *b, uint64_t *v) {
    uint8_t s[8];
    if (sshbuf_get(b, s, 8) != 0) {
        *v = 0;
        return -1;
    }
    *v = 0;
    for (int i = 0; i < 8; i++) *v = (*v << 8) | s[i];
    return 0;
}

int sshbuf_get_bool(struct sshbuf *b, int *v) {
    uint8_t c;
    if (sshbuf_get_u8(b, &c) != 0) {
        *v = 0;
        return -1;
    }
    *v = c != 0;
    return 0;
}

int sshbuf_get_string(struct sshbuf *b, const uint8_t **data, size_t *len) {
    uint32_t n;
    if (sshbuf_get_u32(b, &n) != 0) goto fail;
    if (n > SSHBUF_MAX || sshbuf_remaining(b) < n) goto fail;

    if (data) *data = b->data + b->off;
    if (len) *len = n;
    b->off += n;
    return 0;

fail:
    b->error = 1;
    if (data) *data = NULL;
    if (len) *len = 0;
    return -1;
}

int sshbuf_get_cstring(struct sshbuf *b, const char **s, size_t *len) {
    const uint8_t *p;
    size_t n;

    if (sshbuf_get_string(b, &p, &n) != 0) {
        if (s) *s = NULL;
        if (len) *len = 0;
        return -1;
    }
    /* An embedded NUL would make strlen() and the wire length
     * disagree, which is how a name gets past a check that reads it
     * as C text. Reject it here rather than trust every caller. */
    if (memchr(p, 0, n) != NULL) {
        b->error = 1;
        if (s) *s = NULL;
        if (len) *len = 0;
        return -1;
    }
    if (s) *s = (const char *)p;
    if (len) *len = n;
    return 0;
}

int sshbuf_skip_string(struct sshbuf *b) {
    return sshbuf_get_string(b, NULL, NULL);
}

/* ---- name lists ---- */

/* Walk `list` one comma-separated name at a time. */
static const char *namelist_next(const char *p, const char *end, size_t *len) {
    if (p >= end) return NULL;
    const char *comma = memchr(p, ',', (size_t)(end - p));
    const char *stop = comma ? comma : end;
    *len = (size_t)(stop - p);
    return p;
}

int sshbuf_namelist_has(const char *list, size_t list_len, const char *name) {
    const char *end = list + list_len;
    size_t name_len = strlen(name);

    for (const char *p = list; p < end;) {
        size_t n;
        namelist_next(p, end, &n);
        if (n == name_len && memcmp(p, name, n) == 0) return 1;
        p += n + 1; /* step over the name and its comma */
    }
    return 0;
}

const char *sshbuf_namelist_first_match(const char *client, size_t client_len,
                                        const char *server, size_t server_len,
                                        char *out, size_t out_size) {
    const char *end = client + client_len;

    for (const char *p = client; p < end;) {
        size_t n;
        namelist_next(p, end, &n);
        if (n > 0 && n < out_size) {
            memcpy(out, p, n);
            out[n] = '\0';
            if (sshbuf_namelist_has(server, server_len, out)) return out;
        }
        p += n + 1;
    }
    out[0] = '\0';
    return NULL;
}

/* ---- base64 ---- */

static const char b64_alphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t sshbuf_b64_encode(const uint8_t *in, size_t in_len, char *out,
                         size_t out_size) {
    size_t need = ((in_len + 2) / 3) * 4;
    if (out_size < need + 1) {
        if (out_size) out[0] = '\0';
        return 0;
    }

    size_t o = 0;
    for (size_t i = 0; i < in_len; i += 3) {
        uint32_t v = (uint32_t)in[i] << 16;
        size_t have = in_len - i;
        if (have > 1) v |= (uint32_t)in[i + 1] << 8;
        if (have > 2) v |= in[i + 2];

        out[o++] = b64_alphabet[(v >> 18) & 63];
        out[o++] = b64_alphabet[(v >> 12) & 63];
        out[o++] = have > 1 ? b64_alphabet[(v >> 6) & 63] : '=';
        out[o++] = have > 2 ? b64_alphabet[v & 63] : '=';
    }
    out[o] = '\0';
    return o;
}

static int b64_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

long sshbuf_b64_decode(const char *in, size_t in_len, uint8_t *out,
                       size_t out_size) {
    uint32_t acc = 0;
    int bits = 0;
    size_t o = 0;

    for (size_t i = 0; i < in_len; i++) {
        char c = in[i];
        if (c == '=' ) break;
        /* Key files wrap at 70 columns, so whitespace is expected
         * anywhere and is not an error. */
        if (c == '\n' || c == '\r' || c == ' ' || c == '\t') continue;

        int v = b64_value(c);
        if (v < 0) return -1;

        acc = (acc << 6) | (uint32_t)v;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            if (o >= out_size) return -1;
            out[o++] = (uint8_t)(acc >> bits);
        }
    }
    return (long)o;
}
