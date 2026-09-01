/*
 * ec.h - the ACPI Embedded Controller byte protocol
 *
 * Fan control has no equivalent of VBE or xHCI: there is no single
 * register interface every machine implements. What IS standard
 * (ACPI 5.0 section 12) is the two I/O ports every EC-equipped
 * machine exposes and the four-step handshake to read or write one of
 * its byte registers through them - real hardware wires fan duty,
 * temperature and battery status behind THIS, but which register
 * means what is defined per-machine in its DSDT's AML, which TUS has
 * no interpreter for. This driver gets a caller as far as "read/write
 * EC register N" and no further: genuine, real infrastructure, honest
 * about the part that needs a datasheet or a working `acpi_ec` on the
 * same machine to go with it.
 *
 * QEMU implements no EC at all - these ports read back 0xFF, so
 * ec_probe() reports "not found" rather than a caller discovering
 * that the slow way by waiting forever on a status bit that is never
 * going to change.
 */

#ifndef TUS_DRIVERS_EC_H
#define TUS_DRIVERS_EC_H

#include <stdint.h>

/* Poll for a controller that actually answers. 0 if one does, -1 if
 * every attempt timed out (no EC, or a virtual machine that does not
 * emulate one). Safe to call more than once; cheap either way. */
int ec_probe(void);

/* Read/write one EC byte register. 0 on success, -1 on timeout (the
 * handshake never completed - almost always because there is no EC).
 * `reg` and register meanings are machine-specific: see ec.h's own
 * comment above. */
int ec_read(uint8_t reg, uint8_t *out);
int ec_write(uint8_t reg, uint8_t value);

#endif /* TUS_DRIVERS_EC_H */
