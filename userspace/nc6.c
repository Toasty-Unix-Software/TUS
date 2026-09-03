/*
 * nc6 - netcat over IPv6 (TCP6, plus a -u UDP6 mode)
 *
 * Usage: nc6 [-v] addr port        connect (TCP6) and pump stdin/stdout
 *        nc6 -l port [-v]          listen (TCP6) for one connection
 *        nc6 -u [-v] addr port     send one UDP6 datagram per input line,
 *                                  print whatever comes back
 *        nc6 -u -l port [-v]       UDP6 echo: reply with whatever arrives
 *
 * A minimal test/debug tool for the TCP6/UDP6 transport (kernel/net/
 * tcp6.c, udp6.c) mirroring nc.c's AF_INET shape exactly, so the two
 * protocol families can be exercised the same way from the shell.
 */

#include <errno.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define AF_INET6_LOCAL 10

struct sockaddr_in6_t {
    uint16_t sin6_family;
    uint16_t sin6_port;
    uint32_t sin6_flowinfo;
    uint8_t sin6_addr[16];
    uint32_t sin6_scope_id;
};

static uint16_t hton16(uint16_t v) { return (uint16_t)((v << 8) | (v >> 8)); }

/* Same parser shape as ipv6_parse() in the kernel - "::" compression,
 * standard groups - kept small and separate rather than shared, since
 * this is userspace and the kernel's lives in kernel/net/ipv6.c. */
static int parse6(const char *s, uint8_t out[16]) {
    uint16_t groups[8];
    int ngroups = 0, compress_at = -1;
    const char *p = s;

    if (p[0] == ':' && p[1] == ':') {
        compress_at = 0;
        p += 2;
        if (*p == '\0') { memset(out, 0, 16); return 0; }
    }
    while (*p != '\0' && ngroups < 8) {
        uint32_t value = 0;
        int digits = 0;
        while (digits < 4) {
            char c = *p;
            int nibble;
            if (c >= '0' && c <= '9') nibble = c - '0';
            else if (c >= 'a' && c <= 'f') nibble = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F') nibble = c - 'A' + 10;
            else break;
            value = (value << 4) | (uint32_t)nibble;
            p++; digits++;
        }
        if (digits == 0) return -1;
        groups[ngroups++] = (uint16_t)value;

        if (*p == ':') {
            p++;
            if (*p == ':') {
                if (compress_at >= 0) return -1;
                compress_at = ngroups;
                p++;
                if (*p == '\0') break;
            }
        } else if (*p == '\0') {
            break;
        } else {
            return -1;
        }
    }
    if (*p != '\0') return -1;

    memset(out, 0, 16);
    if (compress_at < 0) {
        if (ngroups != 8) return -1;
        for (int i = 0; i < 8; i++) {
            out[i*2] = (uint8_t)(groups[i] >> 8);
            out[i*2+1] = (uint8_t)groups[i];
        }
        return 0;
    }
    int tail = ngroups - compress_at;
    for (int i = 0; i < compress_at; i++) {
        out[i*2] = (uint8_t)(groups[i] >> 8);
        out[i*2+1] = (uint8_t)groups[i];
    }
    for (int i = 0; i < tail; i++) {
        int d = 8 - tail + i;
        uint16_t g = groups[compress_at + i];
        out[d*2] = (uint8_t)(g >> 8);
        out[d*2+1] = (uint8_t)g;
    }
    return 0;
}

static void fmt6(const uint8_t a[16], char *out) {
    static const char hex[] = "0123456789abcdef";
    char *p = out;
    for (int i = 0; i < 8; i++) {
        uint16_t g = (uint16_t)((a[i*2] << 8) | a[i*2+1]);
        int lead = 1;
        for (int shift = 12; shift >= 0; shift -= 4) {
            uint8_t nib = (uint8_t)((g >> shift) & 0xf);
            if (nib != 0 || !lead || shift == 0) { *p++ = hex[nib]; lead = 0; }
        }
        if (i != 7) *p++ = ':';
    }
    *p = '\0';
}

static int pump_stream(int sock, int verbose) {
    struct pollfd fds[2];
    char buf[2048];
    fds[0].fd = sock; fds[0].events = POLLIN;
    fds[1].fd = 0;    fds[1].events = POLLIN;

    for (;;) {
        int n = poll(fds, 2, -1);
        if (n < 0) { if (errno == EINTR) continue; return 1; }

        if (fds[0].revents & (POLLIN | POLLHUP | POLLERR)) {
            long r = read(sock, buf, sizeof(buf));
            if (r > 0) { write(1, buf, (size_t)r); continue; }
            if (verbose) fprintf(stderr, "nc6: connection closed\n");
            return r < 0 ? 1 : 0;
        }
        if (fds[1].revents & POLLIN) {
            long r = read(0, buf, sizeof(buf));
            if (r <= 0) {
                shutdown(sock, SHUT_WR);
                fds[1].fd = -1;
                continue;
            }
            if (write(sock, buf, (size_t)r) < 0) {
                fprintf(stderr, "nc6: write failed\n");
                return 1;
            }
        }
    }
}

