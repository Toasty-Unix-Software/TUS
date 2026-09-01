/*
 * nvme.h - NVMe controller driver
 *
 * An NVMe controller is a PCI device (class 0x01, subclass 0x08) with
 * one MMIO BAR (BAR0, 64-bit) that holds a handful of controller-wide
 * registers plus a doorbell array. Everything else - the admin queue
 * pair used to configure the controller and one I/O queue pair used
 * to move data - is a ring of fixed-size entries in DMA memory
 * (kernel/mm/dma.c) that the driver owns and the controller is simply
 * told the physical address of.
 *
 * TUS only needs whole-namespace read/write, so this driver brings up
 * one admin queue pair, identifies the controller and namespace 1,
 * creates one I/O queue pair sized for a handful of outstanding
 * commands, and polls completion queues rather than using MSI-X -
 * NVMe requires MSI-X for interrupts, and TUS's PCI layer does not
 * program that capability (see pci.h's comment on pci_msi_enable()).
 */

#ifndef TUS_DRIVERS_NVME_H
#define TUS_DRIVERS_NVME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NVME_SECTOR_SIZE 512
#define NVME_MAX_NAMESPACES 4

struct nvme_namespace {
    bool     present;
    uint32_t nsid;
    uint64_t sectors;   /* in NVME_SECTOR_SIZE units (LBA size assumed 512) */
    char     name[12];  /* nvme0n1 .. */
};

/* Find an NVMe controller on PCI, bring it up, identify namespace 1
 * and register /dev/nvme0n1. 0 on success, -1 when absent. */
int nvme_init(void);

const struct nvme_namespace *nvme_namespace(int index);
int nvme_namespace_count(void);

int nvme_read(int index, uint64_t lba, uint32_t count, void *buf);
int nvme_write(int index, uint64_t lba, uint32_t count, const void *buf);

#endif /* TUS_DRIVERS_NVME_H */
