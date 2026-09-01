/*
 * highx.c - highX display server
 *
 * The server owns three tables: windows (with their backing stores),
 * clients (one per task that said HELLO, each with an event queue)
 * and the stacking order. Every protocol request from
 * kernel/syscall/syscall.c lands in highx_request(), which validates
 * the request, mutates that state and repaints whatever changed.
 *
 * Concurrency: TUS is preemptive, so two tasks can be inside the
 * server at once. Every request runs with preemption disabled, which
 * makes the window tables and the framebuffer writes atomic with
 * respect to other clients. The one exception is the blocking
 * HX_OP_NEXT_EVENT wait, which must let other tasks run - it drops
 * preemption around its hlt() and re-checks the world on every wake.
 *
 * Input: highX drains the keyboard ring itself (the text console is
 * suspended while the server runs). A key first goes to the window
 * manager if the WM grabbed it (HX_OP_GRAB_KEY - this is how tusWM
 * gets Ctrl+Q and friends no matter which window has focus), then to
 * the focused window's client, and finally to the WM as a fallback.
 *
 * Window management: when a window manager is registered, a client's
 * HX_OP_MAP_WINDOW does NOT put the window on screen. The server
 * turns it into an HX_EV_MAP_REQUEST for the WM, which places the
 * window and maps it itself - X11's substructure redirection, minus
 * the reparenting.
 */

#include "highx.h"

#include "../arch/x86_64/io.h"
#include "../core/errno.h"
#include "../core/klib.h"
#include "../drivers/fb/fb.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/mouse/mouse.h"
#include "../drivers/pit/pit.h"
#include "../mm/kmalloc.h"
#include "../sched/sched.h"

/* ---- server state ---- */

struct hxs_client {
    bool     used;
    bool     is_wm;
    uint32_t pid;
    struct hx_event queue[HX_EVENT_QUEUE];
    int      qhead; /* next slot to write */
    int      qtail; /* next slot to read */
    /* A grab is a (key, modifiers) pair. They used to be packed into
     * one word as key | mods << 16, which stopped working when key
     * became a Unicode codepoint plus a range above it for the keys
     * that type nothing - HX_KEY_UP truncated to 1, which is Ctrl+A. */
    struct { uint32_t key; uint32_t mods; } grabs[HX_MAX_GRABS];
    int      ngrabs;
};

static bool g_active;
static struct hxs_window g_windows[HX_MAX_WINDOWS];
static struct hxs_window *g_stack[HX_MAX_WINDOWS]; /* bottom .. top */
static int g_nstack;
static struct hxs_client g_clients[HX_MAX_CLIENTS];
static uint32_t g_next_id = 1;
static uint32_t g_focus;   /* focused window id (0 = none) */
static uint32_t g_wm_pid;  /* client id of the window manager (0 = none) */
static uint32_t g_screen_w, g_screen_h, g_screen_bpp;
static int32_t  g_ptr_x, g_ptr_y;   /* cursor position in screen pixels */
static uint32_t g_ptr_buttons;      /* HX_BTN_* currently held */
static uint32_t g_ptr_grab;         /* window holding the implicit grab */
static uint32_t g_ptr_hold;         /* window holding an explicit grab */

/* Upper bound of the canonical user half; PUT_IMAGE carries a raw
 * pointer inside the request, so the server range-checks it exactly
 * like the syscall layer checks the request itself. */
#define USER_HALF_MAX 0x00007fffffffffffull

static bool user_ptr_ok(bool from_user, uint64_t ptr, uint64_t len) {
    if (!from_user) {
        return true;
    }
    uint64_t end = ptr + len;
    return end >= ptr && end <= USER_HALF_MAX;
}

/* ---- windows ---- */

static struct hxs_window *win_find(uint32_t id) {
    if (id == 0) {
        return NULL;
    }
    for (int i = 0; i < HX_MAX_WINDOWS; i++) {
        if (g_windows[i].id == id) {
            return &g_windows[i];
        }
    }
    return NULL;
}

static void stack_remove(struct hxs_window *win) {
    for (int i = 0; i < g_nstack; i++) {
        if (g_stack[i] == win) {
            for (int j = i; j < g_nstack - 1; j++) {
                g_stack[j] = g_stack[j + 1];
            }
            g_nstack--;
            return;
        }
    }
}

/* Desktop windows are pinned to the bottom of the stack; everything
 * else goes on top of them. */
static void stack_insert_top(struct hxs_window *win) {
    if (g_nstack >= HX_MAX_WINDOWS) {
        return;
    }
    g_stack[g_nstack++] = win;
}

static void stack_insert_bottom(struct hxs_window *win) {
    if (g_nstack >= HX_MAX_WINDOWS) {
        return;
    }
    for (int i = g_nstack; i > 0; i--) {
        g_stack[i] = g_stack[i - 1];
    }
    g_stack[0] = win;
    g_nstack++;
}

/*
 * Holding the screen still for a group of requests.
 *
 * Moving a window is two requests - the frame the window manager drew
 * and the application's window inside it - and painting each as it
 * arrives puts the title bar somewhere the contents are not, for one
 * frame, every time the pointer moves. While a batch is open the
 * damage is unioned here instead of painted, and the group reaches
 * the screen in one pass.
 *
 * One batch at a time, and any client waiting for an event closes it:
 * a batch nobody ever closed would freeze the display, and this way
 * the worst a forgotten end costs is a repaint that came early.
 */
static uint32_t g_batch_pid;
static bool g_batch_any;
static int32_t g_batch_x0, g_batch_y0, g_batch_x1, g_batch_y1;

static void repaint_rect(int32_t x, int32_t y, int32_t w, int32_t h) {
    if (w <= 0 || h <= 0) {
        return;
    }
    if (g_batch_pid != 0) {
        if (!g_batch_any) {
            g_batch_x0 = x;
            g_batch_y0 = y;
            g_batch_x1 = x + w;
            g_batch_y1 = y + h;
            g_batch_any = true;
            return;
        }
        if (x < g_batch_x0) g_batch_x0 = x;
        if (y < g_batch_y0) g_batch_y0 = y;
        if (x + w > g_batch_x1) g_batch_x1 = x + w;
        if (y + h > g_batch_y1) g_batch_y1 = y + h;
        return;
    }
    hxcomp_paint(x, y, w, h, g_stack, g_nstack);
}

/* Close any open batch and put what it collected on screen. */
static void batch_end(void) {
    bool any = g_batch_any;
    int32_t x0 = g_batch_x0, y0 = g_batch_y0;
    int32_t x1 = g_batch_x1, y1 = g_batch_y1;

    g_batch_pid = 0;
    g_batch_any = false;
    if (any) {
        repaint_rect(x0, y0, x1 - x0, y1 - y0);
    }
}

/* Two damaged rectangles that belong together: a window's old place
 * and its new one, the cursor erased and the cursor drawn. When they
 * overlap - which is the whole point of a nudge or a mouse step - the
 * overlap would be painted twice, so the union costs no more than the
 * pair and is one walk of the window stack instead of two. Beyond
 * that (a window flung across the screen) the two rectangles stay
 * separate rather than repainting everything between them. */
