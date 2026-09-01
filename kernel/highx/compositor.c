/*
 * compositor.c - highX compositor and window drawing primitives
 *
 * Two halves live here:
 *
 *   1. Window drawing (hxdraw_*). Clients never touch the screen;
 *      they draw into their window's backing store, in window-local
 *      coordinates, clipped to the window. This is where the protocol
 *      drawing requests end up.
 *
 *   2. Screen composition (hxcomp_paint). Given a damaged screen
 *      rectangle and the window stack (bottom first), the desktop
 *      background is painted first and every visible window is then
 *      blitted over it, clipped to the damage. Nothing outside the
 *      damaged rectangle is ever touched, which is what keeps a
 *      cursor blink or a clock tick cheap on a 1024x768 screen.
 *
 * highX assumes a 32 bits-per-pixel framebuffer (what Limine hands us
 * on every machine TUS supports); hxcomp_init() refuses anything else
 * rather than painting garbage.
 *
 * Speed. Everything here is a loop over pixels, and the kernel is
 * built with -mgeneral-regs-only: there is no vectoriser to lean on,
 * so the inner loops are written to do the pairing themselves. Runs
 * of one colour go through fill_span() (one 64-bit store per two
 * pixels), pixel copies through copy_span_opaque() the same way, and the
 * shapes that used to test every pixel - the border ring, the cursor
 * sprite, a glyph - are clipped once and then walked without a branch
 * per pixel. tests/highx checks the result against a brute-force
 * model, which is what lets these loops be rewritten at all.
 */

#include "highx.h"

#include "../core/klib.h"
#include "../drivers/fb/fb.h"
#include "../drivers/fb/font_latin.h"

/* ---- screen state ---- */

static uint8_t *g_pixels;   /* framebuffer base */
static uint64_t g_pitch;    /* bytes per scanline */
static uint32_t g_width;
static uint32_t g_height;
static uint32_t g_bpp;

static uint32_t g_bg_color = 0x00101820; /* desktop background */
static uint32_t g_bg_style = HX_BG_GRID;
static uint32_t g_bg_grid  = 0x00182430;

#define GRID_STEP 32

/* ---- the pointer sprite ----
 *
 * A 12x19 arrow, the shape every window system has drawn since the
 * eighties: '#' is the black outline, '.' the white body, ' ' lets
 * whatever is underneath show through. Keeping it as text is worth a
 * few bytes of translation at boot - it is the one part of the
 * compositor a human wants to be able to read and change. */
#define CURSOR_ART_W 12
#define CURSOR_ART_H 19

static const char *const g_cursor_art[CURSOR_ART_H] = {
    "#           ",
    "##          ",
    "#.#         ",
    "#..#        ",
    "#...#       ",
    "#....#      ",
    "#.....#     ",
    "#......#    ",
    "#.......#   ",
    "#........#  ",
    "#.....##### ",
    "#..#..#     ",
    "#.# #..#    ",
    "##  #..#    ",
    "#    #..#   ",
    "     #..#   ",
    "      #..#  ",
    "      #..#  ",
    "       ##   ",
};

#define CURSOR_OUTLINE 0x00101010u
#define CURSOR_BODY    0x00F0F0F0u
#define CURSOR_HALO    0x00000000u
#define CURSOR_HALO_COV 90 /* 0..255: a soft dark ring, not a hard edge */

/* The sprite the compositor actually paints is the hand-drawn art
 * above plus a 1px margin on every side, so a dilation pass (see
 * cursor_compile()) has somewhere to put a soft halo around it
 * without clipping at the array edge - a real anti-aliasing move
 * rather than the flat on/off stamp this used to be, and one this
 * codebase can afford to compute once at boot instead of hand-editing
 * new ASCII art for every diagonal (the same reasoning tusfont's
 * coverage rendering and hglui's SDF shapes already use, just derived
 * from a mask by dilation instead of from real geometry). */
#define CURSOR_W (CURSOR_ART_W + 2)
#define CURSOR_H (CURSOR_ART_H + 2)
/* The hotspot (the arrow's tip, the point every click/position report
 * actually means) sits at the original art's (0,0), now shifted to
 * (1,1) in the padded grid - g_ptr_x/g_ptr_y stay exactly the click
 * point they always were, and only the sprite's on-screen bounding
 * box moves up-left by this much to keep the tip there. */
