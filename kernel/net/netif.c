/*
 * netif.c - the interface between the NIC driver and the IP stack
 *
 * The driver's interrupt handler calls netif_rx(), which does nothing
 * but copy the frame into a preallocated ring slot. Everything that
 * can block, allocate or take time - ARP, IP reassembly, TCP's state
 * machine - happens in net_poll(), which runs with interrupts enabled
 * from whichever task is waiting for the network.
 */

#include "netif.h"

#include "ip.h"
#include "tcp.h"

#include "../arch/x86_64/io.h"
#include "../core/klib.h"
#include "../drivers/pit/pit.h"
#include "../drivers/rtl8139/rtl8139.h"
#include "../drivers/e1000/e1000.h"

struct netif g_netif;

/* Which driver g_netif is actually riding on - decided once, in
 * net_init(), by whichever probe succeeds first. NONE until then. */
enum netif_driver_kind { NETIF_DRV_NONE, NETIF_DRV_RTL8139, NETIF_DRV_E1000 };
static enum netif_driver_kind active_driver = NETIF_DRV_NONE;

struct rx_slot {
    uint16_t len;
    uint8_t data[NETIF_FRAME_MAX];
};

static struct rx_slot rx_queue[NETIF_RX_QUEUE];
static volatile uint32_t rx_head;   /* next slot to consume */
static volatile uint32_t rx_tail;   /* next slot to fill */
static uint64_t rx_queued_total;
static uint64_t rx_dropped_total;

void netif_rx(const uint8_t *frame, uint16_t len) {
    if (len < 14 || len > NETIF_FRAME_MAX) {
        rx_dropped_total++;
        return;
    }

    uint32_t tail = rx_tail;
    uint32_t next = (tail + 1) % NETIF_RX_QUEUE;
    if (next == rx_head) {
        rx_dropped_total++; /* the stack is not draining fast enough */
        return;
    }

    rx_queue[tail].len = len;
    memcpy(rx_queue[tail].data, frame, len);

    /* Publish the slot only once its contents are in place. */
    __asm__ volatile("" ::: "memory");
    rx_tail = next;
    rx_queued_total++;
}

int netif_send(const uint8_t *frame, uint16_t len) {
    switch (active_driver) {
    case NETIF_DRV_RTL8139: return rtl8139_send_packet(frame, len);
    case NETIF_DRV_E1000:   return e1000_send_packet(frame, len);
    default:                return -1;
    }
}

int net_poll(void) {
    /* Defensive: if the card's interrupt is not wired up, this keeps
     * reception working, just with latency. e1000 is fully interrupt
     * driven (see e1000_init()), so there is no equivalent poll for
     * it here. */
    if (active_driver == NETIF_DRV_RTL8139) {
        rtl8139_drain_rx();
    } else if (active_driver == NETIF_DRV_E1000) {
        e1000_drain_rx();
    }

    int processed = 0;
    while (rx_head != rx_tail) {
        uint32_t head = rx_head;
        struct rx_slot *slot = &rx_queue[head];

        eth_input(slot->data, slot->len);

        __asm__ volatile("" ::: "memory");
        rx_head = (head + 1) % NETIF_RX_QUEUE;
        processed++;
    }

    /* Retransmissions and TIME_WAIT expiry ride along here: every
     * blocking network path goes through net_poll(), so a connection
     * anybody is waiting on gets its timers serviced. */
    tcp_timer();
    return processed;
}

void net_wait(uint64_t deadline_ms) {
    for (;;) {
        if (net_poll() > 0) return;
        if (pit_uptime_ms() >= deadline_ms) return;
        hlt(); /* an interrupt - the timer or the card - wakes us */
    }
}

void netif_get_stats(uint64_t *rx_queued, uint64_t *rx_dropped) {
    if (rx_queued) *rx_queued = rx_queued_total;
    if (rx_dropped) *rx_dropped = rx_dropped_total;
}

int net_init(void) {
    memset(&g_netif, 0, sizeof(g_netif));
    strncpy(g_netif.name, "eth0", sizeof(g_netif.name) - 1);

    /* QEMU's default user-mode network: 10.0.2.15/24, gateway on
     * 10.0.2.2, DNS relay on 10.0.2.3 - right for the default test
     * environment and nowhere else. `ifconfig` overrides all of it at
     * runtime; `dns` specifically is also overridden at boot by
     * /etc/resolv.conf, if present (see load_resolv_conf() in
     * kernel/main.c) - this is only what's used before that runs, or
     * if it finds nothing to use. */
    g_netif.ip      = htonl(0x0a00020f);
    g_netif.netmask = htonl(0xffffff00);
    g_netif.gateway = htonl(0x0a000202);
    g_netif.dns     = htonl(0x0a000203);

    /* RTL8139 first (QEMU's long-standing default NIC and what every
     * existing test relies on), e1000 as the fallback for a machine
     * or `-device e1000`/e1000e that has no RTL8139 at all. Only one
     * of the two is ever brought up: probing the second after the
     * first already claimed the card would just be wasted PCI scans. */
    if (rtl8139_init() == 0) {
        active_driver = NETIF_DRV_RTL8139;
        rtl8139_get_mac(g_netif.mac);
    } else if (e1000_init() == 0) {
        active_driver = NETIF_DRV_E1000;
        e1000_get_mac(g_netif.mac);
    } else {
        kprintf("network: no interface (no RTL8139 or e1000 found)\n");
        g_netif.up = false;
        return -1;
    }

    g_netif.up = true;

    uint32_t ip = ntohl(g_netif.ip);
    kprintf("network: %s %d.%d.%d.%d up\n", g_netif.name,
            (ip >> 24) & 0xff, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff);
    return 0;
}
