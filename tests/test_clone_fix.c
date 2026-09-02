/* test_clone_fix.c - exercises musl's posix_spawn() -> __clone() path
 * directly, the same path that crashed ksh's `clear` with Invalid
 * Opcode (raw `syscall` instruction, unimplemented on TUS). Prints a
 * marker before and after so a boot test can confirm the child
 * actually ran and the parent got its exit status back, with no
 * fault in between. */
#include <spawn.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

int main(void)
{
    printf("CLONE_TEST: before spawn\n");
    fflush(stdout);

    char *argv[] = {"/bin/echo", "CLONE_TEST: child ran via posix_spawn", NULL};
    pid_t pid;
    int rc = posix_spawn(&pid, "/bin/echo", NULL, NULL, argv, environ);
    if (rc != 0) {
        printf("CLONE_TEST: posix_spawn failed rc=%d\n", rc);
        return 1;
    }

    int status;
    waitpid(pid, &status, 0);
    printf("CLONE_TEST: after spawn, child pid=%d status=%d\n", pid, status);
    return 0;
}
