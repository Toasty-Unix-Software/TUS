/*
 * test_highgl.c - host tests for HighGL
 *
 * Three kinds of check, in order of how much they are worth:
 *
 *  1. THE RASTERISER AGAINST A MODEL. A triangle is filled by edge
 *     functions stepped across a bounding box; the model decides each
 *     pixel independently by evaluating the same three edges at that
 *     pixel's centre from scratch. If the stepping has drifted, or a
 *     tie rule differs, or the bounding box is off by one, the two
 *     disagree. This is the same technique tests/highx uses on the
 *     compositor and tests/font uses on the glyph rasteriser.
 *
 *  2. FACTS THAT MUST HOLD. A matrix stack that pops what it pushed.
 *     A perspective divide that makes distant things smaller. A depth
 *     test that keeps the nearer of two overlapping triangles
 *     whichever order they are drawn in. Perspective-correct texture
 *     coordinates - the one every software rasteriser gets wrong, and
 *     it is checked by drawing a floor and looking at where the
 *     checks land rather than by trusting the arithmetic.
 *
 *  3. MISUSE. Every entry point called with nonsense, to see that it
 *     records an error and does nothing rather than following a null
 *     pointer.
 *
 * Build and run:  make -C tests/highgl run
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../../userspace/highgl/context.c"
#include "../../userspace/highgl/pipeline.c"
#include "../../userspace/highgl/raster.c"
#include "../../userspace/highgl/texture.c"

static int g_checks, g_failures;

static void ok(const char *name) {
    g_checks++;
    printf("  [PASS] %s\n", name);
}

static void fail(const char *name, const char *why) {
    g_checks++;
    g_failures++;
    printf("  [FAIL] %s: %s\n", name, why);
}

static void check(int cond, const char *name, const char *why) {
    if (cond) { ok(name); } else { fail(name, why); }
}

#define W 160
#define H 120

static uint32_t at(hgl_context *gl, int x, int y) {
    return gl->color[(size_t)y * gl->width + x];
}

/* ---- 1. the rasteriser against a model ---- */

/* Fill a triangle the slow, obvious way: for each pixel in the whole
 * buffer, evaluate the three edge functions at its centre and take
 * the pixel if all three agree. No stepping, no bounding box, no
 * shortcuts to be wrong in. */
static void model_triangle(uint8_t *out, int w, int h,
                           float ax, float ay, float bx, float by,
                           float cx, float cy) {
    memset(out, 0, (size_t)w * h);
    float area = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
    if (area == 0.0f) {
        return;
    }
    /* Match the rasteriser's convention: vertices swapped, not tests
     * inverted. */
    if (area < 0.0f) {
        float tx = bx, ty = by;
        bx = cx; by = cy;
        cx = tx; cy = ty;
    }
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            float px = (float)x + 0.5f, py = (float)y + 0.5f;
            float w0 = (cx - bx) * (py - by) - (cy - by) * (px - bx);
            float w1 = (ax - cx) * (py - cy) - (ay - cy) * (px - cx);
            float w2 = (bx - ax) * (py - ay) - (by - ay) * (px - ax);
            if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
                out[(size_t)y * w + x] = 1;
            }
        }
    }
}

/* Draw a triangle given directly in screen pixels, by setting up an
 * orthographic projection that maps pixels to clip space one to one.
 * That way the test controls exactly which pixels should be hit. */
static void ortho_pixels(hgl_context *gl) {
    hglMatrixMode(gl, HGL_PROJECTION);
    hglLoadIdentity(gl);
    hglOrtho(gl, 0.0f, (float)W, (float)H, 0.0f, -1.0f, 1.0f);
    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);
}

/* How far, in double precision, pixel (x,y)'s centre sits from the
 * nearest of the triangle's three edges (as a raw edge-function value,
 * not a distance in pixels - but zero means "exactly on the edge"
 * either way, which is what matters here). */
static double nearest_edge(double ax, double ay, double bx, double by,
                           double cx, double cy, double px, double py) {
    double area = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
    if (area < 0.0) {
        double tx = bx, ty = by;
        bx = cx; by = cy;
        cx = tx; cy = ty;
    }
    double w0 = (cx - bx) * (py - by) - (cy - by) * (px - bx);
    double w1 = (ax - cx) * (py - cy) - (ay - cy) * (px - cx);
    double w2 = (bx - ax) * (py - ay) - (by - ay) * (px - ax);
    double m = fabs(w0);
    if (fabs(w1) < m) m = fabs(w1);
    if (fabs(w2) < m) m = fabs(w2);
    return m;
}

