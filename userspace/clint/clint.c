/*
 * clint - a web browser for TUS
 *
 *   clint [url]
 *
 * Type an address and press Enter; click a link to follow it;
 * Backspace goes back. Arrow keys, PageUp/PageDown, Home and End
 * scroll.
 *
 * The whole page is drawn into one off-screen canvas and handed to
 * highX as a single image (see paint.h), so scrolling and reloading
 * never show a half-built page. Layout is redone when the window
 * changes width and not otherwise - it is the expensive step, and
 * scrolling does not change it.
 *
 * What Clint honestly does not do: images (there is no decoder, so
 * alt text stands in), JavaScript, and forms. What it does do is
 * fetch over http and https, parse real HTML, apply real CSS, and lay
 * text out in boxes.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "highapi/highapi.h"

#include "charset.h"
#include "css.h"
#include "dom.h"
#include "http.h"
#include "image.h"
#include "js.h"
#include "layout.h"
#include "paint.h"
#include "utf8.h"

/*
 * The browser's own furniture. A toolbar tall enough for a round
 * button, a pill for the address, a hairline under it and a quiet
 * footer: the shapes a reader expects, drawn with the primitives in
 * paint.h rather than borrowed from a toolkit TUS does not have.
 */
#define BAR_H        46
#define STATUS_H     22
#define BTN_SIZE     30
#define BTN_GAP      4
#define PILL_H       30
#define SCROLL_STEP  (CELL_H * 3)
/* Wide enough to hit with a pointer, which a 6-pixel bar was not. */
#define SCROLLBAR_W  12
#define THUMB_MIN    24

#define COL_CHROME    0x00F1F3F4u   /* the toolbar */
#define COL_PILL      0x00FFFFFFu   /* the address field */
#define COL_HOVER     0x00E3E5E8u   /* a button under the pointer */
#define COL_BORDER    0x00DADCE0u
#define COL_TEXT      0x00202124u
#define COL_MUTED     0x005F6368u
#define COL_ACCENT    0x001A73E8u
#define COL_PAGE      0x00FFFFFFu
#define COL_STATUS    0x00F8F9FAu
#define COL_ERROR     0x00D93025u
#define COL_SECURE    0x00188038u
#define COL_TRACK     0x00F1F3F4u   /* the scrollbar's groove */
#define COL_THUMB     0x00BDC1C6u
#define COL_THUMB_HOT 0x008E9296u

#define HISTORY_MAX 32

/* Images are fetched one at a time over a link that is not fast, so
 * there is a limit on how much of a page's decoration is worth
 * waiting for. The ones past it keep their alt text. */
#define IMAGE_MAX      24
#define IMAGE_MAX_BYTES (4u * 1024 * 1024)

/* <script src="...">: same idea as a big picture - a script this
 * large is not one the interpreter's own 12 MB/4M-step budget was
 * going to survive anyway, so refusing it early is honest rather
 * than restrictive. */
#define SCRIPT_MAX_BYTES (2u * 1024 * 1024)

/* The padding layout put inside a control's box; the contents are
 * drawn here, so both have to agree about it. */
#define FIELD_PAD    4
#define COL_FIELD    0x00101010u
#define COL_FOCUS    0x001A73E8u

#define FIELD_VALUE_MAX 1024

/* Video is handed to TUS's own player rather than decoded here: the
 * demuxer and the H.264 decoder already exist (userspace/hxvideo.c),
 * and a browser that starts them is a browser that plays video. */
#define VIDEO_PATH  "/tmp/clint-video.mp4"

static struct hx_display dpy;
static unsigned int win;
static int win_w = 900, win_h = 620;

static struct canvas canvas;
static struct dom_node *document_tree;
static struct doc_layout page;

static char address[1024];      /* what the bar shows and edits */
static int address_focus;
static int address_caret;

static char status[256];
static int status_error;

/* The control being typed into, remembered by its element rather than
 * by its rectangle: a relayout builds new rectangles, and the element
 * is what survives it. */
static struct dom_node *focus_node;
static int field_caret;

/* ---- scripting ---- */

/*
 * One interpreter per page, thrown away with the page. Two things
 * scripts do have to be deferred rather than done where they happen:
 * navigating (the document is still being walked) and relaying out
 * (a script that touches the tree fifty times must not lay the page
 * out fifty times). Both become a flag that the browser acts on once
 * the script has finished.
 */
static struct js *script;
static int dom_dirty;
static char pending_url[1024];

static struct url current;
static int have_current;
static int scroll;

static struct url history[HISTORY_MAX];
static int history_len;

/* Going back has to leave somewhere to go forward to. */
static struct url forward[HISTORY_MAX];
static int forward_len;

/* What the chrome is doing: which button the pointer is over, whether
 * a page is on its way, and what the pointer is hovering in the page. */
enum { BTN_NONE = -1, BTN_BACK = 0, BTN_FORWARD, BTN_RELOAD, BTN_COUNT };
static int hover_button = BTN_NONE;

/* The scrollbar under the pointer, and the drag in progress: the
 * thumb is grabbed at a point, so it must follow the pointer without
 * jumping that offset to the top of the thumb. */
static int hover_thumb;
static int drag_thumb;
static int drag_grab_y;   /* where inside the thumb the press landed */
static int loading;
static char hover_link[512];

static char page_title[128];

/* ---- images ---- */

/*
 * One entry per <img src> on the page, keyed by the attribute exactly
 * as the document wrote it - that is what layout asks with, and two
 * <img> tags with the same src are the same picture. `failed` is
 * remembered too: a JPEG or a 404 must not be retried on every
 * relayout.
 */
static struct {
    char src[600];
    struct image img;
    int failed;
} images[IMAGE_MAX];
static int image_count;

static void images_clear(void) {
    for (int i = 0; i < image_count; i++) image_free(&images[i].img);
    image_count = 0;
    memset(images, 0, sizeof(images));
}

/* The hook layout calls. Nothing is fetched here: layout runs on
 * every resize, and a page must not go back to the network for that. */
static const struct image *image_for(const char *src) {
    for (int i = 0; i < image_count; i++) {
        if (strcmp(images[i].src, src) == 0) {
            return images[i].failed ? NULL : &images[i].img;
        }
    }
    return NULL;
}

static int image_known(const char *src) {
    for (int i = 0; i < image_count; i++) {
        if (strcmp(images[i].src, src) == 0) return 1;
    }
    return 0;
}

/* ---- drawing ---- */

static int page_view_h(void) {
    int h = win_h - BAR_H - STATUS_H;
    return h > 0 ? h : 0;
}

/*
 * The scrollbar's geometry, in one place for the same reason the
 * toolbar has one: painting it and grabbing it must agree to the
 * pixel. Returns 0 when the page fits and there is no bar at all.
 */
static int scrollbar_metrics(int *track_y, int *track_h, int *thumb_y,
                             int *thumb_h) {
    int view_h = page_view_h();
    if (view_h <= 0 || page.height <= view_h) return 0;

    int th = view_h * view_h / page.height;
    if (th < THUMB_MIN) th = THUMB_MIN;
    if (th > view_h) th = view_h;

    int span = view_h - th;
    int max_scroll = page.height - view_h;
    int at = max_scroll > 0 ? span * scroll / max_scroll : 0;

    if (track_y != NULL) *track_y = BAR_H;
    if (track_h != NULL) *track_h = view_h;
    if (thumb_y != NULL) *thumb_y = BAR_H + at;
    if (thumb_h != NULL) *thumb_h = th;
    return 1;
}

/* Where each piece of the toolbar sits. One function, so drawing and
 * clicking can never disagree about a button's rectangle. */
static void button_rect(int which, int *x, int *y, int *size) {
    *size = BTN_SIZE;
    *y = (BAR_H - BTN_SIZE) / 2;
    *x = 8 + which * (BTN_SIZE + BTN_GAP);
}

