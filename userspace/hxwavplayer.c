/*
 * hxwavplayer - a real highX window around wavplay's WAV/PCM path
 *
 *     hxwavplayer /tmp/song.wav
 *
 * wavplay (userspace/wavplay.c) is the CLI: open, stream the whole
 * file to /dev/dsp, block until done. This is the same playback path
 * (wav.c's parser/converter, kernel/drivers/hda/hda.c's /dev/dsp) driven
 * from a highX event loop instead, so a window with a play/pause
 * button stays responsive while audio plays - one chunk of PCM
 * written per pass through the loop, interleaved with draining
 * pointer/close events and an LVGL tick, the same shape
 * userspace/lvgldemo.c and userspace/hxvideo.c already use for a
 * "do a bit of media work, then check for events" loop.
 *
 * The chrome is LVGL, via userspace/lvgl_port/tus_lvgl.{c,h} (the
 * same multi-instance API tuswm/tusde use for their own decorations -
 * see userspace/lvgldemo.c for the reference pattern of an LVGL app
 * living inside one highX window) and userspace/lvgl_port/
 * tus_lvgl_font.{c,h} for real TrueType chrome text instead of LVGL's
 * bundled bitmap font, matching the rest of the desktop's look.
 */

#include "highapi/highapi.h"
#include "lvgl.h"
#include "lvgl_port/tus_lvgl.h"
#include "lvgl_port/tus_lvgl_font.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <tusaudio.h>

#include "wav.h"

#define WIN_W 360
#define WIN_H 150

#define CHUNK_FRAMES 1440 /* ~30ms at 48000 Hz - keeps the event loop responsive */

#define CHROME_FONT_PATH "/usr/share/fonts/OpenSans-Light.ttf"
#define CHROME_FONT_SIZE 14

/* SYS_UPTIME (kernel/syscall/syscall.h): milliseconds since boot.
 * musl's syscall_arch.h does not wrap TUS's ABI, so this makes the
 * int $0x80 trap directly - the same three-argument stub every other
 * highX client that needs it uses (hxvideo.c, tus_lvgl.c). */
#define SYS_UPTIME 7

static long tus_syscall(long n, long a1, long a2, long a3) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret)
                     : "a"(n), "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8),
                       "r"(r9)
                     : "memory", "cc");
    return ret;
}

static unsigned long uptime_ms(void) {
    return (unsigned long)tus_syscall(SYS_UPTIME, 0, 0, 0);
}

static struct hx_display g_dpy;
static lv_display_t     *g_lv;
static lv_font_t        *g_chrome_font;

static lv_obj_t *g_play_btn;
static lv_obj_t *g_play_label;
static lv_obj_t *g_time_label;

static int      g_fd = -1;   /* the WAV file, positioned at the next unread frame */
static int      g_dsp = -1;
static struct wav_info g_info;
static uint32_t g_total_frames;
static uint32_t g_frames_done; /* frames handed to /dev/dsp so far (this play-through) */
static int      g_playing;
static int      g_finished;
static uint32_t g_last_shown_sec = 0xffffffffu; /* forces the first update_time_label() to draw */

static void fmt_time(char *buf, size_t n, uint32_t frames) {
    uint32_t secs = frames / 48000;
    snprintf(buf, n, "%u:%02u", secs / 60, secs % 60);
}

/* A real bug this shipped with once, worth remembering: calling this
 * unconditionally from play_one_chunk() (i.e. every ~30ms audio
 * chunk) makes every chunk pay for an lv_label_set_text() ->
 * lv_obj_invalidate() -> a full LVGL redraw + hx_image()/hx_commit()
 * round trip through the kernel - on this project's QEMU/TCG-emulated
 * target that redraw can itself take longer than the 30ms of audio it
 * was supposed to be pacing, so the player falls behind its own
 * output and /dev/dsp's ring underruns: real, audible gaps, confirmed
 * by capturing actual output via -audiodev wav and finding multi-
 * hundred-ms silent runs between short bursts. The label only ever
 * shows whole seconds anyway, so redrawing on every fractional-second
 * chunk was pure waste - gate it on the displayed value actually
 * changing (see maybe_update_time_label()) instead of calling this
 * directly from the audio path. */
