/*
 * lapic.h - the Local APIC (per-CPU interrupt controller)
 *
 * TUS runs single-core (see sched.h), so this brings up only the
 * BSP's Local APIC: enabling it, sending its end-of-interrupt, and
 * reading its id (the destination field every I/O APIC redirection
 * entry needs).
 */

#ifndef TUS_ARCH_LAPIC_H
#define TUS_ARCH_LAPIC_H

#include <stdint.h>

/* Map and enable the Local APIC. `phys_override` is a physical
 * address to use instead of the one in the IA32_APIC_BASE MSR (the
 * MADT's optional type-5 entry); pass 0 to use the MSR, which is
 * correct on every machine that does not have that override.
 * Returns 0 on success, -1 if the MMIO mapping failed. */
int lapic_init(uint64_t phys_override);

/* Acknowledge the interrupt currently being serviced. A no-op if
 * lapic_init() was never called or failed. */
void lapic_send_eoi(void);

/* This CPU's Local APIC id, 0 if the LAPIC was never mapped. Every
 * I/O APIC redirection entry names one as its destination. */
uint32_t lapic_id(void);

/* True once lapic_init() has succeeded. */
int lapic_available(void);

/* Calibrate the Local APIC's own onboard timer (the LVT Timer/Initial
 * Count/Current Count/Divide Config registers - a different piece of
 * hardware from the interrupt-routing role lapic_init() already sets
 * up) against the PIT. A no-op if lapic_available() is false.
 *
 * MUST be called after sti() and after pit_init()/pic_init(): it uses
 * timer_sleep_ms(), which halts waiting for the PIT's IRQ0 to wake it
 * - called with interrupts still masked, this would hang the machine
 * on its first wait instead of measuring one (the same rule xhci_init()
 * follows in main.c, for the same reason). Leaves the LAPIC timer
 * masked and stopped afterward - this only measures its frequency, it
 * does not start it running or wire an interrupt handler to it. */
void lapic_timer_calibrate(void);

/* True once lapic_timer_calibrate() has measured a non-zero frequency. */
int lapic_timer_available(void);

/* The calibrated frequency in Hz (ticks per second at divide-by-1), or
 * 0 if lapic_timer_available() is false. */
uint64_t lapic_timer_hz(void);

#endif /* TUS_ARCH_LAPIC_H */
