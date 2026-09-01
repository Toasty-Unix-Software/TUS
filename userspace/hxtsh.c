/*
 * hxtsh - a terminal window running the kernel's own shell
 *
 * hxterm, the terminal TUS shipped first, had a shell inside it: it
 * tokenised the line itself, ran its own built-ins and spawned
 * programs. That works, but it means `ls` in a window and `ls` at the
 * console are two different `ls`, and the kernel's built-ins - which
 * print with kprintf and never touch a file descriptor - could not
 * appear in a window at all.
 *
 * hxtsh has no shell in it. It asks the kernel for a TERMINAL SESSION
 * (include/tusterm.h): the kernel starts a real tsh as a ring-0 task,
 * captures everything that shell and its children print, and reads
 * their keys from the session instead of the PS/2 keyboard. What is
 * left here is what a terminal actually is:
 *
 *   - a grid of characters with colors and a scrollback;
 *   - an ANSI/VT100 parser, because that is what the stream carries
 *     (the console's colors arrive as SGR, kilo's redraws as CSI);
 *   - a keyboard that turns key events back into the bytes a
 *     terminal sends (arrows as ESC [ A, Enter as CR);
 *   - a window that keeps answering while a program is running.
 *
 * So `ls`, `ps`, `sysinfo`, `cd`, pipelines, redirection, the command
 * history, kilo - all of it is the kernel's, and hxtsh only draws it.
 */

#include "highapi/highapi.h"

#include <tusterm.h>

#include <stdlib.h>
#include <string.h>

#define MAX_COLS    200
#define MAX_ROWS    64
#define SCROLLBACK  480
#define PAD         6

/* The palette. Index 0 is the default ink, 1 the default paper; both
 * are entries in the intern table below, so a cell only ever carries
 * one byte per colour. */
#define COL_BG      0x000C1218u
#define COL_FG      0x00C8D4E0u
#define COL_CURSOR  0x004FA3D1u
#define COL_STATUS  0x00182430u

#define SYS_TERM 52

/* ---- the system call ---- */

static long term_call(long op, void *arg, unsigned long len) {
    long ret;
    /* The trap returns with only RAX preserved (see the ABI note in
     * musl's tus_syscall.c), so every argument register is declared
     * read-write - including the three this call does not use. */
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = op;
    register long rsi __asm__("rsi") = (long)arg;
    register long rdx __asm__("rdx") = (long)len;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"((long)SYS_TERM)
                     : "rcx", "r11", "memory");
    return ret;
}

/* ---- the grid ---- */

struct cell {
    unsigned char ch;
    unsigned char fg;
    unsigned char bg;
};

static struct cell g_screen[MAX_ROWS][MAX_COLS];
static struct cell g_saved[MAX_ROWS][MAX_COLS];  /* the alternate screen */
static struct cell g_back[SCROLLBACK][MAX_COLS]; /* what scrolled off */
static int g_back_count;   /* lines in the scrollback */
static int g_back_head;    /* ring position of the next line to write */
static int g_view;         /* how far back the window is looking */

static unsigned char g_dirty[MAX_ROWS];
static int g_cols = 80, g_rows = 24;
static int g_cur_r, g_cur_c;
static int g_cursor_on = 1;
static int g_alt;          /* the alternate screen is showing */
static int g_saved_r, g_saved_c;

/* Interned colours: a cell carries an index, not a pixel value, so
 * the grid stays three bytes wide even with truecolour SGR. */
static unsigned int g_palette[256] = { COL_FG, COL_BG };
static int g_palette_n = 2;

static unsigned char g_fg = 0, g_bg = 1;
static int g_bold, g_reverse;

static struct hx_display dpy;
static unsigned int win;
static unsigned int win_w = 760, win_h = 480;
static unsigned int g_session;
static unsigned int g_shell_pid;

static unsigned char color_index(unsigned int rgb);

/* Paper. A terminal draws "the default background" as its own, and
 * the kernel console's default background is plain black - taking it
 * literally leaves a window in two tones, black where the shell has
 * written and the terminal's own paper everywhere else. Black means
 * paper here; a program that wants a black box can still have one by
 * asking for a colour that is not the default. */
