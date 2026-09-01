/*
 * spectre.h - Spectre v1 (bounds-check bypass) and v2 (branch target
 * injection) mitigations.
 */

#ifndef TUS_ARCH_SPECTRE_H
#define TUS_ARCH_SPECTRE_H

enum spectre_v1_mitigation {
    SPECTRE_V1_MITIGATION_NONE = 0,
    SPECTRE_V1_MITIGATION_FENCE,
};

enum spectre_v2_mitigation {
    SPECTRE_V2_MITIGATION_NONE = 0,
    SPECTRE_V2_MITIGATION_RETPOLINE,
};

extern enum spectre_v1_mitigation spectre_v1_mitigation;
extern enum spectre_v2_mitigation spectre_v2_mitigation;

/* Detects CPU support and turns on whatever mitigations apply. Must
 * run after cpu_get_vendor()/cpuid are usable (any time before user
 * tasks start) and before print_boot_banner(). */
void spectre_init(void);

/* array_index_nospec(): call after a bounds check and before using an
 * attacker/user-controlled index to read memory, so a mispredicted
 * branch cannot speculatively read past the bound before the LFENCE
 * retires. A no-op if SPECTRE_V1_MITIGATION_NONE (never true on any
 * CPU TUS boots on - LFENCE is part of the baseline x86-64 ISA - kept
 * for completeness/testability). */
static inline void spectre_v1_barrier(void) {
    __asm__ volatile("lfence" ::: "memory");
}

#endif /* TUS_ARCH_SPECTRE_H */
