/*
 * ifconfig - show and change the interface configuration
 *
 * Usage: ifconfig
 *        ifconfig eth0
 *        ifconfig eth0 <addr> [netmask <mask>] [gw <addr>] [dns <addr>]
 *
 * Reconfiguring needs root, which the kernel enforces rather than this
 * program: netctl(NETCTL_SET_IF) refuses a non-zero euid.
 */

#include <stdio.h>
#include <string.h>

#include "tusnetutil.h"

static void print_if(const struct tus_ifinfo *info) {
    char buf[32];

    printf("%s      Link encap:Ethernet  HWaddr %s\n",
           info->name, mac_str(info->mac, buf));
    printf("          inet addr:%s", ip_str(info->ip, buf));
    printf("  Mask:%s", ip_str(info->netmask, buf));
    printf("  Bcast:%s\n", ip_str(info->ip | ~info->netmask, buf));
    printf("          gateway:%s", ip_str(info->gateway, buf));
    printf("  dns:%s\n", ip_str(info->dns, buf));
    printf("          %s  MTU:1500\n",
           info->up ? "UP BROADCAST RUNNING" : "DOWN");
    printf("          RX packets:%llu dropped:%llu errors:%llu\n",
           (unsigned long long)info->rx_packets,
           (unsigned long long)info->rx_dropped,
           (unsigned long long)info->rx_errors);
    printf("          TX packets:%llu dropped:%llu\n",
           (unsigned long long)info->tx_packets,
           (unsigned long long)info->tx_dropped);

    struct tus_if6info info6;
    char buf6[40];
    if (netctl(NETCTL_GET_IF6, &info6, sizeof(info6)) == 0) {
        if (info6.have_link_local) {
            printf("          inet6 addr: %s/64 Scope:Link\n",
                   ipv6_str(info6.link_local, buf6));
        }
        if (info6.have_global) {
            printf("          inet6 addr: %s/64 Scope:Global\n",
                   ipv6_str(info6.global, buf6));
        }
    }
}

int main(int argc, char **argv) {
    struct tus_ifinfo info;

    if (netctl(NETCTL_GET_IF, &info, sizeof(info)) < 0) {
        fprintf(stderr, "ifconfig: no network interface\n");
        return 1;
    }

    if (argc <= 2) {
        if (argc == 2 && strcmp(argv[1], info.name) != 0) {
            fprintf(stderr, "ifconfig: %s: no such interface\n", argv[1]);
            return 1;
        }
        print_if(&info);
        return 0;
    }

    /* ifconfig eth0 10.0.2.15 netmask 255.255.255.0 gw 10.0.2.2 */
    int i = 2;
    if (argv[i][0] >= '0' && argv[i][0] <= '9') {
        uint32_t addr = ip_parse(argv[i]);
        if (!addr) {
            fprintf(stderr, "ifconfig: bad address '%s'\n", argv[i]);
            return 1;
        }
        info.ip = addr;
        i++;
    }

    for (; i < argc; i++) {
        if (i + 1 >= argc) {
            fprintf(stderr, "ifconfig: %s needs a value\n", argv[i]);
            return 1;
        }
        uint32_t addr = ip_parse(argv[i + 1]);
        if (!addr) {
            fprintf(stderr, "ifconfig: bad address '%s'\n", argv[i + 1]);
            return 1;
        }

        if (strcmp(argv[i], "netmask") == 0) {
            info.netmask = addr;
        } else if (strcmp(argv[i], "gw") == 0 ||
                   strcmp(argv[i], "gateway") == 0) {
            info.gateway = addr;
        } else if (strcmp(argv[i], "dns") == 0) {
            info.dns = addr;
        } else {
            fprintf(stderr, "ifconfig: unknown option '%s'\n", argv[i]);
            return 1;
        }
        i++;
    }

    if (netctl(NETCTL_SET_IF, &info, sizeof(info)) < 0) {
        fprintf(stderr, "ifconfig: permission denied (try doas)\n");
        return 1;
    }

    netctl(NETCTL_GET_IF, &info, sizeof(info));
    print_if(&info);
    return 0;
}
