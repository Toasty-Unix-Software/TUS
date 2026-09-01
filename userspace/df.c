/*
 * df.c - show attached disk sizes (TUS port). Real /bin binary
 * version of tsh's cmd_df (kernel/shell/cmd_fs.c): a disk is a byte
 * stream (CLAUDE.md), so its size is just lseek(SEEK_END).
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    static const char *disks[] = {"/dev/hda", "/dev/hdb", "/dev/hdc",
                                  "/dev/hdd"};
    int found = 0;
    printf("DEVICE      SIZE\n");
    for (int i = 0; i < 4; i++) {
        int fd = open(disks[i], O_RDONLY);
        if (fd < 0) {
            continue;
        }
        off_t size = lseek(fd, 0, SEEK_END);
        close(fd);
        if (size > 0) {
            found = 1;
            printf("%-11s %ld MiB\n", disks[i], (long)(size / (1024 * 1024)));
        }
    }
    if (!found) {
        printf("no disks attached\n");
    }
    return 0;
}
