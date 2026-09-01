/*
 * hxfiles - the TUS file manager
 *
 * A window over the VFS: one directory at a time, a row per entry,
 * and the four things anyone actually does with files - look at the
 * tree, open something, make a directory, throw something away.
 *
 * It is a plain highX client (highAPI only, no toolkit): the list is
 * drawn row by row with hx_fill/hx_text, and the only clever part is
 * that everything it knows comes from one system call the C library
 * does not wrap - SYS_READDIR, whose ABI is TUS's own (one entry per
 * call into a fixed structure, see kernel/vfs/vfs.h).
 *
 * Opening something means asking what it is:
 *
 *   a directory        walk into it
 *   an MP4             hand it to hxvideo, which is the program for it
 *   an executable      run it (that is what an executable is for)
 *   anything else      show it here, as text or as a hex dump when
 *                      it is not text - so a file manager can answer
 *                      "what IS this file" without a second program
 *
 * The pointer works the way a pointer works everywhere else: the
 * wheel scrolls the list, the scrollbar can be dragged, a click
 * selects and a click on what is already selected opens it.
 */

#include "highapi/highapi.h"

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* TUS's readdir has its own ABI (kernel/vfs/vfs.h): one entry per
 * call into a fixed-size structure. musl has no wrapper for it, so
 * hxfiles calls it directly - the same `int $0x80` ABI highAPI uses. */
#define SYS_READDIR 11
#define SYS_MKDIR    9
#define SYS_UNLINK  10

#define TUS_DIR    1
#define TUS_FILE   2
#define TUS_DEVICE 3
#define TUS_SOCKET 4

struct tus_dirent {
    char     name[64];
    unsigned type;
    unsigned size;
    unsigned mode;
};

static long tus_syscall(long n, long a1, long a2, long a3) {
    long ret;
    /* The trap returns with only RAX preserved (see the ABI note in
     * musl's tus_syscall.c), so every argument register is declared
     * read-write - including the three this ABI does not use. Leaving
     * them out lets the compiler keep a live value in r8 across the
     * call, which it does. */
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"(n)
                     : "rcx", "r11", "memory");
    return ret;
}

/* ---- furniture ---- */

#define BAR_H       42
#define STATUS_H    22
#define ROW_H       20
#define PAD         12
#define BTN_SIZE    28
#define SCROLLBAR_W 12
#define THUMB_MIN   24
#define ICON_W      18

#define COL_CHROME  0x00F1F3F4u
#define COL_PAGE    0x00FFFFFFu
#define COL_BORDER  0x00DADCE0u
#define COL_TEXT    0x00202124u
#define COL_MUTED   0x005F6368u
#define COL_ACCENT  0x001A73E8u
#define COL_SELECT  0x00E8F0FEu
#define COL_HOVER   0x00E3E5E8u
#define COL_STATUS  0x00F8F9FAu
#define COL_FOLDER  0x00F9AB00u
#define COL_FILE    0x00BDC1C6u
#define COL_EXEC    0x00188038u
#define COL_DEVICE  0x00A142F4u
#define COL_ERROR   0x00D93025u
#define COL_TRACK   0x00F1F3F4u
#define COL_THUMB   0x00BDC1C6u
#define COL_THUMB_HOT 0x008E9296u

#define MAX_ENTRIES 512
#define PATH_MAX_T  256
#define VIEW_MAX    (64u * 1024)

struct entry {
    char     name[64];
    unsigned type;
    unsigned size;
    unsigned mode;
};

static struct hx_display dpy;
static unsigned int win;
static int win_w = 780, win_h = 520;

static char g_path[PATH_MAX_T] = "/";
static struct entry g_entries[MAX_ENTRIES];
static int g_count;
static int g_sel;
static int g_scroll;       /* first visible row */
static int g_hover = -1;
static char g_status[256];
static int g_status_error;

/* What the window is showing: the listing, a file, or a question. */
enum { MODE_LIST, MODE_VIEW, MODE_CONFIRM, MODE_NEWDIR };
static int g_mode = MODE_LIST;

/* The file viewer. */
static char *g_view_buf;
static int   g_view_len;
static int   g_view_scroll;
static int   g_view_hex;
static char  g_view_name[PATH_MAX_T];

/* The dialog's text field (a new directory's name). */
static char g_input[64];
static int  g_input_len;

/* The toolbar. */
enum { BTN_NONE = -1, BTN_UP = 0, BTN_HOME, BTN_REFRESH, BTN_NEWDIR,
       BTN_DELETE, BTN_TERMINAL, BTN_COUNT };
static int g_hover_btn = BTN_NONE;

/* The scrollbar. */
static int g_hover_thumb;
static int g_drag_thumb;
static int g_drag_grab_y;

static void set_status(int error, const char *fmt, ...);
static void redraw(void);
static void load_dir(const char *path);

/* ---- geometry ---- */

static int list_top(void) {
    return BAR_H;
}

static int list_h(void) {
    int h = win_h - BAR_H - STATUS_H;
    return h > 0 ? h : 0;
}

