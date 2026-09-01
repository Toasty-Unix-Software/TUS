/*
 * id.c - print uid/gid/euid (TUS port). Real /bin binary version of
 * tsh's cmd_id (kernel/shell/cmd_fs.c).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int passwd_name_for_uid(unsigned uid, char *out, size_t outsz) {
    FILE *f = fopen("/etc/passwd", "r");
    if (f == NULL) {
        return -1;
    }
    char line[256];
    int found = -1;
    while (fgets(line, sizeof(line), f) != NULL) {
        char *name_end = strchr(line, ':');
        if (name_end == NULL) {
            continue;
        }
        char *pass_end = strchr(name_end + 1, ':');
        if (pass_end == NULL) {
            continue;
        }
        unsigned long line_uid = strtoul(pass_end + 1, NULL, 10);
        if (line_uid == uid) {
            size_t namelen = (size_t)(name_end - line);
            if (namelen >= outsz) {
                namelen = outsz - 1;
            }
            memcpy(out, line, namelen);
            out[namelen] = '\0';
            found = 0;
            break;
        }
    }
    fclose(f);
    return found;
}

int main(void) {
    unsigned uid = (unsigned)getuid();
    unsigned gid = (unsigned)getgid();
    unsigned euid = (unsigned)geteuid();
    char name[64];
    if (passwd_name_for_uid(uid, name, sizeof(name)) == 0) {
        printf("uid=%u(%s) gid=%u euid=%u\n", uid, name, gid, euid);
    } else {
        printf("uid=%u gid=%u euid=%u\n", uid, gid, euid);
    }
    return 0;
}
