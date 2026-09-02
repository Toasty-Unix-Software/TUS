/*
 * devices.c - built-in device implementations
 *
 *   /dev/fb0     raw framebuffer: read/write pixels, ioctl for info/fill
 *   /dev/tty0    console terminal: write text, read keyboard (ESC = EOF)
 *   /dev/kbd0    keyboard: read one keypress at a time (ESC = EOF)
 *   /dev/serial0 COM1: write debug output
 *   /dev/null    write sink, read EOF
 *   /dev/zero    read zero bytes, write sink
 */

#include "devices.h"

#include "vfs.h"
#include "pty.h"

#include "../core/random.h"
#include "../core/console.h"
#include "../core/errno.h"
#include "../core/klib.h"
#include "../drivers/fb/fb.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/serial/serial.h"
#include "../sched/sched.h"
#include "../term/term.h"
#include "../drivers/ata/ata.h"
#include "../core/bootinfo.h"
/* ---- /dev/fb0 ---- */

/* The description /dev/fb0 hands out. It is a snapshot, not a live
 * read of the driver, because a program walking the framebuffer needs
 * one answer that stays put for the length of its frame.
 * devices_refresh_fb() re-takes it after a mode change. */
static struct fb_device_info g_fb_info;

static long fb_read(void *priv, void *buf, size_t count, size_t pos) {
    (void)priv;
    struct fb_device_info *info = (struct fb_device_info *)priv;
    if (pos >= info->pitch * info->height) {
        return 0;
    }
    if (count > info->pitch * info->height - pos) {
        count = info->pitch * info->height - pos;
    }
    memcpy(buf, (void *)(uintptr_t)info->address + pos, count);
    return (long)count;
}

static long fb_write(void *priv, const void *buf, size_t count, size_t pos) {
    struct fb_device_info *info = (struct fb_device_info *)priv;
    if (pos >= info->pitch * info->height) {
        return 0;
    }
    if (count > info->pitch * info->height - pos) {
        count = info->pitch * info->height - pos;
    }
    memcpy((void *)(uintptr_t)info->address + pos, buf, count);
    /* Pixels written straight to the framebuffer: the text console
     * has to stop believing it knows what is on screen. */
    fb_invalidate();
    return (long)count;
}

static int fb_ioctl(void *priv, uint64_t request, void *arg) {
    struct fb_device_info *info = (struct fb_device_info *)priv;
    switch (request) {
    case FB_IOCTL_GET_INFO:
        if (arg == NULL) {
            return -EINVAL;
        }
        memcpy(arg, info, sizeof(*info));
        return 0;
    case FB_IOCTL_FILL:
        fb_fill(arg != NULL ? *(uint32_t *)arg : 0);
        return 0;
    default:
        return -ENOTTY;
    }
}

/* ---- /dev/tty0 ---- */

/* TUS termios: byte-exact layout of the musl x86_64 struct termios
 * (4 x tcflag_t + cc_t c_line + cc_t c_cc[32] + 2 x speed_t = 57
 * bytes). The kernel stores the whole blob and interprets only the
 * flags it implements: ICRNL (Enter arrives as \r in raw mode),
 * ICANON (legacy ESC = EOF for `cat`) and ECHO (echo typed input). */
struct tus_termios {
    uint32_t c_iflag;
    uint32_t c_oflag;
    uint32_t c_cflag;
    uint32_t c_lflag;
    uint8_t c_line;
    uint8_t c_cc[32];
    uint32_t __c_ispeed;
    uint32_t __c_ospeed;
};

/* Linux ioctl request numbers for termios and window size (musl and
 * the TUS kernel agree on these). */
#define TCGETS     0x5401
#define TCSETS     0x5402
#define TCSETSW    0x5403
#define TCSETSF    0x5404
#define TIOCGWINSZ 0x5413
#define TIOCGPGRP  0x540F
#define TIOCSPGRP  0x5410

/* Termios flag bits the tty honours (Linux values, match musl). */
#define TUS_ICRNL  0x0100
#define TUS_ICANON 0x0002
#define TUS_ECHO   0x0008