static void repaint_pair(int32_t ax, int32_t ay, int32_t aw, int32_t ah,
                         int32_t bx, int32_t by, int32_t bw, int32_t bh) {
    if (aw <= 0 || ah <= 0) {
        repaint_rect(bx, by, bw, bh);
        return;
    }
    if (bw <= 0 || bh <= 0) {
        repaint_rect(ax, ay, aw, ah);
        return;
    }
    int32_t ux = ax < bx ? ax : bx;
    int32_t uy = ay < by ? ay : by;
    int32_t ux1 = ax + aw > bx + bw ? ax + aw : bx + bw;
    int32_t uy1 = ay + ah > by + bh ? ay + ah : by + bh;

    int64_t together = (int64_t)(ux1 - ux) * (uy1 - uy);
    int64_t apart = (int64_t)aw * ah + (int64_t)bw * bh;
    if (together <= apart) {
        repaint_rect(ux, uy, ux1 - ux, uy1 - uy);
        return;
    }
    repaint_rect(ax, ay, aw, ah);
    repaint_rect(bx, by, bw, bh);
}

/* Damage covers the border too: it is part of what the window paints
 * on screen, so moving or hiding a window has to clean it up. */
static void repaint_window(const struct hxs_window *win) {
    int32_t x, y, w, h;
    hxwin_outer(win, &x, &y, &w, &h);
    repaint_rect(x, y, w, h);
}

/* ---- clients and events ---- */

static struct hxs_client *client_find(uint32_t pid) {
    for (int i = 0; i < HX_MAX_CLIENTS; i++) {
        if (g_clients[i].used && g_clients[i].pid == pid) {
            return &g_clients[i];
        }
    }
    return NULL;
}

static struct hxs_client *client_add(uint32_t pid) {
    struct hxs_client *c = client_find(pid);
    if (c != NULL) {
        return c;
    }
    for (int i = 0; i < HX_MAX_CLIENTS; i++) {
        if (!g_clients[i].used) {
            memset(&g_clients[i], 0, sizeof(g_clients[i]));
            g_clients[i].used = true;
            g_clients[i].pid = pid;
            return &g_clients[i];
        }
    }
    return NULL;
}

static void ev_push(struct hxs_client *c, const struct hx_event *ev) {
    if (c == NULL) {
        return;
    }
    int next = (c->qhead + 1) % HX_EVENT_QUEUE;
    if (next == c->qtail) {
        /* Queue full: drop the oldest event. A client that stopped
         * reading must not stall the whole display. */
        c->qtail = (c->qtail + 1) % HX_EVENT_QUEUE;
    }
    c->queue[c->qhead] = *ev;
    c->qhead = next;
}

static bool ev_pop(struct hxs_client *c, struct hx_event *out) {
    if (c == NULL || c->qhead == c->qtail) {
        return false;
    }
    *out = c->queue[c->qtail];
    c->qtail = (c->qtail + 1) % HX_EVENT_QUEUE;
    return true;
}

static void ev_send(uint32_t pid, uint32_t type, uint32_t win,
                    int32_t x, int32_t y, uint32_t w, uint32_t h) {
    struct hxs_client *c = client_find(pid);
    if (c == NULL) {
        return;
    }
    struct hx_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = type;
    ev.win = win;
    ev.x = x;
    ev.y = y;
    ev.w = w;
    ev.h = h;
    ev.owner = pid;
    ev_push(c, &ev);
}

/* Tell the window manager about a window that is not its own. */
static void ev_to_wm(uint32_t type, const struct hxs_window *win) {
    if (g_wm_pid == 0 || win == NULL || win->owner == g_wm_pid) {
        return;
    }
    struct hxs_client *c = client_find(g_wm_pid);
    if (c == NULL) {
        return;
    }
    struct hx_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = type;
    ev.win = win->id;
    ev.x = win->x;
    ev.y = win->y;
    ev.w = win->w;
    ev.h = win->h;
    ev.detail = win->flags;
    ev.owner = win->owner;
    ev_push(c, &ev);
}

/* ---- focus ---- */

static bool focusable(const struct hxs_window *win) {
    return win != NULL && win->id != 0 && win->mapped &&
           (win->flags & (HX_WF_FRAME | HX_WF_DESKTOP)) == 0;
}

static void set_focus(uint32_t id) {
    if (g_focus == id) {
        return;
    }
    struct hxs_window *old = win_find(g_focus);
    if (old != NULL) {
        ev_send(old->owner, HX_EV_FOCUS_OUT, old->id, 0, 0, 0, 0);
    }
    g_focus = id;
    struct hxs_window *win = win_find(id);
    if (win != NULL) {
        ev_send(win->owner, HX_EV_FOCUS_IN, win->id, 0, 0, 0, 0);
    }
    if (g_wm_pid != 0) {
        struct hxs_client *wm = client_find(g_wm_pid);
        if (wm != NULL) {
            struct hx_event ev;
            memset(&ev, 0, sizeof(ev));
            ev.type = HX_EV_FOCUS_IN;
            ev.win = id;
            ev.detail = 1; /* detail 1: notification for the WM */
            ev_push(wm, &ev);
        }
    }
}

/* Focus the topmost window that can take it (used after a destroy or
 * an unmap took the focused window away). */
static void focus_topmost(void) {
    for (int i = g_nstack - 1; i >= 0; i--) {
        if (focusable(g_stack[i])) {
            set_focus(g_stack[i]->id);
            return;
        }
    }
    set_focus(0);
}

static void window_free(struct hxs_window *win) {
    if (win == NULL || win->id == 0) {
        return;
    }
    int32_t x, y, w, h;
    hxwin_outer(win, &x, &y, &w, &h);
    bool was_visible = win->mapped;
    uint32_t id = win->id;

    ev_to_wm(HX_EV_DESTROY_NOTIFY, win);
    if (g_ptr_grab == id) {
        g_ptr_grab = 0;
    }
    if (g_ptr_hold == id) {
        g_ptr_hold = 0;
    }
    stack_remove(win);
    if (win->pix != NULL) {
        kfree(win->pix);
    }
    memset(win, 0, sizeof(*win));

    if (was_visible) {
        repaint_rect(x, y, w, h);
    }
    if (g_focus == id) {
        g_focus = 0;
        focus_topmost();
    }
}

/* ---- input ---- */

/* The driver's modifier bits and the protocol's are deliberately
 * separate enumerations; this is the one place that maps them. */
static uint32_t kbd_mods_to_hx(int mods) {
    uint32_t out = 0;
    if (mods & KBD_MOD_SHIFT) {
        out |= HX_MOD_SHIFT;
    }
    if (mods & KBD_MOD_CTRL) {
        out |= HX_MOD_CTRL;
    }
    if (mods & KBD_MOD_ALT) {
        out |= HX_MOD_ALT;
    }
    if (mods & KBD_MOD_SUPER) {
        out |= HX_MOD_SUPER;
    }
    if (mods & KBD_MOD_CAPS) {
        out |= HX_MOD_CAPS;
    }
    return out;
}

static uint32_t kbd_to_hx_key(const struct kbd_event *ev) {
    if (ev->type == KBD_EVENT_CHAR) {
        /* The codepoint, not the byte: a Turkish s-cedilla is U+015F
         * and has no `c`. Clients read hx_event.key the same way they
         * always did - it is just no longer limited to ASCII. */
        return ev->cp;
    }
    switch (ev->code) {
    case KBD_KEY_UP:        return HX_KEY_UP;
    case KBD_KEY_DOWN:      return HX_KEY_DOWN;
    case KBD_KEY_LEFT:      return HX_KEY_LEFT;
    case KBD_KEY_RIGHT:     return HX_KEY_RIGHT;
    case KBD_KEY_HOME:      return HX_KEY_HOME;
    case KBD_KEY_END:       return HX_KEY_END;
    case KBD_KEY_DELETE:    return HX_KEY_DELETE;
    case KBD_KEY_INSERT:    return HX_KEY_INSERT;
    case KBD_KEY_PAGE_UP:   return HX_KEY_PAGE_UP;
    case KBD_KEY_PAGE_DOWN: return HX_KEY_PAGE_DOWN;
    default:                return 0;
    }
}

