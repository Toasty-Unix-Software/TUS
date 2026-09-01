/*
 * dns.c - the resolver
 *
 * Queries are built by hand (they are small and rigidly shaped) and
 * answers are walked with a decompressor, because every real server
 * uses name compression in the answer section even when the question
 * is a single label.
 */

#include "dns.h"

#include "netif.h"
#include "udp.h"

#include "../core/errno.h"
#include "../core/klib.h"
#include "../drivers/pit/pit.h"

#define DNS_PORT        53
#define DNS_TIMEOUT_MS  3000
#define DNS_RETRIES     2

#define DNS_TYPE_A      1
#define DNS_CLASS_IN    1

#define DNS_CACHE_SIZE  16
#define DNS_CACHE_TTL_MS (5 * 60 * 1000)

struct dns_cache_entry {
    char name[256];
    uint32_t addr[DNS_MAX_ADDRS];
    int count;
    uint64_t added_ms;
};

static struct dns_cache_entry dns_cache[DNS_CACHE_SIZE];
static int dns_cache_next;

uint32_t dns_parse_ipv4(const char *s) {
    uint32_t parts[4];
    int part = 0;

    for (int i = 0; part < 4; i++) {
        uint32_t v = 0;
        int digits = 0;

        while (s[i] >= '0' && s[i] <= '9') {
            v = v * 10 + (uint32_t)(s[i] - '0');
            if (v > 255) return 0;
            digits++;
            i++;
        }
        if (digits == 0) return 0;
        parts[part++] = v;

        if (s[i] == '\0') break;
        if (s[i] != '.') return 0;
    }
    if (part != 4) return 0;

    /* Network byte order: the first octet is the most significant. */
    return htonl((parts[0] << 24) | (parts[1] << 16) |
                 (parts[2] << 8) | parts[3]);
}

static int dns_cache_lookup(const char *name, uint32_t *addrs, int max) {
    uint64_t now = pit_uptime_ms();

    for (int i = 0; i < DNS_CACHE_SIZE; i++) {
        if (dns_cache[i].count == 0) continue;
        if (now - dns_cache[i].added_ms > DNS_CACHE_TTL_MS) {
            dns_cache[i].count = 0;
            continue;
        }
        if (strcmp(dns_cache[i].name, name) != 0) continue;

        int n = dns_cache[i].count < max ? dns_cache[i].count : max;
        for (int j = 0; j < n; j++) addrs[j] = dns_cache[i].addr[j];
        return n;
    }
    return 0;
}

static void dns_cache_add(const char *name, const uint32_t *addrs, int count) {
    struct dns_cache_entry *e = &dns_cache[dns_cache_next];
    dns_cache_next = (dns_cache_next + 1) % DNS_CACHE_SIZE;

    strncpy(e->name, name, sizeof(e->name) - 1);
    e->name[sizeof(e->name) - 1] = '\0';
    e->count = count < DNS_MAX_ADDRS ? count : DNS_MAX_ADDRS;
    for (int i = 0; i < e->count; i++) e->addr[i] = addrs[i];
    e->added_ms = pit_uptime_ms();
}

/* Write "www.example.com" as \3www\7example\3com\0. Returns the number
 * of bytes written, or -1 when the name does not fit the wire format. */
static int dns_encode_name(const char *name, uint8_t *out, int max) {
    int written = 0;
    const char *label = name;

    for (;;) {
        const char *dot = label;
        while (*dot != '\0' && *dot != '.') dot++;

        int len = (int)(dot - label);
        if (len == 0 || len > 63) return -1;
        if (written + len + 1 >= max) return -1;

        out[written++] = (uint8_t)len;
        memcpy(out + written, label, (size_t)len);
        written += len;

        if (*dot == '\0') break;
        label = dot + 1;
    }
    if (written + 1 > max) return -1;
    out[written++] = 0;
    return written;
}

/* Step over a name in a response, following compression pointers only
 * far enough to know where the name ends. Returns the number of bytes
 * the name occupies at `off`, or -1 if it is malformed. */