#define CURSOR_HOT_X 1
#define CURSOR_HOT_Y 1

/* The art translated once, at hxcomp_init(): a colour and an 8-bit
 * coverage (0 transparent .. 255 opaque) per pixel, plus the first
 * and last covered column of each row. Painting the cursor then costs
 * a store per opaque pixel (coverage 255 still takes the old fast
 * path) or a cheap blend per halo pixel, instead of a character
 * comparison and four bounds tests per pixel - and a move, which
 * repaints the sprite twice (once to erase, once to draw), is the
 * most frequent paint highX does. */
static uint32_t g_cursor_pix[CURSOR_H][CURSOR_W];
static uint8_t  g_cursor_cov[CURSOR_H][CURSOR_W];
static int8_t   g_cursor_first[CURSOR_H];
static int8_t   g_cursor_last[CURSOR_H]; /* exclusive */

static void cursor_compile(void) {
    /* Pass 1: the art itself, shifted by (1,1) into the padded grid. */
    for (int32_t row = 0; row < CURSOR_H; row++) {
        for (int32_t col = 0; col < CURSOR_W; col++) {
            int32_t ar = row - 1, ac = col - 1;
            char c = ' ';
            if (ar >= 0 && ar < CURSOR_ART_H && ac >= 0 && ac < CURSOR_ART_W) {
                const char *art = g_cursor_art[ar];
                c = art[ac] != '\0' ? art[ac] : ' ';
            }
            if (c == ' ') {
                g_cursor_cov[row][col] = 0;
                g_cursor_pix[row][col] = 0;
            } else {
                g_cursor_cov[row][col] = 255;
                g_cursor_pix[row][col] = c == '#' ? CURSOR_OUTLINE : CURSOR_BODY;
            }
        }
    }

    /* Pass 2: dilate - any still-empty pixel touching an opaque one
     * (4-neighbourhood) becomes a soft dark halo. This is what makes
     * the diagonal edges of the arrow (and the arrow itself, against
     * either a light or a dark window underneath) read as smooth
     * instead of stair-stepped, without hand-placing edge pixels in
     * the art. */
    static uint8_t was_opaque[CURSOR_H][CURSOR_W];
    for (int32_t row = 0; row < CURSOR_H; row++) {
        for (int32_t col = 0; col < CURSOR_W; col++) {
            was_opaque[row][col] = g_cursor_cov[row][col] == 255;
        }
    }
    for (int32_t row = 0; row < CURSOR_H; row++) {
        for (int32_t col = 0; col < CURSOR_W; col++) {
            if (g_cursor_cov[row][col] != 0) {
                continue;
            }
            bool touches_opaque =
                (row > 0 && was_opaque[row - 1][col]) ||
                (row + 1 < CURSOR_H && was_opaque[row + 1][col]) ||
                (col > 0 && was_opaque[row][col - 1]) ||
                (col + 1 < CURSOR_W && was_opaque[row][col + 1]);
            if (touches_opaque) {
                g_cursor_cov[row][col] = CURSOR_HALO_COV;
                g_cursor_pix[row][col] = CURSOR_HALO;
            }
        }
    }

    for (int32_t row = 0; row < CURSOR_H; row++) {
        int32_t first = CURSOR_W, last = 0;
        for (int32_t col = 0; col < CURSOR_W; col++) {
            if (g_cursor_cov[row][col] != 0) {
                if (col < first) {
                    first = col;
                }
                last = col + 1;
            }
        }
        g_cursor_first[row] = (int8_t)(first <= last ? first : 0);
        g_cursor_last[row] = (int8_t)last;
    }
}

/* out = blend `fg` over `bg` at 8-bit coverage `cov` (0..255), one
 * channel at a time - the same lerp `mix()` in userspace's compositors
 * (hglui, tusde's wallpaper) does, just on a packed 0x00RRGGBB word. */
static inline uint32_t cursor_blend(uint32_t bg, uint32_t fg, uint8_t cov) {
    uint32_t inv = 255 - cov;
    uint32_t r = (((bg >> 16) & 0xFF) * inv + ((fg >> 16) & 0xFF) * cov) / 255;
    uint32_t g = (((bg >> 8) & 0xFF) * inv + ((fg >> 8) & 0xFF) * cov) / 255;
    uint32_t b = ((bg & 0xFF) * inv + (fg & 0xFF) * cov) / 255;
    return (r << 16) | (g << 8) | b;
}

