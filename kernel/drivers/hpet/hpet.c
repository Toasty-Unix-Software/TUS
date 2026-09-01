/*
 * hpet.c - HPET main counter driver
 *
 * Three registers matter for a free-running counter, all in the same
 * memory-mapped block the ACPI HPET table points at:
 *   0x000  General Capabilities and ID Register (read-only)
 *          bits 63:32 = COUNTER_CLK_PERIOD, this counter's tick period
 *          in femtoseconds.
 *   0x010  General Configuration Register - bit 0 (ENABLE_CNF) must be
 *          set or the main counter never advances, whatever else here
 *          is configured.
 *   0x0F0  Main Counter Value Register (read-only while running).
 * No comparator is programmed - see hpet.h.
 */

#include "drivers/hpet/hpet.h"

#include "mm/vmm.h"

#include <stddef.h>

#define HPET_REG_CAPS       0x000
#define HPET_REG_CONFIG     0x010
#define HPET_REG_COUNTER    0x0F0

#define HPET_CONFIG_ENABLE_CNF (1ull << 0)

static volatile uint64_t *g_hpet; /* NULL until hpet_init() succeeds */
static uint32_t g_period_fs;

int hpet_init(uint64_t phys) {
    if (phys == 0) {
        return -1;
    }
    void *mapped = vmm_map_mmio(phys, 0x1000);
    if (mapped == NULL) {
        return -1;
    }
    g_hpet = (volatile uint64_t *)mapped;

    uint64_t caps = g_hpet[HPET_REG_CAPS / 8];
    g_period_fs = (uint32_t)(caps >> 32);
    if (g_period_fs == 0) {
        /* A period of 0 is meaningless (the spec forbids it) - treat
         * as "not really here" rather than divide by it later. */
        g_hpet = NULL;
        return -1;
    }

    g_hpet[HPET_REG_CONFIG / 8] |= HPET_CONFIG_ENABLE_CNF;
    return 0;
}

int hpet_available(void) {
    return g_hpet != NULL;
}

uint64_t hpet_read_counter(void) {
    if (g_hpet == NULL) {
        return 0;
    }
    return g_hpet[HPET_REG_COUNTER / 8];
}

uint32_t hpet_period_fs(void) {
    return g_period_fs;
}

uint64_t hpet_uptime_ms(void) {
    if (g_hpet == NULL) {
        return 0;
    }
    /* 1 ms = 10^12 fs. Dividing once, at init, into a ticks-per-ms
     * constant - rather than multiplying the (potentially very large,
     * after a long uptime) raw counter by period_fs on every call -
     * keeps this in plain 64-bit integer range indefinitely: even a
     * 100 MHz counter (the common QEMU value) only reaches 2^64 ticks
     * after roughly 5800 years, but ticks * period_fs would have
     * overflowed 64 bits after about 5 hours at that same rate. */
    static uint64_t ticks_per_ms;
    if (ticks_per_ms == 0) {
        ticks_per_ms = 1000000000000ull / g_period_fs;
        if (ticks_per_ms == 0) {
            ticks_per_ms = 1; /* an absurdly slow counter: avoid div-by-0 */
        }
    }
    return hpet_read_counter() / ticks_per_ms;
}
