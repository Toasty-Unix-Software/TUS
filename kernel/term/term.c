/*
 * term.c - terminal sessions
 *
 * What a terminal application asks for here is not a pipe to a shell
 * it started: it is a shell the KERNEL starts, running the same tsh
 * the console runs, with the same built-ins reading the same command
 * table. The session is what redirects that shell's world:
 *
 *   output  console_write/kprintf -> term_console_capture() -> ring
 *           (so `ls`, whose output never touches a file descriptor,
 *           lands in the window like everything else)
 *   input   /dev/tty0 and the shell's line editor read the input
 *           ring instead of the PS/2 keyboard
 *   size    TIOCGWINSZ reports the window's grid, not the console's
 *
 * All three ask the same question - "does the CURRENT TASK belong to
 * a session?" - and struct task answers it. task_create_user() copies
 * the pointer, so a program the shell spawns is inside the session
 * too, without a line of code in the program.
 *
 * The rings are single-producer/single-consumer and moved with
 * preemption disabled. The output ring never blocks its writer: it
 * cannot, because kprintf runs with preemption disabled and would
 * deadlock waiting for a reader that cannot run. When it fills up
 * (nothing is draining it) the oldest bytes go and the loss is
 * counted, which a client can read with TERM_OP_STATUS.
 */

#include "term.h"

#include "../core/console.h"
#include "../core/errno.h"
#include "../core/klib.h"
#include "../mm/kmalloc.h"
#include "../sched/sched.h"
#include "../vfs/vfs.h"
#include "arch/x86_64/io.h"

/* O_RDWR, for the standard descriptors the shell task opens. */
#define TERM_O_RDWR 2

/* Upper bound of the canonical user half. TERM_OP_READ/WRITE carry a
 * raw pointer inside the request, so it is range-checked here exactly
 * as the syscall layer checked the request itself (the same thing
 * highX does for PUT_IMAGE). */
#define USER_HALF_MAX 0x00007fffffffffffull

static bool user_ptr_ok(bool from_user, const void *p, uint32_t len) {
    if (!from_user) {
        return true;
    }
    uint64_t ptr = (uint64_t)(uintptr_t)p;
    uint64_t end = ptr + len;
    return end >= ptr && end <= USER_HALF_MAX;
}

static struct tsh_term *g_terms[TERM_MAX_SESSIONS];
static uint32_t g_next_id = 1;

/* ---- rings ---- */

static uint32_t ring_used(uint32_t head, uint32_t tail, uint32_t size) {
    return (head - tail) % size;
}

void term_output(struct tsh_term *t, const char *s, size_t n) {
    if (t == NULL || s == NULL) {
        return;
    }
    preempt_disable();
    for (size_t i = 0; i < n; i++) {
        uint32_t next = (t->out_head + 1) % TERM_OUT_SIZE;
        if (next == t->out_tail) {
            /* Full: drop the oldest byte rather than the newest, so
             * what the window finally shows is the END of a long
             * listing - which is the part a reader was waiting for. */
            t->out_tail = (t->out_tail + 1) % TERM_OUT_SIZE;
            t->dropped++;
        }
        t->out[t->out_head] = s[i];
        t->out_head = next;
    }
    preempt_enable();
}

static uint32_t term_output_take(struct tsh_term *t, char *dst, uint32_t max) {
    uint32_t n = 0;
    preempt_disable();
    while (n < max && t->out_tail != t->out_head) {
        dst[n++] = t->out[t->out_tail];
        t->out_tail = (t->out_tail + 1) % TERM_OUT_SIZE;
    }
    preempt_enable();
    return n;
}

static uint32_t term_input_put(struct tsh_term *t, const char *src,
                               uint32_t n) {
    uint32_t taken = 0;
    preempt_disable();
    for (uint32_t i = 0; i < n; i++) {
        uint32_t next = (t->in_head + 1) % TERM_IN_SIZE;
        if (next == t->in_tail) {
            break; /* the person typing is ahead of the shell */
        }
        t->in[t->in_head] = src[i];
        t->in_head = next;
        taken++;
    }
    preempt_enable();
    return taken;
}

bool term_input_ready(struct tsh_term *t) {
    return t != NULL && t->in_head != t->in_tail;
}

