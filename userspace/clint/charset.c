/*
 * charset.c - see charset.h
 *
 * One table per encoding, covering 0x80..0xFF: everything below that
 * is ASCII in all of them, which is why the tables are half the size
 * they look like they should be.
 *
 * The Latin-1 label is deliberately decoded as windows-1252. Pages
 * labelled ISO-8859-1 use the 0x80..0x9F range for curly quotes and
 * dashes constantly - the label is wrong on the wire far more often
 * than it is right, and every browser has resolved it this way for
 * twenty years.
 */

#include "charset.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char lower_ch(char c) {
    return (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c;
}

/* windows-1252: Latin-1 with printable characters where Latin-1 has
 * the C1 control block. */
static const uint16_t CP1252[128] = {
    0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
    0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F,
    0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178,
    0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
    0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
    0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7,
    0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
    0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
    0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7,
    0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
    0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7,
    0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF,
};

/* The differences from windows-1252, as (byte, codepoint) pairs -
 * these encodings are that table with a handful of letters changed,
 * and writing out the changes is both shorter and easier to check
 * against the standard than another 128 lines. */
struct patch { uint8_t byte; uint16_t cp; };

/* windows-1254, which is what a page labelled ISO-8859-9 means:
 * Turkish. */
static const struct patch TURKISH[] = {
    { 0xD0, 0x011E }, { 0xDD, 0x0130 }, { 0xDE, 0x015E },
    { 0xF0, 0x011F }, { 0xFD, 0x0131 }, { 0xFE, 0x015F },
    { 0, 0 }
};

/* ISO-8859-15: Latin-1 with the euro and a few letters French and
 * Finnish wanted. */
static const struct patch LATIN9[] = {
    { 0xA4, 0x20AC }, { 0xA6, 0x0160 }, { 0xA8, 0x0161 },
    { 0xB4, 0x017D }, { 0xB8, 0x017E }, { 0xBC, 0x0152 },
    { 0xBD, 0x0153 }, { 0xBE, 0x0178 },
    { 0, 0 }
};

/* ISO-8859-2: Central European. */
static const struct patch LATIN2[] = {
    { 0xA1, 0x0104 }, { 0xA2, 0x02D8 }, { 0xA3, 0x0141 }, { 0xA5, 0x013D },
    { 0xA6, 0x015A }, { 0xA9, 0x0160 }, { 0xAA, 0x015E }, { 0xAB, 0x0164 },
    { 0xAC, 0x0179 }, { 0xAE, 0x017D }, { 0xAF, 0x017B }, { 0xB1, 0x0105 },
    { 0xB2, 0x02DB }, { 0xB3, 0x0142 }, { 0xB5, 0x013E }, { 0xB6, 0x015B },
    { 0xB7, 0x02C7 }, { 0xB9, 0x0161 }, { 0xBA, 0x015F }, { 0xBB, 0x0165 },
    { 0xBC, 0x017A }, { 0xBD, 0x02DD }, { 0xBE, 0x017E }, { 0xBF, 0x017C },
    { 0xC0, 0x0154 }, { 0xC3, 0x0102 }, { 0xC5, 0x0139 }, { 0xC6, 0x0106 },
    { 0xC8, 0x010C }, { 0xCA, 0x0118 }, { 0xCC, 0x011A }, { 0xCF, 0x010E },
    { 0xD0, 0x0110 }, { 0xD1, 0x0143 }, { 0xD2, 0x0147 }, { 0xD5, 0x0150 },
    { 0xD8, 0x0158 }, { 0xD9, 0x016E }, { 0xDB, 0x0170 }, { 0xDE, 0x0162 },
    { 0xE0, 0x0155 }, { 0xE3, 0x0103 }, { 0xE5, 0x013A }, { 0xE6, 0x0107 },
    { 0xE8, 0x010D }, { 0xEA, 0x0119 }, { 0xEC, 0x011B }, { 0xEF, 0x010F },
    { 0xF0, 0x0111 }, { 0xF1, 0x0144 }, { 0xF2, 0x0148 }, { 0xF5, 0x0151 },
    { 0xF8, 0x0159 }, { 0xF9, 0x016F }, { 0xFB, 0x0171 }, { 0xFE, 0x0163 },
    { 0, 0 }
};

static int label_is(const char *label, const char *want) {
    while (*label != '\0' && *want != '\0') {
        if (lower_ch(*label) != *want) return 0;
        label++;
        want++;
    }
    return *label == '\0' && *want == '\0';
}

