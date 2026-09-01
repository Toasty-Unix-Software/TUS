/*
 * pipeline.c - vertices in, primitives out
 *
 * What happens to a vertex, in order:
 *
 *   1. the modelview matrix puts it in EYE space, which is where
 *      lighting is computed and where the eye is at the origin
 *   2. lighting, per vertex (Gouraud), if it is on
 *   3. the projection matrix puts it in CLIP space
 *   4. the vertex hook, if there is one, may rewrite any of that
 *   5. hglEnd assembles the primitives and clips them
 *   6. the perspective divide and the viewport transform put it in
 *      SCREEN space, and raster.c fills it in
 *
 * CLIPPING
 *
 * Only against the near plane, and that is a decision rather than an
 * omission. A vertex behind the eye has a negative w, and dividing by
 * it turns the triangle inside out - so the near plane MUST be
 * clipped or the picture is wrong. The other five planes only make a
 * triangle large, and a large triangle costs nothing here because the
 * rasteriser walks the intersection of its bounding box with the
 * viewport. Clipping them would generate vertices to save work that
 * is not being done.
 *
 * The one thing that buys is precision: a vertex a hair in front of
 * the near plane divides to an enormous screen coordinate. Hence
 * NEAR_EPSILON below, which pushes the clip plane a sliver in front
 * of w == 0 rather than exactly on it.
 */

#include "highgl.h"
#include "hgl_internal.h"

#include <math.h>
#include <string.h>

/* How far in front of the eye the near clip actually sits. Exactly at
 * w == 0 a vertex divides by zero; a hair in front of it and the
 * largest screen coordinate is bounded. */
#define NEAR_EPSILON 1e-5f

/* ---- lighting ----
 *
 * The fixed-function model, per vertex:
 *
 *   colour = global_ambient * material_ambient
 *          + sum over lights of
 *              attenuation * light_colour *
 *              (material_diffuse * max(0, N.L) +
 *               material_specular * max(0, N.H)^shininess)
 *
 * H is the halfway vector between the light and the eye, which is the
 * Blinn form - cheaper than reflecting the light vector and, at the
 * shininess values anyone actually uses, indistinguishable.
 */
static void apply_lighting(hgl_context *gl, struct hgl_vertex *v) {
    float nl = sqrtf(v->nx * v->nx + v->ny * v->ny + v->nz * v->nz);
    if (nl < 1e-8f) {
        return; /* no normal: leave the colour alone */
    }
    float nx = v->nx / nl, ny = v->ny / nl, nz = v->nz / nl;

    float r = gl->global_ambient[0] * gl->mat_ambient[0];
    float g = gl->global_ambient[1] * gl->mat_ambient[1];
    float b = gl->global_ambient[2] * gl->mat_ambient[2];

    /* The eye is at the origin in eye space, so the view direction is
     * simply the vertex position negated. */
    float vx = -v->ex, vy = -v->ey, vz = -v->ez;
    float vlen = sqrtf(vx * vx + vy * vy + vz * vz);
    if (vlen > 1e-8f) {
        vx /= vlen; vy /= vlen; vz /= vlen;
    }

    for (int i = 0; i < HGL_MAX_LIGHTS; i++) {
        const struct hgl_light *L = &gl->lights[i];
        if (!L->enabled) {
            continue;
        }

        float lx, ly, lz, att = 1.0f;
        if (L->w == 0.0f) {
            /* Directional: the position IS the direction, pointing
             * towards the light. */
            lx = L->x; ly = L->y; lz = L->z;
            float len = sqrtf(lx * lx + ly * ly + lz * lz);
            if (len < 1e-8f) {
                continue;
            }
            lx /= len; ly /= len; lz /= len;
        } else {
            lx = L->x - v->ex;
            ly = L->y - v->ey;
            lz = L->z - v->ez;
            float d = sqrtf(lx * lx + ly * ly + lz * lz);
            if (d < 1e-8f) {
                continue;
            }
            lx /= d; ly /= d; lz /= d;
            float denom = L->att_const + L->att_linear * d +
                          L->att_quad * d * d;
            att = denom > 1e-8f ? 1.0f / denom : 1.0f;
        }

        float ndl = nx * lx + ny * ly + nz * lz;
        if (ndl <= 0.0f) {
            continue; /* the face is turned away from this light */
        }

        float d = att * ndl;
        r += L->r * gl->mat_diffuse[0] * d;
        g += L->g * gl->mat_diffuse[1] * d;
        b += L->b * gl->mat_diffuse[2] * d;

        if (gl->mat_specular[0] > 0.0f || gl->mat_specular[1] > 0.0f ||
            gl->mat_specular[2] > 0.0f) {
            float hx = lx + vx, hy = ly + vy, hz = lz + vz;
            float hl = sqrtf(hx * hx + hy * hy + hz * hz);
            if (hl > 1e-8f) {
                float ndh = (nx * hx + ny * hy + nz * hz) / hl;
                if (ndh > 0.0f) {
                    float sp = att * powf(ndh, gl->mat_shininess);
                    r += L->r * gl->mat_specular[0] * sp;
                    g += L->g * gl->mat_specular[1] * sp;
                    b += L->b * gl->mat_specular[2] * sp;
                }
            }
        }
    }

    /* Clamped here rather than at the fragment: a colour above one
     * would be interpolated as such and clip differently across a
     * triangle, which shows up as a bright seam down its middle. */
    v->r = r > 1.0f ? 1.0f : r;
    v->g = g > 1.0f ? 1.0f : g;
    v->b = b > 1.0f ? 1.0f : b;
}

