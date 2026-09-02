/*
 * dhcp.c - DHCPv4 client
 *
 * Textbook four-message exchange over UDP 68->67, all of it unicast-
 * from-us/broadcast-to-us: DISCOVER goes out to 255.255.255.255 with
 * src 0.0.0.0 (ip_output() already special-cases both - see ip.c),
 * the OFFER/ACK come back to the same broadcast address and udp_input()
 * delivers them by dst port alone, so no bound local_ip is needed.
 */

#include "dhcp.h"

#include <stddef.h>

#include "netif.h"
#include "udp.h"

#include "../core/klib.h"
#include "../drivers/pit/pit.h"

#define DHCP_SERVER_PORT 67
#define DHCP_CLIENT_PORT 68

#define DHCP_OP_REQUEST 1
#define DHCP_OP_REPLY   2
#define DHCP_HTYPE_ETH  1
#define DHCP_MAGIC      0x63825363u

#define DHCP_DISCOVER 1
#define DHCP_OFFER    2
#define DHCP_REQUEST  3
#define DHCP_ACK      5
#define DHCP_NAK      6

struct dhcp_packet {
    uint8_t  op, htype, hlen, hops;
    uint32_t xid;
    uint16_t secs, flags;
    uint32_t ciaddr, yiaddr, siaddr, giaddr;
    uint8_t  chaddr[16];
    uint8_t  sname[64];
    uint8_t  file[128];
    uint32_t magic;
    uint8_t  options[312];
} __attribute__((packed));

static uint8_t *opt_put(uint8_t *p, uint8_t code, uint8_t len, const void *data) {
    *p++ = code;
    *p++ = len;
    memcpy(p, data, len);
    return p + len;
}

static void build_base(struct dhcp_packet *pkt, uint32_t xid, uint32_t ciaddr) {
    memset(pkt, 0, sizeof(*pkt));
    pkt->op = DHCP_OP_REQUEST;
    pkt->htype = DHCP_HTYPE_ETH;
    pkt->hlen = 6;
    pkt->xid = xid;
    pkt->ciaddr = ciaddr;
    memcpy(pkt->chaddr, g_netif.mac, 6);
    pkt->magic = htonl(DHCP_MAGIC);
}

static uint16_t build_discover(struct dhcp_packet *pkt, uint32_t xid) {
    build_base(pkt, xid, 0);
    uint8_t *p = pkt->options;
    uint8_t type = DHCP_DISCOVER;
    p = opt_put(p, 53, 1, &type);
    uint8_t params[] = {1, 3, 6, 51};
    p = opt_put(p, 55, sizeof(params), params);
    *p++ = 255;
    return (uint16_t)(offsetof(struct dhcp_packet, options) + (p - pkt->options));
}

static uint16_t build_request(struct dhcp_packet *pkt, uint32_t xid,
                              uint32_t requested_ip, uint32_t server_id) {
    build_base(pkt, xid, 0);
    uint8_t *p = pkt->options;
    uint8_t type = DHCP_REQUEST;
    p = opt_put(p, 53, 1, &type);
    uint32_t rip = requested_ip;
    p = opt_put(p, 50, 4, &rip);
    uint32_t sid = server_id;
    p = opt_put(p, 54, 4, &sid);
    uint8_t params[] = {1, 3, 6, 51};
    p = opt_put(p, 55, sizeof(params), params);
    *p++ = 255;
    return (uint16_t)(offsetof(struct dhcp_packet, options) + (p - pkt->options));
}

/* Walks the options area looking for `code`. Returns a pointer to the
 * value (not the tag/len) and sets *out_len, or NULL if absent. */
static const uint8_t *opt_find(const struct dhcp_packet *pkt, uint16_t total_len,
                               uint8_t code, uint8_t *out_len) {
    const uint8_t *p = pkt->options;
    const uint8_t *end = (const uint8_t *)pkt + total_len;
    while (p < end && *p != 255) {
        if (*p == 0) { p++; continue; } /* pad */
        uint8_t c = *p++;
        if (p >= end) break;
        uint8_t len = *p++;
        if (p + len > end) break;
        if (c == code) {
            if (out_len) *out_len = len;
            return p;
        }
        p += len;
    }
    return NULL;
}

