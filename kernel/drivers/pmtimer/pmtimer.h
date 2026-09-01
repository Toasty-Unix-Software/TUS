/*
 * pmtimer.h - the ACPI Power Management Timer
 *
 * A single free-running counter at a fixed 3.579545 MHz (the old NTSC
 * colorburst frequency - chosen historically because it was already a
 * crystal every PC chipset had lying around), readable with one port
 * I/O instruction. No enable step exists to get wrong: unlike HPET,
 * the PM timer runs whenever ACPI hardware is present at all.
 */

#ifndef TUS_DRIVERS_PMTIMER_H
#define TUS_DRIVERS_PMTIMER_H

#include <stdint.h>

#define ACPI_PM_TIMER_HZ 3579545u

/* `port` is the FADT's PM_TMR_BLK; `is_32bit` is its TMR_VAL_EXT flag
 * (see acpi_parse_fadt()). Returns 0 on success, -1 if `port` is 0 -
 * some machines genuinely have no ACPI PM timer. */
int pmtimer_init(uint32_t port, int is_32bit);

/* True once pmtimer_init() has succeeded. */
int pmtimer_available(void);

/* The raw counter, already masked to 24 bits if this timer is not the
 * 32-bit variant - a caller comparing two reads to measure elapsed
 * ticks must account for wraparound either way (24-bit wraps roughly
 * every 1.16 s, 32-bit roughly every 20 minutes). */
uint32_t pmtimer_read(void);

#endif /* TUS_DRIVERS_PMTIMER_H */
