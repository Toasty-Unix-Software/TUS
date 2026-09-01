/*
 * ahci.h - AHCI SATA controller driver
 *
 * Unlike ata.c (legacy IDE ports, fixed at 0x1F0/0x170), an AHCI
 * controller is found on the PCI bus (class 0x01, subclass 0x06) and
 * driven entirely through one MMIO BAR (ABAR, BAR5): a handful of
 * host-wide registers plus up to 32 per-port register blocks. Each
 * port that has a drive attached gets a command list, a FIS receive
 * area and a set of command tables - all DMA memory the controller
 * reads and writes on its own, allocated with kernel/mm/dma.c exactly
 * like xHCI's rings.
 *
 * TUS only needs 28/48-bit LBA read/write of whole disks, so this
 * driver does the minimum AHCI 1.3 needs for that: one command slot
 * used at a time per port, polled completion (no interrupts), 512-byte
 * sectors. ATAPI, port multipliers and SEMB enclosures are detected
 * and left alone, the same way ata.c leaves the CD-ROM alone.
 */

#ifndef TUS_DRIVERS_AHCI_H
#define TUS_DRIVERS_AHCI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define AHCI_MAX_PORTS 32
#define AHCI_SECTOR_SIZE 512

struct ahci_disk {
    bool     present;
    uint32_t port;         /* index into the controller's port array */
    uint64_t sectors;      /* 48-bit LBA capacity */
    char     name[8];      /* sda .. sdz */
    char     model[41];
};

/* Find an AHCI controller on PCI, bring it up and register /dev/sd*
 * for every SATA disk found. 0 on success (a controller was found,
 * even with zero disks attached), -1 when absent. */
int ahci_init(void);

const struct ahci_disk *ahci_disk(int index);
int ahci_disk_count(void);

int ahci_read(int index, uint64_t lba, uint32_t count, void *buf);
int ahci_write(int index, uint64_t lba, uint32_t count, const void *buf);

#endif /* TUS_DRIVERS_AHCI_H */
