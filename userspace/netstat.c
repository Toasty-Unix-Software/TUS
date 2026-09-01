/*
 * netstat - show connections and interface statistics
 * Usage: netstat [-i] [-s] [-a]
 */

#include <stdio.h>
#include <string.h>

#include "tusnetutil.h"

static const char *state_name(int state) {
    static const char *names[] = {
        "CLOSED", "LISTEN", "SYN_SENT", "SYN_RCVD", "ESTABLISHED",
        "FIN_WAIT1", "FIN_WAIT2", "CLOSING", "TIME_WAIT", "CLOSE_WAIT",
        "LAST_ACK"
    };
    if (state < 0 || state >= (int)(sizeof(names) / sizeof(names[0]))) {
        return "UNKNOWN";
    }
    return names[state];
}

static void show_connections(void) {
    struct tus_tcp_row rows[64];
    long n = netctl(NETCTL_TCP_DUMP, rows, sizeof(rows));

    if (n <= 0) {
        printf("Active Internet connections\n");
        printf("(none)\n");
        return;
    }

    printf("Active Internet connections\n");
    printf("%-6s %-7s %-7s %-22s %-22s %s\n",
           "Proto", "Recv-Q", "Send-Q", "Local Address",
           "Foreign Address", "State");

    for (long i = 0; i < n; i++) {
        char l[32], r[32], la[48], ra[48];

        sprintf(la, "%s:%u", ip_str(rows[i].local_ip, l), rows[i].local_port);
        if (rows[i].remote_port) {
            sprintf(ra, "%s:%u", ip_str(rows[i].remote_ip, r),
                    rows[i].remote_port);
        } else {
            strcpy(ra, "0.0.0.0:*");
        }

        printf("%-6s %-7u %-7u %-22s %-22s %s\n",
               "tcp", rows[i].rx_queued, rows[i].tx_queued,
               la, ra, state_name(rows[i].state));
    }
}

static void show_interfaces(void) {
    struct tus_ifinfo info;
    if (netctl(NETCTL_GET_IF, &info, sizeof(info)) < 0) {
        printf("netstat: no interface\n");
        return;
    }

    printf("Kernel Interface table\n");
    printf("%-8s %-6s %-10s %-10s %-10s %-10s\n",
           "Iface", "MTU", "RX-OK", "RX-DRP", "TX-OK", "TX-DRP");
    printf("%-8s %-6d %-10llu %-10llu %-10llu %-10llu\n",
           info.name, 1500,
           (unsigned long long)info.rx_packets,
           (unsigned long long)info.rx_dropped,
           (unsigned long long)info.tx_packets,
           (unsigned long long)info.tx_dropped);
}

int main(int argc, char **argv) {
    int interfaces = 0, stats = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) interfaces = 1;
        else if (strcmp(argv[i], "-s") == 0) stats = 1;
        else if (strcmp(argv[i], "-a") == 0) { /* the default view */ }
    }

    if (interfaces) {
        show_interfaces();
        return 0;
    }
    if (stats) {
        struct tus_ifinfo info;
        if (netctl(NETCTL_GET_IF, &info, sizeof(info)) == 0) {
            printf("Ip:\n    %llu packets received\n    %llu packets sent\n",
                   (unsigned long long)info.rx_packets,
                   (unsigned long long)info.tx_packets);
            printf("    %llu incoming packets dropped\n    %llu receive errors\n",
                   (unsigned long long)info.rx_dropped,
                   (unsigned long long)info.rx_errors);
        }
        return 0;
    }

    show_connections();
    return 0;
}
