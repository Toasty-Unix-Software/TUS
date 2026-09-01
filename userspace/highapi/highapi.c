/*
 * highapi.c - highAPI implementation
 *
 * Every entry point here does the same three things: zero a request
 * structure from <highx.h>, fill it in, and trap into the display
 * server. The transport is SYS_HIGHX (see kernel/syscall/syscall.h):
 *
 *     rax = 38, rdi = opcode, rsi = &request, rdx = sizeof(request)
 *
 * The library keeps no shadow copy of the server's state: a client
 * that wants to know a window's geometry asks for it (hx_get_window),
 * which keeps highAPI honest when the window manager moves things
 * around behind the application's back.
 */

#include "highapi.h"

#include <string.h>

/* TUS syscall numbers (kernel/syscall/syscall.h). */
#define SYS_HIGHX 38
#define SYS_SPAWN 39

/*
 * The TUS syscall ABI: int $0x80, rax = number, rdi/rsi/rdx = args,
 * rax = result. Only rax survives the call, so every argument
 * register is declared read-write - including the three highX itself
 * does not use. Leaving those out lets the compiler keep a live value
 * in r8 across the trap, and it does: that is what turned closing a
 * window into a page fault at a garbage address.
 */
static long hx_syscall(long number, long a1, long a2, long a3) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"(number)
                     : "rcx", "r11", "memory");
    return ret;
}

static long hx_request(int op, void *arg, unsigned long len) {
    return hx_syscall(SYS_HIGHX, op, (long)arg, (long)len);
}

/* Requests that only name a window all share one structure. */
static int hx_win_op(int op, unsigned int win) {
    struct hx_win_op req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    return (int)hx_request(op, &req, sizeof(req));
}

/* ---- connection ---- */

int hx_open(struct hx_display *dpy) {
    struct hx_hello hello;
    memset(&hello, 0, sizeof(hello));
    hello.magic = HIGHX_MAGIC;
    hello.version = HIGHX_VERSION;

    long ret = hx_request(HX_OP_HELLO, &hello, sizeof(hello));
    if (ret < 0) {
        if (dpy != NULL) {
            memset(dpy, 0, sizeof(*dpy));
        }
        return (int)ret;
    }
    if (dpy != NULL) {
        dpy->screen_w = hello.screen_w;
        dpy->screen_h = hello.screen_h;
        dpy->bpp = hello.bpp;
        dpy->client_id = hello.client_id;
        dpy->has_wm = hello.is_wm;
        dpy->connected = 1;
    }
    return 0;
}

void hx_close(struct hx_display *dpy) {
    (void)hx_request(HX_OP_BYE, 0, 0);
    if (dpy != NULL) {
        memset(dpy, 0, sizeof(*dpy));
    }
}

/* ---- windows ---- */

unsigned int hx_create_window(int x, int y, unsigned int w, unsigned int h,
                              unsigned int flags, unsigned int background,
                              const char *title) {
    struct hx_window_req req;
    memset(&req, 0, sizeof(req));
    req.x = x;
    req.y = y;
    req.w = w;
    req.h = h;
    req.flags = flags;
    req.background = background;
    if (title != NULL) {
        strncpy(req.title, title, HX_TITLE_MAX - 1);
    }

    long ret = hx_request(HX_OP_CREATE_WINDOW, &req, sizeof(req));
    return ret > 0 ? (unsigned int)ret : 0;
}

int hx_destroy_window(unsigned int win) {
    return hx_win_op(HX_OP_DESTROY_WINDOW, win);
}

int hx_map(unsigned int win) {
    return hx_win_op(HX_OP_MAP_WINDOW, win);
}

int hx_unmap(unsigned int win) {
    return hx_win_op(HX_OP_UNMAP_WINDOW, win);
}

int hx_raise(unsigned int win) {
    return hx_win_op(HX_OP_RAISE_WINDOW, win);
}

int hx_lower(unsigned int win) {
    return hx_win_op(HX_OP_LOWER_WINDOW, win);
}

