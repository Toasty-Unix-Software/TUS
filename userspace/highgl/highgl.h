/*
 * highgl.h - HighGL, a 3D graphics library written from scratch
 *
 * HighGL is to 3D what highX is to windows: a small, documented
 * interface with nothing hidden behind it. It is shaped like OpenGL
 * 1.x because that shape is worth copying - a state machine, a matrix
 * stack, and geometry pushed between hglBegin and hglEnd - and
 * because a programmer who knows OpenGL already knows this. It is not
 * OpenGL: no driver, no GPU, no context loss, and every triangle is
 * rasterised by the CPU into a buffer the caller owns.
 *
 *     hgl_context *gl = hglCreateContext(w, h);
 *     hglViewport(gl, 0, 0, w, h);
 *     hglClearColor(gl, 0.1f, 0.1f, 0.12f, 1.0f);
 *     hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);
 *
 *     hglMatrixMode(gl, HGL_PROJECTION);
 *     hglLoadIdentity(gl);
 *     hglPerspective(gl, 60.0f, aspect, 0.1f, 100.0f);
 *
 *     hglBegin(gl, HGL_TRIANGLES);
 *       hglColor3f(gl, 1, 0, 0); hglVertex3f(gl, ...);
 *     hglEnd(gl);
 *
 *     const uint32_t *pixels = hglColorBuffer(gl);   /+ ARGB, w*h +/
 *
 * WHY FIXED FUNCTION, AND WHAT REPLACES SHADERS
 *
 * A shading language needs a compiler, and a software rasteriser
 * running a compiled shader per fragment is slower than the fixed
 * pipeline it replaced. HighGL instead lets a program hand it two C
 * function pointers: a *vertex hook* that may rewrite a vertex after
 * it is transformed, and a *fragment hook* that may rewrite a colour
 * before it is written. They are called with the same interpolated
 * values a shader would get, they are ordinary C, and they cost one
 * indirect call - which is why they are off by default and the fixed
 * path stays fast when nobody uses them.
 *
 * WHY FLOATING POINT HERE
 *
 * Unlike userspace/tusfont, which is fixed point on purpose, a 3D
 * pipeline is float: a perspective divide and a matrix concatenation
 * in fixed point are a study in overflow, and the numbers span from a
 * near plane at 0.1 to a far plane at 1000. TUS userspace may use SSE
 * - the scheduler saves the FPU per task - so highgl is built with
 * `-mgeneral-regs-only` filtered back out, the same way Clint's
 * JavaScript interpreter is.
 *
 * WHAT IT DOES NOT DO
 *
 * No display lists, no accumulation buffer, no stencil buffer, no
 * evaluators, no selection or feedback mode, no stipple. Textures are
 * 2D only, and RGBA only. These are omissions, not simplifications
 * with a trick behind them.
 */

#ifndef HIGHGL_H
#define HIGHGL_H

#include <stddef.h>
#include <stdint.h>

/* ---- limits ---- */
#define HGL_MAX_TEXTURES     64
#define HGL_MAX_LIGHTS        8
#define HGL_MATRIX_DEPTH     32
#define HGL_MAX_VERTICES  65536   /* between one Begin and its End */

/* ---- enums ----
 *
 * Values are ours, not OpenGL's: a program that mixes the two headers
 * would be a program with two of everything, and matching the numbers
 * would only make that mistake harder to see.
 */

/* Primitives. */
enum {
    HGL_POINTS = 1,
    HGL_LINES,
    HGL_LINE_STRIP,
    HGL_LINE_LOOP,
    HGL_TRIANGLES,
    HGL_TRIANGLE_STRIP,
    HGL_TRIANGLE_FAN,
    HGL_QUADS,
};

/* Matrix stacks. */
enum {
    HGL_MODELVIEW = 1,
    HGL_PROJECTION,
    HGL_TEXTURE,
};

/* Capabilities for hglEnable / hglDisable. */
enum {
    HGL_DEPTH_TEST = 1,
    HGL_BLEND,
    HGL_TEXTURE_2D,
    HGL_CULL_FACE,
    HGL_LIGHTING,
    HGL_SCISSOR_TEST,
    HGL_LIGHT0,   /* LIGHT0 + n enables light n */
    HGL_LIGHT1, HGL_LIGHT2, HGL_LIGHT3,
    HGL_LIGHT4, HGL_LIGHT5, HGL_LIGHT6, HGL_LIGHT7,
};

/* hglClear masks. */
#define HGL_COLOR_BUFFER_BIT 0x1
#define HGL_DEPTH_BUFFER_BIT 0x2