static void pill_rect(int *x, int *y, int *w, int *h) {
    int bx, by, size;
    button_rect(BTN_COUNT - 1, &bx, &by, &size);
    *x = bx + size + 10;
    *y = (BAR_H - PILL_H) / 2;
    *w = win_w - *x - 10;
    *h = PILL_H;
    if (*w < 80) *w = 80;
}

static int button_enabled(int which) {
    switch (which) {
    case BTN_BACK:    return history_len > 0;
    case BTN_FORWARD: return forward_len > 0;
    default:          return have_current;
    }
}

/* The reload glyph: a ring with a bite taken out of it and an
 * arrowhead where the bite is - which is what every browser draws and
 * what the eye reads as "again". */
static void draw_reload(int cx, int cy, uint32_t color) {
    uint32_t behind = hover_button == BTN_RELOAD ? COL_HOVER : COL_CHROME;
    canvas_ring(&canvas, cx, cy, 7, 2, color);
    canvas_fill(&canvas, cx, cy - 9, 9, 6, behind);
    canvas_triangle(&canvas, cx + 5, cy - 5, 5, DIR_RIGHT, color);
}

static void draw_lock(int x, int y, uint32_t color) {
    canvas_ring(&canvas, x + 4, y + 5, 4, 2, color);
    canvas_fill(&canvas, x, y + 5, 9, 7, color);
}

static void draw_globe(int x, int y, uint32_t color) {
    canvas_ring(&canvas, x + 4, y + 6, 5, 1, color);
    canvas_fill(&canvas, x - 1, y + 6, 11, 1, color);
    canvas_ring(&canvas, x + 4, y + 6, 2, 1, color);
}

static void draw_chrome(void) {
    canvas_fill(&canvas, 0, 0, win_w, BAR_H, COL_CHROME);
    canvas_fill(&canvas, 0, BAR_H - 1, win_w, 1, COL_BORDER);

    /* Back, forward, reload. */
    for (int i = 0; i < BTN_COUNT; i++) {
        int x, y, size;
        button_rect(i, &x, &y, &size);
        int enabled = button_enabled(i);
        uint32_t ink = enabled ? COL_TEXT : COL_BORDER;

        if (hover_button == i && enabled) {
            canvas_circle(&canvas, x + size / 2, y + size / 2, size / 2,
                          COL_HOVER);
        }
        if (i == BTN_BACK) {
            canvas_triangle(&canvas, x + size / 2 - 3, y + size / 2, 6,
                            DIR_LEFT, ink);
            canvas_fill(&canvas, x + size / 2 - 3, y + size / 2, 9, 2, ink);
        } else if (i == BTN_FORWARD) {
            canvas_triangle(&canvas, x + size / 2 + 3, y + size / 2, 6,
                            DIR_RIGHT, ink);
            canvas_fill(&canvas, x + size / 2 - 6, y + size / 2, 9, 2, ink);
        } else {
            draw_reload(x + size / 2, y + size / 2, ink);
        }
    }

    /* The address pill. */
    int px, py, pw, ph;
    pill_rect(&px, &py, &pw, &ph);
    canvas_round_rect(&canvas, px, py, pw, ph, ph / 2, COL_PILL);
    canvas_round_border(&canvas, px, py, pw, ph, ph / 2,
                        address_focus ? 2 : 1,
                        address_focus ? COL_ACCENT : COL_BORDER);

    /* What kind of connection this page came over, said once, in the
     * place a reader looks for it. */
    int icon_x = px + 12, icon_y = py + (ph - 14) / 2;
    if (have_current && strcmp(current.scheme, "https") == 0) {
        draw_lock(icon_x, icon_y, COL_SECURE);
    } else {
        draw_globe(icon_x, icon_y, COL_MUTED);
    }

    int text_x = icon_x + 20;
    int room = (px + pw - 12 - text_x) / CELL_W;
    if (room < 1) room = 1;

    /* The end of a long address is the part that changes, and it is
     * what a reader is looking for. */
    const char *shown = address;
    int len = (int)strlen(address);
    if (len > room) shown = address + (len - room);

    canvas_text(&canvas, text_x, py + (ph - CELL_H) / 2, shown, COL_TEXT, 1, 0,
                0);

    if (address_focus) {
        int caret = address_caret;
        if (shown != address) caret -= (int)(shown - address);
        if (caret < 0) caret = 0;
        canvas_fill(&canvas, text_x + caret * CELL_W, py + 6, 2, ph - 12,
                    COL_ACCENT);
    }

    /* A page on its way says so, in the place a progress bar goes. */
    if (loading) {
        canvas_fill(&canvas, 0, BAR_H - 3, win_w, 3, COL_ACCENT);
    }
}

static void draw_status(void) {
    int y = win_h - STATUS_H;
    canvas_fill(&canvas, 0, y, win_w, STATUS_H, COL_STATUS);
    canvas_fill(&canvas, 0, y, win_w, 1, COL_BORDER);

    /* Hovering a link answers "where does this go?" before the click,
     * which is the whole reason a status bar survived. */
    const char *text = hover_link[0] != '\0' ? hover_link : status;
    uint32_t ink = hover_link[0] != '\0' ? COL_MUTED
                                         : (status_error ? COL_ERROR
                                                         : COL_MUTED);

    int room = (win_w - 20) / CELL_W;
    char trimmed[512];
    snprintf(trimmed, sizeof(trimmed), "%s", text);
    if (room > 3 && (int)strlen(trimmed) > room) {
        trimmed[room - 3] = '.';
        trimmed[room - 2] = '.';
        trimmed[room - 1] = '.';
        trimmed[room] = '\0';
    }
    canvas_text(&canvas, 10, y + (STATUS_H - CELL_H) / 2, trimmed, ink, 1, 0,
                0);
}

