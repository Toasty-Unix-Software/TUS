/*
 * dump - run Clint's engine on the build host and print what it would
 * paint
 *
 * The parser, the cascade and the layout are ordinary C over ordinary
 * memory: nothing in them touches TUS, so they can be exercised here
 * where a failure is a printf away instead of a boot away. The
 * program reads HTML on stdin and prints the display list.
 *
 *   dump [width]                     < page.html
 *   dump [width] -c iso-8859-9       < page.html
 *
 * -c reads the document in that encoding instead of UTF-8, which is
 * the one thing about a page that cannot be seen in its display list
 * afterwards - by then it is all UTF-8.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "charset.h"
#include "css.h"
#include "dom.h"
#include "image.h"
#include "layout.h"

/*
 * On the host there is no network, so an <img src> is read as a path:
 * `dump` next to a PNG file lays the page out with the real picture's
 * real dimensions, which is what the sizing rules have to be checked
 * against. One image is cached, which is all a test document needs.
 */
static struct image g_img;
static char g_img_src[512];

static const struct image *host_image(const char *src) {
    if (src == NULL || *src == '\0') return NULL;

    if (strcmp(src, g_img_src) != 0) {
        image_free(&g_img);
        snprintf(g_img_src, sizeof(g_img_src), "%s", src);

        FILE *f = fopen(src, "rb");
        if (f == NULL) return NULL;

        static unsigned char buf[4 << 20];
        size_t n = fread(buf, 1, sizeof(buf), f);
        fclose(f);

        if (image_decode(buf, n, &g_img) != 0) {
            fprintf(stderr, "dump: %s: %s\n", src, image_last_error());
            return NULL;
        }
    }
    return g_img.px != NULL ? &g_img : NULL;
}

int main(int argc, char **argv) {
    int width = argc > 1 ? atoi(argv[1]) : 800;
    const char *charset = NULL;
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-c") == 0) charset = argv[i + 1];
    }

    size_t cap = 65536, len = 0;
    char *html = malloc(cap);
    if (html == NULL) return 1;
    for (;;) {
        if (len + 4096 + 1 > cap) {
            cap *= 2;
            char *p = realloc(html, cap);
            if (p == NULL) return 1;
            html = p;
        }
        size_t n = fread(html + len, 1, 4096, stdin);
        if (n == 0) break;
        len += n;
    }
    html[len] = '\0';

    if (charset != NULL) {
        size_t converted_len = 0;
        char *converted = charset_to_utf8(charset, html, len, &converted_len);
        if (converted != NULL) {
            free(html);
            html = converted;
            len = converted_len;
        }
    }

    layout_set_image_source(host_image);

    struct dom_node *root = dom_parse(html, len);
    if (root == NULL) {
        fprintf(stderr, "dump: cannot parse\n");
        return 1;
    }
    css_apply(root);

    struct doc_layout page;
    memset(&page, 0, sizeof(page));
    layout_document(root, width, &page);

    printf("width %d, height %d, %d items, %d links\n", width, page.height,
           page.nitems, page.nlinks);
    for (int i = 0; i < page.nitems; i++) {
        const struct paint_item *it = &page.items[i];
        if (it->type == ITEM_RECT) {
            printf("  rect  %4d,%4d %4dx%-4d #%06x\n", it->x, it->y, it->w,
                   it->h, it->color);
        } else if (it->type == ITEM_IMAGE) {
            printf("  image %4d,%4d %4dx%-4d (source %dx%d)\n", it->x, it->y,
                   it->w, it->h, it->img != NULL ? it->img->w : 0,
                   it->img != NULL ? it->img->h : 0);
        } else {
            printf("  text  %4d,%4d %4dx%-4d #%06x x%d%s%s \"%s\"\n", it->x,
                   it->y, it->w, it->h, it->color, it->scale,
                   it->bold ? " bold" : "", it->underline ? " under" : "",
                   it->text != NULL ? it->text : "");
        }
    }
    for (int i = 0; i < page.nfields; i++) {
        const struct doc_field *f = &page.fields[i];
        static const char *names[] = { "text", "textarea", "button",
                                       "checkbox", "radio", "select",
                                       "other" };
        char text[256];
        layout_field_text(f->node, f->type, text, sizeof(text));
        printf("  field %4d,%4d %4dx%-4d %-8s name=%s \"%s\"\n", f->x, f->y,
               f->w, f->h, names[f->type],
               dom_attr(f->node, "name") != NULL ? dom_attr(f->node, "name")
                                                 : "-",
               text);
    }
    for (int i = 0; i < page.nlinks; i++) {
        printf("  link  %4d,%4d %4dx%-4d -> %s\n", page.links[i].x,
               page.links[i].y, page.links[i].w, page.links[i].h,
               page.links[i].href);
    }

    layout_free(&page);
    dom_free(root);
    free(html);
    return 0;
}
