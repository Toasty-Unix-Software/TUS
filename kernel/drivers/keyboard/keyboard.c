/*
 * keyboard.c - PS/2 keyboard driver implementation
 *
 * Scancode set 1 layout: a make code (key pressed) is the bare scan
 * code, the matching break code (key released) has bit 7 set. Some
 * keys send a 0xE0 prefix byte first; those (arrows, etc.) are
 * currently ignored.
 *
 * Shift and Ctrl are tracked as state; Caps Lock toggles and updates
 * the keyboard LED. Character generation follows the classic US layout.
 */

#include "drivers/keyboard/keyboard.h"

#include "drivers/keymap/keymap.h"

#include "arch/x86_64/idt.h"
#include "arch/x86_64/io.h"
#include "arch/x86_64/pic.h"
#include "../../sched/sched.h"

#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64

#define KBD_IRQ         1

#define KBD_BUFFER_SIZE 256

#define SC_EXTENDED     0xE0 /* prefix of multi-byte scancodes */
#define SC_CTRL         0x1D
#define SC_ALT          0x38 /* left Alt; E0 38 is the right one (AltGr) */
#define SC_LSHIFT       0x2A
#define SC_RSHIFT       0x36
#define SC_CAPS_LOCK    0x3A
#define SC_LSUPER       0x5B /* E0-prefixed: left/right Super ("Windows") */
#define SC_RSUPER       0x5C

/* Extended (0xE0-prefixed) make codes we understand. */
#define SC_PAGE_UP      0x49
#define SC_PAGE_DOWN    0x51

/* The US tables that used to live here are gone: what a scancode
 * means is now the layout's business (drivers/keymap.c), and having a
 * second answer in the driver is how the two would drift apart. */

/* ---- keyboard state ---- */

static volatile struct kbd_event g_buffer[KBD_BUFFER_SIZE];
static volatile int g_head; /* next slot to write (IRQ context) */
static volatile int g_tail; /* next slot to read (shell context) */

static bool g_shift_pressed;
static bool g_ctrl_pressed;
static bool g_alt_pressed;
static bool g_altgr_pressed; /* right Alt: a shift level, not a modifier */
static bool g_super_pressed;
static bool g_caps_locked;
static bool g_extended; /* set after a 0xE0 prefix byte */

/* A dead key waiting for the letter it modifies (0 = none). Holds a
 * combining mark; see drivers/keymap.h. */
static uint32_t g_dead_mark;

/* The modifier state as the consumers see it. */
static int kbd_mods(void) {
    return (g_shift_pressed ? KBD_MOD_SHIFT : 0) |
           (g_ctrl_pressed ? KBD_MOD_CTRL : 0) |
           (g_alt_pressed ? KBD_MOD_ALT : 0) |
           (g_altgr_pressed ? KBD_MOD_ALTGR : 0) |
           (g_super_pressed ? KBD_MOD_SUPER : 0) |
           (g_caps_locked ? KBD_MOD_CAPS : 0);
}

/* Push one event into the ring buffer; drops on overflow. */
static void kbd_push_event(const struct kbd_event *ev) {
    int next = (g_head + 1) % KBD_BUFFER_SIZE;
    if (next != g_tail) {
        g_buffer[g_head] = *ev;
        /* Every event carries the modifiers held at press time, so a
         * consumer never has to ask the driver about state that may
         * already have changed by the time it looks. */
        g_buffer[g_head].mods = kbd_mods();
        g_head = next;
    }
}

/* Push one character event, carrying both forms: the codepoint always,
 * and the ASCII byte when there is one. */
static void kbd_push_codepoint(uint32_t cp) {
    struct kbd_event ev = { KBD_EVENT_CHAR, 0,
                            cp < 128 ? (char)cp : (char)0, 0, cp };
    kbd_push_event(&ev);
}

/*
 * Update the Caps Lock LED. The keyboard must acknowledge each command
 * byte with 0xFA; we wait for it with a bounded poll so that hardware
 * which never answers cannot hang the interrupt handler.
 */
