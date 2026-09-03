/*
 * sched.c - round-robin task scheduler implementation
 *
 * Each task has its own kernel stack. The IRQ0 assembly stub
 * (sched_tick_entry) runs on the current task's kernel stack, pushes
 * the caller-saved registers and calls sched_tick() with RSP as the
 * argument. sched_tick() records that RSP in the task, picks the next
 * ready task, updates TSS.RSP0, and returns the next task's saved RSP
 * (or 0 to stay). The stub then switches RSP and IRETQs, which
 * resumes the other task exactly where it was interrupted.
 *
 * A freshly created user task's kernel stack is pre-filled with a
 * fake interrupt frame so that the first switch IRETQs straight into
 * ring 3: SS=user data, RSP=user stack top, RFLAGS with IF set,
 * CS=user code, RIP=entry point. The task runs until it makes a
 * syscall or is preempted; on SYS_EXIT it calls task_exit(), which
 * switches to the next task and never returns.
 */

#include "sched.h"

#include "arch/x86_64/gdt.h"
#include "arch/x86_64/io.h"
#include "arch/x86_64/pic.h"
#include "core/console.h"
#include "core/errno.h"
#include "core/klib.h"
#include "drivers/pit/pit.h"
#include "mm/kmalloc.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "drivers/keyboard/keyboard.h"
#include "../highx/highx.h"
#include "../term/term.h"
#include "vfs/vfs.h"

/* Selectors from gdt.c (duplicated here to keep sched.c self-contained
 * for the fake IRETQ frame; they must match gdt.c). User segments are
 * loaded with RPL=3 so the IRETQ actually drops to ring 3. */
#define SEL_USER_CODE 0x1B /* 0x18 | RPL 3 */
#define SEL_USER_DATA 0x23 /* 0x20 | RPL 3 */
/* Ring-0 tasks (a terminal session's shell) IRETQ back into the
 * kernel's own segments. In long mode IRETQ pops SS:RSP whatever the
 * privilege change, so the frame has the same shape either way. */
#define SEL_KERNEL_CODE 0x08
#define SEL_KERNEL_DATA 0x10

#define STACK_SIZE 16384   /* kernel stacks: 16 KiB */
#define USER_STACK_SIZE 1048576
/* Matches MAX_ARGV in kernel/syscall/syscall.c: a real compiler
 * driver's argv is much bigger than a shell command (PCC's cc passes
 * ~90 predefine flags to cpp for one file). The strings and pointer
 * array still have to fit in the one page task_create_user reserves
 * for them below (`need > 4096` check). */
#define TASK_MAX_ARGS 128
/* Frame words: 9 caller-saved + 6 callee-saved (rbx rbp r12-r15) + 5
 * IRETQ fields. The callee-saved registers MUST travel with the task:
 * a tick can preempt ring-0 code mid-function, detour through a
 * ring-3 task that clobbers them, and resume the interrupted code -
 * which then computes with garbage register values (the fb_scroll_up
 * corruption bug). */
#define FRAME_WORDS 20     /* 15 registers + 5 IRETQ fields */

/* Virtual address where user stacks live. Each task has its own
 * address space, so every task can use the same stack address; the
 * pages are mapped with VMM_USER in the task's private space (the
 * kernel heap is supervisor-only and ring 3 must not touch it). */
#define USER_STACK_BASE 0x60000000ull

/* First address handed out by SYS_MMAP (must match syscall.c). */
#define MMAP_CURSOR_START 0x40000000ull

/* IA32_FS_BASE model-specific register (see io.h wrmsr/rdmsr). */
#define MSR_FS_BASE 0xC0000100

/* AT_PAGESZ auxv entry: the C library (musl) reads the page size
 * from the auxiliary vector at startup; without it the allocator
 * breaks (page_size 0). */
#define AT_PAGESZ 6

static struct task g_tasks[TASK_MAX];
static struct task *g_current;
static uint32_t g_next_pid = 1;
static int g_preempt_depth; /* >0: kernel code must not be switched */

static struct task *task_find_slot(void) {
    /* Reuse a zombie slot first (the task finished; its kernel stack
     * and address space are reclaimed lazily), then an unused one.
     * Without recycling, TASK_MAX (16) spawned programs would fill
     * the table and later execs would fail. */
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].state == TASK_ZOMBIE) {
            memset(&g_tasks[i], 0, sizeof(g_tasks[i]));
            return &g_tasks[i];
        }
    }
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].state == 0) { /* unused slot */
            return &g_tasks[i];
        }
    }
    return NULL;
}

/* Round-robin: next live task after the current one. */
static struct task *task_next(struct task *after) {
    for (int i = 1; i <= TASK_MAX; i++) {
        struct task *t = &g_tasks[(after - g_tasks + i) % TASK_MAX];
        if (t->state == TASK_READY || t->state == TASK_RUNNING) {
            return t;
        }
    }
    return after;
}

struct task *sched_current(void) {
    return g_current;
}

int sched_task_count(void) {
    int n = 0;
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].state == TASK_READY || g_tasks[i].state == TASK_RUNNING) {
            n++;
        }
    }
    return n;
}

int sched_task_reap(uint32_t pid, int *status) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].pid != pid) continue;
        if (g_tasks[i].state != TASK_ZOMBIE) {
            return 0;
        }
        if (status != NULL) {
            *status = g_tasks[i].exit_status;
        }
        /* A reaped zombie must stop being observable: waitpid(-1)
         * only ever returns a pid once (POSIX), but this slot was
         * left at TASK_ZOMBIE until task_find_slot() next needed it
         * for something else - so a job-control shell's normal "loop
         * waitpid(WNOHANG) until it returns 0" reap idiom (ksh
         * included) saw the same already-dead child match every
         * single call and never terminated. Clearing it here is the
         * same reclaim task_find_slot() already does for a zombie
         * slot, just done immediately instead of lazily. */
        memset(&g_tasks[i], 0, sizeof(g_tasks[i]));
        return 1;
    }
    return -1;
}

uint32_t sched_find_zombie_child(uint32_t ppid) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].state == TASK_ZOMBIE && g_tasks[i].ppid == ppid) {
            return g_tasks[i].pid;
        }
    }
    return 0;
}

bool sched_has_child(uint32_t ppid) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].state != 0 && g_tasks[i].ppid == ppid) {
            return true;
        }
    }
    return false;
}

bool sched_task_was_signaled(uint32_t pid) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].pid == pid) {
            return g_tasks[i].exit_signaled;
        }
    }
    return false;
}

static uint32_t g_shell_session_uid = 0;
static uint32_t g_shell_session_gid = 0;

void sched_set_session_ids(uint32_t uid, uint32_t gid) {
    g_shell_session_uid = uid;
    g_shell_session_gid = gid;
}

void sched_session_ids(uint32_t *uid, uint32_t *gid) {
    if (uid != NULL) {
        *uid = g_shell_session_uid;
    }
    if (gid != NULL) {
        *gid = g_shell_session_gid;
    }
}