/* A grab matches only when the modifiers are EXACTLY the ones the
 * window manager asked for, so Alt+Left and Alt+Shift+Left are
 * different bindings. */
static bool wm_grabbed(uint32_t key, uint32_t mods) {
    struct hxs_client *wm = g_wm_pid != 0 ? client_find(g_wm_pid) : NULL;
    if (wm == NULL) {
        return false;
    }
    mods &= HX_MOD_MASK;
    for (int i = 0; i < wm->ngrabs; i++) {
        if (wm->grabs[i].key == key && wm->grabs[i].mods == mods) {
            return true;
        }
    }
    return false;
}

static void deliver_key(uint32_t key, uint32_t mods) {
    struct hx_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = HX_EV_KEY;
    ev.key = key;
    ev.mods = mods;
    ev.win = g_focus;

    if (wm_grabbed(key, mods)) {
        ev_push(client_find(g_wm_pid), &ev);
        return;
    }
    struct hxs_window *win = win_find(g_focus);
    if (win != NULL) {
        ev.owner = win->owner;
        struct hxs_client *c = client_find(win->owner);
        if (c != NULL) {
            ev_push(c, &ev);
            return;
        }
    }
    if (g_wm_pid != 0) {
        ev.win = 0;
        ev_push(client_find(g_wm_pid), &ev);
    }
}

/* ---- pointer ----
 *
 * The cursor lives in the server. The driver reports relative
 * movement; highX turns it into a screen position (clamped, so the
 * arrow can never be lost past an edge), repaints the rectangle it
 * left and the one it arrived in, and posts the event.
 *
 * Delivery mirrors the keyboard's shape, with the window under the
 * cursor standing in for the focused one: its owner gets
 * HX_EV_POINTER, and a window manager watching someone else's window
 * gets HX_EV_POINTER_NOTIFY for the button events. Motion is not
 * forwarded to the WM - a window manager that was sent every packet
 * would spend the session draining its own queue. Over the bare
 * desktop the event goes to the WM with .win == 0.
 */

/* Topmost mapped window whose outer rectangle contains the point. The
 * border counts: it is part of what the user is pointing at. */
static struct hxs_window *win_at(int32_t x, int32_t y) {
    for (int i = g_nstack - 1; i >= 0; i--) {
        struct hxs_window *win = g_stack[i];
        if (win == NULL || win->id == 0 || !win->mapped) {
            continue;
        }
        int32_t ox, oy, ow, oh;
        hxwin_outer(win, &ox, &oy, &ow, &oh);
        if (x >= ox && x < ox + ow && y >= oy && y < oy + oh) {
            return win;
        }
    }
    return NULL;
}

/* Motion compression: a client slow to read must not have its queue -
 * and with it an EXPOSE it has not handled yet - pushed out by a
 * stream of positions that are already stale. Consecutive motion on
 * the same window overwrites the entry already queued. */
static void ev_push_pointer(struct hxs_client *c, const struct hx_event *ev) {
    if (c == NULL) {
        return;
    }
    if (ev->detail == HX_PTR_MOTION && c->qhead != c->qtail) {
        struct hx_event *prev =
            &c->queue[(c->qhead + HX_EVENT_QUEUE - 1) % HX_EVENT_QUEUE];
        if (prev->type == ev->type && prev->detail == HX_PTR_MOTION &&
            prev->win == ev->win) {
            *prev = *ev;
            return;
        }
    }
    ev_push(c, ev);
}

static void deliver_pointer(uint32_t detail, uint32_t button) {
    /* While a button is down every event belongs to the window the
     * press landed on, wherever the cursor has wandered since. This is
     * X11's implicit grab, and it is what makes dragging a title bar
     * across another window work instead of stopping at its edge. */
    bool held = false;
    struct hxs_window *win = NULL;
    if (g_ptr_hold != 0) {
        /* An explicit grab (HX_OP_GRAB_POINTER): a menu that wants to
         * see the click that dismisses it, wherever it lands. */
        win = win_find(g_ptr_hold);
        if (win == NULL) {
            g_ptr_hold = 0;
        } else {
            held = true;
        }
    }
    if (win == NULL && g_ptr_grab != 0) {
        win = win_find(g_ptr_grab);
        if (win == NULL) {
            g_ptr_grab = 0;
        }
    }
    if (win == NULL) {
        win = win_at(g_ptr_x, g_ptr_y);
    }

    struct hx_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = HX_EV_POINTER;
    ev.detail = detail;
    ev.key = button;
    ev.mods = g_ptr_buttons;
    /* .x/.y are window-local, which is what a client hit-tests
     * against; .w/.h carry the screen position, which is what a window
     * manager needs and cannot work out from a foreign window. */
    ev.w = (uint32_t)g_ptr_x;
    ev.h = (uint32_t)g_ptr_y;
    ev.x = g_ptr_x; /* over the bare desktop: the screen position */
    ev.y = g_ptr_y;

    if (win != NULL) {
        ev.win = win->id;
        ev.owner = win->owner;
        ev.x = g_ptr_x - win->x;
        ev.y = g_ptr_y - win->y;
        ev_push_pointer(client_find(win->owner), &ev);
    }

    if (g_wm_pid == 0 || held) {
        return;
    }
    if (win == NULL) {
        ev_push_pointer(client_find(g_wm_pid), &ev);
    } else if (win->owner != g_wm_pid && detail != HX_PTR_MOTION) {
        ev.type = HX_EV_POINTER_NOTIFY;
        ev_push_pointer(client_find(g_wm_pid), &ev);
    }
}

static int32_t clamp_i32(int32_t v, int32_t lo, int32_t hi) {
    if (v < lo) {
        return lo;
    }
    return v > hi ? hi : v;
}

/* Move the cursor to a screen position and repaint what changed.
 * Returns true when it actually went somewhere. */
static bool pointer_move_to(int32_t x, int32_t y) {
    if (g_screen_w == 0 || g_screen_h == 0) {
        return false;
    }
    x = clamp_i32(x, 0, (int32_t)g_screen_w - 1);
    y = clamp_i32(y, 0, (int32_t)g_screen_h - 1);
    if (x == g_ptr_x && y == g_ptr_y) {
        return false;
    }

    int32_t ox, oy, ow, oh;
    hxcomp_pointer_rect(&ox, &oy, &ow, &oh);
    g_ptr_x = x;
    g_ptr_y = y;
    hxcomp_set_pointer(x, y);
    /* Erase the sprite where it was and draw it where it now is. A
     * mouse step is a few pixels, so the two 12x19 rectangles almost
     * always overlap and become one paint. */
    repaint_pair(ox, oy, ow, oh, x, y, ow, oh);
    return true;
}

