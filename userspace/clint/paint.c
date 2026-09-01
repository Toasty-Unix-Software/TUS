/*
 * paint.c - see paint.h
 *
 * Clipping is done once per shape rather than per pixel: a rectangle
 * is intersected with the canvas before the fill loop starts, and a
 * glyph that falls entirely outside is skipped before its rows are
 * walked. The inner loops then have no tests in them, which matters
 * because a page of text is tens of thousands of glyph rows and TUS
 * runs under emulation.
 */

#include "paint.h"

#include <stdlib.h>
#include <string.h>

#include "font.h"
#include "utf8.h"

/* Every glyph is one cell wide and sixteen rows tall, whatever the
 * character; the tables behind font_glyph() are what decides which
 * characters there are. */
#define FONT_WIDTH  8
#define FONT_HEIGHT 16

int canvas_init(struct canvas *c, int w, int h) {
    if (w <= 0 || h <= 0) return -1;

    uint32_t *px = realloc(c->px, (size_t)w * (size_t)h * sizeof(uint32_t));
    if (px == NULL) return -1;
    c->px = px;
    c->w = w;
    c->h = h;
    c->clip_top = 0;
    c->clip_bottom = h;
    return 0;
}

void canvas_clip(struct canvas *c, int top, int bottom) {
    if (top < 0) top = 0;
    if (bottom > c->h) bottom = c->h;
    c->clip_top = top;
    c->clip_bottom = bottom > top ? bottom : top;
}

void canvas_free(struct canvas *c) {
    free(c->px);
    c->px = NULL;
    c->w = c->h = 0;
}

void canvas_fill(struct canvas *c, int x, int y, int w, int h, uint32_t color) {
    if (c->px == NULL) return;

    if (x < 0) { w += x; x = 0; }
    if (y < c->clip_top) { h -= c->clip_top - y; y = c->clip_top; }
    if (x + w > c->w) w = c->w - x;
    if (y + h > c->clip_bottom) h = c->clip_bottom - y;
    if (w <= 0 || h <= 0) return;

    for (int row = 0; row < h; row++) {
        uint32_t *dst = c->px + (size_t)(y + row) * c->w + x;
        for (int col = 0; col < w; col++) dst[col] = color;
    }
}

/* One glyph, scaled. Returns nothing: the caller advances the pen. */
static void draw_glyph(struct canvas *c, int x, int y, const uint8_t *glyph,
                       uint32_t color, int scale) {
    int w = FONT_WIDTH * scale, h = FONT_HEIGHT * scale;
    if (x >= c->w || y >= c->clip_bottom || x + w <= 0 ||
        y + h <= c->clip_top) {
        return;
    }

    for (int row = 0; row < FONT_HEIGHT; row++) {
        uint8_t bits = glyph[row];
        if (bits == 0) continue;

        for (int sy = 0; sy < scale; sy++) {
            int py = y + row * scale + sy;
            if (py < c->clip_top || py >= c->clip_bottom) continue;
            uint32_t *dst = c->px + (size_t)py * c->w;

            for (int col = 0; col < FONT_WIDTH; col++) {
                if ((bits & (0x80u >> col)) == 0) continue;
                int px0 = x + col * scale;
                for (int sx = 0; sx < scale; sx++) {
                    int px = px0 + sx;
                    if (px >= 0 && px < c->w) dst[px] = color;
                }
            }
        }
    }
}

void canvas_text(struct canvas *c, int x, int y, const char *text,
                 uint32_t color, int scale, int bold, int underline) {
    if (c->px == NULL || text == NULL) return;
    if (scale < 1) scale = 1;

    int pen = x;
    size_t len = strlen(text);
    for (size_t at = 0; at < len;) {
        uint32_t cp;
        at += utf8_next(text + at, len - at, &cp);

        const uint8_t *glyph = font_glyph(cp);
        if (glyph == NULL) glyph = font_glyph('?');

        draw_glyph(c, pen, y, glyph, color, scale);
        if (bold) {
            /* A second strike one pixel over: the bitmap font has no
             * bold face, and this is what terminals have always
             * done. */
            draw_glyph(c, pen + 1, y, glyph, color, scale);
        }
        pen += FONT_WIDTH * scale;
    }

    if (underline) {
        int thickness = scale;
        canvas_fill(c, x, y + FONT_HEIGHT * scale - thickness, pen - x,
                    thickness, color);
    }
}

/*
 * Scaling is nearest neighbour, computed with integers: for each
 * destination pixel, the source pixel it lands on. It is the only
 * resampling that costs nothing, and at the sizes a page uses an
 * image is usually drawn at its own size anyway, where the mapping is
 * one to one.
 */
void canvas_image(struct canvas *c, int x, int y, int w, int h,
                  const struct image *img) {
    if (c->px == NULL || img == NULL || img->px == NULL) return;
    if (w <= 0 || h <= 0 || img->w <= 0 || img->h <= 0) return;

    int y0 = y > c->clip_top ? y : c->clip_top;
    int y1 = y + h < c->clip_bottom ? y + h : c->clip_bottom;
    int x0 = x > 0 ? x : 0;
    int x1 = x + w < c->w ? x + w : c->w;
    if (y0 >= y1 || x0 >= x1) return;

    for (int py = y0; py < y1; py++) {
        int sy = (py - y) * img->h / h;
        const uint32_t *src = img->px + (size_t)sy * (size_t)img->w;
        uint32_t *dst = c->px + (size_t)py * c->w;

        for (int px = x0; px < x1; px++) {
            uint32_t s = src[(px - x) * img->w / w];
            unsigned a = s >> 24;

            if (a == 255) {
                dst[px] = s & 0x00FFFFFFu;
            } else if (a != 0) {
                uint32_t d = dst[px];
                unsigned r = (((s >> 16) & 0xFFu) * a +
                              ((d >> 16) & 0xFFu) * (255u - a)) / 255u;
                unsigned g = (((s >> 8) & 0xFFu) * a +
                              ((d >> 8) & 0xFFu) * (255u - a)) / 255u;
                unsigned b = ((s & 0xFFu) * a + (d & 0xFFu) * (255u - a)) / 255u;
                dst[px] = (r << 16) | (g << 8) | b;
            }
        }
    }
}

