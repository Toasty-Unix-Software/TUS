/*
 * jsdom.c - the document, as a script sees it
 *
 * Everything here is a view onto the tree the parser built: an
 * element object holds a pointer to a dom_node and nothing else, so a
 * script that changes an attribute changes the same attribute layout
 * will read. That is why there is no synchronisation to get wrong -
 * there is only one copy of the document, and this is a window onto
 * it.
 *
 * What a change costs is a relayout, which is the host's business:
 * every setter that alters the tree calls host.changed() and the
 * browser decides when to act on it.
 */

#include "jsi.h"
#include "jsregex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int name_is(const struct dom_node *n, const char *name) {
    return n != NULL && n->type == DOM_ELEMENT && strcmp(n->name, name) == 0;
}

static int ci_same(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        char x = (*a >= 'A' && *a <= 'Z') ? (char)(*a + 32) : *a;
        char y = (*b >= 'A' && *b <= 'Z') ? (char)(*b + 32) : *b;
        if (x != y) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static void changed(struct js *J) {
    if (J->host.changed != NULL) J->host.changed(J->host.ctx);
}

static struct js_value node_value(struct js *J, struct dom_node *n) {
    if (n == NULL) return js_null();
    return js_object(js_new_node(J, n));
}

/* ---- finding elements ---- */

static struct dom_node *by_id(struct dom_node *root, const char *id) {
    for (struct dom_node *n = root; n != NULL; n = dom_next(n, root)) {
        if (n->type != DOM_ELEMENT) continue;
        const char *v = dom_attr(n, "id");
        if (v != NULL && strcmp(v, id) == 0) return n;
    }
    return NULL;
}

static int has_class(const struct dom_node *n, const char *want) {
    const char *list = dom_attr(n, "class");
    if (list == NULL) return 0;
    size_t len = strlen(want);

    for (const char *p = list; *p != '\0';) {
        while (*p == ' ' || *p == '\t' || *p == '\n') p++;
        const char *start = p;
        while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\n') p++;
        if ((size_t)(p - start) == len && strncmp(start, want, len) == 0) {
            return 1;
        }
    }
    return 0;
}

static void collect(struct js *J, struct js_obj *list, struct dom_node *root,
                    const char *tag, const char *cls) {
    for (struct dom_node *n = root; n != NULL; n = dom_next(n, root)) {
        if (n->type != DOM_ELEMENT) continue;
        if (n == root) continue;
        if (tag != NULL && strcmp(tag, "*") != 0 && !ci_same(n->name, tag)) {
            continue;
        }
        if (cls != NULL && !has_class(n, cls)) continue;
        js_list_add(J, list, n);
    }
}

/*
 * One selector, of the three shapes a script actually passes:
 * "#id", ".class", "tag" - and "tag.class" because it is free.
 */
static struct dom_node *query(struct js *J, struct dom_node *root,
                              const char *sel, struct js_obj *all) {
    if (sel == NULL) return NULL;
    while (*sel == ' ') sel++;

    char tag[64] = "", cls[64] = "", id[64] = "";
    const char *p = sel;
    size_t at = 0;
    while (*p != '\0' && *p != '.' && *p != '#' && *p != ' ' &&
           at + 1 < sizeof(tag)) {
        tag[at++] = *p++;
    }
    tag[at] = '\0';

    if (*p == '#') {
        p++;
        at = 0;
        while (*p != '\0' && *p != '.' && *p != ' ' && at + 1 < sizeof(id)) {
            id[at++] = *p++;
        }
        id[at] = '\0';
    }
    if (*p == '.') {
        p++;
        at = 0;
        while (*p != '\0' && *p != '.' && *p != ' ' && at + 1 < sizeof(cls)) {
            cls[at++] = *p++;
        }
        cls[at] = '\0';
    }

    for (struct dom_node *n = root; n != NULL; n = dom_next(n, root)) {
        if (n->type != DOM_ELEMENT || n == root) continue;
        if (tag[0] != '\0' && !ci_same(n->name, tag)) continue;
        if (id[0] != '\0') {
            const char *v = dom_attr(n, "id");
            if (v == NULL || strcmp(v, id) != 0) continue;
        }
        if (cls[0] != '\0' && !has_class(n, cls)) continue;

        if (all == NULL) return n;
        js_list_add(J, all, n);
    }
    return NULL;
}

/* ---- the element methods ---- */

static struct dom_node *self_node(struct js_value self) {
    if (self.type != JS_OBJ || self.obj == NULL) return NULL;
    return self.obj->node;
}

static struct js_value fn_get_by_id(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    struct dom_node *root = self_node(self);
    if (root == NULL || argc < 1) return js_null();
    return node_value(J, by_id(root, js_to_string(J, argv[0])));
}

static struct js_value fn_get_by_tag(struct js *J, struct js_value self,
                                     int argc, struct js_value *argv) {
    struct dom_node *root = self_node(self);
    struct js_obj *list = js_new_list(J);
    if (root != NULL && argc > 0) {
        collect(J, list, root, js_to_string(J, argv[0]), NULL);
    }
    return js_object(list);
}

static struct js_value fn_get_by_class(struct js *J, struct js_value self,
                                       int argc, struct js_value *argv) {
    struct dom_node *root = self_node(self);
    struct js_obj *list = js_new_list(J);
    if (root != NULL && argc > 0) {
        collect(J, list, root, NULL, js_to_string(J, argv[0]));
    }
    return js_object(list);
}

static struct js_value fn_query(struct js *J, struct js_value self, int argc,
                                struct js_value *argv) {
    struct dom_node *root = self_node(self);
    if (root == NULL || argc < 1) return js_null();
    return node_value(J, query(J, root, js_to_string(J, argv[0]), NULL));
}

static struct js_value fn_query_all(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    struct dom_node *root = self_node(self);
    struct js_obj *list = js_new_list(J);
    if (root != NULL && argc > 0) {
        query(J, root, js_to_string(J, argv[0]), list);
    }
    return js_object(list);
}

static struct js_value fn_create_element(struct js *J, struct js_value self,
                                         int argc, struct js_value *argv) {
    (void)self;
    if (argc < 1) return js_null();
    return node_value(J, dom_create_element(js_to_string(J, argv[0])));
}

static struct js_value fn_create_text(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    return node_value(J, dom_create_text(argc > 0 ? js_to_string(J, argv[0])
                                                  : ""));
}

static struct js_value fn_append_child(struct js *J, struct js_value self,
                                       int argc, struct js_value *argv) {
    struct dom_node *parent = self_node(self);
    struct dom_node *child = argc > 0 ? self_node(argv[0]) : NULL;
    if (parent == NULL || child == NULL) return js_null();
    dom_append(parent, child);
    changed(J);
    return argv[0];
}

static struct js_value fn_insert_before(struct js *J, struct js_value self,
                                        int argc, struct js_value *argv) {
    struct dom_node *parent = self_node(self);
    struct dom_node *child = argc > 0 ? self_node(argv[0]) : NULL;
    struct dom_node *before = argc > 1 ? self_node(argv[1]) : NULL;
    if (parent == NULL || child == NULL) return js_null();
    dom_insert_before(parent, child, before);
    changed(J);
    return argv[0];
}

static struct js_value fn_remove_child(struct js *J, struct js_value self,
                                       int argc, struct js_value *argv) {
    (void)self;
    struct dom_node *child = argc > 0 ? self_node(argv[0]) : NULL;
    if (child == NULL) return js_null();
    dom_remove(child);
    changed(J);
    return argv[0];
}

static struct js_value fn_remove_self(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)argc;
    (void)argv;
    struct dom_node *n = self_node(self);
    if (n == NULL) return js_undefined();
    dom_remove(n);
    changed(J);
    return js_undefined();
}

