/*
 * js.h - JavaScript for Clint
 *
 * A small interpreter: lexer, parser, tree-walking evaluator, and the
 * part of the browser object model a page actually touches. It is not
 * a modern engine and does not pretend to be - there is no JIT, no
 * garbage collector (everything a page allocates is freed when the
 * page is), and the language it accepts is ES5 plus the few later
 * pieces that are everywhere (let/const, arrow functions, template
 * literals).
 *
 * What it is for: the scripts ordinary pages run - showing and hiding
 * things, filling in a field, reacting to a click, writing markup
 * into the document. What it is not for: the minified, bot-checking
 * machinery that a search engine ships. That is a difference of
 * kind, not of degree, and no amount of this file closes it.
 *
 * Everything the interpreter needs from the browser arrives through
 * js_host: the browser owns the window, the network and the address
 * bar, and the interpreter only asks.
 */

#ifndef CLINT_JS_H
#define CLINT_JS_H

#include <stddef.h>

#include "dom.h"

struct js;

struct js_host {
    void *ctx;

    /* location.href = "...", or a link followed from a script. */
    void (*navigate)(void *ctx, const char *url);

    /* console.log and alert; the browser decides where they show. */
    void (*log)(void *ctx, const char *text);
    void (*alert)(void *ctx, const char *text);

    /* A script that failed. Separate from log() because it is not
     * the reader's news: a page whose script did not run is still a
     * page, and the message belongs where a developer looks. */
    void (*script_error)(void *ctx, const char *text);

    /* The document changed: whatever is on screen is now stale. */
    void (*changed)(void *ctx);

    /* The page's own address, for location.* */
    const char *(*location)(void *ctx);

    /* Milliseconds since boot, for Date and setTimeout. */
    unsigned long (*now_ms)(void *ctx);

    /* <script src="...">: fetch it and return a malloc'd, NUL-
     * terminated buffer the interpreter frees, or NULL if it could
     * not be fetched - a missing external script costs that one
     * script, not the page. NULL (the default with no initializer
     * for this field) means the host does not support fetching, and
     * a src script is silently skipped, same as before this existed. */
    char *(*fetch_script)(void *ctx, const char *src);
};

/* One interpreter per document. */
struct js *js_create(struct dom_node *document, const struct js_host *host);
void js_free(struct js *J);

/* Run every <script> in the document, in order. Returns the number
 * that ran. A <script src="..."> is fetched via host.fetch_script
 * when the host provides one; otherwise (or if the fetch fails) it
 * is skipped. */
int js_run_document(struct js *J);

/* Run one piece of source (a <script>, or an onclick attribute with
 * `element` as `this`). Returns 0, or -1 with js_last_error() set. */
int js_run(struct js *J, const char *source, struct dom_node *element);

/*
 * Deliver a click to `element`: its onclick attribute and any
 * listeners added with addEventListener, then the same for its
 * ancestors (bubbling). Returns 1 when a handler ran and asked to
 * cancel the default action (preventDefault or `return false`).
 */
int js_click(struct js *J, struct dom_node *element);

/* True when the element, or one of its ancestors, has a click
 * handler - which is what makes it worth a pointer cursor and a
 * click. */
int js_has_click_handler(struct js *J, struct dom_node *element);

/* Run the timers whose deadline has passed. Returns how many ran. */
int js_run_timers(struct js *J);

/* Milliseconds until the next timer is due, or -1 when there is
 * none: what the browser passes to its event wait. */
int js_next_timer(struct js *J);

const char *js_last_error(const struct js *J);

#endif /* CLINT_JS_H */