static void pointer_button(uint32_t button, bool pressed) {
    if (pressed && g_ptr_hold == 0) {
        struct hxs_window *win = win_at(g_ptr_x, g_ptr_y);
        if (g_ptr_grab == 0 && win != NULL) {
            g_ptr_grab = win->id;
        }
        /* Without a window manager nobody else would act on a click,
         * so the server does the one thing every user expects. */
        if (g_wm_pid == 0 && focusable(win)) {
            set_focus(win->id);
        }
    }
    deliver_pointer(pressed ? HX_PTR_PRESS : HX_PTR_RELEASE, button);
    if (!pressed && g_ptr_buttons == 0) {
        g_ptr_grab = 0; /* the last button came up: the grab is over */
    }
}

/* Move the cursor to the position the packets drained so far add up
 * to: one repaint, one motion event. */
static void flush_motion(int32_t x, int32_t y) {
    if (pointer_move_to(x, y)) {
        deliver_pointer(HX_PTR_MOTION, 0);
    }
}

/* Drain the mouse ring: one packet is a movement, a button change, or
 * both, and the button state is whole in every packet.
 *
 * A PS/2 mouse in motion delivers packets far faster than the screen
 * can be repainted, and every one of them used to erase and redraw
 * the cursor - work whose only visible result is the position of the
 * last packet in the burst. The deltas are therefore added up and
 * applied once, at the end of the ring or at the first packet that
 * changes a button, which is the point where the position has to be
 * exact again: a press is delivered to the window under the cursor,
 * so the move in front of it must have happened. Clients see the same
 * thing they saw before, since consecutive motion events on one
 * window were already collapsed in the event queue. */
static void pump_pointer(void) {
    static const uint32_t buttons[] = { HX_BTN_LEFT, HX_BTN_RIGHT,
                                        HX_BTN_MIDDLE };
    struct mouse_event mev;

    /* Where the cursor would be if every packet drained so far had
     * been applied. It is clamped packet by packet, exactly as it
     * would be if each one were applied on its own, so shoving the
     * mouse into an edge and pulling back leaves the cursor at the
     * edge rather than where it would have been without the wall. */
    int32_t px = g_ptr_x, py = g_ptr_y;

    while (mouse_poll(&mev)) {
        /* The driver reports Y upwards; the screen counts downwards. */
        px = clamp_i32(px + mev.dx, 0, (int32_t)g_screen_w - 1);
        py = clamp_i32(py - mev.dy, 0, (int32_t)g_screen_h - 1);

        /* The wheel is not a position: it is delivered where the
         * cursor already is, and it must not be folded into the next
         * motion event or a slow scroll would arrive as one jump. */
        if (mev.dz != 0) {
            flush_motion(px, py);
            deliver_pointer(HX_PTR_WHEEL, (uint32_t)mev.dz);
        }

        uint32_t before = g_ptr_buttons;
        uint32_t now = 0;
        if (mev.buttons & MOUSE_BTN_LEFT) {
            now |= HX_BTN_LEFT;
        }
        if (mev.buttons & MOUSE_BTN_RIGHT) {
            now |= HX_BTN_RIGHT;
        }
        if (mev.buttons & MOUSE_BTN_MIDDLE) {
            now |= HX_BTN_MIDDLE;
        }
        if (now == before) {
            continue; /* movement only: keep accumulating */
        }
        /* The buttons are updated before the motion is flushed, so a
         * motion event still reports the state this packet carries. */
        g_ptr_buttons = now;
        flush_motion(px, py);
        for (unsigned i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
            uint32_t b = buttons[i];
            if ((before & b) != (now & b)) {
                pointer_button(b, (now & b) != 0);
            }
        }
    }
    flush_motion(px, py);
}

/* Drain the keyboard ring into client queues. Called at the top of
 * every request, so input is picked up as often as clients talk to
 * the server. */
static void pump_input(void) {
    while (kbd_has_char()) {
        struct kbd_event kev = kbd_get_event();
        uint32_t key = kbd_to_hx_key(&kev);
        if (key != 0) {
            deliver_key(key, kbd_mods_to_hx(kev.mods));
        }
    }
    pump_pointer();
}

/* ---- lifecycle ---- */

int highx_start(void) {
    if (g_active) {
        return 0;
    }
    if (hxcomp_init() != 0) {
        return -ENODEV;
    }

    memset(g_windows, 0, sizeof(g_windows));
    memset(g_clients, 0, sizeof(g_clients));
    memset(g_stack, 0, sizeof(g_stack));
    g_nstack = 0;
    g_next_id = 1;
    g_focus = 0;
    g_wm_pid = 0;

    hxcomp_screen(&g_screen_w, &g_screen_h, &g_screen_bpp);
    hxcomp_set_background(0x00101820, HX_BG_GRID);

    /* Swallow keys typed before the session started (the Enter that
     * launched `highx` is still in the ring). */
    while (kbd_has_char()) {
        (void)kbd_get_event();
    }

    /* Same for movement from before the session. The cursor starts in
     * the middle of the screen, and stays hidden on a machine with no
     * mouse: an arrow that can never move is worse than none. */
    while (mouse_has_event()) {
        (void)mouse_poll(NULL);
    }
    g_ptr_buttons = 0;
    g_ptr_grab = 0;
    g_ptr_hold = 0;
    g_ptr_x = (int32_t)g_screen_w / 2;
    g_ptr_y = (int32_t)g_screen_h / 2;
    hxcomp_set_pointer(g_ptr_x, g_ptr_y);
    hxcomp_show_pointer(mouse_present());

    fb_set_graphics(true);
    g_active = true;
    repaint_rect(0, 0, (int32_t)g_screen_w, (int32_t)g_screen_h);
    return 0;
}

/* Rebind the running session to a framebuffer that has changed shape.
 *
 * The display mode can change under a live session (res_set), and
 * everything the server cached about the screen is then wrong: the
 * compositor's pixel pointer, pitch and bounds, the server's idea of
 * the screen size, the pointer position, and the position of every
 * window that now hangs off the edge.
 *
 * Windows are moved, never resized. A resize would mean reallocating
 * a client's backing store behind its back and telling it its drawing
 * is a different size than the one it drew; moving is something the
 * protocol already does to windows every day, and a window wider than
 * the new screen simply clips - which is what a window wider than the
 * screen has always done.
 *
 * Every mapped window gets an expose afterwards, so a client that
 * cached the screen geometry from HX_OP_HELLO repaints and can ask
 * again with HX_OP_GET_SCREEN. */
int highx_rebind(void) {
    if (!g_active) {
        return 0;
    }

    preempt_disable();

    if (hxcomp_init() != 0) {
        /* The new mode is unusable (not 32bpp, no address). The
         * session cannot continue against a framebuffer the
         * compositor will not paint. */
        preempt_enable();
        return -ENODEV;
    }
    hxcomp_screen(&g_screen_w, &g_screen_h, &g_screen_bpp);

    for (int i = 0; i < HX_MAX_WINDOWS; i++) {
        struct hxs_window *win = &g_windows[i];
        if (win->id == 0) {
            continue;
        }
        int32_t x, y, w, h;
        hxwin_outer(win, &x, &y, &w, &h);
        /* Pull the outer rectangle back on screen, then translate the
         * correction onto the window's own origin. */
        int32_t dx = 0, dy = 0;
        if (x + w > (int32_t)g_screen_w) {
            dx = (int32_t)g_screen_w - (x + w);
        }
        if (y + h > (int32_t)g_screen_h) {
            dy = (int32_t)g_screen_h - (y + h);
        }
        if (x + dx < 0) {
            dx = -x;
        }
        if (y + dy < 0) {
            dy = -y;
        }
        win->x += dx;
        win->y += dy;
    }

    g_ptr_x = clamp_i32(g_ptr_x, 0, (int32_t)g_screen_w - 1);
    g_ptr_y = clamp_i32(g_ptr_y, 0, (int32_t)g_screen_h - 1);
    hxcomp_set_pointer(g_ptr_x, g_ptr_y);

    preempt_enable();

    repaint_rect(0, 0, (int32_t)g_screen_w, (int32_t)g_screen_h);

    for (int i = 0; i < HX_MAX_WINDOWS; i++) {
        struct hxs_window *win = &g_windows[i];
        if (win->id != 0 && win->mapped) {
            ev_send(win->owner, HX_EV_EXPOSE, win->id, 0, 0, win->w, win->h);
        }
    }
    return 0;
}

