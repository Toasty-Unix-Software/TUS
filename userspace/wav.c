/*
 * wav.c - see wav.h
 */

#include "wav.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static uint32_t rd32le(const unsigned char *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint16_t rd16le(const unsigned char *p) {
    return (uint16_t)(p[0] | (p[1] << 8));
}

/* Reads exactly `len` bytes or reports why it could not. */
static int read_full(int fd, void *buf, size_t len) {
    size_t got = 0;
    while (got < len) {
        long n = read(fd, (unsigned char *)buf + got, len - got);
        if (n <= 0) {
            return -1;
        }
        got += (size_t)n;
    }
    return 0;
}

int wav_open(const char *prog, const char *path, struct wav_info *info) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "%s: cannot open %s\n", prog, path);
        return -1;
    }

    unsigned char hdr[12];
    if (read_full(fd, hdr, 12) < 0 ||
        memcmp(hdr, "RIFF", 4) != 0 || memcmp(hdr + 8, "WAVE", 4) != 0) {
        fprintf(stderr, "%s: %s is not a RIFF/WAVE file\n", prog, path);
        close(fd);
        return -1;
    }

    uint16_t format_tag = 0, channels = 0, bits = 0;
    uint32_t sample_rate = 0;
    long data_offset = -1;
    uint32_t data_size = 0;
    int have_fmt = 0;

    /* Walk chunks until both fmt and data are found, or the file runs
     * out. Chunk bodies are word-aligned (a trailing pad byte on an
     * odd-sized chunk), same as every other RIFF-family format. */
    for (;;) {
        unsigned char chdr[8];
        if (read_full(fd, chdr, 8) < 0) {
            break;
        }
        uint32_t csize = rd32le(chdr + 4);

        if (memcmp(chdr, "fmt ", 4) == 0) {
            unsigned char fmt[16];
            if (csize < 16 || read_full(fd, fmt, 16) < 0) {
                fprintf(stderr, "%s: truncated fmt chunk\n", prog);
                close(fd);
                return -1;
            }
            format_tag = rd16le(fmt + 0);
            channels = rd16le(fmt + 2);
            sample_rate = rd32le(fmt + 4);
            bits = rd16le(fmt + 14);
            have_fmt = 1;
            if (lseek(fd, (long)csize - 16 + (long)(csize & 1), SEEK_CUR) < 0) {
                break;
            }
        } else if (memcmp(chdr, "data", 4) == 0) {
            data_offset = lseek(fd, 0, SEEK_CUR);
            data_size = csize;
            /* Keep scanning: some writers put LIST/other chunks after
             * data, and we only need fmt to already have been seen. */
            if (lseek(fd, (long)csize + (long)(csize & 1), SEEK_CUR) < 0) {
                break;
            }
        } else {
            if (lseek(fd, (long)csize + (long)(csize & 1), SEEK_CUR) < 0) {
                break;
            }
        }

        if (have_fmt && data_offset >= 0) {
            break;
        }
    }

    if (!have_fmt || data_offset < 0) {
        fprintf(stderr, "%s: %s has no fmt/data chunk\n", prog, path);
        close(fd);
        return -1;
    }
    if (format_tag != WAVE_FORMAT_PCM) {
        fprintf(stderr,
                "%s: format tag %u is not PCM - compressed WAV "
                "(ADPCM, float, mp3-in-wav, ...) is not supported\n",
                prog, (unsigned)format_tag);
        close(fd);
        return -1;
    }
    if (bits != 8 && bits != 16) {
        fprintf(stderr, "%s: %u-bit samples are not supported (only 8 or 16)\n",
                prog, (unsigned)bits);
        close(fd);
        return -1;
    }
    if (channels != 1 && channels != 2) {
        fprintf(stderr, "%s: %u channels is not supported (only 1 or 2)\n",
                prog, (unsigned)channels);
        close(fd);
        return -1;
    }
    if (sample_rate != 48000) {
        fprintf(stderr,
                "%s: %u Hz is not supported - /dev/dsp is fixed at "
                "48000 Hz and this tool does not resample\n",
                prog, (unsigned)sample_rate);
        close(fd);
        return -1;
    }

    if (lseek(fd, data_offset, SEEK_SET) < 0) {
        fprintf(stderr, "%s: cannot seek to the data chunk\n", prog);
        close(fd);
        return -1;
    }

    info->channels = channels;
    info->bits = bits;
    info->sample_rate = sample_rate;
    info->data_offset = data_offset;
    info->data_size = data_size;
    return fd;
}

void wav_convert_frames(const struct wav_info *info, const unsigned char *in,
                        size_t frames, int16_t *out) {
    uint32_t bpf = wav_bytes_per_frame(info);
    for (size_t f = 0; f < frames; f++) {
        int16_t left, right;
        if (info->bits == 16) {
            const unsigned char *p = in + f * bpf;
            left = (int16_t)rd16le(p);
            right = (info->channels == 2) ? (int16_t)rd16le(p + 2) : left;
        } else {
            const unsigned char *p = in + f * bpf;
            left = (int16_t)(((int)p[0] - 128) << 8);
            right = (info->channels == 2) ? (int16_t)(((int)p[1] - 128) << 8) : left;
        }
        out[f * 2] = left;
        out[f * 2 + 1] = right;
    }
}
