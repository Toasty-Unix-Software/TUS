/*
 * lvgldemo - LVGL running for real, in a highX window
 *
 * The proof that userspace/lvgl_port/tus_lvgl.c actually works: a
 * card with a label, a button that counts its own clicks, and a
 * slider that reports its value live - LVGL's widget/style/event
 * system, its animation-capable draw pipeline, and its input device
 * abstraction, all driven by a highX window instead of the
 * framebuffer or SDL window every other LVGL port uses.
 *
 * Not a toolkit TUS is standardising on: the macOS-style window
 * chrome (userspace/tuswm.c, userspace/tusde.c) is drawn with hglui,
 * not LVGL, and stays that way - that decision was already made and
 * tested working. This is what porting LVGL itself, as its own
 * option, actually looks like once it is finished rather than
 * planned.
 */

#include "highapi/highapi.h"
#include "lvgl.h"
#include "lvgl_port/tus_lvgl.h"

#include <stdio.h>

#define WIN_W 360
#define WIN_H 260

static struct hx_display g_dpy;
static lv_display_t *g_lv;
static lv_obj_t *g_count_label;
static int g_clicks;

static void button_clicked(lv_event_t *e) {
    (void)e;
    g_clicks++;
    lv_label_set_text_fmt(g_count_label, "Clicked %d time%s", g_clicks,
                          g_clicks == 1 ? "" : "s");
}

static void slider_changed(lv_event_t *e) {
    lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *value_label = (lv_obj_t *)lv_event_get_user_data(e);
    lv_label_set_text_fmt(value_label, "%d%%", (int)lv_slider_get_value(slider));
}

static void build_ui(void) {
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x14181f), 0);

    lv_obj_t *probe = lv_obj_create(scr);
    lv_obj_remove_style_all(probe);
    lv_obj_set_size(probe, 60, 60);
    lv_obj_set_pos(probe, 10, 10);
    lv_obj_set_style_bg_color(probe, lv_color_hex(0xff0000), 0);
    lv_obj_set_style_bg_opa(probe, LV_OPA_COVER, 0);
    printf("lvgldemo: probe created\n");

    lv_obj_t *card = lv_obj_create(scr);
    lv_obj_set_size(card, lv_pct(90), lv_pct(90));
    lv_obj_center(card);
    printf("lvgldemo: card at (%d,%d) size %dx%d\n", (int)lv_obj_get_x(card),
          (int)lv_obj_get_y(card), (int)lv_obj_get_width(card),
          (int)lv_obj_get_height(card));
    lv_obj_set_style_bg_color(card, lv_color_hex(0x20242c), 0);
    lv_obj_set_style_radius(card, 14, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0x353b46), 0);
    lv_obj_set_style_pad_all(card, 16, 0);

    lv_obj_t *title = lv_label_create(card);
    lv_label_set_text(title, "LVGL on TUS");
    lv_obj_set_style_text_color(title, lv_color_hex(0xf0f0f0), 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *btn = lv_button_create(card);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x4fa3d1), 0);
    lv_obj_set_style_radius(btn, 8, 0);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 40);
    lv_obj_add_event_cb(btn, button_clicked, LV_EVENT_CLICKED, NULL);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click me");

    g_count_label = lv_label_create(card);
    lv_label_set_text(g_count_label, "Clicked 0 times");
    lv_obj_set_style_text_color(g_count_label, lv_color_hex(0x9098a3), 0);
    lv_obj_align(g_count_label, LV_ALIGN_TOP_LEFT, 0, 84);

    lv_obj_t *slider_label = lv_label_create(card);
    lv_label_set_text(slider_label, "Brightness");
    lv_obj_set_style_text_color(slider_label, lv_color_hex(0xf0f0f0), 0);
    lv_obj_align(slider_label, LV_ALIGN_TOP_LEFT, 0, 130);

    lv_obj_t *value_label = lv_label_create(card);
    lv_label_set_text(value_label, "50%");
    lv_obj_set_style_text_color(value_label, lv_color_hex(0x9098a3), 0);
    lv_obj_align(value_label, LV_ALIGN_TOP_RIGHT, 0, 130);

    lv_obj_t *slider = lv_slider_create(card);
    lv_obj_set_width(slider, lv_pct(100));
    lv_obj_align(slider, LV_ALIGN_TOP_LEFT, 0, 156);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_changed, LV_EVENT_VALUE_CHANGED,
                        value_label);
}

int main(void) {
    if (hx_open(&g_dpy) < 0) {
        printf("lvgldemo: no highX display (start it with `highx`)\n");
        return 1;
    }

    unsigned int win = hx_create_window(80, 80, WIN_W, WIN_H, 0, 0x00141822u,
                                        "LVGL");
    if (win == 0) {
        printf("lvgldemo: could not create a window\n");
        hx_close(&g_dpy);
        return 1;
    }

    printf("lvgldemo: window created\n");
    g_lv = tus_lvgl_create(win, WIN_W, WIN_H);
    if (g_lv == NULL) {
        printf("lvgldemo: out of memory bringing up the display\n");
        hx_destroy_window(win);
        hx_close(&g_dpy);
        return 1;
    }
    printf("lvgldemo: lvgl init ok\n");

    build_ui();
    lv_obj_update_layout(lv_screen_active());
    lv_obj_t *card0 = lv_obj_get_child(lv_screen_active(), 0);
    printf("lvgldemo: ui built, screen has %d children, card0 %dx%d at "
          "(%d,%d)\n",
          (int)lv_obj_get_child_count(lv_screen_active()),
          (int)lv_obj_get_width(card0), (int)lv_obj_get_height(card0),
          (int)lv_obj_get_x(card0), (int)lv_obj_get_y(card0));
    hx_map(win);
    printf("lvgldemo: mapped\n");

    /* 30ms: smooth enough for a slider drag or a button's press
     * animation, and - since hx_next_event() with a positive timeout
     * blocks in the kernel (hlt()) rather than spinning - this is a
     * wake every 30ms while idle, not a busy loop. The first wait of
     * each pass blocks; the rest drain whatever else is already
     * queued before the next tick, same as tuswm's event loop. */
    int running = 1;
    while (running) {
        struct hx_event ev;
        int n = hx_next_event(&ev, 30);
        while (n > 0) {
            switch (ev.type) {
            case HX_EV_CLOSE:
                running = 0;
                break;
            case HX_EV_POINTER:
                tus_lvgl_feed_pointer(g_lv, ev.x, ev.y,
                                     ev.detail != HX_PTR_RELEASE);
                break;
            case HX_EV_CONFIGURE:
                /* tusWM resizes a new window to fit its assigned tile
                 * right after mapping it (and again on every relayout)
                 * - the window's backing store gets reallocated
                 * server-side when that happens, so LVGL's buffer and
                 * idea of the display size have to follow or the next
                 * frame silently has nowhere real to go. */
                tus_lvgl_resize(g_lv, (int)ev.w, (int)ev.h);
                printf("lvgldemo: resized to %ux%u\n", ev.w, ev.h);
                break;
            default:
                break;
            }
            if (!running) {
                break;
            }
            n = hx_next_event(&ev, 0);
        }
        if (!running) {
            break;
        }
        static int loop_n;
        if (loop_n < 15) {
            printf("lvgldemo: loop #%d lv_tick_get=%u\n", loop_n,
                  (unsigned)lv_tick_get());
        }
        loop_n++;
        tus_lvgl_tick();
    }

    tus_lvgl_destroy(g_lv);
    hx_destroy_window(win);
    hx_close(&g_dpy);
    return 0;
}
