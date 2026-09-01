/*
 * jsi.h - the interpreter's insides, shared with the DOM bindings
 *
 * Values are small and copied; everything they point at is allocated
 * from the interpreter and lives until the page does. That is the one
 * decision this file is built around: a browser tab runs a script,
 * then a few handlers, and then the whole page goes away, so a
 * collector would be a lot of machinery to avoid one free() at the
 * end. What replaces it is a budget - a script that allocates without
 * end is stopped rather than obeyed.
 */

#ifndef CLINT_JSI_H
#define CLINT_JSI_H

#include <stddef.h>
#include <stdint.h>

#include "dom.h"
#include "js.h"

enum { JS_UNDEF = 0, JS_NULL, JS_BOOL, JS_NUM, JS_STR, JS_OBJ };

struct js_str {
    size_t len;
    char *text;              /* NUL-terminated */
};

struct js_obj;

struct js_value {
    uint8_t type;
    double num;              /* JS_NUM, and 0/1 for JS_BOOL */
    struct js_str *str;      /* JS_STR */
    struct js_obj *obj;      /* JS_OBJ */
};

/* What an object is underneath. The DOM kinds are objects whose
 * properties are looked up in the document instead of in a list. */
enum {
    OBJ_PLAIN = 0,
    OBJ_ARRAY,
    OBJ_FUNC,      /* a function written in JavaScript */
    OBJ_NATIVE,    /* a function written in C */
    OBJ_NODE,      /* an element or the document */
    OBJ_LIST,      /* the result of getElementsByTagName and friends */
    OBJ_STYLE,     /* element.style */
    OBJ_EVENT
};

struct js_prop {
    char *name;
    struct js_value value;
    struct js_prop *next;
};

struct ast;
struct js_env;

typedef struct js_value (*js_native_fn)(struct js *J, struct js_value self,
                                        int argc, struct js_value *argv);

struct js_obj {
    uint8_t kind;
    struct js_prop *props;

    /* OBJ_ARRAY */
    struct js_value *items;
    int len, cap;

    /* OBJ_FUNC */
    struct ast *params;      /* identifier list */
    struct ast *body;
    struct js_env *closure;
    struct js_value bound_this;
    int has_bound_this;

    /* OBJ_NATIVE */
    js_native_fn native;
    const char *name;

    /* OBJ_NODE, OBJ_STYLE, OBJ_EVENT */
    struct dom_node *node;

    /* OBJ_LIST */
    struct dom_node **nodes;
    int nnodes;

    /* A compiled regular expression (jsregex.h), when this object is
     * one. */
    struct rx *regex;

    /* OBJ_EVENT */
    int prevented;

    struct js_obj *next_alloc;
};

struct js_env {
    struct js_prop *vars;
    struct js_env *parent;
    struct js_env *next_alloc;
};

/* Why evaluation stopped, if it did. */
enum {
    SIG_NONE = 0,
    SIG_RETURN,
    SIG_BREAK,
    SIG_CONTINUE,
    SIG_THROW,
    SIG_ERROR        /* out of budget, or a mistake in the interpreter */
};

#define JS_TIMER_MAX 32
#define JS_LISTENER_MAX 128

/* addEventListener, kept beside the tree rather than inside it: the
 * DOM is the parser's, and a handler belongs to the script that
 * registered it. */
struct js_listener {
    struct dom_node *node;      /* NULL means the window */
    char type[24];
    struct js_value fn;
};

struct js_timer {
    int used;
    unsigned long due_ms;
    struct js_value fn;
    long interval;           /* 0 for setTimeout */
};

struct js {
    struct dom_node *document;
    struct js_host host;

    struct js_env *global;
    struct js_obj *window;
    struct js_obj *document_obj;

    /* The methods of the primitive types, built once and handed out
     * on every property access rather than rebuilt per lookup. */
    struct js_obj *string_methods;
    struct js_obj *array_methods;
    struct js_obj *number_methods;