static int rows_visible(void) {
    int r = list_h() / ROW_H;
    return r > 0 ? r : 1;
}

static int content_h(void) {
    return g_count * ROW_H;
}

static void button_rect(int which, int *x, int *y, int *size) {
    *size = BTN_SIZE;
    *y = (BAR_H - BTN_SIZE) / 2;
    *x = PAD / 2 + which * (BTN_SIZE + 4);
}

static int button_at(int x, int y) {
    for (int i = 0; i < BTN_COUNT; i++) {
        int bx, by, size;
        button_rect(i, &bx, &by, &size);
        if (x >= bx && y >= by && x < bx + size && y < by + size) {
            return i;
        }
    }
    return BTN_NONE;
}

/*
 * The scrollbar's geometry, in one place so painting it and grabbing
 * it cannot disagree. Returns 0 when everything fits.
 */
static int scrollbar_metrics(int *track_y, int *track_h, int *thumb_y,
                             int *thumb_h) {
    int view = list_h();
    int total = content_h();
    if (view <= 0 || total <= view) {
        return 0;
    }
    int th = view * view / total;
    if (th < THUMB_MIN) th = THUMB_MIN;
    if (th > view) th = view;

    int span = view - th;
    int max_row = g_count - rows_visible();
    if (max_row < 1) max_row = 1;
    int at = span * g_scroll / max_row;
    if (at > span) at = span;

    if (track_y != NULL) *track_y = list_top();
    if (track_h != NULL) *track_h = view;
    if (thumb_y != NULL) *thumb_y = list_top() + at;
    if (thumb_h != NULL) *thumb_h = th;
    return 1;
}

/* ---- paths ---- */

static void path_join(const char *dir, const char *name, char *out,
                      size_t outsz) {
    if (strcmp(dir, "/") == 0) {
        snprintf(out, outsz, "/%s", name);
    } else {
        snprintf(out, outsz, "%s/%s", dir, name);
    }
}

static void path_parent(const char *path, char *out, size_t outsz) {
    snprintf(out, outsz, "%s", path);
    char *slash = NULL;
    for (char *p = out; *p != '\0'; p++) {
        if (*p == '/') slash = p;
    }
    if (slash == NULL || slash == out) {
        snprintf(out, outsz, "/");
        return;
    }
    *slash = '\0';
}

static const char *extension(const char *name) {
    const char *dot = NULL;
    for (const char *p = name; *p != '\0'; p++) {
        if (*p == '.') dot = p;
    }
    return dot != NULL ? dot + 1 : "";
}

static int ext_is(const char *name, const char *ext) {
    const char *e = extension(name);
    while (*e != '\0' && *ext != '\0') {
        char a = *e >= 'A' && *e <= 'Z' ? (char)(*e + 32) : *e;
        if (a != *ext) return 0;
        e++;
        ext++;
    }
    return *e == '\0' && *ext == '\0';
}

/* ---- reading a directory ---- */

static int entry_before(const struct entry *a, const struct entry *b) {
    int adir = a->type == TUS_DIR, bdir = b->type == TUS_DIR;
    if (adir != bdir) {
        return adir; /* directories first, as every file manager does */
    }
    return strcmp(a->name, b->name) < 0;
}

static void load_dir(const char *path) {
    char resolved[PATH_MAX_T];
    snprintf(resolved, sizeof(resolved), "%s", path);

    int fd = open(resolved, O_RDONLY);
    if (fd < 0) {
        set_status(1, "%s: cannot open", resolved);
        return;
    }

    static struct entry fresh[MAX_ENTRIES];
    int n = 0;
    struct tus_dirent ent;
    while (n < MAX_ENTRIES &&
           tus_syscall(SYS_READDIR, fd, (long)&ent, sizeof(ent)) > 0) {
        if (strcmp(ent.name, ".") == 0 || strcmp(ent.name, "..") == 0) {
            continue;
        }
        snprintf(fresh[n].name, sizeof(fresh[n].name), "%s", ent.name);
        fresh[n].type = ent.type;
        fresh[n].size = ent.size;
        fresh[n].mode = ent.mode;
        n++;
    }
    close(fd);

    /* Insertion sort: a directory here holds tens of entries, and
     * this keeps the comparison in one place. */
    for (int i = 1; i < n; i++) {
        struct entry key = fresh[i];
        int j = i - 1;
        while (j >= 0 && entry_before(&key, &fresh[j])) {
            fresh[j + 1] = fresh[j];
            j--;
        }
        fresh[j + 1] = key;
    }

    memcpy(g_entries, fresh, sizeof(struct entry) * (size_t)n);
    g_count = n;
    g_sel = 0;
    g_scroll = 0;
    snprintf(g_path, sizeof(g_path), "%s", resolved);
    set_status(0, "%d item%s", n, n == 1 ? "" : "s");
}

/* ---- drawing ---- */

static void draw_triangle(int cx, int cy, int size, int dir, unsigned color) {
    /* dir: 0 up, 1 down. Rows of a triangle, one hx_fill each - the
     * server has no polygon primitive and does not need one. */
    for (int i = 0; i < size; i++) {
        int half = dir == 0 ? i : size - 1 - i;
        hx_fill(win, cx - half, cy - size / 2 + i, (unsigned)(half * 2 + 1),
                1, color);
    }
}

