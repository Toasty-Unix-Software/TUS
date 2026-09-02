/*
 * sched.h - round-robin task scheduler
 *
 * The scheduler is driven by the PIT (IRQ0, 100 Hz). A tiny assembly
 * stub saves the caller-saved registers plus the interrupt frame on
 * the current task's kernel stack, asks the C core for the next task,
 * and either returns to the same task or switches RSP to the next
 * task's saved frame and IRETQs into it.
 *
 * Task 0 is the kernel shell (tsh) itself, running in ring 0. Tasks
 * created with task_create_user() run in ring 3 via an IRETQ frame
 * that loads the user segments, the user stack and the entry point.
 */

#ifndef TUS_SCHED_SCHED_H
#define TUS_SCHED_SCHED_H

#include <stdbool.h>
#include <stdint.h>

#include "../vfs/vfs.h" /* VFS_MAX_FDS */

struct vfs_file;
struct tsh_term;

/* Task states. */
#define TASK_READY   1
#define TASK_RUNNING 2
#define TASK_ZOMBIE  3
/* Stopped by SIGSTOP/SIGTSTP/SIGTTIN/SIGTTOU's default action (or an
 * explicit kill -STOP): excluded from scheduling like TASK_ZOMBIE,
 * but SIGCONT (kernel/syscall/syscall.c's SYS_KILL case) reverses it
 * back to TASK_READY instead of the slot being reclaimed. */
#define TASK_STOPPED 4

#define TASK_NAME_MAX 32
#define TASK_MAX      16

/* Longest working directory TUS tracks per task, matching the
 * existing TERM_CWD_MAX convention (kernel/term/term.h) - a terminal
 * session's own cwd was already this size, this just gives every
 * OTHER task (one with no terminal session at all) the same field. */
#define TASK_CWD_MAX 128

struct task {
    uint32_t pid;
    uint32_t state;
    char name[TASK_NAME_MAX];

    /* Process identity ksh's job control and $PPID need: the parent
     * that created this task, and the process group / session it
     * belongs to. A freshly created task inherits pgid/sid from its
     * creator (task 0, the boot shell, seeds pgid=sid=1); setsid()/
     * setpgid() (kernel/syscall/syscall.c) are the only things that
     * change them afterward. */
    uint32_t ppid;
    uint32_t pgid;
    uint32_t sid;

    /* Working directory for relative paths. The VFS itself used to
     * require an absolute path for everything (see the NOTE in
     * kernel/vfs/vfs.h); vfs_lookup() now resolves a relative path
     * against this field via vfs_path_resolve() - the same
     * dot-and-dotdot-collapsing resolver kernel/shell/cmd_fs.c has
     * used for the kernel shell's own `cd` all along, just now
     * reachable from a ring-3 task's chdir()/open() too. */
    char cwd[TASK_CWD_MAX];

    /* Kernel stack: RSP0 (top) is what TSS.RSP0 points at; rsp is
     * the saved frame position while the task is not running. */
    uint64_t kstack;
    uint64_t kstack_top;
    uint64_t rsp;

    /* User stack (ring 3 tasks). */
    uint64_t ustack;
    uint64_t ustack_top;

    /* Per-task address space (physical PML4). Tasks created by
     * task_create_user() run in their own space; the kernel half is
     * shared with the root space, the user half is private. */
    uint64_t cr3;

    /* FPU/SSE state (fxsave image, 512 bytes, 16-byte aligned). User
     * programs compiled with SSE use the XMM registers; the kernel
     * itself is compiled with -mgeneral-regs-only and never touches
     * them, but they must survive a task switch, so every switch
     * fxsaves the outgoing task and fxrstors the incoming one. */
    uint8_t fpu[512] __attribute__((aligned(16)));

    /* User-mode FS base (thread pointer / TLS). Written by
     * arch_prctl(ARCH_SET_FS); reloaded into the MSR on every task
     * switch (the C library stores errno and TLS in it). */
    uint64_t fs_base;

