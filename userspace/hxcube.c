/*
 * hxcube - a highX application
 *
 * A small rotating wireframe/solid cube rendered with HighGL (TUS's
 * from-scratch software 3D pipeline, userspace/highgl) and blitted
 * into a highX window with a single HX_OP_PUT_IMAGE per frame, the
 * same way hxdemo uploads its gradient strip.
 *
 * Run it from tusWM or straight from a highX session:
 *   highx hxcube
 */

#include "highapi/highapi.h"
#include "highgl/highgl.h"

#include <stdio.h>
#include <string.h>

#define WIN_W 320
#define WIN_H 240

static struct hx_display dpy;
static unsigned int win;
static hgl_context *gl;

/* Cube: 8 vertices, 6 faces, each face a distinct color so the
 * rotation reads as a solid object rather than a wire cage. */
static const float verts[8][3] = {
    { -1, -1, -1 }, {  1, -1, -1 }, {  1,  1, -1 }, { -1,  1, -1 },
    { -1, -1,  1 }, {  1, -1,  1 }, {  1,  1,  1 }, { -1,  1,  1 },
};

static const int faces[6][4] = {
    { 0, 1, 2, 3 }, /* back   */
    { 4, 5, 6, 7 }, /* front  */
    { 0, 4, 7, 3 }, /* left   */
    { 1, 5, 6, 2 }, /* right  */
    { 3, 2, 6, 7 }, /* top    */
    { 0, 1, 5, 4 }, /* bottom */
};

static const float face_color[6][3] = {
    { 0.85f, 0.25f, 0.25f }, { 0.25f, 0.80f, 0.30f },
    { 0.25f, 0.35f, 0.90f }, { 0.90f, 0.80f, 0.20f },
    { 0.85f, 0.35f, 0.85f }, { 0.25f, 0.80f, 0.85f },
};

static void draw_cube(float angle_x, float angle_y) {
    hglClear(gl, HGL_COLOR_BUFFER_BIT | HGL_DEPTH_BUFFER_BIT);

    hglMatrixMode(gl, HGL_MODELVIEW);
    hglLoadIdentity(gl);
    hglTranslatef(gl, 0.0f, 0.0f, -5.0f);
    hglRotatef(gl, angle_x, 1.0f, 0.0f, 0.0f);
    hglRotatef(gl, angle_y, 0.0f, 1.0f, 0.0f);

    for (int f = 0; f < 6; f++) {
        hglColor3f(gl, face_color[f][0], face_color[f][1], face_color[f][2]);
        hglBegin(gl, HGL_QUADS);
        for (int v = 0; v < 4; v++) {
            int i = faces[f][v];
            hglVertex3f(gl, verts[i][0], verts[i][1], verts[i][2]);
        }
        hglEnd(gl);
    }
}

int main(void) {
    if (hx_open(&dpy) < 0) {
        fprintf(stderr, "hxcube: hx_open failed\n");
        return 1;
    }

    win = hx_create_window(80, 80, WIN_W, WIN_H, 0, 0x101418, "hxcube");
    if ((int)win < 0) {
        fprintf(stderr, "hxcube: hx_create_window failed\n");
        hx_close(&dpy);
        return 1;
    }

    gl = hglCreateContext(WIN_W, WIN_H);
    if (!gl) {
        fprintf(stderr, "hxcube: hglCreateContext failed\n");
        hx_close(&dpy);
        return 1;
    }

    hglViewport(gl, 0, 0, WIN_W, WIN_H);
    hglClearColor(gl, 0.06f, 0.07f, 0.09f, 1.0f);
    hglEnable(gl, HGL_DEPTH_TEST);
    hglEnable(gl, HGL_CULL_FACE);

    hglMatrixMode(gl, HGL_PROJECTION);
    hglLoadIdentity(gl);
    hglPerspective(gl, 60.0f, (float)WIN_W / (float)WIN_H, 0.1f, 100.0f);

    float ax = 0.0f, ay = 0.0f;
    int running = 1;

    while (running) {
        struct hx_event ev;
        while (hx_poll_event(&ev) == 1) {
            if (ev.type == HX_EV_CLOSE) {
                running = 0;
                break;
            }
            if (ev.type == HX_EV_KEY && ev.key == 'q') {
                running = 0;
                break;
            }
        }
        if (!running)
            break;

        draw_cube(ax, ay);
        ax += 1.3f;
        ay += 2.1f;
        if (ax >= 360.0f) ax -= 360.0f;
        if (ay >= 360.0f) ay -= 360.0f;

        hx_image(win, 0, 0, WIN_W, WIN_H, hglColorBuffer(gl));
        hx_commit(win);
    }

    hglDestroyContext(gl);
    hx_close(&dpy);
    return 0;
}
