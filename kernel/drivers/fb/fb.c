/*
 * fb.c - framebuffer text console implementation
 *
 * A full-screen text buffer (g_text) mirrors what is on screen. Each
 * character cell is redrawn on demand, which makes cursor movement and
 * editing cheap; scrolling shifts both the text buffer and the pixel
 * rows. The cursor is drawn as an inverted block.
 *
 * A second grid, g_shown, records what was last *painted*. Terminal
 * applications do not send what changed, they send the screen: kilo
 * redraws all twenty-four rows on every keystroke, and all but one of
 * them are already on screen. Comparing against g_shown turns those
 * back into two-byte compares, so a repaint costs what actually
 * changed. Anything that writes pixels behind the console's back -
 * fb_fill, the boot splash, /dev/fb0, a highX session - has to say so
 * with fb_invalidate().
 *
 * Only 32-bit RGB framebuffers are supported (the Limine default);
 * a pitch of width*4 bytes is expected but any pitch works.
 */

#include "drivers/fb/fb.h"

#include <stdbool.h>

#include "drivers/fb/font8x16.h"
#include "drivers/fb/font_latin.h"
#include "drivers/vbe/vbe.h"
#include "core/errno.h"
#include "core/klib.h"
#include "mm/vmm.h"
#include "sched/sched.h"

/* Grid size caps; the real grid is clamped to these. */
#define FB_MAX_COLS 512
#define FB_MAX_ROWS 256

/* Scrollback history: completed lines are kept here so the user can
 * page back with PageUp/PageDown. Ring buffer: g_hist_head is the
 * next slot to write, g_hist_count the number of lines stored. */
#define FB_HISTORY_ROWS 2048

/* One screen cell: a Unicode codepoint + palette index for its color
 * pair. Colors are stored as indices into a small palette (see
 * fb_set_color) so a scrollback redraw reproduces the exact colors
 * the text had when it was written.
 *
 * The cell holds a CODEPOINT, not a byte. A console that stored bytes
 * could not put a Turkish 'ş' in one cell: it is two bytes of UTF-8,
 * and storing them in two cells would make one letter take two
 * columns and make every column count downstream - the cursor, the
 * scrollback, TIOCGWINSZ - disagree with what is on screen.
 *
 * uint16_t, not uint32_t: the Basic Multilingual Plane covers every
 * script this font has a glyph for, and the grid plus its 2048-line
 * scrollback is the largest array in the kernel. */
struct fb_cell {
    uint16_t cp;
    uint8_t color;
};

/* Palette of color pairs; index 0 is the default (near-white on
 * black). fb_set_color looks up or appends the requested pair. */
#define FB_PALETTE_SIZE 16

/* Default palette: near-white on black, warm toast accent for prompts. */
#define COLOR_FG 0x00E8E8E8
#define COLOR_BG 0x00000000

static struct limine_framebuffer *g_fb;
static uint32_t *g_pixels;
static uint64_t g_pitch_bytes;
static uint64_t g_pitch_words; /* pitch divided by 4 */
static uint32_t g_width;
static uint32_t g_height;

/* Bits per pixel. Tracked here rather than read back out of the
 * Limine structure: after a runtime mode change that structure
 * describes the mode the machine booted in, not the one it is in. */
static uint32_t g_bpp;

/* Pixel row where the text grid starts. The boot splash draws logos
 * in the band above this offset; text rendering, scrolling and the
 * scrollback redraw all work below it. fb_clear() resets it to 0. */
static uint32_t g_text_top;

/* Graphics mode: while the highX display server owns the screen the
 * text console keeps updating its character buffer (and the serial
 * mirror), but paints nothing - otherwise a kernel message would
 * punch holes through the windows. fb_repaint() puts the text back
 * when the session ends. */
static bool g_graphics;

static int g_cols;
static int g_rows;
static int g_cursor_x;
static int g_cursor_y;

/* Deferred wrap, the rule every real terminal follows: writing the
 * last column of a row does NOT move to the next line. The cursor
 * stays on that column with this flag set, and the line advances only
 * when another printable character arrives. Anything that moves or
 * erases - a newline, a carriage return, a cursor sequence - cancels
 * it.
 *
 * It matters because full-screen applications lay out a status bar
 * exactly as wide as the screen and then send \r\n. Advancing on the
 * last column turns that into two line feeds, which walks the cursor
 * off the bottom of the screen and scrolls it - a full-screen scroll
 * per redraw, which on a 1280x800 framebuffer is four megabytes of
 * memmove for a frame that was about to be overwritten anyway. */
static bool g_wrap_pending;

static uint32_t g_fg = COLOR_FG;
static uint32_t g_bg = COLOR_BG;
static uint8_t g_cur_color; /* palette index of the current colors */

static uint32_t g_palette_fg[FB_PALETTE_SIZE];
static uint32_t g_palette_bg[FB_PALETTE_SIZE];
static int g_palette_count;

static struct fb_cell g_text[FB_MAX_ROWS][FB_MAX_COLS];

/* What each cell was last painted with. A cell whose text and colour
 * still match what is here is already on screen and is left alone. */
static struct fb_cell g_shown[FB_MAX_ROWS][FB_MAX_COLS];

/* The cell currently painted in reverse video, or -1,-1. The cursor
 * is an overlay on top of a cell rather than a property of it, so it
 * is erased and drawn explicitly instead of being decided again on
 * every paint. */
static int g_cur_row = -1;
static int g_cur_col = -1;

/* Forward declarations: the helpers live below, the ANSI engine
 * (inserted right after the globals) calls them. */
static uint8_t palette_lookup(uint32_t fg, uint32_t bg);
static void fb_paint_cell(int row, int col, const struct fb_cell *cell,
                          bool show_cursor);
static void fb_draw_cell(int row, int col, bool show_cursor);
static void fb_recompute_grid(void);
static void fb_redraw_live(void);
static void cursor_erase(void);
static void cursor_draw(void);