int hx_focus(unsigned int win) {
    return hx_win_op(HX_OP_SET_FOCUS, win);
}

int hx_close_window(unsigned int win) {
    return hx_win_op(HX_OP_CLOSE_WINDOW, win);
}

static int hx_configure(unsigned int win, unsigned int mask, int x, int y,
                        unsigned int w, unsigned int h) {
    struct hx_configure req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    req.mask = mask;
    req.x = x;
    req.y = y;
    req.w = w;
    req.h = h;
    return (int)hx_request(HX_OP_CONFIGURE_WINDOW, &req, sizeof(req));
}

int hx_move(unsigned int win, int x, int y) {
    return hx_configure(win, HX_CW_XY, x, y, 0, 0);
}

int hx_resize(unsigned int win, unsigned int w, unsigned int h) {
    return hx_configure(win, HX_CW_SIZE, 0, 0, w, h);
}

int hx_move_resize(unsigned int win, int x, int y, unsigned int w,
                   unsigned int h) {
    return hx_configure(win, HX_CW_ALL, x, y, w, h);
}

static int hx_batch(unsigned int begin) {
    struct hx_batch req;
    memset(&req, 0, sizeof(req));
    req.begin = begin;
    return (int)hx_request(HX_OP_BATCH, &req, sizeof(req));
}

int hx_batch_begin(void) { return hx_batch(1); }
int hx_batch_end(void) { return hx_batch(0); }

int hx_set_title(unsigned int win, const char *title) {
    struct hx_title req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    if (title != NULL) {
        strncpy(req.title, title, HX_TITLE_MAX - 1);
    }
    return (int)hx_request(HX_OP_SET_TITLE, &req, sizeof(req));
}

/* ---- drawing ---- */

static int hx_rect_op(int op, unsigned int win, int x, int y, unsigned int w,
                      unsigned int h, unsigned int color) {
    struct hx_rect_req req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    req.x = x;
    req.y = y;
    req.w = w;
    req.h = h;
    req.color = color;
    return (int)hx_request(op, &req, sizeof(req));
}

int hx_fill(unsigned int win, int x, int y, unsigned int w, unsigned int h,
            unsigned int color) {
    return hx_rect_op(HX_OP_FILL_RECT, win, x, y, w, h, color);
}

int hx_rect(unsigned int win, int x, int y, unsigned int w, unsigned int h,
            unsigned int color) {
    return hx_rect_op(HX_OP_DRAW_RECT, win, x, y, w, h, color);
}

int hx_line(unsigned int win, int x0, int y0, int x1, int y1,
            unsigned int color) {
    struct hx_line_req req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    req.x0 = x0;
    req.y0 = y0;
    req.x1 = x1;
    req.y1 = y1;
    req.color = color;
    return (int)hx_request(HX_OP_DRAW_LINE, &req, sizeof(req));
}

int hx_text(unsigned int win, int x, int y, unsigned int fg, unsigned int bg,
            unsigned int flags, const char *text) {
    struct hx_text_req req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    req.x = x;
    req.y = y;
    req.fg = fg;
    req.bg = bg;
    req.flags = flags;
    if (text != NULL) {
        strncpy(req.text, text, HX_TEXT_MAX - 1);
    }
    return (int)hx_request(HX_OP_DRAW_TEXT, &req, sizeof(req));
}

int hx_image(unsigned int win, int x, int y, unsigned int w, unsigned int h,
             const unsigned int *pixels) {
    struct hx_image_req req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    req.x = x;
    req.y = y;
    req.w = w;
    req.h = h;
    req.pixels = (unsigned long)pixels;
    return (int)hx_request(HX_OP_PUT_IMAGE, &req, sizeof(req));
}

int hx_clear(unsigned int win, unsigned int color) {
    struct hx_window_info info;
    int ret = hx_get_window(win, &info);
    if (ret < 0) {
        return ret;
    }
    return hx_fill(win, 0, 0, info.w, info.h, color);
}

