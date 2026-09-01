/*
 * jsrun - Clint's JavaScript, run on the build host
 *
 * The interpreter and the DOM bindings touch nothing in TUS: they are
 * ordinary C over ordinary memory, so they are tested here, where a
 * failure is a printf away instead of a boot away.
 *
 *   jsrun script.js                 run a script, print what it logs
 *   jsrun page.html -html           run the page's scripts, print the
 *                                   document they left behind
 *   jsrun page.html -html -click id deliver a click to that element
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "js.h"
#include "dom.h"

static void host_log(void *ctx, const char *text) { (void)ctx; printf("%s\n", text); }
static void host_error(void *ctx, const char *text) { (void)ctx; printf("error: %s\n", text); }
static void host_alert(void *ctx, const char *text) { (void)ctx; printf("[alert] %s\n", text); }
static void host_navigate(void *ctx, const char *url) { (void)ctx; printf("[navigate] %s\n", url); }
static void host_changed(void *ctx) { (void)ctx; }
static const char *host_location(void *ctx) { (void)ctx; return "http://tus.local/page.html?q=1"; }
static unsigned long host_now(void *ctx) { (void)ctx; return 1000; }

/* <script src="...">: jsrun has no network, so `src` is taken as a
 * literal filesystem path (relative to wherever jsrun runs) instead
 * of a URL - enough to exercise js_run_document()'s fetch-and-run
 * path against a real file without pulling http.c into a host test
 * binary that otherwise touches nothing outside plain C/memory. */
static char *host_fetch_script(void *ctx, const char *src) {
    (void)ctx;
    FILE *f = fopen(src, "rb");
    if (!f) return NULL;
    static char buf[1 << 16];
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    fclose(f);
    buf[n] = 0;
    char *out = malloc(n + 1);
    if (out != NULL) memcpy(out, buf, n + 1);
    return out;
}

int main(int argc, char **argv) {
    static char buf[1 << 20];
    FILE *f = argc > 1 ? fopen(argv[1], "rb") : stdin;
    if (!f) { perror("open"); return 1; }
    size_t n = fread(buf, 1, sizeof(buf) - 1, f);
    buf[n] = 0;
    if (argc > 1) fclose(f);

    int html = argc > 2 && strcmp(argv[2], "-html") == 0;
    struct dom_node *doc = html ? dom_parse(buf, n)
                                : dom_parse("<html><body></body></html>", 26);
    struct js_host host = { NULL, host_navigate, host_log, host_alert,
                            host_error, host_changed, host_location,
                            host_now, host_fetch_script };
    struct js *J = js_create(doc, &host);

    if (html) {
        js_run_document(J);
        for (int i = 3; i < argc; i++) {
            /* -click <id> */
            if (strcmp(argv[i], "-click") == 0 && i + 1 < argc) {
                struct dom_node *target = NULL;
                for (struct dom_node *nn = doc; nn != NULL; nn = dom_next(nn, doc)) {
                    const char *id = dom_attr(nn, "id");
                    if (id && strcmp(id, argv[i + 1]) == 0) { target = nn; break; }
                }
                if (target) {
                    printf("[click %s] handler=%d cancelled=%d\n", argv[i+1],
                           js_has_click_handler(J, target), js_click(J, target));
                } else printf("[click %s] not found\n", argv[i+1]);
                i++;
            }
        }
        char out[65536];
        dom_serialize(doc, 1, out, sizeof(out));
        printf("--- document ---\n%s\n", out);
    } else if (js_run(J, buf, NULL) != 0) {
        printf("error: %s\n", js_last_error(J));
    }
    js_free(J);
    dom_free(doc);
    return 0;
}
