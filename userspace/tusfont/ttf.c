/*
 * ttf.c - reading a TrueType font
 *
 * The file format, in one paragraph: a directory of tables, each
 * named by four characters, each at a byte offset with a length.
 * Everything is big endian. `loca` says where in `glyf` each glyph's
 * outline lives; an outline is a list of closed contours, and a
 * contour is a list of points, each flagged on-curve or off-curve.
 * Two on-curve points in a row are a straight line; an off-curve
 * point between them is the control point of a quadratic bezier; two
 * off-curve points in a row have an on-curve point implied exactly
 * half way between them, which is the one rule that surprises
 * everybody.
 *
 * See tusfont.h for what this reads and what it deliberately does
 * not.
 */

#include "tusfont.h"
#include "ttf_internal.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *g_error = "";

const char *tf_error(void) {
    return g_error;
}

static void set_error(const char *msg) {
    g_error = msg;
}

/* ---- big endian reads, bounds checked ----
 *
 * Every read goes through these. A font file is data from outside the
 * program - the whole point of a font manager is that a user can drop
 * one in - so a truncated or hostile file has to fail rather than
 * walk off the end of the buffer. `ok` latches: once a read has gone
 * out of bounds, every later read returns 0 and the caller's checks
 * see nonsense rather than reading somewhere else in memory. */

static uint8_t rd8(struct tf_reader *r, size_t off) {
    if (off + 1 > r->len) {
        r->ok = 0;
        return 0;
    }
    return r->data[off];
}

static uint16_t rd16(struct tf_reader *r, size_t off) {
    if (off + 2 > r->len) {
        r->ok = 0;
        return 0;
    }
    return (uint16_t)((r->data[off] << 8) | r->data[off + 1]);
}

static int16_t rd16s(struct tf_reader *r, size_t off) {
    return (int16_t)rd16(r, off);
}

static uint32_t rd32(struct tf_reader *r, size_t off) {
    if (off + 4 > r->len) {
        r->ok = 0;
        return 0;
    }
    return ((uint32_t)r->data[off] << 24) | ((uint32_t)r->data[off + 1] << 16) |
           ((uint32_t)r->data[off + 2] << 8) | (uint32_t)r->data[off + 3];
}

/* ---- the table directory ---- */

static int find_table(struct tf_font *f, const char *tag, size_t *off,
                      size_t *len) {
    uint16_t count = rd16(&f->r, 4);
    for (uint16_t i = 0; i < count; i++) {
        size_t rec = 12 + (size_t)i * 16;
        if (rec + 16 > f->r.len) {
            return 0;
        }
        if (memcmp(f->r.data + rec, tag, 4) == 0) {
            uint32_t o = rd32(&f->r, rec + 8);
            uint32_t l = rd32(&f->r, rec + 12);
            if ((size_t)o > f->r.len || (size_t)o + l > f->r.len) {
                return 0; /* a table that claims to be off the end */
            }
            *off = o;
            *len = l;
            return 1;
        }
    }
    return 0;
}

/* ---- cmap ----
 *
 * A font can carry several character maps; the one to use is the best
 * Unicode one it has. Format 12 covers all of Unicode, format 4
 * covers the Basic Multilingual Plane and is what almost every font
 * actually ships. Anything else is ignored, which costs nothing: a
 * font whose only cmap is a Macintosh 8-bit table has no business
 * being asked for a Turkish letter.
 */
static void pick_cmap(struct tf_font *f) {
    size_t off, len;
    if (!find_table(f, "cmap", &off, &len)) {
        return;
    }
    uint16_t n = rd16(&f->r, off + 2);
    size_t best = 0;
    int best_score = -1;

    for (uint16_t i = 0; i < n; i++) {
        size_t rec = off + 4 + (size_t)i * 8;
        uint16_t plat = rd16(&f->r, rec);
        uint16_t enc = rd16(&f->r, rec + 2);
        uint32_t sub = rd32(&f->r, rec + 4);
        size_t sub_off = off + sub;
        if (sub_off + 4 > f->r.len) {
            continue;
        }
        uint16_t format = rd16(&f->r, sub_off);

        int score = -1;
        if (plat == 3 && enc == 10 && format == 12) {
            score = 4; /* Windows, full Unicode */
        } else if (plat == 0 && format == 12) {
            score = 3;
        } else if (plat == 3 && enc == 1 && format == 4) {
            score = 2; /* Windows, BMP - the common case */
        } else if (plat == 0 && format == 4) {
            score = 1;
        }
        if (score > best_score) {
            best_score = score;
            best = sub_off;
            f->cmap_format = format;
        }
    }
    f->cmap = best;
}