static void draw_icon(int x, int y, const struct entry *e) {
    if (e->type == TUS_DIR) {
        hx_fill(win, x, y + 2, 7, 3, COL_FOLDER);        /* the tab */
        hx_fill(win, x, y + 4, 15, 10, COL_FOLDER);
        return;
    }
    if (e->type == TUS_DEVICE) {
        hx_fill(win, x + 2, y + 3, 11, 11, COL_DEVICE);
        hx_fill(win, x + 5, y + 6, 5, 5, COL_PAGE);
        return;
    }
    if (e->type == TUS_SOCKET) {
        hx_fill(win, x + 2, y + 4, 11, 9, COL_MUTED);
        return;
    }
    unsigned ink = (e->mode & 0111) != 0 ? COL_EXEC : COL_FILE;
    hx_fill(win, x + 2, y + 1, 11, 14, ink);
    hx_fill(win, x + 4, y + 4, 7, 1, COL_PAGE);
    hx_fill(win, x + 4, y + 7, 7, 1, COL_PAGE);
    hx_fill(win, x + 4, y + 10, 4, 1, COL_PAGE);
}

/* A button's glyph, drawn rather than written: the font has no
 * arrows, and a toolbar of letters is not a toolbar. */
static void draw_button(int which, int x, int y, int size) {
    int cx = x + size / 2, cy = y + size / 2;
    unsigned ink = COL_TEXT;

    switch (which) {
    case BTN_UP:
        draw_triangle(cx, cy - 3, 6, 0, ink);
        hx_fill(win, cx - 1, cy, 2, 7, ink);
        break;
    case BTN_HOME:
        draw_triangle(cx, cy - 4, 6, 0, ink);
        hx_fill(win, cx - 5, cy + 1, 10, 7, ink);
        hx_fill(win, cx - 1, cy + 4, 3, 4, COL_CHROME);
        break;
    case BTN_REFRESH:
        /* A ring open at the top right, with an arrow head in the
         * gap: a circular arrow drawn out of rectangles. */
        hx_fill(win, cx - 6, cy - 6, 2, 12, ink);   /* left */
        hx_fill(win, cx - 6, cy + 4, 12, 2, ink);   /* bottom */
        hx_fill(win, cx + 4, cy - 1, 2, 6, ink);    /* right, lower half */
        hx_fill(win, cx - 6, cy - 6, 8, 2, ink);    /* top, stops short */
        draw_triangle(cx + 5, cy - 4, 5, 0, ink);   /* the head */
        break;
    case BTN_NEWDIR:
        hx_fill(win, cx - 8, cy - 5, 6, 2, COL_FOLDER);
        hx_fill(win, cx - 8, cy - 4, 11, 9, COL_FOLDER);
        hx_fill(win, cx + 4, cy - 1, 2, 7, COL_EXEC);   /* a plus */
        hx_fill(win, cx + 2, cy + 1, 6, 2, COL_EXEC);
        break;
    case BTN_DELETE:
        hx_fill(win, cx - 5, cy - 6, 10, 2, COL_ERROR); /* the lid */
        hx_fill(win, cx - 2, cy - 8, 4, 2, COL_ERROR);
        hx_fill(win, cx - 4, cy - 3, 8, 10, COL_ERROR);
        hx_fill(win, cx - 2, cy - 1, 1, 6, COL_CHROME);
        hx_fill(win, cx + 1, cy - 1, 1, 6, COL_CHROME);
        break;
    case BTN_TERMINAL:
        hx_fill(win, cx - 8, cy - 6, 16, 12, COL_TEXT);
        hx_fill(win, cx - 6, cy - 3, 2, 2, COL_PAGE);   /* a prompt */
        hx_fill(win, cx - 4, cy - 1, 2, 2, COL_PAGE);
        hx_fill(win, cx - 6, cy + 1, 2, 2, COL_PAGE);
        hx_fill(win, cx - 1, cy + 2, 6, 2, COL_PAGE);
        break;
    default:
        break;
    }
}

static void draw_chrome(void) {
    hx_fill(win, 0, 0, (unsigned)win_w, BAR_H, COL_CHROME);
    hx_fill(win, 0, BAR_H - 1, (unsigned)win_w, 1, COL_BORDER);

    for (int i = 0; i < BTN_COUNT; i++) {
        int x, y, size;
        button_rect(i, &x, &y, &size);
        if (i == g_hover_btn) {
            hx_fill(win, x, y, (unsigned)size, (unsigned)size, COL_HOVER);
        }
        draw_button(i, x, y, size);
    }

    /* The path, in a field of its own, trimmed from the LEFT: the
     * end of a path is the part that says where you are. */
    int bx, by, bsize;
    button_rect(BTN_COUNT - 1, &bx, &by, &bsize);
    int px = bx + bsize + 8;
    int pw = win_w - px - PAD;
    if (pw < 40) {
        return;
    }
    hx_fill(win, px, (BAR_H - 26) / 2, (unsigned)pw, 26, COL_PAGE);
    hx_rect(win, px, (BAR_H - 26) / 2, (unsigned)pw, 26, COL_BORDER);

    int room = (pw - 16) / HX_FONT_W;
    const char *text = g_mode == MODE_VIEW ? g_view_name : g_path;
    int len = (int)strlen(text);
    if (len > room && room > 3) {
        text += len - room + 1;
    }
    char shown[PATH_MAX_T + 4];
    snprintf(shown, sizeof(shown), "%s%s",
             len > room && room > 3 ? "<" : "", text);
    hx_text(win, px + 8, (BAR_H - HX_FONT_H) / 2, COL_TEXT, 0, 0, shown);
}