/* ---- ANSI/VT100 escape sequence state ----
 *
 * The console understands the subset of ECMA-48 / VT100 sequences a
 * full-screen terminal application (kilo) needs: CUP cursor
 * positioning (H/f), relative moves (A/B/C/D), erase display/line
 * (J/K/X), SGR colours (m, incl. reverse video 7 and the 30-37 /
 * 90-97 / 40-47 / 100-107 colour sets), cursor visibility (?25h/l)
 * and the alternate screen (?1049h/l, ?47h/l). Unknown sequences are
 * consumed and ignored so a misbehaving application can never wedge
 * the parser.
 */

enum {
    ANSI_NORMAL = 0,
    ANSI_ESC,   /* got ESC, waiting for '[' or 'O' */
    ANSI_CSI,   /* inside a control sequence, accumulating parameters */
    ANSI_SS3,   /* ESC O ... single-shift (e.g. ESC O H = Home) */
};

static int g_ansi_state = ANSI_NORMAL;
static char g_csi_buf[32];
static int g_csi_len;
static bool g_csi_private;   /* '?' prefix: private (DEC) mode */
static bool g_cursor_visible = true;
static bool g_reverse;       /* SGR 7: swap fg/bg at write time */

/* Alternate screen storage (ESC[?1049h / ESC[?47h). */
static struct fb_cell g_alt_text[FB_MAX_ROWS][FB_MAX_COLS];
static int g_alt_cursor_x, g_alt_cursor_y;
static uint32_t g_alt_fg, g_alt_bg;
static uint8_t g_alt_cur_color;
static bool g_alt_active;

/* The 16 standard ANSI colours (VGA palette, bright variants in the
 * second half). SGR 30-37 / 90-97 select the foreground, 40-47 /
 * 100-107 the background. */
static const uint32_t ansi_colors[16] = {
    0x000000, 0xAA0000, 0x00AA00, 0xAA5500,
    0x0000AA, 0xAA00AA, 0x00AAAA, 0xAAAAAA,
    0x555555, 0xFF5555, 0x55FF55, 0xFFFF55,
    0x5555FF, 0xFF55FF, 0x55FFFF, 0xFFFFFF,
};

/* Recompute the palette index for the current (fg,bg,reverse) state.
 * Reverse video stores the swapped pair so later redraws (scroll,
 * cursor blink) reproduce the inversion without per-cell flags. */
static void fb_update_color(void) {
    if (g_reverse) {
        g_cur_color = palette_lookup(g_bg, g_fg);
    } else {
        g_cur_color = palette_lookup(g_fg, g_bg);
    }
}

/* Forget what is on screen: the next paint of every cell will happen
 * for real. Anything that puts pixels on the framebuffer without
 * going through the text grid has to call this, or the console will
 * skip repainting cells it believes it already drew. */
void fb_invalidate(void) {
    for (int row = 0; row < g_rows; row++) {
        /* 0xFF is not a palette index the console ever assigns, so no
         * cell can match this by accident. */
        memset(g_shown[row], 0xFF, (size_t)g_cols * sizeof(struct fb_cell));
    }
    g_cur_row = -1;
    g_cur_col = -1;
}

/* Take the cursor block off the cell that is wearing it. */
static void cursor_erase(void) {
    if (g_cur_row < 0) {
        return;
    }
    /* The marker is left standing on the way in: fb_paint_cell needs
     * it to see that this cell is currently wearing the block, which
     * is the difference between repainting it and deciding it already
     * shows the right character. Clearing it is that function's job
     * once the cell is painted plain. */
    fb_paint_cell(g_cur_row, g_cur_col, &g_text[g_cur_row][g_cur_col], false);
}

/* Put it on the cell the cursor is on now, if it is visible at all.
 * Erasing first is what keeps a single block on screen: every path
 * that moves the cursor goes through here. */
static void cursor_draw(void) {
    if (g_graphics || !g_cursor_visible) {
        return;
    }
    if (g_cur_row == g_cursor_y && g_cur_col == g_cursor_x) {
        return; /* already there */
    }
    cursor_erase();
    fb_draw_cell(g_cursor_y, g_cursor_x, true);
}

/* Parse the accumulated CSI parameters into a small integer array.
 * Missing/empty parameters default to `def` (the first one only -
 * real terminals apply the default to every empty field, but no TUS
 * application relies on that). */
static void ansi_parse_params(int *params, int max, int def) {
    int i = 0, n = 0;
    while (i < g_csi_len && n < max) {
        int v = 0;
        bool any = false;
        while (i < g_csi_len && g_csi_buf[i] >= '0' && g_csi_buf[i] <= '9') {
            v = v * 10 + (g_csi_buf[i] - '0');
            if (v > 9999) {
                v = 9999;
            }
            any = true;
            i++;
        }
        params[n++] = any ? v : def;
        if (i < g_csi_len && g_csi_buf[i] == ';') {
            i++;
        } else {
            break;
        }
    }
    while (n < max) {
        params[n++] = def;
    }
}

/* Blank [row][col0..col1) with the current colour pair. */
static void ansi_erase_cells(int row, int col0, int col1) {
    if (row < 0 || row >= g_rows) {
        return;
    }
    struct fb_cell blank = { ' ', g_cur_color };
    for (int c = col0; c < col1 && c < g_cols; c++) {
        if (c < 0) {
            continue;
        }
        g_text[row][c] = blank;
        fb_paint_cell(row, c, &blank, false);
    }
}

static void ansi_alt_enter(void) {
    if (g_alt_active) {
        return;
    }
    memcpy(g_alt_text, g_text, sizeof(g_alt_text));
    g_alt_cursor_x = g_cursor_x;
    g_alt_cursor_y = g_cursor_y;
    g_alt_fg = g_fg;
    g_alt_bg = g_bg;
    g_alt_cur_color = g_cur_color;
    g_alt_active = true;
    fb_clear();
}

