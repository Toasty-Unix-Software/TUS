/*
 * layout.c - see layout.h
 *
 * Two formatting contexts, which is what a document is made of:
 *
 *   Block. Boxes stack downwards, each as wide as its container
 *   allows. Margins, borders and padding come off the width in that
 *   order, and the background and border are painted over the border
 *   box before the children go in.
 *
 *   Inline. Text and inline elements flow along a line until the line
 *   is full, then the pen returns to the left edge. A line is only
 *   finished when it wraps or when a block interrupts it, which is
 *   also when text-align can be applied - the line's width is not
 *   known before then.
 *
 * Clint has one bitmap font, so a character is always eight pixels
 * wide times the scale, and measuring a word is counting its
 * characters rather than walking glyph metrics. The font covers ASCII
 * and the accented Latin letters composed from it; a character
 * outside that is folded to the nearest ASCII before it reaches here,
 * because a page of blanks is worse than a page of approximations.
 */

#include "layout.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "font.h"
#include "utf8.h"

static char lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c;
}

static int tag_is(const struct dom_node *n, const char *name) {
    if (n->type != DOM_ELEMENT) return 0;
    const char *a = n->name;
    while (*a != '\0' && *name != '\0') {
        if (lower(*a) != lower(*name)) return 0;
        a++;
        name++;
    }
    return *a == *name;
}

static int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f';
}

/* ---- text folding ---- */

/*
 * What to do with a character the font has no glyph for. The honest
 * choice is the closest ASCII: a page that reads "don't" where it
 * said "don’t" is still the page, while one full of blanks is not.
 * Letters are handled by the font itself now, so what is left here is
 * punctuation, symbols, and the letters that would need a shape the
 * ASCII font cannot lend.
 */
static const char *fold_codepoint(uint32_t cp) {
    switch (cp) {
    case 0x00A0: return " ";
    case 0x00AB: return "<<";
    case 0x00BB: return ">>";
    case 0x00A9: return "(c)";
    case 0x00AE: return "(R)";
    case 0x00B0: return "deg";
    case 0x00B7: case 0x2022: return "*";
    case 0x00D7: return "x";
    case 0x2010: case 0x2011: case 0x2012:
    case 0x2013: case 0x2014: case 0x2015: return "-";
    case 0x2018: case 0x2019: case 0x201A: return "'";
    case 0x201C: case 0x201D: case 0x201E: return "\"";
    case 0x2026: return "...";
    case 0x2039: return "<";
    case 0x203A: return ">";
    case 0x20AC: return "EUR";
    case 0x00A3: return "GBP";
    case 0x2122: return "(TM)";
    case 0x2190: return "<-";
    case 0x2192: return "->";
    case 0x2264: return "<=";
    case 0x2265: return ">=";
    default: break;
    }

    /* Accented Latin letters keep their base letter. */
    static const struct { uint32_t lo, hi; const char *map; } latin[] = {
        { 0x00C0, 0x00C5, "AAAAAA" }, { 0x00C8, 0x00CB, "EEEE" },
        { 0x00CC, 0x00CF, "IIII" },   { 0x00D2, 0x00D6, "OOOOO" },
        { 0x00D9, 0x00DC, "UUUU" },   { 0x00E0, 0x00E5, "aaaaaa" },
        { 0x00E8, 0x00EB, "eeee" },   { 0x00EC, 0x00EF, "iiii" },
        { 0x00F2, 0x00F6, "ooooo" },  { 0x00F9, 0x00FC, "uuuu" },
        { 0, 0, NULL }
    };
    static char one[2];
    for (int i = 0; latin[i].map != NULL; i++) {
        if (cp >= latin[i].lo && cp <= latin[i].hi) {
            one[0] = latin[i].map[cp - latin[i].lo];
            one[1] = '\0';
            return one;
        }
    }
    switch (cp) {
    case 0x00C7: return "C";
    case 0x00E7: return "c";
    case 0x00D1: return "N";
    case 0x00F1: return "n";
    case 0x00DF: return "ss";
    case 0x00C6: return "AE";
    case 0x00E6: return "ae";
    case 0x00D8: return "O";
    case 0x00F8: return "o";
    case 0x00D0: case 0x00DE: return "D";
    case 0x00F0: case 0x00FE: return "d";
    case 0x0141: return "L";
    case 0x0142: return "l";
    case 0x0152: return "OE";
    case 0x0153: return "oe";
    default: return "?";
    }
}

/*
 * Text as the rest of layout wants it: UTF-8 still, but containing
 * only characters that can be drawn. A character with a glyph is
 * copied through untouched; one without is replaced by the ASCII that
 * comes closest. The result is never longer than three bytes per
 * character in, which is what the buffer allows for.
 */
static char *fold_to_ascii(const char *in) {
    size_t len = strlen(in);
    char *out = malloc(len * 3 + 1);
    if (out == NULL) return NULL;

    size_t at = 0;
    for (size_t i = 0; i < len;) {
        uint32_t cp;
        size_t used = utf8_next(in + i, len - i, &cp);

        if (cp < 0x80) {
            out[at++] = (cp >= 0x20 || cp == '\n' || cp == '\t') ? (char)cp
                                                                 : ' ';
        } else if (font_glyph(cp) != NULL) {
            memcpy(out + at, in + i, used);
            at += used;
        } else {
            const char *rep = fold_codepoint(cp);
            while (*rep != '\0') out[at++] = *rep++;
        }
        i += used;
    }
    out[at] = '\0';
    return out;
}

