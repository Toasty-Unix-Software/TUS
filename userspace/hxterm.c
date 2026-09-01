/*
 * hxterm - a terminal in a highX window
 *
 * hxterm is the first highX client that is a real tool rather than a
 * demo: a character grid, a line editor with history, and a small
 * shell. It has no pty (TUS has no terminal driver for user programs
 * yet) and it does not talk to tsh, which lives in the kernel - it
 * runs programs the way a shell does and shows what they print.
 *
 * The shell understands what a UNIX user expects to type:
 *
 *   echo "hello world" > /tmp/a.txt     quoting and redirection
 *   cat /tmp/a.txt >> /tmp/log          appending
 *   ls /bin | grep hx | sed s/hx/HX/    pipelines of any length
 *   grep -n toast < /etc/motd           input redirection
 *   history                             the last 64 command lines
 *
 * How a command runs: every stage gets its descriptors wired up
 * (pipe(), dup2()), external programs are started with hx_spawn() -
 * which hands them a copy of the fd table - and the last stage's
 * output lands in a pipe hxterm polls, so the window keeps repainting
 * and answering keys while a program runs. EOF on that pipe is the
 * completion signal: it arrives when the child exits and the kernel
 * closes its fd table, which is how hxterm knows a command finished
 * without a waitpid() syscall.
 *
 * Built-ins (ls, cat, cd, pwd, echo, history, clear, help, exit)
 * exist because TUS ships them as shell built-ins, not as programs in
 * /bin. They run inside hxterm, and they honour redirection and
 * pipelines: their output goes to a descriptor when there is one, and
 * straight into the grid when there is not.
 */

#include "highapi/highapi.h"

#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_COLS   200
#define MAX_ROWS   64
#define PAD        6
#define LINE_MAX   256
#define ARG_MAX    12
#define MAX_STAGES 4
#define HIST_MAX   64
#define PATH_MAX_T 256

#define COL_BG      0x000C1218u
#define COL_FG      0x00C8D4E0u
#define COL_PROMPT  0x0050D0A0u
#define COL_ERR     0x00E06060u
#define COL_CURSOR  0x004FA3D1u

/* TUS's readdir has its own ABI (kernel/vfs/vfs.h): one entry per
 * call into a fixed-size structure. musl has no wrapper for it, so
 * hxterm calls it directly - the same `int $0x80` ABI highAPI uses. */
#define SYS_READDIR 11

#define TUS_DIR    1
#define TUS_FILE   2
#define TUS_DEVICE 3

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
     * call, which it does: that is how closing a window turned into a
     * page fault at a garbage address. */
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

/* ---- terminal state ---- */

static struct hx_display dpy;
static unsigned int win;
static unsigned int win_w = 720, win_h = 460;

static char g_grid[MAX_ROWS][MAX_COLS];
static unsigned char g_color[MAX_ROWS];   /* one color per line is enough */
static unsigned char g_dirty[MAX_ROWS];
static int g_cols = 80, g_rows = 24;
static int g_cur_r, g_cur_c;
static unsigned int g_pen = COL_FG;

/* The line editor: text, cursor position inside it, and the column
 * the input starts at (right after the prompt). */
static char g_line[LINE_MAX];
static int  g_len;
static int  g_pos;
static int  g_input_col;
static int  g_input_row;

/* Command history, oldest first. g_browse is the entry the arrow keys
 * are showing, or -1 while a fresh line is being typed. */
static char g_hist[HIST_MAX][LINE_MAX];
static int  g_hist_count;
static int  g_browse = -1;
static char g_stash[LINE_MAX];

static char g_cwd[PATH_MAX_T] = "/";

/* The running child, if any, and where built-in output goes. */
static int g_child_fd = -1;
static int g_out_fd = -1;
static int g_err_fd = -1;
static int g_in_redirected;   /* fd 0 belongs to a file or a pipe */

static const unsigned int g_palette[3] = { COL_FG, COL_PROMPT, COL_ERR };

static int pen_index(void) {
    for (int i = 0; i < 3; i++) {
        if (g_palette[i] == g_pen) {
            return i;
        }
    }
    return 0;
}