static void size_text(const struct entry *e, char *out, size_t outsz) {
    if (e->type == TUS_DIR) {
        snprintf(out, outsz, "--");
        return;
    }
    if (e->size < 1024) {
        snprintf(out, outsz, "%u B", e->size);
    } else if (e->size < 1024 * 1024) {
        snprintf(out, outsz, "%u.%u KB", e->size / 1024,
                 (e->size % 1024) * 10 / 1024);
    } else {
        snprintf(out, outsz, "%u.%u MB", e->size / (1024 * 1024),
                 (e->size % (1024 * 1024)) * 10 / (1024 * 1024));
    }
}

static void draw_list(void) {
    int top = list_top();
    int h = list_h();
    hx_fill(win, 0, top, (unsigned)win_w, (unsigned)h, COL_PAGE);

    int rows = rows_visible();
    int text_x = PAD + ICON_W + 6;
    int size_x = win_w - SCROLLBAR_W - PAD - 10 * HX_FONT_W;

    for (int i = 0; i < rows; i++) {
        int idx = g_scroll + i;
        if (idx >= g_count) {
            break;
        }
        int y = top + i * ROW_H;
        const struct entry *e = &g_entries[idx];

        if (idx == g_sel) {
            hx_fill(win, 0, y, (unsigned)(win_w - SCROLLBAR_W),
                    ROW_H, COL_SELECT);
            hx_fill(win, 0, y, 3, ROW_H, COL_ACCENT);
        } else if (idx == g_hover) {
            hx_fill(win, 0, y, (unsigned)(win_w - SCROLLBAR_W),
                    ROW_H, COL_HOVER);
        }

        draw_icon(PAD, y + (ROW_H - 16) / 2, e);

        char name[80];
        int room = (size_x - text_x) / HX_FONT_W;
        snprintf(name, sizeof(name), "%s%s", e->name,
                 e->type == TUS_DIR ? "/" : "");
        if (room > 3 && (int)strlen(name) > room) {
            name[room - 1] = '.';
            name[room] = '\0';
        }
        unsigned ink = e->type == TUS_DIR ? COL_ACCENT : COL_TEXT;
        hx_text(win, text_x, y + (ROW_H - HX_FONT_H) / 2, ink, 0, 0, name);

        char size[24];
        size_text(e, size, sizeof(size));
        int sx = win_w - SCROLLBAR_W - PAD -
                 (int)strlen(size) * HX_FONT_W;
        hx_text(win, sx, y + (ROW_H - HX_FONT_H) / 2, COL_MUTED, 0, 0, size);
    }

    if (g_count == 0) {
        hx_text(win, PAD, top + 10, COL_MUTED, 0, 0, "(empty directory)");
    }

    int track_y, track_h, thumb_y, thumb_h;
    if (scrollbar_metrics(&track_y, &track_h, &thumb_y, &thumb_h)) {
        int x = win_w - SCROLLBAR_W;
        hx_fill(win, x, track_y, SCROLLBAR_W, (unsigned)track_h, COL_TRACK);
        hx_fill(win, x, track_y, 1, (unsigned)track_h, COL_BORDER);
        unsigned ink = (g_drag_thumb || g_hover_thumb) ? COL_THUMB_HOT
                                                       : COL_THUMB;
        hx_fill(win, x + 3, thumb_y + 1, SCROLLBAR_W - 6,
                (unsigned)(thumb_h - 2), ink);
    }
}

/* ---- the file viewer ---- */

static int printable(const char *buf, int len) {
    int bad = 0;
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)buf[i];
        if (c == '\n' || c == '\t' || c == '\r') {
            continue;
        }
        if (c < 0x20 || c == 0x7F) {
            bad++;
        }
    }
    return len == 0 || bad * 20 < len; /* under 5% control bytes: text */
}