void highx_stop(void) {
    if (!g_active) {
        return;
    }
    preempt_disable();
    for (int i = 0; i < HX_MAX_WINDOWS; i++) {
        if (g_windows[i].id != 0 && g_windows[i].pix != NULL) {
            kfree(g_windows[i].pix);
        }
    }
    memset(g_windows, 0, sizeof(g_windows));
    memset(g_clients, 0, sizeof(g_clients));
    g_nstack = 0;
    g_focus = 0;
    g_wm_pid = 0;
    g_ptr_grab = 0;
    g_ptr_hold = 0;
    g_active = false;
    preempt_enable();

    /* Hand the screen back to the text console exactly as it was. */
    fb_set_graphics(false);
    fb_repaint();
}

bool highx_active(void) {
    return g_active;
}

void highx_client_exit(uint32_t pid) {
    if (!g_active) {
        return;
    }
    preempt_disable();
    /* A client that died mid-batch must not take the screen with
     * it. */
    if (g_batch_pid == pid) {
        batch_end();
    }
    for (int i = 0; i < HX_MAX_WINDOWS; i++) {
        if (g_windows[i].id != 0 && g_windows[i].owner == pid) {
            window_free(&g_windows[i]);
        }
    }
    struct hxs_client *c = client_find(pid);
    if (c != NULL) {
        if (c->is_wm) {
            g_wm_pid = 0;
        }
        memset(c, 0, sizeof(*c));
    }
    preempt_enable();
}

void highx_print_state(void) {
    if (!g_active) {
        kprintf("highX: not running (start it with `highx`)\n");
        return;
    }
    kprintf("highX %d.%d - screen %ux%u %ubpp\n", HIGHX_VERSION_MAJOR,
            HIGHX_VERSION_MINOR, g_screen_w, g_screen_h, g_screen_bpp);
    int clients = 0;
    for (int i = 0; i < HX_MAX_CLIENTS; i++) {
        if (g_clients[i].used) {
            clients++;
        }
    }
    kprintf("clients: %d   windows: %d   focus: %u   wm pid: %u\n",
            clients, g_nstack, g_focus, g_wm_pid);
    if (mouse_present()) {
        kprintf("pointer: %d,%d   buttons: 0x%x   packets: %llu\n",
                (int)g_ptr_x, (int)g_ptr_y, g_ptr_buttons,
                (unsigned long long)mouse_packet_count());
    } else {
        kprintf("pointer: no mouse\n");
    }
    for (int i = g_nstack - 1; i >= 0; i--) {
        struct hxs_window *w = g_stack[i];
        kprintf("  win %-3u pid %-3u %4dx%-4d at %4d,%-4d %-7s %s\n", w->id,
                w->owner, (int)w->w, (int)w->h, (int)w->x, (int)w->y,
                w->mapped ? "mapped" : "hidden", w->title);
    }
}

/* ---- request handling ---- */

/* One scratch copy of the request. Requests run with preemption
 * disabled, so a single static buffer is enough and keeps the 2.5 KiB
 * QUERY_TREE reply off the 16 KiB kernel stack. */
static union {
    struct hx_hello       hello;
    struct hx_window_req  create;
    struct hx_win_op      win;
    struct hx_configure   conf;
    struct hx_title       title;
    struct hx_rect_req    rect;
    struct hx_line_req    line;
    struct hx_text_req    text;
    struct hx_image_req   image;
    struct hx_commit      commit;
    struct hx_window_info info;
    struct hx_tree        tree;
    struct hx_grab        grab;
    struct hx_screen_info screen;
    struct hx_background  back;
    struct hx_border      border;
    struct hx_pointer     pointer;
    struct hx_batch batch;
} g_req;

/* Copy a request in, refusing anything the client sized wrong. */
static long req_in(void *arg, uint64_t len, uint64_t need) {
    if (arg == NULL || len < need) {
        return -EINVAL;
    }
    memcpy(&g_req, arg, (size_t)need);
    return 0;
}

/* True when `c` may act on `win`: owners always may, the window
 * manager may act on every window. */
static bool may_touch(const struct hxs_client *c, const struct hxs_window *w) {
    return w != NULL && (w->owner == c->pid || c->is_wm);
}

static long op_create_window(struct hxs_client *c) {
    struct hx_window_req *req = &g_req.create;
    if (req->w == 0 || req->h == 0 || req->w > g_screen_w ||
        req->h > g_screen_h) {
        return -EINVAL;
    }

    struct hxs_window *win = NULL;
    for (int i = 0; i < HX_MAX_WINDOWS; i++) {
        if (g_windows[i].id == 0) {
            win = &g_windows[i];
            break;
        }
    }
    if (win == NULL || g_nstack >= HX_MAX_WINDOWS) {
        return -ENOSPC;
    }

    uint32_t *pix = kmalloc((size_t)req->w * req->h * sizeof(uint32_t));
    if (pix == NULL) {
        return -ENOMEM;
    }

    memset(win, 0, sizeof(*win));
    win->id = g_next_id++;
    win->owner = c->pid;
    win->x = req->x;
    win->y = req->y;
    win->w = req->w;
    win->h = req->h;
    win->flags = req->flags;
    win->mapped = false;
    win->pix = pix;
    strncpy(win->title, req->title, HX_TITLE_MAX - 1);
    win->title[HX_TITLE_MAX - 1] = '\0';
    hxdraw_fill(win, 0, 0, (int32_t)win->w, (int32_t)win->h, req->background);

    if ((win->flags & HX_WF_DESKTOP) != 0) {
        stack_insert_bottom(win);
    } else {
        stack_insert_top(win);
    }

    ev_to_wm(HX_EV_CREATE_NOTIFY, win);
    req->win = win->id;
    return (long)win->id;
}

static long op_map_window(struct hxs_client *c, struct hxs_window *win) {
    if (win->mapped) {
        return 0;
    }
    /* Substructure redirection: with a WM registered, a client's map
     * becomes a request the WM answers by mapping the window itself. */
    if (g_wm_pid != 0 && !c->is_wm && (win->flags & HX_WF_OVERRIDE) == 0) {
        ev_to_wm(HX_EV_MAP_REQUEST, win);
        return 0;
    }

    win->mapped = true;
    repaint_window(win);
    ev_send(win->owner, HX_EV_EXPOSE, win->id, 0, 0, win->w, win->h);
    ev_to_wm(HX_EV_MAP_NOTIFY, win);
    if (focusable(win) && g_focus == 0) {
        set_focus(win->id);
    }
    return 0;
}

