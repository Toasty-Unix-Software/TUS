/*
 * highx.h - highX ("High Window System") protocol definition, v1.0
 *
 * highX is the TUS window system. Like X11 it splits the world into a
 * display server (here: an in-kernel compositor, kernel/highx/) and
 * clients that speak a request/event protocol to it. Unlike X11 the
 * transport is not a socket but a single system call - SYS_HIGHX -
 * which carries one request structure at a time:
 *
 *     long highx(int op, void *arg, unsigned long len);
 *
 * `op` selects the request (HX_OP_*), `arg` points at the matching
 * request structure and `len` is its size, which the server checks
 * before touching a byte. Requests that carry a result (window ids,
 * events, the window tree) write it back into the same structure, so
 * every round trip is one call - there is no reply stream to parse
 * and no byte order to negotiate.
 *
 * This header is the whole ABI: the kernel server and the userspace
 * library (highAPI, userspace/highapi/) include exactly this file.
 * Nothing else is shared between them.
 *
 * Coordinates: window contents use window-local coordinates with the
 * origin at the top left; window positions are screen coordinates.
 * Colors are 0x00RRGGBB (the top byte is ignored - highX has no
 * alpha blending yet).
 */

#ifndef HIGHX_H
#define HIGHX_H

#include <stdint.h>

/* ---- protocol identity ---- */

#define HIGHX_MAGIC        0x58484748u /* 'HGHX' */
#define HIGHX_VERSION_MAJOR 1
#define HIGHX_VERSION_MINOR 5
#define HIGHX_VERSION      ((HIGHX_VERSION_MAJOR << 16) | HIGHX_VERSION_MINOR)

/* ---- server limits (a client may rely on these) ---- */

#define HX_MAX_WINDOWS  32
#define HX_MAX_CLIENTS  16
/* tusWM alone grabs 22 (Ctrl+Q/W/N/T/L/R/M, Tab, six Super combos and
 * four arrow keys x two modifier combos each) - 16 silently dropped
 * the ones registered after the limit, with no error either side
 * could see: Alt+Right/Up/Down and their Shift variants simply never
 * fired. Grown with headroom for a window manager to add a few more
 * without hitting this again quietly. */
#define HX_MAX_GRABS    32
#define HX_TITLE_MAX    48
#define HX_TEXT_MAX     128
#define HX_EVENT_QUEUE  64

/* ---- requests ---- */

enum hx_opcode {
    HX_OP_HELLO = 1,       /* struct hx_hello        - open the display */
    HX_OP_BYE,             /* (no argument)          - close the display */
    HX_OP_CREATE_WINDOW,   /* struct hx_window_req   - out: .win */
    HX_OP_DESTROY_WINDOW,  /* struct hx_win_op */
    HX_OP_MAP_WINDOW,      /* struct hx_win_op       - WM may redirect */
    HX_OP_UNMAP_WINDOW,    /* struct hx_win_op */
    HX_OP_CONFIGURE_WINDOW,/* struct hx_configure */
    HX_OP_RAISE_WINDOW,    /* struct hx_win_op */
    HX_OP_LOWER_WINDOW,    /* struct hx_win_op */
    HX_OP_SET_TITLE,       /* struct hx_title */
    HX_OP_SET_FOCUS,       /* struct hx_win_op */
    HX_OP_FILL_RECT,       /* struct hx_rect_req */
    HX_OP_DRAW_RECT,       /* struct hx_rect_req     - outline */
    HX_OP_DRAW_LINE,       /* struct hx_line_req */
    HX_OP_DRAW_TEXT,       /* struct hx_text_req */
    HX_OP_PUT_IMAGE,       /* struct hx_image_req */
    HX_OP_COMMIT,          /* struct hx_commit       - damage -> screen */
    HX_OP_NEXT_EVENT,      /* struct hx_next_event */
    HX_OP_QUERY_TREE,      /* struct hx_tree         - window manager */
    HX_OP_GET_WINDOW,      /* struct hx_window_info */
    HX_OP_WM_REGISTER,     /* (no argument)          - become the WM */
    HX_OP_GRAB_KEY,        /* struct hx_grab         - WM hotkeys */
    HX_OP_CLOSE_WINDOW,    /* struct hx_win_op       - ask owner to quit */
    HX_OP_SCREEN_INFO,     /* struct hx_screen_info */
    HX_OP_SET_BACKGROUND,  /* struct hx_background */
    HX_OP_SET_BORDER,      /* struct hx_border      - v1.1 */
    HX_OP_QUERY_POINTER,   /* struct hx_pointer     - v1.2 */
    HX_OP_GRAB_POINTER,    /* struct hx_win_op      - v1.2 */
    HX_OP_WARP_POINTER,    /* struct hx_pointer     - v1.2 */
    HX_OP_BATCH,           /* struct hx_batch       - v1.3 */
    HX_OP_MAX
};

