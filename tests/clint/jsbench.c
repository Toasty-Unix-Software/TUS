/*
 * jsbench.c - host harness for js.c: run a script, print console.log
 * output and elapsed time, and check errors.
 *
 * Not a Makefile target of the main dump/run suite (js.c needs the
 * full DOM+JS object model, not just html/css/layout), so it is
 * built and run by hand: `make -f jsbench.mk`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dom.h"
#include "js.h"

static void log_cb(void *ctx, const char *text) {
    (void)ctx;
    printf("%s\n", text);
}

static void err_cb(void *ctx, const char *text) {
    (void)ctx;
    fprintf(stderr, "script error: %s\n", text);
}

static unsigned long now_ms(void *ctx) {
    (void)ctx;
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <script.js>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror("fopen"); return 1; }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *src = malloc((size_t)len + 1);
    fread(src, 1, (size_t)len, f);
    src[len] = '\0';
    fclose(f);

    struct dom_node *doc = dom_parse("<html><body></body></html>", 27);

    struct js_host host;
    memset(&host, 0, sizeof(host));
    host.log = log_cb;
    host.script_error = err_cb;
    host.now_ms = now_ms;

    struct js *J = js_create(doc, &host);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    int r = js_run(J, src, NULL);
    clock_gettime(CLOCK_MONOTONIC, &t1);

    double ms = (double)(t1.tv_sec - t0.tv_sec) * 1000.0 +
               (double)(t1.tv_nsec - t0.tv_nsec) / 1e6;

    if (r != 0) {
        fprintf(stderr, "run failed: %s\n", js_last_error(J));
        return 1;
    }
    fprintf(stderr, "elapsed: %.3f ms\n", ms);

    js_free(J);
    free(src);
    return 0;
}
