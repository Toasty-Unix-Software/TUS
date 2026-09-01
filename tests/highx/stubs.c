#include "stubs.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/highx.h"

#define PAD_SENTINEL 0x00DEAD00u

static uint32_t *g_pixels;
static uint32_t *g_writes;
static uint32_t g_width;
static uint32_t g_height;
static uint32_t g_stride; /* pixels per scanline, >= g_width */

void stub_fb_init(uint32_t width, uint32_t height, uint32_t pad) {
    free(g_pixels);
    free(g_writes);
    g_writes = NULL;
    g_width = width;
    g_height = height;
    g_stride = width + pad;
    g_pixels = calloc((size_t)g_stride * height, sizeof(uint32_t));
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = width; x < g_stride; x++) {
            g_pixels[(size_t)y * g_stride + x] = PAD_SENTINEL;
        }
    }
}

uint32_t *stub_fb_pixels(void) {
    return g_pixels;
}

uint32_t stub_fb_stride(void) {
    return g_stride;
}

uint32_t stub_fb_get(int32_t x, int32_t y) {
    return g_pixels[(size_t)y * g_stride + x];
}

void stub_fb_set(int32_t x, int32_t y, uint32_t color) {
    g_pixels[(size_t)y * g_stride + x] = color;
}

bool stub_fb_pad_intact(void) {
    for (uint32_t y = 0; y < g_height; y++) {
        for (uint32_t x = g_width; x < g_stride; x++) {
            if (g_pixels[(size_t)y * g_stride + x] != PAD_SENTINEL) {
                return false;
            }
        }
    }
    return true;
}

/* Mirrors hxcomp_set_background(), including how it derives the grid
 * line colour from the base one. */
static uint32_t g_bg_color = 0x00101820u;
static uint32_t g_bg_grid = 0x00182430u;
static bool g_bg_gridded = true;

void stub_set_background(uint32_t color, uint32_t style) {
    uint32_t r = (color >> 16) & 0xFF, g = (color >> 8) & 0xFF, b = color & 0xFF;
    r = r + 20 > 255 ? 255 : r + 20;
    g = g + 20 > 255 ? 255 : g + 20;
    b = b + 24 > 255 ? 255 : b + 24;

    g_bg_color = color & 0x00FFFFFFu;
    g_bg_grid = (r << 16) | (g << 8) | b;
    g_bg_gridded = style == HX_BG_GRID;
}

uint32_t stub_background(int32_t x, int32_t y) {
    if (!g_bg_gridded) {
        return g_bg_color;
    }
    return (x % 32 == 0 || y % 32 == 0) ? g_bg_grid : g_bg_color;
}

/* ---- kernel symbols the compositor expects ---- */

void fb_get_info(uint32_t *width, uint32_t *height, uint32_t *bpp,
                 uint64_t *pitch, void **address);

void fb_get_info(uint32_t *width, uint32_t *height, uint32_t *bpp,
                 uint64_t *pitch, void **address) {
    if (width) {
        *width = g_width;
    }
    if (height) {
        *height = g_height;
    }
    if (bpp) {
        *bpp = 32;
    }
    if (pitch) {
        *pitch = (uint64_t)g_stride * 4;
    }
    if (address) {
        *address = g_pixels;
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

/* ---- counting screen writes ---- */

void stub_trace_reset(void) {
    if (g_writes == NULL) {
        g_writes = calloc((size_t)g_stride * g_height, sizeof(uint32_t));
    } else {
        memset(g_writes, 0, (size_t)g_stride * g_height * sizeof(uint32_t));
    }
}

/* The compositor hands back a pointer into the framebuffer; turn it
 * into a pixel index the same way the hardware would. */
void hxcomp_trace_write(const uint32_t *dst, int32_t n) {
    if (g_writes == NULL || n <= 0) {
        return;
    }
    size_t at = (size_t)(dst - g_pixels);
    for (int32_t i = 0; i < n; i++) {
        if (at + (size_t)i < (size_t)g_stride * g_height) {
            g_writes[at + (size_t)i]++;
        }
    }
}

uint32_t stub_trace_count(int32_t x, int32_t y) {
    if (g_writes == NULL) {
        return 0;
    }
    return g_writes[(size_t)y * g_stride + (size_t)x];
}

uint32_t stub_trace_max(int32_t *x, int32_t *y) {
    uint32_t worst = 0;
    if (g_writes == NULL) {
        return 0;
    }
    for (uint32_t row = 0; row < g_height; row++) {
        for (uint32_t col = 0; col < g_width; col++) {
            uint32_t n = g_writes[(size_t)row * g_stride + col];
            if (n > worst) {
                worst = n;
                if (x) *x = (int32_t)col;
                if (y) *y = (int32_t)row;
            }
        }
    }
    return worst;
}
