/*
 * charset.h - what the bytes of a page actually mean
 *
 * The parser, the layout and the font all speak UTF-8, so a document
 * that is not UTF-8 has to become it before anything looks at it.
 * Most of the web is UTF-8 and needs nothing; the pages that are not
 * are almost always one of a handful of single-byte encodings, and
 * those are a lookup table each.
 *
 * Getting this wrong is not subtle. A Turkish page in ISO-8859-9 read
 * as anything else says "týklayýn" where it meant "tıklayın".
 */

#ifndef CLINT_CHARSET_H
#define CLINT_CHARSET_H

#include <stddef.h>

/*
 * The encoding this document is in: the Content-Type header first,
 * because it comes from the server, then the document's own <meta>.
 * Writes a lowercased label into `out` ("utf-8" when nothing says
 * otherwise).
 */
void charset_of(const char *content_type, const char *body, size_t len,
                char *out, size_t size);

/*
 * Convert `in` to UTF-8. Returns a new buffer the caller frees, or
 * NULL when the bytes are already UTF-8 (or the encoding is one Clint
 * has no table for, where leaving them alone is the least wrong
 * thing).
 */
char *charset_to_utf8(const char *label, const char *in, size_t len,
                      size_t *out_len);

#endif /* CLINT_CHARSET_H */