static int32_t g_ptr_x, g_ptr_y;
/* Off until the server places it: a compositor with no session behind
 * it (the host unit tests, a screen painted before highx_start) must
 * not stamp an arrow into the top left corner. */
static bool g_ptr_visible;

int hxcomp_init(void) {
    uint32_t w = 0, h = 0, bpp = 0;
    uint64_t pitch = 0;
    void *addr = NULL;

    fb_get_info(&w, &h, &bpp, &pitch, &addr);
    if (addr == NULL || w == 0 || h == 0) {
        return -1;
    }
    if (bpp != 32) {
        return -1; /* highX only paints 32bpp surfaces */
    }
    g_pixels = (uint8_t *)addr;
    g_pitch = pitch;
    g_width = w;
    g_height = h;
    g_bpp = bpp;
    cursor_compile();
    return 0;
}

void hxcomp_set_pointer(int32_t x, int32_t y) {
    g_ptr_x = x;
    g_ptr_y = y;
}

void hxcomp_pointer_rect(int32_t *x, int32_t *y, int32_t *w, int32_t *h) {
    if (x != NULL) {
        *x = g_ptr_x - CURSOR_HOT_X;
    }
    if (y != NULL) {
        *y = g_ptr_y - CURSOR_HOT_Y;
    }
    if (w != NULL) {
        *w = CURSOR_W;
    }
    if (h != NULL) {
        *h = CURSOR_H;
    }
}

void hxcomp_show_pointer(bool visible) {
    g_ptr_visible = visible;
}

void hxcomp_screen(uint32_t *w, uint32_t *h, uint32_t *bpp) {
    if (w != NULL) {
        *w = g_width;
    }
    if (h != NULL) {
        *h = g_height;
    }
    if (bpp != NULL) {
        *bpp = g_bpp;
    }
}

/* Derive the grid line color from the base color: a few shades
 * lighter, saturating per channel so dark and light desktops both
 * keep a visible grid. */
void hxcomp_set_background(uint32_t color, uint32_t style) {
    g_bg_color = color & 0x00FFFFFFu;
    g_bg_style = style;

    uint32_t r = (color >> 16) & 0xFF, g = (color >> 8) & 0xFF, b = color & 0xFF;
    r = r + 20 > 255 ? 255 : r + 20;
    g = g + 20 > 255 ? 255 : g + 20;
    b = b + 24 > 255 ? 255 : b + 24;
    g_bg_grid = (r << 16) | (g << 8) | b;
}

/* ---- screen composition ---- */

/*
 * Flicker is not something a pixel comparison can catch: the screen
 * ends up correct either way. What it is, is a pixel written twice
 * inside one repaint - the desktop first, the window over it a moment
 * later - and the eye sees the flash in between. So the screen writes
 * are countable, and tests/highx asserts that one repaint writes each
 * pixel once. In the kernel build the macro expands to nothing.
 */
#ifdef HXCOMP_TRACE_WRITES
void hxcomp_trace_write(const uint32_t *dst, int32_t n);
#define HX_TRACE(dst, n) hxcomp_trace_write((dst), (n))
#else
#define HX_TRACE(dst, n) ((void)0)
#endif

static inline uint32_t *screen_row(uint32_t y) {
    return (uint32_t *)(g_pixels + (uint64_t)y * g_pitch);
}

/* Two pixels are eight bytes, and eight bytes are one store. The
 * kernel has no SSE (-mgeneral-regs-only), so a plain `uint32_t`
 * loop is exactly one store per pixel however high the optimisation
 * level; pairing them by hand halves the stores in every fill highX
 * does - the desktop background, a window's initial clear, an opaque
 * glyph box. may_alias keeps the aliasing rules happy about looking
 * at a pixel array through a 64-bit type. */
typedef uint64_t hx_u64_alias __attribute__((may_alias));

