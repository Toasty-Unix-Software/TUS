/*
 * context.c - the HighGL context, its state, and the matrix stacks
 *
 * Everything here is bookkeeping: allocating the buffers, holding the
 * state a caller sets, and the matrix arithmetic. The interesting
 * work is in pipeline.c and raster.c.
 *
 * One decision worth naming: the projection and modelview matrices
 * are concatenated into `mvp` whenever either changes, not once per
 * vertex. A matrix multiply is sixty-four multiplies; doing it per
 * vertex would be three quarters of the transform cost of a scene,
 * spent recomputing a value that did not change.
 */

#include "highgl.h"
#include "hgl_internal.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---- matrices ---- */

void hgl_mat_identity(struct hgl_mat *m) {
    memset(m->m, 0, sizeof(m->m));
    m->m[0] = m->m[5] = m->m[10] = m->m[15] = 1.0f;
}

/* out = a * b, column major. Written with the indices spelled out
 * rather than as two loops: this is the hottest arithmetic in the
 * library that is not per-fragment, and the compiler vectorises the
 * flat form. */
void hgl_mat_mul(struct hgl_mat *out, const struct hgl_mat *a,
                 const struct hgl_mat *b) {
    struct hgl_mat r;
    for (int c = 0; c < 4; c++) {
        const float b0 = b->m[c * 4 + 0];
        const float b1 = b->m[c * 4 + 1];
        const float b2 = b->m[c * 4 + 2];
        const float b3 = b->m[c * 4 + 3];
        for (int row = 0; row < 4; row++) {
            r.m[c * 4 + row] = a->m[0 * 4 + row] * b0 +
                               a->m[1 * 4 + row] * b1 +
                               a->m[2 * 4 + row] * b2 +
                               a->m[3 * 4 + row] * b3;
        }
    }
    *out = r;
}

void hgl_mat_transform(const struct hgl_mat *m, const float in[4],
                       float out[4]) {
    for (int row = 0; row < 4; row++) {
        out[row] = m->m[0 * 4 + row] * in[0] +
                   m->m[1 * 4 + row] * in[1] +
                   m->m[2 * 4 + row] * in[2] +
                   m->m[3 * 4 + row] * in[3];
    }
}

struct hgl_mat_stack *hgl_current_stack(hgl_context *gl) {
    switch (gl->matrix_mode) {
    case HGL_PROJECTION: return &gl->projection;
    case HGL_TEXTURE:    return &gl->texture;
    default:             return &gl->modelview;
    }
}

/* Rebuild what depends on the two matrices: the combined transform,
 * and the matrix normals go through.
 *
 * Normals need the inverse transpose of the modelview's upper 3x3.
 * Computing a general inverse per frame would be silly for the
 * transforms anyone actually builds - a rotation and a translation,
 * sometimes a uniform scale - so the rotation part is used directly
 * and the normal is renormalised afterwards, which cancels any
 * uniform scale exactly. A NON-uniform scale is the case this gets
 * wrong, and the honest answer is to say so rather than to pay for a
 * general inverse on every frame that does not need one. */
void hgl_update_derived(hgl_context *gl) {
    if (!gl->mvp_dirty) {
        return;
    }
    const struct hgl_mat *mv = &gl->modelview.m[gl->modelview.depth];
    const struct hgl_mat *pr = &gl->projection.m[gl->projection.depth];
    hgl_mat_mul(&gl->mvp, pr, mv);

    gl->normal_matrix = *mv;
    /* Drop the translation: a direction has no position. */
    gl->normal_matrix.m[12] = 0.0f;
    gl->normal_matrix.m[13] = 0.0f;
    gl->normal_matrix.m[14] = 0.0f;
    gl->normal_matrix.m[15] = 1.0f;

    gl->mvp_dirty = 0;
}

/* ---- context ---- */

