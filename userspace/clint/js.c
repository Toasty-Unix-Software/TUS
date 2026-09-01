/*
 * js.c - see js.h
 *
 * The interpreter is three passes and no magic: the source is
 * tokenised in one go, parsed into a tree, and the tree is walked.
 * Tokenising everything first is what makes the parser simple - it
 * can look ahead as far as it likes, which is the only way to tell
 * `(a, b) => a + b` from `(a, b)` without a second grammar.
 *
 * Errors, `return`, `break`, `continue` and `throw` all leave a
 * running statement the same way: a signal on the interpreter, which
 * every evaluation checks for and passes upwards. That is more
 * bookkeeping than longjmp, and it means an interpreter error can
 * never skip past a caller that was holding something.
 */

#include "js.h"
#include "jsi.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JS_ALLOC_LIMIT (12u * 1024 * 1024)
#define JS_STEP_LIMIT  4000000u
#define JS_DEPTH_LIMIT 128

/* eval()/exec() recursion depth, separate from JS_DEPTH_LIMIT above
 * (which only bounds function CALL nesting). A long flat chain like
 * `a+b+c+...` never calls a function, but the parser still builds a
 * left-nested N_BINARY tree one level deep per term, and evaluating
 * that tree recurses just as deeply - confirmed with ASan
 * (stack-overflow in eval(), js.c) on a ~250-term chain against an
 * 8 MiB host stack. TUS's real user stack is 64 KiB
 * (kernel/sched/sched.c: USER_STACK_SIZE), a small fraction of that,
 * so a page shipping a long generated/minified expression can hit
 * this with far fewer terms - this is what actually crashed Clint
 * (`executing unmapped memory at 0x0`: a stack overflow with no guard
 * page below it corrupts whatever is past the stack rather than
 * faulting predictably). Kept deliberately conservative rather than
 * tuned to the exact real frame size, which this host cannot measure
 * accurately for a target with a different libc/ABI/stack layout. */
#define JS_TREE_DEPTH_LIMIT 48

/* ---- memory ---- */

void *js_alloc(struct js *J, size_t size) {
    if (J->allocated + size > J->alloc_limit) {
        js_error(J, "the script asked for more memory than a page may have");
        return NULL;
    }
    void *p = calloc(1, size);
    if (p == NULL) {
        js_error(J, "out of memory");
        return NULL;
    }
    J->allocated += size;
    return p;
}

char *js_strdup(struct js *J, const char *text) {
    size_t len = strlen(text);
    char *p = js_alloc(J, len + 1);
    if (p != NULL) memcpy(p, text, len + 1);
    return p;
}

/* Every string is remembered so the page can free them all at once. */
static struct js_str *str_new(struct js *J, const char *text, size_t len) {
    struct js_str *s = js_alloc(J, sizeof(*s));
    if (s == NULL) return NULL;
    s->text = js_alloc(J, len + 1);
    if (s->text == NULL) return NULL;
    memcpy(s->text, text, len);
    s->text[len] = '\0';
    s->len = len;

    if (J->nstrings == J->strings_cap) {
        int want = J->strings_cap ? J->strings_cap * 2 : 64;
        struct js_str **p = realloc(J->strings, (size_t)want * sizeof(*p));
        if (p == NULL) {
            js_error(J, "out of memory");
            return s;
        }
        J->strings = p;
        J->strings_cap = want;
    }
    J->strings[J->nstrings++] = s;
    return s;
}

/* ---- values ---- */

/* Written as a product so the compiler does not have to hold an
 * out-of-range constant. */
static double js_infinity(void) {
    double big = 1e308;
    return big * 10;
}

struct js_value js_undefined(void) {
    struct js_value v;
    memset(&v, 0, sizeof(v));
    v.type = JS_UNDEF;
    return v;
}

struct js_value js_null(void) {
    struct js_value v = js_undefined();
    v.type = JS_NULL;
    return v;
}

struct js_value js_bool(int b) {
    struct js_value v = js_undefined();
    v.type = JS_BOOL;
    v.num = b ? 1 : 0;
    return v;
}

struct js_value js_number(double n) {
    struct js_value v = js_undefined();
    v.type = JS_NUM;
    v.num = n;
    return v;
}

struct js_value js_string_len(struct js *J, const char *text, size_t len) {
    struct js_value v = js_undefined();
    struct js_str *s = str_new(J, text != NULL ? text : "", text ? len : 0);
    if (s == NULL) return v;
    v.type = JS_STR;
    v.str = s;
    return v;
}

struct js_value js_string(struct js *J, const char *text) {
    return js_string_len(J, text, text != NULL ? strlen(text) : 0);
}

struct js_value js_object(struct js_obj *obj) {
    struct js_value v = js_undefined();
    if (obj == NULL) return v;
    v.type = JS_OBJ;
    v.obj = obj;
    return v;
}

int js_truthy(struct js_value v) {
    switch (v.type) {
    case JS_UNDEF:
    case JS_NULL:  return 0;
    case JS_BOOL:
    case JS_NUM:   return v.num != 0 && !(v.num != v.num);
    case JS_STR:   return v.str != NULL && v.str->len > 0;
    default:       return 1;
    }
}

/*
 * Numbers as JavaScript writes them: integers without a point, the
 * rest with as few digits as still read back the same. %.17g is
 * always exact and usually ugly, so the shortest form that survives
 * a round trip wins.
 */
const char *js_number_to_text(struct js *J, double n) {
    char buf[40];

    if (n != n) {
        return "NaN";
    }
    if (n * 0.0 != 0.0) {   /* an infinity times zero is not zero */
        return n < 0 ? "-Infinity" : "Infinity";
    }
    if (n == 0) return "0";

    if (n == (double)(long long)n && n < 1e15 && n > -1e15) {
        snprintf(buf, sizeof(buf), "%lld", (long long)n);
    } else {
        for (int prec = 1; prec <= 17; prec++) {
            snprintf(buf, sizeof(buf), "%.*g", prec, n);
            if (strtod(buf, NULL) == n) break;
        }
    }
    struct js_value v = js_string(J, buf);
    return v.type == JS_STR ? v.str->text : "";
}

const char *js_to_string(struct js *J, struct js_value v) {
    switch (v.type) {
    case JS_UNDEF: return "undefined";
    case JS_NULL:  return "null";
    case JS_BOOL:  return v.num != 0 ? "true" : "false";
    case JS_NUM:   return js_number_to_text(J, v.num);
    case JS_STR:   return v.str != NULL ? v.str->text : "";
    default: break;
    }

    struct js_obj *o = v.obj;
    if (o == NULL) return "null";
    if (o->kind == OBJ_FUNC || o->kind == OBJ_NATIVE) return "function";
    if (o->kind == OBJ_NODE) {
        if (o->node != NULL && o->node->type == DOM_ELEMENT) {
            static char buf[64];
            snprintf(buf, sizeof(buf), "[object HTML%sElement]", o->node->name);
            return buf;
        }
        return "[object HTMLDocument]";
    }
    if (o->kind == OBJ_ARRAY) {
        /* Array.prototype.toString is join(","), which is what a page
         * that prints an array expects to see. */
        size_t total = 1;
        for (int i = 0; i < o->len; i++) {
            total += strlen(js_to_string(J, o->items[i])) + 1;
        }
        char *out = js_alloc(J, total);
        if (out == NULL) return "";
        out[0] = '\0';
        for (int i = 0; i < o->len; i++) {
            if (i > 0) strcat(out, ",");
            if (o->items[i].type != JS_UNDEF && o->items[i].type != JS_NULL) {
                strcat(out, js_to_string(J, o->items[i]));
            }
        }
        return out;
    }
    return "[object Object]";
}

double js_to_number(struct js *J, struct js_value v) {
    (void)J;
    switch (v.type) {
    case JS_UNDEF: return (double)0.0 / 0.0;   /* NaN */
    case JS_NULL:  return 0;
    case JS_BOOL:
    case JS_NUM:   return v.num;
    case JS_STR: {
        const char *p = v.str != NULL ? v.str->text : "";
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
        if (*p == '\0') return 0;
        char *end = NULL;
        double d = strtod(p, &end);
        if (end == p) return (double)0.0 / 0.0;
        while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') {
            end++;
        }
        return *end == '\0' ? d : (double)0.0 / 0.0;
    }
    default: break;
    }
    if (v.obj != NULL && v.obj->kind == OBJ_ARRAY && v.obj->len == 0) return 0;
    return (double)0.0 / 0.0;
}

int js_equals(struct js *J, struct js_value a, struct js_value b, int strict) {
    if (a.type == b.type) {
        switch (a.type) {
        case JS_UNDEF:
        case JS_NULL: return 1;
        case JS_BOOL:
        case JS_NUM:  return a.num == b.num;
        case JS_STR:  return strcmp(a.str->text, b.str->text) == 0;
        default:      return a.obj == b.obj;
        }
    }
    if (strict) return 0;

    /* Loose equality, in the two shapes pages actually write:
     * null == undefined, and everything else compared as numbers. */
    if ((a.type == JS_NULL && b.type == JS_UNDEF) ||
        (a.type == JS_UNDEF && b.type == JS_NULL)) {
        return 1;
    }
    if (a.type == JS_NULL || a.type == JS_UNDEF || b.type == JS_NULL ||
        b.type == JS_UNDEF) {
        return 0;
    }
    if (a.type == JS_STR && b.type == JS_STR) {
        return strcmp(a.str->text, b.str->text) == 0;
    }
    double x = js_to_number(J, a), y = js_to_number(J, b);
    return x == y;
}

/* ---- objects ---- */

static void track_object(struct js *J, struct js_obj *o) {
    o->next_alloc = J->objects;
    J->objects = o;
}

struct js_obj *js_new_object(struct js *J, int kind) {
    struct js_obj *o = js_alloc(J, sizeof(*o));
    if (o == NULL) return NULL;
    o->kind = (uint8_t)kind;
    track_object(J, o);
    return o;
}

struct js_obj *js_new_array(struct js *J) {
    return js_new_object(J, OBJ_ARRAY);
}

struct js_obj *js_new_native(struct js *J, const char *name, js_native_fn fn) {
    struct js_obj *o = js_new_object(J, OBJ_NATIVE);
    if (o == NULL) return NULL;
    o->native = fn;
    o->name = name;
    return o;
}

struct js_obj *js_new_node(struct js *J, struct dom_node *node) {
    if (node == NULL) return NULL;
    struct js_obj *o = js_new_object(J, OBJ_NODE);
    if (o != NULL) o->node = node;
    return o;
}

struct js_obj *js_new_list(struct js *J) {
    return js_new_object(J, OBJ_LIST);
}

void js_list_add(struct js *J, struct js_obj *list, struct dom_node *node) {
    if (list == NULL || node == NULL) return;
    struct dom_node **p = realloc(list->nodes,
                                  (size_t)(list->nnodes + 1) * sizeof(*p));
    if (p == NULL) {
        js_error(J, "out of memory");
        return;
    }
    list->nodes = p;
    list->nodes[list->nnodes++] = node;
}

void js_array_push(struct js *J, struct js_obj *arr, struct js_value v) {
    if (arr == NULL) return;
    if (arr->len == arr->cap) {
        int want = arr->cap ? arr->cap * 2 : 8;
        struct js_value *p = realloc(arr->items, (size_t)want * sizeof(*p));
        if (p == NULL) {
            js_error(J, "out of memory");
            return;
        }
        arr->items = p;
        arr->cap = want;
    }
    arr->items[arr->len++] = v;
}

static struct js_prop *prop_find(struct js_prop *list, const char *name) {
    for (struct js_prop *p = list; p != NULL; p = p->next) {
        if (strcmp(p->name, name) == 0) return p;
    }
    return NULL;
}

void js_def(struct js *J, struct js_obj *obj, const char *name,
            struct js_value v) {
    if (obj == NULL) return;
    struct js_prop *p = prop_find(obj->props, name);
    if (p != NULL) {
        p->value = v;
        return;
    }
    p = js_alloc(J, sizeof(*p));
    if (p == NULL) return;
    p->name = js_strdup(J, name);
    p->value = v;
    p->next = obj->props;
    obj->props = p;
}

void js_def_native(struct js *J, struct js_obj *obj, const char *name,
                   js_native_fn fn) {
    js_def(J, obj, name, js_object(js_new_native(J, name, fn)));
}

/* Array indexing arrives here as a property name, because that is
 * what JavaScript says an index is. */
static int index_of_name(const char *name, int *out) {
    if (*name < '0' || *name > '9') return 0;
    int v = 0;
    for (const char *p = name; *p != '\0'; p++) {
        if (*p < '0' || *p > '9') return 0;
        v = v * 10 + (*p - '0');
    }
    *out = v;
    return 1;
}

struct js_value string_member(struct js *J, struct js_value self,
                              const char *name);
struct js_value array_member(struct js *J, struct js_obj *arr,
                             const char *name);
struct js_value number_member(struct js *J, struct js_value self,
                              const char *name);

struct js_value js_get(struct js *J, struct js_value target, const char *name) {
    if (target.type == JS_STR) return string_member(J, target, name);
    if (target.type == JS_NUM) return number_member(J, target, name);

    if (target.type != JS_OBJ || target.obj == NULL) {
        if (target.type == JS_UNDEF || target.type == JS_NULL) {
            js_throw(J, "cannot read a property of nothing");
        }
        return js_undefined();
    }

    struct js_obj *o = target.obj;

    if (o->kind == OBJ_NODE) {
        int handled = 0;
        struct js_value v = jsdom_node_get(J, o, name, &handled);
        if (handled) return v;
    } else if (o->kind == OBJ_LIST) {
        int handled = 0;
        struct js_value v = jsdom_list_get(J, o, name, &handled);
        if (handled) return v;
    } else if (o->kind == OBJ_STYLE) {
        int handled = 0;
        struct js_value v = jsdom_style_get(J, o, name, &handled);
        if (handled) return v;
    } else if (o->kind == OBJ_EVENT) {
        int handled = 0;
        struct js_value v = jsdom_event_get(J, o, name, &handled);
        if (handled) return v;
    } else if (o->kind == OBJ_ARRAY) {
        int index;
        if (strcmp(name, "length") == 0) return js_number(o->len);
        if (index_of_name(name, &index)) {
            return index < o->len ? o->items[index] : js_undefined();
        }
        struct js_prop *own = prop_find(o->props, name);
        if (own != NULL) return own->value;
        return array_member(J, o, name);
    }

    struct js_prop *p = prop_find(o->props, name);
    if (p != NULL) return p->value;
    return js_undefined();
}

void js_set(struct js *J, struct js_value target, const char *name,
            struct js_value v) {
    if (target.type != JS_OBJ || target.obj == NULL) return;
    struct js_obj *o = target.obj;

    if (o->kind == OBJ_NODE && jsdom_node_set(J, o, name, v)) return;
    if (o->kind == OBJ_STYLE && jsdom_style_set(J, o, name, v)) return;

    if (o->kind == OBJ_ARRAY) {
        int index;
        if (strcmp(name, "length") == 0) {
            int want = (int)js_to_number(J, v);
            if (want >= 0 && want <= o->len) o->len = want;
            return;
        }
        if (index_of_name(name, &index)) {
            while (o->len <= index) js_array_push(J, o, js_undefined());
            if (index < o->len) o->items[index] = v;
            return;
        }
    }

    js_def(J, o, name, v);
}

/* ---- errors ---- */

void js_error(struct js *J, const char *fmt, ...) {
    if (J->signal == SIG_ERROR) return;   /* keep the first one */
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(J->error, sizeof(J->error), fmt, ap);
    va_end(ap);
    J->signal = SIG_ERROR;
}

void js_throw(struct js *J, const char *message) {
    if (J->signal != SIG_NONE) return;
    J->signal = SIG_THROW;
    J->signal_value = js_string(J, message);
    snprintf(J->error, sizeof(J->error), "%s", message);
}

const char *js_last_error(const struct js *J) {
    return J->error[0] != '\0' ? J->error : "no error";
}

/* ================= the lexer ================= */

enum {
    T_EOF = 0, T_NUM, T_STR, T_IDENT, T_KEYWORD, T_PUNCT, T_REGEX
};

struct token {
    uint8_t type;
    uint8_t nl_before;      /* a line break came before this token */
    char *text;             /* identifier, punctuator, string value */
    char *flags;            /* T_REGEX only */
    double num;
    int line;
};

struct lexer {
    struct js *J;
    const char *src;
    size_t len, at;
    int line;
    int nl;

    struct token *tokens;
    int ntokens, cap;
};

static const char *KEYWORDS[] = {
    "var", "let", "const", "function", "return", "if", "else", "for", "while",
    "do", "break", "continue", "new", "typeof", "instanceof", "in", "of",
    "this", "null", "true", "false", "undefined", "delete", "void", "switch",
    "case", "default", "throw", "try", "catch", "finally", NULL
};