static inline void fill_span(uint32_t *dst, uint32_t color, int32_t n) {
    if (n <= 0) {
        return;
    }
    if (((uintptr_t)dst & 7u) != 0) { /* odd start: one pixel to align */
        *dst++ = color;
        n--;
    }
    hx_u64_alias *pair = (hx_u64_alias *)dst;
    uint64_t both = ((uint64_t)color << 32) | color;
    int32_t npairs = n >> 1;
    for (int32_t i = 0; i < npairs; i++) {
        pair[i] = both;
    }
    if ((n & 1) != 0) {
        dst[n - 1] = color;
    }
}

/* The same trick for a copy that has to drop the top byte of every
 * pixel (backing stores hold 0x00RRGGBB): two loads, one store. */
static inline void copy_span_opaque(uint32_t *dst, const uint32_t *src,
                                    int32_t n) {
    if (n <= 0) {
        return;
    }
    if (((uintptr_t)dst & 7u) != 0) {
        *dst++ = *src++ & 0x00FFFFFFu;
        n--;
    }
    hx_u64_alias *pair = (hx_u64_alias *)dst;
    int32_t npairs = n >> 1;
    for (int32_t i = 0; i < npairs; i++) {
        pair[i] = ((uint64_t)(src[2 * i + 1] & 0x00FFFFFFu) << 32) |
                  (src[2 * i] & 0x00FFFFFFu);
    }
    if ((n & 1) != 0) {
        dst[n - 1] = src[n - 1] & 0x00FFFFFFu;
    }
}

/*
 * Painting front to back.
 *
 * The obvious way to compose is back to front: lay down the desktop
 * across the damaged rectangle, then paint every window over it in
 * stacking order. It is also what makes a window flicker while it is
 * being dragged, because the pixels the window lands on get written
 * twice in the same repaint - desktop first, window a moment later -
 * straight into the memory the display is being scanned out of. At
 * drag speed the eye catches the desktop showing through.
 *
 * So this goes the other way, one scanline at a time. Each row starts
 * as a single span with no colour yet; the stack is walked downwards,
 * and the first window to reach a span claims it and is painted
 * there. Whatever no window claimed is desktop. Every pixel is
 * written exactly once, there is no in-between state to see, and the
 * total work is lower than before: nothing is painted only to be
 * covered up.
 *
 * The cursor stays an overlay on top, because its shape is a mask
 * rather than a rectangle - what is under a transparent pixel of the
 * arrow has to be there already. That is the one place a pixel is
 * still written twice, and it is a 12x20 sprite.
 */

/* The parts of one scanline that no window has claimed yet. A window
 * can split a span in two at worst, so the whole stack can leave at
 * most one more span than there are windows. */
struct span_row {
    int32_t x0[HX_MAX_WINDOWS + 2];
    int32_t x1[HX_MAX_WINDOWS + 2];
    int n;
};

/* Fill one row's span with the desktop. */
static void paint_background_span(int32_t x, int32_t row, int32_t n) {
    if (n <= 0) {
        return;
    }
    uint32_t *dst = screen_row((uint32_t)row) + x;
    HX_TRACE(dst, n);

    if (g_bg_style != HX_BG_GRID) {
        fill_span(dst, g_bg_color, n);
        return;
    }

    /* row is clipped to the screen, so it is never negative and the
     * remainder is a mask rather than a division. */
    bool grid_row = ((uint32_t)row % GRID_STEP) == 0;
    fill_span(dst, grid_row ? g_bg_grid : g_bg_color, n);
    if (grid_row) {
        return;
    }

    /* First grid column at or after x, computed once instead of a
     * modulo per pixel - the desktop is the largest surface highX
     * paints, so this loop is worth keeping tight. The dots do write
     * over the fill underneath them, but a single pixel that changes
     * colour inside the desktop is not what a moving window looks
     * like. */
    int32_t first_col = ((x + GRID_STEP - 1) / GRID_STEP) * GRID_STEP - x;
    for (int32_t col = first_col; col < n; col += GRID_STEP) {
        dst[col] = g_bg_grid;
    }
}

/*
 * Paint [x0, x1) of one row from `win`. The border lives outside the
 * window's own rectangle (X11's model), so a window manager can mark
 * the focused window without taking drawing area away from the
 * application - which means a span can cross up to three regions:
 * left border, the window itself, right border.
 */
