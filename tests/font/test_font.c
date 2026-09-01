/*
 * test_font.c - host tests for the TrueType engine
 *
 * The interesting one is check_against_model(): the rasteriser is
 * compared against a brute-force model that asks, for a grid of
 * sample points inside each pixel, whether that point is inside the
 * outline - by counting how many edges a ray from it crosses. The
 * model is obviously correct and far too slow to ship; the rasteriser
 * is fast and full of arithmetic that could be subtly wrong. Checking
 * one against the other is the only way to know the fast one is
 * right, and it is the same technique tests/highx uses on the
 * compositor.
 *
 * The rest check the parts a model cannot: that the file is read
 * correctly, that a damaged file is refused rather than followed, and
 * that the shapes really do look like the letters they claim to be.
 *
 * Build and run:  make -C tests/font run
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compiled straight in, the way tests/fb compiles the console: the
 * code under test is the code that ships, and the model below needs
 * the internals. */
#include "../../userspace/tusfont/ttf.c"
#include "../../userspace/tusfont/raster.c"
#include "../../userspace/tusfont/text.c"

#define FONT_PATH "../../rootfs/usr/share/fonts/OpenSans-Light.ttf"

static int g_checks, g_failures;

static void ok(const char *name) {
    g_checks++;
    printf("  [PASS] %s\n", name);
}

static void fail(const char *name, const char *why) {
    g_checks++;
    g_failures++;
    printf("  [FAIL] %s: %s\n", name, why);
}

static void check(int cond, const char *name, const char *why) {
    if (cond) { ok(name); } else { fail(name, why); }
}

/* ---- the model ----
 *
 * Rebuild the same edge list the rasteriser builds, then decide each
 * pixel by sampling: SUB x SUB points, each inside-or-out by a
 * non-zero winding count along a horizontal ray. Coverage is the
 * fraction of samples inside.
 */
#define SUB 12

static int model_edges(struct tf_font *f, const struct tf_outline *o,
                       tf_fixed size, tf_fixed x_off, tf_fixed dx,
                       tf_fixed y_shift, struct tf_edges *e) {
    int first = 0;
    for (int c = 0; c < o->ncontours; c++) {
        int end = o->contour_end[c];
        if (end > o->npoints) end = o->npoints;
        if (end > first) {
            if (!emit_contour(e, o->points + first, end - first,
                              f, size, dx, y_shift)) {
                return 0;
            }
        }
        first = end;
    }
    (void)x_off;
    return 1;
}

static int inside(const struct tf_edges *e, tf_fixed x, tf_fixed y) {
    int winding = 0;
    for (int i = 0; i < e->n; i++) {
        const struct tf_edge *ed = &e->v[i];
        if (y < ed->y0 || y >= ed->y1) {
            continue;
        }
        int64_t dy = ed->y1 - ed->y0;
        int64_t num = (int64_t)(ed->x1 - ed->x0) * (y - ed->y0);
        tf_fixed cross = ed->x0 + (tf_fixed)(num / dy);
        if (cross > x) {
            winding += ed->dir;
        }
    }
    return winding != 0;
}

/* Compare one glyph's rasterisation against the model. Returns the
 * worst per-pixel difference in coverage (0..255). */
