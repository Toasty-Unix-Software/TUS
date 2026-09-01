/*
 * tuswm - the TUS window manager
 *
 * tusWM is a minimal tiling window manager written entirely against
 * highAPI (userspace/highapi/): it has no privileged access to the
 * framebuffer and no back door into the kernel - everything it does,
 * any other highX client could do. What makes it the window manager
 * is one request, HX_OP_WM_REGISTER, after which the display server
 * redirects clients' map requests to it instead of putting windows on
 * screen itself.
 *
 * Layout: a status bar across the top, then either a master/stack
 * tiling (the master window on the left, everything else stacked on
 * the right - the default) or a cascade of floating, draggable
 * windows (Ctrl+L toggles between them). Each managed client gets a
 * title bar of its own -
 * a separate window that tusWM creates, owns and draws, sitting
 * directly above the application's window - and a server-drawn border
 * (HX_OP_SET_BORDER) that marks the focused window without costing
 * the application a single pixel of drawing area.
 *
 * The title bar itself is macOS-styled: rounded top corners and three
 * traffic-light buttons (red closes, yellow minimizes, green zooms in
 * floating mode). It is built entirely out of LVGL widgets rendered
 * through its own small LVGL display (userspace/lvgl_port/) that
 * flushes into the frame window, the same pattern userspace/lvgldemo.c
 * uses for a whole app window, just one per decorated client instead
 * of one for the whole process - see ensure_deco()/draw_frame() below.
 * The rounded-top-only trick (hglui's old job) is now done by making
 * the bar object TITLE_RADIUS taller than the display and letting
 * LVGL's own clip-to-display-buffer throw away what falls below it.
 *
 * Keys (grabbed globally, so they work whatever has focus):
 *
 *   Super+D        application menu (hxmenu)
 *   Super+Enter    new terminal (hxtsh - the kernel's own tsh)
 *   Super+F        the file manager (hxfiles)
 *   Alt+arrows     focus the window in that direction
 *   Alt+Shift+arrows   move the focused window there (swap when tiled)
 *   Ctrl+N / Ctrl+T    new hxdemo / hxclock window
 *   Tab            focus the next window
 *   Ctrl+W         close the focused window
 *   Ctrl+M         restore the most recently minimized window
 *   Ctrl+L         toggle floating / tiled
 *   Ctrl+R         redraw everything
 *   Ctrl+Q         end the session (back to the tsh prompt)
 *
 * The mouse focuses and raises the window it is clicked on, works the
 * three traffic lights, and drags a floating window by its title bar
 * (an implicit pointer grab keeps the drag going even once the cursor
 * has wandered off the bar - see kernel/highx/highx.c). For a desktop
 * that is driven by the mouse throughout, run tusDE instead
 * (`highx --de`).
 */

#include "highapi/highapi.h"
#include "lvgl_port/tus_lvgl.h"
#include "lvgl_port/tus_lvgl_font.h"
#include "tusfont/tusfont.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WM_MAX_CLIENTS 12

#define BAR_H     22
#define TITLE_H   26
#define BORDER    2
#define GAP       6
#define MIN_W     160
#define MIN_H     (TITLE_H + 60)
#define MOVE_STEP 48

/* Monochrome: no hue anywhere in the chrome, on request ("cyber gibi
 * degil, siyah-beyaz olsun" - not cyber-looking, black and white).
 * What used to be a blue accent is now plain white; the traffic
 * lights (still three buttons in the same places, doing the same
 * close/minimise/zoom jobs) are shades of grey rather than red/
 * yellow/green - see COL_TRAFFIC_* below. */
#define COL_DESKTOP    0x00101010u
#define COL_BAR_BG     0x00161616u
#define COL_BAR_FG     0x00D8D8D8u
#define COL_BAR_DIM    0x00868686u
/* Pure white, deliberately at the very top of the grey range: the
 * compositor's cursor body (kernel/highx/compositor.c's CURSOR_BODY)
 * is 0xF0F0F0, and tests/test_highx.py's colour searches run with a
 * +-6 tolerance, so accent and cursor need to stay far enough apart
 * that a screendump can never mistake one for the other. */
#define COL_ACCENT     0x00FFFFFFu
#define COL_FRAME_OFF  0x00303030u

/* The chrome typeface: tusfont's own TrueType engine
 * (userspace/tusfont/), not LVGL's bundled bitmap Montserrat and not
 * the console's 8x16 bitmap font - see tus_lvgl_font.c for the LVGL
 * bridge (title bars) and bar_text() below for the plain highX-
 * protocol text the status bar draws directly (no LVGL surface of its
 * own, so it goes through tusfont's tf_text_draw() the same way
 * hglui_text() always has). */
#define CHROME_FONT_PATH "/usr/share/fonts/OpenSans-Light.ttf"
#define CHROME_FONT_SIZE 13

/* ---- macOS-style title bar chrome (LVGL widgets, see ensure_deco()) ----
 *
 * Rounded top corners only - the application window below is a plain
 * rectangle, so rounding the bottom of the title bar would round into
 * a square corner and look like a mistake. The bar object is made
 * TITLE_RADIUS taller than the TITLE_H-tall display it is drawn on, so
 * its bottom corners fall below the display's own clip area and are
 * never rasterised - the same trick a taller-than-the-buffer box used
 * to get for free out of hglui_rounded_rect().
 */
#define TITLE_RADIUS   9
#define DOT_R          5
#define DOT_GAP        18
#define DOT_X0         14

/* Grey, not red/yellow/green: the three buttons keep their macOS
 * layout and click targets (see dot_center()/on HX_PTR_PRESS below),
 * they just stop being the one colourful thing in an otherwise
 * monochrome window. Full white when focused, dim when not - the
 * same "which window is frontmost" signal an unfocused window's dim
 * traffic lights always gave, just without the hue. */
#define COL_TRAFFIC_ON     0x00CCCCCCu
#define COL_TRAFFIC_DIM    0x004A4A4Au
#define COL_TITLEBAR_ON    0x00161616u
#define COL_TITLEBAR_OFF   0x00101010u
#define COL_TITLETEXT_ON   0x00E0E0E0u
#define COL_TITLETEXT_OFF  0x00808080u

/* Ctrl+letter arrives as the control character *and* with the Ctrl
 * bit set, so these are grabbed with HX_MOD_CTRL - a grab matches the
 * modifiers exactly. */