/* Window flags (hx_window_req.flags, hx_window_info.flags). */
#define HX_WF_NODECOR   0x0001u /* hint: the WM should not decorate this */
#define HX_WF_FRAME     0x0002u /* a WM-owned decoration window */
#define HX_WF_DESKTOP   0x0004u /* stays at the bottom of the stack */
#define HX_WF_OVERRIDE  0x0008u /* bypass the WM's map redirection */

/* hx_configure.mask bits. */
#define HX_CW_X         0x1u
#define HX_CW_Y         0x2u
#define HX_CW_WIDTH     0x4u
#define HX_CW_HEIGHT    0x8u
#define HX_CW_XY        (HX_CW_X | HX_CW_Y)
#define HX_CW_SIZE      (HX_CW_WIDTH | HX_CW_HEIGHT)
#define HX_CW_ALL       (HX_CW_XY | HX_CW_SIZE)

/*
 * HX_OP_BATCH: hold the screen still until a group of requests is
 * done.
 *
 * Moving a window means moving two of them - the frame the window
 * manager drew and the application's window inside it - and each
 * request repaints on its own, so for one moment the title bar is
 * somewhere the contents are not. Between `begin` and its matching
 * end the server collects damage instead of painting it, and the
 * whole group reaches the screen in a single pass.
 *
 * A batch left open would freeze the display, so the server also
 * closes one whenever a client waits for an event or disconnects: the
 * worst a forgotten end can do is cost an extra repaint.
 */
struct hx_batch {
    uint32_t begin;          /* non-zero to open, zero to close */
};

/* hx_text_req.flags. */
#define HX_TF_OPAQUE    0x1u /* paint the background box too */

/* Modifier bits reported in hx_event.mods and matched by
 * HX_OP_GRAB_KEY. Shift and Ctrl already shape the character a key
 * produces; Alt and Super do not, which is what makes them the
 * natural window-manager modifiers. */
#define HX_MOD_SHIFT  0x1u
#define HX_MOD_CTRL   0x2u
#define HX_MOD_ALT    0x4u
#define HX_MOD_SUPER  0x8u
#define HX_MOD_CAPS   0x10u
/* Caps Lock is state, not intent: grabs never match on it. */
#define HX_MOD_MASK   (HX_MOD_SHIFT | HX_MOD_CTRL | HX_MOD_ALT | HX_MOD_SUPER)

/* Pointer buttons, as reported in hx_event.key (press/release) and
 * hx_event.mods (currently held). */
#define HX_BTN_LEFT   0x1u
#define HX_BTN_RIGHT  0x2u
#define HX_BTN_MIDDLE 0x4u

/* hx_event.detail for HX_EV_POINTER / HX_EV_POINTER_NOTIFY. */
#define HX_PTR_MOTION  0u
#define HX_PTR_PRESS   1u
#define HX_PTR_RELEASE 2u
/* v1.4: the wheel turned. .key carries the signed number of steps
 * (positive away from the user, i.e. "scroll up"), cast through
 * int32_t; .x/.y are where the cursor was, as for any other pointer
 * event. A client that ignores it loses nothing. */
