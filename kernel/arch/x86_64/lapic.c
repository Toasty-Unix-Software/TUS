#include "lapic.h"

#include "io.h"
#include "mm/vmm.h"
#include "drivers/pit/pit.h"

#include <stddef.h>

#define IA32_APIC_BASE_MSR 0x1B
#define APIC_BASE_ENABLE   (1ull << 11)
#define APIC_BASE_PHYS_MASK 0xFFFFF000ull

#define LAPIC_REG_ID       0x020
#define LAPIC_REG_TPR      0x080
#define LAPIC_REG_EOI      0x0B0
#define LAPIC_REG_SPURIOUS 0x0F0
#define LAPIC_REG_LVT_TIMER 0x320
#define LAPIC_REG_INITIAL_COUNT 0x380
#define LAPIC_REG_CURRENT_COUNT 0x390
#define LAPIC_REG_DIVIDE_CONFIG  0x3E0

#define LAPIC_SPURIOUS_VECTOR 0xFF /* idt.c's irq_ignored: no EOI needed */
#define LAPIC_SOFTWARE_ENABLE (1u << 8)

#define LVT_TIMER_MASKED (1u << 16)
#define LVT_TIMER_VECTOR_UNUSED 0xFF /* masked, so this vector never fires */

/* Divide Configuration Register: the value is a 3-bit code spread
 * across bits 3,1,0 (bit 2 is always 0). 0xB = 1011b = bits(3,1,0) =
 * (1,1,1) = divide-by-1, the maximum resolution for calibration. */
#define LAPIC_DIVIDE_BY_1 0xB

/* How long to let the timer free-run during calibration - long enough
 * that the PIT's own 10 ms tick granularity (see pit.c) does not
 * dominate the measurement's error, short enough that boot is not
 * visibly delayed. */
#define LAPIC_CALIBRATE_MS 20

static volatile uint32_t *g_lapic; /* NULL until lapic_init() succeeds */
static uint64_t g_timer_ticks_per_ms;

static inline uint32_t lapic_read(uint32_t reg) {
    return g_lapic[reg / 4];
}

static inline void lapic_write(uint32_t reg, uint32_t value) {
    g_lapic[reg / 4] = value;
}

int lapic_init(uint64_t phys_override) {
    uint64_t base_msr = rdmsr(IA32_APIC_BASE_MSR);
    uint64_t phys = phys_override != 0
        ? phys_override
        : (base_msr & APIC_BASE_PHYS_MASK);
    if (phys == 0) {
        return -1;
    }

    void *mapped = vmm_map_mmio(phys, 0x1000);
    if (mapped == NULL) {
        return -1;
    }
    g_lapic = (volatile uint32_t *)mapped;

    /* The global enable bit (MSR bit 11) should already be set on
     * every machine an OS has run on before, but this is a from
     * scratch kernel: it does not get to assume that, only ask for
     * it. Also (re)writes the base address, which matters only when
     * the MADT gave an override the MSR does not already point at. */
    wrmsr(IA32_APIC_BASE_MSR,
         (base_msr & ~APIC_BASE_PHYS_MASK) | (phys & APIC_BASE_PHYS_MASK) |
             APIC_BASE_ENABLE);

    lapic_write(LAPIC_REG_TPR, 0); /* accept every interrupt priority */
    lapic_write(LAPIC_REG_SPURIOUS,
               LAPIC_SPURIOUS_VECTOR | LAPIC_SOFTWARE_ENABLE);
    return 0;
}

void lapic_send_eoi(void) {
    if (g_lapic != NULL) {
        lapic_write(LAPIC_REG_EOI, 0);
    }
}

uint32_t lapic_id(void) {
    if (g_lapic == NULL) {
        return 0;
    }
    return lapic_read(LAPIC_REG_ID) >> 24;
}

int lapic_available(void) {
    return g_lapic != NULL;
}

void lapic_timer_calibrate(void) {
    if (g_lapic == NULL) {
        return;
    }

    lapic_write(LAPIC_REG_DIVIDE_CONFIG, LAPIC_DIVIDE_BY_1);
    /* Masked one-shot: bit 17 (timer mode) left 0 is one-shot, bit 16
     * (mask) set means this can never actually deliver an interrupt,
     * so no vector/ISR is needed to calibrate it safely. */
    lapic_write(LAPIC_REG_LVT_TIMER, LVT_TIMER_MASKED | LVT_TIMER_VECTOR_UNUSED);
    lapic_write(LAPIC_REG_INITIAL_COUNT, 0xFFFFFFFFu);

    timer_sleep_ms(LAPIC_CALIBRATE_MS);

    uint32_t remaining = lapic_read(LAPIC_REG_CURRENT_COUNT);
    lapic_write(LAPIC_REG_INITIAL_COUNT, 0); /* stop it - calibration is done */

    uint32_t elapsed = 0xFFFFFFFFu - remaining;
    g_timer_ticks_per_ms = elapsed / LAPIC_CALIBRATE_MS;
}

int lapic_timer_available(void) {
    return g_timer_ticks_per_ms != 0;
}

uint64_t lapic_timer_hz(void) {
    return g_timer_ticks_per_ms * 1000;
}
