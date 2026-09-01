/*
 * test_fb.c - host tests and benchmark for the framebuffer console
 *
 * kernel/drivers/fb/fb.c is a text grid over a pixel buffer and nothing
 * else: given a framebuffer description it needs no hardware, no
 * interrupts and no scheduler. The file is #included here rather than
 * linked, so the test can see the text buffer behind the pixels and
 * check one against the other.
 *
 * Two things are checked:
 *
 *   1. What is on screen is what the text buffer says. After any
 *      stream of bytes - text, escape sequences, scrolling, the
 *      alternate screen - every pixel is compared against a glyph
 *      drawn the slow, obvious way from g_text and the palette. The
 *      console skips cells it believes are already correct, and a
 *      wrong skip shows up here as a stale glyph.
 *
 *   2. A full repaint agrees. fb_repaint() redraws everything from
 *      the text buffer, so its result must equal what incremental
 *      painting produced.
 *
 * The benchmark at the end drives the console with what a full-screen
 * editor actually sends: kilo redraws all 24 rows on every keystroke.
 *
 * Build and run:  make -C tests/fb run
 */

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Stubs for what fb.c calls but this test does not exercise.
 *
 * fb_set_mode() reprograms a display adapter and remaps physical
 * memory; neither means anything on the host. The declarations come
 * from the real headers (fb.c includes them), so a signature change
 * there breaks the build here rather than being silently ignored -
 * which is the point of stubbing rather than #ifdef-ing the code out.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

bool vbe_available(void) { return false; }
uint64_t vbe_lfb_phys(void) { return 0; }
int vbe_set_mode(uint32_t w, uint32_t h, uint32_t bpp, uint64_t *pitch) {
    (void)w; (void)h; (void)bpp; (void)pitch;
    return -1;
}
int vmm_map_region(uint64_t v, uint64_t p, size_t n, uint64_t f) {
    (void)v; (void)p; (void)n; (void)f;
    return -1;
}

#include "../../kernel/drivers/fb/fb.c"

#define SCREEN_W 640
#define SCREEN_H 384          /* 80x24 cells exactly */

static int g_checks, g_failures;
static uint32_t *g_screen;

static void ok(const char *name) {
    g_checks++;
    printf("  [PASS] %s\n", name);
}

static void fail(const char *name, const char *why) {
    g_checks++;
    g_failures++;
    printf("  [FAIL] %s: %s\n", name, why);
}

static void feed(const char *s) {
    for (const char *p = s; *p != '\0'; p++) {
        fb_putchar(*p);
    }
}

static void feed_n(const char *s, size_t n) {
    for (size_t i = 0; i < n; i++) {
        fb_putchar(s[i]);
    }
}

/* ---- the model ----
 *
 * One cell, drawn the way the console did before it learned to work
 * in runs: a conditional per pixel, straight from the font. */
static void model_cell(uint32_t *out, int row, int col) {
    const struct fb_cell *cell = &g_text[row][col];
    const uint8_t *rows = font_glyph_rows(cell->cp);
    if (rows == NULL) {
        rows = font8x16[' ' - FONT_FIRST];
    }
    uint32_t fg = g_palette_fg[cell->color & (FB_PALETTE_SIZE - 1)];
    uint32_t bg = g_palette_bg[cell->color & (FB_PALETTE_SIZE - 1)];
    if (g_cursor_visible && row == g_cursor_y && col == g_cursor_x) {
        uint32_t t = fg;
        fg = bg;
        bg = t;
    }
    const uint8_t *glyph = rows;
    for (int y = 0; y < FONT_HEIGHT; y++) {
        for (int x = 0; x < FONT_WIDTH; x++) {
            out[(size_t)(g_text_top + row * FONT_HEIGHT + y) * g_pitch_words +
                (size_t)col * FONT_WIDTH + x] =
                (glyph[y] & (0x80 >> x)) ? fg : bg;
        }
    }
}

/* Every cell of the screen must match the model. */
static void check_screen(const char *name) {
    static uint32_t *model;
    if (model == NULL) {
        model = calloc((size_t)g_pitch_words * SCREEN_H, sizeof(uint32_t));
    }
    memcpy(model, g_screen, (size_t)g_pitch_words * SCREEN_H * sizeof(uint32_t));
    for (int row = 0; row < g_rows; row++) {
        for (int col = 0; col < g_cols; col++) {
            model_cell(model, row, col);
        }
    }
    for (int row = 0; row < g_rows; row++) {
        for (int col = 0; col < g_cols; col++) {
            for (int y = 0; y < FONT_HEIGHT; y++) {
                for (int x = 0; x < FONT_WIDTH; x++) {
                    size_t off =
                        (size_t)(g_text_top + row * FONT_HEIGHT + y) *
                            g_pitch_words +
                        (size_t)col * FONT_WIDTH + x;
                    if (g_screen[off] != model[off]) {
                        char why[160];
                        snprintf(why, sizeof(why),
                                 "cell %d,%d ('%c') pixel %d,%d is %06x, "
                                 "expected %06x",
                                 row, col, (char)g_text[row][col].cp, x, y,
                                 g_screen[off], model[off]);
                        fail(name, why);
                        return;
                    }
                }
            }
        }
    }
    ok(name);
}

