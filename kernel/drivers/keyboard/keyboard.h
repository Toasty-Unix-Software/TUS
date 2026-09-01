/*
 * keyboard.h - PS/2 keyboard driver
 *
 * Interrupt-driven driver for the standard PS/2 keyboard (scancode
 * set 1). The IRQ1 handler decodes scancodes into events - ASCII
 * characters (honoring Shift, Caps Lock and Ctrl) plus special keys
 * such as PageUp/PageDown for scrollback navigation - and stores them
 * in an internal ring buffer. Consumers block on kbd_get_event().
 */

#ifndef TUS_DRIVERS_KEYBOARD_H
#define TUS_DRIVERS_KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

/* Event types produced by the keyboard. */
enum {
    KBD_EVENT_CHAR = 0,     /* ev.c holds the ASCII character */
    KBD_EVENT_SPECIAL,      /* ev.code holds a KBD_KEY_* code */
    KBD_EVENT_NONE,         /* nothing happened (an escape sequence
                               nobody has a key for); ignore it */
};

/* Modifier state carried by every event (ev.mods). Shift and Ctrl
 * already shape the character itself; Alt and Super do not, which is
 * exactly why they make good window-manager modifiers - a program
 * that ignores mods sees the same characters it always did. */
#define KBD_MOD_SHIFT 0x1
#define KBD_MOD_CTRL  0x2
#define KBD_MOD_ALT   0x4
#define KBD_MOD_SUPER 0x8
#define KBD_MOD_CAPS  0x10
/* AltGr - the right Alt key on a European keyboard. NOT reported as
 * KBD_MOD_ALT: on those layouts it is a third shift level, not a
 * modifier applications should bind to, and a window manager whose
 * Alt binding fired every time someone typed a Turkish '@' would be
 * unusable. See drivers/keymap.h. */
#define KBD_MOD_ALTGR 0x20

/* Special (non-ASCII) keys, reported as KBD_EVENT_SPECIAL. The tty
 * layer translates them into the escape sequences a real terminal
 * sends (ESC [ A ...); the shell maps PageUp/PageDown to scrollback
 * navigation. */
enum {
    KBD_KEY_UP = 1,
    KBD_KEY_DOWN,
    KBD_KEY_LEFT,
    KBD_KEY_RIGHT,
    KBD_KEY_HOME,
    KBD_KEY_END,
    KBD_KEY_DELETE,
    KBD_KEY_INSERT,
    KBD_KEY_PAGE_UP,
    KBD_KEY_PAGE_DOWN,
};

struct kbd_event {
    int type;
    int code;    /* valid when type == KBD_EVENT_SPECIAL */
    char c;      /* ASCII, when type == KBD_EVENT_CHAR and cp < 128;
                    0 for a character that has no ASCII form */
    int mods;    /* KBD_MOD_* held when the key was pressed */
    uint32_t cp; /* Unicode codepoint, when type == KBD_EVENT_CHAR */
};

/*
 * WHY BOTH c AND cp
 *
 * `c` is what every consumer written before TUS had keyboard layouts
 * reads, and for the ASCII that is most of what anyone types it is
 * exactly right. `cp` is the truth: a Turkish 's' with a cedilla is
 * U+015F, which is not a byte and has no `c`.
 *
 * A consumer that only wants to know whether Enter was pressed keeps
 * reading `c`. One that puts characters on a screen or into a pipe
 * reads `cp` and encodes it as UTF-8 - that is what the tty layer
 * does, and it is why a program reading /dev/tty0 gets the two bytes
 * of a Turkish letter rather than one byte of nothing.
 */

/* Register the IRQ1 handler and unmask the keyboard interrupt. */
void kbd_init(void);

/* Decode one scancode-set-1 byte into keyboard events.
 *
 * The IRQ1 handler calls this with the byte it read from the 8042.
 * The USB HID driver calls it too: the boot protocol reports which
 * keys are DOWN rather than which key changed, so usbhid.c diffs
 * consecutive reports back into make and break codes and feeds them
 * here. One decoder, one modifier state, one Caps Lock - a machine
 * with both kinds of keyboard behaves as if it had one. */
void kbd_feed_scancode(uint8_t scancode);

/* Push a fully decoded event into the ring, for an input source that
 * has no scancodes to offer at all. */
void kbd_inject_event(const struct kbd_event *ev);

/* Return the next event, blocking (halting) until one is available. */
struct kbd_event kbd_get_event(void);

/* Console input ownership. Only one task may consume keyboard events
 * at a time: the shell owns the console by default, a foreground
 * user program (kilo) takes it over on its first tty read and
 * releases it on exit (task_exit). The shell releases it before
 * spawning a program. The owner is re-checked on every wake, so a
 * task that lost ownership while blocked never eats a key. */
void kbd_input_release(long pid);
long kbd_input_owner(void);

/* The controlling terminal's foreground process group - TIOCSPGRP/
 * TIOCGPGRP on /dev/tty0 (kernel/vfs/devices.c) read and write this
 * directly. See the comment on g_fg_pgid (keyboard.c) for how this
 * relates to (and currently does not drive) Ctrl+C/Ctrl+Z. */
void kbd_set_fg_pgid(long pgid);
long kbd_get_fg_pgid(void);

/* Like kbd_get_event(), but only returns events while `pid` owns the
 * console input (claiming it first if it is free). While another
 * task owns it, this yields the CPU without consuming anything. */
struct kbd_event kbd_get_event_owned(long pid);

/* Same as kbd_get_event_owned(), except a real signal handler
 * (kernel/sched/sched.c's delivery check, run on every hlt()-wakeup
 * tick like every other wait in TUS) interrupting the caller with no
 * SA_RESTART ends the wait instead of silently resuming it: returns
 * 0 with *out filled in on a real keystroke, or -1 if a signal cut
 * the wait short (the caller - tty_read(), kernel/vfs/devices.c -
 * turns that into -EINTR). */
int kbd_get_event_owned_eintr(long pid, struct kbd_event *out);

/* The shell's variant: consumes events while the console is free or
 * owned by `pid`, but never claims it - a foreground user task must
 * be able to take the console over on its first tty read. */
struct kbd_event kbd_get_event_shell(long pid);

/* Return the next character, ignoring non-character events. */
char kbd_getchar(void);

/* Return the next character, or -1 if the buffer is empty. */
int kbd_poll(void);

/* True if at least one event is buffered. */
bool kbd_has_char(void);

#endif /* TUS_DRIVERS_KEYBOARD_H */
