/*
 * tail.c - print the last lines of a file (TUS port). Real /bin
 * binary version of tsh's cmd_tail (kernel/shell/cmd_fs.c): reads the
 * whole file (capped at 64 KiB) rather than seeking blind, since the
 * last N lines are only knowable once every '\n' has been counted.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: tail <path> [lines]\n");
        return 1;
    }
    long want = argc >= 3 ? strtol(argv[2], NULL, 10) : 10;

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "tail: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    static char buf[65536];
    long total = 0;
    ssize_t n;
    while (total < (long)sizeof(buf) &&
          (n = read(fd, buf + total, sizeof(buf) - (size_t)total)) > 0) {
        total += n;
    }
    close(fd);
    if (total == 0) {
        return 0;
    }

    long total_lines = 0;
    for (long k = 0; k < total; k++) {
        if (buf[k] == '\n') {
            total_lines++;
        }
    }
    if (buf[total - 1] != '\n') {
        total_lines++;
    }
    long skip = total_lines - want;
    if (skip < 0) {
        skip = 0;
    }

    long start = 0, seen = 0;
    for (long k = 0; k < total && seen < skip; k++) {
        if (buf[k] == '\n') {
            seen++;
            start = k + 1;
        }
    }
    write(1, buf + start, (size_t)(total - start));
    return 0;
}
