/*
 * mv.c - rename/move a file (TUS port of the classic UNIX mv).
 *
 * Doesn't exist as a tsh built-in at all - added fresh, since a real
 * Unix always ships mv. Just rename(2) (musl wraps SYS_RENAME
 * already, see kernel/syscall/syscall.c's SYS_RENAME -> vfs_rename());
 * TUS's VFS has no separate filesystems to fall back to a copy+unlink
 * across, so a plain rename() is the whole implementation.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: mv <src> <dst>\n");
        return 1;
    }
    if (rename(argv[1], argv[2]) < 0) {
        fprintf(stderr, "mv: cannot move '%s' to '%s': %s\n",
                argv[1], argv[2], strerror(errno));
        return 1;
    }
    return 0;
}