bool sched_task_ids(uint32_t pid, uint32_t *uid, uint32_t *gid) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].pid == pid && g_tasks[i].state == TASK_ZOMBIE) {
            if (uid != NULL) {
                *uid = g_tasks[i].uid;
            }
            if (gid != NULL) {
                *gid = g_tasks[i].gid;
            }
            return true;
        }
    }
    return false;
}

void sched_mark_exit_signaled(void) {
    if (g_current != NULL) {
        g_current->exit_signaled = true;
    }
}

int sched_task_alive(uint32_t pid) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].pid == pid && g_tasks[i].state != TASK_ZOMBIE) {
            return 1;
        }
    }
    return 0;
}

struct task *sched_find_pid(uint32_t pid) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].pid == pid && g_tasks[i].state != 0 &&
            g_tasks[i].state != TASK_ZOMBIE) {
            return &g_tasks[i];
        }
    }
    return NULL;
}

/* The same cleanup task_exit() does for itself, aimed at a task from
 * the outside instead. */
static void task_kill_cleanup(struct task *t, int status) {
    kbd_input_release(t->pid);
    highx_client_exit(t->pid);
    term_client_exit(t->pid);
    vfs_close_all_table(t->fds);
    t->exit_status = status;
    /* Every path that reaches task_kill_cleanup() - task_kill()
     * itself (kill/pkill, always 128+SIGKILL by their own convention)
     * and sched_tick()'s pending_kill branch (Ctrl+C, SIGKILL via
     * sched_raise(), or a real signal's default terminate action) -
     * is a signal death, never a real exit(2). */
    t->exit_signaled = true;
}

int task_kill(uint32_t pid, int status) {
    if (g_current != NULL && pid == g_current->pid) {
        return -1; /* must go through task_exit() to switch away */
    }
    struct task *t = sched_find_pid(pid);
    if (t == NULL) {
        return -1;
    }

    /* Safe to clean up immediately: TUS is single-core and this code
     * IS the only thing running, so a task that is not g_current
     * cannot be mid-way through anything right now - every hlt()-based
     * wait it might be paused inside is a safe point to abandon it
     * at. */
    task_kill_cleanup(t, status);
    t->state = TASK_ZOMBIE;
    return 0;
}

/* A plain substring search: TUS's klib has no strstr, and pulling one
 * in for a nine-line shell command is not worth a new dependency. */
static const char *find_substr(const char *hay, const char *needle) {
    if (*needle == '\0') {
        return hay;
    }
    for (; *hay != '\0'; hay++) {
        const char *h = hay, *n = needle;
        while (*h != '\0' && *n != '\0' && *h == *n) {
            h++;
            n++;
        }
        if (*n == '\0') {
            return hay;
        }
    }
    return NULL;
}

int task_kill_by_name(const char *substr, int status) {
    int n = 0;
    for (int i = 0; i < TASK_MAX; i++) {
        struct task *t = &g_tasks[i];
        if (t->state == 0 || t->state == TASK_ZOMBIE) {
            continue;
        }
        if (g_current != NULL && t->pid == g_current->pid) {
            continue; /* pkill never takes out the shell running it */
        }
        if (find_substr(t->name, substr) == NULL) {
            continue;
        }
        if (task_kill(t->pid, status) == 0) {
            n++;
        }
    }
    return n;
}

/* See sched.h: what exec_pipeline() is currently waiting on, for
 * Ctrl+C to reach. */
static uint32_t g_foreground_pids[SCHED_MAX_FOREGROUND];
static int g_foreground_count;

void sched_set_foreground(const uint32_t *pids, int count) {
    if (count > SCHED_MAX_FOREGROUND) {
        count = SCHED_MAX_FOREGROUND;
    }
    for (int i = 0; i < count; i++) {
        g_foreground_pids[i] = pids[i];
    }
    g_foreground_count = count;
}

void sched_clear_foreground(void) {
    g_foreground_count = 0;
}

void sched_interrupt_foreground(void) {
    for (int i = 0; i < g_foreground_count; i++) {
        /* A real SIGINT now, via the same sched_raise() kill(2) uses -
         * a job with no handler installed still dies exactly as
         * before (SIG_DEF_TERM's default action is task_kill_cleanup()
         * with the same 128+SIGINT(2)=130 status, deferred to
         * sched_tick() the same "cannot switch itself away" way this
         * used to be done here directly), and a job that HAS
         * installed a real SIGINT handler (ksh's interactive shell
         * does, to abort the current line rather than die) now runs
         * it instead. No special case is needed here for the target
         * being g_current mid-instruction (a `ping` in a tight
         * send/sleep loop, say, is easily what Ctrl+C's key event
         * interrupts) - sched_raise() only ever sets a pending bit,
         * and sched_tick() already re-checks g_current's own pending
         * signals on every tick regardless of who last set them. */
        sched_raise(g_foreground_pids[i], 2); /* SIGINT */
    }
    g_foreground_count = 0;
}

void sched_stop_foreground(void) {
    for (int i = 0; i < g_foreground_count; i++) {
        sched_raise(g_foreground_pids[i], 20); /* SIGTSTP */
    }
    g_foreground_count = 0;
}

/* ---- real (POSIX) signal delivery ----
 *
 * TUS is single-core and cooperative between ticks, which makes this
 * far simpler than a preemptive-anywhere kernel needs: a signal never
 * has to interrupt a DIFFERENT task's live registers (fork()'s whole
 * problem) because delivery is deferred to the one moment a target
 * task's full register frame is already sitting on its kernel stack
 * for inspection anyway - sched_tick(), which runs on every 100 Hz
 * PIT tick for whichever task that tick caught, and additionally
 * whenever a task is about to be switched to. Worst-case delivery
 * latency is therefore one tick (10ms) - unnoticeable for `trap`,
 * SIGINT or SIGCHLD, and it means signal delivery needs no changes to
 * syscall_entry() at all (kept untouched for the same reason fork()
 * got its own IDT vector instead of widening it - see fork_entry()'s
 * own comment above). */

#define SIG_MASK(n) (1ULL << ((n) - 1))
#define SIGALRM_NUM 14

/* The action a signal takes when no handler is installed (SIG_DFL).
 * Matches POSIX's table for the classic 1-31 signal set. */
enum sig_default { SIG_DEF_IGNORE, SIG_DEF_STOP, SIG_DEF_CONTINUE, SIG_DEF_TERM };

static enum sig_default sig_default_action(int sig) {
    switch (sig) {
    case 17: /* SIGCHLD */
    case 23: /* SIGURG */
    case 28: /* SIGWINCH */
        return SIG_DEF_IGNORE;
    case 19: /* SIGSTOP */
    case 20: /* SIGTSTP */
    case 21: /* SIGTTIN */
    case 22: /* SIGTTOU */
        return SIG_DEF_STOP;
    case 18: /* SIGCONT */
        return SIG_DEF_CONTINUE;
    default:
        return SIG_DEF_TERM;
    }
}

/* Called from sched_tick() with the full 20-word frame (see
 * FRAME_WORDS above) of the ring-3 task about to run or keep running.
 * Finds at most one deliverable signal (pending and not blocked) and
 * either performs its default action or redirects execution into a
 * real handler - never both in the same call, so a task with several
 * signals pending gets them one per tick, which is plenty (see the
 * file comment above). */
