/*
 * acpi.c - RSDP -> RSDT/XSDT -> MADT
 *
 * Every table here lives in ordinary physical RAM (marked "ACPI
 * reclaimable" or "ACPI NVS" in the memory map, not MMIO), so
 * pmm_phys_to_virt() - the same phys+HHDM-offset arithmetic PMM uses
 * for page frames - is all that is needed to read them. No page
 * tables have to be built for this, unlike the Local APIC and I/O
 * APIC registers themselves (see lapic.c/ioapic.c), which really are
 * MMIO and go through vmm_map_mmio().
 */

#include "acpi.h"

#include "mm/pmm.h"

#include <stddef.h>

struct acpi_rsdp {
    char sig[8];
    uint8_t checksum;
    char oem[6];
    uint8_t revision;
    uint32_t rsdt_addr;
    /* ACPI 2.0+ only (revision >= 2); a revision-1 table may be
     * shorter than this struct, so these fields are read only after
     * checking `revision`. */
    uint32_t length;
    uint64_t xsdt_addr;
    uint8_t ext_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct acpi_sdt_header {
    char sig[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct acpi_madt {
    struct acpi_sdt_header hdr;
    uint32_t lapic_addr;
    uint32_t flags;
    /* variable-length entries follow */
} __attribute__((packed));

static void *phys_to_ptr(uint64_t phys) {
    return (void *)pmm_phys_to_virt(phys);
}

static int checksum_ok(const void *table, uint32_t length) {
    const uint8_t *p = (const uint8_t *)table;
    uint8_t sum = 0;
    for (uint32_t i = 0; i < length; i++) {
        sum = (uint8_t)(sum + p[i]);
    }
    return sum == 0;
}

static const struct acpi_sdt_header *find_table(const struct acpi_rsdp *rsdp,
                                                 const char sig[4]) {
    int use_xsdt = rsdp->revision >= 2 && rsdp->xsdt_addr != 0;
    const struct acpi_sdt_header *root = use_xsdt
        ? (const struct acpi_sdt_header *)phys_to_ptr(rsdp->xsdt_addr)
        : (const struct acpi_sdt_header *)phys_to_ptr(rsdp->rsdt_addr);
    if (root == NULL || !checksum_ok(root, root->length)) {
        return NULL;
    }

    const uint8_t *entries = (const uint8_t *)root + sizeof(*root);
    uint32_t entry_bytes = use_xsdt ? 8 : 4;
    uint32_t count = (root->length - (uint32_t)sizeof(*root)) / entry_bytes;

    for (uint32_t i = 0; i < count; i++) {
        uint64_t table_phys = use_xsdt
            ? *(const uint64_t *)(entries + i * 8)
            : *(const uint32_t *)(entries + i * 4);
        const struct acpi_sdt_header *t =
            (const struct acpi_sdt_header *)phys_to_ptr(table_phys);
        if (t == NULL) {
            continue;
        }
        if (t->sig[0] == sig[0] && t->sig[1] == sig[1] &&
            t->sig[2] == sig[2] && t->sig[3] == sig[3] &&
            checksum_ok(t, t->length)) {
            return t;
        }
    }
    return NULL;
}

int acpi_parse_madt(void *rsdp_virt, struct acpi_madt_info *out) {
    if (rsdp_virt == NULL || out == NULL) {
        return -1;
    }
    const struct acpi_rsdp *rsdp = (const struct acpi_rsdp *)rsdp_virt;
    if (rsdp->sig[0] != 'R' || rsdp->sig[1] != 'S' || rsdp->sig[2] != 'D' ||
        rsdp->sig[3] != ' ' || rsdp->sig[4] != 'P' || rsdp->sig[5] != 'T' ||
        rsdp->sig[6] != 'R' || rsdp->sig[7] != ' ') {
        return -1;
    }
    /* The revision-1 portion is the only part guaranteed present. */
    if (!checksum_ok(rsdp, 20)) {
        return -1;
    }

    const struct acpi_sdt_header *madt_hdr = find_table(rsdp, "APIC");
    if (madt_hdr == NULL) {
        return -1;
    }
    const struct acpi_madt *madt = (const struct acpi_madt *)madt_hdr;

    out->ioapic_phys = 0;
    out->ioapic_gsi_base = 0;
    out->has_lapic_override = 0;
    out->lapic_phys = 0;
    out->n_override = 0;
    out->n_cpu = 0;

    const uint8_t *p = (const uint8_t *)madt + sizeof(*madt);
    const uint8_t *end = (const uint8_t *)madt + madt->hdr.length;

    while (p + 2 <= end) {
        uint8_t type = p[0];
        uint8_t len = p[1];
        if (len < 2 || p + len > end) {
            break; /* truncated or corrupt entry: stop, keep what we have */
        }

        if (type == 0 && len >= 8 && out->n_cpu < ACPI_MAX_CPUS) {
            /* Processor Local APIC: acpi_processor_id(1) apic_id(1)
             * flags(4, bit0 = enabled) */
            uint32_t flags;
            __builtin_memcpy(&flags, p + 4, 4);
            out->cpu[out->n_cpu].acpi_processor_id = p[2];
            out->cpu[out->n_cpu].apic_id = p[3];
            out->cpu[out->n_cpu].enabled = (flags & 1) ? 1 : 0;
            out->n_cpu++;
        } else if (type == 1 && len >= 12 && out->ioapic_phys == 0) {
            /* I/O APIC: id(1) reserved(1) address(4) gsi_base(4) */
            uint32_t addr;
            uint32_t gsi_base;
            __builtin_memcpy(&addr, p + 4, 4);
            __builtin_memcpy(&gsi_base, p + 8, 4);
            out->ioapic_phys = addr;
            out->ioapic_gsi_base = gsi_base;
        } else if (type == 2 && len >= 10 &&
                  out->n_override < ACPI_MAX_OVERRIDES) {
            /* Interrupt Source Override: bus(1) source(1) gsi(4) flags(2) */
            uint32_t gsi;
            __builtin_memcpy(&gsi, p + 4, 4);
            out->override[out->n_override].isa_irq = p[3];
            out->override[out->n_override].gsi = gsi;
            out->n_override++;
        } else if (type == 5 && len >= 12) {
            /* Local APIC Address Override: reserved(2) address(8) */
            uint64_t addr;
            __builtin_memcpy(&addr, p + 4, 8);
            out->lapic_phys = addr;
            out->has_lapic_override = 1;
        }

        p += len;
    }

    return out->ioapic_phys != 0 ? 0 : -1;
}

/* Common preamble for the two functions below: walk the RSDP down to
 * the RSDT/XSDT the same way acpi_parse_madt() does, and hand back the
 * requested table (or NULL if it does not exist / fails checksum). */
static const struct acpi_sdt_header *find_table_from_rsdp(void *rsdp_virt,
                                                           const char sig[4]) {
    if (rsdp_virt == NULL) {
        return NULL;
    }
    const struct acpi_rsdp *rsdp = (const struct acpi_rsdp *)rsdp_virt;
    if (rsdp->sig[0] != 'R' || rsdp->sig[1] != 'S' || rsdp->sig[2] != 'D' ||
        rsdp->sig[3] != ' ' || rsdp->sig[4] != 'P' || rsdp->sig[5] != 'T' ||
        rsdp->sig[6] != 'R' || rsdp->sig[7] != ' ') {
        return NULL;
    }
    if (!checksum_ok(rsdp, 20)) {
        return NULL;
    }
    return find_table(rsdp, sig);
}

/* FADT layout below the common 36-byte SDT header (ACPI spec, fixed
 * since ACPI 1.0's 116-byte FADT - later revisions only append fields
 * after PM_TMR_BLK/Flags, never move them):
 *   offset 76 (36+40): PM_TMR_BLK, uint32 I/O port
 *   offset 112 (36+76): Flags, uint32, bit 8 = TMR_VAL_EXT
 * Read with memcpy at fixed offsets rather than a packed struct - the
 * fields in between are irrelevant here and this keeps the "did the
 * table layout guess get the offset right" question to two numbers
 * instead of a whole struct's worth of padding/alignment to get wrong. */
#define FADT_OFF_PM_TMR_BLK 76
#define FADT_OFF_FLAGS      112
#define FADT_TMR_VAL_EXT    (1u << 8)
#define FADT_MIN_LEN        116 /* the whole ACPI 1.0 FADT; guarantees
                                  * both fields above are present */

int acpi_parse_fadt(void *rsdp_virt, struct acpi_fadt_info *out) {
    if (out == NULL) {
        return -1;
    }
    out->pm_tmr_blk = 0;
    out->tmr_val_ext = 0;

    const struct acpi_sdt_header *fadt = find_table_from_rsdp(rsdp_virt, "FACP");
    if (fadt == NULL || fadt->length < FADT_MIN_LEN) {
        return -1;
    }
    const uint8_t *base = (const uint8_t *)fadt;

    uint32_t pm_tmr_blk;
    __builtin_memcpy(&pm_tmr_blk, base + FADT_OFF_PM_TMR_BLK, 4);
    if (pm_tmr_blk == 0) {
        return -1; /* this machine genuinely has no ACPI PM timer */
    }

    uint32_t flags;
    __builtin_memcpy(&flags, base + FADT_OFF_FLAGS, 4);

    out->pm_tmr_blk = pm_tmr_blk;
    out->tmr_val_ext = (flags & FADT_TMR_VAL_EXT) ? 1 : 0;
    return 0;
}

/* HPET table layout below the common 36-byte SDT header (ACPI's HPET
 * specification, "HPET Description Table"):
 *   offset 40 (36+4, past a packed hardware/event-timer-block ID
 *              dword this driver has no use for): a 12-byte Generic
 *              Address Structure whose last 8 bytes (offset 44) are
 *              the MMIO base address. address_space_id (offset 40)
 *              should be 0 (system memory) - HPET is always MMIO, but
 *              checked anyway since a non-zero value would mean this
 *              address is not one vmm_map_mmio() can use as-is. */
#define HPET_OFF_ADDR_SPACE 40
#define HPET_OFF_ADDRESS    44
#define HPET_MIN_LEN        52

int acpi_parse_hpet(void *rsdp_virt, struct acpi_hpet_info *out) {
    if (out == NULL) {
        return -1;
    }
    out->phys_addr = 0;

    const struct acpi_sdt_header *hpet = find_table_from_rsdp(rsdp_virt, "HPET");
    if (hpet == NULL || hpet->length < HPET_MIN_LEN) {
        return -1;
    }
    const uint8_t *base = (const uint8_t *)hpet;

    if (base[HPET_OFF_ADDR_SPACE] != 0) {
        return -1; /* not system memory - nothing vmm_map_mmio() can use */
    }
    uint64_t addr;
    __builtin_memcpy(&addr, base + HPET_OFF_ADDRESS, 8);
    if (addr == 0) {
        return -1;
    }
    out->phys_addr = addr;
    return 0;
}
