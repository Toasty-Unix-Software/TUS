/*
 * tusterm.h - terminal sessions (v1.0)
 *
 * A terminal session is the kernel side of a terminal window: a real
 * tsh, running as its own ring-0 task, with its console wired to a
 * pair of ring buffers instead of the framebuffer.
 *
 *     long term(int op, void *arg, unsigned long len);
 *
 * The shape is highX's (see include/highx.h): one system call carries
 * the whole protocol, `op` selects the request, `arg` points at the
 * matching structure and `len` is its size, which the kernel checks
 * before touching a byte. This header is the entire ABI - the kernel
 * and the terminal application include exactly this file.
 *
 * What a session gives a terminal application that a private shell
 * cannot:
 *
 *   - the real tsh, with its built-ins, its history and its cwd;
 *   - the output of kernel built-ins (`ls`, `sysinfo`, `ps`), which
 *     is written with kprintf and never went near a file descriptor;
 *   - programs in /bin, whose stdin/stdout/stderr are the session's
 *     because a spawned task inherits the fd table AND the session;
 *   - keyboard input for those programs, since /dev/tty0 reads from
 *     the session when the task belongs to one.
 *
 * The client writes keystrokes (TERM_OP_WRITE) and reads whatever the
 * session produced (TERM_OP_READ). Neither ever blocks: a terminal is
 * an event loop, and the one thing it must not do is stop answering
 * its window while a program prints.
 *
 * The output the client reads is a byte stream with ANSI escape
 * sequences in it, exactly like a serial line: the kernel console's
 * colors arrive as SGR (ESC[38;2;r;g;bm), a screen clear as ESC[2J,
 * and everything kilo or a shipped program writes passes through
 * untouched.
 */

#ifndef TUSTERM_H
#define TUSTERM_H

#include <stdint.h>

#define TERM_MAGIC 0x4D524554u /* 'TERM' */

/* Server limits a client may rely on. */
#define TERM_MAX_SESSIONS 4
#define TERM_COLS_MAX     200
#define TERM_ROWS_MAX     64

enum term_opcode {
    TERM_OP_OPEN = 1,  /* struct term_open   - start a shell */
    TERM_OP_READ,      /* struct term_io     - out: what it printed */
    TERM_OP_WRITE,     /* struct term_io     - in: keystrokes */
    TERM_OP_RESIZE,    /* struct term_size   - TIOCGWINSZ follows this */
    TERM_OP_CLOSE,     /* struct term_id     - end the session */
    TERM_OP_STATUS,    /* struct term_status */
    TERM_OP_MAX
};

struct term_open {
    uint32_t magic;     /* in:  TERM_MAGIC */
    uint32_t cols;      /* in:  the window's grid ... */
    uint32_t rows;      /* in:  ... which TIOCGWINSZ reports */
    uint32_t id;        /* out: the session id */
    uint32_t shell_pid; /* out: pid of the tsh task */
    uint32_t pad;
};

struct term_io {
    uint32_t id;
    uint32_t len;       /* bytes of `buf` to move */
    void    *buf;
};

struct term_size {
    uint32_t id;
    uint32_t cols;
    uint32_t rows;
};

struct term_id {
    uint32_t id;
};

struct term_status {
    uint32_t id;
    uint32_t alive;     /* out: the shell task is still running */
    uint32_t pending;   /* out: bytes waiting to be read */
    uint32_t dropped;   /* out: bytes lost because nobody read them */
};

#endif /* TUSTERM_H */