/* ---- grid ---- */

static void grid_clear(void) {
    for (int r = 0; r < MAX_ROWS; r++) {
        memset(g_grid[r], ' ', MAX_COLS);
        g_color[r] = 0;
        g_dirty[r] = 1;
    }
    g_cur_r = 0;
    g_cur_c = 0;
}

static void scroll_up(void) {
    for (int r = 1; r < g_rows; r++) {
        memcpy(g_grid[r - 1], g_grid[r], MAX_COLS);
        g_color[r - 1] = g_color[r];
    }
    memset(g_grid[g_rows - 1], ' ', MAX_COLS);
    g_color[g_rows - 1] = (unsigned char)pen_index();
    for (int r = 0; r < g_rows; r++) {
        g_dirty[r] = 1;
    }
    g_cur_r = g_rows - 1;
    if (g_input_row > 0) {
        g_input_row--;
    }
}

static void newline(void) {
    g_cur_c = 0;
    g_cur_r++;
    if (g_cur_r >= g_rows) {
        scroll_up();
    }
    g_dirty[g_cur_r] = 1;
}

static void term_putc(char c) {
    if (c == '\n') {
        newline();
        return;
    }
    if (c == '\r') {
        g_cur_c = 0;
        g_dirty[g_cur_r] = 1;
        return;
    }
    if (c == '\t') {
        int next = (g_cur_c + 8) & ~7;
        while (g_cur_c < next && g_cur_c < g_cols) {
            g_grid[g_cur_r][g_cur_c++] = ' ';
        }
        g_dirty[g_cur_r] = 1;
        return;
    }
    if (c == '\b') {
        if (g_cur_c > 0) {
            g_cur_c--;
            g_grid[g_cur_r][g_cur_c] = ' ';
            g_dirty[g_cur_r] = 1;
        }
        return;
    }
    if (c < 0x20 || (unsigned char)c > 0x7E) {
        return;
    }
    if (g_cur_c >= g_cols) {
        newline();
    }
    g_grid[g_cur_r][g_cur_c++] = c;
    g_color[g_cur_r] = (unsigned char)pen_index();
    g_dirty[g_cur_r] = 1;
}

static void term_puts(const char *s) {
    for (; *s != '\0'; s++) {
        term_putc(*s);
    }
}

static void term_write(const char *buf, int len) {
    for (int i = 0; i < len; i++) {
        term_putc(buf[i]);
    }
}

/* ---- built-in output ----
 *
 * A built-in writes to a descriptor when the command line gave it one
 * (a file after '>' or the pipe to the next stage) and straight into
 * the grid otherwise. Writing into the grid in the common case is not
 * an optimisation: hxterm drains its capture pipe from the same loop
 * that would be blocked inside the built-in, so sending large output
 * through it could fill the pipe with nobody to empty it. */

static void bout(const char *s) {
    if (g_out_fd >= 0) {
        write(g_out_fd, s, strlen(s));
    } else {
        term_puts(s);
    }
}

static void berr(const char *s) {
    if (g_err_fd >= 0) {
        write(g_err_fd, s, strlen(s));
        return;
    }
    unsigned int saved = g_pen;
    g_pen = COL_ERR;
    term_puts(s);
    g_pen = saved;
}

/* ---- painting ---- */

static void paint_row(int r, int with_cursor) {
    int y = PAD + r * HX_FONT_H;
    char line[MAX_COLS + 1];

    int end = g_cols;
    while (end > 0 && g_grid[r][end - 1] == ' ') {
        end--;
    }
    memcpy(line, g_grid[r], (size_t)end);
    line[end] = '\0';

    hx_fill(win, 0, y, win_w, HX_FONT_H, COL_BG);
    /* hx_text carries at most HX_TEXT_MAX characters per request, so
     * a wide line goes out in chunks. */
    for (int col = 0; col < end; col += HX_TEXT_MAX - 1) {
        char chunk[HX_TEXT_MAX];
        int n = end - col;
        if (n > HX_TEXT_MAX - 1) {
            n = HX_TEXT_MAX - 1;
        }
        memcpy(chunk, line + col, (size_t)n);
        chunk[n] = '\0';
        hx_text(win, PAD + col * HX_FONT_W, y, g_palette[g_color[r] % 3], 0, 0,
                chunk);
    }
    if (with_cursor && g_child_fd < 0) {
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
            paint_row(r, r == g_cur_r);
        }
    }
}