hgl_context *hglCreateContext(int width, int height) {
    if (width <= 0 || height <= 0 || width > 8192 || height > 8192) {
        return NULL;
    }
    hgl_context *gl = calloc(1, sizeof(*gl));
    if (gl == NULL) {
        return NULL;
    }

    gl->color = malloc((size_t)width * height * sizeof(uint32_t));
    gl->depth = malloc((size_t)width * height * sizeof(float));
    gl->verts = malloc((size_t)HGL_MAX_VERTICES * sizeof(struct hgl_vertex));
    if (gl->color == NULL || gl->depth == NULL || gl->verts == NULL) {
        hglDestroyContext(gl);
        return NULL;
    }
    gl->width = width;
    gl->height = height;

    hgl_mat_identity(&gl->modelview.m[0]);
    hgl_mat_identity(&gl->projection.m[0]);
    hgl_mat_identity(&gl->texture.m[0]);
    hgl_mat_identity(&gl->mvp);
    hgl_mat_identity(&gl->normal_matrix);
    gl->matrix_mode = HGL_MODELVIEW;
    gl->mvp_dirty = 1;

    gl->vp_x = 0; gl->vp_y = 0; gl->vp_w = width; gl->vp_h = height;
    gl->sc_x = 0; gl->sc_y = 0; gl->sc_w = width; gl->sc_h = height;

    gl->clear_depth = 1.0f;
    gl->depth_func = HGL_LESS;
    gl->depth_write = 1;
    gl->blend_src = HGL_SRC_ALPHA;
    gl->blend_dst = HGL_ONE_MINUS_SRC_ALPHA;
    gl->cull_face = HGL_BACK;
    gl->front_face = HGL_CCW;
    gl->shade_model = HGL_SMOOTH;

    gl->cur_r = gl->cur_g = gl->cur_b = gl->cur_a = 1.0f;
    gl->cur_nz = 1.0f;
    gl->point_size = 1.0f;
    gl->line_width = 1.0f;

    /* A material that shows up under a white light without being set:
     * a program that enables lighting and forgets hglMaterial should
     * see a lit object, not a black one. */
    gl->mat_ambient[0] = gl->mat_ambient[1] = gl->mat_ambient[2] = 0.2f;
    gl->mat_diffuse[0] = gl->mat_diffuse[1] = gl->mat_diffuse[2] = 0.8f;
    gl->mat_specular[0] = gl->mat_specular[1] = gl->mat_specular[2] = 0.0f;
    gl->mat_shininess = 16.0f;
    gl->global_ambient[0] = gl->global_ambient[1] =
        gl->global_ambient[2] = 0.2f;

    for (int i = 0; i < HGL_MAX_LIGHTS; i++) {
        gl->lights[i].w = 0.0f;
        gl->lights[i].z = 1.0f;
        gl->lights[i].r = gl->lights[i].g = gl->lights[i].b = 1.0f;
        gl->lights[i].att_const = 1.0f;
    }

    hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);
    return gl;
}

void hglDestroyContext(hgl_context *gl) {
    if (gl == NULL) {
        return;
    }
    for (int i = 0; i < HGL_MAX_TEXTURES; i++) {
        free(gl->textures[i].pixels);
    }
    free(gl->color);
    free(gl->depth);
    free(gl->verts);
    free(gl);
}

int hglResize(hgl_context *gl, int width, int height) {
    if (gl == NULL || width <= 0 || height <= 0 ||
        width > 8192 || height > 8192) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return -1;
    }
    uint32_t *c = realloc(gl->color, (size_t)width * height * sizeof(*c));
    if (c == NULL) {
        hgl_error(gl, HGL_OUT_OF_MEMORY);
        return -1;
    }
    gl->color = c;
    float *d = realloc(gl->depth, (size_t)width * height * sizeof(*d));
    if (d == NULL) {
        hgl_error(gl, HGL_OUT_OF_MEMORY);
        return -1;
    }
    gl->depth = d;

    gl->width = width;
    gl->height = height;
    /* The viewport and scissor followed the old size; a caller that
     * does not set them again should still get a sane frame. */
    gl->vp_x = 0; gl->vp_y = 0; gl->vp_w = width; gl->vp_h = height;
    gl->sc_x = 0; gl->sc_y = 0; gl->sc_w = width; gl->sc_h = height;
    return 0;
}

const uint32_t *hglColorBuffer(const hgl_context *gl) {
    return gl != NULL ? gl->color : NULL;
}

const float *hglDepthBuffer(const hgl_context *gl) {
    return gl != NULL ? gl->depth : NULL;
}