/* Incremental painting and a from-scratch repaint must agree. */
static void check_repaint_agrees(const char *name) {
    size_t bytes = (size_t)g_pitch_words * SCREEN_H * sizeof(uint32_t);
    uint32_t *before = malloc(bytes);
    memcpy(before, g_screen, bytes);
    fb_repaint();
    int bad = memcmp(before, g_screen, bytes);
    free(before);
    if (bad != 0) {
        fail(name, "a full repaint differs from what incremental painting left");
    } else {
        ok(name);
    }
}

static void reset(void) {
    static struct limine_framebuffer fb;
    if (g_screen == NULL) {
        g_screen = calloc((size_t)(SCREEN_W + 7) * SCREEN_H, sizeof(uint32_t));
    }
    fb.address = g_screen;
    fb.width = SCREEN_W;
    fb.height = SCREEN_H;
    fb.pitch = (uint64_t)(SCREEN_W + 7) * 4; /* padded, like real hardware */
    fb.bpp = 32;
    fb.memory_model = LIMINE_FRAMEBUFFER_RGB;
    g_ansi_state = ANSI_NORMAL;
    g_csi_len = 0;
    g_cursor_visible = true;
    g_reverse = false;
    g_alt_active = false;
    g_graphics = false;
    g_fg = COLOR_FG;
    g_bg = COLOR_BG;
    fb_init(&fb);
}

/* ---- what a full-screen editor sends ---- */

/* kilo's editorRefreshScreen: hide the cursor, home, then every row
 * with an erase-to-end-of-line, a status bar in reverse video, the
 * message line, the cursor position and the cursor back on. */
static size_t kilo_frame(char *out, size_t cap, int cursor_row, int fill) {
    size_t n = 0;
    n += (size_t)snprintf(out + n, cap - n, "\x1b[?25l\x1b[H");
    for (int row = 0; row < 22; row++) {
        for (int col = 0; col < 68; col++) {
            out[n++] = (char)('a' + ((row * 7 + col + fill) % 26));
        }
        n += (size_t)snprintf(out + n, cap - n, "\x1b[39m\x1b[0K\r\n");
    }
    n += (size_t)snprintf(out + n, cap - n,
                          "\x1b[0K\x1b[7m  test.c - 22 lines            "
                          "                    %d/22 \x1b[0m\r\n",
                          cursor_row);
    n += (size_t)snprintf(out + n, cap - n, "\x1b[0KHELP: Ctrl-S = save");
    n += (size_t)snprintf(out + n, cap - n, "\x1b[%d;%dH\x1b[?25h",
                          cursor_row + 1, 12);
    return n;
}

static long long now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

static void benchmark(void) {
    static char frame[16384];
    printf("-- benchmark --\n");
    reset();

    /* Typing: every keystroke redraws all 24 rows, but only the line
     * being edited and the status bar actually change. */
    size_t len = kilo_frame(frame, sizeof(frame), 3, 0);
    feed_n(frame, len); /* first frame: everything is new */

    long long t0 = now_us();
    for (int i = 0; i < 200; i++) {
        len = kilo_frame(frame, sizeof(frame), 3 + (i % 2), 0);
        feed_n(frame, len);
    }
    long long us = now_us() - t0;
    printf("  editor frame, one line changing   %6lld us/frame (%zu bytes)\n",
           us / 200, len);

    /* The worst case: every cell different, nothing can be skipped. */
    t0 = now_us();
    for (int i = 0; i < 200; i++) {
        len = kilo_frame(frame, sizeof(frame), 3, i);
        feed_n(frame, len);
    }
    us = now_us() - t0;
    printf("  editor frame, whole screen new    %6lld us/frame\n", us / 200);

    /* Plain scrolling output, the `ls -l` / build-log case. */
    t0 = now_us();
    for (int i = 0; i < 2000; i++) {
        feed("drwxr-xr-x  2 root root  4096 Jan  1 00:00 a-directory-name\n");
    }
    us = now_us() - t0;
    printf("  scrolling text                    %6lld us/line\n", us / 2000);
}

