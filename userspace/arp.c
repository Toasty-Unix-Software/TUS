/*
 * arp - show the kernel's ARP cache
 * Usage: arp [-n]
 */

#include <stdio.h>
#include <string.h>

#include "tusnetutil.h"

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    struct tus_arp_row rows[32];
    long n = netctl(NETCTL_ARP_DUMP, rows, sizeof(rows));

    if (n < 0) {
        fprintf(stderr, "arp: cannot read the ARP cache\n");
        return 1;
    }
    if (n == 0) {
        printf("arp: cache is empty\n");
        return 0;
    }

    printf("%-16s %-10s %-20s %s\n", "Address", "HWtype", "HWaddress", "Iface");
    for (long i = 0; i < n; i++) {
        char ip[32], mac[32];
        printf("%-16s %-10s %-20s %s\n",
               ip_str(rows[i].ip, ip), "ether",
               mac_str(rows[i].mac, mac), "eth0");
    }
    return 0;
}
