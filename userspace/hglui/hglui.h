/*
 * hglui.h - macOS-style 2D UI chrome, drawn with HighGL
 *
 * A rounded rectangle or a filled circle is not a shape HighGL's
 * triangle pipeline draws directly - it is a triangle (or a quad, two
 * of them) with a FRAGMENT HOOK that turns most of it transparent.
 * The hook computes a signed distance to the true shape at every
 * pixel (Inigo Quilez's rounded-box formula, and the plain circle
 * case of the same idea): negative is inside, positive is outside,
 * and the pixels within about half a pixel of zero are given partial
 * coverage instead of a hard yes/no, which is what makes the edge
 * anti-aliased instead of stair-stepped. HGL_BLEND does the actual
 * mixing - the hook only has to set alpha correctly.
 *
 * This is what "the macOS-style UI is built natively on HighGL" means
 * in practice: not that windows are 3D, but that the library's real
 * programmability (the fragment hook) draws its 2D chrome too, rather
 * than a separate hand-rolled rasteriser sitting next to an unused 3D
 * pipeline.
 *
 * Text is the one thing that does NOT go through HighGL: glyph
 * coverage is data tusfont already produced (see hxfont.c, which
 * established this pattern), not a shape worth expressing as
 * geometry. hglui_text() blends it straight into a plain ARGB buffer
 * - typically the one hglColorBuffer() returns, after the shapes
 * above have been drawn into it, so text layers over them for free.
 */

#ifndef HGLUI_H
#define HGLUI_H

#include <stdint.h>

#include "highgl.h"
#include "tusfont/tusfont.h"

/* Every colour here is 0x00RRGGBB - opaque, the same packing
 * hglColorBuffer() and hx_image() both use. Shape alpha is what the
 * caller passes as a 0..255 opacity, not part of the colour word. */

/* A plain filled rectangle - the common case, drawn without blending
 * since there is nothing beneath it to blend with. */
void hglui_rect(hgl_context *gl, float x, float y, float w, float h,
                uint32_t rgb);

/* A filled rectangle with all four corners rounded to `radius`. To
 * round only the top two corners (a title bar sitting flush above a
 * square window body, which is what every caller in this codebase
 * wants), pass a height taller than the context by `radius` - the
 * bottom corners' rounding falls outside the buffer and is simply
 * never rasterised. */
void hglui_rounded_rect(hgl_context *gl, float x, float y, float w, float h,
                        float radius, uint32_t rgb, uint8_t alpha);

/* A filled, anti-aliased circle - the traffic-light window buttons. */
void hglui_circle(hgl_context *gl, float cx, float cy, float r, uint32_t rgb,
                  uint8_t alpha);

/* Blend `utf8` into `buf` (buf_w * buf_h pixels, 0x00RRGGBB, stride ==
 * buf_w) at `size_px`, pen origin (x, baseline_y). Glyphs outside the
 * buffer are clipped pixel by pixel, so a caller never has to measure
 * first just to stay in bounds. */
void hglui_text(uint32_t *buf, int buf_w, int buf_h, int x, int baseline_y,
                struct tf_font *font, int size_px, uint32_t rgb,
                const char *utf8);

/* How wide `utf8` would be at `size_px`, in whole pixels - for
 * centring a title or sizing a button before drawing it. */
int hglui_text_width(struct tf_font *font, const char *utf8, int size_px);

#endif /* HGLUI_H */
