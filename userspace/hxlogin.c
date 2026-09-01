/*
 * hxlogin - the greeter a highX session starts with
 *
 * A desktop that starts logged in is a desktop that never asked who
 * you are. hxlogin is the small program that does: a full-screen
 * window with two fields, checked against /etc/shadow with the same
 * crypt() the console's `login` uses, and nothing else on screen
 * until they check out.
 *
 * It is also the session's LEADER, which is what makes logging out
 * work: `highx --de` runs hxlogin, hxlogin runs the desktop and waits
 * for it, and when the desktop exits the greeter comes back instead
 * of the session ending. Ending the session is a separate, deliberate
 * act (the button that says so), as is turning the machine off.
 *
 * An account with no password - `!` or empty in /etc/shadow, which is
 * what a fresh TUS image has - is let in with an empty password, and
 * the greeter says so rather than pretending there was a check.
 */

#include "highapi/highapi.h"

#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SYS_POWER        53
#define TUS_POWER_HALT    0
#define TUS_POWER_REBOOT  1

#define _PATH_SHADOW "/etc/shadow"
#define _PATH_PASSWD "/etc/passwd"

/* The card, and the palette it is drawn in: a dark greeter, because
 * it is the first thing on screen after a boot log. */
#define CARD_W      460
#define CARD_H      300
#define FIELD_W     (CARD_W - 80)
#define FIELD_H     30
#define BTN_W       120
#define BTN_H       32

#define COL_BG_TOP    0x000B1220u
#define COL_BG_BOT    0x00182A44u
#define COL_CARD      0x00131C2Bu
#define COL_CARD_EDGE 0x00223349u
#define COL_TEXT      0x00E8EEF6u
#define COL_MUTED     0x008A9AB0u
#define COL_ACCENT    0x004FA3D1u
#define COL_FIELD     0x000C1420u
#define COL_ERROR     0x00E06060u
#define COL_OK        0x0050D0A0u

#define NAME_MAX_T 64

static struct hx_display dpy;
static unsigned int win;
static int scr_w = 800, scr_h = 600;

static char g_user[NAME_MAX_T] = "root";
static char g_pass[NAME_MAX_T];
static int  g_field;            /* 0 = user, 1 = password */
static char g_message[128];
static int  g_message_error;
static int  g_hover_btn = -1;

/* What to start once the credentials check out. */
static const char *g_session = "/bin/tusde";
static const char *g_session_name = "tusDE";

enum { BTN_NONE = -1, BTN_LOGIN = 0, BTN_CONSOLE, BTN_REBOOT, BTN_HALT,
       BTN_COUNT };

static long tus_power(long op) {
    long ret;
    /* Only RAX survives the trap; every argument register is declared
     * read-write (see musl's tus_syscall.c). */
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = op;
    register long rsi __asm__("rsi") = 0;
    register long rdx __asm__("rdx") = 0;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"((long)SYS_POWER)
                     : "rcx", "r11", "memory");
    return ret;
}

/* ---- accounts ---- */

/* The hash for `user`, or NULL when the account does not exist. An
 * empty string or "!" means "no password set". */
static const char *shadow_hash(const char *user) {
    FILE *f = fopen(_PATH_SHADOW, "r");
    if (f == NULL) {
        return NULL;
    }
    static char buf[512];
    const char *hit = NULL;
    while (fgets(buf, sizeof(buf), f) != NULL) {
        char *c1 = strchr(buf, ':');
        if (c1 == NULL) {
            continue;
        }
        *c1 = '\0';
        if (strcmp(buf, user) != 0) {
            continue;
        }
        char *h = c1 + 1;
        char *c2 = strchr(h, ':');
        if (c2 != NULL) {
            *c2 = '\0';
        }
        hit = h;
        break;
    }
    fclose(f);
    return hit;
}

static int user_exists(const char *user) {
    FILE *f = fopen(_PATH_PASSWD, "r");
    if (f == NULL) {
        return 0;
    }
    char buf[512];
    int found = 0;
    while (fgets(buf, sizeof(buf), f) != NULL) {
        char *c = strchr(buf, ':');
        if (c == NULL) {
            continue;
        }
        *c = '\0';
        if (strcmp(buf, user) == 0) {
            found = 1;
            break;
        }
    }
    fclose(f);
    return found;
}

