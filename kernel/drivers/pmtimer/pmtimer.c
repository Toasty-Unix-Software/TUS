/*
 * pmtimer.c - ACPI Power Management Timer driver
 */

#include "drivers/pmtimer/pmtimer.h"

#include "../../arch/x86_64/io.h"

static uint32_t g_port;
static int g_is_32bit;

int pmtimer_init(uint32_t port, int is_32bit) {
    if (port == 0) {
        return -1;
    }
    g_port = port;
    g_is_32bit = is_32bit;
    return 0;
}

int pmtimer_available(void) {
    return g_port != 0;
}

uint32_t pmtimer_read(void) {
    if (g_port == 0) {
        return 0;
    }
    uint32_t v = inl((uint16_t)g_port);
    return g_is_32bit ? v : (v & 0x00FFFFFFu);
}
