/*
 * udp6.c - UDP over IPv6 (see udp6.h)
 */

#include "udp6.h"

#include "ip.h"
#include "ipv6.h"
#include "netif.h"

#include "../core/errno.h"
#include "../core/klib.h"
#include "../drivers/pit/pit.h"
#include "../mm/kmalloc.h"
#include "../vfs/vfs.h"

#define UDP6_PCB_MAX        32
#define UDP6_EPHEMERAL_LOW  49152
#define UDP6_EPHEMERAL_HIGH 65535

static struct udp6_pcb *pcbs[UDP6_PCB_MAX];

static int addr_eq16(const uint8_t a[16], const uint8_t b[16]) {
    for (int i = 0; i < 16; i++) if (a[i] != b[i]) return 0;
    return 1;
}

struct udp6_pcb *udp6_pcb_new(void) {
    int slot = -1;
    for (int i = 0; i < UDP6_PCB_MAX; i++) {
        if (!pcbs[i]) { slot = i; break; }
    }
    if (slot < 0) return NULL;

    struct udp6_pcb *pcb = kmalloc(sizeof(*pcb));
    if (!pcb) return NULL;
    memset(pcb, 0, sizeof(*pcb));

    pcb->queue = kmalloc(sizeof(struct udp6_dgram) * UDP6_QUEUE_DEPTH);
    if (!pcb->queue) {
        kfree(pcb);
        return NULL;
    }

    pcb->in_use = true;
    pcbs[slot] = pcb;
    return pcb;
}

void udp6_pcb_free(struct udp6_pcb *pcb) {
    if (!pcb) return;
    for (int i = 0; i < UDP6_PCB_MAX; i++) {
        if (pcbs[i] == pcb) { pcbs[i] = NULL; break; }
    }
    kfree(pcb->queue);
    kfree(pcb);
}

static bool udp6_port_taken(uint16_t port) {
    for (int i = 0; i < UDP6_PCB_MAX; i++) {
        if (pcbs[i] && pcbs[i]->local_port == port) return true;
    }
    return false;
}

static uint16_t udp6_alloc_port(void) {
    static uint16_t next = UDP6_EPHEMERAL_LOW;

    for (int i = 0; i <= UDP6_EPHEMERAL_HIGH - UDP6_EPHEMERAL_LOW; i++) {
        uint16_t port = next++;
        if (next > UDP6_EPHEMERAL_HIGH) next = UDP6_EPHEMERAL_LOW;
        if (!udp6_port_taken(port)) return port;
    }
    return 0;
}

long udp6_bind(struct udp6_pcb *pcb, const uint8_t ip[16], uint16_t port) {
    if (!pcb) return -EINVAL;

    if (port == 0) {
        port = udp6_alloc_port();
        if (port == 0) return -EADDRINUSE;
    } else if (udp6_port_taken(port)) {
        return -EADDRINUSE;
    }
    if (ip) memcpy(pcb->local_addr, ip, 16);
    pcb->local_port = port;
    return 0;
}

long udp6_connect(struct udp6_pcb *pcb, const uint8_t ip[16], uint16_t port) {
    if (!pcb) return -EINVAL;
    memcpy(pcb->remote_addr, ip, 16);
    pcb->remote_port = port;
    pcb->has_remote = true;
    if (pcb->local_port == 0) {
        return udp6_bind(pcb, NULL, 0);
    }
    return 0;
}

