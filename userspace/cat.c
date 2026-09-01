/*
 * cat.c - print a file or device (TUS port of the classic UNIX cat)
 *
 * Real /bin/cat, same idea as cmd_cat in kernel/shell/cmd_fs.c but
 * through plain musl libc file I/O so any ring-3 shell (ksh) can use
 * it via PATH + execve.
 *
 * With no file operands, or when a operand is "-", reads stdin until
 * EOF - that is what makes `echo hi | cat` and a shell heredoc
 * (`cat <<EOF`, which is just the shell redirecting fd 0) work.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int g_number = 0;
static long g_line = 1;

static int cat_fd(int fd, const char *name) {
    char buf[4096];
    ssize_t n;
    int at_line_start = 1;
    int rc = 0;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        ssize_t start = 0;
        for (ssize_t i = 0; i < n; i++) {
            if (g_number && at_line_start) {
                char num[16];
                int len = snprintf(num, sizeof(num), "%6ld\t", g_line++);
                write(1, num, (size_t)len);
                at_line_start = 0;
            }
            if (buf[i] == '\n') {
                at_line_start = 1;
            }
        }
        (void)start;
        ssize_t off = 0;
        while (off < n) {
            ssize_t w = write(1, buf + off, (size_t)(n - off));
            if (w <= 0) {
                break;
            }
            off += w;
        }
    }
    if (n < 0) {
        fprintf(stderr, "cat: %s: %s\n", name, strerror(errno));
        rc = 1;
    }
    return rc;
}

int main(int argc, char **argv) {
    int rc = 0;
    int nfiles = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            g_number = 1;
            continue;
        }
        nfiles++;
    }

    if (nfiles == 0) {
        return cat_fd(0, "-");
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            continue;
        }
        if (strcmp(argv[i], "-") == 0) {
            if (cat_fd(0, "-") != 0) {
                rc = 1;
            }
            continue;
        }
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "cat: %s: %s\n", argv[i], strerror(errno));
            rc = 1;
            continue;
        }
        if (cat_fd(fd, argv[i]) != 0) {
            rc = 1;
        }
        close(fd);
    }
    return rc;
}
