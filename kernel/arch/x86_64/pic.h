/*
 * pic.h - Programmable Interrupt Controller (8259A)
 *
 * Two cascaded 8259A chips deliver the 15 hardware interrupts (IRQs)
 * to the CPU. By default their vectors collide with CPU exceptions
 * (IRQ0 = vector 8 = Double Fault), so we remap them above vector 31.
 */

#ifndef TUS_ARCH_PIC_H
#define TUS_ARCH_PIC_H

#include <stdint.h>

/* I/O ports of the two PIC chips. */
#define PIC1_CMD_PORT  0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_CMD_PORT  0xA0
#define PIC2_DATA_PORT 0xA1

/* OCW3 command: read the In-Service Register (for spurious IRQ check). */
#define PIC_READ_ISR 0x0B

/* Remap the PICs (master -> vectors 0x20.., slave -> 0x28..) and mask
 * every IRQ line. Call before enabling any individual IRQ. */
void pic_init(void);

/* Unmask / mask a single IRQ line (0..15). */
void pic_enable_irq(uint8_t irq);
void pic_disable_irq(uint8_t irq);

/* Acknowledge an interrupt. Must be called when an IRQ handler done. */
void pic_send_eoi(uint8_t irq);

/* True when pic_init() moved interrupt routing on to the Local APIC
 * and I/O APIC (see acpi.c/lapic.c/ioapic.c) instead of the 8259 -
 * the ISA IRQ numbers and vectors (0x20 + irq) stay the same either
 * way, only the piece of hardware doing the delivery changes. The
 * IRQ7/IRQ15 spurious-interrupt check in idt.c's irq_dispatch() reads
 * 8259-specific registers and must be skipped when this is true (the
 * I/O APIC has no equivalent quirk; the Local APIC's own spurious
 * vector, handled separately, does). */
int pic_using_apic(void);

#endif /* TUS_ARCH_PIC_H */