/* Default canonical termios: BRKINT|ICRNL|IXON input, OPOST output,
 * CS8|CREAD|B38400 control, ISIG|ICANON|ECHO|ECHOE|ECHOK|IEXTEN
 * local, VMIN=1. */
static struct tus_termios g_termios = {
    .c_iflag = 0x0302,
    .c_oflag = 0x0001,
    .c_cflag = 0x10B1,
    .c_lflag = 0x803B,
    .c_line = 0,
    .c_cc = { [6] = 1 }, /* VMIN = 1 */
    .__c_ispeed = 0x1001, /* B38400 */
    .__c_ospeed = 0x1001,
};

/* Pending bytes of a multi-byte escape sequence (arrow keys etc.). */
static char g_tty_pend[8];
static int g_tty_pend_n;
static int g_tty_pend_i;

/* Translate a special key into the escape sequence a real terminal
 * would send, and queue its bytes. Returns the first byte. */
static char tty_special_byte(int code) {
    static const struct {
        const char *seq;
        int len;
    } map[] = {
        [KBD_KEY_UP]       = { "\x1b[A", 3 },
        [KBD_KEY_DOWN]     = { "\x1b[B", 3 },
        [KBD_KEY_RIGHT]    = { "\x1b[C", 3 },
        [KBD_KEY_LEFT]     = { "\x1b[D", 3 },
        [KBD_KEY_HOME]     = { "\x1b[H", 3 },
        [KBD_KEY_END]      = { "\x1b[F", 3 },
        [KBD_KEY_DELETE]   = { "\x1b[3~", 4 },
        [KBD_KEY_INSERT]   = { "\x1b[2~", 4 },
        [KBD_KEY_PAGE_UP]  = { "\x1b[5~", 4 },
        [KBD_KEY_PAGE_DOWN]= { "\x1b[6~", 4 },
    };
    if (code < 0 || code >= (int)(sizeof(map) / sizeof(map[0])) ||
        map[code].seq == NULL) {
        return 0;
    }
    for (int i = 0; i < map[code].len; i++) {
        g_tty_pend[i] = map[code].seq[i];
    }
    g_tty_pend_n = map[code].len;
    g_tty_pend_i = 1; /* first byte returned immediately */
    return g_tty_pend[0];
}

/* Encode one codepoint as UTF-8. Returns the number of bytes, or 0
 * for a codepoint that must not be encoded (a surrogate, or one past
 * the end of Unicode) - the same values the console's decoder
 * refuses, so what goes out is what would come back. */
