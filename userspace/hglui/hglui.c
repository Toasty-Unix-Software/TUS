#include "hglui.h"

#include <math.h>

static void set_ortho_2d(hgl_context *gl) {
    int w, h;
    hglGetSize(gl, &w, &h);
    hglMatrixMode(gl, HGL_PROJECTION);
    hglLoadIdentity(gl);
    /* Pixel-exact: clip-space x/y map straight onto buffer columns and
     * rows, which is what lets the fragment hooks below work in plain
     * pixel coordinates instead of NDC. */
    hglOrtho(gl, 0.0f, (float)w, (float)h, 0.0f, -1.0f, 1.0f);
    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);
}

static void unpack_rgb(uint32_t rgb, float *r, float *g, float *b) {
    *r = (float)((rgb >> 16) & 0xFF) / 255.0f;
    *g = (float)((rgb >> 8) & 0xFF) / 255.0f;
    *b = (float)(rgb & 0xFF) / 255.0f;
}

static void quad(hgl_context *gl, float x, float y, float w, float h) {
    hglBegin(gl, HGL_QUADS);
    hglVertex2f(gl, x, y);
    hglVertex2f(gl, x + w, y);
    hglVertex2f(gl, x + w, y + h);
    hglVertex2f(gl, x, y + h);
    hglEnd(gl);
}

