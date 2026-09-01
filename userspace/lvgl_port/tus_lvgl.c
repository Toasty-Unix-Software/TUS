#include "tus_lvgl.h"

#include "lvgl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SYS_UPTIME (kernel/syscall/syscall.h): milliseconds since boot.
 * musl's syscall_arch.h does not wrap TUS's ABI, so this makes the
 * int $0x80 trap directly - the same three-argument stub every other
 * highX client that needs a syscall musl has no wrapper for uses
 * (hxfiles.c, hxvideo.c). */
#define SYS_UPTIME 7

static long tus_syscall(long n, long a1, long a2, long a3) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret)
                     : "a"(n), "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8),
                       "r"(r9)
                     : "memory", "cc");
    return ret;
}

static uint32_t uptime_ms(void) {
    return (uint32_t)tus_syscall(SYS_UPTIME, 0, 0, 0);
}

/* One of these per live tus_lvgl_create() display, reached through
 * lv_display_set_user_data()/lv_indev_set_user_data() - what used to
 * be this file's file-scope statics (g_win, g_buf, g_ptr_*) before a
 * second simultaneous window (tuswm decorating more than one client)
 * needed a second copy of each. */
typedef struct {
    unsigned int win;
    uint32_t    *buf;
    lv_indev_t  *indev;
    int          ptr_x, ptr_y, ptr_down;
} tus_lvgl_ctx;

static int g_lvgl_inited;

static void flush_cb(lv_display_t *disp, const lv_area_t *area,
                     uint8_t *px_map) {
    (void)area;
    tus_lvgl_ctx *ctx = (tus_lvgl_ctx *)lv_display_get_user_data(disp);
    hx_image(ctx->win, 0, 0,
            (unsigned int)lv_display_get_horizontal_resolution(disp),
            (unsigned int)lv_display_get_vertical_resolution(disp),
            (const unsigned int *)px_map);
    hx_commit(ctx->win);
    lv_display_flush_ready(disp);
}

static void indev_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    tus_lvgl_ctx *ctx = (tus_lvgl_ctx *)lv_indev_get_user_data(indev);
    data->point.x = (int32_t)ctx->ptr_x;
    data->point.y = (int32_t)ctx->ptr_y;
    data->state = ctx->ptr_down ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

static void log_cb(lv_log_level_t level, const char *buf) {
    (void)level;
    printf("lvgl-log: %s", buf);
}

lv_display_t *tus_lvgl_create(unsigned int win, int w, int h) {
    if (!g_lvgl_inited) {
        lv_log_register_print_cb(log_cb);
        lv_init();
        lv_tick_set_cb(uptime_ms);
        g_lvgl_inited = 1;
    }

    tus_lvgl_ctx *ctx = calloc(1, sizeof(*ctx));
    if (ctx == NULL) {
        return NULL;
    }
    ctx->win = win;

    size_t buf_bytes = (size_t)w * (size_t)h * sizeof(uint32_t);
    ctx->buf = malloc(buf_bytes);
    if (ctx->buf == NULL) {
        free(ctx);
        return NULL;
    }

    /* Render mode FULL: one buffer the size of the whole window,
     * redrawn whole every flush - see the longer note this used to
     * carry in tus_lvgl.c's history. Simplest correct choice, and
     * the one with no dirty-rect bookkeeping to get wrong; title bars
     * and desktop chrome are all small enough that repainting them
     * whole costs nothing worth optimising away. */
    lv_display_t *disp = lv_display_create(w, h);
    if (disp == NULL) {
        free(ctx->buf);
        free(ctx);
        return NULL;
    }
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_XRGB8888);
    lv_display_set_buffers(disp, ctx->buf, NULL, (uint32_t)buf_bytes,
                           LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(disp, flush_cb);
    lv_display_set_user_data(disp, ctx);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, indev_read_cb);
    lv_indev_set_display(indev, disp);
    lv_indev_set_user_data(indev, ctx);
    ctx->indev = indev;

    return disp;
}

int tus_lvgl_resize(lv_display_t *disp, int w, int h) {
    if (disp == NULL || w <= 0 || h <= 0) {
        return -1;
    }
    tus_lvgl_ctx *ctx = (tus_lvgl_ctx *)lv_display_get_user_data(disp);
    size_t buf_bytes = (size_t)w * (size_t)h * sizeof(uint32_t);
    uint32_t *new_buf = malloc(buf_bytes);
    if (new_buf == NULL) {
        return -1;
    }
    free(ctx->buf);
    ctx->buf = new_buf;

    /* Resolution before buffers, not after: lv_display_set_buffers()
     * validates the new buffer's size against the display's CURRENT
     * resolution (LV_ASSERT_FORMAT_MSG(stride*h <= buf_size, ...)) -
     * with the old, larger resolution still in effect, a shrink (a
     * tiling WM's master window losing width when a second window
     * appears) hands it a buffer correctly sized for the NEW width but
     * checked against the OLD, larger one, so the assert always fails.
     * LVGL's default LV_ASSERT_HANDLER is `while(1);` - a silent,
     * uncrashed infinite loop - which is why this hung tuswm itself
     * with no panic and no error, one shrink into ever having a second
     * managed window. A grow (lvgldemo's original single-window case,
     * which never shrinks) coincidentally satisfies the same check
     * either order, which is how this shipped without being caught
     * the first time. */
    lv_display_set_resolution(disp, w, h);
    lv_display_set_buffers(disp, ctx->buf, NULL, (uint32_t)buf_bytes,
                           LV_DISPLAY_RENDER_MODE_FULL);
    lv_obj_invalidate(lv_display_get_screen_active(disp));
    return 0;
}

void tus_lvgl_seed(lv_display_t *disp, const uint32_t *pixels) {
    if (disp == NULL || pixels == NULL) {
        return;
    }
    tus_lvgl_ctx *ctx = (tus_lvgl_ctx *)lv_display_get_user_data(disp);
    size_t w = (size_t)lv_display_get_horizontal_resolution(disp);
    size_t h = (size_t)lv_display_get_vertical_resolution(disp);
    memcpy(ctx->buf, pixels, w * h * sizeof(uint32_t));
}

void tus_lvgl_feed_pointer(lv_display_t *disp, int x, int y, int pressed) {
    if (disp == NULL) {
        return;
    }
    tus_lvgl_ctx *ctx = (tus_lvgl_ctx *)lv_display_get_user_data(disp);
    ctx->ptr_x = x;
    ctx->ptr_y = y;
    ctx->ptr_down = pressed;
}

void tus_lvgl_tick(void) {
    lv_timer_handler();
}

void tus_lvgl_destroy(lv_display_t *disp) {
    if (disp == NULL) {
        return;
    }
    tus_lvgl_ctx *ctx = (tus_lvgl_ctx *)lv_display_get_user_data(disp);
    lv_indev_delete(ctx->indev);
    lv_display_delete(disp);
    free(ctx->buf);
    free(ctx);
}
