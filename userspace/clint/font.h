/*
 * font.h - the glyphs Clint can draw
 *
 * Two tables behind one lookup: the kernel's 8x16 ASCII font, and the
 * accented letters composed from it in font_latin.h. Layout asks
 * whether a character has a glyph (if it has not, the character is
 * folded to ASCII before it ever reaches the canvas) and painting
 * asks for the rows. Both get the same answer from the same place.
 */

#ifndef CLINT_FONT_H
#define CLINT_FONT_H

#include <stddef.h>
#include <stdint.h>

/* The 16 rows of `cp`, or NULL when there is no glyph for it. */
const uint8_t *font_glyph(uint32_t cp);

#endif /* CLINT_FONT_H */
