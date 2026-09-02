/*
 * linux_syscall.h - Linux x86_64 SYSCALL (0f 05) compatibility
 *
 * See linux_syscall.c for the design and its honest scope limits.
 */

#ifndef TUS_SYSCALL_LINUX_SYSCALL_H
#define TUS_SYSCALL_LINUX_SYSCALL_H

/* Program the SYSCALL/SYSRET MSRs (EFER.SCE, STAR, LSTAR, SFMASK) so
 * the `syscall` instruction (0f 05) traps into linux_syscall_entry()
 * instead of raising #UD. Call once at boot, after gdt_init() (it
 * depends on the SYSRET-only selectors gdt_init() installs) and
 * before any task can reach ring 3. */
void linux_syscall_init(void);

#endif /* TUS_SYSCALL_LINUX_SYSCALL_H */
