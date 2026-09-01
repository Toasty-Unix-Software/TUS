/*
 * ata.h - IDE/ATA disk driver (PIO)
 *
 * The oldest storage interface a PC has, and the one every emulator
 * still provides: two channels (0x1F0 and 0x170), two devices each,
 * 512-byte sectors moved through a data port a word at a time.
 *
 * TUS uses it for one thing - installing itself onto a disk - so the
 * driver is deliberately the simple half of ATA: 28-bit LBA, polled
 * (no IRQ14/15, the device's interrupt is masked at nIEN), one
 * command per transfer. ATAPI devices (the CD-ROM) are detected and
 * left alone: TUS boots from them, it does not read them.
 */

#ifndef TUS_DRIVERS_ATA_H
#define TUS_DRIVERS_ATA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ATA_MAX_DISKS   4
#define ATA_SECTOR_SIZE 512

struct ata_disk {
    bool     present;
    bool     atapi;        /* a CD-ROM: detected, not used */
    uint16_t io_base;      /* 0x1F0 (primary) or 0x170 (secondary) */
    uint16_t ctrl_base;    /* 0x3F6 / 0x376 */
    bool     slave;
    uint32_t sectors;      /* 28-bit LBA capacity */
    char     name[8];      /* hda .. hdd */
    char     model[41];
};

/* Probe both channels and register /dev/hd* for the disks found. */
void ata_init(void);

/* The slot table (ATA_MAX_DISKS entries; check .present). */
const struct ata_disk *ata_disk(int index);

/* Disks (not counting ATAPI devices) found at init. */
int ata_disk_count(void);

/* Move `count` sectors. Returns 0 or a negative errno. Both refuse a
 * transfer that would run past the end of the disk. */
int ata_read(int index, uint32_t lba, uint32_t count, void *buf);
int ata_write(int index, uint32_t lba, uint32_t count, const void *buf);

#endif /* TUS_DRIVERS_ATA_H */