static const struct patch *table_for(const char *label) {
    if (label_is(label, "iso-8859-9") || label_is(label, "iso8859-9") ||
        label_is(label, "latin5") || label_is(label, "windows-1254") ||
        label_is(label, "cp1254")) {
        return TURKISH;
    }
    if (label_is(label, "iso-8859-15") || label_is(label, "latin9")) {
        return LATIN9;
    }
    if (label_is(label, "iso-8859-2") || label_is(label, "iso8859-2") ||
        label_is(label, "latin2") || label_is(label, "windows-1250") ||
        label_is(label, "cp1250")) {
        return LATIN2;
    }
    if (label_is(label, "iso-8859-1") || label_is(label, "iso8859-1") ||
        label_is(label, "latin1") || label_is(label, "windows-1252") ||
        label_is(label, "cp1252") || label_is(label, "us-ascii") ||
        label_is(label, "ascii")) {
        return NULL;   /* plain windows-1252, with nothing patched */
    }
    return (const struct patch *)-1;   /* not an encoding with a table */
}

/* ---- finding the label ---- */

/* The value of a charset= parameter, wherever it was written. */
static int charset_param(const char *text, size_t len, char *out,
                         size_t size) {
    static const char WORD[] = "charset";

    for (size_t i = 0; i + sizeof(WORD) <= len; i++) {
        int match = 1;
        for (size_t k = 0; k + 1 < sizeof(WORD); k++) {
            if (lower_ch(text[i + k]) != WORD[k]) {
                match = 0;
                break;
            }
        }
        if (!match) continue;

        size_t at = i + sizeof(WORD) - 1;
        while (at < len && (text[at] == ' ' || text[at] == '=' ||
                            text[at] == '"' || text[at] == '\'')) {
            at++;
        }
        size_t start = at;
        while (at < len && (text[at] == '-' || text[at] == '_' ||
                            (text[at] >= '0' && text[at] <= '9') ||
                            (lower_ch(text[at]) >= 'a' &&
                             lower_ch(text[at]) <= 'z'))) {
            at++;
        }
        if (at == start) continue;

        size_t n = at - start;
        if (n >= size) n = size - 1;
        for (size_t k = 0; k < n; k++) out[k] = lower_ch(text[start + k]);
        out[n] = '\0';
        return 1;
    }
    return 0;
}

void charset_of(const char *content_type, const char *body, size_t len,
                char *out, size_t size) {
    snprintf(out, size, "utf-8");

    if (content_type != NULL &&
        charset_param(content_type, strlen(content_type), out, size)) {
        return;
    }

    /* A document's own declaration has to be found before the
     * document is parsed, so it is looked for in the head-shaped part
     * of the bytes rather than in a tree that does not exist yet. */
    size_t window = len < 4096 ? len : 4096;
    for (size_t i = 0; i + 5 < window; i++) {
        if (body[i] != '<' || lower_ch(body[i + 1]) != 'm' ||
            lower_ch(body[i + 2]) != 'e' || lower_ch(body[i + 3]) != 't' ||
            lower_ch(body[i + 4]) != 'a') {
            continue;
        }
        size_t end = i;
        while (end < window && body[end] != '>') end++;
        if (charset_param(body + i, end - i, out, size)) return;
        i = end;
    }
}

char *charset_to_utf8(const char *label, const char *in, size_t len,
                      size_t *out_len) {
    const struct patch *patch = table_for(label);
    if (patch == (const struct patch *)-1) return NULL;

    uint16_t map[128];
    memcpy(map, CP1252, sizeof(map));
    for (int i = 0; patch != NULL && patch[i].byte != 0; i++) {
        map[patch[i].byte - 0x80] = patch[i].cp;
    }

    /* Nothing above ASCII means nothing to convert. */
    size_t high = 0;
    for (size_t i = 0; i < len; i++) {
        if ((unsigned char)in[i] >= 0x80) high++;
    }
    if (high == 0) return NULL;

    char *out = malloc(len + high * 2 + 1);
    if (out == NULL) return NULL;

    size_t at = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)in[i];
        if (c < 0x80) {
            out[at++] = (char)c;
            continue;
        }

        uint32_t cp = map[c - 0x80];
        if (cp < 0x800) {
            out[at++] = (char)(0xC0 | (cp >> 6));
            out[at++] = (char)(0x80 | (cp & 0x3F));
        } else {
            out[at++] = (char)(0xE0 | (cp >> 12));
            out[at++] = (char)(0x80 | ((cp >> 6) & 0x3F));
            out[at++] = (char)(0x80 | (cp & 0x3F));
        }
    }
    out[at] = '\0';
    *out_len = at;
    return out;
}