/* ---- immediate mode ---- */

void hglBegin(hgl_context *gl, int primitive) {
    if (gl == NULL) return;
    if (gl->in_begin) {
        hgl_error(gl, HGL_INVALID_OPERATION);
        return;
    }
    if (primitive < HGL_POINTS || primitive > HGL_QUADS) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    hgl_update_derived(gl);
    gl->primitive = primitive;
    gl->in_begin = 1;
    gl->nverts = 0;
}

void hglVertex4f(hgl_context *gl, float x, float y, float z, float w) {
    if (gl == NULL) return;
    if (!gl->in_begin) {
        hgl_error(gl, HGL_INVALID_OPERATION);
        return;
    }
    if (gl->nverts >= HGL_MAX_VERTICES) {
        /* Rather than grow without bound, the batch is flushed and
         * the caller carries on. A strip or a fan cannot be split
         * that way, so those keep their last two vertices. */
        hgl_flush_primitive(gl);
    }

    struct hgl_vertex *v = &gl->verts[gl->nverts++];
    memset(v, 0, sizeof(*v));

    const struct hgl_mat *mv = &gl->modelview.m[gl->modelview.depth];
    float in[4] = { x, y, z, w };
    float eye[4];
    hgl_mat_transform(mv, in, eye);
    v->ex = eye[0];
    v->ey = eye[1];
    v->ez = eye[2];

    v->r = gl->cur_r; v->g = gl->cur_g; v->b = gl->cur_b; v->a = gl->cur_a;
    v->s = gl->cur_s; v->t = gl->cur_t;

    if (gl->enabled[HGL_LIGHTING]) {
        float n[4] = { gl->cur_nx, gl->cur_ny, gl->cur_nz, 0.0f };
        float en[4];
        hgl_mat_transform(&gl->normal_matrix, n, en);
        v->nx = en[0]; v->ny = en[1]; v->nz = en[2];
        apply_lighting(gl, v);
    }

    float clip[4];
    hgl_mat_transform(&gl->mvp, in, clip);
    v->x = clip[0]; v->y = clip[1]; v->z = clip[2]; v->w = clip[3];

    if (gl->vhook != NULL) {
        gl->vhook(gl->vhook_ctx, v);
    }
    gl->stats.vertices++;
}

void hglVertex3f(hgl_context *gl, float x, float y, float z) {
    hglVertex4f(gl, x, y, z, 1.0f);
}

void hglVertex2f(hgl_context *gl, float x, float y) {
    hglVertex4f(gl, x, y, 0.0f, 1.0f);
}