static void update_time_label(void) {
    char cur[16], tot[16], line[40];
    fmt_time(cur, sizeof(cur), g_frames_done);
    fmt_time(tot, sizeof(tot), g_total_frames);
    snprintf(line, sizeof(line), "%s / %s", cur, tot);
    lv_label_set_text(g_time_label, line);
    g_last_shown_sec = g_frames_done / 48000;
}

/* Cheap (one division, one compare) to call every loop iteration;
 * only actually touches LVGL when the whole-second count displayed
 * would change. */
static void maybe_update_time_label(void) {
    if (g_frames_done / 48000 != g_last_shown_sec) {
        update_time_label();
    }
}

static void set_playing(int playing) {
    g_playing = playing;
    lv_label_set_text(g_play_label, g_playing ? "Pause" : "Play");
}

/* A pause has to actually silence the codec, not just stop feeding
 * it - the ring can hold up to HDA_RING_BYTES (128 KiB, ~682ms at
 * this fixed format) of already-queued audio, so merely stopping
 * write() would keep playing for over half a second after the
 * button says "Play" again. TUS_AUDIO_STOP halts the stream and
 * zeroes the ring immediately; the next write() (on resume, or a
 * replay) re-arms it on its own - see kernel/drivers/hda/hda.c's
 * hda_write(), which sets SDCTL_RUN again the moment g_running is
 * false. Our own file read position is untouched by this, so resume
 * continues exactly where playback was paused. */
static void stop_audio_now(void) {
    if (g_dsp >= 0) {
        ioctl(g_dsp, TUS_AUDIO_STOP, NULL);
    }
}

static void restart_from_beginning(void) {
    lseek(g_fd, g_info.data_offset, SEEK_SET);
    g_frames_done = 0;
    g_finished = 0;
    update_time_label();
}

static void play_pause_clicked(lv_event_t *e) {
    (void)e;
    if (g_finished) {
        restart_from_beginning();
        set_playing(1);
        return;
    }
    if (g_playing) {
        stop_audio_now();
        set_playing(0);
    } else {
        set_playing(1);
    }
}

/* Reads and plays exactly one chunk (<= CHUNK_FRAMES frames). Called
 * once per pass through the event loop while playing, so a single
 * chunk's worth of blocking inside write() (bounded by how much
 * space the 128 KiB ring needs to free, at most tens of ms at this
 * chunk size) is the only thing standing between two rounds of event
 * polling - short enough that the window stays responsive. */
static void play_one_chunk(void) {
    uint32_t bpf = wav_bytes_per_frame(&g_info);
    uint32_t remaining_frames = g_total_frames - g_frames_done;
    if (remaining_frames == 0) {
        ioctl(g_dsp, TUS_AUDIO_DRAIN, NULL);
        ioctl(g_dsp, TUS_AUDIO_STOP, NULL);
        g_finished = 1;
        set_playing(0);
        lv_label_set_text(g_play_label, "Play Again");
        return;
    }

    uint32_t want_frames = CHUNK_FRAMES;
    if (want_frames > remaining_frames) {
        want_frames = remaining_frames;
    }

    unsigned char in_buf[CHUNK_FRAMES * 4];
    int16_t out_buf[CHUNK_FRAMES * 2];

    size_t want_bytes = (size_t)want_frames * bpf;
    size_t got = 0;
    while (got < want_bytes) {
        long n = read(g_fd, in_buf + got, want_bytes - got);
        if (n <= 0) {
            /* Short file: treat whatever we could not read as the end. */
            g_total_frames = g_frames_done + (uint32_t)(got / bpf);
            want_frames = (uint32_t)(got / bpf);
            break;
        }
        got += (size_t)n;
    }
    if (want_frames == 0) {
        ioctl(g_dsp, TUS_AUDIO_DRAIN, NULL);
        ioctl(g_dsp, TUS_AUDIO_STOP, NULL);
        g_finished = 1;
        set_playing(0);
        lv_label_set_text(g_play_label, "Play Again");
        return;
    }

    wav_convert_frames(&g_info, in_buf, want_frames, out_buf);

    size_t out_bytes = (size_t)want_frames * 4;
    size_t written = 0;
    while (written < out_bytes) {
        long n = write(g_dsp, (unsigned char *)out_buf + written, out_bytes - written);
        if (n <= 0) {
            /* /dev/dsp went away - stop trying, leave the window open. */
            g_finished = 1;
            set_playing(0);
            return;
        }
        written += (size_t)n;
    }

    g_frames_done += want_frames;
}