static void ansi_alt_exit(void) {
    if (!g_alt_active) {
        return;
    }
    g_alt_active = false;
    /* Wipe the alternate screen, then put the saved one back. */
    struct fb_cell blank = { ' ', g_cur_color };
    for (int r = 0; r < g_rows; r++) {
        for (int c = 0; c < g_cols; c++) {
            g_text[r][c] = blank;
        }
    }
    memset(g_pixels, 0, (size_t)(g_pitch_bytes * g_height));
    fb_invalidate();
    memcpy(g_text, g_alt_text, sizeof(g_text));
    g_cursor_x = g_alt_cursor_x;
    g_cursor_y = g_alt_cursor_y;
    g_wrap_pending = false;
    g_fg = g_alt_fg;
    g_bg = g_alt_bg;
    g_cur_color = g_alt_cur_color;
    fb_redraw_live();
}

/* Run one finished control sequence. `final` is the final byte in
 * 0x40..0x7E; the parameters live in g_csi_buf. */
static void ansi_exec(char final) {
    int params[8];
    ansi_parse_params(params, 8, 1);

    /* Private (DEC) modes: cursor visibility, alternate screen. */
    if (g_csi_private) {
        int m = params[0];
        if (final == 'h') {
            if (m == 25) {
                /* A full-screen application hides the cursor for the
                 * duration of a redraw and shows it again at the end,
                 * having moved it where it wants it. Showing it has
                 * to paint the block: nothing else will, because the
                 * application is now waiting for a key. */
                g_cursor_visible = true;
                cursor_draw();
            } else if (m == 47 || m == 1049) {
                ansi_alt_enter();
            } else if (m == 1048) {
                g_alt_cursor_x = g_cursor_x;
                g_alt_cursor_y = g_cursor_y;
            }
        } else if (final == 'l') {
            if (m == 25) {
                g_cursor_visible = false;
                cursor_erase();
            } else if (m == 47 || m == 1049) {
                ansi_alt_exit();
            } else if (m == 1048) {
                g_cursor_x = g_alt_cursor_x;
                g_cursor_y = g_alt_cursor_y;
            }
        }
        return;
    }

    /* Take the cursor block off before the cell under it moves. */
    cursor_erase();

    switch (final) {
    case 'H': /* CUP: row;col (1-based), defaults 1;1 */
    case 'f':
        g_cursor_y = params[0] - 1;
        g_cursor_x = params[1] - 1;
        if (g_cursor_y < 0) {
            g_cursor_y = 0;
        }
        if (g_cursor_y >= g_rows) {
            g_cursor_y = g_rows - 1;
        }
        if (g_cursor_x < 0) {
            g_cursor_x = 0;
        }
        if (g_cursor_x >= g_cols) {
            g_cursor_x = g_cols - 1;
        }
        break;
    case 'A': /* cursor up */
        g_cursor_y -= params[0];
        if (g_cursor_y < 0) {
            g_cursor_y = 0;
        }
        break;
    case 'B': /* cursor down */
        g_cursor_y += params[0];
        if (g_cursor_y >= g_rows) {
            g_cursor_y = g_rows - 1;
        }
        break;
    case 'C': /* cursor right */
        g_cursor_x += params[0];
        if (g_cursor_x >= g_cols) {
            g_cursor_x = g_cols - 1;
        }
        break;
    case 'D': /* cursor left */
        g_cursor_x -= params[0];
        if (g_cursor_x < 0) {
            g_cursor_x = 0;
        }
        break;
    case 'G': /* column absolute */
    case '`':
        g_cursor_x = params[0] - 1;
        if (g_cursor_x < 0) {
            g_cursor_x = 0;
        }
        if (g_cursor_x >= g_cols) {
            g_cursor_x = g_cols - 1;
        }
        break;
    case 'd': /* row absolute */
        g_cursor_y = params[0] - 1;
        if (g_cursor_y < 0) {
            g_cursor_y = 0;
        }
        if (g_cursor_y >= g_rows) {
            g_cursor_y = g_rows - 1;
        }
        break;
    case 'J': /* erase display */
        if (params[0] == 2 || params[0] == 3) {
            for (int r = 0; r < g_rows; r++) {
                ansi_erase_cells(r, 0, g_cols);
            }
        } else if (params[0] == 1) {
            for (int r = 0; r <= g_cursor_y; r++) {
                int last = (r == g_cursor_y) ? g_cursor_x + 1 : g_cols;
                ansi_erase_cells(r, 0, last);
            }
        } else { /* 0 or empty: cursor to end of screen */
            ansi_erase_cells(g_cursor_y, g_cursor_x, g_cols);
            for (int r = g_cursor_y + 1; r < g_rows; r++) {
                ansi_erase_cells(r, 0, g_cols);
            }
        }
        break;
    case 'K': /* erase line */
        if (params[0] == 2) {
            ansi_erase_cells(g_cursor_y, 0, g_cols);
        } else if (params[0] == 1) {
            ansi_erase_cells(g_cursor_y, 0, g_cursor_x + 1);
        } else { /* 0 or empty: cursor to end of line */
            ansi_erase_cells(g_cursor_y, g_cursor_x, g_cols);
        }
        break;
    case 'X': /* erase n characters */
        ansi_erase_cells(g_cursor_y, g_cursor_x, g_cursor_x + params[0]);
        break;
    case 'm': /* SGR */
        for (int i = 0; i < 8; i++) {
            int p = params[i];
            if (p == 0) { /* reset */
                g_reverse = false;
                g_fg = COLOR_FG;
                g_bg = COLOR_BG;
            } else if (p == 7) {
                g_reverse = true;
            } else if (p == 27) {
                g_reverse = false;
            } else if (p == 1 || p == 4) {
                /* bold/underline: no font support, ignore */
            } else if (p >= 30 && p <= 37) {
                g_fg = ansi_colors[p - 30];
            } else if (p >= 40 && p <= 47) {
                g_bg = ansi_colors[p - 40];
            } else if (p >= 90 && p <= 97) {
                g_fg = ansi_colors[p - 90 + 8];
            } else if (p >= 100 && p <= 107) {
                g_bg = ansi_colors[p - 100 + 8];
            } else if (p == 39) {
                g_fg = COLOR_FG;
            } else if (p == 49) {
                g_bg = COLOR_BG;
            }
        }
        fb_update_color();
        break;
    default: /* unknown sequence: consume and ignore */
        break;
    }

    /* Moving or erasing cancels a pending wrap; changing colours does
     * not, which is what a terminal does and what lets an application
     * colour the last column of a row. */
    if (final != 'm') {
        g_wrap_pending = false;
    }
    cursor_draw();
}