static int compare_glyph(struct tf_font *f, uint16_t g, tf_fixed size,
                         int *out_pixels) {
    struct tf_outline o;
    if (!tf_load_outline(f, g, &o)) {
        return -1;
    }
    struct tf_bitmap bm;
    if (tf_rasterise(f, &o, size, 0, &bm) != 0 || bm.pixels == NULL) {
        tf_outline_free(&o);
        return -1;
    }

    /* The rasteriser's own framing, recomputed so the model lands on
     * exactly the same pixel grid. */
    int32_t min_x = o.points[0].x, max_x = min_x;
    int32_t min_y = o.points[0].y, max_y = min_y;
    for (int i = 1; i < o.npoints; i++) {
        if (o.points[i].x < min_x) min_x = o.points[i].x;
        if (o.points[i].x > max_x) max_x = o.points[i].x;
        if (o.points[i].y < min_y) min_y = o.points[i].y;
        if (o.points[i].y > max_y) max_y = o.points[i].y;
    }
    tf_fixed fx0 = TF_FLOOR(tf_scale(f, min_x, size)) - TF_ONE;
    tf_fixed fy1 = TF_CEIL(tf_scale(f, max_y, size)) + TF_ONE;

    struct tf_edges e;
    memset(&e, 0, sizeof(e));
    if (!model_edges(f, &o, size, 0, -fx0, fy1, &e)) {
        free(e.v);
        tf_free_bitmap(&bm);
        tf_outline_free(&o);
        return -1;
    }

    int worst = 0;
    for (int y = 0; y < bm.h; y++) {
        for (int x = 0; x < bm.w; x++) {
            int hits = 0;
            for (int sy = 0; sy < SUB; sy++) {
                for (int sx = 0; sx < SUB; sx++) {
                    tf_fixed px = TF_FROM_INT(x) +
                                  (tf_fixed)((2 * sx + 1) * TF_ONE / (2 * SUB));
                    tf_fixed py = TF_FROM_INT(y) +
                                  (tf_fixed)((2 * sy + 1) * TF_ONE / (2 * SUB));
                    if (inside(&e, px, py)) {
                        hits++;
                    }
                }
            }
            int want = hits * 255 / (SUB * SUB);
            int got = bm.pixels[(size_t)y * bm.stride + x];
            int diff = got > want ? got - want : want - got;
            if (diff > worst) {
                worst = diff;
            }
        }
    }
    if (out_pixels != NULL) {
        *out_pixels = bm.w * bm.h;
    }

    free(e.v);
    tf_free_bitmap(&bm);
    tf_outline_free(&o);
    return worst;
}

/* ---- helpers ---- */

static long ink(const struct tf_bitmap *bm) {
    long sum = 0;
    for (int y = 0; y < bm->h; y++) {
        for (int x = 0; x < bm->w; x++) {
            sum += bm->pixels[(size_t)y * bm->stride + x];
        }
    }
    return sum;
}

static void show(const struct tf_bitmap *bm, const char *label) {
    static const char *ramp = " .:-=+*#%@";
    printf("     %s  (%dx%d, left %d, top %d)\n", label, bm->w, bm->h,
           bm->left, bm->top);
    for (int y = 0; y < bm->h; y++) {
        printf("     |");
        for (int x = 0; x < bm->w; x++) {
            int v = bm->pixels[(size_t)y * bm->stride + x];
            putchar(ramp[v * 9 / 255]);
        }
        printf("|\n");
    }
}

