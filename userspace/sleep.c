/*
 * sleep.c - pause for N seconds (TUS port of the classic UNIX sleep).
 *
 * tsh's own `sleep` built-in (kernel/shell/cmd_fs.c) takes
 * MILLISECONDS directly via SYS_SLEEP. Real Unix `sleep(1)` takes
 * SECONDS - this is the real-Unix-compatible version, using musl's
 * standard sleep() (which goes through nanosleep() -> SYS_SLEEP
 * internally, see sources/musl-1.2.6/src/internal/tus_syscall.c).
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: sleep <seconds>\n");
        return 1;
    }
    sleep((unsigned)strtoul(argv[1], NULL, 10));
    return 0;
}
