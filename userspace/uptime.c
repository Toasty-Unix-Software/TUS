/*
 * uptime.c - time since boot (TUS port). Real /bin binary version of
 * tsh's cmd_uptime (kernel/shell/cmd_fs.c).
 *
 * SYS_UPTIME is TUS's own ABI (milliseconds since boot via the PIT) -
 * there is no Linux syscall number for it, so musl has no wrapper
 * (same situation as SYS_READDIR); this makes the raw int $0x80 call
 * itself, same pattern as userspace/hxfiles.c.
 */

#include <stdio.h>

#define SYS_UPTIME 7

static long tus_syscall0(long n) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = 0;
    register long rsi __asm__("rsi") = 0;
    register long rdx __asm__("rdx") = 0;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "a"(n)
                     : "memory", "cc");
    return ret;
}

int main(void) {
    long ms = tus_syscall0(SYS_UPTIME);
    printf("uptime: %ld.%03ld s\n", ms / 1000, ms % 1000);
    return 0;
}