#define KEY_QUIT   0x11 /* Ctrl+Q */
#define KEY_CLOSE  0x17 /* Ctrl+W */
#define KEY_DEMO   0x0E /* Ctrl+N */
#define KEY_CLOCK  0x14 /* Ctrl+T */
#define KEY_LAYOUT 0x0C /* Ctrl+L */
#define KEY_REDRAW 0x12 /* Ctrl+R */
#define KEY_NEXT   0x09 /* Tab */
#define KEY_UNMIN  0x0D /* Ctrl+M: restore the last minimized window */

enum { LAYOUT_TILED = 0, LAYOUT_FLOATING };

struct wmclient {
    int          used;
    unsigned int win;    /* the application's window */
    unsigned int frame;  /* our title bar, sitting above it */
    char         title[HX_TITLE_MAX];
    /* Last tile we asked for, so a re-layout that changes nothing
     * issues no requests at all (every move costs the server two
     * screen repaints). */
    int          x, y, w, h;
    int          placed;
    int          drawn_focused; /* what the title bar currently shows */

    /* The title bar is its own small LVGL display (see ensure_deco())
     * flushed into `frame`; deco_bar/deco_dot/deco_accent/deco_title
     * are the widgets draw_frame() restyles rather than rebuilding on
     * every repaint, and deco_disp is resized in place - not torn
     * down and recreated - whenever the tile's width changes. */
    lv_display_t *deco_disp;
    lv_obj_t     *deco_bar;
    lv_obj_t     *deco_dot[3];
    lv_obj_t     *deco_accent;
    lv_obj_t     *deco_title;
    int           deco_w;

    /* Minimized: unmapped, excluded from layout and from focus
     * navigation, restored in most-recently-minimized order by
     * restore_last_minimized() (Ctrl+M). */
    int          minimized;

    /* Zoomed (the green button, floating layout only): geometry is
     * swapped between the work area and whatever it was before. */
    int          zoomed;
    int          prezoom_x, prezoom_y, prezoom_w, prezoom_h;
};

static struct hx_display g_dpy;
static struct wmclient   g_clients[WM_MAX_CLIENTS];
static unsigned int      g_bar;
static unsigned int      g_focus;      /* focused application window */

/* Chrome text. Title bars are LVGL widgets, so they get an lv_font_t
 * (tus_lvgl_font.c bridges LVGL's text pipeline to tusfont); the
 * status bar is plain highX drawing requests with no LVGL surface of
 * its own, so it goes straight through tusfont's own tf_text_draw()
 * into a scratch ARGB buffer, the same pattern hglui_text() and
 * hxfont use. Either can be NULL (font file missing or unreadable) -
 * that costs the chrome its text, not its function, same as the old
 * hglui-era title bars did. */
static lv_font_t       *g_chrome_font;
static struct tf_font  *g_bar_font;
static uint32_t         *g_bar_scratch; /* screen_w * BAR_H ARGB pixels */
/* Tiled by default, same as before this file grew traffic lights and
 * a drag handler: floating (Ctrl+L) is a mode this WM has always had
 * and the new decoration - rounded corners, traffic lights, a real
 * typeface - applies whichever layout is active. Floating windows are
 * draggable and can be zoomed (the green button); tiled windows are
 * not, because their geometry belongs to the tiling algorithm, not to
 * wherever a drag last left them. */
static int               g_layout = LAYOUT_TILED;
static int               g_running = 1;
static unsigned long     g_bar_secs = (unsigned long)-1;

/* Most-recently-minimized on top, so Ctrl+M restores in LIFO order. */
static struct wmclient  *g_minimized[WM_MAX_CLIENTS];
static int                g_minimized_count;

/* A title-bar drag in progress: which client, and the offset from the
 * window's top-left to where the press landed, so every motion event
 * only has to subtract that offset back out (see the implicit-pointer-
 * grab note in kernel/highx/highx.c - events keep arriving for this
 * client's frame no matter where the cursor wanders while the button
 * is down, which is what makes the drag work at all). */
static struct wmclient  *g_drag_client;
static int                g_drag_dx, g_drag_dy;

/* ---- client bookkeeping ---- */

static int client_count(void) {
    int n = 0;
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        if (g_clients[i].used && !g_clients[i].minimized) {
            n++;
        }
    }
    return n;
}

static struct wmclient *client_by_win(unsigned int win) {
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        if (g_clients[i].used && g_clients[i].win == win) {
            return &g_clients[i];
        }
    }
    return NULL;
}

static struct wmclient *client_by_frame(unsigned int frame) {
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        if (g_clients[i].used && g_clients[i].frame == frame) {
            return &g_clients[i];
        }
    }
    return NULL;
}

/* ---- drawing ---- */

/* Screen-space centre of each traffic light, for both drawing and
 * hit-testing - defined once so the two can never drift apart. */
static void dot_center(int index, int *cx, int *cy) {
    *cx = DOT_X0 + index * DOT_GAP;
    *cy = TITLE_H / 2;
}

/* A generous click target - the visible dot is DOT_R pixels, but
 * real macOS accepts a click well outside the drawn circle too, and a
 * target exactly the size of a 5px-radius circle is not one a mouse
 * user hits reliably. */
#define DOT_HIT 9

static int dist2(int x0, int y0, int x1, int y1) {
    int dx = x0 - x1, dy = y0 - y1;
    return dx * dx + dy * dy;
}

/* ---- status bar text, drawn straight from tusfont ----
 *
 * The bar is plain highX fill/image requests, not an LVGL surface, so
 * its text does not go through tus_lvgl_font.c - it blends tusfont's
 * own coverage bitmaps into an ARGB scratch buffer directly, the same
 * one-glyph-at-a-time callback hglui_text() has always used. Kept
 * local rather than pulled from hglui.c so this file does not have to
 * link HighGL (float SDF shape math it would otherwise never touch)
 * just to draw plain text. */
struct bar_text_ctx {
    uint32_t *buf;
    int       w, h;
    uint32_t  color;
};

