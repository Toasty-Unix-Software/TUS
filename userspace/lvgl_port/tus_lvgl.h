/*
 * tus_lvgl.h - LVGL brought up against highX windows
 *
 * The three things any LVGL port has to provide - a display driver
 * (a buffer plus a flush callback), an input device, and a tick
 * source - written against highAPI instead of a framebuffer, since
 * that is what a TUS GUI program actually has. See tus_lvgl.c for the
 * pixel-format and render-mode choices.
 *
 * Multi-instance: any number of tus_lvgl_create() displays can be
 * alive in one process at once, each bound to its own highX window.
 * A single-window app (lvgldemo) just creates one; tuswm/tusde create
 * one per decorated title bar (plus, in tusde's case, one for the
 * desktop layer) so each gets its own LVGL screen tree while sharing
 * one lv_timer_handler() pump. lv_init() and the tick source are set
 * up once, the first time any display is created.
 */

#ifndef TUS_LVGL_PORT_H
#define TUS_LVGL_PORT_H

#include "highapi/highapi.h"
#include "lvgl.h"

/* Create the display and pointer input device for `win`. `win` must
 * already be created (hx_create_window()) but need not be mapped yet.
 * Returns NULL if the draw buffer could not be allocated. */
lv_display_t *tus_lvgl_create(unsigned int win, int w, int h);

/* A tiling window manager resizes a window right after mapping it (to
 * fit the tile it was assigned) and again on every relayout - highX
 * tells the client with HX_EV_CONFIGURE. Call this from that event:
 * it replaces the draw buffer and tells LVGL the display is now
 * w x h, then invalidates the whole screen so the next tus_lvgl_tick()
 * repaints it. Without this, the window's one and only frame (drawn
 * at the original create size) is silently discarded the moment the
 * WM's resize reallocates the window's backing store server-side -
 * the window is left showing the server's default fill forever. */
int tus_lvgl_resize(lv_display_t *disp, int w, int h);

/* Feed one pointer sample for this display - call for every
 * HX_EV_POINTER (and HX_EV_POINTER_NOTIFY, if the window wants motion
 * while a grab holds it elsewhere) your event loop sees for the
 * window `disp` was created against. `x`/`y` are window-local,
 * matching hx_event.x/.y. */
void tus_lvgl_feed_pointer(lv_display_t *disp, int x, int y, int pressed);

/* Copy exactly (this display's current w * h) ARGB pixels straight
 * into its draw buffer, bypassing LVGL's own rendering entirely - the
 * way to give a "transparent" surface its backdrop. highX has no
 * per-window alpha (every window is opaque, last-writer-wins - see
 * kernel/highx/compositor.c), so real transparency against whatever
 * is actually behind a chrome surface (the desktop wallpaper, a grid
 * pattern) has to be faked by seeding that real backdrop into this
 * buffer BEFORE any semi-opaque LVGL object draws over it - which,
 * because LV_DISPLAY_RENDER_MODE_FULL blends into whatever bytes are
 * already sitting in the buffer, then looks and behaves exactly like
 * real alpha compositing. Call this, then lv_obj_invalidate() the
 * object(s) that should redraw against the fresh backdrop (a screen
 * whose own background is LV_OPA_TRANSP so it does not clobber the
 * seed) - every seed must be followed by a redraw of the same frame,
 * or a later unrelated repaint will blend its own semi-opaque colour
 * on top of whatever backdrop happens to still be sitting there. */
void tus_lvgl_seed(lv_display_t *disp, const uint32_t *pixels);

/* Drive every live display's timers/animations and flush anything
 * dirty to its window. Call this once per pass through your event
 * loop - one call services every tus_lvgl_create() display alive in
 * the process, the same way a single lv_timer_handler() call always
 * has in any LVGL port with more than one display. */
void tus_lvgl_tick(void);

/* Release one display's draw buffer and its display/indev objects.
 * Other live displays are unaffected. */
void tus_lvgl_destroy(lv_display_t *disp);

#endif /* TUS_LVGL_PORT_H */
