/*
 * texture.c - texture objects and sampling
 *
 * 2D only, RGBA only, no mipmaps. A software rasteriser that walked a
 * mip chain would spend more time choosing a level than it saves, and
 * the aliasing mipmaps exist to fix only shows up on surfaces
 * receding into a distance this library is not fast enough to fill.
 * Saying so is better than a `hglGenerateMipmap` that does nothing.
 */

#include "highgl.h"
#include "hgl_internal.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

void hglGenTextures(hgl_context *gl, int n, unsigned *out) {
    if (gl == NULL || out == NULL || n < 0) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    int made = 0;
    /* Name 0 is reserved and means "no texture", as in OpenGL, so the
     * search starts at 1. */
    for (int i = 1; i < HGL_MAX_TEXTURES && made < n; i++) {
        if (!gl->textures[i].used) {
            memset(&gl->textures[i], 0, sizeof(gl->textures[i]));
            gl->textures[i].used = 1;
            gl->textures[i].min_filter = HGL_LINEAR;
            gl->textures[i].mag_filter = HGL_LINEAR;
            gl->textures[i].wrap_s = HGL_REPEAT;
            gl->textures[i].wrap_t = HGL_REPEAT;
            out[made++] = (unsigned)i;
        }
    }
    /* Ran out: the names not filled in are left as they were, and the
     * caller finds out through hglGetError rather than by binding a
     * texture that is not there. */
    while (made < n) {
        out[made++] = 0;
        hgl_error(gl, HGL_OUT_OF_MEMORY);
    }
}

void hglDeleteTextures(hgl_context *gl, int n, const unsigned *names) {
    if (gl == NULL || names == NULL) {
        return;
    }
    for (int i = 0; i < n; i++) {
        unsigned t = names[i];
        if (t == 0 || t >= HGL_MAX_TEXTURES) {
            continue;
        }
        free(gl->textures[t].pixels);
        memset(&gl->textures[t], 0, sizeof(gl->textures[t]));
        if (gl->bound_texture == t) {
            gl->bound_texture = 0;
        }
    }
}

void hglBindTexture(hgl_context *gl, unsigned name) {
    if (gl == NULL) return;
    if (name >= HGL_MAX_TEXTURES) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    if (name != 0 && !gl->textures[name].used) {
        hgl_error(gl, HGL_INVALID_OPERATION);
        return;
    }
    gl->bound_texture = name;
}

void hglTexImage2D(hgl_context *gl, int w, int h, const uint32_t *pixels) {
    if (gl == NULL) return;
    if (gl->bound_texture == 0) {
        hgl_error(gl, HGL_INVALID_OPERATION); /* nothing to upload into */
        return;
    }
    if (w <= 0 || h <= 0 || w > 4096 || h > 4096 || pixels == NULL) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    struct hgl_texture *t = &gl->textures[gl->bound_texture];

    uint32_t *copy = malloc((size_t)w * h * sizeof(uint32_t));
    if (copy == NULL) {
        hgl_error(gl, HGL_OUT_OF_MEMORY);
        return;
    }
    memcpy(copy, pixels, (size_t)w * h * sizeof(uint32_t));

    /* The old image is freed only once the new one exists, so a
     * failed upload leaves the texture as it was rather than blank. */
    free(t->pixels);
    t->pixels = copy;
    t->w = w;
    t->h = h;
}

void hglTexParameter(hgl_context *gl, int min_filter, int mag_filter,
                     int wrap_s, int wrap_t) {
    if (gl == NULL) return;
    if (gl->bound_texture == 0) {
        hgl_error(gl, HGL_INVALID_OPERATION);
        return;
    }
    if (min_filter < HGL_NEAREST || min_filter > HGL_LINEAR ||
        mag_filter < HGL_NEAREST || mag_filter > HGL_LINEAR ||
        wrap_s < HGL_REPEAT || wrap_s > HGL_MIRRORED_REPEAT ||
        wrap_t < HGL_REPEAT || wrap_t > HGL_MIRRORED_REPEAT) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    struct hgl_texture *t = &gl->textures[gl->bound_texture];
    t->min_filter = min_filter;
    t->mag_filter = mag_filter;
    t->wrap_s = wrap_s;
    t->wrap_t = wrap_t;
}