static long op_configure(struct hxs_client *c, struct hxs_window *win) {
    struct hx_configure *req = &g_req.conf;
    (void)c;

    int32_t ox, oy, ow, oh; /* the area the window vacates */
    hxwin_outer(win, &ox, &oy, &ow, &oh);
    uint32_t old_w = win->w, old_h = win->h;
    uint32_t nw = (req->mask & HX_CW_WIDTH) ? req->w : win->w;
    uint32_t nh = (req->mask & HX_CW_HEIGHT) ? req->h : win->h;
    if (nw == 0 || nh == 0 || nw > g_screen_w || nh > g_screen_h) {
        return -EINVAL;
    }

    if (nw != old_w || nh != old_h) {
        /* A resize gets a fresh backing store: the old contents are
         * not stretched, the client is told to repaint instead. */
        uint32_t *pix = kmalloc((size_t)nw * nh * sizeof(uint32_t));
        if (pix == NULL) {
            return -ENOMEM;
        }
        kfree(win->pix);
        win->pix = pix;
        win->w = nw;
        win->h = nh;
        hxdraw_fill(win, 0, 0, (int32_t)nw, (int32_t)nh, 0x00202830);
    }
    if (req->mask & HX_CW_X) {
        win->x = req->x;
    }
    if (req->mask & HX_CW_Y) {
        win->y = req->y;
    }

    if (win->mapped) {
        int32_t nx, ny, nnw, nnh;
        hxwin_outer(win, &nx, &ny, &nnw, &nnh);
        /* The area the window vacated and the one it now occupies. A
         * drag moves a window by a few pixels at a time, so the two
         * overlap almost entirely and are painted as one rectangle. */
        repaint_pair(ox, oy, ow, oh, nx, ny, nnw, nnh);
    }
    ev_send(win->owner, HX_EV_CONFIGURE, win->id, win->x, win->y, win->w,
            win->h);
    ev_send(win->owner, HX_EV_EXPOSE, win->id, 0, 0, win->w, win->h);
    return 0;
}

static long op_commit(struct hxs_window *win) {
    struct hx_commit *req = &g_req.commit;
    if (!win->mapped) {
        return 0;
    }
    if (req->w == 0 || req->h == 0) {
        repaint_window(win);
        return 0;
    }

    /* A commit says what changed in *this* window, so a rectangle
     * that runs past its edges is trimmed rather than repainted: a
     * client that commits its whole window after drawing one line
     * costs the window, never the screen. 64-bit arithmetic because
     * the width and height come from the client. */
    int32_t ox, oy, ow, oh;
    hxwin_outer(win, &ox, &oy, &ow, &oh);
    int64_t x0 = (int64_t)win->x + req->x;
    int64_t y0 = (int64_t)win->y + req->y;
    int64_t x1 = x0 + req->w;
    int64_t y1 = y0 + req->h;
    if (x0 < ox) {
        x0 = ox;
    }
    if (y0 < oy) {
        y0 = oy;
    }
    if (x1 > (int64_t)ox + ow) {
        x1 = (int64_t)ox + ow;
    }
    if (y1 > (int64_t)oy + oh) {
        y1 = (int64_t)oy + oh;
    }
    if (x0 >= x1 || y0 >= y1) {
        return 0;
    }
    repaint_rect((int32_t)x0, (int32_t)y0, (int32_t)(x1 - x0),
                 (int32_t)(y1 - y0));
    return 0;
}

static long op_query_tree(struct hxs_client *c, void *arg, uint64_t len) {
    if (arg == NULL || len < sizeof(struct hx_tree)) {
        return -EINVAL;
    }
    (void)c;
    /* Only the entries that exist are filled in and copied back: a
     * window manager polling the tree with two windows on screen must
     * not pay for a 2.5 KiB structure twice over. Each entry is
     * cleared before it is filled so no padding of the kernel's own
     * copy reaches userspace, and the client's tail is left as the
     * client left it - highAPI's hx_query_tree() zeroes the whole
     * structure before the call, and `count` says how far to read. */
    struct hx_tree *tree = &g_req.tree;
    tree->count = 0;
    tree->max = HX_MAX_WINDOWS;
    for (int i = 0; i < g_nstack && tree->count < HX_MAX_WINDOWS; i++) {
        struct hxs_window *w = g_stack[i];
        struct hx_window_info *info = &tree->win[tree->count++];
        memset(info, 0, sizeof(*info));
        info->win = w->id;
        info->owner = w->owner;
        info->x = w->x;
        info->y = w->y;
        info->w = w->w;
        info->h = w->h;
        info->flags = w->flags;
        info->mapped = w->mapped ? 1 : 0;
        info->focused = (w->id == g_focus) ? 1 : 0;
        info->border_w = w->border_w;
        info->border_color = w->border_color;
        memcpy(info->title, w->title, HX_TITLE_MAX);
    }
    memcpy(arg, tree, sizeof(*tree) - sizeof(tree->win) +
                          (size_t)tree->count * sizeof(tree->win[0]));
    return (long)tree->count;
}

/* HX_OP_NEXT_EVENT: the only request that may block. It runs outside
 * the preemption-disabled fast path so the rest of the system keeps
 * running while a client waits. */
static long op_next_event(uint32_t pid, void *arg, uint64_t len) {
    if (arg == NULL || len < sizeof(struct hx_next_event)) {
        return -EINVAL;
    }
    struct hx_next_event req;
    memcpy(&req, arg, sizeof(req));
    memset(&req.ev, 0, sizeof(req.ev));

    uint64_t start = pit_uptime_ms();
    for (;;) {
        bool got = false;
        preempt_disable();
        if (g_active) {
            /* A client about to wait has finished whatever it was
             * drawing, so this is the natural place to close a batch
             * - and the safety net that stops a forgotten end from
             * freezing the screen. */
            batch_end();
            pump_input();
            struct hxs_client *c = client_find(pid);
            got = ev_pop(c, &req.ev);
        }
        preempt_enable();

        if (got) {
            memcpy(arg, &req, sizeof(req));
            return 1;
        }
        if (!g_active) {
            /* The session ended under the client's feet: report it
             * as a close request so applications exit cleanly. */
            memset(&req.ev, 0, sizeof(req.ev));
            req.ev.type = HX_EV_CLOSE;
            memcpy(arg, &req, sizeof(req));
            return 1;
        }
        if (req.timeout_ms == 0) {
            memcpy(arg, &req, sizeof(req));
            return 0;
        }
        if (req.timeout_ms > 0 &&
            pit_uptime_ms() - start >= (uint64_t)req.timeout_ms) {
            memcpy(arg, &req, sizeof(req));
            return 0;
        }
        hlt(); /* woken by the PIT; other tasks run meanwhile */
    }
}

