/*
 * tree.c - list a directory as a tree (TUS port of the classic Unix
 * tree command)
 *
 * TUS's readdir has its own ABI (kernel/vfs/vfs.h): one entry per call
 * into a fixed-size structure. musl has no libc wrapper for it (same
 * situation as userspace/ls.c), so this makes the raw int $0x80 call
 * itself.
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

static int show_all = 0;
static int max_depth = -1; /* -1 = unlimited */
static long n_dirs = 0, n_files = 0;

struct tree_entry {
    char name[VFS_NAME_MAX];
    unsigned type;
};

static int entry_cmp(const void *a, const void *b) {
    return strcmp(((const struct tree_entry *)a)->name,
                  ((const struct tree_entry *)b)->name);
}

#define TREE_MAX_ENTRIES 128

/* Reads up to TREE_MAX_ENTRIES entries of the directory at `path`
 * into `ents`, sorted. Returns the count, or -1 if `path` could not
 * be opened. A fixed-size array (same approach as ls.c) rather than
 * malloc/realloc: tree recurses, so every directory level gets its
 * own array on that call's stack frame. */
static int read_dir(const char *path, struct tree_entry *ents) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    int count = 0;
    struct vfs_dirent ent;
    long n;
    while ((n = tus_syscall3(SYS_READDIR, fd, (long)&ent, sizeof(ent))) > 0 &&
           count < TREE_MAX_ENTRIES) {
        if (!show_all && ent.name[0] == '.') {
            continue;
        }
        strncpy(ents[count].name, ent.name, VFS_NAME_MAX - 1);
        ents[count].name[VFS_NAME_MAX - 1] = '\0';
        ents[count].type = ent.type;
        count++;
    }
    close(fd);

    qsort(ents, (size_t)count, sizeof(*ents), entry_cmp);
    return count;
}

static void walk(const char *path, char *prefix, int depth) {
    if (max_depth >= 0 && depth >= max_depth) {
        return;
    }

    struct tree_entry ents[TREE_MAX_ENTRIES];
    int count = read_dir(path, ents);
    if (count < 0) {
        return;
    }

    size_t prefix_len = strlen(prefix);
    for (int i = 0; i < count; i++) {
        int last = (i == count - 1);
        printf("%s%s%s\n", prefix, last ? "\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 "
                                          : "\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 ",
               ents[i].name);

        if (ents[i].type == VFS_DIR) {
            n_dirs++;

            char child[600];
            snprintf(child, sizeof(child), "%s%s%s", path,
                     (path[strlen(path) - 1] == '/') ? "" : "/", ents[i].name);

            snprintf(prefix + prefix_len, 256 - prefix_len, "%s",
                      last ? "    " : "\xe2\x94\x82   ");
            walk(child, prefix, depth + 1);
            prefix[prefix_len] = '\0';
        } else {
            n_files++;
        }
    }
}

int main(int argc, char **argv) {
    const char *root = ".";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            show_all = 1;
        } else if (strcmp(argv[i], "-L") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "tree: option requires an argument -- 'L'\n");
                return 1;
            }
            max_depth = atoi(argv[++i]);
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "tree: invalid option -- '%s'\n", argv[i]);
            return 1;
        } else {
            root = argv[i];
        }
    }

    char prefix[256];
    prefix[0] = '\0';

    printf("%s\n", root);
    walk(root, prefix, 0);
    printf("\n%ld director%s, %ld file%s\n", n_dirs, n_dirs == 1 ? "y" : "ies",
           n_files, n_files == 1 ? "" : "s");
    return 0;
}
