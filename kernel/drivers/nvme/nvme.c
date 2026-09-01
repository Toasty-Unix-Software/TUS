/*
 * nvme.c - NVMe controller driver, implementation
 *
 * Polled throughout: no MSI-X wiring (see nvme.h), so every command
 * this driver issues is followed by a bounded spin on the completion
 * queue's phase bit, the same shape ata.c's PIO waits and ahci.c's
 * command-slot waits take. One admin queue pair (depth 2, enough for
 * strictly sequential setup commands) and one I/O queue pair (depth
 * NVME_IOQ_DEPTH) are created; only queue ID 1 is ever used for I/O,
 * so there is no submission-queue full condition to handle beyond the
 * depth check in nvme_issue_io().
 *
 * PRPs, not PRDTs: NVMe's scatter-gather is two physical pointers per
 * command (PRP1/PRP2) plus, for anything larger, a PRP list - this
 * driver never needs the list because transfers are bounced through a
 * single physically-contiguous DMA buffer capped at NVME_MAX_CHUNK
 * sectors, so PRP1 alone (one 4 KiB-aligned page run) always covers
 * it for the small chunk sizes used here... except the buffer can be
 * larger than one page, so PRP1 points at the first page and PRP2 is
 * used as NVMe defines for a two-page transfer, capping the chunk
 * size at 2 pages (8 KiB, 16 sectors) keeps this simple and correct
 * without a PRP list.
 */

#include "drivers/nvme/nvme.h"

#include "core/console.h"
#include "core/errno.h"
#include "core/klib.h"
#include "drivers/pci/pci.h"
#include "mm/dma.h"
#include "mm/vmm.h"
#include "vfs/vfs.h"

/* ---- controller registers, offsets from BAR0 ---- */
#define NVME_REG_CAP   0x00 /* 64-bit */
#define NVME_REG_VS    0x08
#define NVME_REG_INTMS 0x0C
#define NVME_REG_INTMC 0x10
#define NVME_REG_CC    0x14
#define NVME_REG_CSTS  0x1C
#define NVME_REG_AQA   0x24
#define NVME_REG_ASQ   0x28 /* 64-bit */
#define NVME_REG_ACQ   0x30 /* 64-bit */
#define NVME_REG_DOORBELL_BASE 0x1000

#define NVME_CC_EN     (1u << 0)
#define NVME_CC_CSS_NVM (0u << 4)
#define NVME_CC_MPS_SHIFT 7
#define NVME_CC_AMS_RR  (0u << 11)
#define NVME_CC_SHN_NONE (0u << 14)
#define NVME_CC_IOSQES_SHIFT 16 /* 6 -> 64 bytes */
#define NVME_CC_IOCQES_SHIFT 20 /* 4 -> 16 bytes */

#define NVME_CSTS_RDY (1u << 0)
#define NVME_CSTS_CFS (1u << 1)

#define NVME_SQE_SIZE 64
#define NVME_CQE_SIZE 16

#define NVME_ADMIN_QDEPTH 2
#define NVME_IOQ_DEPTH    16

/* Two 4 KiB pages: PRP1 + PRP2 cover it with no PRP list. */
#define NVME_MAX_CHUNK ((4096u * 2u) / NVME_SECTOR_SIZE)

#define NVME_OP_CREATE_IO_SQ 0x01
#define NVME_OP_CREATE_IO_CQ 0x05
#define NVME_OP_IDENTIFY     0x06

#define NVME_OP_IO_WRITE 0x01
#define NVME_OP_IO_READ  0x02

#define NVME_POLL_LIMIT 4000000

struct nvme_sqe {
    uint32_t cdw0;
    uint32_t nsid;
    uint64_t rsvd2;
    uint64_t mptr;
    uint64_t prp1;
    uint64_t prp2;
    uint32_t cdw10;
    uint32_t cdw11;
    uint32_t cdw12;
    uint32_t cdw13;
    uint32_t cdw14;
    uint32_t cdw15;
};

struct nvme_cqe {
    uint32_t dw0;
    uint32_t rsvd;
    uint16_t sq_head;
    uint16_t sq_id;
    uint16_t cid;
    uint16_t status; /* bit0 = phase */
};