/* Feed one byte into the escape state machine. Returns true if the
 * byte was consumed as part of a sequence (nothing was drawn). */
static bool ansi_consume(char c) {
    switch (g_ansi_state) {
    case ANSI_NORMAL:
        if (c == 0x1B) {
            g_ansi_state = ANSI_ESC;
            return true;
        }
        return false;
    case ANSI_ESC:
        if (c == '[') {
            g_ansi_state = ANSI_CSI;
            g_csi_len = 0;
            g_csi_private = false;
        } else if (c == 'O') {
            g_ansi_state = ANSI_SS3;
        } else {
            g_ansi_state = ANSI_NORMAL;
        }
        return true;
    case ANSI_SS3:
        /* ESC O H / ESC O F: Home/End from the application cursor
         * keys. Treated as a no-op (kilo only reads these, never
         * writes them). */
        g_ansi_state = ANSI_NORMAL;
        return true;
    case ANSI_CSI:
        if (c >= 0x40 && c <= 0x7E) { /* final byte */
            g_ansi_state = ANSI_NORMAL;
            ansi_exec(c);
            return true;
        }
        if (g_csi_len == 0 && c == '?') {
            g_csi_private = true;
            return true;
        }
        if ((c >= '0' && c <= '9') || c == ';') {
            if (g_csi_len < (int)sizeof(g_csi_buf) - 1) {
                g_csi_buf[g_csi_len++] = c;
            }
            return true;
        }
        /* Intermediate or unexpected byte: give up on this sequence. */
        g_ansi_state = ANSI_NORMAL;
        return true;
    default:
        g_ansi_state = ANSI_NORMAL;
        return true;
    }
}

/* Scrollback history ring. */
static struct fb_cell g_history[FB_HISTORY_ROWS][FB_MAX_COLS];
static int g_hist_head;   /* next slot to write */
static int g_hist_count;  /* number of stored lines (<= FB_HISTORY_ROWS) */
static int g_view_back;   /* lines we have scrolled back (0 = live) */
static bool g_batch_active;
static bool g_batch_dirty;

/* Look up (fg,bg) in the palette, adding it if room remains.
 * Returns the palette index; falls back to index 0 on overflow. */
static uint8_t palette_lookup(uint32_t fg, uint32_t bg) {
    for (int i = 0; i < g_palette_count; i++) {
        if (g_palette_fg[i] == fg && g_palette_bg[i] == bg) {
            return (uint8_t)i;
        }
    }
    if (g_palette_count < FB_PALETTE_SIZE) {
        int i = g_palette_count++;
        g_palette_fg[i] = fg;
        g_palette_bg[i] = bg;
        return (uint8_t)i;
    }
    return 0;
}

/* Two pixels are eight bytes, and eight bytes are one store. The
 * kernel is built with -mgeneral-regs-only, so there is no vectoriser
 * to do this: a plain `uint32_t` loop is one store per pixel however
 * high the optimisation level. may_alias keeps the aliasing rules
 * happy about looking at a pixel array through a 64-bit type. */
typedef uint64_t fb_u64_alias __attribute__((may_alias));

/* One row of a glyph: eight pixels. The two cases that dominate real
 * text - a row that is all background (every blank cell, and most
 * rows of most glyphs) and one that is all foreground - skip the bit
 * test entirely. */
static inline void fb_glyph_row(uint32_t *p, uint8_t bits, uint32_t fg,
                                uint32_t bg) {
    if (((uintptr_t)p & 7u) != 0) {
        /* Odd scanline start (a pitch that is not a multiple of 8):
         * pairing would be misaligned, so store one at a time. */
        for (int x = 0; x < FONT_WIDTH; x++) {
            p[x] = (bits & (0x80 >> x)) ? fg : bg;
        }
        return;
    }
    fb_u64_alias *q = (fb_u64_alias *)p;
    if (bits == 0x00 || bits == 0xFF) {
        uint32_t c = bits == 0x00 ? bg : fg;
        uint64_t both = ((uint64_t)c << 32) | c;
        q[0] = both;
        q[1] = both;
        q[2] = both;
        q[3] = both;
        return;
    }
    for (int i = 0; i < 4; i++) {
        uint32_t lo = (bits & (0x80 >> (i * 2))) ? fg : bg;
        uint32_t hi = (bits & (0x40 >> (i * 2))) ? fg : bg;
        q[i] = ((uint64_t)hi << 32) | lo;
    }
}

/* Paint one cell from an explicit character and color index.
 * Used both by the live path and by scrollback redraws. */
static void fb_paint_cell(int row, int col, const struct fb_cell *cell,
                          bool show_cursor) {
    if (g_graphics) {
        return; /* highX owns the pixels; only the text buffer moves */
    }
    if (row < 0 || row >= g_rows || col < 0 || col >= g_cols) {
        return;
    }

    bool inv = show_cursor && row == g_cursor_y && col == g_cursor_x;
    bool was_inv = row == g_cur_row && col == g_cur_col;

    /* Already on screen. This is what makes a full-screen redraw cost
     * what changed rather than what was sent. */
    if (inv == was_inv && g_shown[row][col].cp == cell->cp &&
        g_shown[row][col].color == cell->color) {
        return;
    }

    uint32_t fg = g_palette_fg[cell->color & (FB_PALETTE_SIZE - 1)];
    uint32_t bg = g_palette_bg[cell->color & (FB_PALETTE_SIZE - 1)];
    if (inv) {
        uint32_t tmp = fg;
        fg = bg;
        bg = tmp;
    }

    /* A codepoint with no glyph is drawn as a space rather than as a
     * box: the console shows kernel messages, and a run of boxes is
     * harder to read past than a run of gaps. */
    const uint8_t *glyph = font_glyph_rows(cell->cp);
    if (glyph == NULL) {
        glyph = font8x16[' ' - FONT_FIRST];
    }
    uint32_t *pixel = g_pixels
                    + (uint64_t)(g_text_top + (uint32_t)row * FONT_HEIGHT) * g_pitch_words
                    + (uint64_t)col * FONT_WIDTH;

    for (int y = 0; y < FONT_HEIGHT; y++) {
        fb_glyph_row(pixel, glyph[y], fg, bg);
        pixel += g_pitch_words;
    }

    g_shown[row][col] = *cell;
    if (inv) {
        /* Moving the block without erasing it first would leave a
         * cell that looks inverted but is recorded as plain, and it
         * would then be skipped for ever. Every path erases first;
         * this makes the one that forgets repaint instead of rot. */
        if (!was_inv && g_cur_row >= 0) {
            g_shown[g_cur_row][g_cur_col].color = 0xFF;
        }
        g_cur_row = row;
        g_cur_col = col;
    } else if (was_inv) {
        g_cur_row = -1;
        g_cur_col = -1;
    }
}