static int tty_utf8_encode(uint32_t cp, uint8_t *out) {
    if (cp < 0x80) {
        out[0] = (uint8_t)cp;
        return 1;
    }
    if (cp < 0x800) {
        out[0] = (uint8_t)(0xC0 | (cp >> 6));
        out[1] = (uint8_t)(0x80 | (cp & 0x3F));
        return 2;
    }
    if (cp >= 0xD800 && cp <= 0xDFFF) {
        return 0;
    }
    if (cp < 0x10000) {
        out[0] = (uint8_t)(0xE0 | (cp >> 12));
        out[1] = (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
        out[2] = (uint8_t)(0x80 | (cp & 0x3F));
        return 3;
    }
    if (cp <= 0x10FFFF) {
        out[0] = (uint8_t)(0xF0 | (cp >> 18));
        out[1] = (uint8_t)(0x80 | ((cp >> 12) & 0x3F));
        out[2] = (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
        out[3] = (uint8_t)(0x80 | (cp & 0x3F));
        return 4;
    }
    return 0;
}

static long tty_read(void *priv, void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)pos;
    if (count == 0) {
        return 0;
    }

    /* A task inside a terminal session reads that window's keys, not
     * the machine's keyboard. The bytes are already what a terminal
     * sends - the escape sequences included - so there is nothing to
     * translate here. */
    struct tsh_term *term = term_current();
    if (term != NULL) {
        int c = term_input_getc(term);
        if (c < 0) {
            return 0; /* the window closed: end of input */
        }
        if (c == 0x1B && (g_termios.c_lflag & TUS_ICANON) != 0 &&
            !term_input_ready(term)) {
            return 0; /* ESC ends the stream, as on the console */
        }
        if (c == '\r' && (g_termios.c_iflag & TUS_ICRNL) != 0) {
            c = '\n';
        }
        if ((g_termios.c_lflag & TUS_ECHO) != 0 && c >= 0x20 && c != 0x7F) {
            console_putchar((char)c);
        }
        *(char *)buf = (char)c;
        return 1;
    }

    /* Drain any queued bytes of a multi-byte key sequence first. */
    if (g_tty_pend_i < g_tty_pend_n) {
        *(char *)buf = g_tty_pend[g_tty_pend_i++];
        if (g_tty_pend_i == g_tty_pend_n) {
            g_tty_pend_i = g_tty_pend_n = 0;
        }
        return 1;
    }

    struct task *cur = sched_current();
    long pid = cur != NULL ? cur->pid : 1;
    bool canon = (g_termios.c_lflag & TUS_ICANON) != 0;
    bool echo = (g_termios.c_lflag & TUS_ECHO) != 0;

    for (;;) {
        struct kbd_event ev;
        if (kbd_get_event_owned_eintr(pid, &ev) != 0) {
            return -EINTR; /* a signal with no SA_RESTART cut the wait short */
        }

        if (ev.type == KBD_EVENT_SPECIAL) {
            char b = tty_special_byte(ev.code);
            if (b == 0) {
                continue; /* unmapped key: drop */
            }
            *(char *)buf = b;
            return 1;
        }
        if (ev.type != KBD_EVENT_CHAR) {
            continue;
        }

        if (ev.c == 0x1B) {
            if (canon) {
                return 0; /* ESC ends the stream (lets `cat` exit) */
            }
            *(char *)buf = ev.c;
            return 1;
        }

        /* A character outside ASCII - a Turkish letter, a German
         * umlaut - has no single byte. It goes out as UTF-8 through
         * the same queue the arrow-key escape sequences use, so a
         * program reading one byte at a time gets the whole letter
         * across the next few reads. Doing it here rather than in the
         * keyboard driver is deliberate: the driver's job is to say
         * WHICH character was typed, and the tty's is to say how a
         * byte stream spells it. */
        if (ev.cp >= 0x80) {
            uint8_t enc[4];
            int n = tty_utf8_encode(ev.cp, enc);
            if (n == 0) {
                continue;
            }
            if (echo) {
                for (int i = 0; i < n; i++) {
                    console_putchar((char)enc[i]);
                }
            }
            *(char *)buf = (char)enc[0];
            g_tty_pend_i = 0;
            g_tty_pend_n = 0;
            for (int i = 1; i < n; i++) {
                g_tty_pend[g_tty_pend_n++] = (char)enc[i];
            }
            return 1;
        }

        char c = ev.c;
        /* A real terminal sends CR for Enter; with ICRNL cleared
         * (raw mode) the byte must stay \r. Our keyboard produces
         * \n directly, so invert the translation. */
        if (c == '\n' && (g_termios.c_iflag & TUS_ICRNL) == 0) {
            c = '\r';
        }
        if (echo && c >= 0x20 && c != 0x7F) {
            console_putchar(c);
        }
        *(char *)buf = c;
        return 1;
    }
}

static long tty_write(void *priv, const void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)pos;
    console_write_n((const char *)buf, count);
    return (long)count;
}

/* Readiness for poll()/select(). The console is writable at all times;
 * it is readable when a keypress is already buffered AND this task
 * would actually be allowed to consume it - a task that does not own
 * the console input would block in tty_read() even with a key pending,
 * so reporting POLLIN there would be a lie. Bytes left over from a
 * multi-byte escape sequence count as readable too. */
static short tty_poll(void *priv) {
    (void)priv;
    short r = POLLOUT;
    struct tsh_term *term = term_current();
    if (term != NULL) {
        return term_input_ready(term) ? (short)(r | POLLIN) : r;
    }
    struct task *cur = sched_current();
    long pid = cur != NULL ? cur->pid : 1;
    long owner = kbd_input_owner();
    if (g_tty_pend_i < g_tty_pend_n) {
        return r | POLLIN;
    }
    if ((owner == 0 || owner == pid) && kbd_has_char()) {
        r |= POLLIN;
    }
    return r;
}

