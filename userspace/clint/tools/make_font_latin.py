#!/usr/bin/env python3
"""
make_font_latin.py - build font_latin.h from the ASCII font next door

Clint's font is the kernel's 8x16 VGA font, which stops at 0x7F. A
page written in any European language needs more than that, and the
honest way to get it without importing a second font (and a second
licence) is to build the letters the same way a typewriter did: take
the base letter that is already there and put a mark over it.

Every glyph below is therefore composed - a base letter from
font8x16.h, plus a diacritic drawn here in the rows the letter leaves
empty (0..1 above a capital, 2..3 above a lowercase x-height,
11..13 below both). Nothing is copied from another font.

    python3 userspace/clint/tools/make_font_latin.py \\
        > kernel/drivers/fb/font_latin.h
"""

import re
import sys

FONT_H = "kernel/drivers/fb/font8x16.h"

def load_ascii():
    src = open(FONT_H).read()
    glyphs = {}
    for m in re.finditer(r'/\* (0x[0-9A-Fa-f]{2})[^*]*\*/\s*\{([^}]*)\}', src):
        rows = [int(x, 0) for x in m.group(2).split(',')]
        if len(rows) != 16:
            raise SystemExit("font8x16.h: glyph %s is not 16 rows" % m.group(1))
        glyphs[chr(int(m.group(1), 16))] = rows
    if 'A' not in glyphs:
        raise SystemExit("font8x16.h: could not be parsed")
    return glyphs

ASCII = load_ascii()

# Diacritics, as the rows they occupy. Each is drawn once and used by
# every letter that takes it, so they cannot drift apart.
ABOVE = {
    "grave":     [0x60, 0x30],
    "acute":     [0x0C, 0x18],
    "circum":    [0x18, 0x24],
    "tilde":     [0x36, 0x6C],
    "diaer":     [0x66, 0x00],
    "ring":      [0x3C, 0x24],
    "breve":     [0x42, 0x3C],
    "caron":     [0x24, 0x18],
    "macron":    [0x7E, 0x00],
    "dot":       [0x18, 0x18],
    "dacute":    [0x1B, 0x36],
}

# Below the baseline: the cedilla and the comma some languages use for
# the same sound.
BELOW = {
    "cedilla": {11: 0x18, 12: 0x30, 13: 0x38},
    "comma":   {11: 0x18, 12: 0x30, 13: 0x00},
}

CAP_ROWS = (0, 1)      # a capital starts at row 2
LOW_ROWS = (2, 3)      # a lowercase x-height starts at row 4

def compose(base, above=None, below=None, clear=()):
    rows = list(ASCII[base])
    for r in clear:
        rows[r] = 0
    if above is not None:
        top = CAP_ROWS if base.isupper() else LOW_ROWS
        art = ABOVE[above]
        for i, bits in enumerate(art):
            rows[top[0] + i] |= bits
    if below is not None:
        for r, bits in BELOW[below].items():
            rows[r] |= bits
    return rows

# The dotless i, which the accented i letters are built on: the dot
# lives exactly where the accent has to go.
DOTLESS_I = compose('i', clear=LOW_ROWS)

def i_with(above):
    rows = list(DOTLESS_I)
    for i, bits in enumerate(ABOVE[above]):
        rows[LOW_ROWS[0] + i] |= bits
    return rows

GLYPHS = []   # (codepoint, name, rows)

def add(cp, name, rows):
    GLYPHS.append((cp, name, rows))

def accented(pairs):
    for cp, name, base, above, below in pairs:
        if base == 'i' and above is not None:
            add(cp, name, i_with(above))
        else:
            add(cp, name, compose(base, above, below))