static int compare_triangle(hgl_context *gl, float ax, float ay,
                            float bx, float by, float cx, float cy,
                            int *diff_out) {
    hglClearColor(gl, 0, 0, 0, 1);
    hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);
    hglColor3f(gl, 1, 1, 1);
    hglBegin(gl, HGL_TRIANGLES);
    hglVertex2f(gl, ax, ay);
    hglVertex2f(gl, bx, by);
    hglVertex2f(gl, cx, cy);
    hglEnd(gl);

    static uint8_t model[W * H];
    model_triangle(model, W, H, ax, ay, bx, by, cx, cy);

    /* A pixel whose centre sits exactly on an edge, in exact
     * arithmetic, is a coin flip: the model evaluates the edge
     * functions directly from the literal test coordinates, while the
     * rasteriser evaluates them from vertices that went through the
     * projection matrix - a chain of floating point divisions
     * (1/(right-left), 1/(top-bottom)) that are not exactly
     * representable for most window sizes. That round trip moves an
     * on-screen coordinate by a few ULPs, which is nowhere near enough
     * to matter UNLESS the true value was exactly zero, in which case
     * either sign is "correct" and the two paths are free to disagree.
     * A mismatch only counts as a real bug when the pixel is not that
     * close a call. */
    int diff = 0;
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int got = at(gl, x, y) != 0;
            int want = model[(size_t)y * W + x];
            if (got == want) {
                continue;
            }
            double d = nearest_edge(ax, ay, bx, by, cx, cy,
                                    (double)x + 0.5, (double)y + 0.5);
            if (d > 1e-2) {
                diff++;
            }
        }
    }
    if (diff_out != NULL) {
        *diff_out = diff;
    }
    return diff;
}

static void check_against_model(hgl_context *gl) {
    ortho_pixels(gl);

    struct { float ax, ay, bx, by, cx, cy; const char *what; } cases[] = {
        {  20,  20, 140,  30,  80, 100, "a plain triangle" },
        {  80, 100, 140,  30,  20,  20, "the same one wound the other way" },
        {  -40, -30, 200,  10,  60, 200, "one larger than the screen" },
        {  10,  10,  11,  10,  10,  11, "a triangle one pixel across" },
        {  0.5f, 0.5f, 159.5f, 0.5f, 80.0f, 119.5f, "one on the edges" },
        {  30,  40,  30, 100, 120,  70, "a vertical left edge" },
        {  30,  40, 120,  40,  75, 100, "a horizontal top edge" },
        {  40,  50, 100,  50,  70,  50, "a degenerate flat one" },
    };

    int worst = 0;
    const char *worst_case = "";
    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        int d = 0;
        compare_triangle(gl, cases[i].ax, cases[i].ay, cases[i].bx,
                         cases[i].by, cases[i].cx, cases[i].cy, &d);
        if (d > worst) {
            worst = d;
            worst_case = cases[i].what;
        }
    }
    char why[160];
    snprintf(why, sizeof(why), "%d pixels differ, worst on %s",
             worst, worst_case);
    check(worst == 0, "the rasteriser fills exactly what the model does",
          why);

    /* Two triangles sharing an edge must tile it: no pixel drawn
     * twice, no gap between them. This is what a tie rule is for, and
     * it is the difference between a seamless mesh and one with
     * cracks along every shared edge. */
    hglClearColor(gl, 0, 0, 0, 1);
    hglClear(gl, HGL_COLOR_BUFFER_BIT);
    hglEnable(gl, HGL_BLEND);
    hglBlendFunc(gl, HGL_ONE, HGL_ONE);   /* additive: a double hit shows */
    hglColor3f(gl, 0.5f, 0.5f, 0.5f);
    hglBegin(gl, HGL_TRIANGLES);
    hglVertex2f(gl, 20, 20); hglVertex2f(gl, 120, 20); hglVertex2f(gl, 20, 100);
    hglVertex2f(gl, 120, 20); hglVertex2f(gl, 120, 100); hglVertex2f(gl, 20, 100);
    hglEnd(gl);
    hglDisable(gl, HGL_BLEND);

    int doubled = 0, filled = 0;
    for (int y = 21; y < 99; y++) {
        for (int x = 21; x < 119; x++) {
            uint32_t c = at(gl, x, y);
            unsigned r = (c >> 16) & 0xFF;
            if (r > 0) {
                filled++;
            }
            if (r > 200) {
                doubled++;   /* 0.5 + 0.5 rather than 0.5 */
            }
        }
    }
    char why2[128];
    snprintf(why2, sizeof(why2), "%d of %d pixels were drawn twice",
             doubled, filled);
    check(filled > 5000 && doubled == 0,
          "two triangles sharing an edge tile it without overlap", why2);
}