long highx_request(long op, void *arg, uint64_t len, bool from_user) {
    struct task *cur = sched_current();
    uint32_t pid = cur != NULL ? cur->pid : 0;

    if (!g_active) {
        return -ENODEV;
    }
    if (op == HX_OP_NEXT_EVENT) {
        return op_next_event(pid, arg, len);
    }

    preempt_disable();
    pump_input();

    long ret = -EINVAL;
    struct hxs_client *c = client_find(pid);

    /* HELLO opens the connection; everything else needs one. */
    if (op == HX_OP_HELLO) {
        ret = req_in(arg, len, sizeof(struct hx_hello));
        if (ret == 0) {
            if (g_req.hello.magic != HIGHX_MAGIC) {
                ret = -EINVAL;
            } else if ((g_req.hello.version >> 16) != HIGHX_VERSION_MAJOR) {
                ret = -EPROTONOSUPPORT;
            } else {
                c = client_add(pid);
                if (c == NULL) {
                    ret = -ENOSPC;
                } else {
                    g_req.hello.server_version = HIGHX_VERSION;
                    g_req.hello.client_id = pid;
                    g_req.hello.screen_w = g_screen_w;
                    g_req.hello.screen_h = g_screen_h;
                    g_req.hello.bpp = g_screen_bpp;
                    g_req.hello.is_wm = g_wm_pid != 0 ? 1 : 0;
                    memcpy(arg, &g_req.hello, sizeof(g_req.hello));
                    ret = 0;
                }
            }
        }
        preempt_enable();
        return ret;
    }

    if (c == NULL) {
        preempt_enable();
        return -ENOTCONN;
    }

    switch (op) {
    case HX_OP_BYE:
        if (g_batch_pid == pid) {
            batch_end();
        }
        if (c->is_wm) {
            g_wm_pid = 0;
        }
        for (int i = 0; i < HX_MAX_WINDOWS; i++) {
            if (g_windows[i].id != 0 && g_windows[i].owner == pid) {
                window_free(&g_windows[i]);
            }
        }
        memset(c, 0, sizeof(*c));
        ret = 0;
        break;

    case HX_OP_WM_REGISTER:
        if (g_wm_pid != 0 && g_wm_pid != pid) {
            ret = -EACCES; /* one window manager per display, like X11 */
        } else {
            g_wm_pid = pid;
            c->is_wm = true;
            ret = 0;
        }
        break;

    case HX_OP_GRAB_KEY:
        ret = req_in(arg, len, sizeof(struct hx_grab));
        if (ret == 0) {
            if (c->ngrabs >= HX_MAX_GRABS) {
                ret = -ENOSPC;
            } else {
                c->grabs[c->ngrabs].key = g_req.grab.key;
                c->grabs[c->ngrabs].mods = g_req.grab.mods & HX_MOD_MASK;
                c->ngrabs++;
                ret = 0;
            }
        }
        break;

    case HX_OP_CREATE_WINDOW:
        ret = req_in(arg, len, sizeof(struct hx_window_req));
        if (ret == 0) {
            ret = op_create_window(c);
            if (ret > 0) {
                memcpy(arg, &g_req.create, sizeof(g_req.create));
            }
        }
        break;

    case HX_OP_SCREEN_INFO:
        ret = req_in(arg, len, sizeof(struct hx_screen_info));
        if (ret == 0) {
            struct hx_screen_info *s = &g_req.screen;
            memset(s, 0, sizeof(*s));
            s->w = g_screen_w;
            s->h = g_screen_h;
            s->bpp = g_screen_bpp;
            s->windows = (uint32_t)g_nstack;
            s->focus = g_focus;
            s->wm = g_wm_pid;
            for (int i = 0; i < HX_MAX_CLIENTS; i++) {
                if (g_clients[i].used) {
                    s->clients++;
                }
            }
            memcpy(arg, s, sizeof(*s));
            ret = 0;
        }
        break;

    case HX_OP_SET_BACKGROUND:
        ret = req_in(arg, len, sizeof(struct hx_background));
        if (ret == 0) {
            hxcomp_set_background(g_req.back.color, g_req.back.style);
            repaint_rect(0, 0, (int32_t)g_screen_w, (int32_t)g_screen_h);
            ret = 0;
        }
        break;

    case HX_OP_QUERY_TREE:
        ret = op_query_tree(c, arg, len);
        break;

    /* ---- the pointer (v1.2) ---- */

    case HX_OP_QUERY_POINTER:
        ret = req_in(arg, len, sizeof(struct hx_pointer));
        if (ret == 0) {
            struct hxs_window *under = win_at(g_ptr_x, g_ptr_y);
            g_req.pointer.x = g_ptr_x;
            g_req.pointer.y = g_ptr_y;
            g_req.pointer.buttons = g_ptr_buttons;
            g_req.pointer.win = under != NULL ? under->id : 0;
            memcpy(arg, &g_req.pointer, sizeof(g_req.pointer));
            ret = 0;
        }
        break;

    /* Route every pointer event to one window until it is released
     * with .win == 0. A menu holds the pointer so that the click that
     * dismisses it is a click it sees, wherever it lands. */
    case HX_OP_GRAB_POINTER:
        ret = req_in(arg, len, sizeof(struct hx_win_op));
        if (ret == 0) {
            if (g_req.win.win == 0) {
                if (g_ptr_hold != 0) {
                    struct hxs_window *held = win_find(g_ptr_hold);
                    if (held != NULL && !may_touch(c, held)) {
                        ret = -EACCES;
                        break;
                    }
                }
                g_ptr_hold = 0;
                ret = 0;
            } else {
                struct hxs_window *win = win_find(g_req.win.win);
                if (win == NULL) {
                    ret = -ENOENT;
                } else if (!may_touch(c, win)) {
                    ret = -EACCES;
                } else if (g_ptr_hold != 0 && g_ptr_hold != win->id) {
                    ret = -EBUSY; /* somebody else already holds it */
                } else {
                    g_ptr_hold = win->id;
                    ret = 0;
                }
            }
        }
        break;

    case HX_OP_WARP_POINTER:
        ret = req_in(arg, len, sizeof(struct hx_pointer));
        if (ret == 0) {
            if (pointer_move_to(g_req.pointer.x, g_req.pointer.y)) {
                deliver_pointer(HX_PTR_MOTION, 0);
            }
            ret = 0;
        }
        break;

    case HX_OP_BATCH:
        ret = req_in(arg, len, sizeof(struct hx_batch));
        if (ret == 0) {
            if (g_req.batch.begin != 0) {
                /* A second batcher would have its damage flushed by
                 * the first one's end, which is not what it asked
                 * for. One at a time is the honest answer. */
                if (g_batch_pid != 0 && g_batch_pid != pid) {
                    ret = -EBUSY;
                } else {
                    g_batch_pid = pid;
                    ret = 0;
                }
            } else {
                if (g_batch_pid == pid) {
                    batch_end();
                }
                ret = 0;
            }
        }
        break;

    default:
        ret = -ENOSYS;
        break;
    }

    /* The switch above handles the requests that stand on their own.
     * -ENOSYS from its default case is the handoff to the second half:
     * every remaining opcode names a window, so they share the lookup
     * and the ownership check below instead of repeating them. */
    if (ret != -ENOSYS) {
        preempt_enable();
        return ret;
    }

    /* ---- requests that name a window ---- */

    ret = -EINVAL;
    struct hxs_window *win = NULL;

    switch (op) {
    case HX_OP_DESTROY_WINDOW:
    case HX_OP_MAP_WINDOW:
    case HX_OP_UNMAP_WINDOW:
    case HX_OP_RAISE_WINDOW:
    case HX_OP_LOWER_WINDOW:
    case HX_OP_SET_FOCUS:
    case HX_OP_CLOSE_WINDOW:
        ret = req_in(arg, len, sizeof(struct hx_win_op));
        win = ret == 0 ? win_find(g_req.win.win) : NULL;
        break;
    case HX_OP_CONFIGURE_WINDOW:
        ret = req_in(arg, len, sizeof(struct hx_configure));
        win = ret == 0 ? win_find(g_req.conf.win) : NULL;
        break;
    case HX_OP_SET_TITLE:
        ret = req_in(arg, len, sizeof(struct hx_title));
        win = ret == 0 ? win_find(g_req.title.win) : NULL;
        break;
    case HX_OP_SET_BORDER:
        ret = req_in(arg, len, sizeof(struct hx_border));
        win = ret == 0 ? win_find(g_req.border.win) : NULL;
        break;
    case HX_OP_FILL_RECT:
    case HX_OP_DRAW_RECT:
        ret = req_in(arg, len, sizeof(struct hx_rect_req));
        win = ret == 0 ? win_find(g_req.rect.win) : NULL;
        break;
    case HX_OP_DRAW_LINE:
        ret = req_in(arg, len, sizeof(struct hx_line_req));
        win = ret == 0 ? win_find(g_req.line.win) : NULL;
        break;
    case HX_OP_DRAW_TEXT:
        ret = req_in(arg, len, sizeof(struct hx_text_req));
        win = ret == 0 ? win_find(g_req.text.win) : NULL;
        break;
    case HX_OP_PUT_IMAGE:
        ret = req_in(arg, len, sizeof(struct hx_image_req));
        win = ret == 0 ? win_find(g_req.image.win) : NULL;
        break;
    case HX_OP_COMMIT:
        ret = req_in(arg, len, sizeof(struct hx_commit));
        win = ret == 0 ? win_find(g_req.commit.win) : NULL;
        break;
    case HX_OP_GET_WINDOW:
        ret = req_in(arg, len, sizeof(struct hx_window_info));
        win = ret == 0 ? win_find(g_req.info.win) : NULL;
        break;
    default:
        preempt_enable();
        return -ENOSYS;
    }

    if (ret != 0) {
        preempt_enable();
        return ret;
    }
    if (win == NULL) {
        preempt_enable();
        return -ENOENT;
    }
    if (!may_touch(c, win)) {
        preempt_enable();
        return -EACCES;
    }

    switch (op) {
    case HX_OP_DESTROY_WINDOW:
        window_free(win);
        ret = 0;
        break;

    case HX_OP_MAP_WINDOW:
        ret = op_map_window(c, win);
        break;

    case HX_OP_UNMAP_WINDOW:
        if (win->mapped) {
            win->mapped = false;
            repaint_window(win);
            ev_to_wm(HX_EV_UNMAP_NOTIFY, win);
            if (g_focus == win->id) {
                g_focus = 0;
                focus_topmost();
            }
        }
        ret = 0;
        break;

    case HX_OP_CONFIGURE_WINDOW:
        ret = op_configure(c, win);
        break;

    case HX_OP_RAISE_WINDOW: {
        /* Raising a window that is already on top must not repaint
         * it: a window manager re-raises its windows on every layout
         * pass, and a full repaint per window is exactly what makes
         * that feel sluggish. */
        bool already_top = g_nstack > 0 && g_stack[g_nstack - 1] == win;
        stack_remove(win);
        if ((win->flags & HX_WF_DESKTOP) != 0) {
            stack_insert_bottom(win);
        } else {
            stack_insert_top(win);
        }
        if (win->mapped && !already_top) {
            repaint_window(win);
        }
        ret = 0;
        break;
    }

    case HX_OP_LOWER_WINDOW:
        stack_remove(win);
        stack_insert_bottom(win);
        if (win->mapped) {
            repaint_rect(win->x, win->y, (int32_t)win->w, (int32_t)win->h);
        }
        ret = 0;
        break;

    case HX_OP_SET_BORDER: {
        uint32_t width = g_req.border.width;
        if (width > 16) {
            ret = -EINVAL;
            break;
        }
        bool changed = width != win->border_w ||
                       (g_req.border.color & 0x00FFFFFFu) !=
                           (win->border_color & 0x00FFFFFFu);
        uint32_t old_w = win->border_w;
        win->border_w = width;
        win->border_color = g_req.border.color;
        if (win->mapped && changed) {
            if (old_w > width) {
                /* The border shrank: clean up the ring it vacated. */
                struct hxs_window tmp = *win;
                tmp.border_w = old_w;
                int32_t x, y, w, h;
                hxwin_outer(&tmp, &x, &y, &w, &h);
                repaint_rect(x, y, w, h);
            } else {
                repaint_window(win);
            }
        }
        ret = 0;
        break;
    }

    case HX_OP_SET_TITLE:
        strncpy(win->title, g_req.title.title, HX_TITLE_MAX - 1);
        win->title[HX_TITLE_MAX - 1] = '\0';
        ret = 0;
        break;

    case HX_OP_SET_FOCUS:
        if (focusable(win)) {
            set_focus(win->id);
            ret = 0;
        } else {
            ret = -EINVAL;
        }
        break;

    case HX_OP_CLOSE_WINDOW:
        ev_send(win->owner, HX_EV_CLOSE, win->id, 0, 0, 0, 0);
        ret = 0;
        break;

    case HX_OP_FILL_RECT:
        hxdraw_fill(win, g_req.rect.x, g_req.rect.y, (int32_t)g_req.rect.w,
                    (int32_t)g_req.rect.h, g_req.rect.color);
        ret = 0;
        break;

    case HX_OP_DRAW_RECT:
        hxdraw_rect(win, g_req.rect.x, g_req.rect.y, (int32_t)g_req.rect.w,
                    (int32_t)g_req.rect.h, g_req.rect.color);
        ret = 0;
        break;

    case HX_OP_DRAW_LINE:
        hxdraw_line(win, g_req.line.x0, g_req.line.y0, g_req.line.x1,
                    g_req.line.y1, g_req.line.color);
        ret = 0;
        break;

    case HX_OP_DRAW_TEXT:
        g_req.text.text[HX_TEXT_MAX - 1] = '\0';
        hxdraw_text(win, g_req.text.x, g_req.text.y, g_req.text.fg,
                    g_req.text.bg, g_req.text.flags, g_req.text.text);
        ret = 0;
        break;

    case HX_OP_PUT_IMAGE: {
        struct hx_image_req *im = &g_req.image;
        uint64_t bytes = (uint64_t)im->w * im->h * sizeof(uint32_t);
        if (im->w == 0 || im->h == 0 || bytes > 16u * 1024u * 1024u) {
            ret = -EINVAL;
        } else if (!user_ptr_ok(from_user, im->pixels, bytes)) {
            ret = -EFAULT;
        } else {
            hxdraw_image(win, im->x, im->y, im->w, im->h,
                         (const uint32_t *)(uintptr_t)im->pixels);
            ret = 0;
        }
        break;
    }

    case HX_OP_COMMIT:
        ret = op_commit(win);
        break;

    case HX_OP_GET_WINDOW: {
        struct hx_window_info *info = &g_req.info;
        memset(info, 0, sizeof(*info));
        info->win = win->id;
        info->owner = win->owner;
        info->x = win->x;
        info->y = win->y;
        info->w = win->w;
        info->h = win->h;
        info->flags = win->flags;
        info->mapped = win->mapped ? 1 : 0;
        info->focused = (win->id == g_focus) ? 1 : 0;
        info->border_w = win->border_w;
        info->border_color = win->border_color;
        memcpy(info->title, win->title, HX_TITLE_MAX);
        memcpy(arg, info, sizeof(*info));
        ret = 0;
        break;
    }

    default:
        ret = -ENOSYS;
        break;
    }

    preempt_enable();
    return ret;
}