#define HX_PTR_WHEEL   3u

/* hx_background.style. */
#define HX_BG_SOLID     0
#define HX_BG_GRID      1

struct hx_hello {
    uint32_t magic;          /* in:  HIGHX_MAGIC */
    uint32_t version;        /* in:  HIGHX_VERSION the client was built for */
    uint32_t server_version; /* out: what the server speaks */
    uint32_t client_id;      /* out: the client's id (its pid) */
    uint32_t screen_w;       /* out */
    uint32_t screen_h;       /* out */
    uint32_t bpp;            /* out */
    uint32_t is_wm;          /* out: non-zero if a WM is registered */
};

struct hx_window_req {
    int32_t  x, y;
    uint32_t w, h;
    uint32_t flags;
    uint32_t background;     /* fill color of the fresh backing store */
    uint32_t win;            /* out: the new window id */
    char     title[HX_TITLE_MAX];
};

struct hx_win_op {
    uint32_t win;
};

struct hx_configure {
    uint32_t win;
    uint32_t mask;           /* HX_CW_* - which fields are meaningful */
    int32_t  x, y;
    uint32_t w, h;
};

struct hx_title {
    uint32_t win;
    char     title[HX_TITLE_MAX];
};

struct hx_rect_req {
    uint32_t win;
    int32_t  x, y;
    uint32_t w, h;
    uint32_t color;
};

struct hx_line_req {
    uint32_t win;
    int32_t  x0, y0, x1, y1;
    uint32_t color;
};

struct hx_text_req {
    uint32_t win;
    int32_t  x, y;
    uint32_t fg, bg;
    uint32_t flags;          /* HX_TF_* */
    char     text[HX_TEXT_MAX];
};

struct hx_image_req {
    uint32_t win;
    int32_t  x, y;
    uint32_t w, h;
    uint64_t pixels;         /* address of w*h uint32_t in the client */
};

struct hx_commit {
    uint32_t win;
    int32_t  x, y;
    uint32_t w, h;           /* w == 0: commit the whole window */
};

/* ---- events ---- */

enum hx_event_type {
    HX_EV_NONE = 0,
    HX_EV_EXPOSE,          /* redraw .x/.y/.w/.h of .win */
    HX_EV_KEY,             /* .key/.mods, delivered to the focused window */
    HX_EV_FOCUS_IN,
    HX_EV_FOCUS_OUT,
    HX_EV_CONFIGURE,       /* .win has a new geometry */
    HX_EV_MAP_REQUEST,     /* WM: a client wants .win on screen */
    HX_EV_MAP_NOTIFY,      /* WM: .win became visible */
    HX_EV_UNMAP_NOTIFY,    /* WM: .win left the screen */
    HX_EV_CREATE_NOTIFY,   /* WM: .win was created */
    HX_EV_DESTROY_NOTIFY,  /* WM: .win is gone */
    HX_EV_CLOSE,           /* the client should shut this window down */
    HX_EV_POINTER,         /* the mouse moved or a button changed */
    HX_EV_POINTER_NOTIFY,  /* WM: a pointer event on someone else's window */
    HX_EV_TICK             /* reserved */
};

/* Keys with no character (v1.5).
 *
 * hx_event.key carries a UNICODE CODEPOINT for anything that types:
 * 'a' is 0x61, a Turkish s-cedilla is 0x15F, and Ctrl+letter arrives
 * as the control character (0x11 and so on). Keys that type nothing -
 * the arrows, Home, Page Up - need codes that no codepoint can be, so
 * they live above the top of Unicode (0x10FFFF).
 *
 * They used to be 0x101..0x10A, which are perfectly good letters:
 * U+0101 is 'a' with a macron and U+0107 is a 'c' with an acute. The
 * collision cost nothing while the only layout was US ASCII and would
 * have made a Latvian keyboard scroll the window instead of typing. */