/* Redraw one cell from the live text buffer. With show_cursor set,
 * the cursor cell is inverted. */
static void fb_draw_cell(int row, int col, bool show_cursor) {
    fb_paint_cell(row, col, &g_text[row][col], show_cursor);
}

/* Redraw the whole screen from the live text buffer (used when
 * returning from scrollback view). */
static void fb_redraw_live(void) {
    for (int row = 0; row < g_rows; row++) {
        for (int col = 0; col < g_cols; col++) {
            fb_draw_cell(row, col, false);
        }
    }
    cursor_draw();
}

void fb_batch_begin(void) {
    g_batch_active = true;
    g_batch_dirty = false;
}

void fb_batch_end(void) {
    g_batch_active = false;
    if (g_batch_dirty) {
        fb_redraw_live();
        g_batch_dirty = false;
    }
}

/* Redraw the whole screen from the scrollback history. g_view_back
 * lines above the live bottom edge are shown; row 0 of the screen
 * maps to history line (g_hist_count - g_view_back - g_rows). */
static void fb_redraw_history(void) {
    static const struct fb_cell blank = { ' ', 0 };
    for (int row = 0; row < g_rows; row++) {
        int hist = g_hist_count - g_view_back - g_rows + row;
        if (hist < 0) {
            for (int col = 0; col < g_cols; col++) {
                fb_paint_cell(row, col, &blank, false);
            }
            continue;
        }
        int slot = (g_hist_head - g_hist_count + hist) % FB_HISTORY_ROWS;
        if (slot < 0) {
            slot += FB_HISTORY_ROWS;
        }
        for (int col = 0; col < g_cols; col++) {
            fb_paint_cell(row, col, &g_history[slot][col], false);
        }
    }
}

/* Record a completed line (the one the cursor just left) into the
 * scrollback history ring. */
static void history_push(void) {
    memcpy(g_history[g_hist_head], g_text[g_cursor_y],
           (size_t)g_cols * sizeof(struct fb_cell));
    g_hist_head = (g_hist_head + 1) % FB_HISTORY_ROWS;
    if (g_hist_count < FB_HISTORY_ROWS) {
        g_hist_count++;
    }
}

void fb_scroll_page(int dir) {
    int max_back = g_hist_count - g_rows;
    if (max_back < 0) {
        max_back = 0;
    }
    g_view_back += dir * g_rows;
    if (g_view_back < 0) {
        g_view_back = 0;
    }
    if (g_view_back > max_back) {
        g_view_back = max_back;
    }
    if (g_view_back == 0) {
        fb_redraw_live();
    } else {
        fb_redraw_history();
    }
}

bool fb_view_scrolled(void) {
    return g_view_back != 0;
}

/* Move every row up by one and clear the bottom row. The pixel
 * shift only touches the text region (below g_text_top), so the boot
 * splash logos above it stay put. */
static void fb_scroll_up(void) {
    for (int row = 1; row < g_rows; row++) {
        memcpy(g_text[row - 1], g_text[row],
               (size_t)g_cols * sizeof(struct fb_cell));
    }
    struct fb_cell blank = { ' ', g_cur_color };
    for (int col = 0; col < g_cols; col++) {
        g_text[g_rows - 1][col] = blank;
    }

    if (g_batch_active) {
        g_batch_dirty = true;
        return;
    }

    if (g_graphics) {
        return;
    }

    /* Shift the text area up by one row. This is the largest copy the
     * system makes - four megabytes on a 1280x800 screen, for every
     * line of output that reaches the bottom - so it moves eight bytes
     * at a time rather than going through memcpy(), which is byte
     * granular and costs one operation per byte under emulation. The
     * copy runs forward and the destination is below the source, so
     * the overlap is safe at any width. */
    uint8_t *bytes = (uint8_t *)g_pixels + (uint64_t)g_text_top * g_pitch_bytes;
    uint64_t line_bytes = (uint64_t)FONT_HEIGHT * g_pitch_bytes;
    uint64_t total_bytes = (uint64_t)(g_rows - 1) * line_bytes;

    if ((((uintptr_t)bytes | (uintptr_t)line_bytes) & 7u) == 0) {
        fb_u64_alias *dst = (fb_u64_alias *)bytes;
        const fb_u64_alias *src = (const fb_u64_alias *)(bytes + line_bytes);
        uint64_t words = total_bytes / 8;
        for (uint64_t i = 0; i < words; i++) {
            dst[i] = src[i];
        }
        fb_u64_alias *tail = (fb_u64_alias *)(bytes + total_bytes);
        for (uint64_t i = 0; i < line_bytes / 8; i++) {
            tail[i] = 0;
        }
    } else {
        /* A pitch that is not a multiple of eight: fall back. */
        memmove(bytes, bytes + line_bytes, total_bytes);
        memset(bytes + total_bytes, 0, line_bytes);
    }

    /* The pixels moved up a row, so what is on screen moved with
     * them: shift the record too, rather than throwing it away and
     * repainting a screen that is already correct. The bottom row was
     * just blanked to black, which is not necessarily the current
     * background, so it is the one row that has to be forgotten. */
    for (int row = 1; row < g_rows; row++) {
        memcpy(g_shown[row - 1], g_shown[row],
               (size_t)g_cols * sizeof(struct fb_cell));
    }
    memset(g_shown[g_rows - 1], 0xFF, (size_t)g_cols * sizeof(struct fb_cell));
    if (g_cur_row == 0) {
        g_cur_row = -1; /* the cursor's row scrolled off the top */
        g_cur_col = -1;
    } else if (g_cur_row > 0) {
        g_cur_row--;
    }
}

