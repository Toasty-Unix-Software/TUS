/* panic_screen.h - full-screen colour panic displays (BSOD/GSOD/RSOD)
 *
 * These sit on top of the existing register-dump panic path in
 * kernel/arch/x86_64/idt.c; they do not replace it. Calling
 * panic_screen_show() paints the whole framebuffer one flat colour
 * (forcing the console back out of GUI mode first, so it wins over
 * whatever highX/tuswm last drew - correct, since nothing is
 * compositing after a panic) and prints a short banner. The caller
 * still does its own register dump with kprintf() afterwards; this
 * only owns the coloured banner and classification.
 */
#ifndef TUS_PANIC_SCREEN_H
#define TUS_PANIC_SCREEN_H

/* Which screen to show, and why:
 *   PANIC_GSOD - a system/software error inside the kernel itself.
 *                The default for an unclassified fatal exception.
 *   PANIC_RSOD - a hardware error: double fault (untrustworthy
 *                stack), machine check (#MC, the CPU reporting a
 *                real hardware fault), or an explicit hardware-layer
 *                report (e.g. a disk/controller driver).
 *   PANIC_BSOD - a critical *service* crash, reported by tusSM (the
 *                service manager) when a supervised service that is
 *                marked critical dies past its restart budget - not
 *                raised directly from the exception handlers.
 */
enum panic_kind {
    PANIC_GSOD = 0,
    PANIC_RSOD = 1,
    PANIC_BSOD = 2,
};

/* Paints the panic screen and prints `title` as the banner line.
 * Does not halt the machine - the caller keeps control to print its
 * own register dump right after, then halts. Safe to call before
 * interrupts are fully torn down; internally it does not re-enable
 * them. */
void panic_screen_show(enum panic_kind kind, const char *title);

#endif
