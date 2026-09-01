/*
 * ioapic.h - the I/O APIC (routes device interrupt lines to a Local
 * APIC's vector, replacing the 8259's fixed IRQ-to-vector wiring)
 */

#ifndef TUS_ARCH_IOAPIC_H
#define TUS_ARCH_IOAPIC_H

#include <stdint.h>

/* Map the I/O APIC at `phys_base` (from the MADT's type-1 entry) and
 * remember its GSI base (almost always 0). Returns 0 on success, -1
 * if the MMIO mapping failed. */
int ioapic_init(uint32_t phys_base, uint32_t gsi_base);

/* Point Global System Interrupt `gsi` at `vector` on the Local APIC
 * named by `dest_lapic_id`, fixed delivery mode, active-high,
 * edge-triggered (the ISA default, and what every IRQ line this
 * kernel uses expects). `masked` starts the line disabled - matching
 * pic_disable_irq()'s sense, 1 = masked, 0 = delivering. A no-op if
 * ioapic_init() was never called or failed. */
void ioapic_set_redirection(uint32_t gsi, uint8_t vector,
                            uint32_t dest_lapic_id, int masked);

/* True once ioapic_init() has succeeded. */
int ioapic_available(void);

#endif /* TUS_ARCH_IOAPIC_H */