    /* Everything allocated, so it can all go at once. */
    struct js_obj *objects;
    struct js_env *envs;
    struct js_str **strings;
    int nstrings, strings_cap;
    char **texts;            /* parser-owned identifier/string storage */
    int ntexts, texts_cap;
    struct ast *nodes_head;

    size_t allocated;
    size_t alloc_limit;
    unsigned long steps;
    unsigned long step_limit;

    int signal;
    struct js_value signal_value;   /* return value, or the thrown one */
    char error[256];

    struct js_timer timers[JS_TIMER_MAX];
    int timer_seq;

    struct js_listener listeners[JS_LISTENER_MAX];
    int nlisteners;

    int depth;                      /* call depth, against runaway recursion */
    int tree_depth;                 /* eval()/exec() recursion depth, against
                                      * a deeply nested expression or statement
                                      * overflowing the C stack - independent
                                      * of `depth` above, which only counts
                                      * function CALLS, not AST nesting. A
                                      * script never triggers a call for
                                      * something like a long `a+b+c+...`
                                      * chain, which still builds an equally
                                      * deep, equally recursive tree to
                                      * evaluate. */
};

/* ---- values ---- */

struct js_value js_undefined(void);
struct js_value js_null(void);
struct js_value js_bool(int b);
struct js_value js_number(double n);
struct js_value js_string(struct js *J, const char *text);
struct js_value js_string_len(struct js *J, const char *text, size_t len);
struct js_value js_object(struct js_obj *obj);

int js_truthy(struct js_value v);
double js_to_number(struct js *J, struct js_value v);
const char *js_to_string(struct js *J, struct js_value v);   /* interned */
int js_equals(struct js *J, struct js_value a, struct js_value b, int strict);

/* ---- objects ---- */

struct js_obj *js_new_object(struct js *J, int kind);
struct js_obj *js_new_array(struct js *J);
struct js_obj *js_new_native(struct js *J, const char *name, js_native_fn fn);
struct js_obj *js_new_node(struct js *J, struct dom_node *node);
struct js_obj *js_new_list(struct js *J);

void js_list_add(struct js *J, struct js_obj *list, struct dom_node *node);
void js_array_push(struct js *J, struct js_obj *arr, struct js_value v);

struct js_value js_get(struct js *J, struct js_value target, const char *name);
void js_set(struct js *J, struct js_value target, const char *name,
            struct js_value v);
void js_def(struct js *J, struct js_obj *obj, const char *name,
            struct js_value v);
void js_def_native(struct js *J, struct js_obj *obj, const char *name,
                   js_native_fn fn);

struct js_value js_call(struct js *J, struct js_value fn, struct js_value self,
                        int argc, struct js_value *argv);

/* ---- errors ---- */

void js_error(struct js *J, const char *fmt, ...);
void js_throw(struct js *J, const char *message);

/* ---- memory ---- */

void *js_alloc(struct js *J, size_t size);
char *js_strdup(struct js *J, const char *text);

/*
 * An event-handler attribute (onclick="..."), run with `element` as
 * `this` and `event` in scope. Returns 1 when it said `return false`,
 * which is how markup cancels the default action.
 */
int js_run_handler(struct js *J, struct dom_node *element, const char *source,
                   struct js_value event);

/* ---- the DOM half (jsdom.c) ---- */

void jsdom_install(struct js *J);
struct js_value jsdom_node_get(struct js *J, struct js_obj *obj,
                               const char *name, int *handled);
int jsdom_node_set(struct js *J, struct js_obj *obj, const char *name,
                   struct js_value v);
struct js_value jsdom_list_get(struct js *J, struct js_obj *obj,
                               const char *name, int *handled);
struct js_value jsdom_style_get(struct js *J, struct js_obj *obj,
                                const char *name, int *handled);
int jsdom_style_set(struct js *J, struct js_obj *obj, const char *name,
                    struct js_value v);
struct js_value jsdom_event_get(struct js *J, struct js_obj *obj,
                                const char *name, int *handled);

/* Number formatting the way JavaScript writes it. */
const char *js_number_to_text(struct js *J, double n);

#endif /* CLINT_JSI_H */