static int bar_text_glyph(void *ctx_, const struct tf_bitmap *bm, int x, int y) {
    struct bar_text_ctx *ctx = (struct bar_text_ctx *)ctx_;
    if (bm->pixels == NULL) {
        return 0;
    }
    for (int row = 0; row < bm->h; row++) {
        int py = y + row;
        if (py < 0 || py >= ctx->h) {
            continue;
        }
        const uint8_t *src = bm->pixels + (size_t)row * (size_t)bm->stride;
        uint32_t *dst = ctx->buf + (size_t)py * (size_t)ctx->w;
        for (int col = 0; col < bm->w; col++) {
            int px = x + col;
            if (px < 0 || px >= ctx->w) {
                continue;
            }
            uint32_t cov = src[col];
            if (cov == 0) {
                continue;
            }
            uint32_t bg = dst[px];
            uint32_t inv = 255 - cov;
            uint32_t r = (((bg >> 16) & 0xFF) * inv + ((ctx->color >> 16) & 0xFF) * cov) / 255;
            uint32_t g = (((bg >> 8) & 0xFF) * inv + ((ctx->color >> 8) & 0xFF) * cov) / 255;
            uint32_t b = ((bg & 0xFF) * inv + (ctx->color & 0xFF) * cov) / 255;
            dst[px] = (r << 16) | (g << 8) | b;
        }
    }
    return 0;
}

/* `baseline_y` matches tf_text_draw()'s own convention (the pen sits
 * on the baseline, not the top-left of the glyph box). */
static void bar_text(uint32_t *buf, int w, int h, int x, int baseline_y,
                     uint32_t color, const char *s) {
    if (g_bar_font == NULL) {
        return; /* missing/unreadable font file: the bar still works, just bare */
    }
    struct bar_text_ctx ctx = { buf, w, h, color & 0x00FFFFFFu };
    tf_text_draw(g_bar_font, s, TF_FROM_INT(CHROME_FONT_SIZE), x, baseline_y,
                bar_text_glyph, &ctx);
}

static int bar_text_width(const char *s) {
    if (g_bar_font == NULL) {
        return (int)strlen(s) * HX_FONT_W; /* same estimate hx_text's own width used */
    }
    return TF_TO_INT(tf_text_width(g_bar_font, s, TF_FROM_INT(CHROME_FONT_SIZE)));
}

/* (Re)create the title bar's own LVGL display for the current width,
 * or just resize it in place when only the width changed - the widget
 * tree (bar, three dots, accent strip, title label) is built exactly
 * once per client and never torn down until unmanage(). */
