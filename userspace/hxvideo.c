/*
 * hxvideo - an MP4 video player for the highX window system
 *
 * Three pieces, none of which know about each other:
 *
 *   userspace/mp4.c        demuxes the container and hands over one
 *                          coded frame at a time (Annex B)
 *   sources/h264bsd        decodes H.264 baseline into pictures
 *   highAPI                puts the pictures on screen
 *
 * The player owns the loop between them: it feeds the decoder, scales
 * the picture to whatever size the window manager gave the window and
 * uploads it with one HX_OP_PUT_IMAGE request per frame. Playback is
 * paced against the sample durations in the file; when decoding
 * cannot keep up (TUS has no video acceleration and QEMU has no
 * mercy) the player simply runs as fast as it can and says so in the
 * status line.
 *
 *   hxvideo [file.mp4]     (default: /video/sample.mp4)
 *
 *   Space  pause / resume      Left / Right  seek 5 s
 *   R      restart             Q or Escape   quit
 */

#include "highapi/highapi.h"
#include "mp4.h"

#include "h264bsd_decoder.h"
#include "h264bsd_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COL_BG      0x00101418u
#define COL_BAR     0x001B2838u
#define COL_TEXT    0x00D8E4F0u
#define COL_DIM     0x008494A4u
#define COL_ACCENT  0x004FA3D1u

#define BAR_H 20
#define SEEK_MS 5000

/* SYS_UPTIME (kernel/syscall/syscall.h): milliseconds since boot.
 * musl's clocks only offer whole seconds on TUS, and a video player
 * needs better than that to pace frames. */
#define SYS_UPTIME 7

static long tus_syscall(long n, long a1, long a2, long a3) {
    long ret;
    /* The trap returns with only RAX preserved (see the ABI note in
     * musl's tus_syscall.c), so every argument register is declared
     * read-write - including the three this ABI does not use. Leaving
     * them out lets the compiler keep a live value in r8 across the
     * call, which it does: that is how closing a window turned into a
     * page fault at a garbage address. */
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"(n)
                     : "rcx", "r11", "memory");
    return ret;
}

static unsigned long uptime_ms(void) {
    return (unsigned long)tus_syscall(SYS_UPTIME, 0, 0, 0);
}

/* ---- player state ---- */

static struct hx_display dpy;
static struct mp4_video  video;
static storage_t         dec;
static unsigned int      win;
static unsigned int      win_w, win_h;

static unsigned char *g_sample;      /* one coded frame */
static unsigned int  *g_scaled;      /* the picture, scaled for the window */
static unsigned       g_scaled_cap;
/* The last decoded picture, kept so a resize, an expose or the end of
 * the file can repaint the window without decoding anything. */
static unsigned int  *g_last_pic;
static unsigned       g_last_cap, g_last_w, g_last_h;

static unsigned g_index;             /* next sample to decode */
static unsigned g_shown;             /* pictures put on screen */
static int      g_paused;
static int      g_eof;
static unsigned long g_start_ms;     /* wall clock of playback position 0 */
static unsigned long g_play_start_ms; /* when playback began */
static unsigned long g_fps_mark;     /* start of the current fps window */
static unsigned      g_fps_frames;   /* frames shown inside it */
static unsigned      g_fps;          /* frames per second, last full window */
static unsigned long g_media_ms;     /* position of the last shown frame */
static unsigned long g_decode_ms;    /* time spent in the decoder */

/* ---- drawing ---- */

static const char *basename_of(const char *path) {
    const char *slash = strrchr(path, '/');
    return slash != NULL ? slash + 1 : path;
}

static void draw_status(const char *path) {
    char line[HX_TEXT_MAX];
    int y = (int)win_h - BAR_H;

    hx_fill(win, 0, y, win_w, BAR_H, COL_BAR);
    hx_fill(win, 0, y, win_w, 1, COL_ACCENT);

    /* Progress bar across the top edge of the status line. */
    unsigned long total = video.timescale
                              ? video.duration * 1000ul / video.timescale
                              : 0;
    if (total > 0) {
        unsigned bar = (unsigned)((unsigned long)win_w * g_media_ms / total);
        if (bar > win_w) {
            bar = win_w;
        }
        hx_fill(win, 0, y + 1, bar, 3, COL_ACCENT);
    }

    /* Frames per second actually reaching the screen, which is what a
     * viewer cares about - decoding, scaling and compositing all
     * included. */
    unsigned fps = g_fps;

    snprintf(line, sizeof(line), "%s  %ux%u  %lu.%lu/%lu.%lus  %u/%u  %u fps  %s",
             basename_of(path), video.width, video.height, g_media_ms / 1000,
             (g_media_ms % 1000) / 100, total / 1000, (total % 1000) / 100,
             g_shown, video.sample_count, fps,
             g_paused ? "PAUSED" : (g_eof ? "END - R restarts" : "playing"));
    hx_text(win, 6, y + 4, COL_TEXT, COL_BAR, 0, line);

    /* The key hints only appear when they cannot collide with the
     * status text. */
    const char *hint = "Space pause  <- -> seek  R restart  Q quit";
    int used = 6 + (int)strlen(line) * HX_FONT_W + 16;
    int x = (int)win_w - (int)strlen(hint) * HX_FONT_W - 6;
    if (x > used) {
        hx_text(win, x, y + 4, COL_DIM, COL_BAR, 0, hint);
    }
    hx_commit_rect(win, 0, y, win_w, BAR_H);
}

