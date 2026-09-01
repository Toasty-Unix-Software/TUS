/*
 * raster.c - filling triangles, lines and points
 *
 * THE METHOD
 *
 * Edge functions. For a triangle with vertices A, B, C in screen
 * space, the signed area of the sub-triangle (A, B, P) is a linear
 * function of P, and it is positive exactly when P is on the inside
 * of the edge AB. Three of those, one per edge, and a point is inside
 * the triangle when all three agree in sign. Each is affine, so
 * stepping one pixel to the right adds a constant - the whole inner
 * loop is three additions and three sign tests.
 *
 * The three edge functions also ARE the barycentric coordinates, once
 * divided by the triangle's total area, which is why nothing else has
 * to be set up to interpolate: colour, texture coordinates and depth
 * all come out of the same three numbers.
 *
 * PERSPECTIVE
 *
 * A vertex attribute is not linear in screen space - only the
 * attribute DIVIDED BY W is, along with 1/W itself. So both are
 * interpolated and divided back at the end. Depth is the exception:
 * after the perspective divide, z IS linear in screen space, which is
 * the whole reason a depth buffer stores it that way.
 *
 * Skipping this is the classic software-rasteriser bug: a textured
 * wall looks fine head on and its texture visibly bends as it turns
 * away.
 *
 * WHERE THE SIMD WILL GO
 *
 * The inner loop is written as a plain loop over a span with no
 * branches that depend on the pixel, so the compiler vectorises the
 * common cases (opaque, no texture, no hook). The per-pixel work is
 * deliberately kept in that shape rather than being hand-written in
 * intrinsics, so that widening it later is a matter of the flags the
 * file is compiled with rather than a rewrite.
 */

#include "highgl.h"
#include "hgl_internal.h"

#include <math.h>
#include <string.h>

/* A vertex once it has been divided through and put on the screen. */
struct screen_vertex {
    float x, y;      /* pixels */
    float z;         /* 0..1, ready for the depth buffer */
    float inv_w;     /* 1/w, interpolated with everything else */
    float r, g, b, a;
    float s, t;
};

static void project(const hgl_context *gl, const struct hgl_vertex *v,
                    struct screen_vertex *out) {
    float inv_w = 1.0f / v->w;
    float ndc_x = v->x * inv_w;
    float ndc_y = v->y * inv_w;
    float ndc_z = v->z * inv_w;

    out->x = (float)gl->vp_x + (ndc_x * 0.5f + 0.5f) * (float)gl->vp_w;
    /* Screen y grows downward and clip space y grows upward. */
    out->y = (float)gl->vp_y + (1.0f - (ndc_y * 0.5f + 0.5f)) *
             (float)gl->vp_h;
    out->z = ndc_z * 0.5f + 0.5f;
    out->inv_w = inv_w;

    /* Everything interpolated across the triangle is pre-divided by
     * w here and multiplied back by the interpolated w per pixel. */
    out->r = v->r * inv_w;
    out->g = v->g * inv_w;
    out->b = v->b * inv_w;
    out->a = v->a * inv_w;
    out->s = v->s * inv_w;
    out->t = v->t * inv_w;
}

static int depth_passes(int func, float src, float dst) {
    switch (func) {
    case HGL_NEVER:    return 0;
    case HGL_LESS:     return src < dst;
    case HGL_EQUAL:    return src == dst;
    case HGL_LEQUAL:   return src <= dst;
    case HGL_GREATER:  return src > dst;
    case HGL_NOTEQUAL: return src != dst;
    case HGL_GEQUAL:   return src >= dst;
    default:           return 1; /* HGL_ALWAYS */
    }
}