accented([
    (0x00C0, "A grave",      'A', "grave",  None),
    (0x00C1, "A acute",      'A', "acute",  None),
    (0x00C2, "A circumflex", 'A', "circum", None),
    (0x00C3, "A tilde",      'A', "tilde",  None),
    (0x00C4, "A diaeresis",  'A', "diaer",  None),
    (0x00C5, "A ring",       'A', "ring",   None),
    (0x00C7, "C cedilla",    'C', None,     "cedilla"),
    (0x00C8, "E grave",      'E', "grave",  None),
    (0x00C9, "E acute",      'E', "acute",  None),
    (0x00CA, "E circumflex", 'E', "circum", None),
    (0x00CB, "E diaeresis",  'E', "diaer",  None),
    (0x00CC, "I grave",      'I', "grave",  None),
    (0x00CD, "I acute",      'I', "acute",  None),
    (0x00CE, "I circumflex", 'I', "circum", None),
    (0x00CF, "I diaeresis",  'I', "diaer",  None),
    (0x00D1, "N tilde",      'N', "tilde",  None),
    (0x00D2, "O grave",      'O', "grave",  None),
    (0x00D3, "O acute",      'O', "acute",  None),
    (0x00D4, "O circumflex", 'O', "circum", None),
    (0x00D5, "O tilde",      'O', "tilde",  None),
    (0x00D6, "O diaeresis",  'O', "diaer",  None),
    (0x00D9, "U grave",      'U', "grave",  None),
    (0x00DA, "U acute",      'U', "acute",  None),
    (0x00DB, "U circumflex", 'U', "circum", None),
    (0x00DC, "U diaeresis",  'U', "diaer",  None),
    (0x00DD, "Y acute",      'Y', "acute",  None),

    (0x00E0, "a grave",      'a', "grave",  None),
    (0x00E1, "a acute",      'a', "acute",  None),
    (0x00E2, "a circumflex", 'a', "circum", None),
    (0x00E3, "a tilde",      'a', "tilde",  None),
    (0x00E4, "a diaeresis",  'a', "diaer",  None),
    (0x00E5, "a ring",       'a', "ring",   None),
    (0x00E7, "c cedilla",    'c', None,     "cedilla"),
    (0x00E8, "e grave",      'e', "grave",  None),
    (0x00E9, "e acute",      'e', "acute",  None),
    (0x00EA, "e circumflex", 'e', "circum", None),
    (0x00EB, "e diaeresis",  'e', "diaer",  None),
    (0x00EC, "i grave",      'i', "grave",  None),
    (0x00ED, "i acute",      'i', "acute",  None),
    (0x00EE, "i circumflex", 'i', "circum", None),
    (0x00EF, "i diaeresis",  'i', "diaer",  None),
    (0x00F1, "n tilde",      'n', "tilde",  None),
    (0x00F2, "o grave",      'o', "grave",  None),
    (0x00F3, "o acute",      'o', "acute",  None),
    (0x00F4, "o circumflex", 'o', "circum", None),
    (0x00F5, "o tilde",      'o', "tilde",  None),
    (0x00F6, "o diaeresis",  'o', "diaer",  None),
    (0x00F9, "u grave",      'u', "grave",  None),
    (0x00FA, "u acute",      'u', "acute",  None),
    (0x00FB, "u circumflex", 'u', "circum", None),
    (0x00FC, "u diaeresis",  'u', "diaer",  None),
    (0x00FD, "y acute",      'y', "acute",  None),
    (0x00FF, "y diaeresis",  'y', "diaer",  None),

    # Central European
    (0x0106, "C acute",      'C', "acute",  None),
    (0x0107, "c acute",      'c', "acute",  None),
    (0x010C, "C caron",      'C', "caron",  None),
    (0x010D, "c caron",      'c', "caron",  None),
    (0x0112, "E macron",     'E', "macron", None),
    (0x0113, "e macron",     'e', "macron", None),
    (0x0143, "N acute",      'N', "acute",  None),
    (0x0144, "n acute",      'n', "acute",  None),
    (0x014C, "O macron",     'O', "macron", None),
    (0x014D, "o macron",     'o', "macron", None),
    (0x0150, "O double acute", 'O', "dacute", None),
    (0x0151, "o double acute", 'o', "dacute", None),
    (0x015A, "S acute",      'S', "acute",  None),
    (0x015B, "s acute",      's', "acute",  None),
    (0x0160, "S caron",      'S', "caron",  None),
    (0x0161, "s caron",      's', "caron",  None),
    (0x0170, "U double acute", 'U', "dacute", None),
    (0x0171, "u double acute", 'u', "dacute", None),
    (0x0179, "Z acute",      'Z', "acute",  None),
    (0x017A, "z acute",      'z', "acute",  None),
    (0x017B, "Z dot",        'Z', "dot",    None),
    (0x017C, "z dot",        'z', "dot",    None),
    (0x017D, "Z caron",      'Z', "caron",  None),
    (0x017E, "z caron",      'z', "caron",  None),

    # Turkish. The letters TUS is written among.
    (0x011E, "G breve",      'G', "breve",  None),
    (0x011F, "g breve",      'g', "breve",  None),
    (0x015E, "S cedilla",    'S', None,     "cedilla"),
    (0x015F, "s cedilla",    's', None,     "cedilla"),
    (0x0218, "S comma",      'S', None,     "comma"),
    (0x0219, "s comma",      's', None,     "comma"),
    (0x021A, "T comma",      'T', None,     "comma"),
    (0x021B, "t comma",      't', None,     "comma"),
])

