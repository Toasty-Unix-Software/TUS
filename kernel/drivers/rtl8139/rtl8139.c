/*
 * rtl8139.c - RTL8139 Ethernet controller driver
 *
 * Three things this driver has to get right, and each one was wrong
 * before:
 *
 *  - The I/O base comes from PCI BAR0. There is no fixed port range
 *    for this card; reading register 0x37 at port 0x37 talks to the
 *    keyboard controller's neighbourhood, not to a NIC.
 *  - Every address handed to the card is PHYSICAL. kmalloc() returns a
 *    higher-half virtual pointer whose low 32 bits are meaningless to a
 *    bus-mastering device, so the rings come from the PMM and the card
 *    is told pmm addresses (all of which QEMU keeps below 4 GiB).
 *  - Reception is driven by the card's interrupt. Polling from a shell
 *    command means a packet that arrives while a task is blocked in
 *    recv() is never seen.
 */

#include "drivers/rtl8139/rtl8139.h"

#include "arch/x86_64/idt.h"
#include "arch/x86_64/io.h"
#include "arch/x86_64/pic.h"
#include "core/klib.h"
#include "drivers/pci/pci.h"
#include "drivers/pit/pit.h"
#include "mm/pmm.h"
#include "net/netif.h"

static struct rtl8139_device rtl8139;

static inline uint8_t  rtl_read8(uint16_t reg)  { return inb(rtl8139.io_base + reg); }
static inline uint16_t rtl_read16(uint16_t reg) { return inw(rtl8139.io_base + reg); }
static inline uint32_t rtl_read32(uint16_t reg) { return inl(rtl8139.io_base + reg); }
static inline void rtl_write8(uint16_t reg, uint8_t v)   { outb(rtl8139.io_base + reg, v); }
static inline void rtl_write16(uint16_t reg, uint16_t v) { outw(rtl8139.io_base + reg, v); }
static inline void rtl_write32(uint16_t reg, uint32_t v) { outl(rtl8139.io_base + reg, v); }

/* ---- PCI discovery ---- */

/* Walk the bus for 10ec:8139. Returns 0 and fills in io_base/irq. */
static int rtl8139_probe_pci(void) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t dev = 0; dev < 32; dev++) {
            for (uint8_t fn = 0; fn < 8; fn++) {
                uint32_t id = pci_config_read((uint8_t)bus, dev, fn, PCI_VENDOR_ID);
                uint16_t vendor = id & 0xffff;
                uint16_t device = (id >> 16) & 0xffff;

                if (vendor == 0xffff) {
                    if (fn == 0) break; /* no device here at all */
                    continue;
                }
                if (vendor != RTL8139_VENDOR_ID || device != RTL8139_DEVICE_ID) {
                    continue;
                }

                /* BAR0 is the I/O BAR (bit 0 set); mask off the flags. */
                uint32_t bar0 = pci_config_read((uint8_t)bus, dev, fn, PCI_BAR0);
                if ((bar0 & 1) == 0) {
                    kprintf("rtl8139: BAR0 is not an I/O region (%08x)\n", bar0);
                    return -1;
                }
                rtl8139.io_base = (uint16_t)(bar0 & ~0x3u);

                /* Enable I/O space and bus mastering - without the
                 * latter the card cannot DMA into the rings. */
                uint32_t cmd = pci_config_read((uint8_t)bus, dev, fn, PCI_COMMAND);
                cmd |= PCI_COMMAND_IO_SPACE | PCI_COMMAND_MASTER;
                pci_config_write((uint8_t)bus, dev, fn, PCI_COMMAND, cmd);

                /* Interrupt line is the low byte of config register 0x3c. */
                uint32_t intr = pci_config_read((uint8_t)bus, dev, fn, 0x3c);
                rtl8139.irq = (uint8_t)(intr & 0xff);

                kprintf("rtl8139: %02x:%02x.%u io 0x%x irq %d\n",
                        bus, dev, fn, rtl8139.io_base, rtl8139.irq);
                return 0;
            }
        }
    }
    return -1;
}

/* ---- ring allocation ---- */

static int rtl8139_alloc_rings(void) {
    size_t rx_frames = (RTL8139_RX_ALLOC + 4095) / 4096;
    uint64_t rx_phys = pmm_alloc_frames(rx_frames);
    if (rx_phys == 0) {
        kprintf("rtl8139: no memory for the receive ring\n");
        return -1;
    }
    if (rx_phys + RTL8139_RX_ALLOC > 0xffffffffull) {
        kprintf("rtl8139: receive ring above 4 GiB, card cannot reach it\n");
        pmm_free_frames(rx_phys, rx_frames);
        return -1;
    }
    rtl8139.rx_ring_phys = rx_phys;
    rtl8139.rx_ring = (uint8_t *)pmm_phys_to_virt(rx_phys);
    memset(rtl8139.rx_ring, 0, RTL8139_RX_ALLOC);

    for (int i = 0; i < RTL8139_TX_DESCS; i++) {
        uint64_t phys = pmm_alloc_frame();
        if (phys == 0 || phys + RTL8139_TX_BUF_SIZE > 0xffffffffull) {
            kprintf("rtl8139: no reachable memory for transmit buffer %d\n", i);
            return -1;
        }
        rtl8139.tx_buf_phys[i] = phys;
        rtl8139.tx_buf[i] = (uint8_t *)pmm_phys_to_virt(phys);
    }
    return 0;
}

