/*
 * head.c - print the first lines of a file (TUS port). Real /bin
 * binary version of tsh's cmd_head (kernel/shell/cmd_fs.c).
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: head <path> [lines]\n");
        return 1;
    }
    long want = argc >= 3 ? strtol(argv[2], NULL, 10) : 10;

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "head: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    char buf[512];
    ssize_t n;
    long lines = 0;
    while (lines < want && (n = read(fd, buf, sizeof(buf))) > 0) {
        ssize_t i;
        for (i = 0; i < n && lines < want; i++) {
            if (buf[i] == '\n') {
                lines++;
            }
        }
        write(1, buf, (size_t)i);
    }
    close(fd);
    return 0;
}
