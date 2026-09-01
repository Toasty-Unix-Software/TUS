/*
 * mp4.h - a minimal MP4 (ISO base media file format) demuxer
 *
 * Enough of ISO/IEC 14496-12 and -15 to play a video file: it walks
 * the box tree, finds the video track, reads its sample tables and
 * hands the caller one coded frame at a time, already converted from
 * MP4's length-prefixed NAL units to the Annex B byte stream an H.264
 * decoder expects.
 *
 * The demuxer is plain C with no dependency on highX, so the same
 * file is compiled into hxvideo (on TUS) and into the host-side test
 * that checks it against ffmpeg's output.
 */

#ifndef TUS_MP4_H
#define TUS_MP4_H

#include <stddef.h>

/* Error codes returned by mp4_open(). */
#define MP4_OK           0
#define MP4_E_OPEN      -1  /* cannot open the file */
#define MP4_E_FORMAT    -2  /* not an MP4 / box tree damaged */
#define MP4_E_NOVIDEO   -3  /* no video track */
#define MP4_E_CODEC     -4  /* video track is not H.264 (avc1) */
#define MP4_E_MEMORY    -5
#define MP4_E_TOOBIG    -6  /* sample tables larger than we accept */

#define MP4_MAX_PARAMS 1024 /* SPS/PPS, Annex B, from the avcC box */

struct mp4_sample {
    unsigned long offset;   /* byte offset in the file */
    unsigned      size;     /* bytes of coded data */
    unsigned      duration; /* in media timescale ticks */
    int           sync;     /* non-zero for a key frame */
};

struct mp4_video {
    int           fd;
    unsigned      width, height;   /* from the sample description */
    unsigned      timescale;       /* media ticks per second */
    unsigned long duration;        /* media duration in ticks */
    unsigned      nal_length_size; /* 1, 2 or 4 (from avcC) */
    unsigned      profile;         /* H.264 profile_idc (66 = baseline) */
    unsigned      level;           /* H.264 level_idc x10 */
    char          codec[5];        /* e.g. "avc1" */

    unsigned char params[MP4_MAX_PARAMS]; /* SPS/PPS as Annex B */
    unsigned      params_len;

    struct mp4_sample *samples;
    unsigned      sample_count;
    unsigned      max_sample_size;
};

/* Open `path` and read its tables. Returns MP4_OK or an MP4_E_* code. */
int mp4_open(struct mp4_video *v, const char *path);

/* Read sample `index` into `buf`, rewriting the NAL length prefixes
 * into Annex B start codes. Returns the number of bytes written, or a
 * negative value on error. */
long mp4_read_sample(struct mp4_video *v, unsigned index, unsigned char *buf,
                     unsigned buf_size);

/* Index of the last key frame at or before `index` (for seeking). */
unsigned mp4_prev_sync(const struct mp4_video *v, unsigned index);

/* Presentation time of sample `index`, in milliseconds. */
unsigned long mp4_sample_time_ms(const struct mp4_video *v, unsigned index);

void mp4_close(struct mp4_video *v);

/* Human-readable form of an MP4_E_* code. */
const char *mp4_strerror(int code);

#endif /* TUS_MP4_H */
