/*
 * udp6.h - UDP over IPv6
 *
 * Same shape as udp.h (a small per-PCB receive queue, remote_addr set
 * by connect() filters incoming datagrams) with 16-byte addresses and
 * the IPv6 pseudo-header checksum instead of IPv4's. See udp.h for
 * the rationale; this file exists only because a uint32_t address
 * cannot hold an IPv6 one, not because the semantics differ.
 */

#ifndef TUS_NET_UDP6_H
#define TUS_NET_UDP6_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define UDP6_QUEUE_DEPTH 8
#define UDP6_DGRAM_MAX   1452  /* MTU less the IPv6 and UDP headers */

struct udp6_dgram {
    uint8_t src_addr[16];
    uint16_t src_port;
    uint16_t len;
    uint8_t data[UDP6_DGRAM_MAX];
};

struct udp6_pcb {
    bool in_use;
    uint8_t local_addr[16];
    uint16_t local_port;
    uint8_t remote_addr[16];  /* set by connect(), filters incoming */
    uint16_t remote_port;
    bool has_remote;

    struct udp6_dgram *queue;
    int head, count;
};

struct udp6_pcb *udp6_pcb_new(void);
void udp6_pcb_free(struct udp6_pcb *pcb);

long udp6_bind(struct udp6_pcb *pcb, const uint8_t ip[16], uint16_t port);
long udp6_connect(struct udp6_pcb *pcb, const uint8_t ip[16], uint16_t port);

long udp6_sendto(struct udp6_pcb *pcb, const void *buf, size_t len,
                 const uint8_t dst_ip[16], uint16_t dst_port);

/* Blocks up to `timeout_ms` (0 means don't block). Fills in the sender
 * when `src_ip`/`src_port` are non-NULL. */
long udp6_recvfrom(struct udp6_pcb *pcb, void *buf, size_t len,
                   uint8_t src_ip[16], uint16_t *src_port,
                   uint32_t timeout_ms);

short udp6_poll(struct udp6_pcb *pcb);

void udp6_input(const uint8_t src_addr[16], const uint8_t dst_addr[16],
                const uint8_t *datagram, uint16_t len);

#endif /* TUS_NET_UDP6_H */