static void paint_window_span(const struct hxs_window *win, int32_t row,
                              int32_t x0, int32_t x1) {
    uint32_t *dst = screen_row((uint32_t)row);
    uint32_t color = win->border_color & 0x00FFFFFFu;
    int32_t ix0 = win->x, ix1 = win->x + (int32_t)win->w;

    /* Above or below the window proper: the whole span is border. */
    if (row < win->y || row >= win->y + (int32_t)win->h) {
        HX_TRACE(dst + x0, x1 - x0);
        fill_span(dst + x0, color, x1 - x0);
        return;
    }

    int32_t left = x1 < ix0 ? x1 : ix0;
    if (x0 < left) {
        HX_TRACE(dst + x0, left - x0);
        fill_span(dst + x0, color, left - x0);
    }

    int32_t m0 = x0 > ix0 ? x0 : ix0;
    int32_t m1 = x1 < ix1 ? x1 : ix1;
    if (m0 < m1) {
        /* Backing stores already hold 0x00RRGGBB, so the blit is a
         * plain copy. */
        const uint32_t *src = win->pix +
                              (uint64_t)(row - win->y) * win->w + (m0 - win->x);
        HX_TRACE(dst + m0, m1 - m0);
        memcpy(dst + m0, src, (size_t)(m1 - m0) * sizeof(uint32_t));
    }

    int32_t right = x0 > ix1 ? x0 : ix1;
    if (right < x1) {
        HX_TRACE(dst + right, x1 - right);
        fill_span(dst + right, color, x1 - right);
    }
}

/* Hand every span this window reaches to it, and keep the rest. */
static void span_claim(struct span_row *spans, const struct hxs_window *win,
                       int32_t row, int32_t cx0, int32_t cx1) {
    struct span_row left;
    left.n = 0;

    for (int i = 0; i < spans->n; i++) {
        int32_t s0 = spans->x0[i], s1 = spans->x1[i];
        int32_t i0 = s0 > cx0 ? s0 : cx0;
        int32_t i1 = s1 < cx1 ? s1 : cx1;

        if (i0 >= i1) { /* the window is nowhere near this span */
            left.x0[left.n] = s0;
            left.x1[left.n] = s1;
            left.n++;
            continue;
        }

        paint_window_span(win, row, i0, i1);
        if (s0 < i0) {
            left.x0[left.n] = s0;
            left.x1[left.n] = i0;
            left.n++;
        }
        if (i1 < s1) {
            left.x0[left.n] = i1;
            left.x1[left.n] = s1;
            left.n++;
        }
    }
    *spans = left;
}


/* Clip [*pos, *pos + *len) into [0, limit). Returns false when the
 * span is empty, in which case pos and len are meaningless. */
static bool clip_span(int32_t *pos, int32_t *len, int32_t limit) {
    if (*len <= 0) {
        return false;
    }
    if (*pos < 0) {
        *len += *pos;
        *pos = 0;
    }
    if (*pos >= limit) {
        return false;
    }
    if (*pos + *len > limit) {
        *len = limit - *pos;
    }
    return *len > 0;
}

void hxwin_outer(const struct hxs_window *win, int32_t *x, int32_t *y,
                 int32_t *w, int32_t *h) {
    if (win == NULL) {
        return;
    }
    int32_t b = (int32_t)win->border_w;
    if (x != NULL) {
        *x = win->x - b;
    }
    if (y != NULL) {
        *y = win->y - b;
    }
    if (w != NULL) {
        *w = (int32_t)win->w + 2 * b;
    }
    if (h != NULL) {
        *h = (int32_t)win->h + 2 * b;
    }
}

/* Draw the cursor over the damaged rectangle, if it reaches into it.
 * The sprite rectangle is intersected with the damage and the screen
 * once; what is left is walked straight, using the compiled art. */
