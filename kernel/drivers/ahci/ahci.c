/*
 * ahci.c - AHCI SATA controller driver, implementation
 *
 * Polled, one command slot (slot 0) used per port, one port driven at
 * a time - TUS has no need for AHCI's native command queuing, only
 * for whole-disk read/write. Every structure the controller reads or
 * writes (command list, FIS receive area, command table, PRDT, the
 * actual sector data) comes from kernel/mm/dma.c, never kmalloc - see
 * dma.h's comment on why a kmalloc pointer cannot be handed to a bus
 * master. The transfer buffer in particular is bounced through a
 * per-port DMA buffer: the caller's buffer (a stack array, a kmalloc
 * block) is not guaranteed physically contiguous or below any
 * particular address, so data is copied in/out of a page the
 * controller was actually told about.
 */

#include "drivers/ahci/ahci.h"

#include "core/console.h"
#include "core/errno.h"
#include "core/klib.h"
#include "drivers/pci/pci.h"
#include "drivers/pit/pit.h"
#include "mm/dma.h"
#include "mm/vmm.h"
#include "vfs/vfs.h"

/* ---- HBA (host bus adapter) registers, offsets from ABAR ---- */
#define AHCI_CAP    0x00
#define AHCI_GHC    0x04
#define AHCI_IS     0x08
#define AHCI_PI     0x0C
#define AHCI_VS     0x10

#define AHCI_GHC_AE (1u << 31)
#define AHCI_GHC_IE (1u << 1)
#define AHCI_GHC_HR (1u << 0)

/* ---- per-port registers, offset 0x100 + port * 0x80 ---- */
#define AHCI_PORT_BASE   0x100
#define AHCI_PORT_STRIDE 0x80

#define PxCLB   0x00
#define PxCLBU  0x04
#define PxFB    0x08
#define PxFBU   0x0C
#define PxIS    0x10
#define PxIE    0x14
#define PxCMD   0x18
#define PxTFD   0x20
#define PxSIG   0x24
#define PxSSTS  0x28
#define PxSCTL  0x2C
#define PxSERR  0x30
#define PxSACT  0x34
#define PxCI    0x38

#define PxCMD_ST  (1u << 0)
#define PxCMD_FRE (1u << 4)
#define PxCMD_FR  (1u << 14)
#define PxCMD_CR  (1u << 15)

#define PxTFD_STS_BSY 0x80
#define PxTFD_STS_DRQ 0x08
#define PxTFD_STS_ERR 0x01

#define AHCI_SIG_ATA   0x00000101
#define AHCI_SIG_ATAPI 0xEB140101
#define AHCI_SIG_SEMB  0xC33C0101
#define AHCI_SIG_PM    0x96690101

#define AHCI_CMD_READ_DMA_EXT   0x25
#define AHCI_CMD_WRITE_DMA_EXT  0x35
#define AHCI_CMD_IDENTIFY       0xEC

#define AHCI_FIS_TYPE_REG_H2D 0x27
#define AHCI_FIS_H2D_COMMAND  (1u << 7) /* byte 1, "this is a command" */

/* Bounded spin: AHCI has no fixed timing guarantee, so every wait is a
 * loop with a generous iteration cap rather than a real deadline. */
#define AHCI_POLL_LIMIT 2000000

/* Read/write DMA EXT moves at most this many sectors per command - one
 * PRDT entry, comfortably under its 22-bit (4 MiB) byte-count field
 * and small enough that a single dma_alloc bounce buffer covers it. */
#define AHCI_MAX_CHUNK 128

/* AHCI command header (32 bytes), one array of 32 per port. */
struct ahci_cmd_header {
    uint16_t flags;   /* CFL[4:0] | W[6] | ... */
    uint16_t prdtl;
    uint32_t prdbc;
    uint32_t ctba;
    uint32_t ctbau;
    uint32_t reserved[4];
};

