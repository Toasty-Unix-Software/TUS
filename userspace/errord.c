/*
 * errord.c - error-logging daemon ("errorD")
 *
 * Listens on an AF_UNIX stream socket (kernel/net/socket.c's AF_UNIX
 * support only implements SOCK_STREAM, not SOCK_DGRAM - see
 * unix_sock_create() - so this accepts one short-lived connection per
 * report) for short error reports from any other program on the
 * system, and appends each one, timestamped, to /var/log/errors. This
 * is the "who logs a crash" half of tusSM: any service can report a
 * failure by connecting to ERRORD_SOCK_PATH, writing one line, and
 * closing.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define ERRORD_SOCK_PATH "/var/run/errord.sock"
#define ERRORD_LOG_PATH  "/var/log/errors"

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
    unlink(ERRORD_SOCK_PATH);

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("errord: socket");
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, ERRORD_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("errord: bind");
        return 1;
    }

    if (listen(sock, 8) < 0) {
        perror("errord: listen");
        return 1;
    }

    FILE *log = fopen(ERRORD_LOG_PATH, "a");
    if (log == NULL) {
        perror("errord: fopen");
        return 1;
    }
    log_line(log, "errorD started", strlen("errorD started"));

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