static struct js_value fn_get_attribute(struct js *J, struct js_value self,
                                        int argc, struct js_value *argv) {
    struct dom_node *n = self_node(self);
    if (n == NULL || argc < 1) return js_null();
    const char *v = dom_attr(n, js_to_string(J, argv[0]));
    return v != NULL ? js_string(J, v) : js_null();
}

static struct js_value fn_set_attribute(struct js *J, struct js_value self,
                                        int argc, struct js_value *argv) {
    struct dom_node *n = self_node(self);
    if (n == NULL || argc < 2) return js_undefined();
    dom_set_attr(n, js_to_string(J, argv[0]), js_to_string(J, argv[1]));
    changed(J);
    return js_undefined();
}

static struct js_value fn_remove_attribute(struct js *J, struct js_value self,
                                           int argc, struct js_value *argv) {
    struct dom_node *n = self_node(self);
    if (n == NULL || argc < 1) return js_undefined();
    dom_remove_attr(n, js_to_string(J, argv[0]));
    changed(J);
    return js_undefined();
}

static struct js_value fn_has_attribute(struct js *J, struct js_value self,
                                        int argc, struct js_value *argv) {
    struct dom_node *n = self_node(self);
    if (n == NULL || argc < 1) return js_bool(0);
    return js_bool(dom_attr(n, js_to_string(J, argv[0])) != NULL);
}