/* One blend factor, resolved to a per-channel triple. */
static void blend_factor(int factor, float sr, float sg, float sb, float sa,
                         float dr, float dg, float db, float da,
                         float *fr, float *fg, float *fb) {
    switch (factor) {
    case HGL_ZERO: *fr = *fg = *fb = 0.0f; break;
    case HGL_ONE:  *fr = *fg = *fb = 1.0f; break;
    case HGL_SRC_ALPHA: *fr = *fg = *fb = sa; break;
    case HGL_ONE_MINUS_SRC_ALPHA: *fr = *fg = *fb = 1.0f - sa; break;
    case HGL_DST_ALPHA: *fr = *fg = *fb = da; break;
    case HGL_ONE_MINUS_DST_ALPHA: *fr = *fg = *fb = 1.0f - da; break;
    case HGL_SRC_COLOR: *fr = sr; *fg = sg; *fb = sb; break;
    case HGL_ONE_MINUS_SRC_COLOR:
        *fr = 1.0f - sr; *fg = 1.0f - sg; *fb = 1.0f - sb; break;
    case HGL_DST_COLOR: *fr = dr; *fg = dg; *fb = db; break;
    case HGL_ONE_MINUS_DST_COLOR:
        *fr = 1.0f - dr; *fg = 1.0f - dg; *fb = 1.0f - db; break;
    default: *fr = *fg = *fb = 1.0f; break;
    }
}

static inline uint32_t pack_rgb(float r, float g, float b) {
    int ri = (int)(r * 255.0f + 0.5f);
    int gi = (int)(g * 255.0f + 0.5f);
    int bi = (int)(b * 255.0f + 0.5f);
    /* One statement per line: two guarded clauses on one line is how
     * a later edit ends up guarding only the first of them. */
    if (ri < 0) { ri = 0; }
    if (ri > 255) { ri = 255; }
    if (gi < 0) { gi = 0; }
    if (gi > 255) { gi = 255; }
    if (bi < 0) { bi = 0; }
    if (bi > 255) { bi = 255; }
    return ((uint32_t)ri << 16) | ((uint32_t)gi << 8) | (uint32_t)bi;
}

/* Write one fragment, applying the texture, the hook, the depth test
 * and the blend. Shared by triangles, lines and points so that all
 * three obey the same state - a line that ignored the depth buffer
 * would be a line that draws over the wall it is behind. */
static void put_fragment(hgl_context *gl, int x, int y, float z,
                         float r, float g, float b, float a,
                         float s, float t) {
    gl->stats.fragments++;

    if (gl->enabled[HGL_SCISSOR_TEST]) {
        if (x < gl->sc_x || x >= gl->sc_x + gl->sc_w ||
            y < gl->sc_y || y >= gl->sc_y + gl->sc_h) {
            return;
        }
    }
    if (x < 0 || y < 0 || x >= gl->width || y >= gl->height) {
        return;
    }

    if (gl->enabled[HGL_TEXTURE_2D]) {
        uint32_t texel = hgl_sample(gl, s, t);
        /* Modulate, which is the fixed pipeline's default and the one
         * that makes a lit textured surface look lit. */
        r *= (float)((texel >> 16) & 0xFF) / 255.0f;
        g *= (float)((texel >> 8) & 0xFF) / 255.0f;
        b *= (float)(texel & 0xFF) / 255.0f;
        a *= (float)((texel >> 24) & 0xFF) / 255.0f;
    }

    if (gl->fhook != NULL) {
        struct hgl_fragment f;
        f.x = x; f.y = y; f.depth = z;
        f.r = r; f.g = g; f.b = b; f.a = a;
        f.s = s; f.t = t;
        if (!gl->fhook(gl->fhook_ctx, &f)) {
            return; /* discarded */
        }
        z = f.depth;
        r = f.r; g = f.g; b = f.b; a = f.a;
    }

    size_t idx = (size_t)y * gl->width + x;

    if (gl->enabled[HGL_DEPTH_TEST]) {
        if (!depth_passes(gl->depth_func, z, gl->depth[idx])) {
            return;
        }
    }

    if (gl->enabled[HGL_BLEND]) {
        uint32_t d = gl->color[idx];
        float dr = (float)((d >> 16) & 0xFF) / 255.0f;
        float dg = (float)((d >> 8) & 0xFF) / 255.0f;
        float db = (float)(d & 0xFF) / 255.0f;
        float sfr, sfg, sfb, dfr, dfg, dfb;
        blend_factor(gl->blend_src, r, g, b, a, dr, dg, db, 1.0f,
                     &sfr, &sfg, &sfb);
        blend_factor(gl->blend_dst, r, g, b, a, dr, dg, db, 1.0f,
                     &dfr, &dfg, &dfb);
        r = r * sfr + dr * dfr;
        g = g * sfg + dg * dfg;
        b = b * sfb + db * dfb;
    }

    gl->color[idx] = pack_rgb(r, g, b);
    if (gl->enabled[HGL_DEPTH_TEST] && gl->depth_write) {
        gl->depth[idx] = z;
    }
    gl->stats.fragments_drawn++;
}