static int wait_reply(struct udp_pcb *pcb, struct dhcp_packet *pkt,
                      uint32_t xid, uint32_t timeout_ms) {
    uint64_t deadline = pit_uptime_ms() + timeout_ms;
    for (;;) {
        uint32_t remaining = 0;
        uint64_t now = pit_uptime_ms();
        if (now >= deadline) return -1;
        remaining = (uint32_t)(deadline - now);

        long n = udp_recvfrom(pcb, pkt, sizeof(*pkt), NULL, NULL, remaining);
        if (n < (long)offsetof(struct dhcp_packet, options)) continue;
        if (pkt->op != DHCP_OP_REPLY) continue;
        if (pkt->xid != xid) continue;
        if (ntohl(pkt->magic) != DHCP_MAGIC) continue;
        return (int)n;
    }
}

int dhcp_configure(void) {
    if (!g_netif.up) return -1;

    struct udp_pcb *pcb = udp_pcb_new();
    if (!pcb) return -1;
    if (udp_bind(pcb, 0, DHCP_CLIENT_PORT) < 0) {
        udp_pcb_free(pcb);
        return -1;
    }

    /* DISCOVER/REQUEST both go out with src 0.0.0.0 - we don't have a
     * lease yet, so drop whatever address is currently configured for
     * the duration of the exchange. It's restored (to the offered
     * lease) only on success; a failure leaves the caller's prior
     * config in g_netif untouched, since we never wrote to it. */
    uint32_t saved_ip = g_netif.ip;
    g_netif.ip = 0;

    static uint32_t xid_seed = 0x1000;
    uint32_t xid = htonl(xid_seed++ ^ (uint32_t)pit_uptime_ms());

    struct dhcp_packet tx, rx;
    int ok = -1;
    uint32_t offered_ip = 0, server_id = 0;

    for (int attempt = 0; attempt < 3 && ok != 0; attempt++) {
        uint16_t tx_len = build_discover(&tx, xid);
        udp_sendto(pcb, &tx, tx_len, 0xffffffffu, DHCP_SERVER_PORT);

        if (wait_reply(pcb, &rx, xid, 2000) < 0) continue;

        uint8_t len;
        const uint8_t *t = opt_find(&rx, sizeof(rx), 53, &len);
        if (!t || len != 1 || *t != DHCP_OFFER) continue;

        offered_ip = rx.yiaddr;
        const uint8_t *sid = opt_find(&rx, sizeof(rx), 54, &len);
        if (!sid || len != 4) continue;
        memcpy(&server_id, sid, 4);

        ok = 0;
    }

    if (ok != 0) {
        g_netif.ip = saved_ip;
        udp_pcb_free(pcb);
        return -1;
    }

    ok = -1;
    uint32_t netmask = 0, gateway = 0, dns = 0;

    for (int attempt = 0; attempt < 3 && ok != 0; attempt++) {
        uint16_t tx_len = build_request(&tx, xid, offered_ip, server_id);
        udp_sendto(pcb, &tx, tx_len, 0xffffffffu, DHCP_SERVER_PORT);

        if (wait_reply(pcb, &rx, xid, 2000) < 0) continue;

        uint8_t len;
        const uint8_t *t = opt_find(&rx, sizeof(rx), 53, &len);
        if (!t || len != 1) continue;
        if (*t == DHCP_NAK) { ok = -2; break; }
        if (*t != DHCP_ACK) continue;

        const uint8_t *m = opt_find(&rx, sizeof(rx), 1, &len);
        if (m && len == 4) memcpy(&netmask, m, 4);
        const uint8_t *g = opt_find(&rx, sizeof(rx), 3, &len);
        if (g && len >= 4) memcpy(&gateway, g, 4);
        const uint8_t *d = opt_find(&rx, sizeof(rx), 6, &len);
        if (d && len >= 4) memcpy(&dns, d, 4);

        ok = 0;
    }

    udp_pcb_free(pcb);

    if (ok != 0) {
        g_netif.ip = saved_ip;
        return -1;
    }

    g_netif.ip = rx.yiaddr;
    if (netmask) g_netif.netmask = netmask;
    if (gateway) g_netif.gateway = gateway;
    if (dns) g_netif.dns = dns;

    uint32_t ip = ntohl(g_netif.ip);
    kprintf("dhcp: leased %d.%d.%d.%d\n",
            (ip >> 24) & 0xff, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff);
    return 0;
}