/* document.write, which is how a page from before the DOM built
 * itself: the markup goes where the document is being read. */
static struct js_value fn_document_write(struct js *J, struct js_value self,
                                         int argc, struct js_value *argv) {
    struct dom_node *doc = self_node(self);
    if (doc == NULL) return js_undefined();

    struct dom_node *body = NULL;
    for (struct dom_node *n = doc; n != NULL; n = dom_next(n, doc)) {
        if (name_is(n, "body")) {
            body = n;
            break;
        }
    }
    if (body == NULL) body = doc;

    for (int i = 0; i < argc; i++) {
        const char *html = js_to_string(J, argv[i]);
        struct dom_node *fragment = dom_parse_fragment(html, strlen(html));
        if (fragment == NULL) continue;
        struct dom_node *c = fragment->first;
        while (c != NULL) {
            struct dom_node *next = c->next;
            dom_append(body, c);
            c = next;
        }
        dom_free(fragment);
    }
    changed(J);
    return js_undefined();
}

/* ---- listeners and clicking ---- */

static struct js_value fn_add_listener(struct js *J, struct js_value self,
                                       int argc, struct js_value *argv) {
    if (argc < 2) return js_undefined();
    if (J->nlisteners >= JS_LISTENER_MAX) return js_undefined();

    struct js_listener *l = &J->listeners[J->nlisteners++];
    l->node = self_node(self);
    snprintf(l->type, sizeof(l->type), "%s", js_to_string(J, argv[0]));
    l->fn = argv[1];
    return js_undefined();
}

static struct js_value fn_remove_listener(struct js *J, struct js_value self,
                                          int argc, struct js_value *argv) {
    if (argc < 2) return js_undefined();
    struct dom_node *n = self_node(self);
    const char *type = js_to_string(J, argv[0]);

    for (int i = 0; i < J->nlisteners; i++) {
        if (J->listeners[i].node != n) continue;
        if (strcmp(J->listeners[i].type, type) != 0) continue;
        if (J->listeners[i].fn.obj != argv[1].obj) continue;
        J->listeners[i] = J->listeners[--J->nlisteners];
        return js_undefined();
    }
    return js_undefined();
}

static struct js_value fn_prevent_default(struct js *J, struct js_value self,
                                          int argc, struct js_value *argv) {
    (void)J;
    (void)argc;
    (void)argv;
    if (self.type == JS_OBJ && self.obj != NULL) self.obj->prevented = 1;
    return js_undefined();
}

static struct js_value fn_click(struct js *J, struct js_value self, int argc,
                                struct js_value *argv) {
    (void)argc;
    (void)argv;
    struct dom_node *n = self_node(self);
    if (n != NULL) js_click(J, n);
    return js_undefined();
}

static struct js_value fn_nothing(struct js *J, struct js_value self,
                                  int argc, struct js_value *argv) {
    (void)J;
    (void)self;
    (void)argc;
    (void)argv;
    return js_undefined();
}

int js_has_click_handler(struct js *J, struct dom_node *element) {
    if (J == NULL) return 0;
    for (struct dom_node *n = element; n != NULL; n = n->parent) {
        if (n->type == DOM_ELEMENT && dom_attr(n, "onclick") != NULL) return 1;
        for (int i = 0; i < J->nlisteners; i++) {
            if (J->listeners[i].node == n &&
                strcmp(J->listeners[i].type, "click") == 0) {
                return 1;
            }
        }
    }
    return 0;
}

int js_click(struct js *J, struct dom_node *element) {
    if (J == NULL || element == NULL) return 0;

    struct js_obj *event = js_new_object(J, OBJ_EVENT);
    if (event == NULL) return 0;
    event->node = element;
    struct js_value ev = js_object(event);

    int ran = 0;
    for (struct dom_node *n = element; n != NULL; n = n->parent) {
        if (n->type != DOM_ELEMENT) continue;

        for (int i = 0; i < J->nlisteners; i++) {
            if (J->listeners[i].node != n) continue;
            if (strcmp(J->listeners[i].type, "click") != 0) continue;

            J->signal = SIG_NONE;
            J->steps = 0;
            js_call(J, J->listeners[i].fn, node_value(J, n), 1, &ev);
            ran = 1;
            if ((J->signal == SIG_THROW || J->signal == SIG_ERROR) &&
                J->host.script_error != NULL) {
                J->host.script_error(J->host.ctx, J->error);
            }
            J->signal = SIG_NONE;
        }

        const char *handler = dom_attr(n, "onclick");
        if (handler != NULL) {
            /* An attribute handler runs with the element as `this`
             * and `event` in scope, and its `return false` cancels
             * whatever the click would otherwise have done. */
            if (js_run_handler(J, n, handler, ev)) event->prevented = 1;
            ran = 1;
        }
        if (event->prevented) break;
    }

    if (ran) changed(J);
    return event->prevented;
}

