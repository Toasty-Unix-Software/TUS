/*
 * vbe.h - runtime display mode setting (Bochs VBE extensions)
 *
 * Limine hands the kernel one framebuffer and there is no way to ask
 * it for another: the boot protocol chooses a mode once and the
 * bootloader is gone by the time anyone has an opinion about it. To
 * change the resolution while the system runs, something has to
 * program the display adapter directly.
 *
 * This driver programs the *Bochs VBE extensions* (the "DISPI"
 * interface), the register set that Bochs introduced and that QEMU's
 * `-vga std` / `bochs-display` and VirtualBox all implement. It is
 * two I/O ports - an index register and a data register - and a
 * handful of indices for width, height, depth and an enable bit. That
 * is the whole interface, which is exactly why it is the one every
 * emulator agrees on.
 *
 *   outw(0x1CE, index); outw(0x1CF, value);
 *
 * WHAT THIS DOES NOT DO
 *
 * This is not a GPU driver. On real hardware - Intel, AMD, NVIDIA -
 * there is no DISPI register set, `vbe_available()` returns false and
 * the machine keeps whatever mode Limine negotiated with the firmware.
 * Real mode setting there means a real display driver per vendor
 * (GTT, display pipes, PLL programming), which is a separate project.
 * The honest summary: res_set works under QEMU/Bochs/VirtualBox, and
 * reports "not supported" everywhere else rather than pretending.
 *
 * The linear framebuffer is the display adapter's PCI BAR0. Changing
 * the mode does not move it, so the physical address found at init
 * stays valid; only the pitch and the number of bytes behind it
 * change, which is why vbe_set_mode() reports both back.
 */

#ifndef TUS_DRIVERS_VBE_H
#define TUS_DRIVERS_VBE_H

#include <stdbool.h>
#include <stdint.h>

/* Largest mode this driver will program. The cap is not the hardware's
 * (QEMU's stdvga defaults to 16 MiB of VRAM, enough for 1920x1080x4
 * twice over) but ours: fb.c keeps a character cell per grid position,
 * and the kernel maps the framebuffer into a fixed reservation. */
#define VBE_MAX_WIDTH  1920u
#define VBE_MAX_HEIGHT 1200u

/* Probe for the DISPI register set and remember the framebuffer's
 * physical address (the display adapter's PCI BAR0). Safe to call on
 * a machine that has no such adapter. Returns 0 if one was found,
 * -1 otherwise. */
int vbe_init(void);

/* True when vbe_set_mode() can actually do something. */
bool vbe_available(void);

/* The DISPI interface revision (0xB0C0..0xB0C5), or 0. Revisions
 * below 0xB0C4 have no linear framebuffer, so this driver refuses
 * them. */
uint16_t vbe_id(void);

/* Physical address of the linear framebuffer, or 0 if unknown. */
uint64_t vbe_lfb_phys(void);

/* Program a mode. `bpp` must be 32 - the rest of the system paints
 * 0x00RRGGBB words and nothing else. On success *out_pitch receives
 * the new bytes-per-scanline (the adapter is told to make the virtual
 * width equal the visible width, so this is width*4).
 *
 * Returns 0, or a negative errno: -ENODEV (no DISPI adapter),
 * -EINVAL (a mode outside the caps or a depth that is not 32), or
 * -EIO (the adapter accepted the write but reports a different mode,
 * which is how it says "not enough VRAM"). */
int vbe_set_mode(uint32_t width, uint32_t height, uint32_t bpp,
                 uint64_t *out_pitch);

/* The modes res_set offers when asked to list them. `index` walks the
 * table from 0; returns false once it runs off the end. These are the
 * common 16:9/16:10/4:3 modes, not a hardware-reported list: the
 * DISPI interface has no mode enumeration, any width and height that
 * fits in VRAM works, and this table is just the set worth naming. */
bool vbe_mode_at(int index, uint32_t *width, uint32_t *height);

#endif /* TUS_DRIVERS_VBE_H */