/* Nearest-neighbour scale-to-fit, integer factors only: video pixels
 * stay square and every output pixel comes from exactly one source
 * pixel, which is the cheapest thing that still looks right. */
static void blit_last(void) {
    const unsigned int *pic = g_last_pic;
    unsigned pw = g_last_w, ph = g_last_h;
    unsigned area_h = win_h > BAR_H ? win_h - BAR_H : win_h;

    if (pic == NULL || pw == 0 || ph == 0 || win_w == 0 || area_h == 0) {
        return;
    }

    unsigned scale = win_w / pw;
    unsigned scale_y = area_h / ph;
    if (scale_y < scale) {
        scale = scale_y;
    }
    if (scale == 0) {
        scale = 1; /* window smaller than the video: show the top left */
    }

    unsigned out_w = pw * scale;
    unsigned out_h = ph * scale;
    if (out_w > win_w) {
        out_w = win_w;
    }
    if (out_h > area_h) {
        out_h = area_h;
    }

    unsigned need = out_w * out_h;
    if (need > g_scaled_cap) {
        free(g_scaled);
        g_scaled = malloc(need * sizeof(unsigned int));
        g_scaled_cap = g_scaled != NULL ? need : 0;
    }
    if (g_scaled == NULL) {
        return;
    }

    for (unsigned y = 0; y < out_h; y++) {
        const unsigned int *src = pic + (unsigned long)(y / scale) * pw;
        unsigned int *dst = g_scaled + (unsigned long)y * out_w;
        for (unsigned x = 0; x < out_w; x++) {
            /* h264bsd's BGRA byte order is highX's 0x00RRGGBB once the
             * alpha byte is masked off. */
            dst[x] = src[x / scale] & 0x00FFFFFFu;
        }
    }

    int ox = ((int)win_w - (int)out_w) / 2;
    int oy = ((int)area_h - (int)out_h) / 2;
    if (ox < 0) {
        ox = 0;
    }
    if (oy < 0) {
        oy = 0;
    }

    hx_image(win, ox, oy, out_w, out_h, g_scaled);
    hx_commit_rect(win, ox, oy, out_w, out_h);
}

/* Keep a copy of the picture the decoder just produced: its own
 * buffer is recycled by the next frame, and the player has to be able
 * to repaint the window at any time (a resize, an expose, or simply
 * sitting on the last frame at the end of the file). */
static void show_picture(const unsigned int *pic, unsigned pw, unsigned ph) {
    unsigned need = pw * ph;
    if (need == 0) {
        return;
    }
    if (need > g_last_cap) {
        free(g_last_pic);
        g_last_pic = malloc(need * sizeof(unsigned int));
        g_last_cap = g_last_pic != NULL ? need : 0;
    }
    if (g_last_pic == NULL) {
        return;
    }
    memcpy(g_last_pic, pic, need * sizeof(unsigned int));
    g_last_w = pw;
    g_last_h = ph;
    blit_last();
}

/* Nothing decoded after a fair number of frames means the stream is
 * beyond this decoder - almost always a main/high profile file, since
 * h264bsd is a baseline decoder. Say so where the user is looking,
 * with the command that fixes it. */
static void show_unsupported(void) {
    const char *lines[5];
    char l0[HX_TEXT_MAX], l1[HX_TEXT_MAX];

    snprintf(l0, sizeof(l0), "Cannot decode this file: H.264 profile %u,"
                             " level %u.%u", video.profile, video.level / 10,
             video.level % 10);
    snprintf(l1, sizeof(l1), "hxvideo decodes baseline (profile 66) only.");
    lines[0] = l0;
    lines[1] = l1;
    lines[2] = "Re-encode it on the build host:";
    lines[3] = "  ffmpeg -i in.mp4 -c:v libx264 -profile:v baseline \\";
    lines[4] = "         -bf 0 -refs 1 -an -vf scale=320:-2 out.mp4";

    hx_fill(win, 0, 0, win_w, win_h > BAR_H ? win_h - BAR_H : win_h, COL_BG);
    for (int i = 0; i < 5; i++) {
        hx_text(win, 12, 16 + i * (HX_FONT_H + 4),
                i == 0 ? COL_TEXT : COL_DIM, 0, 0, lines[i]);
    }
    hx_commit(win);
}

