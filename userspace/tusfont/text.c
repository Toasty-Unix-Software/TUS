/*
 * text.c - the glyph cache and laying a string out
 *
 * Rasterising a glyph costs a sweep over its bounding box; drawing a
 * paragraph would do it once per letter per frame. The cache is a
 * fixed table keyed by (glyph, size, sub-pixel offset) - all three,
 * because the same glyph at the same size rendered a third of a pixel
 * to the right is a different bitmap and using the wrong one is how
 * text ends up shimmering as it scrolls.
 *
 * Eviction is round-robin rather than least-recently-used. Text is
 * drawn in reading order and comes back to the same few dozen glyphs,
 * so an LRU's bookkeeping would buy nothing that a big enough table
 * does not already give.
 */

#include "tusfont.h"
#include "ttf_internal.h"

#include <stdlib.h>
#include <string.h>

int tf_utf8_next(const char *s, uint32_t *cp) {
    const uint8_t *p = (const uint8_t *)s;
    uint8_t b = p[0];

    if (b < 0x80) {
        *cp = b;
        return 1;
    }

    int extra;
    uint32_t v;
    uint32_t least;
    if ((b & 0xE0) == 0xC0) {
        v = b & 0x1Fu; extra = 1; least = 0x80;
    } else if ((b & 0xF0) == 0xE0) {
        v = b & 0x0Fu; extra = 2; least = 0x800;
    } else if ((b & 0xF8) == 0xF0) {
        v = b & 0x07u; extra = 3; least = 0x10000;
    } else {
        /* A continuation byte with no lead, or 0xF8..0xFF. Consume
         * exactly one byte: a decoder that consumed more could be
         * walked past the end of the string by a malformed one. */
        *cp = 0xFFFD;
        return 1;
    }

    for (int i = 1; i <= extra; i++) {
        if ((p[i] & 0xC0) != 0x80) {
            *cp = 0xFFFD;
            return 1; /* truncated: one byte, and try again from the next */
        }
        v = (v << 6) | (p[i] & 0x3Fu);
    }
    if (v < least || (v >= 0xD800 && v <= 0xDFFF) || v > 0x10FFFF) {
        *cp = 0xFFFD; /* overlong, surrogate, or past the end of Unicode */
        return extra + 1;
    }
    *cp = v;
    return extra + 1;
}

/* ---- the cache ---- */

void tf_cache_clear(struct tf_font *f) {
    for (int i = 0; i < TF_CACHE_SIZE; i++) {
        if (f->cache[i].used) {
            free(f->cache[i].bm.pixels);
            memset(&f->cache[i], 0, sizeof(f->cache[i]));
        }
    }
}

const struct tf_bitmap *tf_cache_get(struct tf_font *f, uint16_t glyph,
                                     tf_fixed size, tf_fixed x_off) {
    /* Sub-pixel positions are quantised to quarters of a pixel. Any
     * finer and the cache would hold four times as many bitmaps for a
     * difference the eye cannot find; any coarser and letters visibly
     * snap to the grid as a line scrolls. */
    x_off = (x_off & 63) & ~15;

    unsigned h = ((unsigned)glyph * 2654435761u) ^
                 ((unsigned)size * 40503u) ^ ((unsigned)x_off * 2246822519u);
    unsigned slot = h % TF_CACHE_SIZE;

    /* Open addressing, a short probe: a miss costs a rasterisation
     * anyway, so a long probe would be pure loss. */
    for (unsigned i = 0; i < 8; i++) {
        struct tf_cache_entry *e = &f->cache[(slot + i) % TF_CACHE_SIZE];
        if (e->used && e->glyph == glyph && e->size == size &&
            e->x_off == x_off) {
            return &e->bm;
        }
    }

    struct tf_outline o;
    if (!tf_load_outline(f, glyph, &o)) {
        return NULL;
    }
    struct tf_bitmap bm;
    int rc = tf_rasterise(f, &o, size, x_off, &bm);
    tf_outline_free(&o);
    if (rc != 0) {
        return NULL;
    }

    /* Take the first free slot in the probe, or evict one. */
    struct tf_cache_entry *victim = NULL;
    for (unsigned i = 0; i < 8; i++) {
        struct tf_cache_entry *e = &f->cache[(slot + i) % TF_CACHE_SIZE];
        if (!e->used) {
            victim = e;
            break;
        }
    }
    if (victim == NULL) {
        victim = &f->cache[(slot + (f->cache_clock++ & 7)) % TF_CACHE_SIZE];
        free(victim->bm.pixels);
    }

    victim->used = 1;
    victim->glyph = glyph;
    victim->size = size;
    victim->x_off = x_off;
    victim->bm = bm;
    return &victim->bm;
}

int tf_render_glyph(struct tf_font *font, uint16_t glyph, tf_fixed size,
                    tf_fixed x_off, struct tf_bitmap *out) {
    struct tf_outline o;
    memset(out, 0, sizeof(*out));
    if (!tf_load_outline(font, glyph, &o)) {
        return -1;
    }
    int rc = tf_rasterise(font, &o, size, x_off & 63, out);
    tf_outline_free(&o);
    return rc;
}

/* ---- layout ---- */

tf_fixed tf_text_width(struct tf_font *font, const char *utf8,
                       tf_fixed size) {
    if (font == NULL || utf8 == NULL) {
        return 0;
    }
    tf_fixed x = 0;
    uint16_t prev = 0;

    for (const char *p = utf8; *p != '\0'; ) {
        uint32_t cp;
        p += tf_utf8_next(p, &cp);
        uint16_t g = tf_glyph_index(font, cp);
        if (prev != 0) {
            x += tf_kern(font, prev, g, size);
        }
        x += tf_advance(font, g, size);
        prev = g;
    }
    return x;
}

int tf_text_draw(struct tf_font *font, const char *utf8, tf_fixed size,
                 int x, int baseline_y, tf_draw_fn fn, void *ctx) {
    if (font == NULL || utf8 == NULL || fn == NULL) {
        return -1;
    }

    /* The pen keeps its fractional part across the whole string. That
     * is the difference between text that is spaced correctly and
     * text whose letters each land on a whole pixel: rounding every
     * advance would accumulate up to half a pixel of error per letter
     * and visibly stretch a long line. */
    tf_fixed pen = TF_FROM_INT(x);
    uint16_t prev = 0;

    for (const char *p = utf8; *p != '\0'; ) {
        uint32_t cp;
        p += tf_utf8_next(p, &cp);
        uint16_t g = tf_glyph_index(font, cp);

        if (prev != 0) {
            pen += tf_kern(font, prev, g, size);
        }

        const struct tf_bitmap *bm = tf_cache_get(font, g, size, pen & 63);
        if (bm != NULL && bm->pixels != NULL) {
            if (fn(ctx, bm, TF_TO_INT(pen) + bm->left,
                   baseline_y - bm->top) != 0) {
                return TF_TO_INT(pen);
            }
        }

        pen += tf_advance(font, g, size);
        prev = g;
    }
    return TF_TO_INT(pen);
}
