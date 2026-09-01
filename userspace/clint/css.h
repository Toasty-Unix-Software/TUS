/*
 * css.h - computed style, and the cascade that produces it
 *
 * One struct per element, filled once before layout. Clint has a
 * single 8x16 bitmap font, so the parts of CSS that would ask for a
 * typeface it does not have are folded into what it does have:
 * font-size becomes an integer scale of the character cell, and
 * font-family is ignored. Everything else in here - the box model,
 * colours, alignment, display - is honoured as written.
 */

#ifndef CLINT_CSS_H
#define CLINT_CSS_H

#include <stdint.h>

#include "dom.h"

enum {
    DISP_INLINE = 0,
    DISP_BLOCK,
    DISP_INLINE_BLOCK,   /* a block that is measured, then put on a line */
    DISP_LIST_ITEM,
    DISP_NONE
};

enum { ALIGN_LEFT = 0, ALIGN_CENTER, ALIGN_RIGHT };

/* A background nobody set: the box is not painted, so whatever is
 * behind it shows through. */
#define CSS_NO_COLOR 0xFF000000u

/* top, right, bottom, left - the order every CSS shorthand uses. */
enum { SIDE_TOP = 0, SIDE_RIGHT, SIDE_BOTTOM, SIDE_LEFT };

struct style {
    int display;
    uint32_t color;
    uint32_t background;
    uint32_t border_color;

    int bold;
    int italic;
    int underline;
    int font_scale;      /* 1, 2 or 3 cells per character */

    int margin[4];
    int padding[4];
    int border[4];

    int width;           /* -1 when auto */
    int align;
    int pre;             /* white-space: pre - keep the spacing */

    /* Not CSS: set for <a href>, so layout can record a link box. */
    int is_link;
};

/*
 * Style the whole tree: the built-in stylesheet first, then every
 * <style> element in the document, then each element's style
 * attribute. Frees any styles from a previous pass.
 */
void css_apply(struct dom_node *root);

/* Parse a colour the way CSS writes them: #rgb, #rrggbb, rgb(), or
 * one of the named colours. Returns CSS_NO_COLOR when it is none of
 * those. */
uint32_t css_parse_color(const char *text);

#endif /* CLINT_CSS_H */