/* ---- element properties ---- */

/* style="a:b;c:d" as a script edits it, one declaration at a time. */
static void style_set(struct js *J, struct dom_node *n, const char *prop,
                      const char *value) {
    const char *current = dom_attr(n, "style");
    size_t len = (current != NULL ? strlen(current) : 0) + strlen(prop) +
                 strlen(value) + 8;
    char *out = malloc(len);
    if (out == NULL) return;
    out[0] = '\0';

    /* Copy every declaration except the one being replaced. */
    size_t at = 0;
    for (const char *p = current != NULL ? current : ""; *p != '\0';) {
        while (*p == ' ' || *p == ';') p++;
        if (*p == '\0') break;
        const char *decl = p;
        while (*p != '\0' && *p != ';') p++;
        size_t dlen = (size_t)(p - decl);

        const char *colon = memchr(decl, ':', dlen);
        size_t nlen = colon != NULL ? (size_t)(colon - decl) : dlen;
        while (nlen > 0 && decl[nlen - 1] == ' ') nlen--;

        if (nlen == strlen(prop) && strncmp(decl, prop, nlen) == 0) continue;
        at += (size_t)snprintf(out + at, len - at, "%.*s;", (int)dlen, decl);
    }
    if (value != NULL && *value != '\0') {
        snprintf(out + at, len - at, "%s:%s;", prop, value);
    }

    dom_set_attr(n, "style", out);
    free(out);
    changed(J);
}

static const char *style_get(struct dom_node *n, const char *prop,
                             char *buf, size_t size) {
    const char *current = dom_attr(n, "style");
    buf[0] = '\0';
    if (current == NULL) return buf;

    for (const char *p = current; *p != '\0';) {
        while (*p == ' ' || *p == ';') p++;
        if (*p == '\0') break;
        const char *decl = p;
        while (*p != '\0' && *p != ';') p++;
        size_t dlen = (size_t)(p - decl);

        const char *colon = memchr(decl, ':', dlen);
        if (colon == NULL) continue;
        size_t nlen = (size_t)(colon - decl);
        while (nlen > 0 && decl[nlen - 1] == ' ') nlen--;
        if (nlen != strlen(prop) || strncmp(decl, prop, nlen) != 0) continue;

        const char *v = colon + 1;
        while (*v == ' ') v++;
        size_t vlen = dlen - (size_t)(v - decl);
        if (vlen >= size) vlen = size - 1;
        memcpy(buf, v, vlen);
        buf[vlen] = '\0';
        return buf;
    }
    return buf;
}

/* backgroundColor -> background-color */
static void camel_to_css(const char *name, char *out, size_t size) {
    size_t at = 0;
    for (const char *p = name; *p != '\0' && at + 2 < size; p++) {
        if (*p >= 'A' && *p <= 'Z') {
            out[at++] = '-';
            out[at++] = (char)(*p + 32);
        } else {
            out[at++] = *p;
        }
    }
    out[at] = '\0';
}

struct js_value jsdom_style_get(struct js *J, struct js_obj *obj,
                                const char *name, int *handled) {
    *handled = 1;
    if (obj->node == NULL) return js_string(J, "");

    char prop[64], buf[256];
    camel_to_css(name, prop, sizeof(prop));
    return js_string(J, style_get(obj->node, prop, buf, sizeof(buf)));
}

int jsdom_style_set(struct js *J, struct js_obj *obj, const char *name,
                    struct js_value v) {
    if (obj->node == NULL) return 1;
    char prop[64];
    camel_to_css(name, prop, sizeof(prop));
    style_set(J, obj->node, prop, js_to_string(J, v));
    return 1;
}

