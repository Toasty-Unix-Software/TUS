/*
 * tusde - the TUS desktop environment
 *
 * tusWM is a tiling window manager driven from the keyboard; tusDE is
 * the other half of the same idea, driven from the mouse. It is a
 * plain highX client too - it registers with HX_OP_WM_REGISTER and
 * everything it does is a request any client could send - but where
 * tusWM asks the user to learn bindings, tusDE puts what it can do on
 * the screen: a wallpaper, a panel with a launcher, a task list and a
 * clock, title bars with buttons, drag to move, a corner grip to
 * resize, and edge snapping.
 *
 * Start it with `highx --de` (tusWM is `highx --wm`, still the
 * default).
 *
 * The mouse:
 *
 *   click a window          focus and raise it
 *   drag the title bar      move it; drop it against the top edge to
 *                           maximise, against a side to half-tile
 *   drag the corner grip    resize
 *   title bar buttons       minimise / maximise / close
 *   panel task button       focus, or minimise when already focused
 *   panel launcher          open the application menu
 *   right-click the desktop the same menu, wherever the pointer is
 *   panel power button      end the session
 *
 * Ctrl+Q still ends the session, because a desktop that can only be
 * left with the mouse is a desktop that traps a user whose mouse just
 * died.
 *
 * Drawing: the panel and the title bars are composed pixel by pixel
 * into one scratch buffer and uploaded with a single HX_OP_PUT_IMAGE,
 * which is what makes gradients affordable - a flat fill and a
 * gradient cost the same one request. Text goes on top through
 * HX_OP_DRAW_TEXT, because the font lives in the server.
 */

#include "highapi/highapi.h"
#include "lvgl_port/tus_lvgl.h"
#include "lvgl_port/tus_lvgl_font.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* ---- geometry ---- */

#define DE_MAX_CLIENTS 8

#define PANEL_H     44
#define TITLE_H     28
#define BORDER      2
#define GRIP        16

#define TITLE_TEXT_X  92 /* clears the three buttons, now at the left */
#define MIN_W       240
#define MIN_H       (TITLE_H + 90)
#define CASCADE     32

/* A thin macOS-style menu bar across the TOP, in addition to the
 * launcher/task/power bar (now styled as a "dock") along the bottom -
 * see ensure_topbar()/draw_topbar(). The clock lives here now, not on
 * the dock, which is also why panel_task_w() below no longer reserves
 * CLOCK_W of room for it - task buttons get that space back. */
#define TOPBAR_H    26

#define LAUNCH_W    84
#define TASK_W      190
#define TASK_GAP    6
#define POWER_W     36

#define MENU_W      272
#define MENU_ROW    34
#define MENU_PAD    10
#define MENU_MAX    16
#define NAME_MAX_E  40
#define PATH_MAX_E  96

/* ---- palette ----
 *
 * Monochrome (2026-08-23: "cyber gibi degil, siyah-beyaz olsun" - not
 * cyber-looking, black and white) - the wallpaper's two soft lights
 * and the dock/topbar's accent are all shades of grey now, no hue.
 * The one deliberate exception is COL_BTN_MIN/MAX/CLOSE: tests/
 * test_tusde.py locates the title bar buttons by searching for THEIR
 * specific colours (find_color(BTN_CLOSE) etc., used to know where to
 * click) rather than by fixed geometry the way tuswm's key/mouse
 * handling does - recolouring all three the same grey would have
 * broken that location strategy, a materially bigger and riskier
 * change than a chrome reskin, so they keep their red/amber/green. */
#define COL_WALL_TOP  0x000A0A0Au
#define COL_WALL_BOT  0x00181818u
#define COL_GLOW_A    0x00303030u /* soft light, top right */
#define COL_GLOW_B    0x00202020u /* soft light, bottom left */

#define COL_PANEL     0x00141414u
#define COL_PANEL_HI  0x00202020u
#define COL_PANEL_TOP 0x001A1A1Au
#define COL_EDGE      0x00303030u
/* Pure white, deliberately: the compositor's cursor body (kernel/
 * highx/compositor.c's CURSOR_BODY) is 0xF0F0F0, and tests/
 * test_tusde.py's colour searches run with a +-6 tolerance, so accent
 * and cursor need to stay unmistakably apart. */
#define COL_ACCENT    0x00FFFFFFu
#define COL_ACCENT_LO 0x00707070u

#define COL_TEXT      0x00EDEDEDu
#define COL_DIM       0x00868686u
#define COL_TITLE_ON  0x00202020u
#define COL_TITLE_OFF 0x00151515u

#define COL_BTN_MIN   0x00F0B45Cu
#define COL_BTN_MAX   0x005CD98Bu
#define COL_BTN_CLOSE 0x00F2606Bu

/* The chrome typeface: tusfont's own TrueType engine, the same bridge
 * (userspace/lvgl_port/tus_lvgl_font.c) tuswm.c uses - not LVGL's
 * bundled bitmap Montserrat. */
#define CHROME_FONT_PATH "/usr/share/fonts/OpenSans-Light.ttf"
#define CHROME_FONT_SIZE 13

/* Ctrl+letter arrives as the control character. */
#define KEY_QUIT 0x11 /* Ctrl+Q */

enum { DRAG_NONE = 0, DRAG_MOVE, DRAG_RESIZE };
enum { SNAP_NONE = 0, SNAP_MAX, SNAP_LEFT, SNAP_RIGHT };

struct declient {
    int          used;
    unsigned int win;    /* the application's window */
    unsigned int frame;  /* our title bar, above it */
    unsigned int grip;   /* the resize handle, over its bottom right */
    char         title[HX_TITLE_MAX];
    /* The outer box: the title bar's top left corner, the full width
     * and the full height including title bar and borders. */
    int          x, y, w, h;
    int          minimised;
    int          maximised;
    int          rx, ry, rw, rh; /* geometry to restore to */
    int          drawn_focused;  /* what the title bar currently shows */

    /* The title bar is its own small LVGL display (see ensure_deco())
     * flushed into `frame`, replacing the old shared-scratch-buffer
     * hglui_text() blit; the grip stays a plain highX window, drawn
     * with hx_fill()/hx_line() as before. */
    lv_display_t *deco_disp;
    lv_obj_t     *deco_bar;
    lv_obj_t     *deco_edge;
    lv_obj_t     *deco_dot[3];
    lv_obj_t     *deco_title;
    int           deco_w;
};

struct entry {
    char name[NAME_MAX_E];
    char path[PATH_MAX_E];
};

/* ---- state ---- */

static struct hx_display g_dpy;
static struct declient   g_clients[DE_MAX_CLIENTS];
static unsigned int      g_desktop;   /* the wallpaper window */
static unsigned int      g_panel;
static unsigned int      g_menu;
static unsigned int      g_focus;     /* focused application window */
static int               g_running = 1;

static unsigned int     *g_scratch;   /* screen_w * PANEL_H pixels */
static unsigned long     g_clock_secs = (unsigned long)-1;

static lv_font_t        *g_chrome_font;

