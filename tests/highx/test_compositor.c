/*
 * test_compositor.c - host unit tests for the highX compositor
 *
 * kernel/highx/compositor.c is pure pixel work over a framebuffer
 * description, so it can be compiled and exercised on the build host.
 * Three things are checked here:
 *
 *   1. Composition. Build window stacks, paint damaged rectangles
 *      with hxcomp_paint() and compare every pixel against a
 *      brute-force reference model (walk the stack bottom to top,
 *      last writer wins). That is what guards the occlusion shortcut:
 *      it skips windows it believes are hidden, and a wrong skip shows
 *      up here as a mismatching pixel instead of as a stale window on
 *      screen.
 *
 *   2. Window drawing. Every hxdraw_* primitive is run against a
 *      plain, obviously-correct model that writes one pixel at a time
 *      with a bounds test. The real ones clip once and then fill, copy
 *      and walk glyph bits in runs; this is what says the two agree,
 *      including at every edge of the backing store.
 *
 *   3. The cursor. Its art lives in the compositor, so instead of
 *      duplicating it the test checks the property that matters: a
 *      partial repaint of any rectangle must leave the screen exactly
 *      as a full repaint would, cursor included, and must never write
 *      outside the rectangle it was given.
 *
 * Everything runs twice, over a framebuffer whose pitch is the width
 * and over one with padded scanlines - which is what real hardware
 * hands out, and what puts alternate rows on an odd 8-byte boundary.
 *
 * Build and run:  make -C tests/highx run
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/highx.h"
#include "stubs.h"
#include "../../kernel/highx/highx.h"
#include "../../kernel/drivers/fb/font8x16.h"

#define SCREEN_W 320
#define SCREEN_H 200

static int g_failures;
static int g_checks;

/* Keep the compositor and the reference model on the same desktop. */
static void set_background(uint32_t color, uint32_t style) {
    hxcomp_set_background(color, style);
    stub_set_background(color, style);
}

static void report(const char *name, int bad) {
    g_checks++;
    if (bad != 0) {
        g_failures++;
    } else {
        printf("  [PASS] %s\n", name);
    }
}

/* ---- reference model: composition ---- */

static uint32_t ref_pixel(struct hxs_window **stack, int nstack,
                          int32_t x, int32_t y, uint32_t bg) {
    uint32_t out = bg;
    for (int i = 0; i < nstack; i++) {
        struct hxs_window *w = stack[i];
        if (w == NULL || w->id == 0 || !w->mapped || w->pix == NULL) {
            continue;
        }
        int32_t b = (int32_t)w->border_w;
        if (x >= w->x && y >= w->y && x < w->x + (int32_t)w->w &&
            y < w->y + (int32_t)w->h) {
            out = w->pix[(y - w->y) * (int32_t)w->w + (x - w->x)];
        } else if (b > 0 && x >= w->x - b && y >= w->y - b &&
                   x < w->x + (int32_t)w->w + b &&
                   y < w->y + (int32_t)w->h + b) {
            out = w->border_color & 0x00FFFFFFu;
        }
    }
    return out;
}

static struct hxs_window *make_window(uint32_t id, int32_t x, int32_t y,
                                      uint32_t w, uint32_t h, uint32_t fill,
                                      bool mapped) {
    struct hxs_window *win = calloc(1, sizeof(*win));
    win->id = id;
    win->x = x;
    win->y = y;
    win->w = w;
    win->h = h;
    win->mapped = mapped;
    win->pix = malloc((size_t)w * h * sizeof(uint32_t));
    for (uint32_t i = 0; i < w * h; i++) {
        /* A per-pixel pattern: a wrong source offset cannot pass. */
        win->pix[i] = fill + (i % 251);
    }
    return win;
}

/* Paint one rectangle and compare the whole screen with the model.
 * The screen starts from a full repaint, so only the damaged area may
 * differ - anything else means the compositor painted out of bounds. */