static void kbd_update_leds(void) {
    outb(KBD_DATA_PORT, 0xED); /* set LEDs command */
    for (unsigned i = 0; i < 10000; i++) {
        if (inb(KBD_STATUS_PORT) & 0x01) {
            (void)inb(KBD_DATA_PORT); /* ACK */
            break;
        }
    }
    outb(KBD_DATA_PORT, g_caps_locked ? 0x04 : 0x00); /* Caps Lock LED */
    for (unsigned i = 0; i < 10000; i++) {
        if (inb(KBD_STATUS_PORT) & 0x01) {
            (void)inb(KBD_DATA_PORT); /* ACK */
            break;
        }
    }
}

/*
 * IRQ1 handler: decode one scancode and feed the ring buffer.
 *
 * NOTE: this is a plain C function, NOT an __attribute__((interrupt))
 * function. The interrupt attribute belongs only to the IDT stubs in
 * idt.c (irq_stub_N), which dispatch to us with the interrupt frame
 * passed as a normal argument. Marking a registered handler as
 * "interrupt" would make it return with IRETQ instead of RET, popping
 * garbage - an instant #GP.
 */
static void kbd_irq_handler(struct interrupt_frame *frame) {
    (void)frame;

    /* The 8042 serves two devices. A byte flagged as auxiliary is the
     * mouse's (see drivers/mouse.c) and must be left in the buffer
     * for IRQ12, or the two drivers eat each other's data. */
    if ((inb(KBD_STATUS_PORT) & 0x20) != 0) {
        return;
    }

    kbd_feed_scancode(inb(KBD_DATA_PORT));
}

/*
 * Decode one scancode-set-1 byte into events.
 *
 * Split out of the IRQ handler so that it has two callers. The second
 * is the USB HID driver (drivers/usbhid.c), which turns the boot
 * protocol's "which keys are down" reports back into make and break
 * codes and feeds them here. Everything that makes a keyboard a
 * keyboard - the modifier state, Caps Lock and its LED, the
 * E0-prefixed arrows, Ctrl turning a letter into a control character
 * - is written once and both buses get it.
 *
 * The state this walks (g_extended, g_shift_pressed, ...) is shared
 * by both callers on purpose: a machine with a PS/2 keyboard and a
 * USB keyboard has ONE modifier state, and Shift held on one of them
 * capitalises a letter typed on the other, which is what a user who
 * has two keyboards plugged in expects.
 */