static void paint_pointer(int32_t dx, int32_t dy, int32_t dw, int32_t dh) {
    if (!g_ptr_visible) {
        return;
    }
    int32_t sprite_x = g_ptr_x - CURSOR_HOT_X;
    int32_t sprite_y = g_ptr_y - CURSOR_HOT_Y;
    int32_t x0 = sprite_x > dx ? sprite_x : dx;
    int32_t y0 = sprite_y > dy ? sprite_y : dy;
    int32_t x1 = sprite_x + CURSOR_W < dx + dw ? sprite_x + CURSOR_W : dx + dw;
    int32_t y1 = sprite_y + CURSOR_H < dy + dh ? sprite_y + CURSOR_H : dy + dh;
    if (x0 < 0) {
        x0 = 0;
    }
    if (y0 < 0) {
        y0 = 0;
    }
    if (x1 > (int32_t)g_width) {
        x1 = (int32_t)g_width;
    }
    if (y1 > (int32_t)g_height) {
        y1 = (int32_t)g_height;
    }
    if (x0 >= x1 || y0 >= y1) {
        return; /* the arrow is nowhere near what was damaged */
    }

    for (int32_t sy = y0; sy < y1; sy++) {
        int32_t row = sy - sprite_y;
        int32_t c0 = x0 - sprite_x, c1 = x1 - sprite_x;
        if (c0 < g_cursor_first[row]) {
            c0 = g_cursor_first[row];
        }
        if (c1 > g_cursor_last[row]) {
            c1 = g_cursor_last[row];
        }
        uint32_t *dst = screen_row((uint32_t)sy) + sprite_x;
        for (int32_t col = c0; col < c1; col++) {
            uint8_t cov = g_cursor_cov[row][col];
            if (cov == 0) {
                continue;
            }
            HX_TRACE(&dst[col], 1);
            /* Full coverage (the outline and the body proper) is a
             * plain store, same cost as before; only the thin halo
             * ring pays for a blend. */
            dst[col] = cov == 255 ? g_cursor_pix[row][col]
                                  : cursor_blend(dst[col], g_cursor_pix[row][col], cov);
        }
    }
}

/* True when `win` has any pixel inside the damaged rectangle. A stack
 * is mostly windows that are nowhere near what changed, and one
 * rejection here saves both the border walk and the blit setup. */
static bool touches(const struct hxs_window *win, int32_t x, int32_t y,
                    int32_t w, int32_t h) {
    int32_t ox, oy, ow, oh;
    hxwin_outer(win, &ox, &oy, &ow, &oh);
    return ox < x + w && oy < y + h && ox + ow > x && oy + oh > y;
}

/* A window slot the compositor may paint from. */
static inline bool paintable(const struct hxs_window *win) {
    return win != NULL && win->id != 0 && win->mapped && win->pix != NULL;
}

void hxcomp_paint(int32_t x, int32_t y, int32_t w, int32_t h,
                  struct hxs_window **stack, int nstack) {
    if (g_pixels == NULL) {
        return;
    }
    if (!clip_span(&x, &w, (int32_t)g_width) ||
        !clip_span(&y, &h, (int32_t)g_height)) {
        return;
    }

    /* The windows that reach into the damaged rectangle at all,
     * topmost first. A stack is mostly windows nowhere near what
     * changed, and rejecting them once here keeps them out of every
     * scanline below. */
    const struct hxs_window *layer[HX_MAX_WINDOWS];
    int nlayers = 0;
    for (int i = nstack - 1; i >= 0 && nlayers < HX_MAX_WINDOWS; i--) {
        struct hxs_window *win = stack[i];
        if (paintable(win) && touches(win, x, y, w, h)) {
            layer[nlayers++] = win;
        }
    }

    for (int32_t row = y; row < y + h; row++) {
        struct span_row spans;
        spans.n = 1;
        spans.x0[0] = x;
        spans.x1[0] = x + w;

        for (int i = 0; i < nlayers && spans.n > 0; i++) {
            const struct hxs_window *win = layer[i];
            /* hxwin_outer() leaves its outputs alone for a NULL
             * window; every layer here is one paintable() accepted,
             * but the zeroes keep that from being an assumption the
             * compiler has to take on trust. */
            int32_t ox = 0, oy = 0, ow = 0, oh = 0;
            hxwin_outer(win, &ox, &oy, &ow, &oh);
            if (row < oy || row >= oy + oh) {
                continue;
            }
            span_claim(&spans, win, row, ox, ox + ow);
        }

        for (int i = 0; i < spans.n; i++) {
            paint_background_span(spans.x0[i], row, spans.x1[i] - spans.x0[i]);
        }
    }

    /* The cursor floats above every window, so it goes on last. */
    paint_pointer(x, y, w, h);
}