static int dns_skip_name(const uint8_t *msg, int len, int off) {
    int start = off;

    while (off < len) {
        uint8_t l = msg[off];

        if ((l & 0xc0) == 0xc0) {
            /* A pointer is always the last thing in a name. */
            if (off + 2 > len) return -1;
            return off + 2 - start;
        }
        if (l == 0) return off + 1 - start;

        off += 1 + l;
    }
    return -1;
}

static int dns_query_server(uint32_t server, const char *name,
                            uint32_t *addrs, int max) {
    uint8_t query[512];
    uint8_t reply[512];

    static uint16_t next_id = 1;
    uint16_t id = next_id++;

    /* Header: one question, recursion desired. */
    query[0] = (uint8_t)(id >> 8);
    query[1] = (uint8_t)(id & 0xff);
    query[2] = 0x01;  /* RD */
    query[3] = 0x00;
    query[4] = 0; query[5] = 1;   /* QDCOUNT */
    query[6] = 0; query[7] = 0;   /* ANCOUNT */
    query[8] = 0; query[9] = 0;   /* NSCOUNT */
    query[10] = 0; query[11] = 0; /* ARCOUNT */

    int qlen = dns_encode_name(name, query + 12, (int)sizeof(query) - 12 - 4);
    if (qlen < 0) return -EINVAL;

    int off = 12 + qlen;
    query[off++] = 0; query[off++] = DNS_TYPE_A;
    query[off++] = 0; query[off++] = DNS_CLASS_IN;

    struct udp_pcb *pcb = udp_pcb_new();
    if (!pcb) return -ENOMEM;
    if (udp_bind(pcb, 0, 0) < 0) {
        udp_pcb_free(pcb);
        return -EADDRINUSE;
    }

    int found = 0;

    for (int attempt = 0; attempt < DNS_RETRIES && found == 0; attempt++) {
        if (udp_sendto(pcb, query, (size_t)off, server, DNS_PORT) < 0) {
            continue;
        }

        uint32_t src_ip = 0;
        uint16_t src_port = 0;
        long n = udp_recvfrom(pcb, reply, sizeof(reply), &src_ip, &src_port,
                              DNS_TIMEOUT_MS);
        if (n < 12) continue;
        if (src_ip != server || src_port != DNS_PORT) continue;

        uint16_t reply_id = (uint16_t)((reply[0] << 8) | reply[1]);
        if (reply_id != id) continue;
        if ((reply[3] & 0x0f) != 0) break; /* RCODE set: NXDOMAIN and friends */

        int qdcount = (reply[4] << 8) | reply[5];
        int ancount = (reply[6] << 8) | reply[7];
        int p = 12;

        for (int i = 0; i < qdcount; i++) {
            int skip = dns_skip_name(reply, (int)n, p);
            if (skip < 0) { p = -1; break; }
            p += skip + 4;
        }
        if (p < 0 || p > n) continue;

        for (int i = 0; i < ancount && found < max; i++) {
            int skip = dns_skip_name(reply, (int)n, p);
            if (skip < 0) break;
            p += skip;

            if (p + 10 > n) break;
            uint16_t type = (uint16_t)((reply[p] << 8) | reply[p + 1]);
            uint16_t rdlength = (uint16_t)((reply[p + 8] << 8) | reply[p + 9]);
            p += 10;

            if (p + rdlength > n) break;
            if (type == DNS_TYPE_A && rdlength == 4) {
                uint32_t a;
                memcpy(&a, reply + p, 4); /* already network order */
                addrs[found++] = a;
            }
            p += rdlength;
        }
    }

    udp_pcb_free(pcb);
    return found;
}

int dns_resolve(const char *name, uint32_t *addrs, int max) {
    if (!name || !addrs || max <= 0) return -EINVAL;

    uint32_t literal = dns_parse_ipv4(name);
    if (literal != 0) {
        addrs[0] = literal;
        return 1;
    }

    int cached = dns_cache_lookup(name, addrs, max);
    if (cached > 0) return cached;

    if (!g_netif.up) return -ENETDOWN;
    if (g_netif.dns == 0) return -ECONNREFUSED;

    int found = dns_query_server(g_netif.dns, name, addrs, max);
    if (found > 0) {
        dns_cache_add(name, addrs, found);
        return found;
    }
    return found < 0 ? found : -EHOSTUNREACH;
}