/* True while the cursor is parked on the last column with its wrap
 * still owed (the host tests check the rule directly). */
bool fb_wrap_pending(void) {
    return g_wrap_pending;
}

int fb_init(struct limine_framebuffer *fb) {
    if (fb == NULL || fb->memory_model != LIMINE_FRAMEBUFFER_RGB || fb->bpp != 32) {
        return -1;
    }

    g_fb = fb;
    g_pixels = (uint32_t *)fb->address;
    g_pitch_bytes = fb->pitch;
    g_pitch_words = fb->pitch / 4;
    g_width = fb->width;
    g_height = fb->height;
    g_bpp = fb->bpp;

    fb_recompute_grid();

    /* Seed the palette with the default color pair. */
    g_palette_count = 0;
    g_cur_color = palette_lookup(COLOR_FG, COLOR_BG);

    fb_clear();
    return 0;
}

void fb_clear(void) {
    struct fb_cell blank = { ' ', g_cur_color };
    for (int row = 0; row < g_rows; row++) {
        for (int col = 0; col < g_cols; col++) {
            g_text[row][col] = blank;
        }
    }
    if (!g_graphics) {
        memset(g_pixels, 0, (size_t)(g_pitch_bytes * g_height));
    }
    fb_invalidate();
    g_text_top = 0; /* the splash band only exists during boot */
    g_cursor_x = 0;
    g_cursor_y = 0;
    g_hist_head = 0;
    g_hist_count = 0;
    g_view_back = 0;
    g_wrap_pending = false;
    cursor_draw(); /* last: the block belongs at the new home position */
}

/* ---- UTF-8 ----
 *
 * Console output is a byte stream and always has been: kprintf, a
 * program writing to /dev/tty0, a terminal escape sequence. Those
 * bytes are now read as UTF-8, which is what makes `ls` show a file
 * called "gündüz" and the shell echo a Turkish 'ş' back as one
 * character in one cell.
 *
 * The decoder is deliberately strict about the things that go wrong
 * in practice rather than complete about the things that do not:
 *
 *  - a continuation byte with no lead byte is dropped, not drawn as
 *    a random glyph
 *  - an ASCII byte arriving mid-sequence abandons the sequence and is
 *    handled as itself, so one corrupt byte costs one character
 *    instead of swallowing the next two
 *  - overlong forms and surrogates are rejected: they decode to a
 *    codepoint that has a glyph but was never meant to be written
 *    that way, and accepting them is how a length check gets fooled
 *
 * Four-byte sequences decode and then find no glyph, which is the
 * right answer: the cell is a uint16_t and this font has nothing
 * above the Basic Multilingual Plane.
 */
static uint32_t g_utf8_cp;    /* codepoint built so far */
static int      g_utf8_left;  /* continuation bytes still expected */
static uint32_t g_utf8_min;   /* smallest value this length may encode */

/* Returns true when *out holds a finished codepoint. */
static bool utf8_feed(uint8_t b, uint32_t *out) {
    if (b < 0x80) {
        g_utf8_left = 0; /* abandon any half-built sequence */
        *out = b;
        return true;
    }

    if ((b & 0xC0) == 0x80) { /* continuation */
        if (g_utf8_left == 0) {
            return false; /* stray: no lead byte to belong to */
        }
        g_utf8_cp = (g_utf8_cp << 6) | (uint32_t)(b & 0x3F);
        if (--g_utf8_left > 0) {
            return false;
        }
        uint32_t cp = g_utf8_cp;
        if (cp < g_utf8_min || (cp >= 0xD800 && cp <= 0xDFFF) ||
            cp > 0x10FFFF) {
            return false; /* overlong, surrogate, or out of range */
        }
        *out = cp;
        return true;
    }

    /* A lead byte. */
    if ((b & 0xE0) == 0xC0) {
        g_utf8_cp = b & 0x1F;
        g_utf8_left = 1;
        g_utf8_min = 0x80;
    } else if ((b & 0xF0) == 0xE0) {
        g_utf8_cp = b & 0x0F;
        g_utf8_left = 2;
        g_utf8_min = 0x800;
    } else if ((b & 0xF8) == 0xF0) {
        g_utf8_cp = b & 0x07;
        g_utf8_left = 3;
        g_utf8_min = 0x10000;
    } else {
        g_utf8_left = 0; /* 0xF8..0xFF are not UTF-8 at all */
    }
    return false;
}

