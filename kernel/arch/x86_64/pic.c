/*
 * pic.c - 8259A PIC initialization and masking, with an upgrade path
 * to the Local APIC + I/O APIC
 *
 * The two 8259 chips are cascaded: the slave's INT line is wired to
 * IRQ2 of the master. During initialization all IRQs are masked;
 * drivers call pic_enable_irq() for exactly the lines they use.
 *
 * pic_init() ALWAYS brings the 8259 up first, unconditionally - that
 * sequence is the safe baseline every IRQ line falls back to. It then
 * tries, as a strict upgrade, to move routing on to the Local APIC
 * and I/O APIC (kernel/arch/x86_64/lapic.c, ioapic.c, acpi.c). If
 * anything along that chain is missing - CPUID says no APIC, Limine
 * found no RSDP, the MADT is absent or has no I/O APIC entry, the
 * MMIO mapping fails - g_apic_active stays false and every driver in
 * the kernel keeps calling pic_enable_irq()/pic_send_eoi() exactly as
 * it always has, none the wiser. This is what makes the upgrade safe
 * to attempt unconditionally: failure is silent and free.
 *
 * The public API's *names* (pic_enable_irq, pic_disable_irq,
 * pic_send_eoi) stay the interrupt-routing seam for the whole kernel
 * on purpose - every driver that unmasks or acknowledges an IRQ
 * (keyboard.c, mouse.c, serial.c, rtl8139.c, pit.c, sched.c) needed
 * zero changes to gain APIC routing.
 */

#include "pic.h"
#include "io.h"
#include "acpi.h"
#include "cpu.h"
#include "ioapic.h"
#include "lapic.h"
#include "core/bootinfo.h"

#include <stddef.h>

#define ICW1_INIT       0x11 /* cascade mode, edge triggered, ICW4 needed */
#define ICW4_8086       0x01 /* 8086/88 mode */

#define MASTER_BASE     0x20 /* master IRQ0..7  -> vectors 0x20..0x27 */
#define SLAVE_BASE      0x28 /* slave  IRQ8..15 -> vectors 0x28..0x2F */

#define SLAVE_ON_IRQ2   0x04 /* master: slave present on IRQ2 */
#define SLAVE_CASCADE_ID 0x02 /* slave:  cascade identity */

static uint8_t g_master_mask = 0xFF; /* 1 = masked */
static uint8_t g_slave_mask  = 0xFF;

static int g_apic_active;
static struct acpi_madt_info g_madt;

/* An ISA IRQ's Global System Interrupt, honouring the MADT's
 * Interrupt Source Overrides - the classic example being IRQ0, which
 * on a real PC chipset (and QEMU's emulation of one) is wired to I/O
 * APIC pin 2, not pin 0. Absent an override, GSI == IRQ number. */
static uint32_t isa_irq_to_gsi(uint8_t irq) {
    for (int i = 0; i < g_madt.n_override; i++) {
        if (g_madt.override[i].isa_irq == irq) {
            return g_madt.override[i].gsi;
        }
    }
    return irq;
}

int pic_using_apic(void) {
    return g_apic_active;
}

void pic_init(void) {
    /* ICW1: start initialization sequence on both chips. */
    outb(PIC1_CMD_PORT, ICW1_INIT);
    io_wait();
    outb(PIC2_CMD_PORT, ICW1_INIT);
    io_wait();

    /* ICW2: vector base for each chip. */
    outb(PIC1_DATA_PORT, MASTER_BASE);
    io_wait();
    outb(PIC2_DATA_PORT, SLAVE_BASE);
    io_wait();

    /* ICW3: cascade wiring. */
    outb(PIC1_DATA_PORT, SLAVE_ON_IRQ2);
    io_wait();
    outb(PIC2_DATA_PORT, SLAVE_CASCADE_ID);
    io_wait();

    /* ICW4: 8086 mode. */
    outb(PIC1_DATA_PORT, ICW4_8086);
    io_wait();
    outb(PIC2_DATA_PORT, ICW4_8086);
    io_wait();

    /* Mask everything; drivers unmask what they need. If the APIC
     * upgrade below succeeds, it stays this way forever - every IRQ
     * a driver enables from this point on goes through the I/O APIC
     * instead, and a fully-masked 8259 can never double-deliver
     * anything it also routes. */
    outb(PIC1_DATA_PORT, g_master_mask);
    outb(PIC2_DATA_PORT, g_slave_mask);

    /* The upgrade attempt. Every step fails closed: g_apic_active
     * only becomes true if all four succeed, and none of the earlier
     * ones touch any state pic_enable_irq()/pic_send_eoi() rely on if
     * a later one fails - the 8259 above is already fully configured
     * and simply keeps being what those functions use. */
    if (cpu_has_apic() &&
        acpi_parse_madt(g_bootinfo.rsdp, &g_madt) == 0 &&
        lapic_init(g_madt.has_lapic_override ? g_madt.lapic_phys : 0) == 0 &&
        ioapic_init(g_madt.ioapic_phys, g_madt.ioapic_gsi_base) == 0) {
        g_apic_active = 1;
    }
}

void pic_enable_irq(uint8_t irq) {
    if (g_apic_active) {
        if (irq == 2) {
            return; /* the 8259 cascade line: nothing to route in APIC mode */
        }
        ioapic_set_redirection(isa_irq_to_gsi(irq), (uint8_t)(MASTER_BASE + irq),
                               lapic_id(), 0 /* unmasked */);
        return;
    }
    if (irq < 8) {
        g_master_mask &= (uint8_t)~(1u << irq);
        outb(PIC1_DATA_PORT, g_master_mask);
    } else if (irq < 16) {
        g_slave_mask &= (uint8_t)~(1u << (irq - 8));
        outb(PIC2_DATA_PORT, g_slave_mask);
    }
}

void pic_disable_irq(uint8_t irq) {
    if (g_apic_active) {
        if (irq == 2) {
            return;
        }
        ioapic_set_redirection(isa_irq_to_gsi(irq), (uint8_t)(MASTER_BASE + irq),
                               lapic_id(), 1 /* masked */);
        return;
    }
    if (irq < 8) {
        g_master_mask |= (uint8_t)(1u << irq);
        outb(PIC1_DATA_PORT, g_master_mask);
    } else if (irq < 16) {
        g_slave_mask |= (uint8_t)(1u << (irq - 8));
        outb(PIC2_DATA_PORT, g_slave_mask);
    }
}

void pic_send_eoi(uint8_t irq) {
    if (g_apic_active) {
        (void)irq; /* the Local APIC's EOI register needs no IRQ number */
        lapic_send_eoi();
        return;
    }
    /* A slave IRQ needs an EOI on both chips. */
    if (irq >= 8) {
        outb(PIC2_CMD_PORT, 0x20);
    }
    outb(PIC1_CMD_PORT, 0x20);
}
