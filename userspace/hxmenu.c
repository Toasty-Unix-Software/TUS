/*
 * hxmenu - the highX application launcher (Super+D)
 *
 * A rofi-style runner: one centred window, a filter line and a list
 * of matches. Type to narrow the list, Up/Down to pick, Enter to
 * launch, Escape to give up.
 *
 * hxmenu is a plain highX client, not part of the window manager. It
 * asks for HX_WF_OVERRIDE so the window manager does not tile it,
 * takes the keyboard for itself and exits as soon as it has done its
 * job - which is why it can be replaced with any other launcher
 * without touching tusWM: the window manager only knows how to spawn
 * /bin/hxmenu.
 *
 * The entries come from /etc/highx/menu ("Name:/path" per line, '#'
 * starts a comment); if that file is missing a built-in list is used,
 * so the launcher always works.
 */

#include "highapi/highapi.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ENTRIES 24
#define NAME_MAX_E  40
#define PATH_MAX_E  96
#define VISIBLE     8
#define ROW_H       22
#define PAD         12

#define MENU_W 520
#define HEADER_H 62

/* Monochrome, matching tusWM/tusDE's chrome (2026-08-23) - see
 * tuswm.c's COL_ACCENT comment for why this exact white and not some
 * other light grey: it has to stay far enough from the compositor's
 * cursor colour (0xF0F0F0) for tests/test_highx.py's +-6 colour
 * search to tell them apart. */
#define COL_BG      0x00161616u
#define COL_EDGE    0x00FFFFFFu
#define COL_FG      0x00E0E0E0u
#define COL_DIM     0x00868686u
#define COL_SEL_BG  0x00FFFFFFu
#define COL_SEL_FG  0x00101010u
#define COL_INPUT   0x00202020u

struct entry {
    char name[NAME_MAX_E];
    char path[PATH_MAX_E];
};

static struct hx_display dpy;
static unsigned int win;
static unsigned int win_w = MENU_W;
static unsigned int win_h = HEADER_H + VISIBLE * ROW_H + PAD;

static struct entry g_all[MAX_ENTRIES];
static int g_count;
static int g_match[MAX_ENTRIES];
static int g_matches;
static int g_sel;
static char g_filter[NAME_MAX_E];
static int g_flen;

/* ---- entries ---- */

static void add_entry(const char *name, const char *path) {
    if (g_count >= MAX_ENTRIES) {
        return;
    }
    snprintf(g_all[g_count].name, NAME_MAX_E, "%s", name);
    snprintf(g_all[g_count].path, PATH_MAX_E, "%s", path);
    g_count++;
}

static void load_builtin(void) {
    add_entry("Terminal", "/bin/hxterm");
    add_entry("Clock", "/bin/hxclock");
    add_entry("Demo", "/bin/hxdemo");
}

/* /etc/highx/menu: "Name:/path" per line. Read with plain read() into
 * one buffer - the file is a few hundred bytes. */
static void load_menu_file(void) {
    int fd = open("/etc/highx/menu", O_RDONLY);
    if (fd < 0) {
        load_builtin();
        return;
    }
    char buf[1024];
    long n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n <= 0) {
        load_builtin();
        return;
    }
    buf[n] = '\0';

    char *line = buf;
    while (line != NULL && *line != '\0') {
        char *end = strchr(line, '\n');
        if (end != NULL) {
            *end = '\0';
        }
        while (*line == ' ' || *line == '\t') {
            line++;
        }
        if (*line != '\0' && *line != '#') {
            char *sep = strchr(line, ':');
            if (sep != NULL) {
                *sep = '\0';
                add_entry(line, sep + 1);
            }
        }
        line = end != NULL ? end + 1 : NULL;
    }
    if (g_count == 0) {
        load_builtin();
    }
}

/* ---- filtering ---- */

static int matches_filter(const struct entry *e) {
    if (g_flen == 0) {
        return 1;
    }
    /* Case-insensitive substring, over the name and the path. */
    for (const char *hay = e->name; ; hay = e->path) {
        size_t hl = strlen(hay);
        for (size_t i = 0; i + (size_t)g_flen <= hl; i++) {
            int ok = 1;
            for (int j = 0; j < g_flen; j++) {
                char a = hay[i + (size_t)j];
                char b = g_filter[j];
                if (a >= 'A' && a <= 'Z') {
                    a = (char)(a - 'A' + 'a');
                }
                if (b >= 'A' && b <= 'Z') {
                    b = (char)(b - 'A' + 'a');
                }
                if (a != b) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                return 1;
            }
        }
        if (hay == e->path) {
            return 0;
        }
    }
}

