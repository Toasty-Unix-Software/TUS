/*
 * ip.h - Ethernet, ARP, IPv4 and ICMP
 *
 * Everything here works in network byte order. Addresses are passed
 * around as uint32_t already byte-swapped, which keeps the checksum
 * code (which sums 16-bit words in network order) from having to care.
 */

#ifndef TUS_NET_IP_H
#define TUS_NET_IP_H

#include <stdbool.h>
#include <stdint.h>

struct eth_header {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t type;
} __attribute__((packed));

struct ip_header {
    uint8_t version_ihl;
    uint8_t dscp_ecn;
    uint16_t length;
    uint16_t id;
    uint16_t flags_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src_ip;
    uint32_t dst_ip;
} __attribute__((packed));

struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;
} __attribute__((packed));

struct arp_header {
    uint16_t hw_type;
    uint16_t proto_type;
    uint8_t hw_len;
    uint8_t proto_len;
    uint16_t opcode;
    uint8_t src_mac[6];
    uint32_t src_ip;
    uint8_t dst_mac[6];
    uint32_t dst_ip;
} __attribute__((packed));

struct udp_header {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
} __attribute__((packed));

struct tcp_header {
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq;
    uint32_t ack;
    uint8_t offset_reserved;
    uint8_t flags;
    uint16_t window;
    uint16_t checksum;
    uint16_t urgent;
} __attribute__((packed));

#define ETH_TYPE_IP  0x0800
#define ETH_TYPE_ARP 0x0806
#define ETH_HDR_LEN  14

#define IP_PROTOCOL_ICMP 1
#define IP_PROTOCOL_TCP  6
#define IP_PROTOCOL_UDP  17

#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY   0
#define ICMP_DEST_UNREACH 3

#define ARP_HW_ETHERNET  1
#define ARP_OP_REQUEST   1
#define ARP_OP_REPLY     2

#define TCP_FLAG_FIN 0x01
#define TCP_FLAG_SYN 0x02
#define TCP_FLAG_RST 0x04
#define TCP_FLAG_PSH 0x08
#define TCP_FLAG_ACK 0x10
#define TCP_FLAG_URG 0x20

/* The largest IP payload we will build in one packet. */
#define IP_MAX_PAYLOAD (1500 - 20)

/* ---- checksums ---- */

/* The standard 16-bit one's complement checksum, ready to store. */
uint16_t net_checksum(const void *data, uint32_t len);

/* TCP/UDP checksum over the pseudo-header plus two payload pieces
 * (the transport header and its data), so callers never have to
 * flatten a packet just to check it. */
uint16_t transport_checksum(uint32_t src_ip, uint32_t dst_ip, uint8_t proto,
                            const void *h, uint32_t hlen,
                            const void *d, uint32_t dlen);

/* ---- link layer ---- */

/* Send one frame with the given ethertype and payload. */
int eth_send(const uint8_t dst_mac[6], uint16_t ethertype,
             const void *h, uint32_t hlen, const void *d, uint32_t dlen);

/* Entry point from netif's receive queue. */
void eth_input(const uint8_t *frame, uint16_t len);

/* ---- ARP ---- */

/* Resolve `ip` (which must be on-link) to a MAC address, sending a
 * request and waiting up to `timeout_ms` for the reply. */
int arp_resolve(uint32_t ip, uint8_t mac[6], uint32_t timeout_ms);

/* Copy out the ARP cache for the `arp` command. Returns the number of
 * entries written. */
struct arp_entry_info {
    uint32_t ip;
    uint8_t mac[6];
};
int arp_cache_dump(struct arp_entry_info *out, int max);

/* ---- IPv4 ---- */

/* The next hop for a destination: the destination itself when it is
 * on-link, otherwise the default gateway. */
uint32_t ip_next_hop(uint32_t dst_ip);

/* Build and send one IPv4 packet. `h`/`hlen` is the transport header,
 * `d`/`dlen` its payload; passing them separately spares the callers a
 * copy into a flat buffer. */
int ip_output(uint32_t dst_ip, uint8_t proto,
              const void *h, uint32_t hlen, const void *d, uint32_t dlen);

/* ---- ICMP ---- */

/* Send one echo request. Replies land in the ping slot below. */
int icmp_send_echo(uint32_t dst_ip, uint16_t id, uint16_t seq,
                   const uint8_t *data, uint16_t len);

/* Wait for an echo reply matching id/seq. Returns the round-trip time
 * in milliseconds, or -1 on timeout. */
long icmp_wait_reply(uint32_t from_ip, uint16_t id, uint16_t seq,
                     uint32_t timeout_ms);

#endif /* TUS_NET_IP_H */
