#include "hostname.h"
#include "errno.h"

static char g_hostname[HOSTNAME_MAX + 1] = "tus";

const char *hostname_get(void) {
    return g_hostname;
}

int hostname_set(const char *name, size_t len) {
    if (len == 0 || len > HOSTNAME_MAX) {
        return -EINVAL;
    }
    size_t n = 0;
    while (n < len && name[n] != '\0') {
        g_hostname[n] = name[n];
        n++;
    }
    g_hostname[n] = '\0';
    return 0;
}
