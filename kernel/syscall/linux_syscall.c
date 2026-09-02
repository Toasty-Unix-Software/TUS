/*
 * linux_syscall.c - a narrow Linux x86_64 binary-compatibility layer
 *
 * TUS's native ABI is `int $0x80`/`$0x81` with TUS-specific syscall
 * numbers (syscall.h); a real Linux binary instead executes the
 * `syscall` (0f 05) instruction with the Linux x86_64 syscall table.
 * Those are genuinely different CPU instructions - `int $0x80` traps
 * through the IDT, `syscall` traps through the SYSCALL/SYSRET MSRs,
 * which TUS never programmed, so a Linux binary's first syscall used
 * to fault #UD (Invalid Opcode) before this file existed.
 *
 * SCOPE, stated honestly:
 *   - Statically-linked Linux x86_64 binaries only. There is no
 *     dynamic linker (ld-linux) here at all; a PT_INTERP binary will
 *     simply fail to load with -ENOEXEC the same as it already did.
 *   - A small, explicit subset of the syscall table: read, write,
 *     close, getpid, exit, exit_group, and mmap (anonymous-only, via
 *     the existing sys_mmap()). Everything else returns -ENOSYS
 *     cleanly rather than crashing. open(2) is intentionally NOT
 *     wired up yet even though TUS's O_* flag values happen to match
 *     Linux's - vfs_open_mode()'s path-permission model hasn't been
 *     audited against being reachable from this new entry point, and
 *     getting a false sense of file-access safety wrong here is worse
 *     than a static binary getting -ENOSYS from open(2) until that
 *     audit happens.
 *   - Gated behind CAP_LINUX_EXEC (kernel/sched/cap.h): only a task
 *     that kernel/elf/tus_elf.c exec'd as a detected foreign-ELF
 *     binary (which itself requires the caller to hold that
 *     capability) has task->linux_abi set, and the SYSCALL entry
 *     below refuses to run Linux syscalls for anything else - so a
 *     native TUS binary that somehow executed `syscall` (it never
 *     should; musl's own copy uses int 0x80/0x81) is killed rather
 *     than granted a whole second, less-audited syscall surface.
 *
 * SYSRET's return-selector formula is rigid: for 64-bit SYSRETQ,
 * CS = (IA32_STAR[63:48] + 16) | 3 and SS = (IA32_STAR[63:48] + 8) | 3.
 * TUS's normal ring-3 selectors are 0x18 (code) then 0x20 (data) - no
 * STAR value produces that pair via the formula (it needs data
 * BEFORE code). gdt.c installs a second, otherwise-identical pair at
 * 0x40 (data) / 0x48 (code) purely to satisfy SYSRETQ; a task's
 * FIRST entry into ring 3 still uses the normal IRETQ path with
 * 0x18/0x20 (see sched.c), and only starts using 0x40/0x48 once it
 * returns from its first `syscall`. Both pairs describe the same flat
 * ring-3 segment, so this is invisible to the running program.
 *
 * Concurrency note: the entry stub switches to ONE static kernel
 * stack (g_linux_kstack), not a per-task one. That is only safe
 * because TUS's scheduler currently runs every task on a single core
 * (see the SMP work's own scope note: APs never leave halt), so at
 * most one `syscall` instruction is ever mid-flight at a time. A real
 * per-task or per-CPU kernel stack would be needed before SMP tasks
 * could use this concurrently.
 */

#include <stdint.h>

#include "linux_syscall.h"

#include "../arch/x86_64/gdt.h"
#include "../arch/x86_64/io.h"
#include "../core/errno.h"
#include "../core/klib.h"
#include "../sched/cap.h"
#include "../sched/sched.h"
#include "../vfs/vfs.h"

/* ---- MSRs ---- */
#define MSR_EFER  0xC0000080
#define MSR_STAR  0xC0000081
#define MSR_LSTAR 0xC0000082
#define MSR_FMASK 0xC0000084
#define EFER_SCE  (1ull << 0)

extern void linux_syscall_entry(void);

/* Reused from kernel/syscall/syscall.c: anonymous-only mmap, already
 * exactly the shape a Linux-compat mmap(2) needs (Linux's
 * MAP_ANONYMOUS is the same bit value, 0x20). */
extern long sys_mmap(long addr, long len, long prot, long flags);

/* Upper bound of the canonical user half - same constant as
 * syscall.c's access_ok(), duplicated because that one is static. */
#define USER_HALF_MAX 0x00007fffffffffffull

static bool linux_access_ok(const void *ptr, size_t len) {
    uint64_t a = (uint64_t)(uintptr_t)ptr;
    uint64_t e = a + len;
    return e >= a && e <= USER_HALF_MAX;
}

/* Linux x86_64 syscall numbers actually implemented. */
#define LX_SYS_read        0
#define LX_SYS_write       1
#define LX_SYS_close       3
#define LX_SYS_mmap        9
#define LX_SYS_getpid      39
#define LX_SYS_exit        60
#define LX_SYS_exit_group  231

/* Register image the asm stub hands us (lowest address first: the
 * order it pushed them in, so this struct's layout must mirror the
 * push sequence exactly). */
struct linux_regs {
    uint64_t rax; /* syscall number in, return value out */
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t r10;
    uint64_t r8;
    uint64_t r9;
};

/* Called by linux_syscall_entry() with a pointer to the saved
 * registers; writes the return value into r->rax. */