static void sched_deliver_signal(uint64_t frame_rsp, bool ring3) {
    struct task *t = g_current;
    uint64_t deliverable = t->sig_pending & ~t->sig_blocked;
    if (deliverable == 0) {
        return;
    }
    int sig = 0;
    for (int i = 1; i < 32; i++) {
        if (deliverable & SIG_MASK(i)) {
            sig = i;
            break;
        }
    }
    if (sig == 0) {
        return;
    }

    uint64_t handler = t->sig_action[sig].handler;
    if (handler != 0 && handler != 1 && !ring3) {
        /* A real handler needs to redirect RIP/RSP for a clean
         * ring-3 iretq, but this tick caught the task mid-syscall
         * (still ring 0 - it has not returned to user code yet since
         * making the call). Leave it pending: kernel/vfs/devices.c's
         * kbd_get_event_owned_eintr() and sys_waitpid()'s own
         * pending-signal check (kernel/syscall/syscall.c) are what
         * get a BLOCKED syscall to actually return control to ring 3
         * in the first place - once it does, the very next tick finds
         * this same pending bit with ring3 now true and delivers it
         * for real. Default actions (below) do not have this
         * problem - task_kill()/pending_kill already terminate a task
         * stuck anywhere, ring irrelevant, exactly like Ctrl+C always
         * has. */
        return;
    }
    if (handler == 0) { /* SIG_DFL */
        t->sig_pending &= ~SIG_MASK(sig);
        switch (sig_default_action(sig)) {
        case SIG_DEF_TERM:
            /* Deferred exactly like Ctrl+C's pending_kill: this task
             * IS g_current right now, so it cannot switch itself
             * away - sched_tick() finishes the job immediately below,
             * in the same tick. */
            t->pending_kill = true;
            t->pending_kill_status = 128 + sig;
            break;
        case SIG_DEF_STOP:
            t->pending_stop = true;
            break;
        case SIG_DEF_IGNORE:
        case SIG_DEF_CONTINUE:
            break; /* nothing to do: already running */
        }
        return;
    }
    if (handler == 1) { /* SIG_IGN */
        t->sig_pending &= ~SIG_MASK(sig);
        return;
    }

    /* Redirect into the real handler. Only RAX/RIP/RSP/RFLAGS and the
     * blocked mask need saving for sched_sigreturn() to undo later -
     * rbx/rbp/r12-r15 are untouched by any of this (the handler is
     * ordinary ABI-compliant C code, so it preserves whatever
     * callee-saved registers it uses on its own, exactly as a normal
     * call would), and rdi/rsi/rdx/r10/r8/r9 need no preservation
     * either - already documented as clobbered by any TUS syscall
     * (see tus_syscall.c), and a signal arriving is indistinguishable
     * from that at this level. */
    uint64_t *f = (uint64_t *)(uintptr_t)frame_rsp;
    t->sig_saved_rax = f[14];
    t->sig_saved_rip = f[15];
    t->sig_saved_rflags = f[17];
    t->sig_saved_rsp = f[18];
    t->sig_saved_blocked = t->sig_blocked;

    /* The handler runs on the same user stack, just below wherever
     * execution was interrupted. Align to 16 first - an asynchronous
     * interruption can land at any alignment, unlike a `call`-entered
     * function - then push ONE return address: musl's restorer (see
     * arch/x86_64/src/signal/restore.s), so the handler's own `ret`
     * lands on sigreturn_entry's "int $0x82" trampoline instead of
     * anywhere the interrupted program owns. We are still running in
     * this task's own address space (it is the one about to resume),
     * so this is a plain pointer write, not a cross-space copy. */
    uint64_t new_rsp = (f[18] & ~0xFULL) - 8;
    *(uint64_t *)(uintptr_t)new_rsp = t->sig_action[sig].restorer;

    f[10] = (uint64_t)sig; /* rdi: the handler's one argument */
    f[15] = handler;       /* rip */
    f[18] = new_rsp;       /* rsp */

    /* Block everything for the duration of the handler (restored by
     * sched_sigreturn()). POSIX allows a second unmasked signal to
     * interrupt a running handler; this is a deliberately simpler,
     * safer rule - there is only ONE saved-context slot per task (see
     * struct task), so nesting would corrupt it - and it is enough
     * for ksh's trap/SIGINT/SIGCHLD use. */
    t->sig_blocked = ~0ULL;
    t->sig_pending &= ~SIG_MASK(sig);
}

/* True while the CURRENT task has a deliverable (pending, unblocked)
 * signal it has not yet received. This is what actually gets a
 * multi-tick blocking wait (kernel/vfs/devices.c's kbd_get_event_
 * owned_eintr(), sys_waitpid() in kernel/syscall/syscall.c) to return
 * control to ring 3 in the first place: sched_deliver_signal() only
 * ever runs from sched_tick(), and a task stuck deep inside a
 * blocking syscall is caught by every tick with CS still ring 0 (it
 * never returned to user code), so the frame-redirect delivery path
 * cannot fire yet (see sched_deliver_signal()'s own comment) - the
 * blocking loop noticing this directly and bailing with -EINTR is
 * what lets the NEXT tick, with the task genuinely back in ring 3,
 * actually deliver it. SA_RESTART is intentionally not consulted
 * here - every blocking wait in TUS returns -EINTR unconditionally
 * once something is pending, which is always a POSIX-legal answer,
 * simpler than re-driving an in-kernel wait loop after the fact. */
bool sched_signal_pending(void) {
    return g_current != NULL &&
          (g_current->sig_pending & ~g_current->sig_blocked) != 0;
}

int sched_raise(uint32_t pid, int sig) {
    if (sig < 1 || sig > 31) {
        return -EINVAL;
    }
    struct task *t = sched_find_pid(pid);
    if (t == NULL) {
        return -ESRCH;
    }

    if (sig == 9) { /* SIGKILL: unblockable, uncatchable, immediate -
                      * no reason to wait for a tick. */
        if (g_current != NULL && t->pid == g_current->pid) {
            g_current->pending_kill = true;
            g_current->pending_kill_status = 128 + 9;
        } else {
            task_kill(t->pid, 128 + 9);
        }
        return 0;
    }

    if (sig == 18 && t->state == TASK_STOPPED) {
        /* SIGCONT on a stopped task: wake it immediately. A stopped
         * task is not scheduled, so it would never reach
         * sched_deliver_signal() on its own to notice this. Still
         * recorded as pending too, below - POSIX says SIGCONT is also
         * catchable, and a task may have installed a real handler for
         * "I was just resumed" bookkeeping (ksh's job control does). */
        t->state = TASK_READY;
    }

    t->sig_pending |= SIG_MASK(sig);
    return 0;
}

/* killpg(2)/kill(2) with pid <= 0: raise `sig` on every live task in
 * process group `pgid` (plain table scan, the same shape
 * task_kill_by_name() already uses for pkill). Returns the number of
 * tasks signalled. */
