/*
 * jsregex.h - the regular expressions JavaScript strings are built on
 *
 * `text.replace(/\s+/g, " ")` is not an exotic thing for a page to
 * do; it is how half of them handle text. So the interpreter needs a
 * matcher, and this is one: a small backtracking engine over a parsed
 * pattern, with the syntax pages actually use and none of the syntax
 * they do not (no lookbehind, no named groups, no unicode property
 * escapes).
 */

#ifndef CLINT_JSREGEX_H
#define CLINT_JSREGEX_H

#define RX_MAX_GROUPS 10

struct rx;

/* Compile `pattern` with `flags` ("g", "i", "m" in any order).
 * Returns NULL when the pattern is not one this engine understands. */
struct rx *rx_compile(const char *pattern, const char *flags);
void rx_free(struct rx *rx);

/*
 * Find the first match at or after `start`. Returns 1 with the match
 * bounds written to mstart and mend, and each capture group as a
 * start/end pair in caps (-1 when a group did not take part), or 0
 * for no match.
 */
int rx_search(struct rx *rx, const char *text, int start, int *mstart,
              int *mend, int *caps);

int rx_groups(const struct rx *rx);
int rx_is_global(const struct rx *rx);
int rx_is_icase(const struct rx *rx);

#endif /* CLINT_JSREGEX_H */