struct nvme_queue {
    struct dma_buf sq;
    struct dma_buf cq;
    uint32_t depth;
    uint16_t sq_tail;
    uint16_t cq_head;
    uint16_t phase;
    uint16_t qid;
};

static volatile uint8_t *g_bar;
static uint32_t g_doorbell_stride; /* bytes between successive doorbells */
static struct nvme_queue g_admin;
static struct nvme_queue g_io;
static struct dma_buf g_identify_buf;
static struct dma_buf g_bounce;

static struct nvme_namespace g_ns[NVME_MAX_NAMESPACES];
static int g_ns_count;

static inline uint32_t rd32(uint32_t off) {
    return *(volatile uint32_t *)(g_bar + off);
}
static inline void wr32(uint32_t off, uint32_t v) {
    *(volatile uint32_t *)(g_bar + off) = v;
}
static inline uint64_t rd64(uint32_t off) {
    return *(volatile uint64_t *)(g_bar + off);
}
static inline void wr64(uint32_t off, uint64_t v) {
    *(volatile uint64_t *)(g_bar + off) = v;
}

static inline uint32_t sq_doorbell(uint16_t qid) {
    return NVME_REG_DOORBELL_BASE + (uint32_t)(2 * qid) * g_doorbell_stride;
}
static inline uint32_t cq_doorbell(uint16_t qid) {
    return NVME_REG_DOORBELL_BASE + (uint32_t)(2 * qid + 1) * g_doorbell_stride;
}

/* ---- queue plumbing ---- */

static int nvme_queue_alloc(struct nvme_queue *q, uint16_t qid, uint32_t depth) {
    memset(q, 0, sizeof(*q));
    q->sq = dma_alloc((size_t)depth * NVME_SQE_SIZE);
    q->cq = dma_alloc((size_t)depth * NVME_CQE_SIZE);
    if (!q->sq.virt || !q->cq.virt) {
        return -1;
    }
    q->depth = depth;
    q->phase = 1;
    q->qid = qid;
    return 0;
}

/* Submit a fully-built entry to `q`, ring its doorbell, then poll the
 * matching completion entry. Returns the completion's status field
 * (0 = success), or -1 on timeout. Admin and I/O completions share
 * this same shape, just different queue/doorbell pairs. */
static int nvme_submit_and_wait(struct nvme_queue *q, const struct nvme_sqe *cmd) {
    struct nvme_sqe *sqe = (struct nvme_sqe *)q->sq.virt;
    sqe[q->sq_tail] = *cmd;

    q->sq_tail = (uint16_t)((q->sq_tail + 1) % q->depth);
    wr32(sq_doorbell(q->qid), q->sq_tail);

    struct nvme_cqe *cqe = (struct nvme_cqe *)q->cq.virt;
    int limit = NVME_POLL_LIMIT;
    while (limit-- > 0) {
        uint16_t status = cqe[q->cq_head].status;
        if ((status & 1u) == q->phase) {
            uint16_t st = (uint16_t)(status >> 1); /* drop phase bit */
            q->cq_head = (uint16_t)((q->cq_head + 1) % q->depth);
            if (q->cq_head == 0) {
                q->phase ^= 1;
            }
            wr32(cq_doorbell(q->qid), q->cq_head);
            return st & 0x7FFu; /* status code, ignoring DNR/more bits */
        }
    }
    return -1;
}

/* ---- identify ---- */

static int nvme_identify(uint32_t nsid, uint8_t cns, void *out512) {
    struct nvme_sqe cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.cdw0 = NVME_OP_IDENTIFY;
    cmd.nsid = nsid;
    cmd.prp1 = g_identify_buf.phys;
    cmd.cdw10 = cns;

    int rc = nvme_submit_and_wait(&g_admin, &cmd);
    if (rc != 0) {
        return -EIO;
    }
    memcpy(out512, g_identify_buf.virt, 4096);
    return 0;
}

/* ---- I/O queue creation ---- */