static void view_line(int index, char *out, size_t outsz, int cols) {
    out[0] = '\0';
    if (g_view_buf == NULL || index < 0) {
        return;
    }
    if (g_view_hex) {
        int off = index * 16;
        if (off >= g_view_len) {
            return;
        }
        int n = g_view_len - off < 16 ? g_view_len - off : 16;
        int w = snprintf(out, outsz, "%08x  ", (unsigned)off);
        for (int i = 0; i < 16 && w < (int)outsz - 4; i++) {
            if (i < n) {
                w += snprintf(out + w, outsz - (size_t)w, "%02x ",
                              (unsigned char)g_view_buf[off + i]);
            } else {
                w += snprintf(out + w, outsz - (size_t)w, "   ");
            }
        }
        w += snprintf(out + w, outsz - (size_t)w, " ");
        for (int i = 0; i < n && w < (int)outsz - 2; i++) {
            unsigned char c = (unsigned char)g_view_buf[off + i];
            out[w++] = (c >= 0x20 && c < 0x7F) ? (char)c : '.';
        }
        out[w] = '\0';
        return;
    }

    /* Text: walk to the requested line, then copy it, wrapping at the
     * window's width the way a pager does. */
    int line = 0, i = 0, col = 0, start = 0;
    while (i < g_view_len) {
        if (line == index && col == 0) {
            start = i;
        }
        if (g_view_buf[i] == '\n' || col >= cols) {
            if (line == index) {
                int n = i - start;
                if (n > (int)outsz - 1) n = (int)outsz - 1;
                if (n > 0) {
                    memcpy(out, g_view_buf + start, (size_t)n);
                }
                out[n > 0 ? n : 0] = '\0';
                /* Tabs and stray control bytes would print as
                 * nothing; make them spaces. */
                for (int k = 0; k < n; k++) {
                    if ((unsigned char)out[k] < 0x20) {
                        out[k] = ' ';
                    }
                }
                return;
            }
            line++;
            col = 0;
            if (g_view_buf[i] == '\n') {
                i++;
            }
            continue;
        }
        col++;
        i++;
    }
    if (line == index) {
        int n = g_view_len - start;
        if (n > (int)outsz - 1) n = (int)outsz - 1;
        if (n > 0) {
            memcpy(out, g_view_buf + start, (size_t)n);
        }
        out[n > 0 ? n : 0] = '\0';
    }
}

static int view_lines(int cols) {
    if (g_view_buf == NULL) {
        return 0;
    }
    if (g_view_hex) {
        return (g_view_len + 15) / 16;
    }
    int lines = 1, col = 0;
    for (int i = 0; i < g_view_len; i++) {
        if (g_view_buf[i] == '\n' || col >= cols) {
            lines++;
            col = 0;
            continue;
        }
        col++;
    }
    return lines;
}

static void draw_view(void) {
    int top = list_top();
    int h = list_h();
    hx_fill(win, 0, top, (unsigned)win_w, (unsigned)h, COL_PAGE);

    int cols = (win_w - 2 * PAD - SCROLLBAR_W) / HX_FONT_W;
    if (cols > HX_TEXT_MAX - 1) cols = HX_TEXT_MAX - 1;
    if (cols < 8) cols = 8;
    int rows = h / HX_FONT_H;
    int total = view_lines(cols);
    if (g_view_scroll > total - 1) g_view_scroll = total - 1;
    if (g_view_scroll < 0) g_view_scroll = 0;

    for (int r = 0; r < rows; r++) {
        char line[HX_TEXT_MAX + 96];
        view_line(g_view_scroll + r, line, sizeof(line), cols);
        if (line[0] == '\0') {
            continue;
        }
        if ((int)strlen(line) > HX_TEXT_MAX - 1) {
            line[HX_TEXT_MAX - 1] = '\0';
        }
        hx_text(win, PAD, top + r * HX_FONT_H, COL_TEXT, 0, 0, line);
    }

    /* The viewer's own scrollbar: the same groove, driven by lines. */
    if (total > rows) {
        int th = h * rows / total;
        if (th < THUMB_MIN) th = THUMB_MIN;
        int span = h - th;
        int at = total - rows > 0 ? span * g_view_scroll / (total - rows) : 0;
        int x = win_w - SCROLLBAR_W;
        hx_fill(win, x, top, SCROLLBAR_W, (unsigned)h, COL_TRACK);
        hx_fill(win, x, top, 1, (unsigned)h, COL_BORDER);
        hx_fill(win, x + 3, top + at + 1, SCROLLBAR_W - 6,
                (unsigned)(th - 2), COL_THUMB);
    }
}

/* ---- dialogs ---- */

static void draw_dialog(const char *title, const char *body,
                        const char *hint) {
    int w = 420, h = 132;
    int x = (win_w - w) / 2, y = (win_h - h) / 2;
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    hx_fill(win, x, y, (unsigned)w, (unsigned)h, COL_PAGE);
    hx_rect(win, x, y, (unsigned)w, (unsigned)h, COL_BORDER);
    hx_fill(win, x, y, (unsigned)w, 3, COL_ACCENT);

    hx_text(win, x + 16, y + 18, COL_TEXT, 0, 0, title);
    hx_text(win, x + 16, y + 44, COL_MUTED, 0, 0, body);

    if (g_mode == MODE_NEWDIR) {
        hx_fill(win, x + 16, y + 66, (unsigned)(w - 32), 24, COL_CHROME);
        hx_rect(win, x + 16, y + 66, (unsigned)(w - 32), 24, COL_ACCENT);
        hx_text(win, x + 22, y + 70, COL_TEXT, 0, 0, g_input);
        hx_fill(win, x + 22 + g_input_len * HX_FONT_W, y + 70, 1,
                HX_FONT_H, COL_TEXT);
    }
    hx_text(win, x + 16, y + h - 26, COL_MUTED, 0, 0, hint);
}