static void check_paint(const char *name, struct hxs_window **stack,
                        int nstack, int32_t x, int32_t y, int32_t w,
                        int32_t h) {
    /* Known-good starting point: repaint everything. */
    hxcomp_paint(0, 0, SCREEN_W, SCREEN_H, stack, nstack);

    /* Scribble over the damaged area so a compositor that paints
     * nothing at all cannot pass by accident. */
    for (int32_t sy = y; sy < y + h; sy++) {
        for (int32_t sx = x; sx < x + w; sx++) {
            if (sx >= 0 && sy >= 0 && sx < SCREEN_W && sy < SCREEN_H) {
                stub_fb_set(sx, sy, 0x00BADBADu);
            }
        }
    }

    hxcomp_paint(x, y, w, h, stack, nstack);

    int bad = 0;
    for (int32_t sy = 0; sy < SCREEN_H && bad < 4; sy++) {
        for (int32_t sx = 0; sx < SCREEN_W && bad < 4; sx++) {
            uint32_t got = stub_fb_get(sx, sy);
            uint32_t want = ref_pixel(stack, nstack, sx, sy,
                                      stub_background(sx, sy));
            if (got != want) {
                printf("  [FAIL] %s: pixel %d,%d is %06x, expected %06x\n",
                       name, sx, sy, got, want);
                bad++;
            }
        }
    }
    if (!stub_fb_pad_intact()) {
        printf("  [FAIL] %s: wrote into the scanline padding\n", name);
        bad++;
    }
    report(name, bad);
}

/* ---- reference model: window drawing ----
 *
 * One pixel at a time, one bounds test each: the shape the primitives
 * had before they were taught to work in runs. */

static uint32_t *g_ref;       /* reference backing store */
static uint32_t g_ref_w, g_ref_h;

static void ref_put(int32_t x, int32_t y, uint32_t color) {
    if (x >= 0 && y >= 0 && x < (int32_t)g_ref_w && y < (int32_t)g_ref_h) {
        g_ref[(size_t)y * g_ref_w + x] = color & 0x00FFFFFFu;
    }
}

static void ref_fill(int32_t x, int32_t y, int32_t w, int32_t h,
                     uint32_t color) {
    for (int32_t row = y; row < y + h; row++) {
        for (int32_t col = x; col < x + w; col++) {
            ref_put(col, row, color);
        }
    }
}

static void ref_rect(int32_t x, int32_t y, int32_t w, int32_t h,
                     uint32_t color) {
    if (w <= 0 || h <= 0) {
        return;
    }
    ref_fill(x, y, w, 1, color);
    ref_fill(x, y + h - 1, w, 1, color);
    ref_fill(x, y, 1, h, color);
    ref_fill(x + w - 1, y, 1, h, color);
}