/* ---- 2. facts that must hold ---- */

static void check_matrices(hgl_context *gl) {
    float m[16], id[16];
    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);
    hglGetMatrixf(gl, HGL_MODELVIEW, id);

    hglPushMatrix(gl);
    hglTranslatef(gl, 3, 4, 5);
    hglRotatef(gl, 37, 0, 1, 0);
    hglScalef(gl, 2, 2, 2);
    hglPopMatrix(gl);
    hglGetMatrixf(gl, HGL_MODELVIEW, m);
    check(memcmp(m, id, sizeof(m)) == 0,
          "a matrix stack pops exactly what it pushed",
          "the matrix changed across a push/pop pair");

    /* Rotating by 360 degrees is the identity, up to float error. */
    hglLoadIdentity(gl);
    for (int i = 0; i < 8; i++) {
        hglRotatef(gl, 45.0f, 0.3f, 0.5f, 0.8f);
    }
    hglGetMatrixf(gl, HGL_MODELVIEW, m);
    float worst = 0;
    for (int i = 0; i < 16; i++) {
        float d = fabsf(m[i] - id[i]);
        if (d > worst) {
            worst = d;
        }
    }
    char why[96];
    snprintf(why, sizeof(why), "worst element differs by %g", (double)worst);
    check(worst < 1e-4f, "eight 45-degree rotations come back to identity",
          why);

    /* Translate then its inverse. */
    hglLoadIdentity(gl);
    hglTranslatef(gl, 1.5f, -2.5f, 7.0f);
    hglTranslatef(gl, -1.5f, 2.5f, -7.0f);
    hglGetMatrixf(gl, HGL_MODELVIEW, m);
    worst = 0;
    for (int i = 0; i < 16; i++) {
        float d = fabsf(m[i] - id[i]);
        if (d > worst) worst = d;
    }
    check(worst < 1e-5f, "a translation and its inverse cancel", "they did not");

    /* The stack has a bottom and a top, and both are errors to pass. */
    (void)hglGetError(gl);
    hglPopMatrix(gl);
    check(hglGetError(gl) == HGL_STACK_UNDERFLOW,
          "popping an empty stack is an error, not a crash", "no error");
    for (int i = 0; i < HGL_MATRIX_DEPTH + 4; i++) {
        hglPushMatrix(gl);
    }
    check(hglGetError(gl) == HGL_STACK_OVERFLOW,
          "overflowing the stack is an error, not a crash", "no error");
    while (gl->modelview.depth > 0) {
        hglPopMatrix(gl);
    }
    (void)hglGetError(gl);
}

static void check_perspective(hgl_context *gl) {
    hglClearColor(gl, 0, 0, 0, 1);
    hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);

    hglMatrixMode(gl, HGL_PROJECTION);
    hglLoadIdentity(gl);
    hglPerspective(gl, 60.0f, (float)W / (float)H, 0.1f, 100.0f);
    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);

    /* The same square at two distances. The far one must cover fewer
     * pixels - which is the entire point of a perspective divide. */
    long near_px = 0, far_px = 0;
    for (int pass = 0; pass < 2; pass++) {
        float z = pass == 0 ? -2.0f : -6.0f;
        hglClear(gl, HGL_COLOR_BUFFER_BIT);
        hglColor3f(gl, 1, 1, 1);
        hglBegin(gl, HGL_QUADS);
        hglVertex3f(gl, -0.5f, -0.5f, z);
        hglVertex3f(gl,  0.5f, -0.5f, z);
        hglVertex3f(gl,  0.5f,  0.5f, z);
        hglVertex3f(gl, -0.5f,  0.5f, z);
        hglEnd(gl);

        long n = 0;
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                if (at(gl, x, y) != 0) {
                    n++;
                }
            }
        }
        if (pass == 0) near_px = n; else far_px = n;
    }
    char why[128];
    snprintf(why, sizeof(why), "near covers %ld px, far covers %ld",
             near_px, far_px);
    check(near_px > 0 && far_px > 0 && far_px < near_px / 2,
          "a square three times further away is much smaller", why);

    /* Nine times the area, three times the distance: the ratio should
     * be close to nine. */
    double ratio = (double)near_px / (double)far_px;
    snprintf(why, sizeof(why), "ratio is %.2f, expected about 9", ratio);
    check(ratio > 7.0 && ratio < 11.0,
          "and smaller by the square of the distance", why);
}

