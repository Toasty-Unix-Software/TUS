/*
 * keymap - show, list and change the keyboard layout
 *
 *     keymap              the layout in use
 *     keymap -l           every layout TUS has
 *     doas keymap tr      load the Turkish Q layout
 *
 * Not setuid, for the same reason res_set is not: the kernel refuses
 * TUS_INPUT_SET_KEYMAP to a caller whose effective uid is not 0, so
 * the privilege lives in /etc/doas.conf where it can be read, rather
 * than in a permission bit on a binary.
 *
 * Making the change survive a reboot is a separate, deliberate step:
 * write the name into /etc/keymap, which the kernel reads at boot.
 * `keymap -s <name>` does both.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <tusinput.h>

#define SYS_INPUT 55

/* musl does not wrap SYS_INPUT - it is a TUS call. Only RAX survives
 * the kernel stub, so every argument register is read-write. */
static long input_call(long op, void *arg, unsigned long len) {
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
                     : "0"((long)SYS_INPUT)
                     : "rcx", "r11", "memory");
    return ret;
}

static void usage(void) {
    fputs("usage: keymap [-l] [-s] [LAYOUT]\n"
          "  keymap              show the layout in use\n"
          "  keymap -l           list the layouts TUS has\n"
          "  doas keymap tr      load a layout for this session\n"
          "  doas keymap -s tr   load it and make it the default\n"
          "                      (writes /etc/keymap, read at boot)\n",
          stderr);
}

static int show(void) {
    struct tus_input_keymap km;
    memset(&km, 0, sizeof(km));
    long rc = input_call(TUS_INPUT_GET_KEYMAP, &km, sizeof(km));
    if (rc < 0) {
        fprintf(stderr, "keymap: %s\n", strerror((int)-rc));
        return 1;
    }
    printf("%s - %s\n", km.name, km.description);
    return 0;
}

static int list(void) {
    struct tus_input_keymap cur;
    memset(&cur, 0, sizeof(cur));
    (void)input_call(TUS_INPUT_GET_KEYMAP, &cur, sizeof(cur));

    for (unsigned i = 0;; i++) {
        struct tus_input_keymap km;
        memset(&km, 0, sizeof(km));
        km.index = i;
        if (input_call(TUS_INPUT_LIST_KEYMAP, &km, sizeof(km)) < 0) {
            break;
        }
        printf("  %-6s %-28s%s\n", km.name, km.description,
               strcmp(km.name, cur.name) == 0 ? "  (current)" : "");
    }
    return 0;
}

/* Write the layout name into /etc/keymap so the next boot loads it.
 * A failure here is reported but does not undo the change that has
 * already taken effect - the layout IS loaded, it just will not
 * survive a reboot, and saying so is more useful than pretending the
 * whole thing failed. */
static int persist(const char *name) {
    int fd = open("/etc/keymap", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        fprintf(stderr, "keymap: cannot write /etc/keymap: %s\n"
                        "        the layout is loaded but will not "
                        "survive a reboot\n", strerror(errno));
        return 1;
    }
    char line[TUS_KEYMAP_NAME_MAX + 2];
    int n = snprintf(line, sizeof(line), "%s\n", name);
    if (write(fd, line, (size_t)n) != n) {
        fprintf(stderr, "keymap: short write to /etc/keymap\n");
        close(fd);
        return 1;
    }
    close(fd);
    printf("keymap: /etc/keymap updated; this is now the default\n");
    return 0;
}

static int set(const char *name, int save) {
    struct tus_input_keymap km;
    memset(&km, 0, sizeof(km));
    strncpy(km.name, name, TUS_KEYMAP_NAME_MAX - 1);

    long rc = input_call(TUS_INPUT_SET_KEYMAP, &km, sizeof(km));
    if (rc < 0) {
        switch ((int)-rc) {
        case EPERM:
            fprintf(stderr,
                    "keymap: only root may change the keyboard layout.\n"
                    "        try: doas keymap %s\n", name);
            break;
        case ENOENT:
            fprintf(stderr, "keymap: no layout called '%s' "
                            "(try keymap -l)\n", name);
            break;
        default:
            fprintf(stderr, "keymap: %s\n", strerror((int)-rc));
            break;
        }
        return 1;
    }

    printf("keymap: %s\n", name);
    if (save) {
        return persist(name);
    }
    return 0;
}

int main(int argc, char **argv) {
    int save = 0;
    int i = 1;

    if (argc > 1 && (strcmp(argv[1], "-h") == 0 ||
                     strcmp(argv[1], "--help") == 0)) {
        usage();
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "-l") == 0) {
        return list();
    }
    if (argc > 1 && strcmp(argv[1], "-s") == 0) {
        save = 1;
        i = 2;
    }

    if (i >= argc) {
        if (save) {
            usage();
            return 2;
        }
        return show();
    }
    if (i != argc - 1) {
        usage();
        return 2;
    }
    return set(argv[i], save);
}