/* ---- the shapes the browser's own chrome is drawn with ---- */

/* Whether (x, y) is inside the rounded rectangle: only the corner
 * squares need the distance test, and that is what makes this cheap
 * enough to do per pixel. */
static int inside_round(int x, int y, int w, int h, int radius) {
    int cx = x < radius ? radius : (x >= w - radius ? w - radius - 1 : x);
    int cy = y < radius ? radius : (y >= h - radius ? h - radius - 1 : y);
    if (x == cx || y == cy) return 1;

    int dx = x - cx, dy = y - cy;
    return dx * dx + dy * dy <= radius * radius;
}

void canvas_round_rect(struct canvas *c, int x, int y, int w, int h,
                       int radius, uint32_t color) {
    if (c->px == NULL || w <= 0 || h <= 0) return;
    if (radius * 2 > w) radius = w / 2;
    if (radius * 2 > h) radius = h / 2;
    if (radius <= 0) {
        canvas_fill(c, x, y, w, h, color);
        return;
    }

    /* The middle is a plain fill; only the two end caps are tested. */
    canvas_fill(c, x + radius, y, w - 2 * radius, h, color);

    for (int row = 0; row < h; row++) {
        int py = y + row;
        if (py < c->clip_top || py >= c->clip_bottom || py < 0 || py >= c->h) {
            continue;
        }
        uint32_t *dst = c->px + (size_t)py * c->w;

        for (int col = 0; col < radius; col++) {
            if (!inside_round(col, row, w, h, radius)) continue;
            int left = x + col, right = x + w - 1 - col;
            if (left >= 0 && left < c->w) dst[left] = color;
            if (right >= 0 && right < c->w) dst[right] = color;
        }
    }
}

void canvas_round_border(struct canvas *c, int x, int y, int w, int h,
                         int radius, int thickness, uint32_t color) {
    if (thickness <= 0) return;
    /* An outline is one rounded rectangle with a smaller one taken
     * out of it - but the inside is not painted, so it is drawn as a
     * ring of pixels: inside the outer shape and outside the inner. */
    if (radius * 2 > w) radius = w / 2;
    if (radius * 2 > h) radius = h / 2;

    for (int row = 0; row < h; row++) {
        int py = y + row;
        if (py < c->clip_top || py >= c->clip_bottom || py < 0 || py >= c->h) {
            continue;
        }
        uint32_t *dst = c->px + (size_t)py * c->w;

        for (int col = 0; col < w; col++) {
            if (!inside_round(col, row, w, h, radius)) continue;

            int inner = col >= thickness && col < w - thickness &&
                        row >= thickness && row < h - thickness &&
                        inside_round(col - thickness, row - thickness,
                                     w - 2 * thickness, h - 2 * thickness,
                                     radius > thickness ? radius - thickness
                                                        : 0);
            if (inner) continue;

            int px = x + col;
            if (px >= 0 && px < c->w) dst[px] = color;
        }
    }
}

void canvas_circle(struct canvas *c, int cx, int cy, int radius,
                   uint32_t color) {
    canvas_round_rect(c, cx - radius, cy - radius, radius * 2, radius * 2,
                      radius, color);
}

void canvas_ring(struct canvas *c, int cx, int cy, int radius, int thickness,
                 uint32_t color) {
    if (c->px == NULL || radius <= 0) return;

    for (int dy = -radius; dy <= radius; dy++) {
        int py = cy + dy;
        if (py < c->clip_top || py >= c->clip_bottom || py < 0 || py >= c->h) {
            continue;
        }
        uint32_t *dst = c->px + (size_t)py * c->w;

        for (int dx = -radius; dx <= radius; dx++) {
            int d2 = dx * dx + dy * dy;
            int inner = radius - thickness;
            if (d2 > radius * radius || d2 < inner * inner) continue;
            int px = cx + dx;
            if (px >= 0 && px < c->w) dst[px] = color;
        }
    }
}

void canvas_triangle(struct canvas *c, int cx, int cy, int size, int dir,
                     uint32_t color) {
    if (c->px == NULL || size <= 0) return;

    for (int i = 0; i < size; i++) {
        /* The point is one pixel and each step back from it widens
         * the shape by two: a triangle drawn as a stack of lines,
         * with the point on the side it faces. */
        int span = i;
        if (dir == DIR_LEFT || dir == DIR_RIGHT) {
            int x = dir == DIR_LEFT ? cx - size / 2 + i : cx + size / 2 - i;
            canvas_fill(c, x, cy - span, 1, span * 2 + 1, color);
        } else {
            int y = dir == DIR_UP ? cy - size / 2 + i : cy + size / 2 - i;
            canvas_fill(c, cx - span, y, span * 2 + 1, 1, color);
        }
    }
}