void hglGetSize(const hgl_context *gl, int *width, int *height) {
    if (gl == NULL) {
        return;
    }
    if (width != NULL) {
        *width = gl->width;
    }
    if (height != NULL) {
        *height = gl->height;
    }
}

int hglGetError(hgl_context *gl) {
    if (gl == NULL) {
        return HGL_INVALID_OPERATION;
    }
    int e = gl->error;
    gl->error = HGL_NO_ERROR;
    return e;
}

const char *hglErrorString(int err) {
    switch (err) {
    case HGL_NO_ERROR:          return "no error";
    case HGL_INVALID_ENUM:      return "invalid enum";
    case HGL_INVALID_VALUE:     return "invalid value";
    case HGL_INVALID_OPERATION: return "invalid operation";
    case HGL_STACK_OVERFLOW:    return "matrix stack overflow";
    case HGL_STACK_UNDERFLOW:   return "matrix stack underflow";
    case HGL_OUT_OF_MEMORY:     return "out of memory";
    default:                    return "unknown error";
    }
}

/* ---- framebuffer ---- */

static void clamp_rect(const hgl_context *gl, int *x, int *y, int *w,
                       int *h) {
    if (*x < 0) { *w += *x; *x = 0; }
    if (*y < 0) { *h += *y; *y = 0; }
    if (*x + *w > gl->width)  { *w = gl->width - *x; }
    if (*y + *h > gl->height) { *h = gl->height - *y; }
    if (*w < 0) { *w = 0; }
    if (*h < 0) { *h = 0; }
}

void hglViewport(hgl_context *gl, int x, int y, int w, int h) {
    if (gl == NULL) {
        return;
    }
    if (w < 0 || h < 0) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    gl->vp_x = x; gl->vp_y = y; gl->vp_w = w; gl->vp_h = h;
}

void hglScissor(hgl_context *gl, int x, int y, int w, int h) {
    if (gl == NULL) {
        return;
    }
    if (w < 0 || h < 0) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    gl->sc_x = x; gl->sc_y = y; gl->sc_w = w; gl->sc_h = h;
    clamp_rect(gl, &gl->sc_x, &gl->sc_y, &gl->sc_w, &gl->sc_h);
}

void hglClearColor(hgl_context *gl, float r, float g, float b, float a) {
    if (gl == NULL) return;
    gl->clear_r = r; gl->clear_g = g; gl->clear_b = b; gl->clear_a = a;
}

void hglClearDepth(hgl_context *gl, float depth) {
    if (gl == NULL) return;
    gl->clear_depth = depth;
}

