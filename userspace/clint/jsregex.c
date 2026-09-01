/*
 * jsregex.c - see jsregex.h
 *
 * The pattern is parsed into a tree first, because backtracking over
 * the pattern text directly cannot express alternation without
 * re-scanning it. Each node knows what comes after it (`next`), so
 * matching is "match me, then match the rest" - which is exactly the
 * shape backtracking wants: a quantifier tries a count, asks the rest
 * of the pattern, and takes the count back if the rest says no.
 */

#include "jsregex.h"

#include <stdlib.h>
#include <string.h>

enum {
    RX_CHAR = 1, RX_ANY, RX_CLASS, RX_GROUP, RX_ALT, RX_REP, RX_BOL, RX_EOL,
    RX_WORD_BOUNDARY, RX_BACKREF
};

struct rxnode {
    unsigned char type;
    unsigned char neg;        /* RX_CLASS: the class is negated */
    unsigned char lazy;       /* RX_REP: as few as possible */
    int ch;                   /* RX_CHAR */
    unsigned char set[32];    /* RX_CLASS: one bit per byte */
    int min, max;             /* RX_REP; max < 0 means no limit */
    int group;                /* RX_GROUP: capture index, 0 = none */
    struct rxnode *child;     /* RX_GROUP/RX_REP */
    struct rxnode *alt;       /* RX_ALT: the other branch */
    struct rxnode *next;
    struct rxnode *alloc_next;
};

struct rx {
    struct rxnode *root;
    struct rxnode *all;       /* every node, for freeing */
    int ngroups;
    int global, icase, multiline;
};

struct parser_state {
    struct rx *rx;
    const char *p;
    int ok;
};

static struct rxnode *rx_node(struct parser_state *S, int type) {
    struct rxnode *n = calloc(1, sizeof(*n));
    if (n == NULL) {
        S->ok = 0;
        return NULL;
    }
    n->type = (unsigned char)type;
    n->alloc_next = S->rx->all;
    S->rx->all = n;
    return n;
}

static void set_add(unsigned char *set, int c) {
    set[(c & 0xFF) >> 3] |= (unsigned char)(1u << (c & 7));
}

static int set_has(const unsigned char *set, int c) {
    return (set[(c & 0xFF) >> 3] >> (c & 7)) & 1;
}

/* \d \w \s and their negations, as bit sets. */
static void set_add_class(unsigned char *set, char kind) {
    switch (kind) {
    case 'd':
        for (int c = '0'; c <= '9'; c++) set_add(set, c);
        break;
    case 'w':
        for (int c = '0'; c <= '9'; c++) set_add(set, c);
        for (int c = 'a'; c <= 'z'; c++) set_add(set, c);
        for (int c = 'A'; c <= 'Z'; c++) set_add(set, c);
        set_add(set, '_');
        break;
    case 's':
        set_add(set, ' ');
        set_add(set, '\t');
        set_add(set, '\n');
        set_add(set, '\r');
        set_add(set, '\f');
        set_add(set, '\v');
        break;
    default:
        break;
    }
}

static int escape_char(char c) {
    switch (c) {
    case 'n': return '\n';
    case 't': return '\t';
    case 'r': return '\r';
    case 'f': return '\f';
    case 'v': return '\v';
    case '0': return '\0';
    default:  return (unsigned char)c;
    }
}

static struct rxnode *parse_alt(struct parser_state *S);

static struct rxnode *parse_atom(struct parser_state *S) {
    char c = *S->p;

    if (c == '(') {
        S->p++;
        int group = 0;
        if (S->p[0] == '?' && (S->p[1] == ':' || S->p[1] == '=' ||
                               S->p[1] == '!')) {
            /* Lookarounds are not supported; treat them as a plain
             * non-capturing group, which is wrong but recoverable. */
            S->p += 2;
        } else {
            group = ++S->rx->ngroups;
            if (group >= RX_MAX_GROUPS) group = 0;
        }
        struct rxnode *n = rx_node(S, RX_GROUP);
        if (n == NULL) return NULL;
        n->group = group;
        n->child = parse_alt(S);
        if (*S->p == ')') S->p++;
        else S->ok = 0;
        return n;
    }

