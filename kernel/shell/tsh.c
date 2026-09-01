/*
 * tsh.c - TUS shell implementation
 *
 * One shell loop serves two callers. The console shell (task 0)
 * blocks in kbd_getchar(), which halts the CPU until the keyboard
 * interrupt wakes it. A terminal session's shell (one ring-0 task per
 * window) blocks the same way on the session's input ring, and its
 * output leaves through the console layer, which hands it to the
 * session instead of the framebuffer (kernel/term/term.c).
 *
 * The state a shell edits - the line, the history, the entry the
 * arrow keys are showing - therefore cannot be static: it lives in a
 * struct tsh_shell that the console owns one of and every session
 * carries its own. tsh_shell_current() is how the built-ins
 * (`history`) find the right one: they ask which shell the CALLING
 * TASK is running, which is the same question everything else in the
 * session machinery asks.
 */

#include "tsh.h"

#include "commands.h"
#include "../core/console.h"
#include "../core/klib.h"
#include "../drivers/keyboard/keyboard.h"
#include "../sched/sched.h"
#include "../term/term.h"
#include "../vfs/devices.h"

/* TUS palette */
#define COLOR_FG     0x00E8E8E8 /* near-white text */
#define COLOR_BG     0x00000000 /* black background */
#define COLOR_ACCENT 0x00FFA040 /* warm toast orange */

/* The console shell's state (task 0). A session's lives in its
 * struct tsh_term. */
static struct tsh_shell g_console_shell = { .browse = -1 };

/* Which shell is the calling task running? A built-in called from a
 * terminal window must walk that window's history, not the console's. */
static struct tsh_shell *tsh_shell_current(void) {
    struct tsh_term *t = term_current();
    return t != NULL ? &t->shell : &g_console_shell;
}

int tsh_history_count(void) {
    return tsh_shell_current()->history_count;
}

const char *tsh_history_get(int index) {
    struct tsh_shell *sh = tsh_shell_current();
    if (index < 0 || index >= sh->history_count) {
        return "";
    }
    return sh->history[index];
}

static void tsh_history_add(struct tsh_shell *sh, const char *line) {
    if (line[0] == '\0') {
        return;
    }
    if (sh->history_count > 0 &&
        strcmp(sh->history[sh->history_count - 1], line) == 0) {
        return; /* a repeated line is not worth a second slot */
    }
    if (sh->history_count == TSH_HISTORY_MAX) {
        for (int i = 1; i < TSH_HISTORY_MAX; i++) {
            memcpy(sh->history[i - 1], sh->history[i], TSH_LINE_MAX);
        }
        sh->history_count--;
    }
    strncpy(sh->history[sh->history_count], line, TSH_LINE_MAX - 1);
    sh->history[sh->history_count][TSH_LINE_MAX - 1] = '\0';
    sh->history_count++;
}

/* Erase what is on the line and type `text` in its place. The console
 * backspace is destructive, so this is all the redrawing a one-line
 * editor needs. */
static void tsh_replace_line(struct tsh_shell *sh, const char *text) {
    while (sh->len > 0) {
        console_putchar('\b');
        sh->len--;
    }
    for (int i = 0; text[i] != '\0' && sh->len < TSH_LINE_MAX - 1; i++) {
        sh->line[sh->len++] = text[i];
        console_putchar(text[i]);
    }
    sh->line[sh->len] = '\0';
}

/* Up (direction < 0) walks towards older entries, Down back towards
 * the line that was being typed. */
static void tsh_browse_history(struct tsh_shell *sh, int direction) {
    if (sh->history_count == 0) {
        return;
    }
    if (direction < 0) {
        if (sh->browse == -1) {
            sh->line[sh->len] = '\0';
            strncpy(sh->stash, sh->line, TSH_LINE_MAX - 1);
            sh->stash[TSH_LINE_MAX - 1] = '\0';
            sh->browse = sh->history_count - 1;
        } else if (sh->browse > 0) {
            sh->browse--;
        }
        tsh_replace_line(sh, sh->history[sh->browse]);
        return;
    }
    if (sh->browse == -1) {
        return;
    }
    sh->browse++;
    if (sh->browse >= sh->history_count) {
        sh->browse = -1;
        tsh_replace_line(sh, sh->stash);
        return;
    }
    tsh_replace_line(sh, sh->history[sh->browse]);
}