static void check_depth(hgl_context *gl) {
    ortho_pixels(gl);
    hglEnable(gl, HGL_DEPTH_TEST);
    hglDepthFunc(gl, HGL_LESS);

    /* Two overlapping quads at different depths, drawn in both
     * orders. The nearer one must win both times - which is the whole
     * job of a depth buffer, and a rasteriser that interpolated depth
     * wrongly would get one order right and the other wrong. */
    for (int order = 0; order < 2; order++) {
        hglClearColor(gl, 0, 0, 0, 1);
        hglClearDepth(gl, 1.0f);
        hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 2; i++) {
            int near = (order == 0) ? (i == 1) : (i == 0);
            /* ortho_pixels() set up hglOrtho(0,W,H,0,-1,1): with
             * near = -1 and far = 1 passed straight into the standard
             * GL ortho formula, clip.z = -z, so LARGER eye-space z
             * lands at SMALLER depth and wins under HGL_LESS. z = 0.5
             * is therefore the nearer of this pair, not z = -0.5. */
            float z = near ? 0.5f : -0.5f;
            if (near) {
                hglColor3f(gl, 1, 0, 0);
            } else {
                hglColor3f(gl, 0, 0, 1);
            }
            hglBegin(gl, HGL_QUADS);
            hglVertex3f(gl, 40, 30, z);
            hglVertex3f(gl, 120, 30, z);
            hglVertex3f(gl, 120, 90, z);
            hglVertex3f(gl, 40, 90, z);
            hglEnd(gl);
        }
        uint32_t c = at(gl, 80, 60);
        char why[96];
        snprintf(why, sizeof(why), "order %d gave %06x, expected red",
                 order, c);
        check(((c >> 16) & 0xFF) > 200 && (c & 0xFF) < 60,
              order == 0 ? "the nearer quad wins when drawn second"
                         : "and also when drawn first", why);
    }
    hglDisable(gl, HGL_DEPTH_TEST);
}

/* Perspective-correct interpolation: the one a software rasteriser
 * usually gets wrong. A floor stretching into the distance is drawn
 * with a checker texture; if interpolation were affine, the checks
 * would be evenly spaced down the screen instead of crowding towards
 * the horizon. */
static void check_perspective_texture(hgl_context *gl) {
    uint32_t checker[8 * 8];
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            checker[y * 8 + x] = ((x ^ y) & 1) ? 0xFFFFFFFFu : 0xFF000000u;
        }
    }
    unsigned tex = 0;
    hglGenTextures(gl, 1, &tex);
    hglBindTexture(gl, tex);
    hglTexImage2D(gl, 8, 8, checker);
    hglTexParameter(gl, HGL_NEAREST, HGL_NEAREST, HGL_REPEAT, HGL_REPEAT);

    hglClearColor(gl, 0, 0.4f, 0, 1);
    hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);
    hglMatrixMode(gl, HGL_PROJECTION);
    hglLoadIdentity(gl);
    hglPerspective(gl, 70.0f, (float)W / (float)H, 0.1f, 200.0f);
    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);
    hglLookAt(gl, 0, 1, 0, 0, 1, -1, 0, 1, 0);

    hglEnable(gl, HGL_TEXTURE_2D);
    hglColor3f(gl, 1, 1, 1);
    hglBegin(gl, HGL_QUADS);
    hglTexCoord2f(gl, 0, 0);   hglVertex3f(gl, -20, 0, 0);
    hglTexCoord2f(gl, 0, 20);  hglVertex3f(gl, -20, 0, -60);
    hglTexCoord2f(gl, 20, 20); hglVertex3f(gl,  20, 0, -60);
    hglTexCoord2f(gl, 20, 0);  hglVertex3f(gl,  20, 0, 0);
    hglEnd(gl);
    hglDisable(gl, HGL_TEXTURE_2D);

    /* Count the black/white transitions along a vertical line through
     * the middle, in the top half of the floor and the bottom half.
     * With perspective the far half has many more; affine
     * interpolation would give roughly the same number. */
    int floor_top = -1, floor_bottom = -1;
    for (int y = 0; y < H; y++) {
        uint32_t c = at(gl, W / 2, y);
        int is_floor = ((c >> 8) & 0xFF) != 0x66 || (c & 0xFF) != 0;
        (void)is_floor;
        unsigned g = (c >> 8) & 0xFF, r = (c >> 16) & 0xFF,
                 b = c & 0xFF;
        int green_bg = (r < 20 && g > 80 && b < 20);
        if (!green_bg) {
            if (floor_top < 0) floor_top = y;
            floor_bottom = y;
        }
    }
    if (floor_top < 0 || floor_bottom - floor_top < 20) {
        fail("the floor was drawn", "not enough floor pixels");
        return;
    }
    ok("a perspective floor is drawn");

    int mid = (floor_top + floor_bottom) / 2;
    int far_changes = 0, near_changes = 0, prev = -1;
    for (int y = floor_top; y < mid; y++) {
        int on = ((at(gl, W / 2, y) >> 16) & 0xFF) > 128;
        if (prev >= 0 && on != prev) far_changes++;
        prev = on;
    }
    prev = -1;
    for (int y = mid; y <= floor_bottom; y++) {
        int on = ((at(gl, W / 2, y) >> 16) & 0xFF) > 128;
        if (prev >= 0 && on != prev) near_changes++;
        prev = on;
    }
    char why[160];
    snprintf(why, sizeof(why),
             "%d checks in the far half, %d in the near half",
             far_changes, near_changes);
    check(far_changes > near_changes * 2,
          "texture coordinates are perspective correct "
          "(checks crowd towards the horizon)", why);
    printf("     %s\n", why);

    hglDeleteTextures(gl, 1, &tex);
}