/* Format 4: four parallel arrays of segments, searched linearly.
 * Binary search is what the format was designed for, but a font has
 * on the order of a hundred segments and this is called once per
 * character before the glyph cache takes over. */
static uint16_t cmap4_lookup(struct tf_font *f, uint32_t cp) {
    if (cp > 0xFFFF) {
        return 0;
    }
    size_t t = f->cmap;
    uint16_t seg_x2 = rd16(&f->r, t + 6);
    size_t ends = t + 14;
    size_t starts = ends + seg_x2 + 2;
    size_t deltas = starts + seg_x2;
    size_t ranges = deltas + seg_x2;

    for (uint16_t i = 0; i < seg_x2; i += 2) {
        uint16_t end = rd16(&f->r, ends + i);
        if (cp > end) {
            continue;
        }
        uint16_t start = rd16(&f->r, starts + i);
        if (cp < start) {
            return 0; /* segments are sorted: nothing later can match */
        }
        int16_t delta = rd16s(&f->r, deltas + i);
        uint16_t range = rd16(&f->r, ranges + i);
        if (range == 0) {
            return (uint16_t)(cp + (uint16_t)delta);
        }
        /* The offset is from the position of the offset itself, which
         * is the format's one genuinely awkward corner. */
        size_t at = ranges + i + range + (size_t)(cp - start) * 2;
        uint16_t g = rd16(&f->r, at);
        if (g == 0) {
            return 0;
        }
        return (uint16_t)(g + (uint16_t)delta);
    }
    return 0;
}

/* Format 12: a sorted list of (start, end, first glyph) groups. */
static uint16_t cmap12_lookup(struct tf_font *f, uint32_t cp) {
    size_t t = f->cmap;
    uint32_t groups = rd32(&f->r, t + 12);
    uint32_t lo = 0, hi = groups;
    while (lo < hi) {
        uint32_t mid = lo + (hi - lo) / 2;
        size_t g = t + 16 + (size_t)mid * 12;
        uint32_t start = rd32(&f->r, g);
        uint32_t end = rd32(&f->r, g + 4);
        if (cp < start) {
            hi = mid;
        } else if (cp > end) {
            lo = mid + 1;
        } else {
            return (uint16_t)(rd32(&f->r, g + 8) + (cp - start));
        }
    }
    return 0;
}

uint16_t tf_glyph_index(const struct tf_font *font, uint32_t cp) {
    struct tf_font *f = (struct tf_font *)font;
    if (f->cmap == 0) {
        return 0;
    }
    if (f->cmap_format == 12) {
        return cmap12_lookup(f, cp);
    }
    if (f->cmap_format == 4) {
        return cmap4_lookup(f, cp);
    }
    return 0;
}

uint16_t tf_glyph_count(const struct tf_font *font) {
    return font->num_glyphs;
}

/* ---- opening ---- */

struct tf_font *tf_load(void *data, size_t len, int own) {
    if (data == NULL || len < 12) {
        set_error("not a font: too short");
        return NULL;
    }

    struct tf_font *f = calloc(1, sizeof(*f));
    if (f == NULL) {
        set_error("out of memory");
        return NULL;
    }
    f->r.data = (const uint8_t *)data;
    f->r.len = len;
    f->r.ok = 1;
    f->owned = own ? data : NULL;

    uint32_t ver = rd32(&f->r, 0);
    /* 0x00010000 is TrueType; 'true' is what old Apple fonts use.
     * 'OTTO' is a font whose outlines are PostScript, which this
     * cannot draw, and saying so is better than drawing nothing. */
    if (ver == 0x4F54544Fu) {
        set_error("OpenType/CFF outlines are not supported (only glyf)");
        goto fail;
    }
    if (ver != 0x00010000u && ver != 0x74727565u) {
        set_error("not a TrueType font");
        goto fail;
    }

