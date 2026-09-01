/*
 * test_layouts.c - host tests for the keyboard layouts
 *
 * One question runs through all of them: can a user SEE what a key
 * types? A layout table that produces a codepoint the font has no
 * glyph for is worse than a key that does nothing, because the key
 * looks broken and the bug is three files away.
 *
 * So this walks every layout, every level and every scancode, and
 * checks each answer against the same font the console draws from. It
 * also checks the two things the lookup gets wrong if nobody is
 * watching: the ordering the composition table's binary search
 * assumes, and the Turkish i, whose capital no case-mapping function
 * gets right.
 *
 * Build and run:  make -C tests/keymap run
 */

#include <stdio.h>
#include <string.h>

#include "../../kernel/drivers/keymap/keymap.c"
#include "../../kernel/drivers/fb/font_latin.h"

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
    if (cond) {
        ok(name);
    } else {
        fail(name, why);
    }
}

/* Is this glyph blank? A codepoint that resolves to sixteen empty
 * rows draws nothing, which for anything but a space means the entry
 * exists but was never actually drawn. */
static int is_blank(const uint8_t *rows) {
    for (int i = 0; i < 16; i++) {
        if (rows[i] != 0) {
            return 0;
        }
    }
    return 1;
}

/* Every character every layout can produce must have a glyph. */
static void check_layout_glyphs(void) {
    for (int i = 0; keymap_at(i, NULL, NULL); i++) {
        const char *name = NULL, *desc = NULL;
        keymap_at(i, &name, &desc);
        if (keymap_set(name) != 0) {
            fail(name, "keymap_set refused a layout keymap_at listed");
            continue;
        }

        int missing = 0, blank = 0;
        uint32_t worst = 0;
        for (int sc = 0; sc < 128; sc++) {
            for (int level = 0; level < 4; level++) {
                uint32_t cp = keymap_lookup((uint8_t)sc, level & 1,
                                            (level & 2) != 0, false);
                if (cp == 0) {
                    continue;
                }
                if (cp >= KEYMAP_DEAD && cp <= KEYMAP_DEAD_MAX) {
                    /* A dead key draws nothing itself, but the accent
                     * it types on its own has to be visible. */
                    uint32_t sp = keymap_spacing(KEYMAP_MARK(cp));
                    if (sp == 0 || font_glyph_rows(sp) == NULL) {
                        missing++;
                        worst = cp;
                    }
                    continue;
                }
                if (cp < 0x20) {
                    continue; /* Escape, Tab, Enter, Backspace */
                }
                const uint8_t *rows = font_glyph_rows(cp);
                if (rows == NULL) {
                    missing++;
                    worst = cp;
                } else if (cp != ' ' && is_blank(rows)) {
                    blank++;
                    worst = cp;
                }
            }
        }

        char msg[128];
        if (missing != 0) {
            snprintf(msg, sizeof(msg),
                     "%d characters have no glyph (e.g. U+%04X)",
                     missing, worst);
            fail(name, msg);
        } else if (blank != 0) {
            snprintf(msg, sizeof(msg),
                     "%d characters draw an empty cell (e.g. U+%04X)",
                     blank, worst);
            fail(name, msg);
        } else {
            snprintf(msg, sizeof(msg), "%s: every key that types has a glyph",
                     name);
            ok(msg);
        }
    }
}

/* The composition table is searched by halving, which is only correct
 * if it is sorted - and a new entry is added by hand in the middle of
 * a group, which is exactly where a sort order goes wrong. */
static void check_compose_sorted(void) {
    for (int i = 1; i < COMPOSE_COUNT; i++) {
        uint32_t prev = ((uint32_t)g_compose[i - 1].mark << 16) |
                        g_compose[i - 1].base;
        uint32_t cur = ((uint32_t)g_compose[i].mark << 16) | g_compose[i].base;
        if (cur <= prev) {
            char msg[128];
            snprintf(msg, sizeof(msg),
                     "entry %d (U+%04X + '%c') is not after the one before it",
                     i, g_compose[i].mark, (char)g_compose[i].base);
            fail("the composition table is sorted", msg);
            return;
        }
    }
    ok("the composition table is sorted");
}

