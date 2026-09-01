/*
 * ioapic.c - I/O APIC register access
 *
 * Indirect, like the VBE DISPI registers: one 32-bit index register
 * (IOREGSEL) selects which internal register the next access to the
 * 32-bit data window (IOWIN) reads or writes. The registers that
 * matter here are the 24 redirection table entries (IOREDTBL0..23),
 * each 64 bits wide and so spread across two consecutive index
 * values.
 */

#include "ioapic.h"

#include "mm/vmm.h"

#include <stddef.h>

#define IOAPIC_IOREGSEL 0x00
#define IOAPIC_IOWIN    0x10

#define IOAPIC_REDTBL_BASE 0x10

#define REDTBL_MASKED (1u << 16)

static volatile uint32_t *g_ioapic; /* NULL until ioapic_init() succeeds */
static uint32_t g_gsi_base;

static void ioapic_write(uint32_t reg, uint32_t value) {
    g_ioapic[IOAPIC_IOREGSEL / 4] = reg;
    g_ioapic[IOAPIC_IOWIN / 4] = value;
}

int ioapic_init(uint32_t phys_base, uint32_t gsi_base) {
    void *mapped = vmm_map_mmio(phys_base, 0x1000);
    if (mapped == NULL) {
        return -1;
    }
    g_ioapic = (volatile uint32_t *)mapped;
    g_gsi_base = gsi_base;
    return 0;
}

void ioapic_set_redirection(uint32_t gsi, uint8_t vector,
                            uint32_t dest_lapic_id, int masked) {
    if (g_ioapic == NULL || gsi < g_gsi_base) {
        return;
    }
    uint32_t index = gsi - g_gsi_base;
    uint32_t reg = IOAPIC_REDTBL_BASE + index * 2;

    uint32_t low = vector; /* delivery mode 0 (fixed), dest mode 0 (physical),
                            * active-high, edge-triggered: all zero bits */
    if (masked) {
        low |= REDTBL_MASKED;
    }
    uint32_t high = dest_lapic_id << 24;

    /* The high dword (destination) first, low dword (vector + mask)
     * second: writing the low word last is what actually arms the
     * entry, so the destination is never briefly stale for a request
     * that could fire between the two writes. */
    ioapic_write(reg + 1, high);
    ioapic_write(reg, low);
}

int ioapic_available(void) {
    return g_ioapic != NULL;
}
