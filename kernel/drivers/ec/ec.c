/*
 * ec.c - the ACPI Embedded Controller byte protocol (ACPI 5.0 12.3)
 *
 * Two ports, EC_DATA and EC_SC, and a status byte with two bits that
 * matter: IBF (the EC has not yet consumed what was last written to
 * it - wait before writing again) and OBF (the EC has a byte ready -
 * wait before reading). A read is command, address, then a read; a
 * write is command, address, then a write. Every step waits on IBF
 * clearing except the final read of a read, which waits on OBF
 * setting instead.
 */

#include "drivers/ec/ec.h"

#include <stddef.h>

#include "arch/x86_64/io.h"

#define EC_DATA_PORT 0x62
#define EC_SC_PORT   0x66

#define EC_SC_OBF (1u << 0)
#define EC_SC_IBF (1u << 1)

#define EC_CMD_READ  0x80
#define EC_CMD_WRITE 0x81

/* Generous, but bounded: real ECs answer within microseconds. A
 * machine with none of them (every QEMU machine) reads 0xFF from
 * EC_SC forever, so IBF never clears and this is what turns that into
 * "not found" in a few hundred microseconds instead of a hang. */
#define EC_POLL_TRIES 20000

static int wait_ibf_clear(void) {
    for (int i = 0; i < EC_POLL_TRIES; i++) {
        if (!(inb(EC_SC_PORT) & EC_SC_IBF)) {
            return 0;
        }
    }
    return -1;
}

static int wait_obf_set(void) {
    for (int i = 0; i < EC_POLL_TRIES; i++) {
        if (inb(EC_SC_PORT) & EC_SC_OBF) {
            return 0;
        }
    }
    return -1;
}

int ec_read(uint8_t reg, uint8_t *out) {
    if (wait_ibf_clear() != 0) {
        return -1;
    }
    outb(EC_SC_PORT, EC_CMD_READ);
    if (wait_ibf_clear() != 0) {
        return -1;
    }
    outb(EC_DATA_PORT, reg);
    if (wait_obf_set() != 0) {
        return -1;
    }
    if (out != NULL) {
        *out = inb(EC_DATA_PORT);
    } else {
        (void)inb(EC_DATA_PORT); /* still has to be drained */
    }
    return 0;
}

int ec_write(uint8_t reg, uint8_t value) {
    if (wait_ibf_clear() != 0) {
        return -1;
    }
    outb(EC_SC_PORT, EC_CMD_WRITE);
    if (wait_ibf_clear() != 0) {
        return -1;
    }
    outb(EC_DATA_PORT, reg);
    if (wait_ibf_clear() != 0) {
        return -1;
    }
    outb(EC_DATA_PORT, value);
    return wait_ibf_clear();
}

int ec_probe(void) {
    /* Register 0 is always readable on a real EC - it does not matter
     * what it means on this particular machine, only that the
     * handshake completes at all. */
    uint8_t discard;
    return ec_read(0, &discard);
}