static void draw_page(void) {
    int top = BAR_H;
    int view_h = page_view_h();

    /* Everything below is clipped to the page's part of the window,
     * so a line that is half scrolled off is half drawn instead of
     * landing on the toolbar. */
    canvas_clip(&canvas, top, top + view_h);
    canvas_fill(&canvas, 0, top, win_w, view_h, COL_PAGE);

    for (int i = 0; i < page.nitems; i++) {
        const struct paint_item *it = &page.items[i];
        int y = it->y - scroll + top;

        /* Off-screen items are skipped rather than clipped: a long
         * page is mostly off-screen, and this is the whole of the
         * scrolling cost. */
        if (y + it->h <= top || y >= top + view_h) continue;

        if (it->type == ITEM_RECT) {
            canvas_fill(&canvas, it->x, y, it->w, it->h, it->color);
        } else if (it->type == ITEM_IMAGE) {
            canvas_image(&canvas, it->x, y, it->w, it->h, it->img);
        } else if (it->text != NULL) {
            canvas_text(&canvas, it->x, y, it->text, it->color, it->scale,
                        it->bold, it->underline);
        }
    }

    /*
     * The contents of the form controls, drawn after the boxes rather
     * than as part of them: what is in a field changes with every
     * keystroke, and laying the page out again to see a letter appear
     * would be absurd.
     */
    for (int i = 0; i < page.nfields; i++) {
        const struct doc_field *f = &page.fields[i];
        int y = f->y - scroll + top;
        if (y + f->h <= top || y >= top + view_h) continue;

        int focused = f->node == focus_node;
        if (focused) {
            /* A second border inside the first, in the accent colour:
             * the reader has to be able to see where the typing is
             * going. */
            canvas_fill(&canvas, f->x + 1, y + 1, f->w - 2, 1, COL_FOCUS);
            canvas_fill(&canvas, f->x + 1, y + f->h - 2, f->w - 2, 1,
                        COL_FOCUS);
            canvas_fill(&canvas, f->x + 1, y + 1, 1, f->h - 2, COL_FOCUS);
            canvas_fill(&canvas, f->x + f->w - 2, y + 1, 1, f->h - 2,
                        COL_FOCUS);
        }

        if (f->type == FIELD_CHECKBOX || f->type == FIELD_RADIO) continue;

        char text[FIELD_VALUE_MAX];
        layout_field_text(f->node, f->type, text, sizeof(text));

        /* Text is measured in characters: the value may be UTF-8, and
         * a letter with an accent on it is still one cell wide. */
        int len = layout_columns(text);
        if (f->password) {
            if (len >= (int)sizeof(text)) len = (int)sizeof(text) - 1;
            memset(text, '*', (size_t)len);
            text[len] = '\0';
        }

        int room = (f->w - 2 * FIELD_PAD) / CELL_W;
        if (room < 1) room = 1;

        int tx = f->x + FIELD_PAD;
        int ty = y + (f->h - CELL_H) / 2;
        const char *shown = text;
        int from = 0;

        if (f->type == FIELD_BUTTON || f->type == FIELD_SELECT) {
            if (len > room) text[layout_column_bytes(text, room)] = '\0';
            else tx += (f->w - 2 * FIELD_PAD - len * CELL_W) / 2;
        } else {
            if (f->type == FIELD_TEXTAREA) ty = y + FIELD_PAD;
            /* A field shows the end of what was typed, which is where
             * the caret is. */
            if (len > room) {
                from = len - room;
                shown = text + layout_column_bytes(text, from);
            }
        }

        canvas_text(&canvas, tx, ty, shown, COL_FIELD, 1, 0, 0);

        if (f->type == FIELD_SELECT) {
            /* A hint that there is more than one choice, in the space
             * the measurement left for it. */
            canvas_text(&canvas, f->x + f->w - FIELD_PAD - CELL_W, ty, "v",
                        COL_FIELD, 1, 0, 0);
        }

        if (focused && (f->type == FIELD_TEXT || f->type == FIELD_TEXTAREA)) {
            /* The caret is counted in bytes as the value is edited,
             * and drawn in cells. */
            const char *value = dom_attr(f->node, "value");
            int caret = 0;
            if (value != NULL) {
                int bytes = field_caret;
                if (bytes > (int)strlen(value)) bytes = (int)strlen(value);
                caret = utf8_columns(value, (size_t)bytes) - from;
            }
            if (caret < 0) caret = 0;
            if (caret > room) caret = room;
            canvas_fill(&canvas, tx + caret * CELL_W, ty, 1, CELL_H, COL_FIELD);
        }
    }

    /* A scrollbar, when there is more page than window. It is drawn
     * as a groove with a rounded thumb, and it lights up under the
     * pointer, because something that can be dragged should look as
     * if it can be. */
    int track_y, track_h, thumb_y, thumb_h;
    if (scrollbar_metrics(&track_y, &track_h, &thumb_y, &thumb_h)) {
        int x = win_w - SCROLLBAR_W;
        canvas_fill(&canvas, x, track_y, SCROLLBAR_W, track_h, COL_TRACK);
        canvas_fill(&canvas, x, track_y, 1, track_h, COL_BORDER);
        uint32_t ink = (drag_thumb || hover_thumb) ? COL_THUMB_HOT
                                                   : COL_THUMB;
        canvas_round_rect(&canvas, x + 3, thumb_y + 1, SCROLLBAR_W - 6,
                          thumb_h - 2, (SCROLLBAR_W - 6) / 2, ink);
    }

    canvas_clip(&canvas, 0, canvas.h);
}

static void redraw(void) {
    if (canvas.px == NULL) return;
    draw_chrome();
    draw_page();
    draw_status();
    hx_image(win, 0, 0, (unsigned)win_w, (unsigned)win_h, canvas.px);
    hx_commit(win);
}

static void set_status(int error, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(status, sizeof(status), fmt, ap);
    va_end(ap);
    status_error = error;
}

/* ---- loading ---- */

static void relayout(void) {
    if (document_tree == NULL) {
        layout_free(&page);
        return;
    }
    int width = win_w - SCROLLBAR_W; /* leave room for the scrollbar */
    layout_document(document_tree, width > 64 ? width : 64, &page);

    int max_scroll = page.height - page_view_h();
    if (max_scroll < 0) max_scroll = 0;
    if (scroll > max_scroll) scroll = max_scroll;
}

/* ---- fetching a page's images ---- */

/*
 * A picture written into the markup instead of fetched:
 *   data:image/png;base64,iVBORw0K...
 * Only the base64 form is worth handling - the percent-encoded form
 * is longer than the bytes it carries, so nothing uses it for images.
 */
static int decode_data_url(const char *src, struct image *out) {
    const char *comma = strchr(src, ',');
    if (comma == NULL) return -1;
    if (strstr(src, ";base64") == NULL || strstr(src, ";base64") > comma) {
        return -1;
    }

    const char *in = comma + 1;
    size_t len = strlen(in);
    unsigned char *raw = malloc(len / 4 * 3 + 4);
    if (raw == NULL) return -1;

    static const char SET[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned int acc = 0;
    int bits = 0;
    size_t at = 0;
    for (size_t i = 0; i < len; i++) {
        const char *p = strchr(SET, in[i]);
        if (p == NULL || in[i] == '\0') continue;   /* padding, newlines */
        acc = (acc << 6) | (unsigned int)(p - SET);
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            raw[at++] = (unsigned char)((acc >> bits) & 0xFFu);
        }
    }

    int rc = image_decode(raw, at, out);
    free(raw);
    return rc;
}

/*
 * Every <img> on the page, fetched after the text is already on
 * screen. That order is the point: a page whose pictures are still
 * coming is readable, and each one that arrives redraws the page it
 * belongs to rather than holding the whole thing back.
 *
 * A page with thirty pictures on it would otherwise hold the browser
 * for as long as it takes to fetch thirty pictures, so a key press
 * stops it: the text is already there, and the reader has said what
 * they came for.
 */
static int loading_interrupted(void) {
    struct hx_event ev;
    while (hx_next_event(&ev, 0) > 0) {
        if (ev.type == HX_EV_KEY || ev.type == HX_EV_CLOSE ||
            (ev.type == HX_EV_POINTER && ev.detail == HX_PTR_PRESS)) {
            return 1;
        }
    }
    return 0;
}

static void load_images(void) {
    if (document_tree == NULL || !have_current) return;

    char saved[sizeof(status)];
    snprintf(saved, sizeof(saved), "%s", status);
    int saved_error = status_error;
    int drawn = 0;
    int stopped = 0;

    for (struct dom_node *n = document_tree; n != NULL;
         n = dom_next(n, document_tree)) {
        if (n->type != DOM_ELEMENT || strcmp(n->name, "img") != 0) continue;
        if (n->style != NULL && n->style->display == DISP_NONE) continue;

        const char *src = dom_attr(n, "src");
        if (src == NULL || *src == '\0' || strlen(src) >= sizeof(images[0].src)) {
            continue;
        }
        if (image_known(src)) continue;
        if (image_count >= IMAGE_MAX) break;
        if (loading_interrupted()) {
            stopped = 1;
            break;
        }

        int slot = image_count++;
        snprintf(images[slot].src, sizeof(images[slot].src), "%s", src);
        images[slot].failed = 1;

        if (strncmp(src, "data:", 5) == 0) {
            if (decode_data_url(src, &images[slot].img) == 0) {
                images[slot].failed = 0;
            }
        } else {
            struct url u;
            if (url_parse(&u, src, &current) != 0) continue;

            char text[1200];
            url_format(&u, text, sizeof(text));
            set_status(0, "image %d: %s", slot + 1, text);
            redraw();

            struct http_response r;
            if (http_get(&u, &r) == 0 && r.status == 200 && r.body != NULL &&
                r.body_len > 0 && r.body_len <= IMAGE_MAX_BYTES) {
                if (image_decode(r.body, r.body_len, &images[slot].img) == 0) {
                    images[slot].failed = 0;
                }
            }
            http_response_free(&r);
        }

        if (!images[slot].failed) {
            /* Redrawing per image is what makes a page fill in rather
             * than jump: layout has to run again anyway, because the
             * picture's size is what the line was missing. */
            relayout();
            drawn++;
        }
    }

    if (stopped) set_status(0, "%s (pictures stopped)", saved);
    else set_status(saved_error, "%s", saved);
    if (drawn > 0 || image_count > 0) redraw();
}

