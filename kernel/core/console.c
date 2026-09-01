/*
 * console.c - console layer implementation
 *
 * A simple fan-out: each character is handed to the serial driver and,
 * if a framebuffer is present, to the framebuffer text console.
 */

#include "console.h"
#include "klib.h"

#include "drivers/fb/fb.h"
#include "drivers/serial/serial.h"

static bool g_fb_active;
static console_capture_fn g_capture;

void console_set_capture(console_capture_fn fn) {
    g_capture = fn;
}

/* Offer the bytes to the capture hook. True means they are spoken
 * for and no console sink should see them. */
static bool captured(const char *s, size_t n) {
    return g_capture != NULL && g_capture(s, n);
}

/* Decimal, for the escape sequences synthesised below. Returns the
 * number of bytes written. */
static size_t put_u32(char *out, uint32_t v) {
    char digits[10];
    size_t n = 0;
    do {
        digits[n++] = (char)('0' + (v % 10));
        v /= 10;
    } while (v != 0);
    for (size_t i = 0; i < n; i++) {
        out[i] = digits[n - 1 - i];
    }
    return n;
}

/* ESC[<38|48>;2;R;G;Bm - the colour a terminal understands. */
static size_t put_sgr(char *out, uint32_t color, bool background) {
    size_t n = 0;
    out[n++] = 0x1B;
    out[n++] = '[';
    out[n++] = background ? '4' : '3';
    out[n++] = '8';
    out[n++] = ';';
    out[n++] = '2';
    out[n++] = ';';
    n += put_u32(out + n, (color >> 16) & 0xFF);
    out[n++] = ';';
    n += put_u32(out + n, (color >> 8) & 0xFF);
    out[n++] = ';';
    n += put_u32(out + n, color & 0xFF);
    out[n++] = 'm';
    return n;
}

void console_init(struct limine_framebuffer *fb) {
    serial_init();
    g_fb_active = (fb != NULL) && (fb_init(fb) == 0);
}

void console_putchar(char c) {
    if (captured(&c, 1)) {
        /* A terminal window took the character - but the serial line
         * is TUS's debug channel, and a session that never reached it
         * would take `exec: /bin/grep started` off the log the moment
         * the command was typed in a window instead of at the
         * console. The mirror keeps the text (not the synthesised
         * escapes below, which are furniture, not output). */
        serial_putchar(c);
        return;
    }
    serial_putchar(c);
    if (g_fb_active) {
        fb_putchar(c);
    }
}

void console_write(const char *s) {
    size_t n = 0;
    while (s[n] != '\0') {
        n++;
    }
    console_write_n(s, n);
}

/* The bulk path. A program's write() arrives as one buffer, and the
 * serial mirror is far cheaper handed the whole thing than fed a byte
 * at a time - it guards its queue with the interrupt flag, which is
 * worth saving once per write rather than once per character. */
void console_write_n(const char *s, size_t n) {
    if (captured(s, n)) {
        serial_write_n(s, n); /* the debug mirror, see console_putchar */
        return;
    }
    if (g_fb_active) {
        fb_batch_begin();
        for (size_t i = 0; i < n; i++) {
            fb_putchar(s[i]);
        }
        fb_batch_end();
    }
    serial_write_n(s, n);
}

void console_write_serial_only(const char *s, size_t n) {
    serial_write_n(s, n);
}

void console_clear(void) {
    /* What a terminal does with "clear the screen": erase it and put
     * the cursor back at the top left. */
    if (captured("\x1b[2J\x1b[H", 7)) {
        return;
    }
    if (g_fb_active) {
        fb_clear();
    }
}

void console_scroll_page(int dir) {
    /* A captured console has no framebuffer scrollback to walk - the
     * terminal window keeps its own. */
    if (g_capture != NULL && g_capture(NULL, 0)) {
        return;
    }
    if (g_fb_active) {
        fb_scroll_page(dir);
    }
}

void console_set_color(uint32_t fg, uint32_t bg) {
    char seq[48];
    size_t n = put_sgr(seq, fg, false);
    n += put_sgr(seq + n, bg, true);
    if (captured(seq, n)) {
        return;
    }
    if (g_fb_active) {
        fb_set_color(fg, bg);
    }
}

void console_set_text_top(uint32_t pixel_y) {
    if (g_fb_active) {
        fb_set_text_top(pixel_y);
    }
}

bool console_has_framebuffer(void) {
    return g_fb_active;
}
