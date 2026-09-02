/*
 * ipv6.h - IPv6: SLAAC autoconfiguration, NDP and ICMPv6 echo
 *
 * Scope: TUS gets a real link-local address (RFC 4291 EUI-64) at
 * boot, discovers the on-link prefix via Router Solicitation /
 * Advertisement and derives a global address from it (SLAAC, RFC
 * 4862 - no DHCPv6), answers/performs Neighbor Discovery so it is
 * reachable and can reach others, and answers ICMPv6 Echo Request
 * (ping6 both ways). What is deliberately NOT here: no AF_INET6
 * socket API (TCP6/UDP6) - that is a much larger, separately
 * verifiable project on top of this; the existing socket layer stays
 * IPv4-only. This module is network-layer only, mirroring ip.c/ip.h's
 * shape for IPv4 but as its own file since the two address families
 * do not share a wire format.
 */

#ifndef TUS_NET_IPV6_H
#define TUS_NET_IPV6_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ETH_TYPE_IPV6 0x86DD

struct ipv6_header {
    uint32_t ver_tc_flow; /* version:4, traffic class:8, flow label:20 */
    uint16_t payload_len;
    uint8_t  next_header;
    uint8_t  hop_limit;
    uint8_t  src[16];
    uint8_t  dst[16];
} __attribute__((packed));

#define IPV6_HDR_LEN 40

#define IPV6_NEXT_ICMPV6 58
#define IPV6_NEXT_UDP    17
#define IPV6_NEXT_TCP    6

struct icmpv6_header {
    uint8_t  type;
    uint8_t  code;
    uint16_t checksum;
} __attribute__((packed));

#define ICMPV6_ECHO_REQUEST 128
#define ICMPV6_ECHO_REPLY   129
#define ICMPV6_RS            133
#define ICMPV6_RA            134
#define ICMPV6_NS            135
#define ICMPV6_NA            136

/* Bring IPv6 up: derive the link-local address from g_netif.mac and
 * send an initial Router Solicitation. Safe to call with the
 * interface down; a no-op then. Must run after net_init(). */
void ipv6_init(void);

/* Entry point from eth_input() for ETH_TYPE_IPV6 frames (payload
 * starts at the IPv6 header, not the Ethernet header). */
void ipv6_input(const uint8_t *packet, uint16_t len);

/* Our addresses. *_out is untouched and false is returned if we do
 * not have one (global: no RA seen yet). */
bool ipv6_get_link_local(uint8_t out[16]);
bool ipv6_get_global(uint8_t out[16]);

/* Text <-> 16-byte address. Parser accepts standard RFC 5952 forms
 * including "::" compression; formatter always prints the full,
 * uncompressed form (8 groups) - correct, just not the prettiest,
 * which is a fine trade for the amount of code a canonical
 * zero-run-elision formatter needs. Returns 0 on success. */
int ipv6_parse(const char *s, uint8_t out[16]);
void ipv6_format(const uint8_t addr[16], char *out, size_t outsz);

/* Send one ICMPv6 echo request to `dst` (resolved via NDP if it is a
 * unicast address we have not talked to yet). Replies land in the
 * same kind of slot icmp_wait_reply() (IPv4) polls. */
int icmpv6_send_echo(const uint8_t dst[16], uint16_t id, uint16_t seq,
                     const uint8_t *data, uint16_t len);
long icmpv6_wait_reply(const uint8_t dst[16], uint16_t id, uint16_t seq,
                       uint32_t timeout_ms);

/* Copy out the neighbor cache for the `ndp`/`arp -6` shell command.
 * Returns the number of entries written. */
struct ndp_entry_info {
    uint8_t addr[16];
    uint8_t mac[6];
};
int ndp_cache_dump(struct ndp_entry_info *out, int max);

#endif /* TUS_NET_IPV6_H */
