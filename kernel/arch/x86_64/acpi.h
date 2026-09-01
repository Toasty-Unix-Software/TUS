/*
 * acpi.h - just enough ACPI to find the MADT
 *
 * TUS has no general ACPI stack (no AML interpreter, no _PRT, no
 * power management) - this exists purely to answer one question:
 * where are the Local APIC and I/O APIC, and does any ISA IRQ get
 * rerouted to a different pin than its number would suggest. That is
 * the Multiple APIC Description Table (MADT, ACPI signature "APIC"),
 * reached from the RSDP Limine hands the kernel at boot.
 */

#ifndef TUS_ARCH_ACPI_H
#define TUS_ARCH_ACPI_H

#include <stdint.h>

#define ACPI_MAX_OVERRIDES 16

struct acpi_madt_info {
    uint32_t ioapic_phys;     /* 0 if the MADT has no I/O APIC entry */
    uint32_t ioapic_gsi_base; /* almost always 0: GSI n == IRQ n */

    int has_lapic_override;
    uint64_t lapic_phys;      /* only meaningful if the flag above is set */

    /* ISA IRQ -> Global System Interrupt remaps (MADT type 2 entries).
     * The classic example is IRQ0: on a real PC chipset (and QEMU's
     * emulation of one) the timer is wired to I/O APIC pin 2, not pin
     * 0, and the MADT is the only way to learn that - guessing wrong
     * here means the redirection table entry points at a pin nothing
     * is connected to, and the timer interrupt never arrives. */
    struct {
        uint8_t isa_irq;
        uint32_t gsi;
    } override[ACPI_MAX_OVERRIDES];
    int n_override;
};

/* Parse the RSDP Limine found (g_bootinfo.rsdp) down to the MADT.
 * Returns 0 and fills `out` when a usable MADT with an I/O APIC entry
 * was found; -1 on anything else - no RSDP, a bad checksum, no MADT,
 * no I/O APIC entry. -1 is not an error the caller reports: it means
 * "fall back to the 8259", which pic_init() does unconditionally. */
int acpi_parse_madt(void *rsdp_virt, struct acpi_madt_info *out);

/* The Fixed ACPI Description Table (signature "FACP", not "FADT" -
 * the four-letter table name and the acronym for what it describes
 * have always disagreed). Only the one field either timer driver in
 * this kernel needs: where the ACPI Power Management Timer's counter
 * lives and how wide it is. */
struct acpi_fadt_info {
    uint32_t pm_tmr_blk; /* I/O port; 0 if the FADT has none */
    int tmr_val_ext;     /* 1 = 32-bit counter, 0 = 24-bit (wraps ~1.16s) */
};

/* Returns 0 and fills `out` when a FADT with a non-zero PM_TMR_BLK was
 * found; -1 otherwise (no RSDP, no FADT, or PM_TMR_BLK is 0 - some
 * machines genuinely have no ACPI PM timer). */
int acpi_parse_fadt(void *rsdp_virt, struct acpi_fadt_info *out);

/* The HPET table (signature "HPET") names nothing but where the
 * counter's MMIO register block is - the rest of what a driver needs
 * (the tick period, whether it is 64-bit capable) lives in the
 * hardware's own Capabilities register, read after mapping it. */
struct acpi_hpet_info {
    uint64_t phys_addr; /* 0 if no HPET table was found */
};

/* Returns 0 and fills `out` when an HPET table with a non-zero address
 * was found; -1 otherwise. */
int acpi_parse_hpet(void *rsdp_virt, struct acpi_hpet_info *out);

#endif /* TUS_ARCH_ACPI_H */
