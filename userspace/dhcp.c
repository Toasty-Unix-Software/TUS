/*
 * dhcp - run a DHCPv4 lease exchange and print the result
 *
 * Wraps NETCTL_DHCP, which does the actual DISCOVER/OFFER/REQUEST/ACK
 * in the kernel and, on success, overwrites g_netif in place - so a
 * plain `ifconfig` right afterward shows the leased address.
 */

#include <stdio.h>

#include "tusnetutil.h"

int main(void) {
    long r = netctl(NETCTL_DHCP, NULL, 0);
    if (r < 0) {
        fprintf(stderr, "dhcp: %s\n",
                r == -1 ? "permission denied (try doas)" : "no lease obtained");
        return 1;
    }

    struct tus_ifinfo info;
    if (netctl(NETCTL_GET_IF, &info, sizeof(info)) == 0) {
        char buf[32];
        printf("dhcp: leased %s", ip_str(info.ip, buf));
        printf(" netmask %s", ip_str(info.netmask, buf));
        printf(" gateway %s\n", ip_str(info.gateway, buf));
    }
    return 0;
}