/* Fold a texel coordinate back into the image. */
static int wrap(int v, int n, int mode) {
    if (n <= 0) {
        return 0;
    }
    switch (mode) {
    case HGL_CLAMP_TO_EDGE:
        return v < 0 ? 0 : (v >= n ? n - 1 : v);
    case HGL_MIRRORED_REPEAT: {
        int period = 2 * n;
        int m = v % period;
        if (m < 0) {
            m += period;
        }
        return m < n ? m : period - 1 - m;
    }
    default: { /* HGL_REPEAT */
        int m = v % n;
        return m < 0 ? m + n : m;
    }
    }
}

uint32_t hgl_sample(const hgl_context *gl, float s, float t) {
    /* An unbound texture samples as opaque white, so that modulating
     * by it leaves the colour alone. A program that enables texturing
     * and forgets to bind gets its untextured surface, not black. */
    if (gl->bound_texture == 0) {
        return 0xFFFFFFFFu;
    }
    const struct hgl_texture *tex = &gl->textures[gl->bound_texture];
    if (tex->pixels == NULL || tex->w <= 0 || tex->h <= 0) {
        return 0xFFFFFFFFu;
    }

    /* Texel centres sit at half-integer coordinates: the subtraction
     * of 0.5 is what stops a linear filter from being shifted half a
     * texel, which shows up as a texture that will not line up with
     * its own edges. */
    float fx = s * (float)tex->w - 0.5f;
    float fy = t * (float)tex->h - 0.5f;

    if (tex->mag_filter == HGL_NEAREST) {
        int x = wrap((int)floorf(fx + 0.5f), tex->w, tex->wrap_s);
        int y = wrap((int)floorf(fy + 0.5f), tex->h, tex->wrap_t);
        return tex->pixels[(size_t)y * tex->w + x];
    }

    int x0 = (int)floorf(fx), y0 = (int)floorf(fy);
    float ax = fx - (float)x0, ay = fy - (float)y0;

    int xi0 = wrap(x0, tex->w, tex->wrap_s);
    int xi1 = wrap(x0 + 1, tex->w, tex->wrap_s);
    int yi0 = wrap(y0, tex->h, tex->wrap_t);
    int yi1 = wrap(y0 + 1, tex->h, tex->wrap_t);

    uint32_t p00 = tex->pixels[(size_t)yi0 * tex->w + xi0];
    uint32_t p10 = tex->pixels[(size_t)yi0 * tex->w + xi1];
    uint32_t p01 = tex->pixels[(size_t)yi1 * tex->w + xi0];
    uint32_t p11 = tex->pixels[(size_t)yi1 * tex->w + xi1];

    /* Bilinear, in fixed point over 8-bit channels: four multiplies
     * per channel and no float conversion per texel. */
    unsigned wx = (unsigned)(ax * 256.0f);
    unsigned wy = (unsigned)(ay * 256.0f);
    if (wx > 256) wx = 256;
    if (wy > 256) wy = 256;
    unsigned w00 = (256 - wx) * (256 - wy);
    unsigned w10 = wx * (256 - wy);
    unsigned w01 = (256 - wx) * wy;
    unsigned w11 = wx * wy;

    uint32_t out = 0;
    for (int shift = 0; shift <= 24; shift += 8) {
        unsigned c = ((p00 >> shift) & 0xFF) * w00 +
                     ((p10 >> shift) & 0xFF) * w10 +
                     ((p01 >> shift) & 0xFF) * w01 +
                     ((p11 >> shift) & 0xFF) * w11;
        out |= ((c >> 16) & 0xFF) << shift;
    }
    return out;
}
