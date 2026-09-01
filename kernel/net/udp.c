/*
 * udp.c - UDP
 */

#include "udp.h"

#include "ip.h"
#include "netif.h"

#include "../core/errno.h"
#include "../core/klib.h"
#include "../drivers/pit/pit.h"
#include "../mm/kmalloc.h"
#include "../vfs/vfs.h"

#define UDP_PCB_MAX        32
#define UDP_EPHEMERAL_LOW  49152
#define UDP_EPHEMERAL_HIGH 65535

static struct udp_pcb *pcbs[UDP_PCB_MAX];

struct udp_pcb *udp_pcb_new(void) {
    int slot = -1;
    for (int i = 0; i < UDP_PCB_MAX; i++) {
        if (!pcbs[i]) { slot = i; break; }
    }
    if (slot < 0) return NULL;

    struct udp_pcb *pcb = kmalloc(sizeof(*pcb));
    if (!pcb) return NULL;
    memset(pcb, 0, sizeof(*pcb));

    pcb->queue = kmalloc(sizeof(struct udp_dgram) * UDP_QUEUE_DEPTH);
    if (!pcb->queue) {
        kfree(pcb);
        return NULL;
    }

    pcb->in_use = true;
    pcbs[slot] = pcb;
    return pcb;
}

void udp_pcb_free(struct udp_pcb *pcb) {
    if (!pcb) return;
    for (int i = 0; i < UDP_PCB_MAX; i++) {
        if (pcbs[i] == pcb) { pcbs[i] = NULL; break; }
    }
    kfree(pcb->queue);
    kfree(pcb);
}

static bool udp_port_taken(uint16_t port) {
    for (int i = 0; i < UDP_PCB_MAX; i++) {
        if (pcbs[i] && pcbs[i]->local_port == port) return true;
    }
    return false;
}

static uint16_t udp_alloc_port(void) {
    static uint16_t next = UDP_EPHEMERAL_LOW;

    for (int i = 0; i <= UDP_EPHEMERAL_HIGH - UDP_EPHEMERAL_LOW; i++) {
        uint16_t port = next++;
        if (next > UDP_EPHEMERAL_HIGH) next = UDP_EPHEMERAL_LOW;
        if (!udp_port_taken(port)) return port;
    }
    return 0;
}

long udp_bind(struct udp_pcb *pcb, uint32_t ip, uint16_t port) {
    if (!pcb) return -EINVAL;

    if (port == 0) {
        port = udp_alloc_port();
        if (port == 0) return -EADDRINUSE;
    } else if (udp_port_taken(port)) {
        return -EADDRINUSE;
    }
    pcb->local_ip = ip;
    pcb->local_port = port;
    return 0;
}

long udp_connect(struct udp_pcb *pcb, uint32_t ip, uint16_t port) {
    if (!pcb) return -EINVAL;
    pcb->remote_ip = ip;
    pcb->remote_port = port;
    if (pcb->local_port == 0) {
        return udp_bind(pcb, 0, 0);
    }
    return 0;
}

long udp_sendto(struct udp_pcb *pcb, const void *buf, size_t len,
                uint32_t dst_ip, uint16_t dst_port) {
    if (!pcb || !buf) return -EINVAL;
    if (len > UDP_DGRAM_MAX) return -EMSGSIZE;
    if (dst_ip == 0 || dst_port == 0) return -EDESTADDRREQ;

    if (pcb->local_port == 0) {
        long r = udp_bind(pcb, 0, 0);
        if (r < 0) return r;
    }

    struct udp_header uh;
    uh.src_port = htons(pcb->local_port);
    uh.dst_port = htons(dst_port);
    uh.length = htons((uint16_t)(sizeof(uh) + len));
    uh.checksum = 0;

    uint16_t sum = transport_checksum(g_netif.ip, dst_ip, IP_PROTOCOL_UDP,
                                      &uh, sizeof(uh), buf, (uint32_t)len);
    /* A zero checksum means "not computed" in UDP, so the all-ones
     * form is sent instead when the sum works out to zero. */
    uh.checksum = sum ? sum : 0xffff;

    int r = ip_output(dst_ip, IP_PROTOCOL_UDP, &uh, sizeof(uh), buf,
                      (uint32_t)len);
    return r < 0 ? -EHOSTUNREACH : (long)len;
}

long udp_recvfrom(struct udp_pcb *pcb, void *buf, size_t len,
                  uint32_t *src_ip, uint16_t *src_port, uint32_t timeout_ms) {
    if (!pcb || !buf) return -EINVAL;

    uint64_t deadline = pit_uptime_ms() + timeout_ms;

    while (pcb->count == 0) {
        if (timeout_ms == 0 || pit_uptime_ms() >= deadline) return -EAGAIN;
        net_wait(deadline);
    }

    struct udp_dgram *d = &pcb->queue[pcb->head];
    size_t n = d->len < len ? d->len : len;
    memcpy(buf, d->data, n);

    if (src_ip) *src_ip = d->src_ip;
    if (src_port) *src_port = d->src_port;

    pcb->head = (pcb->head + 1) % UDP_QUEUE_DEPTH;
    pcb->count--;
    return (long)n;
}

short udp_poll(struct udp_pcb *pcb) {
    short r = POLLOUT;
    if (pcb && pcb->count > 0) r |= POLLIN;
    return r;
}

void udp_input(uint32_t src_ip, uint32_t dst_ip,
               const uint8_t *datagram, uint16_t len) {
    (void)dst_ip;
    if (len < sizeof(struct udp_header)) return;

    const struct udp_header *uh = (const struct udp_header *)datagram;
    uint16_t total = ntohs(uh->length);
    if (total < sizeof(*uh) || total > len) return;

    /* A zero checksum means the sender did not compute one. */
    if (uh->checksum != 0) {
        if (transport_checksum(src_ip, dst_ip, IP_PROTOCOL_UDP,
                               datagram, total, NULL, 0) != 0) {
            return;
        }
    }

    uint16_t dst_port = ntohs(uh->dst_port);
    uint16_t src_port = ntohs(uh->src_port);
    const uint8_t *payload = datagram + sizeof(*uh);
    uint16_t payload_len = (uint16_t)(total - sizeof(*uh));

    for (int i = 0; i < UDP_PCB_MAX; i++) {
        struct udp_pcb *pcb = pcbs[i];
        if (!pcb || pcb->local_port != dst_port) continue;

        /* A connected socket only hears from its peer. */
        if (pcb->remote_ip && (pcb->remote_ip != src_ip ||
                               pcb->remote_port != src_port)) {
            continue;
        }
        if (pcb->count >= UDP_QUEUE_DEPTH) return; /* queue full: drop */

        int tail = (pcb->head + pcb->count) % UDP_QUEUE_DEPTH;
        struct udp_dgram *d = &pcb->queue[tail];
        d->src_ip = src_ip;
        d->src_port = src_port;
        d->len = payload_len > UDP_DGRAM_MAX ? UDP_DGRAM_MAX : payload_len;
        memcpy(d->data, payload, d->len);
        pcb->count++;
        return;
    }
}
