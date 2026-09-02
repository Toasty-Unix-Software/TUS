/*
 * smp.h - CPU topology (from ACPI/MADT) and spinlocks
 *
 * TUS still schedules and runs every task on the boot CPU only - there
 * is no AP (Application Processor) trampoline here, so a multi-core
 * machine boots with its other cores left halted, exactly as before.
 * What this file adds is real: the kernel now knows how many usable
 * CPUs the firmware reports and their APIC ids (smp_init(), the
 * "cpuinfo" shell command), and has a correct spinlock primitive
 * (spin_lock/spin_unlock/spin_trylock) available for the day a second
 * core actually starts executing code. Until then, on this single
 * running core, a spinlock behaves like a very cheap reentrancy
 * guard - it still catches a bug where the same lock is acquired
 * twice without unlocking in between (it would spin forever, which is
 * the correct, honest behavior for that bug rather than silently
 * "working" single-threaded).
 */

#ifndef TUS_ARCH_SMP_H
#define TUS_ARCH_SMP_H

#include <stdint.h>

#define SMP_MAX_CPUS 32

struct cpu_state {
    uint8_t apic_id;
    uint8_t acpi_processor_id;
    int enabled;
    int is_bsp; /* boot strap processor - the one core actually running */
};

/* Parse the MADT (via acpi_parse_madt()) and populate the per-CPU
 * table. Always succeeds in the sense that it leaves at least the BSP
 * entry populated (apic_id from lapic_id(), is_bsp=1) even when no
 * usable MADT is found - a uniprocessor machine is a valid topology,
 * not an error. Call after lapic_init(). */
void smp_init(void);

/* Number of CPU entries the MADT reported (enabled or not). At least 1. */
int smp_cpu_count(void);

/* Number of those entries with the "enabled" flag set. At least 1
 * (the BSP, since it is by definition running this code). */
int smp_cpu_enabled_count(void);

/* Read-only access to entry `idx` (0 <= idx < smp_cpu_count()).
 * Returns NULL if out of range. */
const struct cpu_state *smp_cpu_get(int idx);

/*
 * A basic test-and-set spinlock with a PAUSE-based busy wait. Correct
 * for actual multi-core use (atomic __sync_lock_test_and_set on the
 * lock word, acquire/release semantics) even though nothing in this
 * kernel currently contends it across cores.
 */
typedef struct {
    volatile int locked;
} spinlock_t;

#define SPINLOCK_INIT { 0 }

void spin_lock(spinlock_t *lock);
void spin_unlock(spinlock_t *lock);
/* Returns 1 if the lock was acquired, 0 if it was already held. */
int spin_trylock(spinlock_t *lock);

#endif /* TUS_ARCH_SMP_H */