    /* User identity. TUS has no privilege separation yet - every task
     * starts as uid/gid 0 (root) - but the ids are tracked and can be
     * queried/changed via the uid/gid syscalls, which is what doas,
     * passwd and login use. */
    uint32_t uid;
    uint32_t euid;
    uint32_t gid;
    uint32_t egid;

    /* POSIX-capabilities-style bitmask (see cap.h). A non-root task
     * (euid != 0) has exactly these bits; euid == 0 implicitly has
     * every capability regardless of this field (has_cap() encodes
     * that). Inherited by children (sched_fork), unaffected by
     * setuid/setgid. Settable only by a root task via SYS_CAPSET. */
    uint32_t caps;

    /* Next address for anonymous mmap allocations (see SYS_MMAP). */
    uint64_t mmap_cur;

    /* Per-task file descriptor table (open files, see vfs.c). A new
     * task inherits a refcounted copy of its creator's table at
     * spawn (POSIX fd inheritance without fork); task_exit closes
     * every entry. tsh sets up slots 0/1/2 (redirection, pipes)
     * before spawning, which is exactly how `a | b` and `> file`
     * work: the child keeps its copy when the shell restores its
     * own. */
    struct vfs_file *fds[VFS_MAX_FDS];

    /* FD_CLOEXEC per fd NUMBER, not per open file: two fds sharing
     * the same vfs_file via dup() must be able to disagree (POSIX),
     * so this can't live on vfs_file itself (which dup2'd slots
     * share, refcounted). fcntl(F_SETFD) writes it; sys_execve()
     * (kernel/syscall/syscall.c) is the only reader - it closes every
     * fd marked here before the new image starts. Zeroed for a fresh
     * task same as fds[] (task_create* memsets the whole struct). */
    bool fd_cloexec[VFS_MAX_FDS];

    /* The terminal session this task belongs to, or NULL. Inherited
     * by every task this one creates, which is what puts a program
     * started from a terminal window inside that window: its console
     * output, its keyboard input and its window size all follow this
     * pointer (see kernel/term/term.h). */
    struct tsh_term *term;

    int exit_status;

    /* Set when this task is killed while it was the one actually
     * running (an interrupt handler - Ctrl+C, see kernel/drivers/keyboard/
     * keyboard.c - firing while THIS task's instruction stream is
     * the one that got interrupted, not some other task's hlt()).
     * task_kill() cannot switch away from the caller's own context,
     * so it cannot safely finish killing g_current itself; this flags
     * the job for sched_tick() instead, which already IS the place
     * that switches tasks and can safely discard this one's state
     * rather than preserve it for a resume that must never happen. */
    bool pending_kill;
    int pending_kill_status;

    /* True when exit_status was caused by a signal (task_kill_cleanup()
     * or a fault, see idt.c) rather than a real exit(2) call - what
     * lets waitpid() encode the wait(2) status correctly (WIFSIGNALED/
     * WTERMSIG vs WIFEXITED/WEXITSTATUS use different bit layouts;
     * TUS's OWN exit_status convention of "128+signal" for a signal
     * death, matching what a shell's $? already shows, does not by
     * itself say which layout applies). */
    bool exit_signaled;

    /* Same deferred-switch problem as pending_kill above, for
     * SIGSTOP/SIGTSTP's default action: a task cannot move itself out
     * of TASK_RUNNING from the outside, so sched_tick() (already the
     * one place tasks get switched) finishes the job. */
    bool pending_stop;