static unsigned char paper_index(unsigned int rgb) {
    return rgb == 0x000000u ? 1 : color_index(rgb);
}

static unsigned char color_index(unsigned int rgb) {
    for (int i = 0; i < g_palette_n; i++) {
        if (g_palette[i] == rgb) {
            return (unsigned char)i;
        }
    }
    if (g_palette_n >= 256) {
        return 0;
    }
    g_palette[g_palette_n] = rgb;
    return (unsigned char)g_palette_n++;
}

static void row_clear(struct cell *row, int from, int to) {
    for (int c = from; c < to && c < MAX_COLS; c++) {
        row[c].ch = ' ';
        row[c].fg = g_fg;
        row[c].bg = g_bg;
    }
}

static void grid_clear(void) {
    for (int r = 0; r < MAX_ROWS; r++) {
        row_clear(g_screen[r], 0, MAX_COLS);
        g_dirty[r] = 1;
    }
    g_cur_r = g_cur_c = 0;
}

static void mark_all_dirty(void) {
    for (int r = 0; r < MAX_ROWS; r++) {
        g_dirty[r] = 1;
    }
}

/*
 * The top line leaves the screen. On the main screen it is kept: a
 * terminal whose output scrolls past faster than it can be read is
 * only useful if it remembers. The alternate screen (a full-screen
 * program) is not kept - that is the point of it.
 */
static void scroll_up(void) {
    if (!g_alt) {
        memcpy(g_back[g_back_head], g_screen[0],
               sizeof(struct cell) * MAX_COLS);
        g_back_head = (g_back_head + 1) % SCROLLBACK;
        if (g_back_count < SCROLLBACK) {
            g_back_count++;
        }
        /* Looking at the scrollback while output arrives: hold the
         * view on the same text instead of letting it slide. */
        if (g_view > 0 && g_view < SCROLLBACK) {
            g_view++;
        }
    }
    for (int r = 1; r < g_rows; r++) {
        memcpy(g_screen[r - 1], g_screen[r], sizeof(struct cell) * MAX_COLS);
    }
    row_clear(g_screen[g_rows - 1], 0, MAX_COLS);
    mark_all_dirty();
}

/* Row `r` of the window: the live screen, or a line out of the
 * scrollback when the view has been pulled back. */
static const struct cell *visible_row(int r) {
    if (g_view <= 0 || r >= g_view) {
        return g_screen[r - (g_view > 0 ? g_view : 0)];
    }
    /* The oldest line the view reaches, walked forward. */
    int from_end = g_view - r;                /* 1 = the newest kept line */
    int idx = (g_back_head - from_end + SCROLLBACK * 2) % SCROLLBACK;
    return g_back[idx];
}

static void newline(void) {
    g_cur_c = 0;
    g_cur_r++;
    if (g_cur_r >= g_rows) {
        g_cur_r = g_rows - 1;
        scroll_up();
    }
    g_dirty[g_cur_r] = 1;
}

static void put_char(char c) {
    if (g_cur_c >= g_cols) {
        newline();
    }
    struct cell *cell = &g_screen[g_cur_r][g_cur_c];
    cell->ch = (unsigned char)c;
    cell->fg = g_reverse ? g_bg : g_fg;
    cell->bg = g_reverse ? g_fg : g_bg;
    g_cur_c++;
    g_dirty[g_cur_r] = 1;
}

/* ---- the ANSI parser ----
 *
 * Three states are enough for what TUS produces: text, an escape
 * that has just started, and a CSI with its parameters. OSC (the
 * window title) is swallowed up to its terminator.
 */
enum { ST_TEXT, ST_ESC, ST_CSI, ST_OSC };
static int g_state = ST_TEXT;
static int g_params[8];
static int g_nparams;
static int g_private;      /* the '?' of ESC [ ? 25 l */
static char g_osc[HX_TITLE_MAX];
static int g_osc_len;

/* The 16 ANSI colours, which is what a program that is not the
 * kernel console will ask for. */