/* Longest first: the scanner takes the first that matches. */
static const char *PUNCT[] = {
    ">>>=", "===", "!==", "...", "<<=", ">>=", ">>>", "&&=", "||=", "?\?=",
    "**=", "==", "!=", "<=", ">=", "&&", "||", "??", "++", "--", "+=", "-=",
    "*=", "/=", "%=", "&=", "|=", "^=", "=>", "**", "<<", ">>",
    "{", "}", "(", ")", "[", "]", ";", ",", "<", ">", "+", "-", "*", "/", "%",
    "&", "|", "^", "!", "~", "?", ":", "=", ".", NULL
};

/* Parser-owned text: freed with the interpreter, not with the tree. */
static char *keep_text(struct js *J, const char *text, size_t len) {
    char *p = malloc(len + 1);
    if (p == NULL) {
        js_error(J, "out of memory");
        return NULL;
    }
    memcpy(p, text, len);
    p[len] = '\0';

    if (J->ntexts == J->texts_cap) {
        int want = J->texts_cap ? J->texts_cap * 2 : 128;
        char **grown = realloc(J->texts, (size_t)want * sizeof(*grown));
        if (grown == NULL) {
            free(p);
            js_error(J, "out of memory");
            return NULL;
        }
        J->texts = grown;
        J->texts_cap = want;
    }
    J->texts[J->ntexts++] = p;
    return p;
}

static struct token *push_token(struct lexer *L, int type) {
    if (L->ntokens == L->cap) {
        int want = L->cap ? L->cap * 2 : 256;
        struct token *p = realloc(L->tokens, (size_t)want * sizeof(*p));
        if (p == NULL) {
            js_error(L->J, "out of memory");
            return NULL;
        }
        L->tokens = p;
        L->cap = want;
    }
    struct token *t = &L->tokens[L->ntokens++];
    memset(t, 0, sizeof(*t));
    t->type = (uint8_t)type;
    t->line = L->line;
    t->nl_before = (uint8_t)L->nl;
    L->nl = 0;
    return t;
}

static void push_punct(struct lexer *L, const char *text) {
    struct token *t = push_token(L, T_PUNCT);
    if (t != NULL) t->text = keep_text(L->J, text, strlen(text));
}

static int is_ident_start(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
           c == '$' || (unsigned char)c >= 0x80;
}

static int is_ident_char(char c) {
    return is_ident_start(c) || (c >= '0' && c <= '9');
}

/* Whether a '/' here starts a regular expression or is division: the
 * answer is what the previous token was, which is why the lexer keeps
 * the token list rather than one character of history. */
static int regex_allowed(struct lexer *L) {
    if (L->ntokens == 0) return 1;
    struct token *p = &L->tokens[L->ntokens - 1];
    if (p->type == T_NUM || p->type == T_STR || p->type == T_REGEX) return 0;
    if (p->type == T_IDENT) return 0;
    if (p->type == T_KEYWORD) {
        return !(strcmp(p->text, "this") == 0 || strcmp(p->text, "null") == 0 ||
                 strcmp(p->text, "true") == 0 || strcmp(p->text, "false") == 0 ||
                 strcmp(p->text, "undefined") == 0);
    }
    /* After a closing bracket a slash is division; after anything else
     * an expression is starting. */
    return !(strcmp(p->text, ")") == 0 || strcmp(p->text, "]") == 0 ||
             strcmp(p->text, "}") == 0 || strcmp(p->text, "++") == 0 ||
             strcmp(p->text, "--") == 0);
}

static void lex_string(struct lexer *L, char quote) {
    char *buf = malloc(L->len - L->at + 1);
    if (buf == NULL) {
        js_error(L->J, "out of memory");
        return;
    }
    size_t n = 0;

    while (L->at < L->len && L->src[L->at] != quote) {
        char c = L->src[L->at++];
        if (c == '\\' && L->at < L->len) {
            char e = L->src[L->at++];
            switch (e) {
            case 'n': buf[n++] = '\n'; break;
            case 't': buf[n++] = '\t'; break;
            case 'r': buf[n++] = '\r'; break;
            case 'b': buf[n++] = '\b'; break;
            case 'f': buf[n++] = '\f'; break;
            case '0': buf[n++] = '\0'; break;
            case 'x': {
                int v = 0;
                for (int i = 0; i < 2 && L->at < L->len; i++) {
                    char h = L->src[L->at++];
                    v = v * 16 + (h <= '9' ? h - '0' : (h | 32) - 'a' + 10);
                }
                buf[n++] = (char)v;
                break;
            }
            case 'u': {
                /* \uXXXX, written out as UTF-8: the rest of the
                 * browser is UTF-8 all the way to the glyph. */
                unsigned v = 0;
                int digits = 0;
                if (L->at < L->len && L->src[L->at] == '{') {
                    L->at++;
                    while (L->at < L->len && L->src[L->at] != '}') {
                        char h = L->src[L->at++];
                        v = v * 16 + (unsigned)(h <= '9' ? h - '0'
                                                         : (h | 32) - 'a' + 10);
                        digits++;
                    }
                    if (L->at < L->len) L->at++;
                } else {
                    for (; digits < 4 && L->at < L->len; digits++) {
                        char h = L->src[L->at++];
                        v = v * 16 + (unsigned)(h <= '9' ? h - '0'
                                                         : (h | 32) - 'a' + 10);
                    }
                }
                if (v < 0x80) {
                    buf[n++] = (char)v;
                } else if (v < 0x800) {
                    buf[n++] = (char)(0xC0 | (v >> 6));
                    buf[n++] = (char)(0x80 | (v & 0x3F));
                } else {
                    buf[n++] = (char)(0xE0 | (v >> 12));
                    buf[n++] = (char)(0x80 | ((v >> 6) & 0x3F));
                    buf[n++] = (char)(0x80 | (v & 0x3F));
                }
                break;
            }
            case '\n': L->line++; break;   /* a continued line */
            default: buf[n++] = e; break;
            }
            continue;
        }
        if (c == '\n') L->line++;
        buf[n++] = c;
    }
    if (L->at < L->len) L->at++;   /* the closing quote */

    struct token *t = push_token(L, T_STR);
    if (t != NULL) t->text = keep_text(L->J, buf, n);
    free(buf);
}

static void lex_template(struct lexer *L);

static void lex_all(struct lexer *L) {
    while (L->at < L->len && L->J->signal == SIG_NONE) {
        char c = L->src[L->at];

        if (c == '\n') {
            L->line++;
            L->nl = 1;
            L->at++;
            continue;
        }
        if (c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v') {
            L->at++;
            continue;
        }
        if (c == '/' && L->at + 1 < L->len && L->src[L->at + 1] == '/') {
            while (L->at < L->len && L->src[L->at] != '\n') L->at++;
            continue;
        }
        if (c == '/' && L->at + 1 < L->len && L->src[L->at + 1] == '*') {
            L->at += 2;
            while (L->at + 1 < L->len &&
                   !(L->src[L->at] == '*' && L->src[L->at + 1] == '/')) {
                if (L->src[L->at] == '\n') {
                    L->line++;
                    L->nl = 1;
                }
                L->at++;
            }
            L->at += 2;
            continue;
        }

        /* An HTML comment opener inside a <script> is a comment in
         * JavaScript too - pages from the 1990s still say <!--. */
        if (c == '<' && L->at + 3 < L->len &&
            strncmp(L->src + L->at, "<!--", 4) == 0) {
            while (L->at < L->len && L->src[L->at] != '\n') L->at++;
            continue;
        }

        if (c == '"' || c == '\'') {
            L->at++;
            lex_string(L, c);
            continue;
        }
        if (c == '`') {
            lex_template(L);
            continue;
        }

        if ((c >= '0' && c <= '9') ||
            (c == '.' && L->at + 1 < L->len && L->src[L->at + 1] >= '0' &&
             L->src[L->at + 1] <= '9')) {
            const char *start = L->src + L->at;
            char *end = NULL;
            double v = strtod(start, &end);
            if (start[0] == '0' && (start[1] == 'x' || start[1] == 'X')) {
                v = (double)strtoull(start, &end, 16);
            }
            L->at += (size_t)(end - start);
            struct token *t = push_token(L, T_NUM);
            if (t != NULL) t->num = v;
            continue;
        }

        if (is_ident_start(c)) {
            size_t start = L->at;
            while (L->at < L->len && is_ident_char(L->src[L->at])) L->at++;
            size_t n = L->at - start;

            int keyword = 0;
            for (int i = 0; KEYWORDS[i] != NULL; i++) {
                if (strlen(KEYWORDS[i]) == n &&
                    strncmp(KEYWORDS[i], L->src + start, n) == 0) {
                    keyword = 1;
                    break;
                }
            }
            struct token *t = push_token(L, keyword ? T_KEYWORD : T_IDENT);
            if (t != NULL) t->text = keep_text(L->J, L->src + start, n);
            continue;
        }

        if (c == '/' && regex_allowed(L)) {
            size_t start = ++L->at;
            int in_class = 0;
            while (L->at < L->len) {
                char r = L->src[L->at];
                if (r == '\\') {
                    L->at += 2;
                    continue;
                }
                if (r == '[') in_class = 1;
                else if (r == ']') in_class = 0;
                else if (r == '/' && !in_class) break;
                else if (r == '\n') break;
                L->at++;
            }
            size_t n = L->at - start;
            if (L->at < L->len) L->at++;   /* the closing slash */

            size_t fstart = L->at;
            while (L->at < L->len && is_ident_char(L->src[L->at])) L->at++;

            struct token *t = push_token(L, T_REGEX);
            if (t != NULL) {
                t->text = keep_text(L->J, L->src + start, n);
                t->flags = keep_text(L->J, L->src + fstart, L->at - fstart);
            }
            continue;
        }

        int matched = 0;
        for (int i = 0; PUNCT[i] != NULL; i++) {
            size_t n = strlen(PUNCT[i]);
            if (L->at + n <= L->len &&
                strncmp(L->src + L->at, PUNCT[i], n) == 0) {
                push_punct(L, PUNCT[i]);
                L->at += n;
                matched = 1;
                break;
            }
        }
        if (!matched) L->at++;   /* something we do not know: skip it */
    }

    push_token(L, T_EOF);
}

/*
 * `a ${b} c` becomes ("a" + (b) + " c"): the lexer writes the
 * concatenation out in tokens, and the parser never learns that
 * template literals exist.
 */
static void lex_template(struct lexer *L) {
    L->at++;   /* the opening backtick */
    push_punct(L, "(");

    char *buf = malloc(L->len - L->at + 1);
    if (buf == NULL) {
        js_error(L->J, "out of memory");
        return;
    }
    size_t n = 0;

    for (;;) {
        if (L->at >= L->len) break;
        char c = L->src[L->at];

        if (c == '`') {
            L->at++;
            break;
        }
        if (c == '\\' && L->at + 1 < L->len) {
            char e = L->src[L->at + 1];
            L->at += 2;
            buf[n++] = e == 'n' ? '\n' : e == 't' ? '\t' : e;
            continue;
        }
        if (c == '$' && L->at + 1 < L->len && L->src[L->at + 1] == '{') {
            struct token *t = push_token(L, T_STR);
            if (t != NULL) t->text = keep_text(L->J, buf, n);
            n = 0;
            push_punct(L, "+");
            push_punct(L, "(");

            L->at += 2;
            /* Lex the expression by hand until the brace that closes
             * it, so nested braces and strings inside it survive. */
            int depth = 1;
            size_t start = L->at;
            while (L->at < L->len && depth > 0) {
                char e = L->src[L->at];
                if (e == '{') depth++;
                else if (e == '}') depth--;
                else if (e == '"' || e == '\'') {
                    char q = e;
                    L->at++;
                    while (L->at < L->len && L->src[L->at] != q) {
                        if (L->src[L->at] == '\\') L->at++;
                        L->at++;
                    }
                }
                if (depth > 0) L->at++;
            }
            struct lexer inner;
            memset(&inner, 0, sizeof(inner));
            inner.J = L->J;
            inner.src = L->src + start;
            inner.len = L->at - start;
            inner.line = L->line;
            lex_all(&inner);
            for (int i = 0; i < inner.ntokens; i++) {
                if (inner.tokens[i].type == T_EOF) break;
                struct token *t2 = push_token(L, inner.tokens[i].type);
                if (t2 != NULL) {
                    struct token saved = inner.tokens[i];
                    *t2 = saved;
                }
            }
            free(inner.tokens);

            if (L->at < L->len) L->at++;   /* the closing brace */
            push_punct(L, ")");
            push_punct(L, "+");
            continue;
        }
        if (c == '\n') L->line++;
        buf[n++] = c;
        L->at++;
    }

    struct token *t = push_token(L, T_STR);
    if (t != NULL) t->text = keep_text(L->J, buf, n);
    free(buf);
    push_punct(L, ")");
}

/* ================= the parser ================= */

enum {
    N_NUM = 1, N_STR, N_REGEX, N_IDENT, N_BOOL, N_NULL, N_UNDEF, N_THIS,
    N_ARRAY, N_OBJECT, N_FUNC, N_CALL, N_NEW, N_MEMBER, N_INDEX,
    N_BINARY, N_LOGICAL, N_UNARY, N_UPDATE, N_ASSIGN, N_COND, N_SEQ,
    N_VAR, N_BLOCK, N_IF, N_WHILE, N_DO, N_FOR, N_FORIN, N_RETURN,
    N_BREAK, N_CONTINUE, N_EXPR, N_EMPTY, N_TRY, N_THROW, N_SWITCH,
    N_PROP, N_CASE
};

struct ast {
    uint8_t type;
    uint8_t prefix;          /* N_UPDATE: ++x rather than x++ */
    const char *op;
    char *text;
    double num;
    struct ast *a, *b, *c, *d;
    struct ast *next;        /* statement and argument lists */
    struct ast *alloc_next;
    int line;
};

struct parser {
    struct js *J;
    struct token *tok;
    int n;
    int at;
};

static struct ast *parse_assign(struct parser *P);
static struct ast *parse_expression(struct parser *P);
static struct ast *parse_statement(struct parser *P);
static struct ast *parse_block(struct parser *P);

static struct ast *node_new(struct parser *P, int type) {
    struct ast *a = calloc(1, sizeof(*a));
    if (a == NULL) {
        js_error(P->J, "out of memory");
        return NULL;
    }
    a->type = (uint8_t)type;
    a->line = P->at < P->n ? P->tok[P->at].line : 0;
    a->alloc_next = P->J->nodes_head;
    P->J->nodes_head = a;
    return a;
}

static struct token *peek(struct parser *P, int ahead) {
    int i = P->at + ahead;
    if (i >= P->n) i = P->n - 1;
    return &P->tok[i];
}

static int is_punct(struct token *t, const char *text) {
    return t->type == T_PUNCT && strcmp(t->text, text) == 0;
}

static int is_keyword(struct token *t, const char *text) {
    return t->type == T_KEYWORD && strcmp(t->text, text) == 0;
}

static int eat_punct(struct parser *P, const char *text) {
    if (is_punct(peek(P, 0), text)) {
        P->at++;
        return 1;
    }
    return 0;
}

static int eat_keyword(struct parser *P, const char *text) {
    if (is_keyword(peek(P, 0), text)) {
        P->at++;
        return 1;
    }
    return 0;
}

static void expect(struct parser *P, const char *text) {
    if (eat_punct(P, text)) return;
    struct token *t = peek(P, 0);
    js_error(P->J, "line %d: expected '%s' but found '%s'", t->line, text,
             t->type == T_EOF ? "the end of the script"
                              : (t->text ? t->text : "a number"));
    P->at = P->n - 1;   /* stop rather than spin */
}

/* A statement may end at a semicolon, a closing brace, the end of the
 * script - or at a line break, which is what automatic semicolon
 * insertion amounts to in the code pages actually contain. */
static void end_statement(struct parser *P) {
    if (eat_punct(P, ";")) return;
}

/* ---- functions ---- */

static struct ast *parse_params(struct parser *P) {
    struct ast *first = NULL, **tail = &first;
    expect(P, "(");
    while (!is_punct(peek(P, 0), ")") && peek(P, 0)->type != T_EOF) {
        struct token *t = peek(P, 0);
        if (t->type == T_IDENT || t->type == T_KEYWORD) {
            struct ast *p = node_new(P, N_IDENT);
            if (p == NULL) return first;
            p->text = t->text;
            P->at++;
            /* `function f(a = 1)` - default parameter values. `p->a`
             * is otherwise unused on an N_IDENT parameter node; the
             * default is only evaluated (in js_call, once per call)
             * when the caller passes fewer arguments or explicitly
             * passes `undefined`, matching real JS semantics. */
            if (eat_punct(P, "=")) {
                p->a = parse_assign(P);
            }
            *tail = p;
            tail = &p->next;
        } else {
            P->at++;
        }
        if (!eat_punct(P, ",")) break;
    }
    expect(P, ")");
    return first;
}

