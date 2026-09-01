/*
 * e1000.c - Intel E1000/e1000e Ethernet controller driver
 *
 * Mirrors rtl8139.c's structure: PCI discovery, DMA rings from the
 * PMM (via mm/dma.c) because the card is a bus master, receive driven
 * by the card's interrupt. The one real difference is the register
 * window - BAR0 is normally an MMIO region here rather than an I/O
 * range, so registers are accessed through a mapped pointer
 * (vmm_map_mmio(), the same call xhci.c and ahci.c use) instead of
 * in/out instructions; a handful of real chips (and this driver
 * checks for it rather than assuming) expose an I/O-mapped BAR0
 * instead; both paths are implemented since the OSDev article's
 * example code has to handle both.
 */

#include "drivers/e1000/e1000.h"

#include "arch/x86_64/idt.h"
#include "arch/x86_64/io.h"
#include "arch/x86_64/pic.h"
#include "core/klib.h"
#include "drivers/pci/pci.h"
#include "drivers/pit/pit.h"
#include "mm/dma.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "net/netif.h"

/* ---- register offsets ---- */
#define E1000_REG_CTRL   0x0000
#define E1000_REG_STATUS 0x0008
#define E1000_REG_EERD   0x0014
#define E1000_REG_ICR    0x00C0
#define E1000_REG_IMS    0x00D0
#define E1000_REG_IMC    0x00D8
#define E1000_REG_RCTL   0x0100
#define E1000_REG_TCTL   0x0400
#define E1000_REG_TIPG   0x0410
#define E1000_REG_RDBAL  0x2800
#define E1000_REG_RDBAH  0x2804
#define E1000_REG_RDLEN  0x2808
#define E1000_REG_RDH    0x2810
#define E1000_REG_RDT    0x2818
#define E1000_REG_TDBAL  0x3800
#define E1000_REG_TDBAH  0x3804
#define E1000_REG_TDLEN  0x3808
#define E1000_REG_TDH    0x3810
#define E1000_REG_TDT    0x3818
#define E1000_REG_MTA    0x5200
#define E1000_REG_RAL0   0x5400
#define E1000_REG_RAH0   0x5404

#define E1000_CTRL_RST (1u << 26)
#define E1000_CTRL_SLU (1u << 6)
#define E1000_CTRL_ASDE (1u << 5)

#define E1000_RCTL_EN   (1u << 1)
#define E1000_RCTL_SBP  (1u << 2)
#define E1000_RCTL_UPE  (1u << 3)
#define E1000_RCTL_MPE  (1u << 4)
#define E1000_RCTL_LPE  (1u << 5)
#define E1000_RCTL_BAM  (1u << 15)
#define E1000_RCTL_BSIZE_2048 (0u << 16)
#define E1000_RCTL_SECRC (1u << 26)

#define E1000_TCTL_EN  (1u << 1)
#define E1000_TCTL_PSP (1u << 3)
#define E1000_TCTL_CT_SHIFT   4
#define E1000_TCTL_COLD_SHIFT 12

#define E1000_EERD_START (1u << 0)
#define E1000_EERD_DONE  (1u << 4)
#define E1000_EERD_ADDR_SHIFT 8
#define E1000_EERD_DATA_SHIFT 16

#define E1000_RXD_STAT_DD  0x01
#define E1000_RXD_STAT_EOP 0x02

#define E1000_TXD_CMD_EOP  0x01
#define E1000_TXD_CMD_IFCS 0x02
#define E1000_TXD_CMD_RS   0x08
#define E1000_TXD_STAT_DD  0x01

#define E1000_ICR_RXT0 (1u << 7)  /* receiver timer interrupt */
#define E1000_ICR_RXO  (1u << 6)  /* receiver overrun */
#define E1000_ICR_RXDMT0 (1u << 4)

static struct e1000_device e1000;

static inline uint32_t e1000_read32(uint32_t reg) {
    if (e1000.io_mapped) {
        outl(e1000.io_base, reg);
        return inl(e1000.io_base + 4);
    }
    return *(volatile uint32_t *)(e1000.mmio + reg);
}
static inline void e1000_write32(uint32_t reg, uint32_t val) {
    if (e1000.io_mapped) {
        outl(e1000.io_base, reg);
        outl(e1000.io_base + 4, val);
    } else {
        *(volatile uint32_t *)(e1000.mmio + reg) = val;
    }
}

