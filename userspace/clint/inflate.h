/*
 * inflate.h - DEFLATE decompression (RFC 1951), and the two wrappers
 * the web puts around it
 *
 * Clint needs this twice: PNG image data is a zlib stream, and a
 * server that is allowed to compress a page sends gzip. Both are the
 * same bit format with a different few bytes in front, so one
 * decompressor serves both.
 *
 * The output is grown as it is produced - a compressed stream does not
 * say how big it will be until it ends - and handed to the caller,
 * who frees it.
 */

#ifndef CLINT_INFLATE_H
#define CLINT_INFLATE_H

#include <stddef.h>

/* Each returns 0 with the output and its length filled in, or -1.
 * `limit` caps the output: a stream that would grow past it fails
 * rather than eating the machine. */
int inflate_raw(const void *in, size_t in_len, size_t limit, void **out,
                size_t *out_len);
int inflate_zlib(const void *in, size_t in_len, size_t limit, void **out,
                 size_t *out_len);
int inflate_gzip(const void *in, size_t in_len, size_t limit, void **out,
                 size_t *out_len);

const char *inflate_last_error(void);

#endif /* CLINT_INFLATE_H */
