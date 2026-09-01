/*
 * html.c - HTML into a document tree
 *
 * A tokenizer and a tree builder, written to the shape of real pages
 * rather than to the shape of the specification: HTML5's parser is a
 * state machine with dozens of insertion modes, and most of that
 * exists to reproduce quirks from the 1990s. What actually matters
 * for laying out a page is smaller, and it is all here:
 *
 *   Void elements never take children (<br>, <img>, <meta>...), so a
 *   missing close tag cannot swallow the rest of the document.
 *
 *   Implied end tags. A <p> is closed by the next block element, an
 *   <li> by the next <li>, a <td> by the next row. Pages rely on this
 *   constantly, and without it a list turns into one deeply nested
 *   item.
 *
 *   Raw text elements. Everything between <script> and </script> is
 *   text, tags included; the same for <style>. Parsing them as markup
 *   is how a stray "<" in a script eats the page.
 *
 *   A close tag with no matching open is ignored rather than
 *   unwinding the stack, which is what stops one stray </div> from
 *   ending the document.
 */

#include "dom.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- small helpers ---- */

static char lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c;
}

static int name_eq(const char *a, const char *b) {
    while (*a && *b) {
        if (lower(*a) != lower(*b)) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f';
}

static char *dup_range(const char *s, size_t n) {
    char *p = malloc(n + 1);
    if (p == NULL) return NULL;
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

/* ---- tags with special parsing rules ---- */

static int is_void(const char *name) {
    static const char *v[] = { "area", "base", "br", "col", "embed", "hr",
                               "img", "input", "link", "meta", "param",
                               "source", "track", "wbr", NULL };
    for (int i = 0; v[i] != NULL; i++) {
        if (name_eq(name, v[i])) return 1;
    }
    return 0;
}

static int is_raw_text(const char *name) {
    return name_eq(name, "script") || name_eq(name, "style") ||
           name_eq(name, "textarea") || name_eq(name, "title");
}

static int is_block(const char *name) {
    static const char *b[] = { "address", "article", "aside", "blockquote",
                               "div", "dl", "fieldset", "figcaption", "figure",
                               "footer", "form", "h1", "h2", "h3", "h4", "h5",
                               "h6", "header", "hr", "main", "nav", "ol", "p",
                               "pre", "section", "table", "ul", NULL };
    for (int i = 0; b[i] != NULL; i++) {
        if (name_eq(name, b[i])) return 1;
    }
    return 0;
}

/* ---- entities ---- */

/*
 * The named entities a page is likely to contain, plus numeric
 * references. Anything else is left as written: an unrecognised "&"
 * is far more often a literal ampersand than a typo for an entity.
 */
static int entity_lookup(const char *name, size_t len, uint32_t *out) {
    static const struct { const char *name; uint32_t cp; } table[] = {
        { "amp", '&' },     { "lt", '<' },      { "gt", '>' },
        { "quot", '"' },    { "apos", '\'' },   { "nbsp", 0xA0 },
        { "copy", 0xA9 },   { "reg", 0xAE },    { "hellip", 0x2026 },
        { "mdash", 0x2014 },{ "ndash", 0x2013 },{ "lsquo", 0x2018 },
        { "rsquo", 0x2019 },{ "ldquo", 0x201C },{ "rdquo", 0x201D },
        { "trade", 0x2122 },{ "middot", 0xB7 }, { "bull", 0x2022 },
        { "deg", 0xB0 },    { "plusmn", 0xB1 }, { "times", 0xD7 },
        { "euro", 0x20AC }, { "pound", 0xA3 },  { "sect", 0xA7 },
        { "laquo", 0xAB },  { "raquo", 0xBB },  { "para", 0xB6 },
        { "shy", 0xAD },    { "curren", 0xA4 }, { "yen", 0xA5 },
        { "cent", 0xA2 },   { "frac12", 0xBD }, { "frac14", 0xBC },
        { "sup2", 0xB2 },   { "sup3", 0xB3 },   { "micro", 0xB5 },
        { "divide", 0xF7 }, { "iquest", 0xBF }, { "iexcl", 0xA1 },

        /* The accented letters, which is most of what a page written
         * in a European language is made of. */
        { "Agrave", 0xC0 }, { "Aacute", 0xC1 }, { "Acirc", 0xC2 },
        { "Atilde", 0xC3 }, { "Auml", 0xC4 },   { "Aring", 0xC5 },
        { "AElig", 0xC6 },  { "Ccedil", 0xC7 }, { "Egrave", 0xC8 },
        { "Eacute", 0xC9 }, { "Ecirc", 0xCA },  { "Euml", 0xCB },
        { "Igrave", 0xCC }, { "Iacute", 0xCD }, { "Icirc", 0xCE },
        { "Iuml", 0xCF },   { "ETH", 0xD0 },    { "Ntilde", 0xD1 },
        { "Ograve", 0xD2 }, { "Oacute", 0xD3 }, { "Ocirc", 0xD4 },
        { "Otilde", 0xD5 }, { "Ouml", 0xD6 },   { "Oslash", 0xD8 },
        { "Ugrave", 0xD9 }, { "Uacute", 0xDA }, { "Ucirc", 0xDB },
        { "Uuml", 0xDC },   { "Yacute", 0xDD }, { "THORN", 0xDE },
        { "szlig", 0xDF },  { "agrave", 0xE0 }, { "aacute", 0xE1 },
        { "acirc", 0xE2 },  { "atilde", 0xE3 }, { "auml", 0xE4 },
        { "aring", 0xE5 },  { "aelig", 0xE6 },  { "ccedil", 0xE7 },
        { "egrave", 0xE8 }, { "eacute", 0xE9 }, { "ecirc", 0xEA },
        { "euml", 0xEB },   { "igrave", 0xEC }, { "iacute", 0xED },
        { "icirc", 0xEE },  { "iuml", 0xEF },   { "eth", 0xF0 },
        { "ntilde", 0xF1 }, { "ograve", 0xF2 }, { "oacute", 0xF3 },
        { "ocirc", 0xF4 },  { "otilde", 0xF5 }, { "ouml", 0xF6 },
        { "oslash", 0xF8 }, { "ugrave", 0xF9 }, { "uacute", 0xFA },
        { "ucirc", 0xFB },  { "uuml", 0xFC },   { "yacute", 0xFD },
        { "thorn", 0xFE },  { "yuml", 0xFF },   { "OElig", 0x152 },
        { "oelig", 0x153 }, { "Scaron", 0x160 }, { "scaron", 0x161 },
        { "Yuml", 0x178 },
        { NULL, 0 }
    };
    for (int i = 0; table[i].name != NULL; i++) {
        if (strlen(table[i].name) == len &&
            strncmp(table[i].name, name, len) == 0) {
            *out = table[i].cp;
            return 1;
        }
    }
    return 0;
}

/* Append one code point as UTF-8. */
static size_t put_utf8(char *out, uint32_t cp) {
    if (cp < 0x80) {
        out[0] = (char)cp;
        return 1;
    }
    if (cp < 0x800) {
        out[0] = (char)(0xC0 | (cp >> 6));
        out[1] = (char)(0x80 | (cp & 0x3F));
        return 2;
    }
    if (cp < 0x10000) {
        out[0] = (char)(0xE0 | (cp >> 12));
        out[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[2] = (char)(0x80 | (cp & 0x3F));
        return 3;
    }
    out[0] = (char)(0xF0 | (cp >> 18));
    out[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
    out[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
    out[3] = (char)(0x80 | (cp & 0x3F));
    return 4;
}

/* Expand entities in place; the result is never longer than the
 * input, because every entity is at least as long as what it
 * becomes. */
static void decode_entities(char *s) {
    char *out = s;
    for (const char *in = s; *in != '\0';) {
        if (*in != '&') {
            *out++ = *in++;
            continue;
        }

        const char *semi = in + 1;
        while (*semi != '\0' && *semi != ';' && semi - in < 12 &&
               !is_space(*semi)) {
            semi++;
        }
        if (*semi != ';') {
            *out++ = *in++;
            continue;
        }

        size_t len = (size_t)(semi - in - 1);
        uint32_t cp = 0;
        int ok = 0;

        if (len > 1 && in[1] == '#') {
            const char *digits = in + 2;
            int base = 10;
            size_t dlen = len - 1;
            if (*digits == 'x' || *digits == 'X') {
                base = 16;
                digits++;
                dlen--;
            }
            if (dlen > 0) {
                char buf[16];
                if (dlen < sizeof(buf)) {
                    memcpy(buf, digits, dlen);
                    buf[dlen] = '\0';
                    cp = (uint32_t)strtoul(buf, NULL, base);
                    ok = cp != 0;
                }
            }
        } else {
            ok = entity_lookup(in + 1, len, &cp);
        }

        if (!ok) {
            *out++ = *in++;
            continue;
        }
        out += put_utf8(out, cp);
        in = semi + 1;
    }
    *out = '\0';
}

/* ---- the tree ---- */

static struct dom_node *node_new(int type) {
    struct dom_node *n = calloc(1, sizeof(*n));
    if (n != NULL) n->type = type;
    return n;
}

static void node_append(struct dom_node *parent, struct dom_node *child) {
    child->parent = parent;
    child->prev = parent->last;
    if (parent->last != NULL) {
        parent->last->next = child;
    } else {
        parent->first = child;
    }
    parent->last = child;
}

void dom_free(struct dom_node *n) {
    if (n == NULL) return;
    struct dom_node *c = n->first;
    while (c != NULL) {
        struct dom_node *next = c->next;
        dom_free(c);
        c = next;
    }
    for (struct dom_attr *a = n->attrs; a != NULL;) {
        struct dom_attr *next = a->next;
        free(a->name);
        free(a->value);
        free(a);
        a = next;
    }
    free(n->text);
    free(n->style);
    free(n);
}

const char *dom_attr(const struct dom_node *n, const char *name) {
    if (n == NULL) return NULL;
    for (const struct dom_attr *a = n->attrs; a != NULL; a = a->next) {
        if (name_eq(a->name, name)) return a->value;
    }
    return NULL;
}

int dom_set_attr(struct dom_node *n, const char *name, const char *value) {
    if (n == NULL || name == NULL) return -1;

    for (struct dom_attr *a = n->attrs; a != NULL; a = a->next) {
        if (!name_eq(a->name, name)) continue;
        char *copy = dup_range(value != NULL ? value : "",
                               value != NULL ? strlen(value) : 0);
        if (copy == NULL) return -1;
        free(a->value);
        a->value = copy;
        return 0;
    }

    struct dom_attr *a = calloc(1, sizeof(*a));
    if (a == NULL) return -1;
    a->name = dup_range(name, strlen(name));
    a->value = dup_range(value != NULL ? value : "",
                         value != NULL ? strlen(value) : 0);
    if (a->name == NULL || a->value == NULL) {
        free(a->name);
        free(a->value);
        free(a);
        return -1;
    }
    a->next = n->attrs;
    n->attrs = a;
    return 0;
}

void dom_remove_attr(struct dom_node *n, const char *name) {
    if (n == NULL) return;
    struct dom_attr **link = &n->attrs;
    while (*link != NULL) {
        struct dom_attr *a = *link;
        if (name_eq(a->name, name)) {
            *link = a->next;
            free(a->name);
            free(a->value);
            free(a);
            return;
        }
        link = &a->next;
    }
}

struct dom_node *dom_next(struct dom_node *n, struct dom_node *root) {
    if (n->first != NULL) return n->first;
    while (n != root && n != NULL) {
        if (n->next != NULL) return n->next;
        n = n->parent;
    }
    return NULL;
}

void dom_text_content(const struct dom_node *n, char *out, size_t size) {
    size_t at = 0;
    out[0] = '\0';
    if (n == NULL) return;

    const struct dom_node *cur = n->first;
    while (cur != NULL) {
        if (cur->type == DOM_TEXT && cur->text != NULL) {
            size_t len = strlen(cur->text);
            if (at + len >= size) len = size - at - 1;
            memcpy(out + at, cur->text, len);
            at += len;
            out[at] = '\0';
            if (at + 1 >= size) return;
        }
        if (cur->first != NULL) {
            cur = cur->first;
            continue;
        }
        while (cur != NULL && cur != n && cur->next == NULL) cur = cur->parent;
        if (cur == NULL || cur == n) break;
        cur = cur->next;
    }
}

/* ---- the parser ---- */

struct parser {
    const char *src;
    size_t len, at;
    struct dom_node *root;
    struct dom_node *open[64]; /* the stack of unclosed elements */
    int depth;
};

static struct dom_node *current(struct parser *p) {
    return p->depth > 0 ? p->open[p->depth - 1] : p->root;
}

/* Close elements down to and including `name`, if it is open at all.
 * A close tag matching nothing is dropped: unwinding on it is how a
 * single stray </div> ends a document early. */
static void close_element(struct parser *p, const char *name) {
    for (int i = p->depth - 1; i >= 0; i--) {
        if (name_eq(p->open[i]->name, name)) {
            p->depth = i;
            return;
        }
    }
}

/* The end tags a start tag implies. */
static void imply_close(struct parser *p, const char *name) {
    while (p->depth > 0) {
        const char *top = p->open[p->depth - 1]->name;

        if (name_eq(top, "p") && is_block(name)) {
            p->depth--;
            continue;
        }
        if (name_eq(top, "li") && name_eq(name, "li")) {
            p->depth--;
            continue;
        }
        if ((name_eq(top, "dt") || name_eq(top, "dd")) &&
            (name_eq(name, "dt") || name_eq(name, "dd"))) {
            p->depth--;
            continue;
        }
        if ((name_eq(top, "td") || name_eq(top, "th")) &&
            (name_eq(name, "td") || name_eq(name, "th") ||
             name_eq(name, "tr"))) {
            p->depth--;
            continue;
        }
        if (name_eq(top, "tr") && name_eq(name, "tr")) {
            p->depth--;
            continue;
        }
        if (name_eq(top, "option") &&
            (name_eq(name, "option") || name_eq(name, "optgroup"))) {
            p->depth--;
            continue;
        }
        break;
    }
}

static void add_text(struct parser *p, const char *text, size_t len) {
    if (len == 0) return;

    char *copy = dup_range(text, len);
    if (copy == NULL) return;
    decode_entities(copy);

    if (copy[0] == '\0') {
        free(copy);
        return;
    }
    struct dom_node *n = node_new(DOM_TEXT);
    if (n == NULL) {
        free(copy);
        return;
    }
    n->text = copy;
    node_append(current(p), n);
}

/* Read attributes up to '>' or "/>". */
static void parse_attrs(struct parser *p, struct dom_node *el, int *self_close) {
    for (;;) {
        while (p->at < p->len && is_space(p->src[p->at])) p->at++;
        if (p->at >= p->len) return;

        if (p->src[p->at] == '>') {
            p->at++;
            return;
        }
        if (p->src[p->at] == '/' && p->at + 1 < p->len &&
            p->src[p->at + 1] == '>') {
            *self_close = 1;
            p->at += 2;
            return;
        }

        size_t start = p->at;
        while (p->at < p->len && !is_space(p->src[p->at]) &&
               p->src[p->at] != '=' && p->src[p->at] != '>' &&
               p->src[p->at] != '/') {
            p->at++;
        }
        if (p->at == start) { /* something unparseable: skip it */
            p->at++;
            continue;
        }

        struct dom_attr *a = calloc(1, sizeof(*a));
        if (a == NULL) return;
        a->name = dup_range(p->src + start, p->at - start);
        for (char *s = a->name; s != NULL && *s; s++) *s = lower(*s);

        while (p->at < p->len && is_space(p->src[p->at])) p->at++;
        if (p->at < p->len && p->src[p->at] == '=') {
            p->at++;
            while (p->at < p->len && is_space(p->src[p->at])) p->at++;

            if (p->at < p->len && (p->src[p->at] == '"' || p->src[p->at] == '\'')) {
                char quote = p->src[p->at++];
                size_t vstart = p->at;
                while (p->at < p->len && p->src[p->at] != quote) p->at++;
                a->value = dup_range(p->src + vstart, p->at - vstart);
                if (p->at < p->len) p->at++;
            } else {
                size_t vstart = p->at;
                while (p->at < p->len && !is_space(p->src[p->at]) &&
                       p->src[p->at] != '>') {
                    p->at++;
                }
                a->value = dup_range(p->src + vstart, p->at - vstart);
            }
            if (a->value != NULL) decode_entities(a->value);
        } else {
            a->value = dup_range("", 0);
        }

        a->next = el->attrs;
        el->attrs = a;
    }
}

/* Everything up to the matching close tag is text, tags included. */
static void parse_raw_text(struct parser *p, struct dom_node *el) {
    char closing[DOM_NAME_MAX + 4];
    snprintf(closing, sizeof(closing), "</%s", el->name);
    size_t clen = strlen(closing);

    size_t start = p->at;
    while (p->at < p->len) {
        if (p->src[p->at] == '<' && p->at + clen <= p->len) {
            int match = 1;
            for (size_t i = 0; i < clen; i++) {
                if (lower(p->src[p->at + i]) != lower(closing[i])) {
                    match = 0;
                    break;
                }
            }
            if (match) break;
        }
        p->at++;
    }

    if (p->at > start) {
        struct dom_node *t = node_new(DOM_TEXT);
        if (t != NULL) {
            t->text = dup_range(p->src + start, p->at - start);
            node_append(el, t);
        }
    }

    /* step over the close tag */
    while (p->at < p->len && p->src[p->at] != '>') p->at++;
    if (p->at < p->len) p->at++;
}

/* ---- changing the tree ---- */

struct dom_node *dom_create_element(const char *name) {
    struct dom_node *n = node_new(DOM_ELEMENT);
    if (n == NULL) return NULL;
    size_t at = 0;
    for (const char *p = name; p != NULL && *p != '\0' &&
                               at + 1 < sizeof(n->name); p++) {
        n->name[at++] = lower(*p);
    }
    n->name[at] = '\0';
    return n;
}

struct dom_node *dom_create_text(const char *text) {
    struct dom_node *n = node_new(DOM_TEXT);
    if (n == NULL) return NULL;
    n->text = dup_range(text != NULL ? text : "",
                        text != NULL ? strlen(text) : 0);
    return n;
}

void dom_append(struct dom_node *parent, struct dom_node *child) {
    if (parent == NULL || child == NULL) return;
    dom_remove(child);
    node_append(parent, child);
}

void dom_insert_before(struct dom_node *parent, struct dom_node *child,
                       struct dom_node *before) {
    if (parent == NULL || child == NULL) return;
    if (before == NULL || before->parent != parent) {
        dom_append(parent, child);
        return;
    }
    dom_remove(child);

    child->parent = parent;
    child->next = before;
    child->prev = before->prev;
    if (before->prev != NULL) before->prev->next = child;
    else parent->first = child;
    before->prev = child;
}

void dom_remove(struct dom_node *child) {
    if (child == NULL || child->parent == NULL) return;
    struct dom_node *parent = child->parent;

    if (child->prev != NULL) child->prev->next = child->next;
    else parent->first = child->next;
    if (child->next != NULL) child->next->prev = child->prev;
    else parent->last = child->prev;

    child->parent = NULL;
    child->prev = NULL;
    child->next = NULL;
}

void dom_free_children(struct dom_node *node) {
    if (node == NULL) return;
    struct dom_node *c = node->first;
    while (c != NULL) {
        struct dom_node *next = c->next;
        c->parent = NULL;
        c->prev = NULL;
        c->next = NULL;
        dom_free(c);
        c = next;
    }
    node->first = NULL;
    node->last = NULL;
}

/* ---- writing the tree back out ---- */

static size_t emit(char *out, size_t size, size_t at, const char *text) {
    for (const char *p = text; *p != '\0'; p++) {
        if (at + 1 < size) out[at] = *p;
        at++;
    }
    if (size > 0) out[at < size ? at : size - 1] = '\0';
    return at;
}

static size_t serialize_node(const struct dom_node *n, char *out, size_t size,
                             size_t at) {
    if (n->type == DOM_TEXT) {
        return emit(out, size, at, n->text != NULL ? n->text : "");
    }

    at = emit(out, size, at, "<");
    at = emit(out, size, at, n->name);
    for (const struct dom_attr *a = n->attrs; a != NULL; a = a->next) {
        at = emit(out, size, at, " ");
        at = emit(out, size, at, a->name);
        at = emit(out, size, at, "=\"");
        at = emit(out, size, at, a->value != NULL ? a->value : "");
        at = emit(out, size, at, "\"");
    }
    at = emit(out, size, at, ">");

    for (const struct dom_node *c = n->first; c != NULL; c = c->next) {
        at = serialize_node(c, out, size, at);
    }

    if (!is_void(n->name)) {
        at = emit(out, size, at, "</");
        at = emit(out, size, at, n->name);
        at = emit(out, size, at, ">");
    }
    return at;
}

size_t dom_serialize(const struct dom_node *node, int children_only, char *out,
                     size_t size) {
    if (size > 0) out[0] = '\0';
    if (node == NULL) return 0;

    size_t at = 0;
    if (children_only) {
        for (const struct dom_node *c = node->first; c != NULL; c = c->next) {
            at = serialize_node(c, out, size, at);
        }
    } else {
        at = serialize_node(node, out, size, at);
    }
    return at;
}

struct dom_node *dom_parse_fragment(const char *html, size_t len) {
    /* The same parser, then the wrapper the page structure adds is
     * stepped over: a fragment is its content, not a document. */
    struct dom_node *doc = dom_parse(html, len);
    if (doc == NULL) return NULL;

    struct dom_node *body = NULL;
    for (struct dom_node *n = doc; n != NULL; n = dom_next(n, doc)) {
        if (n->type == DOM_ELEMENT && strcmp(n->name, "body") == 0) {
            body = n;
            break;
        }
    }
    if (body == NULL) return doc;

    /* Move the body's children up to the holder and drop the rest. */
    struct dom_node *holder = node_new(DOM_ELEMENT);
    if (holder == NULL) return doc;
    snprintf(holder->name, sizeof(holder->name), "#fragment");

    struct dom_node *c = body->first;
    while (c != NULL) {
        struct dom_node *next = c->next;
        dom_remove(c);
        node_append(holder, c);
        c = next;
    }
    dom_free(doc);
    return holder;
}

struct dom_node *dom_parse(const char *html, size_t len) {
    struct parser p;
    memset(&p, 0, sizeof(p));
    p.src = html;
    p.len = len;

    p.root = node_new(DOM_ELEMENT);
    if (p.root == NULL) return NULL;
    snprintf(p.root->name, sizeof(p.root->name), "#document");

    while (p.at < p.len) {
        if (p.src[p.at] != '<') {
            size_t start = p.at;
            while (p.at < p.len && p.src[p.at] != '<') p.at++;
            add_text(&p, p.src + start, p.at - start);
            continue;
        }

        /* comments, doctypes and processing instructions */
        if (p.at + 3 < p.len && p.src[p.at + 1] == '!' &&
            p.src[p.at + 2] == '-' && p.src[p.at + 3] == '-') {
            p.at += 4;
            while (p.at + 2 < p.len &&
                   !(p.src[p.at] == '-' && p.src[p.at + 1] == '-' &&
                     p.src[p.at + 2] == '>')) {
                p.at++;
            }
            p.at = p.at + 3 < p.len ? p.at + 3 : p.len;
            continue;
        }
        if (p.at + 1 < p.len && (p.src[p.at + 1] == '!' || p.src[p.at + 1] == '?')) {
            while (p.at < p.len && p.src[p.at] != '>') p.at++;
            if (p.at < p.len) p.at++;
            continue;
        }

        int closing = p.at + 1 < p.len && p.src[p.at + 1] == '/';
        size_t name_at = p.at + (closing ? 2 : 1);
        size_t name_end = name_at;
        while (name_end < p.len && !is_space(p.src[name_end]) &&
               p.src[name_end] != '>' && p.src[name_end] != '/') {
            name_end++;
        }
        if (name_end == name_at) { /* a lone '<' */
            add_text(&p, "<", 1);
            p.at++;
            continue;
        }

        char name[DOM_NAME_MAX];
        size_t nlen = name_end - name_at;
        if (nlen >= sizeof(name)) nlen = sizeof(name) - 1;
        for (size_t i = 0; i < nlen; i++) name[i] = lower(p.src[name_at + i]);
        name[nlen] = '\0';

        p.at = name_end;

        if (closing) {
            while (p.at < p.len && p.src[p.at] != '>') p.at++;
            if (p.at < p.len) p.at++;
            close_element(&p, name);
            continue;
        }

        imply_close(&p, name);

        struct dom_node *el = node_new(DOM_ELEMENT);
        if (el == NULL) break;
        memcpy(el->name, name, nlen + 1);

        int self_close = 0;
        parse_attrs(&p, el, &self_close);
        node_append(current(&p), el);

        if (is_void(name) || self_close) continue;

        if (is_raw_text(name)) {
            parse_raw_text(&p, el);
            continue;
        }
        if (p.depth < (int)(sizeof(p.open) / sizeof(p.open[0]))) {
            p.open[p.depth++] = el;
        }
    }
    return p.root;
}