static int udp_client(int sock, struct sockaddr_in6_t *dst, int verbose) {
    char line[2048];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (sendto(sock, line, len, 0, (struct sockaddr *)dst,
                  sizeof(*dst)) < 0) {
            fprintf(stderr, "nc6: sendto: %s\n", strerror(errno));
            return 1;
        }
        char reply[2048];
        struct sockaddr_in6_t from;
        socklen_t flen = sizeof(from);
        long r = recvfrom(sock, reply, sizeof(reply) - 1, 0,
                          (struct sockaddr *)&from, &flen);
        if (r < 0) {
            if (verbose) fprintf(stderr, "nc6: recvfrom: %s\n", strerror(errno));
            continue;
        }
        reply[r] = '\0';
        write(1, reply, (size_t)r);
    }
    return 0;
}

static int udp_echo_server(int sock, int verbose) {
    char buf[2048];
    for (;;) {
        struct sockaddr_in6_t from;
        socklen_t flen = sizeof(from);
        long r = recvfrom(sock, buf, sizeof(buf), 0,
                          (struct sockaddr *)&from, &flen);
        if (r < 0) return 1;
        if (verbose) {
            char b[48];
            fmt6(from.sin6_addr, b);
            fprintf(stderr, "nc6: %ld bytes from [%s]:%u\n", r, b,
                   hton16(from.sin6_port));
        }
        sendto(sock, buf, (size_t)r, 0, (struct sockaddr *)&from, flen);
    }
}

int main(int argc, char **argv) {
    int listen_mode = 0, verbose = 0, udp_mode = 0;
    const char *host = NULL;
    int port = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) listen_mode = 1;
        else if (strcmp(argv[i], "-v") == 0) verbose = 1;
        else if (strcmp(argv[i], "-u") == 0) udp_mode = 1;
        else if (argv[i][0] == '-') {
            fprintf(stderr, "usage: nc6 [-u] [-v] addr port | nc6 [-u] -l port\n");
            return 1;
        } else if (!listen_mode && host == NULL) host = argv[i];
        else port = atoi(argv[i]);
    }
    if (port == 0 && host != NULL && listen_mode) {
        port = atoi(host);
        host = NULL;
    }
    if (port <= 0 || port > 65535 || (!listen_mode && host == NULL)) {
        fprintf(stderr, "usage: nc6 [-u] [-v] addr port | nc6 [-u] -l port\n");
        return 1;
    }

    int sock = socket(AF_INET6_LOCAL, udp_mode ? SOCK_DGRAM : SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "nc6: socket: %s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in6_t addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6_LOCAL;
    addr.sin6_port = hton16((uint16_t)port);

    if (listen_mode) {
        if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            fprintf(stderr, "nc6: bind: %s\n", strerror(errno));
            return 1;
        }
        if (verbose) fprintf(stderr, "nc6: listening on port %d\n", port);

        if (udp_mode) return udp_echo_server(sock, verbose);

        if (listen(sock, 1) < 0) {
            fprintf(stderr, "nc6: listen: %s\n", strerror(errno));
            return 1;
        }
        struct sockaddr_in6_t peer;
        socklen_t plen = sizeof(peer);
        int conn = accept(sock, (struct sockaddr *)&peer, &plen);
        if (conn < 0) {
            fprintf(stderr, "nc6: accept: %s\n", strerror(errno));
            return 1;
        }
        if (verbose) {
            char b[48];
            fmt6(peer.sin6_addr, b);
            fprintf(stderr, "nc6: connection from [%s]:%u\n", b,
                   hton16(peer.sin6_port));
        }
        close(sock);
        int r = pump_stream(conn, verbose);
        close(conn);
        return r;
    }

    if (parse6(host, addr.sin6_addr) != 0) {
        fprintf(stderr, "nc6: cannot parse address %s\n", host);
        return 1;
    }

    if (verbose) {
        char b[48];
        fmt6(addr.sin6_addr, b);
        fprintf(stderr, "nc6: %s to [%s]:%d\n",
               udp_mode ? "sending" : "connecting", b, port);
    }

    if (udp_mode) return udp_client(sock, &addr, verbose);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "nc6: connect: %s\n", strerror(errno));
        return 1;
    }
    if (verbose) fprintf(stderr, "nc6: connected\n");

    int r = pump_stream(sock, verbose);
    close(sock);
    return r;
}