int term_input_poll(struct tsh_term *t) {
    if (t == NULL || t->in_head == t->in_tail) {
        return -1;
    }
    preempt_disable();
    int c = (unsigned char)t->in[t->in_tail];
    t->in_tail = (t->in_tail + 1) % TERM_IN_SIZE;
    preempt_enable();
    return c;
}

int term_input_getc(struct tsh_term *t) {
    for (;;) {
        if (t == NULL || t->closing) {
            return -1;
        }
        int c = term_input_poll(t);
        if (c >= 0) {
            return c;
        }
        /* Nothing typed: give the CPU back. The next timer tick (or
         * any interrupt) wakes us to look again, which is exactly
         * how the console shell waits for a key. */
        hlt();
    }
}

/* ---- who belongs to a session ---- */

struct tsh_term *term_current(void) {
    struct task *cur = sched_current();
    return cur != NULL ? cur->term : NULL;
}

bool term_console_capture(const char *s, size_t n) {
    struct tsh_term *t = term_current();
    if (t == NULL) {
        return false;
    }
    term_output(t, s, n);
    return true;
}

/* ---- the shell task ---- */

static void term_shell_main(void *arg) {
    struct tsh_term *t = (struct tsh_term *)arg;

    /* stdin/stdout/stderr on /dev/tty0. The device sends them to
     * this session because this task owns one, and every program the
     * shell spawns inherits the three descriptors - that is how a
     * pipeline's last stage prints into the window.
     *
     * open() never hands out 0, 1 or 2 (fd_alloc starts at 3: the
     * standard descriptors are replaced with dup2, never by opening
     * something), so the one open is duplicated into place. Without
     * this the slots stay empty and the first pipeline fails with
     * "too many open files" - vfs_dup(0) on nothing. */
    long tty = vfs_open("/dev/tty0", TERM_O_RDWR);
    if (tty >= 0) {
        vfs_dup2(tty, 0);
        vfs_dup2(tty, 1);
        vfs_dup2(tty, 2);
        vfs_close(tty);
    }

    tsh_run_term(t);
    task_exit(0);
}

/* ---- sessions ---- */

static struct tsh_term *term_find(uint32_t id) {
    for (int i = 0; i < TERM_MAX_SESSIONS; i++) {
        if (g_terms[i] != NULL && g_terms[i]->used && g_terms[i]->id == id) {
            return g_terms[i];
        }
    }
    return NULL;
}

/* A session belongs to the client that opened it; nobody else may
 * type into it or read what it printed. */
static struct tsh_term *term_find_owned(uint32_t id) {
    struct tsh_term *t = term_find(id);
    if (t == NULL) {
        return NULL;
    }
    struct task *cur = sched_current();
    uint32_t pid = cur != NULL ? cur->pid : 0;
    return (t->owner == pid) ? t : NULL;
}

static void term_free(struct tsh_term *t) {
    for (int i = 0; i < TERM_MAX_SESSIONS; i++) {
        if (g_terms[i] == t) {
            g_terms[i] = NULL;
        }
    }
    kfree(t);
}

/* Close a session: tell its shell to stop and let it free the
 * session on its way out. The shell may be halted inside
 * term_input_getc(), which is why `closing` is checked there. */
static void term_close(struct tsh_term *t) {
    t->closing = true;
    t->owner = 0;

    /* The shell task frees nothing itself (it may be anywhere), so
     * wait for it to go and clean up here. A shell that is stuck in
     * a program that never returns keeps the memory: that is one
     * window's worth, and it is better than freeing a struct a
     * running task still points at. */
    for (int spins = 0; spins < 200 && sched_task_alive(t->shell_pid);
         spins++) {
        hlt();
    }
    if (sched_task_alive(t->shell_pid)) {
        return; /* still busy: leak the session rather than corrupt it */
    }
    term_free(t);
}

void term_client_exit(uint32_t pid) {
    for (int i = 0; i < TERM_MAX_SESSIONS; i++) {
        struct tsh_term *t = g_terms[i];
        if (t != NULL && t->used && t->owner == pid) {
            /* Called from task_exit, which must not wait for
             * anything: mark it and let the shell notice. */
            t->closing = true;
            t->owner = 0;
        }
    }
}