/* ---- the display list ---- */

static struct paint_item *item_add(struct doc_layout *l) {
    if (l->nitems == l->item_cap) {
        int want = l->item_cap ? l->item_cap * 2 : 256;
        struct paint_item *p = realloc(l->items, (size_t)want * sizeof(*p));
        if (p == NULL) return NULL;
        l->items = p;
        l->item_cap = want;
    }
    struct paint_item *it = &l->items[l->nitems++];
    memset(it, 0, sizeof(*it));
    return it;
}

static void link_add(struct doc_layout *l, int x, int y, int w, int h,
                     const char *href, struct dom_node *node) {
    if (href == NULL && node == NULL) return;
    if (l->nlinks == l->link_cap) {
        int want = l->link_cap ? l->link_cap * 2 : 64;
        struct doc_link *p = realloc(l->links, (size_t)want * sizeof(*p));
        if (p == NULL) return;
        l->links = p;
        l->link_cap = want;
    }
    struct doc_link *k = &l->links[l->nlinks++];
    k->x = x;
    k->y = y;
    k->w = w;
    k->h = h;
    k->href = href != NULL ? strdup(href) : NULL;
    k->node = node;
}

static struct doc_field *field_add(struct doc_layout *l) {
    if (l->nfields == l->field_cap) {
        int want = l->field_cap ? l->field_cap * 2 : 16;
        struct doc_field *p = realloc(l->fields, (size_t)want * sizeof(*p));
        if (p == NULL) return NULL;
        l->fields = p;
        l->field_cap = want;
    }
    struct doc_field *f = &l->fields[l->nfields++];
    memset(f, 0, sizeof(*f));
    return f;
}

int layout_columns(const char *text) {
    return text != NULL ? utf8_columns(text, strlen(text)) : 0;
}

size_t layout_column_bytes(const char *text, int columns) {
    size_t at = 0, len = text != NULL ? strlen(text) : 0;
    for (int n = 0; n < columns && at < len;) {
        uint32_t cp;
        at += utf8_next(text + at, len - at, &cp);
        n++;
    }
    return at;
}

void layout_free(struct doc_layout *l) {
    for (int i = 0; i < l->nitems; i++) free(l->items[i].text);
    free(l->items);
    for (int i = 0; i < l->nlinks; i++) free(l->links[i].href);
    free(l->links);
    free(l->fields);
    memset(l, 0, sizeof(*l));
}

struct doc_field *layout_field_at(struct doc_layout *l, int x, int y) {
    for (int i = l->nfields - 1; i >= 0; i--) {
        struct doc_field *f = &l->fields[i];
        if (x >= f->x && y >= f->y && x < f->x + f->w && y < f->y + f->h) {
            return f;
        }
    }
    return NULL;
}

const struct doc_link *layout_link_at(const struct doc_layout *l, int x, int y) {
    /* Later links are on top of earlier ones, so search backwards. */
    for (int i = l->nlinks - 1; i >= 0; i--) {
        const struct doc_link *k = &l->links[i];
        if (x >= k->x && y >= k->y && x < k->x + k->w && y < k->y + k->h) {
            return k;
        }
    }
    return NULL;
}

/*
 * Move everything a scratch layout produced into the real one,
 * shifted to where the box ended up. The strings go with it - the
 * items keep the pointers they were built with, and the scratch
 * layout is emptied rather than freed, so nothing is copied twice or
 * released while it is still in use.
 */
static void layout_take(struct doc_layout *dst, struct doc_layout *src,
                        int dx, int dy) {
    for (int i = 0; i < src->nitems; i++) {
        struct paint_item *it = item_add(dst);
        if (it == NULL) break;
        *it = src->items[i];
        it->x += dx;
        it->y += dy;
        src->items[i].text = NULL;
    }
    for (int i = 0; i < src->nlinks; i++) {
        if (dst->nlinks == dst->link_cap) {
            int want = dst->link_cap ? dst->link_cap * 2 : 64;
            struct doc_link *p = realloc(dst->links,
                                         (size_t)want * sizeof(*p));
            if (p == NULL) break;
            dst->links = p;
            dst->link_cap = want;
        }
        struct doc_link *k = &dst->links[dst->nlinks++];
        *k = src->links[i];
        k->x += dx;
        k->y += dy;
        src->links[i].href = NULL;
    }
    for (int i = 0; i < src->nfields; i++) {
        struct doc_field *f = field_add(dst);
        if (f == NULL) break;
        *f = src->fields[i];
        f->x += dx;
        f->y += dy;
    }

    for (int i = 0; i < src->nitems; i++) free(src->items[i].text);
    free(src->items);
    for (int i = 0; i < src->nlinks; i++) free(src->links[i].href);
    free(src->links);
    free(src->fields);
    memset(src, 0, sizeof(*src));
}