int hx_commit_rect(unsigned int win, int x, int y, unsigned int w,
                   unsigned int h) {
    struct hx_commit req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    req.x = x;
    req.y = y;
    req.w = w;
    req.h = h;
    return (int)hx_request(HX_OP_COMMIT, &req, sizeof(req));
}

int hx_commit(unsigned int win) {
    return hx_commit_rect(win, 0, 0, 0, 0); /* w == 0: the whole window */
}

/* ---- events ---- */

int hx_next_event(struct hx_event *ev, int timeout_ms) {
    struct hx_next_event req;
    memset(&req, 0, sizeof(req));
    req.timeout_ms = timeout_ms;

    long ret = hx_request(HX_OP_NEXT_EVENT, &req, sizeof(req));
    if (ret > 0 && ev != NULL) {
        *ev = req.ev;
    }
    return (int)ret;
}

int hx_wait_event(struct hx_event *ev) {
    return hx_next_event(ev, -1);
}

int hx_poll_event(struct hx_event *ev) {
    return hx_next_event(ev, 0);
}

/* ---- window management ---- */

int hx_wm_register(void) {
    return (int)hx_request(HX_OP_WM_REGISTER, 0, 0);
}

int hx_grab_key(unsigned int key, unsigned int mods) {
    struct hx_grab req;
    memset(&req, 0, sizeof(req));
    req.key = key;
    req.mods = mods;
    return (int)hx_request(HX_OP_GRAB_KEY, &req, sizeof(req));
}

int hx_query_tree(struct hx_tree *tree) {
    if (tree == NULL) {
        return -22; /* EINVAL */
    }
    memset(tree, 0, sizeof(*tree));
    return (int)hx_request(HX_OP_QUERY_TREE, tree, sizeof(*tree));
}

int hx_get_window(unsigned int win, struct hx_window_info *info) {
    if (info == NULL) {
        return -22;
    }
    memset(info, 0, sizeof(*info));
    info->win = win;
    return (int)hx_request(HX_OP_GET_WINDOW, info, sizeof(*info));
}

int hx_screen_info(struct hx_screen_info *info) {
    if (info == NULL) {
        return -22;
    }
    memset(info, 0, sizeof(*info));
    return (int)hx_request(HX_OP_SCREEN_INFO, info, sizeof(*info));
}

int hx_set_border(unsigned int win, unsigned int width, unsigned int color) {
    struct hx_border req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    req.width = width;
    req.color = color;
    return (int)hx_request(HX_OP_SET_BORDER, &req, sizeof(req));
}

/* ---- the pointer (v1.2) ---- */

int hx_query_pointer(struct hx_pointer *out) {
    struct hx_pointer req;
    memset(&req, 0, sizeof(req));
    long ret = hx_request(HX_OP_QUERY_POINTER, &req, sizeof(req));
    if (ret >= 0 && out != NULL) {
        *out = req;
    }
    return (int)ret;
}

/* Route every pointer event to `win` until hx_ungrab_pointer(); a menu
 * uses this to see the click that dismisses it. */
int hx_grab_pointer(unsigned int win) {
    struct hx_win_op req;
    memset(&req, 0, sizeof(req));
    req.win = win;
    return (int)hx_request(HX_OP_GRAB_POINTER, &req, sizeof(req));
}

int hx_ungrab_pointer(void) {
    return hx_grab_pointer(0);
}

int hx_warp_pointer(int x, int y) {
    struct hx_pointer req;
    memset(&req, 0, sizeof(req));
    req.x = x;
    req.y = y;
    return (int)hx_request(HX_OP_WARP_POINTER, &req, sizeof(req));
}

int hx_set_background(unsigned int color, unsigned int style) {
    struct hx_background req;
    memset(&req, 0, sizeof(req));
    req.color = color;
    req.style = style;
    return (int)hx_request(HX_OP_SET_BACKGROUND, &req, sizeof(req));
}

/* ---- process helper ---- */

int hx_spawn(const char *path, char *const argv[]) {
    return (int)hx_syscall(SYS_SPAWN, (long)path, (long)argv, 0);
}
