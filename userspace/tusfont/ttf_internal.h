/*
 * ttf_internal.h - what the three files of tusfont share
 *
 * Not a public header: a caller uses tusfont.h. This is the font
 * structure, the outline representation the parser produces and the
 * rasteriser consumes, and the glyph cache both ends touch.
 */

#ifndef TUS_FONT_INTERNAL_H
#define TUS_FONT_INTERNAL_H

#include "tusfont.h"

/* A bounds-checked view of the file. `ok` latches to zero on the
 * first out-of-range read and stays there, so a truncated font makes
 * every later read return 0 rather than reading somewhere else. */
struct tf_reader {
    const uint8_t *data;
    size_t len;
    int ok;
};

/* One outline point, in FONT UNITS. Scaling to pixels happens in the
 * rasteriser so that a composite glyph's transform is applied at full
 * precision first. */
struct tf_point {
    int32_t x, y;
    uint8_t on;   /* on the curve, as opposed to a control point */
};

/* A glyph's contours. contour_end[i] is one past the last point of
 * contour i, so contour i runs [contour_end[i-1], contour_end[i]). */
struct tf_outline {
    struct tf_point *points;
    int npoints, points_cap;
    int *contour_end;
    int ncontours, contours_cap;
};

/* One rasterised glyph, kept so that drawing the same text twice
 * rasterises nothing the second time. Keyed by glyph, size and the
 * sub-pixel offset it was rendered at. */
struct tf_cache_entry {
    uint16_t glyph;
    tf_fixed size;
    tf_fixed x_off;
    int used;
    struct tf_bitmap bm;
};

#define TF_CACHE_SIZE 256

struct tf_font {
    struct tf_reader r;
    void *owned;            /* the buffer to free, or NULL */

    uint16_t units_per_em;
    uint16_t num_glyphs;
    uint16_t num_hmetrics;
    int      loca_long;
    int16_t  ascent, descent, line_gap;

    size_t hmtx, hmtx_len;
    size_t loca, loca_len;
    size_t glyf, glyf_len;
    size_t kern, kern_len;
    size_t cmap;            /* the chosen subtable, 0 if none */
    uint16_t cmap_format;

    struct tf_cache_entry cache[TF_CACHE_SIZE];
    unsigned cache_clock;   /* round-robin eviction */
};

/* ttf.c */
int  tf_load_outline(struct tf_font *f, uint16_t g, struct tf_outline *out);
tf_fixed tf_scale(const struct tf_font *f, int32_t font_units, tf_fixed size);
int  tf_outline_reserve_points(struct tf_outline *o, int n);
int  tf_outline_reserve_contours(struct tf_outline *o, int n);
int  tf_outline_append(struct tf_outline *dst, const struct tf_outline *src);
void tf_outline_free(struct tf_outline *o);

/* raster.c */
int  tf_rasterise(struct tf_font *f, const struct tf_outline *o,
                  tf_fixed size, tf_fixed x_off, struct tf_bitmap *out);

/* text.c */
void tf_cache_clear(struct tf_font *f);
const struct tf_bitmap *tf_cache_get(struct tf_font *f, uint16_t glyph,
                                     tf_fixed size, tf_fixed x_off);

#endif /* TUS_FONT_INTERNAL_H */