/* Command table: CFIS (64B) + ACMD (16B) + reserved (48B) + PRDT. */
struct ahci_prdt_entry {
    uint32_t dba;
    uint32_t dbau;
    uint32_t reserved;
    uint32_t dbc; /* bits 0..21 = byte count - 1; bit 31 = interrupt */
};

struct ahci_cmd_table {
    uint8_t cfis[64];
    uint8_t acmd[16];
    uint8_t reserved[48];
    struct ahci_prdt_entry prdt[1];
};

struct ahci_port_state {
    struct dma_buf clb;   /* command list, 1 KiB */
    struct dma_buf fb;    /* FIS receive area, 256 B */
    struct dma_buf ctba;  /* command table, one page (slot 0 only) */
    struct dma_buf bounce; /* transfer buffer, AHCI_MAX_CHUNK sectors */
};

static volatile uint8_t *g_abar;
static uint32_t g_ports_implemented;
static struct ahci_port_state g_pstate[AHCI_MAX_PORTS];
static struct ahci_disk g_disks[AHCI_MAX_PORTS];
static int g_disk_count;

static inline uint32_t rd32(uint32_t off) {
    return *(volatile uint32_t *)(g_abar + off);
}
static inline void wr32(uint32_t off, uint32_t v) {
    *(volatile uint32_t *)(g_abar + off) = v;
}
static inline uint32_t prd32(uint32_t port, uint32_t off) {
    return rd32(AHCI_PORT_BASE + port * AHCI_PORT_STRIDE + off);
}
static inline void pwr32(uint32_t port, uint32_t off, uint32_t v) {
    wr32(AHCI_PORT_BASE + port * AHCI_PORT_STRIDE + off, v);
}

/* ---- port bring-up ---- */

static int ahci_port_stop_cmd(uint32_t port) {
    uint32_t cmd = prd32(port, PxCMD);
    cmd &= ~(PxCMD_ST | PxCMD_FRE);
    pwr32(port, PxCMD, cmd);

    for (int i = 0; i < AHCI_POLL_LIMIT; i++) {
        if ((prd32(port, PxCMD) & (PxCMD_FR | PxCMD_CR)) == 0) {
            return 0;
        }
    }
    return -1;
}

static int ahci_port_start_cmd(uint32_t port) {
    for (int i = 0; i < AHCI_POLL_LIMIT; i++) {
        if ((prd32(port, PxCMD) & PxCMD_CR) == 0) {
            break;
        }
    }
    uint32_t cmd = prd32(port, PxCMD);
    cmd |= PxCMD_FRE | PxCMD_ST;
    pwr32(port, PxCMD, cmd);
    return 0;
}

static int ahci_port_alloc(uint32_t port) {
    struct ahci_port_state *ps = &g_pstate[port];

    ps->clb = dma_alloc(1024);
    ps->fb = dma_alloc(256);
    ps->ctba = dma_alloc(sizeof(struct ahci_cmd_table));
    ps->bounce = dma_alloc((size_t)AHCI_MAX_CHUNK * AHCI_SECTOR_SIZE);
    if (!ps->clb.virt || !ps->fb.virt || !ps->ctba.virt || !ps->bounce.virt) {
        kprintf("ahci: port %u: out of DMA memory\n", port);
        return -1;
    }

    pwr32(port, PxCLB, (uint32_t)ps->clb.phys);
    pwr32(port, PxCLBU, (uint32_t)(ps->clb.phys >> 32));
    pwr32(port, PxFB, (uint32_t)ps->fb.phys);
    pwr32(port, PxFBU, (uint32_t)(ps->fb.phys >> 32));

    /* Slot 0's command header points at the one command table this
     * driver ever uses for this port. */
    struct ahci_cmd_header *hdr = (struct ahci_cmd_header *)ps->clb.virt;
    hdr[0].ctba = (uint32_t)ps->ctba.phys;
    hdr[0].ctbau = (uint32_t)(ps->ctba.phys >> 32);
    return 0;
}

/* Wait for the drive to stop being busy (BSY and, once it has cleared,
 * not still asserting DRQ from a previous command). */