static int tty_ioctl(void *priv, uint64_t request, void *arg) {
    (void)priv;
    switch (request) {
    case TCGETS:
        if (arg == NULL) {
            return -EINVAL;
        }
        memcpy(arg, &g_termios, sizeof(g_termios));
        return 0;
    case TCSETS:
    case TCSETSW:
    case TCSETSF:
        if (arg == NULL) {
            return -EINVAL;
        }
        memcpy(&g_termios, arg, sizeof(g_termios));
        return 0;
    case TIOCGWINSZ: {
        if (arg == NULL) {
            return -EINVAL;
        }
        int cols = 80, rows = 24;
        /* Inside a terminal session the window is the terminal, and
         * its grid is what a full-screen program must size itself
         * to - not the framebuffer console's. */
        struct tsh_term *term = term_current();
        if (term != NULL) {
            cols = (int)term->cols;
            rows = (int)term->rows;
        } else {
            fb_get_grid(&cols, &rows);
        }
        uint16_t ws[4] = { (uint16_t)rows, (uint16_t)cols, 0, 0 };
        memcpy(arg, ws, sizeof(ws));
        return 0;
    }
    case TIOCGPGRP:
        if (arg == NULL) {
            return -EINVAL;
        }
        *(int *)arg = (int)kbd_get_fg_pgid();
        return 0;
    case TIOCSPGRP:
        if (arg == NULL) {
            return -EINVAL;
        }
        kbd_set_fg_pgid(*(int *)arg);
        return 0;
    default:
        return -ENOTTY;
    }
}

/* ---- /dev/hda .. /dev/hdd (ATA disks) ----
 *
 * A disk is a byte stream here, not a sector stream: read() and
 * write() take any offset and any length, and the partial sectors at
 * either end are handled with a read-modify-write through one bounce
 * buffer. That is what lets an installer use plain open/lseek/write
 * instead of an ioctl language of its own.
 *
 * `priv` is the disk's index in the driver's slot table.
 */

static uint8_t g_disk_bounce[ATA_SECTOR_SIZE];

static long disk_io(void *priv, void *buf, size_t count, size_t pos,
                    bool write) {
    int index = (int)(long)priv;
    const struct ata_disk *d = ata_disk(index);
    if (d == NULL || !d->present || d->atapi) {
        return -ENODEV;
    }

    uint64_t bytes = (uint64_t)d->sectors * ATA_SECTOR_SIZE;
    if (pos >= bytes) {
        return 0; /* end of the disk */
    }
    if (count > bytes - pos) {
        count = (size_t)(bytes - pos);
    }

    uint8_t *p = (uint8_t *)buf;
    size_t done = 0;
    while (done < count) {
        uint32_t lba = (uint32_t)((pos + done) / ATA_SECTOR_SIZE);
        size_t off = (size_t)((pos + done) % ATA_SECTOR_SIZE);
        size_t room = ATA_SECTOR_SIZE - off;
        size_t n = count - done < room ? count - done : room;

        if (off == 0 && n == ATA_SECTOR_SIZE) {
            /* A whole sector, and more of them if the caller asked
             * for a run: this is the path the installer takes. */
            uint32_t whole = (uint32_t)((count - done) / ATA_SECTOR_SIZE);
            int rc = write ? ata_write(index, lba, whole, p + done)
                           : ata_read(index, lba, whole, p + done);
            if (rc != 0) {
                return done > 0 ? (long)done : rc;
            }
            done += (size_t)whole * ATA_SECTOR_SIZE;
            continue;
        }

        int rc = ata_read(index, lba, 1, g_disk_bounce);
        if (rc != 0) {
            return done > 0 ? (long)done : rc;
        }
        if (write) {
            memcpy(g_disk_bounce + off, p + done, n);
            rc = ata_write(index, lba, 1, g_disk_bounce);
            if (rc != 0) {
                return done > 0 ? (long)done : rc;
            }
        } else {
            memcpy(p + done, g_disk_bounce + off, n);
        }
        done += n;
    }
    return (long)done;
}