    size_t off, tlen;
    if (!find_table(f, "head", &off, &tlen) || tlen < 54) {
        set_error("no head table");
        goto fail;
    }
    f->units_per_em = rd16(&f->r, off + 18);
    f->loca_long = rd16s(&f->r, off + 50) != 0;
    if (f->units_per_em == 0) {
        set_error("head says zero units per em");
        goto fail;
    }

    if (!find_table(f, "maxp", &off, &tlen) || tlen < 6) {
        set_error("no maxp table");
        goto fail;
    }
    f->num_glyphs = rd16(&f->r, off + 4);

    if (!find_table(f, "hhea", &off, &tlen) || tlen < 36) {
        set_error("no hhea table");
        goto fail;
    }
    f->ascent = rd16s(&f->r, off + 4);
    f->descent = rd16s(&f->r, off + 6);
    f->line_gap = rd16s(&f->r, off + 8);
    f->num_hmetrics = rd16(&f->r, off + 34);

    if (!find_table(f, "hmtx", &f->hmtx, &f->hmtx_len)) {
        set_error("no hmtx table");
        goto fail;
    }
    if (!find_table(f, "loca", &f->loca, &f->loca_len)) {
        set_error("no loca table (a CFF font?)");
        goto fail;
    }
    if (!find_table(f, "glyf", &f->glyf, &f->glyf_len)) {
        set_error("no glyf table (a CFF font?)");
        goto fail;
    }
    /* kern is optional; a font without it simply does not kern. */
    if (!find_table(f, "kern", &f->kern, &f->kern_len)) {
        f->kern = 0;
    }

    pick_cmap(f);
    if (f->cmap == 0) {
        set_error("no usable cmap (need format 4 or 12)");
        goto fail;
    }

    if (!f->r.ok) {
        set_error("the file is truncated");
        goto fail;
    }
    return f;

fail:
    free(f);
    return NULL;
}

struct tf_font *tf_open(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        set_error("cannot open the file");
        return NULL;
    }
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        set_error("cannot measure the file");
        return NULL;
    }
    long size = ftell(fp);
    if (size <= 0 || size > 64 * 1024 * 1024) {
        fclose(fp);
        set_error("the file is empty or implausibly large");
        return NULL;
    }
    rewind(fp);

    void *buf = malloc((size_t)size);
    if (buf == NULL) {
        fclose(fp);
        set_error("out of memory");
        return NULL;
    }
    if (fread(buf, 1, (size_t)size, fp) != (size_t)size) {
        free(buf);
        fclose(fp);
        set_error("short read");
        return NULL;
    }
    fclose(fp);

    struct tf_font *f = tf_load(buf, (size_t)size, 1);
    if (f == NULL) {
        free(buf);
    }
    return f;
}

void tf_close(struct tf_font *font) {
    if (font == NULL) {
        return;
    }
    tf_cache_clear(font);
    free(font->owned);
    free(font);
}

/* ---- metrics ----
 *
 * Font units are scaled to pixels by size/unitsPerEm. Both are 26.6,
 * so the product is 12.12 and has to come back down by six bits. The
 * multiply is done in 64 bits: unitsPerEm is commonly 2048 and a
 * coordinate can be several times that, which overflows 32 bits at
 * any reasonable size.
 */
tf_fixed tf_scale(const struct tf_font *f, int32_t font_units,
                  tf_fixed size) {
    int64_t v = (int64_t)font_units * (int64_t)size;
    return (tf_fixed)(v / (int64_t)f->units_per_em);
}

void tf_metrics_at(const struct tf_font *font, tf_fixed size,
                   struct tf_metrics *out) {
    if (out == NULL) {
        return;
    }
    out->ascent = tf_scale(font, font->ascent, size);
    out->descent = tf_scale(font, -font->descent, size); /* stored negative */
    out->line_gap = tf_scale(font, font->line_gap, size);
    out->height = out->ascent + out->descent + out->line_gap;
}

tf_fixed tf_advance(const struct tf_font *font, uint16_t glyph,
                    tf_fixed size) {
    struct tf_font *f = (struct tf_font *)font;
    if (f->num_hmetrics == 0) {
        return 0;
    }
    /* hmtx has one entry per glyph up to num_hmetrics, and then only
     * left side bearings: every glyph past that point shares the last
     * advance. Monospaced fonts store exactly one entry. */
    uint16_t i = glyph < f->num_hmetrics ? glyph
                                         : (uint16_t)(f->num_hmetrics - 1);
    uint16_t adv = rd16(&f->r, f->hmtx + (size_t)i * 4);
    return tf_scale(f, adv, size);
}

