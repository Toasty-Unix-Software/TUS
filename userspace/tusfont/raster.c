/*
 * raster.c - turning an outline into coverage
 *
 * THE METHOD
 *
 * Flatten the quadratic beziers into line segments, then fill with a
 * scanline sweep: for each of TF_SAMPLES horizontal sample lines
 * inside a pixel row, find where every edge crosses it, sort the
 * crossings, and walk them keeping a winding count. Where the winding
 * is non-zero, the shape is inside, and that span's coverage is added
 * to the row.
 *
 * Coverage is exact horizontally and sampled vertically. That split
 * is deliberate. A span's ends are almost never on a pixel boundary,
 * and rounding them is what makes unhinted text look ragged, so the
 * fractional pixel at each end gets its exact share. Vertically, a
 * pixel row is crossed by at most a couple of edges and sampling it a
 * few times is indistinguishable from integrating it - TF_SAMPLES is
 * 5, which puts the worst-case error below one part in ten and costs
 * five sorts per row instead of an analytic solve per edge.
 *
 * THE WINDING RULE
 *
 * Non-zero, which is what TrueType specifies. A counter in an 'o' is
 * an inner contour wound the other way, so the two windings cancel
 * and the middle stays empty. The even-odd rule would give the same
 * answer here and the wrong one for a glyph whose contours overlap -
 * an accented letter built as two overlapping shapes, which is most
 * of font_latin.h's ancestry.
 *
 * There is not a float anywhere: see the note in tusfont.h.
 */

#include "tusfont.h"
#include "ttf_internal.h"

#include <stdlib.h>
#include <string.h>

/* Sample lines per pixel row. Odd on purpose: an even count puts no
 * sample on the row's centre line, which is exactly where a
 * horizontal stem sits. */
#define TF_SAMPLES 5

/* How finely a bezier is chopped. A quadratic segment of a glyph at
 * a readable size is a few pixels long; eight pieces puts the error
 * well under a sixty-fourth of a pixel, which is the grid everything
 * is on anyway. Curvature is not measured: the check would cost more
 * than the extra segments. */
#define TF_CURVE_STEPS 8

/* An edge of the flattened outline, in 26.6 pixels. Horizontal edges
 * are dropped when they are built: they cross no sample line, and
 * keeping them means dividing by zero to find out. */
struct tf_edge {
    tf_fixed x0, y0, x1, y1;
    int dir; /* +1 if the edge points down the page, -1 if up */
};

struct tf_edges {
    struct tf_edge *v;
    int n, cap;
};

static int edges_push(struct tf_edges *e, tf_fixed x0, tf_fixed y0,
                      tf_fixed x1, tf_fixed y1) {
    if (y0 == y1) {
        return 1; /* horizontal: crosses nothing */
    }
    if (e->n == e->cap) {
        int cap = e->cap ? e->cap * 2 : 128;
        struct tf_edge *v = realloc(e->v, (size_t)cap * sizeof(*v));
        if (v == NULL) {
            return 0;
        }
        e->v = v;
        e->cap = cap;
    }
    struct tf_edge *ed = &e->v[e->n++];
    /* Stored top to bottom, with the original direction remembered:
     * the sweep only ever asks "does this edge span this y", and the
     * winding needs the direction the contour actually went. */
    if (y0 < y1) {
        ed->x0 = x0; ed->y0 = y0; ed->x1 = x1; ed->y1 = y1; ed->dir = 1;
    } else {
        ed->x0 = x1; ed->y0 = y1; ed->x1 = x0; ed->y1 = y0; ed->dir = -1;
    }
    return 1;
}

/* A quadratic bezier, chopped into straight pieces.
 *
 * The arithmetic is 26.6 throughout, and the products are taken in
 * 64 bits: a coordinate is up to a few thousand times 64, and
 * squaring that overflows a signed 32-bit multiply on a large glyph
 * at a large size. */
static int emit_quad(struct tf_edges *e, tf_fixed x0, tf_fixed y0,
                     tf_fixed cx, tf_fixed cy, tf_fixed x1, tf_fixed y1) {
    tf_fixed px = x0, py = y0;
    for (int i = 1; i <= TF_CURVE_STEPS; i++) {
        int32_t t = (i << 10) / TF_CURVE_STEPS;      /* 0..1024 */
        int32_t u = 1024 - t;
        int64_t a = (int64_t)u * u;                  /* 20 bits */
        int64_t b = 2LL * u * t;
        int64_t c = (int64_t)t * t;
        tf_fixed nx = (tf_fixed)((a * x0 + b * cx + c * x1) >> 20);
        tf_fixed ny = (tf_fixed)((a * y0 + b * cy + c * y1) >> 20);
        if (!edges_push(e, px, py, nx, ny)) {
            return 0;
        }
        px = nx;
        py = ny;
    }
    return 1;
}

/* Walk one contour, turning TrueType's on/off point sequence into
 * lines and quadratic curves.
 *
 * The two rules that matter:
 *   - a contour may START on an off-curve point, in which case the
 *     first on-curve point is somewhere later (or, if there is none
 *     at all, half way between the first two off-curve points)
 *   - two off-curve points in a row have an on-curve point implied
 *     exactly half way between them
 */
