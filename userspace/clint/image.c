/*
 * image.c - see image.h
 *
 * PNG, read the way the specification is written: a signature, then
 * chunks. Only four matter here - IHDR for the shape, PLTE and tRNS
 * for the colours a palette image refers to, and IDAT for one zlib
 * stream split across however many chunks the encoder felt like.
 *
 * The part that is easy to get wrong is the filtering. Every scanline
 * is prefixed with a filter type, and the reconstruction refers to
 * the pixel to the left and the one above - in *bytes*, offset by the
 * pixel size, and after the row above has itself been reconstructed.
 * So the rows are undone in place, in order, and the previous row is
 * the one already fixed.
 *
 * Interlaced (Adam7) images are refused rather than half-drawn: they
 * are seven small images in a trench coat, and no page Clint is meant
 * for uses them.
 */

#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inflate.h"

#define IMAGE_MAX_DIM    8192
#define IMAGE_MAX_PIXELS (16u * 1024 * 1024)
#define IMAGE_MAX_RAW    (64u * 1024 * 1024)

static char g_error[128] = "no error";

static int fail(const char *why) {
    size_t n = strlen(why);
    if (n >= sizeof(g_error)) n = sizeof(g_error) - 1;
    memcpy(g_error, why, n);
    g_error[n] = '\0';
    return -1;
}

const char *image_last_error(void) { return g_error; }

void image_free(struct image *img) {
    if (img == NULL) return;
    free(img->px);
    img->px = NULL;
    img->w = img->h = 0;
}

int image_kind(const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    if (len >= 8 && memcmp(p, "\x89PNG\r\n\x1a\n", 8) == 0) return IMAGE_PNG;
    if (len >= 6 && memcmp(p, "GIF8", 4) == 0) return IMAGE_GIF;
    if (len >= 3 && p[0] == 0xFF && p[1] == 0xD8 && p[2] == 0xFF) {
        return IMAGE_JPEG;
    }
    if (len >= 2 && p[0] == 'B' && p[1] == 'M') return IMAGE_BMP;
    return IMAGE_UNKNOWN;
}

const char *image_kind_name(int kind) {
    switch (kind) {
    case IMAGE_PNG:  return "PNG";
    case IMAGE_GIF:  return "GIF";
    case IMAGE_JPEG: return "JPEG";
    case IMAGE_BMP:  return "BMP";
    default:         return "image";
    }
}

/* ---- PNG ---- */

static uint32_t be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static int channels_for(int color_type) {
    switch (color_type) {
    case 0: return 1;   /* grey */
    case 2: return 3;   /* truecolour */
    case 3: return 1;   /* palette index */
    case 4: return 2;   /* grey + alpha */
    case 6: return 4;   /* truecolour + alpha */
    default: return 0;
    }
}

static int paeth(int a, int b, int c) {
    int p = a + b - c;
    int pa = p > a ? p - a : a - p;
    int pb = p > b ? p - b : b - p;
    int pc = p > c ? p - c : c - p;
    if (pa <= pb && pa <= pc) return a;
    return pb <= pc ? b : c;
}

/* Undo the per-scanline filters, in place, dropping the filter byte
 * so what is left is `h` rows of `stride` bytes back to back. */
static int unfilter(uint8_t *raw, size_t raw_len, int h, size_t stride,
                    int bpp) {
    if (raw_len < (size_t)h * (stride + 1)) return fail("PNG data is short");

    uint8_t *prev = NULL;
    for (int y = 0; y < h; y++) {
        uint8_t *line = raw + (size_t)y * (stride + 1);
        int filter = line[0];
        uint8_t *cur = line + 1;

        for (size_t i = 0; i < stride; i++) {
            int a = i >= (size_t)bpp ? cur[i - bpp] : 0;
            int b = prev != NULL ? prev[i] : 0;
            int c = (prev != NULL && i >= (size_t)bpp) ? prev[i - bpp] : 0;
            int x = cur[i];

            switch (filter) {
            case 0: break;
            case 1: x += a; break;
            case 2: x += b; break;
            case 3: x += (a + b) / 2; break;
            case 4: x += paeth(a, b, c); break;
            default: return fail("unknown PNG filter");
            }
            cur[i] = (uint8_t)x;
        }

        /* Shift the row down over the filter bytes already consumed,
         * so the caller sees a plain image. */
        memmove(raw + (size_t)y * stride, cur, stride);
        prev = raw + (size_t)y * stride;
    }
    return 0;
}

