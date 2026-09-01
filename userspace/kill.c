/*
 * kill.c - send a signal to a process (TUS port of the classic UNIX
 * kill). Real POSIX kill(2), NOT tsh's own `cmd_kill` built-in
 * (kernel/shell/cmd_fs.c), which calls task_kill() directly and
 * unconditionally terminates - this goes through TUS's real signal
 * delivery (SYS_KILL -> sched_raise()/sched_raise_pgid(), see
 * kernel/syscall/syscall.c), which is blockable/catchable/subject to
 * default-action rules like a real Unix kill(2), and is what any
 * script or program (not just this binary) already gets from musl's
 * kill().
 *
 * Accepts the usual forms: `kill PID`, `kill -9 PID`, `kill -KILL PID`,
 * `kill -SIGTERM PID`.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct signame {
    const char *name;
    int sig;
};

static const struct signame g_signames[] = {
    {"HUP", SIGHUP},   {"INT", SIGINT},   {"QUIT", SIGQUIT},
    {"ILL", SIGILL},   {"ABRT", SIGABRT}, {"FPE", SIGFPE},
    {"KILL", SIGKILL}, {"SEGV", SIGSEGV}, {"PIPE", SIGPIPE},
    {"ALRM", SIGALRM}, {"TERM", SIGTERM}, {"USR1", SIGUSR1},
    {"USR2", SIGUSR2}, {"CHLD", SIGCHLD}, {"CONT", SIGCONT},
    {"STOP", SIGSTOP}, {"TSTP", SIGTSTP}, {"TTIN", SIGTTIN},
    {"TTOU", SIGTTOU},
};

static int parse_sig(const char *s) {
    if (s[0] >= '0' && s[0] <= '9') {
        return atoi(s);
    }
    if (s[0] == 'S' && s[1] == 'I' && s[2] == 'G') {
        s += 3;
    }
    for (size_t i = 0; i < sizeof(g_signames) / sizeof(g_signames[0]); i++) {
        if (strcmp(s, g_signames[i].name) == 0) {
            return g_signames[i].sig;
        }
    }
    return -1;
}

int main(int argc, char **argv) {
    int sig = SIGTERM;
    int i = 1;

    if (argc > 1 && argv[1][0] == '-' && argv[1][1] != '\0') {
        int s = parse_sig(argv[1] + 1);
        if (s < 0) {
            fprintf(stderr, "kill: invalid signal -- '%s'\n", argv[1] + 1);
            return 1;
        }
        sig = s;
        i = 2;
    }

    if (i >= argc) {
        fprintf(stderr, "usage: kill [-signal] pid [pid...]\n");
        return 1;
    }

    int failed = 0;
    for (; i < argc; i++) {
        pid_t pid = (pid_t)atol(argv[i]);
        if (kill(pid, sig) < 0) {
            fprintf(stderr, "kill: (%ld): %s\n", (long)pid, strerror(errno));
            failed = 1;
        }
    }
    return failed;
}