void fb_putchar(char c) {
    /* Escape sequences never reach the normal drawing path. */
    if (ansi_consume(c)) {
        return;
    }

    /* New output snaps the view back to the live bottom edge. */
    if (g_view_back != 0) {
        g_view_back = 0;
        fb_redraw_live();
    }

    /* Take the cursor block off before the cell under it changes -
     * UNLESS the character about to be handled is a plain printable
     * one that is going to redraw that exact same cell anyway (every
     * ordinary character, as long as it isn't completing a deferred
     * line wrap onto a new row). fb_paint_cell()'s own inv/was_inv
     * bookkeeping already clears the inversion as a side effect of
     * that redraw - it does not care WHY the cell changed - so an
     * explicit erase here would just paint the cell blank-and-plain
     * only to have the real character overwrite it a few instructions
     * later. Under heavy output that redundant middle draw was the
     * dominant cost: profiling with a temporary probe (`fbstress`,
     * since removed) measured 8 full 1920x1080 screens of plain text
     * through console_write() at 7245ms before this change; the
     * per-character cost is 3 real glyph draws (erase-old-cursor-cell,
     * the character itself at that same cell, invert-the-new-cursor-
     * cell) when only 2 are ever needed. \n, \r, \b and a wrap all
     * move the cursor away without a compensating draw at the old
     * cell, so they still need the standalone erase. */
    bool defer_erase = !g_wrap_pending && c != '\n' && c != '\r' &&
                       c != '\b' && c != '\t';
    if (!defer_erase) {
        cursor_erase();
    }

    switch (c) {
    case '\n':
        g_wrap_pending = false;
        history_push(); /* the line the cursor just left is complete */
        g_cursor_x = 0;
        g_cursor_y++;
        break;
    case '\r':
        g_wrap_pending = false;
        g_cursor_x = 0;
        break;
    case '\b':
        g_wrap_pending = false;
        if (g_cursor_x > 0) {
            g_cursor_x--;
            g_text[g_cursor_y][g_cursor_x].cp = ' ';
            g_text[g_cursor_y][g_cursor_x].color = g_cur_color;
            fb_draw_cell(g_cursor_y, g_cursor_x, false);
        }
        break;
    case '\t':
        /* A tab that reaches the last column stops there rather than
         * spinning: the column no longer advances once the wrap is
         * pending. */
        do {
            fb_putchar(' ');
        } while (g_cursor_x % 8 != 0 && !g_wrap_pending);
        return;
    default: {
        /* Bytes accumulate into a codepoint; only a finished one
         * reaches a cell. A control byte other than the four handled
         * above, and any codepoint this font cannot draw, produces
         * nothing - the same as before, now decided by whether a
         * glyph exists rather than by an ASCII range. */
        uint32_t cp = 0;
        if (utf8_feed((uint8_t)c, &cp) && font_glyph_rows(cp) != NULL) {
            if (g_wrap_pending) {
                /* The wrap the previous character earned. */
                g_wrap_pending = false;
                history_push();
                g_cursor_x = 0;
                g_cursor_y++;
                if (g_cursor_y >= g_rows) {
                    fb_scroll_up();
                    g_cursor_y = g_rows - 1;
                }
            }
            g_text[g_cursor_y][g_cursor_x].cp = (uint16_t)cp;
            g_text[g_cursor_y][g_cursor_x].color = g_cur_color;
            fb_draw_cell(g_cursor_y, g_cursor_x, false);
            if (g_cursor_x + 1 >= g_cols) {
                g_wrap_pending = true; /* stay put until the next one */
            } else {
                g_cursor_x++;
            }
        }
        break;
    }
    }

    if (g_cursor_y >= g_rows) {
        fb_scroll_up();
        g_cursor_y = g_rows - 1;
    }

    /* Draw the cursor at its new position (when visible). */
    cursor_draw();
}

void fb_set_color(uint32_t fg, uint32_t bg) {
    g_fg = fg;
    g_bg = bg;
    fb_update_color();
}

/* Report the text grid size (columns x rows) for TIOCGWINSZ. */
void fb_get_grid(int *cols, int *rows) {
    if (cols) {
        *cols = g_cols;
    }
    if (rows) {
        *rows = g_rows;
    }
}

void fb_fill(uint32_t color) {
    if (g_graphics) {
        return;
    }
    uint32_t *p = g_pixels;
    uint64_t words = (g_pitch_bytes / 4) * g_height;
    uint64_t i = 0;
    if (((uintptr_t)p & 7u) != 0 && words > 0) {
        p[i++] = color;
    }
    fb_u64_alias *q = (fb_u64_alias *)(p + i);
    uint64_t both = ((uint64_t)color << 32) | color;
    uint64_t pairs = (words - i) / 2;
    for (uint64_t j = 0; j < pairs; j++) {
        q[j] = both;
    }
    for (i += pairs * 2; i < words; i++) {
        p[i] = color;
    }
    fb_invalidate(); /* the text grid is gone from the screen */
}

/* Start the text grid `pixel_y` pixels below the top of the screen
 * (the boot splash draws its logos above this line). */
void fb_set_text_top(uint32_t pixel_y) {
    if (g_text_top != pixel_y) {
        g_text_top = pixel_y;
        fb_invalidate(); /* every cell maps to a different scanline now */
    }
}

/* Draw a scaled RGB image (nearest neighbour) with its top-left
 * corner at (x, y). `scale` is a 16.16 fixed-point factor: an output
 * pixel of size 1x1 samples src[oy*scale>>16][ox*scale>>16]. */
void fb_blit_scaled(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                    const uint8_t *rgb, uint32_t scale) {
    if (g_pixels == NULL || rgb == NULL || scale == 0 || g_graphics) {
        return;
    }

    uint32_t out_w = (uint32_t)(((uint64_t)w * scale) >> 16);
    uint32_t out_h = (uint32_t)(((uint64_t)h * scale) >> 16);
    if (out_w == 0 || out_h == 0) {
        return;
    }

    for (uint32_t oy = 0; oy < out_h; oy++) {
        uint32_t sy = (uint32_t)(((uint64_t)oy * h) / out_h);
        if (sy >= h) {
            sy = h - 1;
        }
        uint32_t py = y + oy;
        if (py >= g_height) {
            break;
        }
        const uint8_t *src_row = rgb + (uint64_t)sy * w * 3;
        uint32_t *dst = g_pixels + (uint64_t)py * g_pitch_words + x;

        for (uint32_t ox = 0; ox < out_w; ox++) {
            uint32_t sx = (uint32_t)(((uint64_t)ox * w) / out_w);
            if (sx >= w) {
                sx = w - 1;
            }
            uint32_t px = x + ox;
            if (px >= g_width) {
                break;
            }
            const uint8_t *s = src_row + (uint64_t)sx * 3;
            dst[ox] = ((uint32_t)s[0] << 16) | ((uint32_t)s[1] << 8) | s[2];
        }
    }
    fb_invalidate(); /* the image may have covered part of the grid */
}