static struct ast *parse_function(struct parser *P, int is_declaration) {
    struct ast *fn = node_new(P, N_FUNC);
    if (fn == NULL) return NULL;

    struct token *name = peek(P, 0);
    if (name->type == T_IDENT) {
        fn->text = name->text;
        P->at++;
    }
    fn->a = parse_params(P);
    fn->b = parse_block(P);
    fn->num = is_declaration;
    return fn;
}

/* An arrow function: the body may be an expression, which is a
 * return in disguise. */
static struct ast *parse_arrow_body(struct parser *P, struct ast *params) {
    struct ast *fn = node_new(P, N_FUNC);
    if (fn == NULL) return NULL;
    fn->prefix = 1;   /* an arrow: `this` comes from where it was written */
    fn->a = params;

    if (is_punct(peek(P, 0), "{")) {
        fn->b = parse_block(P);
    } else {
        struct ast *ret = node_new(P, N_RETURN);
        if (ret == NULL) return fn;
        ret->a = parse_assign(P);
        struct ast *block = node_new(P, N_BLOCK);
        if (block == NULL) return fn;
        block->a = ret;
        fn->b = block;
    }
    return fn;
}

/* Is the parenthesis at `at` the parameter list of an arrow? Only a
 * scan to the matching bracket can say. */
static int looks_like_arrow(struct parser *P) {
    int depth = 0;
    for (int i = P->at; i < P->n; i++) {
        struct token *t = &P->tok[i];
        if (t->type == T_PUNCT) {
            if (strcmp(t->text, "(") == 0) depth++;
            else if (strcmp(t->text, ")") == 0) {
                depth--;
                if (depth == 0) {
                    struct token *next = &P->tok[i + 1 < P->n ? i + 1 : i];
                    return is_punct(next, "=>");
                }
            }
        } else if (t->type == T_EOF) {
            break;
        }
    }
    return 0;
}

/* ---- expressions ---- */

static struct ast *parse_primary(struct parser *P) {
    struct token *t = peek(P, 0);

    if (t->type == T_NUM) {
        P->at++;
        struct ast *a = node_new(P, N_NUM);
        if (a != NULL) a->num = t->num;
        return a;
    }
    if (t->type == T_STR) {
        P->at++;
        struct ast *a = node_new(P, N_STR);
        if (a != NULL) a->text = t->text;
        return a;
    }
    if (t->type == T_REGEX) {
        P->at++;
        struct ast *a = node_new(P, N_REGEX);
        if (a != NULL) {
            a->text = t->text;
            a->op = t->flags;
        }
        return a;
    }
    if (t->type == T_IDENT) {
        /* x => ... */
        if (is_punct(peek(P, 1), "=>")) {
            struct ast *param = node_new(P, N_IDENT);
            if (param == NULL) return NULL;
            param->text = t->text;
            P->at += 2;
            return parse_arrow_body(P, param);
        }
        P->at++;
        struct ast *a = node_new(P, N_IDENT);
        if (a != NULL) a->text = t->text;
        return a;
    }

    if (t->type == T_KEYWORD) {
        if (eat_keyword(P, "true") || eat_keyword(P, "false")) {
            struct ast *a = node_new(P, N_BOOL);
            if (a != NULL) a->num = t->text[0] == 't' ? 1 : 0;
            return a;
        }
        if (eat_keyword(P, "null")) return node_new(P, N_NULL);
        if (eat_keyword(P, "undefined")) return node_new(P, N_UNDEF);
        if (eat_keyword(P, "this")) return node_new(P, N_THIS);
        if (eat_keyword(P, "function")) return parse_function(P, 0);
        if (eat_keyword(P, "new")) {
            struct ast *a = node_new(P, N_NEW);
            if (a == NULL) return NULL;
            a->a = parse_primary(P);
            /* member accesses belong to the constructor name */
            for (;;) {
                if (eat_punct(P, ".")) {
                    struct ast *m = node_new(P, N_MEMBER);
                    if (m == NULL) break;
                    m->a = a->a;
                    m->text = peek(P, 0)->text;
                    P->at++;
                    a->a = m;
                    continue;
                }
                break;
            }
            if (is_punct(peek(P, 0), "(")) {
                P->at++;
                struct ast **tail = &a->b;
                while (!is_punct(peek(P, 0), ")") &&
                       peek(P, 0)->type != T_EOF) {
                    struct ast *arg = parse_assign(P);
                    *tail = arg;
                    if (arg == NULL) break;
                    tail = &arg->next;
                    if (!eat_punct(P, ",")) break;
                }
                expect(P, ")");
            }
            return a;
        }
        /* A keyword used as a name (obj.default, obj.new): let it be
         * an identifier, which is what the property is. */
        P->at++;
        struct ast *a = node_new(P, N_IDENT);
        if (a != NULL) a->text = t->text;
        return a;
    }

    if (is_punct(t, "(")) {
        if (looks_like_arrow(P)) {
            struct ast *params = parse_params(P);
            expect(P, "=>");
            return parse_arrow_body(P, params);
        }
        P->at++;
        struct ast *inner = parse_expression(P);
        expect(P, ")");
        return inner;
    }

    if (is_punct(t, "[")) {
        P->at++;
        struct ast *a = node_new(P, N_ARRAY);
        if (a == NULL) return NULL;
        struct ast **tail = &a->a;
        while (!is_punct(peek(P, 0), "]") && peek(P, 0)->type != T_EOF) {
            struct ast *item = parse_assign(P);
            *tail = item;
            if (item == NULL) break;
            tail = &item->next;
            if (!eat_punct(P, ",")) break;
        }
        expect(P, "]");
        return a;
    }

    if (is_punct(t, "{")) {
        P->at++;
        struct ast *a = node_new(P, N_OBJECT);
        if (a == NULL) return NULL;
        struct ast **tail = &a->a;
        while (!is_punct(peek(P, 0), "}") && peek(P, 0)->type != T_EOF) {
            struct token *key = peek(P, 0);
            struct ast *prop = node_new(P, N_PROP);
            if (prop == NULL) break;
            prop->text = key->text;
            if (key->type == T_NUM) {
                char buf[32];
                snprintf(buf, sizeof(buf), "%g", key->num);
                prop->text = keep_text(P->J, buf, strlen(buf));
            }
            P->at++;

            if (eat_punct(P, ":")) {
                prop->a = parse_assign(P);
            } else if (is_punct(peek(P, 0), "(")) {
                struct ast *fn = node_new(P, N_FUNC);
                if (fn != NULL) {
                    fn->a = parse_params(P);
                    fn->b = parse_block(P);
                }
                prop->a = fn;
            } else {
                /* { x } is { x: x } */
                struct ast *ref = node_new(P, N_IDENT);
                if (ref != NULL) ref->text = prop->text;
                prop->a = ref;
            }
            *tail = prop;
            tail = &prop->next;
            if (!eat_punct(P, ",")) break;
        }
        expect(P, "}");
        return a;
    }

    /* Nothing recognisable: consume it so the parse cannot loop. */
    if (t->type != T_EOF) P->at++;
    return node_new(P, N_UNDEF);
}

static struct ast *parse_call_member(struct parser *P) {
    struct ast *left = parse_primary(P);

    for (;;) {
        if (eat_punct(P, ".")) {
            struct token *name = peek(P, 0);
            struct ast *m = node_new(P, N_MEMBER);
            if (m == NULL) return left;
            m->a = left;
            m->text = name->text;
            P->at++;
            left = m;
            continue;
        }
        if (eat_punct(P, "?.")) {   /* not in PUNCT; kept for clarity */
            continue;
        }
        if (is_punct(peek(P, 0), "[")) {
            P->at++;
            struct ast *m = node_new(P, N_INDEX);
            if (m == NULL) return left;
            m->a = left;
            m->b = parse_expression(P);
            expect(P, "]");
            left = m;
            continue;
        }
        if (is_punct(peek(P, 0), "(")) {
            P->at++;
            struct ast *call = node_new(P, N_CALL);
            if (call == NULL) return left;
            call->a = left;
            struct ast **tail = &call->b;
            while (!is_punct(peek(P, 0), ")") && peek(P, 0)->type != T_EOF) {
                struct ast *arg = parse_assign(P);
                *tail = arg;
                if (arg == NULL) break;
                tail = &arg->next;
                if (!eat_punct(P, ",")) break;
            }
            expect(P, ")");
            left = call;
            continue;
        }
        break;
    }
    return left;
}

static struct ast *parse_unary(struct parser *P) {
    struct token *t = peek(P, 0);

    if (is_punct(t, "!") || is_punct(t, "-") || is_punct(t, "+") ||
        is_punct(t, "~") || is_keyword(t, "typeof") ||
        is_keyword(t, "void") || is_keyword(t, "delete")) {
        P->at++;
        struct ast *a = node_new(P, N_UNARY);
        if (a == NULL) return NULL;
        a->op = t->text;
        a->a = parse_unary(P);
        return a;
    }
    if (is_punct(t, "++") || is_punct(t, "--")) {
        P->at++;
        struct ast *a = node_new(P, N_UPDATE);
        if (a == NULL) return NULL;
        a->op = t->text;
        a->prefix = 1;
        a->a = parse_unary(P);
        return a;
    }

    struct ast *left = parse_call_member(P);
    struct token *next = peek(P, 0);
    if ((is_punct(next, "++") || is_punct(next, "--")) && !next->nl_before) {
        P->at++;
        struct ast *a = node_new(P, N_UPDATE);
        if (a == NULL) return left;
        a->op = next->text;
        a->a = left;
        return a;
    }
    return left;
}

/* Binding power, highest binds tightest. 0 means "not a binary
 * operator". */
static int binary_power(struct token *t) {
    if (t->type == T_KEYWORD) {
        if (strcmp(t->text, "instanceof") == 0) return 7;
        if (strcmp(t->text, "in") == 0) return 7;
        return 0;
    }
    if (t->type != T_PUNCT) return 0;
    const char *o = t->text;

    if (strcmp(o, "||") == 0 || strcmp(o, "??") == 0) return 1;
    if (strcmp(o, "&&") == 0) return 2;
    if (strcmp(o, "|") == 0) return 3;
    if (strcmp(o, "^") == 0) return 4;
    if (strcmp(o, "&") == 0) return 5;
    if (strcmp(o, "==") == 0 || strcmp(o, "!=") == 0 ||
        strcmp(o, "===") == 0 || strcmp(o, "!==") == 0) {
        return 6;
    }
    if (strcmp(o, "<") == 0 || strcmp(o, ">") == 0 || strcmp(o, "<=") == 0 ||
        strcmp(o, ">=") == 0) {
        return 7;
    }
    if (strcmp(o, "<<") == 0 || strcmp(o, ">>") == 0 ||
        strcmp(o, ">>>") == 0) {
        return 8;
    }
    if (strcmp(o, "+") == 0 || strcmp(o, "-") == 0) return 9;
    if (strcmp(o, "*") == 0 || strcmp(o, "/") == 0 || strcmp(o, "%") == 0) {
        return 10;
    }
    if (strcmp(o, "**") == 0) return 11;
    return 0;
}

static struct ast *parse_binary(struct parser *P, int min_power) {
    struct ast *left = parse_unary(P);

    for (;;) {
        struct token *t = peek(P, 0);
        int power = binary_power(t);
        if (power == 0 || power < min_power) break;
        P->at++;

        struct ast *right = parse_binary(P, power + 1);
        int logical = strcmp(t->text, "&&") == 0 || strcmp(t->text, "||") == 0 ||
                      strcmp(t->text, "??") == 0;
        struct ast *a = node_new(P, logical ? N_LOGICAL : N_BINARY);
        if (a == NULL) return left;
        a->op = t->text;
        a->a = left;
        a->b = right;
        left = a;
    }
    return left;
}

static struct ast *parse_conditional(struct parser *P) {
    struct ast *cond = parse_binary(P, 1);
    if (!is_punct(peek(P, 0), "?")) return cond;
    P->at++;

    struct ast *a = node_new(P, N_COND);
    if (a == NULL) return cond;
    a->a = cond;
    a->b = parse_assign(P);
    expect(P, ":");
    a->c = parse_assign(P);
    return a;
}

static int is_assign_op(struct token *t) {
    if (t->type != T_PUNCT) return 0;
    static const char *ops[] = { "=", "+=", "-=", "*=", "/=", "%=", "&=",
                                 "|=", "^=", "<<=", ">>=", ">>>=", "&&=",
                                 "||=", "?\?=", "**=", NULL };
    for (int i = 0; ops[i] != NULL; i++) {
        if (strcmp(t->text, ops[i]) == 0) return 1;
    }
    return 0;
}

static struct ast *parse_assign(struct parser *P) {
    struct ast *left = parse_conditional(P);
    struct token *t = peek(P, 0);
    if (!is_assign_op(t)) return left;
    P->at++;

    struct ast *a = node_new(P, N_ASSIGN);
    if (a == NULL) return left;
    a->op = t->text;
    a->a = left;
    a->b = parse_assign(P);
    return a;
}

static struct ast *parse_expression(struct parser *P) {
    struct ast *left = parse_assign(P);
    while (is_punct(peek(P, 0), ",")) {
        P->at++;
        struct ast *a = node_new(P, N_SEQ);
        if (a == NULL) return left;
        a->a = left;
        a->b = parse_assign(P);
        left = a;
    }
    return left;
}

/* ---- statements ---- */

static struct ast *parse_var(struct parser *P) {
    struct ast *first = NULL, **tail = &first;

    for (;;) {
        struct token *name = peek(P, 0);
        if (name->type != T_IDENT) break;
        P->at++;

        struct ast *d = node_new(P, N_VAR);
        if (d == NULL) break;
        d->text = name->text;
        if (eat_punct(P, "=")) d->a = parse_assign(P);
        *tail = d;
        tail = &d->next;

        if (!eat_punct(P, ",")) break;
    }
    return first;
}

/* A block that declares nothing needs no scope of its own, and a
 * loop body runs thousands of times: giving each iteration a scope
 * nobody uses is how an empty loop runs out of memory. */
static int block_declares(struct ast *list) {
    for (struct ast *st = list; st != NULL; st = st->next) {
        if (st->type == N_VAR) return 1;
        if (st->type == N_FUNC && st->num != 0) return 1;
    }
    return 0;
}

static struct ast *parse_block(struct parser *P) {
    struct ast *block = node_new(P, N_BLOCK);
    if (block == NULL) return NULL;
    expect(P, "{");

    struct ast **tail = &block->a;
    while (!is_punct(peek(P, 0), "}") && peek(P, 0)->type != T_EOF) {
        struct ast *st = parse_statement(P);
        if (st == NULL) break;
        *tail = st;
        while (*tail != NULL && (*tail)->next != NULL) tail = &(*tail)->next;
        tail = &(*tail)->next;
    }
    expect(P, "}");
    block->num = block_declares(block->a);
    return block;
}

static struct ast *parse_statement(struct parser *P) {
    struct token *t = peek(P, 0);

    if (t->type == T_EOF) return NULL;

    if (is_punct(t, "{")) return parse_block(P);
    if (eat_punct(P, ";")) return node_new(P, N_EMPTY);

    if (is_keyword(t, "var") || is_keyword(t, "let") || is_keyword(t, "const")) {
        P->at++;
        struct ast *decls = parse_var(P);
        end_statement(P);
        return decls;
    }

    if (eat_keyword(P, "function")) return parse_function(P, 1);

    if (eat_keyword(P, "if")) {
        struct ast *a = node_new(P, N_IF);
        if (a == NULL) return NULL;
        expect(P, "(");
        a->a = parse_expression(P);
        expect(P, ")");
        a->b = parse_statement(P);
        if (eat_keyword(P, "else")) a->c = parse_statement(P);
        return a;
    }

    if (eat_keyword(P, "while")) {
        struct ast *a = node_new(P, N_WHILE);
        if (a == NULL) return NULL;
        expect(P, "(");
        a->a = parse_expression(P);
        expect(P, ")");
        a->b = parse_statement(P);
        return a;
    }

    if (eat_keyword(P, "do")) {
        struct ast *a = node_new(P, N_DO);
        if (a == NULL) return NULL;
        a->b = parse_statement(P);
        if (!eat_keyword(P, "while")) return a;
        expect(P, "(");
        a->a = parse_expression(P);
        expect(P, ")");
        end_statement(P);
        return a;
    }

