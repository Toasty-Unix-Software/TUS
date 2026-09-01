/*
 * vbe.c - runtime display mode setting (Bochs VBE extensions)
 *
 * See vbe.h for what this is and, more importantly, what it is not.
 */

#include "drivers/vbe/vbe.h"

#include "drivers/pci/pci.h"
#include "arch/x86_64/io.h"
#include "core/errno.h"
#include "core/klib.h"

/* ---- the DISPI register set ---- */

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA  0x01CF

#define VBE_DISPI_INDEX_ID          0x0
#define VBE_DISPI_INDEX_XRES        0x1
#define VBE_DISPI_INDEX_YRES        0x2
#define VBE_DISPI_INDEX_BPP         0x3
#define VBE_DISPI_INDEX_ENABLE      0x4
#define VBE_DISPI_INDEX_BANK        0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH  0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 0x7
#define VBE_DISPI_INDEX_X_OFFSET    0x8
#define VBE_DISPI_INDEX_Y_OFFSET    0x9

/* Interface revisions. 0xB0C4 is the first with a linear framebuffer;
 * anything older is bank-switched VGA memory at 0xA0000 and useless
 * to us. */
#define VBE_DISPI_ID0 0xB0C0
#define VBE_DISPI_ID4 0xB0C4
#define VBE_DISPI_ID5 0xB0C5

#define VBE_DISPI_DISABLED    0x00
#define VBE_DISPI_ENABLED     0x01
#define VBE_DISPI_LFB_ENABLED 0x40
#define VBE_DISPI_NOCLEARMEM  0x80

static uint16_t g_id;         /* 0 = no adapter */
static uint64_t g_lfb_phys;   /* BAR0 of the display adapter */

static void dispi_write(uint16_t index, uint16_t value) {
    outw(VBE_DISPI_IOPORT_INDEX, index);
    outw(VBE_DISPI_IOPORT_DATA, value);
}

static uint16_t dispi_read(uint16_t index) {
    outw(VBE_DISPI_IOPORT_INDEX, index);
    return inw(VBE_DISPI_IOPORT_DATA);
}

/* The mode table res_set lists. Widest first is deliberate: it reads
 * as "best available" going down. */
static const struct {
    uint32_t w, h;
} g_modes[] = {
    { 1920, 1200 }, { 1920, 1080 }, { 1680, 1050 }, { 1600,  900 },
    { 1440,  900 }, { 1366,  768 }, { 1280, 1024 }, { 1280,  800 },
    { 1280,  720 }, { 1152,  864 }, { 1024,  768 }, {  800,  600 },
    {  640,  480 },
};

#define MODE_COUNT ((int)(sizeof(g_modes) / sizeof(g_modes[0])))

bool vbe_mode_at(int index, uint32_t *width, uint32_t *height) {
    if (index < 0 || index >= MODE_COUNT) {
        return false;
    }
    if (width != NULL) {
        *width = g_modes[index].w;
    }
    if (height != NULL) {
        *height = g_modes[index].h;
    }
    return true;
}

/* Find the display adapter and take its BAR0.
 *
 * PCI class 0x03 is "display controller". We do our own one-pass scan
 * rather than asking pci.c, which enumerates for its driver-matching
 * pass and keeps no table we can walk afterwards. Bus 0 is enough:
 * the emulated VGA adapter is always there, and a machine whose
 * display sits behind a bridge is a machine that has no DISPI
 * registers either. */
static uint64_t find_lfb_bar(void) {
    for (uint8_t dev = 0; dev < 32; dev++) {
        for (uint8_t fn = 0; fn < 8; fn++) {
            uint32_t id = pci_config_read(0, dev, fn, 0x00);
            if ((id & 0xFFFF) == 0xFFFF) {
                continue;
            }
            uint32_t cls = pci_config_read(0, dev, fn, 0x08);
            if (((cls >> 24) & 0xFF) != 0x03) {
                continue; /* not a display controller */
            }

            uint32_t bar0 = pci_config_read(0, dev, fn, 0x10);
            if ((bar0 & 0x1) != 0) {
                continue; /* an I/O BAR: not the framebuffer */
            }
            uint64_t base = bar0 & 0xFFFFFFF0u;
            /* A 64-bit BAR carries its upper half in the next slot. */
            if (((bar0 >> 1) & 0x3) == 0x2) {
                uint32_t hi = pci_config_read(0, dev, fn, 0x14);
                base |= (uint64_t)hi << 32;
            }
            if (base == 0) {
                continue;
            }

            /* Memory decoding has to be on for the BAR to answer. */
            uint32_t cmd = pci_config_read(0, dev, fn, 0x04);
            if ((cmd & PCI_COMMAND_MEM_SPACE) == 0) {
                pci_config_write(0, dev, fn, 0x04,
                                 cmd | PCI_COMMAND_MEM_SPACE);
            }
            kprintf("[vbe] display adapter %u:%u.%u  LFB 0x%lx\n",
                    0, dev, (unsigned)fn, (unsigned long)base);
            return base;
        }
    }
    return 0;
}

