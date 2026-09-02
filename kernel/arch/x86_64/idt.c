/*
 * idt.c - Interrupt Descriptor Table implementation
 *
 * All 256 IDT slots are populated:
 *   - 0..31   : CPU exceptions -> the faulting ring-3 task is killed;
 *               a fault in the kernel dumps registers and halts
 *   - 32..47  : PIC hardware interrupts -> dispatch to registered IRQ
 *               handlers, then acknowledge the PIC
 *   - 48..255 : ignored (silently returned from)
 *
 * GCC's "interrupt" attribute turns a normal C function into a full
 * interrupt service routine: it saves all clobbered registers, uses
 * IRETQ to return, and consumes a CPU-pushed error code when the
 * function declares a second parameter. This keeps the whole IDT layer
 * in portable C.
 */

#include "idt.h"
#include "pic.h"
#include "io.h"
#include "core/console.h"
#include "drivers/serial/serial.h"
#include "core/klib.h"
#include "mm/swap.h"
#include "mm/vmm.h"
#include "sched/sched.h"
#include "syscall/syscall.h"
#include "core/panic_screen.h"

/* 64-bit interrupt gate: present, DPL 0. IF is cleared on entry. */
#define IDT_GATE_64_INTERRUPT 0x8E

/* 64-bit trap gate at DPL 3: used for the syscall vector (int 0x80),
 * callable from user mode once processes exist, without clearing IF. */
#define IDT_GATE_64_TRAP_USER 0xEF

/* Vector used for POSIX system calls. */
#define IDT_VECTOR_SYSCALL 0x80

/* 64-bit trap gate at DPL 3: used for the syscall vector (int 0x80),
 * callable from user mode once processes exist, without clearing IF. */
#define IDT_GATE_64_TRAP_USER 0xEF

/* Vector used for POSIX system calls. */
#define IDT_VECTOR_SYSCALL 0x80

/* Code segment selector the kernel runs in. TUS installs its own GDT
 * (gdt.c) where 0x08 is the 64-bit kernel code segment; interrupt
 * gates must point back at 0x08. */
#define KERNEL_CODE_SELECTOR 0x08

struct idt_entry {
    uint16_t offset_low;   /* bits  0..15 of handler address */
    uint16_t selector;     /* code segment selector */
    uint8_t  ist;          /* interrupt stack table index (0 = none) */
    uint8_t  attributes;   /* gate type, DPL, present bit */
    uint16_t offset_mid;   /* bits 16..31 of handler address */
    uint32_t offset_high;  /* bits 32..63 of handler address */
    uint32_t zero;         /* reserved, must be zero */
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;        /* size of the IDT in bytes minus one */
    uint64_t base;         /* virtual address of the IDT */
} __attribute__((packed));

static struct idt_entry g_idt[256];
static struct idt_ptr g_idt_ptr;
static irq_handler_t g_irq_handlers[16];

/* Human-readable names for the first 32 vectors. */
static const char *const g_exception_names[32] = {
    "Divide Error",              "Debug",                  "Non-Maskable Interrupt",
    "Breakpoint",                "Overflow",               "Bound Range Exceeded",
    "Invalid Opcode",            "Device Not Available",   "Double Fault",
    "Coprocessor Segment Overrun","Invalid TSS",           "Segment Not Present",
    "Stack-Segment Fault",       "General Protection",     "Page Fault",
    "Reserved",                  "x87 FPU Error",          "Alignment Check",
    "Machine Check",             "SIMD FPU Exception",     "Virtualization Exception",
    "Control Protection",        "Reserved",               "Reserved",
    "Reserved",                  "Reserved",               "Reserved",
    "Reserved",                  "Hypervisor Injection",   "VMM Communication",
    "Security Exception",        "Reserved"
};

/* Vectors on which the CPU pushes an error code onto the stack.
 * The DEFINE_EXC_WITH_ERROR lines below are exactly these vectors:
 * 8, 10, 11, 12, 13, 14, 17, 21, 29, 30. */

/*
 * ---- a fault in a user program ----
 *
 * A ring-3 program that faults has broken itself, not the machine, so
 * TUS does what every Unix does: says what happened, kills the task
 * and carries on. task_exit() gives back the task's windows, its
 * keyboard grab and its file descriptors and switches to the next
 * task, so it never returns here - which is exactly right, because
 * there is no user context left to return to.
 *
 * Only the kernel's own faults reach the panic path below. So do the
 * three exceptions that are never a program's fault however they were
 * raised: NMI, double fault (its stack is not to be trusted) and
 * machine check.
 */