static int ahci_wait_ready(uint32_t port) {
    for (int i = 0; i < AHCI_POLL_LIMIT; i++) {
        uint32_t tfd = prd32(port, PxTFD);
        if ((tfd & (PxTFD_STS_BSY | PxTFD_STS_DRQ)) == 0) {
            return 0;
        }
    }
    return -EIO;
}

/* Build and issue one H2D register FIS in slot 0, wait for it to
 * complete (CI bit clears). `buf` is the bounce buffer's virtual
 * address, sized for `count` sectors; the caller has already copied
 * write data into it (or will copy read data out of it). */
static int ahci_issue(uint32_t port, uint8_t command, uint64_t lba,
                      uint32_t count, bool write) {
    struct ahci_port_state *ps = &g_pstate[port];

    if (ahci_wait_ready(port) != 0) {
        return -EIO;
    }

    struct ahci_cmd_header *hdr = (struct ahci_cmd_header *)ps->clb.virt;
    struct ahci_cmd_table *tbl = (struct ahci_cmd_table *)ps->ctba.virt;
    memset(tbl, 0, sizeof(*tbl));

    uint32_t bytes = count * AHCI_SECTOR_SIZE;
    tbl->prdt[0].dba = (uint32_t)ps->bounce.phys;
    tbl->prdt[0].dbau = (uint32_t)(ps->bounce.phys >> 32);
    tbl->prdt[0].dbc = (bytes - 1) & 0x3FFFFFu;

    uint8_t *fis = tbl->cfis;
    fis[0] = AHCI_FIS_TYPE_REG_H2D;
    fis[1] = AHCI_FIS_H2D_COMMAND;
    fis[2] = command;
    fis[3] = 0; /* features */
    fis[4] = (uint8_t)(lba & 0xFF);
    fis[5] = (uint8_t)((lba >> 8) & 0xFF);
    fis[6] = (uint8_t)((lba >> 16) & 0xFF);
    fis[7] = 0x40; /* device: LBA mode */
    fis[8] = (uint8_t)((lba >> 24) & 0xFF);
    fis[9] = (uint8_t)((lba >> 32) & 0xFF);
    fis[10] = (uint8_t)((lba >> 40) & 0xFF);
    fis[11] = 0; /* features high */
    fis[12] = (uint8_t)(count & 0xFF);
    fis[13] = (uint8_t)((count >> 8) & 0xFF);

    hdr[0].flags = (uint16_t)(5 /* CFIS length in dwords */ |
                              (write ? (1u << 6) : 0));
    hdr[0].prdtl = 1;
    hdr[0].prdbc = 0;

    pwr32(port, PxIS, 0xFFFFFFFFu); /* clear stale status */
    pwr32(port, PxCI, 1u << 0);     /* issue slot 0 */

    for (int i = 0; i < AHCI_POLL_LIMIT; i++) {
        if ((prd32(port, PxCI) & 1u) == 0) {
            break;
        }
        if (prd32(port, PxIS) & (1u << 30)) { /* TFES: task file error */
            return -EIO;
        }
    }
    if (prd32(port, PxCI) & 1u) {
        return -EIO; /* never completed */
    }
    if (prd32(port, PxTFD) & PxTFD_STS_ERR) {
        return -EIO;
    }
    return 0;
}

/* ---- identification ---- */

static void ahci_model(const uint16_t *id, char *out) {
    for (int i = 0; i < 20; i++) {
        out[i * 2] = (char)(id[27 + i] >> 8);
        out[i * 2 + 1] = (char)(id[27 + i] & 0xFF);
    }
    out[40] = '\0';
    for (int i = 39; i >= 0 && (out[i] == ' ' || out[i] == '\0'); i--) {
        out[i] = '\0';
    }
}

