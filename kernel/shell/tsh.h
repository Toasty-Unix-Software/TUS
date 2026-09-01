/*
 * tsh.h - TUS shell
 *
 * tsh is the interactive command line of TUS. It reads characters,
 * maintains a small line editor (backspace, Ctrl+L to clear, Up/Down
 * for the command history) and dispatches completed lines to the
 * command table.
 *
 * There are two places a tsh can run, and they differ only in where
 * its characters come from and where its output goes:
 *
 *   tsh_run()      task 0, reading the PS/2 keyboard and writing the
 *                  framebuffer console - the shell you get at boot.
 *   tsh_run_term() one ring-0 task per terminal window, reading the
 *                  session's input ring and writing its output ring
 *                  (see kernel/term/term.h).
 *
 * Everything else - the built-ins, pipelines, redirection, spawning
 * programs - is the same code in both, which is the point: a terminal
 * window runs the real shell, not a copy of it.
 */

#ifndef TUS_SHELL_TSH_H
#define TUS_SHELL_TSH_H

/* Maximum length of a single command line, including the NUL byte. */
#define TSH_LINE_MAX 128

/* Number of command lines kept in the history. */
#define TSH_HISTORY_MAX 32

struct tsh_term;

/* One shell's editing state. The console shell owns a static one;
 * every terminal session carries its own inside struct tsh_term, so
 * two windows never type into the same line or share a history. */
struct tsh_shell {
    char line[TSH_LINE_MAX];
    int  len;
    char history[TSH_HISTORY_MAX][TSH_LINE_MAX];
    int  history_count;
    /* The entry the arrow keys are showing, or -1 while a fresh line
     * is being typed (its text waits in `stash`). */
    int  browse;
    char stash[TSH_LINE_MAX];
};

/* Run the console shell; never returns. */
void tsh_run(void);

/* Run a shell inside a terminal session. Returns when the session
 * ends (the window closed, or `exit` was typed). */
void tsh_run_term(struct tsh_term *term);

/* The command history of the shell the calling task is running -
 * what the Up/Down keys walk and what the `history` command prints. */
int tsh_history_count(void);
const char *tsh_history_get(int index);

#endif /* TUS_SHELL_TSH_H */
