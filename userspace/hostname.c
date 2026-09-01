/*
 * hostname - show or set the system hostname
 * Usage: hostname [new_hostname]
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        char name[65];
        if (gethostname(name, sizeof(name)) != 0) {
            perror("hostname");
            return 1;
        }
        printf("%s\n", name);
        return 0;
    }

    if (argc == 2) {
        if (sethostname(argv[1], strlen(argv[1])) != 0) {
            perror("hostname");
            return 1;
        }
        return 0;
    }

    fprintf(stderr, "Usage: hostname [new_hostname]\n");
    return 1;
}
