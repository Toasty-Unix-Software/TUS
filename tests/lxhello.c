/* lxhello.c - a genuine, unmodified static Linux x86_64 binary.
 *
 * Compiled with `clang -target x86_64-linux-gnu -static -nostdlib`
 * (see the Makefile rule) rather than TUS's own USER_CFLAGS: it uses
 * the real Linux `syscall` (0f 05) instruction and the real Linux
 * x86_64 syscall numbers (1=write, 60=exit), and links at ~0x400000
 * (lld's default non-PIE base), nowhere near TUS's own fixed
 * -Ttext 0x10000000 convention. Both are exactly what
 * kernel/elf/tus_elf.c's elf_is_foreign_linux() and
 * kernel/syscall/linux_syscall.c's SYSCALL/SYSRET handler exist to
 * detect and run. See tests/test_linux_elf.py.
 */

static long sys_write(long fd, const void *buf, long n) {
    long ret;
    __asm__ volatile("syscall"
                     : "=a"(ret)
                     : "a"(1), "D"(fd), "S"(buf), "d"(n)
                     : "rcx", "r11", "memory");
    return ret;
}

static void sys_exit(long code) {
    __asm__ volatile("syscall" : : "a"(60), "D"(code));
    __builtin_unreachable();
}

void _start(void) {
    const char msg[] = "hello from linux elf on TUS\n";
    sys_write(1, msg, sizeof(msg) - 1);
    sys_exit(0);
}