/* Non-zero while a shrink-to-fit measurement is running. */
static int g_measuring;

/* ---- inline formatting ---- */

struct line {
    struct doc_layout *out;
    int left, right;    /* the content edges */
    int y;              /* top of the current line */
    int pen;            /* where the next word goes */
    int height;         /* the tallest thing on this line */
    int first;          /* first item index belonging to this line */
    int first_link;     /* and the same for its links and controls */
    int first_field;
    int align;
    int pending_space;  /* a space is owed before the next word */
    int any;            /* something has been placed on this line */
};

static void line_start(struct line *ln, struct doc_layout *out, int left,
                       int right, int y, int align) {
    ln->out = out;
    ln->left = left;
    ln->right = right;
    ln->y = y;
    ln->pen = left;
    ln->height = 0;
    ln->first = out->nitems;
    ln->first_link = out->nlinks;
    ln->first_field = out->nfields;
    ln->align = align;
    ln->pending_space = 0;
    ln->any = 0;
}

/*
 * Finish the current line: apply text-align now that its width is
 * known, then move down. Alignment shifts the items already emitted,
 * which is why they are tracked from `first`.
 */
static void line_break(struct line *ln) {
    if (!ln->any) {
        ln->pending_space = 0;
        return;
    }

    /* While an inline-block is being measured the line is as wide as
     * the sky, so centring it would push every word into the middle
     * of nowhere - and it is the content's width being measured, not
     * where it sits. */
    if (ln->align != ALIGN_LEFT && !g_measuring) {
        int used = ln->pen - ln->left;
        int slack = (ln->right - ln->left) - used;
        if (slack > 0) {
            int shift = ln->align == ALIGN_CENTER ? slack / 2 : slack;
            for (int i = ln->first; i < ln->out->nitems; i++) {
                ln->out->items[i].x += shift;
            }
            /* The link and control rectangles have to move with the
             * text they belong to. They are tracked by where the line
             * started, not by their own y: an inline-block puts
             * things on this line whose y is its own inner offset. */
            for (int i = ln->first_link; i < ln->out->nlinks; i++) {
                ln->out->links[i].x += shift;
            }
            for (int i = ln->first_field; i < ln->out->nfields; i++) {
                ln->out->fields[i].x += shift;
            }
        }
    }

    ln->y += ln->height > 0 ? ln->height : CELL_H;
    ln->pen = ln->left;
    ln->height = 0;
    ln->first = ln->out->nitems;
    ln->first_link = ln->out->nlinks;
    ln->first_field = ln->out->nfields;
    ln->pending_space = 0;
    ln->any = 0;
}

static void line_word(struct line *ln, const char *word, size_t len,
                      const struct style *st, const char *href,
                      struct dom_node *click) {
    if (len == 0) return;

    int scale = st->font_scale > 0 ? st->font_scale : 1;
    int w = utf8_columns(word, len) * CELL_W * scale;
    int h = CELL_H * scale;
    int space = ln->pending_space ? CELL_W * scale : 0;

    if (ln->any && ln->pen + space + w > ln->right) {
        line_break(ln);
        space = 0;
    }
    ln->pen += space;

    struct paint_item *it = item_add(ln->out);
    if (it == NULL) return;
    it->type = ITEM_TEXT;
    it->x = ln->pen;
    it->y = ln->y;
    it->w = w;
    it->h = h;
    it->color = st->color;
    it->scale = scale;
    it->bold = st->bold;
    it->underline = st->underline;
    it->text = malloc(len + 1);
    if (it->text != NULL) {
        memcpy(it->text, word, len);
        it->text[len] = '\0';
    }

    if (href != NULL || click != NULL) {
        link_add(ln->out, ln->pen, ln->y, w, h, href, click);
    }

    ln->pen += w;
    if (h > ln->height) ln->height = h;
    ln->any = 1;
    ln->pending_space = 0;
}

/*
 * A box on the line - an image, or a form control. It is placed like
 * a word (the same wrapping, the same owed space, the same link
 * rectangle) but the caller fills in what it is.
 */
static struct paint_item *line_box(struct line *ln, int w, int h,
                                   const char *href, struct dom_node *click) {
    if (w <= 0 || h <= 0) return NULL;

    int space = ln->pending_space ? CELL_W : 0;
    if (ln->any && ln->pen + space + w > ln->right) {
        line_break(ln);
        space = 0;
    }
    ln->pen += space;

    struct paint_item *it = item_add(ln->out);
    if (it == NULL) return NULL;
    it->x = ln->pen;
    it->y = ln->y;
    it->w = w;
    it->h = h;

    if (href != NULL || click != NULL) {
        link_add(ln->out, ln->pen, ln->y, w, h, href, click);
    }

    ln->pen += w;
    if (h > ln->height) ln->height = h;
    ln->any = 1;
    ln->pending_space = 0;
    return it;
}