    /* ---- real signal delivery (see sched_tick()'s ring-3 check and
     * sched_sigreturn()/sigreturn_entry, kernel/sched/sched.c) ----
     *
     * Signal numbers 1..31 (the classic, non-realtime set) map to bit
     * (n-1) throughout. sig_pending is set by whatever raises a
     * signal (SYS_KILL, the keyboard's Ctrl+C/Ctrl+Z, SIGALRM's own
     * timer below) and cleared once delivered; sig_blocked is what
     * sigprocmask() manipulates - a pending-but-blocked signal stays
     * pending until unblocked. SIGKILL(9)/SIGSTOP(19) are neither
     * blockable nor catchable (enforced where sig_blocked and
     * sig_action are written, not read) so they need no entry here
     * beyond the pending bit itself. */
    uint64_t sig_pending;
    uint64_t sig_blocked;
    struct {
        uint64_t handler;  /* 0 = SIG_DFL, 1 = SIG_IGN, else a user address */
        uint64_t flags;    /* SA_RESTART / SA_SIGINFO / ... (Linux bit values) */
        uint64_t restorer; /* musl's __restore_rt, pushed as the handler's
                             * return address so `ret` lands on sigreturn_entry's
                             * "int $0x82" trampoline (see arch/x86_64/src/
                             * signal/restore.s) */
        uint64_t mask;     /* additional signals blocked while this one runs */
    } sig_action[32];      /* index 0 unused; 1..31 are the real signals */

    /* The ONE in-flight handler's pre-signal context, saved by
     * sched_tick()'s delivery check and restored by sched_sigreturn()
     * when the handler returns. Signals are fully blocked (see
     * sched_tick()) for the duration of a handler, so there is never
     * more than one of these live per task - deliberately simpler
     * than POSIX's allowance for nested handlers, and enough for
     * ksh's trap/SIGINT/SIGCHLD use. */
    uint64_t sig_saved_rax, sig_saved_rip, sig_saved_rsp, sig_saved_rflags;
    uint64_t sig_saved_blocked;

    /* alarm(): an uptime-ms deadline (0 = none) sched_tick() compares
     * against pit_uptime_ms() on every PIT tick, raising SIGALRM
     * (setting the pending bit) once it passes - the same "checked at
     * the one place ticks already happen" shape as pending_kill. */
    uint64_t alarm_deadline_ms;
};

/* Initialise the scheduler; the calling context (tsh) becomes task 0. */
void sched_init(void);

/* IRQ0 entry point (assembly stub, see sched.c). */
void sched_tick_entry(void);

/* C core of the tick: called from the stub with RSP pointing at the
 * saved registers. Returns the RSP to switch to, or 0 to continue. */
uint64_t sched_tick(uint64_t frame_rsp);

/* fork()'s own gate (int 0x81, see kernel/arch/x86_64/idt.c) and its
 * C core. sched_fork() duplicates the calling task - address space,
 * fd table, register state - and returns the new pid, which becomes
 * the parent's syscall return value; the child's own first return of
 * 0 comes from resuming the copied frame, not from anything sched_fork()
 * itself returns to. See the comment above fork_entry()'s definition
 * in sched.c for the full design. */
void fork_entry(void);
long sched_fork(uint64_t frame_rsp);

/* Create a ring-3 task that starts executing `entry` in the address
 * space `cr3` (see vmm_space_clone). The user stack is mapped inside
 * that space, and the fake interrupt frame is pre-built so the first
 * switch IRETQs straight into ring 3. `argc`/`argv` (argv[0] is
 * conventionally the program path) are copied onto the user stack in
 * the standard SysV layout so a C runtime sees real arguments.
 * Returns the new PID or -1. */
int task_create_user(uint64_t entry, const char *name, uint64_t cr3,
                     int argc, char **argv);

/* Create a ring-0 task: `entry(arg)` runs in the kernel's own address
 * space with a stack of its own, scheduled like any other task. This
 * is what a terminal session's shell runs on - tsh is kernel code,
 * and a window full of it is a task, not a process. The function is
 * expected to end in task_exit(); returning from it does too.
 * Returns the new PID or -1. */
int task_create_kernel(void (*entry)(void *), void *arg, const char *name);

/* Attach a terminal session to a task (term.c, right after it has
 * created the shell task). Returns 0 on success. */