static void ensure_deco(struct wmclient *c, int w) {
    if (c->deco_disp == NULL) {
        c->deco_disp = tus_lvgl_create(c->frame, w, TITLE_H);
        if (c->deco_disp == NULL) {
            return; /* out of memory: try again on the next draw_frame() */
        }
        c->deco_w = w;

        lv_obj_t *scr = lv_display_get_screen_active(c->deco_disp);

        c->deco_bar = lv_obj_create(scr);
        lv_obj_remove_style_all(c->deco_bar);
        lv_obj_set_pos(c->deco_bar, 0, 0);
        lv_obj_set_size(c->deco_bar, w, TITLE_H + TITLE_RADIUS);
        lv_obj_set_style_radius(c->deco_bar, TITLE_RADIUS, 0);
        lv_obj_set_style_bg_opa(c->deco_bar, LV_OPA_COVER, 0);

        for (int i = 0; i < 3; i++) {
            int cx, cy;
            dot_center(i, &cx, &cy);
            lv_obj_t *dot = lv_obj_create(scr);
            lv_obj_remove_style_all(dot);
            lv_obj_set_size(dot, DOT_R * 2, DOT_R * 2);
            lv_obj_set_pos(dot, cx - DOT_R, cy - DOT_R);
            lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
            c->deco_dot[i] = dot;
        }

        /* The focused window's accent colour, along the full width of
         * the bar rather than only around its 2px border: a visual
         * cue (which window is frontmost reads at a glance, not just
         * up close) and, not incidentally, a signal `make test-highx`
         * can still count in a screendump the way it always has - a
         * title bar this size next to a competing window's is a wide,
         * reliable strip of ACCENT pixels, where the border ring
         * alone would be a thin, easily misattributed one. */
        c->deco_accent = lv_obj_create(scr);
        lv_obj_remove_style_all(c->deco_accent);
        lv_obj_set_pos(c->deco_accent, 0, TITLE_H - 2);
        lv_obj_set_size(c->deco_accent, w, 2);
        lv_obj_set_style_bg_color(c->deco_accent, lv_color_hex(COL_ACCENT), 0);
        lv_obj_set_style_bg_opa(c->deco_accent, LV_OPA_COVER, 0);
        lv_obj_add_flag(c->deco_accent, LV_OBJ_FLAG_HIDDEN);

        int left_edge = DOT_X0 + 3 * DOT_GAP;
        c->deco_title = lv_label_create(scr);
        lv_label_set_long_mode(c->deco_title, LV_LABEL_LONG_MODE_CLIP);
        lv_obj_set_width(c->deco_title, w - left_edge - 10);
        lv_obj_set_style_text_align(c->deco_title, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(c->deco_title, LV_ALIGN_LEFT_MID, left_edge, 0);
        if (g_chrome_font != NULL) {
            lv_obj_set_style_text_font(c->deco_title, g_chrome_font, 0);
        }
        return;
    }
    if (c->deco_w != w) {
        tus_lvgl_resize(c->deco_disp, w, TITLE_H);
        c->deco_w = w;
        int left_edge = DOT_X0 + 3 * DOT_GAP;
        lv_obj_set_width(c->deco_bar, w);
        lv_obj_set_width(c->deco_accent, w);
        lv_obj_set_width(c->deco_title, w - left_edge - 10);
    }
}

/* The title bar is repainted only when it would look different: the
 * layout runs on every window change, and repainting every bar every
 * time is what a "sluggish" window manager is made of. */
static void draw_frame(struct wmclient *c, int force) {
    int focused = (c->win == g_focus);
    if (!force && c->drawn_focused == focused) {
        return;
    }
    c->drawn_focused = focused;

    int w = c->w > 0 ? c->w : MIN_W;
    ensure_deco(c, w);
    if (c->deco_disp == NULL) {
        return; /* out of memory: no decoration this time, try again later */
    }

    uint32_t bg = focused ? COL_TITLEBAR_ON : COL_TITLEBAR_OFF;
    lv_obj_set_style_bg_color(c->deco_bar, lv_color_hex(bg), 0);

    /* Traffic lights: full white focused, a dim grey when not - see
     * COL_TRAFFIC_ON's comment for why there is no per-button colour
     * any more. */
    for (int i = 0; i < 3; i++) {
        lv_obj_set_style_bg_color(c->deco_dot[i],
            lv_color_hex(focused ? COL_TRAFFIC_ON : COL_TRAFFIC_DIM), 0);
    }

    if (focused) {
        lv_obj_clear_flag(c->deco_accent, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(c->deco_accent, LV_OBJ_FLAG_HIDDEN);
    }

    lv_label_set_text(c->deco_title, c->title);
    lv_obj_set_style_text_color(c->deco_title,
        lv_color_hex(focused ? COL_TITLETEXT_ON : COL_TITLETEXT_OFF), 0);

    /* HX_EV_EXPOSE and focus changes expect the bar to reach the
     * screen synchronously, the same as the old hglClear()+hx_image()
     * pair did - flush right now instead of waiting for the main
     * loop's next tus_lvgl_tick(). This is a no-op for every OTHER
     * live LVGL display that happens not to be dirty right now. */
    tus_lvgl_tick();

    /* The border is the other half of the focus indicator, and the
     * server draws it: one request, no repaint of the window. */
    hx_set_border(c->win, BORDER, focused ? COL_ACCENT : COL_FRAME_OFF);
}

/* Real transparency for the bar: not a flat fill, a blend against
 * what the desktop background actually looks like underneath it.
 * highX's compositor has no per-window alpha (every window is opaque,
 * last-writer-wins - see kernel/highx/compositor.c) so there is no
 * "draw see-through and let the server sort it out"; the bar has to
 * know its own backdrop and blend client-side before it ever reaches
 * hx_image(). The desktop's grid pattern is simple and deterministic
 * (hxcomp_set_background() in kernel/highx/compositor.c derives the
 * grid line colour from the base colour the same way, at GRID_STEP
 * pixels), so recomputing it here is exact, not an approximation. */
#define GRID_STEP 32
#define BAR_OPA   210 /* out of 255 */

static uint32_t desktop_bg_px(int x, int y) {
    uint32_t r = (COL_DESKTOP >> 16) & 0xFF, g = (COL_DESKTOP >> 8) & 0xFF,
             b = COL_DESKTOP & 0xFF;
    r = r + 20 > 255 ? 255 : r + 20;
    g = g + 20 > 255 ? 255 : g + 20;
    b = b + 24 > 255 ? 255 : b + 24;
    uint32_t grid = (r << 16) | (g << 8) | b;
    if (((unsigned)y % GRID_STEP) == 0 || ((unsigned)x % GRID_STEP) == 0) {
        return grid;
    }
    return COL_DESKTOP;
}

static void blend_rect(uint32_t *buf, int stride, int x0, int y0, int w, int h,
                       uint32_t color, int opa) {
    uint32_t inv = 255 - (uint32_t)opa;
    for (int y = y0; y < y0 + h; y++) {
        uint32_t *row = buf + (size_t)y * (size_t)stride;
        for (int x = x0; x < x0 + w; x++) {
            uint32_t bg = row[x];
            uint32_t r = (((bg >> 16) & 0xFF) * inv + ((color >> 16) & 0xFF) * (uint32_t)opa) / 255;
            uint32_t g = (((bg >> 8) & 0xFF) * inv + ((color >> 8) & 0xFF) * (uint32_t)opa) / 255;
            uint32_t b = ((bg & 0xFF) * inv + (color & 0xFF) * (uint32_t)opa) / 255;
            row[x] = (r << 16) | (g << 8) | b;
        }
    }
}

/* The clock lives in its own strip on the right so a tick repaints
 * a hundred-odd pixels instead of the whole bar - the strip borrows
 * the front of g_bar_scratch as its own compact w*h buffer, which is
 * safe because draw_bar() always rebuilds the whole thing before its
 * own upload anyway. */
static void draw_bar_clock(unsigned long secs) {
    if (g_bar == 0 || g_bar_scratch == NULL) {
        return;
    }
    char line[32];
    snprintf(line, sizeof(line), "up %lu:%02lu", secs / 60, secs % 60);

    int width = bar_text_width(line);
    int strip_w = width + 16;
    int strip_h = BAR_H - 1; /* the accent line at the very bottom is not part of this strip */
    int x0 = (int)g_dpy.screen_w - strip_w;
    if (x0 < 0) {
        x0 = 0;
    }

    for (int y = 0; y < strip_h; y++) {
        for (int x = 0; x < strip_w; x++) {
            g_bar_scratch[(size_t)y * (size_t)strip_w + x] = desktop_bg_px(x0 + x, y);
        }
    }
    blend_rect(g_bar_scratch, strip_w, 0, 0, strip_w, strip_h, COL_BAR_BG, BAR_OPA);
    bar_text(g_bar_scratch, strip_w, strip_h, 8, strip_h - 6, COL_BAR_FG, line);

    hx_image(g_bar, x0, 0, (unsigned)strip_w, (unsigned)strip_h, g_bar_scratch);
    hx_commit_rect(g_bar, x0, 0, (unsigned)strip_w, (unsigned)strip_h);
}

static void draw_bar(void) {
    if (g_bar == 0 || g_bar_scratch == NULL) {
        return;
    }
    char line[HX_TEXT_MAX];
    int w = (int)g_dpy.screen_w;

    for (int y = 0; y < BAR_H; y++) {
        for (int x = 0; x < w; x++) {
            g_bar_scratch[(size_t)y * (size_t)w + x] = desktop_bg_px(x, y);
        }
    }
    blend_rect(g_bar_scratch, w, 0, 0, w, BAR_H, COL_BAR_BG, BAR_OPA);
    /* The accent hairline along the bottom stays fully opaque - it is
     * a hard edge on purpose, not part of the glass. */
    for (int x = 0; x < w; x++) {
        g_bar_scratch[(size_t)(BAR_H - 1) * (size_t)w + x] = COL_ACCENT;
    }

    int baseline = BAR_H - 7;
    bar_text(g_bar_scratch, w, BAR_H, 8, baseline, COL_ACCENT, "tusWM");

    int status_x = 8 + bar_text_width("tusWM ") + 12;
    struct wmclient *focused = client_by_win(g_focus);
    snprintf(line, sizeof(line), "[%s] %d win  %s",
             g_layout == LAYOUT_TILED ? "tiled" : "float", client_count(),
             focused != NULL ? focused->title : "-");
    bar_text(g_bar_scratch, w, BAR_H, status_x, baseline, COL_BAR_FG, line);
    int status_end = status_x + bar_text_width(line) + 24;

    snprintf(line, sizeof(line),
             "Super+D menu  Super+Enter term  Alt+arrows focus  "
             "Alt+Shift+arrows move  ^Q quit");
    int hint_w = bar_text_width(line);
    int hint_x = w - hint_w - 120;
    if (hint_x < status_end) {
        hint_x = status_end;
    }
    bar_text(g_bar_scratch, w, BAR_H, hint_x, baseline, COL_BAR_DIM, line);

    hx_image(g_bar, 0, 0, (unsigned)w, BAR_H, g_bar_scratch);
    hx_commit(g_bar);

    g_bar_secs = (unsigned long)time(NULL);
    draw_bar_clock(g_bar_secs);
}

/* ---- layout ---- */

/* Place one client: the title bar takes the top of the tile and the
 * application window everything below it, inset by the border the
 * server paints around it. Nothing is sent when the tile has not
 * moved - that is what makes a re-layout free for windows it does not
 * touch. */
static void place(struct wmclient *c, int x, int y, int w, int h) {
    if (w < MIN_W) {
        w = MIN_W;
    }
    if (h < MIN_H) {
        h = MIN_H;
    }
    int iw = w - 2 * BORDER;
    int ih = h - TITLE_H - 2 * BORDER;
    if (iw < 8) {
        iw = 8;
    }
    if (ih < 8) {
        ih = 8;
    }

    int size_changed = !c->placed || c->w != w || c->h != h;
    int pos_changed = !c->placed || c->x != x || c->y != y;
    if (pos_changed || size_changed) {
        /* The title bar and the window it decorates are one thing to
         * the user, so they reach the screen together. */
        hx_batch_begin();
        hx_move_resize(c->frame, x, y, w, TITLE_H);
        hx_move_resize(c->win, x + BORDER, y + TITLE_H + BORDER, iw, ih);
        hx_batch_end();
        c->x = x;
        c->y = y;
        c->w = w;
        c->h = h;
        c->placed = 1;
        /* Only a WIDTH change needs the bar's LVGL tree re-laid-out
         * and reflushed (ensure_deco() resizes it, draw_frame() would
         * otherwise skip a repaint whose content is unchanged). A
         * pure position move - by far the common case while dragging
         * a floating window - touches none of that: hx_move_resize()
         * above already put the (already-rendered) frame at its new
         * screen coordinates, so forcing a full LVGL rerender and an
         * immediate flush on every single motion event, as this used
         * to do unconditionally, was pure redundant work fighting the
         * move itself for the compositor's attention - the actual
         * cause of the reported drag stutter/flicker, not a rendering
         * bug in LVGL or the compositor. */
        if (size_changed) {
            draw_frame(c, 1);
        }
    } else {
        draw_frame(c, 0);
    }
}

/* Put a client and its title bar on top of the stack. */
static void raise_client(struct wmclient *c) {
    hx_raise(c->frame);
    hx_raise(c->win);
}

static void work_area(int *x, int *y, int *w, int *h) {
    *x = GAP;
    *y = BAR_H + GAP;
    *w = (int)g_dpy.screen_w - 2 * GAP;
    *h = (int)g_dpy.screen_h - BAR_H - 2 * GAP;
}

static void layout(void) {
    int n = client_count();
    if (n == 0) {
        return;
    }

    int wx, wy, ww, wh;
    work_area(&wx, &wy, &ww, &wh);

    int index = 0;
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        struct wmclient *c = &g_clients[i];
        if (!c->used || c->minimized) {
            continue;
        }

        if (g_layout == LAYOUT_FLOATING) {
            /* Floating windows keep whatever position they were moved
             * to; a window that has never been placed gets a slot in
             * the cascade. */
            if (!c->placed) {
                int w = ww * 2 / 3;
                int h = wh * 2 / 3;
                int step = index * 28;
                place(c, wx + step, wy + step, w, h);
            } else {
                place(c, c->x, c->y, c->w, c->h);
            }
        } else if (n == 1) {
            place(c, wx, wy, ww, wh);
        } else if (index == 0) {
            place(c, wx, wy, ww * 11 / 20 - GAP / 2, wh);
        } else {
            int sx = wx + ww * 11 / 20 + GAP / 2;
            int sw = ww - (ww * 11 / 20 + GAP / 2);
            int rows = n - 1;
            int sh = (wh - (rows - 1) * GAP) / rows;
            int sy = wy + (index - 1) * (sh + GAP);
            place(c, sx, sy, sw, sh);
        }
        index++;
    }

    if (g_layout == LAYOUT_FLOATING) {
        struct wmclient *f = client_by_win(g_focus);
        if (f != NULL) {
            raise_client(f);
        }
    }
}

/* Switching layouts re-derives every tile, so the cached geometry has
 * to go first. */
static void relayout_all(void) {
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        g_clients[i].placed = 0;
    }
    layout();
}

