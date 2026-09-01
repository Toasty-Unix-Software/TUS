/*
 * layout.h - the document turned into things to paint
 *
 * Layout runs once per page and per window width, and produces a flat
 * list rather than a tree: painting is then a loop, scrolling is a
 * subtraction, and hit-testing a link is a search. The tree has
 * already done its job by the time this is built.
 *
 * Coordinates are document coordinates - the top of the page is y=0,
 * however far down the reader has scrolled.
 */

#ifndef CLINT_LAYOUT_H
#define CLINT_LAYOUT_H

#include <stddef.h>
#include <stdint.h>

#include "css.h"
#include "dom.h"
#include "image.h"

enum { ITEM_RECT, ITEM_TEXT, ITEM_IMAGE };

struct paint_item {
    int type;
    int x, y, w, h;
    uint32_t color;

    /* ITEM_TEXT only. */
    char *text;
    int scale;
    int bold;
    int underline;

    /* ITEM_IMAGE only: the decoded picture, owned by whoever fetched
     * it (the browser's cache), not by the display list. */
    const struct image *img;
};

struct doc_link {
    int x, y, w, h;
    char *href;              /* NULL when the box is only clickable */
    struct dom_node *node;   /* the element the click belongs to */
};

/*
 * A form control, recorded the way a link is: a rectangle and the
 * element it came from. Layout draws the box and stops there - what
 * is inside changes as the reader types, and the element's `value`
 * attribute is where that lives, so the browser paints the contents
 * and the caret over the box without laying the page out again.
 */
enum {
    FIELD_TEXT = 0,   /* text, search, email, url, tel, number, password */
    FIELD_TEXTAREA,
    FIELD_BUTTON,     /* submit, reset, button */
    FIELD_CHECKBOX,
    FIELD_RADIO,
    FIELD_SELECT,
    FIELD_OTHER       /* a control Clint draws but cannot operate */
};

struct doc_field {
    int x, y, w, h;
    int type;
    int password;            /* show the value as dots */
    struct dom_node *node;
};

struct doc_layout {
    struct paint_item *items;
    int nitems, item_cap;

    struct doc_link *links;
    int nlinks, link_cap;

    struct doc_field *fields;
    int nfields, field_cap;

    int width;   /* what it was laid out for */
    int height;  /* how tall the document turned out */
};

/*
 * Where a picture comes from. Layout knows an <img> by its src and
 * nothing else; the browser is what fetches and decodes, so it hands
 * layout a lookup and gets asked as each image is reached. Returning
 * NULL - no fetcher set, still loading, or a format with no decoder -
 * puts the alt text on the line instead, which is what it is for.
 */
typedef const struct image *(*layout_image_fn)(const char *src);
void layout_set_image_source(layout_image_fn fn);

/*
 * Which elements answer to a click even though they are not links.
 * Layout cannot know - a handler may have been added by a script - so
 * it asks. Without a probe, only <a href> is clickable, which is what
 * a browser without scripting has.
 */
typedef int (*layout_click_fn)(struct dom_node *node);
void layout_set_click_probe(layout_click_fn fn);

/* Lay `root` out for a viewport `width` pixels wide. */
void layout_document(struct dom_node *root, int width, struct doc_layout *out);
void layout_free(struct doc_layout *l);

/* The link under a document coordinate, or NULL. */
const struct doc_link *layout_link_at(const struct doc_layout *l, int x, int y);

/* The form control under a document coordinate, or NULL. */
struct doc_field *layout_field_at(struct doc_layout *l, int x, int y);

/*
 * What a control shows: the value of a text field, the label of a
 * button, the chosen option of a select. One function, so the box
 * that was measured and the text drawn in it can never be measured
 * from different strings.
 */
void layout_field_text(const struct dom_node *n, int type, char *out,
                       size_t size);

/*
 * Text is UTF-8 and the font is one cell per character, so a string's
 * width is its character count and never its length in bytes. These
 * are what anything outside layout uses to measure and to cut it.
 */
int layout_columns(const char *text);
size_t layout_column_bytes(const char *text, int columns);

/* One character cell, before scaling. */
#define CELL_W 8
#define CELL_H 16

#endif /* CLINT_LAYOUT_H */