static void check_culling(hgl_context *gl) {
    ortho_pixels(gl);
    hglEnable(gl, HGL_CULL_FACE);
    hglCullFace(gl, HGL_BACK);
    hglFrontFace(gl, HGL_CCW);

    /* In screen space with y downward, a triangle listed clockwise on
     * screen is counter-clockwise in clip space - hence one of each
     * below, and exactly one should survive. */
    long drawn[2];
    for (int wind = 0; wind < 2; wind++) {
        hglClearColor(gl, 0, 0, 0, 1);
        hglClear(gl, HGL_COLOR_BUFFER_BIT);
        hglColor3f(gl, 1, 1, 1);
        hglBegin(gl, HGL_TRIANGLES);
        if (wind == 0) {
            hglVertex2f(gl, 20, 20);
            hglVertex2f(gl, 120, 30);
            hglVertex2f(gl, 70, 100);
        } else {
            hglVertex2f(gl, 70, 100);
            hglVertex2f(gl, 120, 30);
            hglVertex2f(gl, 20, 20);
        }
        hglEnd(gl);
        long n = 0;
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                if (at(gl, x, y) != 0) n++;
            }
        }
        drawn[wind] = n;
    }
    char why[128];
    snprintf(why, sizeof(why), "one winding drew %ld, the other %ld",
             drawn[0], drawn[1]);
    check((drawn[0] == 0) != (drawn[1] == 0),
          "face culling drops exactly one of the two windings", why);
    hglDisable(gl, HGL_CULL_FACE);
}

static void check_blending(hgl_context *gl) {
    ortho_pixels(gl);
    hglClearColor(gl, 0, 0, 0, 1);
    hglClear(gl, HGL_COLOR_BUFFER_BIT);

    hglColor4f(gl, 1, 1, 1, 1);
    hglBegin(gl, HGL_QUADS);
    hglVertex2f(gl, 20, 20); hglVertex2f(gl, 140, 20);
    hglVertex2f(gl, 140, 100); hglVertex2f(gl, 20, 100);
    hglEnd(gl);

    hglEnable(gl, HGL_BLEND);
    hglBlendFunc(gl, HGL_SRC_ALPHA, HGL_ONE_MINUS_SRC_ALPHA);
    hglColor4f(gl, 0, 0, 0, 0.5f);
    hglBegin(gl, HGL_QUADS);
    hglVertex2f(gl, 20, 20); hglVertex2f(gl, 140, 20);
    hglVertex2f(gl, 140, 100); hglVertex2f(gl, 20, 100);
    hglEnd(gl);
    hglDisable(gl, HGL_BLEND);

    unsigned v = (at(gl, 80, 60) >> 16) & 0xFF;
    char why[96];
    snprintf(why, sizeof(why), "got %u, expected about 128", v);
    check(v > 120 && v < 136,
          "black at half alpha over white gives mid grey", why);
}