/* ---- receive ---- */

/*
 * The ring holds back-to-back entries of [status:16][length:16][frame].
 * `length` counts the 4-byte CRC the card appends, which is not part of
 * the frame. CAPR trails the card's CBR by 16 bytes by convention: the
 * card treats CAPR as "the last byte the driver has consumed", and the
 * 16-byte bias is what the datasheet's reset value (0xfff0) encodes.
 */
void rtl8139_drain_rx(void) {
    if (!rtl8139.present) return;

    while ((rtl_read8(RTL8139_CMD) & RTL8139_CMD_BUFE) == 0) {
        uint16_t off = rtl8139.rx_read_ptr;
        const uint8_t *entry = rtl8139.rx_ring + off;

        uint16_t status = (uint16_t)(entry[0] | (entry[1] << 8));
        uint16_t length = (uint16_t)(entry[2] | (entry[3] << 8));

        /* A length of 0xfff0 means the card is still writing this
         * entry; come back when the interrupt fires again. */
        if (length == 0xfff0) break;

        if ((status & RTL8139_RX_OK) == 0 ||
            length < 4 + 14 || length > RTL8139_FRAME_MAX + 4) {
            /* A corrupt entry costs us the ring: there is no way to
             * find the next header, so reset the receiver. */
            rtl8139.rx_errors++;
            rtl8139.rx_read_ptr = 0;
            rtl_write8(RTL8139_CMD, RTL8139_CMD_TXENABLE);
            rtl_write32(RTL8139_RXBUF, (uint32_t)rtl8139.rx_ring_phys);
            rtl_write16(RTL8139_CAPR, (uint16_t)(0 - 16));
            rtl_write8(RTL8139_CMD, RTL8139_CMD_RXENABLE | RTL8139_CMD_TXENABLE);
            return;
        }

        netif_rx(entry + 4, (uint16_t)(length - 4)); /* drop the CRC */
        rtl8139.rx_packets++;
        rtl8139.rx_bytes += (uint64_t)(length - 4);

        /* Advance past the entry, keeping the read pointer 4-byte
         * aligned as the card requires, and wrap by hand. */
        uint32_t next = (uint32_t)off + length + 4;
        next = (next + 3) & ~3u;
        rtl8139.rx_read_ptr = (uint16_t)(next % RTL8139_RX_RING_SIZE);

        rtl_write16(RTL8139_CAPR, (uint16_t)(rtl8139.rx_read_ptr - 16));
    }
}

static void rtl8139_irq_handler(struct interrupt_frame *frame) {
    (void)frame;

    for (;;) {
        uint16_t isr = rtl_read16(RTL8139_ISR);
        if (isr == 0) break;

        /* Acknowledge first: a packet that arrives between the drain
         * and the acknowledge would otherwise have its interrupt
         * cleared without ever being read. */
        rtl_write16(RTL8139_ISR, isr);

        if (isr & (RTL8139_ISR_ROK | RTL8139_ISR_RXOVW | RTL8139_ISR_FIFOOVW)) {
            rtl8139_drain_rx();
        }
        if (isr & RTL8139_ISR_RER) {
            rtl8139.rx_errors++;
        }
    }

    pic_send_eoi(rtl8139.irq);
}

/* ---- transmit ---- */

