/*
 * route - show or change the routing table
 *
 * TUS routes with one interface, one prefix and one default gateway,
 * so the "table" is two lines and `route add default gw X` is the only
 * change worth making.
 *
 * Usage: route
 *        route add default gw <addr>
 *        route del default
 */

#include <stdio.h>
#include <string.h>

#include "tusnetutil.h"

int main(int argc, char **argv) {
    struct tus_ifinfo info;

    if (netctl(NETCTL_GET_IF, &info, sizeof(info)) < 0) {
        fprintf(stderr, "route: no network interface\n");
        return 1;
    }

    if (argc == 1) {
        char a[32], b[32];
        printf("%-18s %-16s %-16s %s\n",
               "Destination", "Gateway", "Genmask", "Iface");
        printf("%-18s %-16s %-16s %s\n",
               ip_str(info.ip & info.netmask, a), "0.0.0.0",
               ip_str(info.netmask, b), info.name);
        if (info.gateway) {
            printf("%-18s %-16s %-16s %s\n",
                   "0.0.0.0", ip_str(info.gateway, a), "0.0.0.0", info.name);
        }
        return 0;
    }

    if (argc >= 2 && strcmp(argv[1], "del") == 0) {
        info.gateway = 0;
    } else if (argc >= 5 && strcmp(argv[1], "add") == 0 &&
               strcmp(argv[2], "default") == 0 && strcmp(argv[3], "gw") == 0) {
        uint32_t gw = ip_parse(argv[4]);
        if (!gw) {
            fprintf(stderr, "route: bad gateway '%s'\n", argv[4]);
            return 1;
        }
        info.gateway = gw;
    } else {
        fprintf(stderr, "usage: route | route add default gw <addr> | route del default\n");
        return 1;
    }

    if (netctl(NETCTL_SET_IF, &info, sizeof(info)) < 0) {
        fprintf(stderr, "route: permission denied (try doas)\n");
        return 1;
    }
    return 0;
}