struct js_value jsdom_event_get(struct js *J, struct js_obj *obj,
                                const char *name, int *handled) {
    *handled = 1;
    if (strcmp(name, "type") == 0) return js_string(J, "click");
    if (strcmp(name, "target") == 0 || strcmp(name, "currentTarget") == 0 ||
        strcmp(name, "srcElement") == 0) {
        return node_value(J, obj->node);
    }
    if (strcmp(name, "preventDefault") == 0) {
        return js_object(js_new_native(J, "preventDefault", fn_prevent_default));
    }
    if (strcmp(name, "stopPropagation") == 0) {
        return js_object(js_new_native(J, "stopPropagation", fn_nothing));
    }
    if (strcmp(name, "defaultPrevented") == 0) {
        return js_bool(obj->prevented);
    }
    *handled = 0;
    return js_undefined();
}

struct js_value jsdom_list_get(struct js *J, struct js_obj *obj,
                               const char *name, int *handled) {
    *handled = 1;
    if (strcmp(name, "length") == 0) return js_number(obj->nnodes);

    if (name[0] >= '0' && name[0] <= '9') {
        int index = atoi(name);
        if (index >= 0 && index < obj->nnodes) {
            return node_value(J, obj->nodes[index]);
        }
        return js_undefined();
    }
    if (strcmp(name, "item") == 0) {
        /* The list is indexable, so item() is the same lookup. */
        *handled = 0;
        return js_undefined();
    }
    *handled = 0;
    return js_undefined();
}

/* ---- classList ---- */

/* The class attribute as a list of words: rewritten whole each time,
 * because a class attribute is short and the alternative is a parser
 * that has to agree with the one in css.c. */
static void class_write(struct js *J, struct dom_node *n, const char *add,
                        const char *drop) {
    const char *current = dom_attr(n, "class");
    size_t len = (current != NULL ? strlen(current) : 0) +
                 (add != NULL ? strlen(add) : 0) + 4;
    char *out = malloc(len);
    if (out == NULL) return;
    out[0] = '\0';
    size_t at = 0;

    for (const char *p = current != NULL ? current : ""; *p != '\0';) {
        while (*p == ' ' || *p == '\t' || *p == '\n') p++;
        const char *start = p;
        while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\n') p++;
        size_t wlen = (size_t)(p - start);
        if (wlen == 0) continue;
        if (drop != NULL && wlen == strlen(drop) &&
            strncmp(start, drop, wlen) == 0) {
            continue;
        }
        if (add != NULL && wlen == strlen(add) &&
            strncmp(start, add, wlen) == 0) {
            add = NULL;   /* already there */
        }
        at += (size_t)snprintf(out + at, len - at, "%s%.*s",
                               at > 0 ? " " : "", (int)wlen, start);
    }
    if (add != NULL && *add != '\0') {
        snprintf(out + at, len - at, "%s%s", at > 0 ? " " : "", add);
    }

    dom_set_attr(n, "class", out);
    free(out);
    changed(J);
}

static struct js_value fn_class_add(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    struct dom_node *n = self_node(self);
    for (int i = 0; i < argc && n != NULL; i++) {
        class_write(J, n, js_to_string(J, argv[i]), NULL);
    }
    return js_undefined();
}

static struct js_value fn_class_remove(struct js *J, struct js_value self,
                                       int argc, struct js_value *argv) {
    struct dom_node *n = self_node(self);
    for (int i = 0; i < argc && n != NULL; i++) {
        class_write(J, n, NULL, js_to_string(J, argv[i]));
    }
    return js_undefined();
}

static struct js_value fn_class_contains(struct js *J, struct js_value self,
                                         int argc, struct js_value *argv) {
    struct dom_node *n = self_node(self);
    if (n == NULL || argc < 1) return js_bool(0);
    return js_bool(has_class(n, js_to_string(J, argv[0])));
}

static struct js_value fn_class_toggle(struct js *J, struct js_value self,
                                       int argc, struct js_value *argv) {
    struct dom_node *n = self_node(self);
    if (n == NULL || argc < 1) return js_bool(0);
    const char *name = js_to_string(J, argv[0]);
    int on = has_class(n, name);
    class_write(J, n, on ? NULL : name, on ? name : NULL);
    return js_bool(!on);
}

/* ---- the properties of an element ---- */