static long disk_read(void *priv, void *buf, size_t count, size_t pos) {
    return disk_io(priv, buf, count, pos, false);
}

static long disk_write(void *priv, const void *buf, size_t count, size_t pos) {
    return disk_io(priv, (void *)buf, count, pos, true);
}

/* ---- /dev/kernel and /dev/rootfs ----
 *
 * The two images the bootloader loaded, readable as files. They are
 * how the installer copies the RUNNING system onto a disk: no build
 * step has to keep a second copy of the kernel around, and what gets
 * installed is by definition what is running.
 */

static long image_read(void *priv, void *buf, size_t count, size_t pos) {
    const struct limine_file *f = (const struct limine_file *)priv;
    if (f == NULL || f->address == NULL) {
        return -ENODEV;
    }
    if (pos >= f->size) {
        return 0;
    }
    if (count > f->size - pos) {
        count = (size_t)(f->size - pos);
    }
    memcpy(buf, (const uint8_t *)f->address + pos, count);
    return (long)count;
}

/* ---- /dev/kbd0 ---- */

static long kbd_read(void *priv, void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)pos;
    if (count == 0) {
        return 0;
    }
    for (;;) {
        struct kbd_event ev = kbd_get_event();
        if (ev.type == KBD_EVENT_SPECIAL) {
            continue;
        }
        if (ev.type != KBD_EVENT_CHAR) {
            continue;
        }
        if (ev.c == 0x1B) {
            return 0;
        }
        *(char *)buf = ev.c;
        return 1;
    }
}

/* /dev/kbd0 is not gated by console ownership (kbd_read consumes
 * events directly), so a buffered keypress is enough. */
static short kbd_poll_dev(void *priv) {
    (void)priv;
    return kbd_has_char() ? POLLIN : 0;
}

/* ---- /dev/serial0 ---- */

static long serial_write_dev(void *priv, const void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)pos;
    const char *p = (const char *)buf;
    for (size_t i = 0; i < count; i++) {
        serial_putchar(p[i]);
    }
    return (long)count;
}

/* ---- /dev/null ---- */

static long null_write(void *priv, const void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)buf;
    (void)pos;
    return (long)count; /* swallow everything */
}

static long null_read(void *priv, void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)buf;
    (void)count;
    (void)pos;
    return 0; /* EOF */
}

/* ---- /dev/zero ---- */

static long zero_read(void *priv, void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)pos;
    memset(buf, 0, count);
    return (long)count;
}

/* ---- /dev/random and /dev/urandom ----
 *
 * Both are the same generator. TUS makes no distinction because there
 * is nothing to distinguish: the pool is a CSPRNG that is seeded once
 * at boot and never runs "low" on entropy, so a blocking /dev/random
 * would block forever for no benefit. */
static long random_read_dev(void *priv, void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)pos;
    random_bytes(buf, count);
    return (long)count;
}

/* Writing to /dev/urandom stirs the pool, as it does on Linux. */
static long random_write_dev(void *priv, const void *buf, size_t count,
                             size_t pos) {
    (void)priv;
    (void)pos;
    random_add_entropy(buf, count);
    return (long)count;
}

/* Re-take the framebuffer snapshot. Called after a runtime mode
 * change: /dev/fb0's size, pitch and address all move, and a program
 * still writing against the old ones writes off the end of the new
 * mode's memory. */