static int emit_contour(struct tf_edges *e, const struct tf_point *pts,
                        int n, const struct tf_font *f, tf_fixed size,
                        tf_fixed x_off, tf_fixed y_shift) {
    if (n < 2) {
        return 1;
    }

    /* Scale a font-unit point into 26.6 device space. */
    #define PX(i) (tf_scale(f, pts[i].x, size) + x_off)
    #define PY(i) (y_shift - tf_scale(f, pts[i].y, size))

    /* Find the starting on-curve point. */
    tf_fixed sx, sy;
    int start;
    if (pts[0].on) {
        sx = PX(0);
        sy = PY(0);
        start = 1;
    } else if (pts[n - 1].on) {
        sx = PX(n - 1);
        sy = PY(n - 1);
        start = 0;
        n--; /* the last point has become the first */
    } else {
        /* No on-curve point to start from: the midpoint of the first
         * and last control points is one, by the same rule that
         * implies midpoints anywhere else. */
        sx = (PX(0) + PX(n - 1)) / 2;
        sy = (PY(0) + PY(n - 1)) / 2;
        start = 0;
    }

    tf_fixed cur_x = sx, cur_y = sy;
    int have_ctrl = 0;
    tf_fixed ctrl_x = 0, ctrl_y = 0;

    for (int k = 0; k < n; k++) {
        int i = (start + k) % n;
        tf_fixed x = PX(i), y = PY(i);

        if (pts[i].on) {
            if (have_ctrl) {
                if (!emit_quad(e, cur_x, cur_y, ctrl_x, ctrl_y, x, y)) {
                    return 0;
                }
                have_ctrl = 0;
            } else if (!edges_push(e, cur_x, cur_y, x, y)) {
                return 0;
            }
            cur_x = x;
            cur_y = y;
        } else {
            if (have_ctrl) {
                /* Two controls in a row: the implied on-curve point
                 * between them ends the first curve and starts the
                 * next. */
                tf_fixed mx = (ctrl_x + x) / 2;
                tf_fixed my = (ctrl_y + y) / 2;
                if (!emit_quad(e, cur_x, cur_y, ctrl_x, ctrl_y, mx, my)) {
                    return 0;
                }
                cur_x = mx;
                cur_y = my;
            }
            ctrl_x = x;
            ctrl_y = y;
            have_ctrl = 1;
        }
    }

    /* Close the contour back to where it began. */
    if (have_ctrl) {
        if (!emit_quad(e, cur_x, cur_y, ctrl_x, ctrl_y, sx, sy)) {
            return 0;
        }
    } else if (!edges_push(e, cur_x, cur_y, sx, sy)) {
        return 0;
    }
    #undef PX
    #undef PY
    return 1;
}

/* One crossing of a sample line. */
struct tf_hit {
    tf_fixed x;
    int dir;
};

static int hit_compare(const void *a, const void *b) {
    tf_fixed xa = ((const struct tf_hit *)a)->x;
    tf_fixed xb = ((const struct tf_hit *)b)->x;
    return xa < xb ? -1 : (xa > xb ? 1 : 0);
}

/* Add coverage for the span [x0, x1) (26.6 pixels) into one row of
 * 8-bit accumulators, giving the partial pixels at each end their
 * exact share. `weight` is what a fully covered pixel gets from this
 * one sample line. */
static void add_span(uint8_t *row, int w, tf_fixed x0, tf_fixed x1,
                     int weight) {
    if (x1 <= x0) {
        return;
    }
    if (x0 < 0) {
        x0 = 0;
    }
    if (x1 > TF_FROM_INT(w)) {
        x1 = TF_FROM_INT(w);
    }
    if (x1 <= x0) {
        return;
    }

    int px0 = TF_TO_INT(x0);
    int px1 = TF_TO_INT(x1 - 1); /* last pixel the span actually touches */

    if (px0 == px1) {
        int part = (int)((x1 - x0) * weight / TF_ONE);
        int v = row[px0] + part;
        row[px0] = (uint8_t)(v > 255 ? 255 : v);
        return;
    }

    /* The first partial pixel. */
    tf_fixed first = TF_FROM_INT(px0 + 1) - x0;
    int v = row[px0] + (int)(first * weight / TF_ONE);
    row[px0] = (uint8_t)(v > 255 ? 255 : v);

    /* Whole pixels in the middle. */
    for (int x = px0 + 1; x < px1; x++) {
        v = row[x] + weight;
        row[x] = (uint8_t)(v > 255 ? 255 : v);
    }

    /* The last partial pixel. */
    tf_fixed last = x1 - TF_FROM_INT(px1);
    v = row[px1] + (int)(last * weight / TF_ONE);
    row[px1] = (uint8_t)(v > 255 ? 255 : v);
}