#define HX_KEY_BASE      0x01000000u
#define HX_KEY_UP        (HX_KEY_BASE + 1u)
#define HX_KEY_DOWN      (HX_KEY_BASE + 2u)
#define HX_KEY_LEFT      (HX_KEY_BASE + 3u)
#define HX_KEY_RIGHT     (HX_KEY_BASE + 4u)
#define HX_KEY_HOME      (HX_KEY_BASE + 5u)
#define HX_KEY_END       (HX_KEY_BASE + 6u)
#define HX_KEY_DELETE    (HX_KEY_BASE + 7u)
#define HX_KEY_INSERT    (HX_KEY_BASE + 8u)
#define HX_KEY_PAGE_UP   (HX_KEY_BASE + 9u)
#define HX_KEY_PAGE_DOWN (HX_KEY_BASE + 10u)

/* True when .key is a character rather than one of the keys above. */
#define HX_KEY_IS_CHAR(k) ((k) < HX_KEY_BASE)

/* A pointer event (HX_EV_POINTER / HX_EV_POINTER_NOTIFY) fills the
 * shared fields in its own way: .detail is HX_PTR_*, .key the button
 * that changed (0 for motion), .mods the buttons still held, .x/.y the
 * position relative to .win (the screen position when .win is 0, i.e.
 * the bare desktop) and .w/.h the position on screen - which is what a
 * window manager needs and cannot derive from a foreign window.
 * A wheel event (.detail == HX_PTR_WHEEL, v1.4) fills .key with the
 * step count instead of a button: (int32_t)ev.key is negative when
 * the wheel was pulled towards the user. */
struct hx_event {
    uint32_t type;           /* enum hx_event_type */
    uint32_t win;
    int32_t  x, y;
    uint32_t w, h;
    uint32_t key;
    uint32_t mods;           /* HX_MOD_* held when the key was pressed */
    uint32_t detail;
    uint32_t owner;          /* client id of the window's owner */
};

struct hx_next_event {
    int32_t  timeout_ms;     /* <0 block, 0 poll, >0 wait this long */
    uint32_t pad;
    struct hx_event ev;      /* out */
};

/* ---- introspection ---- */

struct hx_window_info {
    uint32_t win;            /* in for GET_WINDOW, out for QUERY_TREE */
    uint32_t owner;
    int32_t  x, y;
    uint32_t w, h;
    uint32_t flags;
    uint32_t mapped;
    uint32_t focused;
    uint32_t border_w;       /* v1.1 */
    uint32_t border_color;   /* v1.1 */
    char     title[HX_TITLE_MAX];
};

/* Windows in stacking order, bottom first. */
struct hx_tree {
    uint32_t count;          /* out: entries filled in */
    uint32_t max;            /* out: HX_MAX_WINDOWS */
    struct hx_window_info win[HX_MAX_WINDOWS];
};

struct hx_grab {
    uint32_t key;            /* key code as reported in hx_event.key */
    uint32_t mods;           /* HX_MOD_* that must be held (0 = none) */
};

/* A window's border is painted by the server around the window, like
 * X11's: it costs the client no drawing area and no requests, which
 * is what makes it the cheapest possible focus indicator for a window
 * manager - one request per focus change, no repaint of the window
 * itself. */
struct hx_border {
    uint32_t win;
    uint32_t width;          /* pixels on every side (0 = no border) */
    uint32_t color;
};

struct hx_screen_info {
    uint32_t w, h, bpp;
    uint32_t windows;        /* live windows */
    uint32_t clients;        /* connected clients */
    uint32_t focus;          /* focused window id (0 = none) */
    uint32_t wm;             /* client id of the window manager (0 = none) */
};

struct hx_background {
    uint32_t color;
    uint32_t style;          /* HX_BG_* */
};

/* The pointer: where it is, what is held, and which window is under
 * it. QUERY_POINTER fills all of it in; WARP_POINTER reads x and y. */
struct hx_pointer {
    int32_t  x, y;
    uint32_t buttons;        /* HX_BTN_* */
    uint32_t win;            /* window under the pointer (0 = desktop) */
};

#endif /* HIGHX_H */