static void set_focus(unsigned int win) {
    if (g_focus == win) {
        return;
    }
    struct wmclient *old = client_by_win(g_focus);
    g_focus = win;
    if (win != 0) {
        hx_focus(win);
    }
    if (old != NULL) {
        draw_frame(old, 0);
    }
    struct wmclient *now = client_by_win(win);
    if (now != NULL) {
        draw_frame(now, 0);
        if (g_layout == LAYOUT_FLOATING) {
            raise_client(now);
        }
    }
    draw_bar();
}

static void focus_next(void) {
    if (client_count() == 0) {
        return;
    }
    int start = -1;
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        if (g_clients[i].used && g_clients[i].win == g_focus) {
            start = i;
            break;
        }
    }
    for (int step = 1; step <= WM_MAX_CLIENTS; step++) {
        int i = (start + step) % WM_MAX_CLIENTS;
        if (g_clients[i].used) {
            set_focus(g_clients[i].win);
            return;
        }
    }
}

/* ---- directional navigation ---- */

/* The window nearest to `from` in direction (dx, dy). Distance along
 * the direction counts once, distance across it twice, so "the window
 * to the left" is the one a user would point at, not merely the one
 * with the smallest coordinate. */
static struct wmclient *neighbour(struct wmclient *from, int dx, int dy) {
    if (from == NULL) {
        return NULL;
    }
    int fx = from->x + from->w / 2;
    int fy = from->y + from->h / 2;