    if (c == '[') {
        S->p++;
        struct rxnode *n = rx_node(S, RX_CLASS);
        if (n == NULL) return NULL;
        if (*S->p == '^') {
            n->neg = 1;
            S->p++;
        }
        while (*S->p != '\0' && *S->p != ']') {
            int lo;
            if (*S->p == '\\') {
                S->p++;
                char e = *S->p++;
                if (e == 'd' || e == 'w' || e == 's') {
                    set_add_class(n->set, e);
                    continue;
                }
                if (e == 'D' || e == 'W' || e == 'S') {
                    unsigned char tmp[32];
                    memset(tmp, 0, sizeof(tmp));
                    set_add_class(tmp, (char)(e | 32));
                    for (int i = 0; i < 256; i++) {
                        if (!set_has(tmp, i)) set_add(n->set, i);
                    }
                    continue;
                }
                lo = escape_char(e);
            } else {
                lo = (unsigned char)*S->p++;
            }

            if (S->p[0] == '-' && S->p[1] != ']' && S->p[1] != '\0') {
                S->p++;
                int hi;
                if (*S->p == '\\') {
                    S->p++;
                    hi = escape_char(*S->p++);
                } else {
                    hi = (unsigned char)*S->p++;
                }
                for (int i = lo; i <= hi; i++) set_add(n->set, i);
            } else {
                set_add(n->set, lo);
            }
        }
        if (*S->p == ']') S->p++;
        return n;
    }

    if (c == '.') {
        S->p++;
        return rx_node(S, RX_ANY);
    }
    if (c == '^') {
        S->p++;
        return rx_node(S, RX_BOL);
    }
    if (c == '$') {
        S->p++;
        return rx_node(S, RX_EOL);
    }

    if (c == '\\') {
        S->p++;
        char e = *S->p++;
        if (e == 'd' || e == 'w' || e == 's' || e == 'D' || e == 'W' ||
            e == 'S') {
            struct rxnode *n = rx_node(S, RX_CLASS);
            if (n == NULL) return NULL;
            if (e >= 'A' && e <= 'Z') {
                n->neg = 1;
                set_add_class(n->set, (char)(e | 32));
            } else {
                set_add_class(n->set, e);
            }
            return n;
        }
        if (e == 'b' || e == 'B') {
            struct rxnode *n = rx_node(S, RX_WORD_BOUNDARY);
            if (n == NULL) return NULL;
            n->neg = (unsigned char)(e == 'B');
            return n;
        }
        if (e >= '1' && e <= '9') {
            struct rxnode *n = rx_node(S, RX_BACKREF);
            if (n == NULL) return NULL;
            n->group = e - '0';
            return n;
        }
        struct rxnode *n = rx_node(S, RX_CHAR);
        if (n == NULL) return NULL;
        n->ch = escape_char(e);
        return n;
    }

    S->p++;
    struct rxnode *n = rx_node(S, RX_CHAR);
    if (n == NULL) return NULL;
    n->ch = (unsigned char)c;
    return n;
}

static struct rxnode *parse_piece(struct parser_state *S) {
    struct rxnode *atom = parse_atom(S);
    if (atom == NULL) return NULL;

    int min = -1, max = -1;
    char c = *S->p;

    if (c == '*') { min = 0; max = -1; S->p++; }
    else if (c == '+') { min = 1; max = -1; S->p++; }
    else if (c == '?') { min = 0; max = 1; S->p++; }
    else if (c == '{') {
        const char *save = S->p;
        S->p++;
        int lo = 0, hi = -1, digits = 0;
        while (*S->p >= '0' && *S->p <= '9') {
            lo = lo * 10 + (*S->p++ - '0');
            digits++;
        }
        if (digits == 0) {
            S->p = save;
        } else {
            hi = lo;
            if (*S->p == ',') {
                S->p++;
                if (*S->p >= '0' && *S->p <= '9') {
                    hi = 0;
                    while (*S->p >= '0' && *S->p <= '9') {
                        hi = hi * 10 + (*S->p++ - '0');
                    }
                } else {
                    hi = -1;
                }
            }
            if (*S->p == '}') {
                S->p++;
                min = lo;
                max = hi;
            } else {
                S->p = save;
            }
        }
    }

    if (min < 0) return atom;

    struct rxnode *rep = rx_node(S, RX_REP);
    if (rep == NULL) return NULL;
    rep->child = atom;
    rep->min = min;
    rep->max = max;
    if (*S->p == '?') {
        rep->lazy = 1;
        S->p++;
    }
    return rep;
}

static struct rxnode *parse_seq(struct parser_state *S) {
    struct rxnode *first = NULL, **tail = &first;
    while (*S->p != '\0' && *S->p != '|' && *S->p != ')' && S->ok) {
        struct rxnode *piece = parse_piece(S);
        if (piece == NULL) break;
        *tail = piece;
        tail = &piece->next;
    }
    return first;
}

static struct rxnode *parse_alt(struct parser_state *S) {
    struct rxnode *left = parse_seq(S);
    if (*S->p != '|') return left;

