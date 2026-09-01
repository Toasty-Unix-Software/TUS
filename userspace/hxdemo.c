/*
 * hxdemo - a highX application
 *
 * A small client that exercises most of the protocol: it creates one
 * window, uploads a gradient with HX_OP_PUT_IMAGE, animates a box
 * with fill/line requests, echoes the keys it receives while focused
 * and repaints itself whenever the window manager resizes it.
 *
 * Run it from tusWM with Ctrl+N, or straight from a highX session:
 *   highx hxdemo
 */

#include "highapi/highapi.h"

#include <stdio.h>
#include <string.h>

#define MAX_GRADIENT_W 1600
#define STRIP_H        28

/* One scanline block of the header gradient, uploaded in a single
 * HX_OP_PUT_IMAGE request. Wide enough for the screens TUS boots on;
 * a wider window simply gets a gradient that stops early. */
static unsigned int g_gradient[MAX_GRADIENT_W * STRIP_H];

static struct hx_display dpy;
static unsigned int win;
static unsigned int win_w = 380, win_h = 260;

/* Animation state: a box bouncing inside the content panel, tracked
 * in window coordinates so only the pixels it touches are repainted. */
static int boxx = 24, boxy = 60, dx = 4, dy = 3;
static int prevx = 24, prevy = 60;
static unsigned long frames;
static char keys[41];

#define BOX_W 34
#define BOX_H 26

#define COL_BG      0x00161E28u
#define COL_PANEL   0x001E2A38u
#define COL_TEXT    0x00DCE6F0u
#define COL_DIM     0x008494A4u
#define COL_BOX     0x00E0A040u
#define COL_EDGE    0x00404E5Cu

static int panel_x(void)  { return 8; }
static int panel_y(void)  { return STRIP_H + 8; }
static int panel_w(void)  { return (int)win_w - 16; }
static int panel_h(void) {
    int h = (int)win_h - panel_y() - 56;
    return h < 40 ? 40 : h;
}
static int status_y(void) { return (int)win_h - 46; }

static void put_gradient(void) {
    unsigned int w = win_w > MAX_GRADIENT_W ? MAX_GRADIENT_W : win_w;
    for (unsigned int y = 0; y < STRIP_H; y++) {
        for (unsigned int x = 0; x < w; x++) {
            unsigned int r = 0x20 + (x * 0x60) / (w ? w : 1);
            unsigned int g = 0x40 + (x * 0x90) / (w ? w : 1);
            unsigned int b = 0x80 + (y * 0x40) / STRIP_H;
            g_gradient[y * w + x] = (r << 16) | (g << 8) | b;
        }
    }
    /* One HX_OP_PUT_IMAGE request uploads the whole strip. */
    hx_image(win, 0, 0, w, STRIP_H, g_gradient);
}

static void draw_box(void) {
    hx_fill(win, boxx, boxy, BOX_W, BOX_H, COL_BOX);
    hx_line(win, boxx, boxy, boxx + BOX_W - 1, boxy + BOX_H - 1, COL_BG);
    hx_line(win, boxx + BOX_W - 1, boxy, boxx, boxy + BOX_H - 1, COL_BG);
}

static void draw_status(void) {
    char line[HX_TEXT_MAX];

    hx_fill(win, 0, status_y() - 2, win_w, 40, COL_BG);
    snprintf(line, sizeof(line), "window %u  %ux%u  frame %lu", win, win_w,
             win_h, frames);
    hx_text(win, 12, status_y(), COL_TEXT, 0, 0, line);
    snprintf(line, sizeof(line), "keys: %s", keys);
    hx_text(win, 12, status_y() + 18, COL_DIM, 0, 0, line);
    hx_commit_rect(win, 0, status_y() - 2, win_w, 40);
}

/* Full repaint: only on expose and after a resize. */
static void redraw(void) {
    hx_fill(win, 0, 0, win_w, win_h, COL_BG);
    put_gradient();
    hx_text(win, 8, STRIP_H / 2 - HX_FONT_H / 2, 0x00101820, 0, 0,
            "highX demo client");

    hx_fill(win, panel_x(), panel_y(), panel_w(), panel_h(), COL_PANEL);
    hx_rect(win, panel_x(), panel_y(), panel_w(), panel_h(), COL_EDGE);
    draw_box();
    hx_commit(win);
    draw_status();
}

/* One animation frame: erase the box where it was, draw it where it
 * is now and commit just that region - highX repaints exactly the
 * damaged rectangle, so an animation costs almost nothing. */
static void step(void) {
    int px = panel_x() + 2, py = panel_y() + 2;
    int pw = panel_w() - 4, ph = panel_h() - 4;

    prevx = boxx;
    prevy = boxy;
    boxx += dx;
    boxy += dy;
    if (boxx < px) {
        boxx = px;
        dx = -dx;
    }
    if (boxx + BOX_W > px + pw) {
        boxx = px + pw - BOX_W;
        dx = -dx;
    }
    if (boxy < py) {
        boxy = py;
        dy = -dy;
    }
    if (boxy + BOX_H > py + ph) {
        boxy = py + ph - BOX_H;
        dy = -dy;
    }
    frames++;

    hx_fill(win, prevx, prevy, BOX_W, BOX_H, COL_PANEL);
    draw_box();

    int rx = prevx < boxx ? prevx : boxx;
    int ry = prevy < boxy ? prevy : boxy;
    int rw = (prevx > boxx ? prevx : boxx) - rx + BOX_W;
    int rh = (prevy > boxy ? prevy : boxy) - ry + BOX_H;
    hx_commit_rect(win, rx, ry, rw, rh);

    if ((frames % 5) == 0) {
        draw_status();
    }
}

int main(void) {
    if (hx_open(&dpy) < 0) {
        printf("hxdemo: no highX display (start one with `highx`)\n");
        return 1;
    }
    keys[0] = '\0';

    if (win_w > dpy.screen_w) {
        win_w = dpy.screen_w;
    }
    if (win_h > dpy.screen_h) {
        win_h = dpy.screen_h;
    }
    win = hx_create_window(60, 60, win_w, win_h, 0, COL_BG, "hxdemo");
    if (win == 0) {
        printf("hxdemo: cannot create a window\n");
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);

    for (;;) {
        struct hx_event ev;
        int n = hx_next_event(&ev, 40);
        if (n > 0) {
            if (ev.type == HX_EV_CLOSE) {
                break;
            }
            if (ev.type == HX_EV_CONFIGURE) {
                win_w = ev.w;
                win_h = ev.h;
                boxx = panel_x() + 4;
                boxy = panel_y() + 4;
                continue;
            }
            if (ev.type == HX_EV_EXPOSE) {
                redraw();
                continue;
            }
            if (ev.type == HX_EV_KEY) {
                size_t len = strlen(keys);
                char c = (ev.key >= 0x20 && ev.key < 0x7F) ? (char)ev.key : '.';
                if (len + 1 >= sizeof(keys)) {
                    memmove(keys, keys + 1, len);
                    len--;
                }
                keys[len] = c;
                keys[len + 1] = '\0';
                redraw();
                continue;
            }
            continue;
        }
        /* step() repaints and commits only the pixels the box
         * touched - the full redraw above is for exposes. */
        step();
    }

    hx_destroy_window(win);
    hx_close(&dpy);
    return 0;
}