# ---- the symbols the layouts type ----
#
# A European keyboard reaches more than letters: AltGr on a Turkish
# layout types a euro sign and a pound sign, a German one types the
# section mark, a Spanish one an inverted question mark. Without
# glyphs those keys produce a blank cell, which looks exactly like a
# key that does not work.
#
# Where a symbol IS another character rearranged, it is derived, the
# way the accented letters are - the inverted marks are the upright
# ones flipped, the currency signs are their letters with bars. The
# rest are drawn here, eight columns wide, in the same style: '#' is a
# lit pixel, '.' is not, so a row can be read at a glance.

def art(*rows):
    if len(rows) != 16:
        raise SystemExit("a glyph is 16 rows, got %d" % len(rows))
    out = []
    for r in rows:
        if len(r) != 8:
            raise SystemExit("a row is 8 columns, got %r" % r)
        bits = 0
        for i, c in enumerate(r):
            if c == '#':
                bits |= 0x80 >> i
        out.append(bits)
    return out

def flipped(base):
    """Vertically mirrored, for the inverted punctuation Spanish uses.
    The glyph is flipped inside rows 0..14 so it keeps its baseline
    rather than floating a row high."""
    rows = list(ASCII[base])
    body = rows[0:15][::-1]
    return body + [0]

add(0x00A1, "inverted !",   flipped('!'))
add(0x00BF, "inverted ?",   flipped('?'))

def with_bars(base, bar_rows, bits=0xFF):
    """A letter with horizontal bars struck through it: the euro sign
    is a C with two, the yen a Y with two, the cent a c with one."""
    rows = list(ASCII[base])
    for r in bar_rows:
        rows[r] |= bits
    return rows

add(0x20AC, "euro",  with_bars('C', (5, 8), 0x7E))
add(0x00A5, "yen",   with_bars('Y', (8, 10), 0x7E))

# The cent is a c with a stroke through it, which has to reach above
# and below the letter to read as one.
def struck(base, col_bits, rows_range):
    rows = list(ASCII[base])
    for r in rows_range:
        rows[r] |= col_bits
    return rows

add(0x00A2, "cent", struck('c', 0x18, range(3, 13)))

# The micro sign is a u with a descender on its left stem - which is
# what it is: a Greek mu built from the Latin letter.
def micro():
    rows = list(ASCII['u'])
    for r in range(11, 14):
        rows[r] |= 0x60
    return rows

add(0x00B5, "micro", micro())

# Spacing accents: the marks the accented letters use, standing alone
# in the middle of the cell where a typewriter put them.
for cp, name, mark in ((0x00B4, "acute accent",  "acute"),
                       (0x00A8, "diaeresis",     "diaer"),
                       (0x00B0, "degree",        "ring")):
    rows = [0] * 16
    for i, bits in enumerate(ABOVE[mark]):
        rows[2 + i] = bits
    add(cp, name, rows)

# The cedilla on its own keeps the position it has under a letter.
rows = [0] * 16
for r, bits in BELOW["cedilla"].items():
    rows[r] = bits
add(0x00B8, "cedilla", rows)

