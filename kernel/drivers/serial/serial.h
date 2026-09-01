/*
 * serial.h - 16550 UART driver
 *
 * The serial port is the kernel's most trustworthy debug channel: it
 * works from the very first instruction and does not depend on any
 * display hardware. TUS uses COM1 at 115200 baud, 8 data bits, no
 * parity, 1 stop bit (8N1).
 *
 * It is also a mirror of everything the console prints, and 115200
 * baud is 11 KiB per second - slower than a program can write. The
 * driver therefore starts out writing straight to the port (so a
 * kernel that dies during boot still says why) and switches to a
 * queue drained by the UART's transmit interrupt once the system is
 * running, so a console write costs a memory store rather than the
 * time it takes to shift the bits out of the port.
 */

#ifndef TUS_DRIVERS_SERIAL_H
#define TUS_DRIVERS_SERIAL_H

#include <stdbool.h>
#include <stddef.h>

/* Initialize COM1. Returns true on success. */
bool serial_init(void);

/* Transmit a single character. Synchronous until serial_start_async();
 * after that it queues, and only waits when the queue is full. */
void serial_putchar(char c);

/* Transmit a NUL-terminated string. */
void serial_write(const char *s);

/* Transmit `n` bytes. In buffered mode this is what makes the mirror
 * cheap: the queue is guarded by the interrupt flag, and taking it
 * once for a whole write beats taking it once per character. */
void serial_write_n(const char *s, unsigned long n);

/* Hand the port over to its transmit interrupt (IRQ4). Call once
 * interrupts are running. */
void serial_start_async(void);

/* Push everything queued out of the port and go back to writing
 * directly. The panic path does this: a log that arrives after the
 * machine has stopped is not a log. */
void serial_sync(void);

#endif /* TUS_DRIVERS_SERIAL_H */