/* ---- kerning ----
 *
 * Only format 0 (a sorted list of pairs), and only horizontal
 * subtables. That is what a `kern` table almost always contains;
 * anything else is skipped, and a font whose kerning is all in GPOS
 * simply does not kern here.
 */
tf_fixed tf_kern(const struct tf_font *font, uint16_t left, uint16_t right,
                 tf_fixed size) {
    struct tf_font *f = (struct tf_font *)font;
    if (f->kern == 0) {
        return 0;
    }
    uint16_t tables = rd16(&f->r, f->kern + 2);
    size_t at = f->kern + 4;
    uint32_t want = ((uint32_t)left << 16) | right;

    for (uint16_t t = 0; t < tables && f->r.ok; t++) {
        uint16_t length = rd16(&f->r, at + 2);
        uint16_t coverage = rd16(&f->r, at + 4);
        if ((coverage & 0xFF00) == 0 && (coverage & 0x0001) != 0) {
            /* format 0, horizontal */
            uint16_t pairs = rd16(&f->r, at + 6);
            size_t base = at + 14;
            uint32_t lo = 0, hi = pairs;
            while (lo < hi) {
                uint32_t mid = lo + (hi - lo) / 2;
                size_t p = base + (size_t)mid * 6;
                uint32_t key = ((uint32_t)rd16(&f->r, p) << 16) |
                               rd16(&f->r, p + 2);
                if (key == want) {
                    return tf_scale(f, rd16s(&f->r, p + 4), size);
                }
                if (key < want) {
                    lo = mid + 1;
                } else {
                    hi = mid;
                }
            }
        }
        if (length == 0) {
            break; /* a zero length would loop forever */
        }
        at += length;
    }
    return 0;
}

/* ---- outlines ---- */

/* Where glyph `g` lives in glyf, and how long it is. A zero length
 * means the glyph has no outline - a space - which is normal and not
 * an error. */
static int glyph_range(struct tf_font *f, uint16_t g, size_t *off,
                       size_t *len) {
    if (g >= f->num_glyphs) {
        return 0;
    }
    uint32_t a, b;
    if (f->loca_long) {
        a = rd32(&f->r, f->loca + (size_t)g * 4);
        b = rd32(&f->r, f->loca + (size_t)g * 4 + 4);
    } else {
        a = (uint32_t)rd16(&f->r, f->loca + (size_t)g * 2) * 2;
        b = (uint32_t)rd16(&f->r, f->loca + (size_t)g * 2 + 2) * 2;
    }
    if (!f->r.ok || b < a || a > f->glyf_len || b > f->glyf_len) {
        return 0;
    }
    *off = f->glyf + a;
    *len = b - a;
    return 1;
}

/* Simple glyph flags. */
#define ON_CURVE  0x01
#define X_SHORT   0x02
#define Y_SHORT   0x04
#define REPEAT    0x08
#define X_SAME    0x10  /* with X_SHORT: positive. Without: same as last */
#define Y_SAME    0x20

/* Composite glyph flags. */
#define ARGS_ARE_WORDS   0x0001
#define ARGS_ARE_XY      0x0002
#define HAVE_SCALE       0x0008
#define MORE_COMPONENTS  0x0020
#define HAVE_XY_SCALE    0x0040
#define HAVE_2X2         0x0080

/* F2Dot14 -> 16.16, for the transforms a composite glyph carries. */
static int32_t f2dot14(int16_t v) {
    return (int32_t)v << 2; /* 2.14 -> 16.16 */
}

static int load_outline(struct tf_font *f, uint16_t g,
                        struct tf_outline *out, int depth);

/* A composite glyph is other glyphs, each placed and possibly scaled.
 * `depth` stops a font whose components refer to each other from
 * recursing forever - a real thing in damaged fonts, and an easy
 * crash to hand an attacker. */