static const unsigned int ANSI16[16] = {
    0x00000000u, 0x00CC3333u, 0x0033AA55u, 0x00CCAA33u,
    0x004488DDu, 0x00AA55CCu, 0x0033AAAAu, 0x00C8D4E0u,
    0x00555555u, 0x00FF6666u, 0x0066DD88u, 0x00FFDD66u,
    0x0077AAFFu, 0x00DD88FFu, 0x0066DDDDu, 0x00FFFFFFu,
};

static int param(int i, int def) {
    if (i >= g_nparams || g_params[i] < 0) {
        return def;
    }
    return g_params[i];
}

static void erase_display(int mode) {
    if (mode == 2 || mode == 3) {
        for (int r = 0; r < g_rows; r++) {
            row_clear(g_screen[r], 0, g_cols);
        }
    } else if (mode == 1) {
        for (int r = 0; r < g_cur_r; r++) {
            row_clear(g_screen[r], 0, g_cols);
        }
        row_clear(g_screen[g_cur_r], 0, g_cur_c + 1);
    } else {
        row_clear(g_screen[g_cur_r], g_cur_c, g_cols);
        for (int r = g_cur_r + 1; r < g_rows; r++) {
            row_clear(g_screen[r], 0, g_cols);
        }
    }
    mark_all_dirty();
}

static void erase_line(int mode) {
    if (mode == 1) {
        row_clear(g_screen[g_cur_r], 0, g_cur_c + 1);
    } else if (mode == 2) {
        row_clear(g_screen[g_cur_r], 0, g_cols);
    } else {
        row_clear(g_screen[g_cur_r], g_cur_c, g_cols);
    }
    g_dirty[g_cur_r] = 1;
}

static void sgr(void) {
    if (g_nparams == 0) {
        g_params[0] = 0;
        g_nparams = 1;
    }
    for (int i = 0; i < g_nparams; i++) {
        int p = param(i, 0);
        if (p == 0) {
            g_fg = 0;
            g_bg = 1;
            g_bold = g_reverse = 0;
        } else if (p == 1) {
            g_bold = 1;
        } else if (p == 7) {
            g_reverse = 1;
        } else if (p == 22) {
            g_bold = 0;
        } else if (p == 27) {
            g_reverse = 0;
        } else if (p >= 30 && p <= 37) {
            g_fg = color_index(ANSI16[(p - 30) + (g_bold ? 8 : 0)]);
        } else if (p >= 40 && p <= 47) {
            g_bg = paper_index(ANSI16[p - 40]);
        } else if (p >= 90 && p <= 97) {
            g_fg = color_index(ANSI16[(p - 90) + 8]);
        } else if (p >= 100 && p <= 107) {
            g_bg = color_index(ANSI16[(p - 100) + 8]);
        } else if (p == 39) {
            g_fg = 0;
        } else if (p == 49) {
            g_bg = 1;
        } else if ((p == 38 || p == 48) && param(i + 1, 0) == 2) {
            /* 38;2;R;G;B - what the kernel console sends. */
            unsigned int rgb = ((unsigned)param(i + 2, 0) << 16) |
                               ((unsigned)param(i + 3, 0) << 8) |
                               (unsigned)param(i + 4, 0);
            if (p == 38) {
                g_fg = color_index(rgb);
            } else {
                g_bg = paper_index(rgb);
            }
            i += 4;
        } else if ((p == 38 || p == 48) && param(i + 1, 0) == 5) {
            int n = param(i + 2, 0);
            unsigned int rgb;
            if (n < 16) {
                rgb = ANSI16[n & 15];
            } else if (n < 232) {
                /* The 6x6x6 cube. */
                int v = n - 16;
                unsigned int r = (unsigned)(v / 36) * 51;
                unsigned int g = (unsigned)((v / 6) % 6) * 51;
                unsigned int b = (unsigned)(v % 6) * 51;
                rgb = (r << 16) | (g << 8) | b;
            } else {
                unsigned int g = (unsigned)((n - 232) * 10 + 8);
                rgb = (g << 16) | (g << 8) | g;
            }
            if (p == 38) {
                g_fg = color_index(rgb);
            } else {
                g_bg = paper_index(rgb);
            }
            i += 2;
        }
    }
}