int rtl8139_send_packet(const uint8_t *data, uint16_t len) {
    if (!rtl8139.present) return -1;
    if (len == 0 || len > RTL8139_TX_BUF_SIZE) return -1;

    /* Ethernet's minimum frame is 60 bytes before the CRC; the card
     * does not pad for us, and a short ARP frame gets dropped by the
     * receiver if we do not. */
    uint16_t xmit = len < 60 ? 60 : len;

    uint8_t desc = rtl8139.tx_next;
    uint16_t status_reg = RTL8139_TXSTATUS0 + (uint16_t)(desc * 4);

    /* Wait for the card to be done with this descriptor. OWN comes
     * back set when the DMA has drained the buffer. */
    uint64_t deadline = pit_uptime_ms() + 100;
    while ((rtl_read32(status_reg) & RTL8139_TX_OWN) == 0) {
        if (pit_uptime_ms() > deadline) {
            rtl8139.tx_dropped++;
            return -1;
        }
        asm volatile("pause");
    }

    memcpy(rtl8139.tx_buf[desc], data, len);
    if (xmit > len) {
        memset(rtl8139.tx_buf[desc] + len, 0, (size_t)(xmit - len));
    }

    rtl_write32(RTL8139_TXADDR0 + (uint16_t)(desc * 4),
                (uint32_t)rtl8139.tx_buf_phys[desc]);
    /* Early-transmit threshold of 8 (256 bytes) plus the length; the
     * write clears OWN, which is what starts the transfer. */
    rtl_write32(status_reg, (8u << 16) | xmit);

    rtl8139.tx_next = (uint8_t)((desc + 1) % RTL8139_TX_DESCS);
    rtl8139.tx_packets++;
    rtl8139.tx_bytes += (uint64_t)len;
    return len;
}

/* ---- bring-up ---- */

int rtl8139_init(void) {
    memset(&rtl8139, 0, sizeof(rtl8139));

    if (rtl8139_probe_pci() != 0) {
        kprintf("rtl8139: no card found\n");
        return -1;
    }
    if (rtl8139_alloc_rings() != 0) {
        return -1;
    }

    /* Power on, then soft reset and wait for the card to clear it. */
    rtl_write8(RTL8139_CFG9346, RTL8139_9346_UNLOCK);
    rtl_write8(RTL8139_CONFIG1, 0x00);
    rtl_write8(RTL8139_CFG9346, RTL8139_9346_LOCK);

    rtl_write8(RTL8139_CMD, RTL8139_CMD_RESET);
    uint64_t deadline = pit_uptime_ms() + 100;
    while (rtl_read8(RTL8139_CMD) & RTL8139_CMD_RESET) {
        if (pit_uptime_ms() > deadline) {
            kprintf("rtl8139: reset timed out\n");
            return -1;
        }
        asm volatile("pause");
    }

    for (int i = 0; i < RTL8139_MAC_ADDR_LEN; i++) {
        rtl8139.mac_addr[i] = rtl_read8((uint16_t)(RTL8139_IDR0 + i));
    }

    rtl8139.rx_read_ptr = 0;
    rtl8139.tx_next = 0;
    rtl_write32(RTL8139_RXBUF, (uint32_t)rtl8139.rx_ring_phys);
    rtl_write16(RTL8139_CAPR, (uint16_t)(0 - 16));

    for (int i = 0; i < RTL8139_TX_DESCS; i++) {
        rtl_write32(RTL8139_TXADDR0 + (uint16_t)(i * 4),
                    (uint32_t)rtl8139.tx_buf_phys[i]);
    }

    /* RXFTH=111 (no threshold, whole frames), MXDMA=111 (unlimited),
     * WRAP set so a frame is never split across the ring's end,
     * RBLEN=00 (8 KiB), accept physical-match and broadcast. */
    rtl_write32(RTL8139_RXCFG, 0xe000u | 0x0700u | 0x0080u | 0x000au);
    /* MXDMA=111, standard interframe gap. */
    rtl_write32(RTL8139_TXCFG, 0x03000700u);

    rtl_write16(RTL8139_ISR, 0xffff);
    rtl_write16(RTL8139_IMR, RTL8139_ISR_ROK | RTL8139_ISR_RER |
                             RTL8139_ISR_TOK | RTL8139_ISR_TER |
                             RTL8139_ISR_RXOVW | RTL8139_ISR_FIFOOVW);

    rtl_write8(RTL8139_CMD, RTL8139_CMD_RXENABLE | RTL8139_CMD_TXENABLE);

    rtl8139.present = true;

    if (rtl8139.irq < 16) {
        irq_install(rtl8139.irq, rtl8139_irq_handler);
        if (rtl8139.irq >= 8) {
            pic_enable_irq(2); /* the slave PIC is mute without the cascade */
        }
        pic_enable_irq(rtl8139.irq);
    } else {
        kprintf("rtl8139: IRQ %d is not on the PIC; receive will poll\n",
                rtl8139.irq);
    }

    kprintf("rtl8139: MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
            rtl8139.mac_addr[0], rtl8139.mac_addr[1], rtl8139.mac_addr[2],
            rtl8139.mac_addr[3], rtl8139.mac_addr[4], rtl8139.mac_addr[5]);
    return 0;
}

bool rtl8139_present(void) { return rtl8139.present; }

void rtl8139_get_mac(uint8_t *mac) {
    memcpy(mac, rtl8139.mac_addr, RTL8139_MAC_ADDR_LEN);
}

void rtl8139_get_stats(struct rtl8139_device *out) {
    if (out) *out = rtl8139;
}
