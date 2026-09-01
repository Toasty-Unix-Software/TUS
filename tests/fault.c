/*
 * fault - a program that breaks itself, on purpose
 *
 * The kernel used to panic when a ring-3 program faulted, which meant
 * one bad pointer in one application stopped the machine. It now
 * kills the task and carries on, and this is what proves it: run
 * `fault` and the shell comes back with the reason on the line above.
 *
 *   fault          write through a null pointer   (page fault)
 *   fault read     read through a null pointer    (page fault)
 *   fault kernel   write into the kernel's half   (page fault)
 *   fault opcode   execute something that is not  (invalid opcode)
 *   fault divide   divide by zero
 *
 * Freestanding, like hello and enforce: no libc, so nothing between
 * the fault and the instruction that caused it.
 */

/* write(1, buf, len) through the TUS ABI (see tests/enforce.c). */
static long tus_syscall(long number, long a1, long a2, long a3) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "a"(number)
                     : "rcx", "r11", "memory");
    return ret;
}

static void print(const char *s) {
    long len = 0;
    while (s[len] != '\0') len++;
    tus_syscall(2, 1, (long)s, len);
}

static int same(const char *a, const char *b) {
    while (*a != '\0' && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

/*
 * The task loader leaves argc at the top of the stack with argv after
 * it, the way SysV says. With no C runtime to unpack that, _start
 * hands the stack pointer over as it is and the work happens in C.
 */
static void run(long *sp) __attribute__((used));
static void run(long *sp) {
    long argc = sp[0];
    char **argv = (char **)&sp[1];

    const char *what = argc >= 2 && argv[1] != (void *)0 ? argv[1] : "write";

    if (same(what, "read")) {
        print("fault: reading through a null pointer\n");
        volatile int *p = (int *)0;
        int v = *p;
        (void)v;
    } else if (same(what, "kernel")) {
        print("fault: writing into the kernel's half of the address space\n");
        volatile long *p = (long *)0xffffffff81000000ull;
        *p = 1;
    } else if (same(what, "opcode")) {
        print("fault: executing an invalid opcode\n");
        __asm__ volatile("ud2");
    } else if (same(what, "divide")) {
        /* Written out as the instruction: a compiler that can see the
         * zero is entitled to fold the division away, and at -O2 it
         * does. */
        print("fault: dividing by zero\n");
        int zero = 0;
        __asm__ volatile("xor %%edx, %%edx\n\t"
                         "idivl %1"
                         :
                         : "a"(1), "r"(zero)
                         : "rdx", "cc");
    } else {
        print("fault: writing through a null pointer\n");
        volatile int *p = (int *)0;
        *p = 1;
    }

    print("fault: still running - the fault did not happen\n");
    tus_syscall(0, 1, 0, 0);   /* exit(1) */
    for (;;) {
    }
}

__attribute__((naked)) void _start(void) {
    __asm__ volatile("mov %rsp, %rdi\n\t"
                     "call run\n");
}