struct js_value jsdom_node_get(struct js *J, struct js_obj *obj,
                               const char *name, int *handled) {
    struct dom_node *n = obj->node;
    *handled = 1;
    if (n == NULL) return js_undefined();

    /* The methods every element and the document both answer to. */
    if (strcmp(name, "getElementById") == 0) {
        return js_object(js_new_native(J, name, fn_get_by_id));
    }
    if (strcmp(name, "getElementsByTagName") == 0) {
        return js_object(js_new_native(J, name, fn_get_by_tag));
    }
    if (strcmp(name, "getElementsByClassName") == 0) {
        return js_object(js_new_native(J, name, fn_get_by_class));
    }
    if (strcmp(name, "querySelector") == 0) {
        return js_object(js_new_native(J, name, fn_query));
    }
    if (strcmp(name, "querySelectorAll") == 0) {
        return js_object(js_new_native(J, name, fn_query_all));
    }
    if (strcmp(name, "createElement") == 0) {
        return js_object(js_new_native(J, name, fn_create_element));
    }
    if (strcmp(name, "createTextNode") == 0) {
        return js_object(js_new_native(J, name, fn_create_text));
    }
    if (strcmp(name, "appendChild") == 0) {
        return js_object(js_new_native(J, name, fn_append_child));
    }
    if (strcmp(name, "insertBefore") == 0) {
        return js_object(js_new_native(J, name, fn_insert_before));
    }
    if (strcmp(name, "removeChild") == 0) {
        return js_object(js_new_native(J, name, fn_remove_child));
    }
    if (strcmp(name, "remove") == 0) {
        return js_object(js_new_native(J, name, fn_remove_self));
    }
    if (strcmp(name, "getAttribute") == 0) {
        return js_object(js_new_native(J, name, fn_get_attribute));
    }
    if (strcmp(name, "setAttribute") == 0) {
        return js_object(js_new_native(J, name, fn_set_attribute));
    }
    if (strcmp(name, "removeAttribute") == 0) {
        return js_object(js_new_native(J, name, fn_remove_attribute));
    }
    if (strcmp(name, "hasAttribute") == 0) {
        return js_object(js_new_native(J, name, fn_has_attribute));
    }
    if (strcmp(name, "addEventListener") == 0) {
        return js_object(js_new_native(J, name, fn_add_listener));
    }
    if (strcmp(name, "removeEventListener") == 0) {
        return js_object(js_new_native(J, name, fn_remove_listener));
    }
    if (strcmp(name, "write") == 0 || strcmp(name, "writeln") == 0) {
        return js_object(js_new_native(J, name, fn_document_write));
    }
    if (strcmp(name, "click") == 0) {
        return js_object(js_new_native(J, name, fn_click));
    }
    if (strcmp(name, "focus") == 0 || strcmp(name, "blur") == 0 ||
        strcmp(name, "scrollIntoView") == 0) {
        return js_object(js_new_native(J, name, fn_nothing));
    }

    /* Where this node sits. */
    if (strcmp(name, "parentNode") == 0 || strcmp(name, "parentElement") == 0) {
        return node_value(J, n->parent);
    }
    if (strcmp(name, "firstChild") == 0) return node_value(J, n->first);
    if (strcmp(name, "lastChild") == 0) return node_value(J, n->last);
    if (strcmp(name, "nextSibling") == 0) return node_value(J, n->next);
    if (strcmp(name, "previousSibling") == 0) return node_value(J, n->prev);
    if (strcmp(name, "childNodes") == 0 || strcmp(name, "children") == 0) {
        struct js_obj *list = js_new_list(J);
        for (struct dom_node *c = n->first; c != NULL; c = c->next) {
            if (strcmp(name, "children") == 0 && c->type != DOM_ELEMENT) {
                continue;
            }
            js_list_add(J, list, c);
        }
        return js_object(list);
    }

    /* What this node is. */
    if (strcmp(name, "tagName") == 0 || strcmp(name, "nodeName") == 0) {
        char upper[DOM_NAME_MAX];
        size_t i = 0;
        for (; n->name[i] != '\0' && i + 1 < sizeof(upper); i++) {
            upper[i] = (n->name[i] >= 'a' && n->name[i] <= 'z')
                           ? (char)(n->name[i] - 32) : n->name[i];
        }
        upper[i] = '\0';
        return js_string(J, upper);
    }
    if (strcmp(name, "nodeType") == 0) {
        return js_number(n->type == DOM_TEXT ? 3 : 1);
    }
    if (strcmp(name, "classList") == 0) {
        struct js_obj *list = js_new_object(J, OBJ_PLAIN);
        if (list != NULL) {
            list->node = n;
            js_def_native(J, list, "add", fn_class_add);
            js_def_native(J, list, "remove", fn_class_remove);
            js_def_native(J, list, "contains", fn_class_contains);
            js_def_native(J, list, "toggle", fn_class_toggle);
        }
        return js_object(list);
    }
    if (strcmp(name, "style") == 0) {
        struct js_obj *style = js_new_object(J, OBJ_STYLE);
        if (style != NULL) style->node = n;
        return js_object(style);
    }
    if (strcmp(name, "document") == 0) {
        return node_value(J, J->document);
    }
    if (strcmp(name, "body") == 0) {
        for (struct dom_node *c = J->document; c != NULL;
             c = dom_next(c, J->document)) {
            if (name_is(c, "body")) return node_value(J, c);
        }
        return js_null();
    }
    if (strcmp(name, "documentElement") == 0) {
        for (struct dom_node *c = J->document; c != NULL;
             c = dom_next(c, J->document)) {
            if (name_is(c, "html")) return node_value(J, c);
        }
        return node_value(J, J->document);
    }
    if (strcmp(name, "title") == 0) {
        for (struct dom_node *c = J->document; c != NULL;
             c = dom_next(c, J->document)) {
            if (name_is(c, "title")) {
                char buf[256];
                dom_text_content(c, buf, sizeof(buf));
                return js_string(J, buf);
            }
        }
        return js_string(J, "");
    }

    /* What is inside it. */
    if (strcmp(name, "innerHTML") == 0 || strcmp(name, "outerHTML") == 0) {
        size_t want = dom_serialize(n, strcmp(name, "innerHTML") == 0, NULL, 0);
        char *buf = js_alloc(J, want + 2);
        if (buf == NULL) return js_string(J, "");
        dom_serialize(n, strcmp(name, "innerHTML") == 0, buf, want + 2);
        return js_string(J, buf);
    }
    if (strcmp(name, "textContent") == 0 || strcmp(name, "innerText") == 0) {
        char buf[4096];
        dom_text_content(n, buf, sizeof(buf));
        return js_string(J, buf);
    }
    if (strcmp(name, "nodeValue") == 0 || strcmp(name, "data") == 0) {
        return js_string(J, n->text != NULL ? n->text : "");
    }

    if (strcmp(name, "className") == 0) {
        const char *v = dom_attr(n, "class");
        return js_string(J, v != NULL ? v : "");
    }
    if (strcmp(name, "checked") == 0 || strcmp(name, "disabled") == 0 ||
        strcmp(name, "selected") == 0) {
        return js_bool(dom_attr(n, name) != NULL);
    }

    /* Anything else with the name of an attribute is that attribute:
     * id, value, href, src, name, type, alt, title. */
    const char *v = dom_attr(n, name);
    if (v != NULL) return js_string(J, v);

    static const char *ATTRS[] = { "id", "value", "href", "src", "name",
                                   "type", "alt", "placeholder", "target",
                                   "rel", "action", "method", NULL };
    for (int i = 0; ATTRS[i] != NULL; i++) {
        if (strcmp(name, ATTRS[i]) == 0) return js_string(J, "");
    }

    *handled = 0;
    return js_undefined();
}

