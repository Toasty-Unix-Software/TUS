/*
 * ata.c - IDE/ATA disk driver (PIO), implementation
 *
 * Polled, 28-bit LBA, one command per transfer. Everything here is
 * bounded by a timeout: a machine with no disk (or a controller that
 * never answers) must end up without one, not with a kernel that
 * spins in a status loop during boot.
 *
 * Why polling: the transfer sizes TUS moves are an installation, not
 * a workload, and a polled driver has no interrupt handler to get
 * wrong. The device's own interrupt is masked (nIEN) so a stray
 * IRQ14 cannot arrive while nobody is listening for it.
 */

#include "drivers/ata/ata.h"

#include "../../core/console.h"
#include "../../core/errno.h"
#include "../../core/klib.h"
#include "arch/x86_64/io.h"

/* Register offsets from the channel's I/O base. */
#define ATA_REG_DATA     0
#define ATA_REG_ERROR    1
#define ATA_REG_FEATURES 1
#define ATA_REG_SECCOUNT 2
#define ATA_REG_LBA_LO   3
#define ATA_REG_LBA_MID  4
#define ATA_REG_LBA_HI   5
#define ATA_REG_DRIVE    6
#define ATA_REG_STATUS   7
#define ATA_REG_COMMAND  7

/* Status bits. */
#define ATA_SR_BSY  0x80
#define ATA_SR_DRDY 0x40
#define ATA_SR_DF   0x20
#define ATA_SR_DRQ  0x08
#define ATA_SR_ERR  0x01

/* Commands. */
#define ATA_CMD_READ_PIO     0x20
#define ATA_CMD_WRITE_PIO    0x30
#define ATA_CMD_FLUSH_CACHE  0xE7
#define ATA_CMD_IDENTIFY     0xEC

/* Device control register bits (write-only, at ctrl_base). */
#define ATA_CTRL_NIEN 0x02
#define ATA_CTRL_SRST 0x04

/* Spin limits. A PIO sector is microseconds; these are milliseconds
 * worth of port reads, which is generous and still finite. */
#define ATA_POLL_LIMIT  1000000

static struct ata_disk g_disks[ATA_MAX_DISKS];
static int g_count;

/* Reading the alternate status register takes ~100 ns and has no side
 * effects; four of them are the standard way to wait for the drive to
 * put its status on the bus. */
static void ata_delay(const struct ata_disk *d) {
    for (int i = 0; i < 4; i++) {
        (void)inb(d->ctrl_base);
    }
}

static int ata_wait_clear_busy(const struct ata_disk *d) {
    for (unsigned i = 0; i < ATA_POLL_LIMIT; i++) {
        uint8_t st = inb(d->io_base + ATA_REG_STATUS);
        if ((st & ATA_SR_BSY) == 0) {
            return st;
        }
    }
    return -1;
}

/* Wait for DRQ (the drive has a sector for us, or wants one).
 * Returns 0, or a negative errno for an error or a timeout. */
static int ata_wait_drq(const struct ata_disk *d) {
    for (unsigned i = 0; i < ATA_POLL_LIMIT; i++) {
        uint8_t st = inb(d->io_base + ATA_REG_STATUS);
        if (st & ATA_SR_BSY) {
            continue;
        }
        if (st & (ATA_SR_ERR | ATA_SR_DF)) {
            return -EIO;
        }
        if (st & ATA_SR_DRQ) {
            return 0;
        }
    }
    return -EIO;
}

/* Select master or slave on this channel and let the drive settle.
 * A select is only free the second time around, so the caller should
 * not do it per sector - the transfer functions do it once. */
static void ata_select(const struct ata_disk *d, uint32_t lba_high_nibble) {
    outb(d->io_base + ATA_REG_DRIVE,
         (uint8_t)(0xE0 | (d->slave ? 0x10 : 0x00) |
                   (lba_high_nibble & 0x0F)));
    ata_delay(d);
}

/* ---- identification ---- */

/* Pull a model string out of the IDENTIFY block: 40 bytes of ASCII
 * with every pair of characters byte-swapped, padded with spaces. */
static void ata_model(const uint16_t *id, char *out) {
    for (int i = 0; i < 20; i++) {
        out[i * 2] = (char)(id[27 + i] >> 8);
        out[i * 2 + 1] = (char)(id[27 + i] & 0xFF);
    }
    out[40] = '\0';
    for (int i = 39; i >= 0 && (out[i] == ' ' || out[i] == '\0'); i--) {
        out[i] = '\0';
    }
}

