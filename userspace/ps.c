/*
 * ps.c - list running tasks (TUS port of the classic UNIX ps).
 *
 * Unlike every other command in this file, there was previously no
 * way for a ring-3 program to see the task table at all - tsh's own
 * `cmd_ps` (kernel/shell/cmd_fs.c) calls task_list_all() directly,
 * which only works because tsh runs at ring 0. This uses a new
 * syscall, SYS_GETPROCS (kernel/syscall/syscall.h), added specifically
 * to make a real /bin/ps possible: it copies a struct tus_procinfo
 * per live task (kernel/sched/sched.h) into a user buffer.
 *
 * musl has no libc wrapper for a TUS-specific syscall like this (same
 * situation as SYS_READDIR - see userspace/hxfiles.c), so this makes
 * the raw int $0x80 call itself.
 */

#include <stdint.h>
#include <stdio.h>

#define SYS_GETPROCS 83
#define TASK_NAME_MAX 32
#define PROCS_MAX 64

/* Mirrors struct tus_procinfo in kernel/sched/sched.h exactly - field
 * order, types and sizes must match what the kernel writes. */
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

static const char *state_name(uint32_t state) {
    switch (state) {
    case 1: return "ready";
    case 2: return "running";
    case 3: return "zombie";
    case 4: return "stopped";
    default: return "?";
    }
}

int main(void) {
    static struct tus_procinfo procs[PROCS_MAX];
    long n = tus_syscall2(SYS_GETPROCS, (long)procs, sizeof(procs));
    if (n < 0) {
        fprintf(stderr, "ps: error %ld\n", -n);
        return 1;
    }
    printf("PID  PPID PGID UID  STATE   NAME\n");
    for (long i = 0; i < n; i++) {
        printf("%-4u %-4u %-4u %-4u %-7s %s\n", procs[i].pid, procs[i].ppid,
               procs[i].pgid, procs[i].uid, state_name(procs[i].state),
               procs[i].name);
    }
    return 0;
}