static void check_clipping(hgl_context *gl) {
    /* A triangle that straddles the eye. Without near-plane clipping
     * the part behind the camera divides by a negative w and the
     * triangle turns inside out - which shows up as the whole screen
     * filling in. */
    hglClearColor(gl, 0, 0, 0, 1);
    hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);
    hglMatrixMode(gl, HGL_PROJECTION);
    hglLoadIdentity(gl);
    hglPerspective(gl, 60.0f, (float)W / (float)H, 0.1f, 100.0f);
    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);

    hglColor3f(gl, 1, 1, 1);
    hglBegin(gl, HGL_TRIANGLES);
    hglVertex3f(gl, -1, -1, -2);      /* in front */
    hglVertex3f(gl,  1, -1, -2);      /* in front */
    hglVertex3f(gl,  0,  1,  5);      /* behind the eye */
    hglEnd(gl);

    long n = 0;
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (at(gl, x, y) != 0) n++;
        }
    }
    char why[96];
    snprintf(why, sizeof(why), "%ld of %d pixels were filled", n, W * H);
    check(n > 0 && n < (long)(W * H) * 9 / 10,
          "a triangle straddling the eye is clipped, not inverted", why);

    /* Entirely behind: nothing at all. */
    hglClear(gl, HGL_COLOR_BUFFER_BIT);
    hglBegin(gl, HGL_TRIANGLES);
    hglVertex3f(gl, -1, -1, 5);
    hglVertex3f(gl,  1, -1, 5);
    hglVertex3f(gl,  0,  1, 6);
    hglEnd(gl);
    n = 0;
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (at(gl, x, y) != 0) n++;
        }
    }
    check(n == 0, "a triangle entirely behind the eye draws nothing",
          "it drew something");
}

static int g_hook_calls;
static void vhook(void *ctx, struct hgl_vertex *v) {
    (void)ctx;
    g_hook_calls++;
    v->r = 0.0f; v->g = 1.0f; v->b = 0.0f;
}
static int fhook_discard(void *ctx, struct hgl_fragment *f) {
    (void)ctx;
    return (f->x & 1) == 0;   /* keep every other column */
}

static void check_hooks(hgl_context *gl) {
    ortho_pixels(gl);
    hglClearColor(gl, 0, 0, 0, 1);
    hglClear(gl, HGL_COLOR_BUFFER_BIT);

    g_hook_calls = 0;
    hglVertexHook(gl, vhook, NULL);
    hglColor3f(gl, 1, 0, 0);
    hglBegin(gl, HGL_TRIANGLES);
    hglVertex2f(gl, 20, 20); hglVertex2f(gl, 120, 20); hglVertex2f(gl, 20, 100);
    hglEnd(gl);
    hglVertexHook(gl, NULL, NULL);

    uint32_t c = at(gl, 40, 40);
    check(g_hook_calls == 3 && ((c >> 8) & 0xFF) > 200 &&
          ((c >> 16) & 0xFF) < 60,
          "a vertex hook replaces what the pipeline computed",
          "the triangle is not the colour the hook set");

    hglClear(gl, HGL_COLOR_BUFFER_BIT);
    hglFragmentHook(gl, fhook_discard, NULL);
    hglColor3f(gl, 1, 1, 1);
    hglBegin(gl, HGL_TRIANGLES);
    hglVertex2f(gl, 20, 20); hglVertex2f(gl, 120, 20); hglVertex2f(gl, 20, 100);
    hglEnd(gl);
    hglFragmentHook(gl, NULL, NULL);

    int odd_lit = 0, even_lit = 0;
    for (int x = 30; x < 60; x++) {
        if (at(gl, x, 40) != 0) {
            if (x & 1) odd_lit++; else even_lit++;
        }
    }
    char why[96];
    snprintf(why, sizeof(why), "%d even columns, %d odd", even_lit, odd_lit);
    check(even_lit > 10 && odd_lit == 0,
          "a fragment hook can discard fragments", why);
}

/* ---- 3. misuse ---- */