static void ahci_identify_port(uint32_t port, const char *name) {
    struct ahci_port_state *ps = &g_pstate[port];

    if (ahci_issue(port, AHCI_CMD_IDENTIFY, 0, 1, false) != 0) {
        return;
    }
    uint16_t id[256];
    memcpy(id, ps->bounce.virt, sizeof(id));

    uint64_t sectors = (uint64_t)id[100] | ((uint64_t)id[101] << 16) |
                       ((uint64_t)id[102] << 32) | ((uint64_t)id[103] << 48);
    if (sectors == 0) {
        /* No 48-bit LBA support reported: fall back to the 28-bit
         * words, same layout ata.c reads. */
        sectors = (uint32_t)id[60] | ((uint32_t)id[61] << 16);
    }
    if (sectors == 0) {
        return;
    }

    struct ahci_disk *d = &g_disks[g_disk_count];
    d->present = true;
    d->port = port;
    d->sectors = sectors;
    strncpy(d->name, name, sizeof(d->name) - 1);
    ahci_model(id, d->model);
    g_disk_count++;
}

/* ---- transfers ---- */

static struct ahci_disk *ahci_find(int index) {
    if (index < 0 || index >= g_disk_count) {
        return NULL;
    }
    return &g_disks[index];
}

static int ahci_transfer(int index, uint64_t lba, uint32_t count, void *buf,
                         bool write) {
    struct ahci_disk *d = ahci_find(index);
    if (d == NULL) {
        return -ENODEV;
    }
    if (count == 0) {
        return 0;
    }
    if (lba > d->sectors || count > d->sectors - lba) {
        return -EINVAL;
    }

    struct ahci_port_state *ps = &g_pstate[d->port];
    uint8_t *p = (uint8_t *)buf;

    while (count > 0) {
        uint32_t chunk = count > AHCI_MAX_CHUNK ? AHCI_MAX_CHUNK : count;
        size_t bytes = (size_t)chunk * AHCI_SECTOR_SIZE;

        if (write) {
            memcpy(ps->bounce.virt, p, bytes);
        }

        int rc = ahci_issue(d->port,
                            write ? AHCI_CMD_WRITE_DMA_EXT
                                  : AHCI_CMD_READ_DMA_EXT,
                            lba, chunk, write);
        if (rc != 0) {
            return rc;
        }

        if (!write) {
            memcpy(p, ps->bounce.virt, bytes);
        }

        p += bytes;
        lba += chunk;
        count -= chunk;
    }
    return 0;
}

int ahci_read(int index, uint64_t lba, uint32_t count, void *buf) {
    return ahci_transfer(index, lba, count, buf, false);
}

int ahci_write(int index, uint64_t lba, uint32_t count, const void *buf) {
    return ahci_transfer(index, lba, count, (void *)buf, true);
}

const struct ahci_disk *ahci_disk(int index) {
    return ahci_find(index);
}

int ahci_disk_count(void) {
    return g_disk_count;
}

/* ---- /dev/sd* ---- */

static long ahci_disk_read(void *priv, void *buf, size_t count, size_t pos) {
    int index = (int)(long)priv;
    const struct ahci_disk *d = ahci_find(index);
    if (d == NULL) {
        return -ENODEV;
    }
    uint64_t total = d->sectors * AHCI_SECTOR_SIZE;
    if (pos >= total) {
        return 0;
    }
    if (count > total - pos) {
        count = (size_t)(total - pos);
    }

    uint8_t *p = (uint8_t *)buf;
    size_t done = 0;
    static uint8_t bounce[AHCI_SECTOR_SIZE];
    while (done < count) {
        uint64_t lba = (pos + done) / AHCI_SECTOR_SIZE;
        size_t off = (size_t)((pos + done) % AHCI_SECTOR_SIZE);
        size_t room = AHCI_SECTOR_SIZE - off;
        size_t n = count - done < room ? count - done : room;

        if (off == 0 && n == AHCI_SECTOR_SIZE) {
            uint32_t whole = (uint32_t)((count - done) / AHCI_SECTOR_SIZE);
            if (whole > 0xFFFF) whole = 0xFFFF;
            if (ahci_read(index, lba, whole, p + done) != 0) {
                return done > 0 ? (long)done : -EIO;
            }
            done += (size_t)whole * AHCI_SECTOR_SIZE;
            continue;
        }
        if (ahci_read(index, lba, 1, bounce) != 0) {
            return done > 0 ? (long)done : -EIO;
        }
        memcpy(p + done, bounce + off, n);
        done += n;
    }
    return (long)done;
}