static int nvme_create_io_queues(void) {
    if (nvme_queue_alloc(&g_io, 1, NVME_IOQ_DEPTH) != 0) {
        kprintf("nvme: no DMA memory for the I/O queue pair\n");
        return -1;
    }

    struct nvme_sqe cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.cdw0 = NVME_OP_CREATE_IO_CQ;
    cmd.prp1 = g_io.cq.phys;
    cmd.cdw10 = ((uint32_t)(NVME_IOQ_DEPTH - 1) << 16) | g_io.qid;
    cmd.cdw11 = 1u << 0; /* physically contiguous, interrupts disabled */
    if (nvme_submit_and_wait(&g_admin, &cmd) != 0) {
        kprintf("nvme: create I/O completion queue failed\n");
        return -1;
    }

    memset(&cmd, 0, sizeof(cmd));
    cmd.cdw0 = NVME_OP_CREATE_IO_SQ;
    cmd.prp1 = g_io.sq.phys;
    cmd.cdw10 = ((uint32_t)(NVME_IOQ_DEPTH - 1) << 16) | g_io.qid;
    cmd.cdw11 = (uint32_t)(g_io.qid << 16) | (1u << 0); /* CQ id, contiguous */
    if (nvme_submit_and_wait(&g_admin, &cmd) != 0) {
        kprintf("nvme: create I/O submission queue failed\n");
        return -1;
    }
    return 0;
}

/* ---- namespaces ---- */

static struct nvme_namespace *nvme_find(int index) {
    if (index < 0 || index >= g_ns_count) {
        return NULL;
    }
    return &g_ns[index];
}

static int nvme_issue_io(uint8_t opcode, uint32_t nsid, uint64_t lba,
                         uint32_t count) {
    struct nvme_sqe cmd;
    memset(&cmd, 0, sizeof(cmd));
    cmd.cdw0 = opcode;
    cmd.nsid = nsid;
    cmd.prp1 = g_bounce.phys;
    if ((size_t)count * NVME_SECTOR_SIZE > 4096) {
        cmd.prp2 = g_bounce.phys + 4096;
    }
    cmd.cdw10 = (uint32_t)(lba & 0xFFFFFFFFu);
    cmd.cdw11 = (uint32_t)(lba >> 32);
    cmd.cdw12 = count - 1; /* 0-based number of blocks */

    int rc = nvme_submit_and_wait(&g_io, &cmd);
    return rc == 0 ? 0 : -EIO;
}

static int nvme_transfer(int index, uint64_t lba, uint32_t count, void *buf,
                         bool write) {
    struct nvme_namespace *ns = nvme_find(index);
    if (ns == NULL) {
        return -ENODEV;
    }
    if (count == 0) {
        return 0;
    }
    if (lba > ns->sectors || count > ns->sectors - lba) {
        return -EINVAL;
    }

    uint8_t *p = (uint8_t *)buf;
    while (count > 0) {
        uint32_t chunk = count > NVME_MAX_CHUNK ? NVME_MAX_CHUNK : count;
        size_t bytes = (size_t)chunk * NVME_SECTOR_SIZE;

        if (write) {
            memcpy(g_bounce.virt, p, bytes);
        }
        int rc = nvme_issue_io(write ? NVME_OP_IO_WRITE : NVME_OP_IO_READ,
                               ns->nsid, lba, chunk);
        if (rc != 0) {
            return rc;
        }
        if (!write) {
            memcpy(p, g_bounce.virt, bytes);
        }

        p += bytes;
        lba += chunk;
        count -= chunk;
    }
    return 0;
}

int nvme_read(int index, uint64_t lba, uint32_t count, void *buf) {
    return nvme_transfer(index, lba, count, buf, false);
}

int nvme_write(int index, uint64_t lba, uint32_t count, const void *buf) {
    return nvme_transfer(index, lba, count, (void *)buf, true);
}

const struct nvme_namespace *nvme_namespace(int index) {
    return nvme_find(index);
}

int nvme_namespace_count(void) {
    return g_ns_count;
}

/* ---- /dev/nvme0n1 ---- */