static void ref_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                     uint32_t color) {
    int32_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int32_t dy = y1 - y0 >= 0 ? y1 - y0 : y0 - y1;
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx - dy;

    for (;;) {
        ref_put(x0, y0, color);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int32_t e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

static void ref_text(int32_t x, int32_t y, uint32_t fg, uint32_t bg,
                     uint32_t flags, const char *text) {
    for (const char *p = text; *p != '\0'; p++, x += FONT_WIDTH) {
        unsigned char c = (unsigned char)*p;
        if (c < FONT_FIRST || c > FONT_LAST) {
            c = '?';
        }
        const uint8_t *glyph = font8x16[c - FONT_FIRST];

        if (x + FONT_WIDTH <= 0 || x >= (int32_t)g_ref_w) {
            continue;
        }
        if ((flags & HX_TF_OPAQUE) != 0) {
            ref_fill(x, y, FONT_WIDTH, FONT_HEIGHT, bg);
        }
        for (int32_t row = 0; row < FONT_HEIGHT; row++) {
            uint8_t bits = glyph[row];
            for (int32_t col = 0; col < FONT_WIDTH; col++) {
                if ((bits & (0x80 >> col)) != 0) {
                    ref_put(x + col, y + row, fg);
                }
            }
        }
    }
}

static void ref_image(int32_t x, int32_t y, uint32_t w, uint32_t h,
                      const uint32_t *src) {
    for (uint32_t row = 0; row < h; row++) {
        for (uint32_t col = 0; col < w; col++) {
            ref_put(x + (int32_t)col, y + (int32_t)row,
                    src[(size_t)row * w + col]);
        }
    }
}

/* A window and a reference buffer holding the same starting pixels. */
static struct hxs_window *g_dw;

static void draw_begin(uint32_t w, uint32_t h) {
    if (g_dw != NULL) {
        free(g_dw->pix);
        free(g_dw);
        free(g_ref);
    }
    g_dw = make_window(1, 0, 0, w, h, 0x00303030, true);
    g_ref_w = w;
    g_ref_h = h;
    g_ref = malloc((size_t)w * h * sizeof(uint32_t));
    memcpy(g_ref, g_dw->pix, (size_t)w * h * sizeof(uint32_t));
}

static void draw_check(const char *name) {
    int bad = 0;
    for (uint32_t i = 0; i < g_ref_w * g_ref_h && bad < 4; i++) {
        if (g_dw->pix[i] != g_ref[i]) {
            printf("  [FAIL] %s: pixel %u,%u is %06x, expected %06x\n", name,
                   i % g_ref_w, i / g_ref_w, g_dw->pix[i], g_ref[i]);
            bad++;
        }
    }
    report(name, bad);
}

static void run_drawing_tests(void) {
    printf("-- window drawing --\n");

    /* An odd width puts half the rows on an odd 8-byte boundary. */
    draw_begin(37, 23);

    hxdraw_fill(g_dw, 3, 2, 10, 7, 0x00A03040);
    ref_fill(3, 2, 10, 7, 0x00A03040);
    draw_check("fill: inside");

    hxdraw_fill(g_dw, -5, -4, 12, 9, 0x0020C060);
    ref_fill(-5, -4, 12, 9, 0x0020C060);
    draw_check("fill: clipped at the top left");

    hxdraw_fill(g_dw, 30, 18, 40, 40, 0x00C0A020);
    ref_fill(30, 18, 40, 40, 0x00C0A020);
    draw_check("fill: clipped at the bottom right");

    hxdraw_fill(g_dw, 0, 0, 37, 23, 0x00112233);
    ref_fill(0, 0, 37, 23, 0x00112233);
    draw_check("fill: the whole window");

    hxdraw_fill(g_dw, 5, 5, 1, 1, 0x00FFFFFF);
    ref_fill(5, 5, 1, 1, 0x00FFFFFF);
    draw_check("fill: one pixel");

    hxdraw_fill(g_dw, 4, 4, 0, 5, 0x00FF0000);
    draw_check("fill: empty rectangle changes nothing");

    hxdraw_fill(g_dw, 7, 3, 9, 6, 0xFF445566); /* alpha must be dropped */
    ref_fill(7, 3, 9, 6, 0xFF445566);
    draw_check("fill: the top byte is dropped");

    hxdraw_rect(g_dw, 2, 2, 20, 12, 0x00E0E0E0);
    ref_rect(2, 2, 20, 12, 0x00E0E0E0);
    draw_check("rect: outline");

    hxdraw_rect(g_dw, -3, -3, 12, 12, 0x00203040);
    ref_rect(-3, -3, 12, 12, 0x00203040);
    draw_check("rect: clipped outline");

    hxdraw_line(g_dw, 1, 1, 30, 1, 0x00FF00FF);
    ref_line(1, 1, 30, 1, 0x00FF00FF);
    draw_check("line: horizontal");

    hxdraw_line(g_dw, 30, 20, 4, 20, 0x0000FFFF);
    ref_line(30, 20, 4, 20, 0x0000FFFF);
    draw_check("line: horizontal, right to left");

    hxdraw_line(g_dw, 6, 21, 6, 2, 0x00FFFF00);
    ref_line(6, 21, 6, 2, 0x00FFFF00);
    draw_check("line: vertical");

    hxdraw_line(g_dw, -10, 11, 50, 11, 0x00808080);
    ref_line(-10, 11, 50, 11, 0x00808080);
    draw_check("line: horizontal, clipped at both ends");

    hxdraw_line(g_dw, 9, -6, 9, 40, 0x00404040);
    ref_line(9, -6, 9, 40, 0x00404040);
    draw_check("line: vertical, clipped at both ends");

    hxdraw_line(g_dw, 0, 0, 36, 22, 0x0090C0F0);
    ref_line(0, 0, 36, 22, 0x0090C0F0);
    draw_check("line: diagonal");

    hxdraw_line(g_dw, 33, 3, 3, 19, 0x00F0C090);
    ref_line(33, 3, 3, 19, 0x00F0C090);
    draw_check("line: diagonal, the other way");

    hxdraw_line(g_dw, 12, 12, 12, 12, 0x00FFFFFF);
    ref_line(12, 12, 12, 12, 0x00FFFFFF);
    draw_check("line: a single point");

    hxdraw_text(g_dw, 1, 1, 0x00FFFFFF, 0, 0, "Hi!");
    ref_text(1, 1, 0x00FFFFFF, 0, 0, "Hi!");
    draw_check("text: transparent");

    hxdraw_text(g_dw, 2, 4, 0x00102030, 0x00E0D0C0, HX_TF_OPAQUE, "abc");
    ref_text(2, 4, 0x00102030, 0x00E0D0C0, HX_TF_OPAQUE, "abc");
    draw_check("text: opaque");

    hxdraw_text(g_dw, -12, 6, 0x00FF8000, 0x00003366, HX_TF_OPAQUE, "MMMM");
    ref_text(-12, 6, 0x00FF8000, 0x00003366, HX_TF_OPAQUE, "MMMM");
    draw_check("text: starts left of the window");

    hxdraw_text(g_dw, 30, 8, 0x0000FF00, 0, 0, "clipped right");
    ref_text(30, 8, 0x0000FF00, 0, 0, "clipped right");
    draw_check("text: runs off the right edge");

    hxdraw_text(g_dw, 4, -7, 0x00FF0000, 0x00112233, HX_TF_OPAQUE, "up");
    ref_text(4, -7, 0x00FF0000, 0x00112233, HX_TF_OPAQUE, "up");
    draw_check("text: half above the window");

    hxdraw_text(g_dw, 4, 17, 0x00FF0000, 0x00112233, HX_TF_OPAQUE, "down");
    ref_text(4, 17, 0x00FF0000, 0x00112233, HX_TF_OPAQUE, "down");
    draw_check("text: half below the window");

    hxdraw_text(g_dw, 4, 40, 0x00FF0000, 0, HX_TF_OPAQUE, "gone");
    draw_check("text: entirely below the window changes nothing");

    hxdraw_text(g_dw, 5, 2, 0x00FFFFFF, 0, 0, "\x01\x02~\x7f");
    ref_text(5, 2, 0x00FFFFFF, 0, 0, "\x01\x02~\x7f");
    draw_check("text: characters outside the font");

    hxdraw_text(g_dw, 3, 3, 0x00FFFFFF, 0, 0, "");
    draw_check("text: the empty string changes nothing");

    /* Images: a source with a distinct value per pixel, so a wrong
     * row stride or a wrong clip cannot go unnoticed. */
    static uint32_t img[16 * 11];
    for (unsigned i = 0; i < sizeof(img) / sizeof(img[0]); i++) {
        img[i] = 0xFF000000u | (i * 7919u);
    }

    hxdraw_image(g_dw, 4, 3, 16, 11, img);
    ref_image(4, 3, 16, 11, img);
    draw_check("image: inside");

    hxdraw_image(g_dw, -6, -5, 16, 11, img);
    ref_image(-6, -5, 16, 11, img);
    draw_check("image: clipped at the top left");

    hxdraw_image(g_dw, 28, 15, 16, 11, img);
    ref_image(28, 15, 16, 11, img);
    draw_check("image: clipped at the bottom right");

    hxdraw_image(g_dw, 100, 2, 16, 11, img);
    draw_check("image: entirely outside changes nothing");

    hxdraw_image(g_dw, 5, 5, 1, 1, img);
    ref_image(5, 5, 1, 1, img);
    draw_check("image: one pixel");

    /* NULL and empty arguments must be no-ops, not crashes. */
    hxdraw_fill(NULL, 0, 0, 4, 4, 0);
    hxdraw_rect(NULL, 0, 0, 4, 4, 0);
    hxdraw_line(NULL, 0, 0, 4, 4, 0);
    hxdraw_text(NULL, 0, 0, 0, 0, 0, "x");
    hxdraw_text(g_dw, 0, 0, 0, 0, 0, NULL);
    hxdraw_image(NULL, 0, 0, 4, 4, img);
    hxdraw_image(g_dw, 0, 0, 4, 4, NULL);
    hxdraw_image(g_dw, 0, 0, 0, 0, img);
    draw_check("null and empty arguments are no-ops");
}

/* ---- the cursor ----
 *
 * The sprite lives in the compositor and is deliberately not repeated
 * here. What is checked instead is the invariant a damage-based
 * compositor lives or dies by: repainting a rectangle must produce
 * exactly the pixels a full repaint would, and must touch nothing
 * else. */
static void check_cursor(const char *name, struct hxs_window **stack,
                         int nstack, int32_t px, int32_t py, int32_t dx,
                         int32_t dy, int32_t dw, int32_t dh) {
    static uint32_t golden[SCREEN_W * SCREEN_H];

    hxcomp_set_pointer(px, py);
    hxcomp_paint(0, 0, SCREEN_W, SCREEN_H, stack, nstack);
    for (int32_t y = 0; y < SCREEN_H; y++) {
        for (int32_t x = 0; x < SCREEN_W; x++) {
            golden[y * SCREEN_W + x] = stub_fb_get(x, y);
        }
    }

    for (int32_t y = dy; y < dy + dh; y++) {
        for (int32_t x = dx; x < dx + dw; x++) {
            if (x >= 0 && y >= 0 && x < SCREEN_W && y < SCREEN_H) {
                stub_fb_set(x, y, 0x00BADBADu);
            }
        }
    }
    hxcomp_paint(dx, dy, dw, dh, stack, nstack);

    int bad = 0;
    for (int32_t y = 0; y < SCREEN_H && bad < 4; y++) {
        for (int32_t x = 0; x < SCREEN_W && bad < 4; x++) {
            if (stub_fb_get(x, y) != golden[y * SCREEN_W + x]) {
                printf("  [FAIL] %s: pixel %d,%d is %06x, expected %06x\n",
                       name, x, y, stub_fb_get(x, y),
                       golden[y * SCREEN_W + x]);
                bad++;
            }
        }
    }
    if (!stub_fb_pad_intact()) {
        printf("  [FAIL] %s: wrote into the scanline padding\n", name);
        bad++;
    }
    report(name, bad);
}

static void run_cursor_tests(struct hxs_window **stack, int nstack) {
    printf("-- cursor --\n");
    hxcomp_show_pointer(true);

    check_cursor("cursor: partial repaint over its own rectangle", stack,
                 nstack, 160, 100, 160, 100, 12, 19);
    check_cursor("cursor: repaint of one corner of the sprite", stack, nstack,
                 160, 100, 164, 104, 4, 6);
    check_cursor("cursor: repaint that only clips its left edge", stack,
                 nstack, 160, 100, 150, 90, 15, 40);
    check_cursor("cursor: repaint nowhere near it", stack, nstack, 160, 100,
                 10, 10, 30, 30);
    check_cursor("cursor: against the top left corner", stack, nstack, -5, -7,
                 0, 0, 20, 20);
    check_cursor("cursor: against the bottom right corner", stack, nstack,
                 SCREEN_W - 3, SCREEN_H - 4, SCREEN_W - 20, SCREEN_H - 20,
                 20, 20);
    check_cursor("cursor: at the very last pixel", stack, nstack,
                 SCREEN_W - 1, SCREEN_H - 1, 0, 0, SCREEN_W, SCREEN_H);

    hxcomp_show_pointer(false);
}

/* ---- composition ---- */


/* ---- flicker: one repaint, one write per pixel ---- */

/*
 * Flicker is invisible to the pixel comparisons above, because the
 * screen ends up correct either way. What the eye catches is the
 * moment in between: a compositor that fills the desktop across the
 * whole damaged rectangle and then paints the windows back on top
 * writes every window pixel twice, and at drag speed that reads as a
 * flashing window.
 *
 * compositor.c is compiled here with -DHXCOMP_TRACE_WRITES, so the
 * property can simply be counted: within one hxcomp_paint(), no pixel
 * may be written more than once.
 */
static void check_single_write(const char *name, struct hxs_window **stack,
                               int nstack, int32_t x, int32_t y, int32_t w,
                               int32_t h) {
    stub_trace_reset();
    hxcomp_paint(x, y, w, h, stack, nstack);

    int32_t wx = -1, wy = -1;
    uint32_t worst = stub_trace_max(&wx, &wy);
    if (worst > 1) {
        printf("  [FAIL] %s: pixel (%d,%d) written %u times in one repaint\n",
               name, wx, wy, worst);
    }
    report(name, worst > 1 ? 1 : 0);
}

static void run_flicker_tests(void) {
    printf("-- one repaint, one write per pixel --\n");

    /* The grid desktop draws its dots over the fill it just laid
     * down, and the cursor is an overlay by nature: both are honest
     * second writes and neither is what flickers when a window moves.
     * A plain desktop with no pointer leaves the window painting on
     * its own. */
    set_background(0x00101820, HX_BG_SOLID);
    hxcomp_show_pointer(false);

    struct hxs_window *a = make_window(1, 40, 30, 120, 80, 0x00204060, true);
    struct hxs_window *b = make_window(2, 100, 60, 140, 90, 0x00603020, true);
    a->border_w = 2;
    a->border_color = 0x00FFAA00;
    b->border_w = 1;
    b->border_color = 0x0000FF88;

    struct hxs_window *stack[2] = { a, b };

    check_single_write("a full repaint writes each pixel once", stack, 2, 0, 0,
                       SCREEN_W, SCREEN_H);

    check_single_write("overlapping windows are not painted over each other",
                       stack, 2, 90, 50, 120, 80);

    /* The drag: the window is now at its new place, and the damage is
     * the union of the two, which is what the server repaints. */
    int32_t ox = a->x, oy = a->y;
    for (int step = 0; step < 3; step++) {
        a->x += 8;
        a->y += 5;

        int32_t bx = a->border_w;
        int32_t x0 = (ox < a->x ? ox : a->x) - bx;
        int32_t y0 = (oy < a->y ? oy : a->y) - bx;
        int32_t x1 = (ox > a->x ? ox : a->x) + (int32_t)a->w + bx;
        int32_t y1 = (oy > a->y ? oy : a->y) + (int32_t)a->h + bx;

        check_single_write("a moved window is painted once, not erased first",
                           stack, 2, x0, y0, x1 - x0, y1 - y0);
        ox = a->x;
        oy = a->y;
    }

    /* And it still has to be correct: the same stack, checked pixel
     * for pixel against the model. */
    check_paint("the moved window lands where the model says", stack, 2, 0, 0,
                SCREEN_W, SCREEN_H);

    /* Leave the desktop as the other tests expect to find it. The
     * pointer stays hidden: the cursor tests park it in the last
     * pixel and hide it again when they are done. */
    hxcomp_show_pointer(false);
    set_background(0x00101820, HX_BG_GRID);
    free(a->pix);
    free(a);
    free(b->pix);
    free(b);
}

static void run_composition_tests(void) {
    printf("-- composition --\n");

    /* 1. One window on the desktop. */
    struct hxs_window *a = make_window(2, 10, 10, 120, 80, 0x00204060, true);
    struct hxs_window *stack1[] = { a };
    check_paint("single window, full damage", stack1, 1, 10, 10, 120, 80);
    check_paint("single window, inner damage", stack1, 1, 40, 30, 20, 20);
    check_paint("desktop only", stack1, 1, 200, 120, 60, 40);

    /* 2. Two overlapping windows: b is on top. */
    struct hxs_window *b = make_window(4, 60, 40, 140, 90, 0x00603020, true);
    struct hxs_window *stack2[] = { a, b };
    check_paint("overlap: damage inside the lower window", stack2, 2,
                70, 50, 30, 20);
    check_paint("overlap: damage of the lower window's whole rect", stack2, 2,
                10, 10, 120, 80);
    check_paint("overlap: damage inside the upper window", stack2, 2,
                150, 60, 20, 20);
    check_paint("overlap: damage across both", stack2, 2, 0, 0, 320, 200);

    /* 3. The same stack with the order reversed (a on top). */
    struct hxs_window *stack3[] = { b, a };
    check_paint("raised: damage inside the now-lower window", stack3, 2,
                70, 50, 30, 20);
    check_paint("raised: damage inside the now-upper window", stack3, 2,
                20, 20, 40, 30);

    /* 4. An unmapped window must be invisible. */
    b->mapped = false;
    check_paint("unmapped window is skipped", stack2, 2, 60, 40, 140, 90);
    b->mapped = true;

    /* 5. A window partly off screen (negative coordinates). */
    struct hxs_window *c = make_window(6, -30, -20, 100, 60, 0x00306020, true);
    struct hxs_window *stack4[] = { a, b, c };
    check_paint("off-screen window is clipped", stack4, 3, 0, 0, 90, 60);

    /* 6. A full-screen window covering everything (the shortcut's
     *    best case) with a small window above it. */
    struct hxs_window *big = make_window(8, 0, 0, SCREEN_W, SCREEN_H,
                                         0x00202020, true);
    struct hxs_window *small = make_window(9, 100, 100, 40, 30, 0x00E0A040,
                                           true);
    struct hxs_window *stack5[] = { big, small };
    check_paint("full-screen window below a small one", stack5, 2,
                100, 100, 40, 30);
    check_paint("full-screen window, damage beside the small one", stack5, 2,
                0, 0, 90, 90);
    check_paint("small window above, damage across the edge", stack5, 2,
                90, 90, 60, 60);

    /* 7. Bottom-of-stack window covering the damage (the shortcut's
     *    index 0 special case). */
    struct hxs_window *stack6[] = { big };
    check_paint("bottom window covers the damage", stack6, 1, 10, 10, 50, 50);

    /* 8. Server-drawn borders (v1.1): the ring lives outside the
     *    window and is opaque, so it both paints and occludes. */
    struct hxs_window *bordered = make_window(10, 60, 60, 80, 50,
                                              0x00405060, true);
    bordered->border_w = 3;
    bordered->border_color = 0x004FA3D1;
    struct hxs_window *stack7[] = { bordered };
    check_paint("border: damage over the whole outer rect", stack7, 1,
                50, 50, 100, 70);
    check_paint("border: damage on the left edge only", stack7, 1,
                55, 70, 10, 10);
    check_paint("border: damage inside the window", stack7, 1, 80, 70, 20, 20);
    check_paint("border: damage on the top edge only", stack7, 1,
                70, 57, 20, 4);
    check_paint("border: damage on the bottom right corner", stack7, 1,
                136, 106, 8, 8);

    struct hxs_window *stack8[] = { big, bordered };
    check_paint("border above a full-screen window", stack8, 2,
                50, 50, 100, 70);

    /* 9. A window whose border reaches off screen. */
    struct hxs_window *edge = make_window(11, 1, 1, 40, 30, 0x00507080, true);
    edge->border_w = 4;
    edge->border_color = 0x00FF3060;
    struct hxs_window *stack9[] = { edge };
    check_paint("border clipped by the screen edge", stack9, 1, 0, 0, 60, 50);

    /* 10. Odd geometry: an odd x and an odd width exercise the
     *     unaligned head and tail of every paired store. */
    struct hxs_window *odd = make_window(12, 45, 33, 37, 23, 0x0090A0B0, true);
    struct hxs_window *stack10[] = { odd };
    check_paint("odd offset and width", stack10, 1, 45, 33, 37, 23);
    check_paint("odd damage rectangle on the desktop", stack10, 1,
                201, 111, 39, 25);

    /* 11. A plain (non-grid) desktop takes the one-fill path, which
     *     fills the whole damaged rectangle in one go rather than a
     *     row at a time. */
    set_background(0x00203040, HX_BG_SOLID);
    check_paint("solid desktop, damage beside the window", stack10, 1,
                150, 20, 61, 43);
    check_paint("solid desktop, damage across the window", stack10, 1,
                40, 28, 50, 33);
    set_background(0x00101820, HX_BG_GRID);

    run_cursor_tests(stack2, 2);
}

int main(void) {
    printf("== highX compositor tests ==\n");

    for (int pass = 0; pass < 2; pass++) {
        uint32_t pad = pass == 0 ? 0 : 7;
        printf("== framebuffer pitch = width + %u ==\n", pad);
        stub_fb_init(SCREEN_W, SCREEN_H, pad);
        hxcomp_init();
        set_background(0x00101820, HX_BG_GRID);
        run_composition_tests();
        run_flicker_tests();
    }
    run_drawing_tests();

    printf("== %d checks, %d failed ==\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