static long ahci_disk_write(void *priv, const void *buf, size_t count,
                            size_t pos) {
    int index = (int)(long)priv;
    const struct ahci_disk *d = ahci_find(index);
    if (d == NULL) {
        return -ENODEV;
    }
    uint64_t total = d->sectors * AHCI_SECTOR_SIZE;
    if (pos >= total) {
        return 0;
    }
    if (count > total - pos) {
        count = (size_t)(total - pos);
    }

    const uint8_t *p = (const uint8_t *)buf;
    size_t done = 0;
    static uint8_t bounce[AHCI_SECTOR_SIZE];
    while (done < count) {
        uint64_t lba = (pos + done) / AHCI_SECTOR_SIZE;
        size_t off = (size_t)((pos + done) % AHCI_SECTOR_SIZE);
        size_t room = AHCI_SECTOR_SIZE - off;
        size_t n = count - done < room ? count - done : room;

        if (off == 0 && n == AHCI_SECTOR_SIZE) {
            uint32_t whole = (uint32_t)((count - done) / AHCI_SECTOR_SIZE);
            if (whole > 0xFFFF) whole = 0xFFFF;
            if (ahci_write(index, lba, whole, p + done) != 0) {
                return done > 0 ? (long)done : -EIO;
            }
            done += (size_t)whole * AHCI_SECTOR_SIZE;
            continue;
        }
        if (ahci_read(index, lba, 1, bounce) != 0) {
            return done > 0 ? (long)done : -EIO;
        }
        memcpy(bounce + off, p + done, n);
        if (ahci_write(index, lba, 1, bounce) != 0) {
            return done > 0 ? (long)done : -EIO;
        }
        done += n;
    }
    return (long)done;
}

static const struct file_ops ahci_disk_ops = {
    ahci_disk_read, ahci_disk_write, NULL, NULL
};

static void ahci_register_devices(void) {
    for (int i = 0; i < g_disk_count; i++) {
        char path[16];
        strcpy(path, "/dev/");
        strcpy(path + 5, g_disks[i].name);
        struct vfs_node *node = vfs_create_device(path, &ahci_disk_ops,
                                                  (void *)(long)i);
        if (node != NULL) {
            node->size = (size_t)(g_disks[i].sectors * AHCI_SECTOR_SIZE);
        }
        kprintf("ahci: %s: %s, %llu sectors\n", g_disks[i].name,
                g_disks[i].model, (unsigned long long)g_disks[i].sectors);
    }
}

/* ---- PCI discovery and controller bring-up ---- */