    if (eat_keyword(P, "for")) {
        expect(P, "(");

        /* for (x in obj) and for (x of list) share everything but the
         * name of the thing they walk. */
        int save = P->at;
        int declared = 0;
        if (is_keyword(peek(P, 0), "var") || is_keyword(peek(P, 0), "let") ||
            is_keyword(peek(P, 0), "const")) {
            P->at++;
            declared = 1;
        }
        if (peek(P, 0)->type == T_IDENT &&
            (is_keyword(peek(P, 1), "in") || is_keyword(peek(P, 1), "of"))) {
            struct ast *a = node_new(P, N_FORIN);
            if (a == NULL) return NULL;
            a->text = peek(P, 0)->text;
            a->num = is_keyword(peek(P, 1), "of") ? 1 : 0;
            a->prefix = (uint8_t)declared;
            P->at += 2;
            a->a = parse_expression(P);
            expect(P, ")");
            a->b = parse_statement(P);
            return a;
        }
        P->at = save;

        struct ast *a = node_new(P, N_FOR);
        if (a == NULL) return NULL;
        if (!is_punct(peek(P, 0), ";")) {
            if (is_keyword(peek(P, 0), "var") || is_keyword(peek(P, 0), "let") ||
                is_keyword(peek(P, 0), "const")) {
                P->at++;
                a->a = parse_var(P);
            } else {
                struct ast *e = node_new(P, N_EXPR);
                if (e != NULL) e->a = parse_expression(P);
                a->a = e;
            }
        }
        expect(P, ";");
        if (!is_punct(peek(P, 0), ";")) a->b = parse_expression(P);
        expect(P, ";");
        if (!is_punct(peek(P, 0), ")")) a->c = parse_expression(P);
        expect(P, ")");
        a->d = parse_statement(P);
        return a;
    }

    if (eat_keyword(P, "return")) {
        struct ast *a = node_new(P, N_RETURN);
        if (a == NULL) return NULL;
        struct token *next = peek(P, 0);
        if (!is_punct(next, ";") && !is_punct(next, "}") &&
            next->type != T_EOF && !next->nl_before) {
            a->a = parse_expression(P);
        }
        end_statement(P);
        return a;
    }

    if (eat_keyword(P, "break")) {
        end_statement(P);
        return node_new(P, N_BREAK);
    }
    if (eat_keyword(P, "continue")) {
        end_statement(P);
        return node_new(P, N_CONTINUE);
    }

    if (eat_keyword(P, "throw")) {
        struct ast *a = node_new(P, N_THROW);
        if (a == NULL) return NULL;
        a->a = parse_expression(P);
        end_statement(P);
        return a;
    }

    if (eat_keyword(P, "try")) {
        struct ast *a = node_new(P, N_TRY);
        if (a == NULL) return NULL;
        a->a = parse_block(P);
        if (eat_keyword(P, "catch")) {
            if (eat_punct(P, "(")) {
                a->text = peek(P, 0)->text;
                P->at++;
                expect(P, ")");
            }
            a->b = parse_block(P);
        }
        if (eat_keyword(P, "finally")) a->c = parse_block(P);
        return a;
    }

    if (eat_keyword(P, "switch")) {
        struct ast *a = node_new(P, N_SWITCH);
        if (a == NULL) return NULL;
        expect(P, "(");
        a->a = parse_expression(P);
        expect(P, ")");
        expect(P, "{");

        struct ast **tail = &a->b;
        while (!is_punct(peek(P, 0), "}") && peek(P, 0)->type != T_EOF) {
            struct ast *c = node_new(P, N_CASE);
            if (c == NULL) break;
            if (eat_keyword(P, "case")) {
                c->a = parse_expression(P);
            } else if (!eat_keyword(P, "default")) {
                P->at++;
                continue;
            }
            expect(P, ":");

            struct ast **body = &c->b;
            while (!is_punct(peek(P, 0), "}") &&
                   !is_keyword(peek(P, 0), "case") &&
                   !is_keyword(peek(P, 0), "default") &&
                   peek(P, 0)->type != T_EOF) {
                struct ast *st = parse_statement(P);
                if (st == NULL) break;
                *body = st;
                while (*body != NULL && (*body)->next != NULL) {
                    body = &(*body)->next;
                }
                body = &(*body)->next;
            }
            *tail = c;
            tail = &c->next;
        }
        expect(P, "}");
        return a;
    }

    struct ast *a = node_new(P, N_EXPR);
    if (a == NULL) return NULL;
    a->a = parse_expression(P);
    end_statement(P);
    return a;
}

static struct ast *parse_program(struct js *J, struct token *tok, int n) {
    struct parser P;
    P.J = J;
    P.tok = tok;
    P.n = n;
    P.at = 0;

    struct ast *first = NULL, **tail = &first;
    while (P.at < P.n && P.tok[P.at].type != T_EOF && J->signal == SIG_NONE) {
        struct ast *st = parse_statement(&P);
        if (st == NULL) break;
        *tail = st;
        while (*tail != NULL && (*tail)->next != NULL) tail = &(*tail)->next;
        tail = &(*tail)->next;
    }
    return first;
}

/* ================= the interpreter ================= */

static struct js_value eval(struct js *J, struct js_env *env, struct ast *n);
static void exec(struct js *J, struct js_env *env, struct ast *n);
static struct js_value eval_impl(struct js *J, struct js_env *env, struct ast *n);
static void exec_impl(struct js *J, struct js_env *env, struct ast *n);

/* Every recursive call from inside eval_impl()/exec_impl() goes
 * through these two wrappers (they call each other, and themselves,
 * by these names throughout the switch below) - so this is the one
 * place depth is counted, on every level of real AST recursion, not
 * just at the top of a script. See JS_TREE_DEPTH_LIMIT. */
static struct js_value eval(struct js *J, struct js_env *env, struct ast *n) {
    if (n == NULL || J->signal != SIG_NONE) return js_undefined();
    if (++J->tree_depth > JS_TREE_DEPTH_LIMIT) {
        J->tree_depth--;
        js_error(J, "an expression was nested too deeply");
        return js_undefined();
    }
    struct js_value v = eval_impl(J, env, n);
    J->tree_depth--;
    return v;
}

static void exec(struct js *J, struct js_env *env, struct ast *n) {
    if (n == NULL || J->signal != SIG_NONE) return;
    if (++J->tree_depth > JS_TREE_DEPTH_LIMIT) {
        J->tree_depth--;
        js_error(J, "the script was nested too deeply");
        return;
    }
    exec_impl(J, env, n);
    J->tree_depth--;
}

static struct js_env *env_new(struct js *J, struct js_env *parent) {
    struct js_env *e = js_alloc(J, sizeof(*e));
    if (e == NULL) return parent;
    e->parent = parent;
    e->next_alloc = J->envs;
    J->envs = e;
    return e;
}

static struct js_value *env_find(struct js_env *env, const char *name) {
    for (struct js_env *e = env; e != NULL; e = e->parent) {
        struct js_prop *p = prop_find(e->vars, name);
        if (p != NULL) return &p->value;
    }
    return NULL;
}

static void env_define(struct js *J, struct js_env *env, const char *name,
                       struct js_value v) {
    struct js_prop *p = prop_find(env->vars, name);
    if (p != NULL) {
        p->value = v;
        return;
    }
    p = js_alloc(J, sizeof(*p));
    if (p == NULL) return;
    p->name = js_strdup(J, name);
    p->value = v;
    p->next = env->vars;
    env->vars = p;
}

/* Assigning to a name nobody declared defines it globally, which is
 * what a browser does with sloppy-mode code - and all page code is
 * sloppy-mode code. */
static void env_assign(struct js *J, struct js_env *env, const char *name,
                       struct js_value v) {
    struct js_value *slot = env_find(env, name);
    if (slot != NULL) {
        *slot = v;
        return;
    }
    env_define(J, J->global, name, v);
}

static int32_t to_int32(struct js *J, struct js_value v) {
    double d = js_to_number(J, v);
    if (d != d || d == d * 2) return 0;   /* NaN or infinity */
    return (int32_t)(int64_t)d;
}

/* ---- calling ---- */

struct js_value js_call(struct js *J, struct js_value fn, struct js_value self,
                        int argc, struct js_value *argv) {
    if (J->signal != SIG_NONE) return js_undefined();
    if (fn.type != JS_OBJ || fn.obj == NULL) {
        js_throw(J, "tried to call something that is not a function");
        return js_undefined();
    }

    struct js_obj *f = fn.obj;
    if (f->kind == OBJ_NATIVE) {
        if (f->native == NULL) return js_undefined();
        return f->native(J, self, argc, argv);
    }
    if (f->kind != OBJ_FUNC) {
        js_throw(J, "tried to call something that is not a function");
        return js_undefined();
    }

    if (++J->depth > JS_DEPTH_LIMIT) {
        J->depth--;
        js_error(J, "the script called itself too deeply");
        return js_undefined();
    }

    struct js_env *env = env_new(J, f->closure);
    if (f->has_bound_this) {
        env_define(J, env, "this", f->bound_this);
    } else {
        env_define(J, env, "this", self);
    }

    int i = 0;
    for (struct ast *p = f->params; p != NULL; p = p->next, i++) {
        struct js_value v = i < argc ? argv[i] : js_undefined();
        /* A default only applies for a missing argument or one
         * explicitly passed as `undefined` - real JS semantics, and
         * the reason this checks the VALUE, not just `i < argc`. The
         * default expression is evaluated in `env` so it can already
         * see earlier parameters, e.g. `function f(a, b = a + 1)`. */
        if (v.type == JS_UNDEF && p->a != NULL) v = eval(J, env, p->a);
        env_define(J, env, p->text, v);
    }

    struct js_obj *args = js_new_array(J);
    for (int k = 0; k < argc; k++) js_array_push(J, args, argv[k]);
    env_define(J, env, "arguments", js_object(args));

    /* JS_TREE_DEPTH_LIMIT guards expression/statement nesting WITHIN
     * one function body (that's the bug it exists for - see its
     * comment). Deep RECURSIVE CALLS are a separate, already-governed
     * risk (JS_DEPTH_LIMIT/`J->depth` just above) - without this
     * reset, a legitimate call chain a few dozen levels deep would
     * also trip the tree-depth guard, since each level's own body
     * adds a few levels of eval()/exec() nesting on top of the last
     * one's before the interpreter ever gets back around to counting
     * call depth again. Save and restore rather than just clearing,
     * so the CALLER's own nesting (whatever expression this call sits
     * inside of) is still correctly accounted for once this call
     * returns into it. */
    int saved_tree_depth = J->tree_depth;
    J->tree_depth = 0;
    exec(J, env, f->body);
    J->tree_depth = saved_tree_depth;
    J->depth--;

    if (J->signal == SIG_RETURN) {
        J->signal = SIG_NONE;
        struct js_value r = J->signal_value;
        J->signal_value = js_undefined();
        return r;
    }
    return js_undefined();
}

/* ---- lvalues ---- */

static void assign_to(struct js *J, struct js_env *env, struct ast *target,
                      struct js_value v) {
    if (target == NULL) return;

    if (target->type == N_IDENT) {
        env_assign(J, env, target->text, v);
        return;
    }
    if (target->type == N_MEMBER) {
        struct js_value obj = eval(J, env, target->a);
        js_set(J, obj, target->text, v);
        return;
    }
    if (target->type == N_INDEX) {
        struct js_value obj = eval(J, env, target->a);
        struct js_value key = eval(J, env, target->b);
        js_set(J, obj, js_to_string(J, key), v);
        return;
    }
    js_throw(J, "that is not something you can assign to");
}

/* ---- operators ---- */

static struct js_value binary_op(struct js *J, const char *op,
                                 struct js_value a, struct js_value b) {
    if (strcmp(op, "+") == 0) {
        if (a.type == JS_STR || b.type == JS_STR ||
            (a.type == JS_OBJ && a.obj != NULL && a.obj->kind != OBJ_ARRAY) ||
            (b.type == JS_OBJ && b.obj != NULL && b.obj->kind != OBJ_ARRAY)) {
            const char *x = js_to_string(J, a);
            size_t xlen = strlen(x);
            const char *y = js_to_string(J, b);
            size_t ylen = strlen(y);

            char *joined = js_alloc(J, xlen + ylen + 1);
            if (joined == NULL) return js_undefined();
            memcpy(joined, x, xlen);
            memcpy(joined + xlen, y, ylen + 1);
            struct js_value v = js_string_len(J, joined, xlen + ylen);
            return v;
        }
        return js_number(js_to_number(J, a) + js_to_number(J, b));
    }

    if (strcmp(op, "-") == 0) return js_number(js_to_number(J, a) - js_to_number(J, b));
    if (strcmp(op, "*") == 0) return js_number(js_to_number(J, a) * js_to_number(J, b));
    if (strcmp(op, "/") == 0) return js_number(js_to_number(J, a) / js_to_number(J, b));
    if (strcmp(op, "%") == 0) {
        double x = js_to_number(J, a), y = js_to_number(J, b);
        if (y == 0) return js_number((double)0.0 / 0.0);
        double r = x - y * (double)(long long)(x / y);
        return js_number(r);
    }
    if (strcmp(op, "**") == 0) {
        double x = js_to_number(J, a), y = js_to_number(J, b);
        double r = 1;
        int e = (int)y;
        if ((double)e == y && e >= 0 && e < 64) {
            for (int i = 0; i < e; i++) r *= x;
            return js_number(r);
        }
        return js_number((double)0.0 / 0.0);
    }

    if (strcmp(op, "==") == 0)  return js_bool(js_equals(J, a, b, 0));
    if (strcmp(op, "!=") == 0)  return js_bool(!js_equals(J, a, b, 0));
    if (strcmp(op, "===") == 0) return js_bool(js_equals(J, a, b, 1));
    if (strcmp(op, "!==") == 0) return js_bool(!js_equals(J, a, b, 1));

    if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<=") == 0 ||
        strcmp(op, ">=") == 0) {
        if (a.type == JS_STR && b.type == JS_STR) {
            int c = strcmp(a.str->text, b.str->text);
            if (strcmp(op, "<") == 0)  return js_bool(c < 0);
            if (strcmp(op, ">") == 0)  return js_bool(c > 0);
            if (strcmp(op, "<=") == 0) return js_bool(c <= 0);
            return js_bool(c >= 0);
        }
        double x = js_to_number(J, a), y = js_to_number(J, b);
        if (x != x || y != y) return js_bool(0);
        if (strcmp(op, "<") == 0)  return js_bool(x < y);
        if (strcmp(op, ">") == 0)  return js_bool(x > y);
        if (strcmp(op, "<=") == 0) return js_bool(x <= y);
        return js_bool(x >= y);
    }

    if (strcmp(op, "&") == 0)  return js_number(to_int32(J, a) & to_int32(J, b));
    if (strcmp(op, "|") == 0)  return js_number(to_int32(J, a) | to_int32(J, b));
    if (strcmp(op, "^") == 0)  return js_number(to_int32(J, a) ^ to_int32(J, b));
    if (strcmp(op, "<<") == 0) {
        return js_number(to_int32(J, a) << (to_int32(J, b) & 31));
    }
    if (strcmp(op, ">>") == 0) {
        return js_number(to_int32(J, a) >> (to_int32(J, b) & 31));
    }
    if (strcmp(op, ">>>") == 0) {
        uint32_t x = (uint32_t)to_int32(J, a);
        return js_number(x >> (to_int32(J, b) & 31));
    }

    if (strcmp(op, "in") == 0) {
        if (b.type != JS_OBJ) return js_bool(0);
        struct js_value got = js_get(J, b, js_to_string(J, a));
        return js_bool(got.type != JS_UNDEF);
    }
    if (strcmp(op, "instanceof") == 0) {
        return js_bool(a.type == JS_OBJ);
    }

    return js_undefined();
}

static const char *type_of(struct js_value v) {
    switch (v.type) {
    case JS_UNDEF: return "undefined";
    case JS_NULL:  return "object";
    case JS_BOOL:  return "boolean";
    case JS_NUM:   return "number";
    case JS_STR:   return "string";
    default:
        if (v.obj != NULL &&
            (v.obj->kind == OBJ_FUNC || v.obj->kind == OBJ_NATIVE)) {
            return "function";
        }
        return "object";
    }
}

/* ---- expressions ---- */

struct js_value js_new_regex(struct js *J, const char *source,
                             const char *flags);

static struct js_value eval_impl(struct js *J, struct js_env *env, struct ast *n) {
    if (n == NULL || J->signal != SIG_NONE) return js_undefined();
    if (++J->steps > J->step_limit) {
        js_error(J, "the script ran for too long and was stopped");
        return js_undefined();
    }