/* ---- PCI discovery ---- */

static bool e1000_matches(uint16_t vendor, uint16_t device) {
    if (vendor != E1000_VENDOR_ID) {
        return false;
    }
    return device == E1000_DEV_82540EM || device == E1000_DEV_82577LM ||
          device == E1000_DEV_I217;
}

static int e1000_probe_pci(void) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t dev = 0; dev < 32; dev++) {
            for (uint8_t fn = 0; fn < 8; fn++) {
                uint32_t id = pci_config_read((uint8_t)bus, dev, fn, PCI_VENDOR_ID);
                uint16_t vendor = id & 0xffff;
                if (vendor == 0xffff) {
                    if (fn == 0) break;
                    continue;
                }
                uint16_t device = (id >> 16) & 0xffff;
                if (!e1000_matches(vendor, device)) {
                    continue;
                }

                uint32_t bar0 = pci_config_read((uint8_t)bus, dev, fn, PCI_BAR0);
                uint32_t cmd = pci_config_read((uint8_t)bus, dev, fn, PCI_COMMAND);

                if (bar0 & 0x1) {
                    e1000.io_mapped = true;
                    e1000.io_base = (uint16_t)(bar0 & ~0x3u);
                    cmd |= PCI_COMMAND_IO_SPACE | PCI_COMMAND_MASTER;
                } else {
                    uint64_t base = bar0 & ~0xFu;
                    if (((bar0 >> 1) & 0x3) == 0x2) {
                        base |= (uint64_t)pci_config_read((uint8_t)bus, dev, fn, PCI_BAR1) << 32;
                    }
                    e1000.io_mapped = false;
                    e1000.mmio = (volatile uint8_t *)vmm_map_mmio(base, 0x20000);
                    if (e1000.mmio == NULL) {
                        kprintf("e1000: could not map BAR0\n");
                        return -1;
                    }
                    cmd |= PCI_COMMAND_MEM_SPACE | PCI_COMMAND_MASTER;
                }
                pci_config_write((uint8_t)bus, dev, fn, PCI_COMMAND, cmd);

                uint32_t intr = pci_config_read((uint8_t)bus, dev, fn, 0x3c);
                e1000.irq = (uint8_t)(intr & 0xff);

                kprintf("e1000: %02x:%02x.%u %s irq %d\n", bus, dev, fn,
                        e1000.io_mapped ? "io" : "mmio", e1000.irq);
                return 0;
            }
        }
    }
    return -1;
}

/* ---- EEPROM / MAC ---- */

static bool e1000_eeprom_read(uint16_t addr, uint16_t *out) {
    e1000_write32(E1000_REG_EERD, ((uint32_t)addr << E1000_EERD_ADDR_SHIFT) |
                                  E1000_EERD_START);
    for (int i = 0; i < 100000; i++) {
        uint32_t val = e1000_read32(E1000_REG_EERD);
        if (val & E1000_EERD_DONE) {
            *out = (uint16_t)(val >> E1000_EERD_DATA_SHIFT);
            return true;
        }
    }
    return false;
}

static void e1000_read_mac(void) {
    uint16_t w;
    if (e1000_eeprom_read(0, &w)) {
        e1000.mac_addr[0] = (uint8_t)w;
        e1000.mac_addr[1] = (uint8_t)(w >> 8);
        if (e1000_eeprom_read(1, &w)) {
            e1000.mac_addr[2] = (uint8_t)w;
            e1000.mac_addr[3] = (uint8_t)(w >> 8);
        }
        if (e1000_eeprom_read(2, &w)) {
            e1000.mac_addr[4] = (uint8_t)w;
            e1000.mac_addr[5] = (uint8_t)(w >> 8);
        }
        return;
    }

    /* No EEPROM (some MMIO-only parts, e.g. 82577LM/i217): the MAC is
     * already latched into RAL0/RAH0 by firmware. */
    uint32_t ral = e1000_read32(E1000_REG_RAL0);
    uint32_t rah = e1000_read32(E1000_REG_RAH0);
    e1000.mac_addr[0] = (uint8_t)ral;
    e1000.mac_addr[1] = (uint8_t)(ral >> 8);
    e1000.mac_addr[2] = (uint8_t)(ral >> 16);
    e1000.mac_addr[3] = (uint8_t)(ral >> 24);
    e1000.mac_addr[4] = (uint8_t)rah;
    e1000.mac_addr[5] = (uint8_t)(rah >> 8);
}

