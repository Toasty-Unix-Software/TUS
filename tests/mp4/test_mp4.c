/*
 * test_mp4.c - host test for the MP4 demuxer and the H.264 decoder
 *
 * Builds userspace/mp4.c and the vendored h264bsd library on the
 * build host, demuxes a real MP4 file and decodes it, then checks the
 * result against what the file claims and (optionally) writes the
 * frames out as PPM images so a human can look at them.
 *
 * This is the same code path hxvideo runs on TUS - only the display
 * differs - so a failure here is a failure there.
 *
 *   ./test_mp4 <file.mp4> [ppm-prefix]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../userspace/mp4.h"
#include "h264bsd_decoder.h"
#include "h264bsd_util.h"

static int save_ppm(const char *prefix, int index, const unsigned int *rgba,
                    unsigned w, unsigned h) {
    char path[256];
    snprintf(path, sizeof(path), "%s%03d.ppm", prefix, index);
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return -1;
    }
    fprintf(f, "P6\n%u %u\n255\n", w, h);
    for (unsigned i = 0; i < w * h; i++) {
        unsigned int px = rgba[i];
        unsigned char rgb[3] = { (unsigned char)(px & 0xFF),
                                 (unsigned char)((px >> 8) & 0xFF),
                                 (unsigned char)((px >> 16) & 0xFF) };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file.mp4> [ppm-prefix]\n", argv[0]);
        return 2;
    }
    const char *prefix = argc > 2 ? argv[2] : NULL;
    int failures = 0;

    struct mp4_video v;
    int rc = mp4_open(&v, argv[1]);
    if (rc != MP4_OK) {
        fprintf(stderr, "  [FAIL] mp4_open: %s\n", mp4_strerror(rc));
        return 1;
    }
    printf("  [PASS] demuxed %s: %s %ux%u, %u samples, timescale %u\n",
           argv[1], v.codec, v.width, v.height, v.sample_count, v.timescale);
    printf("         H.264 profile %u, level %u.%u\n", v.profile,
           v.level / 10, v.level % 10);
    if (v.profile != 66) {
        /* h264bsd is a baseline decoder; anything else is expected to
         * be refused, and hxvideo says so in its window. */
        printf("  [SKIP] not a baseline file: the decoder will refuse it, "
               "which is the documented behaviour\n");
        mp4_close(&v);
        return 0;
    }

    if (v.params_len == 0) {
        printf("  [FAIL] no SPS/PPS in avcC\n");
        failures++;
    }
    if (v.sample_count == 0 || v.width == 0 || v.height == 0) {
        printf("  [FAIL] empty sample table or unknown picture size\n");
        return 1;
    }

    unsigned syncs = 0;
    unsigned long ticks = 0;
    for (unsigned i = 0; i < v.sample_count; i++) {
        syncs += v.samples[i].sync ? 1 : 0;
        ticks += v.samples[i].duration;
    }
    printf("  [PASS] %u key frames, %.2f s of media\n", syncs,
           (double)ticks / (double)v.timescale);
    if (syncs == 0) {
        printf("  [FAIL] no key frame: the video could never start\n");
        failures++;
    }

    storage_t dec;
    if (h264bsdInit(&dec, HANTRO_FALSE) != HANTRO_OK) {
        printf("  [FAIL] h264bsdInit\n");
        return 1;
    }

    /* The parameter sets from avcC come first, exactly as hxvideo
     * feeds them. */
    unsigned char *buf = malloc(v.max_sample_size + 64);
    u32 read_bytes;
    u8 *strm = v.params;
    u32 len = v.params_len;
    while (len > 0) {
        u32 result = h264bsdDecode(&dec, strm, len, 0, &read_bytes);
        if (result == H264BSD_ERROR || result == H264BSD_PARAM_SET_ERROR) {
            printf("  [FAIL] decoder rejected the parameter sets\n");
            return 1;
        }
        strm += read_bytes;
        len -= read_bytes;
    }
    printf("  [PASS] decoder accepted the SPS/PPS from avcC\n");

    unsigned decoded = 0, nonblack = 0;
    for (unsigned i = 0; i < v.sample_count; i++) {
        long got = mp4_read_sample(&v, i, buf, v.max_sample_size + 64);
        if (got <= 0) {
            printf("  [FAIL] sample %u unreadable\n", i);
            failures++;
            break;
        }
        strm = buf;
        len = (u32)got;
        while (len > 0) {
            u32 result = h264bsdDecode(&dec, strm, len, 0, &read_bytes);
            strm += read_bytes;
            len -= read_bytes;
            if (result == H264BSD_PIC_RDY) {
                u32 pic_id, is_idr, err_mbs;
                u32 *rgba = h264bsdNextOutputPictureRGBA(&dec, &pic_id, &is_idr,
                                                         &err_mbs);
                if (rgba == NULL) {
                    continue;
                }
                decoded++;
                unsigned w = h264bsdPicWidth(&dec) * 16;
                unsigned h = h264bsdPicHeight(&dec) * 16;
                for (unsigned px = 0; px < w * h; px++) {
                    if ((rgba[px] & 0x00FFFFFF) != 0) {
                        nonblack++;
                        break;
                    }
                }
                if (prefix != NULL && decoded <= 5) {
                    save_ppm(prefix, (int)decoded, rgba, w, h);
                }
            } else if (result == H264BSD_ERROR ||
                       result == H264BSD_PARAM_SET_ERROR) {
                printf("  [FAIL] decode error in sample %u\n", i);
                failures++;
                len = 0;
            }
        }
    }
    /* Seeking: an H.264 key frame resets the decoder by itself, so
     * jumping back to one and decoding on must produce pictures
     * again. (Re-initialising the decoder instead does not work -
     * h264bsdInit() after h264bsdShutdown() rejects the parameter
     * sets - which is why hxvideo never does that.) */
    unsigned mid = v.sample_count / 2;
    unsigned sync = mp4_prev_sync(&v, mid);
    unsigned after_seek = 0;
    for (unsigned i = sync; i < sync + 10 && i < v.sample_count; i++) {
        long got = mp4_read_sample(&v, i, buf, v.max_sample_size + 64);
        if (got <= 0) {
            break;
        }
        strm = buf;
        len = (u32)got;
        while (len > 0) {
            u32 result = h264bsdDecode(&dec, strm, len, 0, &read_bytes);
            strm += read_bytes;
            len -= read_bytes;
            if (result == H264BSD_PIC_RDY) {
                u32 pic_id, is_idr, err_mbs;
                if (h264bsdNextOutputPictureBGRA(&dec, &pic_id, &is_idr,
                                                 &err_mbs) != NULL) {
                    after_seek++;
                }
            } else if (result == H264BSD_ERROR ||
                       result == H264BSD_PARAM_SET_ERROR) {
                len = 0;
            }
        }
    }
    printf("  [%s] seeking to key frame %u decodes again (%u pictures)\n",
           after_seek > 0 ? "PASS" : "FAIL", sync, after_seek);
    if (after_seek == 0) {
        failures++;
    }

    h264bsdShutdown(&dec);
    free(buf);

    printf("  [%s] decoded %u of %u frames\n",
           decoded >= v.sample_count - 1 ? "PASS" : "FAIL", decoded,
           v.sample_count);
    if (decoded < v.sample_count - 1) {
        failures++;
    }
    printf("  [%s] %u frames carry a picture\n", nonblack > 0 ? "PASS" : "FAIL",
           nonblack);
    if (nonblack == 0) {
        failures++;
    }

    mp4_close(&v);
    printf("== %s ==\n", failures == 0 ? "mp4 + h264 tests passed"
                                       : "mp4 + h264 tests FAILED");
    return failures == 0 ? 0 : 1;
}