static void term_send(const char *s, int len);

static void csi(char final) {
    switch (final) {
    case 'A': g_cur_r -= param(0, 1); break;
    case 'B': g_cur_r += param(0, 1); break;
    case 'C': g_cur_c += param(0, 1); break;
    case 'D': g_cur_c -= param(0, 1); break;
    case 'E': g_cur_r += param(0, 1); g_cur_c = 0; break;
    case 'F': g_cur_r -= param(0, 1); g_cur_c = 0; break;
    case 'G': g_cur_c = param(0, 1) - 1; break;
    case 'd': g_cur_r = param(0, 1) - 1; break;
    case 'H':
    case 'f':
        g_cur_r = param(0, 1) - 1;
        g_cur_c = param(1, 1) - 1;
        break;
    case 'J': erase_display(param(0, 0)); break;
    case 'K': erase_line(param(0, 0)); break;
    case 'm': sgr(); break;
    case 'n':
        if (param(0, 0) == 6) {
            /* A program asking where the cursor is - which is how a
             * program without TIOCGWINSZ measures the window. */
            char reply[24];
            int n = 0;
            int r = g_cur_r + 1, c = g_cur_c + 1;
            reply[n++] = 0x1B;
            reply[n++] = '[';
            if (r >= 10) reply[n++] = (char)('0' + r / 10);
            reply[n++] = (char)('0' + r % 10);
            reply[n++] = ';';
            if (c >= 100) reply[n++] = (char)('0' + c / 100);
            if (c >= 10) reply[n++] = (char)('0' + (c / 10) % 10);
            reply[n++] = (char)('0' + c % 10);
            reply[n++] = 'R';
            term_send(reply, n);
        }
        break;
    case 'h':
    case 'l': {
        int set = (final == 'h');
        if (g_private && param(0, 0) == 25) {
            g_cursor_on = set;
            g_dirty[g_cur_r] = 1;
        } else if (g_private && (param(0, 0) == 1049 || param(0, 0) == 47 ||
                                 param(0, 0) == 1047)) {
            /* The alternate screen: a full-screen program gets a
             * clean grid and gives back what was underneath. */
            if (set && !g_alt) {
                memcpy(g_saved, g_screen, sizeof(g_screen));
                g_saved_r = g_cur_r;
                g_saved_c = g_cur_c;
                g_alt = 1;
                grid_clear();
            } else if (!set && g_alt) {
                memcpy(g_screen, g_saved, sizeof(g_screen));
                g_cur_r = g_saved_r;
                g_cur_c = g_saved_c;
                g_alt = 0;
                mark_all_dirty();
            }
        }
        break;
    }
    case 's': g_saved_r = g_cur_r; g_saved_c = g_cur_c; break;
    case 'u': g_cur_r = g_saved_r; g_cur_c = g_saved_c; break;
    default:
        break; /* everything else is ignored, not printed */
    }

    if (g_cur_r < 0) g_cur_r = 0;
    if (g_cur_r >= g_rows) g_cur_r = g_rows - 1;
    if (g_cur_c < 0) g_cur_c = 0;
    if (g_cur_c > g_cols) g_cur_c = g_cols;
    g_dirty[g_cur_r] = 1;
}

