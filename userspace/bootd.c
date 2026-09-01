/*
 * bootd.c - boot log / journal daemon ("bootD")
 *
 * Owns /var/log/journal: a single append-only log any part of the
 * system can add a line to, over an AF_UNIX stream socket at
 * JOURNALD_SOCK_PATH - the same shape as errord.c's error socket
 * (kernel/net/socket.c's AF_UNIX support only implements SOCK_STREAM),
 * but for general lifecycle events (service started/stopped/restarted,
 * boot milestones) rather than just errors. tusSM sends every service
 * start/stop/restart/crash decision here, so the journal is a real
 * timeline of "what tusSM decided and when", not just a static copy
 * of the console's own boot text (TUS has no /proc/kmsg-style kernel
 * ring buffer to read - see kernel/vfs/procfs.c - so this journal is
 * userspace-and-later only, honestly scoped rather than pretending to
 * replay pre-boot kernel messages it cannot see).
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define JOURNALD_SOCK_PATH "/var/run/journald.sock"
#define JOURNAL_LOG_PATH   "/var/log/journal"

static void log_line(FILE *log, const char *msg, size_t len) {
    time_t t = time(NULL);
    struct tm tm;
    gmtime_r(&t, &tm);
    char stamp[32];
    strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", &tm);
    fprintf(log, "[%s] %.*s\n", stamp, (int)len, msg);
    fflush(log);
}

int main(void) {
    unlink(JOURNALD_SOCK_PATH);

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("bootd: socket");
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, JOURNALD_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bootd: bind");
        return 1;
    }

    if (listen(sock, 8) < 0) {
        perror("bootd: listen");
        return 1;
    }

    FILE *log = fopen(JOURNAL_LOG_PATH, "a");
    if (log == NULL) {
        perror("bootd: fopen");
        return 1;
    }
    log_line(log, "bootD started, journal opened", strlen("bootD started, journal opened"));

    char buf[512];
    for (;;) {
        int conn = accept(sock, NULL, NULL);
        if (conn < 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        long n = read(conn, buf, sizeof(buf) - 1);
        if (n > 0) {
            log_line(log, buf, (size_t)n);
        }
        close(conn);
    }

    fclose(log);
    close(sock);
    return 0;
}