/* The dock (the old "panel", now LVGL-rendered and genuinely
 * translucent - see dock_seed()/draw_dock()). One task slot per
 * client, indexed by the SAME slot index g_clients[] itself uses (not
 * by left-to-right visual position, which changes as clients come and
 * go) - a widget is shown/positioned/hidden, never recreated. */
static lv_display_t     *g_dock_disp;
static lv_obj_t         *g_dock_glass;   /* the whole bar's translucent plate */
static lv_obj_t         *g_dock_launch;  /* also carries the "tusDE" label */
static lv_obj_t         *g_dock_launch_label;
static lv_obj_t         *g_dock_power;
static lv_obj_t         *g_dock_power_label;
struct dock_task {
    lv_obj_t *bg;
    lv_obj_t *label;
    lv_obj_t *accent;
};
static struct dock_task  g_dock_task[DE_MAX_CLIENTS];

/* The new top menu bar - branding on the left, the live clock on the
 * right (moved off the dock, see TOPBAR_H's comment). */
static unsigned int      g_topbar;
static lv_display_t     *g_topbar_disp;
static lv_obj_t         *g_topbar_glass;
static lv_obj_t         *g_topbar_label;
static lv_obj_t         *g_topbar_clock;

static struct entry      g_entries[MENU_MAX];
static int               g_nentries;
static int               g_menu_open;
static int               g_menu_hover = -1;
static int               g_menu_x, g_menu_y, g_menu_h;

/* What the pointer is over in the panel: -1 nothing, -2 the launcher,
 * -3 the power button, >= 0 a task button. */
#define HOVER_NONE   (-1)
#define HOVER_LAUNCH (-2)
#define HOVER_POWER  (-3)
static int g_panel_hover = HOVER_NONE;

static struct {
    int              mode;
    struct declient *c;
    int              dx, dy;  /* pointer offset inside the window */
    int              snap;    /* the edge the drag is currently over */
} g_drag;

/* ---- colour helpers ---- */

static unsigned int mix(unsigned int a, unsigned int b, int t) {
    if (t < 0) {
        t = 0;
    }
    if (t > 255) {
        t = 255;
    }
    unsigned int r = (((a >> 16) & 0xFF) * (255 - t) + ((b >> 16) & 0xFF) * t) / 255;
    unsigned int g = (((a >> 8) & 0xFF) * (255 - t) + ((b >> 8) & 0xFF) * t) / 255;
    unsigned int bl = ((a & 0xFF) * (255 - t) + (b & 0xFF) * t) / 255;
    return (r << 16) | (g << 8) | bl;
}

/* ---- clients ---- */

static int client_count(void) {
    int n = 0;
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        if (g_clients[i].used) {
            n++;
        }
    }
    return n;
}

static struct declient *client_by_win(unsigned int win) {
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        if (g_clients[i].used && g_clients[i].win == win) {
            return &g_clients[i];
        }
    }
    return NULL;
}

static struct declient *client_by_frame(unsigned int frame) {
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        if (g_clients[i].used && g_clients[i].frame == frame) {
            return &g_clients[i];
        }
    }
    return NULL;
}

static struct declient *client_by_grip(unsigned int grip) {
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        if (g_clients[i].used && g_clients[i].grip == grip) {
            return &g_clients[i];
        }
    }
    return NULL;
}

/* The client a window belongs to, whether it is the application's own
 * window or one of the two we put around it. */
static struct declient *client_of(unsigned int win) {
    struct declient *c = client_by_win(win);
    if (c == NULL) {
        c = client_by_frame(win);
    }
    if (c == NULL) {
        c = client_by_grip(win);
    }
    return c;
}

static void work_area(int *x, int *y, int *w, int *h) {
    *x = 0;
    *y = TOPBAR_H;
    *w = (int)g_dpy.screen_w;
    *h = (int)g_dpy.screen_h - PANEL_H - TOPBAR_H;
}

/* ---- the wallpaper ----
 *
 * A vertical gradient with two soft lights in it, painted in bands
 * through the scratch buffer: one PUT_IMAGE per band, no per-pixel
 * requests, and the falloff is (1 - d^2/r^2) squared, which needs no
 * square root and lands close enough to a real glow. */

static unsigned int wallpaper_px(int x, int y, int w, int h) {
    unsigned int base = mix(COL_WALL_TOP, COL_WALL_BOT, y * 255 / (h - 1));

    long ra = (long)w * 3 / 5;
    long dxa = x - (long)w * 4 / 5;
    long dya = y - (long)h / 6;
    long da = dxa * dxa + dya * dya;
    if (da < ra * ra) {
        long t = 255 - da * 255 / (ra * ra);
        base = mix(base, COL_GLOW_A, (int)(t * t / 255 / 3));
    }

    long rb = (long)w / 2;
    long dxb = x - (long)w / 6;
    long dyb = y - (long)h;
    long db = dxb * dxb + dyb * dyb;
    if (db < rb * rb) {
        long t = 255 - db * 255 / (rb * rb);
        base = mix(base, COL_GLOW_B, (int)(t * t / 255 / 3));
    }
    return base;
}

static void draw_wallpaper(void) {
    int w = (int)g_dpy.screen_w;
    int h = (int)g_dpy.screen_h;
    if (g_desktop == 0 || g_scratch == NULL) {
        return;
    }

    for (int y0 = 0; y0 < h; y0 += PANEL_H) {
        int rows = h - y0 < PANEL_H ? h - y0 : PANEL_H;
        for (int y = 0; y < rows; y++) {
            unsigned int *row = g_scratch + (unsigned)y * w;
            for (int x = 0; x < w; x++) {
                row[x] = wallpaper_px(x, y0 + y, w, h);
            }
        }
        hx_image(g_desktop, 0, y0, (unsigned)w, (unsigned)rows, g_scratch);
    }
    hx_commit(g_desktop);
}

/* ---- the panel ----
 *
 * Composed into the scratch buffer as one strip: background gradient,
 * the accent hairline along the top, then a rounded-ish plate under
 * every button that is hovered or active. Text is drawn afterwards,
 * over the uploaded image. */

static unsigned int panel_bg(int y) {
    return y == 0 ? COL_ACCENT_LO
                  : mix(COL_PANEL_TOP, COL_PANEL, y * 255 / (PANEL_H - 1));
}

static int panel_task_w(int n) {
    if (n <= 0) {
        return 0;
    }
    /* The clock moved to the topbar (see TOPBAR_H) - no CLOCK_W term
     * to reserve here any more, so task buttons get that room back. */
    int room = (int)g_dpy.screen_w - LAUNCH_W - 24 - POWER_W - 24;
    int w = (room - (n - 1) * TASK_GAP) / n;
    if (w > TASK_W) {
        w = TASK_W;
    }
    return w;
}

static int panel_task_x(int index, int n) {
    return LAUNCH_W + 16 + index * (panel_task_w(n) + TASK_GAP);
}

/* The task button the pointer is over, or HOVER_* for the fixed
 * controls at either end of the panel. */