/* Everything a dead key can compose to must be drawable too. */
static void check_compose_glyphs(void) {
    for (int i = 0; i < COMPOSE_COUNT; i++) {
        const uint8_t *rows = font_glyph_rows(g_compose[i].result);
        if (rows == NULL || is_blank(rows)) {
            char msg[128];
            snprintf(msg, sizeof(msg), "U+%04X has no glyph",
                     g_compose[i].result);
            fail("every composed letter has a glyph", msg);
            return;
        }
    }
    ok("every composed letter has a glyph");
}

/* Composition must round-trip through the search it is stored for. */
static void check_compose_lookup(void) {
    for (int i = 0; i < COMPOSE_COUNT; i++) {
        if (keymap_compose(g_compose[i].mark, g_compose[i].base) !=
            g_compose[i].result) {
            fail("every composition is findable", "the search missed one");
            return;
        }
    }
    check(keymap_compose(MARK_ACUTE, 'x') == 0,
          "a pair that does not combine returns 0",
          "acute + x composed to something");
    ok("every composition is findable");
}

/* The Turkish i, the reason Caps Lock swaps levels instead of
 * upper-casing: there are two i's and they are different letters. */
static void check_turkish_i(void) {
    if (keymap_set("tr") != 0) {
        fail("Turkish Q loads", "keymap_set(\"tr\") failed");
        return;
    }
    /* 0x17 is the US 'i' key, 0x28 the US apostrophe key. */
    check(keymap_lookup(0x17, false, false, false) == 0x0131,
          "the US i key types a dotless i", "wrong codepoint");
    check(keymap_lookup(0x17, true, false, false) == 'I',
          "shifted, it types a plain capital I", "wrong codepoint");
    check(keymap_lookup(0x28, false, false, false) == 'i',
          "the apostrophe key types a dotted i", "wrong codepoint");
    check(keymap_lookup(0x28, true, false, false) == 0x0130,
          "shifted, it types a dotted capital I", "wrong codepoint");

    /* Caps Lock must do the same thing Shift does for letters... */
    check(keymap_lookup(0x17, false, false, true) == 'I',
          "Caps Lock on the dotless i key gives a plain I",
          "Caps Lock did not swap the level");
    check(keymap_lookup(0x28, false, false, true) == 0x0130,
          "Caps Lock on the dotted i key gives a dotted I",
          "Caps Lock did not swap the level");
    /* ...and Shift with Caps Lock must give the lowercase back. */
    check(keymap_lookup(0x17, true, false, true) == 0x0131,
          "Shift with Caps Lock gives the lowercase back",
          "the two did not cancel");
    /* ...and nothing at all to a digit. */
    check(keymap_lookup(0x02, false, false, true) == '1',
          "Caps Lock leaves the digit row alone",
          "Caps Lock shifted a digit");
}

/* A layout with no name, and a name with no layout. */
static void check_bad_names(void) {
    const char *before = keymap_name();
    check(keymap_set("klingon") != 0 && keymap_name() == before,
          "an unknown layout is refused and changes nothing",
          "keymap_set accepted a name it has no table for");
    check(keymap_set(NULL) != 0,
          "a NULL name is refused", "keymap_set(NULL) succeeded");
}

int main(void) {
    printf("== TUS keyboard layout tests ==\n");
    printf("-- glyph coverage --\n");
    check_layout_glyphs();
    printf("-- dead keys --\n");
    check_compose_sorted();
    check_compose_glyphs();
    check_compose_lookup();
    printf("-- the Turkish i --\n");
    check_turkish_i();
    printf("-- bad input --\n");
    check_bad_names();

    printf("== %d checks, %d failed ==\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