/* Depth comparison. */
enum {
    HGL_NEVER = 0, HGL_LESS, HGL_EQUAL, HGL_LEQUAL,
    HGL_GREATER, HGL_NOTEQUAL, HGL_GEQUAL, HGL_ALWAYS,
};

/* Blend factors. */
enum {
    HGL_ZERO = 0, HGL_ONE,
    HGL_SRC_ALPHA, HGL_ONE_MINUS_SRC_ALPHA,
    HGL_DST_ALPHA, HGL_ONE_MINUS_DST_ALPHA,
    HGL_SRC_COLOR, HGL_ONE_MINUS_SRC_COLOR,
    HGL_DST_COLOR, HGL_ONE_MINUS_DST_COLOR,
};

/* Face culling. */
enum { HGL_BACK = 0, HGL_FRONT, HGL_FRONT_AND_BACK };
enum { HGL_CCW = 0, HGL_CW };

/* Texture filters and wrap modes. */
enum { HGL_NEAREST = 0, HGL_LINEAR };
enum { HGL_REPEAT = 0, HGL_CLAMP_TO_EDGE, HGL_MIRRORED_REPEAT };

/* Shading. */
enum { HGL_FLAT = 0, HGL_SMOOTH };

typedef struct hgl_context hgl_context;

/* ---- the hooks that stand in for shaders ---- */

/* A vertex after the modelview and projection matrices have been
 * applied, before clipping. A vertex hook may change anything here;
 * the pipeline carries on with what it left. */
struct hgl_vertex {
    float x, y, z, w;      /* clip space */
    float r, g, b, a;      /* colour, 0..1 */
    float s, t;            /* texture coordinates */
    float nx, ny, nz;      /* normal, in eye space */
    float ex, ey, ez;      /* position in eye space, for lighting */
};

/* A fragment about to be written. `depth` is the interpolated z in
 * 0..1; writing to it changes what the depth buffer records. Return 0
 * to discard the fragment. */
struct hgl_fragment {
    int   x, y;
    float depth;
    float r, g, b, a;
    float s, t;
};

typedef void (*hgl_vertex_hook)(void *ctx, struct hgl_vertex *v);
typedef int  (*hgl_fragment_hook)(void *ctx, struct hgl_fragment *f);

/* ---- context ---- */

hgl_context *hglCreateContext(int width, int height);
void hglDestroyContext(hgl_context *gl);

/* Resize the buffers. The contents are not preserved. Returns 0. */
int hglResize(hgl_context *gl, int width, int height);

/* The rendered image: width*height pixels of 0x00RRGGBB, ready for
 * hx_image(). The depth buffer is exposed too, for a caller that
 * wants to composite several passes. */
const uint32_t *hglColorBuffer(const hgl_context *gl);
const float    *hglDepthBuffer(const hgl_context *gl);
void hglGetSize(const hgl_context *gl, int *width, int *height);

/* The last error, and a string for it. Errors latch the way OpenGL's
 * do: a call that fails records it and does nothing, and the program
 * keeps running. */
int         hglGetError(hgl_context *gl);   /* clears it */
const char *hglErrorString(int err);

enum {
    HGL_NO_ERROR = 0,
    HGL_INVALID_ENUM,
    HGL_INVALID_VALUE,
    HGL_INVALID_OPERATION,
    HGL_STACK_OVERFLOW,
    HGL_STACK_UNDERFLOW,
    HGL_OUT_OF_MEMORY,
};

/* ---- framebuffer ---- */

void hglViewport(hgl_context *gl, int x, int y, int w, int h);
void hglScissor(hgl_context *gl, int x, int y, int w, int h);
void hglClearColor(hgl_context *gl, float r, float g, float b, float a);
void hglClearDepth(hgl_context *gl, float depth);
void hglClear(hgl_context *gl, unsigned mask);

/* ---- state ---- */

void hglEnable(hgl_context *gl, int cap);
void hglDisable(hgl_context *gl, int cap);
int  hglIsEnabled(const hgl_context *gl, int cap);

void hglDepthFunc(hgl_context *gl, int func);
void hglDepthMask(hgl_context *gl, int write);
void hglBlendFunc(hgl_context *gl, int src, int dst);
void hglCullFace(hgl_context *gl, int face);
void hglFrontFace(hgl_context *gl, int winding);
void hglShadeModel(hgl_context *gl, int model);

/* ---- matrices ----
 *
 * Column-major, sixteen floats, the same layout OpenGL uses - so a
 * matrix from anywhere else can be handed straight to
 * hglLoadMatrixf().
 */

