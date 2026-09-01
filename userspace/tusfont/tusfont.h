/*
 * tusfont.h - TrueType fonts, rasterised from scratch
 *
 * TUS draws text from an 8x16 bitmap font. That is the right thing
 * for a console - every cell the same width, every glyph the same
 * height, no layout to do - and the wrong thing for everything else:
 * a window title, a menu, a document all want type that scales.
 *
 * This library reads a `.ttf` file and turns a glyph into a coverage
 * bitmap. It is written here rather than ported: the format is
 * documented, the geometry is quadratic beziers, and the whole thing
 * fits in three files that can be read.
 *
 * WHAT IT READS
 *
 *   head  units per em, and which size the offsets are
 *   maxp  how many glyphs
 *   hhea  ascent, descent, line gap, and how many are in hmtx
 *   hmtx  advance widths
 *   cmap  character -> glyph (formats 4 and 12)
 *   loca  where each glyph's outline is
 *   glyf  the outlines: contours of on- and off-curve points
 *   kern  format 0 pairs, when the font has them
 *
 * What it does NOT read: the hinting bytecode (`fpgm`, `prep`, the
 * instructions inside a glyph), `CFF ` outlines (PostScript fonts),
 * and `GPOS` positioning. Hinting matters below about 12 pixels,
 * where an unhinted stem lands between two pixels and goes grey; the
 * rasteriser's answer to that is to sample vertically rather than to
 * run a bytecode interpreter.
 *
 * WHY FIXED POINT
 *
 * There is not a float in here. TUS userspace is built with
 * `-mgeneral-regs-only`, so floating point is not available to a
 * program that has not asked for it, and a rasteriser is exactly the
 * kind of code where "it works on my machine" comes from rounding
 * anyway. Coordinates are 26.6 - a signed integer counting 1/64ths
 * of a pixel, which is what FreeType uses and what TrueType's own
 * grid is designed around. The result is bit-identical everywhere,
 * which is what lets the host tests check the rasteriser against a
 * brute-force model instead of against a tolerance.
 */

#ifndef TUS_FONT_H
#define TUS_FONT_H

#include <stddef.h>
#include <stdint.h>

/* 26.6 fixed point: 64 units to the pixel. */
typedef int32_t tf_fixed;
#define TF_ONE 64
#define TF_FROM_INT(i)  ((tf_fixed)((i) * TF_ONE))
#define TF_TO_INT(f)    ((int)((f) >> 6))
#define TF_FLOOR(f)     ((f) & ~(tf_fixed)63)
#define TF_CEIL(f)      (((f) + 63) & ~(tf_fixed)63)

struct tf_font;

/* Everything a caller needs to put lines under each other, in 26.6
 * pixels at the size that was asked for. */
struct tf_metrics {
    tf_fixed ascent;   /* above the baseline, positive */
    tf_fixed descent;  /* below the baseline, positive */
    tf_fixed line_gap; /* the font's own leading */
    tf_fixed height;   /* ascent + descent + line_gap: baseline to baseline */
};

/* A rendered glyph: 8-bit coverage, one byte per pixel, 0..255.
 *
 * `left` and `top` place it against the pen: the bitmap's top left
 * corner goes at (pen_x + left, baseline_y - top). They are signed
 * because a glyph can start left of the pen (an italic 'f') and can
 * hang below the baseline (a 'g').
 */
struct tf_bitmap {
    int      w, h;
    int      left, top;
    int      stride;
    uint8_t *pixels;   /* h * stride bytes; NULL for a blank glyph */
};

/* ---- opening a font ---- */

/* Read a font file. Returns NULL if it cannot be read or is not a
 * TrueType font with `glyf` outlines. */
struct tf_font *tf_open(const char *path);

/* Use a font already in memory. `own` non-zero hands ownership of the
 * buffer to the font, which frees it in tf_close(). */
struct tf_font *tf_load(void *data, size_t len, int own);

void tf_close(struct tf_font *font);

/* Why the last call failed, for a caller that wants to say so. */
const char *tf_error(void);

/* ---- sizes and metrics ----
 *
 * A size is given in 26.6 pixels-per-em, which is what a user means
 * by "12 point at 96 dpi" once the arithmetic is done. Every call
 * that needs a size takes it; the font holds no current size, so two
 * parts of a program can draw at two sizes without stepping on each
 * other.
 */

void tf_metrics_at(const struct tf_font *font, tf_fixed size,
                   struct tf_metrics *out);

/* Glyph index for a Unicode codepoint; 0 (the .notdef glyph) when the
 * font has no glyph for it. */
uint16_t tf_glyph_index(const struct tf_font *font, uint32_t cp);

/* How far the pen moves after drawing this glyph. */
tf_fixed tf_advance(const struct tf_font *font, uint16_t glyph,
                    tf_fixed size);

/* The kerning adjustment between two glyphs (usually negative, and 0
 * when the font has no `kern` table or no pair for these two). */
tf_fixed tf_kern(const struct tf_font *font, uint16_t left, uint16_t right,
                 tf_fixed size);

/* Glyph count, for a caller walking the whole font. */
uint16_t tf_glyph_count(const struct tf_font *font);

/* ---- rendering ---- */

/* Rasterise one glyph at `size`, with the pen at a fractional
 * position `x_off` (0..63) so that text laid out at sub-pixel
 * positions does not snap.
 *
 * On success *out describes a bitmap the CALLER frees with
 * tf_free_bitmap(). A glyph with no outline (a space) succeeds with
 * .pixels == NULL and w == h == 0. Returns 0, or -1. */
int tf_render_glyph(struct tf_font *font, uint16_t glyph, tf_fixed size,
                    tf_fixed x_off, struct tf_bitmap *out);

void tf_free_bitmap(struct tf_bitmap *bm);

/* ---- laying out a string ----
 *
 * tf_text_width measures, tf_text_draw draws. Both walk UTF-8 and
 * apply kerning, so a caller never has to know that a Turkish 'ş' is
 * two bytes or that 'AV' is tighter than 'AX'.
 */

tf_fixed tf_text_width(struct tf_font *font, const char *utf8,
                       tf_fixed size);

/* The callback a draw makes for each glyph: `bm` is the coverage,
 * `x` and `y` are where its top left corner goes, in whole pixels
 * relative to the pen origin the caller passed in. Return non-zero to
 * stop early. */
typedef int (*tf_draw_fn)(void *ctx, const struct tf_bitmap *bm,
                          int x, int y);

/* Lay `utf8` out with the pen origin at (x, baseline_y) in whole
 * pixels, calling `fn` once per glyph. Returns the pen's final x, or
 * a negative value on failure. Glyphs are cached inside the font, so
 * drawing the same text twice rasterises nothing the second time. */
int tf_text_draw(struct tf_font *font, const char *utf8, tf_fixed size,
                 int x, int baseline_y, tf_draw_fn fn, void *ctx);

/* Decode one UTF-8 character. Returns the number of bytes consumed
 * (at least 1, so a caller always makes progress) and writes the
 * codepoint, or U+FFFD for a byte sequence that is not valid. */
int tf_utf8_next(const char *s, uint32_t *cp);

#endif /* TUS_FONT_H */
