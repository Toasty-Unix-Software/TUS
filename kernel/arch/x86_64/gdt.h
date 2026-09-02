/*
 * gdt.h - Global Descriptor Table and Task State Segment
 *
 * Selectors (see gdt.c for the full layout):
 *   0x08 kernel code, 0x10 kernel data, 0x18 user code,
 *   0x20 user data, 0x28 TSS, 0x40/0x48 SYSRET-only user data/code
 *   (see the Linux-syscall comment in gdt.c - same flat ring-3
 *   segment as 0x18/0x20, just placed where SYSRETQ's rigid
 *   STAR-relative layout requires it).
 */

#ifndef TUS_ARCH_GDT_H
#define TUS_ARCH_GDT_H

#include <stdint.h>

/* Install the TUS GDT (kernel + user segments + TSS) and load the
 * task register. Call before enabling interrupts. */
void gdt_init(void);

/* Point TSS.RSP0 at a kernel stack; call on every task switch so
 * interrupts from user mode land on the current task's stack. */
void tss_set_rsp0(uint64_t rsp0);

/* The base value SYSRETQ derives the return CS/SS from (IA32_STAR
 * bits 63:48): CS = (base+16)|3, SS = (base+8)|3. Exposed so
 * linux_syscall.c can program IA32_STAR without hardcoding gdt.c's
 * internal layout. */
#define SEL_KERNEL_CODE      0x08
#define SEL_SYSRET_USER_BASE 0x38

#endif /* TUS_ARCH_GDT_H */