static int load_composite(struct tf_font *f, uint16_t self, size_t off,
                          size_t len, struct tf_outline *out, int depth) {
    size_t p = off + 10;
    size_t end = off + len;

    for (;;) {
        if (p + 4 > end) {
            return 0;
        }
        uint16_t flags = rd16(&f->r, p);
        uint16_t index = rd16(&f->r, p + 2);
        p += 4;

        int32_t dx = 0, dy = 0;
        if (flags & ARGS_ARE_WORDS) {
            if (p + 4 > end) return 0;
            dx = rd16s(&f->r, p);
            dy = rd16s(&f->r, p + 2);
            p += 4;
        } else {
            if (p + 2 > end) return 0;
            dx = (int8_t)rd8(&f->r, p);
            dy = (int8_t)rd8(&f->r, p + 1);
            p += 2;
        }
        if (!(flags & ARGS_ARE_XY)) {
            /* Point matching: the component is placed so two point
             * numbers coincide. Vanishingly rare, and getting it
             * wrong silently is worse than not placing the component
             * at all, so it is skipped with the offset left at zero. */
            dx = dy = 0;
        }

        int32_t a = 0x10000, b = 0, c = 0, d = 0x10000;
        if (flags & HAVE_SCALE) {
            if (p + 2 > end) return 0;
            a = d = f2dot14(rd16s(&f->r, p));
            p += 2;
        } else if (flags & HAVE_XY_SCALE) {
            if (p + 4 > end) return 0;
            a = f2dot14(rd16s(&f->r, p));
            d = f2dot14(rd16s(&f->r, p + 2));
            p += 4;
        } else if (flags & HAVE_2X2) {
            if (p + 8 > end) return 0;
            a = f2dot14(rd16s(&f->r, p));
            b = f2dot14(rd16s(&f->r, p + 2));
            c = f2dot14(rd16s(&f->r, p + 4));
            d = f2dot14(rd16s(&f->r, p + 6));
            p += 8;
        }

        /* A component that is this glyph, or nesting too deep, is a
         * damaged font trying to make the parser recurse forever.
         * Skipping the component draws a glyph missing a piece, which
         * is the right failure: the rest of the page still renders. */
        struct tf_outline part;
        memset(&part, 0, sizeof(part));
        if (index != self && depth < 5) {
            if (!load_outline(f, index, &part, depth + 1)) {
                tf_outline_free(&part);
                return 0;
            }
        }

        /* Transform and append. The multiply is 16.16 by font units,
         * so it comes back down by sixteen bits. */
        for (int i = 0; i < part.npoints; i++) {
            int32_t px = part.points[i].x;
            int32_t py = part.points[i].y;
            part.points[i].x =
                (int32_t)(((int64_t)a * px + (int64_t)c * py) >> 16) + dx;
            part.points[i].y =
                (int32_t)(((int64_t)b * px + (int64_t)d * py) >> 16) + dy;
        }
        if (!tf_outline_append(out, &part)) {
            tf_outline_free(&part);
            return 0;
        }
        tf_outline_free(&part);

        if (!(flags & MORE_COMPONENTS)) {
            break;
        }
    }
    return f->r.ok;
}

/* Read one glyph's contours, in FONT UNITS. Scaling happens later, in
 * the rasteriser, so a composite glyph's transform is applied at full
 * precision first. */
