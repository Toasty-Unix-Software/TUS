/*
 * ipv6.c - IPv6: SLAAC, NDP and ICMPv6 (see ipv6.h for scope)
 */

#include "ipv6.h"

#include "ip.h"
#include "netif.h"
#include "udp6.h"
#include "tcp6.h"

#include "../core/klib.h"
#include "../drivers/pit/pit.h"

static const uint8_t g_all_zero[16];

/* klib has no memcmp; addresses are 16 bytes, so a byte loop is fine. */
static int mem_eq(const void *a, const void *b, size_t n) {
    const uint8_t *pa = (const uint8_t *)a, *pb = (const uint8_t *)b;
    for (size_t i = 0; i < n; i++) {
        if (pa[i] != pb[i]) return 0;
    }
    return 1;
}

static uint8_t g_link_local[16];
static bool    g_have_link_local;
static uint8_t g_global[16];
static bool    g_have_global;

/* ---- address helpers ---- */

static bool addr_is_multicast(const uint8_t a[16]) { return a[0] == 0xff; }
static bool addr_eq(const uint8_t a[16], const uint8_t b[16]) {
    return mem_eq(a, b, 16) != 0;
}
static bool addr_is_zero(const uint8_t a[16]) {
    return mem_eq(a, g_all_zero, 16) != 0;
}

static void multicast_mac(const uint8_t a[16], uint8_t mac[6]) {
    /* RFC 2464: 33:33 followed by the low 32 bits of the address. */
    mac[0] = 0x33; mac[1] = 0x33;
    mac[2] = a[12]; mac[3] = a[13]; mac[4] = a[14]; mac[5] = a[15];
}

static void solicited_node_addr(const uint8_t target[16], uint8_t out[16]) {
    memset(out, 0, 16);
    out[0] = 0xff; out[1] = 0x02;
    out[11] = 0x01; out[12] = 0xff;
    out[13] = target[13]; out[14] = target[14]; out[15] = target[15];
}

void ipv6_format(const uint8_t addr[16], char *out, size_t outsz) {
    static const char hex[] = "0123456789abcdef";
    char buf[40];
    char *p = buf;
    for (int i = 0; i < 8; i++) {
        uint16_t group = (uint16_t)((addr[i * 2] << 8) | addr[i * 2 + 1]);
        bool leading = true;
        for (int shift = 12; shift >= 0; shift -= 4) {
            uint8_t nibble = (uint8_t)((group >> shift) & 0xf);
            if (nibble != 0 || !leading || shift == 0) {
                *p++ = hex[nibble];
                leading = false;
            }
        }
        if (i != 7) *p++ = ':';
    }
    *p = '\0';
    strncpy(out, buf, outsz - 1);
    out[outsz - 1] = '\0';
}