int main(void) {
    printf("== TUS TrueType tests ==\n");

    printf("-- reading the file --\n");
    struct tf_font *f = tf_open(FONT_PATH);
    if (f == NULL) {
        fail("the font opens", tf_error());
        printf("== %d checks, %d failed ==\n", g_checks, g_failures);
        return 1;
    }
    ok("the font opens");

    check(f->units_per_em == 2048, "units per em is read",
          "expected 2048");
    check(f->num_glyphs > 200, "the glyph count is read", "too few glyphs");
    check(f->cmap_format == 4 || f->cmap_format == 12,
          "a usable cmap was chosen", "no format 4 or 12 cmap");

    printf("-- the character map --\n");
    uint16_t gA = tf_glyph_index(f, 'A');
    uint16_t gW = tf_glyph_index(f, 'W');
    uint16_t gi = tf_glyph_index(f, 'i');
    check(gA != 0 && gW != 0 && gi != 0, "ASCII letters have glyphs",
          "a letter mapped to .notdef");
    check(tf_glyph_index(f, 0x4E2D) == 0,
          "a character the font lacks maps to .notdef",
          "a Chinese character found a glyph in a Latin font");

    /* The letters TUS is written among. */
    struct { uint32_t cp; const char *name; } turkish[] = {
        { 0x011F, "g breve" }, { 0x011E, "G breve" },
        { 0x015F, "s cedilla" }, { 0x015E, "S cedilla" },
        { 0x0131, "dotless i" }, { 0x0130, "dotted I" },
        { 0x00E7, "c cedilla" }, { 0x00FC, "u diaeresis" },
    };
    int missing = 0;
    for (unsigned i = 0; i < sizeof(turkish) / sizeof(turkish[0]); i++) {
        if (tf_glyph_index(f, turkish[i].cp) == 0) {
            missing++;
        }
    }
    check(missing == 0, "the Turkish letters have glyphs",
          "the font is missing some");

    printf("-- metrics --\n");
    tf_fixed size = TF_FROM_INT(32);
    struct tf_metrics m;
    tf_metrics_at(f, size, &m);
    check(m.ascent > 0 && m.descent > 0,
          "ascent and descent are positive", "one of them is not");
    check(m.height > m.ascent, "the line height clears the ascent",
          "lines would overlap");
    printf("     ascent %d, descent %d, height %d (26.6 px at 32 px/em)\n",
           m.ascent, m.descent, m.height);

    check(tf_advance(f, gi, size) < tf_advance(f, gW, size),
          "an i is narrower than a W", "the advances are wrong");
    check(tf_advance(f, tf_glyph_index(f, ' '), size) > 0,
          "a space has an advance", "a space would take no room");

    printf("-- shapes --\n");
    struct tf_bitmap bm;
    if (tf_render_glyph(f, gA, size, 0, &bm) == 0 && bm.pixels != NULL) {
        show(&bm, "'A' at 32 px");
        check(ink(&bm) > 0, "'A' has ink", "the glyph rendered empty");
        tf_free_bitmap(&bm);

        /* Ink is area, and area scales with the square of the size.
         * This is the check that catches a scale factor applied once
         * instead of twice, or a coordinate that overflows on the way
         * up - and unlike "is an A symmetric" it is a fact about the
         * rasteriser rather than about the typeface. */
        struct tf_bitmap small, big;
        if (tf_render_glyph(f, gA, TF_FROM_INT(20), 0, &small) == 0 &&
            tf_render_glyph(f, gA, TF_FROM_INT(40), 0, &big) == 0) {
            long a = ink(&small), b = ink(&big);
            long expect = a * 4;
            long off = b > expect ? b - expect : expect - b;
            char why[96];
            snprintf(why, sizeof(why),
                     "20px has %ld, 40px has %ld, expected about %ld",
                     a, b, expect);
            check(a > 0 && off * 100 / expect < 12,
                  "doubling the size quadruples the ink", why);
            tf_free_bitmap(&small);
            tf_free_bitmap(&big);
        } else {
            fail("doubling the size quadruples the ink", "a render failed");
        }
    } else {
        fail("'A' renders", "tf_render_glyph failed");
    }

    /* An 'H' is two vertical stems with a bar between them. A row
     * above the bar must have ink at two separate places and none in
     * between; the bar's own row must be continuous. That is a fact
     * about the letter that a broken fill (a winding rule inverted,
     * a span that leaks) would break immediately. */
    uint16_t gH = tf_glyph_index(f, 'H');
    if (tf_render_glyph(f, gH, size, 0, &bm) == 0 && bm.pixels != NULL) {
        int runs_top = 0, prev = 0;
        int y = bm.h / 6;   /* above the crossbar */
        for (int x = 0; x < bm.w; x++) {
            int on = bm.pixels[(size_t)y * bm.stride + x] > 96;
            if (on && !prev) {
                runs_top++;
            }
            prev = on;
        }
        char why[64];
        snprintf(why, sizeof(why), "found %d runs of ink, expected 2",
                 runs_top);
        check(runs_top == 2, "an 'H' has two stems above its crossbar", why);

        /* Find the crossbar: the row with the most ink. */
        int best_row = 0;
        long best = -1;
        for (int r = 0; r < bm.h; r++) {
            long sum = 0;
            for (int x = 0; x < bm.w; x++) {
                sum += bm.pixels[(size_t)r * bm.stride + x];
            }
            if (sum > best) {
                best = sum;
                best_row = r;
            }
        }
        int runs_bar = 0;
        prev = 0;
        for (int x = 0; x < bm.w; x++) {
            int on = bm.pixels[(size_t)best_row * bm.stride + x] > 96;
            if (on && !prev) {
                runs_bar++;
            }
            prev = on;
        }
        check(runs_bar == 1, "and one continuous crossbar",
              "the bar is broken");
        tf_free_bitmap(&bm);
    } else {
        fail("'H' renders", "tf_render_glyph failed");
    }

    /* An 'o' is a ring: the middle must be EMPTY. That is the
     * non-zero winding rule doing its job on a counter contour, and
     * it is the single most likely thing to be wrong. */
    uint16_t go = tf_glyph_index(f, 'o');
    if (tf_render_glyph(f, go, size, 0, &bm) == 0 && bm.pixels != NULL) {
        int cx = bm.w / 2, cy = bm.h / 2;
        int centre = bm.pixels[(size_t)cy * bm.stride + cx];
        check(centre < 32, "the counter of an 'o' is empty",
              "the middle of the o is filled in");
        check(ink(&bm) > 0, "'o' has ink", "the glyph rendered empty");
        tf_free_bitmap(&bm);
    } else {
        fail("'o' renders", "tf_render_glyph failed");
    }

    /* A composite glyph: the Turkish g-breve is a 'g' with a mark
     * placed over it, stored as two components. */
    uint16_t gbreve = tf_glyph_index(f, 0x011F);
    uint16_t gplain = tf_glyph_index(f, 'g');
    struct tf_bitmap b1, b2;
    if (tf_render_glyph(f, gbreve, size, 0, &b1) == 0 &&
        tf_render_glyph(f, gplain, size, 0, &b2) == 0 &&
        b1.pixels != NULL && b2.pixels != NULL) {
        show(&b1, "'g breve' at 32 px");
        check(b1.h > b2.h, "a composite glyph includes its accent",
              "the g-breve is no taller than a plain g");
        check(ink(&b1) > ink(&b2), "and has more ink than the base letter",
              "the accent drew nothing");
        tf_free_bitmap(&b1);
        tf_free_bitmap(&b2);
    } else {
        fail("the g-breve renders", "tf_render_glyph failed");
    }

    printf("-- the rasteriser against a brute-force model --\n");
    {
        const char *sample = "AWoegimBQ8@#";
        int worst_all = 0;
        long pixels = 0;
        for (const char *p = sample; *p; p++) {
            int px = 0;
            int worst = compare_glyph(f, tf_glyph_index(f, (uint32_t)*p),
                                      TF_FROM_INT(24), &px);
            if (worst < 0) {
                fail("every glyph rasterises", "one failed to render");
                worst_all = 999;
                break;
            }
            pixels += px;
            if (worst > worst_all) {
                worst_all = worst;
            }
        }
        char why[128];
        snprintf(why, sizeof(why),
                 "worst pixel differs by %d of 255", worst_all);
        /* The two disagree only where they sample differently: the
         * rasteriser takes five vertical samples with exact
         * horizontal coverage, the model a 12x12 grid. On an edge
         * pixel that is a genuine difference of method, not of
         * result, and it is bounded. */
        check(worst_all <= 64, "the rasteriser matches the model", why);
        printf("     %ld pixels compared, worst difference %d\n",
               pixels, worst_all);
    }

    /* Sizes from tiny to large, to catch arithmetic that overflows
     * only when the numbers get big. */
    {
        int bad = 0;
        for (int px = 6; px <= 200; px += 7) {
            int worst = compare_glyph(f, gA, TF_FROM_INT(px), NULL);
            if (worst < 0 || worst > 80) {
                bad = px;
                break;
            }
        }
        char why[64];
        snprintf(why, sizeof(why), "it went wrong at %d px", bad);
        check(bad == 0, "the rasteriser is right at every size", why);
    }

    printf("-- laying out a string --\n");
    {
        tf_fixed w1 = tf_text_width(f, "AV", size);
        tf_fixed w2 = tf_text_width(f, "AX", size);
        check(w1 != 0 && w2 != 0, "a string can be measured",
              "the width came back zero");
        /* Open Sans kerns AV tighter than AX. If the font has no kern
         * table this is equal, which is not a failure of the code. */
        printf("     AV %d, AX %d (26.6 px)%s\n", w1, w2,
               w1 < w2 ? "  - kerned" : "  - no kerning in this font");

        tf_fixed turkish_w = tf_text_width(f, "gündüz ışığı", size);
        check(turkish_w > 0, "a Turkish string can be measured",
              "the width came back zero");
    }

    printf("-- UTF-8 --\n");
    {
        uint32_t cp = 0;
        check(tf_utf8_next("A", &cp) == 1 && cp == 'A',
              "ASCII decodes", "wrong");
        check(tf_utf8_next("\xc5\x9f", &cp) == 2 && cp == 0x015F,
              "a two-byte letter decodes", "wrong");
        check(tf_utf8_next("\xe2\x82\xac", &cp) == 3 && cp == 0x20AC,
              "a three-byte symbol decodes", "wrong");
        check(tf_utf8_next("\xbf", &cp) == 1 && cp == 0xFFFD,
              "a stray continuation byte consumes one byte",
              "a decoder that consumed more could be walked off the end");
        check(tf_utf8_next("\xc0\xaf", &cp) == 2 && cp == 0xFFFD,
              "an overlong encoding is refused", "it decoded to '/'");
        check(tf_utf8_next("\xed\xa0\x80", &cp) == 3 && cp == 0xFFFD,
              "a surrogate is refused", "it decoded");
    }

    printf("-- damaged fonts --\n");
    {
        /* A font file is data from outside the program - the point of
         * a font directory is that a user can drop one in - so every
         * one of these has to fail rather than read past the end of
         * the buffer. Built with -fsanitize=address (see the
         * Makefile), so an out-of-bounds read fails the test loudly
         * instead of quietly returning a byte of something else. */
        size_t len = f->r.len;
        unsigned char *copy = malloc(len);
        memcpy(copy, f->r.data, len);

        struct tf_font *t = tf_load(copy, 12, 0);
        check(t == NULL, "a file cut off after the header is refused",
              "it opened");
        tf_close(t);

        unsigned char two[2] = { 0, 0 };
        t = tf_load(two, sizeof(two), 0);
        check(t == NULL, "a two-byte file is refused", "it opened");
        tf_close(t);

        memcpy(copy, "OTTO", 4);
        t = tf_load(copy, len, 0);
        check(t == NULL, "a CFF font is refused with a reason",
              "it opened as if it had glyf outlines");
        tf_close(t);
        memcpy(copy, f->r.data, 4);

        /* Truncation, everywhere. A font cut short may legitimately
         * still work - Open Sans keeps its outlines in the first
         * half and the rest is kerning, names and a signature - so
         * the test is not "it must be refused" but "whatever it does,
         * it does not read memory it does not own".
         *
         * Each surviving font is then asked to render, because the
         * dangerous reads are in the glyph parser, not in the
         * opening. */
        int opened = 0, refused = 0;
        for (size_t cut = 16; cut < len; cut += 977) {
            struct tf_font *tt = tf_load(copy, cut, 0);
            if (tt == NULL) {
                refused++;
                continue;
            }
            opened++;
            for (uint16_t g = 0; g < tt->num_glyphs && g < 300; g++) {
                struct tf_bitmap b;
                if (tf_render_glyph(tt, g, TF_FROM_INT(18), 0, &b) == 0) {
                    tf_free_bitmap(&b);
                }
            }
            tf_close(tt);
        }
        char why[128];
        snprintf(why, sizeof(why), "%d opened, %d refused", opened, refused);
        check(refused > 0 && opened > 0,
              "truncated fonts either work or are refused, and never crash",
              why);
        printf("     %s\n", why);

        /* Corruption, in the tables the glyph parser walks. A flipped
         * byte in loca points a glyph anywhere in glyf; a flipped
         * byte in a glyph header claims a contour count that is not
         * there. */
        unsigned seed = 12345;
        int rendered = 0;
        for (int round = 0; round < 40; round++) {
            memcpy(copy, f->r.data, len);
            for (int k = 0; k < 64; k++) {
                seed = seed * 1103515245u + 12345u;
                size_t at = f->loca + (seed >> 8) % (f->loca_len + f->glyf_len);
                if (at < len) {
                    copy[at] = (unsigned char)(seed >> 16);
                }
            }
            struct tf_font *tt = tf_load(copy, len, 0);
            if (tt == NULL) {
                continue;
            }
            for (uint16_t g = 0; g < tt->num_glyphs && g < 200; g++) {
                struct tf_bitmap b;
                if (tf_render_glyph(tt, g, TF_FROM_INT(18), 0, &b) == 0) {
                    rendered++;
                    tf_free_bitmap(&b);
                }
            }
            tf_close(tt);
        }
        snprintf(why, sizeof(why), "%d glyphs came back from corrupt fonts",
                 rendered);
        check(1, "corrupt loca and glyf tables do not crash the parser", why);
        printf("     %s\n", why);

        free(copy);
    }

    tf_close(f);
    printf("== %d checks, %d failed ==\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