/* Replace the document with a page built here rather than fetched -
 * an error, or the welcome screen. Going through the same parser and
 * layout as a real page means there is only one renderer. */
static void show_local_page(const char *html) {
    images_clear();
    focus_node = NULL;
    js_free(script);
    script = NULL;
    dom_free(document_tree);
    document_tree = dom_parse(html, strlen(html));
    if (document_tree != NULL) css_apply(document_tree);
    scroll = 0;
    relayout();
}

/* ---- video ---- */

static int is_media_type(const char *content_type) {
    return content_type != NULL &&
           (strncmp(content_type, "video/", 6) == 0 ||
            strncmp(content_type, "audio/", 6) == 0);
}

/*
 * Save what was fetched and start the player on it. The file goes to
 * /tmp because that is where a downloaded thing belongs and because
 * hxvideo takes a path, not a socket: the browser fetches, the player
 * plays, and neither has to know how the other works.
 */
static int play_media(const struct http_response *r) {
    if (r->body == NULL || r->body_len == 0) {
        set_status(1, "the server sent no video");
        return -1;
    }

    FILE *f = fopen(VIDEO_PATH, "wb");
    if (f == NULL) {
        set_status(1, "cannot write %s", VIDEO_PATH);
        return -1;
    }
    size_t wrote = fwrite(r->body, 1, r->body_len, f);
    fclose(f);

    if (wrote != r->body_len) {
        set_status(1, "only %lu of %lu bytes could be saved",
                   (unsigned long)wrote, (unsigned long)r->body_len);
        return -1;
    }

    char *argv[3];
    argv[0] = (char *)"/bin/hxvideo";
    argv[1] = (char *)VIDEO_PATH;
    argv[2] = NULL;
    if (hx_spawn("/bin/hxvideo", argv) < 0) {
        set_status(1, "cannot start the video player");
        return -1;
    }

    set_status(0, "%lu KB handed to the video player",
               (unsigned long)(r->body_len / 1024));
    return 0;
}

/* ---- what a script may ask the browser for ---- */

static void navigate(const char *text, const struct url *base);

static void script_navigate(void *ctx, const char *url) {
    (void)ctx;
    /* Not now: the document this script belongs to is still in use.
     * The browser follows it once the script has finished. */
    snprintf(pending_url, sizeof(pending_url), "%s", url != NULL ? url : "");
}

static void script_log(void *ctx, const char *text) {
    (void)ctx;
    /* The console goes to the serial log, where a developer can read
     * it, and the last line goes on the status bar, where the reader
     * can. */
    fprintf(stderr, "clint: %s\n", text != NULL ? text : "");
    set_status(0, "%s", text != NULL ? text : "");
}

/* A script that failed is a developer's problem, not the reader's:
 * every real page has one, and none of them are worth a line of the
 * browser's own furniture. */
static void script_failed(void *ctx, const char *text) {
    (void)ctx;
    fprintf(stderr, "clint: script error: %s\n", text != NULL ? text : "");
}

static void script_alert(void *ctx, const char *text) {
    (void)ctx;
    fprintf(stderr, "clint: alert: %s\n", text != NULL ? text : "");
    set_status(1, "%s", text != NULL ? text : "");
}

static void script_changed(void *ctx) {
    (void)ctx;
    dom_dirty = 1;
}

static const char *script_location(void *ctx) {
    (void)ctx;
    static char text[1200];
    if (!have_current) return "";
    url_format(&current, text, sizeof(text));
    return text;
}

static unsigned long script_now(void *ctx) {
    (void)ctx;
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) return 0;
    return (unsigned long)ts.tv_sec * 1000ul +
           (unsigned long)(ts.tv_nsec / 1000000);
}

/* <script src="...">, resolved against the page it was found on -
 * the same `current`/url_parse pattern load_images() uses for <img
 * src>. data: URIs are not handled here (real-world external scripts
 * are fetched, not inlined that way, and the page's own inline
 * <script> already covers the inline case). A script that cannot be
 * fetched, or is too large, costs that one <script> tag, not the
 * page - js_run_document() treats a NULL return as "skip this one". */
static char *script_fetch(void *ctx, const char *src) {
    (void)ctx;
    if (!have_current || src == NULL || *src == '\0') return NULL;

    struct url u;
    if (url_parse(&u, src, &current) != 0) return NULL;

    struct http_response r;
    if (http_get(&u, &r) != 0) return NULL;

    char *text = NULL;
    if (r.status == 200 && r.body != NULL && r.body_len > 0 &&
        r.body_len <= SCRIPT_MAX_BYTES) {
        text = malloc(r.body_len + 1);
        if (text != NULL) {
            memcpy(text, r.body, r.body_len);
            text[r.body_len] = '\0';
        }
    }
    http_response_free(&r);
    return text;
}

static int script_click_probe(struct dom_node *node) {
    return script != NULL && js_has_click_handler(script, node);
}

/* A script changed the document: style what is new, lay it out again
 * and put it on screen. Nothing else in the browser has to know that
 * a script was involved. */
static void refresh_after_script(void) {
    if (!dom_dirty) return;
    dom_dirty = 0;
    if (document_tree == NULL) return;

    focus_node = NULL;
    css_apply(document_tree);
    relayout();
    redraw();
}

static void run_page_scripts(void) {
    if (document_tree == NULL) return;

    js_free(script);
    script = NULL;

    struct js_host host;
    memset(&host, 0, sizeof(host));
    host.navigate = script_navigate;
    host.log = script_log;
    host.alert = script_alert;
    host.script_error = script_failed;
    host.changed = script_changed;
    host.location = script_location;
    host.now_ms = script_now;
    host.fetch_script = script_fetch;

    script = js_create(document_tree, &host);
    if (script == NULL) return;

    dom_dirty = 0;
    pending_url[0] = '\0';
    js_run_document(script);
    refresh_after_script();
}

/*
 * One page load, whether it came from a link or from a form:
 * `post_body` is NULL for a GET and the encoded fields for a POST.
 * Everything after the request is the same either way, which is the
 * point of having one of these rather than two.
 */