void kbd_feed_scancode(uint8_t scancode) {
    if (scancode == SC_EXTENDED) {
        g_extended = true;
        return;
    }
    if (g_extended) {
        g_extended = false;

        /* E0-prefixed modifiers: right Ctrl, right Alt and both
         * Super keys. They update state and produce no character -
         * the make/break pair is what tells us they are held. */
        bool ext_make = !(scancode & 0x80);
        switch (scancode & 0x7F) {
        case SC_CTRL:
            g_ctrl_pressed = ext_make;
            return;
        case SC_ALT:
            /* E0 38 is the RIGHT Alt key, which on every European
             * layout is AltGr - a third shift level, not Alt. Left
             * Alt (the bare 0x38 below) is the real modifier. */
            g_altgr_pressed = ext_make;
            return;
        case SC_LSUPER:
        case SC_RSUPER:
            g_super_pressed = ext_make;
            return;
        default:
            break;
        }

        if (!(scancode & 0x80)) { /* make (press) only */
            /* Extended keys: arrows, Home/End, Insert/Delete,
             * PageUp/PageDown (scancode set 1, E0-prefixed). */
            static const uint8_t extmap[128] = {
                [0x47] = KBD_KEY_HOME,
                [0x4F] = KBD_KEY_END,
                [0x48] = KBD_KEY_UP,
                [0x50] = KBD_KEY_DOWN,
                [0x4B] = KBD_KEY_LEFT,
                [0x4D] = KBD_KEY_RIGHT,
                [0x49] = KBD_KEY_PAGE_UP,
                [0x51] = KBD_KEY_PAGE_DOWN,
                [0x52] = KBD_KEY_INSERT,
                [0x53] = KBD_KEY_DELETE,
            };
            int key = extmap[scancode & 0x7F];
            if (key) {
                struct kbd_event ev = { KBD_EVENT_SPECIAL, key, 0, 0, 0 };
                kbd_push_event(&ev);
            }
        }
        return;
    }

    bool make = !(scancode & 0x80);
    uint8_t code = scancode & 0x7F;

    /* Modifier keys update state and produce no character. */
    switch (code) {
    case SC_CTRL:
        g_ctrl_pressed = make;
        return;
    case SC_ALT:
        g_alt_pressed = make;
        return;
    case SC_LSHIFT:
    case SC_RSHIFT:
        g_shift_pressed = make;
        return;
    case SC_CAPS_LOCK:
        if (make) {
            g_caps_locked = !g_caps_locked;
            kbd_update_leds();
        }
        return;
    default:
        break;
    }

    if (!make) {
        return; /* break codes of ordinary keys produce nothing */
    }

    /* The layout decides what this key means. Everything about which
     * letter a position carries - including the two Turkish i's and
     * whether Caps Lock applies at all - lives in drivers/keymap.c. */
    uint32_t cp = keymap_lookup(code, g_shift_pressed, g_altgr_pressed,
                                g_caps_locked);
    if (cp == 0) {
        return; /* this key produces no character on this layout */
    }

    /* A dead key: produce nothing now, and remember the accent. */
    if (cp >= KEYMAP_DEAD && cp <= KEYMAP_DEAD_MAX) {
        uint32_t mark = KEYMAP_MARK(cp);
        if (g_dead_mark != 0) {
            /* The same dead key twice is how every layout types the
             * accent on its own. */
            uint32_t spacing = keymap_spacing(g_dead_mark);
            if (spacing != 0) {
                kbd_push_codepoint(spacing);
            }
            g_dead_mark = (g_dead_mark == mark) ? 0 : mark;
            return;
        }
        g_dead_mark = mark;
        return;
    }

    /* A character after a dead key: compose if the pair exists. If it
     * does not, emit the accent and then the character - a user who
     * pressed the wrong key sees both, rather than losing one. */
    if (g_dead_mark != 0) {
        uint32_t mark = g_dead_mark;
        g_dead_mark = 0;
        if (cp == ' ') {
            uint32_t spacing = keymap_spacing(mark);
            kbd_push_codepoint(spacing != 0 ? spacing : cp);
            return;
        }
        uint32_t composed = keymap_compose(mark, cp);
        if (composed != 0) {
            kbd_push_codepoint(composed);
            return;
        }
        uint32_t spacing = keymap_spacing(mark);
        if (spacing != 0) {
            kbd_push_codepoint(spacing);
        }
    }

    /* Ctrl + letter produces the corresponding control character. It
     * is tested on the ASCII form on purpose: Ctrl+C means SIGINT
     * whatever layout is loaded, and Ctrl with a Turkish 's' has no
     * control character to become. */
    if (g_ctrl_pressed && ((cp >= 'a' && cp <= 'z') || (cp >= 'A' && cp <= 'Z'))) {
        uint32_t ctrl_cp = cp & 0x1F;

        /* Ctrl+C (0x03, ETX) and Ctrl+Z (0x1A, SUB): a real SIGINT/
         * SIGTSTP (kernel/sched/sched.c) on whatever exec_pipeline()
         * is currently waiting on, not queued as ordinary input. That
         * covers a job whether or not it ever reads the keyboard at
         * all - most don't (`ping`) - and queuing either byte as data
         * for the ones that do would just type "^C"/"^Z" into a
         * program instead of signalling it, which is not what a real
         * terminal in canonical mode does either. At an idle prompt
         * (nothing tracked) both are a harmless no-op. */
        if (ctrl_cp == 0x03) {
            sched_interrupt_foreground();
            return;
        }
        if (ctrl_cp == 0x1A) {
            sched_stop_foreground();
            return;
        }

        kbd_push_codepoint(ctrl_cp);
        return;
    }

    kbd_push_codepoint(cp);
}

void kbd_inject_event(const struct kbd_event *ev) {
    if (ev != 0) {
        kbd_push_event(ev);
    }
}

void kbd_init(void) {
    irq_install(KBD_IRQ, kbd_irq_handler);
    pic_enable_irq(KBD_IRQ);
}