/* Break a run of text into words and place them. */
static void line_text(struct line *ln, const char *text,
                      const struct style *st, const char *href,
                      struct dom_node *click) {
    if (st->pre) {
        /* Preformatted: newlines break lines, spaces are kept, and
         * nothing wraps. */
        const char *p = text;
        while (*p != '\0') {
            const char *nl = strchr(p, '\n');
            size_t len = nl != NULL ? (size_t)(nl - p) : strlen(p);
            if (len > 0) {
                line_word(ln, p, len, st, href, click);
            } else if (nl != NULL) {
                ln->any = 1; /* an empty line still takes a line */
                if (ln->height == 0) ln->height = CELL_H * st->font_scale;
            }
            if (nl == NULL) break;
            line_break(ln);
            p = nl + 1;
        }
        return;
    }

    const char *p = text;
    while (*p != '\0') {
        if (is_space(*p)) {
            /* Runs of whitespace are one space, and a space at the
             * start of a line is dropped. */
            if (ln->any) ln->pending_space = 1;
            while (is_space(*p)) p++;
            continue;
        }
        size_t len = 0;
        while (p[len] != '\0' && !is_space(p[len])) len++;
        line_word(ln, p, len, st, href, click);
        p += len;
    }
}

/* ---- images ---- */

static layout_image_fn g_image_source;
static layout_click_fn g_click_probe;

void layout_set_image_source(layout_image_fn fn) { g_image_source = fn; }
void layout_set_click_probe(layout_click_fn fn) { g_click_probe = fn; }

/* The element a click on this content belongs to: the innermost one
 * with a handler, or whatever the parent already had. */
static struct dom_node *click_target(struct dom_node *n,
                                     struct dom_node *inherited) {
    if (g_click_probe != NULL && g_click_probe(n)) return n;
    return inherited;
}

/* An HTML length attribute: a plain number is pixels, and a
 * percentage is not one of those - it is relative to a box layout has
 * not finished measuring, so it is left to the intrinsic size. */
static int attr_pixels(const struct dom_node *n, const char *name) {
    const char *v = dom_attr(n, name);
    if (v == NULL) return -1;
    while (*v == ' ') v++;
    int value = 0, digits = 0;
    while (*v >= '0' && *v <= '9') {
        value = value * 10 + (*v - '0');
        v++;
        digits++;
    }
    if (digits == 0 || *v == '%') return -1;
    return value;
}

static void place_alt_text(struct line *ln, const struct dom_node *n,
                           const struct style *st, const char *href,
                           struct dom_node *click) {
    const char *alt = dom_attr(n, "alt");
    if (alt != NULL && *alt == '\0') return;   /* alt="" means "skip me" */

    char buf[128];
    snprintf(buf, sizeof(buf), "[%s]", alt != NULL ? alt : "image");
    char *folded = fold_to_ascii(buf);
    if (folded != NULL) {
        line_text(ln, folded, st, href, click);
        free(folded);
    }
}

/*
 * The size an image is drawn at: what the page asked for, then what
 * the picture is, and in either case no wider than the line. A height
 * given without a width (or the other way round) scales the other to
 * match, so an image is never stretched out of shape by accident.
 */
static void image_size(const struct dom_node *n, const struct image *img,
                       const struct style *st, int avail, int *out_w,
                       int *out_h) {
    int w = attr_pixels(n, "width");
    int h = attr_pixels(n, "height");
    if (w <= 0 && st->width > 0) w = st->width;

    if (w > 0 && h <= 0) h = w * img->h / img->w;
    else if (h > 0 && w <= 0) w = h * img->w / img->h;
    else if (w <= 0 && h <= 0) { w = img->w; h = img->h; }

    if (avail > 0 && w > avail) {
        h = h * avail / w;
        w = avail;
    }
    if (h < 1) h = 1;
    if (w < 1) w = 1;

    *out_w = w;
    *out_h = h;
}

static void line_image(struct line *ln, struct dom_node *n,
                       const struct style *st, const char *href,
                       struct dom_node *click) {
    const char *src = dom_attr(n, "src");
    const struct image *img = NULL;
    if (g_image_source != NULL && src != NULL && *src != '\0') {
        img = g_image_source(src);
    }
    if (img == NULL || img->px == NULL) {
        place_alt_text(ln, n, st, href, click);
        return;
    }

    int w, h;
    image_size(n, img, st, ln->right - ln->left, &w, &h);

    struct paint_item *it = line_box(ln, w, h, href, click);
    if (it == NULL) {
        place_alt_text(ln, n, st, href, click);
        return;
    }
    it->type = ITEM_IMAGE;
    it->img = img;
    it->color = CSS_NO_COLOR;
}

/* ---- video ---- */

/*
 * A <video> is drawn as what it is: a box that says what it holds and
 * plays when it is clicked. Clint has no decoder of its own - TUS's
 * video player does - so the box is a link to the media, and
 * following it hands the file to the player.
 */
