/*
 * tusvideo.h - the display mode ABI (SYS_VIDEO)
 *
 * Shared verbatim by the kernel and by `res_set`, the same way
 * include/highx.h and include/tusnet.h are: one header, one truth
 * about the structure layout, no chance of the two drifting.
 *
 *     long video(int op, void *arg, unsigned long len);   SYS_VIDEO = 54
 *
 * `op` is a TUS_VIDEO_* opcode, `arg` points at a struct
 * tus_video_mode and `len` is its size, which the kernel validates
 * before touching a byte. Every opcode uses the same structure and
 * writes its result back into it, so a round trip is one call.
 *
 * WHO MAY CHANGE THE MODE
 *
 * TUS_VIDEO_SET_MODE is the only opcode that requires root: it
 * reprograms a piece of hardware the whole machine shares, and a
 * program that could shrink the screen out from under the window
 * system would be a program that could make the machine unusable to
 * everyone on it. Reading the current mode and listing modes are
 * open to anyone.
 *
 * That is why /bin/res_set is NOT setuid. It is an ordinary program;
 * the check lives in the kernel, and the way to pass it is `doas`.
 */

#ifndef TUS_VIDEO_H
#define TUS_VIDEO_H

#include <stdint.h>

/* Opcodes. */
#define TUS_VIDEO_GET_MODE  0 /* fill in the mode on screen right now */
#define TUS_VIDEO_SET_MODE  1 /* program width x height (root only) */
#define TUS_VIDEO_LIST_MODE 2 /* mode number `index` from the table */
#define TUS_VIDEO_CAPS      3 /* what this machine can do */

/* Capability flags reported in tus_video_mode.flags by
 * TUS_VIDEO_CAPS and TUS_VIDEO_GET_MODE. */
#define TUS_VIDEO_F_MODESET  0x1 /* the mode can be changed at runtime */
#define TUS_VIDEO_F_HIGHX    0x2 /* a highX session owns the screen */

struct tus_video_mode {
    uint32_t width;   /* in for SET, out for GET/LIST */
    uint32_t height;  /* in for SET, out for GET/LIST */
    uint32_t bpp;     /* out; SET always programs 32 */
    uint32_t pitch;   /* out: bytes per scanline */
    uint32_t index;   /* in for LIST: 0, 1, 2 ... until -ENOENT */
    uint32_t flags;   /* out: TUS_VIDEO_F_* */
    uint32_t max_width;   /* out: the largest mode SET will accept */
    uint32_t max_height;
};

#endif /* TUS_VIDEO_H */