    struct rxnode *alt = rx_node(S, RX_ALT);
    if (alt == NULL) return left;
    alt->child = left;
    S->p++;
    alt->alt = parse_alt(S);
    return alt;
}

struct rx *rx_compile(const char *pattern, const char *flags) {
    if (pattern == NULL) return NULL;

    struct rx *rx = calloc(1, sizeof(*rx));
    if (rx == NULL) return NULL;

    for (const char *f = flags != NULL ? flags : ""; *f != '\0'; f++) {
        if (*f == 'g') rx->global = 1;
        else if (*f == 'i') rx->icase = 1;
        else if (*f == 'm') rx->multiline = 1;
    }

    struct parser_state S;
    S.rx = rx;
    S.p = pattern;
    S.ok = 1;
    rx->root = parse_alt(&S);

    if (!S.ok || *S.p != '\0') {
        rx_free(rx);
        return NULL;
    }
    return rx;
}

void rx_free(struct rx *rx) {
    if (rx == NULL) return;
    struct rxnode *n = rx->all;
    while (n != NULL) {
        struct rxnode *next = n->alloc_next;
        free(n);
        n = next;
    }
    free(rx);
}

int rx_groups(const struct rx *rx) { return rx != NULL ? rx->ngroups : 0; }
int rx_is_global(const struct rx *rx) { return rx != NULL && rx->global; }
int rx_is_icase(const struct rx *rx) { return rx != NULL && rx->icase; }

/* ---- matching ---- */

struct rxctx {
    struct rx *rx;
    const char *text;
    int len;
    int caps[RX_MAX_GROUPS * 2];
    long steps;
};

static int fold(struct rxctx *C, int c) {
    if (!C->rx->icase) return c;
    return (c >= 'A' && c <= 'Z') ? c + 32 : c;
}

static int is_word(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_';
}

static int match_node(struct rxctx *C, struct rxnode *n, int pos);
static int close_group(struct rxctx *C, struct rxnode *n, int pos);

/* The rest of the pattern after `n`, which is what every node has to
 * hand control to once it has matched itself. */
static int match_next(struct rxctx *C, struct rxnode *n, int pos) {
    return n->next != NULL ? match_node(C, n->next, pos) : pos;
}

static int match_rep(struct rxctx *C, struct rxnode *n, int pos, int count) {
    if (++C->steps > 2000000) return -1;

    int can_more = n->max < 0 || count < n->max;

    if (n->lazy && count >= n->min) {
        int r = match_next(C, n, pos);
        if (r >= 0) return r;
    }

    if (can_more) {
        /* One more repetition, then the rest - and if that fails,
         * fall through to stopping here. */
        struct rxnode *child = n->child;
        struct rxnode *saved_next = child->next;
        child->next = NULL;

        int caps_saved[RX_MAX_GROUPS * 2];
        memcpy(caps_saved, C->caps, sizeof(caps_saved));

        int after = match_node(C, child, pos);
        child->next = saved_next;

        if (after >= 0 && (after > pos || count < n->min)) {
            int r = match_rep(C, n, after, count + 1);
            if (r >= 0) return r;
        }
        memcpy(C->caps, caps_saved, sizeof(caps_saved));
    }

    if (!n->lazy && count >= n->min) return match_next(C, n, pos);
    return -1;
}

