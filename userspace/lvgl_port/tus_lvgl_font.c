#include "tus_lvgl_font.h"

#include "tusfont/tusfont.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    struct tf_font *font;
    tf_fixed        size; /* 26.6 pixels-per-em */
} tus_font_dsc_t;

/* What get_glyph_bitmap allocates and release_glyph frees - one
 * rendered coverage bitmap plus the lv_draw_buf_t wrapping a
 * properly-strided copy of it (tf's own row stride and LVGL's A8
 * stride need not match). */
typedef struct {
    struct tf_bitmap bm;
    lv_draw_buf_t    *draw_buf;
} tus_glyph_entry_t;

static bool tus_get_glyph_dsc_cb(const lv_font_t *font, lv_font_glyph_dsc_t *dsc_out,
                                 uint32_t letter, uint32_t letter_next) {
    tus_font_dsc_t *fd = (tus_font_dsc_t *)font->dsc;
    uint16_t gi = tf_glyph_index(fd->font, letter);

    tf_fixed adv = tf_advance(fd->font, gi, fd->size);
    if (letter_next != 0) {
        uint16_t gi_next = tf_glyph_index(fd->font, letter_next);
        adv += tf_kern(fd->font, gi, gi_next, fd->size);
    }

    /* tf_render_glyph is the only way tusfont exposes a glyph's box -
     * render once just to read w/h/left/top, then throw the bitmap
     * away: this call has no matching release_glyph (measurement-only
     * callers like lv_font_get_glyph_width() never call it), so
     * anything stashed in dsc_out->entry here would leak on every
     * character of every text-width measurement during layout. */
    struct tf_bitmap bm;
    if (tf_render_glyph(fd->font, gi, fd->size, 0, &bm) != 0) {
        return false;
    }

    dsc_out->adv_w = (uint16_t)((adv + TF_ONE / 2) >> 6);
    dsc_out->box_w = (uint16_t)bm.w;
    dsc_out->box_h = (uint16_t)bm.h;
    dsc_out->ofs_x = (int16_t)bm.left;
    dsc_out->ofs_y = (int16_t)(bm.top - bm.h);
    dsc_out->format = (bm.w > 0 && bm.h > 0) ? LV_FONT_GLYPH_FORMAT_A8
                                             : LV_FONT_GLYPH_FORMAT_NONE;
    dsc_out->is_placeholder = (gi == 0);
    dsc_out->gid.index = gi;
    dsc_out->entry = NULL;

    tf_free_bitmap(&bm);
    return true;
}

static const void *tus_get_glyph_bitmap_cb(lv_font_glyph_dsc_t *g_dsc, lv_draw_buf_t *draw_buf) {
    (void)draw_buf;
    const lv_font_t *font = g_dsc->resolved_font;
    tus_font_dsc_t *fd = (tus_font_dsc_t *)font->dsc;
    uint16_t gi = (uint16_t)g_dsc->gid.index;

    tus_glyph_entry_t *e = calloc(1, sizeof(*e));
    if (e == NULL) {
        return NULL;
    }
    if (tf_render_glyph(fd->font, gi, fd->size, 0, &e->bm) != 0 ||
        e->bm.pixels == NULL || e->bm.w <= 0 || e->bm.h <= 0) {
        free(e);
        return NULL;
    }

    uint32_t stride = lv_draw_buf_width_to_stride((uint32_t)e->bm.w, LV_COLOR_FORMAT_A8);
    e->draw_buf = lv_draw_buf_create((uint32_t)e->bm.w, (uint32_t)e->bm.h,
                                     LV_COLOR_FORMAT_A8, stride);
    if (e->draw_buf == NULL) {
        tf_free_bitmap(&e->bm);
        free(e);
        return NULL;
    }
    for (int y = 0; y < e->bm.h; y++) {
        memcpy((uint8_t *)e->draw_buf->data + (size_t)y * stride,
               e->bm.pixels + (size_t)y * (size_t)e->bm.stride, (size_t)e->bm.w);
    }

    g_dsc->entry = e;
    return e->draw_buf;
}

static void tus_release_glyph_cb(const lv_font_t *font, lv_font_glyph_dsc_t *g_dsc) {
    (void)font;
    tus_glyph_entry_t *e = (tus_glyph_entry_t *)g_dsc->entry;
    if (e == NULL) {
        return;
    }
    if (e->draw_buf != NULL) {
        lv_draw_buf_destroy(e->draw_buf);
    }
    tf_free_bitmap(&e->bm);
    free(e);
    g_dsc->entry = NULL;
}

lv_font_t *tus_lvgl_font_create(const char *path, int px_size) {
    struct tf_font *tf = tf_open(path);
    if (tf == NULL) {
        return NULL;
    }

    tus_font_dsc_t *fd = calloc(1, sizeof(*fd));
    if (fd == NULL) {
        tf_close(tf);
        return NULL;
    }
    fd->font = tf;
    fd->size = TF_FROM_INT(px_size);

    lv_font_t *font = calloc(1, sizeof(*font));
    if (font == NULL) {
        free(fd);
        tf_close(tf);
        return NULL;
    }

    struct tf_metrics m;
    tf_metrics_at(tf, fd->size, &m);

    font->dsc = fd;
    font->get_glyph_dsc = tus_get_glyph_dsc_cb;
    font->get_glyph_bitmap = tus_get_glyph_bitmap_cb;
    font->release_glyph = tus_release_glyph_cb;
    font->line_height = TF_TO_INT(m.height);
    font->base_line = TF_TO_INT(m.descent);
    font->subpx = LV_FONT_SUBPX_NONE;
    font->kerning = LV_FONT_KERNING_NORMAL;
    font->underline_position = (int8_t)(-TF_TO_INT(m.descent) / 2);
    font->underline_thickness = 1;

    return font;
}

void tus_lvgl_font_destroy(lv_font_t *font) {
    if (font == NULL) {
        return;
    }
    tus_font_dsc_t *fd = (tus_font_dsc_t *)font->dsc;
    if (fd != NULL) {
        tf_close(fd->font);
        free(fd);
    }
    free(font);
}