static void feed(char c) {
    switch (g_state) {
    case ST_ESC:
        if (c == '[') {
            g_state = ST_CSI;
            g_nparams = 0;
            g_private = 0;
            g_params[0] = -1;
        } else if (c == ']') {
            g_state = ST_OSC;
            g_osc_len = 0;
        } else if (c == 'c') {
            grid_clear();
            g_state = ST_TEXT;
        } else {
            g_state = ST_TEXT; /* ESC 7, ESC (B and friends: ignored */
        }
        return;

    case ST_CSI:
        if (c == '?' || c == '>' || c == '!') {
            g_private = 1;
            return;
        }
        if (c >= '0' && c <= '9') {
            if (g_nparams == 0) {
                g_nparams = 1;
                g_params[0] = 0;
            }
            if (g_params[g_nparams - 1] < 0) {
                g_params[g_nparams - 1] = 0;
            }
            g_params[g_nparams - 1] =
                g_params[g_nparams - 1] * 10 + (c - '0');
            return;
        }
        if (c == ';') {
            if (g_nparams == 0) {
                g_nparams = 1;
                g_params[0] = -1;
            }
            if (g_nparams < (int)(sizeof(g_params) / sizeof(g_params[0]))) {
                g_params[g_nparams++] = -1;
            }
            return;
        }
        csi(c);
        g_state = ST_TEXT;
        return;

    case ST_OSC:
        /* ESC ] 0 ; title BEL  (or ST). The title is the window's. */
        if (c == 0x07 || c == 0x1B) {
            g_osc[g_osc_len] = '\0';
            const char *title = g_osc;
            while (*title != '\0' && *title != ';') {
                title++;
            }
            if (*title == ';') {
                hx_set_title(win, title + 1);
            }
            g_state = ST_TEXT;
            return;
        }
        if (g_osc_len < (int)sizeof(g_osc) - 1) {
            g_osc[g_osc_len++] = c;
        }
        return;

    default:
        break;
    }

    switch (c) {
    case 0x1B: g_state = ST_ESC; return;
    case '\n': newline(); return;
    case '\r': g_cur_c = 0; g_dirty[g_cur_r] = 1; return;
    case '\b':
        /* The console's backspace is destructive: it erases as it
         * moves, which is what the shell's line editor expects. */
        if (g_cur_c > 0) {
            g_cur_c--;
            g_screen[g_cur_r][g_cur_c].ch = ' ';
            g_dirty[g_cur_r] = 1;
        }
        return;
    case '\t': {
        int next = (g_cur_c + 8) & ~7;
        while (g_cur_c < next && g_cur_c < g_cols) {
            put_char(' ');
        }
        return;
    }
    case 0x07: return; /* the bell has nowhere to ring */
    default:
        break;
    }
    if ((unsigned char)c < 0x20) {
        return;
    }
    put_char(c);
}

/* ---- painting ---- */

static void paint_row(int r) {
    int y = PAD + r * HX_FONT_H;
    const struct cell *row = visible_row(r);

    hx_fill(win, 0, y, win_w, HX_FONT_H, g_palette[1]);

    /* One request per run of cells that share their colours, which
     * makes a line of one colour one call. */
    int c = 0;
    while (c < g_cols) {
        int end = c;
        while (end < g_cols && row[end].fg == row[c].fg &&
               row[end].bg == row[c].bg && end - c < HX_TEXT_MAX - 1) {
            end++;
        }
        int last = end;
        while (last > c && row[last - 1].ch == ' ' &&
               row[last - 1].bg == 1) {
            last--; /* trailing blanks on the default paper: skip */
        }
        if (last > c) {
            char chunk[HX_TEXT_MAX];
            int n = 0;
            for (int i = c; i < last; i++) {
                chunk[n++] = (char)row[i].ch;
            }
            chunk[n] = '\0';
            unsigned int bg = g_palette[row[c].bg];
            hx_text(win, PAD + c * HX_FONT_W, y, g_palette[row[c].fg], bg,
                    row[c].bg == 1 ? 0 : HX_TF_OPAQUE, chunk);
        }
        c = end;
    }

    if (g_cursor_on && g_view == 0 && r == g_cur_r) {
        int cx = PAD + g_cur_c * HX_FONT_W;
        hx_fill(win, cx, y + HX_FONT_H - 2, HX_FONT_W, 2, COL_CURSOR);
    }
    hx_commit_rect(win, 0, y, win_w, HX_FONT_H);
}

/* Only the lines that changed are repainted, and each is committed on
 * its own - typing a character costs one 8x16 update, not a window. */
static void flush(void) {
    for (int r = 0; r < g_rows; r++) {
        if (g_dirty[r]) {
            g_dirty[r] = 0;
            paint_row(r);
        }
    }
}

static void redraw_all(void) {
    hx_fill(win, 0, 0, win_w, win_h, g_palette[1]);
    hx_commit(win);
    mark_all_dirty();
    flush();
}

