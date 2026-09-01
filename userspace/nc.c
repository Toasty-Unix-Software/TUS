/*
 * nc - netcat
 *
 * Usage: nc [-v] host port      connect and pump stdin/stdout
 *        nc -l port [-v]        listen for one connection and do the same
 *
 * The pump is a poll() over the socket and stdin, so neither direction
 * can starve the other - which is exactly what a half-duplex
 * read-then-write loop would do the first time a server spoke without
 * being spoken to.
 */

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "tusnetutil.h"

/* musl's sockaddr_in, which the kernel's netctl ABI matches. */
struct sockaddr_in_t {
    uint16_t sin_family;
    uint16_t sin_port;
    uint32_t sin_addr;
    uint8_t sin_zero[8];
};

static uint16_t hton16(uint16_t v) { return (uint16_t)((v << 8) | (v >> 8)); }

static int pump(int sock, int verbose) {
    struct pollfd fds[2];
    char buf[2048];

    fds[0].fd = sock;
    fds[0].events = POLLIN;
    fds[1].fd = 0;
    fds[1].events = POLLIN;

    for (;;) {
        int n = poll(fds, 2, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            return 1;
        }

        /* POLLIN and POLLHUP arrive together when the peer's FIN
         * lands on top of data still queued, so both are handled by
         * the same read: whatever is buffered comes out first, and
         * only a read that returns nothing ends the session. */
        if (fds[0].revents & (POLLIN | POLLHUP | POLLERR)) {
            long r = read(sock, buf, sizeof(buf));
            if (r > 0) {
                write(1, buf, (size_t)r);
                continue;
            }
            if (verbose) fprintf(stderr, "nc: connection closed\n");
            return r < 0 ? 1 : 0;
        }

        if (fds[1].revents & POLLIN) {
            long r = read(0, buf, sizeof(buf));
            if (r <= 0) {
                /* End of input: half-close so the peer sees EOF, then
                 * keep reading its reply. */
                shutdown(sock, SHUT_WR);
                fds[1].fd = -1;
                continue;
            }
            if (write(sock, buf, (size_t)r) < 0) {
                fprintf(stderr, "nc: write failed\n");
                return 1;
            }
        }
    }
}

int main(int argc, char **argv) {
    int listen_mode = 0, verbose = 0;
    const char *host = NULL;
    int port = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            listen_mode = 1;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "usage: nc [-v] host port | nc -l port\n");
            return 1;
        } else if (!listen_mode && host == NULL) {
            host = argv[i];
        } else {
            port = atoi(argv[i]);
        }
    }

    if (port == 0 && host != NULL && listen_mode) {
        port = atoi(host);
        host = NULL;
    }
    if (port <= 0 || port > 65535 || (!listen_mode && host == NULL)) {
        fprintf(stderr, "usage: nc [-v] host port | nc -l port\n");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "nc: socket: %s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in_t addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = hton16((uint16_t)port);

    if (listen_mode) {
        addr.sin_addr = 0;
        if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            fprintf(stderr, "nc: bind: %s\n", strerror(errno));
            return 1;
        }
        if (listen(sock, 1) < 0) {
            fprintf(stderr, "nc: listen: %s\n", strerror(errno));
            return 1;
        }
        if (verbose) fprintf(stderr, "nc: listening on port %d\n", port);

        struct sockaddr_in_t peer;
        socklen_t plen = sizeof(peer);
        int conn = accept(sock, (struct sockaddr *)&peer, &plen);
        if (conn < 0) {
            fprintf(stderr, "nc: accept: %s\n", strerror(errno));
            return 1;
        }
        if (verbose) {
            char b[32];
            fprintf(stderr, "nc: connection from %s:%u\n",
                    ip_str(peer.sin_addr, b), hton16(peer.sin_port));
        }
        close(sock);
        int r = pump(conn, verbose);
        close(conn);
        return r;
    }

    uint32_t ip = host_resolve(host);
    if (!ip) {
        fprintf(stderr, "nc: cannot resolve %s\n", host);
        return 1;
    }
    addr.sin_addr = ip;

    if (verbose) {
        char b[32];
        fprintf(stderr, "nc: connecting to %s:%d\n", ip_str(ip, b), port);
    }
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "nc: connect: %s\n", strerror(errno));
        return 1;
    }
    if (verbose) fprintf(stderr, "nc: connected\n");

    int r = pump(sock, verbose);
    close(sock);
    return r;
}