int tf_rasterise(struct tf_font *f, const struct tf_outline *o,
                 tf_fixed size, tf_fixed x_off, struct tf_bitmap *out) {
    memset(out, 0, sizeof(*out));
    if (o->ncontours == 0 || o->npoints == 0) {
        return 0; /* a space: no outline, and that is a success */
    }

    /* The bounding box in font units, so the bitmap is exactly as
     * large as the glyph and no larger. */
    int32_t min_x = o->points[0].x, max_x = min_x;
    int32_t min_y = o->points[0].y, max_y = min_y;
    for (int i = 1; i < o->npoints; i++) {
        if (o->points[i].x < min_x) min_x = o->points[i].x;
        if (o->points[i].x > max_x) max_x = o->points[i].x;
        if (o->points[i].y < min_y) min_y = o->points[i].y;
        if (o->points[i].y > max_y) max_y = o->points[i].y;
    }

    /* A curve can bulge past its control points' box by up to half a
     * pixel once flattened; one pixel of margin on each side is
     * cheaper than clipping a glyph's edge off. */
    tf_fixed fx0 = TF_FLOOR(tf_scale(f, min_x, size) + x_off) - TF_ONE;
    tf_fixed fx1 = TF_CEIL(tf_scale(f, max_x, size) + x_off) + TF_ONE;
    tf_fixed fy0 = TF_FLOOR(tf_scale(f, min_y, size)) - TF_ONE;
    tf_fixed fy1 = TF_CEIL(tf_scale(f, max_y, size)) + TF_ONE;

    int w = TF_TO_INT(fx1 - fx0);
    int h = TF_TO_INT(fy1 - fy0);
    if (w <= 0 || h <= 0 || w > 4096 || h > 4096) {
        return 0; /* degenerate or implausible: draw nothing */
    }

    /* Build the edges with the glyph moved so that its top left
     * corner is the bitmap's origin: y grows downward here and upward
     * in the font, hence the flip. */
    struct tf_edges edges;
    memset(&edges, 0, sizeof(edges));
    int first = 0;
    for (int c = 0; c < o->ncontours; c++) {
        int end = o->contour_end[c];
        if (end > o->npoints) {
            end = o->npoints;
        }
        if (end > first) {
            if (!emit_contour(&edges, o->points + first, end - first,
                              f, size, x_off - fx0, fy1)) {
                free(edges.v);
                return -1;
            }
        }
        first = end;
    }
    if (edges.n == 0) {
        free(edges.v);
        return 0;
    }

    uint8_t *pixels = calloc((size_t)w * h, 1);
    struct tf_hit *hits = malloc((size_t)edges.n * sizeof(*hits));
    if (pixels == NULL || hits == NULL) {
        free(pixels);
        free(hits);
        free(edges.v);
        return -1;
    }

    /* Each sample line contributes an equal share of a full pixel.
     * The shares are summed rather than each being 255/TF_SAMPLES, so
     * that TF_SAMPLES lines of full coverage reach exactly 255. */
    int weight = 255 / TF_SAMPLES;
    int extra = 255 - weight * TF_SAMPLES; /* handed to the first line */

    for (int row = 0; row < h; row++) {
        uint8_t *dst = pixels + (size_t)row * w;
        for (int s = 0; s < TF_SAMPLES; s++) {
            /* Sample at the centre of each of TF_SAMPLES bands. */
            tf_fixed y = TF_FROM_INT(row) +
                         (tf_fixed)((2 * s + 1) * TF_ONE / (2 * TF_SAMPLES));

            int nhits = 0;
            for (int i = 0; i < edges.n; i++) {
                const struct tf_edge *e = &edges.v[i];
                /* Half open: a vertex shared by two edges is counted
                 * once, which is what keeps a winding count from
                 * going wrong at every corner. */
                if (y < e->y0 || y >= e->y1) {
                    continue;
                }
                int64_t dy = e->y1 - e->y0;
                int64_t dx = (int64_t)(e->x1 - e->x0) * (y - e->y0);
                hits[nhits].x = e->x0 + (tf_fixed)(dx / dy);
                hits[nhits].dir = e->dir;
                nhits++;
            }
            if (nhits < 2) {
                continue;
            }
            qsort(hits, (size_t)nhits, sizeof(*hits), hit_compare);

            int winding = 0;
            tf_fixed span_start = 0;
            int this_weight = weight + (s == 0 ? extra : 0);
            for (int i = 0; i < nhits; i++) {
                int before = winding;
                winding += hits[i].dir;
                if (before == 0 && winding != 0) {
                    span_start = hits[i].x;
                } else if (before != 0 && winding == 0) {
                    add_span(dst, w, span_start, hits[i].x, this_weight);
                }
            }
        }
    }

    free(hits);
    free(edges.v);

    out->w = w;
    out->h = h;
    out->stride = w;
    out->pixels = pixels;
    /* Where the bitmap goes relative to the pen and the baseline. */
    out->left = TF_TO_INT(fx0);
    out->top = TF_TO_INT(fy1);
    return 0;
}

void tf_free_bitmap(struct tf_bitmap *bm) {
    if (bm == NULL) {
        return;
    }
    free(bm->pixels);
    memset(bm, 0, sizeof(*bm));
}