/* One sample out of a packed row, scaled to 0..255. */
static int sample(const uint8_t *row, int index, int depth) {
    switch (depth) {
    case 8:  return row[index];
    case 16: return row[index * 2];   /* the low byte is precision we lack */
    case 4:  return ((row[index / 2] >> (index % 2 ? 0 : 4)) & 0x0F) * 17;
    case 2:  return ((row[index / 4] >> (6 - 2 * (index % 4))) & 0x03) * 85;
    case 1:  return ((row[index / 8] >> (7 - (index % 8))) & 0x01) * 255;
    default: return 0;
    }
}

/* The raw index, unscaled - what a palette lookup needs. */
static int sample_raw(const uint8_t *row, int index, int depth) {
    switch (depth) {
    case 8:  return row[index];
    case 16: return row[index * 2];
    case 4:  return (row[index / 2] >> (index % 2 ? 0 : 4)) & 0x0F;
    case 2:  return (row[index / 4] >> (6 - 2 * (index % 4))) & 0x03;
    case 1:  return (row[index / 8] >> (7 - (index % 8))) & 0x01;
    default: return 0;
    }
}

static int decode_png(const uint8_t *data, size_t len, struct image *out) {
    if (len < 8 + 25) return fail("truncated PNG");

    int width = 0, height = 0, depth = 0, color_type = 0, interlace = 0;
    uint8_t palette[256][3];
    uint8_t alpha[256];
    int palette_len = 0;
    memset(alpha, 0xFF, sizeof(alpha));

    /* tRNS for the two colour types that use a single transparent
     * value rather than a table. */
    int trans_grey = -1, trans_r = -1, trans_g = -1, trans_b = -1;

    uint8_t *idat = NULL;
    size_t idat_len = 0, idat_cap = 0;

    size_t at = 8;
    int seen_ihdr = 0, done = 0;
    while (at + 8 <= len && !done) {
        uint32_t clen = be32(data + at);
        const uint8_t *type = data + at + 4;
        const uint8_t *body = data + at + 8;
        if (clen > len || at + 12 + clen > len) {
            free(idat);
            return fail("PNG chunk runs past the end");
        }

        if (memcmp(type, "IHDR", 4) == 0 && clen >= 13) {
            width = (int)be32(body);
            height = (int)be32(body + 4);
            depth = body[8];
            color_type = body[9];
            interlace = body[12];
            seen_ihdr = 1;

            if (width <= 0 || height <= 0 || width > IMAGE_MAX_DIM ||
                height > IMAGE_MAX_DIM ||
                (uint32_t)width * (uint32_t)height > IMAGE_MAX_PIXELS) {
                free(idat);
                return fail("PNG is too large to draw");
            }
            if (channels_for(color_type) == 0) {
                free(idat);
                return fail("unsupported PNG colour type");
            }
            if (depth != 1 && depth != 2 && depth != 4 && depth != 8 &&
                depth != 16) {
                free(idat);
                return fail("unsupported PNG bit depth");
            }
            if (interlace != 0) {
                free(idat);
                return fail("interlaced PNG");
            }
        } else if (memcmp(type, "PLTE", 4) == 0) {
            palette_len = (int)(clen / 3);
            if (palette_len > 256) palette_len = 256;
            for (int i = 0; i < palette_len; i++) {
                palette[i][0] = body[i * 3];
                palette[i][1] = body[i * 3 + 1];
                palette[i][2] = body[i * 3 + 2];
            }
        } else if (memcmp(type, "tRNS", 4) == 0) {
            if (color_type == 3) {
                for (uint32_t i = 0; i < clen && i < 256; i++) {
                    alpha[i] = body[i];
                }
            } else if (color_type == 0 && clen >= 2) {
                trans_grey = body[1];
            } else if (color_type == 2 && clen >= 6) {
                trans_r = body[1];
                trans_g = body[3];
                trans_b = body[5];
            }
        } else if (memcmp(type, "IDAT", 4) == 0) {
            if (idat_len + clen > idat_cap) {
                size_t want = idat_cap ? idat_cap * 2 : 16384;
                while (want < idat_len + clen) want *= 2;
                uint8_t *p = realloc(idat, want);
                if (p == NULL) {
                    free(idat);
                    return fail("out of memory");
                }
                idat = p;
                idat_cap = want;
            }
            memcpy(idat + idat_len, body, clen);
            idat_len += clen;
        } else if (memcmp(type, "IEND", 4) == 0) {
            done = 1;
        }

        at += 12 + clen;   /* length, type, body, CRC */
    }

    if (!seen_ihdr || idat_len == 0) {
        free(idat);
        return fail("PNG has no image data");
    }
    if (color_type == 3 && palette_len == 0) {
        free(idat);
        return fail("PNG indexes a palette it does not have");
    }

    int chans = channels_for(color_type);
    size_t stride = ((size_t)width * (size_t)chans * (size_t)depth + 7) / 8;
    int bpp = (chans * depth + 7) / 8;

    void *raw = NULL;
    size_t raw_len = 0;
    size_t need = (stride + 1) * (size_t)height;
    if (need > IMAGE_MAX_RAW) {
        free(idat);
        return fail("PNG is too large to draw");
    }
    if (inflate_zlib(idat, idat_len, need + 64, &raw, &raw_len) != 0) {
        free(idat);
        return fail(inflate_last_error());
    }
    free(idat);

    if (unfilter((uint8_t *)raw, raw_len, height, stride, bpp) != 0) {
        free(raw);
        return -1;
    }

    uint32_t *px = malloc((size_t)width * (size_t)height * sizeof(uint32_t));
    if (px == NULL) {
        free(raw);
        return fail("out of memory");
    }

    for (int y = 0; y < height; y++) {
        const uint8_t *row = (const uint8_t *)raw + (size_t)y * stride;
        uint32_t *dst = px + (size_t)y * (size_t)width;

        for (int x = 0; x < width; x++) {
            int r = 0, g = 0, b = 0, a = 255;

            switch (color_type) {
            case 0:
                r = g = b = sample(row, x, depth);
                if (trans_grey >= 0 && sample_raw(row, x, depth) == trans_grey) {
                    a = 0;
                }
                break;
            case 2:
                r = sample(row, x * 3 + 0, depth);
                g = sample(row, x * 3 + 1, depth);
                b = sample(row, x * 3 + 2, depth);
                if (trans_r >= 0 && r == trans_r && g == trans_g &&
                    b == trans_b) {
                    a = 0;
                }
                break;
            case 3: {
                int index = sample_raw(row, x, depth);
                if (index >= palette_len) index = 0;
                r = palette[index][0];
                g = palette[index][1];
                b = palette[index][2];
                a = alpha[index];
                break;
            }
            case 4:
                r = g = b = sample(row, x * 2 + 0, depth);
                a = sample(row, x * 2 + 1, depth);
                break;
            default:
                r = sample(row, x * 4 + 0, depth);
                g = sample(row, x * 4 + 1, depth);
                b = sample(row, x * 4 + 2, depth);
                a = sample(row, x * 4 + 3, depth);
                break;
            }

            dst[x] = ((uint32_t)a << 24) | ((uint32_t)r << 16) |
                     ((uint32_t)g << 8) | (uint32_t)b;
        }
    }

    free(raw);
    out->w = width;
    out->h = height;
    out->px = px;
    return 0;
}

int image_decode(const void *data, size_t len, struct image *out) {
    memset(out, 0, sizeof(*out));

    int kind = image_kind(data, len);
    if (kind == IMAGE_PNG) return decode_png((const uint8_t *)data, len, out);

    if (kind == IMAGE_UNKNOWN) return fail("not an image");
    {
        char why[64];
        snprintf(why, sizeof(why), "%s images are not decoded",
                 image_kind_name(kind));
        return fail(why);
    }
}