void hglColor4f(hgl_context *gl, float r, float g, float b, float a) {
    if (gl == NULL) return;
    gl->cur_r = r; gl->cur_g = g; gl->cur_b = b; gl->cur_a = a;
}

void hglColor3f(hgl_context *gl, float r, float g, float b) {
    hglColor4f(gl, r, g, b, 1.0f);
}

void hglColor4ub(hgl_context *gl, uint8_t r, uint8_t g, uint8_t b,
                 uint8_t a) {
    hglColor4f(gl, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void hglTexCoord2f(hgl_context *gl, float s, float t) {
    if (gl == NULL) return;
    gl->cur_s = s; gl->cur_t = t;
}

void hglNormal3f(hgl_context *gl, float x, float y, float z) {
    if (gl == NULL) return;
    gl->cur_nx = x; gl->cur_ny = y; gl->cur_nz = z;
}

/* ---- clipping ---- */

static void lerp_vertex(struct hgl_vertex *out, const struct hgl_vertex *a,
                        const struct hgl_vertex *b, float t) {
    out->x = a->x + (b->x - a->x) * t;
    out->y = a->y + (b->y - a->y) * t;
    out->z = a->z + (b->z - a->z) * t;
    out->w = a->w + (b->w - a->w) * t;
    out->r = a->r + (b->r - a->r) * t;
    out->g = a->g + (b->g - a->g) * t;
    out->b = a->b + (b->b - a->b) * t;
    out->a = a->a + (b->a - a->a) * t;
    out->s = a->s + (b->s - a->s) * t;
    out->t = a->t + (b->t - a->t) * t;
    out->nx = a->nx + (b->nx - a->nx) * t;
    out->ny = a->ny + (b->ny - a->ny) * t;
    out->nz = a->nz + (b->nz - a->nz) * t;
    out->ex = a->ex + (b->ex - a->ex) * t;
    out->ey = a->ey + (b->ey - a->ey) * t;
    out->ez = a->ez + (b->ez - a->ez) * t;
}

/* Clip a polygon against w >= NEAR_EPSILON (Sutherland-Hodgman).
 * Returns the new vertex count; `out` must hold n + 1. */
static int clip_near(const struct hgl_vertex *in, int n,
                     struct hgl_vertex *out) {
    int m = 0;
    for (int i = 0; i < n; i++) {
        const struct hgl_vertex *a = &in[i];
        const struct hgl_vertex *b = &in[(i + 1) % n];
        float da = a->w - NEAR_EPSILON;
        float db = b->w - NEAR_EPSILON;

        if (da >= 0.0f) {
            out[m++] = *a;
        }
        if ((da >= 0.0f) != (db >= 0.0f)) {
            float t = da / (da - db);
            lerp_vertex(&out[m++], a, b, t);
        }
    }
    return m;
}

static void emit_triangle(hgl_context *gl, const struct hgl_vertex *a,
                          const struct hgl_vertex *b,
                          const struct hgl_vertex *c) {
    gl->stats.primitives++;

    /* All three in front of the near plane: the common case, and
     * worth not paying for a clip. */
    if (a->w >= NEAR_EPSILON && b->w >= NEAR_EPSILON &&
        c->w >= NEAR_EPSILON) {
        hgl_raster_triangle(gl, a, b, c);
        return;
    }

    struct hgl_vertex in[3] = { *a, *b, *c };
    struct hgl_vertex out[4];
    int n = clip_near(in, 3, out);
    if (n < 3) {
        gl->stats.primitives_clipped++;
        return; /* entirely behind the eye */
    }
    gl->stats.primitives_clipped++;
    /* A clipped triangle is a polygon of up to four vertices; fan it
     * back into triangles. */
    for (int i = 1; i + 1 < n; i++) {
        hgl_raster_triangle(gl, &out[0], &out[i], &out[i + 1]);
    }
}

static void emit_line(hgl_context *gl, const struct hgl_vertex *a,
                      const struct hgl_vertex *b) {
    gl->stats.primitives++;
    if (a->w >= NEAR_EPSILON && b->w >= NEAR_EPSILON) {
        hgl_raster_line(gl, a, b);
        return;
    }
    if (a->w < NEAR_EPSILON && b->w < NEAR_EPSILON) {
        gl->stats.primitives_clipped++;
        return;
    }
    /* One end behind the eye: move it onto the near plane. */
    struct hgl_vertex ca = *a, cb = *b;
    float da = a->w - NEAR_EPSILON;
    float db = b->w - NEAR_EPSILON;
    float t = da / (da - db);
    if (da < 0.0f) {
        lerp_vertex(&ca, a, b, t);
    } else {
        lerp_vertex(&cb, a, b, t);
    }
    gl->stats.primitives_clipped++;
    hgl_raster_line(gl, &ca, &cb);
}

/* ---- primitive assembly ---- */

void hgl_flush_primitive(hgl_context *gl) {
    const struct hgl_vertex *v = gl->verts;
    int n = gl->nverts;

    switch (gl->primitive) {
    case HGL_POINTS:
        for (int i = 0; i < n; i++) {
            gl->stats.primitives++;
            if (v[i].w >= NEAR_EPSILON) {
                hgl_raster_point(gl, &v[i]);
            } else {
                gl->stats.primitives_clipped++;
            }
        }
        break;

    case HGL_LINES:
        for (int i = 0; i + 1 < n; i += 2) {
            emit_line(gl, &v[i], &v[i + 1]);
        }
        break;

    case HGL_LINE_STRIP:
        for (int i = 0; i + 1 < n; i++) {
            emit_line(gl, &v[i], &v[i + 1]);
        }
        break;

    case HGL_LINE_LOOP:
        for (int i = 0; i + 1 < n; i++) {
            emit_line(gl, &v[i], &v[i + 1]);
        }
        if (n > 2) {
            emit_line(gl, &v[n - 1], &v[0]);
        }
        break;

    case HGL_TRIANGLES:
        for (int i = 0; i + 2 < n; i += 3) {
            emit_triangle(gl, &v[i], &v[i + 1], &v[i + 2]);
        }
        break;

    case HGL_TRIANGLE_STRIP:
        for (int i = 0; i + 2 < n; i++) {
            /* Every other triangle has its first two vertices
             * swapped, so that the winding - and therefore which face
             * is the front - stays the same along the strip. */
            if (i & 1) {
                emit_triangle(gl, &v[i + 1], &v[i], &v[i + 2]);
            } else {
                emit_triangle(gl, &v[i], &v[i + 1], &v[i + 2]);
            }
        }
        break;

    case HGL_TRIANGLE_FAN:
        for (int i = 1; i + 1 < n; i++) {
            emit_triangle(gl, &v[0], &v[i], &v[i + 1]);
        }
        break;

    case HGL_QUADS:
        for (int i = 0; i + 3 < n; i += 4) {
            emit_triangle(gl, &v[i], &v[i + 1], &v[i + 2]);
            emit_triangle(gl, &v[i], &v[i + 2], &v[i + 3]);
        }
        break;

    default:
        break;
    }

    /* A strip or a fan that overflowed keeps what the next triangle
     * needs; anything else starts empty. */
    if (gl->primitive == HGL_TRIANGLE_STRIP && n >= 2) {
        gl->verts[0] = v[n - 2];
        gl->verts[1] = v[n - 1];
        gl->nverts = 2;
    } else if (gl->primitive == HGL_TRIANGLE_FAN && n >= 2) {
        gl->verts[1] = v[n - 1];
        gl->nverts = 2;
    } else if (gl->primitive == HGL_LINE_STRIP && n >= 1) {
        gl->verts[0] = v[n - 1];
        gl->nverts = 1;
    } else {
        gl->nverts = 0;
    }
}

void hglEnd(hgl_context *gl) {
    if (gl == NULL) return;
    if (!gl->in_begin) {
        hgl_error(gl, HGL_INVALID_OPERATION);
        return;
    }
    hgl_flush_primitive(gl);
    gl->in_begin = 0;
    gl->nverts = 0;
}