/* 1 on success. `why` receives the reason a refusal was a refusal. */
static int authenticate(const char *user, const char *pass, const char **why) {
    if (!user_exists(user)) {
        *why = "no such user";
        return 0;
    }
    const char *hash = shadow_hash(user);
    if (hash == NULL || hash[0] == '\0' || strcmp(hash, "!") == 0) {
        /* No password has been set for this account. A fresh image is
         * exactly this, so say it instead of refusing everyone. */
        if (pass[0] != '\0') {
            *why = "this account has no password - leave it empty";
            return 0;
        }
        *why = "no password set for this account";
        return 1;
    }
    char *c = crypt(pass, hash);
    if (c == NULL || strcmp(c, hash) != 0) {
        *why = "wrong password";
        return 0;
    }
    *why = NULL;
    return 1;
}

/* ---- geometry ---- */

static int card_x(void) { return (scr_w - CARD_W) / 2; }
static int card_y(void) { return (scr_h - CARD_H) / 2 - 20; }

static void field_rect(int which, int *x, int *y, int *w, int *h) {
    *x = card_x() + 40;
    *y = card_y() + 96 + which * (FIELD_H + 34);
    *w = FIELD_W;
    *h = FIELD_H;
}

static void button_rect(int which, int *x, int *y, int *w, int *h) {
    *w = BTN_W;
    *h = BTN_H;
    if (which == BTN_LOGIN) {
        *x = card_x() + CARD_W - 40 - BTN_W;
        *y = card_y() + CARD_H - 30 - BTN_H;
        return;
    }
    /* The three session buttons live along the bottom of the screen,
     * away from the fields: nothing there is part of logging in. */
    *w = 130;
    *h = 28;
    *x = scr_w - 3 * (130 + 10) + (which - BTN_CONSOLE) * (130 + 10) - 16;
    *y = scr_h - 28 - 16;
}

static int button_at(int px, int py) {
    for (int i = 0; i < BTN_COUNT; i++) {
        int x, y, w, h;
        button_rect(i, &x, &y, &w, &h);
        if (px >= x && py >= y && px < x + w && py < y + h) {
            return i;
        }
    }
    return BTN_NONE;
}

/* ---- drawing ---- */

static void draw_button(int which, const char *label, unsigned int ink) {
    int x, y, w, h;
    button_rect(which, &x, &y, &w, &h);
    unsigned int face = which == g_hover_btn ? 0x00243449u : 0x001A2637u;
    if (which == BTN_LOGIN) {
        face = which == g_hover_btn ? 0x005FB4E2u : COL_ACCENT;
        ink = 0x000B1220u;
    }
    hx_fill(win, x, y, (unsigned)w, (unsigned)h, face);
    hx_rect(win, x, y, (unsigned)w, (unsigned)h, COL_CARD_EDGE);
    int tx = x + (w - (int)strlen(label) * HX_FONT_W) / 2;
    hx_text(win, tx, y + (h - HX_FONT_H) / 2, ink, 0, 0, label);
}

static void draw_field(int which, const char *label, const char *text,
                       int secret) {
    int x, y, w, h;
    field_rect(which, &x, &y, &w, &h);

    hx_text(win, x, y - 20, COL_MUTED, 0, 0, label);
    hx_fill(win, x, y, (unsigned)w, (unsigned)h, COL_FIELD);
    unsigned int edge = which == g_field ? COL_ACCENT : COL_CARD_EDGE;
    hx_rect(win, x, y, (unsigned)w, (unsigned)h, edge);
    if (which == g_field) {
        hx_rect(win, x - 1, y - 1, (unsigned)w + 2, (unsigned)h + 2, edge);
    }

    char shown[NAME_MAX_T + 1];
    if (secret) {
        size_t n = strlen(text);
        if (n > NAME_MAX_T - 1) {
            n = NAME_MAX_T - 1;
        }
        for (size_t i = 0; i < n; i++) {
            shown[i] = '*';
        }
        shown[n] = '\0';
    } else {
        snprintf(shown, sizeof(shown), "%s", text);
    }
    int room = (w - 20) / HX_FONT_W;
    if ((int)strlen(shown) > room && room > 0) {
        memmove(shown, shown + strlen(shown) - room, (size_t)room + 1);
    }
    hx_text(win, x + 10, y + (h - HX_FONT_H) / 2, COL_TEXT, 0, 0, shown);

    if (which == g_field) {
        int caret = x + 10 + (int)strlen(shown) * HX_FONT_W;
        hx_fill(win, caret, y + 7, 1, FIELD_H - 14, COL_ACCENT);
    }
}

