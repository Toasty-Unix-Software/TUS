/*
 * touch.c - create an empty file if it does not exist (TUS port).
 * Real /bin binary version of tsh's cmd_touch (kernel/shell/cmd_fs.c).
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: touch <path>\n");
        return 1;
    }
    int rc = 0;
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_CREAT | O_RDWR, 0644);
        if (fd < 0) {
            fprintf(stderr, "touch: %s: %s\n", argv[i], strerror(errno));
            rc = 1;
            continue;
        }
        close(fd);
    }
    return rc;
}
