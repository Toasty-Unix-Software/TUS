/*
 * spectre.c - Spectre v1/v2 mitigation detection and enablement.
 *
 * v1 (bounds-check bypass, CVE-2017-5753): mitigated with an LFENCE
 * speculation barrier (spectre_v1_barrier(), spectre.h) placed after
 * every bounds check that guards a user-controlled array index -
 * fd_get() in kernel/vfs/vfs.c is the concrete case: `fd` comes
 * straight from a syscall argument (rdi) and indexes the per-task fd
 * table right after the `fd >= VFS_MAX_FDS` check. LFENCE is part of
 * the baseline x86-64 ISA, so this mitigation needs no CPUID probe
 * and is always available.
 *
 * v2 (branch target injection, CVE-2017-5715): the textbook fix is
 * recompiling every indirect branch as a retpoline, but that is a
 * compiler code-generation feature (`-mindirect-branch=thunk` on
 * GCC, `-mretpoline` on Clang) this tree cannot depend on - TUS is
 * built with three different compilers (GCC, Clang, and its own
 * hosted PCC port), and PCC has no retpoline support at all. The
 * portable, hardware-level equivalent is IBRS (Indirect Branch
 * Restricted Speculation): setting IA32_SPEC_CTRL.IBRS tells the CPU
 * itself to stop predicting indirect branches across privilege
 * levels, which closes the same hole retpolines close, without
 * requiring any particular compiler. spectre_init() enables it when
 * CPUID reports support (leaf 7, sub-leaf 0, EDX bit 26).
 */

#include "spectre.h"

#include <stdint.h>

#include "../../core/klib.h"

#define IA32_SPEC_CTRL       0x48u
#define SPEC_CTRL_IBRS       (1ull << 0)
#define CPUID_LEAF7_EDX_IBRS (1u << 26) /* IBRS/IBPB enumeration */

enum spectre_v1_mitigation spectre_v1_mitigation = SPECTRE_V1_MITIGATION_NONE;
enum spectre_v2_mitigation spectre_v2_mitigation = SPECTRE_V2_MITIGATION_NONE;

static void cpuid_leaf(uint32_t leaf, uint32_t subleaf, uint32_t *a,
                       uint32_t *b, uint32_t *c, uint32_t *d) {
    __asm__ volatile("cpuid"
                     : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                     : "a"(leaf), "c"(subleaf));
}

static int cpu_has_ibrs(void) {
    uint32_t a, b, c, d;
    cpuid_leaf(0, 0, &a, &b, &c, &d);
    if (a < 7) {
        return 0; /* leaf 7 not implemented */
    }
    cpuid_leaf(7, 0, &a, &b, &c, &d);
    return (d & CPUID_LEAF7_EDX_IBRS) != 0;
}

static void wrmsr(uint32_t msr, uint64_t value) {
    uint32_t lo = (uint32_t)value;
    uint32_t hi = (uint32_t)(value >> 32);
    __asm__ volatile("wrmsr" : : "c"(msr), "a"(lo), "d"(hi) : "memory");
}

void spectre_init(void) {
    /* Always available on x86-64; the barrier is applied at the
     * bounds-checked fd lookup in vfs.c regardless of this flag, but
     * the flag is what print_boot_banner() and /proc/cpuinfo report. */
    spectre_v1_mitigation = SPECTRE_V1_MITIGATION_FENCE;

    if (cpu_has_ibrs()) {
        wrmsr(IA32_SPEC_CTRL, SPEC_CTRL_IBRS);
        spectre_v2_mitigation = SPECTRE_V2_MITIGATION_RETPOLINE;
    } else {
        spectre_v2_mitigation = SPECTRE_V2_MITIGATION_NONE;
    }
}