int sched_raise_pgid(uint32_t pgid, int sig) {
    int n = 0;
    for (int i = 0; i < TASK_MAX; i++) {
        struct task *t = &g_tasks[i];
        if (t->state == 0 || t->state == TASK_ZOMBIE) {
            continue;
        }
        if (t->pgid != pgid) {
            continue;
        }
        if (sched_raise(t->pid, sig) == 0) {
            n++;
        }
    }
    return n;
}

void task_list_all(void) {
    for (int i = 0; i < TASK_MAX; i++) {
        struct task *t = &g_tasks[i];
        if (t->state == 0) {
            continue;
        }
        const char *state = t->state == TASK_RUNNING ? "running"
                          : t->state == TASK_READY  ? "ready"
                          : "zombie";
        kprintf("%-4u %-8s %-10llx %s\n", t->pid, state,
                (unsigned long long)t->cr3, t->name);
    }
}

int task_snapshot(struct tus_procinfo *out, int max) {
    int n = 0;
    for (int i = 0; i < TASK_MAX && n < max; i++) {
        struct task *t = &g_tasks[i];
        if (t->state == 0) {
            continue;
        }
        out[n].pid = t->pid;
        out[n].ppid = t->ppid;
        out[n].pgid = t->pgid;
        out[n].uid = t->uid;
        out[n].state = t->state;
        strncpy(out[n].name, t->name, TASK_NAME_MAX - 1);
        out[n].name[TASK_NAME_MAX - 1] = '\0';
        n++;
    }
    return n;
}

void sched_init(void) {
    g_current = task_find_slot();
    g_current->pid = g_next_pid++;
    g_current->state = TASK_RUNNING;
    strcpy(g_current->name, "tsh");
    /* Task 0 is the kernel shell: it keeps the boot address space. */
    g_current->cr3 = vmm_root_cr3();
    g_current->fs_base = 0;
    g_current->mmap_cur = MMAP_CURSOR_START;
    /* Task 0 is its own process group and session leader - every
     * other task's pgid/sid traces back to this by inheritance. */
    g_current->ppid = 0;
    g_current->pgid = g_current->pid;
    g_current->sid = g_current->pid;
    g_current->cwd[0] = '/';
    g_current->cwd[1] = '\0';
    /* Task 0 runs on the boot stack; give it a real kernel stack so
     * TSS.RSP0 is always valid. */
    g_current->kstack = (uint64_t)(uintptr_t)kmalloc(STACK_SIZE);
    g_current->kstack_top = g_current->kstack + STACK_SIZE;
    g_current->rsp = 0;
    /* Capture the boot CPU's FPU state so switching back to task 0
     * restores exactly what the shell left behind. */
    __asm__ volatile("fxsave (%0)" : : "r"(g_current->fpu) : "memory");
    wrmsr(MSR_FS_BASE, 0);
    tss_set_rsp0(g_current->kstack_top);
}