static void clear_video_area(void) {
    hx_fill(win, 0, 0, win_w, win_h > BAR_H ? win_h - BAR_H : win_h, COL_BG);
    hx_commit_rect(win, 0, 0, win_w, win_h > BAR_H ? win_h - BAR_H : win_h);
}

/* ---- decoding ---- */

/* Feed the parameter sets from avcC. Without them the decoder has no
 * idea how big the picture is and refuses every slice. */
static int feed_params(void) {
    u8 *strm = video.params;
    u32 len = video.params_len;
    u32 read_bytes;

    while (len > 0) {
        u32 result = h264bsdDecode(&dec, strm, len, 0, &read_bytes);
        if (result == H264BSD_ERROR || result == H264BSD_PARAM_SET_ERROR) {
            return -1;
        }
        strm += read_bytes;
        len -= read_bytes;
    }
    return 0;
}

/* Decode the next sample and, if it completes a picture, show it.
 * Returns 1 when a picture was displayed. */
static int decode_next(const char *path) {
    if (g_index >= video.sample_count) {
        g_eof = 1;
        return 0;
    }

    long got = mp4_read_sample(&video, g_index, g_sample,
                               video.max_sample_size + 64);
    unsigned long duration = video.samples[g_index].duration;
    g_index++;
    if (got <= 0) {
        return 0;
    }

    unsigned long t0 = uptime_ms();
    int shown = 0;
    u8 *strm = g_sample;
    u32 len = (u32)got;
    u32 read_bytes;

    while (len > 0) {
        u32 result = h264bsdDecode(&dec, strm, len, 0, &read_bytes);
        strm += read_bytes;
        len -= read_bytes;

        if (result == H264BSD_PIC_RDY) {
            u32 pic_id, is_idr, err_mbs;
            u32 *pic = h264bsdNextOutputPictureBGRA(&dec, &pic_id, &is_idr,
                                                    &err_mbs);
            if (pic != NULL) {
                show_picture(pic, h264bsdPicWidth(&dec) * 16,
                             h264bsdPicHeight(&dec) * 16);
                g_shown++;
                shown = 1;
            }
        } else if (result == H264BSD_ERROR ||
                   result == H264BSD_PARAM_SET_ERROR) {
            break; /* damaged frame: skip it, keep playing */
        }
    }

    unsigned long now = uptime_ms();
    g_decode_ms += now - t0;
    if (shown) {
        g_fps_frames++;
        if (now - g_fps_mark >= 1000) {
            g_fps = (unsigned)((unsigned long)g_fps_frames * 1000ul /
                               (now - g_fps_mark));
            g_fps_frames = 0;
            g_fps_mark = now;
        }
    }
    if (video.timescale != 0) {
        g_media_ms += duration * 1000ul / video.timescale;
    }
    if (shown) {
        draw_status(path);
    }
    return shown;
}

/* Seek by jumping to the key frame at or before `target`. An H.264
 * key frame (IDR) resets the decoder's reference state by definition,
 * so there is nothing to tear down - and tearing down is in fact
 * fatal: h264bsdShutdown() followed by h264bsdInit() on the same
 * storage leaves the decoder rejecting every parameter set (see the
 * seek checks in tests/mp4). */
static void seek_to(unsigned target) {
    unsigned sync = mp4_prev_sync(&video, target);

    g_index = sync;
    g_media_ms = mp4_sample_time_ms(&video, sync);
    g_eof = 0;
    g_start_ms = uptime_ms() - g_media_ms;
    g_fps_mark = uptime_ms();
    g_fps_frames = 0;
    clear_video_area();
}

/* Sample index at a media position, used by the seek keys. */
static unsigned sample_at_ms(unsigned long ms) {
    unsigned long acc = 0;
    for (unsigned i = 0; i < video.sample_count; i++) {
        unsigned long step = video.timescale
                                 ? video.samples[i].duration * 1000ul /
                                       video.timescale
                                 : 0;
        if (acc + step > ms) {
            return i;
        }
        acc += step;
    }
    return video.sample_count ? video.sample_count - 1 : 0;
}