static void draw(void) {
    hx_batch_begin();

    /* The background is a gradient, drawn as one filled band per few
     * rows: the server has no gradient primitive, and a login screen
     * on a flat colour looks like a crash. */
    for (int y = 0; y < scr_h; y += 4) {
        int t = y * 255 / (scr_h > 0 ? scr_h : 1);
        unsigned int r = (((COL_BG_TOP >> 16) & 0xFF) * (255 - t) +
                          ((COL_BG_BOT >> 16) & 0xFF) * t) / 255;
        unsigned int g = (((COL_BG_TOP >> 8) & 0xFF) * (255 - t) +
                          ((COL_BG_BOT >> 8) & 0xFF) * t) / 255;
        unsigned int b = ((COL_BG_TOP & 0xFF) * (255 - t) +
                          (COL_BG_BOT & 0xFF) * t) / 255;
        hx_fill(win, 0, y, (unsigned)scr_w, 4, (r << 16) | (g << 8) | b);
    }

    int cx = card_x(), cy = card_y();
    hx_fill(win, cx, cy, CARD_W, CARD_H, COL_CARD);
    hx_rect(win, cx, cy, CARD_W, CARD_H, COL_CARD_EDGE);
    hx_fill(win, cx, cy, CARD_W, 3, COL_ACCENT);

    hx_text(win, cx + 40, cy + 34, COL_TEXT, 0, 0, "Toasty Unix Software");
    char sub[96];
    snprintf(sub, sizeof(sub), "log in to start %s", g_session_name);
    hx_text(win, cx + 40, cy + 56, COL_MUTED, 0, 0, sub);

    draw_field(0, "User", g_user, 0);
    draw_field(1, "Password", g_pass, 1);

    /* The message goes on a line of its own, above the button row:
     * "this account has no password - leave it empty" is longer than
     * the space beside a button. */
    if (g_message[0] != '\0') {
        char shown[64];
        snprintf(shown, sizeof(shown), "%s", g_message);
        int room = (CARD_W - 80) / HX_FONT_W;
        if (room > 1 && (int)strlen(shown) > room) {
            shown[room] = '\0';
        }
        hx_text(win, cx + 40, cy + CARD_H - 42 - BTN_H,
                g_message_error ? COL_ERROR : COL_OK, 0, 0, shown);
    }
    draw_button(BTN_LOGIN, "Log in", COL_TEXT);
    draw_button(BTN_CONSOLE, "Exit to console", COL_TEXT);
    draw_button(BTN_REBOOT, "Reboot", COL_TEXT);
    draw_button(BTN_HALT, "Shut down", COL_TEXT);

    hx_text(win, 16, scr_h - 24, COL_MUTED, 0, 0,
            "Tab switches fields   Enter logs in");

    hx_commit(win);
    hx_batch_end();
}

/* ---- the session ---- */

/* Run the desktop and wait for it. The greeter's window goes away
 * while it runs and comes back when it ends, which is what makes
 * "log out" mean something. */
static void run_session(void) {
    char *argv[2];
    argv[0] = (char *)g_session;
    argv[1] = NULL;

    hx_unmap(win);
    int pid = hx_spawn(g_session, argv);
    if (pid < 0) {
        hx_map(win);
        snprintf(g_message, sizeof(g_message), "%s: cannot start",
                 g_session);
        g_message_error = 1;
        draw();
        return;
    }

    int status = 0;
    waitpid(pid, &status, 0);

    hx_map(win);
    hx_raise(win);
    hx_focus(win);
    memset(g_pass, 0, sizeof(g_pass));
    g_field = 1;
    snprintf(g_message, sizeof(g_message), "session ended - log in again");
    g_message_error = 0;
    draw();
}

