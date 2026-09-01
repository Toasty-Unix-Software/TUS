/*
 * hpet.h - HPET (High Precision Event Timer) as a free-running counter
 *
 * TUS only reads the main counter here - no comparators/one-shot or
 * periodic interrupts are programmed, so this never fires an IRQ.
 * That is enough to make it a usable, higher-resolution replacement
 * for what pit_uptime_ms() reports, without touching anything that
 * drives interrupt delivery (see pit.c/lapic.c for those).
 */

#ifndef TUS_DRIVERS_HPET_H
#define TUS_DRIVERS_HPET_H

#include <stdint.h>

/* Map the HPET's MMIO register block at `phys` (from the ACPI HPET
 * table, see acpi_parse_hpet()) and enable its main counter. Returns
 * 0 on success, -1 if the mapping failed. */
int hpet_init(uint64_t phys);

/* True once hpet_init() has succeeded. */
int hpet_available(void);

/* The raw, free-running main counter. Meaningless without the period
 * below; most callers want hpet_uptime_ms() instead. */
uint64_t hpet_read_counter(void);

/* One tick's period in femtoseconds, straight from the Capabilities
 * register - typically 10,000,000 (10 ns, a 100 MHz counter) on QEMU,
 * but never assumed rather than read. 0 if hpet_init() never
 * succeeded. */
uint32_t hpet_period_fs(void);

/* Milliseconds since hpet_init() enabled the counter. 0 if HPET is not
 * available - callers must check hpet_available() first if 0 needs to
 * be distinguished from "really has been 0 ms". */
uint64_t hpet_uptime_ms(void);

#endif /* TUS_DRIVERS_HPET_H */