static void redraw_all(void) {
    hx_fill(win, 0, 0, win_w, win_h, COL_BG);
    hx_commit(win);
    for (int r = 0; r < g_rows; r++) {
        g_dirty[r] = 1;
    }
    flush();
}

static void resize(unsigned int w, unsigned int h) {
    win_w = w;
    win_h = h;
    g_cols = (int)(win_w - 2 * PAD) / HX_FONT_W;
    g_rows = (int)(win_h - 2 * PAD) / HX_FONT_H;
    if (g_cols > MAX_COLS) {
        g_cols = MAX_COLS;
    }
    if (g_rows > MAX_ROWS) {
        g_rows = MAX_ROWS;
    }
    if (g_cols < 8) {
        g_cols = 8;
    }
    if (g_rows < 2) {
        g_rows = 2;
    }
    if (g_cur_r >= g_rows) {
        g_cur_r = g_rows - 1;
    }
    if (g_cur_c >= g_cols) {
        g_cur_c = g_cols - 1;
    }
    if (g_input_row >= g_rows) {
        g_input_row = g_rows - 1;
    }
}

/* ---- the line editor ---- */

/* Repaint the input line from the prompt to the end of the row and
 * put the cursor where it belongs. */
static void redraw_input(void) {
    int room = g_cols - g_input_col - 1;
    if (room < 0) {
        room = 0;
    }
    int shown = g_len < room ? g_len : room;

    for (int i = 0; i < room; i++) {
        g_grid[g_input_row][g_input_col + i] = i < shown ? g_line[i] : ' ';
    }
    g_color[g_input_row] = 0;
    g_dirty[g_input_row] = 1;

    g_cur_r = g_input_row;
    g_cur_c = g_input_col + (g_pos < room ? g_pos : room);
}

static void line_set(const char *text) {
    snprintf(g_line, sizeof(g_line), "%s", text);
    g_len = (int)strlen(g_line);
    g_pos = g_len;
    redraw_input();
}

static void prompt(void) {
    g_pen = COL_PROMPT;
    term_puts(g_cwd);
    term_puts("$ ");
    g_pen = COL_FG;

    g_input_row = g_cur_r;
    g_input_col = g_cur_c;
    g_line[0] = '\0';
    g_len = 0;
    g_pos = 0;
    g_browse = -1;
    redraw_input();
}

static void history_push(const char *line) {
    if (line[0] == '\0') {
        return;
    }
    if (g_hist_count > 0 && strcmp(g_hist[g_hist_count - 1], line) == 0) {
        return; /* no duplicate right after itself, like a real shell */
    }
    if (g_hist_count == HIST_MAX) {
        for (int i = 1; i < HIST_MAX; i++) {
            memcpy(g_hist[i - 1], g_hist[i], LINE_MAX);
        }
        g_hist_count--;
    }
    snprintf(g_hist[g_hist_count], LINE_MAX, "%s", line);
    g_hist_count++;
}

/* Up and Down walk the history; the line being typed is stashed on
 * the way in and comes back when walking past the newest entry. */
static void history_browse(int direction) {
    if (g_hist_count == 0) {
        return;
    }
    if (direction < 0) { /* Up: towards older entries */
        if (g_browse == -1) {
            snprintf(g_stash, sizeof(g_stash), "%s", g_line);
            g_browse = g_hist_count - 1;
        } else if (g_browse > 0) {
            g_browse--;
        }
        line_set(g_hist[g_browse]);
        return;
    }
    if (g_browse == -1) {
        return;
    }
    g_browse++;
    if (g_browse >= g_hist_count) {
        g_browse = -1;
        line_set(g_stash);
        return;
    }
    line_set(g_hist[g_browse]);
}

/* ---- paths ---- */

/* Resolve `name` against the terminal's own working directory. TUS
 * has no chdir syscall - tsh keeps its cwd in the shell, and hxterm
 * keeps its own the same way. */