/* ---- rings ---- */

static int e1000_alloc_rings(void) {
    size_t rx_bytes = (size_t)E1000_RX_DESCS * sizeof(struct e1000_rx_desc);
    struct dma_buf rxd = dma_alloc(rx_bytes);
    if (!rxd.virt) {
        kprintf("e1000: no memory for the receive ring\n");
        return -1;
    }
    e1000.rx_ring = (struct e1000_rx_desc *)rxd.virt;
    e1000.rx_ring_phys = rxd.phys;

    for (int i = 0; i < E1000_RX_DESCS; i++) {
        struct dma_buf b = dma_alloc(E1000_RX_BUF_SIZE);
        if (!b.virt) {
            kprintf("e1000: no memory for receive buffer %d\n", i);
            return -1;
        }
        e1000.rx_buf[i] = (uint8_t *)b.virt;
        e1000.rx_buf_phys[i] = b.phys;
        e1000.rx_ring[i].addr = b.phys;
        e1000.rx_ring[i].status = 0;
    }

    size_t tx_bytes = (size_t)E1000_TX_DESCS * sizeof(struct e1000_tx_desc);
    struct dma_buf txd = dma_alloc(tx_bytes);
    if (!txd.virt) {
        kprintf("e1000: no memory for the transmit ring\n");
        return -1;
    }
    e1000.tx_ring = (struct e1000_tx_desc *)txd.virt;
    e1000.tx_ring_phys = txd.phys;

    for (int i = 0; i < E1000_TX_DESCS; i++) {
        struct dma_buf b = dma_alloc(E1000_TX_BUF_SIZE);
        if (!b.virt) {
            kprintf("e1000: no memory for transmit buffer %d\n", i);
            return -1;
        }
        e1000.tx_buf[i] = (uint8_t *)b.virt;
        e1000.tx_buf_phys[i] = b.phys;
        e1000.tx_ring[i].addr = b.phys;
        e1000.tx_ring[i].status = E1000_TXD_STAT_DD; /* free */
    }
    return 0;
}

/* ---- receive ---- */

void e1000_drain_rx(void) {
    if (!e1000.present) return;

    for (;;) {
        struct e1000_rx_desc *d = &e1000.rx_ring[e1000.rx_tail];
        if ((d->status & E1000_RXD_STAT_DD) == 0) {
            break;
        }

        if ((d->status & E1000_RXD_STAT_EOP) && d->length >= 14 &&
            d->length <= E1000_FRAME_MAX) {
            netif_rx(e1000.rx_buf[e1000.rx_tail], d->length);
            e1000.rx_packets++;
        } else {
            e1000.rx_dropped++;
        }

        d->status = 0;
        uint16_t done = e1000.rx_tail;
        e1000.rx_tail = (uint16_t)((e1000.rx_tail + 1) % E1000_RX_DESCS);
        e1000_write32(E1000_REG_RDT, done);
    }
}

static void e1000_irq_handler(struct interrupt_frame *frame) {
    (void)frame;

    uint32_t icr = e1000_read32(E1000_REG_ICR); /* read-to-clear */
    if (icr & (E1000_ICR_RXT0 | E1000_ICR_RXO | E1000_ICR_RXDMT0)) {
        e1000_drain_rx();
    }
    pic_send_eoi(e1000.irq);
}

/* ---- transmit ---- */

int e1000_send_packet(const uint8_t *data, uint16_t len) {
    if (!e1000.present) return -1;
    if (len == 0 || len > E1000_TX_BUF_SIZE) return -1;

    uint16_t desc = e1000.tx_tail;
    struct e1000_tx_desc *d = &e1000.tx_ring[desc];

    uint64_t deadline = pit_uptime_ms() + 100;
    while ((d->status & E1000_TXD_STAT_DD) == 0) {
        if (pit_uptime_ms() > deadline) {
            e1000.tx_dropped++;
            return -1;
        }
        asm volatile("pause");
    }

    memcpy(e1000.tx_buf[desc], data, len);
    d->length = len;
    d->cmd = E1000_TXD_CMD_EOP | E1000_TXD_CMD_IFCS | E1000_TXD_CMD_RS;
    d->status = 0;

    e1000.tx_tail = (uint16_t)((desc + 1) % E1000_TX_DESCS);
    e1000_write32(E1000_REG_TDT, e1000.tx_tail);
    e1000.tx_packets++;
    return len;
}