static int panel_hit(int x, int y) {
    if (y < 6 || y > PANEL_H - 6) {
        return HOVER_NONE;
    }
    if (x >= 8 && x < 8 + LAUNCH_W) {
        return HOVER_LAUNCH;
    }
    if (x >= (int)g_dpy.screen_w - POWER_W - 8) {
        return HOVER_POWER;
    }

    int n = client_count();
    int tw = panel_task_w(n);
    int index = 0;
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        if (!g_clients[i].used) {
            continue;
        }
        int bx = panel_task_x(index, n);
        if (x >= bx && x < bx + tw) {
            return i;
        }
        index++;
    }
    return HOVER_NONE;
}

/* "up %lu:%02lu[:%02lu]" without an RTC, "YYYY-MM-DD HH:MM" with one -
 * time() answers seconds since the epoch when TUS has read the CMOS
 * clock at boot, and that is a DATE, not a duration: printing it as
 * hours-since-midnight gave the old panel a five-digit hour. Shared
 * by the topbar's clock and the dock's old one (see draw_dock()). */
static void format_clock(char *buf, size_t n, unsigned long secs) {
    if (secs > 1000000000UL) {
        time_t t = (time_t)secs;
        struct tm *tm = gmtime(&t);
        if (tm != NULL) {
            snprintf(buf, n, "%04d-%02d-%02d %02d:%02d", tm->tm_year + 1900,
                    tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min);
        } else {
            snprintf(buf, n, "%lu", secs);
        }
    } else {
        unsigned long h = secs / 3600, m = (secs / 60) % 60, s = secs % 60;
        if (h > 0) {
            snprintf(buf, n, "up %lu:%02lu:%02lu", h, m, s);
        } else {
            snprintf(buf, n, "up %lu:%02lu", m, s);
        }
    }
}

/* ---- the dock (the old "panel") ----
 *
 * Real transparency, not a flat/gradient fill: highX's compositor has
 * no per-window alpha (every window is opaque, last-writer-wins - see
 * kernel/highx/compositor.c), so there is no "draw see-through and
 * let the server sort it out". Because the wallpaper is a pure
 * function of (x, y) - wallpaper_px() above, no framebuffer read
 * needed - the dock (and the topbar, below) can compute exactly what
 * is behind them and seed that into their own LVGL draw buffer
 * (tus_lvgl_seed()) before drawing their translucent chrome on top,
 * which is genuine alpha blending, just done by this client instead
 * of the server. A title bar could not do the same trick cheaply (it
 * sits over arbitrary, changing application content, not a pure
 * function), which is why title bars stay opaque. */
static void dock_seed(void) {
    int w = (int)g_dpy.screen_w;
    int y0 = (int)g_dpy.screen_h - PANEL_H;
    for (int row = 0; row < PANEL_H; row++) {
        unsigned int *dst = g_scratch + (size_t)row * (size_t)w;
        for (int x = 0; x < w; x++) {
            dst[x] = wallpaper_px(x, y0 + row, w, (int)g_dpy.screen_h);
        }
    }
    tus_lvgl_seed(g_dock_disp, g_scratch);
}

/* Builds the dock's widget tree exactly once (called from main()) -
 * everything after this only shows/hides/repositions/restyles what
 * is already there, the same discipline tuswm.c's ensure_deco() and
 * this file's own ensure_deco() (title bars) use. */
static void ensure_dock(void) {
    int w = (int)g_dpy.screen_w;
    g_dock_disp = tus_lvgl_create(g_panel, w, PANEL_H);
    if (g_dock_disp == NULL) {
        return;
    }
    lv_obj_t *scr = lv_display_get_screen_active(g_dock_disp);
    /* Transparent screen background: dock_seed()'s wallpaper pixels
     * are what shows through everywhere the glass plate below does
     * not cover them at full opacity. */
    lv_obj_set_style_bg_opa(scr, LV_OPA_TRANSP, 0);

    g_dock_glass = lv_obj_create(scr);
    lv_obj_remove_style_all(g_dock_glass);
    lv_obj_set_pos(g_dock_glass, 0, 0);
    lv_obj_set_size(g_dock_glass, w, PANEL_H);
    lv_obj_set_style_bg_grad_dir(g_dock_glass, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_bg_opa(g_dock_glass, LV_OPA_80, 0);

    g_dock_launch = lv_obj_create(scr);
    lv_obj_remove_style_all(g_dock_launch);
    lv_obj_set_pos(g_dock_launch, 8, 6);
    lv_obj_set_size(g_dock_launch, LAUNCH_W, PANEL_H - 12);
    lv_obj_set_style_radius(g_dock_launch, 8, 0);
    lv_obj_set_style_bg_opa(g_dock_launch, LV_OPA_COVER, 0);
    g_dock_launch_label = lv_label_create(scr);
    lv_obj_align_to(g_dock_launch_label, g_dock_launch, LV_ALIGN_LEFT_MID, 14, 0);
    lv_label_set_text(g_dock_launch_label, "tusDE");
    if (g_chrome_font != NULL) {
        lv_obj_set_style_text_font(g_dock_launch_label, g_chrome_font, 0);
    }

    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        struct dock_task *t = &g_dock_task[i];
        t->bg = lv_obj_create(scr);
        lv_obj_remove_style_all(t->bg);
        lv_obj_set_style_radius(t->bg, 8, 0);
        lv_obj_set_style_bg_opa(t->bg, LV_OPA_COVER, 0);
        lv_obj_add_flag(t->bg, LV_OBJ_FLAG_HIDDEN);

        t->label = lv_label_create(scr);
        lv_label_set_long_mode(t->label, LV_LABEL_LONG_MODE_CLIP);
        if (g_chrome_font != NULL) {
            lv_obj_set_style_text_font(t->label, g_chrome_font, 0);
        }
        lv_obj_add_flag(t->label, LV_OBJ_FLAG_HIDDEN);

        t->accent = lv_obj_create(scr);
        lv_obj_remove_style_all(t->accent);
        lv_obj_set_size(t->accent, 1, 2);
        lv_obj_set_style_bg_color(t->accent, lv_color_hex(COL_ACCENT), 0);
        lv_obj_set_style_bg_opa(t->accent, LV_OPA_COVER, 0);
        lv_obj_add_flag(t->accent, LV_OBJ_FLAG_HIDDEN);
    }

    g_dock_power = lv_obj_create(scr);
    lv_obj_remove_style_all(g_dock_power);
    lv_obj_set_pos(g_dock_power, w - POWER_W - 8, 6);
    lv_obj_set_size(g_dock_power, POWER_W, PANEL_H - 12);
    lv_obj_set_style_radius(g_dock_power, 8, 0);
    lv_obj_set_style_bg_opa(g_dock_power, LV_OPA_COVER, 0);
    g_dock_power_label = lv_label_create(scr);
    lv_label_set_text(g_dock_power_label, "x");
    if (g_chrome_font != NULL) {
        lv_obj_set_style_text_font(g_dock_power_label, g_chrome_font, 0);
    }
    lv_obj_align_to(g_dock_power_label, g_dock_power, LV_ALIGN_CENTER, 0, 0);
}

