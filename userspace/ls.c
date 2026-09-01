/*
 * ls.c - list a directory (TUS port of the classic UNIX ls)
 *
 * A real /bin/ls: everything tsh's own `cmd_ls` built-in
 * (kernel/shell/cmd_fs.c) does, reached through the syscall ABI
 * instead of kernel functions directly, so ksh (or any other ring-3
 * shell) can use it via a plain PATH lookup + execve like on any real
 * Unix. Mirrors cmd_ls's flags and output format exactly: -l and -a,
 * the same "-rwxrwxrwx" mode string, the same "root root SIZE name"
 * column layout for -l.
 *
 * TUS's readdir has its own ABI (kernel/vfs/vfs.h): one entry per call
 * into a fixed-size structure. musl has no libc wrapper for it (same
 * situation as userspace/hxfiles.c), so this makes the raw int $0x80
 * call itself.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SYS_READDIR 11

#define VFS_DIR    1
#define VFS_FILE   2
#define VFS_DEVICE 3
#define VFS_SOCKET 4
#define VFS_NAME_MAX 64

struct vfs_dirent {
    char name[VFS_NAME_MAX];
    unsigned type;
    unsigned size;
    unsigned mode;
};

static long tus_syscall3(long n, long a1, long a2, long a3) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "a"(n)
                     : "memory", "cc");
    return ret;
}

static void mode_string(unsigned mode, unsigned type, char out[11]) {
    out[0] = (type == VFS_DIR)    ? 'd'
           : (type == VFS_DEVICE) ? 'c'
           : (type == VFS_SOCKET) ? 's'
                                  : '-';
    static const char rwx[10] = "rwxrwxrwx";
    for (int i = 0; i < 9; i++) {
        out[1 + i] = (mode & (0400u >> i)) ? rwx[i] : '-';
    }
    if (mode & 04000) {
        out[3] = (out[3] == 'x') ? 's' : 'S';
    }
    if (mode & 02000) {
        out[6] = (out[6] == 'x') ? 's' : 'S';
    }
    if (mode & 01000) {
        out[9] = (out[9] == 'x') ? 't' : 'T';
    }
    out[10] = '\0';
}

struct ls_entry {
    char name[VFS_NAME_MAX];
    unsigned type;
    unsigned size;
    unsigned mode;
};

static int list_one(const char *target, int long_fmt, int all,
                     int print_header) {
    int fd = open(target, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "ls: %s: %s\n", target, strerror(errno));
        return 1;
    }

    if (print_header) {
        printf("%s:\n", target);
    }

    struct ls_entry ents[128];
    int count = 0;
    struct vfs_dirent ent;
    long n;
    int rc = 0;
    while ((n = tus_syscall3(SYS_READDIR, fd, (long)&ent, sizeof(ent))) > 0 &&
           count < 128) {
        if (!all && ent.name[0] == '.') {
            continue;
        }
        strncpy(ents[count].name, ent.name, VFS_NAME_MAX - 1);
        ents[count].name[VFS_NAME_MAX - 1] = '\0';
        ents[count].type = ent.type;
        ents[count].size = ent.size;
        ents[count].mode = ent.mode;
        count++;
    }
    if (n < 0) {
        fprintf(stderr, "ls: %s: error %ld\n", target, -n);
        rc = 1;
    }
    close(fd);

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(ents[j].name, ents[i].name) < 0) {
                struct ls_entry tmp = ents[i];
                ents[i] = ents[j];
                ents[j] = tmp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        if (long_fmt) {
            char m[11];
            mode_string(ents[i].mode, ents[i].type, m);
            const char *kind = (ents[i].type == VFS_DIR)    ? "/"
                             : (ents[i].type == VFS_SOCKET) ? "=" : "";
            printf("%s root root %8u %s%s\n", m, ents[i].size,
                   ents[i].name, kind);
        } else {
            printf("%s\n", ents[i].name);
        }
    }
    if (print_header) {
        printf("\n");
    }
    return rc;
}

int main(int argc, char **argv) {
    int long_fmt = 0, all = 0;
    const char *targets[64];
    int ntargets = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            for (const char *p = argv[i] + 1; *p != '\0'; p++) {
                if (*p == 'l') {
                    long_fmt = 1;
                } else if (*p == 'a') {
                    all = 1;
                } else if (*p == '1') {
                    /* one entry per line: already the default format */
                } else {
                    fprintf(stderr, "ls: invalid option -- '%c'\n", *p);
                    return 1;
                }
            }
        } else if (ntargets < 64) {
            targets[ntargets++] = argv[i];
        }
    }
    if (ntargets == 0) {
        targets[ntargets++] = ".";
    }

    int rc = 0;
    for (int i = 0; i < ntargets; i++) {
        if (list_one(targets[i], long_fmt, all, ntargets > 1) != 0) {
            rc = 1;
        }
    }
    return rc;
}
