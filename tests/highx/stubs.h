/*
 * stubs.h - the little bit of "kernel" the compositor test needs
 *
 * kernel/highx/compositor.c talks to exactly two things outside its
 * own file: the framebuffer description (fb_get_info) and kprintf.
 * The test provides both over an ordinary malloc'd buffer, which is
 * what makes the compositor testable on the build host.
 */

#ifndef HIGHX_TEST_STUBS_H
#define HIGHX_TEST_STUBS_H

#include <stdbool.h>
#include <stdint.h>

/* Create the fake framebuffer fb_get_info() will hand out. `pad` extra
 * pixels are added to every scanline, so the pitch is not width * 4:
 * that is what real hardware does, and it puts every other row on an
 * odd 8-byte boundary - the case the compositor's paired stores have
 * to get right. The padding is filled with a sentinel that
 * stub_fb_pad_intact() checks nothing has touched. */
void stub_fb_init(uint32_t width, uint32_t height, uint32_t pad);

/* The pixels behind it, and the scanline stride in pixels. */
uint32_t *stub_fb_pixels(void);
uint32_t stub_fb_stride(void);

/* One pixel of the fake framebuffer. */
uint32_t stub_fb_get(int32_t x, int32_t y);
void stub_fb_set(int32_t x, int32_t y, uint32_t color);

/* False once anything has written into the scanline padding. */
bool stub_fb_pad_intact(void);

/* Mirror of hxcomp_set_background(): the test tells the model which
 * desktop the compositor was given, and stub_background() then says
 * what colour the bottom layer has at (x, y). */
void stub_set_background(uint32_t color, uint32_t style);
uint32_t stub_background(int32_t x, int32_t y);

/* ---- counting screen writes ---- */
/*
 * compositor.c is compiled here with -DHXCOMP_TRACE_WRITES, so every
 * write it makes to the framebuffer is reported. Counting them per
 * pixel is what turns "the window flickers when it moves" into
 * something a test can state: a repaint that writes a pixel twice is
 * a repaint the eye can catch in between.
 */
void stub_trace_reset(void);
uint32_t stub_trace_count(int32_t x, int32_t y);

/* The most times any pixel was written since the last reset, and
 * where that happened. */
uint32_t stub_trace_max(int32_t *x, int32_t *y);

#endif /* HIGHX_TEST_STUBS_H */