static long term_open(struct term_open *req) {
    if (req->magic != TERM_MAGIC) {
        return -EINVAL;
    }

    int slot = -1;
    for (int i = 0; i < TERM_MAX_SESSIONS; i++) {
        /* A session whose shell has exited is reaped here rather
         * than in the shell itself, which cannot free the struct it
         * is running on. */
        if (g_terms[i] != NULL && g_terms[i]->used &&
            !sched_task_alive(g_terms[i]->shell_pid)) {
            term_free(g_terms[i]);
        }
        if (g_terms[i] == NULL && slot < 0) {
            slot = i;
        }
    }
    if (slot < 0) {
        return -EMFILE;
    }

    struct tsh_term *t = kmalloc(sizeof(*t));
    if (t == NULL) {
        return -ENOMEM;
    }
    memset(t, 0, sizeof(*t));
    t->used = true;
    t->id = g_next_id++;
    struct task *cur = sched_current();
    t->owner = cur != NULL ? cur->pid : 0;
    t->cols = req->cols > 0 && req->cols <= TERM_COLS_MAX ? req->cols : 80;
    t->rows = req->rows > 0 && req->rows <= TERM_ROWS_MAX ? req->rows : 24;
    t->shell.browse = -1;
    strcpy(t->cwd, "/");
    t->oldpwd[0] = '\0';
    g_terms[slot] = t;

    int pid = task_create_kernel(term_shell_main, t, "tsh");
    if (pid < 0) {
        term_free(t);
        return -EAGAIN;
    }
    t->shell_pid = (uint32_t)pid;
    task_set_term((uint32_t)pid, t);

    req->id = t->id;
    req->shell_pid = t->shell_pid;
    return 0;
}

long term_syscall(long op, void *arg, size_t len, bool from_user) {
    switch (op) {
    case TERM_OP_OPEN: {
        if (arg == NULL || len < sizeof(struct term_open)) {
            return -EINVAL;
        }
        return term_open((struct term_open *)arg);
    }
    case TERM_OP_READ: {
        if (arg == NULL || len < sizeof(struct term_io)) {
            return -EINVAL;
        }
        struct term_io *io = (struct term_io *)arg;
        struct tsh_term *t = term_find_owned(io->id);
        if (t == NULL) {
            return -ENOENT;
        }
        if (io->buf == NULL || io->len == 0) {
            return 0;
        }
        if (!user_ptr_ok(from_user, io->buf, io->len)) {
            return -EFAULT;
        }
        return (long)term_output_take(t, (char *)io->buf, io->len);
    }
    case TERM_OP_WRITE: {
        if (arg == NULL || len < sizeof(struct term_io)) {
            return -EINVAL;
        }
        struct term_io *io = (struct term_io *)arg;
        struct tsh_term *t = term_find_owned(io->id);
        if (t == NULL) {
            return -ENOENT;
        }
        if (io->buf == NULL || io->len == 0) {
            return 0;
        }
        if (!user_ptr_ok(from_user, io->buf, io->len)) {
            return -EFAULT;
        }
        return (long)term_input_put(t, (const char *)io->buf, io->len);
    }
    case TERM_OP_RESIZE: {
        if (arg == NULL || len < sizeof(struct term_size)) {
            return -EINVAL;
        }
        struct term_size *sz = (struct term_size *)arg;
        struct tsh_term *t = term_find_owned(sz->id);
        if (t == NULL) {
            return -ENOENT;
        }
        if (sz->cols > 0 && sz->cols <= TERM_COLS_MAX) {
            t->cols = sz->cols;
        }
        if (sz->rows > 0 && sz->rows <= TERM_ROWS_MAX) {
            t->rows = sz->rows;
        }
        return 0;
    }
    case TERM_OP_CLOSE: {
        if (arg == NULL || len < sizeof(struct term_id)) {
            return -EINVAL;
        }
        struct tsh_term *t = term_find_owned(((struct term_id *)arg)->id);
        if (t == NULL) {
            return -ENOENT;
        }
        term_close(t);
        return 0;
    }
    case TERM_OP_STATUS: {
        if (arg == NULL || len < sizeof(struct term_status)) {
            return -EINVAL;
        }
        struct term_status *st = (struct term_status *)arg;
        struct tsh_term *t = term_find_owned(st->id);
        if (t == NULL) {
            return -ENOENT;
        }
        st->alive = sched_task_alive(t->shell_pid) ? 1 : 0;
        st->pending = ring_used(t->out_head, t->out_tail, TERM_OUT_SIZE);
        st->dropped = t->dropped;
        return 0;
    }
    default:
        return -EINVAL;
    }
}

void term_init(void) {
    console_set_capture(term_console_capture);
}