/* ---- window drawing ---- */

/* Clip a window-local rectangle to the backing store. */
static bool clip_window(const struct hxs_window *win, int32_t *x, int32_t *y,
                        int32_t *w, int32_t *h) {
    if (win == NULL || win->pix == NULL) {
        return false;
    }
    int32_t cx = *x, cy = *y, cw = *w, ch = *h;
    if (!clip_span(&cx, &cw, (int32_t)win->w) ||
        !clip_span(&cy, &ch, (int32_t)win->h)) {
        return false;
    }
    *x = cx;
    *y = cy;
    *w = cw;
    *h = ch;
    return true;
}

static inline void win_put(struct hxs_window *win, int32_t x, int32_t y,
                           uint32_t color) {
    if (x >= 0 && y >= 0 && x < (int32_t)win->w && y < (int32_t)win->h) {
        win->pix[(uint64_t)y * win->w + x] = color & 0x00FFFFFFu;
    }
}

void hxdraw_fill(struct hxs_window *win, int32_t x, int32_t y,
                 int32_t w, int32_t h, uint32_t color) {
    if (!clip_window(win, &x, &y, &w, &h)) {
        return;
    }
    color &= 0x00FFFFFFu;
    uint32_t *dst = win->pix + (uint64_t)y * win->w + x;
    for (int32_t row = 0; row < h; row++) {
        fill_span(dst, color, w);
        dst += win->w;
    }
}

void hxdraw_rect(struct hxs_window *win, int32_t x, int32_t y,
                 int32_t w, int32_t h, uint32_t color) {
    if (win == NULL || win->pix == NULL || w <= 0 || h <= 0) {
        return;
    }
    hxdraw_fill(win, x, y, w, 1, color);
    hxdraw_fill(win, x, y + h - 1, w, 1, color);
    hxdraw_fill(win, x, y, 1, h, color);
    hxdraw_fill(win, x + w - 1, y, 1, h, color);
}

/* Bresenham, so diagonals look the same as they would on real
 * hardware acceleration - no floating point in kernel mode. */
