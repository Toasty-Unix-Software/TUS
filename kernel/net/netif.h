/*
 * netif.h - the network interface: addressing plus the receive queue
 *
 * One interface (`eth0`) sits between the NIC driver and the IP stack.
 * The driver's interrupt hands frames to netif_rx(), which copies them
 * into a fixed ring; net_poll() drains that ring into the protocol
 * handlers with interrupts on.
 *
 * That split is the whole point: protocol code allocates, blocks and
 * takes locks, none of which belongs in an interrupt handler. The
 * interrupt does one memcpy into a preallocated slot and returns.
 */

#ifndef TUS_NET_NETIF_H
#define TUS_NET_NETIF_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NETIF_MTU        1500
#define NETIF_FRAME_MAX  1522
#define NETIF_RX_QUEUE   64

struct netif {
    bool up;
    char name[8];
    uint8_t mac[6];
    uint32_t ip;        /* all addresses are network byte order */
    uint32_t netmask;
    uint32_t gateway;
    uint32_t dns;
};

/* The one interface. Configured by ifconfig / the boot defaults. */
extern struct netif g_netif;

/* Bring the stack up: probe the NIC, seed the interface addresses. */
int net_init(void);

/* Stack -> driver. Sends on whichever NIC net_init() found (RTL8139
 * tried first, e1000 as the fallback) - transport code no longer
 * needs to know which driver is behind g_netif. Returns the driver's
 * own result (>= 0 sent, < 0 dropped). */
int netif_send(const uint8_t *frame, uint16_t len);

/* Driver -> stack. Safe to call from an interrupt handler; drops the
 * frame (counting it) when the queue is full. */
void netif_rx(const uint8_t *frame, uint16_t len);

/* Drain the receive queue through the protocol handlers. Returns the
 * number of frames processed. Call it with interrupts enabled. */
int net_poll(void);

/* net_poll() plus a yield, for the blocking paths in TCP/UDP: waits
 * until `deadline_ms` (pit_uptime_ms based) for something to arrive. */
void net_wait(uint64_t deadline_ms);

/* Queue depth statistics for `netstat`. */
void netif_get_stats(uint64_t *rx_queued, uint64_t *rx_dropped);

/* Byte-order helpers. TUS is little-endian only, so these are swaps. */
static inline uint16_t htons(uint16_t x) { return (uint16_t)((x << 8) | (x >> 8)); }
static inline uint16_t ntohs(uint16_t x) { return htons(x); }
static inline uint32_t htonl(uint32_t x) {
    return ((x & 0xffu) << 24) | ((x & 0xff00u) << 8) |
           ((x >> 8) & 0xff00u) | ((x >> 24) & 0xffu);
}
static inline uint32_t ntohl(uint32_t x) { return htonl(x); }

#endif /* TUS_NET_NETIF_H */