int ipv6_parse(const char *s, uint8_t out[16]) {
    uint16_t groups[8];
    int ngroups = 0;
    int compress_at = -1;
    const char *p = s;

    if (p[0] == ':' && p[1] == ':') {
        compress_at = 0;
        p += 2;
        if (*p == '\0') {
            memset(out, 0, 16);
            return 0;
        }
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
            p++;
            digits++;
        }
        if (digits == 0) {
            return -1;
        }
        groups[ngroups++] = (uint16_t)value;

        if (*p == ':') {
            p++;
            if (*p == ':') {
                if (compress_at >= 0) return -1; /* only one "::" allowed */
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
    if (*p != '\0') {
        return -1; /* more than 8 groups, or junk left over */
    }

    memset(out, 0, 16);
    if (compress_at < 0) {
        if (ngroups != 8) return -1;
        for (int i = 0; i < 8; i++) {
            out[i * 2] = (uint8_t)(groups[i] >> 8);
            out[i * 2 + 1] = (uint8_t)groups[i];
        }
        return 0;
    }

    int tail = ngroups - compress_at;
    for (int i = 0; i < compress_at; i++) {
        out[i * 2] = (uint8_t)(groups[i] >> 8);
        out[i * 2 + 1] = (uint8_t)groups[i];
    }
    for (int i = 0; i < tail; i++) {
        int dst_group = 8 - tail + i;
        uint16_t g = groups[compress_at + i];
        out[dst_group * 2] = (uint8_t)(g >> 8);
        out[dst_group * 2 + 1] = (uint8_t)g;
    }
    return 0;
}

/* ---- checksum ---- */

uint16_t transport_checksum6(const uint8_t src[16], const uint8_t dst[16],
                             uint8_t next_header, const void *h, uint32_t hlen,
                             const void *d, uint32_t dlen) {
    uint8_t buf[1024];
    if (40 + hlen + dlen > sizeof(buf)) {
        return 0; /* caller-sized payloads never hit this */
    }
    uint8_t *p = buf;
    memcpy(p, src, 16); p += 16;
    memcpy(p, dst, 16); p += 16;
    *(uint32_t *)p = htonl(hlen + dlen); p += 4;
    p[0] = 0; p[1] = 0; p[2] = 0; p[3] = next_header; p += 4;
    memcpy(p, h, hlen); p += hlen;
    if (dlen > 0) { memcpy(p, d, dlen); p += dlen; }
    return net_checksum(buf, (uint32_t)(p - buf));
}

static uint16_t icmpv6_checksum(const uint8_t src[16], const uint8_t dst[16],
                                uint16_t upper_len, const void *h, uint16_t hlen,
                                const void *d, uint16_t dlen) {
    (void)upper_len; /* == hlen+dlen; kept as a documented parameter */
    return transport_checksum6(src, dst, IPV6_NEXT_ICMPV6, h, hlen, d, dlen);
}

/* ---- neighbor cache ---- */

#define NDP_CACHE_SIZE 16

struct ndp_entry {
    bool valid;
    uint8_t addr[16];
    uint8_t mac[6];
    uint64_t added_ms;
};
static struct ndp_entry g_ndp_cache[NDP_CACHE_SIZE];

static void ndp_cache_add(const uint8_t addr[16], const uint8_t mac[6]) {
    int free_slot = -1, oldest = 0;
    for (int i = 0; i < NDP_CACHE_SIZE; i++) {
        if (g_ndp_cache[i].valid && addr_eq(g_ndp_cache[i].addr, addr)) {
            memcpy(g_ndp_cache[i].mac, mac, 6);
            g_ndp_cache[i].added_ms = pit_uptime_ms();
            return;
        }
        if (!g_ndp_cache[i].valid && free_slot < 0) free_slot = i;
        if (g_ndp_cache[i].added_ms < g_ndp_cache[oldest].added_ms) oldest = i;
    }
    int slot = free_slot >= 0 ? free_slot : oldest;
    memcpy(g_ndp_cache[slot].addr, addr, 16);
    memcpy(g_ndp_cache[slot].mac, mac, 6);
    g_ndp_cache[slot].added_ms = pit_uptime_ms();
    g_ndp_cache[slot].valid = true;
}

static bool ndp_cache_lookup(const uint8_t addr[16], uint8_t mac[6]) {
    for (int i = 0; i < NDP_CACHE_SIZE; i++) {
        if (g_ndp_cache[i].valid && addr_eq(g_ndp_cache[i].addr, addr)) {
            memcpy(mac, g_ndp_cache[i].mac, 6);
            return true;
        }
    }
    return false;
}

int ndp_cache_dump(struct ndp_entry_info *out, int max) {
    int n = 0;
    for (int i = 0; i < NDP_CACHE_SIZE && n < max; i++) {
        if (!g_ndp_cache[i].valid) continue;
        memcpy(out[n].addr, g_ndp_cache[i].addr, 16);
        memcpy(out[n].mac, g_ndp_cache[i].mac, 6);
        n++;
    }
    return n;
}

/* ---- send path ---- */

static int ipv6_send(const uint8_t dst_mac[6], const uint8_t src[16],
                     const uint8_t dst[16], uint8_t next_header, uint8_t hop_limit,
                     const void *h, uint16_t hlen, const void *d, uint16_t dlen) {
    struct ipv6_header ip6;
    ip6.ver_tc_flow = htonl(6u << 28);
    ip6.payload_len = htons((uint16_t)(hlen + dlen));
    ip6.next_header = next_header;
    ip6.hop_limit = hop_limit;
    memcpy(ip6.src, src, 16);
    memcpy(ip6.dst, dst, 16);

    uint8_t payload[NETIF_MTU];
    if ((uint32_t)(hlen + dlen) > sizeof(payload)) return -1;
    if (hlen) memcpy(payload, h, hlen);
    if (dlen) memcpy(payload + hlen, d, dlen);

    return eth_send(dst_mac, ETH_TYPE_IPV6, &ip6, sizeof(ip6), payload,
                    (uint16_t)(hlen + dlen));
}

/* Resolve `dst` to a destination MAC: multicast addresses translate
 * directly (RFC 2464), unicast ones go through the neighbor cache,
 * sending a Neighbor Solicitation and waiting if it is a miss. */
static int resolve_dst_mac(const uint8_t dst[16], uint8_t mac[6]) {
    if (addr_is_multicast(dst)) {
        multicast_mac(dst, mac);
        return 0;
    }
    if (ndp_cache_lookup(dst, mac)) {
        return 0;
    }
    if (!g_have_link_local) {
        return -1;
    }

    uint8_t sn_addr[16];
    solicited_node_addr(dst, sn_addr);
    uint8_t sn_mac[6];
    multicast_mac(sn_addr, sn_mac);

    struct {
        struct icmpv6_header icmp;
        uint32_t reserved;
        uint8_t target[16];
        uint8_t opt_type;
        uint8_t opt_len;
        uint8_t opt_mac[6];
    } __attribute__((packed)) ns;
    memset(&ns, 0, sizeof(ns));
    ns.icmp.type = ICMPV6_NS;
    memcpy(ns.target, dst, 16);
    ns.opt_type = 1; /* source link-layer address */
    ns.opt_len = 1;  /* 8 bytes */
    memcpy(ns.opt_mac, g_netif.mac, 6);
    ns.icmp.checksum = icmpv6_checksum(g_link_local, sn_addr, sizeof(ns),
                                       &ns, sizeof(ns), NULL, 0);

    for (int attempt = 0; attempt < 3; attempt++) {
        ipv6_send(sn_mac, g_link_local, sn_addr, IPV6_NEXT_ICMPV6, 255,
                 &ns, sizeof(ns), NULL, 0);
        uint64_t deadline = pit_uptime_ms() + 500;
        while (pit_uptime_ms() < deadline) {
            if (ndp_cache_lookup(dst, mac)) return 0;
            net_wait(deadline);
        }
    }
    return -1;
}

/* ---- ICMPv6 echo ---- */

struct icmpv6_ping_slot {
    bool waiting;
    bool got_reply;
    uint8_t from[16];
    uint16_t id, seq;
    uint64_t sent_ms, reply_ms;
};
static struct icmpv6_ping_slot g_ping6;

int icmpv6_send_echo(const uint8_t dst[16], uint16_t id, uint16_t seq,
                     const uint8_t *data, uint16_t len) {
    if (!g_have_link_local) return -1;

    uint8_t mac[6];
    if (resolve_dst_mac(dst, mac) != 0) return -1;

    uint8_t buf[8 + 512];
    uint16_t send_len = len;
    if (send_len > 512) send_len = 512;
    struct icmpv6_header *icmp = (struct icmpv6_header *)buf;
    icmp->type = ICMPV6_ECHO_REQUEST;
    icmp->code = 0;
    icmp->checksum = 0;
    buf[4] = (uint8_t)(id >> 8); buf[5] = (uint8_t)id;
    buf[6] = (uint8_t)(seq >> 8); buf[7] = (uint8_t)seq;
    if (send_len) memcpy(buf + 8, data, send_len);

    icmp->checksum = icmpv6_checksum(g_link_local, dst,
                                     (uint16_t)(8 + send_len), buf, 8,
                                     buf + 8, send_len);

    g_ping6.waiting = true;
    g_ping6.got_reply = false;
    memcpy(g_ping6.from, dst, 16);
    g_ping6.id = id;
    g_ping6.seq = seq;
    g_ping6.sent_ms = pit_uptime_ms();

    return ipv6_send(mac, g_link_local, dst, IPV6_NEXT_ICMPV6, 64,
                     buf, 8, buf + 8, send_len);
}

long icmpv6_wait_reply(const uint8_t dst[16], uint16_t id, uint16_t seq,
                       uint32_t timeout_ms) {
    uint64_t deadline = pit_uptime_ms() + timeout_ms;
    while (pit_uptime_ms() < deadline) {
        if (g_ping6.got_reply && g_ping6.id == id && g_ping6.seq == seq &&
            addr_eq(g_ping6.from, dst)) {
            g_ping6.waiting = false;
            return (long)(g_ping6.reply_ms - g_ping6.sent_ms);
        }
        net_wait(deadline);
    }
    g_ping6.waiting = false;
    return -1;
}

/* ---- receive path ---- */

static void handle_ns(const uint8_t src[16], const uint8_t *icmp_data, uint16_t len) {
    if (len < 4 + 16) return;
    const uint8_t *target = icmp_data + 4;

    bool for_us = (g_have_link_local && addr_eq(target, g_link_local)) ||
                  (g_have_global && addr_eq(target, g_global));
    if (!for_us) return;

    /* Learn the solicitor's address while we are here - the option
     * carries it and we are about to talk straight back to them. */
    if (len >= 4 + 16 + 8 && icmp_data[4 + 16] == 1) {
        ndp_cache_add(src, icmp_data + 4 + 16 + 2);
    }

    struct {
        struct icmpv6_header icmp;
        uint8_t flags[4];
        uint8_t target[16];
        uint8_t opt_type;
        uint8_t opt_len;
        uint8_t opt_mac[6];
    } __attribute__((packed)) na;
    memset(&na, 0, sizeof(na));
    na.icmp.type = ICMPV6_NA;
    na.flags[0] = 0x60; /* solicited + override */
    memcpy(na.target, target, 16);
    na.opt_type = 2; /* target link-layer address */
    na.opt_len = 1;
    memcpy(na.opt_mac, g_netif.mac, 6);

    const uint8_t *reply_dst = addr_is_zero(src) ? NULL : src;
    uint8_t dst_addr[16];
    if (reply_dst == NULL) {
        memset(dst_addr, 0, 16);
        dst_addr[0] = 0xff; dst_addr[1] = 0x02; dst_addr[15] = 0x01; /* ff02::1 */
        reply_dst = dst_addr;
    }
    uint8_t dst_mac[6];
    if (resolve_dst_mac(reply_dst, dst_mac) != 0) return;

    na.icmp.checksum = icmpv6_checksum(target, reply_dst, sizeof(na),
                                       &na, sizeof(na), NULL, 0);
    ipv6_send(dst_mac, target, reply_dst, IPV6_NEXT_ICMPV6, 255,
             &na, sizeof(na), NULL, 0);
}

static void handle_na(const uint8_t src[16], const uint8_t *icmp_data, uint16_t len) {
    if (len < 4 + 16) return;
    if (len >= 4 + 16 + 8 && icmp_data[4 + 16] == 2) {
        ndp_cache_add(src, icmp_data + 4 + 16 + 2);
    } else {
        ndp_cache_add(src, (const uint8_t[6]){0, 0, 0, 0, 0, 0});
    }
}

/* Prefix Information option (type 3, RFC 4861 4.6.2): derive our
 * global SLAAC address (autonomous flag) from the first advertised
 * /64 on-link prefix, keeping the same interface identifier our
 * link-local address already uses. */
static void handle_ra(const uint8_t *icmp_data, uint16_t len) {
    if (g_have_global || len < 12) return;

    uint16_t off = 12; /* past cur-hop-limit/flags/lifetime/reachable/retrans */
    while (off + 2 <= len) {
        uint8_t opt_type = icmp_data[off];
        uint8_t opt_len8 = icmp_data[off + 1];
        if (opt_len8 == 0) break;
        uint16_t opt_len = (uint16_t)(opt_len8 * 8);
        if (off + opt_len > len) break;

        if (opt_type == 3 && opt_len >= 32) {
            uint8_t prefix_len = icmp_data[off + 2];
            uint8_t flags = icmp_data[off + 3];
            const uint8_t *prefix = icmp_data + off + 16;
            if (prefix_len == 64 && (flags & 0x40)) { /* autonomous */
                memcpy(g_global, prefix, 8);
                memcpy(g_global + 8, g_link_local + 8, 8);
                g_have_global = true;
                kprintf("net: SLAAC global address configured\n");
                break;
            }
        }
        off = (uint16_t)(off + opt_len);
    }
}

static void icmpv6_input(const uint8_t src[16], const uint8_t *data, uint16_t len) {
    if (len < sizeof(struct icmpv6_header)) return;
    const struct icmpv6_header *icmp = (const struct icmpv6_header *)data;

    switch (icmp->type) {
    case ICMPV6_ECHO_REQUEST: {
        uint16_t data_len = (uint16_t)(len - 4);
        uint8_t reply[8 + 512];
        if (data_len > 512) data_len = 512;
        reply[0] = ICMPV6_ECHO_REPLY; reply[1] = 0; reply[2] = 0; reply[3] = 0;
        memcpy(reply + 4, data + 4, 4);
        if (data_len) memcpy(reply + 8, data + 8, data_len);

        uint16_t sum = icmpv6_checksum(g_have_link_local ? g_link_local : g_global,
                                       src, (uint16_t)(8 + data_len), reply, 8,
                                       reply + 8, data_len);
        reply[2] = (uint8_t)(sum >> 8); reply[3] = (uint8_t)sum;

        uint8_t mac[6];
        if (resolve_dst_mac(src, mac) != 0) return;
        ipv6_send(mac, g_have_link_local ? g_link_local : g_global, src,
                 IPV6_NEXT_ICMPV6, 64, reply, 8, reply + 8, data_len);
        break;
    }
    case ICMPV6_ECHO_REPLY:
        if (g_ping6.waiting && len >= 8) {
            uint16_t id = (uint16_t)((data[4] << 8) | data[5]);
            uint16_t seq = (uint16_t)((data[6] << 8) | data[7]);
            if (id == g_ping6.id && seq == g_ping6.seq) {
                g_ping6.got_reply = true;
                g_ping6.reply_ms = pit_uptime_ms();
            }
        }
        break;
    case ICMPV6_NS:
        handle_ns(src, data, len);
        break;
    case ICMPV6_NA:
        handle_na(src, data, len);
        break;
    case ICMPV6_RA:
        handle_ra(data, len);
        break;
    default:
        break;
    }
}

void ipv6_input(const uint8_t *packet, uint16_t len) {
    if (len < IPV6_HDR_LEN) return;
    const struct ipv6_header *ip6 = (const struct ipv6_header *)packet;
    if ((ntohl(ip6->ver_tc_flow) >> 28) != 6) return;

    uint16_t payload_len = ntohs(ip6->payload_len);
    if ((uint32_t)IPV6_HDR_LEN + payload_len > len) return;

    /* Accept unicast to either of our addresses, or multicast (the
     * all-nodes group and solicited-node groups we did not
     * explicitly join a multicast filter for, since the NIC is in
     * promiscuous-enough mode that everything reaches netif_rx()
     * anyway - see ip_input()'s equivalent broadcast acceptance). */
    bool for_us = addr_is_multicast(ip6->dst) ||
                  (g_have_link_local && addr_eq(ip6->dst, g_link_local)) ||
                  (g_have_global && addr_eq(ip6->dst, g_global));
    if (!for_us) return;

    const uint8_t *payload = packet + IPV6_HDR_LEN;
    if (ip6->next_header == IPV6_NEXT_ICMPV6) {
        icmpv6_input(ip6->src, payload, payload_len);
    } else if (ip6->next_header == IPV6_NEXT_UDP) {
        udp6_input(ip6->src, ip6->dst, payload, payload_len);
    } else if (ip6->next_header == IPV6_NEXT_TCP) {
        tcp6_input(ip6->src, ip6->dst, payload, payload_len);
    }
}

void ipv6_init(void) {
    if (!g_netif.up) return;

    memset(g_link_local, 0, 16);
    g_link_local[0] = 0xfe; g_link_local[1] = 0x80;
    /* RFC 4291 Appendix A: modified EUI-64 from the 48-bit MAC. */
    g_link_local[8]  = (uint8_t)(g_netif.mac[0] ^ 0x02);
    g_link_local[9]  = g_netif.mac[1];
    g_link_local[10] = g_netif.mac[2];
    g_link_local[11] = 0xff;
    g_link_local[12] = 0xfe;
    g_link_local[13] = g_netif.mac[3];
    g_link_local[14] = g_netif.mac[4];
    g_link_local[15] = g_netif.mac[5];
    g_have_link_local = true;

    char buf[40];
    ipv6_format(g_link_local, buf, sizeof(buf));
    kprintf("net: ipv6 link-local %s\n", buf);

    /* Router Solicitation to ff02::2 (all-routers), source unspecified
     * is also legal, but we already have a link-local address, so use
     * it - most routers (QEMU's slirp included) accept either. */
    uint8_t dst[16] = {0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x02};
    uint8_t dst_mac[6];
    multicast_mac(dst, dst_mac);

    struct {
        struct icmpv6_header icmp;
        uint32_t reserved;
        uint8_t opt_type;
        uint8_t opt_len;
        uint8_t opt_mac[6];
    } __attribute__((packed)) rs;
    memset(&rs, 0, sizeof(rs));
    rs.icmp.type = ICMPV6_RS;
    rs.opt_type = 1;
    rs.opt_len = 1;
    memcpy(rs.opt_mac, g_netif.mac, 6);
    rs.icmp.checksum = icmpv6_checksum(g_link_local, dst, sizeof(rs), &rs,
                                       sizeof(rs), NULL, 0);

    ipv6_send(dst_mac, g_link_local, dst, IPV6_NEXT_ICMPV6, 255, &rs,
             sizeof(rs), NULL, 0);
}

bool ipv6_get_link_local(uint8_t out[16]) {
    if (!g_have_link_local) return false;
    memcpy(out, g_link_local, 16);
    return true;
}

bool ipv6_get_global(uint8_t out[16]) {
    if (!g_have_global) return false;
    memcpy(out, g_global, 16);
    return true;
}

bool ipv6_pick_source(uint8_t out[16]) {
    if (g_have_global) { memcpy(out, g_global, 16); return true; }
    if (g_have_link_local) { memcpy(out, g_link_local, 16); return true; }
    return false;
}

int ipv6_output(const uint8_t dst[16], uint8_t next_header,
                const void *h, uint32_t hlen, const void *d, uint32_t dlen) {
    uint8_t src[16];
    if (!ipv6_pick_source(src)) return -1;

    uint8_t mac[6];
    if (resolve_dst_mac(dst, mac) != 0) return -1;

    return ipv6_send(mac, src, dst, next_header, 64, h, (uint16_t)hlen, d,
                     (uint16_t)dlen);
}