void linux_syscall_dispatch(struct linux_regs *r) {
    struct task *cur = sched_current();
    if (cur == NULL || !cur->linux_abi) {
        /* A native TUS task hit the `syscall` instruction - that
         * should be unreachable (musl's clone.s/vfork.s fix removed
         * the only place it used to happen), so treat it as the
         * fault it is rather than quietly emulating anything. */
        task_exit(128 + 4); /* SIGILL, matching the #UD it replaces */
    }

    long ret;
    switch (r->rax) {
    case LX_SYS_read:
        if (!linux_access_ok((void *)r->rsi, (size_t)r->rdx)) {
            ret = -EFAULT;
        } else {
            ret = vfs_read((long)r->rdi, (void *)r->rsi, (size_t)r->rdx);
        }
        break;
    case LX_SYS_write:
        if (!linux_access_ok((const void *)r->rsi, (size_t)r->rdx)) {
            ret = -EFAULT;
        } else {
            ret = vfs_write((long)r->rdi, (const void *)r->rsi, (size_t)r->rdx);
        }
        break;
    case LX_SYS_close:
        ret = vfs_close((long)r->rdi);
        break;
    case LX_SYS_mmap:
        /* Linux's mmap(addr, len, prot, flags, fd, offset); TUS's
         * anonymous-only sys_mmap() takes just the first four - a
         * file-backed request (no MAP_ANONYMOUS) is rejected inside
         * it exactly like the native path. */
        ret = sys_mmap((long)r->rdi, (long)r->rsi, (long)r->rdx, (long)r->r10);
        break;
    case LX_SYS_getpid:
        ret = (long)cur->pid;
        break;
    case LX_SYS_exit:
    case LX_SYS_exit_group:
        /* No thread groups to distinguish (TUS has no threads yet),
         * so exit and exit_group are the same operation here. */
        task_exit((int)r->rdi);
        __builtin_unreachable();
    default:
        ret = -ENOSYS;
        break;
    }
    r->rax = (uint64_t)ret;
}

/* A small static kernel stack for the SYSCALL entry - see the
 * concurrency note in the file comment above for why one shared
 * stack is currently safe. 16 KiB matches the margin TUS's other
 * fixed kernel stacks use for a handful of nested C calls. */
static uint8_t g_linux_kstack[16384] __attribute__((aligned(16)));
/* __attribute__((used)): both are only ever touched from inside the
 * raw asm text of linux_syscall_entry() below, which the compiler's
 * own use-analysis cannot see - without this, --gc-sections quietly
 * drops them and the linker fails with "undefined reference". */
static uint64_t g_linux_kstack_top __attribute__((used)) =
    (uint64_t)(uintptr_t)(g_linux_kstack + sizeof(g_linux_kstack));
static uint64_t g_linux_user_rsp __attribute__((used));

/* SYSCALL enters here directly (LSTAR points at this symbol): RCX
 * holds the return RIP and R11 the saved RFLAGS (both clobbered by
 * the CPU, per the SYSCALL spec), RSP is still the user stack. The
 * stub parks the user RSP, switches to the scratch kernel stack,
 * saves the argument/number registers plus RCX/R11, calls the C
 * dispatcher, restores everything and SYSRETQs back - mirroring
 * syscall_entry()'s int-0x80 stub in kernel/syscall/syscall.c, just
 * for the different (hardware-defined) register/selector contract. */
__attribute__((naked)) void linux_syscall_entry(void) {
    __asm__ volatile(
        "movq %rsp, g_linux_user_rsp(%rip)\n\t"
        "movq g_linux_kstack_top(%rip), %rsp\n\t"
        "push %r11\n\t"      /* saved rflags */
        "push %rcx\n\t"      /* saved return rip */
        "push %r9\n\t"
        "push %r8\n\t"
        "push %r10\n\t"
        "push %rdx\n\t"
        "push %rsi\n\t"
        "push %rdi\n\t"
        "push %rax\n\t"
        "mov %rsp, %rdi\n\t"
        "call linux_syscall_dispatch\n\t"
        "pop %rax\n\t"
        "pop %rdi\n\t"
        "pop %rsi\n\t"
        "pop %rdx\n\t"
        "pop %r10\n\t"
        "pop %r8\n\t"
        "pop %r9\n\t"
        "pop %rcx\n\t"
        "pop %r11\n\t"
        "movq g_linux_user_rsp(%rip), %rsp\n\t"
        "sysretq\n");
}

void linux_syscall_init(void) {
    /* EFER.SCE: without this bit, `syscall` still raises #UD. */
    uint64_t efer = rdmsr(MSR_EFER);
    wrmsr(MSR_EFER, efer | EFER_SCE);

    /* STAR[47:32] = kernel CS for SYSCALL's own CS/SS (SS = CS+8, so
     * 0x08/0x10 - exactly gdt.c's kernel code/data). STAR[63:48] is
     * the arithmetic base SYSRETQ derives its CS/SS from; see the
     * SYSRET comment in this file's header and in gdt.h. */
    uint64_t star = ((uint64_t)SEL_SYSRET_USER_BASE << 48) |
                    ((uint64_t)SEL_KERNEL_CODE << 32);
    wrmsr(MSR_STAR, star);

    wrmsr(MSR_LSTAR, (uint64_t)(uintptr_t)linux_syscall_entry);

    /* RFLAGS bits cleared on entry: IF (no nested interrupts while
     * we're on the one shared scratch stack), TF (no single-step
     * escape), DF (string-op direction must start clean). */
    wrmsr(MSR_FMASK, 0x700);
}