/* Print the prompt in the accent color, then switch back. The
 * prompt shows the shell's working directory, like a real UNIX
 * shell: tus:/tmp>  (the directory part comes from cmd_fs.c). */
static void tsh_prompt(void) {
    console_set_color(COLOR_ACCENT, COLOR_BG);
    console_write("tus:");
    console_write(shell_cwd());
    console_write("> ");
    console_set_color(COLOR_FG, COLOR_BG);
}

/* Execute the accumulated line and reset the editor. */
static void tsh_process_line(struct tsh_shell *sh) {
    sh->line[sh->len] = '\0';
    tsh_history_add(sh, sh->line);
    sh->browse = -1;
    command_execute(sh->line);
    sh->len = 0;
}

/*
 * One keystroke. Returns 0 while the shell should keep reading and 1
 * when the line editor has been asked to stop (only a session can be
 * stopped: the console shell is the machine's last resort).
 */
static /* One codepoint as UTF-8; returns the byte count, 0 for a value that
 * must not be encoded. */
static int tsh_utf8_encode(uint32_t cp, char *out) {
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

void tsh_key(struct tsh_shell *sh, struct kbd_event ev) {
    if (ev.type == KBD_EVENT_SPECIAL) {
        /* PageUp/PageDown navigate the framebuffer scrollback. A
         * terminal window has a scrollback of its own and never sends
         * these; console_scroll_page() is a no-op for a session. */
        if (ev.code == KBD_KEY_PAGE_UP) {
            console_scroll_page(1);
            return;
        }
        if (ev.code == KBD_KEY_PAGE_DOWN) {
            console_scroll_page(-1);
            return;
        }
        /* Up/Down walk the command history, as in any shell. */
        if (ev.code == KBD_KEY_UP) {
            tsh_browse_history(sh, -1);
            return;
        }
        if (ev.code == KBD_KEY_DOWN) {
            tsh_browse_history(sh, 1);
            return;
        }
        return; /* other special keys are ignored by the shell */
    }
    if (ev.type != KBD_EVENT_CHAR) {
        return;
    }

    /* A character outside ASCII - a Turkish letter typed on a Turkish
     * layout - arrives as a codepoint, because that is what a
     * keyboard produces. The line buffer holds BYTES (it is handed to
     * programs and written to files), so the codepoint is encoded
     * here, at the seam.
     *
     * Events synthesised from a terminal session (term_key below)
     * leave .cp zero and put the raw byte in .c: a terminal already
     * delivers UTF-8, one byte at a time, and re-encoding it would
     * mangle it. Those bytes fall through to the printable branch. */
    if (ev.cp >= 0x80) {
        char enc[4];
        int n = tsh_utf8_encode(ev.cp, enc);
        for (int i = 0; i < n && sh->len < TSH_LINE_MAX - 1; i++) {
            sh->line[sh->len++] = enc[i];
            console_putchar(enc[i]);
        }
        return;
    }

    char c = ev.c;

    if (c == '\n') {
        console_putchar('\n');
        tsh_process_line(sh);
        /* `exit` in a window closes it; printing one more prompt into
         * a terminal that is going away is just noise on the way out. */
        struct tsh_term *t = term_current();
        if (t == NULL || !t->closing) {
            tsh_prompt();
        }
    } else if (c == '\b' || c == 0x7F) {
        if (sh->len > 0) {
            sh->len--;
            /* A bare backspace only moves a real terminal's cursor
             * left; it does not erase what was there; the framebuffer
             * console erases on '\b' alone (fb_putchar, kernel/
             * drivers/fb.c), so the extra space+backspace are a no-op
             * there (idempotent: same final cursor cell either way) -
             * but they are exactly what a real serial terminal, or
             * anything else parsing this as a plain VT100 byte
             * stream, needs to see the character actually vanish. */
            console_putchar('\b');
            console_putchar(' ');
            console_putchar('\b');
        }
    } else if (c == 0x0C) { /* Ctrl+L: clear the screen */
        console_clear();
        tsh_prompt();
    } else if ((unsigned char)c >= 0x20 && (unsigned char)c != 0x7F) {
        /* Unsigned on purpose: a UTF-8 continuation byte from a
         * terminal session is 0x80 or above, which as a signed char
         * is negative and used to fail this test - which is what made
         * a Turkish letter typed into a terminal window vanish. */
        if (sh->len < TSH_LINE_MAX - 1) {
            sh->line[sh->len++] = c;
            console_putchar(c);
        }
    }
    /* any other control character is ignored */
}

void tsh_run(void) {
    /* The boot splash (toast logos + boot log) is wiped here: the
     * shell takes over the whole screen. */
    console_clear();
    tsh_prompt();

    for (;;) {
        /* A foreground user task (kilo and friends) owns the console
         * keyboard while it runs; kbd_get_event_shell() yields until
         * ownership comes back (the owner releases it in task_exit). */
        struct task *me = sched_current();
        long pid = me != NULL ? me->pid : 1;
        tsh_key(&g_console_shell, kbd_get_event_shell(pid));
    }
}

/* ---- the shell inside a terminal window ---- */

/*
 * A terminal sends what a terminal sends: bytes, with the arrow keys
 * and their friends as escape sequences. Turning those back into the
 * events the shell loop already understands is this function's whole
 * job. A lone ESC (the key, not a sequence) is told apart the way
 * every terminal emulator tells it apart: by whether more bytes are
 * ALREADY waiting - the client writes a sequence in one call, so its
 * bytes are never split across the ring.
 */
static struct kbd_event term_key(struct tsh_term *t, int byte) {
    struct kbd_event ev;
    ev.type = KBD_EVENT_NONE;
    ev.code = 0;
    ev.c = 0;
    ev.mods = 0;
    /* Zero, always: a terminal delivers a byte stream that is already
     * UTF-8, so .c carries the raw byte and there is no codepoint to
     * report. See the note in tsh_key. */
    ev.cp = 0;

    if (byte != 0x1B || !term_input_ready(t)) {
        ev.type = KBD_EVENT_CHAR;
        ev.c = (char)byte;
        /* A terminal sends CR for Enter; the shell wants a newline. */
        if (ev.c == '\r') {
            ev.c = '\n';
        }
        return ev;
    }

    int b = term_input_poll(t);
    if (b != '[' && b != 'O') {
        ev.type = KBD_EVENT_CHAR; /* ESC followed by something else */
        ev.c = (char)b;
        return ev;
    }

    int code = term_input_poll(t);
    ev.type = KBD_EVENT_SPECIAL;
    switch (code) {
    case 'A': ev.code = KBD_KEY_UP; break;
    case 'B': ev.code = KBD_KEY_DOWN; break;
    case 'C': ev.code = KBD_KEY_RIGHT; break;
    case 'D': ev.code = KBD_KEY_LEFT; break;
    case 'H': ev.code = KBD_KEY_HOME; break;
    case 'F': ev.code = KBD_KEY_END; break;
    default:
        /* ESC [ n ~ - Delete, Insert, PageUp, PageDown. The digits
         * are read up to the tilde so no stray byte is left behind. */
        if (code >= '0' && code <= '9') {
            int n = 0;
            while (code >= '0' && code <= '9') {
                n = n * 10 + (code - '0');
                code = term_input_poll(t);
            }
            switch (n) {
            case 2: ev.code = KBD_KEY_INSERT; break;
            case 3: ev.code = KBD_KEY_DELETE; break;
            case 5: ev.code = KBD_KEY_PAGE_UP; break;
            case 6: ev.code = KBD_KEY_PAGE_DOWN; break;
            default: ev.type = KBD_EVENT_NONE; break;
            }
        } else {
            ev.type = KBD_EVENT_NONE;
        }
        break;
    }
    return ev;
}

void tsh_run_term(struct tsh_term *t) {
    struct tsh_shell *sh = &t->shell;
    sh->browse = -1;

    console_write("TUS terminal - this is tsh, the kernel's own shell.\n"
                  "Type `help` for the built-ins; /bin is on the path.\n\n");
    tsh_prompt();

    for (;;) {
        int byte = term_input_getc(t);
        if (byte < 0) {
            return; /* the window is gone */
        }
        tsh_key(sh, term_key(t, byte));
        if (t->closing) {
            return; /* `exit` inside the window */
        }
    }
}
