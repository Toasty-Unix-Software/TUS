/*
 * wav.h - RIFF/WAVE parsing shared by wavplay.c and hxwavplayer.c
 *
 * Both tools have to agree on exactly what /dev/dsp can play (see
 * kernel/drivers/hda/hda.c's top comment: 16-bit, 48000 Hz, stereo, fixed
 * at bring-up, never renegotiated) - one parser, one set of
 * validation rules, so the CLI and GUI players cannot silently drift
 * apart on what counts as a playable file.
 */

#ifndef TUS_WAV_H
#define TUS_WAV_H

#include <stddef.h>
#include <stdint.h>

#define WAVE_FORMAT_PCM 1

struct wav_info {
    uint16_t channels;   /* 1 or 2 */
    uint16_t bits;       /* 8 or 16 */
    uint32_t sample_rate;
    long     data_offset; /* file offset of the first sample byte */
    uint32_t data_size;   /* bytes in the data chunk */
};

/* Bytes of INPUT (source-file) audio per sample frame - one sample
 * per channel, not yet upconverted to /dev/dsp's fixed 16-bit stereo
 * shape. */
static inline uint32_t wav_bytes_per_frame(const struct wav_info *w) {
    return (uint32_t)(w->bits / 8) * w->channels;
}

/* Opens `path`, walks its RIFF/WAVE chunks (fmt + data; anything else
 * is skipped) and validates the result against what /dev/dsp requires:
 * PCM, 8 or 16-bit, mono or stereo, 48000 Hz. On success returns an
 * open read-only fd already seeked to the start of the data chunk and
 * fills `info`; the fd is the caller's to close. On failure returns -1
 * and has already printed "<prog>: <reason>\n" to stderr - the caller
 * has nothing further to report. */
int wav_open(const char *prog, const char *path, struct wav_info *info);

/* Converts `frames` input frames (raw bytes from the data chunk, as
 * described by `info`) into /dev/dsp's fixed 16-bit-stereo shape,
 * writing 4 bytes per frame to `out`. `in` must hold
 * frames * wav_bytes_per_frame(info) bytes; `out` must hold
 * frames * 4 bytes. Handles 8->16 bit upconversion and mono->stereo
 * duplication - the only two shape differences /dev/dsp ever needs
 * bridged, since sample rate mismatches are refused outright by
 * wav_open() rather than resampled. */
void wav_convert_frames(const struct wav_info *info, const unsigned char *in,
                        size_t frames, int16_t *out);

#endif /* TUS_WAV_H */
