/* panic_screen.c - see panic_screen.h */

#include "panic_screen.h"
#include "console.h"
#include "klib.h"
#include "../drivers/fb/fb.h"

/* 0xRRGGBB, matching fb_fill()'s existing convention. */
#define COLOR_BSOD 0x0000AAu /* blue   - critical service crash   */
#define COLOR_GSOD 0x008000u /* green  - system/software error    */
#define COLOR_RSOD 0xAA0000u /* red    - hardware error           */

void panic_screen_show(enum panic_kind kind, const char *title) {
    uint32_t color;
    const char *label;

    switch (kind) {
    case PANIC_BSOD: color = COLOR_BSOD; label = "BSOD"; break;
    case PANIC_RSOD: color = COLOR_RSOD; label = "RSOD"; break;
    case PANIC_GSOD:
    default:         color = COLOR_GSOD; label = "GSOD"; break;
    }

    /* A panic outlives whatever highX/tuswm was compositing - drop
     * back to the raw text console so fb_fill()/fb_putchar() draw
     * straight onto the physical framebuffer instead of being a
     * silent no-op under fb_fill()'s own g_graphics guard. This is
     * what makes the "GUI" and "TTY" panic screens the same code
     * path: there is nothing left compositing after this point. */
    fb_set_graphics(false);

    fb_set_color(0xFFFFFFu, color);
    console_set_color(0xFFFFFFu, color);
    fb_fill(color);

    console_write("\n\n");
    console_write("  ");
    console_write(label);
    console_write("\n  ");
    console_write(title);
    console_write("\n\n");
}