static float clampf(float v, float lo, float hi) {
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

void hglui_rect(hgl_context *gl, float x, float y, float w, float h,
               uint32_t rgb) {
    set_ortho_2d(gl);
    hglDisable(gl, HGL_BLEND);
    float r, g, b;
    unpack_rgb(rgb, &r, &g, &b);
    hglColor3f(gl, r, g, b);
    quad(gl, x, y, w, h);
}

/* ---- rounded rectangle ----
 *
 * Inigo Quilez's rounded-box SDF: q = |p| - halfsize + radius, then
 * length(max(q,0)) + min(max(q.x,q.y),0) - radius. Negative inside,
 * positive outside, and the magnitude near zero is a real distance in
 * pixels - which is exactly what a soft, correctly-sized AA edge
 * needs.
 */
struct rr_ctx {
    float cx, cy, hw, hh, radius;
    float r, g, b;
    float alpha;
};

static int rr_hook(void *ctxp, struct hgl_fragment *f) {
    struct rr_ctx *c = (struct rr_ctx *)ctxp;
    float px = (float)f->x + 0.5f - c->cx;
    float py = (float)f->y + 0.5f - c->cy;
    float qx = fabsf(px) - c->hw + c->radius;
    float qy = fabsf(py) - c->hh + c->radius;
    float mqx = qx > 0.0f ? qx : 0.0f;
    float mqy = qy > 0.0f ? qy : 0.0f;
    float outside = sqrtf(mqx * mqx + mqy * mqy);
    float inside = qx > qy ? qx : qy;
    if (inside > 0.0f) {
        inside = 0.0f;
    }
    float dist = outside + inside - c->radius;

    float cov = clampf(0.5f - dist, 0.0f, 1.0f);
    if (cov <= 0.0f) {
        return 0; /* well outside the rounded rect: nothing to draw */
    }
    f->r = c->r;
    f->g = c->g;
    f->b = c->b;
    f->a = c->alpha * cov;
    return 1;
}

void hglui_rounded_rect(hgl_context *gl, float x, float y, float w, float h,
                        float radius, uint32_t rgb, uint8_t alpha) {
    set_ortho_2d(gl);
    hglEnable(gl, HGL_BLEND);
    hglBlendFunc(gl, HGL_SRC_ALPHA, HGL_ONE_MINUS_SRC_ALPHA);

    struct rr_ctx c;
    c.cx = x + w * 0.5f;
    c.cy = y + h * 0.5f;
    c.hw = w * 0.5f;
    c.hh = h * 0.5f;
    c.radius = radius;
    c.alpha = (float)alpha / 255.0f;
    unpack_rgb(rgb, &c.r, &c.g, &c.b);

    hglFragmentHook(gl, rr_hook, &c);
    hglColor4f(gl, 1.0f, 1.0f, 1.0f, 1.0f);
    quad(gl, x, y, w, h);
    hglFragmentHook(gl, NULL, NULL);
}

/* ---- circle ---- */

struct circ_ctx {
    float cx, cy, radius;
    float r, g, b;
    float alpha;
};

static int circ_hook(void *ctxp, struct hgl_fragment *f) {
    struct circ_ctx *c = (struct circ_ctx *)ctxp;
    float px = (float)f->x + 0.5f - c->cx;
    float py = (float)f->y + 0.5f - c->cy;
    float dist = sqrtf(px * px + py * py) - c->radius;

    float cov = clampf(0.5f - dist, 0.0f, 1.0f);
    if (cov <= 0.0f) {
        return 0;
    }
    f->r = c->r;
    f->g = c->g;
    f->b = c->b;
    f->a = c->alpha * cov;
    return 1;
}

void hglui_circle(hgl_context *gl, float cx, float cy, float r, uint32_t rgb,
                  uint8_t alpha) {
    set_ortho_2d(gl);
    hglEnable(gl, HGL_BLEND);
    hglBlendFunc(gl, HGL_SRC_ALPHA, HGL_ONE_MINUS_SRC_ALPHA);

    struct circ_ctx c;
    c.cx = cx;
    c.cy = cy;
    c.radius = r;
    c.alpha = (float)alpha / 255.0f;
    unpack_rgb(rgb, &c.r, &c.g, &c.b);

    hglFragmentHook(gl, circ_hook, &c);
    hglColor4f(gl, 1.0f, 1.0f, 1.0f, 1.0f);
    /* The quad only needs to cover the circle's bounding box; a
     * 1-pixel margin keeps the AA ring from being clipped by the quad
     * itself rather than by the hook's own coverage falloff. */
    quad(gl, cx - r - 1.0f, cy - r - 1.0f, 2.0f * (r + 1.0f), 2.0f * (r + 1.0f));
    hglFragmentHook(gl, NULL, NULL);
}

/* ---- text ----
 *
 * Straight alpha blend, integer arithmetic, one multiply and one
 * shift-free divide per channel - the same blend hxfont.c's demo
 * established, generalised to an arbitrary target buffer instead of
 * one window's fixed size.
 */
static void blend_px(uint32_t *buf, int buf_w, int buf_h, int x, int y,
                     uint32_t color, unsigned cov) {
    if (x < 0 || y < 0 || x >= buf_w || y >= buf_h || cov == 0) {
        return;
    }
    uint32_t *p = &buf[(size_t)y * buf_w + x];
    if (cov >= 255) {
        *p = color;
        return;
    }
    uint32_t dst = *p;
    unsigned inv = 255 - cov;
    unsigned r = (((color >> 16) & 0xFF) * cov + ((dst >> 16) & 0xFF) * inv) / 255;
    unsigned g = (((color >> 8) & 0xFF) * cov + ((dst >> 8) & 0xFF) * inv) / 255;
    unsigned b = ((color & 0xFF) * cov + (dst & 0xFF) * inv) / 255;
    *p = (r << 16) | (g << 8) | b;
}

struct text_ctx {
    uint32_t *buf;
    int buf_w, buf_h;
    uint32_t color;
};

static int text_glyph(void *ctxp, const struct tf_bitmap *bm, int x, int y) {
    struct text_ctx *t = (struct text_ctx *)ctxp;
    if (bm->pixels == NULL) {
        return 0; /* a space, or a glyph with no ink */
    }
    for (int row = 0; row < bm->h; row++) {
        const uint8_t *src = bm->pixels + (size_t)row * bm->stride;
        for (int col = 0; col < bm->w; col++) {
            blend_px(t->buf, t->buf_w, t->buf_h, x + col, y + row, t->color,
                    src[col]);
        }
    }
    return 0;
}

void hglui_text(uint32_t *buf, int buf_w, int buf_h, int x, int baseline_y,
                struct tf_font *font, int size_px, uint32_t rgb,
                const char *utf8) {
    if (font == NULL) {
        return;
    }
    struct text_ctx t;
    t.buf = buf;
    t.buf_w = buf_w;
    t.buf_h = buf_h;
    t.color = rgb & 0x00FFFFFFu;
    tf_text_draw(font, utf8, TF_FROM_INT(size_px), x, baseline_y, text_glyph,
                &t);
}

int hglui_text_width(struct tf_font *font, const char *utf8, int size_px) {
    if (font == NULL) {
        return 0;
    }
    return TF_TO_INT(tf_text_width(font, utf8, TF_FROM_INT(size_px)));
}
