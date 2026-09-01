/*
 * mkdir.c - create a directory (TUS port of the classic UNIX mkdir)
 *
 * Same flags as tsh's cmd_mkdir (kernel/shell/cmd_fs.c): -p, -v,
 * -m mode. Real /bin binary via mkdir(2) (musl wraps this already).
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    int parents = 0, verbose = 0;
    unsigned mode = 0777;
    const char *target = NULL;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0' && target == NULL) {
            for (const char *p = argv[i] + 1; *p != '\0'; p++) {
                if (*p == 'p') {
                    parents = 1;
                } else if (*p == 'v') {
                    verbose = 1;
                } else if (*p == 'm') {
                    if (i + 1 < argc) {
                        mode = (unsigned)strtoul(argv[++i], NULL, 8);
                    } else {
                        fprintf(stderr,
                                "mkdir: option requires an argument -- 'm'\n");
                        return 1;
                    }
                } else {
                    fprintf(stderr, "mkdir: invalid option -- '%c'\n", *p);
                    return 1;
                }
            }
        } else if (target == NULL) {
            target = argv[i];
        }
    }
    if (target == NULL) {
        fprintf(stderr, "usage: mkdir [-p] [-v] [-m mode] <directory>\n");
        return 1;
    }

    int r;
    if (parents) {
        char comp[512];
        size_t len = strlen(target);
        if (len >= sizeof(comp)) {
            fprintf(stderr, "mkdir: path too long\n");
            return 1;
        }
        r = 0;
        for (size_t i = 1; i <= len; i++) {
            if (i == len || target[i] == '/') {
                size_t n = i;
                if (i == len && n > 1 && target[n - 1] == '/') {
                    n--;
                }
                memcpy(comp, target, n);
                comp[n] = '\0';
                if (n > 0 && strcmp(comp, "/") != 0) {
                    int rr = mkdir(comp, mode);
                    if (rr < 0 && errno != EEXIST) {
                        r = -1;
                        break;
                    } else if (verbose && rr == 0) {
                        printf("mkdir: created directory '%s'\n", comp);
                    }
                }
            }
        }
    } else {
        r = mkdir(target, mode);
        if (r == 0 && verbose) {
            printf("mkdir: created directory '%s'\n", target);
        }
    }
    if (r < 0) {
        fprintf(stderr, "mkdir: %s: %s\n", target, strerror(errno));
        return 1;
    }
    return 0;
}
