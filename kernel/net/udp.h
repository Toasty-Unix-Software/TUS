/*
 * udp.h - UDP
 *
 * Datagram in, datagram out. Each PCB keeps a small queue of received
 * datagrams with their source address, because a UDP reader needs to
 * know who answered - which is exactly what a DNS resolver checks
 * before believing a reply.
 */

#ifndef TUS_NET_UDP_H
#define TUS_NET_UDP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define UDP_QUEUE_DEPTH 8
#define UDP_DGRAM_MAX   1472  /* MTU less the IP and UDP headers */

struct udp_dgram {
    uint32_t src_ip;
    uint16_t src_port;
    uint16_t len;
    uint8_t data[UDP_DGRAM_MAX];
};

struct udp_pcb {
    bool in_use;
    uint32_t local_ip;
    uint16_t local_port;
    uint32_t remote_ip;   /* set by connect(), filters incoming */
    uint16_t remote_port;

    struct udp_dgram *queue;
    int head, count;
};

struct udp_pcb *udp_pcb_new(void);
void udp_pcb_free(struct udp_pcb *pcb);

long udp_bind(struct udp_pcb *pcb, uint32_t ip, uint16_t port);
long udp_connect(struct udp_pcb *pcb, uint32_t ip, uint16_t port);

long udp_sendto(struct udp_pcb *pcb, const void *buf, size_t len,
                uint32_t dst_ip, uint16_t dst_port);

/* Blocks up to `timeout_ms` (0 means don't block). Fills in the sender
 * when `src_ip`/`src_port` are non-NULL. */
long udp_recvfrom(struct udp_pcb *pcb, void *buf, size_t len,
                  uint32_t *src_ip, uint16_t *src_port, uint32_t timeout_ms);

short udp_poll(struct udp_pcb *pcb);

void udp_input(uint32_t src_ip, uint32_t dst_ip,
               const uint8_t *datagram, uint16_t len);

#endif /* TUS_NET_UDP_H */
