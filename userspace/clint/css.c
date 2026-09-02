/*
 * css.c - see css.h
 *
 * A stylesheet is a list of rules; a rule is a list of selectors and
 * a list of declarations. Matching an element means walking the
 * selectors right to left, which is the order that fails fastest: the
 * rightmost part has to match the element itself, and most rules are
 * ruled out on that alone.
 *
 * The cascade here is the real one, minus the parts that need
 * machinery Clint has no use for. Rules are sorted by specificity and
 * then by document order, applied in that order, and the style
 * attribute goes on last. !important is honoured because pages use it
 * to defeat exactly the kind of default stylesheet below.
 *
 * Inheritance is a property of the property: colour and text settings
 * pass down to children, boxes do not. That is the whole rule, and it
 * is applied in one pass down the tree.
 */

#include "css.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * The default stylesheet. Written as CSS rather than as C, because
 * this is exactly what a browser's defaults are - and because the
 * parser below is then the only thing that has to be right.
 */
static const char UA_STYLESHEET[] =
    "html, body { display: block; color: #101010; background: #ffffff; }"
    "body { margin: 8px; }"
    "div, p, ul, ol, dl, dd, blockquote, pre, form, fieldset, hr,"
    "header, footer, main, section, article, aside, nav, figure,"
    "figcaption, table, tr, address, center { display: block; }"
    "h1, h2, h3, h4, h5, h6 { display: block; font-weight: bold; }"
    "h1 { font-size: 40px; margin: 16px 0 16px 0; }"
    "h2 { font-size: 28px; margin: 14px 0 14px 0; }"
    "h3 { font-size: 24px; margin: 12px 0 12px 0; }"
    "h4, h5, h6 { font-size: 16px; margin: 10px 0 10px 0; }"
    "p { margin: 10px 0 10px 0; }"
    "blockquote { margin: 10px 24px 10px 24px; }"
    "ul, ol { margin: 10px 0 10px 0; padding: 0 0 0 32px; }"
    "li { display: list-item; }"
    "dd { margin: 0 0 0 32px; }"
    "b, strong, th { font-weight: bold; }"
    "i, em, cite, var, address { font-style: italic; }"
    "a { color: #1a4fa0; text-decoration: underline; }"
    "u, ins { text-decoration: underline; }"
    "pre, code, kbd, samp, tt { white-space: pre; }"
    "pre { margin: 10px 0 10px 0; background: #f4f4f4; padding: 8px;"
    "      border: 1px solid #dddddd; }"
    "code, kbd, samp, tt { background: #f4f4f4; }"
    "hr { margin: 10px 0 10px 0; border: 1px solid #cccccc; }"
    "center { text-align: center; }"
    /* A table cell is the one box in HTML that has always shrunk to
     * fit its contents; without that a two-button row is two bars
     * across the page. Cells laid out this way do not line up into
     * columns, but they do sit next to each other. */
    "th, td { display: inline-block; padding: 2px 6px 2px 6px; }"
    "table { margin: 8px 0 8px 0; }"
    "head, title, meta, link, script, style, base { display: none; }";

/* ---- text helpers ---- */

static char lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c;
}

static int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f';
}

