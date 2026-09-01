/*
 * cp.c - copy a file (TUS port). Real /bin binary version of tsh's
 * cmd_cp (kernel/shell/cmd_fs.c), via plain musl libc file I/O.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: cp <src> <dst>\n");
        return 1;
    }

    int sfd = open(argv[1], O_RDONLY);
    if (sfd < 0) {
        fprintf(stderr, "cp: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    int dfd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (dfd < 0) {
        fprintf(stderr, "cp: %s: %s\n", argv[2], strerror(errno));
        close(sfd);
        return 1;
    }

    char buf[4096];
    ssize_t n;
    int rc = 0;
    while ((n = read(sfd, buf, sizeof(buf))) > 0) {
        ssize_t off = 0;
        while (off < n) {
            ssize_t w = write(dfd, buf + off, (size_t)(n - off));
            if (w <= 0) {
                rc = 1;
                break;
            }
            off += w;
        }
    }
    if (n < 0) {
        fprintf(stderr, "cp: %s: %s\n", argv[1], strerror(errno));
        rc = 1;
    }
    close(sfd);
    close(dfd);
    return rc;
}