/* Returns 0 for HX_EV_CLOSE (caller should stop), 1 otherwise. Shared
 * between the priming drain below and the main loop so the two paths
 * cannot handle the same event types differently. */
static int handle_event(struct hx_event *ev) {
    switch (ev->type) {
    case HX_EV_CLOSE:
        return 0;
    case HX_EV_POINTER:
        tus_lvgl_feed_pointer(g_lv, ev->x, ev->y, ev->detail != HX_PTR_RELEASE);
        break;
    case HX_EV_CONFIGURE:
        tus_lvgl_resize(g_lv, (int)ev->w, (int)ev->h);
        break;
    default:
        break;
    }
    return 1;
}

static void build_ui(const char *path) {
    lv_obj_t *scr = lv_display_get_screen_active(g_lv);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x181818), 0);

    const char *base = strrchr(path, '/');
    base = base ? base + 1 : path;

    lv_obj_t *name = lv_label_create(scr);
    if (g_chrome_font) {
        lv_obj_set_style_text_font(name, g_chrome_font, 0);
    }
    lv_obj_set_style_text_color(name, lv_color_hex(0xf0f0f0), 0);
    lv_label_set_text(name, base);
    lv_obj_set_width(name, WIN_W - 32);
    lv_obj_align(name, LV_ALIGN_TOP_LEFT, 16, 16);

    g_time_label = lv_label_create(scr);
    if (g_chrome_font) {
        lv_obj_set_style_text_font(g_time_label, g_chrome_font, 0);
    }
    lv_obj_set_style_text_color(g_time_label, lv_color_hex(0x9098a3), 0);
    lv_obj_align(g_time_label, LV_ALIGN_TOP_LEFT, 16, 44);
    update_time_label();

    g_play_btn = lv_button_create(scr);
    lv_obj_set_size(g_play_btn, 120, 40);
    lv_obj_set_style_bg_color(g_play_btn, lv_color_hex(0xe8e8e8), 0);
    lv_obj_set_style_radius(g_play_btn, 8, 0);
    lv_obj_align(g_play_btn, LV_ALIGN_BOTTOM_LEFT, 16, -16);
    lv_obj_add_event_cb(g_play_btn, play_pause_clicked, LV_EVENT_CLICKED, NULL);

    g_play_label = lv_label_create(g_play_btn);
    if (g_chrome_font) {
        lv_obj_set_style_text_font(g_play_label, g_chrome_font, 0);
    }
    lv_obj_set_style_text_color(g_play_label, lv_color_hex(0x181818), 0);
    lv_obj_center(g_play_label);
    lv_label_set_text(g_play_label, "Pause");
}