static void resize(unsigned int w, unsigned int h) {
    win_w = w;
    win_h = h;
    g_cols = (int)(win_w - 2 * PAD) / HX_FONT_W;
    g_rows = (int)(win_h - 2 * PAD) / HX_FONT_H;
    if (g_cols > MAX_COLS) g_cols = MAX_COLS;
    if (g_rows > MAX_ROWS) g_rows = MAX_ROWS;
    if (g_cols < 8) g_cols = 8;
    if (g_rows < 2) g_rows = 2;
    if (g_cur_r >= g_rows) g_cur_r = g_rows - 1;
    if (g_cur_c >= g_cols) g_cur_c = g_cols - 1;

    /* The shell and every program in the window must agree with what
     * is on screen: TIOCGWINSZ reads this back. */
    struct term_size sz = { g_session, (unsigned)g_cols, (unsigned)g_rows };
    term_call(TERM_OP_RESIZE, &sz, sizeof(sz));

    redraw_all();
}

/* ---- talking to the session ---- */

static void term_send(const char *s, int len) {
    struct term_io io;
    io.id = g_session;
    io.len = (unsigned)len;
    io.buf = (void *)s;
    term_call(TERM_OP_WRITE, &io, sizeof(io));
}

/* Everything the shell printed since the last look. Returns the
 * number of bytes taken, so the caller knows whether to repaint. */
static int term_drain(void) {
    char buf[2048];
    int total = 0;
    for (;;) {
        struct term_io io;
        io.id = g_session;
        io.len = sizeof(buf);
        io.buf = buf;
        long n = term_call(TERM_OP_READ, &io, sizeof(io));
        if (n <= 0) {
            break;
        }
        for (long i = 0; i < n; i++) {
            feed(buf[i]);
        }
        total += (int)n;
        if (n < (long)sizeof(buf)) {
            break;
        }
    }
    return total;
}

/* ---- keys ---- */

/* One codepoint as UTF-8. Returns the byte count, or 0 for a value
 * that must not be encoded. */
