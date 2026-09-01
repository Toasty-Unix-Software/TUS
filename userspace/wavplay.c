/*
 * wavplay - play a RIFF/WAVE file through /dev/dsp
 *
 *     wavplay /tmp/song.wav
 *
 * All the WAV parsing/validation/format-conversion logic lives in
 * wav.c now (shared with userspace/hxwavplayer.c, the graphical
 * player, so the two tools cannot silently disagree on what counts as
 * a playable file) - this file is just the streaming loop.
 */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <tusaudio.h>

#include "wav.h"

#define CHUNK_FRAMES 4096 /* input frames per read; output is 4 bytes/frame */

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: wavplay <file.wav>\n");
        return 1;
    }

    struct wav_info info;
    int fd = wav_open("wavplay", argv[1], &info);
    if (fd < 0) {
        return 1;
    }

    int dsp = open("/dev/dsp", O_WRONLY);
    if (dsp < 0) {
        fprintf(stderr, "wavplay: cannot open /dev/dsp - no HD Audio codec found\n");
        close(fd);
        return 1;
    }

    uint32_t bytes_per_frame = wav_bytes_per_frame(&info);
    unsigned char in_buf[CHUNK_FRAMES * 4]; /* worst case: 16-bit stereo */
    int16_t out_buf[CHUNK_FRAMES * 2];      /* always 16-bit stereo out */

    uint32_t remaining = info.data_size;
    while (remaining >= bytes_per_frame) {
        size_t want_frames = CHUNK_FRAMES;
        if ((uint32_t)(want_frames * bytes_per_frame) > remaining) {
            want_frames = remaining / bytes_per_frame;
        }
        size_t want_bytes = want_frames * bytes_per_frame;
        size_t got = 0;
        while (got < want_bytes) {
            long n = read(fd, in_buf + got, want_bytes - got);
            if (n <= 0) {
                goto done;
            }
            got += (size_t)n;
        }
        remaining -= (uint32_t)want_bytes;

        wav_convert_frames(&info, in_buf, want_frames, out_buf);

        size_t out_bytes = want_frames * 4;
        size_t written = 0;
        while (written < out_bytes) {
            long n = write(dsp, (unsigned char *)out_buf + written, out_bytes - written);
            if (n <= 0) {
                fprintf(stderr, "wavplay: write to /dev/dsp failed\n");
                close(dsp);
                close(fd);
                return 1;
            }
            written += (size_t)n;
        }
    }

done:
    /* Wait for the ring to actually finish playing before stopping the
     * stream - stopping right after the last write() would cut off
     * whatever is still queued in hardware. */
    ioctl(dsp, TUS_AUDIO_DRAIN, NULL);
    ioctl(dsp, TUS_AUDIO_STOP, NULL);

    close(dsp);
    close(fd);
    printf("wavplay: played %s\n", argv[1]);
    return 0;
}