    switch (n->type) {
    case N_NUM:   return js_number(n->num);
    case N_STR:   return js_string(J, n->text != NULL ? n->text : "");
    case N_BOOL:  return js_bool(n->num != 0);
    case N_NULL:  return js_null();
    case N_UNDEF: return js_undefined();
    case N_REGEX: return js_new_regex(J, n->text, n->op);

    case N_THIS: {
        struct js_value *slot = env_find(env, "this");
        return slot != NULL ? *slot : js_object(J->window);
    }

    case N_IDENT: {
        struct js_value *slot = env_find(env, n->text);
        if (slot != NULL) return *slot;
        /* Globals live on the window object, which is where a page
         * expects to find them. */
        struct js_value v = js_get(J, js_object(J->window), n->text);
        return v;
    }

    case N_ARRAY: {
        struct js_obj *arr = js_new_array(J);
        for (struct ast *item = n->a; item != NULL; item = item->next) {
            js_array_push(J, arr, eval(J, env, item));
        }
        return js_object(arr);
    }

    case N_OBJECT: {
        struct js_obj *obj = js_new_object(J, OBJ_PLAIN);
        for (struct ast *p = n->a; p != NULL; p = p->next) {
            js_def(J, obj, p->text != NULL ? p->text : "", eval(J, env, p->a));
        }
        return js_object(obj);
    }

    case N_FUNC: {
        struct js_obj *fn = js_new_object(J, OBJ_FUNC);
        if (fn == NULL) return js_undefined();
        fn->params = n->a;
        fn->body = n->b;
        fn->closure = env;
        fn->name = n->text;
        if (n->prefix) {
            /* An arrow keeps the `this` it was written in. */
            struct js_value *outer = env_find(env, "this");
            fn->has_bound_this = 1;
            fn->bound_this = outer != NULL ? *outer : js_undefined();
        }
        if (n->text != NULL && n->num == 0) {
            /* A named function expression can call itself. */
            struct js_env *self_env = env_new(J, env);
            env_define(J, self_env, n->text, js_object(fn));
            fn->closure = self_env;
        }
        return js_object(fn);
    }

    case N_MEMBER: {
        struct js_value obj = eval(J, env, n->a);
        return js_get(J, obj, n->text != NULL ? n->text : "");
    }

    case N_INDEX: {
        struct js_value obj = eval(J, env, n->a);
        struct js_value key = eval(J, env, n->b);
        return js_get(J, obj, js_to_string(J, key));
    }

    case N_CALL: {
        struct js_value self = js_undefined();
        struct js_value fn;

        if (n->a != NULL && n->a->type == N_MEMBER) {
            self = eval(J, env, n->a->a);
            fn = js_get(J, self, n->a->text != NULL ? n->a->text : "");
        } else if (n->a != NULL && n->a->type == N_INDEX) {
            self = eval(J, env, n->a->a);
            struct js_value key = eval(J, env, n->a->b);
            fn = js_get(J, self, js_to_string(J, key));
        } else {
            fn = eval(J, env, n->a);
        }
        if (J->signal != SIG_NONE) return js_undefined();

        struct js_value argv[16];
        int argc = 0;
        for (struct ast *arg = n->b; arg != NULL && argc < 16; arg = arg->next) {
            argv[argc++] = eval(J, env, arg);
        }
        if (J->signal != SIG_NONE) return js_undefined();

        if (fn.type != JS_OBJ) {
            const char *what = n->a != NULL && n->a->text != NULL ? n->a->text
                                                                  : "it";
            js_throw(J, "there is no function to call");
            snprintf(J->error, sizeof(J->error),
                     "line %d: %s is not a function", n->line, what);
            return js_undefined();
        }
        return js_call(J, fn, self, argc, argv);
    }

    case N_NEW: {
        struct js_value fn = eval(J, env, n->a);
        struct js_value argv[16];
        int argc = 0;
        for (struct ast *arg = n->b; arg != NULL && argc < 16; arg = arg->next) {
            argv[argc++] = eval(J, env, arg);
        }
        if (J->signal != SIG_NONE) return js_undefined();

        if (fn.type == JS_OBJ && fn.obj != NULL && fn.obj->kind == OBJ_NATIVE) {
            /* A native constructor builds its own object. */
            return js_call(J, fn, js_undefined(), argc, argv);
        }

        struct js_obj *obj = js_new_object(J, OBJ_PLAIN);
        struct js_value self = js_object(obj);
        if (fn.type == JS_OBJ && fn.obj != NULL) {
            struct js_value proto = js_get(J, fn, "prototype");
            if (proto.type == JS_OBJ) js_def(J, obj, "__proto__", proto);
        }
        struct js_value r = js_call(J, fn, self, argc, argv);
        return r.type == JS_OBJ ? r : self;
    }

    case N_BINARY: {
        struct js_value a = eval(J, env, n->a);
        struct js_value b = eval(J, env, n->b);
        if (J->signal != SIG_NONE) return js_undefined();
        return binary_op(J, n->op, a, b);
    }

    case N_LOGICAL: {
        struct js_value a = eval(J, env, n->a);
        if (strcmp(n->op, "&&") == 0) {
            return js_truthy(a) ? eval(J, env, n->b) : a;
        }
        if (strcmp(n->op, "??") == 0) {
            return (a.type == JS_UNDEF || a.type == JS_NULL)
                       ? eval(J, env, n->b) : a;
        }
        return js_truthy(a) ? a : eval(J, env, n->b);
    }

    case N_UNARY: {
        if (strcmp(n->op, "typeof") == 0) {
            /* typeof must not complain about a name that is not
             * there - that is what it is for. */
            if (n->a != NULL && n->a->type == N_IDENT) {
                struct js_value *slot = env_find(env, n->a->text);
                if (slot == NULL) {
                    struct js_value g = js_get(J, js_object(J->window),
                                               n->a->text);
                    return js_string(J, type_of(g));
                }
                return js_string(J, type_of(*slot));
            }
            return js_string(J, type_of(eval(J, env, n->a)));
        }

        struct js_value v = eval(J, env, n->a);
        if (strcmp(n->op, "!") == 0) return js_bool(!js_truthy(v));
        if (strcmp(n->op, "-") == 0) return js_number(-js_to_number(J, v));
        if (strcmp(n->op, "+") == 0) return js_number(js_to_number(J, v));
        if (strcmp(n->op, "~") == 0) return js_number(~to_int32(J, v));
        if (strcmp(n->op, "void") == 0) return js_undefined();
        if (strcmp(n->op, "delete") == 0) return js_bool(1);
        return js_undefined();
    }

    case N_UPDATE: {
        struct js_value old = eval(J, env, n->a);
        double d = js_to_number(J, old);
        double next = strcmp(n->op, "++") == 0 ? d + 1 : d - 1;
        assign_to(J, env, n->a, js_number(next));
        return js_number(n->prefix ? next : d);
    }

    case N_ASSIGN: {
        if (strcmp(n->op, "=") == 0) {
            struct js_value v = eval(J, env, n->b);
            assign_to(J, env, n->a, v);
            return v;
        }
        if (strcmp(n->op, "&&=") == 0 || strcmp(n->op, "||=") == 0 ||
            strcmp(n->op, "?\?=") == 0) {
            struct js_value cur = eval(J, env, n->a);
            int assign = strcmp(n->op, "&&=") == 0 ? js_truthy(cur)
                       : strcmp(n->op, "||=") == 0 ? !js_truthy(cur)
                       : (cur.type == JS_UNDEF || cur.type == JS_NULL);
            if (!assign) return cur;
            struct js_value v = eval(J, env, n->b);
            assign_to(J, env, n->a, v);
            return v;
        }

        char op[4] = { n->op[0], '\0', '\0', '\0' };
        if (n->op[1] != '=') {   /* <<=, >>=, >>>= */
            op[1] = n->op[1];
            if (n->op[2] != '=') op[2] = n->op[2];
        }
        struct js_value cur = eval(J, env, n->a);
        struct js_value rhs = eval(J, env, n->b);
        struct js_value v = binary_op(J, op, cur, rhs);
        assign_to(J, env, n->a, v);
        return v;
    }

    case N_COND: {
        struct js_value c = eval(J, env, n->a);
        return js_truthy(c) ? eval(J, env, n->b) : eval(J, env, n->c);
    }

    case N_SEQ:
        eval(J, env, n->a);
        return eval(J, env, n->b);

    default:
        break;
    }
    return js_undefined();
}

/* ---- statements ---- */

/* Function declarations are visible before the line that declares
 * them, which is how pages put their helpers at the bottom. */
static void hoist(struct js *J, struct js_env *env, struct ast *list) {
    for (struct ast *st = list; st != NULL; st = st->next) {
        if (st->type == N_FUNC && st->num != 0 && st->text != NULL) {
            env_define(J, env, st->text, eval(J, env, st));
        }
    }
}

static void exec_list(struct js *J, struct js_env *env, struct ast *list) {
    hoist(J, env, list);
    for (struct ast *st = list; st != NULL; st = st->next) {
        exec(J, env, st);
        if (J->signal != SIG_NONE) return;
    }
}

static void exec_impl(struct js *J, struct js_env *env, struct ast *n) {
    if (n == NULL || J->signal != SIG_NONE) return;
    if (++J->steps > J->step_limit) {
        js_error(J, "the script ran for too long and was stopped");
        return;
    }

    switch (n->type) {
    case N_BLOCK:
        exec_list(J, n->num != 0 ? env_new(J, env) : env, n->a);
        return;

    case N_EMPTY:
        return;

    case N_EXPR:
        eval(J, env, n->a);
        return;

    case N_VAR:
        env_define(J, env, n->text,
                   n->a != NULL ? eval(J, env, n->a) : js_undefined());
        return;

    case N_FUNC:
        if (n->num != 0 && n->text != NULL) {
            if (env_find(env, n->text) == NULL) {
                env_define(J, env, n->text, eval(J, env, n));
            }
        } else {
            eval(J, env, n);
        }
        return;

    case N_IF:
        if (js_truthy(eval(J, env, n->a))) exec(J, env, n->b);
        else exec(J, env, n->c);
        return;

    case N_WHILE:
        for (;;) {
            if (!js_truthy(eval(J, env, n->a)) || J->signal != SIG_NONE) break;
            exec(J, env, n->b);
            if (J->signal == SIG_BREAK) {
                J->signal = SIG_NONE;
                break;
            }
            if (J->signal == SIG_CONTINUE) J->signal = SIG_NONE;
            if (J->signal != SIG_NONE) break;
        }
        return;

    case N_DO:
        for (;;) {
            exec(J, env, n->b);
            if (J->signal == SIG_BREAK) {
                J->signal = SIG_NONE;
                break;
            }
            if (J->signal == SIG_CONTINUE) J->signal = SIG_NONE;
            if (J->signal != SIG_NONE) break;
            if (!js_truthy(eval(J, env, n->a))) break;
        }
        return;

    case N_FOR: {
        struct js_env *loop = env_new(J, env);
        if (n->a != NULL) {
            if (n->a->type == N_VAR) {
                for (struct ast *d = n->a; d != NULL; d = d->next) {
                    exec(J, loop, d);
                }
            } else {
                exec(J, loop, n->a);
            }
        }
        for (;;) {
            if (n->b != NULL && !js_truthy(eval(J, loop, n->b))) break;
            if (J->signal != SIG_NONE) break;

            exec(J, loop, n->d);
            if (J->signal == SIG_BREAK) {
                J->signal = SIG_NONE;
                break;
            }
            if (J->signal == SIG_CONTINUE) J->signal = SIG_NONE;
            if (J->signal != SIG_NONE) break;

            if (n->c != NULL) eval(J, loop, n->c);
            if (J->signal != SIG_NONE) break;
        }
        return;
    }

    case N_FORIN: {
        struct js_value src = eval(J, env, n->a);
        struct js_env *loop = env_new(J, env);
        env_define(J, loop, n->text, js_undefined());

        int of = n->num != 0;
        struct js_value *slot = env_find(loop, n->text);

        if (src.type == JS_STR && of) {
            for (size_t i = 0; i < src.str->len; i++) {
                *slot = js_string_len(J, src.str->text + i, 1);
                exec(J, loop, n->b);
                if (J->signal == SIG_BREAK) { J->signal = SIG_NONE; break; }
                if (J->signal == SIG_CONTINUE) J->signal = SIG_NONE;
                if (J->signal != SIG_NONE) break;
            }
            return;
        }
        if (src.type != JS_OBJ || src.obj == NULL) return;

        struct js_obj *o = src.obj;
        if (o->kind == OBJ_ARRAY) {
            for (int i = 0; i < o->len; i++) {
                *slot = of ? o->items[i] : js_number(i);
                exec(J, loop, n->b);
                if (J->signal == SIG_BREAK) { J->signal = SIG_NONE; break; }
                if (J->signal == SIG_CONTINUE) J->signal = SIG_NONE;
                if (J->signal != SIG_NONE) break;
            }
            return;
        }
        if (o->kind == OBJ_LIST) {
            for (int i = 0; i < o->nnodes; i++) {
                *slot = of ? js_object(js_new_node(J, o->nodes[i]))
                           : js_number(i);
                exec(J, loop, n->b);
                if (J->signal == SIG_BREAK) { J->signal = SIG_NONE; break; }
                if (J->signal == SIG_CONTINUE) J->signal = SIG_NONE;
                if (J->signal != SIG_NONE) break;
            }
            return;
        }
        for (struct js_prop *p = o->props; p != NULL; p = p->next) {
            if (strcmp(p->name, "__proto__") == 0) continue;
            *slot = of ? p->value : js_string(J, p->name);
            exec(J, loop, n->b);
            if (J->signal == SIG_BREAK) { J->signal = SIG_NONE; break; }
            if (J->signal == SIG_CONTINUE) J->signal = SIG_NONE;
            if (J->signal != SIG_NONE) break;
        }
        return;
    }

    case N_RETURN: {
        struct js_value v = n->a != NULL ? eval(J, env, n->a) : js_undefined();
        /* If working out the value threw, that is what leaves the
         * function - overwriting the signal here would swallow the
         * error and return undefined instead. */
        if (J->signal != SIG_NONE) return;
        J->signal_value = v;
        J->signal = SIG_RETURN;
        return;
    }

    case N_BREAK:
        J->signal = SIG_BREAK;
        return;

    case N_CONTINUE:
        J->signal = SIG_CONTINUE;
        return;

    case N_THROW: {
        struct js_value v = eval(J, env, n->a);
        if (J->signal != SIG_NONE) return;
        J->signal = SIG_THROW;
        J->signal_value = v;
        snprintf(J->error, sizeof(J->error), "%s", js_to_string(J, v));
        return;
    }

    case N_TRY: {
        exec(J, env, n->a);
        if (J->signal == SIG_THROW && n->b != NULL) {
            struct js_value thrown = J->signal_value;
            J->signal = SIG_NONE;
            J->signal_value = js_undefined();
            J->error[0] = '\0';

            struct js_env *catch_env = env_new(J, env);
            if (n->text != NULL) env_define(J, catch_env, n->text, thrown);
            exec_list(J, catch_env, n->b->a);
        }
        if (n->c != NULL) {
            int saved = J->signal;
            struct js_value saved_value = J->signal_value;
            J->signal = SIG_NONE;
            exec(J, env, n->c);
            if (J->signal == SIG_NONE) {
                J->signal = saved;
                J->signal_value = saved_value;
            }
        }
        return;
    }

    case N_SWITCH: {
        struct js_value v = eval(J, env, n->a);
        struct js_env *body = env_new(J, env);
        int running = 0;

        for (int pass = 0; pass < 2 && !running; pass++) {
            for (struct ast *c = n->b; c != NULL; c = c->next) {
                if (!running) {
                    if (pass == 0) {
                        if (c->a == NULL) continue;   /* default: pass two */
                        if (!js_equals(J, v, eval(J, body, c->a), 1)) continue;
                    } else if (c->a != NULL) {
                        continue;
                    }
                    running = 1;
                }
                exec_list(J, body, c->b);
                if (J->signal == SIG_BREAK) {
                    J->signal = SIG_NONE;
                    return;
                }
                if (J->signal != SIG_NONE) return;
            }
        }
        return;
    }

    default:
        eval(J, env, n);
        return;
    }
}

/* ================= the standard library ================= */

#include "jsregex.h"

static struct js_value arg(int argc, struct js_value *argv, int i) {
    return i < argc ? argv[i] : js_undefined();
}

static const char *self_text(struct js *J, struct js_value self) {
    return js_to_string(J, self);
}

/* An index into a string, the way JavaScript counts them: negative
 * means from the end, and everything is clamped. */
static int clamp_index(double d, int len, int def) {
    if (d != d) return def;
    int i = (int)d;
    if (i < 0) i += len;
    if (i < 0) i = 0;
    if (i > len) i = len;
    return i;
}

/* ---- strings ---- */

static struct js_value fn_char_at(struct js *J, struct js_value self, int argc,
                                  struct js_value *argv) {
    const char *s = self_text(J, self);
    int len = (int)strlen(s);
    int i = (int)js_to_number(J, arg(argc, argv, 0));
    if (i < 0 || i >= len) return js_string(J, "");
    return js_string_len(J, s + i, 1);
}

static struct js_value fn_char_code(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    const char *s = self_text(J, self);
    int len = (int)strlen(s);
    int i = (int)js_to_number(J, arg(argc, argv, 0));
    if (i < 0 || i >= len) return js_number((double)0.0 / 0.0);
    return js_number((unsigned char)s[i]);
}

static struct js_value fn_index_of(struct js *J, struct js_value self,
                                   int argc, struct js_value *argv) {
    const char *s = self_text(J, self);
    const char *needle = js_to_string(J, arg(argc, argv, 0));
    int from = argc > 1 ? (int)js_to_number(J, argv[1]) : 0;
    if (from < 0) from = 0;
    if (from > (int)strlen(s)) return js_number(-1);
    const char *hit = strstr(s + from, needle);
    return js_number(hit != NULL ? (double)(hit - s) : -1);
}

