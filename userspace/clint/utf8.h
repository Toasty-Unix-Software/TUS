/*
 * utf8.h - one decoder, for the two passes that need one
 *
 * Layout decides what a character is (a glyph, or something to be
 * folded into ASCII) and painting draws it. Both walk the same UTF-8,
 * so they walk it with the same function rather than each with a
 * copy that could be subtly different about a truncated sequence.
 */

#ifndef CLINT_UTF8_H
#define CLINT_UTF8_H

#include <stddef.h>
#include <stdint.h>

/*
 * Decode the character at `s`, which is `len` bytes from its end.
 * Returns how many bytes it took (never zero for a non-empty string)
 * and puts the codepoint in *cp. A byte that starts nothing valid is
 * reported as itself, one byte long: that is what keeps a page of
 * mislabelled bytes from swallowing the text after it.
 */
static inline size_t utf8_next(const char *s, size_t len, uint32_t *cp) {
    const unsigned char *p = (const unsigned char *)s;
    if (len == 0) {
        *cp = 0;
        return 0;
    }

    unsigned char c = p[0];
    int extra;
    uint32_t value;
    if (c < 0x80) { *cp = c; return 1; }
    else if ((c & 0xE0) == 0xC0) { value = c & 0x1Fu; extra = 1; }
    else if ((c & 0xF0) == 0xE0) { value = c & 0x0Fu; extra = 2; }
    else if ((c & 0xF8) == 0xF0) { value = c & 0x07u; extra = 3; }
    else { *cp = c; return 1; }

    if (len < (size_t)extra + 1) { *cp = c; return 1; }
    for (int i = 1; i <= extra; i++) {
        if ((p[i] & 0xC0) != 0x80) { *cp = c; return 1; }
        value = (value << 6) | (p[i] & 0x3Fu);
    }
    *cp = value;
    return (size_t)extra + 1;
}

/* How many characters (not bytes) `len` bytes of UTF-8 hold - which
 * is how wide they are, in a font where every glyph is one cell. */
static inline int utf8_columns(const char *s, size_t len) {
    int n = 0;
    for (size_t i = 0; i < len; i++) {
        if (((unsigned char)s[i] & 0xC0) != 0x80) n++;
    }
    return n;
}

#endif /* CLINT_UTF8_H */
