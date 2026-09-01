/*
 * host - look a name up in the DNS
 * Usage: host <name>
 */

#include <stdio.h>
#include <string.h>

#include "tusnetutil.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: host <name>\n");
        return 1;
    }

    struct tus_resolve req;
    memset(&req, 0, sizeof(req));
    strncpy(req.name, argv[1], sizeof(req.name) - 1);

    long n = netctl(NETCTL_RESOLVE, &req, sizeof(req));
    if (n < 0 || req.count <= 0) {
        printf("host: %s not found\n", argv[1]);
        return 1;
    }

    char buf[32];
    for (int i = 0; i < req.count; i++) {
        printf("%s has address %s\n", argv[1], ip_str(req.addr[i], buf));
    }
    return 0;
}
