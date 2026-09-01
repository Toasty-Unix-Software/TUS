/*
 * highx.h - highX display server (kernel side)
 *
 * highX is the TUS window system; the wire protocol lives in
 * include/highx.h and is shared verbatim with userspace. This header
 * is the kernel's own view: the server state, the entry point the
 * SYS_HIGHX dispatcher calls, and the hooks the rest of the kernel
 * needs (task exit, the `highx` shell command).
 *
 * The server is a compositor. Every window owns an ARGB backing store
 * in the kernel heap; clients draw into it with protocol requests and
 * ask for a commit, and the server paints the damaged screen region
 * from the window stack. While it runs, the framebuffer text console
 * is suspended (fb_set_graphics) and restored on exit, so the shell
 * comes back exactly as it was.
 */

#ifndef TUS_HIGHX_HIGHX_H
#define TUS_HIGHX_HIGHX_H

#include <stdbool.h>
#include <stdint.h>

#include <highx.h>

/* A server-side window: geometry, flags and the pixel backing store
 * (w*h uint32_t, 0x00RRGGBB, allocated with kmalloc). */
struct hxs_window {
    uint32_t id;      /* 0 = free slot */
    uint32_t owner;   /* client id (pid) that created it */
    int32_t  x, y;    /* screen coordinates of the top left corner */
    uint32_t w, h;
    uint32_t flags;   /* HX_WF_* */
    uint32_t border_w;     /* border painted outside the window rect */
    uint32_t border_color;
    bool     mapped;
    char     title[HX_TITLE_MAX];
    uint32_t *pix;
};

/* ---- server lifecycle (kernel/shell/commands.c) ---- */

/* Take over the screen: initialise the compositor, reset the window
 * and client tables and suspend the text console. Returns 0, or a
 * negative errno when there is no usable framebuffer. */
int highx_start(void);

/* Give the screen back to the text console and drop every window. */
void highx_stop(void);

/* Rebind a running session to a framebuffer whose geometry changed
 * (a res_set under a live session). Windows are moved back on screen,
 * the pointer is clamped, the screen is repainted and every mapped
 * window is exposed. Returns 0 (also when no session is running), or
 * -ENODEV if the new mode is one the compositor cannot paint. */
int highx_rebind(void);

/* True while the server owns the screen. */
bool highx_active(void);

/* Print the server state (the `highx info` shell command). */
void highx_print_state(void);

/* ---- protocol entry point (kernel/syscall/syscall.c) ---- */

/* Execute one protocol request. `arg` points at the request structure
 * in the caller's address space (already range-checked when the call
 * came from ring 3) and `len` is its size. Returns a non-negative
 * result or a negative errno. */
long highx_request(long op, void *arg, uint64_t len, bool from_user);

/* ---- hooks ---- */

/* A task died: destroy its windows and release its client slot
 * (called from task_exit, whether the task exited or crashed). */
void highx_client_exit(uint32_t pid);

/* ---- compositor (kernel/highx/compositor.c) ---- */

/* Latch the framebuffer geometry. Returns a negative errno if the
 * framebuffer is missing or not 32 bits per pixel. */
int hxcomp_init(void);
void hxcomp_screen(uint32_t *w, uint32_t *h, uint32_t *bpp);
void hxcomp_set_background(uint32_t color, uint32_t style);

/* The rectangle a window occupies on screen including its border -
 * the area the compositor paints and the one damage is computed
 * from. Any pointer may be NULL. */
void hxwin_outer(const struct hxs_window *win, int32_t *x, int32_t *y,
                 int32_t *w, int32_t *h);

/* ---- pointer ----
 *
 * The cursor is drawn by the compositor, on top of everything, at the
 * end of every paint that touches its rectangle. Nothing is saved or
 * restored: a repaint regenerates the pixels under the cursor from
 * the window stack anyway, so the sprite is simply applied last. */
void hxcomp_set_pointer(int32_t x, int32_t y);
void hxcomp_pointer_rect(int32_t *x, int32_t *y, int32_t *w, int32_t *h);
void hxcomp_show_pointer(bool visible);

/* Repaint the screen rectangle from the window stack (bottom first).
 * The rectangle is clipped to the screen; an empty one is ignored. */
void hxcomp_paint(int32_t x, int32_t y, int32_t w, int32_t h,
                  struct hxs_window **stack, int nstack);

/* ---- drawing into a window's backing store ---- */

void hxdraw_fill(struct hxs_window *win, int32_t x, int32_t y,
                 int32_t w, int32_t h, uint32_t color);
void hxdraw_rect(struct hxs_window *win, int32_t x, int32_t y,
                 int32_t w, int32_t h, uint32_t color);
void hxdraw_line(struct hxs_window *win, int32_t x0, int32_t y0,
                 int32_t x1, int32_t y1, uint32_t color);
void hxdraw_text(struct hxs_window *win, int32_t x, int32_t y,
                 uint32_t fg, uint32_t bg, uint32_t flags, const char *text);
void hxdraw_image(struct hxs_window *win, int32_t x, int32_t y,
                  uint32_t w, uint32_t h, const uint32_t *src);

#endif /* TUS_HIGHX_HIGHX_H */