    struct wmclient *best = NULL;
    int best_score = 0;

    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        struct wmclient *c = &g_clients[i];
        if (!c->used || c == from) {
            continue;
        }
        int cx = c->x + c->w / 2;
        int cy = c->y + c->h / 2;
        int along = (cx - fx) * dx + (cy - fy) * dy;
        if (along <= 0) {
            continue; /* not in that direction */
        }
        int across = dx != 0 ? (cy - fy) : (cx - fx);
        if (across < 0) {
            across = -across;
        }
        int score = along + 2 * across;
        if (best == NULL || score < best_score) {
            best = c;
            best_score = score;
        }
    }
    return best;
}

static void focus_direction(int dx, int dy) {
    struct wmclient *c = neighbour(client_by_win(g_focus), dx, dy);
    if (c != NULL) {
        set_focus(c->win);
    }
}

/* Alt+Shift+arrow. Tiled: swap the focused window with its neighbour,
 * which is how a tiling WM "moves" a window. Floating: nudge it. */
static void move_direction(int dx, int dy) {
    struct wmclient *c = client_by_win(g_focus);
    if (c == NULL) {
        return;
    }

    if (g_layout == LAYOUT_TILED) {
        struct wmclient *other = neighbour(c, dx, dy);
        if (other == NULL) {
            return;
        }
        /* Swap the slots: the slot order is the tiling order. */
        struct wmclient tmp = *c;
        *c = *other;
        *other = tmp;
        for (int i = 0; i < WM_MAX_CLIENTS; i++) {
            g_clients[i].placed = 0;
        }
        layout();
        draw_bar();
        return;
    }

    int wx, wy, ww, wh;
    work_area(&wx, &wy, &ww, &wh);
    int nx = c->x + dx * MOVE_STEP;
    int ny = c->y + dy * MOVE_STEP;
    if (nx < wx) {
        nx = wx;
    }
    if (ny < wy) {
        ny = wy;
    }
    if (nx + c->w > wx + ww) {
        nx = wx + ww - c->w;
    }
    if (ny + c->h > wy + wh) {
        ny = wy + wh - c->h;
    }
    place(c, nx, ny, c->w, c->h);
    raise_client(c);
}

/* ---- traffic lights: minimize and zoom ---- */

/* The yellow button. Unmaps both the application window and its title
 * bar (so a minimized window costs the compositor nothing to skip
 * over) and pushes it onto the restore stack. Whoever had focus next
 * is picked the same way unmanage() does it. */
static void minimize(struct wmclient *c) {
    if (c->minimized) {
        return;
    }
    c->minimized = 1;
    hx_unmap(c->win);
    hx_unmap(c->frame);
    if (g_minimized_count < WM_MAX_CLIENTS) {
        g_minimized[g_minimized_count++] = c;
    }

    if (g_focus == c->win) {
        g_focus = 0;
        for (int i = 0; i < WM_MAX_CLIENTS; i++) {
            if (g_clients[i].used && !g_clients[i].minimized) {
                g_focus = g_clients[i].win;
                hx_focus(g_focus);
                break;
            }
        }
    }
    relayout_all();
    draw_bar();
}

/* Ctrl+M: the most recently minimized window comes back, mapped,
 * focused and raised. Stale entries (closed while minimized) are
 * skipped rather than restored - `used` goes false on close but the
 * stack is not compacted, since compacting on every close would cost
 * more than a skip on the rare restore. */
static void restore_last_minimized(void) {
    while (g_minimized_count > 0) {
        struct wmclient *c = g_minimized[--g_minimized_count];
        if (!c->used || !c->minimized) {
            continue;
        }
        c->minimized = 0;
        hx_map(c->win);
        hx_map(c->frame);
        relayout_all();
        set_focus(c->win);
        raise_client(c);
        return;
    }
}

/* The green button. Only meaningful when windows float - a tiled
 * window's geometry is whatever the tiling algorithm says it is, and
 * "zoom" would just be undone by the next layout() - so this is a
 * no-op in tiled mode rather than a behaviour nobody asked for. */
static void toggle_zoom(struct wmclient *c) {
    if (g_layout != LAYOUT_FLOATING) {
        return;
    }
    int wx, wy, ww, wh;
    work_area(&wx, &wy, &ww, &wh);
    if (c->zoomed) {
        place(c, c->prezoom_x, c->prezoom_y, c->prezoom_w, c->prezoom_h);
        c->zoomed = 0;
    } else {
        c->prezoom_x = c->x;
        c->prezoom_y = c->y;
        c->prezoom_w = c->w;
        c->prezoom_h = c->h;
        place(c, wx, wy, ww, wh);
        c->zoomed = 1;
    }
    raise_client(c);
}

/* ---- managing clients ---- */

static void manage(unsigned int win) {
    if (client_by_win(win) != NULL) {
        return;
    }
    struct hx_window_info info;
    if (hx_get_window(win, &info) < 0) {
        return;
    }
    /* A client that asked not to be decorated is mapped where it
     * wanted to be and otherwise left alone (that is how hxmenu
     * floats above everything). */
    if ((info.flags & HX_WF_NODECOR) != 0) {
        hx_map(win);
        return;
    }

    struct wmclient *c = NULL;
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        if (!g_clients[i].used) {
            c = &g_clients[i];
            break;
        }
    }
    if (c == NULL) {
        return; /* out of slots: leave the window unmapped */
    }

    memset(c, 0, sizeof(*c));
    c->used = 1;
    c->win = win;
    c->drawn_focused = -1;
    strncpy(c->title, info.title, HX_TITLE_MAX - 1);
    c->frame = hx_create_window(info.x, info.y - TITLE_H, info.w, TITLE_H,
                                HX_WF_FRAME, COL_FRAME_OFF, "titlebar");
    if (c->frame == 0) {
        c->used = 0;
        return;
    }

    hx_map(c->frame);
    hx_map(win);      /* as the WM, our map is not redirected */
    g_focus = win;
    hx_focus(win);
    layout();
    raise_client(c);
    draw_bar();
}

