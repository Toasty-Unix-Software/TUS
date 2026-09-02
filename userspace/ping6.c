/*
 * ping6 - send ICMPv6 echo requests to a host
 * Usage: ping6 [-c count] [-i interval_ms] [-s size] [-w timeout_ms] addr
 *
 * Literal addresses only (fe80::1, ff02::1, ...): TUS's DNS resolver
 * is IPv4-only (see kernel/net/ipv6.c's file header for the rest of
 * what IPv6 support here does and does not cover), so there is no
 * AAAA lookup to fall back to.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tusnetutil.h"

int main(int argc, char **argv) {
    const char *host = NULL;
    int count = 4;
    int timeout_ms = 1000;
    int interval_ms = 1000;
    int size = 56;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            count = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
            timeout_ms = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            interval_ms = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            size = atoi(argv[++i]);
        } else if (argv[i][0] != '-') {
            host = argv[i];
        } else {
            fprintf(stderr, "usage: ping6 [-c count] [-w ms] [-s size] addr\n");
            return 1;
        }
    }

    if (!host) {
        fprintf(stderr, "usage: ping6 [-c count] [-w ms] [-s size] addr\n");
        return 1;
    }

    uint8_t addr[16];
    if (ipv6_parse(host, addr) != 0) {
        fprintf(stderr, "ping6: not a valid IPv6 address: %s\n", host);
        return 1;
    }

    char buf[40];
    printf("PING6 %s %d bytes of data.\n", ipv6_str(addr, buf), size);

    int sent = 0, received = 0;
    long total_rtt = 0, min_rtt = -1, max_rtt = 0;

    for (int seq = 0; count <= 0 || seq < count; seq++) {
        struct tus_ping6 p;
        memset(&p, 0, sizeof(p));
        memcpy(p.dst, addr, 16);
        p.timeout_ms = (uint32_t)timeout_ms;
        p.id = (uint16_t)(getpid() & 0xffff);
        p.seq = (uint16_t)seq;
        p.payload_len = (uint32_t)size;

        sent++;
        long r = netctl(NETCTL_PING6, &p, sizeof(p));

        if (r < 0 || p.rtt_ms < 0) {
            printf("Request timeout for icmp_seq %d\n", seq);
        } else {
            received++;
            total_rtt += p.rtt_ms;
            if (min_rtt < 0 || p.rtt_ms < min_rtt) min_rtt = p.rtt_ms;
            if (p.rtt_ms > max_rtt) max_rtt = p.rtt_ms;
            printf("%d bytes from %s: icmp_seq=%d time=%d ms\n",
                   size, ipv6_str(addr, buf), seq, p.rtt_ms);
        }

        if ((count <= 0 || seq + 1 < count) && interval_ms > 0) {
            usleep((useconds_t)interval_ms * 1000);
        }
    }

    printf("\n--- %s ping6 statistics ---\n", host);
    printf("%d packets transmitted, %d received, %d%% packet loss\n",
           sent, received, sent ? (sent - received) * 100 / sent : 0);
    if (received > 0) {
        printf("rtt min/avg/max = %ld/%ld/%ld ms\n",
               min_rtt, total_rtt / received, max_rtt);
    }
    return received > 0 ? 0 : 1;
}