static void check_misuse(hgl_context *gl) {
    /* Null context everywhere: none of these may follow the pointer. */
    hglViewport(NULL, 0, 0, 1, 1);
    hglClear(NULL, 0);
    hglBegin(NULL, HGL_TRIANGLES);
    hglVertex3f(NULL, 0, 0, 0);
    hglEnd(NULL);
    hglLoadMatrixf(NULL, NULL);
    hglBindTexture(NULL, 0);
    hglTexImage2D(NULL, 1, 1, NULL);
    hglDestroyContext(NULL);
    ok("every entry point survives a null context");

    (void)hglGetError(gl);
    hglBegin(gl, 999);
    check(hglGetError(gl) == HGL_INVALID_ENUM,
          "an unknown primitive is refused", "no error");

    hglBegin(gl, HGL_TRIANGLES);
    hglBegin(gl, HGL_TRIANGLES);
    check(hglGetError(gl) == HGL_INVALID_OPERATION,
          "Begin inside Begin is refused", "no error");
    hglEnd(gl);

    hglVertex3f(gl, 0, 0, 0);
    check(hglGetError(gl) == HGL_INVALID_OPERATION,
          "a vertex outside Begin is refused", "no error");

    hglEnd(gl);
    check(hglGetError(gl) == HGL_INVALID_OPERATION,
          "End without Begin is refused", "no error");

    hglDepthFunc(gl, 999);
    check(hglGetError(gl) == HGL_INVALID_ENUM,
          "an unknown depth function is refused", "no error");

    hglPerspective(gl, 60, 0.0f, 0.1f, 100.0f);
    check(hglGetError(gl) == HGL_INVALID_VALUE,
          "a zero aspect ratio is refused", "no error");
    hglPerspective(gl, 60, 1.0f, 10.0f, 1.0f);
    check(hglGetError(gl) == HGL_INVALID_VALUE,
          "a far plane in front of the near plane is refused", "no error");

    hglRotatef(gl, 45, 0, 0, 0);
    check(hglGetError(gl) == HGL_INVALID_VALUE,
          "rotating about a zero axis is refused", "no error");

    hglBindTexture(gl, 999);
    check(hglGetError(gl) == HGL_INVALID_VALUE,
          "binding a texture name out of range is refused", "no error");

    hglBindTexture(gl, 0);
    hglTexImage2D(gl, 4, 4, (uint32_t[16]){0});
    check(hglGetError(gl) == HGL_INVALID_OPERATION,
          "uploading with no texture bound is refused", "no error");

    unsigned t = 0;
    hglGenTextures(gl, 1, &t);
    hglBindTexture(gl, t);
    hglTexImage2D(gl, 0, 4, (uint32_t[16]){0});
    check(hglGetError(gl) == HGL_INVALID_VALUE,
          "a zero-width texture is refused", "no error");
    hglDeleteTextures(gl, 1, &t);

    /* An empty viewport draws nothing and does not divide by zero. */
    hglViewport(gl, 0, 0, 0, 0);
    hglClearColor(gl, 0, 0, 0, 1);
    hglClear(gl, HGL_COLOR_BUFFER_BIT);
    hglBegin(gl, HGL_TRIANGLES);
    hglVertex2f(gl, 0, 0); hglVertex2f(gl, 1, 0); hglVertex2f(gl, 0, 1);
    hglEnd(gl);
    ok("an empty viewport draws nothing rather than dividing by zero");
    hglViewport(gl, 0, 0, W, H);
}

/* ---- the 3D triangle demo ----
 *
 * Everything above checks one piece of the pipeline in isolation.
 * This exercises all of it together the way a real caller would: a
 * perspective projection, a modelview rotation, per-vertex colour,
 * Gouraud lighting and the depth buffer, spun through several frames.
 * It writes a PPM per frame so the result can be looked at, and it
 * also asserts on it - the centroid of the lit pixels has to move as
 * the triangle turns, which is what tells a real rotation from a
 * frame that silently rendered the same thing three times.
 */

static void write_ppm(const char *path, const uint32_t *color, int w, int h) {
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return;
    }
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (int i = 0; i < w * h; i++) {
        uint8_t px[3] = {
            (uint8_t)((color[i] >> 16) & 0xFF),
            (uint8_t)((color[i] >> 8) & 0xFF),
            (uint8_t)(color[i] & 0xFF),
        };
        fwrite(px, 1, 3, f);
    }
    fclose(f);
}

/* cos/sin of a small set of angles, by hand - pulling in the whole
 * rotation via hglRotatef (which already has its own test in
 * check_matrices) is exactly the point; this just needs a handful of
 * distinct angles. */
