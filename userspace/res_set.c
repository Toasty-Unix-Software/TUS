/*
 * res_set - show, list and change the display resolution
 *
 *     res_set                 the mode on screen now
 *     res_set -l              the modes this machine will accept
 *     doas res_set 1920x1080  change to that mode
 *
 * The program is deliberately NOT setuid. Everything it does goes
 * through SYS_VIDEO (include/tusvideo.h), and the kernel refuses
 * TUS_VIDEO_SET_MODE to a caller whose effective uid is not 0. A user
 * who may change the screen is a user `doas` already says may - the
 * privilege lives in /etc/doas.conf, in one place, where it can be
 * read and audited, instead of in a permission bit on a binary.
 *
 * Run without root, it says so and prints the doas line to use rather
 * than failing with a bare number.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <tusvideo.h>

/* musl does not wrap SYS_VIDEO - it is a TUS call, not a Linux one -
 * so make it directly. Only RAX survives the kernel's stub, which is
 * why every argument register is declared read-write (see the note in
 * kernel/syscall/syscall.h). */
#define SYS_VIDEO 54

static long video_call(long op, void *arg, unsigned long len) {
    long ret;
    register long rdi __asm__("rdi") = op;
    register long rsi __asm__("rsi") = (long)arg;
    register long rdx __asm__("rdx") = (long)len;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"((long)SYS_VIDEO)
                     : "rcx", "r11", "memory");
    return ret;
}

static void usage(void) {
    fputs("usage: res_set [-l] [WIDTHxHEIGHT]\n"
          "  res_set                 show the current display mode\n"
          "  res_set -l              list the modes this machine accepts\n"
          "  doas res_set 1280x800   change the mode (needs root)\n",
          stderr);
}

/* Parse "1920x1080". Accepts 'X' as well as 'x', rejects anything
 * with trailing rubbish - "1920x1080foo" is a typo, not a mode. */
static int parse_mode(const char *s, unsigned *w, unsigned *h) {
    char *end = NULL;
    unsigned long a = strtoul(s, &end, 10);
    if (end == s || (*end != 'x' && *end != 'X')) {
        return -1;
    }
    const char *rest = end + 1;
    unsigned long b = strtoul(rest, &end, 10);
    if (end == rest || *end != '\0') {
        return -1;
    }
    if (a == 0 || b == 0 || a > 0xFFFF || b > 0xFFFF) {
        return -1;
    }
    *w = (unsigned)a;
    *h = (unsigned)b;
    return 0;
}

static void print_mode(const struct tus_video_mode *m) {
    printf("%ux%u at %u bpp (%u bytes per scanline)\n",
           m->width, m->height, m->bpp, m->pitch);
}

static int show_current(void) {
    struct tus_video_mode m;
    memset(&m, 0, sizeof(m));
    long rc = video_call(TUS_VIDEO_GET_MODE, &m, sizeof(m));
    if (rc < 0) {
        fprintf(stderr, "res_set: cannot read the display mode: %s\n",
                strerror((int)-rc));
        return 1;
    }
    print_mode(&m);
    if ((m.flags & TUS_VIDEO_F_MODESET) != 0) {
        printf("mode setting: available, up to %ux%u\n",
               m.max_width, m.max_height);
    } else {
        printf("mode setting: not available on this display adapter\n");
    }
    if ((m.flags & TUS_VIDEO_F_HIGHX) != 0) {
        printf("highX        : a session owns the screen\n");
    }
    return 0;
}

static int list_modes(void) {
    struct tus_video_mode cur;
    memset(&cur, 0, sizeof(cur));
    (void)video_call(TUS_VIDEO_GET_MODE, &cur, sizeof(cur));

    if ((cur.flags & TUS_VIDEO_F_MODESET) == 0) {
        fputs("res_set: this display adapter has no runtime mode setting;\n"
              "         the machine keeps the mode the bootloader chose.\n",
              stderr);
        return 1;
    }

    for (unsigned i = 0;; i++) {
        struct tus_video_mode m;
        memset(&m, 0, sizeof(m));
        m.index = i;
        long rc = video_call(TUS_VIDEO_LIST_MODE, &m, sizeof(m));
        if (rc < 0) {
            break;
        }
        if (m.width > cur.max_width || m.height > cur.max_height) {
            continue;
        }
        printf("  %4ux%-4u%s\n", m.width, m.height,
               (m.width == cur.width && m.height == cur.height) ?
                   "  (current)" : "");
    }
    return 0;
}

static int set_mode(unsigned w, unsigned h) {
    struct tus_video_mode m;
    memset(&m, 0, sizeof(m));
    m.width = w;
    m.height = h;

    long rc = video_call(TUS_VIDEO_SET_MODE, &m, sizeof(m));
    if (rc == 0) {
        printf("res_set: now ");
        print_mode(&m);
        return 0;
    }

    switch ((int)-rc) {
    case EPERM:
        fprintf(stderr,
                "res_set: only root may change the display mode.\n"
                "         try: doas res_set %ux%u\n", w, h);
        break;
    case ENODEV:
        fputs("res_set: this display adapter has no runtime mode setting.\n"
              "         The Bochs VBE registers are what TUS programs, and\n"
              "         only Bochs, QEMU (-vga std) and VirtualBox have them.\n",
              stderr);
        break;
    case EINVAL:
        fprintf(stderr, "res_set: %ux%u is not a mode this machine accepts "
                        "(try res_set -l)\n", w, h);
        break;
    case ENOSPC:
        fprintf(stderr, "res_set: %ux%u needs more video memory than the "
                        "kernel maps for the framebuffer\n", w, h);
        break;
    case EIO:
        fprintf(stderr, "res_set: the adapter refused %ux%u - not enough "
                        "video memory\n", w, h);
        break;
    default:
        fprintf(stderr, "res_set: %s\n", strerror((int)-rc));
        break;
    }
    return 1;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        return show_current();
    }
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 ||
                      strcmp(argv[1], "--help") == 0)) {
        usage();
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "-l") == 0) {
        return list_modes();
    }
    if (argc != 2) {
        usage();
        return 2;
    }

    unsigned w = 0, h = 0;
    if (parse_mode(argv[1], &w, &h) != 0) {
        fprintf(stderr, "res_set: '%s' is not a WIDTHxHEIGHT mode\n", argv[1]);
        usage();
        return 2;
    }
    return set_mode(w, h);
}
