/*
 * mail - read the system mailbox
 *
 * The smallest thing that can honestly be called `mail`: it prints
 * the mailbox of the user who asked (/var/mail/<user>, or the file
 * named on the command line). TUS has no mail transport, no delivery
 * agent and nothing to send with - what it has is the message the
 * installer tells you to read, and this is what reads it.
 */

#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char path[256];

    if (argc > 1) {
        snprintf(path, sizeof(path), "%s", argv[1]);
    } else {
        struct passwd *pw = getpwuid(getuid());
        const char *user = (pw != NULL && pw->pw_name != NULL) ? pw->pw_name
                                                               : "root";
        snprintf(path, sizeof(path), "/var/mail/%s", user);
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("No mail for %s.\n", argc > 1 ? argv[1] : "you");
        return 1;
    }

    char buf[1024];
    long n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, (size_t)n);
    }
    close(fd);
    return 0;
}