add(0x00A3, "pound", art(
    "........",
    "........",
    "...####.",
    "..##..##",
    "..##....",
    "..##....",
    ".#####..",
    "..##....",
    "..##....",
    "..##....",
    "..##..#.",
    "#######.",
    "........",
    "........",
    "........",
    "........"))

add(0x00A7, "section", art(
    "........",
    "..####..",
    ".##..##.",
    ".##.....",
    "..###...",
    ".##.##..",
    "##...##.",
    "##...##.",
    ".##.##..",
    "...###..",
    ".....##.",
    ".##..##.",
    "..####..",
    "........",
    "........",
    "........"))

add(0x00AC, "not sign", art(
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "#######.",
    "......#.",
    "......#.",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00A4, "currency", art(
    "........",
    "........",
    "........",
    "#.....#.",
    ".#####..",
    ".##..##.",
    ".##..##.",
    ".##..##.",
    ".#####..",
    "#.....#.",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00DF, "sharp s", art(
    "........",
    "........",
    "..####..",
    ".##..##.",
    ".##..##.",
    ".##.##..",
    ".##.##..",
    ".##..##.",
    ".##..##.",
    ".##..##.",
    ".##.###.",
    ".##.....",
    ".##.....",
    "........",
    "........",
    "........"))

add(0x00C6, "AE", art(
    "........",
    "........",
    "..######",
    ".###..#.",
    "###...#.",
    "##....#.",
    "##.#####",
    "##.#..#.",
    "#######.",
    "##....#.",
    "##....#.",
    "##....##",
    "##...###",
    "........",
    "........",
    "........"))

add(0x00E6, "ae", art(
    "........",
    "........",
    "........",
    "........",
    ".####.#.",
    "....####",
    ".#######",
    "##..#...",
    "##..#...",
    "##..####",
    ".#######",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00AA, "ordinal a", art(
    "..####..",
    ".....##.",
    "..#####.",
    ".##..##.",
    "..#####.",
    "........",
    ".######.",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00BA, "ordinal o", art(
    "..####..",
    ".##..##.",
    ".##..##.",
    ".##..##.",
    "..####..",
    "........",
    ".######.",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

# Superscript digits, drawn small enough to read as raised.
add(0x00B2, "superscript 2", art(
    "..###...",
    ".##.##..",
    "....##..",
    "...##...",
    "..##....",
    ".#####..",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00B3, "superscript 3", art(
    ".####...",
    "....##..",
    "..###...",
    "....##..",
    ".##.##..",
    "..###...",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00B9, "superscript 1", art(
    "...##...",
    "..###...",
    "...##...",
    "...##...",
    "...##...",
    "..####..",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00BD, "one half", art(
    "........",
    ".##...#.",
    "###..#..",
    ".##..#..",
    ".##.#...",
    "####....",
    "....#.#.",
    "...#.##.",
    "...#####",
    "..#...#.",
    "..#...#.",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00BC, "one quarter", art(
    "........",
    ".##...#.",
    "###..#..",
    ".##..#..",
    ".##.#...",
    "####....",
    "...##.#.",
    "..#.#.#.",
    "..#####.",
    "....#.#.",
    "....#.#.",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00B7, "middle dot", art(
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "...##...",
    "...##...",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00B1, "plus minus", art(
    "........",
    "........",
    "...##...",
    "...##...",
    ".######.",
    "...##...",
    "...##...",
    "........",
    ".######.",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00D7, "multiply", art(
    "........",
    "........",
    "........",
    "........",
    "........",
    "##....##",
    ".##..##.",
    "..####..",
    "..####..",
    ".##..##.",
    "##....##",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00F7, "divide", art(
    "........",
    "........",
    "........",
    "...##...",
    "...##...",
    "........",
    "########",
    "........",
    "...##...",
    "...##...",
    "........",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00AB, "left guillemet", art(
    "........",
    "........",
    "........",
    "........",
    "...#..#.",
    "..#..#..",
    ".#..#...",
    "#..#....",
    ".#..#...",
    "..#..#..",
    "...#..#.",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x00BB, "right guillemet", art(
    "........",
    "........",
    "........",
    "........",
    "#..#....",
    ".#..#...",
    "..#..#..",
    "...#..#.",
    "..#..#..",
    ".#..#...",
    "#..#....",
    "........",
    "........",
    "........",
    "........",
    "........"))

add(0x0130, "I dot above", compose('I', "dot"))
add(0x0131, "dotless i", DOTLESS_I)

# Braille Patterns (U+2800-U+28FF). Not composed from font8x16 like
# everything above: a braille cell is not a letterform, it is a 2x4
# grid of eight dots, and the codepoint itself IS the bitmap (bit N
# set means dot N+1 is raised - ISO/TR 11548-1, the same encoding
# Unicode uses). Drawing the dot at the position its own bit encodes
# is a correct renderer for any braille codepoint; no font's letter
# shapes enter into it, so unlike the accented letters this is
# generated by formula, not composed from a base glyph.
BRAILLE_DOTS = [
    (0, 0), (1, 0), (2, 0), (0, 1),   # dots 1,2,3,4
    (1, 1), (2, 1), (3, 0), (3, 1),   # dots 5,6,7,8
]

def braille_rows(cp):
    rows = [0] * 16
    bits = cp - 0x2800
    for bit, (dot_row, dot_col) in enumerate(BRAILLE_DOTS):
        if not (bits & (1 << bit)):
            continue
        r0 = dot_row * 4 + 1
        c0 = dot_col * 4 + 1
        mask = 0
        for c in range(c0, c0 + 3):
            mask |= 1 << (7 - c)
        for r in range(r0, r0 + 3):
            rows[r] |= mask
    return rows

for cp in range(0x2800, 0x2900):
    add(cp, "braille %02x" % (cp - 0x2800), braille_rows(cp))

GLYPHS.sort()

out = sys.stdout
out.write("""/*
 * font_latin.h - the characters the 8x16 ASCII font does not have
 *
 * GENERATED by userspace/clint/tools/make_font_latin.py - edit that,
 * not this.
 *
 * Every accented letter here is the ASCII font's own letter with a
 * mark added in the rows it leaves empty, which is why they sit
 * together on a line without looking borrowed. The symbols a European
 * keyboard types are here too - AltGr reaches a euro sign, a pound
 * sign, an inverted question mark, and without a glyph such a key
 * produces a blank cell that looks exactly like a key that does not
 * work. The table is sorted by codepoint and searched by halving.
 *
 * Lives next to font8x16.h in the kernel tree because both need it:
 * the framebuffer console draws Turkish, German and French text, and
 * so does the browser. One table, so the console and Clint can never
 * disagree about a glyph.
 */
#ifndef TUS_FONT_LATIN_H
#define TUS_FONT_LATIN_H

#include <stddef.h>
#include <stdint.h>

#include "font8x16.h"

struct latin_glyph {
    uint32_t cp;
    uint8_t rows[16];
};

static const struct latin_glyph font_latin[] = {
""")
for cp, name, rows in GLYPHS:
    out.write("    /* U+%04X %-16s */ { 0x%04X, {%s} },\n" %
              (cp, name, cp, ",".join("0x%02X" % r for r in rows)))
out.write("""};

#define FONT_LATIN_COUNT ((int)(sizeof(font_latin) / sizeof(font_latin[0])))

/* The 16 rows of `cp`, or NULL when nothing here draws it.
 *
 * Two tables behind one lookup: ASCII by subtraction, everything else
 * by halving the sorted table above. A caller that gets NULL has to
 * decide what to show instead - the console draws a space, the
 * browser folds the character to ASCII before it reaches the canvas.
 */
static inline const uint8_t *font_glyph_rows(uint32_t cp) {
    if (cp >= FONT_FIRST && cp <= FONT_LAST) {
        return font8x16[cp - FONT_FIRST];
    }
    int lo = 0, hi = FONT_LATIN_COUNT - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (font_latin[mid].cp == cp) {
            return font_latin[mid].rows;
        }
        if (font_latin[mid].cp < cp) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return NULL;
}

#endif /* TUS_FONT_LATIN_H */
""")