static void draw_status(void) {
    int y = win_h - STATUS_H;
    hx_fill(win, 0, y, (unsigned)win_w, STATUS_H, COL_STATUS);
    hx_fill(win, 0, y, (unsigned)win_w, 1, COL_BORDER);

    char trimmed[200];
    snprintf(trimmed, sizeof(trimmed), "%s", g_status);
    int room = (win_w - 20) / HX_FONT_W;
    if (room > 3 && (int)strlen(trimmed) > room) {
        trimmed[room] = '\0';
    }
    hx_text(win, 10, y + (STATUS_H - HX_FONT_H) / 2,
            g_status_error ? COL_ERROR : COL_MUTED, 0, 0, trimmed);
}

static void redraw(void) {
    hx_batch_begin();
    draw_chrome();
    if (g_mode == MODE_VIEW) {
        draw_view();
    } else {
        draw_list();
    }
    draw_status();
    if (g_mode == MODE_CONFIRM) {
        char body[128];
        snprintf(body, sizeof(body), "Delete %s?",
                 g_sel < g_count ? g_entries[g_sel].name : "");
        draw_dialog("Delete", body, "Enter or Y to delete, Esc to keep it");
    } else if (g_mode == MODE_NEWDIR) {
        draw_dialog("New folder", "Name:", "Enter to create, Esc to cancel");
    }
    hx_commit(win);
    hx_batch_end();
}

static void set_status(int error, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(g_status, sizeof(g_status), fmt, args);
    va_end(args);
    g_status_error = error;
}

/* ---- actions ---- */

static void scroll_to(int row) {
    int max_row = g_count - rows_visible();
    if (max_row < 0) max_row = 0;
    if (row < 0) row = 0;
    if (row > max_row) row = max_row;
    if (row == g_scroll) {
        return;
    }
    g_scroll = row;
    redraw();
}

static void select_row(int idx) {
    if (idx < 0) idx = 0;
    if (idx >= g_count) idx = g_count - 1;
    if (idx < 0) {
        return;
    }
    g_sel = idx;
    if (g_sel < g_scroll) {
        g_scroll = g_sel;
    }
    int rows = rows_visible();
    if (g_sel >= g_scroll + rows) {
        g_scroll = g_sel - rows + 1;
    }
    const struct entry *e = &g_entries[g_sel];
    char size[24];
    size_text(e, size, sizeof(size));
    set_status(0, "%s   %s   mode %o", e->name, size, e->mode & 07777);
}

static void open_viewer(const char *path, const char *name) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        set_status(1, "%s: cannot open", name);
        return;
    }
    if (g_view_buf == NULL) {
        g_view_buf = malloc(VIEW_MAX);
    }
    if (g_view_buf == NULL) {
        close(fd);
        set_status(1, "out of memory");
        return;
    }
    int len = 0;
    for (;;) {
        long n = read(fd, g_view_buf + len, VIEW_MAX - (unsigned)len);
        if (n <= 0) {
            break;
        }
        len += (int)n;
        if ((unsigned)len >= VIEW_MAX) {
            break;
        }
    }
    close(fd);

    g_view_len = len;
    g_view_hex = !printable(g_view_buf, len);
    g_view_scroll = 0;
    snprintf(g_view_name, sizeof(g_view_name), "%s", path);
    g_mode = MODE_VIEW;
    set_status(0, "%s - %d bytes%s   (Esc goes back, X switches view)",
               name, len, g_view_hex ? ", shown as hex" : "");
}

static void spawn(const char *program, const char *arg) {
    char *argv[3];
    argv[0] = (char *)program;
    argv[1] = (char *)arg;
    argv[2] = NULL;
    if (hx_spawn(program, argv) < 0) {
        set_status(1, "%s: could not start", program);
    } else {
        set_status(0, "started %s %s", program, arg != NULL ? arg : "");
    }
}

static void open_selected(void) {
    if (g_sel < 0 || g_sel >= g_count) {
        return;
    }
    const struct entry *e = &g_entries[g_sel];
    char full[PATH_MAX_T];
    path_join(g_path, e->name, full, sizeof(full));

    if (e->type == TUS_DIR) {
        load_dir(full);
        redraw();
        return;
    }
    if (ext_is(e->name, "mp4")) {
        spawn("/bin/hxvideo", full);
        redraw();
        return;
    }
    if ((e->mode & 0111) != 0 && e->type == TUS_FILE) {
        spawn(full, NULL);
        redraw();
        return;
    }
    open_viewer(full, e->name);
    redraw();
}

static void go_up(void) {
    char parent[PATH_MAX_T];
    path_parent(g_path, parent, sizeof(parent));
    load_dir(parent);
    redraw();
}