static bool fault_from_user_task(const uint64_t cs, int vec) {
    if (vec == 2 || vec == 8 || vec == 18) return false;
    if (sched_current() == NULL) return false;
    return (cs & 3) == 3;   /* the CPL the fault came from */
}

/* The signal a Unix would have killed it with; the exit status is
 * 128 + that, which is what a shell reports. */
static int fault_signal(int vec) {
    switch (vec) {
    case 0:  case 4:  case 16: case 19: return 8;  /* SIGFPE */
    case 3:  case 1:                    return 5;  /* SIGTRAP */
    case 6:                             return 4;  /* SIGILL */
    case 17:                            return 7;  /* SIGBUS */
    default:                            return 11; /* SIGSEGV */
    }
}

/* A page fault's error code, in the words the reader needs: what the
 * program was doing to the address, and why it was not allowed. */
static const char *page_fault_reason(uint64_t error_code) {
    int present = (error_code & 1) != 0;
    if ((error_code & 0x10) != 0) {
        return present ? "executing protected memory"
                       : "executing unmapped memory";
    }
    if ((error_code & 2) != 0) {
        return present ? "writing to protected memory"
                       : "writing to unmapped memory";
    }
    return present ? "reading protected memory" : "reading unmapped memory";
}

static void fault_kill_task(int vec, uint64_t error_code, bool has_error_code,
                            uint64_t rip) __attribute__((noreturn));
static void fault_kill_task(int vec, uint64_t error_code, bool has_error_code,
                            uint64_t rip) {
    const struct task *t = sched_current();

    if (vec == 14) {
        uint64_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        kprintf("fault: %s (pid %u) killed: %s at 0x%llx, rip 0x%llx\n",
                t->name, (unsigned)t->pid, page_fault_reason(error_code),
                (unsigned long long)cr2, (unsigned long long)rip);
    } else if (has_error_code) {
        kprintf("fault: %s (pid %u) killed: %s (error 0x%llx), rip 0x%llx\n",
                t->name, (unsigned)t->pid, g_exception_names[vec],
                (unsigned long long)error_code, (unsigned long long)rip);
    } else {
        kprintf("fault: %s (pid %u) killed: %s, rip 0x%llx\n", t->name,
                (unsigned)t->pid, g_exception_names[vec],
                (unsigned long long)rip);
    }

    /* A fault is a signal death (SIGSEGV/SIGFPE/...), not a real
     * exit(2) - waitpid()'s WIFSIGNALED/WTERMSIG depend on knowing
     * that (kernel/syscall/syscall.c's sys_waitpid(), struct task::
     * exit_signaled). task_exit() itself has no way to tell the two
     * apart since a real exit(2) also just calls it with a plain
     * status. */
    sched_mark_exit_signaled();
    task_exit(128 + fault_signal(vec));
}

/* Page fault (vector 14): the #PF handler is wrapped in a naked stub
 * that saves every register, so a crash dump shows the exact state
 * (including RDI = memset/memcpy destination) instead of only the
 * CPU-pushed frame. */
struct pf_regs {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi;
    uint64_t rbp, r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t error_code;
    uint64_t rip, cs, rflags, rsp, ss;
};