static struct js_value fn_last_index_of(struct js *J, struct js_value self,
                                        int argc, struct js_value *argv) {
    const char *s = self_text(J, self);
    const char *needle = js_to_string(J, arg(argc, argv, 0));
    size_t nlen = strlen(needle);
    if (nlen == 0) return js_number((double)strlen(s));

    long best = -1;
    for (const char *p = s; (p = strstr(p, needle)) != NULL; p++) {
        best = p - s;
    }
    return js_number((double)best);
}

static struct js_value fn_slice(struct js *J, struct js_value self, int argc,
                                struct js_value *argv) {
    const char *s = self_text(J, self);
    int len = (int)strlen(s);
    int start = clamp_index(js_to_number(J, arg(argc, argv, 0)), len, 0);
    int end = argc > 1 && argv[1].type != JS_UNDEF
                  ? clamp_index(js_to_number(J, argv[1]), len, len)
                  : len;
    if (end < start) end = start;
    return js_string_len(J, s + start, (size_t)(end - start));
}

static struct js_value fn_substring(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    const char *s = self_text(J, self);
    int len = (int)strlen(s);
    double a = js_to_number(J, arg(argc, argv, 0));
    double b = argc > 1 && argv[1].type != JS_UNDEF ? js_to_number(J, argv[1])
                                                    : len;
    int start = a != a || a < 0 ? 0 : (a > len ? len : (int)a);
    int end = b != b || b < 0 ? 0 : (b > len ? len : (int)b);
    if (start > end) {
        int t = start;
        start = end;
        end = t;
    }
    return js_string_len(J, s + start, (size_t)(end - start));
}

static struct js_value fn_substr(struct js *J, struct js_value self, int argc,
                                 struct js_value *argv) {
    const char *s = self_text(J, self);
    int len = (int)strlen(s);
    int start = clamp_index(js_to_number(J, arg(argc, argv, 0)), len, 0);
    int count = argc > 1 ? (int)js_to_number(J, argv[1]) : len - start;
    if (count < 0) count = 0;
    if (start + count > len) count = len - start;
    return js_string_len(J, s + start, (size_t)count);
}

static struct js_value fn_to_upper(struct js *J, struct js_value self,
                                   int argc, struct js_value *argv) {
    (void)argc;
    (void)argv;
    const char *s = self_text(J, self);
    size_t len = strlen(s);
    char *out = js_alloc(J, len + 1);
    if (out == NULL) return js_undefined();
    for (size_t i = 0; i < len; i++) {
        out[i] = (s[i] >= 'a' && s[i] <= 'z') ? (char)(s[i] - 32) : s[i];
    }
    out[len] = '\0';
    return js_string_len(J, out, len);
}

static struct js_value fn_to_lower(struct js *J, struct js_value self,
                                   int argc, struct js_value *argv) {
    (void)argc;
    (void)argv;
    const char *s = self_text(J, self);
    size_t len = strlen(s);
    char *out = js_alloc(J, len + 1);
    if (out == NULL) return js_undefined();
    for (size_t i = 0; i < len; i++) {
        out[i] = (s[i] >= 'A' && s[i] <= 'Z') ? (char)(s[i] + 32) : s[i];
    }
    out[len] = '\0';
    return js_string_len(J, out, len);
}

static struct js_value fn_trim(struct js *J, struct js_value self, int argc,
                               struct js_value *argv) {
    (void)argc;
    (void)argv;
    const char *s = self_text(J, self);
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' ||
                       s[len - 1] == '\n' || s[len - 1] == '\r')) {
        len--;
    }
    return js_string_len(J, s, len);
}

static struct js_value fn_includes(struct js *J, struct js_value self,
                                   int argc, struct js_value *argv) {
    if (self.type == JS_OBJ && self.obj != NULL && self.obj->kind == OBJ_ARRAY) {
        struct js_value needle = arg(argc, argv, 0);
        for (int i = 0; i < self.obj->len; i++) {
            if (js_equals(J, self.obj->items[i], needle, 1)) return js_bool(1);
        }
        return js_bool(0);
    }
    const char *s = self_text(J, self);
    return js_bool(strstr(s, js_to_string(J, arg(argc, argv, 0))) != NULL);
}

static struct js_value fn_starts_with(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    const char *s = self_text(J, self);
    const char *needle = js_to_string(J, arg(argc, argv, 0));
    return js_bool(strncmp(s, needle, strlen(needle)) == 0);
}

static struct js_value fn_ends_with(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    const char *s = self_text(J, self);
    const char *needle = js_to_string(J, arg(argc, argv, 0));
    size_t slen = strlen(s), nlen = strlen(needle);
    return js_bool(nlen <= slen && strcmp(s + slen - nlen, needle) == 0);
}

static struct js_value fn_repeat(struct js *J, struct js_value self, int argc,
                                 struct js_value *argv) {
    const char *s = self_text(J, self);
    size_t len = strlen(s);
    int times = (int)js_to_number(J, arg(argc, argv, 0));
    if (times < 0) times = 0;
    if (len * (size_t)times > 1u << 20) {
        js_throw(J, "repeat() asked for too much text");
        return js_undefined();
    }
    char *out = js_alloc(J, len * (size_t)times + 1);
    if (out == NULL) return js_undefined();
    out[0] = '\0';
    for (int i = 0; i < times; i++) memcpy(out + len * (size_t)i, s, len);
    out[len * (size_t)times] = '\0';
    return js_string_len(J, out, len * (size_t)times);
}

static struct js_value fn_concat(struct js *J, struct js_value self, int argc,
                                 struct js_value *argv) {
    if (self.type == JS_OBJ && self.obj != NULL && self.obj->kind == OBJ_ARRAY) {
        struct js_obj *out = js_new_array(J);
        for (int i = 0; i < self.obj->len; i++) {
            js_array_push(J, out, self.obj->items[i]);
        }
        for (int i = 0; i < argc; i++) {
            if (argv[i].type == JS_OBJ && argv[i].obj != NULL &&
                argv[i].obj->kind == OBJ_ARRAY) {
                for (int k = 0; k < argv[i].obj->len; k++) {
                    js_array_push(J, out, argv[i].obj->items[k]);
                }
            } else {
                js_array_push(J, out, argv[i]);
            }
        }
        return js_object(out);
    }

    struct js_value v = js_string(J, self_text(J, self));
    for (int i = 0; i < argc; i++) v = binary_op(J, "+", v, argv[i]);
    return v;
}

/* The regular-expression half of the string methods, which is most of
 * what pages use them for. */
static struct rx *value_regex(struct js *J, struct js_value v, int *owned) {
    *owned = 0;
    if (v.type == JS_OBJ && v.obj != NULL && v.obj->regex != NULL) {
        return v.obj->regex;
    }
    /* A plain string pattern is matched literally, so its regex
     * characters have to be escaped before compiling. */
    const char *text = js_to_string(J, v);
    size_t len = strlen(text);
    char *quoted = malloc(len * 2 + 1);
    if (quoted == NULL) return NULL;
    size_t at = 0;
    for (size_t i = 0; i < len; i++) {
        if (strchr("\\^$.|?*+()[]{}", text[i]) != NULL) quoted[at++] = '\\';
        quoted[at++] = text[i];
    }
    quoted[at] = '\0';
    struct rx *rx = rx_compile(quoted, "");
    free(quoted);
    *owned = 1;
    return rx;
}

static struct js_value fn_split(struct js *J, struct js_value self, int argc,
                                struct js_value *argv) {
    const char *s = self_text(J, self);
    struct js_obj *out = js_new_array(J);

    if (argc == 0 || argv[0].type == JS_UNDEF) {
        js_array_push(J, out, js_string(J, s));
        return js_object(out);
    }

    if (argv[0].type == JS_STR && argv[0].str->len == 0) {
        for (const char *p = s; *p != '\0'; p++) {
            js_array_push(J, out, js_string_len(J, p, 1));
        }
        return js_object(out);
    }

    int owned = 0;
    struct rx *rx = value_regex(J, argv[0], &owned);
    if (rx == NULL) {
        js_array_push(J, out, js_string(J, s));
        return js_object(out);
    }

    int at = 0, mstart, mend, caps[RX_MAX_GROUPS * 2];
    int len = (int)strlen(s);
    while (at <= len && rx_search(rx, s, at, &mstart, &mend, caps)) {
        if (mend == mstart && mstart >= len) break;
        js_array_push(J, out, js_string_len(J, s + at, (size_t)(mstart - at)));
        for (int g = 1; g <= rx_groups(rx) && g < RX_MAX_GROUPS; g++) {
            if (caps[g * 2] >= 0) {
                js_array_push(J, out,
                              js_string_len(J, s + caps[g * 2],
                                            (size_t)(caps[g * 2 + 1] -
                                                     caps[g * 2])));
            }
        }
        at = mend > mstart ? mend : mend + 1;
    }
    js_array_push(J, out, js_string(J, s + (at < len ? at : len)));

    if (owned) rx_free(rx);
    return js_object(out);
}

/* $1..$9 and $& in a replacement string. */
static void append_replacement(struct js *J, char **out, size_t *at,
                               size_t *cap, const char *repl, const char *text,
                               int mstart, int mend, const int *caps) {
    (void)J;
    for (const char *p = repl; *p != '\0'; p++) {
        const char *piece = NULL;
        size_t plen = 0;
        char one[2];

        if (*p == '$' && p[1] != '\0') {
            if (p[1] == '&') {
                piece = text + mstart;
                plen = (size_t)(mend - mstart);
                p++;
            } else if (p[1] >= '1' && p[1] <= '9') {
                int g = p[1] - '0';
                p++;
                if (caps[g * 2] >= 0) {
                    piece = text + caps[g * 2];
                    plen = (size_t)(caps[g * 2 + 1] - caps[g * 2]);
                } else {
                    continue;
                }
            } else if (p[1] == '$') {
                p++;
                one[0] = '$';
                piece = one;
                plen = 1;
            }
        }
        if (piece == NULL) {
            one[0] = *p;
            piece = one;
            plen = 1;
        }

        if (*at + plen + 1 > *cap) {
            size_t want = *cap ? *cap * 2 : 256;
            while (want < *at + plen + 1) want *= 2;
            char *grown = realloc(*out, want);
            if (grown == NULL) return;
            *out = grown;
            *cap = want;
        }
        memcpy(*out + *at, piece, plen);
        *at += plen;
    }
}

static struct js_value fn_replace(struct js *J, struct js_value self,
                                  int argc, struct js_value *argv) {
    const char *s = self_text(J, self);
    int len = (int)strlen(s);

    int owned = 0;
    struct rx *rx = value_regex(J, arg(argc, argv, 0), &owned);
    if (rx == NULL) return js_string(J, s);

    int global = rx_is_global(rx);
    struct js_value repl = arg(argc, argv, 1);
    int callable = repl.type == JS_OBJ && repl.obj != NULL &&
                   (repl.obj->kind == OBJ_FUNC || repl.obj->kind == OBJ_NATIVE);

    char *out = NULL;
    size_t at = 0, cap = 0;
    int pos = 0, mstart, mend, caps[RX_MAX_GROUPS * 2];

    while (pos <= len && rx_search(rx, s, pos, &mstart, &mend, caps)) {
        append_replacement(J, &out, &at, &cap, "", s, 0, 0, caps);
        /* the text before the match */
        if (at + (size_t)(mstart - pos) + 1 > cap) {
            size_t want = cap ? cap * 2 : 256;
            while (want < at + (size_t)(mstart - pos) + 1) want *= 2;
            char *grown = realloc(out, want);
            if (grown == NULL) break;
            out = grown;
            cap = want;
        }
        memcpy(out + at, s + pos, (size_t)(mstart - pos));
        at += (size_t)(mstart - pos);

        if (callable) {
            struct js_value args[RX_MAX_GROUPS + 2];
            int n = 0;
            args[n++] = js_string_len(J, s + mstart, (size_t)(mend - mstart));
            for (int g = 1; g <= rx_groups(rx) && g < RX_MAX_GROUPS; g++) {
                args[n++] = caps[g * 2] >= 0
                                ? js_string_len(J, s + caps[g * 2],
                                                (size_t)(caps[g * 2 + 1] -
                                                         caps[g * 2]))
                                : js_undefined();
            }
            args[n++] = js_number(mstart);
            struct js_value r = js_call(J, repl, js_undefined(), n, args);
            append_replacement(J, &out, &at, &cap, js_to_string(J, r), s,
                               mstart, mend, caps);
        } else {
            append_replacement(J, &out, &at, &cap, js_to_string(J, repl), s,
                               mstart, mend, caps);
        }

        pos = mend > mstart ? mend : mend + 1;
        if (!global) break;
        if (J->signal != SIG_NONE) break;
    }

    size_t tail = pos <= len ? (size_t)(len - pos) : 0;
    if (at + tail + 1 > cap) {
        size_t want = cap ? cap * 2 : 256;
        while (want < at + tail + 1) want *= 2;
        char *grown = realloc(out, want);
        if (grown != NULL) {
            out = grown;
            cap = want;
        }
    }
    if (out != NULL && at + tail + 1 <= cap) {
        memcpy(out + at, s + pos, tail);
        at += tail;
        out[at] = '\0';
    }

    struct js_value result = out != NULL ? js_string_len(J, out, at)
                                         : js_string(J, s);
    free(out);
    if (owned) rx_free(rx);
    return result;
}

static struct js_value fn_match(struct js *J, struct js_value self, int argc,
                                struct js_value *argv) {
    const char *s = self_text(J, self);
    int owned = 0;
    struct rx *rx = value_regex(J, arg(argc, argv, 0), &owned);
    if (rx == NULL) return js_null();

    int mstart, mend, caps[RX_MAX_GROUPS * 2];
    struct js_obj *out = js_new_array(J);

    if (rx_is_global(rx)) {
        int pos = 0, len = (int)strlen(s);
        while (pos <= len && rx_search(rx, s, pos, &mstart, &mend, caps)) {
            js_array_push(J, out,
                          js_string_len(J, s + mstart,
                                        (size_t)(mend - mstart)));
            pos = mend > mstart ? mend : mend + 1;
        }
        if (owned) rx_free(rx);
        return out->len > 0 ? js_object(out) : js_null();
    }

    if (!rx_search(rx, s, 0, &mstart, &mend, caps)) {
        if (owned) rx_free(rx);
        return js_null();
    }
    js_array_push(J, out, js_string_len(J, s + mstart,
                                        (size_t)(mend - mstart)));
    for (int g = 1; g <= rx_groups(rx) && g < RX_MAX_GROUPS; g++) {
        js_array_push(J, out,
                      caps[g * 2] >= 0
                          ? js_string_len(J, s + caps[g * 2],
                                          (size_t)(caps[g * 2 + 1] -
                                                   caps[g * 2]))
                          : js_undefined());
    }
    js_def(J, out, "index", js_number(mstart));
    js_def(J, out, "input", js_string(J, s));
    if (owned) rx_free(rx);
    return js_object(out);
}

static struct js_value fn_search(struct js *J, struct js_value self, int argc,
                                 struct js_value *argv) {
    const char *s = self_text(J, self);
    int owned = 0;
    struct rx *rx = value_regex(J, arg(argc, argv, 0), &owned);
    if (rx == NULL) return js_number(-1);
    int mstart, mend, caps[RX_MAX_GROUPS * 2];
    int found = rx_search(rx, s, 0, &mstart, &mend, caps);
    if (owned) rx_free(rx);
    return js_number(found ? mstart : -1);
}

/* ---- arrays ---- */

static struct js_value fn_push(struct js *J, struct js_value self, int argc,
                               struct js_value *argv) {
    if (self.type != JS_OBJ || self.obj == NULL) return js_undefined();
    for (int i = 0; i < argc; i++) js_array_push(J, self.obj, argv[i]);
    return js_number(self.obj->len);
}

static struct js_value fn_pop(struct js *J, struct js_value self, int argc,
                              struct js_value *argv) {
    (void)J;
    (void)argc;
    (void)argv;
    if (self.type != JS_OBJ || self.obj == NULL || self.obj->len == 0) {
        return js_undefined();
    }
    return self.obj->items[--self.obj->len];
}

static struct js_value fn_shift(struct js *J, struct js_value self, int argc,
                                struct js_value *argv) {
    (void)J;
    (void)argc;
    (void)argv;
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL || a->len == 0) return js_undefined();
    struct js_value first = a->items[0];
    memmove(a->items, a->items + 1, (size_t)(a->len - 1) * sizeof(*a->items));
    a->len--;
    return first;
}

static struct js_value fn_unshift(struct js *J, struct js_value self,
                                  int argc, struct js_value *argv) {
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return js_undefined();
    for (int i = argc - 1; i >= 0; i--) {
        js_array_push(J, a, js_undefined());
        memmove(a->items + 1, a->items, (size_t)(a->len - 1) * sizeof(*a->items));
        a->items[0] = argv[i];
    }
    return js_number(a->len);
}

