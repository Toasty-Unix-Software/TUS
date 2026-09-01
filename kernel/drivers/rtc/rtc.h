/*
 * rtc.h - the CMOS real-time clock (MC146818)
 *
 * TUS needs a wall clock for exactly one reason at first: a git commit
 * records when it was made, and "seconds since boot" is not an answer.
 * The RTC is read once at boot and the PIT carries the time forward
 * from there, so nothing has to poll the CMOS in a loop.
 */

#ifndef TUS_DRIVERS_RTC_H
#define TUS_DRIVERS_RTC_H

#include <stdbool.h>
#include <stdint.h>

/* Read the CMOS clock and anchor the wall clock to it. */
void rtc_init(void);

/* Seconds since 1970-01-01 UTC. */
uint64_t rtc_now(void);

/* The boot time, in the same units; rtc_now() is this plus uptime. */
uint64_t rtc_boot_epoch(void);

/* The inverse of the conversion rtc_init() does: seconds since the
 * epoch back to a civil (Gregorian) date and time of day, UTC. Used
 * by the `date` shell command - nothing else needs it, which is why
 * it lives here next to days_from_civil() rather than in klib. */
void rtc_civil_from_epoch(uint64_t epoch, int *year, int *month, int *day,
                          int *hour, int *minute, int *second);

#endif /* TUS_DRIVERS_RTC_H */