int task_create_user(uint64_t entry, const char *name, uint64_t cr3,
                     int argc, char **argv, uint32_t uid, uint32_t gid,
                     uint32_t euid, uint32_t egid) {
    struct task *t = task_find_slot();
    if (t == NULL) {
        return -1;
    }

    uint8_t *kstack = kmalloc(STACK_SIZE);
    if (kstack == NULL) {
        return -1;
    }

    /* Map a fresh user stack inside the task's private address space:
     * ring 3 needs USER pages there. The kernel heap (where this
     * code runs) is supervisor-only. Frames are zeroed via the HHDM
     * mapping: the C library's startup code expects a clean stack. */
    uint64_t ustack = USER_STACK_BASE;
    uint64_t pages = USER_STACK_SIZE / 4096;
    for (uint64_t i = 0; i < pages; i++) {
        uint64_t frame = pmm_alloc_frame();
        if (frame == 0) {
            return -1;
        }
        memset((void *)pmm_phys_to_virt(frame), 0, 4096);
        /* Stacks are data, never code: NX closes the classic
         * stack-smashing-to-shellcode path even if a future bug lets
         * an overflow overwrite the return address. */
        if (vmm_map_page_in(cr3, ustack + i * 4096, frame,
                            VMM_PRESENT | VMM_WRITE | VMM_USER | VMM_NX) != 0) {
            return -1;
        }
    }

    /* Initial user stack image, laid out the way a C runtime expects
     * it (crt1 reads argc at (%rsp), argv after it, then a NULL
     * envp, then the auxiliary vector):
     *   [0] argc
     *   [1..argc] argv[0..argc-1] pointers
     *   [argc+1] NULL (argv terminator)
     *   [argc+2] envp[0] = NULL
     *   [argc+3] auxv[0] = AT_PAGESZ
     *   [argc+4] auxv[1] = 4096
     *   [argc+5] auxv[2] = 0 (terminator)
     * RSP points at word [0]. The argument strings are copied just
     * below the pointer array (still inside the top stack page) and
     * the pointers are written as user virtual addresses, since the
     * kernel heap is not readable from ring 3. All writes go through
     * the HHDM mapping (the caller may not be in this space). */
    int nargs = argc + 1;                 /* program name + arguments */
    int nwords = nargs + 6;               /* argc + argv[] + NULL +
                                             envp[0] + 2 auxv + term */
    if (nargs > TASK_MAX_ARGS) {
        nargs = TASK_MAX_ARGS;
        nwords = nargs + 6;
    }
    uint64_t top_frame = vmm_translate_in(cr3, ustack + USER_STACK_SIZE - 4096);
    if (top_frame == 0) {
        return -1;
    }
    char *frame_base = (char *)pmm_phys_to_virt(top_frame);

    /* Argument strings live at the VERY TOP of the top page, growing
     * downward; the pointer array sits below them and the initial RSP
     * below that. The stack grows down from RSP, so it can never
     * reach the strings - with the strings at the BOTTOM of the page
     * (older layout) a program using more than ~4 KiB of stack
     * silently corrupted argv. All writes go through the HHDM
     * mapping (the caller may not be in this space); the pointers
     * stored in the array are user virtual addresses. */
    const char *args[TASK_MAX_ARGS];
    args[0] = name != NULL ? name : "";
    for (int i = 1; i < nargs; i++) {
        args[i] = (argv != NULL && argv[i - 1] != NULL) ? argv[i - 1] : "";
    }
    size_t need = 8 + (size_t)nwords * 8;
    for (int i = 0; i < nargs; i++) {
        need += strlen(args[i]) + 1;
    }
    if (need > 4096) {
        return -1;
    }

    char *sptr = frame_base + 4096;
    uint64_t arg_ptrs[TASK_MAX_ARGS];
    for (int i = 0; i < nargs; i++) {
        size_t slen = strlen(args[i]);
        sptr -= slen + 1;
        memcpy(sptr, args[i], slen + 1);
        arg_ptrs[i] = (uint64_t)(ustack + USER_STACK_SIZE - 4096) +
                      (uint64_t)(sptr - frame_base);
    }

    uint64_t *init = (uint64_t *)((uintptr_t)sptr & ~(uintptr_t)7);
    init -= nwords;
    init[0] = (uint64_t)nargs;   /* argc */
    for (int i = 0; i < nargs; i++) {
        init[1 + i] = arg_ptrs[i];
    }
    init[1 + nargs] = 0;        /* argv terminator */
    init[2 + nargs] = 0;        /* envp[0] */
    init[3 + nargs] = AT_PAGESZ;
    init[4 + nargs] = 4096;     /* page size */
    init[5 + nargs] = 0;        /* auxv terminator */
    uint64_t ustack_rsp = (uint64_t)(ustack + USER_STACK_SIZE - 4096) +
                          (uint64_t)((char *)init - frame_base);

    t->pid = g_next_pid++;
    t->state = TASK_READY;
    strncpy(t->name, name, TASK_NAME_MAX - 1);
    t->name[TASK_NAME_MAX - 1] = '\0';

    t->kstack = (uint64_t)(uintptr_t)kstack;
    t->kstack_top = t->kstack + STACK_SIZE;
    t->ustack = ustack;
    t->ustack_top = ustack + USER_STACK_SIZE;
    t->cr3 = cr3;
    t->fs_base = 0;
    t->mmap_cur = MMAP_CURSOR_START;
    t->uid = uid;
    t->euid = euid;
    t->gid = gid;
    t->egid = egid;
    t->caps = 0;

    /* Process identity and working directory are inherited from the
     * spawning task, exactly like a real fork()+exec() would leave
     * them - SYS_SPAWN has no fork() of its own, so this is where a
     * child's ppid/pgid/sid/cwd actually come from. */
    if (g_current != NULL) {
        t->ppid = g_current->pid;
        t->pgid = g_current->pgid;
        t->sid = g_current->sid;
        strncpy(t->cwd, g_current->cwd, TASK_CWD_MAX - 1);
        t->cwd[TASK_CWD_MAX - 1] = '\0';
    } else {
        t->ppid = 0;
        t->pgid = t->pid;
        t->sid = t->pid;
        t->cwd[0] = '/';
        t->cwd[1] = '\0';
    }

    /* Inherit the creator's open files: the child runs with the same
     * fds the shell set up (redirection/pipes) and the shell can
     * restore its own slots immediately after spawning. */
    vfs_fd_inherit(t->fds, g_current->fds);
    /* ...and which of those the creator had marked close-on-exec
     * (fcntl F_SETFD/FD_CLOEXEC) - a per-fd-number flag, not part of
     * the vfs_file vfs_fd_inherit just copied pointers to. */
    memcpy(t->fd_cloexec, g_current->fd_cloexec, sizeof(t->fd_cloexec));

    /* ... and its terminal session, so a program started from a
     * terminal window prints into that window and reads its keys.
     * The program itself knows nothing about it. */
    t->term = g_current != NULL ? g_current->term : NULL;

    /* Default FPU state (hardware reset values): x87 control word
     * with all exceptions masked, empty x87 tag word, MXCSR with all
     * exceptions masked. fxrstor of a fully zeroed image would leave
     * MXCSR unmasked (NaN operations would raise #XM). */
    memset(t->fpu, 0, sizeof(t->fpu));
    *(uint16_t *)(t->fpu + 0) = 0x037F;  /* x87 CW */
    *(uint16_t *)(t->fpu + 4) = 0xFFFF;  /* x87 FTW (all empty) */
    *(uint32_t *)(t->fpu + 24) = 0x1F80; /* MXCSR */
    *(uint32_t *)(t->fpu + 28) = 0xFFFF; /* MXCSR_MASK */

    /* Build the fake interrupt frame at the top of the kernel stack.
     * Layout (lowest first, matching what the CPU pushes on a
     * ring-3->ring-0 interrupt, plus the 15 registers the stub
     * pushes):
     *   [r15 r14 r13 r12 rbp rbx r11 r10 r9 r8 rdi rsi rdx rcx rax]
     *   [rip cs rflags rsp ss]
     * The stub pops the 15 registers, IRETQ consumes the rest and
     * the task starts at `entry` in ring 3. */
    uint64_t *f = (uint64_t *)(uintptr_t)t->kstack_top;
    f[-1]  = SEL_USER_DATA;   /* ss  */
    f[-2]  = ustack_rsp;      /* rsp: points at the init stack image */
    /* rflags: IF set + IOPL=3. IRETQ to a lower privilege level
     * requires IOPL >= new CPL (else #GP); user code here runs with
     * I/O privileges for now, tightened when the syscall gate is the
     * only hardware entry point. */
    f[-3]  = 0x3202;          /* rflags: IF, IOPL=3, reserved bit 1 */
    f[-4]  = SEL_USER_CODE;   /* cs  */
    f[-5]  = entry;           /* rip */
    for (int i = 6; i <= FRAME_WORDS; i++) {
        f[-i] = 0;            /* registers: don't care for a fresh task */
    }
    t->rsp = t->kstack_top - FRAME_WORDS * 8;

    return (int)t->pid;
}

/*
 * A ring-0 task. Everything a user task needs from its own address
 * space - a user stack, an argument image, a private CR3 - a kernel
 * task already has: it runs in the kernel's space on its kernel
 * stack. All that is left is the frame the first IRETQ consumes, with
 * the kernel selectors instead of the user ones and `arg` parked in
 * RDI, which is where the stub's `pop %rdi` will find it.
 */
int task_create_kernel(void (*entry)(void *), void *arg, const char *name) {
    struct task *t = task_find_slot();
    if (t == NULL) {
        return -1;
    }

    uint8_t *kstack = kmalloc(STACK_SIZE);
    if (kstack == NULL) {
        return -1;
    }

    t->pid = g_next_pid++;
    t->state = TASK_READY;
    strncpy(t->name, name, TASK_NAME_MAX - 1);
    t->name[TASK_NAME_MAX - 1] = '\0';

    t->kstack = (uint64_t)(uintptr_t)kstack;
    t->kstack_top = t->kstack + STACK_SIZE;
    t->ustack = 0;
    t->ustack_top = 0;
    t->cr3 = vmm_root_cr3();
    t->fs_base = 0;
    t->mmap_cur = MMAP_CURSOR_START;
    t->uid = t->euid = t->gid = t->egid = 0;
    t->caps = 0;
    t->term = NULL;

    /* A terminal session's shell is still a real task in the table
     * (ps/kill see it); it inherits identity the same way a spawned
     * one does. */
    if (g_current != NULL) {
        t->ppid = g_current->pid;
        t->pgid = g_current->pgid;
        t->sid = g_current->sid;
        strncpy(t->cwd, g_current->cwd, TASK_CWD_MAX - 1);
        t->cwd[TASK_CWD_MAX - 1] = '\0';
    } else {
        t->ppid = 0;
        t->pgid = t->pid;
        t->sid = t->pid;
        t->cwd[0] = '/';
        t->cwd[1] = '\0';
    }

    /* A fresh table: the shell opens its own stdin/stdout/stderr on
     * /dev/tty0, which is what routes them into its session. */
    for (int i = 0; i < VFS_MAX_FDS; i++) {
        t->fds[i] = NULL;
    }

    memset(t->fpu, 0, sizeof(t->fpu));
    *(uint16_t *)(t->fpu + 0) = 0x037F;  /* x87 CW */
    *(uint16_t *)(t->fpu + 4) = 0xFFFF;  /* x87 FTW (all empty) */
    *(uint32_t *)(t->fpu + 24) = 0x1F80; /* MXCSR */
    *(uint32_t *)(t->fpu + 28) = 0xFFFF; /* MXCSR_MASK */

    /* The frame the stub pops, top word first: ss, rsp, rflags, cs,
     * rip, then the 15 registers (rax rcx rdx rsi rdi ...). The task
     * starts with the whole stack below the frame free: IRETQ has
     * consumed the frame by the time the first instruction runs.
     * RSP is left 8 modulo 16, which is what a function entered by
     * `call` sees, so the SysV alignment rules hold. */
    uint64_t *f = (uint64_t *)(uintptr_t)t->kstack_top;
    f[-1] = SEL_KERNEL_DATA;      /* ss */
    f[-2] = t->kstack_top - 8;    /* rsp */
    f[-3] = 0x202;                /* rflags: IF + reserved bit 1 */
    f[-4] = SEL_KERNEL_CODE;      /* cs */
    f[-5] = (uint64_t)(uintptr_t)entry; /* rip */
    for (int i = 6; i <= FRAME_WORDS; i++) {
        f[-i] = 0;
    }
    f[-10] = (uint64_t)(uintptr_t)arg;  /* rdi: the entry's argument */
    t->rsp = t->kstack_top - FRAME_WORDS * 8;

    return (int)t->pid;
}