static void exc_page_fault_c(struct pf_regs *r) __attribute__((used));
static void exc_page_fault_c(struct pf_regs *r) {
    /* Swap-in check runs before anything else, for every ring: a page
     * kernel/mm/swap.c evicted carries a not-present PTE with a
     * software marker bit, and the fault it produces is not a real
     * fault at all - read the page back from disk, restore the PTE,
     * and return so the faulting instruction just retries. Only a
     * genuine fault (never evicted, or a real bug) falls through to
     * the kill/panic paths below. */
    {
        uint64_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        uint64_t cr3 = vmm_current_cr3();
        if (swap_fault(cr3, cr2)) {
            return;
        }
    }

    if (fault_from_user_task(r->cs, 14)) {
        fault_kill_task(14, r->error_code, true, r->rip);
    }

    /* Put the serial mirror back into direct mode first: the queue
     * is drained by an interrupt, and a panic never returns to let
     * one happen. */
    serial_sync();

    panic_screen_show(PANIC_GSOD, "Page fault in the kernel");
    console_write("\n\n*** KERNEL PANIC ***\n");
    kprintf("Exception 14: Page Fault\n");
    kprintf("Error code: 0x%llx\n", (unsigned long long)r->error_code);
    uint64_t cr2;
    __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
    kprintf("CR2     : 0x%llx\n", (unsigned long long)cr2);
    kprintf("RDI     : 0x%llx\n", (unsigned long long)r->rdi);
    kprintf("RSI     : 0x%llx\n", (unsigned long long)r->rsi);
    kprintf("RDX     : 0x%llx\n", (unsigned long long)r->rdx);
    kprintf("RAX     : 0x%llx\n", (unsigned long long)r->rax);
    kprintf("RCX     : 0x%llx\n", (unsigned long long)r->rcx);
    kprintf("RBX     : 0x%llx\n", (unsigned long long)r->rbx);
    kprintf("RBP     : 0x%llx\n", (unsigned long long)r->rbp);
    kprintf("R12     : 0x%llx\n", (unsigned long long)r->r12);
    kprintf("RIP    : 0x%llx\n", (unsigned long long)r->rip);
    kprintf("CS     : 0x%llx\n", (unsigned long long)r->cs);
    kprintf("RFLAGS : 0x%llx\n", (unsigned long long)r->rflags);
    kprintf("RSP    : 0x%llx\n", (unsigned long long)r->rsp);
    kprintf("SS     : 0x%llx\n", (unsigned long long)r->ss);
    kprintf("STACK  :");
    uint64_t *sp = (uint64_t *)(uintptr_t)r->rsp;
    for (int i = 0; i < 24; i++) {
        kprintf(" %llx", (unsigned long long)sp[i]);
    }
    kprintf("\n");
    console_write("System halted.\n");
    for (;;) {
        cli();
        hlt();
    }
}

__attribute__((naked)) static void exc_page_fault(void) {
    __asm__ volatile(
        "push %r15\n\t"
        "push %r14\n\t"
        "push %r13\n\t"
        "push %r12\n\t"
        "push %r11\n\t"
        "push %r10\n\t"
        "push %r9\n\t"
        "push %r8\n\t"
        "push %rbp\n\t"
        "push %rdi\n\t"
        "push %rsi\n\t"
        "push %rdx\n\t"
        "push %rcx\n\t"
        "push %rbx\n\t"
        "push %rax\n\t"
        "mov %rsp, %rdi\n\t"
        "call exc_page_fault_c\n\t"
        /* Only reached when exc_page_fault_c returned - the swap-in
         * case above. Every other path it takes ends in task_exit()
         * or the panic loop, neither of which comes back here. Undo
         * the pushes in reverse order, drop the hardware error code
         * (vector 14 pushes one), then IRETQ back to the faulting
         * instruction to retry it against the now-present page. */
        "pop %rax\n\t"
        "pop %rbx\n\t"
        "pop %rcx\n\t"
        "pop %rdx\n\t"
        "pop %rsi\n\t"
        "pop %rdi\n\t"
        "pop %rbp\n\t"
        "pop %r8\n\t"
        "pop %r9\n\t"
        "pop %r10\n\t"
        "pop %r11\n\t"
        "pop %r12\n\t"
        "pop %r13\n\t"
        "pop %r14\n\t"
        "pop %r15\n\t"
        "add $8, %rsp\n\t"
        "iretq\n");
}

/*
 * Fatal exception: print a register dump and stop the system. This is
 * the kernel panic path. We disable interrupts and halt forever so the
 * state stays visible on screen and on the serial log.
 */
