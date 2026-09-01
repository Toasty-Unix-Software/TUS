/*
 * tusaudio.h - the /dev/dsp ioctl ABI
 *
 * Shared verbatim by the kernel (kernel/drivers/hda/hda.c) and by
 * userspace/wavplay.c, the same way include/tusvideo.h is shared with
 * res_set. There is no SYS_AUDIO: /dev/dsp is a plain device node -
 * open() it, write() raw interleaved 16-bit little-endian PCM at
 * 48000 Hz, stereo (the one format kernel/drivers/hda/hda.c programs the
 * codec for - see its top comment for why that is fixed rather than
 * negotiated), and use these two ioctls for flow control around the
 * ring buffer a write() streams into.
 */

#ifndef TUS_AUDIO_H
#define TUS_AUDIO_H

#define TUS_AUDIO_DRAIN 0x1 /* block until every written byte has actually played */
#define TUS_AUDIO_STOP  0x2 /* stop the stream and reset it for a fresh session */

#endif /* TUS_AUDIO_H */
