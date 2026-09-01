/*
 * hxfont - a type specimen, drawn with the TrueType engine
 *
 * TUS draws text from an 8x16 bitmap font: one size, one weight, one
 * shape per character. This window is the other thing - a real
 * typeface, scaled, anti-aliased and kerned, out of a `.ttf` file
 * that nothing in the system was compiled against.
 *
 * It is a demo, and it is also the proof: userspace/tusfont has host
 * tests that check the rasteriser against a brute-force model, and
 * this is what checks that the same code draws on a real screen
 * through the real protocol.
 *
 *     hxfont [FONT.ttf]
 *
 * Defaults to /usr/share/fonts/OpenSans-Light.ttf.
 *
 * HOW THE PIXELS GET THERE
 *
 * The engine produces 8-bit coverage - how much of each pixel the
 * glyph covers - and highX takes ARGB. So the window is drawn into a
 * buffer of our own, glyph coverage is blended into it, and the whole
 * thing goes up with one HX_OP_PUT_IMAGE. Blending in the client is
 * not a workaround: coverage is what anti-aliasing IS, and a protocol
 * request that took a bitmap font's on/off bits could not carry it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "highapi/highapi.h"
#include "tusfont/tusfont.h"

#define WIN_W 760
#define WIN_H 560

#define BG      0x001E2028u
#define FG      0x00E8E8E8u
#define DIM     0x00808A99u
#define ACCENT  0x004FA3D1u

static uint32_t *g_buf;

/* Blend one coverage value over what is already in the buffer.
 *
 * Straight alpha, done per channel in integer arithmetic: the
 * userspace of TUS is built without floating point (see tusfont.h),
 * and a blend is one multiply and one shift per channel anyway. */
static void blend(int x, int y, uint32_t color, unsigned cov) {
    if (x < 0 || y < 0 || x >= WIN_W || y >= WIN_H || cov == 0) {
        return;
    }
    uint32_t *p = &g_buf[(size_t)y * WIN_W + x];
    if (cov >= 255) {
        *p = color;
        return;
    }
    uint32_t dst = *p;
    unsigned inv = 255 - cov;
    unsigned r = ((color >> 16 & 0xFF) * cov + (dst >> 16 & 0xFF) * inv) / 255;
    unsigned g = ((color >> 8 & 0xFF) * cov + (dst >> 8 & 0xFF) * inv) / 255;
    unsigned b = ((color & 0xFF) * cov + (dst & 0xFF) * inv) / 255;
    *p = (r << 16) | (g << 8) | b;
}

struct draw_ctx {
    uint32_t color;
};

static int draw_glyph(void *ctx, const struct tf_bitmap *bm, int x, int y) {
    struct draw_ctx *d = (struct draw_ctx *)ctx;
    for (int row = 0; row < bm->h; row++) {
        const uint8_t *src = bm->pixels + (size_t)row * bm->stride;
        for (int col = 0; col < bm->w; col++) {
            blend(x + col, y + row, d->color, src[col]);
        }
    }
    return 0;
}

static void text(struct tf_font *f, const char *s, int size_px, int x, int y,
                 uint32_t color) {
    struct draw_ctx ctx = { color };
    tf_text_draw(f, s, TF_FROM_INT(size_px), x, y, draw_glyph, &ctx);
}

static void fill(uint32_t color) {
    for (size_t i = 0; i < (size_t)WIN_W * WIN_H; i++) {
        g_buf[i] = color;
    }
}

static void rule(int y, uint32_t color) {
    for (int x = 24; x < WIN_W - 24; x++) {
        g_buf[(size_t)y * WIN_W + x] = color;
    }
}

int main(int argc, char **argv) {
    const char *path = argc > 1 ? argv[1]
                                : "/usr/share/fonts/OpenSans-Light.ttf";

    struct tf_font *f = tf_open(path);
    if (f == NULL) {
        fprintf(stderr, "hxfont: %s: %s\n", path, tf_error());
        return 1;
    }

    g_buf = malloc((size_t)WIN_W * WIN_H * sizeof(uint32_t));
    if (g_buf == NULL) {
        fprintf(stderr, "hxfont: out of memory\n");
        return 1;
    }

    struct hx_display dpy;
    if (hx_open(&dpy) < 0) {
        fprintf(stderr, "hxfont: no highX session\n");
        return 1;
    }
    unsigned int win = hx_create_window(60, 40, WIN_W, WIN_H, 0, BG,
                                        "hxfont - TrueType");
    if (win == 0) {
        fprintf(stderr, "hxfont: cannot create a window\n");
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);

    for (;;) {
        struct hx_event ev;
        if (hx_next_event(&ev, -1) < 0) {
            break;
        }
        if (ev.type == HX_EV_CLOSE) {
            break;
        }
        if (ev.type == HX_EV_KEY && (ev.key == 'q' || ev.key == 27)) {
            break;
        }
        if (ev.type != HX_EV_EXPOSE) {
            continue;
        }

        fill(BG);

        int y = 52;
        text(f, "TUS", 40, 28, y, ACCENT);
        text(f, "Toasty Unix Software", 20, 130, y, FG);
        y += 16;
        rule(y, 0x00303643u);
        y += 40;

        /* A size ladder. The point of a scalable font is that these
         * are the same shapes, not different fonts. */
        const int sizes[] = { 10, 12, 14, 18, 24, 32 };
        for (unsigned i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
            char label[16];
            snprintf(label, sizeof(label), "%d", sizes[i]);
            text(f, label, 11, 28, y, DIM);
            text(f, "Handgloves \xc3\xa4\xc3\xb6\xc3\xbc \xc3\x9f",
                 sizes[i], 64, y, FG);
            y += sizes[i] + 12;
        }

        y += 12;
        rule(y, 0x00303643u);
        y += 34;

        /* The letters TUS is written among, and a Turkish pangram -
         * which is what makes this more than an ASCII demo. */
        text(f, "T\xc3\xbcrk\xc3\xa7""e", 15, 28, y, DIM);
        text(f, "\xc4\x9e\xc4\x9f \xc4\xb0\xc4\xb1 \xc5\x9e\xc5\x9f "
                "\xc3\x87\xc3\xa7 \xc3\x96\xc3\xb6 \xc3\x9c\xc3\xbc",
             28, 120, y, FG);
        y += 40;
        text(f, "Pijamal\xc4\xb1 hasta ya\xc4\x9f\xc4\xb1z "
                "\xc5\x9fof\xc3\xb6re \xc3\xa7""abucak g\xc3\xbcvendi.",
             17, 28, y, FG);
        y += 40;
        text(f, "Vom Ma\xc3\x9f" "e her: gr\xc3\xb6\xc3\x9f" "ere Zw\xc3\xb6lf. "
                "Fran\xc3\xa7""ais: o\xc3\xb9 est l'\xc3\xa9t\xc3\xa9?",
             17, 28, y, DIM);
        y += 44;

        char foot[160];
        struct tf_metrics m;
        tf_metrics_at(f, TF_FROM_INT(17), &m);
        snprintf(foot, sizeof(foot),
                 "%s - %u glyphs, %u units/em, line height %d px  |  q to quit",
                 path, (unsigned)tf_glyph_count(f), 2048,
                 TF_TO_INT(m.height));
        text(f, foot, 12, 28, WIN_H - 24, DIM);

        hx_image(win, 0, 0, WIN_W, WIN_H, g_buf);
        hx_commit(win);
    }

    hx_destroy_window(win);
    hx_close(&dpy);
    tf_close(f);
    free(g_buf);
    return 0;
}