static struct js_value fn_array_slice(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return js_undefined();
    int start = clamp_index(js_to_number(J, arg(argc, argv, 0)), a->len, 0);
    int end = argc > 1 && argv[1].type != JS_UNDEF
                  ? clamp_index(js_to_number(J, argv[1]), a->len, a->len)
                  : a->len;
    struct js_obj *out = js_new_array(J);
    for (int i = start; i < end; i++) js_array_push(J, out, a->items[i]);
    return js_object(out);
}

static struct js_value fn_splice(struct js *J, struct js_value self, int argc,
                                 struct js_value *argv) {
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return js_undefined();

    int start = clamp_index(js_to_number(J, arg(argc, argv, 0)), a->len, 0);
    int count = argc > 1 ? (int)js_to_number(J, argv[1]) : a->len - start;
    if (count < 0) count = 0;
    if (start + count > a->len) count = a->len - start;

    struct js_obj *removed = js_new_array(J);
    for (int i = 0; i < count; i++) {
        js_array_push(J, removed, a->items[start + i]);
    }

    int extra = argc > 2 ? argc - 2 : 0;
    int newlen = a->len - count + extra;
    while (a->cap < newlen) js_array_push(J, a, js_undefined());
    a->len = a->len;

    memmove(a->items + start + extra, a->items + start + count,
            (size_t)(a->len - start - count) * sizeof(*a->items));
    for (int i = 0; i < extra; i++) a->items[start + i] = argv[2 + i];
    a->len = newlen;
    return js_object(removed);
}

static struct js_value fn_join(struct js *J, struct js_value self, int argc,
                               struct js_value *argv) {
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return js_string(J, "");
    const char *sep = argc > 0 ? js_to_string(J, argv[0]) : ",";

    size_t total = 1;
    for (int i = 0; i < a->len; i++) {
        total += strlen(js_to_string(J, a->items[i])) + strlen(sep);
    }
    char *out = js_alloc(J, total);
    if (out == NULL) return js_undefined();
    out[0] = '\0';
    for (int i = 0; i < a->len; i++) {
        if (i > 0) strcat(out, sep);
        if (a->items[i].type != JS_UNDEF && a->items[i].type != JS_NULL) {
            strcat(out, js_to_string(J, a->items[i]));
        }
    }
    return js_string(J, out);
}

static struct js_value fn_array_index_of(struct js *J, struct js_value self,
                                         int argc, struct js_value *argv) {
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return js_number(-1);
    struct js_value needle = arg(argc, argv, 0);
    for (int i = 0; i < a->len; i++) {
        if (js_equals(J, a->items[i], needle, 1)) return js_number(i);
    }
    return js_number(-1);
}

static struct js_value fn_for_each(struct js *J, struct js_value self,
                                   int argc, struct js_value *argv) {
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return js_undefined();
    struct js_value fn = arg(argc, argv, 0);
    for (int i = 0; i < a->len && J->signal == SIG_NONE; i++) {
        struct js_value args[3] = { a->items[i], js_number(i), self };
        js_call(J, fn, js_undefined(), 3, args);
    }
    return js_undefined();
}

static struct js_value fn_map(struct js *J, struct js_value self, int argc,
                              struct js_value *argv) {
    struct js_obj *a = self.obj;
    struct js_obj *out = js_new_array(J);
    if (self.type != JS_OBJ || a == NULL) return js_object(out);
    struct js_value fn = arg(argc, argv, 0);
    for (int i = 0; i < a->len && J->signal == SIG_NONE; i++) {
        struct js_value args[3] = { a->items[i], js_number(i), self };
        js_array_push(J, out, js_call(J, fn, js_undefined(), 3, args));
    }
    return js_object(out);
}

static struct js_value fn_filter(struct js *J, struct js_value self, int argc,
                                 struct js_value *argv) {
    struct js_obj *a = self.obj;
    struct js_obj *out = js_new_array(J);
    if (self.type != JS_OBJ || a == NULL) return js_object(out);
    struct js_value fn = arg(argc, argv, 0);
    for (int i = 0; i < a->len && J->signal == SIG_NONE; i++) {
        struct js_value args[3] = { a->items[i], js_number(i), self };
        if (js_truthy(js_call(J, fn, js_undefined(), 3, args))) {
            js_array_push(J, out, a->items[i]);
        }
    }
    return js_object(out);
}

static struct js_value fn_reduce(struct js *J, struct js_value self, int argc,
                                 struct js_value *argv) {
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return js_undefined();
    struct js_value fn = arg(argc, argv, 0);

    int i = 0;
    struct js_value acc;
    if (argc > 1) {
        acc = argv[1];
    } else {
        if (a->len == 0) return js_undefined();
        acc = a->items[0];
        i = 1;
    }
    for (; i < a->len && J->signal == SIG_NONE; i++) {
        struct js_value args[4] = { acc, a->items[i], js_number(i), self };
        acc = js_call(J, fn, js_undefined(), 4, args);
    }
    return acc;
}

static struct js_value fn_reverse(struct js *J, struct js_value self,
                                  int argc, struct js_value *argv) {
    (void)J;
    (void)argc;
    (void)argv;
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return self;
    for (int i = 0, k = a->len - 1; i < k; i++, k--) {
        struct js_value t = a->items[i];
        a->items[i] = a->items[k];
        a->items[k] = t;
    }
    return self;
}

static struct js_value fn_sort(struct js *J, struct js_value self, int argc,
                               struct js_value *argv) {
    struct js_obj *a = self.obj;
    if (self.type != JS_OBJ || a == NULL) return self;
    struct js_value cmp = arg(argc, argv, 0);
    int have_cmp = cmp.type == JS_OBJ;

    /* Insertion sort: an array a page sorts is a menu, not a
     * database. */
    for (int i = 1; i < a->len && J->signal == SIG_NONE; i++) {
        struct js_value v = a->items[i];
        int k = i - 1;
        while (k >= 0) {
            int greater;
            if (have_cmp) {
                struct js_value args[2] = { a->items[k], v };
                greater = js_to_number(J, js_call(J, cmp, js_undefined(), 2,
                                                  args)) > 0;
            } else {
                greater = strcmp(js_to_string(J, a->items[k]),
                                 js_to_string(J, v)) > 0;
            }
            if (!greater) break;
            a->items[k + 1] = a->items[k];
            k--;
        }
        a->items[k + 1] = v;
    }
    return self;
}

/* ---- numbers ---- */

static struct js_value fn_to_fixed(struct js *J, struct js_value self,
                                   int argc, struct js_value *argv) {
    int digits = argc > 0 ? (int)js_to_number(J, argv[0]) : 0;
    if (digits < 0) digits = 0;
    if (digits > 20) digits = 20;
    char buf[64];
    snprintf(buf, sizeof(buf), "%.*f", digits, js_to_number(J, self));
    return js_string(J, buf);
}

static struct js_value fn_to_string(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    if (self.type == JS_NUM && argc > 0) {
        int base = (int)js_to_number(J, argv[0]);
        if (base >= 2 && base <= 36 && base != 10) {
            char buf[80];
            int at = sizeof(buf) - 1;
            buf[at] = '\0';
            long long v = (long long)self.num;
            int negative = v < 0;
            if (negative) v = -v;
            if (v == 0) buf[--at] = '0';
            while (v > 0) {
                int d = (int)(v % base);
                buf[--at] = (char)(d < 10 ? '0' + d : 'a' + d - 10);
                v /= base;
            }
            if (negative) buf[--at] = '-';
            return js_string(J, buf + at);
        }
    }
    return js_string(J, js_to_string(J, self));
}

/* ---- the member tables ---- */

struct js_value string_member(struct js *J, struct js_value self,
                              const char *name) {
    if (strcmp(name, "length") == 0) {
        return js_number((double)(self.str != NULL ? self.str->len : 0));
    }
    int index;
    if (index_of_name(name, &index)) {
        const char *s = self.str != NULL ? self.str->text : "";
        if (index < (int)strlen(s)) return js_string_len(J, s + index, 1);
        return js_undefined();
    }
    return js_get(J, js_object(J->string_methods), name);
}

struct js_value array_member(struct js *J, struct js_obj *arr,
                             const char *name) {
    (void)arr;
    return js_get(J, js_object(J->array_methods), name);
}

struct js_value number_member(struct js *J, struct js_value self,
                              const char *name) {
    (void)self;
    return js_get(J, js_object(J->number_methods), name);
}

/* ---- regular expressions as objects ---- */

static struct js_value fn_regex_test(struct js *J, struct js_value self,
                                     int argc, struct js_value *argv) {
    if (self.type != JS_OBJ || self.obj == NULL || self.obj->regex == NULL) {
        return js_bool(0);
    }
    const char *text = js_to_string(J, arg(argc, argv, 0));
    int s, e, caps[RX_MAX_GROUPS * 2];
    return js_bool(rx_search(self.obj->regex, text, 0, &s, &e, caps));
}

static struct js_value fn_regex_exec(struct js *J, struct js_value self,
                                     int argc, struct js_value *argv) {
    if (self.type != JS_OBJ || self.obj == NULL || self.obj->regex == NULL) {
        return js_null();
    }
    const char *text = js_to_string(J, arg(argc, argv, 0));
    struct rx *rx = self.obj->regex;

    int from = 0;
    if (rx_is_global(rx)) {
        from = (int)js_to_number(J, js_get(J, self, "lastIndex"));
    }
    int s, e, caps[RX_MAX_GROUPS * 2];
    if (!rx_search(rx, text, from, &s, &e, caps)) {
        js_set(J, self, "lastIndex", js_number(0));
        return js_null();
    }
    if (rx_is_global(rx)) {
        js_set(J, self, "lastIndex", js_number(e > s ? e : e + 1));
    }

    struct js_obj *out = js_new_array(J);
    js_array_push(J, out, js_string_len(J, text + s, (size_t)(e - s)));
    for (int g = 1; g <= rx_groups(rx) && g < RX_MAX_GROUPS; g++) {
        js_array_push(J, out,
                      caps[g * 2] >= 0
                          ? js_string_len(J, text + caps[g * 2],
                                          (size_t)(caps[g * 2 + 1] -
                                                   caps[g * 2]))
                          : js_undefined());
    }
    js_def(J, out, "index", js_number(s));
    return js_object(out);
}

struct js_value js_new_regex(struct js *J, const char *source,
                             const char *flags) {
    struct js_obj *o = js_new_object(J, OBJ_PLAIN);
    if (o == NULL) return js_undefined();

    o->regex = rx_compile(source != NULL ? source : "",
                          flags != NULL ? flags : "");
    js_def(J, o, "source", js_string(J, source != NULL ? source : ""));
    js_def(J, o, "flags", js_string(J, flags != NULL ? flags : ""));
    js_def(J, o, "global", js_bool(rx_is_global(o->regex)));
    js_def(J, o, "lastIndex", js_number(0));
    js_def_native(J, o, "test", fn_regex_test);
    js_def_native(J, o, "exec", fn_regex_exec);

    if (o->regex == NULL) {
        /* An unsupported pattern is honest about itself rather than
         * matching everything. */
        js_def(J, o, "unsupported", js_bool(1));
    }
    return js_object(o);
}

/* ---- the global functions ---- */

static struct js_value fn_console_log(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    char line[512];
    size_t at = 0;
    line[0] = '\0';

    for (int i = 0; i < argc; i++) {
        const char *text = js_to_string(J, argv[i]);
        size_t len = strlen(text);
        if (at + len + 2 >= sizeof(line)) len = sizeof(line) - at - 2;
        if (i > 0 && at + 1 < sizeof(line)) line[at++] = ' ';
        memcpy(line + at, text, len);
        at += len;
        line[at] = '\0';
    }
    if (J->host.log != NULL) J->host.log(J->host.ctx, line);
    return js_undefined();
}

static struct js_value fn_alert(struct js *J, struct js_value self, int argc,
                                struct js_value *argv) {
    (void)self;
    const char *text = argc > 0 ? js_to_string(J, argv[0]) : "";
    if (J->host.alert != NULL) J->host.alert(J->host.ctx, text);
    else if (J->host.log != NULL) J->host.log(J->host.ctx, text);
    return js_undefined();
}

static struct js_value fn_parse_int(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    (void)self;
    const char *s = js_to_string(J, arg(argc, argv, 0));
    while (*s == ' ' || *s == '\t' || *s == '\n') s++;
    int base = argc > 1 ? (int)js_to_number(J, argv[1]) : 10;
    if (base == 0) base = 10;
    char *end = NULL;
    long long v = strtoll(s, &end, base);
    if (end == s) return js_number((double)0.0 / 0.0);
    return js_number((double)v);
}

static struct js_value fn_parse_float(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    const char *s = js_to_string(J, arg(argc, argv, 0));
    char *end = NULL;
    double v = strtod(s, &end);
    if (end == s) return js_number((double)0.0 / 0.0);
    return js_number(v);
}

static struct js_value fn_is_nan(struct js *J, struct js_value self, int argc,
                                 struct js_value *argv) {
    (void)self;
    double d = js_to_number(J, arg(argc, argv, 0));
    return js_bool(d != d);
}

static struct js_value fn_string_ctor(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    return js_string(J, argc > 0 ? js_to_string(J, argv[0]) : "");
}

static struct js_value fn_number_ctor(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    return js_number(argc > 0 ? js_to_number(J, argv[0]) : 0);
}

static struct js_value fn_boolean_ctor(struct js *J, struct js_value self,
                                       int argc, struct js_value *argv) {
    (void)J;
    (void)self;
    return js_bool(argc > 0 ? js_truthy(argv[0]) : 0);
}

static struct js_value fn_array_ctor(struct js *J, struct js_value self,
                                     int argc, struct js_value *argv) {
    (void)self;
    struct js_obj *a = js_new_array(J);
    if (argc == 1 && argv[0].type == JS_NUM) {
        int n = (int)argv[0].num;
        for (int i = 0; i < n && i < 100000; i++) {
            js_array_push(J, a, js_undefined());
        }
        return js_object(a);
    }
    for (int i = 0; i < argc; i++) js_array_push(J, a, argv[i]);
    return js_object(a);
}

static struct js_value fn_object_ctor(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    if (argc > 0 && argv[0].type == JS_OBJ) return argv[0];
    return js_object(js_new_object(J, OBJ_PLAIN));
}

static struct js_value fn_object_keys(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    struct js_obj *out = js_new_array(J);
    struct js_value src = arg(argc, argv, 0);
    if (src.type == JS_OBJ && src.obj != NULL) {
        if (src.obj->kind == OBJ_ARRAY) {
            for (int i = 0; i < src.obj->len; i++) {
                js_array_push(J, out, js_string(J, js_number_to_text(J, i)));
            }
        }
        for (struct js_prop *p = src.obj->props; p != NULL; p = p->next) {
            if (strcmp(p->name, "__proto__") == 0) continue;
            js_array_push(J, out, js_string(J, p->name));
        }
    }
    return js_object(out);
}

static struct js_value fn_object_values(struct js *J, struct js_value self,
                                        int argc, struct js_value *argv) {
    (void)self;
    struct js_obj *out = js_new_array(J);
    struct js_value src = arg(argc, argv, 0);
    if (src.type == JS_OBJ && src.obj != NULL) {
        if (src.obj->kind == OBJ_ARRAY) {
            for (int i = 0; i < src.obj->len; i++) {
                js_array_push(J, out, src.obj->items[i]);
            }
        }
        for (struct js_prop *p = src.obj->props; p != NULL; p = p->next) {
            if (strcmp(p->name, "__proto__") == 0) continue;
            js_array_push(J, out, p->value);
        }
    }
    return js_object(out);
}

static struct js_value fn_regexp_ctor(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    const char *source = argc > 0 ? js_to_string(J, argv[0]) : "";
    const char *flags = argc > 1 ? js_to_string(J, argv[1]) : "";
    if (argc > 0 && argv[0].type == JS_OBJ && argv[0].obj != NULL &&
        argv[0].obj->regex != NULL) {
        return argv[0];
    }
    return js_new_regex(J, source, flags);
}

/* Math, which is where a page's arithmetic beyond + and - lives. */
static double math_arg(struct js *J, int argc, struct js_value *argv) {
    return js_to_number(J, arg(argc, argv, 0));
}

static struct js_value fn_floor(struct js *J, struct js_value s, int c,
                                struct js_value *v) {
    (void)s;
    double d = math_arg(J, c, v);
    double f = (double)(long long)d;
    return js_number(d < 0 && f != d ? f - 1 : f);
}

static struct js_value fn_ceil(struct js *J, struct js_value s, int c,
                               struct js_value *v) {
    (void)s;
    double d = math_arg(J, c, v);
    double f = (double)(long long)d;
    return js_number(d > 0 && f != d ? f + 1 : f);
}