int ahci_init(void) {
    memset(g_disks, 0, sizeof(g_disks));
    memset(g_pstate, 0, sizeof(g_pstate));
    g_disk_count = 0;

    uint8_t found_bus = 0, found_dev = 0, found_fn = 0;
    bool found = false;

    for (uint16_t bus = 0; bus < 256 && !found; bus++) {
        for (uint8_t dev = 0; dev < 32 && !found; dev++) {
            for (uint8_t fn = 0; fn < 8; fn++) {
                uint32_t id = pci_config_read((uint8_t)bus, dev, fn, PCI_VENDOR_ID);
                if ((id & 0xFFFF) == 0xFFFF) {
                    if (fn == 0) break;
                    continue;
                }
                uint32_t cls = pci_config_read((uint8_t)bus, dev, fn, PCI_REVISION);
                uint8_t class_code = (cls >> 24) & 0xFF;
                uint8_t subclass = (cls >> 16) & 0xFF;
                if (class_code == 0x01 && subclass == 0x06) {
                    found_bus = (uint8_t)bus;
                    found_dev = dev;
                    found_fn = fn;
                    found = true;
                    break;
                }
            }
        }
    }
    if (!found) {
        return -1;
    }

    uint32_t bar5 = pci_config_read(found_bus, found_dev, found_fn, PCI_BAR5);
    if (bar5 & 0x1) {
        kprintf("ahci: BAR5 is not memory-mapped (%08x)\n", bar5);
        return -1;
    }
    uint64_t base = bar5 & ~0xFu;

    uint32_t cmd = pci_config_read(found_bus, found_dev, found_fn, PCI_COMMAND);
    cmd |= PCI_COMMAND_MEM_SPACE | PCI_COMMAND_MASTER;
    pci_config_write(found_bus, found_dev, found_fn, PCI_COMMAND, cmd);

    g_abar = (volatile uint8_t *)vmm_map_mmio(base, 0x1100);
    if (g_abar == NULL) {
        kprintf("ahci: could not map ABAR\n");
        return -1;
    }

    kprintf("ahci: controller at %02x:%02x.%u, ABAR 0x%lx\n",
            found_bus, found_dev, found_fn, (unsigned long)base);

    /* AHCI Enable, then a full HBA reset and wait for it to clear. */
    wr32(AHCI_GHC, rd32(AHCI_GHC) | AHCI_GHC_AE);
    wr32(AHCI_GHC, rd32(AHCI_GHC) | AHCI_GHC_HR);
    for (int i = 0; i < AHCI_POLL_LIMIT; i++) {
        if ((rd32(AHCI_GHC) & AHCI_GHC_HR) == 0) {
            break;
        }
    }
    wr32(AHCI_GHC, rd32(AHCI_GHC) | AHCI_GHC_AE);

    g_ports_implemented = rd32(AHCI_PI);

    static const char letters[] = "abcdefghijklmnopqrstuvwxyz";
    for (uint32_t port = 0; port < AHCI_MAX_PORTS; port++) {
        if ((g_ports_implemented & (1u << port)) == 0) {
            continue;
        }
        uint32_t ssts = prd32(port, PxSSTS);
        uint32_t det = ssts & 0xF;
        if (det != 3) {
            continue; /* no device present on this port */
        }

        if (ahci_port_stop_cmd(port) != 0) {
            kprintf("ahci: port %u would not stop, skipping\n", port);
            continue;
        }
        if (ahci_port_alloc(port) != 0) {
            continue;
        }

        /* The HBA reset above (GHC.HR) clears every port's signature
         * register back to 0xFFFFFFFF; it is only repopulated once
         * this port's FIS receive is running and the device has sent
         * its first D2H FIS, which takes a moment after COMRESET -
         * so FRE (and SUD, for controllers with staggered spin-up)
         * have to come up and the signature has to be waited for
         * before it says anything meaningful. */
        pwr32(port, PxCMD, prd32(port, PxCMD) | PxCMD_FRE | (1u << 1) /* SUD */);
        uint32_t sig = AHCI_SIG_ATA ^ 1; /* anything != every known sig */
        for (int i = 0; i < AHCI_POLL_LIMIT; i++) {
            sig = prd32(port, PxSIG);
            if (sig != 0xFFFFFFFFu) {
                break;
            }
        }
        kprintf("ahci: port %u ssts=0x%x sig=0x%x\n", port, ssts, sig);

        if (sig == AHCI_SIG_ATAPI || sig == AHCI_SIG_SEMB ||
            sig == AHCI_SIG_PM) {
            continue; /* detected, not handled - like ata.c and ATAPI */
        }
        if (sig != AHCI_SIG_ATA) {
            continue;
        }

        ahci_port_start_cmd(port);

        if ((size_t)g_disk_count >= 26) {
            break;
        }
        char name[8] = "sd?";
        name[2] = letters[g_disk_count];
        ahci_identify_port(port, name);
    }

    ahci_register_devices();
    return 0;
}