static int load_outline(struct tf_font *f, uint16_t g,
                        struct tf_outline *out, int depth) {
    size_t off, len;
    if (!glyph_range(f, g, &off, &len)) {
        return 0;
    }
    if (len == 0) {
        return 1; /* a space: no contours, and that is fine */
    }
    if (len < 10) {
        return 0;
    }

    int16_t ncontours = rd16s(&f->r, off);
    if (ncontours < 0) {
        return load_composite(f, g, off, len, out, depth);
    }
    if (ncontours == 0) {
        return 1;
    }

    size_t p = off + 10;
    int base_contour = out->ncontours;
    int base_point = out->npoints;

    /* End point of each contour, which also gives the point count. */
    if (!tf_outline_reserve_contours(out, out->ncontours + ncontours)) {
        return 0;
    }
    int npoints = 0;
    for (int i = 0; i < ncontours; i++) {
        uint16_t e = rd16(&f->r, p + (size_t)i * 2);
        out->contour_end[base_contour + i] = base_point + e + 1;
        npoints = e + 1;
    }
    out->ncontours += ncontours;
    p += (size_t)ncontours * 2;

    /* Skip the hinting bytecode. */
    uint16_t ilen = rd16(&f->r, p);
    p += 2 + ilen;

    if (npoints <= 0 || npoints > 10000) {
        return 0; /* a glyph with more points than any real one has */
    }
    if (!tf_outline_reserve_points(out, base_point + npoints)) {
        return 0;
    }

    /* Flags, run-length encoded: a flag with REPEAT set is followed
     * by a count of how many more points share it. */
    uint8_t *flags = malloc((size_t)npoints);
    if (flags == NULL) {
        return 0;
    }
    for (int i = 0; i < npoints; ) {
        uint8_t fl = rd8(&f->r, p++);
        flags[i++] = fl;
        if (fl & REPEAT) {
            uint8_t rep = rd8(&f->r, p++);
            while (rep-- > 0 && i < npoints) {
                flags[i++] = fl;
            }
        }
        if (!f->r.ok) {
            free(flags);
            return 0;
        }
    }

    /* X then Y, each a run of deltas from the point before. */
    int32_t v = 0;
    for (int i = 0; i < npoints; i++) {
        uint8_t fl = flags[i];
        if (fl & X_SHORT) {
            uint8_t d = rd8(&f->r, p++);
            v += (fl & X_SAME) ? d : -(int32_t)d;
        } else if (!(fl & X_SAME)) {
            v += rd16s(&f->r, p);
            p += 2;
        }
        out->points[base_point + i].x = v;
        out->points[base_point + i].on = (fl & ON_CURVE) ? 1 : 0;
    }
    v = 0;
    for (int i = 0; i < npoints; i++) {
        uint8_t fl = flags[i];
        if (fl & Y_SHORT) {
            uint8_t d = rd8(&f->r, p++);
            v += (fl & Y_SAME) ? d : -(int32_t)d;
        } else if (!(fl & Y_SAME)) {
            v += rd16s(&f->r, p);
            p += 2;
        }
        out->points[base_point + i].y = v;
    }
    free(flags);

    out->npoints = base_point + npoints;
    return f->r.ok;
}

int tf_load_outline(struct tf_font *f, uint16_t g, struct tf_outline *out) {
    memset(out, 0, sizeof(*out));
    if (!load_outline(f, g, out, 0)) {
        tf_outline_free(out);
        return 0;
    }
    return 1;
}

/* ---- outline storage ---- */

int tf_outline_reserve_points(struct tf_outline *o, int n) {
    if (n <= o->points_cap) {
        return 1;
    }
    int cap = o->points_cap ? o->points_cap * 2 : 64;
    while (cap < n) {
        cap *= 2;
    }
    struct tf_point *p = realloc(o->points, (size_t)cap * sizeof(*p));
    if (p == NULL) {
        return 0;
    }
    o->points = p;
    o->points_cap = cap;
    return 1;
}

int tf_outline_reserve_contours(struct tf_outline *o, int n) {
    if (n <= o->contours_cap) {
        return 1;
    }
    int cap = o->contours_cap ? o->contours_cap * 2 : 8;
    while (cap < n) {
        cap *= 2;
    }
    int *c = realloc(o->contour_end, (size_t)cap * sizeof(*c));
    if (c == NULL) {
        return 0;
    }
    o->contour_end = c;
    o->contours_cap = cap;
    return 1;
}

int tf_outline_append(struct tf_outline *dst, const struct tf_outline *src) {
    if (src->npoints == 0) {
        return 1;
    }
    if (!tf_outline_reserve_points(dst, dst->npoints + src->npoints) ||
        !tf_outline_reserve_contours(dst, dst->ncontours + src->ncontours)) {
        return 0;
    }
    memcpy(dst->points + dst->npoints, src->points,
           (size_t)src->npoints * sizeof(struct tf_point));
    for (int i = 0; i < src->ncontours; i++) {
        dst->contour_end[dst->ncontours + i] =
            src->contour_end[i] + dst->npoints;
    }
    dst->npoints += src->npoints;
    dst->ncontours += src->ncontours;
    return 1;
}

void tf_outline_free(struct tf_outline *o) {
    free(o->points);
    free(o->contour_end);
    memset(o, 0, sizeof(*o));
}
