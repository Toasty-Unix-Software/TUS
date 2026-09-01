/*
 * pit.c - PIT driver implementation
 *
 * Channel 0 is configured in mode 2 (rate generator), not mode 3
 * (square wave) - see pit_init() for why the difference actually
 * matters here, not just cosmetically. The 1.19318 MHz input clock
 * divided by 11932 gives 100.0008 Hz - close enough for a monotonic
 * tick counter and coarse sleeps.
 */

#include "drivers/pit/pit.h"

#include "drivers/hpet/hpet.h"
#include "../../arch/x86_64/idt.h"
#include "../../arch/x86_64/io.h"
#include "../../arch/x86_64/pic.h"

#define PIT_CH0_DATA  0x40
#define PIT_CMD       0x43

#define PIT_IRQ       0

#define DIVISOR_100HZ 11932

static volatile uint64_t g_ticks;

/* Advance the tick counter; called from the scheduler's IRQ0 path. */
void pit_tick(void) {
    g_ticks++;
}

void pit_init(void) {
    /* Command: channel 0, lobyte/hibyte access, mode 2, binary.
     *
     * Mode 3 (square wave, command 0x36) toggles OUT high-then-low
     * for one half of the count each - a clean rising AND a clean
     * falling edge every period, both at ISA-legacy voltage levels.
     * Routed through the 8259 that is harmless (the 8259's own edge
     * latch only ever asserts once per period in practice), but
     * routed through the I/O APIC's plain edge-triggered pin 2 with
     * no such filtering, BOTH edges of an even-divisor square wave
     * register as a delivery - the scheduler tick fires twice per
     * real PIT period, uptime and every ms-based timer runs exactly
     * 2x fast, and this measures as *exactly* 2.00 (not a jittery
     * race) because it is deterministic, not a timing accident.
     * Confirmed by forcing g_apic_active off (legacy 8259 routing):
     * the same 0x36/mode-3 programming reports uptime at the correct
     * 1.00x real-time ratio there, isolating the doubling to mode 3
     * plus I/O APIC delivery specifically, not the divisor or the
     * tick-counting code.
     *
     * Mode 2 (rate generator, command 0x34) produces one narrow LOW
     * pulse at terminal count and stays high the rest of the period -
     * a single edge per period under either routing path. This is
     * also what real BIOS/OS timer code almost always uses PIT mode 2
     * for, for exactly this reason. */
    outb(PIT_CMD, 0x34);
    outb(PIT_CH0_DATA, (uint8_t)(DIVISOR_100HZ & 0xFF));
    outb(PIT_CH0_DATA, (uint8_t)((DIVISOR_100HZ >> 8) & 0xFF));

    /* IRQ0 is wired to the scheduler (sched_tick_entry) in idt.c;
     * here we only unmask the interrupt. */
    pic_enable_irq(PIT_IRQ);
}

uint64_t pit_ticks(void) {
    return g_ticks;
}

uint64_t pit_uptime_ms(void) {
    /* HPET's counter (hpet.c) is read directly, at whatever real
     * resolution its period gives - no calibration, no 10ms-per-tick
     * quantization. Every existing caller of this function (SYS_UPTIME,
     * timer_sleep_ms's own target math is a separate, PIT-tick-based
     * path below and unaffected) gets the better number for free, with
     * no signature change - exactly the "API stays the same" this
     * function's header comment already anticipated. Falls back to the
     * PIT tick count, unchanged, on any machine without a working
     * HPET. */
    if (hpet_available()) {
        return hpet_uptime_ms();
    }
    return g_ticks * 10;
}

void timer_sleep_ms(uint32_t ms) {
    uint64_t target = g_ticks + (uint64_t)(ms + 9) / 10;
    while (g_ticks < target) {
        hlt(); /* IRQ0 wakes us */
    }
}
