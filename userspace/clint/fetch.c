/*
 * fetch - retrieve a URL and print it, the command-line half of Clint
 *
 *   fetch https://example.com/            print the body
 *   fetch -i http://host/page             headers first
 *
 * The browser and this program share every line of the fetching code,
 * so a page that will not load can be examined from a shell without a
 * window system in the way - and the network path can be tested
 * before there is anything to draw it in.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http.h"

int main(int argc, char **argv) {
    int show_info = 0;
    int i = 1;

    for (; i < argc && argv[i][0] == '-' && argv[i][1] != '\0'; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            show_info = 1;
        } else {
            fprintf(stderr, "usage: fetch [-i] <url>\n");
            return 2;
        }
    }
    if (i >= argc) {
        fprintf(stderr, "usage: fetch [-i] <url>\n");
        return 2;
    }

    struct url u;
    if (url_parse(&u, argv[i], NULL) != 0) {
        fprintf(stderr, "fetch: %s\n", http_last_error());
        return 1;
    }

    struct http_response r;
    if (http_get(&u, &r) != 0) {
        fprintf(stderr, "fetch: %s\n", http_last_error());
        return 1;
    }

    if (show_info) {
        char text[1200];
        url_format(&r.final_url, text, sizeof(text));
        printf("url: %s\n", text);
        printf("status: %d\n", r.status);
        printf("type: %s\n", r.content_type[0] ? r.content_type : "(none)");
        printf("bytes: %lu\n", (unsigned long)r.body_len);
        printf("secure: %s\n", r.secure ? "yes" : "no");
        if (r.tls_warning != NULL) {
            printf("caveat: %s\n", r.tls_warning);
        }
        printf("\n");
    }

    if (r.body != NULL && r.body_len > 0) {
        fwrite(r.body, 1, r.body_len, stdout);
    }
    http_response_free(&r);
    return 0;
}