static void ata_identify(struct ata_disk *d, const char *name) {
    /* Mask the device's interrupt: this driver polls. */
    outb(d->ctrl_base, ATA_CTRL_NIEN);

    ata_select(d, 0);
    outb(d->io_base + ATA_REG_SECCOUNT, 0);
    outb(d->io_base + ATA_REG_LBA_LO, 0);
    outb(d->io_base + ATA_REG_LBA_MID, 0);
    outb(d->io_base + ATA_REG_LBA_HI, 0);
    outb(d->io_base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    ata_delay(d);

    /* Status 0 means "nothing on this slot" - a floating bus reads
     * back 0xFF, an empty slot 0x00. */
    uint8_t st = inb(d->io_base + ATA_REG_STATUS);
    if (st == 0 || st == 0xFF) {
        return;
    }
    if (ata_wait_clear_busy(d) < 0) {
        return;
    }

    /* A device that answers IDENTIFY with an error is usually ATAPI:
     * its signature is in the two LBA middle/high registers. TUS
     * boots from the CD, it never reads it, so note it and stop. */
    uint8_t mid = inb(d->io_base + ATA_REG_LBA_MID);
    uint8_t hi = inb(d->io_base + ATA_REG_LBA_HI);
    if (mid == 0x14 && hi == 0xEB) {
        d->present = true;
        d->atapi = true;
        strncpy(d->name, name, sizeof(d->name) - 1);
        strcpy(d->model, "ATAPI device");
        return;
    }
    if (mid != 0 || hi != 0) {
        return; /* SATA or something else that is not an ATA disk */
    }
    if (ata_wait_drq(d) != 0) {
        return;
    }

    uint16_t id[256];
    for (int i = 0; i < 256; i++) {
        id[i] = inw(d->io_base + ATA_REG_DATA);
    }

    /* Words 60/61: the 28-bit LBA sector count. A disk that reports
     * zero there is one TUS cannot address. */
    uint32_t sectors = (uint32_t)id[60] | ((uint32_t)id[61] << 16);
    if (sectors == 0) {
        return;
    }

    d->present = true;
    d->atapi = false;
    d->sectors = sectors;
    strncpy(d->name, name, sizeof(d->name) - 1);
    ata_model(id, d->model);
    g_count++;
}

void ata_init(void) {
    static const struct {
        uint16_t io, ctrl;
        bool slave;
        const char *name;
    } slots[ATA_MAX_DISKS] = {
        { 0x1F0, 0x3F6, false, "hda" },
        { 0x1F0, 0x3F6, true,  "hdb" },
        { 0x170, 0x376, false, "hdc" },
        { 0x170, 0x376, true,  "hdd" },
    };

    for (int i = 0; i < ATA_MAX_DISKS; i++) {
        struct ata_disk *d = &g_disks[i];
        memset(d, 0, sizeof(*d));
        d->io_base = slots[i].io;
        d->ctrl_base = slots[i].ctrl;
        d->slave = slots[i].slave;
        ata_identify(d, slots[i].name);
    }
}

const struct ata_disk *ata_disk(int index) {
    if (index < 0 || index >= ATA_MAX_DISKS) {
        return NULL;
    }
    return &g_disks[index];
}

int ata_disk_count(void) {
    return g_count;
}

/* ---- transfers ---- */

/* One command moves at most 256 sectors (a sector count of 0 means
 * 256); the callers loop. */
#define ATA_MAX_CHUNK 128

static int ata_transfer(int index, uint32_t lba, uint32_t count, void *buf,
                        bool write) {
    if (index < 0 || index >= ATA_MAX_DISKS) {
        return -ENODEV;
    }
    struct ata_disk *d = &g_disks[index];
    if (!d->present || d->atapi) {
        return -ENODEV;
    }
    if (count == 0) {
        return 0;
    }
    if (lba > d->sectors || count > d->sectors - lba) {
        return -EINVAL;
    }

    uint8_t *p = (uint8_t *)buf;
    while (count > 0) {
        uint32_t chunk = count > ATA_MAX_CHUNK ? ATA_MAX_CHUNK : count;

        if (ata_wait_clear_busy(d) < 0) {
            return -EIO;
        }
        ata_select(d, lba >> 24);
        outb(d->io_base + ATA_REG_FEATURES, 0);
        outb(d->io_base + ATA_REG_SECCOUNT, (uint8_t)chunk);
        outb(d->io_base + ATA_REG_LBA_LO, (uint8_t)lba);
        outb(d->io_base + ATA_REG_LBA_MID, (uint8_t)(lba >> 8));
        outb(d->io_base + ATA_REG_LBA_HI, (uint8_t)(lba >> 16));
        outb(d->io_base + ATA_REG_COMMAND,
             write ? ATA_CMD_WRITE_PIO : ATA_CMD_READ_PIO);

        for (uint32_t s = 0; s < chunk; s++) {
            int rc = ata_wait_drq(d);
            if (rc != 0) {
                return rc;
            }
            /* The data register is 16 bits wide and the buffer may
             * not be aligned for a 16-bit access, so words are
             * assembled and split byte by byte. */
            if (write) {
                for (int i = 0; i < ATA_SECTOR_SIZE / 2; i++) {
                    uint16_t w = (uint16_t)p[i * 2] |
                                 ((uint16_t)p[i * 2 + 1] << 8);
                    outw(d->io_base + ATA_REG_DATA, w);
                }
            } else {
                for (int i = 0; i < ATA_SECTOR_SIZE / 2; i++) {
                    uint16_t w = inw(d->io_base + ATA_REG_DATA);
                    p[i * 2] = (uint8_t)w;
                    p[i * 2 + 1] = (uint8_t)(w >> 8);
                }
            }
            p += ATA_SECTOR_SIZE;
            ata_delay(d);
        }

        if (write) {
            /* Without the flush the drive may still be holding the
             * data in its own buffer when the machine reboots into
             * what was just installed. */
            outb(d->io_base + ATA_REG_COMMAND, ATA_CMD_FLUSH_CACHE);
            ata_delay(d);
            if (ata_wait_clear_busy(d) < 0) {
                return -EIO;
            }
        }

        lba += chunk;
        count -= chunk;
    }
    return 0;
}

int ata_read(int index, uint32_t lba, uint32_t count, void *buf) {
    return ata_transfer(index, lba, count, buf, false);
}

int ata_write(int index, uint32_t lba, uint32_t count, const void *buf) {
    return ata_transfer(index, lba, count, (void *)buf, true);
}
