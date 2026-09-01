/*
 * paint.h - an ARGB canvas Clint draws a page into
 *
 * The whole page is rendered off screen and then handed to highX as
 * one image. That is a deliberate choice over drawing straight into
 * the window with hx_fill/hx_text calls: a page is hundreds of small
 * pieces, and sending them one request at a time would show the page
 * assembling itself. One buffer, one blit, nothing to see in between.
 *
 * It is also what makes headings possible. highAPI's text is a single
 * 8x16 cell; here the glyphs are rasterised from the same font by
 * hand, so a character can be drawn at two or three times the size.
 */

#ifndef CLINT_PAINT_H
#define CLINT_PAINT_H

#include <stdint.h>

#include "image.h"

struct canvas {
    uint32_t *px;
    int w, h;

    /* Rows outside [clip_top, clip_bottom) are never touched. The
     * page is drawn into the middle of a window that also has a
     * toolbar and a status line, and half a line of a scrolled page
     * must not land on either. */
    int clip_top, clip_bottom;
};

int canvas_init(struct canvas *c, int w, int h);
void canvas_free(struct canvas *c);

/* Restrict every later shape to these rows. canvas_init() starts with
 * the whole canvas. */
void canvas_clip(struct canvas *c, int top, int bottom);

void canvas_fill(struct canvas *c, int x, int y, int w, int h, uint32_t color);

/*
 * The shapes a browser's own chrome is made of. A rounded rectangle
 * is the whole visual language of a modern toolbar, and a bitmap
 * canvas can draw one honestly: the corners are quarter circles
 * tested per pixel, which at radius eight is a few hundred tests.
 */
void canvas_round_rect(struct canvas *c, int x, int y, int w, int h,
                       int radius, uint32_t color);
void canvas_round_border(struct canvas *c, int x, int y, int w, int h,
                         int radius, int thickness, uint32_t color);

/* A filled circle and a ring, for buttons and their hover halos. */
void canvas_circle(struct canvas *c, int cx, int cy, int radius,
                   uint32_t color);
void canvas_ring(struct canvas *c, int cx, int cy, int radius, int thickness,
                 uint32_t color);

/* A solid triangle pointing left, right, up or down - the arrows on
 * the back and forward buttons. */
enum { DIR_LEFT = 0, DIR_RIGHT, DIR_UP, DIR_DOWN };
void canvas_triangle(struct canvas *c, int cx, int cy, int size, int dir,
                     uint32_t color);

/*
 * Draw `text` with its top-left corner at (x, y). `scale` replicates
 * each glyph pixel; `bold` strikes it twice one pixel apart, which is
 * what a bitmap font has instead of a bold face.
 */
void canvas_text(struct canvas *c, int x, int y, const char *text,
                 uint32_t color, int scale, int bold, int underline);

/*
 * Draw `img` into the rectangle (x, y, w, h), scaling it to fit and
 * blending it over what is already there - a PNG's transparent
 * corners have to show the page behind them, not a black square.
 */
void canvas_image(struct canvas *c, int x, int y, int w, int h,
                  const struct image *img);

#endif /* CLINT_PAINT_H */