int vbe_init(void) {
    /* Probe by writing an index and reading the ID back. On a machine
     * with no such register set the ports float and read 0xFFFF, which
     * fails the range check below. */
    uint16_t id = dispi_read(VBE_DISPI_INDEX_ID);
    if (id < VBE_DISPI_ID0 || id > VBE_DISPI_ID5) {
        g_id = 0;
        return -1;
    }
    if (id < VBE_DISPI_ID4) {
        kprintf("[vbe] DISPI 0x%x has no linear framebuffer - ignoring\n", id);
        g_id = 0;
        return -1;
    }

    g_lfb_phys = find_lfb_bar();
    if (g_lfb_phys == 0) {
        kprintf("[vbe] DISPI 0x%x found but no display BAR - ignoring\n", id);
        g_id = 0;
        return -1;
    }

    g_id = id;
    kprintf("[vbe] Bochs VBE 0x%x: runtime mode setting available\n", id);
    return 0;
}

bool vbe_available(void) {
    return g_id != 0;
}

uint16_t vbe_id(void) {
    return g_id;
}

uint64_t vbe_lfb_phys(void) {
    return g_lfb_phys;
}

int vbe_set_mode(uint32_t width, uint32_t height, uint32_t bpp,
                 uint64_t *out_pitch) {
    if (g_id == 0) {
        return -ENODEV;
    }
    if (bpp != 32) {
        return -EINVAL;
    }
    if (width == 0 || height == 0 ||
        width > VBE_MAX_WIDTH || height > VBE_MAX_HEIGHT) {
        return -EINVAL;
    }
    /* The adapter counts scanlines in whole pixels and the console
     * draws 8x16 cells; a width that is not a multiple of 8 is legal
     * for the hardware and merely wastes a sliver of the last column,
     * so it is allowed. A width the hardware cannot express is not:
     * DISPI registers are 16 bits. */
    if (width > 0xFFFFu || height > 0xFFFFu) {
        return -EINVAL;
    }

    /* The programming order is fixed: disable, load the geometry,
     * re-enable with the LFB bit. Writing geometry while enabled is
     * undefined on real Bochs. */
    dispi_write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    dispi_write(VBE_DISPI_INDEX_XRES, (uint16_t)width);
    dispi_write(VBE_DISPI_INDEX_YRES, (uint16_t)height);
    dispi_write(VBE_DISPI_INDEX_BPP, (uint16_t)bpp);
    dispi_write(VBE_DISPI_INDEX_VIRT_WIDTH, (uint16_t)width);
    dispi_write(VBE_DISPI_INDEX_VIRT_HEIGHT, (uint16_t)height);
    dispi_write(VBE_DISPI_INDEX_X_OFFSET, 0);
    dispi_write(VBE_DISPI_INDEX_Y_OFFSET, 0);
    dispi_write(VBE_DISPI_INDEX_BANK, 0);
    dispi_write(VBE_DISPI_INDEX_ENABLE,
                VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);

    /* Read the geometry back. An adapter without the VRAM for the
     * mode accepts the writes and then reports what it actually did,
     * so this is the only way to find out. */
    uint16_t got_w = dispi_read(VBE_DISPI_INDEX_XRES);
    uint16_t got_h = dispi_read(VBE_DISPI_INDEX_YRES);
    uint16_t got_bpp = dispi_read(VBE_DISPI_INDEX_BPP);
    if (got_w != (uint16_t)width || got_h != (uint16_t)height ||
        got_bpp != (uint16_t)bpp) {
        kprintf("[vbe] asked for %ux%ux%u, adapter reports %ux%ux%u\n",
                width, height, bpp, got_w, got_h, got_bpp);
        return -EIO;
    }

    /* VIRT_WIDTH is what the pitch follows, and the adapter may have
     * rounded it up to suit its own alignment. Ask, do not assume. */
    uint16_t virt_w = dispi_read(VBE_DISPI_INDEX_VIRT_WIDTH);
    if (virt_w < got_w) {
        virt_w = got_w;
    }
    if (out_pitch != NULL) {
        *out_pitch = (uint64_t)virt_w * 4u;
    }
    return 0;
}