static int utf8_encode(unsigned int cp, char *out) {
    if (cp < 0x80) {
        out[0] = (char)cp;
        return 1;
    }
    if (cp < 0x800) {
        out[0] = (char)(0xC0 | (cp >> 6));
        out[1] = (char)(0x80 | (cp & 0x3F));
        return 2;
    }
    if (cp >= 0xD800 && cp <= 0xDFFF) {
        return 0;
    }
    if (cp < 0x10000) {
        out[0] = (char)(0xE0 | (cp >> 12));
        out[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[2] = (char)(0x80 | (cp & 0x3F));
        return 3;
    }
    if (cp <= 0x10FFFF) {
        out[0] = (char)(0xF0 | (cp >> 18));
        out[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
        out[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[3] = (char)(0x80 | (cp & 0x3F));
        return 4;
    }
    return 0;
}

static void send_key(const struct hx_event *ev) {
    static const struct {
        unsigned int key;
        const char *seq;
    } special[] = {
        { HX_KEY_UP,        "\x1b[A" },
        { HX_KEY_DOWN,      "\x1b[B" },
        { HX_KEY_RIGHT,     "\x1b[C" },
        { HX_KEY_LEFT,      "\x1b[D" },
        { HX_KEY_HOME,      "\x1b[H" },
        { HX_KEY_END,       "\x1b[F" },
        { HX_KEY_DELETE,    "\x1b[3~" },
        { HX_KEY_INSERT,    "\x1b[2~" },
        { HX_KEY_PAGE_UP,   "\x1b[5~" },
        { HX_KEY_PAGE_DOWN, "\x1b[6~" },
    };

    /* Typing means "show me the bottom", as it does in every
     * terminal: an answer nobody can see is no answer. */
    if (g_view != 0) {
        g_view = 0;
        mark_all_dirty();
    }

    /* Shift+PageUp/PageDown walk the scrollback instead of reaching
     * the program, which is the one binding a terminal keeps for
     * itself. */
    if ((ev->mods & HX_MOD_SHIFT) != 0 &&
        (ev->key == HX_KEY_PAGE_UP || ev->key == HX_KEY_PAGE_DOWN)) {
        int step = g_rows - 1;
        g_view += ev->key == HX_KEY_PAGE_UP ? step : -step;
        if (g_view > g_back_count) g_view = g_back_count;
        if (g_view < 0) g_view = 0;
        mark_all_dirty();
        return;
    }

    for (unsigned i = 0; i < sizeof(special) / sizeof(special[0]); i++) {
        if (ev->key == special[i].key) {
            term_send(special[i].seq, (int)strlen(special[i].seq));
            return;
        }
    }

    if (ev->key == '\n' || ev->key == '\r') {
        term_send("\r", 1); /* a terminal sends CR; the tty maps it */
        return;
    }

    /* Anything else that types is a Unicode codepoint (v1.5), and a
     * terminal carries bytes - so it goes down the pipe as UTF-8. The
     * shell reads bytes, the console draws codepoints, and this is
     * the seam between them. */
    if (HX_KEY_IS_CHAR(ev->key)) {
        char enc[4];
        int n = utf8_encode(ev->key, enc);
        if (n > 0) {
            term_send(enc, n);
        }
    }
}

static void wheel(int steps) {
    g_view += steps * 3;
    if (g_view > g_back_count) g_view = g_back_count;
    if (g_view < 0) g_view = 0;
    mark_all_dirty();
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    if (hx_open(&dpy) != 0) {
        return 1;
    }

    win = hx_create_window(120, 90, win_w, win_h, 0, COL_BG, "tsh");
    if (win == 0) {
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);

    g_cols = (int)(win_w - 2 * PAD) / HX_FONT_W;
    g_rows = (int)(win_h - 2 * PAD) / HX_FONT_H;
    grid_clear();

    struct term_open req;
    memset(&req, 0, sizeof(req));
    req.magic = TERM_MAGIC;
    req.cols = (unsigned)g_cols;
    req.rows = (unsigned)g_rows;
    if (term_call(TERM_OP_OPEN, &req, sizeof(req)) != 0) {
        const char *msg = "hxtsh: the kernel would not start a shell\n";
        while (*msg != '\0') {
            feed(*msg++);
        }
        flush();
        hx_wait_event(NULL);
        hx_close(&dpy);
        return 1;
    }
    g_session = req.id;
    g_shell_pid = req.shell_pid;
    redraw_all();

    for (;;) {
        struct hx_event ev;
        /* A short wait, not a block: the session has no descriptor to
         * poll on, so the window looks for output as often as it
         * would repaint anyway. */
        int have = hx_next_event(&ev, 16) > 0;

        if (term_drain() > 0) {
            flush();
        }

        if (have) {
            if (ev.type == HX_EV_CLOSE) {
                break;
            } else if (ev.type == HX_EV_EXPOSE) {
                redraw_all();
            } else if (ev.type == HX_EV_CONFIGURE) {
                if (ev.w > 0 && ev.h > 0 &&
                    (ev.w != win_w || ev.h != win_h)) {
                    resize(ev.w, ev.h);
                }
            } else if (ev.type == HX_EV_KEY) {
                send_key(&ev);
                flush();
            } else if (ev.type == HX_EV_POINTER &&
                       ev.detail == HX_PTR_WHEEL) {
                wheel((int)(int32_t)ev.key);
                flush();
            }
        }

        /* The shell exited (`exit`, or a fault): so does the window.
         * A session the kernel no longer knows about (-ENOENT, after
         * its slot was reclaimed) counts as gone too - otherwise the
         * window would sit there talking to nothing. */
        struct term_status st;
        memset(&st, 0, sizeof(st));
        st.id = g_session;
        long rc = term_call(TERM_OP_STATUS, &st, sizeof(st));
        if (rc < 0) {
            break;
        }
        if (!st.alive && st.pending == 0) {
            break;
        }
    }

    struct term_id id = { g_session };
    term_call(TERM_OP_CLOSE, &id, sizeof(id));
    hx_destroy_window(win);
    hx_close(&dpy);
    return 0;
}