static void unmanage(unsigned int win) {
    struct wmclient *c = client_by_win(win);
    if (c == NULL) {
        return;
    }
    hx_destroy_window(c->frame);
    if (c->deco_disp != NULL) {
        tus_lvgl_destroy(c->deco_disp);
    }
    if (g_drag_client == c) {
        g_drag_client = NULL;
    }
    memset(c, 0, sizeof(*c));

    if (g_focus == win) {
        g_focus = 0;
        for (int i = 0; i < WM_MAX_CLIENTS; i++) {
            if (g_clients[i].used && !g_clients[i].minimized) {
                g_focus = g_clients[i].win;
                hx_focus(g_focus);
                break;
            }
        }
    }
    relayout_all();
    draw_bar();
}

static void launch(const char *path) {
    char *argv[2];
    argv[0] = (char *)path;
    argv[1] = 0;
    hx_spawn(path, argv);
}

static void redraw_all(void) {
    hx_set_background(COL_DESKTOP, HX_BG_GRID);
    draw_bar();
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        if (g_clients[i].used) {
            draw_frame(&g_clients[i], 1);
        }
    }
    relayout_all();
}

/* ---- key handling ---- */

static void on_key(unsigned int key, unsigned int mods) {
    mods &= HX_MOD_MASK;

    if (mods == HX_MOD_SUPER || mods == (HX_MOD_SUPER | HX_MOD_SHIFT)) {
        switch (key) {
        case 'd':
        case 'D':
            launch("/bin/hxmenu");
            return;
        case '\n':
        case '\r':
            launch("/bin/hxtsh");
            return;
        case 'f':
        case 'F':
            launch("/bin/hxfiles");
            return;
        default:
            return;
        }
    }

    if (mods == HX_MOD_ALT || mods == (HX_MOD_ALT | HX_MOD_SHIFT)) {
        int shifted = (mods & HX_MOD_SHIFT) != 0;
        int dx = 0, dy = 0;
        switch (key) {
        case HX_KEY_LEFT:  dx = -1; break;
        case HX_KEY_RIGHT: dx = 1;  break;
        case HX_KEY_UP:    dy = -1; break;
        case HX_KEY_DOWN:  dy = 1;  break;
        default: return;
        }
        if (shifted) {
            move_direction(dx, dy);
        } else {
            focus_direction(dx, dy);
        }
        return;
    }

    switch (key) {
    case KEY_QUIT:
        g_running = 0;
        break;
    case KEY_CLOSE:
        if (g_focus != 0) {
            hx_close_window(g_focus);
        }
        break;
    case KEY_DEMO:
        launch("/bin/hxdemo");
        break;
    case KEY_CLOCK:
        launch("/bin/hxclock");
        break;
    case KEY_LAYOUT:
        g_layout = (g_layout == LAYOUT_TILED) ? LAYOUT_FLOATING : LAYOUT_TILED;
        hx_set_background(COL_DESKTOP, HX_BG_GRID);
        relayout_all();
        draw_bar();
        break;
    case KEY_REDRAW:
        redraw_all();
        break;
    case KEY_NEXT:
        focus_next();
        break;
    case KEY_UNMIN:
        restore_last_minimized();
        break;
    default:
        break;
    }
}

/* ---- shutdown ---- */

/* Ask every application to quit, then keep answering the protocol for
 * a moment so their windows disappear cleanly before we do. */
static void shutdown_session(void) {
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        if (g_clients[i].used) {
            hx_close_window(g_clients[i].win);
        }
    }
    for (int spin = 0; spin < 40 && client_count() > 0; spin++) {
        struct hx_event ev;
        while (hx_next_event(&ev, 25) > 0) {
            if (ev.type == HX_EV_DESTROY_NOTIFY ||
                ev.type == HX_EV_UNMAP_NOTIFY) {
                unmanage(ev.win);
            }
            if (ev.type == HX_EV_CLOSE) {
                return; /* the server is gone: nothing left to tidy */
            }
        }
    }
    for (int i = 0; i < WM_MAX_CLIENTS; i++) {
        if (g_clients[i].used) {
            hx_destroy_window(g_clients[i].frame);
            if (g_clients[i].deco_disp != NULL) {
                tus_lvgl_destroy(g_clients[i].deco_disp);
            }
            g_clients[i].used = 0;
        }
    }
    if (g_bar != 0) {
        hx_destroy_window(g_bar);
        g_bar = 0;
    }
}