/* ---- triangles ---- */

/* Twice the signed area of (a, b, p). Positive when p is to the left
 * of a->b in a coordinate system whose y grows downward, which after
 * the y flip in project() means a counter-clockwise triangle in clip
 * space comes out positive here. */
static inline float edge(const struct screen_vertex *a,
                         const struct screen_vertex *b,
                         float px, float py) {
    return (b->x - a->x) * (py - a->y) - (b->y - a->y) * (px - a->x);
}

void hgl_raster_triangle(hgl_context *gl, const struct hgl_vertex *va,
                         const struct hgl_vertex *vb,
                         const struct hgl_vertex *vc) {
    struct screen_vertex a, b, c;
    project(gl, va, &a);
    project(gl, vb, &b);
    project(gl, vc, &c);

    float area = edge(&a, &b, c.x, c.y);
    if (area == 0.0f) {
        return; /* degenerate: no pixels either way */
    }

    /* Culling. The winding is decided before anything is flipped, so
     * that hglFrontFace means what it says. */
    if (gl->enabled[HGL_CULL_FACE]) {
        int front = (gl->front_face == HGL_CCW) ? (area > 0.0f)
                                                : (area < 0.0f);
        if (gl->cull_face == HGL_FRONT_AND_BACK ||
            (gl->cull_face == HGL_BACK && !front) ||
            (gl->cull_face == HGL_FRONT && front)) {
            gl->stats.primitives_culled++;
            return;
        }
    }

    /* From here on the three edge functions must be positive inside,
     * so a clockwise triangle has its vertices swapped rather than
     * its tests inverted - one branch here instead of three per
     * pixel. */
    if (area < 0.0f) {
        struct screen_vertex tmp = b;
        b = c;
        c = tmp;
        area = -area;
    }
    float inv_area = 1.0f / area;

    /* Flat shading takes the LAST vertex's colour, as OpenGL does. */
    int flat = (gl->shade_model == HGL_FLAT);
    float fr = c.r / c.inv_w, fg = c.g / c.inv_w;
    float fb = c.b / c.inv_w, fa = c.a / c.inv_w;

    /* The bounding box, clipped to whatever the frame actually has.
     * This is what makes clipping the side planes unnecessary: a
     * triangle a mile wide walks only the pixels that exist. */
    int min_x = (int)floorf(fminf(a.x, fminf(b.x, c.x)));
    int max_x = (int)ceilf(fmaxf(a.x, fmaxf(b.x, c.x)));
    int min_y = (int)floorf(fminf(a.y, fminf(b.y, c.y)));
    int max_y = (int)ceilf(fmaxf(a.y, fmaxf(b.y, c.y)));

    int lo_x = 0, lo_y = 0, hi_x = gl->width, hi_y = gl->height;
    if (gl->enabled[HGL_SCISSOR_TEST]) {
        lo_x = gl->sc_x; lo_y = gl->sc_y;
        hi_x = gl->sc_x + gl->sc_w; hi_y = gl->sc_y + gl->sc_h;
    }
    if (min_x < lo_x) min_x = lo_x;
    if (min_y < lo_y) min_y = lo_y;
    if (max_x > hi_x) max_x = hi_x;
    if (max_y > hi_y) max_y = hi_y;
    if (min_x >= max_x || min_y >= max_y) {
        return;
    }

    /* Edge functions are affine, so they are evaluated once at the
     * corner and stepped. */
    float px = (float)min_x + 0.5f, py = (float)min_y + 0.5f;
    float w0_row = edge(&b, &c, px, py);
    float w1_row = edge(&c, &a, px, py);
    float w2_row = edge(&a, &b, px, py);

    float w0_dx = -(c.y - b.y), w0_dy = (c.x - b.x);
    float w1_dx = -(a.y - c.y), w1_dy = (a.x - c.x);
    float w2_dx = -(b.y - a.y), w2_dy = (b.x - a.x);

    for (int y = min_y; y < max_y; y++) {
        float w0 = w0_row, w1 = w1_row, w2 = w2_row;

        for (int x = min_x; x < max_x; x++) {
            if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
                float l0 = w0 * inv_area;
                float l1 = w1 * inv_area;
                float l2 = w2 * inv_area;

                /* Depth is linear in screen space after the divide,
                 * so it interpolates directly. */
                float z = l0 * a.z + l1 * b.z + l2 * c.z;

                /* Everything else was pre-divided by w and comes back
                 * by the interpolated 1/w. */
                float iw = l0 * a.inv_w + l1 * b.inv_w + l2 * c.inv_w;
                float w = iw != 0.0f ? 1.0f / iw : 0.0f;

                float s = (l0 * a.s + l1 * b.s + l2 * c.s) * w;
                float t = (l0 * a.t + l1 * b.t + l2 * c.t) * w;

                if (flat) {
                    put_fragment(gl, x, y, z, fr, fg, fb, fa, s, t);
                } else {
                    float r = (l0 * a.r + l1 * b.r + l2 * c.r) * w;
                    float g = (l0 * a.g + l1 * b.g + l2 * c.g) * w;
                    float bl = (l0 * a.b + l1 * b.b + l2 * c.b) * w;
                    float al = (l0 * a.a + l1 * b.a + l2 * c.a) * w;
                    put_fragment(gl, x, y, z, r, g, bl, al, s, t);
                }
            }
            w0 += w0_dx; w1 += w1_dx; w2 += w2_dx;
        }
        w0_row += w0_dy; w1_row += w1_dy; w2_row += w2_dy;
    }
}