static void exception_fatal(const struct interrupt_frame *frame,
                            uint64_t error_code, int vec, bool has_error_code) {
    if (fault_from_user_task(frame->cs, vec)) {
        fault_kill_task(vec, error_code, has_error_code, frame->rip);
    }

    /* Put the serial mirror back into direct mode first: the queue
     * is drained by an interrupt, and a panic never returns to let
     * one happen. */
    serial_sync();

    /* Double fault (8) and machine check (18) are the two vectors
     * that mean the hardware itself is in a bad state, not just
     * kernel logic - RSOD for those, GSOD for everything else. */
    enum panic_kind kind = (vec == 8 || vec == 18) ? PANIC_RSOD : PANIC_GSOD;
    panic_screen_show(kind, g_exception_names[vec]);

    console_write("\n\n*** KERNEL PANIC ***\n");
    kprintf("Exception %d: %s\n", vec, g_exception_names[vec]);
    if (has_error_code) {
        kprintf("Error code: 0x%llx\n", (unsigned long long)error_code);
    }
    if (vec == 14) { /* Page Fault: CR2 holds the faulting address. */
        uint64_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        kprintf("CR2     : 0x%llx\n", (unsigned long long)cr2);
        kprintf("PTE     : 0x%llx (translate 0x%llx)\n",
                (unsigned long long)vmm_pte(cr2),
                (unsigned long long)vmm_translate(cr2));
    }
    kprintf("RIP    : 0x%llx\n", (unsigned long long)frame->rip);
    kprintf("CS     : 0x%llx\n", (unsigned long long)frame->cs);
    kprintf("RFLAGS : 0x%llx\n", (unsigned long long)frame->rflags);
    kprintf("RSP    : 0x%llx\n", (unsigned long long)frame->rsp);
    kprintf("SS     : 0x%llx\n", (unsigned long long)frame->ss);
    console_write("System halted.\n");
    for (;;) {
        cli();
        hlt();
    }
}

/* Device Not Available (#NM) handler: clear TS bit in CR0 to allow FPU access.
 * When a task with dirty FPU state is preempted, CR0.TS is set. When the next
 * task tries to use the FPU, #NM is raised. We just clear TS and retry. */
__attribute__((interrupt)) static void exc_no_error_7(struct interrupt_frame *frame) {
    uint64_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 &= ~(1ull << 3);   /* CR0.TS: clear task switched bit */
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0) : "memory");
    /* Return and retry the FPU instruction */
}

/* One generated handler per exception vector; the two families differ
 * only in whether the CPU pushes an error code for that vector. */
#define DEFINE_EXC_NO_ERROR(vec)                                          \
    __attribute__((interrupt)) static void exc_no_error_##vec(            \
        struct interrupt_frame *frame) {                                  \
        exception_fatal(frame, 0, vec, false);                            \
    }

#define DEFINE_EXC_WITH_ERROR(vec)                                        \
    __attribute__((interrupt)) static void exc_with_error_##vec(          \
        struct interrupt_frame *frame, uint64_t error_code) {             \
        exception_fatal(frame, error_code, vec, true);                    \
    }

DEFINE_EXC_NO_ERROR(0)  DEFINE_EXC_NO_ERROR(1)  DEFINE_EXC_NO_ERROR(2)
DEFINE_EXC_NO_ERROR(3)  DEFINE_EXC_NO_ERROR(4)  DEFINE_EXC_NO_ERROR(5)
DEFINE_EXC_NO_ERROR(6)  DEFINE_EXC_WITH_ERROR(8)
DEFINE_EXC_NO_ERROR(9)  DEFINE_EXC_WITH_ERROR(10) DEFINE_EXC_WITH_ERROR(11)
DEFINE_EXC_WITH_ERROR(12) DEFINE_EXC_WITH_ERROR(13)
DEFINE_EXC_NO_ERROR(15) DEFINE_EXC_NO_ERROR(16) DEFINE_EXC_WITH_ERROR(17)
DEFINE_EXC_NO_ERROR(18) DEFINE_EXC_NO_ERROR(19) DEFINE_EXC_NO_ERROR(20)
DEFINE_EXC_WITH_ERROR(21) DEFINE_EXC_NO_ERROR(22) DEFINE_EXC_NO_ERROR(23)
DEFINE_EXC_NO_ERROR(24) DEFINE_EXC_NO_ERROR(25) DEFINE_EXC_NO_ERROR(26)
DEFINE_EXC_NO_ERROR(27) DEFINE_EXC_NO_ERROR(28) DEFINE_EXC_WITH_ERROR(29)
DEFINE_EXC_WITH_ERROR(30) DEFINE_EXC_NO_ERROR(31)

/* Vector -> handler lookup tables. Function pointers are stored as
 * integers to keep the tables free of pedantic cast warnings. */
#define STUB_CAST(fn) ((uintptr_t)(fn))

