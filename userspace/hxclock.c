/*
 * hxclock - a highX clock, drawn with seven segments
 *
 * The second highX client, deliberately different from hxdemo: it
 * draws nothing but rectangles, redraws only when the time changes
 * (so an idle window costs nothing) and keeps working at whatever
 * size the window manager gives it.
 *
 * Run it from tusWM with Ctrl+T, or as `highx hxclock`.
 */

#include "highapi/highapi.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#define COL_BG    0x00101820u
#define COL_ON    0x0050D0A0u
#define COL_OFF   0x001C2A32u
#define COL_TEXT  0x00A8B8C4u

static struct hx_display dpy;
static unsigned int win;
static unsigned int win_w = 300, win_h = 150;

/* Segment bits: a=1 b=2 c=4 d=8 e=16 f=32 g=64 (classic order). */
static const unsigned char g_digits[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static void draw_digit(int x, int y, int w, int h, int t, int value) {
    unsigned char seg = (value >= 0 && value <= 9) ? g_digits[value] : 0;
    int half = h / 2;

    hx_fill(win, x + t, y, w - 2 * t, t, (seg & 0x01) ? COL_ON : COL_OFF);
    hx_fill(win, x + w - t, y + t, t, half - t,
            (seg & 0x02) ? COL_ON : COL_OFF);
    hx_fill(win, x + w - t, y + half, t, half - t,
            (seg & 0x04) ? COL_ON : COL_OFF);
    hx_fill(win, x + t, y + h - t, w - 2 * t, t,
            (seg & 0x08) ? COL_ON : COL_OFF);
    hx_fill(win, x, y + half, t, half - t, (seg & 0x10) ? COL_ON : COL_OFF);
    hx_fill(win, x, y + t, t, half - t, (seg & 0x20) ? COL_ON : COL_OFF);
    hx_fill(win, x + t, y + half - t / 2, w - 2 * t, t,
            (seg & 0x40) ? COL_ON : COL_OFF);
}

static void redraw(unsigned long secs) {
    char line[HX_TEXT_MAX];

    /* Scale the display to the window: four digits, a colon and the
     * gaps between them have to fit horizontally. */
    int dw = ((int)win_w - 40) / 5;
    if (dw > 46) {
        dw = 46;
    }
    if (dw < 12) {
        dw = 12;
    }
    int dh = (int)win_h - 60;
    if (dh > dw * 2) {
        dh = dw * 2;
    }
    if (dh < 24) {
        dh = 24;
    }
    int t = dw / 6 > 3 ? dw / 6 : 3;
    int gap = dw / 4;
    int total = 4 * dw + 3 * gap + dw / 2;
    int x = ((int)win_w - total) / 2;
    int y = 26;

    unsigned long mins = (secs / 60) % 100;
    unsigned long ss = secs % 60;

    hx_fill(win, 0, 0, win_w, win_h, COL_BG);
    hx_text(win, 10, 6, COL_TEXT, 0, 0, "uptime");

    draw_digit(x, y, dw, dh, t, (int)(mins / 10));
    x += dw + gap;
    draw_digit(x, y, dw, dh, t, (int)(mins % 10));
    x += dw + gap;

    /* Colon, blinking once a second. */
    int cw = dw / 2;
    if ((secs & 1) == 0) {
        hx_fill(win, x + cw / 2 - t / 2, y + dh / 3, t, t, COL_ON);
        hx_fill(win, x + cw / 2 - t / 2, y + 2 * dh / 3, t, t, COL_ON);
    }
    x += cw + gap / 2;

    draw_digit(x, y, dw, dh, t, (int)(ss / 10));
    x += dw + gap;
    draw_digit(x, y, dw, dh, t, (int)(ss % 10));

    snprintf(line, sizeof(line), "%lu seconds since boot", secs);
    hx_text(win, 10, (int)win_h - 22, COL_TEXT, 0, 0, line);

    hx_commit(win);
}

int main(void) {
    if (hx_open(&dpy) < 0) {
        printf("hxclock: no highX display (start one with `highx`)\n");
        return 1;
    }
    if (win_w > dpy.screen_w) {
        win_w = dpy.screen_w;
    }
    if (win_h > dpy.screen_h) {
        win_h = dpy.screen_h;
    }

    win = hx_create_window(120, 120, win_w, win_h, 0, COL_BG, "hxclock");
    if (win == 0) {
        printf("hxclock: cannot create a window\n");
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);

    unsigned long shown = (unsigned long)-1;
    for (;;) {
        struct hx_event ev;
        int n = hx_next_event(&ev, 100);
        if (n > 0) {
            if (ev.type == HX_EV_CLOSE) {
                break;
            }
            if (ev.type == HX_EV_CONFIGURE) {
                win_w = ev.w;
                win_h = ev.h;
                shown = (unsigned long)-1;
                continue;
            }
            if (ev.type == HX_EV_EXPOSE) {
                shown = (unsigned long)-1;
                continue;
            }
            continue;
        }
        unsigned long secs = (unsigned long)time(NULL);
        if (secs != shown) {
            shown = secs;
            redraw(secs);
        }
    }

    hx_destroy_window(win);
    hx_close(&dpy);
    return 0;
}