static int match_node(struct rxctx *C, struct rxnode *n, int pos) {
    if (n == NULL) return pos;
    if (++C->steps > 2000000) return -1;

    switch (n->type) {
    case RX_CHAR:
        if (pos < C->len && fold(C, (unsigned char)C->text[pos]) ==
                                fold(C, n->ch)) {
            return match_next(C, n, pos + 1);
        }
        return -1;

    case RX_ANY:
        if (pos < C->len && C->text[pos] != '\n') {
            return match_next(C, n, pos + 1);
        }
        return -1;

    case RX_CLASS: {
        if (pos >= C->len) return -1;
        int c = (unsigned char)C->text[pos];
        int in = set_has(n->set, c);
        if (!in && C->rx->icase) {
            int other = (c >= 'a' && c <= 'z') ? c - 32
                      : (c >= 'A' && c <= 'Z') ? c + 32 : c;
            in = set_has(n->set, other);
        }
        if (n->neg) in = !in;
        return in ? match_next(C, n, pos + 1) : -1;
    }

    case RX_BOL:
        if (pos == 0 || (C->rx->multiline && C->text[pos - 1] == '\n')) {
            return match_next(C, n, pos);
        }
        return -1;

    case RX_EOL:
        if (pos == C->len ||
            (C->rx->multiline && C->text[pos] == '\n')) {
            return match_next(C, n, pos);
        }
        return -1;

    case RX_WORD_BOUNDARY: {
        int before = pos > 0 && is_word((unsigned char)C->text[pos - 1]);
        int after = pos < C->len && is_word((unsigned char)C->text[pos]);
        int boundary = before != after;
        if (n->neg) boundary = !boundary;
        return boundary ? match_next(C, n, pos) : -1;
    }

    case RX_BACKREF: {
        int g = n->group;
        if (g >= RX_MAX_GROUPS || C->caps[g * 2] < 0) return match_next(C, n, pos);
        int start = C->caps[g * 2], end = C->caps[g * 2 + 1];
        int len = end - start;
        if (pos + len > C->len) return -1;
        for (int i = 0; i < len; i++) {
            if (fold(C, (unsigned char)C->text[pos + i]) !=
                fold(C, (unsigned char)C->text[start + i])) {
                return -1;
            }
        }
        return match_next(C, n, pos + len);
    }

    case RX_GROUP: {
        /* The marker a group leaves behind to record where it ended. */
        if (n->child == NULL && n->group < 0) return close_group(C, n, pos);

        /* The group's contents, then whatever follows the group: the
         * child list is temporarily terminated so the recursion knows
         * where the group ends. */
        int saved_start = n->group > 0 ? C->caps[n->group * 2] : 0;
        int saved_end = n->group > 0 ? C->caps[n->group * 2 + 1] : 0;

        struct rxnode *child = n->child;
        if (child == NULL) return match_next(C, n, pos);

        /* Walk the child list to its end and attach the rest there. */
        struct rxnode *last = child;
        while (last->next != NULL) last = last->next;
        struct rxnode *saved = last->next;

        struct rxnode tail;
        memset(&tail, 0, sizeof(tail));
        tail.type = RX_GROUP;
        tail.group = -n->group;      /* a marker: close this group */
        tail.next = n->next;
        tail.child = NULL;
        last->next = &tail;

        if (n->group > 0) C->caps[n->group * 2] = pos;
        int r = match_node(C, child, pos);
        last->next = saved;

        if (r < 0 && n->group > 0) {
            C->caps[n->group * 2] = saved_start;
            C->caps[n->group * 2 + 1] = saved_end;
        }
        return r;
    }

    case RX_ALT: {
        int caps_saved[RX_MAX_GROUPS * 2];
        memcpy(caps_saved, C->caps, sizeof(caps_saved));

        if (n->child != NULL) {
            struct rxnode *last = n->child;
            while (last->next != NULL) last = last->next;
            struct rxnode *saved = last->next;
            last->next = n->next;
            int r = match_node(C, n->child, pos);
            last->next = saved;
            if (r >= 0) return r;
        }
        memcpy(C->caps, caps_saved, sizeof(caps_saved));

        if (n->alt != NULL) {
            struct rxnode *last = n->alt;
            while (last->next != NULL) last = last->next;
            struct rxnode *saved = last->next;
            last->next = n->next;
            int r = match_node(C, n->alt, pos);
            last->next = saved;
            if (r >= 0) return r;
        }
        memcpy(C->caps, caps_saved, sizeof(caps_saved));
        return -1;
    }

    case RX_REP:
        return match_rep(C, n, pos, 0);

    default:
        return -1;
    }
}

/* The close marker a group pushes in front of the rest of the
 * pattern; it records where the group ended and carries on. */
static int close_group(struct rxctx *C, struct rxnode *n, int pos) {
    int g = -n->group;
    if (g > 0 && g < RX_MAX_GROUPS) C->caps[g * 2 + 1] = pos;
    return n->next != NULL ? match_node(C, n->next, pos) : pos;
}

int rx_search(struct rx *rx, const char *text, int start, int *mstart,
              int *mend, int *caps) {
    if (rx == NULL || text == NULL) return 0;

    struct rxctx C;
    C.rx = rx;
    C.text = text;
    C.len = (int)strlen(text);
    C.steps = 0;

    if (start < 0) start = 0;

    for (int at = start; at <= C.len; at++) {
        for (int i = 0; i < RX_MAX_GROUPS * 2; i++) C.caps[i] = -1;
        C.steps = 0;

        int end = match_node(&C, rx->root, at);
        if (end >= 0) {
            *mstart = at;
            *mend = end;
            if (caps != NULL) {
                memcpy(caps, C.caps, sizeof(C.caps));
            }
            return 1;
        }
    }
    return 0;
}