static void do_delete(void) {
    if (g_sel < 0 || g_sel >= g_count) {
        return;
    }
    char full[PATH_MAX_T];
    char name[64];
    path_join(g_path, g_entries[g_sel].name, full, sizeof(full));
    snprintf(name, sizeof(name), "%s", g_entries[g_sel].name);
    int sel = g_sel;
    if (tus_syscall(SYS_UNLINK, (long)full, 0, 0) < 0) {
        set_status(1, "%s: could not delete", name);
        return;
    }
    load_dir(g_path);
    select_row(sel);
    set_status(0, "deleted %s", name);
}

static void do_mkdir(void) {
    if (g_input_len == 0) {
        return;
    }
    char full[PATH_MAX_T];
    char name[64];
    snprintf(name, sizeof(name), "%s", g_input);
    path_join(g_path, name, full, sizeof(full));
    if (tus_syscall(SYS_MKDIR, (long)full, 0755, 0) < 0) {
        set_status(1, "%s: could not create", name);
        return;
    }
    load_dir(g_path);
    for (int i = 0; i < g_count; i++) {
        if (strcmp(g_entries[i].name, name) == 0) {
            select_row(i);
            break;
        }
    }
    set_status(0, "created %s/", name);
}

static void press_button(int which) {
    switch (which) {
    case BTN_UP: go_up(); break;
    case BTN_HOME: load_dir("/"); redraw(); break;
    case BTN_REFRESH: {
        int sel = g_sel;
        load_dir(g_path);
        select_row(sel);
        redraw();
        break;
    }
    case BTN_NEWDIR:
        g_mode = MODE_NEWDIR;
        g_input[0] = '\0';
        g_input_len = 0;
        redraw();
        break;
    case BTN_DELETE:
        if (g_count > 0) {
            g_mode = MODE_CONFIRM;
            redraw();
        }
        break;
    case BTN_TERMINAL:
        /* A file manager that cannot get you a shell is only half a
         * file manager. */
        spawn("/bin/hxtsh", NULL);
        redraw();
        break;
    default:
        break;
    }
}

/* ---- input ---- */

static void on_key(const struct hx_event *ev) {
    unsigned int k = ev->key;

    if (g_mode == MODE_CONFIRM) {
        if (k == '\n' || k == '\r' || k == 'y' || k == 'Y') {
            g_mode = MODE_LIST;
            do_delete();
        } else if (k == 0x1B || k == 'n' || k == 'N') {
            g_mode = MODE_LIST;
            set_status(0, "kept");
        }
        redraw();
        return;
    }

    if (g_mode == MODE_NEWDIR) {
        if (k == '\n' || k == '\r') {
            g_mode = MODE_LIST;
            do_mkdir();
        } else if (k == 0x1B) {
            g_mode = MODE_LIST;
            set_status(0, "cancelled");
        } else if (k == '\b' || k == 0x7F) {
            if (g_input_len > 0) {
                g_input[--g_input_len] = '\0';
            }
        } else if (k >= 0x20 && k < 0x7F &&
                   g_input_len < (int)sizeof(g_input) - 1) {
            g_input[g_input_len++] = (char)k;
            g_input[g_input_len] = '\0';
        }
        redraw();
        return;
    }

    if (g_mode == MODE_VIEW) {
        int rows = list_h() / HX_FONT_H;
        switch (k) {
        case 0x1B:
        case '\b':
            g_mode = MODE_LIST;
            set_status(0, "%d items", g_count);
            break;
        case HX_KEY_DOWN: g_view_scroll++; break;
        case HX_KEY_UP: g_view_scroll--; break;
        case HX_KEY_PAGE_DOWN:
        case ' ': g_view_scroll += rows - 1; break;
        case HX_KEY_PAGE_UP: g_view_scroll -= rows - 1; break;
        case HX_KEY_HOME: g_view_scroll = 0; break;
        case 'x':
        case 'X':
            g_view_hex = !g_view_hex;   /* text or bytes, on demand */
            g_view_scroll = 0;
            break;
        default: break;
        }
        if (g_view_scroll < 0) g_view_scroll = 0;
        redraw();
        return;
    }

    switch (k) {
    case HX_KEY_DOWN: select_row(g_sel + 1); redraw(); break;
    case HX_KEY_UP: select_row(g_sel - 1); redraw(); break;
    case HX_KEY_PAGE_DOWN: select_row(g_sel + rows_visible()); redraw(); break;
    case HX_KEY_PAGE_UP: select_row(g_sel - rows_visible()); redraw(); break;
    case HX_KEY_HOME: select_row(0); redraw(); break;
    case HX_KEY_END: select_row(g_count - 1); redraw(); break;
    case '\n':
    case '\r': open_selected(); break;
    case '\b':
    case HX_KEY_LEFT: go_up(); break;
    case HX_KEY_RIGHT: open_selected(); break;
    case HX_KEY_DELETE:
        if (g_count > 0) {
            g_mode = MODE_CONFIRM;
            redraw();
        }
        break;
    case 0x12: press_button(BTN_REFRESH); break;  /* Ctrl+R */
    case 0x0E: press_button(BTN_NEWDIR); break;   /* Ctrl+N */
    case 0x14: press_button(BTN_TERMINAL); break; /* Ctrl+T */
    default:
        break;
    }
}

