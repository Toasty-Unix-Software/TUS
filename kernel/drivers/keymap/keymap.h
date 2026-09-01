/*
 * keymap.h - keyboard layouts
 *
 * A scancode is not a letter. Scancode 0x1E is "the key one to the
 * right of Caps Lock"; on a US keyboard that is 'a', on a French one
 * it is 'q', and on a Turkish F keyboard it is 'u'. The keyboard
 * driver used to have the US answer built into it, which is why TUS
 * could not type Turkish.
 *
 * This module is the answer table, kept apart from the drivers so
 * that both of them - PS/2 and USB HID - go through one layout.
 *
 * FOUR LEVELS
 *
 * Each layout gives four codepoints per scancode, chosen by the
 * modifiers held:
 *
 *   0  plain            a
 *   1  Shift            A
 *   2  AltGr            @   (right Alt; how European layouts reach
 *                            the characters that do not fit)
 *   3  Shift + AltGr
 *
 * Caps Lock is not a fifth level. On every real layout it means
 * "Shift, but only for letters", so it swaps levels 0 and 1 for
 * alphabetic keys and leaves the digit row alone. Doing it that way
 * rather than by upper-casing the result is what makes the Turkish
 * i work: the 'i' key's shift level is dotted capital I (U+0130) and
 * the dotless-i key's is plain I, and no case-mapping function gets
 * that right without being told which language it is in.
 *
 * DEAD KEYS
 *
 * German, French and Spanish reach their accented letters by pressing
 * an accent that produces nothing and then a letter. A table entry in
 * the range KEYMAP_DEAD..KEYMAP_DEAD+0xFF is such a key; the low byte
 * is the offset of the combining mark from U+0300, and the driver
 * holds it until the next key arrives (see keymap_compose).
 *
 * A dead key followed by space produces the accent on its own, and a
 * dead key followed by something that does not combine produces both
 * characters - which is what every other system does, and what a user
 * who pressed the wrong key expects to see.
 */

#ifndef TUS_DRIVERS_KEYMAP_H
#define TUS_DRIVERS_KEYMAP_H

#include <stdbool.h>
#include <stdint.h>

/* Table entries at or above this are dead keys, not characters.
 * The Unicode private use area, which no layout wants to type. */
#define KEYMAP_DEAD      0xE000u
#define KEYMAP_DEAD_MAX  0xE0FFu
/* The combining mark a dead-key entry stands for. */
#define KEYMAP_MARK(v)   (0x0300u + ((v) - KEYMAP_DEAD))
/* Build a dead-key entry from a combining mark. */
#define KEYMAP_DK(mark)  (KEYMAP_DEAD + ((mark) - 0x0300u))

/* Combining marks the layouts here use, for readability in the tables. */
#define MARK_GRAVE      0x0300
#define MARK_ACUTE      0x0301
#define MARK_CIRCUMFLEX 0x0302
#define MARK_TILDE      0x0303
#define MARK_DIAERESIS  0x0308
#define MARK_RING       0x030A
#define MARK_CARON      0x030C
#define MARK_CEDILLA    0x0327

/* Select a layout by name ("us", "tr", "tr-f", "de", "fr", "es").
 * Returns 0, or -1 when there is no such layout (the current one is
 * left alone). */
int keymap_set(const char *name);

/* The name of the layout in use. */
const char *keymap_name(void);

/* Walk the available layouts, `index` from 0, for `keymap -l` and the
 * installer. Returns false past the end. */
bool keymap_at(int index, const char **name, const char **description);

/* The codepoint for one scancode-set-1 make code under the given
 * modifiers, or 0 when the key produces no character.
 *
 * A return value in KEYMAP_DEAD..KEYMAP_DEAD_MAX is a dead key, not
 * something to print. */
uint32_t keymap_lookup(uint8_t scancode, bool shift, bool altgr, bool caps);

/* Combine a pending dead key with the character that followed it.
 *
 * Returns the composed codepoint, or 0 when the two do not combine -
 * in which case the caller emits the accent as a standalone character
 * and then the base, so nothing a user typed is silently swallowed. */
uint32_t keymap_compose(uint32_t mark, uint32_t base);

/* The standalone (spacing) form of a combining mark, for a dead key
 * followed by space. Returns 0 if there is no such character. */
uint32_t keymap_spacing(uint32_t mark);

#endif /* TUS_DRIVERS_KEYMAP_H */