int jsdom_node_set(struct js *J, struct js_obj *obj, const char *name,
                   struct js_value v) {
    struct dom_node *n = obj->node;
    if (n == NULL) return 0;

    if (strcmp(name, "innerHTML") == 0) {
        const char *html = js_to_string(J, v);
        struct dom_node *fragment = dom_parse_fragment(html, strlen(html));
        dom_free_children(n);
        if (fragment != NULL) {
            struct dom_node *c = fragment->first;
            while (c != NULL) {
                struct dom_node *next = c->next;
                dom_append(n, c);
                c = next;
            }
            dom_free(fragment);
        }
        changed(J);
        return 1;
    }

    if (strcmp(name, "textContent") == 0 || strcmp(name, "innerText") == 0 ||
        strcmp(name, "nodeValue") == 0) {
        const char *text = js_to_string(J, v);
        if (n->type == DOM_TEXT) {
            free(n->text);
            n->text = strdup(text);
        } else {
            dom_free_children(n);
            dom_append(n, dom_create_text(text));
        }
        changed(J);
        return 1;
    }

    if (strcmp(name, "className") == 0) {
        dom_set_attr(n, "class", js_to_string(J, v));
        changed(J);
        return 1;
    }

    if (strcmp(name, "checked") == 0 || strcmp(name, "disabled") == 0 ||
        strcmp(name, "selected") == 0) {
        if (js_truthy(v)) dom_set_attr(n, name, name);
        else dom_remove_attr(n, name);
        changed(J);
        return 1;
    }

    if (strcmp(name, "onclick") == 0) {
        /* Assigning a function to onclick is a listener by another
         * name; assigning a string is markup by another name. */
        if (v.type == JS_OBJ && J->nlisteners < JS_LISTENER_MAX) {
            struct js_listener *l = &J->listeners[J->nlisteners++];
            l->node = n;
            snprintf(l->type, sizeof(l->type), "click");
            l->fn = v;
        } else if (v.type == JS_STR) {
            dom_set_attr(n, "onclick", js_to_string(J, v));
        }
        return 1;
    }

    static const char *ATTRS[] = { "id", "value", "href", "src", "name",
                                   "type", "alt", "placeholder", "target",
                                   "rel", "action", "method", "title", NULL };
    for (int i = 0; ATTRS[i] != NULL; i++) {
        if (strcmp(name, ATTRS[i]) == 0) {
            dom_set_attr(n, name, js_to_string(J, v));
            changed(J);
            return 1;
        }
    }
    return 0;
}

