/*
 * tus_lvgl_font.h - tusfont as an LVGL font backend
 *
 * LVGL bitmap-font text (lv_label, and every widget that draws text)
 * goes through a `const lv_font_t *` with three callbacks:
 * get_glyph_dsc (metrics), get_glyph_bitmap (pixels, A8 coverage) and
 * release_glyph. This wires those three straight into
 * userspace/tusfont/ - the project's own from-scratch TrueType
 * rasteriser - so chrome text (title bars, the dock, the top bar)
 * renders with a real scalable face instead of LVGL's bundled bitmap
 * Montserrat.
 *
 * Modelled on sources/lvgl/src/font/freetype/lv_freetype_glyph.c +
 * lv_freetype_image.c: get_glyph_dsc renders once to learn a glyph's
 * box/offsets/advance and immediately frees the bitmap (LVGL calls
 * this alone, with no matching release, every time it only needs to
 * MEASURE text - lv_font_get_glyph_width() never releases), then
 * get_glyph_bitmap renders again for the pixels it actually keeps
 * until release_glyph - tf_render_glyph's own (glyph, size, sub-pixel)
 * cache (see userspace/tusfont/tusfont.h) makes the second render
 * cheap rather than a real doubling of work.
 */

#ifndef TUS_LVGL_FONT_H
#define TUS_LVGL_FONT_H

#include "lvgl.h"

/* Opens `path` (a .ttf file) at `px_size` pixels-per-em and returns an
 * lv_font_t ready for lv_obj_set_style_text_font()/LV_FONT_DEFAULT.
 * NULL if the file cannot be read or is not a TrueType font with
 * `glyf` outlines (see tf_open()). The returned font owns its own
 * tf_font and must be freed with tus_lvgl_font_destroy(), not just
 * lv_free()'d. */
lv_font_t *tus_lvgl_font_create(const char *path, int px_size);

void tus_lvgl_font_destroy(lv_font_t *font);

#endif /* TUS_LVGL_FONT_H */
