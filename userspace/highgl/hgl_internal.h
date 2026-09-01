/*
 * hgl_internal.h - what the HighGL source files share
 *
 * Not a public header; a caller uses highgl.h. This is the context
 * with all its state, and the handful of functions the three source
 * files call across the boundary between them.
 */

#ifndef HIGHGL_INTERNAL_H
#define HIGHGL_INTERNAL_H

#include "highgl.h"

/* A 4x4 matrix, column major (m[column * 4 + row]) - the layout
 * OpenGL uses, so a matrix from anywhere else drops straight in. */
struct hgl_mat {
    float m[16];
};

struct hgl_texture {
    int      used;
    int      w, h;
    uint32_t *pixels;      /* 0xAARRGGBB */
    int      min_filter, mag_filter;
    int      wrap_s, wrap_t;
};

struct hgl_light {
    int   enabled;
    float x, y, z, w;      /* eye space; w == 0 means a direction */
    float r, g, b;
    float att_const, att_linear, att_quad;
};

/* One stack of matrices. */
struct hgl_mat_stack {
    struct hgl_mat m[HGL_MATRIX_DEPTH];
    int depth;             /* index of the top */
};

struct hgl_context {
    int width, height;
    uint32_t *color;
    float    *depth;

    /* Viewport and scissor, in pixels, already clamped to the
     * buffers so the rasteriser never has to check twice. */
    int vp_x, vp_y, vp_w, vp_h;
    int sc_x, sc_y, sc_w, sc_h;

    float clear_r, clear_g, clear_b, clear_a;
    float clear_depth;

    /* Capabilities, indexed by the HGL_* enum. */
    unsigned char enabled[32];

    int depth_func;
    int depth_write;
    int blend_src, blend_dst;
    int cull_face, front_face;
    int shade_model;

    struct hgl_mat_stack modelview, projection, texture;
    int matrix_mode;

    /* The concatenation of projection * modelview, rebuilt only when
     * one of them changes. A matrix multiply per vertex would be
     * three quarters of the transform cost for no reason. */
    struct hgl_mat mvp;
    int mvp_dirty;
    /* The modelview's inverse transpose, for normals - or, since the
     * modelview here is always a rigid motion plus uniform scale, its
     * rotation part with the scale divided back out. */
    struct hgl_mat normal_matrix;

    /* Current vertex attributes, as set by the hglColor and hglTexCoord
     * calls. */
    float cur_r, cur_g, cur_b, cur_a;
    float cur_s, cur_t;
    float cur_nx, cur_ny, cur_nz;

    /* Between Begin and End. */
    int primitive;
    int in_begin;
    struct hgl_vertex *verts;
    int nverts;

    struct hgl_texture textures[HGL_MAX_TEXTURES];
    unsigned bound_texture;

    struct hgl_light lights[HGL_MAX_LIGHTS];
    float mat_ambient[3], mat_diffuse[3], mat_specular[3], mat_shininess;
    float global_ambient[3];

    float point_size, line_width;

    hgl_vertex_hook   vhook;
    void             *vhook_ctx;
    hgl_fragment_hook fhook;
    void             *fhook_ctx;

    int error;
    struct hgl_stats stats;
};

/* ---- matrix.c ---- */
void hgl_mat_identity(struct hgl_mat *m);
void hgl_mat_mul(struct hgl_mat *out, const struct hgl_mat *a,
                 const struct hgl_mat *b);
void hgl_mat_transform(const struct hgl_mat *m, const float in[4],
                       float out[4]);
void hgl_update_derived(hgl_context *gl);
struct hgl_mat_stack *hgl_current_stack(hgl_context *gl);

/* ---- pipeline.c ---- */
void hgl_flush_primitive(hgl_context *gl);

/* ---- raster.c ---- */
void hgl_raster_triangle(hgl_context *gl, const struct hgl_vertex *a,
                         const struct hgl_vertex *b,
                         const struct hgl_vertex *c);
void hgl_raster_line(hgl_context *gl, const struct hgl_vertex *a,
                     const struct hgl_vertex *b);
void hgl_raster_point(hgl_context *gl, const struct hgl_vertex *v);

/* ---- texture.c ---- */
/* Sample the bound texture. Returns 0xAARRGGBB; white when nothing is
 * bound, so an unbound texture multiplies a colour by one. */
uint32_t hgl_sample(const hgl_context *gl, float s, float t);

static inline void hgl_error(hgl_context *gl, int err) {
    if (gl != NULL && gl->error == HGL_NO_ERROR) {
        gl->error = err; /* the FIRST error is the interesting one */
    }
}

#endif /* HIGHGL_INTERNAL_H */
