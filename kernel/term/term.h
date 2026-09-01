/*
 * term.h - terminal sessions (kernel side)
 *
 * One session is one terminal window: a tsh task of its own, two ring
 * buffers and the small amount of shell state that must not be shared
 * with the console shell (the line editor, the history, the working
 * directory). See include/tusterm.h for the protocol the terminal
 * application speaks, and kernel/shell/tsh.c for the shell that runs
 * inside one.
 *
 * The rule that makes this work with no pty layer: a session belongs
 * to a TASK, not to a file descriptor. struct task carries a pointer
 * to it, task_create_user() copies that pointer into every child, and
 * the three places that produce or consume console text - the console
 * layer, /dev/tty0 and the shell's own line editor - ask for the
 * current task's session first. A program started from a terminal
 * therefore prints into that terminal and reads its keys, without
 * knowing sessions exist.
 */

#ifndef TUS_TERM_TERM_H
#define TUS_TERM_TERM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <tusterm.h>

#include "../shell/tsh.h"

/* Ring sizes. The output ring has to survive one program printing
 * faster than the window repaints (`ls /bin` into a fresh grid), the
 * input ring only ever holds what a human typed. */
#define TERM_OUT_SIZE 32768
#define TERM_IN_SIZE  1024

#define TERM_CWD_MAX 128

struct tsh_term {
    bool     used;
    uint32_t id;
    uint32_t owner;      /* pid of the terminal application */
    uint32_t shell_pid;  /* pid of the tsh task */
    uint32_t cols, rows;
    bool     closing;    /* the client is gone: the shell must stop */
    uint32_t dropped;    /* output bytes lost to a full ring */

    /* Single producer, single consumer in each direction. The
     * producer only ever moves head, the consumer only tail, and
     * every move happens with preemption disabled, so a task switch
     * in the middle of a copy cannot tear a byte in half. */
    volatile uint32_t in_head, in_tail;
    volatile uint32_t out_head, out_tail;
    char in[TERM_IN_SIZE];
    char out[TERM_OUT_SIZE];

    /* Per-session shell state: the line editor and history live here
     * so two windows do not type into each other's line, and the
     * working directory so `cd` in one is not `cd` in all of them. */
    struct tsh_shell shell;
    char cwd[TERM_CWD_MAX];
    char oldpwd[TERM_CWD_MAX];
};

/* The session the current task belongs to, or NULL for the console
 * shell and anything it started. */
struct tsh_term *term_current(void);

/* Console capture hook (installed on console.c at boot): takes the
 * bytes when the current task belongs to a session. */
bool term_console_capture(const char *s, size_t n);

/* Input for the shell running in `t`: the next byte, or -1 once the
 * session is closing. Yields (hlt) while the ring is empty. */
int term_input_getc(struct tsh_term *t);

/* Non-blocking variants, for /dev/tty0's poll() and read(). */
bool term_input_ready(struct tsh_term *t);
int  term_input_poll(struct tsh_term *t);

/* Write to the session's output ring (what the window shows). */
void term_output(struct tsh_term *t, const char *s, size_t n);

/* The system call (SYS_TERM). `from_user` gates the pointer checks. */
long term_syscall(long op, void *arg, size_t len, bool from_user);

/* A task exited: close the sessions it owned (called from task_exit,
 * next to highx_client_exit). */
void term_client_exit(uint32_t pid);

/* Install the console hook. Called once, from the boot sequence. */
void term_init(void);

#endif /* TUS_TERM_TERM_H */
