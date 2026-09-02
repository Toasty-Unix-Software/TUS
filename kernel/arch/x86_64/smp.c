#include "smp.h"

#include "acpi.h"
#include "lapic.h"
#include "core/bootinfo.h"

#include <stddef.h>

static struct cpu_state g_cpus[SMP_MAX_CPUS];
static int g_n_cpus = 0;

void smp_init(void) {
    struct acpi_madt_info madt;
    int have_madt = (acpi_parse_madt(g_bootinfo.rsdp, &madt) == 0 ||
                      madt.n_cpu > 0);
    /* acpi_parse_madt() returns -1 when there is no I/O APIC entry even
     * if it did find processor entries first (it still fills `out`) -
     * re-check n_cpu directly rather than trusting the return value,
     * since a CPU list is independent of whether an I/O APIC exists. */

    uint32_t bsp_apic_id = lapic_available() ? lapic_id() : 0;

    g_n_cpus = 0;
    if (have_madt && madt.n_cpu > 0) {
        for (int i = 0; i < madt.n_cpu && g_n_cpus < SMP_MAX_CPUS; i++) {
            g_cpus[g_n_cpus].apic_id = madt.cpu[i].apic_id;
            g_cpus[g_n_cpus].acpi_processor_id = madt.cpu[i].acpi_processor_id;
            g_cpus[g_n_cpus].enabled = madt.cpu[i].enabled;
            g_cpus[g_n_cpus].is_bsp =
                (madt.cpu[i].apic_id == (uint8_t)bsp_apic_id);
            g_n_cpus++;
        }
    }

    if (g_n_cpus == 0) {
        /* No MADT / no processor entries: fall back to a one-entry
         * uniprocessor topology describing the core we are running on. */
        g_cpus[0].apic_id = (uint8_t)bsp_apic_id;
        g_cpus[0].acpi_processor_id = 0;
        g_cpus[0].enabled = 1;
        g_cpus[0].is_bsp = 1;
        g_n_cpus = 1;
    }
}

int smp_cpu_count(void) {
    return g_n_cpus > 0 ? g_n_cpus : 1;
}

int smp_cpu_enabled_count(void) {
    int n = 0;
    for (int i = 0; i < g_n_cpus; i++) {
        if (g_cpus[i].enabled) {
            n++;
        }
    }
    return n > 0 ? n : 1;
}

const struct cpu_state *smp_cpu_get(int idx) {
    if (idx < 0 || idx >= g_n_cpus) {
        return NULL;
    }
    return &g_cpus[idx];
}

void spin_lock(spinlock_t *lock) {
    while (__sync_lock_test_and_set(&lock->locked, 1)) {
        while (lock->locked) {
            __asm__ volatile("pause");
        }
    }
}

void spin_unlock(spinlock_t *lock) {
    __sync_lock_release(&lock->locked);
}

int spin_trylock(spinlock_t *lock) {
    return __sync_lock_test_and_set(&lock->locked, 1) == 0;
}