static const uintptr_t g_exception_stubs[32] = {
    [0]  = STUB_CAST(exc_no_error_0),   [1]  = STUB_CAST(exc_no_error_1),
    [2]  = STUB_CAST(exc_no_error_2),   [3]  = STUB_CAST(exc_no_error_3),
    [4]  = STUB_CAST(exc_no_error_4),   [5]  = STUB_CAST(exc_no_error_5),
    [6]  = STUB_CAST(exc_no_error_6),   [7]  = STUB_CAST(exc_no_error_7),  /* FPU handler */
    [8]  = STUB_CAST(exc_with_error_8), [9]  = STUB_CAST(exc_no_error_9),
    [10] = STUB_CAST(exc_with_error_10),[11] = STUB_CAST(exc_with_error_11),
    [12] = STUB_CAST(exc_with_error_12),[13] = STUB_CAST(exc_with_error_13),
    [14] = STUB_CAST(exc_page_fault),[15] = STUB_CAST(exc_no_error_15),
    [16] = STUB_CAST(exc_no_error_16),[17] = STUB_CAST(exc_with_error_17),
    [18] = STUB_CAST(exc_no_error_18),[19] = STUB_CAST(exc_no_error_19),
    [20] = STUB_CAST(exc_no_error_20),[21] = STUB_CAST(exc_with_error_21),
    [22] = STUB_CAST(exc_no_error_22),[23] = STUB_CAST(exc_no_error_23),
    [24] = STUB_CAST(exc_no_error_24),[25] = STUB_CAST(exc_no_error_25),
    [26] = STUB_CAST(exc_no_error_26),[27] = STUB_CAST(exc_no_error_27),
    [28] = STUB_CAST(exc_no_error_28),[29] = STUB_CAST(exc_with_error_29),
    [30] = STUB_CAST(exc_with_error_30),[31] = STUB_CAST(exc_no_error_31),
};

/*
 * Common path for all hardware interrupts: run the registered handler
 * (if any) and acknowledge the PIC so the next interrupt can arrive.
 */
static void irq_dispatch(uint8_t irq, struct interrupt_frame *frame) {
    /*
     * Spurious IRQ7/IRQ15: the PIC can raise these when no device is
     * actually asserting an interrupt. Only acknowledge them if the
     * in-service bit really is set, otherwise the cascade would get
     * stuck.
     */
    /* This 8259-specific spurious check is meaningless once routing
     * has moved to the I/O APIC (pic_using_apic()): the 8259 is fully
     * masked at that point, so its In-Service Register would read 0
     * for these bits regardless, which would make this treat every
     * real IRQ7/IRQ15 as spurious and silently drop it. The Local
     * APIC has its own spurious-vector mechanism (0xFF, idt_init())
     * that does not need this check at all. */
    if (!pic_using_apic() && (irq == 7 || irq == 15)) {
        uint16_t cmd_port = (irq == 7) ? PIC1_CMD_PORT : PIC2_CMD_PORT;
        outb(cmd_port, PIC_READ_ISR);
        if (!(inb(cmd_port) & (1u << (irq & 7)))) {
            return; /* spurious - no EOI */
        }
    }

    if (g_irq_handlers[irq] != NULL) {
        g_irq_handlers[irq](frame);
    }
    pic_send_eoi(irq);
}

/* One IRETQ stub per PIC IRQ line. */
#define DEFINE_IRQ_STUB(irq)                                              \
    __attribute__((interrupt)) static void irq_stub_##irq(                \
        struct interrupt_frame *frame) {                                  \
        irq_dispatch(irq, frame);                                         \
    }

DEFINE_IRQ_STUB(0)  DEFINE_IRQ_STUB(1)  DEFINE_IRQ_STUB(2)  DEFINE_IRQ_STUB(3)
DEFINE_IRQ_STUB(4)  DEFINE_IRQ_STUB(5)  DEFINE_IRQ_STUB(6)  DEFINE_IRQ_STUB(7)
DEFINE_IRQ_STUB(8)  DEFINE_IRQ_STUB(9)  DEFINE_IRQ_STUB(10) DEFINE_IRQ_STUB(11)
DEFINE_IRQ_STUB(12) DEFINE_IRQ_STUB(13) DEFINE_IRQ_STUB(14) DEFINE_IRQ_STUB(15)

