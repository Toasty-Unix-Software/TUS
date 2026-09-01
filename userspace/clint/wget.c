/*
 * wget - retrieve a URL and save it to a file, wget's own basic
 * behavior: default output name taken from the URL, -O to choose a
 * different one ("-O -" for stdout), -q to run quiet. Shares Clint's
 * own fetching code (http.c/url.c) with `fetch` and the browser
 * itself - one HTTP client, three doors into it.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http.h"

/* The last path segment of the URL, "index.html" when there isn't
 * one (a bare host, or a path ending in '/') - the same default
 * every real wget picks. */
static const char *default_name(const struct url *u) {
    const char *slash = strrchr(u->path, '/');
    const char *name = slash ? slash + 1 : u->path;
    return name[0] != '\0' ? name : "index.html";
}

int main(int argc, char **argv) {
    const char *out_name = NULL;
    int quiet = 0;
    int i = 1;

    for (; i < argc && argv[i][0] == '-' && argv[i][1] != '\0'; i++) {
        if (strcmp(argv[i], "-O") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "wget: -O needs a filename\n");
                return 2;
            }
            out_name = argv[++i];
        } else if (strcmp(argv[i], "-q") == 0) {
            quiet = 1;
        } else {
            fprintf(stderr, "usage: wget [-q] [-O file] <url>\n");
            return 2;
        }
    }
    if (i >= argc) {
        fprintf(stderr, "usage: wget [-q] [-O file] <url>\n");
        return 2;
    }
    const char *url_text = argv[i];

    struct url u;
    if (url_parse(&u, url_text, NULL) != 0) {
        fprintf(stderr, "wget: %s\n", http_last_error());
        return 1;
    }

    if (!quiet) {
        fprintf(stderr, "--%s--\n", url_text);
    }

    struct http_response r;
    if (http_get(&u, &r) != 0) {
        fprintf(stderr, "wget: %s\n", http_last_error());
        return 1;
    }

    if (r.status < 200 || r.status >= 300) {
        if (!quiet) {
            fprintf(stderr, "wget: server returned %d\n", r.status);
        }
        http_response_free(&r);
        return 1;
    }

    const char *name = out_name ? out_name : default_name(&r.final_url);
    int to_stdout = (name[0] == '-' && name[1] == '\0');

    if (to_stdout) {
        fwrite(r.body, 1, r.body_len, stdout);
    } else {
        int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("wget: open");
            http_response_free(&r);
            return 1;
        }
        size_t written = 0;
        while (written < r.body_len) {
            ssize_t n = write(fd, r.body + written, r.body_len - written);
            if (n <= 0) {
                perror("wget: write");
                close(fd);
                http_response_free(&r);
                return 1;
            }
            written += (size_t) n;
        }
        close(fd);
    }

    if (!quiet) {
        fprintf(stderr, "%s (%lu bytes) saved to \xe2\x80\x98%s\xe2\x80\x99\n",
                to_stdout ? "-" : name, (unsigned long) r.body_len,
                to_stdout ? "-" : name);
    }

    http_response_free(&r);
    return 0;
}