struct kbd_event kbd_get_event(void) {
    /* Sleep until the keyboard IRQ wakes us with a new event. */
    while (g_head == g_tail) {
        hlt();
    }
    struct kbd_event ev = g_buffer[g_tail];
    g_tail = (g_tail + 1) % KBD_BUFFER_SIZE;
    return ev;
}

/* ---- console input ownership ---- */

static long g_kbd_owner; /* pid of the foreground console consumer */

/* The console's controlling-terminal foreground process group
 * (TIOCSPGRP/TIOCGPGRP on /dev/tty0, kernel/vfs/devices.c) - what a
 * real job-control shell's fg/bg change. Distinct from g_kbd_owner
 * above (which pid may currently read keystrokes): this is what
 * kernel/sched/sched.c's sched_interrupt_foreground()/
 * sched_stop_foreground() consult... actually those still use the
 * shell's own exec_pipeline()-tracked pid list (sched_set_foreground()),
 * not this pgid - g_fg_pgid exists so a job-control-aware program
 * (ksh) can query/set it the same way a real terminal driver
 * exposes one, even though TUS's own Ctrl+C/Ctrl+Z path does not
 * consult it yet. Starts at 0 (unset). */
static long g_fg_pgid;

void kbd_set_fg_pgid(long pgid) {
    g_fg_pgid = pgid;
}

long kbd_get_fg_pgid(void) {
    return g_fg_pgid;
}

void kbd_input_release(long pid) {
    if (g_kbd_owner == pid) {
        g_kbd_owner = 0;
    }
}

long kbd_input_owner(void) {
    return g_kbd_owner;
}

struct kbd_event kbd_get_event_owned(long pid) {
    for (;;) {
        /* Claim the console if it is free, otherwise yield until the
         * owner (or the shell) gives it up. Checking on every wake
         * is what makes the handover race-free: a task only consumes
         * events while it is the registered owner. */
        if (g_kbd_owner == 0) {
            g_kbd_owner = pid;
        }
        if (g_kbd_owner != pid) {
            hlt();
            continue;
        }
        if (g_head == g_tail) {
            hlt();
            continue;
        }
        struct kbd_event ev = g_buffer[g_tail];
        g_tail = (g_tail + 1) % KBD_BUFFER_SIZE;
        return ev;
    }
}

int kbd_get_event_owned_eintr(long pid, struct kbd_event *out) {
    for (;;) {
        if (g_kbd_owner == 0) {
            g_kbd_owner = pid;
        }
        if (g_kbd_owner != pid) {
            if (sched_signal_pending()) {
                return -1;
            }
            hlt();
            continue;
        }
        if (g_head == g_tail) {
            if (sched_signal_pending()) {
                return -1;
            }
            hlt();
            continue;
        }
        *out = g_buffer[g_tail];
        g_tail = (g_tail + 1) % KBD_BUFFER_SIZE;
        return 0;
    }
}

struct kbd_event kbd_get_event_shell(long pid) {
    for (;;) {
        /* The shell never claims the console: it consumes only while
         * it is free or already owned by us, so a foreground user
         * task can take over on its first read. */
        if (g_kbd_owner != 0 && g_kbd_owner != pid) {
            hlt();
            continue;
        }
        if (g_head == g_tail) {
            hlt();
            continue;
        }
        struct kbd_event ev = g_buffer[g_tail];
        g_tail = (g_tail + 1) % KBD_BUFFER_SIZE;
        return ev;
    }
}

char kbd_getchar(void) {
    for (;;) {
        struct kbd_event ev = kbd_get_event();
        if (ev.type == KBD_EVENT_CHAR) {
            return ev.c;
        }
        /* Scroll events are consumed by the console layer (the shell
         * never sees them as characters). */
    }
}

int kbd_poll(void) {
    if (g_head == g_tail) {
        return -1;
    }
    struct kbd_event ev = g_buffer[g_tail];
    g_tail = (g_tail + 1) % KBD_BUFFER_SIZE;
    if (ev.type != KBD_EVENT_CHAR) {
        return -1; /* non-character event; retry */
    }
    return ev.c;
}

bool kbd_has_char(void) {
    return g_head != g_tail;
}