void devices_refresh_fb(void) {
    uint32_t w = 0, h = 0, bpp = 0;
    uint64_t pitch = 0;
    void *address = NULL;
    fb_get_info(&w, &h, &bpp, &pitch, &address);
    g_fb_info.width = w;
    g_fb_info.height = h;
    g_fb_info.bpp = bpp;
    g_fb_info.pitch = pitch;
    g_fb_info.address = (uint64_t)(uintptr_t)address;

    /* vfs_lseek() only allows seeking a device whose node carries a
     * size (the same rule /dev/hda uses) - without this, a program
     * that wants to write one scanline at a time (rather than the
     * whole buffer in one write(), like fb_fill() does internally)
     * gets -ESPIPE from every lseek() and silently keeps writing from
     * wherever its fd position already was. The node does not exist
     * yet on the very first call (devices_init() takes it from
     * g_fb_info right after creating it); every call after a mode
     * change finds it and keeps it in sync. */
    struct vfs_node *fb_node = vfs_lookup("/dev/fb0");
    if (fb_node != NULL) {
        fb_node->size = (size_t)pitch * h;
    }
}

void devices_init(void) {
    /* The trailing NULL poll op means "always ready" (see file_ops);
     * only the keyboard-backed devices can make a reader block. */
    static const struct file_ops fb_ops = { fb_read, fb_write, fb_ioctl, NULL };
    static const struct file_ops tty_ops = { tty_read, tty_write, tty_ioctl,
                                             tty_poll };
    static const struct file_ops kbd_ops = { kbd_read, NULL, NULL,
                                             kbd_poll_dev };
    static const struct file_ops serial_ops = { NULL, serial_write_dev, NULL,
                                                NULL };
    static const struct file_ops null_ops = { null_read, null_write, NULL,
                                              NULL };
    static const struct file_ops zero_ops = { zero_read, null_write, NULL,
                                              NULL };
    static const struct file_ops random_ops = { random_read_dev,
                                                random_write_dev, NULL, NULL };

    /* Snapshot the framebuffer description for /dev/fb0. */
    devices_refresh_fb();

    struct vfs_node *fb_node = vfs_create_device("/dev/fb0", &fb_ops, &g_fb_info);
    if (fb_node != NULL) {
        fb_node->size = (size_t)g_fb_info.pitch * g_fb_info.height;
    }
    vfs_create_device("/dev/tty0", &tty_ops, NULL);
    vfs_create_device("/dev/kbd0", &kbd_ops, NULL);
    vfs_create_device("/dev/serial0", &serial_ops, NULL);
    vfs_create_device("/dev/null", &null_ops, NULL);
    vfs_create_device("/dev/zero", &zero_ops, NULL);
    vfs_create_device("/dev/random", &random_ops, NULL);
    vfs_create_device("/dev/urandom", &random_ops, NULL);
    pty_init();

    /* One node per ATA disk the driver found, named after its slot
     * (hda .. hdd). The node's size is the disk's, so `ls -l /dev`
     * shows how big it is and lseek(SEEK_END) lands at the end. */
    static const struct file_ops disk_ops = { disk_read, disk_write, NULL,
                                              NULL };
    for (int i = 0; i < ATA_MAX_DISKS; i++) {
        const struct ata_disk *d = ata_disk(i);
        if (d == NULL || !d->present || d->atapi) {
            continue;
        }
        char path[32];
        strcpy(path, "/dev/");
        strncpy(path + 5, d->name, sizeof(path) - 6);
        path[sizeof(path) - 1] = '\0';
        struct vfs_node *node = vfs_create_device(path, &disk_ops,
                                                  (void *)(long)i);
        if (node != NULL) {
            node->size = (size_t)d->sectors * ATA_SECTOR_SIZE;
        }
    }

    /* The images the bootloader loaded, readable as files: this is
     * where the installer gets the system it installs. */
    static const struct file_ops image_ops = { image_read, NULL, NULL, NULL };
    if (g_bootinfo.kernel_file != NULL) {
        struct vfs_node *node = vfs_create_device("/dev/kernel", &image_ops,
                                                  g_bootinfo.kernel_file);
        if (node != NULL) {
            node->size = (size_t)g_bootinfo.kernel_file->size;
        }
    }
    if (g_bootinfo.rootfs_module != NULL) {
        struct vfs_node *node = vfs_create_device("/dev/rootfs", &image_ops,
                                                  g_bootinfo.rootfs_module);
        if (node != NULL) {
            node->size = (size_t)g_bootinfo.rootfs_module->size;
        }
    }
}