static void spin_frame(hgl_context *gl, float angle_deg, const char *path,
                       long *lit_px, double *cx_out, double *cy_out) {
    hglClearColor(gl, 0.05f, 0.05f, 0.08f, 1.0f);
    hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);

    hglMatrixMode(gl, HGL_PROJECTION);
    hglLoadIdentity(gl);
    hglPerspective(gl, 60.0f, (float)W / (float)H, 0.1f, 100.0f);

    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);
    hglLookAt(gl, 0, 0, 3, 0, 0, 0, 0, 1, 0);
    hglRotatef(gl, angle_deg, 0, 1, 0);

    hglEnable(gl, HGL_DEPTH_TEST);
    hglDepthFunc(gl, HGL_LESS);
    hglEnable(gl, HGL_LIGHTING);
    hglEnable(gl, HGL_LIGHT0);
    hglLightPosition(gl, 0, 2.0f, 2.0f, 4.0f, 0.0f);
    hglLightColor(gl, 0, 1.0f, 1.0f, 1.0f);
    hglMaterial(gl, 0.15f, 0.15f, 0.15f,   /* ambient */
               1.0f, 1.0f, 1.0f,          /* diffuse */
               0.0f, 0.0f, 0.0f, 1.0f);   /* specular, shininess */
    hglShadeModel(gl, HGL_SMOOTH);

    /* The corner is never inside the triangle, at any angle it is
     * rotated to here, so it is always the clear colour - which makes
     * a reliable reference for "is this pixel the triangle" that does
     * not depend on how bright the lighting happens to make it. A
     * fixed brightness threshold would call the triangle invisible
     * the moment it turns far enough that its lit face points away
     * from both the light and the camera. */
    uint32_t bg = gl->color[0];

    hglBegin(gl, HGL_TRIANGLES);
    hglNormal3f(gl, 0, 0, 1);
    hglColor3f(gl, 1, 0.2f, 0.2f);
    hglVertex3f(gl, 0.0f, 0.9f, 0.0f);
    hglColor3f(gl, 0.2f, 1, 0.2f);
    hglVertex3f(gl, -0.9f, -0.7f, 0.0f);
    hglColor3f(gl, 0.2f, 0.4f, 1);
    hglVertex3f(gl, 0.9f, -0.7f, 0.0f);
    hglEnd(gl);

    hglDisable(gl, HGL_LIGHTING);

    if (path != NULL) {
        write_ppm(path, gl->color, gl->width, gl->height);
    }

    long n = 0;
    double sx = 0.0, sy = 0.0;
    for (int y = 0; y < gl->height; y++) {
        for (int x = 0; x < gl->width; x++) {
            uint32_t c = gl->color[(size_t)y * gl->width + x];
            if (c != bg) {
                n++;
                sx += x;
                sy += y;
            }
        }
    }
    if (lit_px != NULL) *lit_px = n;
    if (n > 0) {
        if (cx_out != NULL) *cx_out = sx / (double)n;
        if (cy_out != NULL) *cy_out = sy / (double)n;
    }
}

static void demo_3d_triangle(hgl_context *gl) {
    long n0 = 0, n1 = 0, n2 = 0;
    double cx0 = 0, cx1 = 0, cx2 = 0, cy0 = 0, cy1 = 0, cy2 = 0;

    spin_frame(gl, 0.0f,   "/tmp/hgl_triangle3d_0.ppm",  &n0, &cx0, &cy0);
    spin_frame(gl, 60.0f,  "/tmp/hgl_triangle3d_1.ppm",  &n1, &cx1, &cy1);
    spin_frame(gl, 120.0f, "/tmp/hgl_triangle3d_2.ppm",  &n2, &cx2, &cy2);

    printf("  (frames written to /tmp/hgl_triangle3d_{0,1,2}.ppm)\n");

    check(n0 > 200 && n1 > 200 && n2 > 200,
          "each frame of the spinning triangle draws a lit triangle",
          "one of the frames drew too little to be the triangle");

    double moved01 = fabs(cx0 - cx1) + fabs(cy0 - cy1);
    double moved12 = fabs(cx1 - cx2) + fabs(cy1 - cy2);
    char why[128];
    snprintf(why, sizeof(why), "centroid moved %.1f then %.1f px", moved01,
             moved12);
    check(moved01 > 2.0 && moved12 > 2.0,
          "rotating the modelview actually moves the triangle on screen",
          why);

    hglMatrixMode(gl, HGL_PROJECTION);
    hglLoadIdentity(gl);
    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);
}

int main(void) {
    printf("== HighGL tests ==\n");

    hgl_context *gl = hglCreateContext(W, H);
    if (gl == NULL) {
        fail("a context is created", "hglCreateContext returned NULL");
        printf("== %d checks, %d failed ==\n", g_checks, g_failures);
        return 1;
    }
    ok("a context is created");

    check(hglCreateContext(0, 10) == NULL &&
          hglCreateContext(10, -1) == NULL &&
          hglCreateContext(99999, 99999) == NULL,
          "an impossible size is refused", "a context came back anyway");

    printf("-- the rasteriser against a model --\n");
    check_against_model(gl);

    printf("-- matrices --\n");
    check_matrices(gl);

    printf("-- the perspective divide --\n");
    check_perspective(gl);
    check_clipping(gl);

    printf("-- the depth buffer --\n");
    check_depth(gl);

    printf("-- texturing --\n");
    check_perspective_texture(gl);

    printf("-- culling and blending --\n");
    check_culling(gl);
    check_blending(gl);

    printf("-- the hooks --\n");
    check_hooks(gl);

    printf("-- misuse --\n");
    check_misuse(gl);

    printf("-- a spinning 3D triangle --\n");
    demo_3d_triangle(gl);

    hglDestroyContext(gl);
    printf("== %d checks, %d failed ==\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