static void resolve(const char *name, char *out, size_t size) {
    if (name == NULL || name[0] == '\0') {
        snprintf(out, size, "%s", g_cwd);
        return;
    }
    if (name[0] == '/') {
        snprintf(out, size, "%s", name);
        return;
    }
    if (strcmp(g_cwd, "/") == 0) {
        snprintf(out, size, "/%s", name);
    } else {
        snprintf(out, size, "%s/%s", g_cwd, name);
    }
}

/* ---- built-in commands ---- */

static void cmd_ls(int argc, char **argv) {
    char path[PATH_MAX_T];
    resolve(argc > 1 ? argv[1] : NULL, path, sizeof(path));

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        berr("ls: cannot open ");
        berr(path);
        berr("\n");
        return;
    }

    struct tus_dirent ent;
    int col = 0;
    int per_row = g_out_fd >= 0 ? 1 : g_cols / 20;
    if (per_row < 1) {
        per_row = 1;
    }
    while (tus_syscall(SYS_READDIR, fd, (long)&ent, sizeof(ent)) > 0) {
        char name[72];
        char item[80];
        /* Directories get the usual trailing slash, devices a '*'. */
        snprintf(name, sizeof(name), "%s%s", ent.name,
                 ent.type == TUS_DIR ? "/" :
                 ent.type == TUS_DEVICE ? "*" : "");
        /* One name per line when the output is going somewhere else:
         * that is what the next program in the pipeline expects. */
        if (per_row == 1) {
            snprintf(item, sizeof(item), "%s\n", name);
        } else {
            snprintf(item, sizeof(item), "%-19s ", name);
        }
        bout(item);
        if (per_row > 1 && ++col >= per_row) {
            col = 0;
            bout("\n");
        }
    }
    if (per_row > 1 && col != 0) {
        bout("\n");
    }
    close(fd);
}

/* cat with no arguments copies stdin, like UNIX - that is what makes
 * `ls /bin | cat` work. */
static void cmd_cat(int argc, char **argv) {
    char buf[512];
    long n;

    if (argc < 2) {
        if (!g_in_redirected) {
            berr("usage: cat <file>\n");
            return;
        }
        while ((n = read(0, buf, sizeof(buf))) > 0) {
            if (g_out_fd >= 0) {
                write(g_out_fd, buf, (size_t)n);
            } else {
                term_write(buf, (int)n);
            }
        }
        return;
    }

    for (int i = 1; i < argc; i++) {
        char path[PATH_MAX_T];
        resolve(argv[i], path, sizeof(path));
        int fd = open(path, O_RDONLY);
        if (fd < 0) {
            berr("cat: cannot open ");
            berr(path);
            berr("\n");
            continue;
        }
        while ((n = read(fd, buf, sizeof(buf))) > 0) {
            if (g_out_fd >= 0) {
                write(g_out_fd, buf, (size_t)n);
            } else {
                term_write(buf, (int)n);
            }
        }
        close(fd);
    }
}

static void cmd_cd(int argc, char **argv) {
    char path[PATH_MAX_T];
    resolve(argc > 1 ? argv[1] : "/", path, sizeof(path));

    /* ".." walks up in our own string: there is nothing to ask the
     * kernel, the cwd belongs to hxterm. */
    if (argc > 1 && strcmp(argv[1], "..") == 0) {
        snprintf(path, sizeof(path), "%s", g_cwd);
        char *slash = strrchr(path, '/');
        if (slash == path) {
            path[1] = '\0';
        } else if (slash != NULL) {
            *slash = '\0';
        }
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        berr("cd: no such directory\n");
        return;
    }
    close(fd);
    snprintf(g_cwd, sizeof(g_cwd), "%s", path);
}

static void cmd_history(void) {
    for (int i = 0; i < g_hist_count; i++) {
        char line[LINE_MAX + 16];
        snprintf(line, sizeof(line), "%4d  %s\n", i + 1, g_hist[i]);
        bout(line);
    }
}

