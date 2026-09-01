/*
 * pkill.c - terminate tasks by name (TUS port of the classic UNIX
 * pkill). Built entirely in userspace on top of two other syscalls:
 * SYS_GETPROCS (see userspace/ps.c and kernel/syscall/syscall.h) to
 * list tasks, and the real kill(2) to signal each match - exactly how
 * real Unix pkill works against /proc, just against this snapshot
 * syscall instead.
 */

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SYS_GETPROCS 83
#define TASK_NAME_MAX 32
#define PROCS_MAX 64

struct tus_procinfo {
    uint32_t pid;
    uint32_t ppid;
    uint32_t pgid;
    uint32_t uid;
    uint32_t state;
    char name[TASK_NAME_MAX];
};

static long tus_syscall2(long n, long a1, long a2) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = 0;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "a"(n)
                     : "memory", "cc");
    return ret;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: pkill <name-substring>\n");
        return 1;
    }
    pid_t self = getpid();

    static struct tus_procinfo procs[PROCS_MAX];
    long n = tus_syscall2(SYS_GETPROCS, (long)procs, sizeof(procs));
    if (n < 0) {
        fprintf(stderr, "pkill: error %ld\n", -n);
        return 1;
    }

    int killed = 0;
    for (long i = 0; i < n; i++) {
        if ((pid_t)procs[i].pid == self) {
            continue;
        }
        if (strstr(procs[i].name, argv[1]) != NULL) {
            if (kill((pid_t)procs[i].pid, SIGTERM) == 0) {
                killed++;
            }
        }
    }
    printf("pkill: %d process%s killed\n", killed, killed == 1 ? "" : "es");
    return killed > 0 ? 0 : 1;
}
