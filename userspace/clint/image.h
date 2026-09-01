/*
 * image.h - decoded pictures, in the one format the canvas paints
 *
 * A page's images arrive as bytes in whatever the server chose to
 * send; everything above this line wants pixels. Clint decodes PNG,
 * which is what the web uses for logos, buttons and everything else
 * with a flat colour or an edge that has to stay sharp - and what a
 * decoder without floating point can do faithfully. A JPEG cannot be
 * decoded here yet, so it stays what it was before: alt text.
 *
 * Pixels are 0xAARRGGBB, the canvas's own layout, so painting is a
 * copy and a blend and never a conversion.
 */

#ifndef CLINT_IMAGE_H
#define CLINT_IMAGE_H

#include <stddef.h>
#include <stdint.h>

struct image {
    int w, h;
    uint32_t *px;    /* w*h, 0xAARRGGBB */
};

/* What `data` is, by its first bytes - so the browser can say "a JPEG
 * it cannot draw" instead of "something broken". */
enum { IMAGE_UNKNOWN = 0, IMAGE_PNG, IMAGE_GIF, IMAGE_JPEG, IMAGE_BMP };
int image_kind(const void *data, size_t len);
const char *image_kind_name(int kind);

/* Decode into `out`. Returns 0, or -1 with image_last_error() set. */
int image_decode(const void *data, size_t len, struct image *out);

void image_free(struct image *img);

const char *image_last_error(void);

#endif /* CLINT_IMAGE_H */