void fb_get_info(uint32_t *width, uint32_t *height, uint32_t *bpp,
                 uint64_t *pitch, void **address) {
    if (width) {
        *width = g_width;
    }
    if (height) {
        *height = g_height;
    }
    if (bpp) {
        *bpp = g_bpp;
    }
    if (pitch) {
        *pitch = g_pitch_bytes;
    }
    if (address) {
        *address = (void *)g_pixels;
    }
}

/* ---- runtime mode setting ---- */

/* Recompute the character grid for the current pixel geometry and
 * clamp it to the cell arrays. Shared by fb_init and fb_set_mode so
 * the two can never disagree about what a mode means. */
static void fb_recompute_grid(void) {
    g_cols = (int)(g_width / FONT_WIDTH);
    g_rows = (int)(g_height / FONT_HEIGHT);
    if (g_cols > FB_MAX_COLS) {
        g_cols = FB_MAX_COLS;
    }
    if (g_rows > FB_MAX_ROWS) {
        g_rows = FB_MAX_ROWS;
    }
}

/* Change the display mode at runtime.
 *
 * Three things have to happen in an order that survives a failure at
 * any step:
 *
 *   1. Map enough of the linear framebuffer for the NEW mode. The
 *      physical address does not move when the mode changes, so this
 *      is safe to do first - and doing it first is the point: if the
 *      mapping fails we return with the old mode still on screen
 *      instead of with a mode nobody can paint into.
 *   2. Program the adapter.
 *   3. Rebind the console to the new geometry.
 *
 * The framebuffer gets its own kernel virtual reservation rather than
 * staying on the mapping Limine made: Limine mapped exactly the boot
 * mode's bytes, and a larger mode reads and writes past the end of
 * it. VMM_FB_BASE lives in the same 1 GiB PDPT slot as the kernel
 * image, so every address space already shares the page directory
 * that the new leaf tables hang off - a task created before the mode
 * change still sees the new mapping.
 *
 * Returns 0, or a negative errno (see vbe_set_mode, plus -ENOSPC for
 * a mode that does not fit the reservation). */
int fb_set_mode(uint32_t width, uint32_t height) {
    if (!vbe_available()) {
        return -ENODEV;
    }
    if (width == 0 || height == 0 ||
        width > VBE_MAX_WIDTH || height > VBE_MAX_HEIGHT) {
        return -EINVAL;
    }

    uint64_t phys = vbe_lfb_phys();
    if (phys == 0) {
        return -ENODEV;
    }

    /* Worst-case size for the requested mode. The adapter may report
     * a wider pitch afterwards; allow a scanline of slack so a small
     * alignment bump does not walk off the mapping. */
    uint64_t max_pitch = ((uint64_t)width * 4u + 255u) & ~255ull;
    uint64_t need = max_pitch * (uint64_t)height;
    need = (need + 0xFFFull) & ~0xFFFull;
    if (need > VMM_FB_SIZE) {
        return -ENOSPC;
    }

    /* From here on, the adapter's registers and this file's cached
     * idea of the screen (g_width/g_height/g_pixels/g_pitch_bytes,
     * g_cols/g_rows) must change together. Every console write -
     * kprintf, a program's /dev/tty0 write, a highX repaint - reads
     * that cache with no lock of its own (it trusts it to be
     * consistent), and the 100 Hz scheduler tick can switch to one of
     * those the instant this task is preemptible. Without this guard,
     * a tick landing between "the hardware is already the new mode"
     * (vbe_set_mode returned) and "the cache says so too" (the
     * assignments below) lets another task paint using the OLD
     * stride/width into a framebuffer the adapter is now scanning out
     * at the NEW one - a sheared, garbled screen that gets more likely
     * the more times res_set is used in a session, not a fixed repro
     * count. Same class of bug as kmalloc.c's missing
     * preempt_disable() (see tus-lvgl-port memory). */
    preempt_disable();

    if (vmm_map_region(VMM_FB_BASE, phys, (size_t)need,
                       VMM_PRESENT | VMM_WRITE) != 0) {
        preempt_enable();
        return -ENOMEM;
    }

    uint64_t pitch = 0;
    int rc = vbe_set_mode(width, height, 32, &pitch);
    if (rc != 0) {
        preempt_enable();
        return rc;
    }
    if (pitch > max_pitch) {
        /* The adapter wants more per scanline than we mapped. Put the
         * old mode back rather than paint into unmapped memory. */
        (void)vbe_set_mode(g_width, g_height, 32, NULL);
        preempt_enable();
        return -ENOSPC;
    }

    g_pixels = (uint32_t *)(uintptr_t)VMM_FB_BASE;
    g_pitch_bytes = pitch;
    g_pitch_words = pitch / 4;
    g_width = width;
    g_height = height;
    g_bpp = 32;
    g_fb = NULL; /* the Limine description is now historical */

    fb_recompute_grid();

    /* fb_clear() resets the cursor, the scrollback and the splash
     * offset, and blanks the pixels - all of which a mode change
     * needs: the history holds lines laid out for the old width, and
     * the framebuffer holds whatever the adapter left behind. In
     * graphics mode it leaves the pixels alone; highX repaints the
     * whole screen when the server rebinds. */
    fb_clear();
    preempt_enable();
    return 0;
}

/* ---- graphics mode (highX) ---- */

void fb_set_graphics(bool on) {
    if (g_graphics != on) {
        g_graphics = on;
        /* highX painted over everything; the console knows nothing
         * about the screen until it repaints it. */
        fb_invalidate();
    }
}

bool fb_graphics(void) {
    return g_graphics;
}

/* Repaint the text grid from the character buffer. The console never
 * stopped tracking output while highX had the screen, so this brings
 * the shell back with every line that scrolled past in the meantime. */
void fb_repaint(void) {
    if (g_pixels == NULL || g_graphics) {
        return;
    }
    memset(g_pixels, 0, (size_t)(g_pitch_bytes * g_height));
    fb_invalidate();
    fb_redraw_live();
}