int task_set_term(uint32_t pid, struct tsh_term *term);

/* Current task; NULL before sched_init(). */
struct task *sched_current(void);

/* Terminate the current task (noreturn; switches to the next). */
void task_exit(int status) __attribute__((noreturn));

/* Preemption control: while the counter is non-zero the PIT tick does
 * not switch tasks (used around kernel code that must not be
 * interrupted mid-way, e.g. kprintf's format walk). Nested-safe. */
void preempt_disable(void);
void preempt_enable(void);

/* Number of live tasks (for sysinfo). */
int sched_task_count(void);

/* Print the task table (ps command): PID, state, name. */
void task_list_all(void);

/* One task's worth of the fields a ring-3 /bin/ps needs - the ABI
 * SYS_GETPROCS (kernel/syscall/syscall.h) copies to userspace, one
 * per live task. Deliberately NOT struct task itself: that carries
 * kernel-internal state (page tables, kernel/user stacks, FPU image,
 * the fd table) that must never cross the ring-3 boundary. */
struct tus_procinfo {
    uint32_t pid;
    uint32_t ppid;
    uint32_t pgid;
    uint32_t uid;
    uint32_t state;
    char name[TASK_NAME_MAX];
};

/* Fill `out` (room for `max` entries) with one struct tus_procinfo per
 * live (non-empty-slot) task, in task-table order - same tasks
 * task_list_all() would have printed. Returns the number written. */
int task_snapshot(struct tus_procinfo *out, int max);

/* Find a live (non-zombie) task by pid, or NULL. Exposed for `kill`
 * and `pkill`. */
struct task *sched_find_pid(uint32_t pid);

/* Terminate a task OTHER than the current one: releases the console
 * keyboard, its highX windows and terminal sessions, and its fd
 * table, then marks it a zombie so task_next() stops scheduling it.
 * This is the shell's `kill`/`pkill` - an unconditional, unblockable
 * termination with no signal semantics, unlike sched_raise() below,
 * which is the real POSIX kill(2) a ring-3 task calls (blockable,
 * catchable, subject to default-action rules). Returns 0 on success,
 * -1 if pid does not name a live task, or if pid names the CALLING
 * task (which must go through task_exit() instead: it needs to
 * switch away, which a task cannot do to itself from the outside).
 */
int task_kill(uint32_t pid, int status);

/* task_kill() every live task whose name contains `substr` (plain
 * substring match, no globbing), skipping the calling task itself.
 * Returns the number killed. */
int task_kill_by_name(const char *substr, int status);

/* True while a task with this pid exists and has not exited. The
 * shell spins on this (hlt) to wait for a spawned pipeline stage. */
int sched_task_alive(uint32_t pid);

#define SCHED_MAX_FOREGROUND 8

/* Which pids exec_pipeline() is currently hlt()-spinning on, so
 * Ctrl+C (kernel/drivers/keyboard/keyboard.c) has something to kill: the
 * shell's own wait loop is a plain sched_task_alive()/hlt() poll, not
 * a call that goes through the keyboard queue, so nothing about a
 * foreground job (which usually never reads the keyboard at all - a
 * `ping` doesn't) is otherwise visible to the driver. Call with
 * count 0 (or sched_clear_foreground()) once the wait loop is done,
 * successfully or not - a stale entry would let a *later* unrelated
 * task be killed by an incoming Ctrl+C that meant nothing to it. */
void sched_set_foreground(const uint32_t *pids, int count);
void sched_clear_foreground(void);

/* sched_raise()s a real SIGINT on every currently tracked foreground
 * pid and clears the list. A no-op when nothing is tracked - an idle
 * prompt's Ctrl+C has nothing to interrupt. A target with no handler
 * installed still dies (128+SIGINT, same as always); one that has
 * installed a real handler (an interactive shell trapping SIGINT to
 * abort its current line rather than exit) runs it instead. */
void sched_interrupt_foreground(void);