static void line_media(struct line *ln, struct dom_node *n,
                       const struct style *st) {
    const char *src = dom_attr(n, "src");
    if (src == NULL) {
        for (struct dom_node *c = n->first; c != NULL; c = c->next) {
            if (c->type == DOM_ELEMENT && tag_is(c, "source")) {
                src = dom_attr(c, "src");
                if (src != NULL) break;
            }
        }
    }
    if (src == NULL) return;

    int is_audio = tag_is(n, "audio");
    const char *label = is_audio ? "[>] play audio" : "[>] play video";
    int cols = (int)strlen(label);

    int w = attr_pixels(n, "width");
    int h = attr_pixels(n, "height");
    if (w <= 0) w = is_audio ? 320 : 480;
    if (h <= 0) h = is_audio ? 48 : 200;

    int avail = ln->right - ln->left;
    if (avail > 0 && w > avail) {
        h = h * avail / w;
        w = avail;
    }
    if (h < CELL_H * 2) h = CELL_H * 2;

    struct paint_item *box = line_box(ln, w, h, src, n);
    if (box == NULL) return;
    box->type = ITEM_RECT;
    box->color = 0x00202124u;

    struct paint_item *label_item = item_add(ln->out);
    if (label_item == NULL) return;
    label_item->type = ITEM_TEXT;
    label_item->x = box->x + (w - cols * CELL_W) / 2;
    label_item->y = box->y + (h - CELL_H) / 2;
    label_item->w = cols * CELL_W;
    label_item->h = CELL_H;
    label_item->color = 0x00FFFFFFu;
    label_item->scale = 1;
    label_item->text = strdup(label);
}

/* ---- form controls ---- */

#define FIELD_PAD       4
#define FIELD_BG        0x00FFFFFFu
#define FIELD_BUTTON_BG 0x00E9E9EDu
#define FIELD_BORDER    0x00767676u

static int value_is(const char *value, const char *want) {
    if (value == NULL) return 0;
    while (*value != '\0' && *want != '\0') {
        if (lower(*value) != lower(*want)) return 0;
        value++;
        want++;
    }
    return *value == *want;
}

static int attr_number(const struct dom_node *n, const char *name,
                       int fallback) {
    int v = attr_pixels(n, name);
    return v > 0 ? v : fallback;
}

/* Which control this element is, or -1 for one that is never drawn. */
static int field_type_of(const struct dom_node *n, int *password) {
    *password = 0;

    if (tag_is(n, "textarea")) return FIELD_TEXTAREA;
    if (tag_is(n, "select")) return FIELD_SELECT;
    if (tag_is(n, "button")) return FIELD_BUTTON;
    if (!tag_is(n, "input")) return -1;

    const char *type = dom_attr(n, "type");
    if (type == NULL) return FIELD_TEXT;
    if (value_is(type, "hidden")) return -1;
    if (value_is(type, "submit") || value_is(type, "reset") ||
        value_is(type, "button")) {
        return FIELD_BUTTON;
    }
    if (value_is(type, "checkbox")) return FIELD_CHECKBOX;
    if (value_is(type, "radio")) return FIELD_RADIO;
    if (value_is(type, "password")) {
        *password = 1;
        return FIELD_TEXT;
    }
    if (value_is(type, "text") || value_is(type, "search") ||
        value_is(type, "email") || value_is(type, "url") ||
        value_is(type, "tel") || value_is(type, "number")) {
        return FIELD_TEXT;
    }
    return FIELD_OTHER;   /* file, colour, date: drawn, not operated */
}

/* The chosen <option> of a <select>: the one marked selected, or the
 * first, which is what a browser shows before anything is chosen. */
static const struct dom_node *selected_option(const struct dom_node *sel) {
    const struct dom_node *first = NULL;
    for (const struct dom_node *n = sel->first; n != NULL; n = n->next) {
        if (n->type != DOM_ELEMENT || !tag_is(n, "option")) continue;
        if (first == NULL) first = n;
        if (dom_attr(n, "selected") != NULL) return n;
    }
    return first;
}

void layout_field_text(const struct dom_node *n, int type, char *out,
                       size_t size) {
    char raw[512];
    raw[0] = '\0';

    if (type == FIELD_SELECT) {
        const struct dom_node *opt = selected_option(n);
        if (opt != NULL) dom_text_content(opt, raw, sizeof(raw));
    } else if (type == FIELD_BUTTON) {
        const char *value = dom_attr(n, "value");
        if (tag_is(n, "button")) {
            dom_text_content(n, raw, sizeof(raw));
            if (raw[0] == '\0' && value != NULL) {
                snprintf(raw, sizeof(raw), "%s", value);
            }
        } else if (value != NULL) {
            snprintf(raw, sizeof(raw), "%s", value);
        } else {
            const char *type_attr = dom_attr(n, "type");
            snprintf(raw, sizeof(raw), "%s",
                     value_is(type_attr, "reset") ? "Reset" : "Submit");
        }
    } else if (type == FIELD_TEXTAREA) {
        const char *value = dom_attr(n, "value");
        if (value != NULL) snprintf(raw, sizeof(raw), "%s", value);
        else dom_text_content(n, raw, sizeof(raw));
    } else {
        const char *value = dom_attr(n, "value");
        if (value != NULL) snprintf(raw, sizeof(raw), "%s", value);
    }

    /* Trim, because a label written across three lines of markup is
     * still one word on a button. */
    char *folded = fold_to_ascii(raw);
    if (folded == NULL) {
        out[0] = '\0';
        return;
    }
    const char *start = folded;
    while (is_space(*start)) start++;
    size_t len = strlen(start);
    while (len > 0 && is_space(start[len - 1])) len--;
    if (len >= size) len = size - 1;
    memcpy(out, start, len);
    out[len] = '\0';
    free(folded);
}