/* ---- lines ----
 *
 * A DDA rather than Bresenham: the attributes have to be interpolated
 * anyway, so stepping in floats costs nothing extra and keeps the
 * interpolation and the position on the same parameter.
 */
void hgl_raster_line(hgl_context *gl, const struct hgl_vertex *va,
                     const struct hgl_vertex *vb) {
    struct screen_vertex a, b;
    project(gl, va, &a);
    project(gl, vb, &b);

    float dx = b.x - a.x, dy = b.y - a.y;
    float adx = fabsf(dx), ady = fabsf(dy);
    int steps = (int)(adx > ady ? adx : ady);
    if (steps <= 0) {
        steps = 1;
    }
    if (steps > 1 << 16) {
        return; /* a line this long is a clipping failure, not a line */
    }

    int width = (int)(gl->line_width + 0.5f);
    if (width < 1) {
        width = 1;
    }

    for (int i = 0; i <= steps; i++) {
        float u = (float)i / (float)steps;
        float x = a.x + dx * u;
        float y = a.y + dy * u;
        float z = a.z + (b.z - a.z) * u;

        float iw = a.inv_w + (b.inv_w - a.inv_w) * u;
        float w = iw != 0.0f ? 1.0f / iw : 0.0f;
        float r = (a.r + (b.r - a.r) * u) * w;
        float g = (a.g + (b.g - a.g) * u) * w;
        float bl = (a.b + (b.b - a.b) * u) * w;
        float al = (a.a + (b.a - a.a) * u) * w;
        float s = (a.s + (b.s - a.s) * u) * w;
        float t = (a.t + (b.t - a.t) * u) * w;

        /* Width is applied across the line, not around the point: a
         * thick diagonal drawn as squares looks like a string of
         * beads. */
        int half = width / 2;
        for (int k = -half; k <= half - ((width & 1) ? 0 : 1); k++) {
            int px = (int)x, py = (int)y;
            if (adx > ady) {
                py += k;
            } else {
                px += k;
            }
            put_fragment(gl, px, py, z, r, g, bl, al, s, t);
        }
    }
}

/* ---- points ---- */

void hgl_raster_point(hgl_context *gl, const struct hgl_vertex *v) {
    struct screen_vertex p;
    project(gl, v, &p);

    int size = (int)(gl->point_size + 0.5f);
    if (size < 1) {
        size = 1;
    }
    int half = size / 2;

    float w = p.inv_w != 0.0f ? 1.0f / p.inv_w : 0.0f;
    float r = p.r * w, g = p.g * w, b = p.b * w, a = p.a * w;
    float s = p.s * w, t = p.t * w;

    int cx = (int)p.x, cy = (int)p.y;
    for (int dy = -half; dy <= half - ((size & 1) ? 0 : 1); dy++) {
        for (int dx = -half; dx <= half - ((size & 1) ? 0 : 1); dx++) {
            put_fragment(gl, cx + dx, cy + dy, p.z, r, g, b, a, s, t);
        }
    }
}