/* Same as sched_interrupt_foreground(), for Ctrl+Z: a real SIGTSTP
 * (kernel/drivers/keyboard/keyboard.c). A target with no handler installed
 * stops (TASK_STOPPED); `fg`/`bg`/SIGCONT bring it back. */
void sched_stop_foreground(void);

/* Look up a finished task's exit status. Returns 1 and fills in
 * `status` when `pid` has exited, 0 while it is still running, and -1
 * when no task by that pid exists at all (already reaped, or never
 * started). The zombie slot survives until the scheduler reuses it,
 * which is what gives waitpid() something to find. */
int sched_task_reap(uint32_t pid, int *status);

/* Companion to sched_task_reap(): true if `pid`'s exit_status came
 * from a signal death rather than a real exit(2) - see struct
 * task::exit_signaled. Valid for the same lifetime as
 * sched_task_reap() (the zombie slot, until the scheduler reuses
 * it), so call it before or after reaping, not after the pid is
 * gone. */
bool sched_task_was_signaled(uint32_t pid);

/* Mark the CURRENT task's eventual exit as signal-caused - called
 * from a ring-3 fault handler (kernel/arch/x86_64/idt.c) right before
 * task_exit(128 + signal), the one task_exit() call site that is NOT
 * a real exit(2) but shares the same function. */
void sched_mark_exit_signaled(void);

/* waitpid(-1, ...) support (kernel/syscall/syscall.c): the pid of a
 * zombie task whose ppid is `ppid`, or 0 if none is waiting to be
 * reaped. sched_has_child() is true while ANY task (zombie or still
 * running) has this ppid - what tells waitpid(-1) apart from -ECHILD
 * (no children at all) versus "children exist, none have exited
 * yet" (keep waiting). */
uint32_t sched_find_zombie_child(uint32_t ppid);
bool sched_has_child(uint32_t ppid);

/* ---- real (POSIX) signal delivery ----
 *
 * sched_raise(pid, sig) is kill(2): records the signal as pending on
 * the target task (SYS_KILL, kernel/syscall/syscall.c), special-cased
 * for SIGCONT (wakes a TASK_STOPPED task immediately - it is not
 * being scheduled, so it would otherwise never reach the delivery
 * check below to notice its own pending bit) and SIGKILL (goes
 * straight through task_kill()/pending_kill - unblockable,
 * uncatchable, no reason to wait for the next tick). Returns 0, or
 * -ESRCH if pid names no task.
 *
 * Actual delivery - default action, or redirecting into a real
 * handler - happens lazily, inside sched_tick(), the moment a ring-3
 * task is next about to run: see the comment there. This is what
 * lets kill() targeting a task that IS NOT g_current work without any
 * of fork()'s register-capture problem - nothing needs interrupting,
 * the target simply picks its own pending signal up next time it is
 * scheduled in. */
int sched_raise(uint32_t pid, int sig);

/* killpg(2)/kill(2) with pid <= 0: sched_raise() every live task in
 * process group `pgid`. Returns the number signalled. */
int sched_raise_pgid(uint32_t pgid, int sig);

/* True while the current task has a pending, unblocked signal it has
 * not yet received - see the comment on its definition
 * (kernel/sched/sched.c) for why a multi-tick blocking wait needs to
 * check this itself rather than waiting for sched_tick()'s own
 * delivery. */
bool sched_signal_pending(void);

/* sigreturn's own gate (int 0x82) and its C core - structurally
 * fork_entry()/sched_fork()'s naked-stub-plus-full-frame-copy
 * pattern, run in reverse: instead of copying the parent's captured
 * frame into a new task, it overwrites the CURRENT frame with the
 * pre-signal context sched_tick()'s delivery check saved. See the
 * comment above sigreturn_entry()'s definition in sched.c. */
void sigreturn_entry(void);
long sched_sigreturn(uint64_t frame_rsp);

#endif /* TUS_SCHED_SCHED_H */