void hxdraw_line(struct hxs_window *win, int32_t x0, int32_t y0,
                 int32_t x1, int32_t y1, uint32_t color) {
    if (win == NULL || win->pix == NULL) {
        return;
    }
    int32_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int32_t dy = y1 - y0 >= 0 ? y1 - y0 : y0 - y1;

    /* Horizontal and vertical lines are what clients actually draw -
     * rules, separators, the frames of a seven-segment digit - and a
     * clipped run is a fill, not a per-pixel walk with four bounds
     * tests each. */
    if (dy == 0) {
        hxdraw_fill(win, x0 < x1 ? x0 : x1, y0, dx + 1, 1, color);
        return;
    }
    if (dx == 0) {
        hxdraw_fill(win, x0, y0 < y1 ? y0 : y1, 1, dy + 1, color);
        return;
    }

    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx - dy;

    for (;;) {
        win_put(win, x0, y0, color);
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

/* 8x16 VGA text, the same font the console uses. HX_TF_OPAQUE fills
 * the glyph box with `bg` first; without it the background pixels are
 * left alone, so text can sit on top of drawn content.
 *
 * Text is the request hxterm, tusWM and tusDE send most, so the loop
 * is arranged around what a glyph actually is: the box is clipped to
 * the backing store once per character, an opaque box is one fill_span
 * per row, and the foreground pixels are found by walking the set
 * bits of the row byte rather than testing all eight. A blank row -
 * most of the rows in most glyphs - costs one compare. */
void hxdraw_text(struct hxs_window *win, int32_t x, int32_t y,
                 uint32_t fg, uint32_t bg, uint32_t flags, const char *text) {
    if (win == NULL || win->pix == NULL || text == NULL) {
        return;
    }
    bool opaque = (flags & HX_TF_OPAQUE) != 0;
    fg &= 0x00FFFFFFu;
    bg &= 0x00FFFFFFu;

    /* Rows are the same for every glyph in the string. */
    int32_t row0 = y < 0 ? -y : 0;
    int32_t row1 = FONT_HEIGHT;
    if (y + row1 > (int32_t)win->h) {
        row1 = (int32_t)win->h - y;
    }
    if (row0 >= row1) {
        return; /* the whole line is above or below the window */
    }

    /* The text is UTF-8 (v1.5). Bytes are decoded to codepoints as
     * they are walked, so a Turkish letter advances x by ONE column
     * rather than two - which is what makes a client that lays out a
     * grid, like the terminal, stay aligned. ASCII decodes to itself,
     * so every client written before this still draws the same. */
    const char *p = text;
    while (*p != '\0') {
        if (x >= (int32_t)win->w) {
            return; /* x only grows: nothing further can be visible */
        }

        uint32_t cp = 0;
        int extra = 0;
        unsigned char b = (unsigned char)*p++;
        if (b < 0x80) {
            cp = b;
        } else if ((b & 0xE0) == 0xC0) {
            cp = b & 0x1Fu; extra = 1;
        } else if ((b & 0xF0) == 0xE0) {
            cp = b & 0x0Fu; extra = 2;
        } else if ((b & 0xF8) == 0xF0) {
            cp = b & 0x07u; extra = 3;
        } else {
            continue; /* a stray continuation byte draws nothing */
        }
        while (extra-- > 0) {
            unsigned char cont = (unsigned char)*p;
            if ((cont & 0xC0) != 0x80) {
                cp = 0; /* truncated: give up on this character */
                break;
            }
            p++;
            cp = (cp << 6) | (cont & 0x3Fu);
        }
        if (cp == 0) {
            continue;
        }

        if (x + FONT_WIDTH <= 0) {
            x += FONT_WIDTH;
            continue; /* fully clipped column, keep advancing */
        }
        const uint8_t *glyph = font_glyph_rows(cp);
        if (glyph == NULL) {
            glyph = font8x16['?' - FONT_FIRST];
        }

        int32_t col0 = x < 0 ? -x : 0;
        int32_t col1 = FONT_WIDTH;
        if (x + col1 > (int32_t)win->w) {
            col1 = (int32_t)win->w - x;
        }
        /* The bits of the glyph row that survive the clip: bit for
         * column c is 0x80 >> c. */
        uint8_t keep = (uint8_t)((0xFFu >> col0) & (0xFFu << (8 - col1)));

        uint32_t *dst = win->pix + (uint64_t)(y + row0) * win->w + x;
        for (int32_t row = row0; row < row1; row++, dst += win->w) {
            if (opaque) {
                fill_span(dst + col0, bg, col1 - col0);
            }
            uint8_t bits = glyph[row] & keep;
            while (bits != 0) {
                int32_t col = __builtin_clz((uint32_t)bits) - 24;
                dst[col] = fg;
                bits &= (uint8_t)~(0x80u >> col);
            }
        }
        x += FONT_WIDTH;
    }
}

/* Raw pixel upload: the client's own w*h ARGB array (the highX answer
 * to XPutImage). Only the visible part is read and stored - this is
 * how hxvideo delivers a frame and how tusDE paints its panel, so the
 * rectangle is clipped once and each row is then one copy loop with
 * no per-pixel bounds test. The top byte is dropped on the way in:
 * backing stores hold 0x00RRGGBB. */
void hxdraw_image(struct hxs_window *win, int32_t x, int32_t y,
                  uint32_t w, uint32_t h, const uint32_t *src) {
    if (win == NULL || win->pix == NULL || src == NULL || w == 0 || h == 0) {
        return;
    }
    int32_t row0 = y < 0 ? -y : 0;
    int32_t row1 = (int32_t)h;
    if (y + row1 > (int32_t)win->h) {
        row1 = (int32_t)win->h - y;
    }
    int32_t col0 = x < 0 ? -x : 0;
    int32_t col1 = (int32_t)w;
    if (x + col1 > (int32_t)win->w) {
        col1 = (int32_t)win->w - x;
    }
    if (row0 >= row1 || col0 >= col1) {
        return;
    }

    const uint32_t *s = src + (uint64_t)row0 * w + col0;
    uint32_t *dst = win->pix + (uint64_t)(y + row0) * win->w + (x + col0);
    for (int32_t row = row0; row < row1; row++) {
        copy_span_opaque(dst, s, col1 - col0);
        s += w;
        dst += win->w;
    }
}