int main(int argc, char **argv) {
    const char *path = argc > 1 ? argv[1] : "/video/sample.mp4";

    if (hx_open(&dpy) < 0) {
        printf("hxvideo: no highX display (start one with `highx`)\n");
        return 1;
    }

    int rc = mp4_open(&video, path);
    if (rc != MP4_OK) {
        printf("hxvideo: %s: %s\n", path, mp4_strerror(rc));
        if (rc == MP4_E_CODEC) {
            printf("hxvideo: only H.264 baseline (avc1) is supported; "
                   "re-encode with:\n  ffmpeg -i in.mp4 -c:v libx264 "
                   "-profile:v baseline -bf 0 out.mp4\n");
        }
        hx_close(&dpy);
        return 1;
    }

    g_sample = malloc(video.max_sample_size + 64);
    if (g_sample == NULL) {
        printf("hxvideo: out of memory\n");
        mp4_close(&video);
        hx_close(&dpy);
        return 1;
    }

    if (h264bsdInit(&dec, HANTRO_FALSE) != HANTRO_OK) {
        printf("hxvideo: cannot initialise the H.264 decoder\n");
        return 1;
    }

    /* Open at 2x when the screen has room: 176x144 video in a 176x144
     * window would be a stamp on a 1280x800 desktop. */
    unsigned scale = 2;
    while (video.width * (scale + 1) < dpy.screen_w / 2 && scale < 4) {
        scale++;
    }
    win_w = video.width * scale;
    win_h = video.height * scale + BAR_H;
    if (win_w > dpy.screen_w) {
        win_w = dpy.screen_w;
    }
    if (win_h > dpy.screen_h) {
        win_h = dpy.screen_h;
    }

    char title[HX_TITLE_MAX];
    snprintf(title, sizeof(title), "hxvideo - %s", path);
    win = hx_create_window(100, 100, win_w, win_h, 0, COL_BG, title);
    if (win == 0) {
        printf("hxvideo: cannot create a window\n");
        return 1;
    }
    hx_map(win);

    g_start_ms = uptime_ms();
    g_play_start_ms = g_start_ms;
    g_fps_mark = g_start_ms;

    /* The parameter sets decide whether this file can be played at
     * all: a main or high profile stream is rejected right here, and
     * the window says so instead of the program exiting into a
     * console nobody can see while highX owns the screen. */
    if (feed_params() != 0) {
        g_eof = 1;
        show_unsupported();
    }
    draw_status(path);

    for (;;) {
        struct hx_event ev;
        /* Poll while playing (the decoder is the pacemaker), block
         * when paused or finished so an idle player costs nothing. */
        int timeout = (g_paused || g_eof) ? 100 : 0;
        int n = hx_next_event(&ev, timeout);
        while (n > 0) {
            if (ev.type == HX_EV_CLOSE) {
                goto done;
            }
            if (ev.type == HX_EV_CONFIGURE) {
                win_w = ev.w;
                win_h = ev.h;
                clear_video_area();
                blit_last();
                draw_status(path);
            } else if (ev.type == HX_EV_EXPOSE) {
                clear_video_area();
                if (g_shown == 0 && g_eof) {
                    show_unsupported();
                } else {
                    blit_last();
                }
                draw_status(path);
            } else if (ev.type == HX_EV_KEY) {
                unsigned key = ev.key;
                if (key == 'q' || key == 'Q' || key == 0x1B) {
                    goto done;
                }
                if (key == ' ') {
                    g_paused = !g_paused;
                    if (!g_paused) {
                        g_start_ms = uptime_ms() - g_media_ms;
                    }
                    draw_status(path);
                } else if (key == 'r' || key == 'R') {
                    seek_to(0);
                    draw_status(path);
                } else if (key == HX_KEY_RIGHT) {
                    seek_to(sample_at_ms(g_media_ms + SEEK_MS));
                    draw_status(path);
                } else if (key == HX_KEY_LEFT) {
                    unsigned long back = g_media_ms > SEEK_MS
                                             ? g_media_ms - SEEK_MS
                                             : 0;
                    seek_to(sample_at_ms(back));
                    draw_status(path);
                }
            }
            n = hx_next_event(&ev, 0);
        }

        if (g_paused || g_eof) {
            continue;
        }

        /* Pacing: decode the next frame when its presentation time has
         * arrived. If the decoder is slower than the video's frame
         * rate this is always true and playback simply runs behind -
         * the status line shows the rate that is actually achieved. */
        unsigned long now = uptime_ms();
        if (now - g_start_ms + 5 >= g_media_ms) {
            decode_next(path);
            if (g_shown == 0 && g_index >= 15) {
                show_unsupported();
                g_eof = 1;
            }
        }
    }

done:
    h264bsdShutdown(&dec);
    free(g_scaled);
    free(g_last_pic);
    free(g_sample);
    mp4_close(&video);
    hx_destroy_window(win);
    hx_close(&dpy);
    return 0;
}