static long nvme_ns_read(void *priv, void *buf, size_t count, size_t pos) {
    int index = (int)(long)priv;
    const struct nvme_namespace *ns = nvme_find(index);
    if (ns == NULL) {
        return -ENODEV;
    }
    uint64_t total = ns->sectors * NVME_SECTOR_SIZE;
    if (pos >= total) return 0;
    if (count > total - pos) count = (size_t)(total - pos);

    uint8_t *p = (uint8_t *)buf;
    size_t done = 0;
    static uint8_t bounce[NVME_SECTOR_SIZE];
    while (done < count) {
        uint64_t lba = (pos + done) / NVME_SECTOR_SIZE;
        size_t off = (size_t)((pos + done) % NVME_SECTOR_SIZE);
        size_t room = NVME_SECTOR_SIZE - off;
        size_t n = count - done < room ? count - done : room;

        if (off == 0 && n == NVME_SECTOR_SIZE) {
            uint32_t whole = (uint32_t)((count - done) / NVME_SECTOR_SIZE);
            if (nvme_read(index, lba, whole, p + done) != 0) {
                return done > 0 ? (long)done : -EIO;
            }
            done += (size_t)whole * NVME_SECTOR_SIZE;
            continue;
        }
        if (nvme_read(index, lba, 1, bounce) != 0) {
            return done > 0 ? (long)done : -EIO;
        }
        memcpy(p + done, bounce + off, n);
        done += n;
    }
    return (long)done;
}

static long nvme_ns_write(void *priv, const void *buf, size_t count, size_t pos) {
    int index = (int)(long)priv;
    const struct nvme_namespace *ns = nvme_find(index);
    if (ns == NULL) {
        return -ENODEV;
    }
    uint64_t total = ns->sectors * NVME_SECTOR_SIZE;
    if (pos >= total) return 0;
    if (count > total - pos) count = (size_t)(total - pos);

    const uint8_t *p = (const uint8_t *)buf;
    size_t done = 0;
    static uint8_t bounce[NVME_SECTOR_SIZE];
    while (done < count) {
        uint64_t lba = (pos + done) / NVME_SECTOR_SIZE;
        size_t off = (size_t)((pos + done) % NVME_SECTOR_SIZE);
        size_t room = NVME_SECTOR_SIZE - off;
        size_t n = count - done < room ? count - done : room;

        if (off == 0 && n == NVME_SECTOR_SIZE) {
            uint32_t whole = (uint32_t)((count - done) / NVME_SECTOR_SIZE);
            if (nvme_write(index, lba, whole, p + done) != 0) {
                return done > 0 ? (long)done : -EIO;
            }
            done += (size_t)whole * NVME_SECTOR_SIZE;
            continue;
        }
        if (nvme_read(index, lba, 1, bounce) != 0) {
            return done > 0 ? (long)done : -EIO;
        }
        memcpy(bounce + off, p + done, n);
        if (nvme_write(index, lba, 1, bounce) != 0) {
            return done > 0 ? (long)done : -EIO;
        }
        done += n;
    }
    return (long)done;
}

static const struct file_ops nvme_ns_ops = { nvme_ns_read, nvme_ns_write, NULL, NULL };

static void nvme_register_devices(void) {
    for (int i = 0; i < g_ns_count; i++) {
        char path[24];
        strcpy(path, "/dev/");
        strcpy(path + 5, g_ns[i].name);
        struct vfs_node *node = vfs_create_device(path, &nvme_ns_ops, (void *)(long)i);
        if (node != NULL) {
            node->size = (size_t)(g_ns[i].sectors * NVME_SECTOR_SIZE);
        }
        kprintf("nvme: %s: %llu sectors\n", g_ns[i].name,
                (unsigned long long)g_ns[i].sectors);
    }
}

/* ---- bring-up ---- */