static void draw_dock(void) {
    if (g_dock_disp == NULL) {
        return;
    }
    dock_seed();

    lv_obj_set_style_bg_color(g_dock_glass, lv_color_hex(panel_bg(0)), 0);
    lv_obj_set_style_bg_grad_color(g_dock_glass, lv_color_hex(panel_bg(PANEL_H - 1)), 0);

    lv_obj_set_style_bg_color(g_dock_launch,
        lv_color_hex(g_menu_open ? COL_ACCENT
                                 : (g_panel_hover == HOVER_LAUNCH ? COL_PANEL_HI : COL_EDGE)), 0);
    lv_obj_set_style_text_color(g_dock_launch_label,
        lv_color_hex(g_menu_open ? COL_TITLE_ON : COL_ACCENT), 0);

    int n = client_count();
    int tw = panel_task_w(n);
    int index = 0;
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        struct declient *c = &g_clients[i];
        struct dock_task *t = &g_dock_task[i];
        if (!c->used || tw <= 40) {
            lv_obj_add_flag(t->bg, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(t->label, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(t->accent, LV_OBJ_FLAG_HIDDEN);
            continue;
        }
        int bx = panel_task_x(index, n);
        int focused = (c->win == g_focus && !c->minimised);

        lv_obj_clear_flag(t->bg, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(t->label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_pos(t->bg, bx, 6);
        lv_obj_set_size(t->bg, tw, PANEL_H - 12);
        lv_obj_set_style_bg_color(t->bg,
            lv_color_hex(focused ? COL_PANEL_HI : (g_panel_hover == i ? COL_EDGE : COL_PANEL)), 0);

        lv_obj_set_pos(t->label, bx + 14, 0);
        lv_obj_set_width(t->label, tw - 22);
        lv_obj_align(t->label, LV_ALIGN_LEFT_MID, bx + 14, 0);
        lv_label_set_text(t->label, c->title);
        lv_obj_set_style_text_color(t->label,
            lv_color_hex(c->minimised ? COL_DIM : (focused ? COL_TEXT : COL_DIM)), 0);

        if (focused) {
            lv_obj_clear_flag(t->accent, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_pos(t->accent, bx + 8, PANEL_H - 8);
            lv_obj_set_width(t->accent, tw - 16);
        } else {
            lv_obj_add_flag(t->accent, LV_OBJ_FLAG_HIDDEN);
        }
        index++;
    }

    lv_obj_set_style_bg_color(g_dock_power,
        lv_color_hex(g_panel_hover == HOVER_POWER ? COL_BTN_CLOSE : COL_EDGE), 0);
    lv_obj_set_style_text_color(g_dock_power_label,
        lv_color_hex(g_panel_hover == HOVER_POWER ? COL_TITLE_ON : COL_DIM), 0);

    tus_lvgl_tick();
}

/* ---- the top menu bar ----
 *
 * Branding on the left, the live clock on the right - see TOPBAR_H's
 * comment for why the clock moved up here instead of staying on the
 * dock. Real transparency against the wallpaper, same technique as
 * the dock (dock_seed()'s twin, at world y in [0, TOPBAR_H) instead
 * of [screen_h - PANEL_H, screen_h)). */
static void topbar_seed(void) {
    int w = (int)g_dpy.screen_w;
    for (int row = 0; row < TOPBAR_H; row++) {
        unsigned int *dst = g_scratch + (size_t)row * (size_t)w;
        for (int x = 0; x < w; x++) {
            dst[x] = wallpaper_px(x, row, w, (int)g_dpy.screen_h);
        }
    }
    tus_lvgl_seed(g_topbar_disp, g_scratch);
}

static void ensure_topbar(void) {
    int w = (int)g_dpy.screen_w;
    g_topbar_disp = tus_lvgl_create(g_topbar, w, TOPBAR_H);
    if (g_topbar_disp == NULL) {
        return;
    }
    lv_obj_t *scr = lv_display_get_screen_active(g_topbar_disp);
    lv_obj_set_style_bg_opa(scr, LV_OPA_TRANSP, 0);

    g_topbar_glass = lv_obj_create(scr);
    lv_obj_remove_style_all(g_topbar_glass);
    lv_obj_set_pos(g_topbar_glass, 0, 0);
    lv_obj_set_size(g_topbar_glass, w, TOPBAR_H);
    lv_obj_set_style_bg_color(g_topbar_glass, lv_color_hex(COL_PANEL), 0);
    lv_obj_set_style_bg_opa(g_topbar_glass, LV_OPA_80, 0);

    g_topbar_label = lv_label_create(scr);
    lv_label_set_text(g_topbar_label, "tusDE");
    lv_obj_set_style_text_color(g_topbar_label, lv_color_hex(COL_TEXT), 0);
    lv_obj_align(g_topbar_label, LV_ALIGN_LEFT_MID, 12, 0);
    if (g_chrome_font != NULL) {
        lv_obj_set_style_text_font(g_topbar_label, g_chrome_font, 0);
    }

    g_topbar_clock = lv_label_create(scr);
    lv_obj_set_style_text_color(g_topbar_clock, lv_color_hex(COL_DIM), 0);
    if (g_chrome_font != NULL) {
        lv_obj_set_style_text_font(g_topbar_clock, g_chrome_font, 0);
    }
}

static void draw_topbar(unsigned long secs) {
    if (g_topbar_disp == NULL) {
        return;
    }
    topbar_seed();

    char line[32];
    format_clock(line, sizeof(line), secs);
    lv_label_set_text(g_topbar_clock, line);
    lv_obj_align(g_topbar_clock, LV_ALIGN_RIGHT_MID, -12, 0);

    tus_lvgl_tick();
}

/* ---- the application menu ----
 *
 * The entries come from /etc/highx/menu ("Name:/path" per line), the
 * same file hxmenu reads, so a system has one list of applications
 * whichever session is running. */

static void add_entry(const char *name, const char *path) {
    if (g_nentries >= MENU_MAX) {
        return;
    }
    snprintf(g_entries[g_nentries].name, NAME_MAX_E, "%s", name);
    snprintf(g_entries[g_nentries].path, PATH_MAX_E, "%s", path);
    g_nentries++;
}

static void load_entries(void) {
    int fd = open("/etc/highx/menu", O_RDONLY);
    if (fd >= 0) {
        char buf[1024];
        int n = (int)read(fd, buf, sizeof(buf) - 1);
        close(fd);
        if (n > 0) {
            buf[n] = '\0';
            char *line = buf;
            while (line != NULL && *line != '\0') {
                char *nl = strchr(line, '\n');
                if (nl != NULL) {
                    *nl = '\0';
                }
                char *colon = strchr(line, ':');
                if (*line != '#' && colon != NULL) {
                    *colon = '\0';
                    add_entry(line, colon + 1);
                }
                line = nl != NULL ? nl + 1 : NULL;
            }
        }
    }
    if (g_nentries == 0) {
        add_entry("Terminal", "/bin/hxtsh");
        add_entry("Files", "/bin/hxfiles");
        add_entry("Clock", "/bin/hxclock");
        add_entry("Demo", "/bin/hxdemo");
    }
}

static void draw_menu(void) {
    if (g_menu == 0) {
        return;
    }
    hx_fill(g_menu, 0, 0, MENU_W, (unsigned)g_menu_h, COL_PANEL);
    hx_fill(g_menu, 0, 0, MENU_W, 2, COL_ACCENT);
    hx_text(g_menu, MENU_PAD + 4, 8, COL_DIM, COL_PANEL, 0, "applications");

    for (int i = 0; i < g_nentries; i++) {
        int y = MENU_PAD + 24 + i * MENU_ROW;
        unsigned int bg = i == g_menu_hover ? COL_PANEL_HI : COL_PANEL;
        hx_fill(g_menu, 4, y, MENU_W - 8, MENU_ROW - 2, bg);
        if (i == g_menu_hover) {
            hx_fill(g_menu, 4, y, 3, MENU_ROW - 2, COL_ACCENT);
        }
        hx_fill(g_menu, MENU_PAD + 4, y + 12, 8, 8,
                i == g_menu_hover ? COL_ACCENT : COL_ACCENT_LO);
        hx_text(g_menu, MENU_PAD + 22, y + 8,
                i == g_menu_hover ? COL_TEXT : COL_DIM, bg, 0,
                g_entries[i].name);
    }
    hx_commit(g_menu);
}

static void menu_close(void) {
    if (!g_menu_open) {
        return;
    }
    g_menu_open = 0;
    g_menu_hover = -1;
    hx_ungrab_pointer();
    hx_unmap(g_menu);
    draw_dock();
}

static void menu_show(int x, int y) {
    if (g_menu == 0) {
        return;
    }
    int maxx = (int)g_dpy.screen_w - MENU_W - 8;
    if (x > maxx) {
        x = maxx;
    }
    if (x < 8) {
        x = 8;
    }
    int maxy = (int)g_dpy.screen_h - PANEL_H - g_menu_h - 8;
    if (y > maxy) {
        y = maxy;
    }
    if (y < 8) {
        y = 8;
    }

    g_menu_x = x;
    g_menu_y = y;
    g_menu_open = 1;
    g_menu_hover = -1;
    hx_move(g_menu, x, y);
    hx_map(g_menu);
    hx_raise(g_menu);
    /* Holding the pointer means the click that dismisses the menu is a
     * click the menu sees, wherever on the screen it lands. */
    hx_grab_pointer(g_menu);
    draw_menu();
    draw_dock();
}

static int menu_row_at(int y) {
    int row = (y - MENU_PAD - 24) / MENU_ROW;
    if (y < MENU_PAD + 24 || row < 0 || row >= g_nentries) {
        return -1;
    }
    return row;
}

/* ---- frames ---- */

static void raise_client(struct declient *c) {
    hx_raise(c->frame);
    hx_raise(c->win);
    hx_raise(c->grip);
    hx_raise(g_panel);
    if (g_menu_open) {
        hx_raise(g_menu);
    }
}

/* (Re)create the title bar's own LVGL display for the current width,
 * or just resize it in place - the widget tree (gradient bar, top
 * edge line, three dots, title label) is built exactly once per
 * client and never torn down until unmanage(). */
static void ensure_deco(struct declient *c, int w) {
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
        lv_obj_set_size(c->deco_bar, w, TITLE_H);
        lv_obj_set_style_bg_opa(c->deco_bar, LV_OPA_COVER, 0);
        lv_obj_set_style_bg_grad_dir(c->deco_bar, LV_GRAD_DIR_VER, 0);

        /* A 1px top edge, drawn over the gradient bar rather than as
         * its first row, so it can be the accent colour (or a snap-
         * preview mix of it) independently of the gradient underneath. */
        c->deco_edge = lv_obj_create(scr);
        lv_obj_remove_style_all(c->deco_edge);
        lv_obj_set_pos(c->deco_edge, 0, 0);
        lv_obj_set_size(c->deco_edge, w, 1);
        lv_obj_set_style_bg_opa(c->deco_edge, LV_OPA_COVER, 0);

        /* Three round buttons, macOS order and side: close, minimise,
         * maximise, left to right, at the LEFT of the bar rather than
         * the right - the one detail that makes a title bar read as
         * "macOS style" at a glance instead of "a title bar with round
         * buttons". */
        for (int b = 0; b < 3; b++) {
            int cx = 20 + b * 22, cy = TITLE_H / 2;
            lv_obj_t *dot = lv_obj_create(scr);
            lv_obj_remove_style_all(dot);
            lv_obj_set_size(dot, 10, 10);
            lv_obj_set_pos(dot, cx - 5, cy - 5);
            lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
            c->deco_dot[b] = dot;
        }

        c->deco_title = lv_label_create(scr);
        lv_label_set_long_mode(c->deco_title, LV_LABEL_LONG_MODE_CLIP);
        lv_obj_set_width(c->deco_title, w - TITLE_TEXT_X - 8);
        lv_obj_align(c->deco_title, LV_ALIGN_LEFT_MID, TITLE_TEXT_X, 0);
        if (g_chrome_font != NULL) {
            lv_obj_set_style_text_font(c->deco_title, g_chrome_font, 0);
        }
        return;
    }
    if (c->deco_w != w) {
        tus_lvgl_resize(c->deco_disp, w, TITLE_H);
        c->deco_w = w;
        lv_obj_set_width(c->deco_bar, w);
        lv_obj_set_width(c->deco_edge, w);
        lv_obj_set_width(c->deco_title, w - TITLE_TEXT_X - 8);
    }
}

/* The title bar: a gradient plate, the three buttons on the left and
 * the title to their right. */
static void draw_frame(struct declient *c, int force) {
    int focused = (c->win == g_focus);
    if (!force && c->drawn_focused == focused) {
        return;
    }
    c->drawn_focused = focused;

    int w = c->w;
    if (w <= 0 || w > (int)g_dpy.screen_w) {
        return;
    }
    ensure_deco(c, w);
    if (c->deco_disp == NULL) {
        return; /* out of memory: no decoration this time, try again later */
    }

    int snapping = (g_drag.c == c && g_drag.snap != SNAP_NONE);
    unsigned int top = focused ? COL_TITLE_ON : COL_TITLE_OFF;
    if (snapping) {
        top = mix(top, COL_ACCENT, 90); /* letting go now will tile it */
    }
    unsigned int bottom = mix(top, COL_PANEL, 120);
    lv_obj_set_style_bg_color(c->deco_bar, lv_color_hex(top), 0);
    lv_obj_set_style_bg_grad_color(c->deco_bar, lv_color_hex(bottom), 0);
    lv_obj_set_style_bg_color(c->deco_edge,
        lv_color_hex((focused || snapping) ? COL_ACCENT : COL_EDGE), 0);

    static const unsigned int colors[3] = { COL_BTN_CLOSE, COL_BTN_MIN,
                                            COL_BTN_MAX };
    for (int b = 0; b < 3; b++) {
        unsigned int col = focused ? colors[b] : mix(colors[b], COL_TITLE_OFF, 150);
        lv_obj_set_style_bg_color(c->deco_dot[b], lv_color_hex(col), 0);
    }

    lv_label_set_text(c->deco_title, c->title);
    lv_obj_set_style_text_color(c->deco_title,
        lv_color_hex(focused ? COL_TEXT : COL_DIM), 0);

    /* HX_EV_EXPOSE and focus changes expect the bar to reach the
     * screen synchronously, the same as the old hx_image()+hx_commit()
     * pair did - flush right now instead of waiting for the main
     * loop's next tus_lvgl_tick(). No-op for every other live LVGL
     * display that isn't dirty right now. */
    tus_lvgl_tick();

    /* The grip is three diagonal ticks in the corner, and the server
     * border is the other half of the focus indicator. */
    hx_fill(c->grip, 0, 0, GRIP, GRIP, focused ? COL_TITLE_ON : COL_TITLE_OFF);
    for (int i = 0; i < 3; i++) {
        int o = 3 + i * 4;
        hx_line(c->grip, o, GRIP - 3, GRIP - 3, o,
                focused ? COL_ACCENT : COL_DIM);
    }
    hx_commit(c->grip);
    hx_set_border(c->win, BORDER, focused ? COL_ACCENT_LO : COL_EDGE);
}

/* The button under a point on the title bar: 0 close, 1 minimise,
 * 2 maximise, -1 the bar itself (which means "drag me"). Matches the
 * left-to-right order draw_frame() paints them in. */
static int frame_button_at(struct declient *c, int x, int y) {
    (void)c;
    if (y < 3 || y > TITLE_H - 3) {
        return -1;
    }
    for (int b = 0; b < 3; b++) {
        int cx = 20 + b * 22;
        if (x >= cx - 9 && x <= cx + 9) {
            return b;
        }
    }
    return -1;
}

/* ---- placing windows ---- */

static void place(struct declient *c, int x, int y, int w, int h) {
    if (w < MIN_W) {
        w = MIN_W;
    }
    if (h < MIN_H) {
        h = MIN_H;
    }
    if (w > (int)g_dpy.screen_w) {
        w = (int)g_dpy.screen_w;
    }
    if (h > (int)g_dpy.screen_h) {
        h = (int)g_dpy.screen_h;
    }

    int iw = w - 2 * BORDER;
    int ih = h - TITLE_H - 2 * BORDER;
    if (iw < 8) {
        iw = 8;
    }
    if (ih < 8) {
        ih = 8;
    }

    int resized = (w != c->w || h != c->h);
    c->x = x;
    c->y = y;
    c->w = w;
    c->h = h;

    /* Three windows make up one on screen - the title bar, the
     * application, the resize grip - and a drag moves all three every
     * time the pointer does. Batched, they land together instead of
     * chasing each other across the screen. */
    hx_batch_begin();
    hx_move_resize(c->frame, x, y, (unsigned)w, TITLE_H);
    hx_move_resize(c->win, x + BORDER, y + TITLE_H + BORDER, (unsigned)iw,
                   (unsigned)ih);
    hx_move(c->grip, x + w - BORDER - GRIP, y + h - BORDER - GRIP);
    hx_batch_end();
    if (resized) {
        draw_frame(c, 1);
    }
}

static void set_focus(unsigned int win) {
    if (g_focus == win) {
        return;
    }
    struct declient *old = client_by_win(g_focus);
    g_focus = win;
    if (win != 0) {
        hx_focus(win);
    }
    if (old != NULL) {
        draw_frame(old, 0);
    }
    struct declient *now = client_by_win(win);
    if (now != NULL) {
        draw_frame(now, 0);
    }
    draw_dock();
}

static void focus_topmost(void) {
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        if (g_clients[i].used && !g_clients[i].minimised) {
            set_focus(g_clients[i].win);
            return;
        }
    }
    set_focus(0);
}

static void activate(struct declient *c) {
    if (c->minimised) {
        c->minimised = 0;
        hx_map(c->frame);
        hx_map(c->win);
        hx_map(c->grip);
    }
    raise_client(c);
    set_focus(c->win);
    draw_dock();
}

static void minimise(struct declient *c) {
    if (c->minimised) {
        return;
    }
    c->minimised = 1;
    hx_unmap(c->grip);
    hx_unmap(c->win);
    hx_unmap(c->frame);
    if (g_focus == c->win) {
        g_focus = 0;
        focus_topmost();
    }
    draw_dock();
}

static void maximise(struct declient *c, int on) {
    if (on == c->maximised) {
        return;
    }
    int wx, wy, ww, wh;
    work_area(&wx, &wy, &ww, &wh);

    if (on) {
        c->rx = c->x;
        c->ry = c->y;
        c->rw = c->w;
        c->rh = c->h;
        c->maximised = 1;
        place(c, wx, wy, ww, wh);
    } else {
        c->maximised = 0;
        place(c, c->rx, c->ry, c->rw, c->rh);
    }
    raise_client(c);
}

/* Half-tile against a side - the other thing every desktop does when
 * a window is dropped on an edge. */
static void snap_side(struct declient *c, int left) {
    int wx, wy, ww, wh;
    work_area(&wx, &wy, &ww, &wh);
    if (!c->maximised) {
        c->rx = c->x;
        c->ry = c->y;
        c->rw = c->w;
        c->rh = c->h;
    }
    c->maximised = 0;
    place(c, left ? wx : wx + ww / 2, wy, ww / 2, wh);
    raise_client(c);
}

/* ---- managing clients ---- */

static void launch(const char *path) {
    char *argv[2];
    argv[0] = (char *)path;
    argv[1] = 0;
    hx_spawn(path, argv);
}

static void manage(unsigned int win) {
    if (client_by_win(win) != NULL) {
        return;
    }
    struct hx_window_info info;
    if (hx_get_window(win, &info) < 0) {
        return;
    }
    /* A client that asked not to be decorated is mapped where it
     * wanted to be and otherwise left alone. */
    if ((info.flags & HX_WF_NODECOR) != 0) {
        hx_map(win);
        return;
    }

    struct declient *c = NULL;
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        if (!g_clients[i].used) {
            c = &g_clients[i];
            break;
        }
    }
    if (c == NULL) {
        return; /* out of slots: the window stays unmapped */
    }

    memset(c, 0, sizeof(*c));
    c->used = 1;
    c->win = win;
    c->drawn_focused = -1;
    snprintf(c->title, sizeof(c->title), "%s", info.title);

    int wx, wy, ww, wh;
    work_area(&wx, &wy, &ww, &wh);
    int w = (int)info.w + 2 * BORDER;
    int h = (int)info.h + TITLE_H + 2 * BORDER;
    if (w > ww - 40) {
        w = ww - 40;
    }
    if (h > wh - 40) {
        h = wh - 40;
    }
    /* Cascade from the top left, wrapping before the panel. */
    int step = (client_count() - 1) * CASCADE;
    int x = wx + 40 + step % (ww / 3);
    int y = wy + 30 + step % (wh / 4);

    c->frame = hx_create_window(x, y, (unsigned)(w > MIN_W ? w : MIN_W), TITLE_H,
                                HX_WF_FRAME, COL_TITLE_OFF, "titlebar");
    if (c->frame == 0) {
        c->used = 0;
        return;
    }
    c->grip = hx_create_window(x, y, GRIP, GRIP, HX_WF_FRAME, COL_TITLE_OFF,
                               "grip");
    if (c->grip == 0) {
        hx_destroy_window(c->frame);
        c->used = 0;
        return;
    }

    place(c, x, y, w, h);
    hx_map(c->frame);
    hx_map(c->win);   /* as the WM, our map is not redirected */
    hx_map(c->grip);
    draw_frame(c, 1);
    activate(c);
}

static void unmanage(unsigned int win) {
    struct declient *c = client_by_win(win);
    if (c == NULL) {
        return;
    }
    if (g_drag.c == c) {
        g_drag.mode = DRAG_NONE;
        g_drag.c = NULL;
    }
    hx_destroy_window(c->grip);
    hx_destroy_window(c->frame);
    if (c->deco_disp != NULL) {
        tus_lvgl_destroy(c->deco_disp);
    }
    memset(c, 0, sizeof(*c));

    if (g_focus == win) {
        g_focus = 0;
        focus_topmost();
    }
    draw_dock();
}

/* ---- the pointer ---- */

/* While a window is being dragged the edge it is over decides what
 * dropping it means; the title bar turns accent-coloured as a hint
 * that letting go now will snap it. */
static int snap_at(int sx, int sy) {
    if (sy <= TOPBAR_H + 2) {
        return SNAP_MAX;
    }
    if (sx <= 2) {
        return SNAP_LEFT;
    }
    if (sx >= (int)g_dpy.screen_w - 3) {
        return SNAP_RIGHT;
    }
    return SNAP_NONE;
}

static void drag_start(struct declient *c, int mode, int sx, int sy) {
    g_drag.mode = mode;
    g_drag.c = c;
    g_drag.dx = sx - c->x;
    g_drag.dy = sy - c->y;
    g_drag.snap = SNAP_NONE;
    raise_client(c);
    set_focus(c->win);
}

static void drag_motion(int sx, int sy) {
    struct declient *c = g_drag.c;
    if (c == NULL || !c->used) {
        g_drag.mode = DRAG_NONE;
        return;
    }

    if (g_drag.mode == DRAG_MOVE) {
        /* Dragging a maximised window restores it under the pointer,
         * the way a user expects to be able to "pull it down". */
        if (c->maximised) {
            c->maximised = 0;
            int nw = c->rw > 0 ? c->rw : c->w / 2;
            g_drag.dx = g_drag.dx * nw / (c->w > 0 ? c->w : nw);
            place(c, sx - g_drag.dx, sy - g_drag.dy, nw,
                  c->rh > 0 ? c->rh : c->h);
        }
        int y = sy - g_drag.dy;
        int wx, wy, ww, wh;
        work_area(&wx, &wy, &ww, &wh);
        if (y < wy) {
            y = wy;
        }
        if (y > wy + wh - TITLE_H) {
            y = wy + wh - TITLE_H;
        }
        place(c, sx - g_drag.dx, y, c->w, c->h);

        int snap = snap_at(sx, sy);
        if (snap != g_drag.snap) {
            g_drag.snap = snap;
            draw_frame(c, 1);
        }
    } else if (g_drag.mode == DRAG_RESIZE) {
        place(c, c->x, c->y, sx - c->x + GRIP / 2, sy - c->y + GRIP / 2);
    }
}

static void drag_end(int sx, int sy) {
    struct declient *c = g_drag.c;
    int mode = g_drag.mode;
    g_drag.mode = DRAG_NONE;
    g_drag.c = NULL;
    if (c == NULL || !c->used || mode != DRAG_MOVE) {
        return;
    }
    switch (snap_at(sx, sy)) {
    case SNAP_MAX:
        maximise(c, 1);
        break;
    case SNAP_LEFT:
        snap_side(c, 1);
        break;
    case SNAP_RIGHT:
        snap_side(c, 0);
        break;
    default:
        break;
    }
    g_drag.snap = SNAP_NONE;
    draw_dock();
}

static void panel_press(int x, int y) {
    int hit = panel_hit(x, y);
    if (hit == HOVER_LAUNCH) {
        if (g_menu_open) {
            menu_close();
        } else {
            menu_show(8, (int)g_dpy.screen_h - PANEL_H - g_menu_h - 6);
        }
        return;
    }
    if (hit == HOVER_POWER) {
        g_running = 0;
        return;
    }
    menu_close();
    if (hit >= 0 && g_clients[hit].used) {
        struct declient *c = &g_clients[hit];
        /* Clicking the focused task puts it away again - a taskbar
         * button is a toggle everywhere else, too. */
        if (c->win == g_focus && !c->minimised) {
            minimise(c);
        } else {
            activate(c);
        }
    }
}

static void on_pointer(const struct hx_event *ev) {
    int sx = (int)ev->w; /* the pointer's position on screen */
    int sy = (int)ev->h;

    /* The desktop has nothing to scroll, and the fall-through at the
     * bottom of this function treats anything that is not motion or a
     * release as a press - a wheel turn must not open the menu. */
    if (ev->detail == HX_PTR_WHEEL) {
        return;
    }

    if (ev->detail == HX_PTR_MOTION) {
        if (g_drag.mode != DRAG_NONE) {
            drag_motion(sx, sy);
            return;
        }
        if (ev->win == g_panel) {
            int hover = panel_hit(ev->x, ev->y);
            if (hover != g_panel_hover) {
                g_panel_hover = hover;
                draw_dock();
            }
        } else if (ev->win == g_menu && g_menu_open) {
            int row = (ev->x >= 0 && ev->x < MENU_W) ? menu_row_at(ev->y) : -1;
            if (row != g_menu_hover) {
                g_menu_hover = row;
                draw_menu();
            }
        } else if (g_panel_hover != HOVER_NONE) {
            g_panel_hover = HOVER_NONE;
            draw_dock();
        }
        return;
    }

    if (ev->detail == HX_PTR_RELEASE) {
        if (g_drag.mode != DRAG_NONE) {
            drag_end(sx, sy);
        }
        return;
    }

    /* A press. */
    if (ev->win == g_panel) {
        panel_press(ev->x, ev->y);
        return;
    }
    if (ev->win == g_menu && g_menu_open) {
        /* The grab routes every press here, so one outside the window
         * (negative or past the edge in local coordinates) is the
         * "click away to dismiss" every menu has. */
        int inside = ev->x >= 0 && ev->x < MENU_W && ev->y >= 0 &&
                     ev->y < g_menu_h;
        int row = inside ? menu_row_at(ev->y) : -1;
        if (row >= 0) {
            launch(g_entries[row].path);
        }
        menu_close();
        return;
    }
    if (ev->win == g_desktop) {
        if (ev->key == HX_BTN_RIGHT) {
            menu_show(sx, sy - g_menu_h);
        } else {
            menu_close();
        }
        return;
    }

    menu_close();

    struct declient *c = client_of(ev->win);
    if (c == NULL) {
        return;
    }
    if (ev->win == c->grip) {
        drag_start(c, DRAG_RESIZE, sx, sy);
        return;
    }
    if (ev->win == c->frame) {
        int button = frame_button_at(c, ev->x, ev->y);
        switch (button) {
        case 0:
            hx_close_window(c->win);
            return;
        case 1:
            minimise(c);
            return;
        case 2:
            maximise(c, !c->maximised);
            return;
        default:
            drag_start(c, DRAG_MOVE, sx, sy);
            return;
        }
    }
    /* A click on the application itself: focus and raise, nothing
     * more - the click belongs to the application. */
    activate(c);
}

/* ---- session ---- */

static void shutdown_session(void) {
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
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
    for (int i = 0; i < DE_MAX_CLIENTS; i++) {
        if (g_clients[i].used) {
            hx_destroy_window(g_clients[i].grip);
            hx_destroy_window(g_clients[i].frame);
            if (g_clients[i].deco_disp != NULL) {
                tus_lvgl_destroy(g_clients[i].deco_disp);
            }
            g_clients[i].used = 0;
        }
    }
    if (g_menu != 0) {
        hx_destroy_window(g_menu);
    }
    if (g_dock_disp != NULL) {
        tus_lvgl_destroy(g_dock_disp);
        g_dock_disp = NULL;
    }
    if (g_panel != 0) {
        hx_destroy_window(g_panel);
    }
    if (g_topbar_disp != NULL) {
        tus_lvgl_destroy(g_topbar_disp);
        g_topbar_disp = NULL;
    }
    if (g_topbar != 0) {
        hx_destroy_window(g_topbar);
    }
    if (g_desktop != 0) {
        hx_destroy_window(g_desktop);
    }
    if (g_chrome_font != NULL) {
        tus_lvgl_font_destroy(g_chrome_font);
        g_chrome_font = NULL;
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
        break;
    case HX_EV_POINTER:
    case HX_EV_POINTER_NOTIFY:
        on_pointer(ev);
        break;
    case HX_EV_KEY:
        if (ev->key == KEY_QUIT) {
            g_running = 0;
        }
        break;
    case HX_EV_FOCUS_IN:
        /* detail 1: the server telling us where focus went (an
         * override-redirect window can take it). */
        if (ev->detail == 1 && ev->win != g_focus) {
            g_focus = ev->win;
            for (int i = 0; i < DE_MAX_CLIENTS; i++) {
                if (g_clients[i].used) {
                    draw_frame(&g_clients[i], 0);
                }
            }
            draw_dock();
        }
        break;
    case HX_EV_EXPOSE:
        if (ev->win == g_desktop) {
            draw_wallpaper();
        } else if (ev->win == g_panel) {
            draw_dock();
        } else if (ev->win == g_topbar) {
            draw_topbar(g_clock_secs);
        } else if (ev->win == g_menu) {
            draw_menu();
        } else {
            struct declient *c = client_by_frame(ev->win);
            if (c != NULL) {
                draw_frame(c, 1);
            }
        }
        break;
    case HX_EV_CLOSE:
        g_running = 0;
        break;
    default:
        break;
    }
}

int main(void) {
    if (hx_open(&g_dpy) < 0) {
        printf("tusde: no highX display (start it with `highx --de`)\n");
        return 1;
    }
    if (hx_wm_register() < 0) {
        printf("tusde: another window manager is already running\n");
        hx_close(&g_dpy);
        return 1;
    }

    /* One scratch strip, wide as the screen: the panel, the title bars
     * and the wallpaper bands are all composed in it. */
    g_scratch = malloc((size_t)g_dpy.screen_w * PANEL_H * sizeof(unsigned int));
    if (g_scratch == NULL) {
        printf("tusde: out of memory\n");
        hx_close(&g_dpy);
        return 1;
    }

    load_entries();
    g_menu_h = MENU_PAD * 2 + 24 + g_nentries * MENU_ROW;

    /* Chrome text: tusfont, not LVGL's bundled bitmap font. A missing
     * or unreadable font file costs the chrome its text, not its
     * function - every drawing path above already checks for NULL. */
    g_chrome_font = tus_lvgl_font_create(CHROME_FONT_PATH, CHROME_FONT_SIZE);

    hx_grab_key(KEY_QUIT, HX_MOD_CTRL);
    hx_set_background(COL_WALL_TOP, HX_BG_SOLID);

    g_desktop = hx_create_window(0, 0, g_dpy.screen_w, g_dpy.screen_h,
                                 HX_WF_DESKTOP | HX_WF_NODECOR, COL_WALL_TOP,
                                 "desktop");
    if (g_desktop != 0) {
        hx_map(g_desktop);
        draw_wallpaper();
    }

    g_panel = hx_create_window(0, (int)g_dpy.screen_h - PANEL_H,
                               g_dpy.screen_w, PANEL_H,
                               HX_WF_NODECOR | HX_WF_OVERRIDE, COL_PANEL,
                               "tusDE panel");
    if (g_panel != 0) {
        hx_map(g_panel);
        ensure_dock();
        draw_dock();
    }

    g_topbar = hx_create_window(0, 0, g_dpy.screen_w, TOPBAR_H,
                                HX_WF_NODECOR | HX_WF_OVERRIDE, COL_PANEL,
                                "tusDE topbar");
    if (g_topbar != 0) {
        hx_map(g_topbar);
        ensure_topbar();
        g_clock_secs = (unsigned long)time(NULL);
        draw_topbar(g_clock_secs);
    }

    g_menu = hx_create_window(8, 8, MENU_W, (unsigned)g_menu_h,
                              HX_WF_NODECOR | HX_WF_OVERRIDE, COL_PANEL,
                              "tusDE menu");

    /* Start a terminal so the desktop is never empty. It is
     * hxtsh: the window is a view onto the kernel's own tsh. */
    launch("/bin/hxtsh");

    while (g_running) {
        struct hx_event ev;
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

        /* Motion over an application's window goes to the application,
         * not to us, so a pointer that left the panel that way would
         * leave a button lit. One query per idle pass settles it. */
        if (g_panel_hover != HOVER_NONE && g_drag.mode == DRAG_NONE) {
            struct hx_pointer ptr;
            if (hx_query_pointer(&ptr) >= 0 && ptr.win != g_panel) {
                g_panel_hover = HOVER_NONE;
                draw_dock();
            }
        }

        unsigned long now = (unsigned long)time(NULL);
        if (now != g_clock_secs) {
            g_clock_secs = now;
            draw_topbar(now);
        }

        /* Belt and suspenders: draw_frame() already flushes
         * synchronously on every state change, but this catches
         * anything a future change leaves merely invalidated. */
        tus_lvgl_tick();
    }

    shutdown_session();
    hx_close(&g_dpy);
    return 0;
}