int task_set_term(uint32_t pid, struct tsh_term *term) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (g_tasks[i].pid == pid && g_tasks[i].state != 0) {
            g_tasks[i].term = term;
            return 0;
        }
    }
    return -1;
}

/*
 * IRQ0 entry stub. Saves ALL general-purpose registers (caller- and
 * callee-saved), calls sched_tick(), and either resumes the current
 * task or switches RSP to the next task's frame and returns through
 * it. Saving the callee-saved registers is essential: the interrupted
 * kernel code may be mid-computation (e.g. fb_putchar's scroll) and
 * relies on rbx/rbp/r12-r15 surviving a detour through a ring-3 task.
 */
__attribute__((naked)) void sched_tick_entry(void) {
    __asm__ volatile(
        "push %rax\n\t"
        "push %rcx\n\t"
        "push %rdx\n\t"
        "push %rsi\n\t"
        "push %rdi\n\t"
        "push %r8\n\t"
        "push %r9\n\t"
        "push %r10\n\t"
        "push %r11\n\t"
        "push %rbx\n\t"
        "push %rbp\n\t"
        "push %r12\n\t"
        "push %r13\n\t"
        "push %r14\n\t"
        "push %r15\n\t"
        "mov %rsp, %rdi\n\t"
        "call sched_tick\n\t"
        "test %rax, %rax\n\t"
        "jz 1f\n\t"
        "mov %rax, %rsp\n\t"
        "1:\n\t"
        "pop %r15\n\t"
        "pop %r14\n\t"
        "pop %r13\n\t"
        "pop %r12\n\t"
        "pop %rbp\n\t"
        "pop %rbx\n\t"
        "pop %r11\n\t"
        "pop %r10\n\t"
        "pop %r9\n\t"
        "pop %r8\n\t"
        "pop %rdi\n\t"
        "pop %rsi\n\t"
        "pop %rdx\n\t"
        "pop %rcx\n\t"
        "pop %rax\n\t"
        "iretq\n\t");
}

void preempt_disable(void) {
    g_preempt_depth++;
}

void preempt_enable(void) {
    if (g_preempt_depth > 0) {
        g_preempt_depth--;
    }
}

uint64_t sched_tick(uint64_t frame_rsp) {
    /* Acknowledge the PIT interrupt. */
    pic_send_eoi(0);
    pit_tick();

    if (g_current == NULL || g_current->state == TASK_ZOMBIE) {
        return 0; /* nothing sensible to do; stay put */
    }

    /* Preempt unless we are inside a critical section (kprintf,
     * syscall dispatch). The interrupted task is resumed later via
     * its saved frame, so switching at any non-critical point is
     * safe - including the shell's hlt() idle wait, which is exactly
     * when user tasks must get CPU time. */
    if (g_preempt_depth > 0) {
        return 0; /* critical section: no preemption */
    }

    if (!g_current->pending_kill) {
        /* alarm(): fires as a real SIGALRM, picked up by the delivery
         * check right below - the same "checked at the one place
         * ticks already happen" shape pending_kill itself uses. */
        if (g_current->alarm_deadline_ms != 0 &&
            pit_uptime_ms() >= g_current->alarm_deadline_ms) {
            g_current->alarm_deadline_ms = 0;
            g_current->sig_pending |= SIG_MASK(SIGALRM_NUM);
        }

        /* Real POSIX signal delivery only applies to a task that CAN
         * run in ring 3 - a ring-0-only one (tsh) has no sigaction()
         * of its own and uses task_kill()/pending_kill directly.
         * ustack is 0 exactly for those (task_create_kernel never
         * sets one) and nonzero for everything task_create_user()/
         * sched_fork() create - checking ustack rather than the
         * interrupted frame's CURRENT ring matters because a task
         * deep inside a blocking syscall is caught by every tick
         * still in ring 0 (see sched_deliver_signal()'s own ring3
         * parameter): it still needs default actions (terminate/
         * stop) to apply immediately, only the real-handler redirect
         * has to wait for an actual ring-3 tick. */
        if (g_current->ustack != 0) {
            uint64_t *chk = (uint64_t *)(uintptr_t)frame_rsp;
            bool ring3 = (chk[16] & 3) == 3;
            sched_deliver_signal(frame_rsp, ring3);
        }
    }

    if (g_current->pending_kill) {
        /* Ctrl+C (sched_interrupt_foreground(), same file) asked to
         * kill the task that was actually running when its request
         * arrived - task_kill() cannot do that safely on its own, it
         * has no way to switch away from a context it is not running
         * in, so it deferred here, which already IS where tasks get
         * switched. Clean up and discard this task's state instead of
         * the usual save-and-resume-later below: there is no "later"
         * for it. */
        task_kill_cleanup(g_current, g_current->pending_kill_status);
        g_current->pending_kill = false;
        g_current->state = TASK_ZOMBIE;

        struct task *next = task_next(g_current);
        if (next == g_current) {
            /* No other runnable task. TUS always has tsh as task 0,
             * so this should not happen - but resuming a zombie's own
             * stack would be worse than doing nothing. */
            return 0;
        }
        next->state = TASK_RUNNING;
        tss_set_rsp0(next->kstack_top);
        vmm_space_switch(next->cr3);
        __asm__ volatile("fxrstor (%0)" : : "r"(next->fpu) : "memory");
        wrmsr(MSR_FS_BASE, next->fs_base);
        g_current = next;
        return next->rsp;
    }

    if (g_current->pending_stop) {
        /* SIGSTOP/SIGTSTP's default action: same deferred-switch
         * shape as pending_kill above (a task cannot move itself out
         * of TASK_RUNNING from the outside), but the task survives -
         * sched_raise()'s SIGCONT case (kernel/sched/sched.c) is what
         * brings it back, by setting it TASK_READY again; nothing
         * special is needed to resume it from there, since that is
         * exactly what task_next() already knows how to schedule. */
        g_current->pending_stop = false;
        g_current->rsp = frame_rsp;
        g_current->state = TASK_STOPPED;

        struct task *next = task_next(g_current);
        if (next == g_current) {
            return 0; /* should not happen: tsh is always runnable */
        }
        next->state = TASK_RUNNING;
        tss_set_rsp0(next->kstack_top);
        vmm_space_switch(next->cr3);
        __asm__ volatile("fxrstor (%0)" : : "r"(next->fpu) : "memory");
        wrmsr(MSR_FS_BASE, next->fs_base);
        g_current = next;
        return next->rsp;
    }

    g_current->rsp = frame_rsp;
    struct task *next = task_next(g_current);
    if (next == g_current) {
        return 0; /* only one runnable task: no switch */
    }

    g_current->state = TASK_READY;
    next->state = TASK_RUNNING;
    tss_set_rsp0(next->kstack_top);
    /* Save the outgoing task's FPU state before it is switched away;
     * restore the incoming task's state after the address-space
     * switch. Same for the FS base (thread pointer / TLS). */
    __asm__ volatile("fxsave (%0)" : : "r"(g_current->fpu) : "memory");
    /* Load the next task's address space. This is safe here: the
     * kernel stack and all kernel data are mapped in every space via
     * the shared kernel half. The CR3 reload also flushes the TLB. */
    vmm_space_switch(next->cr3);
    __asm__ volatile("fxrstor (%0)" : : "r"(next->fpu) : "memory");
    wrmsr(MSR_FS_BASE, next->fs_base);
    g_current = next;
    return next->rsp;
}