static void cmd_help(void) {
    bout("hxterm - a terminal for the highX window system\n");
    bout("built-ins: ls, cat, cd, pwd, echo, history, clear, help, exit\n");
    bout("anything else runs /bin/<name> and shows its output\n");
    bout("pipes and redirection work: cmd | cmd, > file, >> file, < file\n");
    bout("quotes group arguments: echo \"one two\" > /tmp/a.txt\n");
    bout("Up/Down walk the command history, Left/Right move the cursor\n");
}

/* ---- command line parsing ---- */

#define RD_NONE   0
#define RD_TRUNC  1
#define RD_APPEND 2
#define RD_INPUT  3

struct stage {
    char *argv[ARG_MAX + 1];
    int   argc;
    char *in_file;
    char *out_file;
    int   out_mode;
    char *err_file;
    int   err_mode;
};

/* Split a line into words and operators, honouring single and double
 * quotes (the quotes are removed, what is inside them stays one
 * word). Returns the token count, or -1 when there are too many. */
static int tokenize(char *line, char **tok, int max) {
    int n = 0;
    char *p = line;

    while (*p != '\0') {
        while (*p == ' ' || *p == '\t') {
            p++;
        }
        if (*p == '\0') {
            break;
        }
        if (n >= max) {
            return -1;
        }

        /* Operators are tokens of their own. */
        if (*p == '|' || *p == '<' || *p == '>') {
            tok[n++] = p;
            char op = *p++;
            if (op == '>' && *p == '>') {
                p++;
            }
            if (*p != '\0') {
                char saved = *p;
                *p = '\0';
                p++;
                /* Put back what we overwrote unless it was a space:
                 * the operator token ends here either way. */
                if (saved != ' ' && saved != '\t') {
                    p--;
                    *p = saved;
                }
            }
            continue;
        }

        /* A word, possibly containing quoted runs. The word is built
         * in place: `"a b"c` becomes `a bc`, exactly as a shell would
         * hand it to the program. */
        char *out = p;
        tok[n++] = out;
        int quote = 0;
        while (*p != '\0') {
            char c = *p;
            if (quote == 0 && (c == ' ' || c == '\t' || c == '|' ||
                               c == '<' || c == '>')) {
                break;
            }
            if (quote == 0 && (c == '"' || c == '\'')) {
                quote = c;
                p++;
                continue;
            }
            if (quote != 0 && c == quote) {
                quote = 0;
                p++;
                continue;
            }
            *out++ = c;
            p++;
        }
        char rest = *p;
        *out = '\0';
        if (rest != '\0' && out != p) {
            *p = rest; /* the operator we stopped on is still a token */
        }
        if (rest == ' ' || rest == '\t') {
            p++;
        }
    }
    return n;
}

/* Group the tokens into pipeline stages and pull out the
 * redirections. Returns the number of stages, or -1 on a syntax
 * error. */
static int parse_pipeline(char **tok, int ntok, struct stage *stages,
                          int max_stages) {
    int nstage = 0;
    memset(&stages[0], 0, sizeof(stages[0]));

    for (int i = 0; i < ntok; i++) {
        const char *t = tok[i];
        struct stage *s = &stages[nstage];

        if (strcmp(t, "|") == 0) {
            if (s->argc == 0 || nstage + 1 >= max_stages) {
                return -1;
            }
            nstage++;
            memset(&stages[nstage], 0, sizeof(stages[nstage]));
            continue;
        }
        if (strcmp(t, ">") == 0 || strcmp(t, ">>") == 0 ||
            strcmp(t, "<") == 0) {
            if (i + 1 >= ntok) {
                return -1;
            }
            char *file = tok[++i];
            if (t[0] == '<') {
                s->in_file = file;
            } else {
                s->out_file = file;
                s->out_mode = t[1] == '>' ? RD_APPEND : RD_TRUNC;
            }
            continue;
        }
        /* `2>file` and `2>>file`: stderr, the way sh spells it. */
        if (t[0] == '2' && t[1] == '>') {
            const char *name = t[2] == '>' ? t + 3 : t + 2;
            int mode = t[2] == '>' ? RD_APPEND : RD_TRUNC;
            if (*name == '\0') {
                if (i + 1 >= ntok) {
                    return -1;
                }
                name = tok[++i];
            }
            s->err_file = (char *)name;
            s->err_mode = mode;
            continue;
        }
        if (s->argc >= ARG_MAX) {
            return -1;
        }
        s->argv[s->argc++] = (char *)t;
        s->argv[s->argc] = 0;
    }

    if (stages[nstage].argc == 0) {
        return nstage == 0 ? 0 : -1;
    }
    return nstage + 1;
}

