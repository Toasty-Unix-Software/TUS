/*
 * tussm.c - tusSM, the Toasty Unix Software Service Manager
 *
 * A small ring-3 supervisor, started once at boot (see
 * kernel/main.c:load_boot_services()), that starts a hardcoded table
 * of services, forks/execs each, and watches them with a WNOHANG
 * waitpid() poll loop - the same "own the child, notice it die"
 * pattern kill.c/pkill.c already lean on for signal delivery. A
 * crashed service is restarted, up to a small per-service budget; a
 * service marked `critical` that exceeds its budget escalates to a
 * kernel panic screen (BSOD) via SYS_PANIC (kernel/syscall/syscall.h)
 * rather than just being logged and left dead, since by definition
 * the rest of the system cannot be trusted to keep running without
 * it.
 *
 * Status is published to /var/run/tussm.status (one line per service:
 * "name pid state restarts") so the `sm`/`service` tsh command
 * (kernel/shell/cmd_fs.c) can show it without any IPC of its own -
 * plain re-reads of a small file, same shape as /proc's generated
 * files.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* SYS_PANIC (84): no musl wrapper, same raw int $0x80 pattern
 * ps.c/pkill.c already use for TUS-only syscalls. */
#define SYS_PANIC 84
#define TUS_PANIC_BSOD 0

static long tus_syscall1(long nr, long a1) {
    register long rax __asm__("rax") = nr;
    register long r10 __asm__("r10") = 0;
    register long r8  __asm__("r8")  = 0;
    register long r9  __asm__("r9")  = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = 0;
    register long rdx __asm__("rdx") = 0;
    __asm__ volatile("int $0x80"
                      : "+r"(rax)
                      : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8), "r"(r9)
                      : "memory", "cc");
    return rax;
}

#define STATUS_PATH  "/var/run/tussm.status"
#define JOURNAL_SOCK "/var/run/journald.sock"

/* Restart budget: how many crashes a service may have before tusSM
 * gives up on it (non-critical) or escalates (critical). Small on
 * purpose - a service crash-looping more than this fast is broken,
 * not unlucky, and restarting it forever would just burn CPU. */
#define MAX_RESTARTS 5

struct service {
    const char *name;
    const char *path;
    int critical;
};

/* The hardcoded service table. errorD and bootD are the two tusSM
 * itself depends on (every other service's crash gets logged/
 * journaled through them), so both are critical - if either of them
 * cannot be kept running, the system has lost its own error/boot
 * record and that is exactly the "critical service crash" BSOD is
 * for. */
static const struct service g_services[] = {
    { "errorD", "/bin/errord", 1 },
    { "bootD",  "/bin/bootd",  1 },
};
#define SERVICE_COUNT (sizeof(g_services) / sizeof(g_services[0]))

enum svc_state { SVC_STOPPED = 0, SVC_RUNNING, SVC_FAILED };

struct svc_runtime {
    pid_t pid;
    enum svc_state state;
    int restarts;
};

static struct svc_runtime g_rt[SERVICE_COUNT];

static void journal_send(const char *msg) {
    /* AF_UNIX here only implements SOCK_STREAM (kernel/net/socket.c:
     * unix_sock_create()), so each journal line is its own short-lived
     * connection rather than a connectionless datagram. */
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        return;
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, JOURNAL_SOCK, sizeof(addr.sun_path) - 1);
    /* Best-effort: bootD may not be up yet on the very first call
     * (tusSM starts it, so there is an unavoidable startup race) or
     * may itself be down - a lost journal line is not worth blocking
     * the supervisor over. */
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
        write(sock, msg, strlen(msg));
    }
    close(sock);
}

static void write_status(void) {
    FILE *f = fopen(STATUS_PATH, "w");
    if (f == NULL) {
        return;
    }
    static const char *state_name[] = { "stopped", "running", "failed" };
    for (size_t i = 0; i < SERVICE_COUNT; i++) {
        fprintf(f, "%s %d %s %d\n", g_services[i].name,
                (int)g_rt[i].pid, state_name[g_rt[i].state], g_rt[i].restarts);
    }
    fclose(f);
}

static void start_service(size_t i) {
    pid_t pid = fork();
    if (pid < 0) {
        g_rt[i].state = SVC_FAILED;
        return;
    }
    if (pid == 0) {
        execl(g_services[i].path, g_services[i].name, (char *)NULL);
        _exit(127);
    }
    g_rt[i].pid = pid;
    g_rt[i].state = SVC_RUNNING;

    char buf[128];
    snprintf(buf, sizeof(buf), "tusSM: started %s (pid %d)",
             g_services[i].name, (int)pid);
    journal_send(buf);
}

static void handle_exit(pid_t pid, int status) {
    for (size_t i = 0; i < SERVICE_COUNT; i++) {
        if (g_rt[i].pid != pid) {
            continue;
        }
        g_rt[i].restarts++;

        char buf[160];
        snprintf(buf, sizeof(buf),
                 "tusSM: %s (pid %d) exited status 0x%x - restart %d/%d",
                 g_services[i].name, (int)pid, status,
                 g_rt[i].restarts, MAX_RESTARTS);
        journal_send(buf);

        if (g_rt[i].restarts > MAX_RESTARTS) {
            g_rt[i].state = SVC_FAILED;
            write_status();
            if (g_services[i].critical) {
                /* The service this depends on (the journal/error log
                 * itself) cannot be trusted to be up any more - fall
                 * back to the console directly, then escalate. */
                fprintf(stderr,
                        "tusSM: critical service '%s' crash-looped past "
                        "its restart budget - escalating to BSOD\n",
                        g_services[i].name);
                tus_syscall1(SYS_PANIC, TUS_PANIC_BSOD);
                /* Not reached: SYS_PANIC halts the machine. */
            }
            return;
        }

        start_service(i);
        write_status();
        return;
    }
}

int main(void) {
    for (size_t i = 0; i < SERVICE_COUNT; i++) {
        start_service(i);
    }
    write_status();

    for (;;) {
        int status = 0;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if (pid > 0) {
            handle_exit(pid, status);
            continue;
        }
        sleep(1);
    }
}
