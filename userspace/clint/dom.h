/*
 * dom.h - the document tree Clint parses HTML into
 *
 * Two node types, which is all HTML needs once comments and doctypes
 * are dropped: elements, which have a name, attributes and children,
 * and text. Everything above this - style, layout, painting - walks
 * this tree and never looks at the markup again.
 */

#ifndef CLINT_DOM_H
#define CLINT_DOM_H

#include <stddef.h>
#include <stdint.h>

#define DOM_NAME_MAX 32

enum { DOM_ELEMENT, DOM_TEXT };

struct dom_attr {
    char *name;
    char *value;
    struct dom_attr *next;
};

struct style;

struct dom_node {
    int type;
    char name[DOM_NAME_MAX]; /* lowercased tag name, elements only */
    char *text;              /* text nodes only */

    struct dom_attr *attrs;
    struct dom_node *parent, *first, *last, *prev, *next;

    struct style *style;     /* computed by css.c */
};

/*
 * Parse a document. Never returns NULL for well-formed-enough input:
 * HTML has no parse errors, only recoveries, so anything that cannot
 * be understood becomes text.
 */
struct dom_node *dom_parse(const char *html, size_t len);
void dom_free(struct dom_node *root);

const char *dom_attr(const struct dom_node *n, const char *name);

/*
 * Set an attribute, adding it when it is not there. This is what
 * makes a form field editable: what the reader types is the
 * element's value, kept where every later pass already looks for it,
 * so layout and submission cannot disagree about what is in the box.
 * Returns 0, or -1 when there is no memory for it.
 */
int dom_set_attr(struct dom_node *n, const char *name, const char *value);
void dom_remove_attr(struct dom_node *n, const char *name);

/*
 * Building and changing the tree - what a script does when it says
 * createElement, appendChild or innerHTML. The parser builds the same
 * shapes through the same calls, so a node made here is
 * indistinguishable from one that came off the wire.
 */
struct dom_node *dom_create_element(const char *name);
struct dom_node *dom_create_text(const char *text);
void dom_append(struct dom_node *parent, struct dom_node *child);
void dom_insert_before(struct dom_node *parent, struct dom_node *child,
                       struct dom_node *before);
void dom_remove(struct dom_node *child);      /* detach; does not free */
void dom_free_children(struct dom_node *node);

/* Parse a fragment of markup: the returned node is a holder whose
 * children are the content, ready to be moved into the document. */
struct dom_node *dom_parse_fragment(const char *html, size_t len);

/* The markup of a subtree, as innerHTML gives it back. Returns the
 * length it wanted, which may be more than `size`. */
size_t dom_serialize(const struct dom_node *node, int children_only, char *out,
                     size_t size);

/* Depth-first walk order, for the passes that visit every node. */
struct dom_node *dom_next(struct dom_node *n, struct dom_node *root);

/* The text of a subtree, concatenated - what <title> needs. */
void dom_text_content(const struct dom_node *n, char *out, size_t size);

#endif /* CLINT_DOM_H */