static void load_request(const struct url *u, const char *post_body,
                         int record_history) {
    char text[1200];
    url_format(u, text, sizeof(text));
    loading = 1;
    hover_link[0] = '\0';
    set_status(0, "loading %s", text);
    redraw();

    struct http_response r;
    int failed = (post_body != NULL ? http_post(u, post_body, &r)
                                    : http_get(u, &r)) != 0;
    loading = 0;
    if (failed) {
        char html[1024];
        snprintf(html, sizeof(html),
                 "<html><body><h2>Cannot open this page</h2>"
                 "<p>%s</p><p><b>%s</b></p></body></html>",
                 text, http_last_error());
        show_local_page(html);
        set_status(1, "%s", http_last_error());
        snprintf(page_title, sizeof(page_title), "Error");
        redraw();
        return;
    }

    /* Video and audio are not documents: they go to the player, and
     * the page the reader was on stays exactly where it was - which
     * is why this comes before anything is torn down. */
    if (is_media_type(r.content_type)) {
        set_status(0, "starting the player...");
        redraw();
        play_media(&r);
        http_response_free(&r);
        redraw();
        return;
    }

    if (record_history && have_current && history_len < HISTORY_MAX) {
        history[history_len++] = current;
    }
    current = r.final_url;
    have_current = 1;
    url_format(&current, address, sizeof(address));
    address_caret = (int)strlen(address);

    images_clear();
    focus_node = NULL;
    js_free(script);
    script = NULL;
    dom_free(document_tree);
    document_tree = NULL;

    /* Everything above this line works in UTF-8, so a page that is
     * not becomes it here - before the parser, which is the last
     * moment the encoding is still knowable. */
    const char *body = r.body != NULL ? r.body : "";
    size_t body_len = r.body != NULL ? r.body_len : 0;

    char label[32];
    charset_of(r.content_type, body, body_len, label, sizeof(label));

    size_t converted_len = 0;
    char *converted = charset_to_utf8(label, body, body_len, &converted_len);
    if (converted != NULL) {
        body = converted;
        body_len = converted_len;
    }

    /* Anything that is not HTML is shown as text rather than parsed
     * as markup, which is what makes viewing a stylesheet or a plain
     * file useful instead of blank. */
    int is_html = strstr(r.content_type, "html") != NULL ||
                  r.content_type[0] == '\0';
    if (is_html) {
        document_tree = dom_parse(body, body_len);
    } else {
        size_t cap = body_len + 64;
        char *wrapped = malloc(cap);
        if (wrapped != NULL) {
            snprintf(wrapped, cap, "<html><body><pre>%s</pre></body></html>",
                     body);
            document_tree = dom_parse(wrapped, strlen(wrapped));
            free(wrapped);
        }
    }

    free(converted);

    if (document_tree != NULL) {
        css_apply(document_tree);

        page_title[0] = '\0';
        for (struct dom_node *n = document_tree; n != NULL;
             n = dom_next(n, document_tree)) {
            if (n->type == DOM_ELEMENT && strcmp(n->name, "title") == 0) {
                dom_text_content(n, page_title, sizeof(page_title));
                break;
            }
        }
    }

    scroll = 0;
    relayout();

    char title[HX_TITLE_MAX];
    snprintf(title, sizeof(title), "%s - Clint",
             page_title[0] != '\0' ? page_title : current.host);
    hx_set_title(win, title);

    if (r.secure) {
        set_status(0, "%d  %s  https (%s)", r.status,
                   r.content_type[0] ? r.content_type : "?",
                   r.tls_warning != NULL ? "dates unchecked" : "verified");
    } else {
        set_status(0, "%d  %s  http (not encrypted)", r.status,
                   r.content_type[0] ? r.content_type : "?");
    }

    http_response_free(&r);
    redraw();

    load_images();

    /* Scripts run after the page is drawn, so a slow one cannot keep
     * the reader from seeing anything at all. */
    run_page_scripts();
    if (pending_url[0] != '\0') {
        char target[sizeof(pending_url)];
        snprintf(target, sizeof(target), "%s", pending_url);
        pending_url[0] = '\0';
        navigate(target, &current);
    }
}

static void load(const struct url *u, int record_history) {
    load_request(u, NULL, record_history);
}

static void load_post(const struct url *u, const char *body) {
    load_request(u, body, 1);
}

static void navigate(const char *text, const struct url *base) {
    struct url u;
    if (url_parse(&u, text, base) != 0) {
        set_status(1, "%s", http_last_error());
        redraw();
        return;
    }
    forward_len = 0;   /* a new branch: what was ahead is not any more */
    load(&u, 1);
}

static void go_back(void) {
    if (history_len == 0) {
        set_status(0, "no page to go back to");
        redraw();
        return;
    }
    if (have_current && forward_len < HISTORY_MAX) {
        forward[forward_len++] = current;
    }
    struct url u = history[--history_len];
    load(&u, 0);
}

static void go_forward(void) {
    if (forward_len == 0) {
        set_status(0, "no page to go forward to");
        redraw();
        return;
    }
    if (have_current && history_len < HISTORY_MAX) {
        history[history_len++] = current;
    }
    struct url u = forward[--forward_len];
    load(&u, 0);
}

static void reload_page(void) {
    if (!have_current) return;
    struct url u = current;
    load(&u, 0);
}

/* ---- forms ---- */

/*
 * A control belongs to the <form> it sits inside. HTML has a form
 * attribute for the ones that do not, but no page Clint is meant for
 * uses it, and an ancestor walk is what the other 99% need.
 */
static struct dom_node *form_of(struct dom_node *n) {
    for (struct dom_node *p = n; p != NULL; p = p->parent) {
        if (p->type == DOM_ELEMENT && strcmp(p->name, "form") == 0) return p;
    }
    return NULL;
}

static int is_unreserved(unsigned char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' ||
           c == '~';
}

/* application/x-www-form-urlencoded: spaces become +, everything that
 * is not unreserved becomes %XX. Returns how much was appended. */
static size_t url_encode(char *out, size_t size, const char *in) {
    static const char HEX[] = "0123456789ABCDEF";
    size_t at = 0;
    for (const unsigned char *p = (const unsigned char *)in;
         *p != '\0' && at + 4 < size; p++) {
        if (is_unreserved(*p)) {
            out[at++] = (char)*p;
        } else if (*p == ' ') {
            out[at++] = '+';
        } else {
            out[at++] = '%';
            out[at++] = HEX[*p >> 4];
            out[at++] = HEX[*p & 0x0F];
        }
    }
    out[at] = '\0';
    return at;
}

static int attr_is(const struct dom_node *n, const char *name,
                   const char *want) {
    const char *v = dom_attr(n, name);
    if (v == NULL) return 0;
    while (*v != '\0' && *want != '\0') {
        char a = *v >= 'A' && *v <= 'Z' ? (char)(*v + 32) : *v;
        if (a != *want) return 0;
        v++;
        want++;
    }
    return *v == '\0' && *want == '\0';
}

/* The value a control contributes, as the document holds it - not the
 * folded ASCII that was drawn. */
static const char *control_value(struct dom_node *n, char *buf, size_t size) {
    const char *value = dom_attr(n, "value");

    if (strcmp(n->name, "textarea") == 0) {
        if (value != NULL) return value;
        dom_text_content(n, buf, size);
        return buf;
    }
    if (strcmp(n->name, "select") == 0) {
        for (struct dom_node *o = n->first; o != NULL; o = o->next) {
            if (o->type != DOM_ELEMENT || strcmp(o->name, "option") != 0) {
                continue;
            }
            if (dom_attr(o, "selected") == NULL && o != n->first) continue;
            const char *ov = dom_attr(o, "value");
            if (ov != NULL) return ov;
            dom_text_content(o, buf, size);
            return buf;
        }
        return "";
    }
    return value != NULL ? value : "";
}

/*
 * Turn the form into a query string. `activated` is the button that
 * was pressed, because a form with two submit buttons means two
 * different things and only the pressed one is sent - that is how
 * "Google Search" and "I'm Feeling Lucky" are told apart.
 */
static size_t form_query(struct dom_node *form, struct dom_node *activated,
                         char *out, size_t size) {
    size_t at = 0;

    for (struct dom_node *n = form; n != NULL; n = dom_next(n, form)) {
        if (n->type != DOM_ELEMENT) continue;

        int is_input = strcmp(n->name, "input") == 0;
        int is_select = strcmp(n->name, "select") == 0;
        int is_textarea = strcmp(n->name, "textarea") == 0;
        int is_button = strcmp(n->name, "button") == 0;
        if (!is_input && !is_select && !is_textarea && !is_button) continue;

        const char *name = dom_attr(n, "name");
        if (name == NULL || *name == '\0') continue;
        if (dom_attr(n, "disabled") != NULL) continue;

        if (is_input || is_button) {
            if (attr_is(n, "type", "checkbox") || attr_is(n, "type", "radio")) {
                if (dom_attr(n, "checked") == NULL) continue;
            } else if (attr_is(n, "type", "submit") ||
                       attr_is(n, "type", "reset") ||
                       attr_is(n, "type", "button") || is_button) {
                if (n != activated) continue;
                if (attr_is(n, "type", "reset")) continue;
            } else if (attr_is(n, "type", "file") ||
                       attr_is(n, "type", "image")) {
                continue;
            }
        }

        char buf[FIELD_VALUE_MAX];
        const char *value = control_value(n, buf, sizeof(buf));
        if ((attr_is(n, "type", "checkbox") || attr_is(n, "type", "radio")) &&
            dom_attr(n, "value") == NULL) {
            value = "on";
        }

        if (at + 2 >= size) break;
        if (at > 0) out[at++] = '&';
        at += url_encode(out + at, size - at, name);
        if (at + 2 >= size) break;
        out[at++] = '=';
        at += url_encode(out + at, size - at, value);
    }

    out[at] = '\0';
    return at;
}

