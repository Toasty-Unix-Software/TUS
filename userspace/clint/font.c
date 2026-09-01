/*
 * font.c - see font.h
 */

#include "font.h"

/* The kernel's fonts, included rather than copied: one ASCII table and
 * one accented table, so the browser and the console can never
 * disagree about a glyph. font_latin.h pulls in font8x16.h itself. */
#include "../../kernel/drivers/fb/font_latin.h"

const uint8_t *font_glyph(uint32_t cp) {
    return font_glyph_rows(cp);
}