/* ---- window ---- */

static struct js_value fn_location_assign(struct js *J, struct js_value self,
                                          int argc, struct js_value *argv) {
    (void)self;
    if (argc > 0 && J->host.navigate != NULL) {
        J->host.navigate(J->host.ctx, js_to_string(J, argv[0]));
    }
    return js_undefined();
}

static struct js_value fn_location_reload(struct js *J, struct js_value self,
                                          int argc, struct js_value *argv) {
    (void)self;
    (void)argc;
    (void)argv;
    if (J->host.navigate != NULL && J->host.location != NULL) {
        J->host.navigate(J->host.ctx, J->host.location(J->host.ctx));
    }
    return js_undefined();
}

/*
 * location is an object whose href, when assigned, navigates - which
 * is the one piece of the browser object model that pages use to move
 * the reader somewhere else.
 */
static struct js_value make_location(struct js *J) {
    struct js_obj *loc = js_new_object(J, OBJ_PLAIN);
    const char *href = J->host.location != NULL ? J->host.location(J->host.ctx)
                                                : "";

    js_def(J, loc, "href", js_string(J, href));
    js_def_native(J, loc, "assign", fn_location_assign);
    js_def_native(J, loc, "replace", fn_location_assign);
    js_def_native(J, loc, "reload", fn_location_reload);

    /* The pieces of the URL, split here rather than in every page. */
    const char *host_start = strstr(href, "://");
    host_start = host_start != NULL ? host_start + 3 : href;
    const char *path = strchr(host_start, '/');
    const char *query = strchr(href, '?');

    char buf[512];
    size_t hlen = path != NULL ? (size_t)(path - host_start) : strlen(host_start);
    if (hlen >= sizeof(buf)) hlen = sizeof(buf) - 1;
    memcpy(buf, host_start, hlen);
    buf[hlen] = '\0';
    js_def(J, loc, "host", js_string(J, buf));
    js_def(J, loc, "hostname", js_string(J, buf));
    js_def(J, loc, "protocol",
           js_string(J, strncmp(href, "https", 5) == 0 ? "https:" : "http:"));
    js_def(J, loc, "pathname", js_string(J, path != NULL ? path : "/"));
    js_def(J, loc, "search", js_string(J, query != NULL ? query : ""));
    return js_object(loc);
}

void jsdom_install(struct js *J) {
    struct js_obj *w = J->window;

    J->document_obj = js_new_node(J, J->document);
    js_def(J, w, "document", js_object(J->document_obj));
    js_def(J, w, "window", js_object(w));
    js_def(J, w, "self", js_object(w));
    js_def(J, w, "location", make_location(J));

    struct js_obj *nav = js_new_object(J, OBJ_PLAIN);
    js_def(J, nav, "userAgent", js_string(J, "Clint/1.0 (TUS)"));
    js_def(J, nav, "platform", js_string(J, "TUS"));
    js_def(J, nav, "language", js_string(J, "en"));
    js_def(J, w, "navigator", js_object(nav));

    js_def(J, w, "innerWidth", js_number(900));
    js_def(J, w, "innerHeight", js_number(620));

    js_def_native(J, w, "addEventListener", fn_add_listener);
    js_def_native(J, w, "removeEventListener", fn_remove_listener);
    js_def_native(J, w, "scrollTo", fn_nothing);
    js_def_native(J, w, "focus", fn_nothing);
}
