/*
 * whoami.c - print the current user name (TUS port). Real /bin
 * binary version of tsh's cmd_whoami (kernel/shell/cmd_fs.c): looks
 * up getuid()'s numeric id against /etc/passwd, same as login/passwd
 * already do for their own purposes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Same small linear lookup as cmd_whoami/cmd_id use - not worth a
 * general passwd-parsing library for one field. */
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
    char name[64];
    if (passwd_name_for_uid(uid, name, sizeof(name)) == 0) {
        printf("%s\n", name);
    } else {
        printf("%u\n", uid);
    }
    return 0;
}