/*
 * fork()'s own gate (int 0x81, kernel/arch/x86_64/idt.c). Structurally
 * a copy of sched_tick_entry: push the same 15 general-purpose
 * registers in the same order, hand the stub's %rsp to a C function.
 *
 * The two entry points exist for the same reason and use the same
 * frame - sched_tick's needs syscall_entry() (kernel/syscall/
 * syscall.c) cannot meet. syscall_entry only pushes the 7 registers
 * a normal syscall's arguments and dispatch actually need; rbx/rbp/
 * r12-r15 are not among them, and are not safely readable from C
 * afterward either - callee-saved only means preserved *across* a
 * call, not untouched *within* one, so by the time any C function
 * runs, the compiler may already have repurposed them for its own
 * locals. fork() needs the user's true values for all of them (the
 * child resumes with the same registers the parent had, apart from
 * the syscall's own return value), so it gets its own wider stub
 * instead of widening the one every other syscall shares.
 *
 * Unlike sched_tick_entry, this tail never resumes anything but the
 * frame it just captured - it is only ever the parent's own return
 * path. The child is a brand new TASK_READY task by the time this
 * returns; the ordinary, unmodified sched_tick_entry resumes it later,
 * from the copy of this same frame sched_fork() built for it.
 */
__attribute__((naked)) void fork_entry(void) {
    __asm__ volatile(
        "push %rax\n\t"
        "push %rcx\n\t"
        "push %rdx\n\t"
        "push %rsi\n\t"
        "push %rdi\n\t"
        "push %r8\n\t"
        "push %r9\n\t"
        "push %r10\n\t"
        "push %r11\n\t"
        "push %rbx\n\t"
        "push %rbp\n\t"
        "push %r12\n\t"
        "push %r13\n\t"
        "push %r14\n\t"
        "push %r15\n\t"
        "mov %rsp, %rdi\n\t"
        "call sched_fork\n\t"
        "mov %rax, 112(%rsp)\n\t"  /* overwrite the saved rax with the
                                    * return value: child pid, or a
                                    * negative errno on failure */
        "pop %r15\n\t"
        "pop %r14\n\t"
        "pop %r13\n\t"
        "pop %r12\n\t"
        "pop %rbp\n\t"
        "pop %rbx\n\t"
        "pop %r11\n\t"
        "pop %r10\n\t"
        "pop %r9\n\t"
        "pop %r8\n\t"
        "pop %rdi\n\t"
        "pop %rsi\n\t"
        "pop %rdx\n\t"
        "pop %rcx\n\t"
        "pop %rax\n\t"
        "iretq\n\t");
}

/* frame_rsp: the 20 words fork_entry just captured - the 15 pushed
 * registers followed by the 5 the CPU itself pushed for the ring3->
 * ring0 trap (RIP, CS, RFLAGS, RSP, SS), all contiguous on the
 * parent's kernel stack. Building the child's initial frame is then
 * a plain copy of those 20 words onto its own kernel stack, at the
 * same offset from its own kstack_top that task_create_user() and
 * sched_tick() already use (FRAME_WORDS, above) - patching only the
 * rax slot (index 14: 15 registers pushed rax first, so it sits
 * furthest from frame_rsp - matches the 112-byte offset fork_entry's
 * own stub uses) to 0, the child's fork() return value. Nothing else
 * needs to be named or reinterpreted; the copy IS a resumable task,
 * because this is exactly the shape sched_tick_entry already knows
 * how to resume. */
long sched_fork(uint64_t frame_rsp) {
    struct task *parent = g_current;
    if (parent == NULL) {
        return -ENOMEM;
    }

    preempt_disable();

    struct task *t = task_find_slot();
    if (t == NULL) {
        preempt_enable();
        return -ENOMEM;
    }
    uint8_t *kstack = kmalloc(STACK_SIZE);
    if (kstack == NULL) {
        preempt_enable();
        return -ENOMEM;
    }
    uint64_t new_cr3 = vmm_space_fork(parent->cr3);
    if (new_cr3 == 0) {
        preempt_enable();
        return -ENOMEM;
    }

    t->pid = g_next_pid++;
    t->state = TASK_READY;
    strncpy(t->name, parent->name, TASK_NAME_MAX - 1);
    t->name[TASK_NAME_MAX - 1] = '\0';

    t->kstack = (uint64_t)(uintptr_t)kstack;
    t->kstack_top = t->kstack + STACK_SIZE;
    t->ustack = parent->ustack;
    t->ustack_top = parent->ustack_top;
    t->cr3 = new_cr3;
    t->fs_base = parent->fs_base;
    t->mmap_cur = parent->mmap_cur;
    t->uid = parent->uid;
    t->euid = parent->euid;
    t->gid = parent->gid;
    t->egid = parent->egid;
    t->caps = parent->caps;
    t->term = parent->term;
    t->pending_kill = false;

    /* Real fork(): ppid is the caller, pgid/sid and the working
     * directory carry over unchanged (setpgid()/setsid()/chdir() are
     * what a shell calls afterward to change them, in the child only,
     * same as every real fork()). */
    t->ppid = parent->pid;
    t->pgid = parent->pgid;
    t->sid = parent->sid;
    memcpy(t->cwd, parent->cwd, TASK_CWD_MAX);

    /* POSIX fork(): signal dispositions and the blocked mask are
     * inherited exactly; pending signals are not (nothing is pending
     * "for" the child yet - it hasn't existed long enough to have
     * anything delivered to it). */
    memcpy(t->sig_action, parent->sig_action, sizeof(t->sig_action));
    t->sig_blocked = parent->sig_blocked;

    /* Same refcounted-sharing semantics fork() needs (shared open
     * file description, shared offset, independent fd numbers) -
     * already exactly what SYS_SPAWN uses this for. */
    vfs_fd_inherit(t->fds, parent->fds);
    /* fork()'s child keeps the parent's close-on-exec bits per fd
     * number (POSIX) - vfs_fd_inherit only copied the shared
     * vfs_file pointers, not this task-level array. */
    memcpy(t->fd_cloexec, parent->fd_cloexec, sizeof(t->fd_cloexec));

    /* The parent is the one currently running, so its FPU/SSE state
     * lives in the real registers right now, not in parent->fpu
     * (which only gets fxsave'd into when the parent is switched
     * away) - save it directly into the child's own copy. */
    __asm__ volatile("fxsave (%0)" : : "r"(t->fpu) : "memory");

    uint64_t *src = (uint64_t *)(uintptr_t)frame_rsp;
    uint64_t *dst = (uint64_t *)(uintptr_t)(t->kstack_top - FRAME_WORDS * 8);
    memcpy(dst, src, FRAME_WORDS * 8);
    dst[14] = 0; /* child's rax: fork() returns 0 in the child */
    t->rsp = t->kstack_top - FRAME_WORDS * 8;

    preempt_enable();
    return (long)t->pid;
}