static void try_login(void) {
    const char *why = NULL;
    if (authenticate(g_user, g_pass, &why)) {
        snprintf(g_message, sizeof(g_message), "welcome, %s", g_user);
        g_message_error = 0;
        draw();
        run_session();
        return;
    }
    snprintf(g_message, sizeof(g_message), "%s",
             why != NULL ? why : "login incorrect");
    g_message_error = 1;
    memset(g_pass, 0, sizeof(g_pass));
    g_field = 1;
    draw();
}

/* ---- input ---- */

static int g_running = 1;

static void on_key(const struct hx_event *ev) {
    unsigned int k = ev->key;
    char *field = g_field == 0 ? g_user : g_pass;

    if (k == '\t') {
        g_field = !g_field;
        draw();
        return;
    }
    if (k == '\n' || k == '\r') {
        if (g_field == 0) {
            g_field = 1;
            draw();
            return;
        }
        try_login();
        return;
    }
    if (k == 0x11) { /* Ctrl+Q, as everywhere else in highX */
        g_running = 0;
        return;
    }
    if (k == '\b' || k == 0x7F) {
        size_t n = strlen(field);
        if (n > 0) {
            field[n - 1] = '\0';
        }
        draw();
        return;
    }
    if (k >= 0x20 && k < 0x7F) {
        size_t n = strlen(field);
        if (n + 1 < NAME_MAX_T) {
            field[n] = (char)k;
            field[n + 1] = '\0';
        }
        draw();
    }
}

static void on_press(const struct hx_event *ev) {
    int b = button_at(ev->x, ev->y);
    if (b == BTN_LOGIN) {
        try_login();
        return;
    }
    if (b == BTN_CONSOLE) {
        g_running = 0;
        return;
    }
    if (b == BTN_REBOOT) {
        tus_power(TUS_POWER_REBOOT);
        return;
    }
    if (b == BTN_HALT) {
        tus_power(TUS_POWER_HALT);
        return;
    }

    for (int i = 0; i < 2; i++) {
        int x, y, w, h;
        field_rect(i, &x, &y, &w, &h);
        if (ev->x >= x && ev->y >= y && ev->x < x + w && ev->y < y + h) {
            g_field = i;
            draw();
            return;
        }
    }
}

static void on_motion(const struct hx_event *ev) {
    int b = button_at(ev->x, ev->y);
    if (b != g_hover_btn) {
        g_hover_btn = b;
        draw();
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && (strcmp(argv[1], "--wm") == 0 ||
                     strcmp(argv[1], "wm") == 0)) {
        g_session = "/bin/tuswm";
        g_session_name = "tusWM";
    }

    if (hx_open(&dpy) != 0) {
        printf("hxlogin: no highX display (start one with `highx`)\n");
        return 1;
    }

    struct hx_screen_info info;
    if (hx_screen_info(&info) == 0 && info.w > 0 && info.h > 0) {
        scr_w = (int)info.w;
        scr_h = (int)info.h;
    }

    /* The greeter owns the screen: no decoration, no window manager
     * to ask (there is none yet - the desktop it starts is the one). */
    win = hx_create_window(0, 0, (unsigned)scr_w, (unsigned)scr_h,
                           HX_WF_NODECOR | HX_WF_OVERRIDE, COL_BG_TOP,
                           "TUS login");
    if (win == 0) {
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);
    hx_focus(win);

    g_field = strlen(g_user) > 0 ? 1 : 0;
    snprintf(g_message, sizeof(g_message), "%s", "");
    draw();

    while (g_running) {
        struct hx_event ev;
        if (hx_wait_event(&ev) <= 0) {
            continue;
        }
        if (ev.type == HX_EV_CLOSE) {
            break;
        }
        if (ev.type == HX_EV_EXPOSE) {
            draw();
        } else if (ev.type == HX_EV_KEY) {
            on_key(&ev);
        } else if (ev.type == HX_EV_POINTER) {
            if (ev.detail == HX_PTR_PRESS) {
                on_press(&ev);
            } else if (ev.detail == HX_PTR_MOTION) {
                on_motion(&ev);
            }
        }
    }

    hx_destroy_window(win);
    hx_close(&dpy);
    return 0;
}
