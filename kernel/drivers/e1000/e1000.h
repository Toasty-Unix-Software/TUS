/*
 * e1000.h - Intel E1000/e1000e Ethernet controller driver
 *
 * Same shape as rtl8139.h - PCI discovery, an MMIO (not I/O port) BAR,
 * a receive ring and a fixed set of transmit buffers, both DMA memory
 * from kernel/mm/dma.c because the card is a bus master and every
 * address it is told about must be physical. Reception is interrupt
 * driven, feeding netif_rx() (kernel/net/netif.h) exactly like
 * rtl8139.c does.
 *
 * kernel/net/netif.c brings this driver up as the fallback NIC: if
 * rtl8139_init() finds no card, net_init() calls e1000_init(), and
 * netif_send() dispatches outbound frames to whichever driver
 * actually claimed the interface. Only one of the two drivers is
 * ever active at once.
 */

#ifndef TUS_DRIVERS_E1000_H
#define TUS_DRIVERS_E1000_H

#include <stdint.h>
#include <stdbool.h>

#define E1000_VENDOR_ID 0x8086

/* QEMU's default `-device e1000` model, plus the real chips the
 * OSDev article calls out (82577LM / i217, found on real laptops). */
#define E1000_DEV_82540EM 0x100E
#define E1000_DEV_82577LM 0x10EA
#define E1000_DEV_I217    0x153A

#define E1000_MAC_ADDR_LEN 6

#define E1000_RX_DESCS 32
#define E1000_TX_DESCS 8
#define E1000_RX_BUF_SIZE 2048
#define E1000_TX_BUF_SIZE 2048
#define E1000_FRAME_MAX 1522

struct e1000_rx_desc {
    uint64_t addr;
    uint16_t length;
    uint16_t checksum;
    uint8_t  status;
    uint8_t  errors;
    uint16_t special;
} __attribute__((packed));

struct e1000_tx_desc {
    uint64_t addr;
    uint16_t length;
    uint8_t  cso;
    uint8_t  cmd;
    uint8_t  status;
    uint8_t  css;
    uint16_t special;
} __attribute__((packed));

struct e1000_device {
    bool present;
    bool io_mapped;    /* true if BAR0 turned out to be an I/O BAR */
    volatile uint8_t *mmio;
    uint16_t io_base;
    uint8_t irq;
    uint8_t mac_addr[E1000_MAC_ADDR_LEN];

    struct e1000_rx_desc *rx_ring; /* virtual (HHDM) */
    uint64_t rx_ring_phys;
    uint8_t *rx_buf[E1000_RX_DESCS];
    uint64_t rx_buf_phys[E1000_RX_DESCS];
    uint16_t rx_tail;

    struct e1000_tx_desc *tx_ring;
    uint64_t tx_ring_phys;
    uint8_t *tx_buf[E1000_TX_DESCS];
    uint64_t tx_buf_phys[E1000_TX_DESCS];
    uint16_t tx_tail;

    uint64_t rx_packets, tx_packets, rx_dropped, tx_dropped;
};

/* Find the card on PCI and bring it up. 0 on success, -1 when absent. */
int e1000_init(void);

bool e1000_present(void);
void e1000_get_mac(uint8_t *mac);

/* Queue one frame for transmission. Returns `len`, or negative errno. */
int e1000_send_packet(const uint8_t *data, uint16_t len);

/* Drain everything the card has received into the netif queue. Called
 * from the receive interrupt and, defensively, available to poll. */
void e1000_drain_rx(void);

void e1000_get_stats(struct e1000_device *out);

#endif /* TUS_DRIVERS_E1000_H */