static struct js_value fn_round(struct js *J, struct js_value s, int c,
                                struct js_value *v) {
    (void)s;
    double d = math_arg(J, c, v);
    return js_number((double)(long long)(d + (d < 0 ? -0.5 : 0.5)));
}

static struct js_value fn_abs(struct js *J, struct js_value s, int c,
                              struct js_value *v) {
    (void)s;
    double d = math_arg(J, c, v);
    return js_number(d < 0 ? -d : d);
}

static struct js_value fn_sqrt(struct js *J, struct js_value s, int c,
                               struct js_value *v) {
    (void)s;
    double d = math_arg(J, c, v);
    if (d < 0) return js_number((double)0.0 / 0.0);
    /* Newton's method: the libm TUS links has no sqrt of its own. */
    double x = d > 1 ? d : 1;
    for (int i = 0; i < 40; i++) x = 0.5 * (x + d / x);
    return js_number(d == 0 ? 0 : x);
}

static struct js_value fn_pow(struct js *J, struct js_value s, int c,
                              struct js_value *v) {
    (void)s;
    return binary_op(J, "**", arg(c, v, 0), arg(c, v, 1));
}

static struct js_value fn_min(struct js *J, struct js_value s, int c,
                              struct js_value *v) {
    (void)s;
    if (c == 0) return js_number(js_infinity());
    double best = js_to_number(J, v[0]);
    for (int i = 1; i < c; i++) {
        double d = js_to_number(J, v[i]);
        if (d < best) best = d;
    }
    return js_number(best);
}

static struct js_value fn_max(struct js *J, struct js_value s, int c,
                              struct js_value *v) {
    (void)s;
    if (c == 0) return js_number(-js_infinity());
    double best = js_to_number(J, v[0]);
    for (int i = 1; i < c; i++) {
        double d = js_to_number(J, v[i]);
        if (d > best) best = d;
    }
    return js_number(best);
}

static struct js_value fn_random(struct js *J, struct js_value s, int c,
                                 struct js_value *v) {
    (void)s;
    (void)c;
    (void)v;
    /* A page's Math.random is for shuffling a banner, not for keys:
     * a small linear congruential sequence seeded from the clock. */
    static unsigned long long seed;
    if (seed == 0) {
        seed = J->host.now_ms != NULL ? J->host.now_ms(J->host.ctx) : 1;
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    }
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    return js_number((double)((seed >> 11) & 0xFFFFFFFFULL) / 4294967296.0);
}

static struct js_value fn_date_now(struct js *J, struct js_value s, int c,
                                   struct js_value *v) {
    (void)s;
    (void)c;
    (void)v;
    return js_number(J->host.now_ms != NULL
                         ? (double)J->host.now_ms(J->host.ctx) : 0);
}

static struct js_value fn_date_ctor(struct js *J, struct js_value self,
                                    int argc, struct js_value *argv) {
    (void)self;
    (void)argc;
    (void)argv;
    struct js_obj *o = js_new_object(J, OBJ_PLAIN);
    double now = J->host.now_ms != NULL ? (double)J->host.now_ms(J->host.ctx)
                                        : 0;
    js_def(J, o, "__time", js_number(now));
    js_def_native(J, o, "getTime", fn_date_now);
    js_def_native(J, o, "valueOf", fn_date_now);
    js_def_native(J, o, "toString", fn_to_string);
    return js_object(o);
}

/* ---- timers ---- */

static struct js_value fn_set_timeout(struct js *J, struct js_value self,
                                      int argc, struct js_value *argv) {
    (void)self;
    struct js_value fn = arg(argc, argv, 0);
    if (fn.type != JS_OBJ) return js_number(0);

    long delay = argc > 1 ? (long)js_to_number(J, argv[1]) : 0;
    if (delay < 0) delay = 0;
    if (delay > 60000) delay = 60000;

    for (int i = 0; i < JS_TIMER_MAX; i++) {
        if (J->timers[i].used) continue;
        J->timers[i].used = 1;
        J->timers[i].fn = fn;
        J->timers[i].due_ms = (J->host.now_ms != NULL
                                   ? J->host.now_ms(J->host.ctx) : 0) +
                              (unsigned long)delay;
        J->timers[i].interval = 0;
        return js_number(i + 1);
    }
    return js_number(0);
}

static struct js_value fn_clear_timeout(struct js *J, struct js_value self,
                                        int argc, struct js_value *argv) {
    (void)self;
    int id = (int)js_to_number(J, arg(argc, argv, 0));
    if (id >= 1 && id <= JS_TIMER_MAX) J->timers[id - 1].used = 0;
    return js_undefined();
}

int js_next_timer(struct js *J) {
    unsigned long now = J->host.now_ms != NULL ? J->host.now_ms(J->host.ctx)
                                               : 0;
    long best = -1;
    for (int i = 0; i < JS_TIMER_MAX; i++) {
        if (!J->timers[i].used) continue;
        long wait = (long)J->timers[i].due_ms - (long)now;
        if (wait < 0) wait = 0;
        if (best < 0 || wait < best) best = wait;
    }
    return (int)best;
}

int js_run_timers(struct js *J) {
    unsigned long now = J->host.now_ms != NULL ? J->host.now_ms(J->host.ctx)
                                               : 0;
    int ran = 0;
    for (int i = 0; i < JS_TIMER_MAX; i++) {
        if (!J->timers[i].used || J->timers[i].due_ms > now) continue;
        struct js_value fn = J->timers[i].fn;
        J->timers[i].used = 0;

        J->signal = SIG_NONE;
        J->steps = 0;
        js_call(J, fn, js_object(J->window), 0, NULL);
        ran++;
        if ((J->signal == SIG_THROW || J->signal == SIG_ERROR) &&
            J->host.script_error != NULL) {
            J->host.script_error(J->host.ctx, J->error);
        }
        J->signal = SIG_NONE;
    }
    if (ran > 0 && J->host.changed != NULL) J->host.changed(J->host.ctx);
    return ran;
}

/* ---- building the global object ---- */

static void install_globals(struct js *J) {
    struct js_obj *w = J->window;

    /* The methods of the primitive types. */
    struct js_obj *s = J->string_methods;
    js_def_native(J, s, "charAt", fn_char_at);
    js_def_native(J, s, "charCodeAt", fn_char_code);
    js_def_native(J, s, "indexOf", fn_index_of);
    js_def_native(J, s, "lastIndexOf", fn_last_index_of);
    js_def_native(J, s, "slice", fn_slice);
    js_def_native(J, s, "substring", fn_substring);
    js_def_native(J, s, "substr", fn_substr);
    js_def_native(J, s, "toUpperCase", fn_to_upper);
    js_def_native(J, s, "toLowerCase", fn_to_lower);
    js_def_native(J, s, "trim", fn_trim);
    js_def_native(J, s, "includes", fn_includes);
    js_def_native(J, s, "startsWith", fn_starts_with);
    js_def_native(J, s, "endsWith", fn_ends_with);
    js_def_native(J, s, "repeat", fn_repeat);
    js_def_native(J, s, "concat", fn_concat);
    js_def_native(J, s, "split", fn_split);
    js_def_native(J, s, "replace", fn_replace);
    js_def_native(J, s, "replaceAll", fn_replace);
    js_def_native(J, s, "match", fn_match);
    js_def_native(J, s, "search", fn_search);
    js_def_native(J, s, "toString", fn_to_string);

    struct js_obj *a = J->array_methods;
    js_def_native(J, a, "push", fn_push);
    js_def_native(J, a, "pop", fn_pop);
    js_def_native(J, a, "shift", fn_shift);
    js_def_native(J, a, "unshift", fn_unshift);
    js_def_native(J, a, "slice", fn_array_slice);
    js_def_native(J, a, "splice", fn_splice);
    js_def_native(J, a, "join", fn_join);
    js_def_native(J, a, "indexOf", fn_array_index_of);
    js_def_native(J, a, "includes", fn_includes);
    js_def_native(J, a, "forEach", fn_for_each);
    js_def_native(J, a, "map", fn_map);
    js_def_native(J, a, "filter", fn_filter);
    js_def_native(J, a, "reduce", fn_reduce);
    js_def_native(J, a, "reverse", fn_reverse);
    js_def_native(J, a, "sort", fn_sort);
    js_def_native(J, a, "concat", fn_concat);
    js_def_native(J, a, "toString", fn_to_string);

    struct js_obj *n = J->number_methods;
    js_def_native(J, n, "toFixed", fn_to_fixed);
    js_def_native(J, n, "toString", fn_to_string);

    /* console */
    struct js_obj *console = js_new_object(J, OBJ_PLAIN);
    js_def_native(J, console, "log", fn_console_log);
    js_def_native(J, console, "warn", fn_console_log);
    js_def_native(J, console, "error", fn_console_log);
    js_def_native(J, console, "info", fn_console_log);
    js_def(J, w, "console", js_object(console));

    /* Math */
    struct js_obj *math = js_new_object(J, OBJ_PLAIN);
    js_def_native(J, math, "floor", fn_floor);
    js_def_native(J, math, "ceil", fn_ceil);
    js_def_native(J, math, "round", fn_round);
    js_def_native(J, math, "abs", fn_abs);
    js_def_native(J, math, "sqrt", fn_sqrt);
    js_def_native(J, math, "pow", fn_pow);
    js_def_native(J, math, "min", fn_min);
    js_def_native(J, math, "max", fn_max);
    js_def_native(J, math, "random", fn_random);
    js_def(J, math, "PI", js_number(3.141592653589793));
    js_def(J, math, "E", js_number(2.718281828459045));
    js_def(J, w, "Math", js_object(math));

    /* Date, enough of it for a page that stamps something. */
    struct js_obj *date = js_new_native(J, "Date", fn_date_ctor);
    js_def_native(J, date, "now", fn_date_now);
    js_def(J, w, "Date", js_object(date));

    struct js_obj *object_ctor = js_new_native(J, "Object", fn_object_ctor);
    js_def_native(J, object_ctor, "keys", fn_object_keys);
    js_def_native(J, object_ctor, "values", fn_object_values);
    js_def(J, w, "Object", js_object(object_ctor));

    js_def(J, w, "String", js_object(js_new_native(J, "String",
                                                   fn_string_ctor)));
    js_def(J, w, "Number", js_object(js_new_native(J, "Number",
                                                   fn_number_ctor)));
    js_def(J, w, "Boolean", js_object(js_new_native(J, "Boolean",
                                                    fn_boolean_ctor)));
    js_def(J, w, "Array", js_object(js_new_native(J, "Array", fn_array_ctor)));
    js_def(J, w, "RegExp", js_object(js_new_native(J, "RegExp",
                                                   fn_regexp_ctor)));

    js_def_native(J, w, "parseInt", fn_parse_int);
    js_def_native(J, w, "parseFloat", fn_parse_float);
    js_def_native(J, w, "isNaN", fn_is_nan);
    js_def_native(J, w, "alert", fn_alert);
    js_def_native(J, w, "setTimeout", fn_set_timeout);
    js_def_native(J, w, "setInterval", fn_set_timeout);
    js_def_native(J, w, "clearTimeout", fn_clear_timeout);
    js_def_native(J, w, "clearInterval", fn_clear_timeout);
    js_def(J, w, "NaN", js_number((double)0.0 / 0.0));
    js_def(J, w, "Infinity", js_number(js_infinity()));
    js_def(J, w, "undefined", js_undefined());
}

/* ---- the public half ---- */

struct js *js_create(struct dom_node *document, const struct js_host *host) {
    struct js *J = calloc(1, sizeof(*J));
    if (J == NULL) return NULL;

    J->document = document;
    if (host != NULL) J->host = *host;
    J->alloc_limit = JS_ALLOC_LIMIT;
    J->step_limit = JS_STEP_LIMIT;
    snprintf(J->error, sizeof(J->error), "%s", "");

    J->global = env_new(J, NULL);
    J->window = js_new_object(J, OBJ_PLAIN);
    J->string_methods = js_new_object(J, OBJ_PLAIN);
    J->array_methods = js_new_object(J, OBJ_PLAIN);
    J->number_methods = js_new_object(J, OBJ_PLAIN);

    install_globals(J);
    jsdom_install(J);

    env_define(J, J->global, "this", js_object(J->window));
    return J;
}

void js_free(struct js *J) {
    if (J == NULL) return;

    for (struct js_obj *o = J->objects; o != NULL; o = o->next_alloc) {
        free(o->items);
        free(o->nodes);
        rx_free(o->regex);
    }
    /* The allocations themselves were made with calloc through
     * js_alloc, so they are freed here in the order they were
     * chained. */
    struct js_obj *o = J->objects;
    while (o != NULL) {
        struct js_obj *next = o->next_alloc;
        free(o);
        o = next;
    }
    struct js_env *e = J->envs;
    while (e != NULL) {
        struct js_env *next = e->next_alloc;
        free(e);
        e = next;
    }
    for (int i = 0; i < J->nstrings; i++) {
        if (J->strings[i] != NULL) free(J->strings[i]->text);
        free(J->strings[i]);
    }
    free(J->strings);
    for (int i = 0; i < J->ntexts; i++) free(J->texts[i]);
    free(J->texts);

    struct ast *n = J->nodes_head;
    while (n != NULL) {
        struct ast *next = n->alloc_next;
        free(n);
        n = next;
    }
    free(J);
}

int js_run(struct js *J, const char *source, struct dom_node *element) {
    if (J == NULL || source == NULL) return -1;

    struct lexer L;
    memset(&L, 0, sizeof(L));
    L.J = J;
    L.src = source;
    L.len = strlen(source);
    L.line = 1;

    J->signal = SIG_NONE;
    J->error[0] = '\0';
    J->steps = 0;
    J->depth = 0;

    lex_all(&L);
    struct ast *program = parse_program(J, L.tokens, L.ntokens);
    free(L.tokens);

    if (J->signal != SIG_NONE) return -1;

    struct js_env *env = J->global;
    if (element != NULL) {
        /* An attribute handler runs with the element as `this` and
         * with `event` in scope. */
        env = env_new(J, J->global);
        env_define(J, env, "this", js_object(js_new_node(J, element)));
    }

    exec_list(J, env, program);

    if (J->signal == SIG_THROW) {
        snprintf(J->error, sizeof(J->error), "%s",
                 js_to_string(J, J->signal_value));
    }
    int failed = J->signal == SIG_THROW || J->signal == SIG_ERROR;
    J->signal = SIG_NONE;
    return failed ? -1 : 0;
}

int js_run_document(struct js *J) {
    if (J == NULL || J->document == NULL) return 0;
    int ran = 0;

    for (struct dom_node *n = J->document; n != NULL;
         n = dom_next(n, J->document)) {
        if (n->type != DOM_ELEMENT || strcmp(n->name, "script") != 0) continue;

        const char *type = dom_attr(n, "type");
        if (type != NULL && strstr(type, "javascript") == NULL &&
            strstr(type, "module") == NULL && *type != '\0') {
            continue;    /* JSON-LD and friends are data, not code */
        }

        const char *src = dom_attr(n, "src");
        char *fetched = NULL;
        const char *code;
        if (src != NULL) {
            if (J->host.fetch_script == NULL) continue;
            fetched = J->host.fetch_script(J->host.ctx, src);
            if (fetched == NULL) continue;   /* this script only, not the page */
            code = fetched;
        } else {
            if (n->first == NULL || n->first->text == NULL) continue;
            code = n->first->text;
        }

        if (js_run(J, code, NULL) != 0) {
            if (J->host.script_error != NULL) {
                J->host.script_error(J->host.ctx, J->error);
            } else if (J->host.log != NULL) {
                J->host.log(J->host.ctx, J->error);
            }
        }
        free(fetched);
        ran++;
    }

    if (ran > 0 && J->host.changed != NULL) J->host.changed(J->host.ctx);
    return ran;
}

int js_run_handler(struct js *J, struct dom_node *element, const char *source,
                   struct js_value event) {
    if (J == NULL || source == NULL) return 0;

    struct lexer L;
    memset(&L, 0, sizeof(L));
    L.J = J;
    L.src = source;
    L.len = strlen(source);
    L.line = 1;

    J->signal = SIG_NONE;
    J->error[0] = '\0';
    J->steps = 0;
    J->depth = 0;

    lex_all(&L);
    struct ast *program = parse_program(J, L.tokens, L.ntokens);
    free(L.tokens);

    struct js_env *env = env_new(J, J->global);
    env_define(J, env, "this", js_object(js_new_node(J, element)));
    env_define(J, env, "event", event);

    exec_list(J, env, program);

    int cancelled = 0;
    if (J->signal == SIG_RETURN) {
        cancelled = !js_truthy(J->signal_value);
    } else if (J->signal == SIG_THROW || J->signal == SIG_ERROR) {
        if (J->host.script_error != NULL) {
            J->host.script_error(J->host.ctx, J->error);
        }
    }
    J->signal = SIG_NONE;
    return cancelled;
}