long udp6_sendto(struct udp6_pcb *pcb, const void *buf, size_t len,
                 const uint8_t dst_ip[16], uint16_t dst_port) {
    if (!pcb || !buf || !dst_ip) return -EINVAL;
    if (len > UDP6_DGRAM_MAX) return -EMSGSIZE;
    if (dst_port == 0) return -EDESTADDRREQ;

    if (pcb->local_port == 0) {
        long r = udp6_bind(pcb, NULL, 0);
        if (r < 0) return r;
    }

    uint8_t src[16];
    if (!ipv6_pick_source(src)) return -EHOSTUNREACH;

    struct udp_header uh;
    uh.src_port = htons(pcb->local_port);
    uh.dst_port = htons(dst_port);
    uh.length = htons((uint16_t)(sizeof(uh) + len));
    uh.checksum = 0;

    uint16_t sum = transport_checksum6(src, dst_ip, IPV6_NEXT_UDP, &uh,
                                       sizeof(uh), buf, (uint32_t)len);
    /* Unlike IPv4, a zero UDP6 checksum is illegal (RFC 8200 8.1), so
     * the all-ones form covers the (astronomically unlikely) case
     * where the sum comes out to zero. */
    uh.checksum = sum ? sum : 0xffff;

    int r = ipv6_output(dst_ip, IPV6_NEXT_UDP, &uh, sizeof(uh), buf,
                        (uint32_t)len);
    return r < 0 ? -EHOSTUNREACH : (long)len;
}

long udp6_recvfrom(struct udp6_pcb *pcb, void *buf, size_t len,
                   uint8_t src_ip[16], uint16_t *src_port,
                   uint32_t timeout_ms) {
    if (!pcb || !buf) return -EINVAL;

    uint64_t deadline = pit_uptime_ms() + timeout_ms;

    while (pcb->count == 0) {
        if (timeout_ms == 0 || pit_uptime_ms() >= deadline) return -EAGAIN;
        net_wait(deadline);
    }

    struct udp6_dgram *d = &pcb->queue[pcb->head];
    size_t n = d->len < len ? d->len : len;
    memcpy(buf, d->data, n);

    if (src_ip) memcpy(src_ip, d->src_addr, 16);
    if (src_port) *src_port = d->src_port;

    pcb->head = (pcb->head + 1) % UDP6_QUEUE_DEPTH;
    pcb->count--;
    return (long)n;
}

short udp6_poll(struct udp6_pcb *pcb) {
    short r = POLLOUT;
    if (pcb && pcb->count > 0) r |= POLLIN;
    return r;
}

void udp6_input(const uint8_t src_addr[16], const uint8_t dst_addr[16],
                const uint8_t *datagram, uint16_t len) {
    (void)dst_addr;
    if (len < sizeof(struct udp_header)) return;

    const struct udp_header *uh = (const struct udp_header *)datagram;
    uint16_t total = ntohs(uh->length);
    if (total < sizeof(*uh) || total > len) return;

    /* UDP6 checksums are mandatory (RFC 8200 8.1): no "0 means
     * uncomputed" exemption like IPv4's. */
    if (transport_checksum6(src_addr, dst_addr, IPV6_NEXT_UDP, datagram,
                            total, NULL, 0) != 0) {
        return;
    }

    uint16_t dst_port = ntohs(uh->dst_port);
    uint16_t src_port = ntohs(uh->src_port);
    const uint8_t *payload = datagram + sizeof(*uh);
    uint16_t payload_len = (uint16_t)(total - sizeof(*uh));

    for (int i = 0; i < UDP6_PCB_MAX; i++) {
        struct udp6_pcb *pcb = pcbs[i];
        if (!pcb || pcb->local_port != dst_port) continue;

        if (pcb->has_remote && (!addr_eq16(pcb->remote_addr, src_addr) ||
                                pcb->remote_port != src_port)) {
            continue;
        }
        if (pcb->count >= UDP6_QUEUE_DEPTH) return; /* queue full: drop */

        int tail = (pcb->head + pcb->count) % UDP6_QUEUE_DEPTH;
        struct udp6_dgram *d = &pcb->queue[tail];
        memcpy(d->src_addr, src_addr, 16);
        d->src_port = src_port;
        d->len = payload_len > UDP6_DGRAM_MAX ? UDP6_DGRAM_MAX : payload_len;
        memcpy(d->data, payload, d->len);
        pcb->count++;
        return;
    }
}