static uint32_t pack(float r, float g, float b) {
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

void hglClear(hgl_context *gl, unsigned mask) {
    if (gl == NULL) {
        return;
    }
    /* The scissor bounds a clear, as it does in OpenGL - which is
     * what lets a program clear one pane of a split view. */
    int x = 0, y = 0, w = gl->width, h = gl->height;
    if (gl->enabled[HGL_SCISSOR_TEST]) {
        x = gl->sc_x; y = gl->sc_y; w = gl->sc_w; h = gl->sc_h;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    if (mask & HGL_COLOR_BUFFER_BIT) {
        uint32_t c = pack(gl->clear_r, gl->clear_g, gl->clear_b);
        for (int row = y; row < y + h; row++) {
            uint32_t *p = gl->color + (size_t)row * gl->width + x;
            for (int i = 0; i < w; i++) {
                p[i] = c;
            }
        }
    }
    if (mask & HGL_DEPTH_BUFFER_BIT) {
        float d = gl->clear_depth;
        for (int row = y; row < y + h; row++) {
            float *p = gl->depth + (size_t)row * gl->width + x;
            for (int i = 0; i < w; i++) {
                p[i] = d;
            }
        }
    }
}

/* ---- state ---- */

static int cap_ok(int cap) {
    return cap > 0 && cap < 32;
}

void hglEnable(hgl_context *gl, int cap) {
    if (gl == NULL) return;
    if (!cap_ok(cap)) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    gl->enabled[cap] = 1;
    if (cap >= HGL_LIGHT0 && cap < HGL_LIGHT0 + HGL_MAX_LIGHTS) {
        gl->lights[cap - HGL_LIGHT0].enabled = 1;
    }
}

void hglDisable(hgl_context *gl, int cap) {
    if (gl == NULL) return;
    if (!cap_ok(cap)) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    gl->enabled[cap] = 0;
    if (cap >= HGL_LIGHT0 && cap < HGL_LIGHT0 + HGL_MAX_LIGHTS) {
        gl->lights[cap - HGL_LIGHT0].enabled = 0;
    }
}

int hglIsEnabled(const hgl_context *gl, int cap) {
    if (gl == NULL || !cap_ok(cap)) {
        return 0;
    }
    return gl->enabled[cap];
}

void hglDepthFunc(hgl_context *gl, int func) {
    if (gl == NULL) return;
    if (func < HGL_NEVER || func > HGL_ALWAYS) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    gl->depth_func = func;
}

void hglDepthMask(hgl_context *gl, int write) {
    if (gl != NULL) gl->depth_write = write ? 1 : 0;
}

void hglBlendFunc(hgl_context *gl, int src, int dst) {
    if (gl == NULL) return;
    if (src < HGL_ZERO || src > HGL_ONE_MINUS_DST_COLOR ||
        dst < HGL_ZERO || dst > HGL_ONE_MINUS_DST_COLOR) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    gl->blend_src = src;
    gl->blend_dst = dst;
}

void hglCullFace(hgl_context *gl, int face) {
    if (gl == NULL) return;
    if (face < HGL_BACK || face > HGL_FRONT_AND_BACK) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    gl->cull_face = face;
}

void hglFrontFace(hgl_context *gl, int winding) {
    if (gl == NULL) return;
    if (winding != HGL_CCW && winding != HGL_CW) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    gl->front_face = winding;
}

void hglShadeModel(hgl_context *gl, int model) {
    if (gl == NULL) return;
    if (model != HGL_FLAT && model != HGL_SMOOTH) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    gl->shade_model = model;
}

void hglPointSize(hgl_context *gl, float size) {
    if (gl != NULL && size > 0.0f) gl->point_size = size;
}

void hglLineWidth(hgl_context *gl, float width) {
    if (gl != NULL && width > 0.0f) gl->line_width = width;
}

/* ---- the matrix stack ---- */

void hglMatrixMode(hgl_context *gl, int mode) {
    if (gl == NULL) return;
    if (mode != HGL_MODELVIEW && mode != HGL_PROJECTION &&
        mode != HGL_TEXTURE) {
        hgl_error(gl, HGL_INVALID_ENUM);
        return;
    }
    gl->matrix_mode = mode;
}

/* Every path that writes a matrix goes through this, so there is one
 * place that remembers to mark the combined transform stale. */
static struct hgl_mat *top(hgl_context *gl) {
    struct hgl_mat_stack *s = hgl_current_stack(gl);
    gl->mvp_dirty = 1;
    return &s->m[s->depth];
}

void hglLoadIdentity(hgl_context *gl) {
    if (gl != NULL) hgl_mat_identity(top(gl));
}

void hglLoadMatrixf(hgl_context *gl, const float *m) {
    if (gl == NULL) return;
    if (m == NULL) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    memcpy(top(gl)->m, m, sizeof(float) * 16);
}

void hglMultMatrixf(hgl_context *gl, const float *m) {
    if (gl == NULL) return;
    if (m == NULL) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    struct hgl_mat b;
    memcpy(b.m, m, sizeof(b.m));
    struct hgl_mat *t = top(gl);
    hgl_mat_mul(t, t, &b);
}

void hglPushMatrix(hgl_context *gl) {
    if (gl == NULL) return;
    struct hgl_mat_stack *s = hgl_current_stack(gl);
    if (s->depth + 1 >= HGL_MATRIX_DEPTH) {
        hgl_error(gl, HGL_STACK_OVERFLOW);
        return;
    }
    s->m[s->depth + 1] = s->m[s->depth];
    s->depth++;
    gl->mvp_dirty = 1;
}

void hglPopMatrix(hgl_context *gl) {
    if (gl == NULL) return;
    struct hgl_mat_stack *s = hgl_current_stack(gl);
    if (s->depth == 0) {
        hgl_error(gl, HGL_STACK_UNDERFLOW);
        return;
    }
    s->depth--;
    gl->mvp_dirty = 1;
}

void hglGetMatrixf(const hgl_context *gl, int mode, float *out) {
    if (gl == NULL || out == NULL) {
        return;
    }
    const struct hgl_mat_stack *s;
    switch (mode) {
    case HGL_PROJECTION: s = &gl->projection; break;
    case HGL_TEXTURE:    s = &gl->texture; break;
    default:             s = &gl->modelview; break;
    }
    memcpy(out, s->m[s->depth].m, sizeof(float) * 16);
}

void hglTranslatef(hgl_context *gl, float x, float y, float z) {
    if (gl == NULL) return;
    struct hgl_mat t;
    hgl_mat_identity(&t);
    t.m[12] = x; t.m[13] = y; t.m[14] = z;
    struct hgl_mat *m = top(gl);
    hgl_mat_mul(m, m, &t);
}

void hglScalef(hgl_context *gl, float x, float y, float z) {
    if (gl == NULL) return;
    struct hgl_mat t;
    hgl_mat_identity(&t);
    t.m[0] = x; t.m[5] = y; t.m[10] = z;
    struct hgl_mat *m = top(gl);
    hgl_mat_mul(m, m, &t);
}

void hglRotatef(hgl_context *gl, float degrees, float x, float y, float z) {
    if (gl == NULL) return;
    float len = sqrtf(x * x + y * y + z * z);
    if (len < 1e-8f) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    x /= len; y /= len; z /= len;

    float rad = degrees * 3.14159265358979f / 180.0f;
    float c = cosf(rad), s = sinf(rad), ic = 1.0f - c;

    struct hgl_mat t;
    hgl_mat_identity(&t);
    t.m[0]  = x * x * ic + c;
    t.m[1]  = y * x * ic + z * s;
    t.m[2]  = z * x * ic - y * s;
    t.m[4]  = x * y * ic - z * s;
    t.m[5]  = y * y * ic + c;
    t.m[6]  = z * y * ic + x * s;
    t.m[8]  = x * z * ic + y * s;
    t.m[9]  = y * z * ic - x * s;
    t.m[10] = z * z * ic + c;

    struct hgl_mat *m = top(gl);
    hgl_mat_mul(m, m, &t);
}

void hglOrtho(hgl_context *gl, float l, float r, float b, float t,
              float n, float f) {
    if (gl == NULL) return;
    if (l == r || b == t || n == f) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    struct hgl_mat o;
    hgl_mat_identity(&o);
    o.m[0]  = 2.0f / (r - l);
    o.m[5]  = 2.0f / (t - b);
    o.m[10] = -2.0f / (f - n);
    o.m[12] = -(r + l) / (r - l);
    o.m[13] = -(t + b) / (t - b);
    o.m[14] = -(f + n) / (f - n);

    struct hgl_mat *m = top(gl);
    hgl_mat_mul(m, m, &o);
}

void hglFrustum(hgl_context *gl, float l, float r, float b, float t,
                float n, float f) {
    if (gl == NULL) return;
    if (l == r || b == t || n <= 0.0f || f <= n) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    struct hgl_mat p;
    memset(p.m, 0, sizeof(p.m));
    p.m[0]  = 2.0f * n / (r - l);
    p.m[5]  = 2.0f * n / (t - b);
    p.m[8]  = (r + l) / (r - l);
    p.m[9]  = (t + b) / (t - b);
    p.m[10] = -(f + n) / (f - n);
    p.m[11] = -1.0f;
    p.m[14] = -2.0f * f * n / (f - n);

    struct hgl_mat *m = top(gl);
    hgl_mat_mul(m, m, &p);
}

void hglPerspective(hgl_context *gl, float fovy_degrees, float aspect,
                    float near_z, float far_z) {
    if (gl == NULL) return;
    if (aspect <= 0.0f || near_z <= 0.0f || far_z <= near_z ||
        fovy_degrees <= 0.0f || fovy_degrees >= 180.0f) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    float t = near_z * tanf(fovy_degrees * 3.14159265358979f / 360.0f);
    float r = t * aspect;
    hglFrustum(gl, -r, r, -t, t, near_z, far_z);
}

void hglLookAt(hgl_context *gl, float ex, float ey, float ez,
               float cx, float cy, float cz,
               float ux, float uy, float uz) {
    if (gl == NULL) return;

    float fx = cx - ex, fy = cy - ey, fz = cz - ez;
    float fl = sqrtf(fx * fx + fy * fy + fz * fz);
    if (fl < 1e-8f) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    fx /= fl; fy /= fl; fz /= fl;

    /* s = f x up, u = s x f. The up vector need not be perpendicular
     * to the view direction; the second cross product fixes it, which
     * is why a caller can pass (0,1,0) and forget about it. */
    float sx = fy * uz - fz * uy;
    float sy = fz * ux - fx * uz;
    float sz = fx * uy - fy * ux;
    float sl = sqrtf(sx * sx + sy * sy + sz * sz);
    if (sl < 1e-8f) {
        hgl_error(gl, HGL_INVALID_VALUE); /* up is parallel to the view */
        return;
    }
    sx /= sl; sy /= sl; sz /= sl;

    float ux2 = sy * fz - sz * fy;
    float uy2 = sz * fx - sx * fz;
    float uz2 = sx * fy - sy * fx;

    struct hgl_mat v;
    hgl_mat_identity(&v);
    v.m[0] = sx;  v.m[4] = sy;  v.m[8]  = sz;
    v.m[1] = ux2; v.m[5] = uy2; v.m[9]  = uz2;
    v.m[2] = -fx; v.m[6] = -fy; v.m[10] = -fz;

    struct hgl_mat *m = top(gl);
    hgl_mat_mul(m, m, &v);
    hglTranslatef(gl, -ex, -ey, -ez);
}

/* ---- lighting state ---- */

void hglLightPosition(hgl_context *gl, int light, float x, float y, float z,
                      float w) {
    if (gl == NULL) return;
    if (light < 0 || light >= HGL_MAX_LIGHTS) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    gl->lights[light].x = x;
    gl->lights[light].y = y;
    gl->lights[light].z = z;
    gl->lights[light].w = w;
}

void hglLightColor(hgl_context *gl, int light, float r, float g, float b) {
    if (gl == NULL) return;
    if (light < 0 || light >= HGL_MAX_LIGHTS) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    gl->lights[light].r = r;
    gl->lights[light].g = g;
    gl->lights[light].b = b;
}

void hglLightAttenuation(hgl_context *gl, int light, float constant,
                         float linear, float quadratic) {
    if (gl == NULL) return;
    if (light < 0 || light >= HGL_MAX_LIGHTS) {
        hgl_error(gl, HGL_INVALID_VALUE);
        return;
    }
    gl->lights[light].att_const = constant;
    gl->lights[light].att_linear = linear;
    gl->lights[light].att_quad = quadratic;
}

void hglMaterial(hgl_context *gl, float ar, float ag, float ab,
                 float dr, float dg, float db,
                 float sr, float sg, float sb, float shininess) {
    if (gl == NULL) return;
    gl->mat_ambient[0] = ar; gl->mat_ambient[1] = ag; gl->mat_ambient[2] = ab;
    gl->mat_diffuse[0] = dr; gl->mat_diffuse[1] = dg; gl->mat_diffuse[2] = db;
    gl->mat_specular[0] = sr; gl->mat_specular[1] = sg;
    gl->mat_specular[2] = sb;
    gl->mat_shininess = shininess > 0.0f ? shininess : 1.0f;
}

void hglGlobalAmbient(hgl_context *gl, float r, float g, float b) {
    if (gl == NULL) return;
    gl->global_ambient[0] = r;
    gl->global_ambient[1] = g;
    gl->global_ambient[2] = b;
}

/* ---- hooks ---- */

void hglVertexHook(hgl_context *gl, hgl_vertex_hook fn, void *ctx) {
    if (gl == NULL) return;
    gl->vhook = fn;
    gl->vhook_ctx = ctx;
}

void hglFragmentHook(hgl_context *gl, hgl_fragment_hook fn, void *ctx) {
    if (gl == NULL) return;
    gl->fhook = fn;
    gl->fhook_ctx = ctx;
}

/* ---- statistics ---- */

void hglGetStats(const hgl_context *gl, struct hgl_stats *out) {
    if (gl != NULL && out != NULL) {
        *out = gl->stats;
    }
}

void hglResetStats(hgl_context *gl) {
    if (gl != NULL) {
        memset(&gl->stats, 0, sizeof(gl->stats));
    }
}