/*
 * Send the form. GET puts the fields in the query string and replaces
 * whatever query the action had; POST sends them as the body. Both
 * end up in load(), so a form's answer is an ordinary page with
 * ordinary history behind it.
 */
static void submit_form(struct dom_node *form, struct dom_node *activated) {
    if (form == NULL || !have_current) return;

    char query[4096];
    form_query(form, activated, query, sizeof(query));

    const char *action = dom_attr(form, "action");
    char target[2048];
    if (action == NULL || *action == '\0') {
        url_format(&current, target, sizeof(target));
    } else {
        snprintf(target, sizeof(target), "%s", action);
    }

    if (attr_is(form, "method", "post")) {
        struct url u;
        if (url_parse(&u, target, &current) != 0) {
            set_status(1, "%s", http_last_error());
            redraw();
            return;
        }
        focus_node = NULL;
        load_post(&u, query);
        return;
    }

    /* The action's own query is replaced, not added to. */
    char *q = strchr(target, '?');
    if (q != NULL) *q = '\0';

    char full[8192];
    snprintf(full, sizeof(full), "%s?%s", target, query);
    focus_node = NULL;
    navigate(full, &current);
}

/* ---- editing a field ---- */

static void field_set_value(struct dom_node *n, const char *value) {
    dom_set_attr(n, "value", value);
}

static void field_insert(struct dom_node *n, char c) {
    char buf[FIELD_VALUE_MAX];
    const char *value = dom_attr(n, "value");
    snprintf(buf, sizeof(buf), "%s", value != NULL ? value : "");

    int len = (int)strlen(buf);
    if (len + 1 >= (int)sizeof(buf)) return;
    if (field_caret > len) field_caret = len;

    memmove(buf + field_caret + 1, buf + field_caret,
            (size_t)(len - field_caret) + 1);
    buf[field_caret] = c;
    field_caret++;
    field_set_value(n, buf);
}

static void field_backspace(struct dom_node *n) {
    char buf[FIELD_VALUE_MAX];
    const char *value = dom_attr(n, "value");
    snprintf(buf, sizeof(buf), "%s", value != NULL ? value : "");

    int len = (int)strlen(buf);
    if (field_caret > len) field_caret = len;
    if (field_caret == 0) return;

    memmove(buf + field_caret - 1, buf + field_caret,
            (size_t)(len - field_caret) + 1);
    field_caret--;
    field_set_value(n, buf);
}

/*
 * Enter in a text field submits, which is what every form with one
 * field expects and what makes a search box a search box. The form's
 * first submit button counts as pressed - a form that says which
 * button was used has to be told something, and that is the one a
 * browser reports.
 */
static void field_enter(struct dom_node *n) {
    struct dom_node *form = form_of(n);
    struct dom_node *button = NULL;

    for (struct dom_node *o = form; o != NULL && button == NULL;
         o = dom_next(o, form)) {
        if (o->type != DOM_ELEMENT) continue;
        if (strcmp(o->name, "button") == 0 ||
            (strcmp(o->name, "input") == 0 && attr_is(o, "type", "submit"))) {
            if (dom_attr(o, "name") != NULL) button = o;
        }
    }
    submit_form(form, button);
}

static void focus_next_field(void) {
    if (page.nfields == 0) return;

    int at = -1;
    for (int i = 0; i < page.nfields; i++) {
        if (page.fields[i].node == focus_node) at = i;
    }
    for (int step = 1; step <= page.nfields; step++) {
        struct doc_field *f = &page.fields[(at + step) % page.nfields];
        if (f->type == FIELD_TEXT || f->type == FIELD_TEXTAREA) {
            focus_node = f->node;
            const char *value = dom_attr(f->node, "value");
            field_caret = value != NULL ? (int)strlen(value) : 0;
            return;
        }
    }
}

/* ---- input ---- */

static void scroll_to(int where) {
    int max_scroll = page.height - page_view_h();
    if (max_scroll < 0) max_scroll = 0;

    if (where < 0) where = 0;
    if (where > max_scroll) where = max_scroll;
    if (where == scroll) return;
    scroll = where;
    redraw();
}

static void scroll_by(int delta) {
    scroll_to(scroll + delta);
}

static void address_insert(char c) {
    size_t len = strlen(address);
    if (len + 1 >= sizeof(address)) return;
    if (address_caret > (int)len) address_caret = (int)len;

    memmove(address + address_caret + 1, address + address_caret,
            len - (size_t)address_caret + 1);
    address[address_caret] = c;
    address_caret++;
}

static void on_key(const struct hx_event *ev) {
    unsigned int k = ev->key;

    /* Ctrl+L focuses the address bar, as every browser does. highX
     * delivers Ctrl+letter as the control character itself, so 0x0C
     * is the one to look for. */
    if (k == 0x0C || ((ev->mods & HX_MOD_CTRL) != 0 && (k == 'l' || k == 'L'))) {
        address_focus = 1;
        address_caret = (int)strlen(address);
        redraw();
        return;
    }

    if (address_focus) {
        if (k == '\n' || k == '\r') {
            address_focus = 0;
            navigate(address, have_current ? &current : NULL);
            return;
        }
        if (k == 0x1B) { /* escape: put the real address back */
            address_focus = 0;
            if (have_current) url_format(&current, address, sizeof(address));
            address_caret = (int)strlen(address);
            redraw();
            return;
        }
        if (k == '\b' || k == 0x7F) {
            if (address_caret > 0) {
                size_t len = strlen(address);
                memmove(address + address_caret - 1, address + address_caret,
                        len - (size_t)address_caret + 1);
                address_caret--;
            }
            redraw();
            return;
        }
        if (k >= 0x20 && k < 0x7F) {
            address_insert((char)k);
            redraw();
            return;
        }
        return;
    }

    /*
     * A focused control takes the keys a text field needs and leaves
     * the rest - PageDown still scrolls the page while the caret is
     * in a search box, which is what a reader expects.
     */
    if (focus_node != NULL) {
        const char *value = dom_attr(focus_node, "value");
        int len = value != NULL ? (int)strlen(value) : 0;

        switch (k) {
        case '\n':
        case '\r':
            field_enter(focus_node);
            return;
        case 0x1B:
            focus_node = NULL;
            redraw();
            return;
        case '\t':
            focus_next_field();
            redraw();
            return;
        case '\b':
        case 0x7F:
            field_backspace(focus_node);
            redraw();
            return;
        case HX_KEY_LEFT:
            if (field_caret > 0) field_caret--;
            redraw();
            return;
        case HX_KEY_RIGHT:
            if (field_caret < len) field_caret++;
            redraw();
            return;
        case HX_KEY_HOME:
            field_caret = 0;
            redraw();
            return;
        case HX_KEY_END:
            field_caret = len;
            redraw();
            return;
        default:
            break;
        }
        if (k >= 0x20 && k < 0x7F) {
            field_insert(focus_node, (char)k);
            redraw();
            return;
        }
    }

    switch (k) {
    case HX_KEY_DOWN:  scroll_by(SCROLL_STEP); break;
    case HX_KEY_UP:    scroll_by(-SCROLL_STEP); break;
    case HX_KEY_PAGE_DOWN:  scroll_by(page_view_h() - CELL_H); break;
    case HX_KEY_PAGE_UP:  scroll_by(-(page_view_h() - CELL_H)); break;
    case HX_KEY_HOME:  scroll = 0; redraw(); break;
    case HX_KEY_END:   scroll_by(page.height); break;
    case ' ':          scroll_by(page_view_h() - CELL_H); break;
    case '\b':
    case 0x7F:         go_back(); break;
    case '\n':
    case '\r':
        address_focus = 1;
        address_caret = (int)strlen(address);
        redraw();
        break;
    default:
        break;
    }
}