int main(void) {
    printf("== framebuffer console tests ==\n");
    printf("-- text --\n");

    reset();
    feed("hello, world\n");
    check_screen("plain text");

    feed("second line\nthird\n");
    check_screen("several lines");
    check_repaint_agrees("repaint agrees after plain text");

    feed("tab\there\n");
    check_screen("tabs");

    feed("backspace: xy\b\b!\n");
    check_screen("backspace");

    feed("\roverwritten\n");
    check_screen("carriage return overwrites");

    printf("-- escape sequences --\n");
    feed("\x1b[10;20Hplaced");
    check_screen("cursor positioning (CUP)");

    feed("\x1b[2Jcleared");
    check_screen("erase display");

    feed("\x1b[5;1Hxxxxxxxxxx\x1b[5;4H\x1b[0K");
    check_screen("erase to end of line");

    feed("\x1b[6;1Hyyyyyyyyyy\x1b[6;5H\x1b[1K");
    check_screen("erase to start of line");

    feed("\x1b[8;1H\x1b[31mred\x1b[32mgreen\x1b[0mplain");
    check_screen("SGR colours");

    feed("\x1b[9;1H\x1b[7mreverse\x1b[27mnormal");
    check_screen("reverse video");

    feed("\x1b[12;30H");
    check_screen("the cursor block moved");
    check_repaint_agrees("repaint agrees after escapes");

    feed("\x1b[?25l");
    check_screen("cursor hidden");
    feed("\x1b[?25h");
    check_screen("cursor shown again");

    feed("\x1b[3;5Hkeep\x1b[?1049h");
    check_screen("alternate screen is blank");
    feed("alt screen text\x1b[?1049l");
    check_screen("leaving the alternate screen restores the main one");
    check_repaint_agrees("repaint agrees after the alternate screen");

    printf("-- scrolling --\n");
    reset();
    for (int i = 0; i < 40; i++) {
        char line[64];
        snprintf(line, sizeof(line), "line %d with some text on it\n", i);
        feed(line);
    }
    check_screen("scrolled past the bottom");
    check_repaint_agrees("repaint agrees after scrolling");

    feed("\x1b[1;1Hafter a scroll");
    check_screen("drawing over a scrolled screen");

    printf("-- wrapping and edges --\n");
    reset();
    for (int i = 0; i < 200; i++) {
        fb_putchar((char)('A' + i % 26));
    }
    check_screen("long line wraps");

    feed("\x1b[24;80Hz");
    check_screen("the bottom right cell");
    check_repaint_agrees("repaint agrees at the edges");

    /* Deferred wrap: writing the last column parks the cursor on it
     * and owes a line feed, which the next printable character - and
     * nothing else - collects. A terminal that advances immediately
     * turns "a full-width line, then \r\n" into two line feeds, which
     * is what made a full-screen editor scroll the whole screen on
     * every redraw. */
    reset();
    for (int i = 0; i < g_cols; i++) {
        fb_putchar('#');
    }
    if (g_cursor_y != 0 || g_cursor_x != g_cols - 1 || !fb_wrap_pending()) {
        fail("a full-width line does not advance on its own", "cursor moved");
    } else {
        ok("a full-width line does not advance on its own");
    }
    check_screen("the full-width line is on screen");

    feed("\r\n");
    if (g_cursor_y == 1 && g_cursor_x == 0 && !fb_wrap_pending()) {
        ok("CR LF after a full-width line advances exactly one row");
    } else {
        char why[80];
        snprintf(why, sizeof(why), "cursor at %d,%d (expected 1,0)",
                 g_cursor_y, g_cursor_x);
        fail("CR LF after a full-width line advances exactly one row", why);
    }

    reset();
    for (int i = 0; i < g_cols; i++) {
        fb_putchar('#');
    }
    fb_putchar('X');
    if (g_cursor_y == 1 && g_cursor_x == 1) {
        ok("the next character collects the owed wrap");
    } else {
        fail("the next character collects the owed wrap", "wrong position");
    }
    check_screen("the wrapped character landed on the next row");

    reset();
    for (int i = 0; i < g_cols; i++) {
        fb_putchar('#');
    }
    feed("\x1b[1;1H");
    if (!fb_wrap_pending() && g_cursor_x == 0 && g_cursor_y == 0) {
        ok("a cursor sequence cancels the owed wrap");
    } else {
        fail("a cursor sequence cancels the owed wrap", "flag survived");
    }

    /* A full-width line at the bottom of the screen must not scroll
     * until something is actually written past it. */
    reset();
    for (int row = 0; row < g_rows - 1; row++) {
        feed("x\n");
    }
    for (int i = 0; i < g_cols; i++) {
        fb_putchar('#');
    }
    if (g_cursor_y == g_rows - 1 && g_text[g_rows - 1][0].cp == '#') {
        ok("a full-width bottom line does not scroll the screen");
    } else {
        fail("a full-width bottom line does not scroll the screen",
             "the screen scrolled");
    }
    check_screen("the bottom line is intact");

    /* A tab that runs into the last column stops there. */
    reset();
    feed("\x1b[1;1H");
    for (int i = 0; i < g_cols - 2; i++) {
        fb_putchar('.');
    }
    fb_putchar('\t');
    if (g_cursor_y == 0) {
        ok("a tab at the right margin does not run away");
    } else {
        fail("a tab at the right margin does not run away", "it wrapped");
    }

    printf("-- an editor's frames --\n");
    reset();
    static char frame[16384];
    size_t len = kilo_frame(frame, sizeof(frame), 3, 0);
    feed_n(frame, len);
    check_screen("first editor frame");
    for (int i = 0; i < 5; i++) {
        len = kilo_frame(frame, sizeof(frame), 3 + i, i);
        feed_n(frame, len);
        check_screen("editor frame redrawn");
    }
    check_repaint_agrees("repaint agrees after editor frames");

    printf("-- direct pixel writers --\n");
    fb_fill(0x00FFFFFF);
    fb_repaint();
    check_screen("the console recovers the screen after fb_fill");

    fb_invalidate(); /* what /dev/fb0 writes must announce */
    feed("\x1b[2J\x1b[1;1Hback");
    check_screen("a stale screen is repainted after fb_invalidate");

    printf("-- UTF-8 --\n");
    /* The console reads its byte stream as UTF-8, so a Turkish word
     * has to land one letter per cell - not one BYTE per cell, which
     * is what makes a column count downstream (the cursor, the
     * scrollback, TIOCGWINSZ) disagree with the screen. */
    feed("\x1b[2J\x1b[1;1H");
    feed("g\xc3\xbcnd\xc3\xbcz");             /* gunduz with two u-umlauts */
    if (g_cursor_x == 6 &&
        g_text[0][0].cp == 'g' && g_text[0][1].cp == 0x00FC &&
        g_text[0][2].cp == 'n' && g_text[0][3].cp == 'd' &&
        g_text[0][4].cp == 0x00FC && g_text[0][5].cp == 'z') {
        ok("a two-byte letter takes one cell (gunduz)");
    } else {
        char why[128];
        snprintf(why, sizeof(why), "cursor at %d, cells %04x %04x %04x",
                 g_cursor_x, g_text[0][1].cp, g_text[0][2].cp,
                 g_text[0][4].cp);
        fail("a two-byte letter takes one cell (gunduz)", why);
    }
    check_screen("the accented letters are painted");

    /* Every letter Turkish needs, including the two the language is
     * usually broken by: dotted capital I and dotless lowercase i. */
    feed("\x1b[2J\x1b[1;1H");
    feed("\xc4\x9f\xc4\xb0\xc4\xb1\xc5\x9f\xc3\xa7\xc3\xb6"); /* g I i s c o */
    if (g_text[0][0].cp == 0x011F && g_text[0][1].cp == 0x0130 &&
        g_text[0][2].cp == 0x0131 && g_text[0][3].cp == 0x015F &&
        g_text[0][4].cp == 0x00E7 && g_text[0][5].cp == 0x00F6) {
        ok("the Turkish letters decode to the right codepoints");
    } else {
        fail("the Turkish letters decode to the right codepoints", "wrong");
    }
    check_screen("the Turkish letters are painted");

    /* Malformed input must cost one character, not the next two. */
    feed("\x1b[2J\x1b[1;1H");
    /* Split literal on purpose: 'b' is a hex digit, so "a\xbfb" is
     * ONE greedy escape (0xbfb) rather than three bytes. */
    feed("a\xbf" "b");                          /* stray continuation byte */
    if (g_text[0][0].cp == 'a' && g_text[0][1].cp == 'b' &&
        g_cursor_x == 2) {
        ok("a stray continuation byte is dropped, not drawn");
    } else {
        fail("a stray continuation byte is dropped, not drawn", "wrong");
    }

    feed("\x1b[2J\x1b[1;1H");
    feed("\xc3");                              /* lead byte, then ASCII */
    feed("x");
    if (g_text[0][0].cp == 'x' && g_cursor_x == 1) {
        ok("an ASCII byte abandons a half-built sequence");
    } else {
        fail("an ASCII byte abandons a half-built sequence", "wrong");
    }

    feed("\x1b[2J\x1b[1;1H");
    feed("\xc0\xaf");            /* overlong '/' - must not become '/' */
    if (g_cursor_x == 0) {
        ok("an overlong encoding is rejected");
    } else {
        fail("an overlong encoding is rejected", "it drew something");
    }

    feed("\x1b[2J\x1b[1;1H");
    feed("\xed\xa0\x80");                      /* a UTF-16 surrogate */
    if (g_cursor_x == 0) {
        ok("a surrogate codepoint is rejected");
    } else {
        fail("a surrogate codepoint is rejected", "it drew something");
    }

    check_repaint_agrees("repaint agrees after UTF-8 text");

    benchmark();

    printf("== %d checks, %d failed ==\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
