/*
 * wc.c - count lines/words/bytes (TUS port). Real /bin binary version
 * of tsh's cmd_wc (kernel/shell/cmd_fs.c).
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: wc <path>\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "wc: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    char buf[4096];
    ssize_t n;
    long lines = 0, words = 0, bytes = 0;
    int in_word = 0;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        bytes += n;
        for (ssize_t i = 0; i < n; i++) {
            char c = buf[i];
            if (c == '\n') {
                lines++;
            }
            if (c == ' ' || c == '\t' || c == '\n') {
                in_word = 0;
            } else if (!in_word) {
                in_word = 1;
                words++;
            }
        }
    }
    close(fd);
    printf("%ld %ld %ld %s\n", lines, words, bytes, argv[1]);
    return 0;
}