static void box_border(struct doc_layout *out, int x, int y, int w, int h,
                       uint32_t color) {
    const int sides[4][4] = {
        { x, y, w, 1 }, { x, y + h - 1, w, 1 },
        { x, y, 1, h }, { x + w - 1, y, 1, h },
    };
    for (int i = 0; i < 4; i++) {
        struct paint_item *it = item_add(out);
        if (it == NULL) return;
        it->type = ITEM_RECT;
        it->x = sides[i][0];
        it->y = sides[i][1];
        it->w = sides[i][2];
        it->h = sides[i][3];
        it->color = color;
    }
}

/*
 * A control takes its width from the page when the page says
 * something - `size` in characters, `cols` and `rows`, a CSS width -
 * and from its own label otherwise. The height is one line plus the
 * padding a box needs to look like a box.
 */
static void line_field(struct line *ln, struct dom_node *n,
                       const struct style *st, const char *href) {
    (void)href;
    int password = 0;
    int type = field_type_of(n, &password);
    if (type < 0) return;

    char label[256];
    layout_field_text(n, type, label, sizeof(label));

    int w, h;
    if (type == FIELD_CHECKBOX || type == FIELD_RADIO) {
        w = h = CELL_H - 2;
    } else if (type == FIELD_TEXTAREA) {
        int cols = attr_number(n, "cols", 40);
        int rows = attr_number(n, "rows", 3);
        w = cols * CELL_W + 2 * FIELD_PAD + 2;
        h = rows * CELL_H + 2 * FIELD_PAD + 2;
    } else if (type == FIELD_BUTTON) {
        w = (int)strlen(label) * CELL_W + 4 * FIELD_PAD + 2;
        h = CELL_H + 2 * FIELD_PAD + 2;
    } else if (type == FIELD_SELECT) {
        w = (int)strlen(label) * CELL_W + 4 * FIELD_PAD + CELL_W + 2;
        h = CELL_H + 2 * FIELD_PAD + 2;
    } else {
        int cols = attr_number(n, "size", 0);
        if (cols <= 0 && st->width > 0) cols = st->width / CELL_W;
        if (cols <= 0) cols = 20;
        if (cols > 200) cols = 200;
        w = cols * CELL_W + 2 * FIELD_PAD + 2;
        h = CELL_H + 2 * FIELD_PAD + 2;
    }

    int avail = ln->right - ln->left;
    if (avail > 0 && w > avail) w = avail;

    /* A control inside a link is still a control: it is not the link
     * that gets the click, so no link rectangle is recorded for it. */
    (void)href;

    struct paint_item *box = line_box(ln, w, h, NULL, NULL);
    if (box == NULL) return;
    box->type = ITEM_RECT;
    box->color = type == FIELD_BUTTON || type == FIELD_SELECT
                     ? FIELD_BUTTON_BG
                     : FIELD_BG;

    int x = box->x, y = box->y;
    box_border(ln->out, x, y, w, h, FIELD_BORDER);

    if ((type == FIELD_CHECKBOX || type == FIELD_RADIO) &&
        dom_attr(n, "checked") != NULL) {
        struct paint_item *mark = item_add(ln->out);
        if (mark != NULL) {
            mark->type = ITEM_RECT;
            mark->x = x + 3;
            mark->y = y + 3;
            mark->w = w - 6;
            mark->h = h - 6;
            mark->color = st->color;
        }
    }

    struct doc_field *f = field_add(ln->out);
    if (f == NULL) return;
    f->x = x;
    f->y = y;
    f->w = w;
    f->h = h;
    f->type = type;
    f->password = password;
    f->node = n;
}

/* ---- block formatting ---- */

struct ctx {
    struct doc_layout *out;
    int y;              /* the pen, in document coordinates */
};

static void layout_block(struct ctx *c, struct dom_node *node, int left,
                         int right, const char *href, struct dom_node *click,
                         int depth);

/*
 * Measuring an inline-block means laying it out twice, so a document
 * that is nothing but nested ones could cost a great deal. The budget
 * is per document: past it, an inline-block is laid out as an
 * ordinary block, which is what Clint did before it could measure at
 * all.
 */
#define PROBE_WIDTH   65536
static int g_measure_budget;

/*
 * A box that is a block inside and a word outside: laid out at the
 * width it wants, then placed on the line like anything else. The
 * width it wants is what its contents reach when nothing wraps,
 * clamped to the room the line has left.
 */