/*
 * sigreturn's own gate (int 0x82, kernel/arch/x86_64/idt.c). A signal
 * handler's `ret` lands here (musl's restorer, arch/x86_64/src/
 * signal/restore.s, is what sched_deliver_signal() above pushed as
 * its return address) - structurally fork_entry()'s naked-stub-plus-
 * full-frame pattern, but in reverse: instead of copying a captured
 * frame INTO a new task, sched_sigreturn() overwrites the CURRENT
 * frame with the pre-signal context sched_deliver_signal() saved,
 * so the pop-and-iretq tail below resumes exactly where the signal
 * interrupted, as if it had never arrived.
 *
 * Like fork(), this needs the full 15-register capture: the trap can
 * land here from anywhere, and syscall_entry()'s narrow 7-register
 * gate is not wide enough to safely read or restore the rest (see
 * fork_entry()'s own comment above for why that gate stays
 * untouched). Unlike fork_entry(), nothing here needs injecting into
 * the saved rax slot - sched_sigreturn() writes every field it
 * restores directly into the frame in place.
 */
__attribute__((naked)) void sigreturn_entry(void) {
    __asm__ volatile(
        "push %rax\n\t"
        "push %rcx\n\t"
        "push %rdx\n\t"
        "push %rsi\n\t"
        "push %rdi\n\t"
        "push %r8\n\t"
        "push %r9\n\t"
        "push %r10\n\t"
        "push %r11\n\t"
        "push %rbx\n\t"
        "push %rbp\n\t"
        "push %r12\n\t"
        "push %r13\n\t"
        "push %r14\n\t"
        "push %r15\n\t"
        "mov %rsp, %rdi\n\t"
        "call sched_sigreturn\n\t"
        "pop %r15\n\t"
        "pop %r14\n\t"
        "pop %r13\n\t"
        "pop %r12\n\t"
        "pop %rbp\n\t"
        "pop %rbx\n\t"
        "pop %r11\n\t"
        "pop %r10\n\t"
        "pop %r9\n\t"
        "pop %r8\n\t"
        "pop %rdi\n\t"
        "pop %rsi\n\t"
        "pop %rdx\n\t"
        "pop %rcx\n\t"
        "pop %rax\n\t"
        "iretq\n\t");
}

/* frame_rsp: the same 20-word layout fork()/sched_tick() use (see
 * FRAME_WORDS above) - the handler's own captured state at the
 * moment it executed the restorer's "int $0x82". Only the fields the
 * signal delivery actually changed need restoring (see
 * sched_deliver_signal()'s comment on why rbx/rbp/r12-r15 and the
 * other argument registers never needed saving in the first place). */
long sched_sigreturn(uint64_t frame_rsp) {
    struct task *t = g_current;
    if (t == NULL) {
        return 0;
    }
    uint64_t *f = (uint64_t *)(uintptr_t)frame_rsp;
    f[14] = t->sig_saved_rax;
    f[15] = t->sig_saved_rip;
    f[17] = t->sig_saved_rflags;
    f[18] = t->sig_saved_rsp;
    t->sig_blocked = t->sig_saved_blocked;
    return 0;
}

void task_exit(int status) __attribute__((noreturn));
void task_exit(int status) {
    if (g_current == NULL) {
        for (;;) {
            cli();
            hlt();
        }
    }
    g_current->state = TASK_ZOMBIE;
    g_current->exit_status = status;

    /* A foreground program (kilo) that owned the console keyboard
     * must give it back so the shell can read again. */
    kbd_input_release(g_current->pid);

    /* A highX client that dies (cleanly or through a fault) must not
     * leave its windows on screen: the display server drops them and
     * repaints whatever was behind them. */
    highx_client_exit(g_current->pid);

    /* A terminal application that dies takes its sessions with it:
     * the shells inside them are told to stop the next time they
     * look for a keystroke. */
    term_client_exit(g_current->pid);

    /* Release the task's file descriptors: the fd table is per-task,
     * and closing the whole table is also what lets a pipe reader
     * see EOF once its writer exits. */
    vfs_close_all();

    struct task *next = task_next(g_current);
    if (next == g_current) {
        /* No other task: halt the system. */
        console_write("task_exit: no other tasks - system halted.\n");
        for (;;) {
            cli();
            hlt();
        }
    }

    next->state = TASK_RUNNING;
    tss_set_rsp0(next->kstack_top);
    /* Same address-space switch as in sched_tick: the iretq below
     * resumes the next task inside its own space. FPU state and the
     * FS base travel with the task as well. */
    __asm__ volatile("fxsave (%0)" : : "r"(g_current->fpu) : "memory");
    vmm_space_switch(next->cr3);
    __asm__ volatile("fxrstor (%0)" : : "r"(next->fpu) : "memory");
    wrmsr(MSR_FS_BASE, next->fs_base);
    g_current = next;

    /* Switch to the next task's frame without returning. The pop order
 * mirrors sched_tick_entry's pushes (callee-saved first). */
    __asm__ volatile(
        "mov %0, %%rsp\n\t"
        "pop %%r15\n\t"
        "pop %%r14\n\t"
        "pop %%r13\n\t"
        "pop %%r12\n\t"
        "pop %%rbp\n\t"
        "pop %%rbx\n\t"
        "pop %%r11\n\t"
        "pop %%r10\n\t"
        "pop %%r9\n\t"
        "pop %%r8\n\t"
        "pop %%rdi\n\t"
        "pop %%rsi\n\t"
        "pop %%rdx\n\t"
        "pop %%rcx\n\t"
        "pop %%rax\n\t"
        "iretq\n\t" : : "r"(next->rsp) : "memory");
    __builtin_unreachable();
}
