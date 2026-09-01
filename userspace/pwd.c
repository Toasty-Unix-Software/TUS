/*
 * pwd.c - print the working directory (TUS port).
 *
 * ksh (like bash) already has a builtin `pwd`, but real Unix ships
 * /bin/pwd too - for scripts, other shells, or `command pwd`. Just
 * getcwd(2) (musl wraps SYS_GETCWD already).
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char buf[512];
    if (getcwd(buf, sizeof(buf)) == NULL) {
        fprintf(stderr, "pwd: %s\n", strerror(errno));
        return 1;
    }
    printf("%s\n", buf);
    return 0;
}