/* ---- running commands ---- */

static int is_builtin(const char *name) {
    static const char *const names[] = { "ls", "cat", "cd", "pwd", "echo",
                                         "history", "clear", "help", "exit" };
    for (unsigned i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        if (strcmp(name, names[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static void run_builtin(struct stage *s) {
    const char *name = s->argv[0];

    if (strcmp(name, "exit") == 0) {
        hx_destroy_window(win);
        hx_close(&dpy);
        _exit(0);
    }
    if (strcmp(name, "clear") == 0) {
        grid_clear();
        return;
    }
    if (strcmp(name, "help") == 0) {
        cmd_help();
        return;
    }
    if (strcmp(name, "history") == 0) {
        cmd_history();
        return;
    }
    if (strcmp(name, "pwd") == 0) {
        bout(g_cwd);
        bout("\n");
        return;
    }
    if (strcmp(name, "echo") == 0) {
        for (int i = 1; i < s->argc; i++) {
            bout(s->argv[i]);
            bout(i + 1 < s->argc ? " " : "\n");
        }
        if (s->argc == 1) {
            bout("\n");
        }
        return;
    }
    if (strcmp(name, "ls") == 0) {
        cmd_ls(s->argc, s->argv);
        return;
    }
    if (strcmp(name, "cat") == 0) {
        cmd_cat(s->argc, s->argv);
        return;
    }
    if (strcmp(name, "cd") == 0) {
        cmd_cd(s->argc, s->argv);
        return;
    }
}

static int open_redirect(const char *file, int mode) {
    char path[PATH_MAX_T];
    resolve(file, path, sizeof(path));

    if (mode == RD_INPUT) {
        return open(path, O_RDONLY);
    }
    int flags = O_WRONLY | O_CREAT | (mode == RD_APPEND ? O_APPEND : O_TRUNC);
    return open(path, flags);
}

/*
 * Run one pipeline.
 *
 * The descriptor choreography is the shell's classic one, and the
 * order matters: a stage is fully set up, started, and its pipe write
 * end closed *before* the next stage is started. TUS has no fork - a
 * spawned program inherits a copy of the fd table as it is at that
 * moment - so a write end still open here would be inherited by the
 * reader downstream, and a pipe whose reader holds its own write end
 * never reaches end of file. That is the difference between `ls |
 * grep hx` printing its matches and hanging forever.
 *
 * The last stage writes into a capture pipe the main loop drains into
 * the window. A built-in whose output would go there writes into the
 * grid instead: hxterm cannot drain that pipe while it is itself
 * inside the built-in.
 */
static int run_pipeline(struct stage *stages, int nstage) {
    int cap[2];
    if (pipe(cap) != 0) {
        berr("cannot create a pipe\n");
        return -1;
    }

    int save_in = dup(0);
    int save_out = dup(1);
    int save_err = dup(2);

    int prev_read = -1;
    int failed = 0;
    int spawned = 0;

    for (int i = 0; i < nstage && !failed; i++) {
        struct stage *s = &stages[i];
        int stage_in = -1, stage_out = -1, stage_err = -1;
        int next_read = -1;

        /* stdin: a file, or the pipe from the previous stage. */
        if (s->in_file != NULL) {
            stage_in = open_redirect(s->in_file, RD_INPUT);
            if (stage_in < 0) {
                berr(s->in_file);
                berr(": cannot open\n");
                failed = 1;
            }
            if (prev_read >= 0) {
                close(prev_read);
                prev_read = -1;
            }
        } else if (prev_read >= 0) {
            stage_in = prev_read;
            prev_read = -1;
        }

        /* stdout: a file, a pipe to the next stage, or the capture
         * pipe (left as -1 and filled in at the dup2). */
        if (!failed && s->out_file != NULL) {
            stage_out = open_redirect(s->out_file, s->out_mode);
            if (stage_out < 0) {
                berr(s->out_file);
                berr(": cannot create\n");
                failed = 1;
            }
        } else if (!failed && i < nstage - 1) {
            int pp[2];
            if (pipe(pp) != 0) {
                berr("cannot create a pipe\n");
                failed = 1;
            } else {
                stage_out = pp[1];
                next_read = pp[0];
            }
        }

        if (!failed && s->err_file != NULL) {
            stage_err = open_redirect(s->err_file, s->err_mode);
        }

        if (!failed && is_builtin(s->argv[0])) {
            if (stage_in >= 0) {
                dup2(stage_in, 0);
                g_in_redirected = 1;
            }
            g_out_fd = stage_out;   /* -1 means "into the window" */
            g_err_fd = stage_err;
            run_builtin(s);
            g_out_fd = -1;
            g_err_fd = -1;
            if (stage_in >= 0) {
                dup2(save_in, 0);
                g_in_redirected = 0;
            }
        } else if (!failed) {
            char path[PATH_MAX_T];
            if (strchr(s->argv[0], '/') != NULL) {
                resolve(s->argv[0], path, sizeof(path));
            } else {
                snprintf(path, sizeof(path), "/bin/%s", s->argv[0]);
            }

            char *argv[ARG_MAX + 1];
            argv[0] = path;
            for (int a = 1; a < s->argc && a < ARG_MAX; a++) {
                argv[a] = s->argv[a];
            }
            argv[s->argc < ARG_MAX ? s->argc : ARG_MAX] = 0;

            if (stage_in >= 0) {
                dup2(stage_in, 0);
            }
            dup2(stage_out >= 0 ? stage_out : cap[1], 1);
            dup2(stage_err >= 0 ? stage_err : cap[1], 2);

            int pid = hx_spawn(path, argv);

            dup2(save_in, 0);
            dup2(save_out, 1);
            dup2(save_err, 2);

            if (pid < 0) {
                berr(s->argv[0]);
                berr(": command not found\n");
                failed = 1;
            } else {
                spawned++;
            }
        }

        /* Everything this stage was given is now the child's problem
         * (or already used by the built-in): let go of our copies so
         * the next stage inherits a clean table. */
        if (stage_in >= 0) {
            close(stage_in);
        }
        if (stage_out >= 0) {
            close(stage_out);
        }
        if (stage_err >= 0) {
            close(stage_err);
        }
        if (failed && next_read >= 0) {
            close(next_read);
            next_read = -1;
        }
        prev_read = next_read;
    }

    if (prev_read >= 0) {
        close(prev_read);
    }
    close(save_in);
    close(save_out);
    close(save_err);
    close(cap[1]);

    if (spawned == 0) {
        /* Nothing is running: no output will arrive on the pipe. */
        close(cap[0]);
        return 0;
    }
    g_child_fd = cap[0];
    return 1;
}

/* Drain whatever the running programs have written. Returns 1 while
 * they are still going, 0 once the pipe reaches EOF. */
static int pump_child(void) {
    if (g_child_fd < 0) {
        return 0;
    }
    struct pollfd pfd;
    pfd.fd = g_child_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    while (poll(&pfd, 1, 0) > 0 && (pfd.revents & (POLLIN | POLLHUP)) != 0) {
        char buf[512];
        long n = read(g_child_fd, buf, sizeof(buf));
        if (n > 0) {
            term_write(buf, (int)n);
            continue;
        }
        /* n == 0: end of file - every write end is closed. */
        close(g_child_fd);
        g_child_fd = -1;
        return 0;
    }
    return 1;
}

static void execute(char *line) {
    char *tok[ARG_MAX * MAX_STAGES];
    struct stage stages[MAX_STAGES];

    int ntok = tokenize(line, tok, (int)(sizeof(tok) / sizeof(tok[0])));
    if (ntok <= 0) {
        if (ntok < 0) {
            berr("too many arguments\n");
        }
        prompt();
        return;
    }

    int nstage = parse_pipeline(tok, ntok, stages, MAX_STAGES);
    if (nstage <= 0) {
        berr("syntax error\n");
        prompt();
        return;
    }

    if (run_pipeline(stages, nstage) == 0) {
        prompt(); /* everything ran in-process: the prompt comes back */
    }
    /* Otherwise the prompt returns when the capture pipe hits EOF. */
}

/* ---- input ---- */

static void on_key(unsigned int key, unsigned int mods) {
    if ((mods & (HX_MOD_ALT | HX_MOD_SUPER)) != 0) {
        return; /* window manager territory */
    }
    if (g_child_fd >= 0) {
        return; /* a program is running: nothing to type into */
    }

    switch (key) {
    case HX_KEY_UP:
        history_browse(-1);
        return;
    case HX_KEY_DOWN:
        history_browse(1);
        return;
    case HX_KEY_LEFT:
        if (g_pos > 0) {
            g_pos--;
            redraw_input();
        }
        return;
    case HX_KEY_RIGHT:
        if (g_pos < g_len) {
            g_pos++;
            redraw_input();
        }
        return;
    case HX_KEY_HOME:
        g_pos = 0;
        redraw_input();
        return;
    case HX_KEY_END:
        g_pos = g_len;
        redraw_input();
        return;
    case HX_KEY_DELETE:
        if (g_pos < g_len) {
            memmove(g_line + g_pos, g_line + g_pos + 1,
                    (size_t)(g_len - g_pos));
            g_len--;
            redraw_input();
        }
        return;
    default:
        break;
    }

    if (key == '\n' || key == '\r') {
        char line[LINE_MAX];
        memcpy(line, g_line, sizeof(line));
        history_push(line);

        g_cur_r = g_input_row;
        g_cur_c = g_input_col + g_len;
        term_putc('\n');
        g_len = 0;
        g_pos = 0;
        g_line[0] = '\0';
        execute(line);
        return;
    }
    if (key == '\b' || key == 0x7F) {
        if (g_pos > 0) {
            memmove(g_line + g_pos - 1, g_line + g_pos,
                    (size_t)(g_len - g_pos + 1));
            g_pos--;
            g_len--;
            redraw_input();
        }
        return;
    }
    if (key == 0x0C) { /* Ctrl+L, as in every shell */
        grid_clear();
        prompt();
        return;
    }
    if (key >= 0x20 && key < 0x7F && g_len + 1 < LINE_MAX) {
        memmove(g_line + g_pos + 1, g_line + g_pos,
                (size_t)(g_len - g_pos + 1));
        g_line[g_pos] = (char)key;
        g_pos++;
        g_len++;
        redraw_input();
    }
}

int main(void) {
    if (hx_open(&dpy) < 0) {
        printf("hxterm: no highX display (start one with `highx`)\n");
        return 1;
    }
    if (win_w > dpy.screen_w) {
        win_w = dpy.screen_w;
    }
    if (win_h > dpy.screen_h) {
        win_h = dpy.screen_h;
    }

    win = hx_create_window(80, 80, win_w, win_h, 0, COL_BG, "hxterm");
    if (win == 0) {
        printf("hxterm: cannot create a window\n");
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);

    resize(win_w, win_h);
    grid_clear();
    term_puts("hxterm - TUS terminal.  Type `help` for the built-ins.\n");
    prompt();
    flush();

    for (;;) {
        struct hx_event ev;
        /* A short wait keeps typing responsive; a running program is
         * polled on every pass through the loop. */
        int n = hx_next_event(&ev, g_child_fd >= 0 ? 15 : 60);
        while (n > 0) {
            if (ev.type == HX_EV_CLOSE) {
                hx_destroy_window(win);
                hx_close(&dpy);
                return 0;
            }
            if (ev.type == HX_EV_CONFIGURE) {
                resize(ev.w, ev.h);
                redraw_all();
            } else if (ev.type == HX_EV_EXPOSE) {
                redraw_all();
            } else if (ev.type == HX_EV_KEY) {
                on_key(ev.key, ev.mods);
            }
            n = hx_next_event(&ev, 0);
        }

        if (g_child_fd >= 0 && pump_child() == 0) {
            prompt(); /* the programs finished: hand the line back */
        }
        flush();
    }
}
