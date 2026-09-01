/*
 * console.h - unified text console
 *
 * Every kernel message goes through this layer. Output is always
 * written to the serial port (the reliable debug channel) and, when a
 * Limine framebuffer is available, mirrored to the framebuffer text
 * console. If no framebuffer exists the system still works, serial-only.
 */

#ifndef TUS_CORE_CONSOLE_H
#define TUS_CORE_CONSOLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <limine.h>

/* Initialize serial and (if given) the framebuffer console. */
void console_init(struct limine_framebuffer *fb);

/* Write one character to every active console sink. */
void console_putchar(char c);

/* Write a NUL-terminated string to every active console sink. */
void console_write(const char *s);

/* Write `n` bytes. Cheaper than the same bytes one at a time - use it
 * whenever the whole buffer is already in hand. */
void console_write_n(const char *s, size_t n);

/* Write to the serial mirror only - never the framebuffer, and never
 * offered to a terminal session's capture hook. For verbose diagnostic
 * output (hardware detection, memory map, PCI enumeration) that would
 * otherwise flood the framebuffer text console during boot; the screen
 * keeps the splash and a short summary, the serial log keeps everything. */
void console_write_serial_only(const char *s, size_t n);

/* Clear the framebuffer console (no-op on serial). */
void console_clear(void);

/* Scroll the framebuffer console one page (PageUp/PageDown). */
void console_scroll_page(int dir);

/* Set the foreground/background colors used by the framebuffer console. */
void console_set_color(uint32_t fg, uint32_t bg);

/* Start the framebuffer text grid below the boot splash logo band. */
void console_set_text_top(uint32_t pixel_y);

/* ---- capture ----
 *
 * A terminal window runs the kernel's own shell, and the built-ins
 * that shell runs print with kprintf/console_write - output that
 * never passes a file descriptor. A capture hook is how it reaches
 * the window instead of the screen: it is asked first, and when it
 * says it took the bytes the framebuffer and the serial mirror are
 * skipped. kernel/term/term.c installs one that answers for the task
 * currently running; everything else keeps the console it always had.
 *
 * The hook is called from kprintf, i.e. with preemption disabled, so
 * it must never block. */
typedef bool (*console_capture_fn)(const char *s, size_t n);
void console_set_capture(console_capture_fn fn);

/* True if a framebuffer console is active. */
bool console_has_framebuffer(void);

#endif /* TUS_CORE_CONSOLE_H */