int main(int argc, char **argv) {
    /* Default to the 440Hz demo clip kernel/drivers/hda/hda.c's bring-up
     * shipped (rootfs/tmp/test.wav) - same reason hxvideo.c defaults
     * to /video/sample.mp4: a menu/dock launch passes no argv, and a
     * player that only works from a command line argument is useless
     * from there. */
    const char *path = argc > 1 ? argv[1] : "/tmp/test.wav";

    if (hx_open(&g_dpy) < 0) {
        printf("hxwavplayer: no highX display (start one with `highx`)\n");
        return 1;
    }

    g_fd = wav_open("hxwavplayer", path, &g_info);
    if (g_fd < 0) {
        hx_close(&g_dpy);
        return 1;
    }
    g_total_frames = g_info.data_size / wav_bytes_per_frame(&g_info);

    g_dsp = open("/dev/dsp", O_WRONLY);
    if (g_dsp < 0) {
        printf("hxwavplayer: cannot open /dev/dsp - no HD Audio codec found\n");
        close(g_fd);
        hx_close(&g_dpy);
        return 1;
    }

    char title[HX_TITLE_MAX];
    snprintf(title, sizeof(title), "WAV Player");
    unsigned int win = hx_create_window(120, 120, WIN_W, WIN_H, 0, 0x00181818u,
                                        title);
    if (win == 0) {
        printf("hxwavplayer: could not create a window\n");
        close(g_dsp);
        close(g_fd);
        hx_close(&g_dpy);
        return 1;
    }

    g_lv = tus_lvgl_create(win, WIN_W, WIN_H);
    if (g_lv == NULL) {
        printf("hxwavplayer: out of memory bringing up the display\n");
        hx_destroy_window(win);
        close(g_dsp);
        close(g_fd);
        hx_close(&g_dpy);
        return 1;
    }

    g_chrome_font = tus_lvgl_font_create(CHROME_FONT_PATH, CHROME_FONT_SIZE);
    build_ui(path);
    hx_map(win);
    /* Prime the display BEFORE any audio is queued: the very first
     * tus_lvgl_tick() call has to render every glyph in the window
     * (tusfont TrueType rasterisation, all cache misses the first
     * time) and push the first full-window hx_commit() - genuinely
     * slow under this project's QEMU/TCG-emulated target (measured:
     * over a second on this host). Doing that AFTER the first audio
     * chunk was already written left the codec's DAC running dry for
     * that whole stretch - a real, audible gap at the start of every
     * playback, confirmed via a real -audiodev wav capture showing a
     * multi-hundred-ms silent run right after the first few samples.
     * Rendering once here, before set_playing(1), means that one-time
     * cost is paid before the ring has anything in it to starve. */
    /* A tiling WM (tuswm's default layout) retiles and sends
     * HX_EV_CONFIGURE the moment a second/third window maps - that
     * does not happen synchronously inside hx_map() above, it is the
     * WM's own task running on its own schedule, so a single
     * non-blocking poll right here can miss it. Give it real time
     * (measured: arrives within tens of ms in practice) to show up
     * and drain it - along with the resize+redraw it triggers - before
     * any audio is queued, same reasoning as the tus_lvgl_tick() calls
     * below. */
    for (int i = 0; i < 5; i++) {
        struct hx_event ev;
        int n = hx_next_event(&ev, 40);
        while (n > 0) {
            handle_event(&ev);
            n = hx_next_event(&ev, 0);
        }
    }
    tus_lvgl_tick();
    tus_lvgl_tick();
    set_playing(1);

    int running = 1;
    while (running) {
        struct hx_event ev;
        /* Non-blocking while audio is flowing (play_one_chunk()'s own
         * write() below provides the pacing/wait); a real 30ms block
         * (hlt() in the kernel, not a spin) while paused/finished, so
         * an idle player costs nothing. */
        int n = hx_next_event(&ev, g_playing ? 0 : 30);
        while (n > 0) {
            if (!handle_event(&ev)) {
                running = 0;
                break;
            }
            n = hx_next_event(&ev, 0);
        }
        if (!running) {
            break;
        }
        if (g_playing) {
            play_one_chunk();
            maybe_update_time_label();
        }
        tus_lvgl_tick();
    }

    if (!g_finished) {
        stop_audio_now();
    }
    if (g_chrome_font) {
        tus_lvgl_font_destroy(g_chrome_font);
    }
    tus_lvgl_destroy(g_lv);
    hx_destroy_window(win);
    close(g_dsp);
    close(g_fd);
    hx_close(&g_dpy);
    return 0;
}