static int ci_eq(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        if (lower(*a) != lower(*b)) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static int ci_eq_n(const char *a, size_t alen, const char *b) {
    size_t blen = strlen(b);
    if (alen != blen) return 0;
    for (size_t i = 0; i < alen; i++) {
        if (lower(a[i]) != lower(b[i])) return 0;
    }
    return 1;
}

static char *dup_range(const char *s, size_t n) {
    char *p = malloc(n + 1);
    if (p == NULL) return NULL;
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

/* ---- colours ---- */

static int hex_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

uint32_t css_parse_color(const char *text) {
    static const struct { const char *name; uint32_t rgb; } named[] = {
        { "black", 0x000000 },   { "white", 0xFFFFFF },
        { "red", 0xFF0000 },     { "green", 0x008000 },
        { "lime", 0x00FF00 },    { "blue", 0x0000FF },
        { "yellow", 0xFFFF00 },  { "cyan", 0x00FFFF },
        { "aqua", 0x00FFFF },    { "magenta", 0xFF00FF },
        { "fuchsia", 0xFF00FF }, { "gray", 0x808080 },
        { "grey", 0x808080 },    { "silver", 0xC0C0C0 },
        { "maroon", 0x800000 },  { "olive", 0x808000 },
        { "navy", 0x000080 },    { "teal", 0x008080 },
        { "purple", 0x800080 },  { "orange", 0xFFA500 },
        { "pink", 0xFFC0CB },    { "brown", 0xA52A2A },
        { "gold", 0xFFD700 },    { "beige", 0xF5F5DC },
        { "ivory", 0xFFFFF0 },   { "khaki", 0xF0E68C },
        { "lavender", 0xE6E6FA },{ "salmon", 0xFA8072 },
        { "tan", 0xD2B48C },     { "violet", 0xEE82EE },
        { "indigo", 0x4B0082 },  { "crimson", 0xDC143C },
        { "darkgray", 0xA9A9A9 },{ "darkgrey", 0xA9A9A9 },
        { "lightgray", 0xD3D3D3 }, { "lightgrey", 0xD3D3D3 },
        { "whitesmoke", 0xF5F5F5 }, { "transparent", CSS_NO_COLOR },
        { NULL, 0 }
    };

    while (is_space(*text)) text++;

    if (*text == '#') {
        const char *h = text + 1;
        size_t n = 0;
        while (hex_digit(h[n]) >= 0) n++;

        if (n == 3) {
            int r = hex_digit(h[0]), g = hex_digit(h[1]), b = hex_digit(h[2]);
            return (uint32_t)((r * 17) << 16 | (g * 17) << 8 | (b * 17));
        }
        if (n >= 6) {
            uint32_t v = 0;
            for (int i = 0; i < 6; i++) v = (v << 4) | (uint32_t)hex_digit(h[i]);
            return v;
        }
        return CSS_NO_COLOR;
    }

    if (strncmp(text, "rgb", 3) == 0) {
        const char *p = strchr(text, '(');
        if (p == NULL) return CSS_NO_COLOR;
        int c[3] = { 0, 0, 0 };
        p++;
        for (int i = 0; i < 3; i++) {
            while (is_space(*p) || *p == ',') p++;
            c[i] = atoi(p);
            if (c[i] < 0) c[i] = 0;
            if (c[i] > 255) c[i] = 255;
            while (*p != '\0' && *p != ',' && *p != ')') p++;
        }
        return (uint32_t)(c[0] << 16 | c[1] << 8 | c[2]);
    }

    size_t len = 0;
    while (text[len] != '\0' && !is_space(text[len])) len++;
    for (int i = 0; named[i].name != NULL; i++) {
        if (ci_eq_n(text, len, named[i].name)) return named[i].rgb;
    }
    return CSS_NO_COLOR;
}

/* ---- lengths ---- */

/*
 * A length the parser could not resolve - a percentage, a viewport
 * unit, a keyword. It is not zero: a declaration that cannot be
 * computed should be ignored, leaving whatever was there before,
 * rather than applied as nothing. "margin: 15vh auto" must not take
 * a page's margins away.
 */
#define CSS_UNRESOLVED (-2147483647 - 1)

/*
 * A length in pixels. Clint's character cell is 8x16, so `em` and
 * `rem` are 16 pixels and a percentage of an unknown container is not
 * something this layout engine can answer - those come back as
 * `fallback`, which for a width means "auto".
 */
static int css_length(const char *text, int fallback) {
    while (is_space(*text)) text++;
    if (*text == '\0') return fallback;

    int negative = 0;
    if (*text == '-') {
        negative = 1;
        text++;
    } else if (*text == '+') {
        text++;
    }

    int value = 0, digits = 0;
    while (*text >= '0' && *text <= '9') {
        value = value * 10 + (*text - '0');
        text++;
        digits++;
    }
    if (digits == 0) return fallback;

    /* A fraction only matters for rounding at these sizes. */
    int frac = 0;
    if (*text == '.') {
        text++;
        if (*text >= '0' && *text <= '9') frac = *text - '0';
        while (*text >= '0' && *text <= '9') text++;
    }

    while (is_space(*text)) text++;

    /* The fraction has to be folded in before the unit is applied,
     * not after: 1.5em is 24 pixels, and rounding first makes it
     * 17. Tenths carry it through the multiply exactly. */
    long tenths = (long)value * 10 + frac;

    /*
     * Just the unit, not the rest of the line. A shorthand hands this
     * function "16px 0 16px 0" and the unit is the two characters
     * after the number - comparing against everything that follows
     * makes every value of a multi-value shorthand unresolvable, and
     * the shorthand then silently does nothing.
     */
    char unit[8];
    size_t ulen = 0;
    while (ulen + 1 < sizeof(unit) &&
           ((*text >= 'a' && *text <= 'z') || (*text >= 'A' && *text <= 'Z') ||
            *text == '%')) {
        unit[ulen++] = *text++;
    }
    unit[ulen] = '\0';

    if (ulen == 0) {
        /* A bare number is a length only where HTML allows one (a
         * width attribute); everywhere else zero is the only legal
         * unitless value. Treating it as pixels is what those
         * attributes mean. */
        int out = (int)((tenths + 5) / 10);
        return negative ? -out : out;
    }
    if (ci_eq(unit, "px")) {
        /* nothing to convert */
    } else if (ci_eq(unit, "em") || ci_eq(unit, "rem")) {
        tenths *= 16;
    } else if (ci_eq(unit, "pt")) {
        tenths = tenths * 4 / 3;
    } else {
        /*
         * Percentages and the viewport units are relative to
         * something this function is not given: the containing block,
         * or the window. Answering with the number as pixels is how
         * "width: 60vw" becomes a sixty pixel column - so they come
         * back as the fallback instead, which for a width means auto
         * and for a margin means none.
         */
        return fallback;
    }

    int out = (int)((tenths + 5) / 10);
    return negative ? -out : out;
}

/* ---- the stylesheet ---- */

#define MAX_SIMPLE  4   /* parts of one descendant chain we keep */
#define MAX_CLASSES 4

struct simple_sel {
    char tag[DOM_NAME_MAX];               /* empty means any */
    char id[64];
    char classes[MAX_CLASSES][64];
    int nclasses;
};

struct selector {
    struct simple_sel part[MAX_SIMPLE];   /* ancestor .. element */
    int nparts;
    int specificity;
};

struct decl {
    char *name;
    char *value;
    int important;
    struct decl *next;
};

struct rule {
    struct selector sel;
    struct decl *decls, *last_decl;
    int order;
    struct rule *next;
};

struct sheet {
    struct rule *first, *last;
    int count;
};

static void sheet_free(struct sheet *s) {
    struct rule *r = s->first;
    while (r != NULL) {
        struct rule *next = r->next;
        struct decl *d = r->decls;
        while (d != NULL) {
            struct decl *dn = d->next;
            free(d->name);
            free(d->value);
            free(d);
            d = dn;
        }
        free(r);
        r = next;
    }
    s->first = s->last = NULL;
}

/* Split one compound selector ("div.note#x") into its parts. */
static void parse_simple(struct simple_sel *out, const char *text, size_t len) {
    memset(out, 0, sizeof(*out));

    size_t at = 0;
    while (at < len) {
        char kind = text[at];
        if (kind == '.' || kind == '#') at++;

        size_t start = at;
        while (at < len && text[at] != '.' && text[at] != '#') at++;
        size_t n = at - start;
        if (n == 0) continue;

        if (kind == '.') {
            if (out->nclasses < MAX_CLASSES) {
                size_t max = sizeof(out->classes[0]) - 1;
                if (n > max) n = max;
                memcpy(out->classes[out->nclasses], text + start, n);
                out->classes[out->nclasses][n] = '\0';
                out->nclasses++;
            }
        } else if (kind == '#') {
            size_t max = sizeof(out->id) - 1;
            if (n > max) n = max;
            memcpy(out->id, text + start, n);
            out->id[n] = '\0';
        } else {
            size_t max = sizeof(out->tag) - 1;
            if (n > max) n = max;
            for (size_t i = 0; i < n; i++) out->tag[i] = lower(text[start + i]);
            out->tag[n] = '\0';
            if (out->tag[0] == '*') out->tag[0] = '\0'; /* * matches all */
        }
    }
}

/*
 * Parse "main article p.lead" into a chain, keeping the rightmost
 * MAX_SIMPLE parts. Dropping the leftmost ancestors makes a selector
 * match more elements than it should, never fewer, and a chain that
 * deep is rare enough that the alternative - refusing the rule - is
 * the worse answer.
 */
static int parse_selector(struct selector *out, const char *text, size_t len) {
    memset(out, 0, sizeof(*out));

    /* Combinators other than descendant are treated as descendant:
     * ">" narrows what matches, so this is again the safe direction. */
    struct { size_t at, len; } part[16];
    int nparts = 0;

    size_t at = 0;
    while (at < len && nparts < 16) {
        while (at < len && (is_space(text[at]) || text[at] == '>' ||
                            text[at] == '+' || text[at] == '~')) {
            at++;
        }
        if (at >= len) break;

        size_t start = at;
        while (at < len && !is_space(text[at]) && text[at] != '>' &&
               text[at] != '+' && text[at] != '~') {
            at++;
        }
        /* Pseudo-classes and pseudo-elements are not evaluated; a
         * rule that hangs off one is dropped rather than applied to
         * everything the part before it matches. */
        for (size_t i = start; i < at; i++) {
            if (text[i] == ':' || text[i] == '[') return -1;
        }
        part[nparts].at = start;
        part[nparts].len = at - start;
        nparts++;
    }
    if (nparts == 0) return -1;

    int keep = nparts < MAX_SIMPLE ? nparts : MAX_SIMPLE;
    for (int i = 0; i < keep; i++) {
        const char *p = text + part[nparts - keep + i].at;
        parse_simple(&out->part[i], p, part[nparts - keep + i].len);
    }
    out->nparts = keep;

    for (int i = 0; i < keep; i++) {
        if (out->part[i].id[0] != '\0') out->specificity += 10000;
        out->specificity += 100 * out->part[i].nclasses;
        if (out->part[i].tag[0] != '\0') out->specificity += 1;
    }
    return 0;
}

static void add_decl(struct rule *r, const char *name, size_t nlen,
                     const char *value, size_t vlen) {
    /* Both ends: "display: list-item" hands the value over with the
     * space after the colon still on it, and every keyword here is
     * compared whole. Trimming only the tail leaves each one a
     * character too long, which fails silently - the declaration
     * parses, matches nothing, and falls through to the default. */
    while (nlen > 0 && is_space(name[0])) {
        name++;
        nlen--;
    }
    while (nlen > 0 && is_space(name[nlen - 1])) nlen--;
    while (vlen > 0 && is_space(value[0])) {
        value++;
        vlen--;
    }
    while (vlen > 0 && is_space(value[vlen - 1])) vlen--;
    if (nlen == 0 || vlen == 0) return;

    struct decl *d = calloc(1, sizeof(*d));
    if (d == NULL) return;

    d->name = dup_range(name, nlen);
    for (char *s = d->name; s != NULL && *s != '\0'; s++) *s = lower(*s);

    /* !important is part of the value as written; take it off and
     * remember it. */
    const char *bang = NULL;
    for (size_t i = 0; i + 10 <= vlen; i++) {
        if (value[i] == '!' && ci_eq_n(value + i + 1, 9, "important")) {
            bang = value + i;
            break;
        }
    }
    if (bang != NULL) {
        d->important = 1;
        vlen = (size_t)(bang - value);
        while (vlen > 0 && is_space(value[vlen - 1])) vlen--;
    }
    d->value = dup_range(value, vlen);

    /* In source order, because that is the order they win in: two
     * declarations of the same property in one rule are resolved by
     * which was written last, and pages rely on it - `display:
     * inline-box; display: inline-block` is one line of markup asking
     * for the second if the browser understands it. */
    if (r->last_decl != NULL) r->last_decl->next = d;
    else r->decls = d;
    r->last_decl = d;
}

/* Parse "a: b; c: d" into declarations on `r`. */
static void parse_decls(struct rule *r, const char *text, size_t len) {
    size_t at = 0;
    while (at < len) {
        while (at < len && (is_space(text[at]) || text[at] == ';')) at++;
        if (at >= len) break;

        size_t nstart = at;
        while (at < len && text[at] != ':' && text[at] != ';') at++;
        if (at >= len || text[at] != ':') {
            while (at < len && text[at] != ';') at++;
            continue;
        }
        size_t nlen = at - nstart;
        at++;

        size_t vstart = at;
        while (at < len && text[at] != ';') at++;
        add_decl(r, text + nstart, nlen, text + vstart, at - vstart);
    }
}

static void sheet_parse(struct sheet *s, const char *css, size_t len) {
    size_t at = 0;
    while (at < len) {
        while (at < len && is_space(css[at])) at++;
        if (at >= len) break;

        /* Comments can appear anywhere. */
        if (at + 1 < len && css[at] == '/' && css[at + 1] == '*') {
            at += 2;
            while (at + 1 < len && !(css[at] == '*' && css[at + 1] == '/')) at++;
            at = at + 2 < len ? at + 2 : len;
            continue;
        }

        /* @media, @import, @font-face: skipped whole. Honouring
         * @media would mean deciding which one Clint is, and the
         * answer changes with the window size. */
        if (css[at] == '@') {
            int depth = 0;
            while (at < len) {
                if (css[at] == '{') depth++;
                if (css[at] == '}') {
                    depth--;
                    if (depth == 0) {
                        at++;
                        break;
                    }
                }
                if (css[at] == ';' && depth == 0) {
                    at++;
                    break;
                }
                at++;
            }
            continue;
        }

        size_t sel_start = at;
        while (at < len && css[at] != '{') at++;
        if (at >= len) break;
        size_t sel_len = at - sel_start;
        at++;

        size_t body_start = at;
        int depth = 1;
        while (at < len && depth > 0) {
            if (css[at] == '{') depth++;
            if (css[at] == '}') depth--;
            if (depth > 0) at++;
        }
        size_t body_len = at - body_start;
        if (at < len) at++;

        /* One rule per selector in the group, so each carries its own
         * specificity. */
        size_t p = 0;
        while (p <= sel_len) {
            size_t start = p;
            while (p < sel_len && css[sel_start + p] != ',') p++;

            struct rule *r = calloc(1, sizeof(*r));
            if (r == NULL) return;
            if (parse_selector(&r->sel, css + sel_start + start, p - start) != 0) {
                free(r);
            } else {
                parse_decls(r, css + body_start, body_len);
                r->order = s->count++;
                if (s->last != NULL) {
                    s->last->next = r;
                } else {
                    s->first = r;
                }
                s->last = r;
            }
            if (p >= sel_len) break;
            p++;
        }
    }
}

/* ---- matching ---- */

static int has_class(const struct dom_node *n, const char *want) {
    const char *list = dom_attr(n, "class");
    if (list == NULL) return 0;

    size_t wlen = strlen(want);
    while (*list != '\0') {
        while (is_space(*list)) list++;
        size_t n2 = 0;
        while (list[n2] != '\0' && !is_space(list[n2])) n2++;
        if (n2 == wlen && strncmp(list, want, n2) == 0) return 1;
        list += n2;
    }
    return 0;
}

static int simple_matches(const struct simple_sel *s, const struct dom_node *n) {
    if (n->type != DOM_ELEMENT) return 0;
    if (s->tag[0] != '\0' && !ci_eq(s->tag, n->name)) return 0;

    if (s->id[0] != '\0') {
        const char *id = dom_attr(n, "id");
        if (id == NULL || !ci_eq(id, s->id)) return 0;
    }
    for (int i = 0; i < s->nclasses; i++) {
        if (!has_class(n, s->classes[i])) return 0;
    }
    return 1;
}

/* Right to left: the last part must match the element, and each part
 * before it must match some ancestor, in order. */
static int selector_matches(const struct selector *sel,
                            const struct dom_node *n) {
    if (!simple_matches(&sel->part[sel->nparts - 1], n)) return 0;

    const struct dom_node *at = n->parent;
    for (int i = sel->nparts - 2; i >= 0; i--) {
        int found = 0;
        while (at != NULL) {
            if (simple_matches(&sel->part[i], at)) {
                at = at->parent;
                found = 1;
                break;
            }
            at = at->parent;
        }
        if (!found) return 0;
    }
    return 1;
}

/* ---- applying declarations ---- */

/* Split a shorthand into four sides, CSS's 1/2/3/4 rule. Values that
 * did not resolve leave their side alone. */
static int split_sides(const char *value, int out[4]) {
    const char *p = value;
    int v[4], n = 0;

    while (*p != '\0' && n < 4) {
        while (is_space(*p)) p++;
        if (*p == '\0') break;
        v[n++] = css_length(p, CSS_UNRESOLVED);
        while (*p != '\0' && !is_space(*p)) p++;
    }
    if (n == 0) return 0;

    int side[4];
    if (n == 1) {
        side[0] = side[1] = side[2] = side[3] = v[0];
    } else if (n == 2) {
        side[SIDE_TOP] = side[SIDE_BOTTOM] = v[0];
        side[SIDE_RIGHT] = side[SIDE_LEFT] = v[1];
    } else if (n == 3) {
        side[SIDE_TOP] = v[0];
        side[SIDE_RIGHT] = side[SIDE_LEFT] = v[1];
        side[SIDE_BOTTOM] = v[2];
    } else {
        for (int i = 0; i < 4; i++) side[i] = v[i];
    }

    for (int i = 0; i < 4; i++) {
        if (side[i] != CSS_UNRESOLVED) out[i] = side[i];
    }
    return 1;
}

static void apply_decl(struct style *st, const char *name, const char *value) {
    if (ci_eq(name, "display")) {
        if (ci_eq(value, "none")) st->display = DISP_NONE;
        else if (ci_eq(value, "inline")) st->display = DISP_INLINE;
        else if (ci_eq(value, "inline-block") ||
                 ci_eq(value, "inline-flex")) {
            st->display = DISP_INLINE_BLOCK;
        }
        else if (ci_eq(value, "list-item")) st->display = DISP_LIST_ITEM;
        else if (ci_eq(value, "flex")) st->display = DISP_FLEX;
        else if (ci_eq(value, "grid")) st->display = DISP_GRID;
        else st->display = DISP_BLOCK; /* block, table... */
        return;
    }
    if (ci_eq(name, "flex-direction")) {
        st->flex_direction = ci_eq(value, "column") ? FLEX_COLUMN : FLEX_ROW;
        return;
    }
    if (ci_eq(name, "justify-content")) {
        if (ci_eq(value, "center")) st->justify_content = JUSTIFY_CENTER;
        else if (ci_eq(value, "flex-end") || ci_eq(value, "end")) {
            st->justify_content = JUSTIFY_END;
        } else if (ci_eq(value, "space-between") ||
                   ci_eq(value, "space-around") ||
                   ci_eq(value, "space-evenly")) {
            st->justify_content = JUSTIFY_BETWEEN;
        } else {
            st->justify_content = JUSTIFY_START;
        }
        return;
    }
    if (ci_eq(name, "align-items")) {
        if (ci_eq(value, "center")) st->align_items = ALIGN_ITEMS_CENTER;
        else if (ci_eq(value, "flex-end") || ci_eq(value, "end")) {
            st->align_items = ALIGN_ITEMS_END;
        } else if (ci_eq(value, "stretch")) st->align_items = ALIGN_ITEMS_STRETCH;
        else st->align_items = ALIGN_ITEMS_START;
        return;
    }
    if (ci_eq(name, "flex-grow") || ci_eq(name, "flex")) {
        /* `flex: 1` (or `flex: 1 1 auto`, etc.) and `flex-grow: 1`
         * both boil down to "how much of the free space" - only the
         * leading number is read. */
        st->flex_grow = css_length(value, st->flex_grow > 0 ? st->flex_grow : 1);
        return;
    }
    if (ci_eq(name, "grid-template-columns")) {
        int n = 0;
        const char *p = value;
        while (*p != '\0' && n < 16) {
            while (is_space(*p) || *p == ',') p++;
            if (*p == '\0') break;
            st->grid_columns[n++] = css_length(p, 100);
            while (*p != '\0' && !is_space(*p) && *p != ',') p++;
        }
        if (n > 0) st->grid_column_count = n;
        return;
    }
    if (ci_eq(name, "color")) {
        uint32_t c = css_parse_color(value);
        if (c != CSS_NO_COLOR) st->color = c;
        return;
    }
    if (ci_eq(name, "background") || ci_eq(name, "background-color")) {
        /* The `background` shorthand can carry images and positions;
         * the colour is the only part Clint can draw, so it is looked
         * for and the rest ignored. */
        uint32_t c = css_parse_color(value);
        if (c != CSS_NO_COLOR) st->background = c;
        return;
    }
    if (ci_eq(name, "font-weight")) {
        st->bold = ci_eq(value, "bold") || ci_eq(value, "bolder") ||
                   atoi(value) >= 600;
        return;
    }
    if (ci_eq(name, "font-style")) {
        st->italic = ci_eq(value, "italic") || ci_eq(value, "oblique");
        return;
    }
    if (ci_eq(name, "text-decoration") || ci_eq(name, "text-decoration-line")) {
        st->underline = strstr(value, "underline") != NULL;
        return;
    }
    if (ci_eq(name, "font-size")) {
        /*
         * There is one bitmap font, so every size lands on an integer
         * multiple of the 16-pixel cell. Sizes go through the same
         * length parser as everything else - which is what makes
         * "1.5em" and "24px" agree - and the keywords are mapped to
         * the pixel sizes browsers use for them.
         */
        int px = CSS_UNRESOLVED;
        if (ci_eq(value, "xx-large")) px = 40;
        else if (ci_eq(value, "x-large")) px = 32;
        else if (ci_eq(value, "large")) px = 24;
        else if (ci_eq(value, "medium")) px = 16;
        else if (ci_eq(value, "small") || ci_eq(value, "x-small") ||
                 ci_eq(value, "xx-small")) px = 13;
        else px = css_length(value, CSS_UNRESOLVED);

        if (px != CSS_UNRESOLVED) {
            st->font_scale = px >= 38 ? 3 : px >= 22 ? 2 : 1;
        }
        return;
    }
    if (ci_eq(name, "text-align")) {
        if (ci_eq(value, "center")) st->align = ALIGN_CENTER;
        else if (ci_eq(value, "right")) st->align = ALIGN_RIGHT;
        else st->align = ALIGN_LEFT;
        return;
    }
    if (ci_eq(name, "white-space")) {
        st->pre = ci_eq(value, "pre") || ci_eq(value, "pre-wrap");
        return;
    }
    if (ci_eq(name, "width")) {
        st->width = ci_eq(value, "auto") ? -1 : css_length(value, -1);
        return;
    }
    if (ci_eq(name, "margin")) {
        split_sides(value, st->margin);
        return;
    }
    if (ci_eq(name, "padding")) {
        split_sides(value, st->padding);
        return;
    }
    if (ci_eq(name, "border")) {
        /* "1px solid #ccc": a width, a style, a colour, in any
         * order. "none" turns the border off however it is spelled. */
        int w = css_length(value, CSS_UNRESOLVED);
        if (strstr(value, "none") != NULL || strstr(value, "hidden") != NULL) {
            w = 0;
        } else if (w == CSS_UNRESOLVED) {
            /* "border: solid red" with no width is one pixel, which
             * is also CSS's initial border-width. */
            w = 1;
        }
        for (int i = 0; i < 4; i++) st->border[i] = w;
        uint32_t c = CSS_NO_COLOR;
        const char *p = value;
        while (*p != '\0' && c == CSS_NO_COLOR) {
            while (is_space(*p)) p++;
            if (*p == '\0') break;
            c = css_parse_color(p);
            while (*p != '\0' && !is_space(*p)) p++;
        }
        if (c != CSS_NO_COLOR) st->border_color = c;
        return;
    }

    /* The per-side forms, which pages use far more than the
     * shorthand once a layout is being tuned. */
    static const struct { const char *suffix; int side; } sides[] = {
        { "-top", SIDE_TOP }, { "-right", SIDE_RIGHT },
        { "-bottom", SIDE_BOTTOM }, { "-left", SIDE_LEFT }, { NULL, 0 }
    };
    for (int i = 0; sides[i].suffix != NULL; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "margin%s", sides[i].suffix);
        if (ci_eq(name, buf)) {
            int v = css_length(value, CSS_UNRESOLVED);
            if (v != CSS_UNRESOLVED) st->margin[sides[i].side] = v;
            return;
        }
        snprintf(buf, sizeof(buf), "padding%s", sides[i].suffix);
        if (ci_eq(name, buf)) {
            int v = css_length(value, CSS_UNRESOLVED);
            if (v != CSS_UNRESOLVED) st->padding[sides[i].side] = v;
            return;
        }
        snprintf(buf, sizeof(buf), "border%s-width", sides[i].suffix);
        if (ci_eq(name, buf)) {
            int v = css_length(value, CSS_UNRESOLVED);
            if (v != CSS_UNRESOLVED) st->border[sides[i].side] = v;
            return;
        }
    }
}

/* ---- the cascade ---- */

static void style_inherit(struct style *child, const struct style *parent) {
    child->color = parent->color;
    child->bold = parent->bold;
    child->italic = parent->italic;
    child->underline = parent->underline;
    child->font_scale = parent->font_scale;
    child->align = parent->align;
    child->pre = parent->pre;
}

static void style_init_root(struct style *st) {
    memset(st, 0, sizeof(*st));
    st->display = DISP_INLINE;
    st->color = 0x101010;
    st->background = CSS_NO_COLOR;
    st->border_color = 0x808080;
    st->font_scale = 1;
    st->width = -1;
    st->align = ALIGN_LEFT;
    st->flex_direction = FLEX_ROW;
    st->justify_content = JUSTIFY_START;
    st->align_items = ALIGN_ITEMS_START; /* real default is `stretch`; not
                                          * implemented (needs a second
                                          * layout pass), START avoids
                                          * silently mis-sizing content */
}

/* Apply every rule that matches, in cascade order. The list is
 * walked twice rather than sorted: once for the normal declarations
 * and once for the !important ones, which is what puts them last. */
static void apply_sheet(const struct sheet *s, struct dom_node *n,
                        int important_pass) {
    /* Insertion sort by specificity into a small window would need an
     * allocation per element; instead the list is walked once per
     * specificity band, which for the handful of bands CSS actually
     * uses costs less than sorting. */
    static const int bands[] = { 0, 1, 100, 10000, 1000000 };
    for (int b = 0; b + 1 < (int)(sizeof(bands) / sizeof(bands[0])); b++) {
        for (const struct rule *r = s->first; r != NULL; r = r->next) {
            if (r->sel.specificity < bands[b] ||
                r->sel.specificity >= bands[b + 1]) {
                continue;
            }
            if (!selector_matches(&r->sel, n)) continue;

            for (const struct decl *d = r->decls; d != NULL; d = d->next) {
                if (d->important != important_pass) continue;
                apply_decl(n->style, d->name, d->value);
            }
        }
    }
}

void css_apply(struct dom_node *root) {
    struct sheet sheet;
    memset(&sheet, 0, sizeof(sheet));

    sheet_parse(&sheet, UA_STYLESHEET, sizeof(UA_STYLESHEET) - 1);

    /* Every <style> in the document, in the order they appear. */
    for (struct dom_node *n = root; n != NULL; n = dom_next(n, root)) {
        if (n->type != DOM_ELEMENT || !ci_eq(n->name, "style")) continue;
        for (struct dom_node *t = n->first; t != NULL; t = t->next) {
            if (t->type == DOM_TEXT && t->text != NULL) {
                sheet_parse(&sheet, t->text, strlen(t->text));
            }
        }
    }

    for (struct dom_node *n = root; n != NULL; n = dom_next(n, root)) {
        free(n->style);
        n->style = calloc(1, sizeof(struct style));
        if (n->style == NULL) continue;

        if (n->parent != NULL && n->parent->style != NULL) {
            style_init_root(n->style);
            style_inherit(n->style, n->parent->style);
        } else {
            style_init_root(n->style);
        }

        if (n->type != DOM_ELEMENT) continue;

        apply_sheet(&sheet, n, 0);

        /* An element's own style attribute outranks any stylesheet
         * rule that is not !important. */
        const char *inline_css = dom_attr(n, "style");
        if (inline_css != NULL) {
            struct rule tmp;
            memset(&tmp, 0, sizeof(tmp));
            parse_decls(&tmp, inline_css, strlen(inline_css));
            for (struct decl *d = tmp.decls; d != NULL;) {
                struct decl *next = d->next;
                apply_decl(n->style, d->name, d->value);
                free(d->name);
                free(d->value);
                free(d);
                d = next;
            }
        }

        apply_sheet(&sheet, n, 1);

        /* Not a CSS property: a link is what layout needs to know to
         * make the box clickable. */
        if (ci_eq(n->name, "a") && dom_attr(n, "href") != NULL) {
            n->style->is_link = 1;
        }
    }

    sheet_free(&sheet);
}