static int row_at(int y) {
    if (y < list_top() || y >= list_top() + list_h()) {
        return -1;
    }
    int idx = g_scroll + (y - list_top()) / ROW_H;
    return idx < g_count ? idx : -1;
}

static void drag_to(int pointer_y) {
    int track_y, track_h, thumb_y, thumb_h;
    if (!scrollbar_metrics(&track_y, &track_h, &thumb_y, &thumb_h)) {
        return;
    }
    int span = track_h - thumb_h;
    if (span <= 0) {
        return;
    }
    int at = pointer_y - g_drag_grab_y - track_y;
    if (at < 0) at = 0;
    if (at > span) at = span;
    scroll_to((g_count - rows_visible()) * at / span);
}

static void on_press(const struct hx_event *ev) {
    if (g_mode == MODE_CONFIRM || g_mode == MODE_NEWDIR) {
        return; /* the dialog is answered with the keyboard */
    }

    if (ev->y < BAR_H) {
        int b = button_at(ev->x, ev->y);
        if (b != BTN_NONE) {
            press_button(b);
        }
        return;
    }

    if (g_mode == MODE_VIEW) {
        return;
    }

    int track_y, track_h, thumb_y, thumb_h;
    if (ev->x >= win_w - SCROLLBAR_W &&
        scrollbar_metrics(&track_y, &track_h, &thumb_y, &thumb_h) &&
        ev->y >= track_y && ev->y < track_y + track_h) {
        if (ev->y >= thumb_y && ev->y < thumb_y + thumb_h) {
            g_drag_thumb = 1;
            g_drag_grab_y = ev->y - thumb_y;
        } else {
            scroll_to(g_scroll + (ev->y < thumb_y ? -rows_visible()
                                                  : rows_visible()));
        }
        return;
    }

    int idx = row_at(ev->y);
    if (idx < 0) {
        return;
    }
    /* A click on what is already selected opens it: the second click
     * of a double click, without a clock to measure it with. */
    if (idx == g_sel) {
        open_selected();
        return;
    }
    select_row(idx);
    redraw();
}

static void on_motion(const struct hx_event *ev) {
    int changed = 0;

    if (g_drag_thumb) {
        drag_to(ev->y);
        return;
    }

    int b = ev->y < BAR_H ? button_at(ev->x, ev->y) : BTN_NONE;
    if (b != g_hover_btn) {
        g_hover_btn = b;
        changed = 1;
    }

    int track_y, track_h, thumb_y, thumb_h;
    int over = ev->x >= win_w - SCROLLBAR_W && g_mode == MODE_LIST &&
               scrollbar_metrics(&track_y, &track_h, &thumb_y, &thumb_h) &&
               ev->y >= thumb_y && ev->y < thumb_y + thumb_h;
    if (over != g_hover_thumb) {
        g_hover_thumb = over;
        changed = 1;
    }

    int row = g_mode == MODE_LIST && ev->x < win_w - SCROLLBAR_W
                  ? row_at(ev->y) : -1;
    if (row != g_hover) {
        g_hover = row;
        changed = 1;
    }

    if (changed) {
        redraw();
    }
}

static void on_wheel(int steps) {
    if (g_mode == MODE_VIEW) {
        g_view_scroll -= steps * 3;
        if (g_view_scroll < 0) g_view_scroll = 0;
        redraw();
        return;
    }
    if (g_mode != MODE_LIST) {
        return;
    }
    scroll_to(g_scroll - steps * 3);
}

int main(int argc, char **argv) {
    if (hx_open(&dpy) != 0) {
        return 1;
    }

    win = hx_create_window(140, 110, (unsigned)win_w, (unsigned)win_h, 0,
                           COL_PAGE, "Files");
    if (win == 0) {
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);

    load_dir(argc > 1 ? argv[1] : "/");
    select_row(0);
    redraw();

    for (;;) {
        struct hx_event ev;
        if (hx_wait_event(&ev) <= 0) {
            continue;
        }
        if (ev.type == HX_EV_CLOSE) {
            break;
        }
        if (ev.type == HX_EV_EXPOSE) {
            redraw();
        } else if (ev.type == HX_EV_CONFIGURE) {
            if (ev.w > 0 && ev.h > 0 &&
                ((int)ev.w != win_w || (int)ev.h != win_h)) {
                win_w = (int)ev.w;
                win_h = (int)ev.h;
                redraw();
            }
        } else if (ev.type == HX_EV_KEY) {
            on_key(&ev);
        } else if (ev.type == HX_EV_POINTER) {
            if (ev.detail == HX_PTR_PRESS) {
                on_press(&ev);
            } else if (ev.detail == HX_PTR_MOTION) {
                on_motion(&ev);
            } else if (ev.detail == HX_PTR_RELEASE) {
                g_drag_thumb = 0;
            } else if (ev.detail == HX_PTR_WHEEL) {
                on_wheel((int)(int32_t)ev.key);
            }
        }
    }

    free(g_view_buf);
    hx_destroy_window(win);
    hx_close(&dpy);
    return 0;
}
