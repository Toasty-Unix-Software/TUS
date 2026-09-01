/*
 * lv_conf.h - LVGL configuration for TUS
 *
 * Deliberately small: include/lvgl/config/lv_conf_internal.h supplies
 * a default for every setting this does not override (that is its
 * entire job - "ensure all defines of lv_conf.h have a default
 * value"), so only the handful of choices that actually depend on
 * TUS - which C library it has, that there is no RTOS, what a highX
 * window's pixels look like - need to be made here. Everything else
 * is upstream LVGL's own judgment, not TUS's.
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* TUS has a real libc (musl): let LVGL call straight into it instead
 * of carrying its own allocator/string/sprintf implementations. */
#define LV_USE_STDLIB_MALLOC  LV_STDLIB_CLIB
#define LV_USE_STDLIB_STRING  LV_STDLIB_CLIB
#define LV_USE_STDLIB_SPRINTF LV_STDLIB_CLIB

/* No RTOS: TUS's own scheduler runs this like any other task, and the
 * event loop in userspace/lvgl_port/ IS lv_timer_handler()'s caller -
 * LVGL needs no thread of its own. */
#define LV_USE_OS LV_OS_NONE

/* The software rasteriser. Defaults to 0 (off) when lv_conf.h does not
 * mention it at all - "ensure a default for everything" does not mean
 * "everything defaults to on". Without this, LVGL still runs and the
 * display's own background fill still shows (that part does not go
 * through a draw unit), but no widget draws anything: a background
 * colour with nothing on it looks exactly like nothing having drawn
 * at all, which is precisely how this was first found - by noticing
 * lv_obj_get_width() after lv_obj_update_layout() reported the right
 * size for a card that still would not appear. */
#define LV_USE_DRAW_SW 1

/* Which pixel formats the software rasteriser can actually write -
 * ALSO off by default per format, independently of LV_USE_DRAW_SW
 * itself, which is what made this two bugs instead of one: with only
 * LV_USE_DRAW_SW set, the display's own full-buffer background clear
 * (a simpler, format-agnostic fill) still worked and looked like a
 * successful render, while every widget's own draw task silently did
 * nothing because XRGB8888 - this port's LV_COLOR_DEPTH/colour format
 * - was not in the "supported" list. ARGB8888 is needed too: an
 * intermediate layer (any object with rounded corners, a shadow, or
 * partial opacity - which is to say a card, which is to say every
 * widget the default theme draws) composites through it even when
 * the final display format is XRGB8888. */
#define LV_DRAW_SW_SUPPORT_XRGB8888 1
#define LV_DRAW_SW_SUPPORT_ARGB8888 1

/* 32 bits per pixel, XRGB - the same 0x00RRGGBB layout hglColorBuffer()
 * and hx_image() already use everywhere else in TUS, so a frame goes
 * from LVGL's draw buffer to the highX window with no conversion pass
 * (userspace/lvgl_port/tus_lvgl.c's flush callback is a straight
 * hx_image() of the dirty area). */
#define LV_COLOR_DEPTH 32

/* On while the port is still being brought up (see
 * userspace/lvgldemo.c's lv_log_register_print_cb) - LVGL's own
 * LV_LOG_WARN calls are what actually explained the invisible-widget
 * bug below, silently swallowed the first time this was 0. */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

/* A real typeface belongs in this project (userspace/tusfont/), not
 * a font compiled into every LVGL binary - but until every widget
 * that wants a default font is wired to ask tusfont for one, LVGL
 * still needs SOMETHING for LV_FONT_DEFAULT, so its own bitmap
 * Montserrat ships as that fallback the same way the console's own
 * bitmap font is a deliberate, not-a-workaround choice elsewhere in
 * this codebase. */
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

#endif /* LV_CONF_H */