static const uintptr_t g_irq_stubs[16] = {
    [0] = STUB_CAST(irq_stub_0),  [1] = STUB_CAST(irq_stub_1),
    [2] = STUB_CAST(irq_stub_2),  [3] = STUB_CAST(irq_stub_3),
    [4] = STUB_CAST(irq_stub_4),  [5] = STUB_CAST(irq_stub_5),
    [6] = STUB_CAST(irq_stub_6),  [7] = STUB_CAST(irq_stub_7),
    [8] = STUB_CAST(irq_stub_8),  [9] = STUB_CAST(irq_stub_9),
    [10] = STUB_CAST(irq_stub_10),[11] = STUB_CAST(irq_stub_11),
    [12] = STUB_CAST(irq_stub_12),[13] = STUB_CAST(irq_stub_13),
    [14] = STUB_CAST(irq_stub_14),[15] = STUB_CAST(irq_stub_15),
};

/* Vectors 48..255: no device behind them; just return. */
__attribute__((interrupt)) static void irq_ignored(struct interrupt_frame *frame) {
    (void)frame;
}

static void idt_set_gate_attr(int vector, uintptr_t handler, uint8_t attributes) {
    g_idt[vector].offset_low  = (uint16_t)(handler & 0xFFFF);
    g_idt[vector].selector    = KERNEL_CODE_SELECTOR;
    g_idt[vector].ist         = 0;
    g_idt[vector].attributes  = attributes;
    g_idt[vector].offset_mid  = (uint16_t)((handler >> 16) & 0xFFFF);
    g_idt[vector].offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
    g_idt[vector].zero        = 0;
}

static void idt_set_gate(int vector, uintptr_t handler) {
    idt_set_gate_attr(vector, handler, IDT_GATE_64_INTERRUPT);
}

/* fork()'s own gate: unlike every other syscall, it needs to capture
 * and later replay the full 15-register CPU state, not just compute
 * and return a value - see fork_entry() in kernel/sched/sched.c. A
 * dedicated vector keeps that entirely separate from syscall_entry(),
 * which every other syscall still shares unmodified. */
#define IDT_VECTOR_FORK 0x81

/* sigreturn's own gate: a signal handler's restorer (musl's
 * __restore_rt, arch/x86_64/src/signal/restore.s) traps here directly
 * rather than going through the int-0x80 dispatch, for the same
 * reason fork() has its own vector instead of widening syscall_entry
 * - see sigreturn_entry()'s comment in kernel/sched/sched.c. */
#define IDT_VECTOR_SIGRETURN 0x82

void idt_init(void) {
    for (int vector = 0; vector < 256; vector++) {
        if (vector < 32) {
            idt_set_gate(vector, g_exception_stubs[vector]);
        } else if (vector == 32) {
            /* IRQ0 (PIT) is the scheduler tick: its stub performs the
             * context switch, so it bypasses the generic IRQ path. */
            idt_set_gate(vector, STUB_CAST(sched_tick_entry));
        } else if (vector < 48) {
            idt_set_gate(vector, g_irq_stubs[vector - 32]);
        } else {
            idt_set_gate(vector, STUB_CAST(irq_ignored));
        }
    }

    /* POSIX system call gate (int 0x80), trap gate at DPL 3. */
    idt_set_gate_attr(IDT_VECTOR_SYSCALL, STUB_CAST(syscall_entry),
                      IDT_GATE_64_TRAP_USER);

    /* fork()'s own gate (int 0x81), same attributes - see the comment
     * on IDT_VECTOR_FORK above. */
    idt_set_gate_attr(IDT_VECTOR_FORK, STUB_CAST(fork_entry),
                      IDT_GATE_64_TRAP_USER);

    /* sigreturn's own gate (int 0x82), same attributes - see the
     * comment on IDT_VECTOR_SIGRETURN above. */
    idt_set_gate_attr(IDT_VECTOR_SIGRETURN, STUB_CAST(sigreturn_entry),
                      IDT_GATE_64_TRAP_USER);

    g_idt_ptr.limit = (uint16_t)(sizeof(g_idt) - 1);
    g_idt_ptr.base  = (uint64_t)(uintptr_t)g_idt;
    __asm__ volatile("lidt %0" : : "m"(g_idt_ptr));
}

void irq_install(uint8_t irq, irq_handler_t handler) {
    if (irq < 16) {
        g_irq_handlers[irq] = handler;
    }
}
