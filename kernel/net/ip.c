/*
 * ip.c - Ethernet, ARP, IPv4 and ICMP
 *
 * The send path is one function deep: a transport hands ip_output() a
 * header and a payload, ip_output() finds the next hop, ARP turns that
 * into a MAC address and eth_send() puts the frame on the wire. The
 * receive path is eth_input(), called by net_poll() once per queued
 * frame, which dispatches on the ethertype and IP protocol.
 *
 * ARP is what the old code was missing: every packet went to the
 * broadcast address, which a switch tolerates and a router does not.
 */

#include "ip.h"

#include "ipv6.h"
#include "netif.h"
#include "tcp.h"
#include "udp.h"

#include "../arch/x86_64/io.h"
#include "../core/klib.h"
#include "../drivers/pit/pit.h"
#include "../mm/kmalloc.h"

static const uint8_t eth_broadcast[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/* ---- checksums ---- */

/* Sum 16-bit words without folding, so the pieces of a packet can be
 * summed independently and folded once at the end. */
static uint32_t checksum_partial(const void *data, uint32_t len, uint32_t sum) {
    const uint8_t *p = (const uint8_t *)data;

    while (len > 1) {
        sum += (uint32_t)(p[0] | (p[1] << 8));
        p += 2;
        len -= 2;
    }
    if (len == 1) {
        sum += (uint32_t)p[0];
    }
    return sum;
}

static uint16_t checksum_fold(uint32_t sum) {
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return (uint16_t)~sum;
}

uint16_t net_checksum(const void *data, uint32_t len) {
    return checksum_fold(checksum_partial(data, len, 0));
}

uint16_t transport_checksum(uint32_t src_ip, uint32_t dst_ip, uint8_t proto,
                            const void *h, uint32_t hlen,
                            const void *d, uint32_t dlen) {
    struct {
        uint32_t src;
        uint32_t dst;
        uint8_t zero;
        uint8_t proto;
        uint16_t length;
    } __attribute__((packed)) pseudo = {
        src_ip, dst_ip, 0, proto, htons((uint16_t)(hlen + dlen))
    };

    uint32_t sum = checksum_partial(&pseudo, sizeof(pseudo), 0);
    sum = checksum_partial(h, hlen, sum);

    /* An odd-length header would misalign the payload's word
     * boundaries; transport headers are always even, so this holds. */
    if (d != NULL && dlen > 0) {
        sum = checksum_partial(d, dlen, sum);
    }
    return checksum_fold(sum);
}

/* ---- link layer ---- */

int eth_send(const uint8_t dst_mac[6], uint16_t ethertype,
             const void *h, uint32_t hlen, const void *d, uint32_t dlen) {
    if (!g_netif.up) return -1;
    if (hlen + dlen > NETIF_MTU) return -1;

    uint8_t frame[NETIF_FRAME_MAX];
    struct eth_header *eth = (struct eth_header *)frame;

    memcpy(eth->dst_mac, dst_mac, 6);
    memcpy(eth->src_mac, g_netif.mac, 6);
    eth->type = htons(ethertype);

    if (hlen > 0) memcpy(frame + ETH_HDR_LEN, h, hlen);
    if (dlen > 0) memcpy(frame + ETH_HDR_LEN + hlen, d, dlen);

    return netif_send(frame, (uint16_t)(ETH_HDR_LEN + hlen + dlen));
}

/* ---- ARP ---- */

#define ARP_CACHE_SIZE 32
#define ARP_ENTRY_TTL_MS (5 * 60 * 1000)

struct arp_entry {
    uint32_t ip;
    uint8_t mac[6];
    uint64_t added_ms;
    bool valid;
};

static struct arp_entry arp_cache[ARP_CACHE_SIZE];

static void arp_cache_add(uint32_t ip, const uint8_t *mac) {
    int free_slot = -1;
    int oldest = 0;

    for (int i = 0; i < ARP_CACHE_SIZE; i++) {
        if (arp_cache[i].valid && arp_cache[i].ip == ip) {
            memcpy(arp_cache[i].mac, mac, 6);
            arp_cache[i].added_ms = pit_uptime_ms();
            return;
        }
        if (!arp_cache[i].valid && free_slot < 0) {
            free_slot = i;
        }
        if (arp_cache[i].added_ms < arp_cache[oldest].added_ms) {
            oldest = i;
        }
    }

    int slot = free_slot >= 0 ? free_slot : oldest;
    arp_cache[slot].ip = ip;
    memcpy(arp_cache[slot].mac, mac, 6);
    arp_cache[slot].added_ms = pit_uptime_ms();
    arp_cache[slot].valid = true;
}

static int arp_cache_lookup(uint32_t ip, uint8_t *mac) {
    uint64_t now = pit_uptime_ms();

    for (int i = 0; i < ARP_CACHE_SIZE; i++) {
        if (!arp_cache[i].valid || arp_cache[i].ip != ip) continue;
        if (now - arp_cache[i].added_ms > ARP_ENTRY_TTL_MS) {
            arp_cache[i].valid = false;
            return -1;
        }
        memcpy(mac, arp_cache[i].mac, 6);
        return 0;
    }
    return -1;
}

int arp_cache_dump(struct arp_entry_info *out, int max) {
    int n = 0;
    for (int i = 0; i < ARP_CACHE_SIZE && n < max; i++) {
        if (!arp_cache[i].valid) continue;
        out[n].ip = arp_cache[i].ip;
        memcpy(out[n].mac, arp_cache[i].mac, 6);
        n++;
    }
    return n;
}

static int arp_send(uint16_t opcode, const uint8_t *target_mac,
                    uint32_t target_ip, const uint8_t *dst_mac) {
    struct arp_header arp;

    arp.hw_type = htons(ARP_HW_ETHERNET);
    arp.proto_type = htons(ETH_TYPE_IP);
    arp.hw_len = 6;
    arp.proto_len = 4;
    arp.opcode = htons(opcode);
    memcpy(arp.src_mac, g_netif.mac, 6);
    arp.src_ip = g_netif.ip;
    memcpy(arp.dst_mac, target_mac, 6);
    arp.dst_ip = target_ip;

    return eth_send(dst_mac, ETH_TYPE_ARP, &arp, sizeof(arp), NULL, 0);
}

int arp_resolve(uint32_t ip, uint8_t mac[6], uint32_t timeout_ms) {
    if (arp_cache_lookup(ip, mac) == 0) return 0;

    /* Broadcast and the subnet's broadcast address need no resolution. */
    uint32_t bcast = g_netif.ip | ~g_netif.netmask;
    if (ip == 0xffffffffu || ip == bcast) {
        memcpy(mac, eth_broadcast, 6);
        return 0;
    }

    uint64_t deadline = pit_uptime_ms() + timeout_ms;
    uint64_t next_retry = 0;
    static const uint8_t zero_mac[6] = {0};

    while (pit_uptime_ms() < deadline) {
        if (pit_uptime_ms() >= next_retry) {
            arp_send(ARP_OP_REQUEST, zero_mac, ip, eth_broadcast);
            next_retry = pit_uptime_ms() + 250;
        }
        net_wait(next_retry < deadline ? next_retry : deadline);
        if (arp_cache_lookup(ip, mac) == 0) return 0;
    }
    return -1;
}

static void arp_input(const struct arp_header *arp) {
    if (ntohs(arp->hw_type) != ARP_HW_ETHERNET) return;
    if (ntohs(arp->proto_type) != ETH_TYPE_IP) return;
    if (arp->hw_len != 6 || arp->proto_len != 4) return;

    uint16_t op = ntohs(arp->opcode);

    /* Learn from anything addressed to us, request or reply: a host
     * that just asked for our address is about to talk to us. */
    if (arp->dst_ip == g_netif.ip || op == ARP_OP_REPLY) {
        arp_cache_add(arp->src_ip, arp->src_mac);
    }

    if (op == ARP_OP_REQUEST && arp->dst_ip == g_netif.ip) {
        arp_send(ARP_OP_REPLY, arp->src_mac, arp->src_ip, arp->src_mac);
    }
}

/* ---- IPv4 ---- */

uint32_t ip_next_hop(uint32_t dst_ip) {
    if ((dst_ip & g_netif.netmask) == (g_netif.ip & g_netif.netmask)) {
        return dst_ip;
    }
    return g_netif.gateway ? g_netif.gateway : dst_ip;
}

int ip_output(uint32_t dst_ip, uint8_t proto,
              const void *h, uint32_t hlen, const void *d, uint32_t dlen) {
    if (!g_netif.up) return -1;
    if (hlen + dlen > IP_MAX_PAYLOAD) return -1;

    static uint16_t ip_id_counter = 1;

    uint8_t payload[IP_MAX_PAYLOAD + sizeof(struct ip_header)];
    struct ip_header *ip = (struct ip_header *)payload;

    ip->version_ihl = 0x45;
    ip->dscp_ecn = 0;
    ip->length = htons((uint16_t)(sizeof(struct ip_header) + hlen + dlen));
    ip->id = htons(ip_id_counter++);
    ip->flags_offset = htons(0x4000); /* don't fragment */
    ip->ttl = 64;
    ip->protocol = proto;
    ip->checksum = 0;
    ip->src_ip = g_netif.ip;
    ip->dst_ip = dst_ip;
    ip->checksum = net_checksum(ip, sizeof(struct ip_header));

    if (hlen > 0) memcpy(payload + sizeof(struct ip_header), h, hlen);
    if (dlen > 0) memcpy(payload + sizeof(struct ip_header) + hlen, d, dlen);

    uint8_t mac[6];
    uint32_t hop = ip_next_hop(dst_ip);
    if (arp_resolve(hop, mac, 1000) != 0) {
        return -1; /* nobody answered for the next hop */
    }

    return eth_send(mac, ETH_TYPE_IP, payload,
                    sizeof(struct ip_header) + hlen + dlen, NULL, 0);
}

/* ---- ICMP ---- */

struct ping_slot {
    bool waiting;
    bool got_reply;
    uint32_t from_ip;
    uint16_t id;
    uint16_t seq;
    uint64_t sent_ms;
    uint64_t reply_ms;
};

static struct ping_slot ping_slot;

int icmp_send_echo(uint32_t dst_ip, uint16_t id, uint16_t seq,
                   const uint8_t *data, uint16_t len) {
    struct icmp_header icmp;

    icmp.type = ICMP_ECHO_REQUEST;
    icmp.code = 0;
    icmp.checksum = 0;
    icmp.id = htons(id);
    icmp.seq = htons(seq);
    icmp.checksum = 0;

    /* The ICMP checksum covers the header and the payload together. */
    uint32_t sum = checksum_partial(&icmp, sizeof(icmp), 0);
    if (data && len) sum = checksum_partial(data, len, sum);
    icmp.checksum = checksum_fold(sum);

    ping_slot.waiting = true;
    ping_slot.got_reply = false;
    ping_slot.from_ip = dst_ip;
    ping_slot.id = id;
    ping_slot.seq = seq;
    ping_slot.sent_ms = pit_uptime_ms();

    return ip_output(dst_ip, IP_PROTOCOL_ICMP, &icmp, sizeof(icmp), data, len);
}

long icmp_wait_reply(uint32_t from_ip, uint16_t id, uint16_t seq,
                     uint32_t timeout_ms) {
    uint64_t deadline = pit_uptime_ms() + timeout_ms;

    while (pit_uptime_ms() < deadline) {
        if (ping_slot.got_reply && ping_slot.id == id &&
            ping_slot.seq == seq && ping_slot.from_ip == from_ip) {
            ping_slot.waiting = false;
            return (long)(ping_slot.reply_ms - ping_slot.sent_ms);
        }
        net_wait(deadline);
    }
    ping_slot.waiting = false;
    return -1;
}

static void icmp_input(const struct ip_header *ip,
                       const uint8_t *payload, uint16_t len) {
    if (len < sizeof(struct icmp_header)) return;

    const struct icmp_header *icmp = (const struct icmp_header *)payload;

    if (icmp->type == ICMP_ECHO_REQUEST) {
        /* Reply with the request's payload, as every ping expects. */
        uint16_t data_len = (uint16_t)(len - sizeof(struct icmp_header));
        const uint8_t *data = payload + sizeof(struct icmp_header);

        struct icmp_header reply;
        reply.type = ICMP_ECHO_REPLY;
        reply.code = 0;
        reply.checksum = 0;
        reply.id = icmp->id;
        reply.seq = icmp->seq;

        uint32_t sum = checksum_partial(&reply, sizeof(reply), 0);
        if (data_len) sum = checksum_partial(data, data_len, sum);
        reply.checksum = checksum_fold(sum);

        ip_output(ip->src_ip, IP_PROTOCOL_ICMP, &reply, sizeof(reply),
                  data, data_len);
        return;
    }

    if (icmp->type == ICMP_ECHO_REPLY && ping_slot.waiting) {
        if (ntohs(icmp->id) == ping_slot.id &&
            ntohs(icmp->seq) == ping_slot.seq) {
            ping_slot.got_reply = true;
            ping_slot.reply_ms = pit_uptime_ms();
        }
    }
}

/* ---- receive dispatch ---- */

static void ip_input(const uint8_t *packet, uint16_t len) {
    if (len < sizeof(struct ip_header)) return;

    const struct ip_header *ip = (const struct ip_header *)packet;
    if ((ip->version_ihl >> 4) != 4) return;

    uint16_t ihl = (uint16_t)((ip->version_ihl & 0x0f) * 4);
    if (ihl < sizeof(struct ip_header) || ihl > len) return;

    uint16_t total = ntohs(ip->length);
    if (total < ihl || total > len) return;

    /* Accept unicast to us plus broadcast, so DHCP-style replies and
     * ARP-driven probes are not silently dropped. */
    uint32_t bcast = g_netif.ip | ~g_netif.netmask;
    if (ip->dst_ip != g_netif.ip && ip->dst_ip != 0xffffffffu &&
        ip->dst_ip != bcast) {
        return;
    }

    if (net_checksum(ip, ihl) != 0) return;

    /* Fragments are not reassembled; a fragmented packet is dropped
     * rather than handed to a transport as if it were whole. */
    uint16_t frag = ntohs(ip->flags_offset);
    if ((frag & 0x2000) || (frag & 0x1fff)) return;

    const uint8_t *payload = packet + ihl;
    uint16_t payload_len = (uint16_t)(total - ihl);

    switch (ip->protocol) {
    case IP_PROTOCOL_ICMP:
        icmp_input(ip, payload, payload_len);
        break;
    case IP_PROTOCOL_UDP:
        udp_input(ip->src_ip, ip->dst_ip, payload, payload_len);
        break;
    case IP_PROTOCOL_TCP:
        tcp_input(ip->src_ip, ip->dst_ip, payload, payload_len);
        break;
    default:
        break;
    }
}

void eth_input(const uint8_t *frame, uint16_t len) {
    if (len < ETH_HDR_LEN) return;

    const struct eth_header *eth = (const struct eth_header *)frame;
    uint16_t type = ntohs(eth->type);
    const uint8_t *payload = frame + ETH_HDR_LEN;
    uint16_t payload_len = (uint16_t)(len - ETH_HDR_LEN);

    if (type == ETH_TYPE_ARP) {
        if (payload_len >= sizeof(struct arp_header)) {
            arp_input((const struct arp_header *)payload);
        }
    } else if (type == ETH_TYPE_IP) {
        ip_input(payload, payload_len);
    } else if (type == ETH_TYPE_IPV6) {
        ipv6_input(payload, payload_len);
    }
}