static void refilter(void) {
    g_matches = 0;
    for (int i = 0; i < g_count; i++) {
        if (matches_filter(&g_all[i])) {
            g_match[g_matches++] = i;
        }
    }
    if (g_sel >= g_matches) {
        g_sel = g_matches > 0 ? g_matches - 1 : 0;
    }
}

/* ---- drawing ---- */

static void redraw(void) {
    char line[HX_TEXT_MAX];

    hx_fill(win, 0, 0, win_w, win_h, COL_BG);
    hx_rect(win, 0, 0, win_w, win_h, COL_EDGE);

    hx_text(win, PAD, PAD, COL_EDGE, 0, 0, "Run a program");
    snprintf(line, sizeof(line), "%d of %d", g_matches, g_count);
    hx_text(win, (int)win_w - PAD - (int)strlen(line) * HX_FONT_W, PAD,
            COL_DIM, 0, 0, line);

    /* Filter line with a block cursor. */
    int iy = PAD + 22;
    hx_fill(win, PAD, iy, win_w - 2 * PAD, 24, COL_INPUT);
    hx_text(win, PAD + 8, iy + 4, COL_FG, COL_INPUT, 0, g_filter);
    hx_fill(win, PAD + 8 + g_flen * HX_FONT_W, iy + 4, 2, HX_FONT_H, COL_EDGE);

    int first = 0;
    if (g_sel >= VISIBLE) {
        first = g_sel - VISIBLE + 1;
    }
    for (int row = 0; row < VISIBLE; row++) {
        int idx = first + row;
        int y = HEADER_H + row * ROW_H;
        if (idx >= g_matches) {
            break;
        }
        struct entry *e = &g_all[g_match[idx]];
        int selected = idx == g_sel;
        if (selected) {
            hx_fill(win, PAD / 2, y, win_w - PAD, ROW_H - 2, COL_SEL_BG);
        }
        hx_text(win, PAD + 4, y + 3, selected ? COL_SEL_FG : COL_FG, 0, 0,
                e->name);
        int px = PAD + 4 + 18 * HX_FONT_W;
        hx_text(win, px, y + 3, selected ? COL_SEL_FG : COL_DIM, 0, 0,
                e->path);
    }
    if (g_matches == 0) {
        hx_text(win, PAD + 4, HEADER_H + 4, COL_DIM, 0, 0, "no match");
    }

    hx_commit(win);
}

/* ---- main ---- */

static void launch_selected(void) {
    if (g_matches == 0) {
        return;
    }
    struct entry *e = &g_all[g_match[g_sel]];
    char *argv[2];
    argv[0] = e->path;
    argv[1] = 0;
    hx_spawn(e->path, argv);
}

int main(void) {
    if (hx_open(&dpy) < 0) {
        printf("hxmenu: no highX display\n");
        return 1;
    }
    load_menu_file();
    refilter();

    if (win_w > dpy.screen_w) {
        win_w = dpy.screen_w;
    }
    if (win_h > dpy.screen_h) {
        win_h = dpy.screen_h;
    }
    int x = ((int)dpy.screen_w - (int)win_w) / 2;
    int y = (int)dpy.screen_h / 5;

    /* Override-redirect: the launcher places itself and the window
     * manager leaves it alone. */
    win = hx_create_window(x, y, win_w, win_h,
                           HX_WF_NODECOR | HX_WF_OVERRIDE, COL_BG, "hxmenu");
    if (win == 0) {
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);
    hx_raise(win);
    hx_focus(win);
    redraw();

    for (;;) {
        struct hx_event ev;
        if (hx_next_event(&ev, -1) <= 0) {
            continue;
        }
        if (ev.type == HX_EV_CLOSE) {
            break;
        }
        if (ev.type == HX_EV_EXPOSE) {
            redraw();
            continue;
        }
        if (ev.type != HX_EV_KEY) {
            continue;
        }

        unsigned int key = ev.key;
        if (key == 0x1B) { /* Escape */
            break;
        }
        if (key == '\n' || key == '\r') {
            launch_selected();
            break;
        }
        if (key == HX_KEY_UP) {
            if (g_sel > 0) {
                g_sel--;
            }
        } else if (key == HX_KEY_DOWN) {
            if (g_sel + 1 < g_matches) {
                g_sel++;
            }
        } else if (key == '\b' || key == 0x7F) {
            if (g_flen > 0) {
                g_filter[--g_flen] = '\0';
                g_sel = 0;
                refilter();
            }
        } else if (key >= 0x20 && key < 0x7F && g_flen + 1 < NAME_MAX_E) {
            g_filter[g_flen++] = (char)key;
            g_filter[g_flen] = '\0';
            g_sel = 0;
            refilter();
        } else {
            continue;
        }
        redraw();
    }

    hx_destroy_window(win);
    hx_close(&dpy);
    return 0;
}