void hglMatrixMode(hgl_context *gl, int mode);
void hglLoadIdentity(hgl_context *gl);
void hglLoadMatrixf(hgl_context *gl, const float *m);
void hglMultMatrixf(hgl_context *gl, const float *m);
void hglPushMatrix(hgl_context *gl);
void hglPopMatrix(hgl_context *gl);
void hglGetMatrixf(const hgl_context *gl, int mode, float *out);

void hglTranslatef(hgl_context *gl, float x, float y, float z);
void hglScalef(hgl_context *gl, float x, float y, float z);
void hglRotatef(hgl_context *gl, float degrees, float x, float y, float z);

void hglOrtho(hgl_context *gl, float l, float r, float b, float t,
              float n, float f);
void hglFrustum(hgl_context *gl, float l, float r, float b, float t,
                float n, float f);
/* The convenience the other two are usually wrapped in. */
void hglPerspective(hgl_context *gl, float fovy_degrees, float aspect,
                    float near_z, float far_z);
void hglLookAt(hgl_context *gl, float ex, float ey, float ez,
               float cx, float cy, float cz,
               float ux, float uy, float uz);

/* ---- geometry ---- */

void hglBegin(hgl_context *gl, int primitive);
void hglEnd(hgl_context *gl);

void hglVertex2f(hgl_context *gl, float x, float y);
void hglVertex3f(hgl_context *gl, float x, float y, float z);
void hglVertex4f(hgl_context *gl, float x, float y, float z, float w);
void hglColor3f(hgl_context *gl, float r, float g, float b);
void hglColor4f(hgl_context *gl, float r, float g, float b, float a);
void hglColor4ub(hgl_context *gl, uint8_t r, uint8_t g, uint8_t b,
                 uint8_t a);
void hglTexCoord2f(hgl_context *gl, float s, float t);
void hglNormal3f(hgl_context *gl, float x, float y, float z);

void hglPointSize(hgl_context *gl, float size);
void hglLineWidth(hgl_context *gl, float width);

/* ---- textures ---- */

/* Allocate `n` texture names into `out`. Name 0 is reserved and means
 * "no texture", as it does in OpenGL. */
void hglGenTextures(hgl_context *gl, int n, unsigned *out);
void hglDeleteTextures(hgl_context *gl, int n, const unsigned *names);
void hglBindTexture(hgl_context *gl, unsigned name);

/* Upload w*h pixels of 0xAARRGGBB. The data is copied. */
void hglTexImage2D(hgl_context *gl, int w, int h, const uint32_t *pixels);
void hglTexParameter(hgl_context *gl, int min_filter, int mag_filter,
                     int wrap_s, int wrap_t);

/* ---- lighting ----
 *
 * Per-vertex (Gouraud), which is what fixed-function lighting always
 * was. Positions and directions are given in EYE space - the caller
 * has already decided where the camera is, and transforming a light
 * by whatever the modelview happens to hold at the moment it is set
 * is the single most confusing thing about OpenGL's version.
 */

void hglLightPosition(hgl_context *gl, int light, float x, float y, float z,
                      float w); /* w == 0: a direction, not a position */
void hglLightColor(hgl_context *gl, int light, float r, float g, float b);
void hglLightAttenuation(hgl_context *gl, int light, float constant,
                         float linear, float quadratic);
void hglMaterial(hgl_context *gl, float ar, float ag, float ab,
                 float dr, float dg, float db,
                 float sr, float sg, float sb, float shininess);
void hglGlobalAmbient(hgl_context *gl, float r, float g, float b);

/* ---- the hooks ---- */

/* Pass NULL to remove a hook. */
void hglVertexHook(hgl_context *gl, hgl_vertex_hook fn, void *ctx);
void hglFragmentHook(hgl_context *gl, hgl_fragment_hook fn, void *ctx);

/* ---- statistics ----
 *
 * What the last frame actually cost. A software rasteriser lives or
 * dies on how many fragments it touched, and a program that cannot
 * see that number tunes blind.
 */
struct hgl_stats {
    unsigned long vertices;
    unsigned long primitives;
    unsigned long primitives_culled;
    unsigned long primitives_clipped;
    unsigned long fragments;      /* considered */
    unsigned long fragments_drawn;/* survived depth, hook and scissor */
};

void hglGetStats(const hgl_context *gl, struct hgl_stats *out);
void hglResetStats(hgl_context *gl);

#endif /* HIGHGL_H */