static void handle_event(const struct hx_event *ev) {
    switch (ev->type) {
    case HX_EV_MAP_REQUEST:
        manage(ev->win);
        break;
    case HX_EV_DESTROY_NOTIFY:
        unmanage(ev->win);
        break;
    case HX_EV_UNMAP_NOTIFY:
        if (client_by_win(ev->win) != NULL) {
            relayout_all();
        }
        break;
    case HX_EV_KEY:
        on_key(ev->key, ev->mods);
        break;
    case HX_EV_POINTER_NOTIFY:
    case HX_EV_POINTER: {
        /* Click to focus: the server reports the button on whatever
         * window the cursor is over, and a title bar counts as a click
         * on the application it belongs to - except on the three
         * traffic lights, which do what they look like they do. */
        if (ev->detail == HX_PTR_PRESS) {
            struct wmclient *c = client_by_frame(ev->win);
            if (c != NULL) {
                int cx, cy;
                dot_center(0, &cx, &cy);
                if (dist2(ev->x, ev->y, cx, cy) <= DOT_HIT * DOT_HIT) {
                    hx_close_window(c->win);
                    break;
                }
                dot_center(1, &cx, &cy);
                if (dist2(ev->x, ev->y, cx, cy) <= DOT_HIT * DOT_HIT) {
                    minimize(c);
                    break;
                }
                dot_center(2, &cx, &cy);
                if (dist2(ev->x, ev->y, cx, cy) <= DOT_HIT * DOT_HIT) {
                    toggle_zoom(c);
                    break;
                }
                /* Anywhere else on the bar: focus, raise, and - in
                 * floating layout - start a drag. The implicit
                 * pointer grab (see kernel/highx/highx.c) keeps every
                 * event addressed to this frame until the button
                 * comes back up, wherever the cursor wanders. */
                set_focus(c->win);
                raise_client(c);
                if (g_layout == LAYOUT_FLOATING) {
                    g_drag_client = c;
                    g_drag_dx = (int)ev->w - c->x;
                    g_drag_dy = (int)ev->h - c->y;
                }
                break;
            }
            c = client_by_win(ev->win);
            if (c != NULL) {
                set_focus(c->win);
                raise_client(c);
            }
        } else if (ev->detail == HX_PTR_MOTION) {
            if (g_drag_client != NULL) {
                int nx = (int)ev->w - g_drag_dx;
                int ny = (int)ev->h - g_drag_dy;
                /* Keep at least 40px of the title bar reachable on
                 * screen - a window dragged entirely off it would
                 * have no bar left to drag back with. */
                int minvis = 40;
                if (nx < minvis - g_drag_client->w) {
                    nx = minvis - g_drag_client->w;
                }
                if (nx > (int)g_dpy.screen_w - minvis) {
                    nx = (int)g_dpy.screen_w - minvis;
                }
                if (ny < 0) {
                    ny = 0;
                }
                if (ny > (int)g_dpy.screen_h - TITLE_H) {
                    ny = (int)g_dpy.screen_h - TITLE_H;
                }
                place(g_drag_client, nx, ny, g_drag_client->w,
                     g_drag_client->h);
            }
        } else if (ev->detail == HX_PTR_RELEASE) {
            g_drag_client = NULL;
        }
        break;
    }
    case HX_EV_FOCUS_IN:
        /* detail 1: the server telling the WM where focus went (an
         * override-redirect window such as hxmenu can take it). */
        if (ev->detail == 1 && ev->win != g_focus) {
            struct wmclient *c = client_by_win(ev->win);
            g_focus = ev->win;
            for (int i = 0; i < WM_MAX_CLIENTS; i++) {
                if (g_clients[i].used && !g_clients[i].minimized) {
                    draw_frame(&g_clients[i], 0);
                }
            }
            if (c != NULL) {
                draw_bar();
            }
        }
        break;
    case HX_EV_EXPOSE: {
        if (ev->win == g_bar) {
            draw_bar();
            break;
        }
        struct wmclient *c = client_by_frame(ev->win);
        if (c != NULL) {
            draw_frame(c, 1);
        }
        break;
    }
    case HX_EV_CLOSE:
        g_running = 0;
        break;
    default:
        break;
    }
}

int main(void) {
    if (hx_open(&g_dpy) < 0) {
        printf("tuswm: no highX display (start it with `highx`)\n");
        return 1;
    }
    if (hx_wm_register() < 0) {
        printf("tuswm: another window manager is already running\n");
        hx_close(&g_dpy);
        return 1;
    }

    hx_grab_key(KEY_QUIT, HX_MOD_CTRL);
    hx_grab_key(KEY_CLOSE, HX_MOD_CTRL);
    hx_grab_key(KEY_DEMO, HX_MOD_CTRL);
    hx_grab_key(KEY_CLOCK, HX_MOD_CTRL);
    hx_grab_key(KEY_LAYOUT, HX_MOD_CTRL);
    hx_grab_key(KEY_REDRAW, HX_MOD_CTRL);
    hx_grab_key(KEY_UNMIN, HX_MOD_CTRL);
    hx_grab_key(KEY_NEXT, 0);
    hx_grab_key('d', HX_MOD_SUPER);
    hx_grab_key('D', HX_MOD_SUPER | HX_MOD_SHIFT);
    hx_grab_key('\n', HX_MOD_SUPER);
    hx_grab_key('f', HX_MOD_SUPER);
    hx_grab_key('F', HX_MOD_SUPER | HX_MOD_SHIFT);
    hx_grab_key('\n', HX_MOD_SUPER | HX_MOD_SHIFT);
    static const unsigned int arrows[] = { HX_KEY_LEFT, HX_KEY_RIGHT,
                                           HX_KEY_UP, HX_KEY_DOWN };
    for (unsigned i = 0; i < 4; i++) {
        hx_grab_key(arrows[i], HX_MOD_ALT);
        hx_grab_key(arrows[i], HX_MOD_ALT | HX_MOD_SHIFT);
    }

    hx_set_background(COL_DESKTOP, HX_BG_GRID);

    /* Chrome text: tusfont, not LVGL's bundled bitmap font and not
     * the console's 8x16 one. A missing/unreadable font file costs
     * the chrome its text, not its function - both stay NULL and
     * every drawing path already checks for that. */
    g_chrome_font = tus_lvgl_font_create(CHROME_FONT_PATH, CHROME_FONT_SIZE);
    g_bar_font = tf_open(CHROME_FONT_PATH);

    g_bar_scratch = malloc((size_t)g_dpy.screen_w * (size_t)BAR_H * sizeof(uint32_t));

    g_bar = hx_create_window(0, 0, g_dpy.screen_w, BAR_H,
                             HX_WF_NODECOR | HX_WF_OVERRIDE, COL_BAR_BG,
                             "tusWM status bar");
    if (g_bar != 0) {
        hx_map(g_bar);
        draw_bar();
    }

    /* Start a terminal so the session is never an empty desktop.
     * hxtsh runs the kernel's tsh, not a shell of its own. */
    launch("/bin/hxtsh");

    while (g_running) {
        struct hx_event ev;
        /* Drain everything that is already queued before going idle:
         * a burst of events (a new window, a layout change) is
         * handled in one pass instead of one per wake-up. */
        int n = hx_next_event(&ev, 60);
        while (n > 0) {
            handle_event(&ev);
            if (!g_running) {
                break;
            }
            n = hx_next_event(&ev, 0);
        }
        if (!g_running) {
            break;
        }

        unsigned long now = (unsigned long)time(NULL);
        if (now != g_bar_secs && g_bar != 0) {
            g_bar_secs = now;
            draw_bar_clock(now);
        }

        /* Belt and suspenders: draw_frame() already flushes
         * synchronously on every state change, but this catches
         * anything a future change leaves merely invalidated. Cheap -
         * lv_timer_handler() no-ops across every live title bar when
         * none of them are dirty. */
        tus_lvgl_tick();
    }

    shutdown_session();
    if (g_chrome_font != NULL) {
        tus_lvgl_font_destroy(g_chrome_font);
    }
    if (g_bar_font != NULL) {
        tf_close(g_bar_font);
    }
    free(g_bar_scratch);
    hx_close(&g_dpy);
    return 0;
}