/* ---- bring-up ---- */

int e1000_init(void) {
    memset(&e1000, 0, sizeof(e1000));

    if (e1000_probe_pci() != 0) {
        kprintf("e1000: no card found\n");
        return -1;
    }

    /* Reset, then wait for the reset bit to self-clear. */
    e1000_write32(E1000_REG_CTRL, e1000_read32(E1000_REG_CTRL) | E1000_CTRL_RST);
    for (int i = 0; i < 100000; i++) {
        if ((e1000_read32(E1000_REG_CTRL) & E1000_CTRL_RST) == 0) break;
        asm volatile("pause");
    }

    /* Link up, auto-speed detect; disable every interrupt cause until
     * the ring is actually ready to receive into. */
    e1000_write32(E1000_REG_CTRL,
                  e1000_read32(E1000_REG_CTRL) | E1000_CTRL_SLU | E1000_CTRL_ASDE);
    e1000_write32(E1000_REG_IMC, 0xFFFFFFFFu);

    /* Multicast table array: zero every entry, same as the reference
     * driver - nothing here relies on multicast reception. */
    for (int i = 0; i < 128; i++) {
        e1000_write32(E1000_REG_MTA + (uint32_t)(i * 4), 0);
    }

    e1000_read_mac();

    if (e1000_alloc_rings() != 0) {
        return -1;
    }

    e1000_write32(E1000_REG_RDBAL, (uint32_t)e1000.rx_ring_phys);
    e1000_write32(E1000_REG_RDBAH, (uint32_t)(e1000.rx_ring_phys >> 32));
    e1000_write32(E1000_REG_RDLEN,
                  (uint32_t)(E1000_RX_DESCS * sizeof(struct e1000_rx_desc)));
    e1000_write32(E1000_REG_RDH, 0);
    e1000_write32(E1000_REG_RDT, E1000_RX_DESCS - 1);
    e1000.rx_tail = 0;
    e1000_write32(E1000_REG_RCTL, E1000_RCTL_EN | E1000_RCTL_SBP | E1000_RCTL_UPE |
                                  E1000_RCTL_MPE | E1000_RCTL_LPE | E1000_RCTL_BAM |
                                  E1000_RCTL_BSIZE_2048 | E1000_RCTL_SECRC);

    e1000_write32(E1000_REG_TDBAL, (uint32_t)e1000.tx_ring_phys);
    e1000_write32(E1000_REG_TDBAH, (uint32_t)(e1000.tx_ring_phys >> 32));
    e1000_write32(E1000_REG_TDLEN,
                  (uint32_t)(E1000_TX_DESCS * sizeof(struct e1000_tx_desc)));
    e1000_write32(E1000_REG_TDH, 0);
    e1000_write32(E1000_REG_TDT, 0);
    e1000.tx_tail = 0;
    e1000_write32(E1000_REG_TCTL,
                  E1000_TCTL_EN | E1000_TCTL_PSP |
                  (0x0Fu << E1000_TCTL_CT_SHIFT) |
                  (0x40u << E1000_TCTL_COLD_SHIFT));
    /* Recommended IPG for full duplex (10,10,10 field, 0x0060200A). */
    e1000_write32(E1000_REG_TIPG, 0x0060200A);

    e1000.present = true;

    if (e1000.irq < 16) {
        irq_install(e1000.irq, e1000_irq_handler);
        if (e1000.irq >= 8) {
            pic_enable_irq(2);
        }
        pic_enable_irq(e1000.irq);
        e1000_write32(E1000_REG_IMS,
                      E1000_ICR_RXT0 | E1000_ICR_RXO | E1000_ICR_RXDMT0);
    } else {
        kprintf("e1000: IRQ %d is not on the PIC; receive will not fire\n",
                e1000.irq);
    }

    kprintf("e1000: MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
            e1000.mac_addr[0], e1000.mac_addr[1], e1000.mac_addr[2],
            e1000.mac_addr[3], e1000.mac_addr[4], e1000.mac_addr[5]);
    return 0;
}

bool e1000_present(void) { return e1000.present; }

void e1000_get_mac(uint8_t *mac) {
    memcpy(mac, e1000.mac_addr, E1000_MAC_ADDR_LEN);
}

void e1000_get_stats(struct e1000_device *out) {
    if (out) *out = e1000;
}
