/*
 * highapi.h - highAPI, the highX client library
 *
 * highAPI is to highX what Xlib is to the X protocol: a thin C
 * binding over the wire format in <highx.h>. Every call builds one
 * protocol request, traps into the display server with SYS_HIGHX and
 * hands back the result - there is no client-side state beyond the
 * display handle filled in by hx_open().
 *
 * A minimal client:
 *
 *     struct hx_display dpy;
 *     hx_open(&dpy);
 *     unsigned win = hx_create_window(0, 0, 320, 200, 0, 0x202830,
 *                                     "hello");
 *     hx_map(win);
 *     for (;;) {
 *         struct hx_event ev;
 *         if (hx_next_event(&ev, -1) <= 0) continue;
 *         if (ev.type == HX_EV_CLOSE) break;
 *         if (ev.type == HX_EV_EXPOSE) {
 *             hx_text(win, 8, 8, 0xF0F0F0, 0, 0, "hello, highX");
 *             hx_commit(win);
 *         }
 *     }
 *     hx_close(&dpy);
 *
 * Every function returns 0 (or a positive value) on success and a
 * negative errno on failure, except hx_create_window(), which returns
 * the window id or 0.
 */

#ifndef HIGHAPI_H
#define HIGHAPI_H

#include <highx.h>

/* Glyph metrics of the built-in 8x16 font, so a client can size its
 * windows in characters. */
#define HX_FONT_W 8
#define HX_FONT_H 16

struct hx_display {
    unsigned int screen_w;
    unsigned int screen_h;
    unsigned int bpp;
    unsigned int client_id;   /* the client's pid, as the server sees it */
    unsigned int has_wm;      /* a window manager was registered */
    int          connected;
};

/* ---- connection ---- */

int  hx_open(struct hx_display *dpy);
void hx_close(struct hx_display *dpy);

/* ---- windows ---- */

unsigned int hx_create_window(int x, int y, unsigned int w, unsigned int h,
                              unsigned int flags, unsigned int background,
                              const char *title);
int hx_destroy_window(unsigned int win);
int hx_map(unsigned int win);
int hx_unmap(unsigned int win);
int hx_move(unsigned int win, int x, int y);
int hx_resize(unsigned int win, unsigned int w, unsigned int h);
int hx_move_resize(unsigned int win, int x, int y, unsigned int w,
                   unsigned int h);
int hx_raise(unsigned int win);
int hx_lower(unsigned int win);
int hx_set_title(unsigned int win, const char *title);
int hx_focus(unsigned int win);
int hx_close_window(unsigned int win); /* ask the owner to shut it down */

/*
 * Hold the screen still for a group of requests. Moving a window
 * means moving its frame too, and painting each one as it arrives
 * shows the title bar a step ahead of the contents. Wrap the pair:
 *
 *     hx_batch_begin();
 *     hx_move_resize(frame, ...);
 *     hx_move_resize(win, ...);
 *     hx_batch_end();
 *
 * The server puts the whole group on screen in one pass. Forgetting
 * the end is not fatal - waiting for the next event closes it - but
 * it gives back the extra repaint the batch was there to avoid.
 */
int hx_batch_begin(void);
int hx_batch_end(void);

/* ---- drawing (window-local coordinates) ---- */

int hx_fill(unsigned int win, int x, int y, unsigned int w, unsigned int h,
            unsigned int color);
int hx_rect(unsigned int win, int x, int y, unsigned int w, unsigned int h,
            unsigned int color);
int hx_line(unsigned int win, int x0, int y0, int x1, int y1,
            unsigned int color);
int hx_text(unsigned int win, int x, int y, unsigned int fg, unsigned int bg,
            unsigned int flags, const char *text);
int hx_image(unsigned int win, int x, int y, unsigned int w, unsigned int h,
             const unsigned int *pixels);
int hx_clear(unsigned int win, unsigned int color);

/* Push drawn content to the screen. */
int hx_commit(unsigned int win);
int hx_commit_rect(unsigned int win, int x, int y, unsigned int w,
                   unsigned int h);

/* ---- events ---- */

/* timeout_ms < 0 blocks, 0 polls, > 0 waits at most that long.
 * Returns 1 when an event was written, 0 on timeout, < 0 on error. */
int hx_next_event(struct hx_event *ev, int timeout_ms);
int hx_wait_event(struct hx_event *ev);
int hx_poll_event(struct hx_event *ev);

/* ---- window management ---- */

int hx_wm_register(void);

/* Route `key` with exactly these modifiers (HX_MOD_*) to this client
 * whatever has focus. Only the window manager's grabs are consulted. */
int hx_grab_key(unsigned int key, unsigned int mods);
int hx_query_tree(struct hx_tree *tree);
int hx_get_window(unsigned int win, struct hx_window_info *info);
int hx_screen_info(struct hx_screen_info *info);
int hx_set_background(unsigned int color, unsigned int style);

/* Server-drawn border around a window (outside its drawing area) -
 * a window manager's cheapest focus indicator. */
int hx_set_border(unsigned int win, unsigned int width, unsigned int color);

/* ---- the pointer (v1.2) ---- */

/* Where the cursor is, which buttons are down and the window under it
 * (0 = the bare desktop). */
int hx_query_pointer(struct hx_pointer *out);

/* Send every pointer event to `win` until the grab is released - what
 * a menu needs so the click that dismisses it is a click it sees.
 * Only windows this client may touch can hold the pointer. */
int hx_grab_pointer(unsigned int win);
int hx_ungrab_pointer(void);

/* Put the cursor somewhere (screen coordinates, clamped). */
int hx_warp_pointer(int x, int y);

/* ---- process helper ---- */

/* Start `path` as a new task and return its pid (the caller keeps
 * running). This is SYS_SPAWN, not part of the highX protocol, but
 * every window manager needs it to launch applications. */
int hx_spawn(const char *path, char *const argv[]);

#endif /* HIGHAPI_H */