static void line_inline_block(struct line *ln, struct dom_node *node,
                              const struct style *st, const char *href,
                              struct dom_node *click, int depth) {
    int avail = ln->right - ln->left;
    if (avail <= 0) return;

    int want = st->width > 0 ? st->width + st->border[SIDE_LEFT] +
                                   st->border[SIDE_RIGHT] +
                                   st->padding[SIDE_LEFT] +
                                   st->padding[SIDE_RIGHT]
                             : 0;

    if (want <= 0) {
        /* Nothing said how wide it is, so lay it out where nothing
         * can wrap and see how far the content reaches. Backgrounds
         * are skipped in that measurement: they are as wide as the
         * box, which is the thing being measured. */
        struct doc_layout probe;
        memset(&probe, 0, sizeof(probe));
        struct ctx pc = { &probe, 0 };
        g_measuring++;
        layout_block(&pc, node, 0, PROBE_WIDTH, href, click, depth + 1);
        g_measuring--;

        for (int i = 0; i < probe.nitems; i++) {
            if (probe.items[i].type == ITEM_RECT) continue;
            int edge = probe.items[i].x + probe.items[i].w;
            if (edge > want) want = edge;
        }
        for (int i = 0; i < probe.nfields; i++) {
            int edge = probe.fields[i].x + probe.fields[i].w;
            if (edge > want) want = edge;
        }
        layout_free(&probe);

        want += st->padding[SIDE_RIGHT] + st->border[SIDE_RIGHT] +
                st->margin[SIDE_RIGHT];
    }

    if (want <= 0 || want > avail) want = avail;

    struct doc_layout box;
    memset(&box, 0, sizeof(box));
    struct ctx bc = { &box, 0 };
    layout_block(&bc, node, 0, want, href, click, depth + 1);

    int h = bc.y;
    if (h <= 0) {
        layout_free(&box);
        return;
    }

    int space = ln->pending_space ? CELL_W : 0;
    if (ln->any && ln->pen + space + want > ln->right) {
        line_break(ln);
        space = 0;
    }
    ln->pen += space;

    layout_take(ln->out, &box, ln->pen, ln->y);

    ln->pen += want;
    if (h > ln->height) ln->height = h;
    ln->any = 1;
    ln->pending_space = 0;
}

/* Walk the inline content of `node` into the open line. */
static void layout_inline(struct ctx *c, struct line *ln, struct dom_node *node,
                          const char *href, struct dom_node *click, int depth) {
    if (depth > 64) return;

    for (struct dom_node *n = node->first; n != NULL; n = n->next) {
        struct style *st = n->style;
        if (st == NULL || st->display == DISP_NONE) continue;

        if (n->type == DOM_TEXT) {
            if (n->text == NULL) continue;
            char *folded = fold_to_ascii(n->text);
            if (folded == NULL) continue;
            line_text(ln, folded, st, href, click);
            free(folded);
            continue;
        }

        if (tag_is(n, "br")) {
            ln->any = 1;
            if (ln->height == 0) ln->height = CELL_H * st->font_scale;
            line_break(ln);
            continue;
        }

        if (tag_is(n, "img")) {
            line_image(ln, n, st, href, click_target(n, click));
            continue;
        }

        if (tag_is(n, "input") || tag_is(n, "button") ||
            tag_is(n, "select") || tag_is(n, "textarea")) {
            line_field(ln, n, st, href);
            continue;
        }

        if (tag_is(n, "video") || tag_is(n, "audio")) {
            line_media(ln, n, st);
            continue;
        }

        const char *child_href = href;
        if (st->is_link) child_href = dom_attr(n, "href");
        struct dom_node *child_click = click_target(n, click);

        if (st->display == DISP_INLINE) {
            layout_inline(c, ln, n, child_href, child_click, depth + 1);
            continue;
        }

        if (st->display == DISP_INLINE_BLOCK && g_measure_budget > 0 &&
            depth < 32) {
            g_measure_budget--;
            line_inline_block(ln, n, st, child_href, child_click, depth);
            continue;
        }

        /* A block inside inline content ends the line and lays itself
         * out where the line was. */
        line_break(ln);
        c->y = ln->y;
        layout_block(c, n, ln->left, ln->right, child_href, child_click,
                     depth + 1);
        line_start(ln, c->out, ln->left, ln->right, c->y, ln->align);
    }
}

static void paint_box(struct doc_layout *out, const struct style *st,
                      int x, int y, int w, int h) {
    if (w <= 0 || h <= 0) return;

    if (st->background != CSS_NO_COLOR) {
        struct paint_item *it = item_add(out);
        if (it == NULL) return;
        it->type = ITEM_RECT;
        it->x = x;
        it->y = y;
        it->w = w;
        it->h = h;
        it->color = st->background;
    }

    /* Four rectangles rather than an outline primitive: a border can
     * be thick, and each side can differ. */
    const int sides[4][4] = {
        { x, y, w, st->border[SIDE_TOP] },
        { x, y + h - st->border[SIDE_BOTTOM], w, st->border[SIDE_BOTTOM] },
        { x, y, st->border[SIDE_LEFT], h },
        { x + w - st->border[SIDE_RIGHT], y, st->border[SIDE_RIGHT], h },
    };
    for (int i = 0; i < 4; i++) {
        if (sides[i][2] <= 0 || sides[i][3] <= 0) continue;
        struct paint_item *it = item_add(out);
        if (it == NULL) return;
        it->type = ITEM_RECT;
        it->x = sides[i][0];
        it->y = sides[i][1];
        it->w = sides[i][2];
        it->h = sides[i][3];
        it->color = st->border_color;
    }
}

/* True when this element has any block-level child; a block whose
 * children are all inline runs one line builder over the lot. */
