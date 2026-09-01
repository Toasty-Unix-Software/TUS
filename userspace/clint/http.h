/*
 * http.h - fetching a document, over http:// or https://
 *
 * One call: give it a URL, get back the bytes and the content type.
 * Redirects are followed here rather than by the caller, because the
 * URL that comes back is the one the page has to be resolved
 * against - a relative link on a redirected page means nothing
 * without it.
 *
 * https:// goes through userspace/tls; http:// uses the socket
 * directly. Which one was used, and whether the certificate could be
 * checked, is reported back so the browser can be honest about it
 * rather than drawing a padlock it has not earned.
 */

#ifndef CLINT_HTTP_H
#define CLINT_HTTP_H

#include <stddef.h>

struct url {
    char scheme[8];
    char host[256];
    char path[1024];
    int port;
};

/* Parse `text` into `out`. `base`, when not NULL, is the page the
 * link was found on, so relative references resolve. Returns 0 or
 * -1. */
int url_parse(struct url *out, const char *text, const struct url *base);

/* Render a url back into text, for the address bar and for
 * comparison. */
void url_format(const struct url *u, char *out, size_t size);

struct http_response {
    int status;
    char content_type[128];
    char *body;
    size_t body_len;
    struct url final_url;   /* after redirects */
    int secure;             /* the last hop was TLS */
    const char *tls_warning; /* what TLS could not check, or NULL */
};

/*
 * Fetch `u`. Returns 0 with `out` filled in (the caller frees
 * out->body), or -1 with the reason in http_last_error().
 */
int http_get(const struct url *u, struct http_response *out);

/*
 * The same, for a form that asked to be sent rather than linked to.
 * `body` is an already encoded application/x-www-form-urlencoded
 * string. A redirect after a POST is followed as a GET, which is what
 * the status codes mean and what every site's "thank you" page
 * depends on.
 */
int http_post(const struct url *u, const char *body,
              struct http_response *out);
void http_response_free(struct http_response *r);

const char *http_last_error(void);

#endif /* CLINT_HTTP_H */