/*
 * A click on a control. A text field takes the caret, a button sends
 * the form, and the ones with a state of their own change it - a
 * select cycles through its options rather than opening a menu, which
 * is a small lie that keeps every choice reachable with one button.
 */
static void activate_field(struct doc_field *f, int click_x) {
    struct dom_node *n = f->node;
    int type = f->type;
    int text_left = f->x + FIELD_PAD;

    if (type == FIELD_TEXT || type == FIELD_TEXTAREA) {
        focus_node = n;

        const char *value = dom_attr(n, "value");
        int cols = layout_columns(value);
        int room = (f->w - 2 * FIELD_PAD) / CELL_W;
        int from = cols > room ? cols - room : 0;

        int at = from + (click_x - text_left) / CELL_W;
        if (at < 0) at = 0;
        if (at > cols) at = cols;
        field_caret = (int)layout_column_bytes(value, at);
        redraw();
        return;
    }

    focus_node = NULL;

    if (type == FIELD_BUTTON) {
        if (script != NULL) {
            int cancelled = js_click(script, n);
            refresh_after_script();
            if (pending_url[0] != '\0') {
                char target[sizeof(pending_url)];
                snprintf(target, sizeof(target), "%s", pending_url);
                pending_url[0] = '\0';
                navigate(target, have_current ? &current : NULL);
                return;
            }
            if (cancelled) return;
        }
        if (attr_is(n, "type", "reset")) {
            set_status(0, "reset is not implemented");
            redraw();
            return;
        }
        if (attr_is(n, "type", "button")) {
            /* A plain button with no handler does nothing, which is
             * exactly what it does in any browser. */
            redraw();
            return;
        }
        submit_form(form_of(n), n);
        return;
    }

    if (type == FIELD_CHECKBOX) {
        if (dom_attr(n, "checked") != NULL) dom_remove_attr(n, "checked");
        else dom_set_attr(n, "checked", "checked");
    } else if (type == FIELD_RADIO) {
        struct dom_node *form = form_of(n);
        const char *name = dom_attr(n, "name");
        if (form != NULL && name != NULL) {
            for (struct dom_node *o = form; o != NULL; o = dom_next(o, form)) {
                if (o->type != DOM_ELEMENT || strcmp(o->name, "input") != 0) {
                    continue;
                }
                if (!attr_is(o, "type", "radio")) continue;
                const char *other = dom_attr(o, "name");
                if (other != NULL && strcmp(other, name) == 0) {
                    dom_remove_attr(o, "checked");
                }
            }
        }
        dom_set_attr(n, "checked", "checked");
    } else if (type == FIELD_SELECT) {
        struct dom_node *first = NULL, *chosen = NULL, *next = NULL;
        for (struct dom_node *o = n->first; o != NULL; o = o->next) {
            if (o->type != DOM_ELEMENT || strcmp(o->name, "option") != 0) {
                continue;
            }
            if (first == NULL) first = o;
            if (chosen != NULL && next == NULL) next = o;
            if (dom_attr(o, "selected") != NULL) chosen = o;
        }
        if (chosen == NULL) {
            chosen = first;
            next = NULL;
            for (struct dom_node *o = first != NULL ? first->next : NULL;
                 o != NULL; o = o->next) {
                if (o->type == DOM_ELEMENT && strcmp(o->name, "option") == 0) {
                    next = o;
                    break;
                }
            }
        }
        if (chosen != NULL) dom_remove_attr(chosen, "selected");
        dom_set_attr(next != NULL ? next : first, "selected", "selected");
    } else {
        set_status(0, "Clint cannot fill in this kind of field");
    }

    /* The mark, and the width of a select, are drawn by layout. */
    relayout();
    redraw();
}

/* Which toolbar button is at this point, or BTN_NONE. */
static int button_at(int x, int y) {
    for (int i = 0; i < BTN_COUNT; i++) {
        int bx, by, size;
        button_rect(i, &bx, &by, &size);
        if (x >= bx && y >= by && x < bx + size && y < by + size) return i;
    }
    return BTN_NONE;
}

static void on_click(const struct hx_event *ev) {
    if (ev->y < BAR_H) {
        int button = button_at(ev->x, ev->y);
        if (button != BTN_NONE) {
            address_focus = 0;
            if (!button_enabled(button)) {
                redraw();
                return;
            }
            if (button == BTN_BACK) go_back();
            else if (button == BTN_FORWARD) go_forward();
            else reload_page();
            return;
        }

        /* The pill takes the caret where it was clicked, which is
         * what a text field does everywhere else. */
        int px, py, pw, ph;
        pill_rect(&px, &py, &pw, &ph);
        address_focus = 1;
        int len = (int)strlen(address);
        int text_x = px + 32;
        int room = (px + pw - 12 - text_x) / CELL_W;
        int from = len > room ? len - room : 0;
        address_caret = from + (ev->x - text_x) / CELL_W;
        if (address_caret < 0) address_caret = 0;
        if (address_caret > len) address_caret = len;
        redraw();
        return;
    }
    address_focus = 0;

    /* The scrollbar sits over the page, so it is asked first. The
     * thumb starts a drag; the groove above or below it is a page
     * step, which is what clicking a track does everywhere. */
    int track_y, track_h, thumb_y, thumb_h;
    if (ev->x >= win_w - SCROLLBAR_W &&
        scrollbar_metrics(&track_y, &track_h, &thumb_y, &thumb_h) &&
        ev->y >= track_y && ev->y < track_y + track_h) {
        if (ev->y >= thumb_y && ev->y < thumb_y + thumb_h) {
            drag_thumb = 1;
            drag_grab_y = ev->y - thumb_y;
        } else {
            scroll_by(ev->y < thumb_y ? -(page_view_h() - CELL_H)
                                      : page_view_h() - CELL_H);
        }
        return;
    }

    int doc_x = ev->x;
    int doc_y = ev->y - BAR_H + scroll;

    /* A control is on top of whatever link surrounds it: clicking a
     * search box inside a linked banner types, it does not follow. */
    struct doc_field *f = layout_field_at(&page, doc_x, doc_y);
    if (f != NULL) {
        activate_field(f, doc_x);
        return;
    }
    focus_node = NULL;

    const struct doc_link *k = layout_link_at(&page, doc_x, doc_y);
    if (k != NULL) {
        /* Both of these are copied out first: running a handler can
         * change the document, and the display list this came from is
         * rebuilt when it does. */
        char href[1024];
        snprintf(href, sizeof(href), "%s", k->href != NULL ? k->href : "");
        struct dom_node *node = k->node;

        int cancelled = 0;
        if (node != NULL && script != NULL) {
            cancelled = js_click(script, node);
            refresh_after_script();
            if (pending_url[0] != '\0') {
                char target[sizeof(pending_url)];
                snprintf(target, sizeof(target), "%s", pending_url);
                pending_url[0] = '\0';
                navigate(target, have_current ? &current : NULL);
                return;
            }
        }
        if (!cancelled && href[0] != '\0') {
            navigate(href, have_current ? &current : NULL);
        }
        return;
    }
    redraw();
}

/*
 * The pointer moving is not an event a browser can ignore: it is how
 * a button lights up and how a link says where it goes. Both are
 * cheap, and the redraw only happens when something actually
 * changed.
 */
