/*
 * fb.h - framebuffer text console
 *
 * Draws an 8x16 text grid over the framebuffer that Limine hands us.
 * This driver owns the raw pixel surface; a future /dev/fb0 device
 * will expose exactly this memory to userspace.
 */

#ifndef TUS_DRIVERS_FB_H
#define TUS_DRIVERS_FB_H

#include <stdbool.h>
#include <stdint.h>

#include <limine.h>

/* Initialize the console for the given framebuffer.
 * Returns 0 on success, -1 if the framebuffer is unusable. */
int fb_init(struct limine_framebuffer *fb);

/* Write one character at the cursor (handles \n \r \b \t). */
void fb_putchar(char c);

void fb_batch_begin(void);
void fb_batch_end(void);

/* Clear the screen and reset the cursor to the top-left corner. */
void fb_clear(void);

/* Scroll the visible window one page. dir > 0 looks back into the
 * scrollback history (PageUp), dir < 0 returns towards the live
 * output (PageDown). Any new output snaps back to live mode. */
void fb_scroll_page(int dir);

/* True while the window is scrolled back into history. */
bool fb_view_scrolled(void);

/* True while the cursor sits on the last column of a row owing a line
 * feed it has not taken yet (see the deferred-wrap note in fb.c). */
bool fb_wrap_pending(void);

/* Set the colors used for subsequently drawn text. */
void fb_set_color(uint32_t fg, uint32_t bg);

/* Fill the whole framebuffer with one color (pixels only; the text
 * buffer is untouched so on-screen text survives a redraw). */
void fb_fill(uint32_t color);

/* Tell the console that the screen no longer shows what it painted.
 * It skips repainting cells it believes are already correct, so
 * anything that writes pixels directly - /dev/fb0, an ioctl fill, a
 * splash image - has to say so, or stale glyphs stay on screen. */
void fb_invalidate(void);

/* Start the text grid this many pixels below the top of the screen
 * (used by the boot splash, which draws logos above the logs). */
void fb_set_text_top(uint32_t pixel_y);

/* Draw a scaled RGB image (nearest neighbour) at (x, y). `scale` is
 * 16.16 fixed point (1:1 = 65536). */
void fb_blit_scaled(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                    const uint8_t *rgb, uint32_t scale);

/* Change the display mode at runtime (see vbe.h - this works on the
 * Bochs VBE adapter that QEMU, Bochs and VirtualBox provide, and
 * returns -ENODEV on hardware with a real GPU). The console is
 * rebound to the new geometry and cleared; the scrollback is dropped,
 * since its lines were wrapped for the old width.
 *
 * Anything else holding the framebuffer description has to be told:
 * highX through highx_rebind(), /dev/fb0 through
 * vfs_devices_refresh_fb(). */
int fb_set_mode(uint32_t width, uint32_t height);

/* Copy the framebuffer description; any pointer may be NULL. */
void fb_get_info(uint32_t *width, uint32_t *height, uint32_t *bpp,
                 uint64_t *pitch, void **address);

/* Report the text grid size (columns x rows) - what TIOCGWINSZ
 * returns to user programs. */
void fb_get_grid(int *cols, int *rows);

/* Graphics mode: hand the raw pixels to another owner (the highX
 * display server). Text output keeps updating the character buffer
 * and the serial mirror, but nothing is painted, so windows are never
 * overwritten by a kernel message. */
void fb_set_graphics(bool on);
bool fb_graphics(void);

/* Repaint the text grid from the character buffer (called when a
 * highX session ends: the console reappears exactly as it was). */
void fb_repaint(void);

#endif /* TUS_DRIVERS_FB_H */