static int has_block_child(const struct dom_node *n) {
    for (const struct dom_node *c = n->first; c != NULL; c = c->next) {
        if (c->style == NULL) continue;
        if (c->style->display == DISP_BLOCK ||
            c->style->display == DISP_LIST_ITEM) {
            return 1;
        }
    }
    return 0;
}

static void layout_block(struct ctx *c, struct dom_node *node, int left,
                         int right, const char *href, struct dom_node *click,
                         int depth) {
    if (depth > 64) return;

    click = click_target(node, click);
    struct style *st = node->style;
    if (st == NULL || st->display == DISP_NONE) return;

    int box_left = left + st->margin[SIDE_LEFT];
    int box_right = right - st->margin[SIDE_RIGHT];
    if (st->width > 0 && box_left + st->width < box_right) {
        box_right = box_left + st->width;
    }
    if (box_right <= box_left) box_right = box_left + CELL_W;

    int content_left = box_left + st->border[SIDE_LEFT] + st->padding[SIDE_LEFT];
    int content_right = box_right - st->border[SIDE_RIGHT] -
                        st->padding[SIDE_RIGHT];
    if (content_right <= content_left) content_right = content_left + CELL_W;

    c->y += st->margin[SIDE_TOP];
    int box_top = c->y;

    /* The background and border are emitted before the children so
     * they end up underneath, but their height is only known
     * afterwards - so the slot is reserved and filled in at the end. */
    int box_item = c->out->nitems;
    int reserve = (st->background != CSS_NO_COLOR) || st->border[SIDE_TOP] ||
                  st->border[SIDE_BOTTOM] || st->border[SIDE_LEFT] ||
                  st->border[SIDE_RIGHT];

    c->y += st->border[SIDE_TOP] + st->padding[SIDE_TOP];

    if (tag_is(node, "hr")) {
        struct paint_item *it = item_add(c->out);
        if (it != NULL) {
            it->type = ITEM_RECT;
            it->x = content_left;
            it->y = c->y;
            it->w = content_right - content_left;
            it->h = st->border[SIDE_TOP] > 0 ? st->border[SIDE_TOP] : 1;
            it->color = st->border_color;
            c->y += it->h;
        }
    } else if (st->display == DISP_LIST_ITEM) {
        /* The marker sits in the padding the list reserved on the
         * left, which is why it is placed relative to the content
         * edge rather than given a box of its own. */
        struct paint_item *it = item_add(c->out);
        if (it != NULL) {
            it->type = ITEM_TEXT;
            it->x = content_left - 2 * CELL_W;
            it->y = c->y;
            it->w = CELL_W;
            it->h = CELL_H;
            it->color = st->color;
            it->scale = 1;
            it->text = strdup("*");
        }
        struct line ln;
        line_start(&ln, c->out, content_left, content_right, c->y, st->align);
        layout_inline(c, &ln, node, href, click, depth + 1);
        line_break(&ln);
        c->y = ln.y;
    } else if (has_block_child(node)) {
        /* Mixed content: inline runs between blocks each get their
         * own line builder, opened lazily. */
        struct line ln;
        line_start(&ln, c->out, content_left, content_right, c->y, st->align);
        layout_inline(c, &ln, node, href, click, depth + 1);
        line_break(&ln);
        c->y = ln.y;
    } else {
        struct line ln;
        line_start(&ln, c->out, content_left, content_right, c->y, st->align);
        layout_inline(c, &ln, node, href, click, depth + 1);
        line_break(&ln);
        c->y = ln.y;
    }

    c->y += st->padding[SIDE_BOTTOM] + st->border[SIDE_BOTTOM];

    if (reserve) {
        /* Insert the box behind everything the children emitted. */
        int before = c->out->nitems;
        paint_box(c->out, st, box_left, box_top, box_right - box_left,
                  c->y - box_top);
        int added = c->out->nitems - before;
        if (added > 0 && before > box_item) {
            struct paint_item tmp[8];
            if (added <= (int)(sizeof(tmp) / sizeof(tmp[0]))) {
                memcpy(tmp, &c->out->items[before],
                       (size_t)added * sizeof(tmp[0]));
                memmove(&c->out->items[box_item + added],
                        &c->out->items[box_item],
                        (size_t)(before - box_item) * sizeof(tmp[0]));
                memcpy(&c->out->items[box_item], tmp,
                       (size_t)added * sizeof(tmp[0]));
            }
        }
    }

    c->y += st->margin[SIDE_BOTTOM];
}

void layout_document(struct dom_node *root, int width, struct doc_layout *out) {
    layout_free(out);
    out->width = width;

    g_measure_budget = 2000;

    struct ctx c;
    c.out = out;
    c.y = 0;

    /* <body> if there is one, so the page's own margins apply; the
     * document otherwise, which is what a fragment gets. */
    struct dom_node *body = NULL;
    for (struct dom_node *n = root; n != NULL; n = dom_next(n, root)) {
        if (tag_is(n, "body")) {
            body = n;
            break;
        }
    }
    layout_block(&c, body != NULL ? body : root, 0, width, NULL, NULL, 0);

    out->height = c.y;
}
