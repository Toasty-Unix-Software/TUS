/*
 * tusnet.h - the netctl(2) ABI
 *
 * Included verbatim by the kernel and by ifconfig/route/arp/netstat/
 * ping, so the tools and the stack cannot drift apart. Every address
 * in these structures is in network byte order, the same as the
 * kernel's.
 */

#ifndef TUS_NET_ABI_H
#define TUS_NET_ABI_H

#include <stdint.h>

/* netctl(op, arg, len) operations. */
#define NETCTL_GET_IF     1  /* arg: struct tus_ifinfo    */
#define NETCTL_SET_IF     2  /* arg: struct tus_ifinfo (root only) */
#define NETCTL_PING       3  /* arg: struct tus_ping      */
#define NETCTL_ARP_DUMP   4  /* arg: struct tus_arp_row[] */
#define NETCTL_TCP_DUMP   5  /* arg: struct tus_tcp_row[] */
#define NETCTL_RESOLVE    6  /* arg: struct tus_resolve   */
#define NETCTL_GET_IF6    7  /* arg: struct tus_if6info   */
#define NETCTL_PING6      8  /* arg: struct tus_ping6     */
#define NETCTL_NDP_DUMP   9  /* arg: struct tus_ndp_row[] */
#define NETCTL_DHCP       10 /* arg: NULL, len 0 (root only) - blocks until lease/timeout */

struct tus_ifinfo {
    char name[8];
    uint8_t mac[6];
    uint8_t up;
    uint8_t _pad;
    uint32_t ip;
    uint32_t netmask;
    uint32_t gateway;
    uint32_t dns;
    uint64_t rx_packets;
    uint64_t tx_packets;
    uint64_t rx_dropped;
    uint64_t rx_errors;
    uint64_t tx_dropped;
};

struct tus_ping {
    uint32_t dst_ip;
    uint32_t timeout_ms;
    uint16_t id;
    uint16_t seq;
    int32_t rtt_ms;      /* filled in; -1 on timeout */
    uint32_t payload_len;
};

struct tus_arp_row {
    uint32_t ip;
    uint8_t mac[6];
    uint8_t _pad[2];
};

struct tus_tcp_row {
    int32_t state;
    uint32_t local_ip;
    uint32_t remote_ip;
    uint16_t local_port;
    uint16_t remote_port;
    uint32_t rx_queued;
    uint32_t tx_queued;
};

struct tus_if6info {
    uint8_t have_link_local;
    uint8_t have_global;
    uint8_t _pad[6];
    uint8_t link_local[16];
    uint8_t global[16];
};

struct tus_ping6 {
    uint8_t dst[16];
    uint32_t timeout_ms;
    uint16_t id;
    uint16_t seq;
    int32_t rtt_ms;      /* filled in; -1 on timeout */
    uint32_t payload_len;
};

struct tus_ndp_row {
    uint8_t addr[16];
    uint8_t mac[6];
    uint8_t _pad[2];
};

/* DNS lives in the kernel because every tool needs it and none of them
 * should carry a resolver. `name` in, up to four addresses out. */
struct tus_resolve {
    char name[256];
    uint32_t addr[4];
    int32_t count;
};

#endif /* TUS_NET_ABI_H */