int nvme_init(void) {
    memset(g_ns, 0, sizeof(g_ns));
    g_ns_count = 0;

    uint8_t fbus = 0, fdev = 0, ffn = 0;
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
                if (((cls >> 24) & 0xFF) == 0x01 && ((cls >> 16) & 0xFF) == 0x08) {
                    fbus = (uint8_t)bus; fdev = dev; ffn = fn;
                    found = true;
                    break;
                }
            }
        }
    }
    if (!found) {
        return -1;
    }

    uint32_t bar0 = pci_config_read(fbus, fdev, ffn, PCI_BAR0);
    if (bar0 & 0x1) {
        kprintf("nvme: BAR0 is not memory-mapped\n");
        return -1;
    }
    uint64_t base = bar0 & ~0xFu;
    if (((bar0 >> 1) & 0x3) == 0x2) {
        base |= (uint64_t)pci_config_read(fbus, fdev, ffn, PCI_BAR1) << 32;
    }

    uint32_t cmd = pci_config_read(fbus, fdev, ffn, PCI_COMMAND);
    cmd |= PCI_COMMAND_MEM_SPACE | PCI_COMMAND_MASTER;
    pci_config_write(fbus, fdev, ffn, PCI_COMMAND, cmd);

    g_bar = (volatile uint8_t *)vmm_map_mmio(base, 0x2000);
    if (g_bar == NULL) {
        kprintf("nvme: could not map BAR0\n");
        return -1;
    }
    kprintf("nvme: controller at %02x:%02x.%u, BAR0 0x%lx\n",
            fbus, fdev, ffn, (unsigned long)base);

    uint64_t cap = rd64(NVME_REG_CAP);
    g_doorbell_stride = 4u << ((cap >> 32) & 0xF);

    /* Disable the controller before touching AQA/ASQ/ACQ - the spec
     * requires CC.EN to be 0 while they are configured. */
    wr32(NVME_REG_CC, rd32(NVME_REG_CC) & ~NVME_CC_EN);
    for (int i = 0; i < NVME_POLL_LIMIT; i++) {
        if ((rd32(NVME_REG_CSTS) & NVME_CSTS_RDY) == 0) break;
    }

    if (nvme_queue_alloc(&g_admin, 0, NVME_ADMIN_QDEPTH) != 0) {
        kprintf("nvme: no DMA memory for the admin queue pair\n");
        return -1;
    }
    g_identify_buf = dma_alloc(4096);
    g_bounce = dma_alloc((size_t)NVME_MAX_CHUNK * NVME_SECTOR_SIZE);
    if (!g_identify_buf.virt || !g_bounce.virt) {
        kprintf("nvme: no DMA memory for identify/bounce buffers\n");
        return -1;
    }

    wr32(NVME_REG_AQA, ((uint32_t)(NVME_ADMIN_QDEPTH - 1) << 16) |
                       (uint32_t)(NVME_ADMIN_QDEPTH - 1));
    wr64(NVME_REG_ASQ, g_admin.sq.phys);
    wr64(NVME_REG_ACQ, g_admin.cq.phys);

    uint32_t cc = NVME_CC_CSS_NVM | (0u << NVME_CC_MPS_SHIFT) | NVME_CC_AMS_RR |
                 NVME_CC_SHN_NONE |
                 (6u << NVME_CC_IOSQES_SHIFT) | (4u << NVME_CC_IOCQES_SHIFT) |
                 NVME_CC_EN;
    wr32(NVME_REG_CC, cc);

    bool ready = false;
    for (int i = 0; i < NVME_POLL_LIMIT; i++) {
        uint32_t csts = rd32(NVME_REG_CSTS);
        if (csts & NVME_CSTS_CFS) {
            kprintf("nvme: controller reports fatal status\n");
            return -1;
        }
        if (csts & NVME_CSTS_RDY) {
            ready = true;
            break;
        }
    }
    if (!ready) {
        kprintf("nvme: controller did not become ready\n");
        return -1;
    }

    if (nvme_create_io_queues() != 0) {
        return -1;
    }

    /* Identify namespace 1 only - TUS has no need to enumerate an
     * active namespace list for a controller that, under QEMU, is
     * given exactly one. */
    uint8_t id[4096];
    if (nvme_identify(1, 0 /* CNS: namespace */, id) == 0) {
        uint64_t nsze;
        memcpy(&nsze, id, sizeof(nsze));
        if (nsze != 0) {
            struct nvme_namespace *ns = &g_ns[0];
            ns->present = true;
            ns->nsid = 1;
            ns->sectors = nsze;
            strcpy(ns->name, "nvme0n1");
            g_ns_count = 1;
        }
    }

    nvme_register_devices();
    return 0;
}
