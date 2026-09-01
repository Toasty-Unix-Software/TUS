/*
 * rtl8139.h - RTL8139 Ethernet controller driver
 *
 * The card is found on the PCI bus (10ec:8139), driven through its I/O
 * BAR, and DMAs into memory the PMM hands out - so every address the
 * card is told about is a PHYSICAL address below 4 GiB, never one of
 * the kernel's higher-half pointers.
 *
 * Reception is interrupt driven: the card's ROK interrupt drains the
 * receive ring into the netif queue (kernel/net/netif.c), so packets
 * arrive while a task sits blocked in recv().
 */

#ifndef TUS_DRIVERS_RTL8139_H
#define TUS_DRIVERS_RTL8139_H

#include <stdint.h>
#include <stdbool.h>

#define RTL8139_VENDOR_ID 0x10ec
#define RTL8139_DEVICE_ID 0x8139

#define RTL8139_MAC_ADDR_LEN 6

/* The receive ring is 8 KiB + 16 bytes of header slack. WRAP is set,
 * so the card may run up to one full frame past the end: the buffer
 * gets 2 KiB of tail padding and the ring never has to be unwrapped. */
#define RTL8139_RX_RING_SIZE  (8 * 1024)
#define RTL8139_RX_SLACK      (16 + 2048)
#define RTL8139_RX_ALLOC      (RTL8139_RX_RING_SIZE + RTL8139_RX_SLACK)

/* Four transmit descriptors, one 2 KiB buffer each. */
#define RTL8139_TX_DESCS      4
#define RTL8139_TX_BUF_SIZE   2048

#define RTL8139_FRAME_MAX     1522

/* I/O port registers (offsets from the I/O BAR). */
#define RTL8139_IDR0      0x00
#define RTL8139_MAR0      0x08
#define RTL8139_TXSTATUS0 0x10
#define RTL8139_TXADDR0   0x20
#define RTL8139_RXBUF     0x30
#define RTL8139_CAPR      0x38
#define RTL8139_CBR       0x3a
#define RTL8139_CMD       0x37
#define RTL8139_IMR       0x3c
#define RTL8139_ISR       0x3e
#define RTL8139_TXCFG     0x40
#define RTL8139_RXCFG     0x44
#define RTL8139_MPC       0x4c
#define RTL8139_CFG9346   0x50
#define RTL8139_CONFIG1   0x52

#define RTL8139_CMD_RESET    0x10
#define RTL8139_CMD_RXENABLE 0x08
#define RTL8139_CMD_TXENABLE 0x04
#define RTL8139_CMD_BUFE     0x01 /* receive buffer empty */

/* Interrupt status/mask bits. */
#define RTL8139_ISR_ROK      0x0001
#define RTL8139_ISR_RER      0x0002
#define RTL8139_ISR_TOK      0x0004
#define RTL8139_ISR_TER      0x0008
#define RTL8139_ISR_RXOVW    0x0010
#define RTL8139_ISR_FIFOOVW  0x0040

/* Per-packet receive status word (first 2 bytes of a ring entry). */
#define RTL8139_RX_OK   0x0001
#define RTL8139_RX_FAE  0x0008
#define RTL8139_RX_CRC  0x0004
#define RTL8139_RX_LONG 0x0010
#define RTL8139_RX_RUNT 0x0020

/* Transmit status bits. */
#define RTL8139_TX_OWN  0x2000
#define RTL8139_TX_TUN  0x4000
#define RTL8139_TX_TOK  0x8000

/* CFG9346 unlock value for writing CONFIG registers. */
#define RTL8139_9346_UNLOCK 0xc0
#define RTL8139_9346_LOCK   0x00

struct rtl8139_device {
    bool present;
    uint16_t io_base;
    uint8_t irq;
    uint8_t mac_addr[RTL8139_MAC_ADDR_LEN];

    uint8_t *rx_ring;       /* virtual (HHDM) */
    uint64_t rx_ring_phys;
    uint16_t rx_read_ptr;   /* offset into the 8 KiB ring */

    uint8_t *tx_buf[RTL8139_TX_DESCS];
    uint64_t tx_buf_phys[RTL8139_TX_DESCS];
    uint8_t tx_next;        /* descriptor to hand the next frame to */

    uint64_t rx_packets, tx_packets, rx_dropped, tx_dropped, rx_errors;
};

/* Find the card on PCI and bring it up. 0 on success, -1 when absent. */
int rtl8139_init(void);

/* True once a card has been found and initialised. */
bool rtl8139_present(void);

void rtl8139_get_mac(uint8_t *mac);

/* Queue one frame for transmission. Returns `len`, or negative errno. */
int rtl8139_send_packet(const uint8_t *data, uint16_t len);

/* Drain everything the card has received into the netif queue.
 * Called from the ROK interrupt and, defensively, from net_poll(). */
void rtl8139_drain_rx(void);

void rtl8139_get_stats(struct rtl8139_device *out);

#endif /* TUS_DRIVERS_RTL8139_H */