/* Turn a thumb position into a scroll offset - the inverse of what
 * scrollbar_metrics() computes, so dragging the thumb to the bottom
 * of the groove lands exactly on the last line of the page. */
static void drag_to(int pointer_y) {
    int track_y, track_h, thumb_y, thumb_h;
    if (!scrollbar_metrics(&track_y, &track_h, &thumb_y, &thumb_h)) return;

    int span = track_h - thumb_h;
    if (span <= 0) return;
    int at = pointer_y - drag_grab_y - track_y;
    if (at < 0) at = 0;
    if (at > span) at = span;
    scroll_to((page.height - page_view_h()) * at / span);
}

static void on_motion(const struct hx_event *ev) {
    int changed_anything = 0;

    if (drag_thumb) {
        drag_to(ev->y);
        return;
    }

    int track_y, track_h, thumb_y, thumb_h;
    int over = ev->x >= win_w - SCROLLBAR_W &&
               scrollbar_metrics(&track_y, &track_h, &thumb_y, &thumb_h) &&
               ev->y >= thumb_y && ev->y < thumb_y + thumb_h;
    if (over != hover_thumb) {
        hover_thumb = over;
        changed_anything = 1;
    }

    int button = ev->y < BAR_H ? button_at(ev->x, ev->y) : BTN_NONE;
    if (button != hover_button) {
        hover_button = button;
        changed_anything = 1;
    }

    char link[sizeof(hover_link)];
    link[0] = '\0';
    if (ev->y >= BAR_H && ev->y < win_h - STATUS_H) {
        const struct doc_link *k = layout_link_at(&page, ev->x,
                                                  ev->y - BAR_H + scroll);
        if (k != NULL && k->href != NULL) {
            snprintf(link, sizeof(link), "%s", k->href);
        }
    }
    if (strcmp(link, hover_link) != 0) {
        snprintf(hover_link, sizeof(hover_link), "%s", link);
        changed_anything = 1;
    }

    if (changed_anything) redraw();
}

/* ---- main ---- */

static const char WELCOME[] =
    "<html><head><title>Clint</title><style>"
    "body { margin: 0; background: #ffffff; color: #202124; }"
    ".head { background: #1a73e8; color: #ffffff; padding: 24px 28px 24px 28px; }"
    ".head h1 { margin: 0; color: #ffffff; }"
    ".head p { margin: 6px 0 0 0; color: #d2e3fc; }"
    ".body { padding: 20px 28px 20px 28px; }"
    ".card { border: 1px solid #dadce0; padding: 14px 16px 14px 16px;"
    "        margin: 0 0 14px 0; }"
    ".card h3 { margin: 0 0 8px 0; color: #1a73e8; }"
    "code { background: #f1f3f4; padding: 1px 4px 1px 4px; }"
    ".muted { color: #5f6368; }"
    "</style></head><body>"
    "<div class=\"head\"><h1>Clint</h1>"
    "<p>a browser for TUS</p></div>"
    "<div class=\"body\">"
    "<div class=\"card\"><h3>Getting there</h3>"
    "<p>Type an address in the bar above and press Enter, or click a "
    "link. The arrows go back and forward; the circle reloads.</p>"
    "<ul><li><a href=\"https://example.com/\">https://example.com/</a>"
    " - the smallest real page on the web</li>"
    "<li><a href=\"https://www.google.com/\">https://www.google.com/</a>"
    " - pictures, a form and Turkish text</li></ul></div>"
    "<div class=\"card\"><h3>Keys</h3>"
    "<ul><li><code>Enter</code> or <code>Ctrl+L</code> - edit the address</li>"
    "<li><code>Arrows</code>, <code>PageUp</code>/<code>PageDown</code>, "
    "<code>Home</code>/<code>End</code>, <code>Space</code> - scroll</li>"
    "<li>the mouse wheel, and the scrollbar can be dragged</li>"
    "<li><code>Backspace</code> - go back</li>"
    "<li><code>Tab</code> - the next field in a form</li></ul></div>"
    "<div class=\"card\"><h3>What it does</h3>"
    "<p>HTML, CSS, PNG pictures, forms that submit, JavaScript with a "
    "document object model, and video handed to TUS's own player. "
    "<b>https</b> is verified against /etc/ssl/ca-bundle.crt; TUS has no "
    "real-time clock, so certificate dates are not checked and the "
    "status line says so on every secure page.</p>"
    "<p class=\"muted\">What it does not do: JavaScript that expects a "
    "modern engine, and the sites that require one.</p></div>"
    "</div></body></html>";

int main(int argc, char **argv) {
    if (hx_open(&dpy) != 0) {
        fprintf(stderr, "clint: no highX display (run it from a session)\n");
        return 1;
    }

    if ((int)dpy.screen_w - 80 < win_w) win_w = (int)dpy.screen_w - 80;
    if ((int)dpy.screen_h - 80 < win_h) win_h = (int)dpy.screen_h - 80;
    if (win_w < 320) win_w = 320;
    if (win_h < 240) win_h = 240;

    win = hx_create_window(40, 30, (unsigned)win_w, (unsigned)win_h, 0,
                           COL_PAGE, "Clint");
    if (win == 0) {
        fprintf(stderr, "clint: cannot create a window\n");
        hx_close(&dpy);
        return 1;
    }
    hx_map(win);

    if (canvas_init(&canvas, win_w, win_h) != 0) {
        fprintf(stderr, "clint: out of memory for a %dx%d canvas\n", win_w,
                win_h);
        hx_close(&dpy);
        return 1;
    }

    layout_set_image_source(image_for);
    layout_set_click_probe(script_click_probe);

    set_status(0, "ready");
    show_local_page(WELCOME);
    snprintf(address, sizeof(address), "%s", argc > 1 ? argv[1] : "");
    address_caret = (int)strlen(address);
    redraw();

    if (argc > 1) navigate(argv[1], NULL);

    for (;;) {
        struct hx_event ev;

        /* Waiting is bounded by the next timer a script asked for, so
         * setTimeout fires without the browser polling for it. */
        int wait = script != NULL ? js_next_timer(script) : -1;
        if (hx_next_event(&ev, wait) <= 0) {
            if (script != NULL && js_run_timers(script) > 0) {
                dom_dirty = 1;
                refresh_after_script();
                if (pending_url[0] != '\0') {
                    char target[sizeof(pending_url)];
                    snprintf(target, sizeof(target), "%s", pending_url);
                    pending_url[0] = '\0';
                    navigate(target, have_current ? &current : NULL);
                }
            }
            continue;
        }

        if (ev.type == HX_EV_CLOSE) break;

        if (ev.type == HX_EV_EXPOSE) {
            redraw();
        } else if (ev.type == HX_EV_CONFIGURE) {
            int w = (int)ev.w, h = (int)ev.h;
            if (w > 0 && h > 0 && (w != win_w || h != win_h)) {
                int relayout_needed = w != win_w;
                win_w = w;
                win_h = h;
                if (canvas_init(&canvas, win_w, win_h) != 0) break;
                if (relayout_needed) relayout();
                redraw();
            }
        } else if (ev.type == HX_EV_KEY) {
            on_key(&ev);
        } else if (ev.type == HX_EV_POINTER) {
            if (ev.detail == HX_PTR_PRESS) on_click(&ev);
            else if (ev.detail == HX_PTR_MOTION) on_motion(&ev);
            else if (ev.detail == HX_PTR_RELEASE) drag_thumb = 0;
            else if (ev.detail == HX_PTR_WHEEL) {
                /* The wheel counts up, the page counts down. */
                scroll_by(-(int32_t)ev.key * SCROLL_STEP);
            }
        }
    }

    layout_free(&page);
    js_free(script);
    dom_free(document_tree);
    images_clear();
    canvas_free(&canvas);
    hx_close(&dpy);
    return 0;
}
