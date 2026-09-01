/*
 * tpm.c - the TUS Package Manager
 *
 * A lightweight apt-alike for TUS. Repository addresses live in
 * /etc/tpm/source.list (one "name url" pair per line, '#' comments and
 * blank lines ignored) - same idea as apt's sources.list.
 *
 * The network backend reuses Clint's own HTTP client (http.c/url.c,
 * see userspace/clint/http.h) - the same one `fetch` and `wget` link
 * against - rather than opening sockets itself. A repository is a
 * plain directory an HTTP server hands out: `<url>/Packages` is a
 * text index (one "name version filename" line per package) and
 * `<url>/<filename>` is the .tpkg itself. `tpm update` fetches every
 * configured repo's index into /var/lib/tpm/lists/<name>.packages;
 * `tpm search`/`tpm install <name>` read those cached indexes rather
 * than re-fetching them, exactly like apt's own list cache.
 *
 * Installing a package that is already a local file needs no network
 * at all: TUS packages are ".tpkg" files (TUS PacKaGe), and `tpm
 * install /path/to/foo.tpkg` unpacks one right here, exactly like
 * `dpkg -i foo.deb` needs no network once the .deb is already on
 * disk.
 *
 * .tpkg format
 * ------------
 * A .tpkg is a plain ustar tar archive - the same format and the same
 * parser shape as rootfs.img (see kernel/vfs/rootfs.c), so it can be
 * built with the stock `tar --format=ustar`:
 *
 *     tar --format=ustar -cf foo.tpkg control data
 *
 * It contains exactly two things at its root:
 *
 *   control     text file, "Key: value" lines (Name, Version,
 *               Description are read; unknown keys are ignored)
 *   data/       a directory tree mirroring the target filesystem -
 *               data/bin/foo installs to /bin/foo, data/etc/foo.conf
 *               installs to /etc/foo.conf, and so on
 *
 * `tpm install` records every path it writes to
 * /var/lib/tpm/status/<name>.list so `tpm remove <name>` can undo it
 * and `tpm list --installed` can enumerate what is on the system.
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#include "http.h"

#define SOURCE_LIST "/etc/tpm/source.list"
#define STATUS_DIR  "/var/lib/tpm/status"
#define LISTS_DIR   "/var/lib/tpm/lists"
#define TAR_BLOCK   512

/* ---- source.list (repositories) ---- */

static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) {
        s++;
    }
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) {
        *--end = '\0';
    }
    return s;
}

/* Reads source.list, calling cb(name, url) for each repository entry.
 * Returns the number of entries found, or -1 if the file is missing. */
static int for_each_source(void (*cb)(const char *name, const char *url)) {
    FILE *f = fopen(SOURCE_LIST, "r");
    if (!f) {
        return -1;
    }

    char line[512];
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        char *s = trim(line);
        if (*s == '\0' || *s == '#') {
            continue;
        }
        char *url = s;
        while (*url && !isspace((unsigned char)*url)) {
            url++;
        }
        if (*url == '\0') {
            continue;
        }
        *url++ = '\0';
        url = trim(url);
        if (*url == '\0') {
            continue;
        }
        cb(s, url);
        count++;
    }
    fclose(f);
    return count;
}

static void print_source(const char *name, const char *url) {
    printf("  %-16s %s\n", name, url);
}

/* mkdir -p, defined further down; declared here so the network
 * helpers (which run before it in the file) can create LISTS_DIR
 * and the temp download directory. */
static int mkdir_parents(const char *path);

/* ---- network backend ----
 *
 * A repository url points at a plain directory an HTTP server hands
 * out. join_url() builds "<repo url>/<name>" (one '/' regardless of
 * whether the configured url already ends in one), and fetch_url()
 * is the one place that turns a URL into bytes in memory - update,
 * search and a networked install all go through it.
 */

static void join_url(char *out, size_t out_size, const char *base, const char *name) {
    size_t blen = strlen(base);
    int need_slash = (blen > 0 && base[blen - 1] != '/');
    snprintf(out, out_size, "%s%s%s", base, need_slash ? "/" : "", name);
}

/* Fetches `url` into a malloc'd, NUL-terminated buffer. Returns 0
 * with out_buf/out_len filled in (caller frees *out_buf), or -1
 * with a message already printed to stderr. */
static int fetch_url(const char *url, uint8_t **out_buf, size_t *out_len) {
    struct url u;
    if (url_parse(&u, url, NULL) != 0) {
        fprintf(stderr, "tpm: %s: %s\n", url, http_last_error());
        return -1;
    }

    struct http_response r;
    if (http_get(&u, &r) != 0) {
        fprintf(stderr, "tpm: %s: %s\n", url, http_last_error());
        return -1;
    }
    if (r.status < 200 || r.status >= 300) {
        fprintf(stderr, "tpm: %s: server returned %d\n", url, r.status);
        http_response_free(&r);
        return -1;
    }

    uint8_t *buf = malloc(r.body_len + 1);
    if (!buf) {
        fprintf(stderr, "tpm: %s: out of memory\n", url);
        http_response_free(&r);
        return -1;
    }
    memcpy(buf, r.body, r.body_len);
    buf[r.body_len] = '\0';
    *out_buf = buf;
    *out_len = r.body_len;
    http_response_free(&r);
    return 0;
}

static int write_buffer(const char *path, const uint8_t *buf, size_t len) {
    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    size_t off = 0;
    while (off < len) {
        ssize_t w = write(fd, buf + off, len - off);
        if (w <= 0) {
            close(fd);
            return -1;
        }
        off += (size_t)w;
    }
    close(fd);
    return 0;
}

static void update_one(const char *name, const char *url) {
    char index_url[600];
    join_url(index_url, sizeof(index_url), url, "Packages");

    uint8_t *buf;
    size_t len;
    if (fetch_url(index_url, &buf, &len) < 0) {
        return;
    }

    char cache_path[300];
    snprintf(cache_path, sizeof(cache_path), "%s/%s.packages", LISTS_DIR, name);
    if (write_buffer(cache_path, buf, len) < 0) {
        fprintf(stderr, "tpm: %s: %s\n", cache_path, strerror(errno));
        free(buf);
        return;
    }

    int count = 0;
    for (size_t i = 0; i < len; i++) {
        if (buf[i] == '\n') {
            count++;
        }
    }
    free(buf);
    printf("tpm: %-16s fetched %d package%s\n", name, count, count == 1 ? "" : "s");
}

static int cmd_update(void) {
    printf("tpm: reading %s\n", SOURCE_LIST);
    if (mkdir_parents(LISTS_DIR "/x") < 0) {
        fprintf(stderr, "tpm: %s: %s\n", LISTS_DIR, strerror(errno));
        return 1;
    }

    int n = for_each_source(update_one);
    if (n < 0) {
        fprintf(stderr, "tpm: %s: %s\n", SOURCE_LIST, strerror(errno));
        return 1;
    }
    if (n == 0) {
        fprintf(stderr, "tpm: no repositories configured in %s\n", SOURCE_LIST);
        return 1;
    }
    return 0;
}

/* Search state for find_package()/cmd_search_net() - file-scope
 * because for_each_source()'s callback takes no context pointer, and
 * tpm is single-threaded so there is nothing to race with. */
static const char *g_search_pkgname;
static char *g_search_repo_url;
static size_t g_search_repo_url_size;
static char *g_search_filename;
static size_t g_search_filename_size;
static char *g_search_version;
static size_t g_search_version_size;
static int g_search_found;

static void search_one_source(const char *name, const char *url) {
    if (g_search_found) {
        return;
    }
    char cache_path[300];
    snprintf(cache_path, sizeof(cache_path), "%s/%s.packages", LISTS_DIR, name);
    FILE *f = fopen(cache_path, "r");
    if (!f) {
        return;
    }
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char *s = trim(line);
        if (*s == '\0' || *s == '#') {
            continue;
        }
        char *p = s;
        char *pkg_name = p;
        while (*p && !isspace((unsigned char)*p)) p++;
        if (!*p) continue;
        *p++ = '\0';
        if (strcmp(pkg_name, g_search_pkgname) != 0) {
            continue;
        }
        while (isspace((unsigned char)*p)) p++;
        char *pkg_ver = p;
        while (*p && !isspace((unsigned char)*p)) p++;
        if (!*p) continue;
        *p++ = '\0';
        while (isspace((unsigned char)*p)) p++;
        char *pkg_file = trim(p);
        if (*pkg_file == '\0') {
            continue;
        }
        snprintf(g_search_repo_url, g_search_repo_url_size, "%s", url);
        snprintf(g_search_filename, g_search_filename_size, "%s", pkg_file);
        snprintf(g_search_version, g_search_version_size, "%s", pkg_ver);
        g_search_found = 1;
        break;
    }
    fclose(f);
}

/* Looks up `pkgname` across every cached "<repo>.packages" index in
 * LISTS_DIR. On a match fills in repo_url/filename/version (each
 * caller-sized) and returns 0; returns -1 if not found in any cached
 * index (run `tpm update` first). */
static int find_package(const char *pkgname, char *repo_url, size_t repo_url_size,
                         char *filename, size_t filename_size,
                         char *version, size_t version_size) {
    g_search_pkgname = pkgname;
    g_search_repo_url = repo_url;
    g_search_repo_url_size = repo_url_size;
    g_search_filename = filename;
    g_search_filename_size = filename_size;
    g_search_version = version;
    g_search_version_size = version_size;
    g_search_found = 0;

    for_each_source(search_one_source);
    return g_search_found ? 0 : -1;
}

/* ---- tiny ustar reader, shared by install/list-contents ---- */

struct tar_entry {
    const char *name;   /* not NUL-terminated; use name_len */
    size_t name_len;
    char type;
    uint32_t mode;
    const uint8_t *data;
    uint64_t size;
};

static uint64_t tar_octal(const char *field, size_t len) {
    uint64_t v = 0;
    for (size_t i = 0; i < len; i++) {
        char c = field[i];
        if (c >= '0' && c <= '7') {
            v = (v << 3) | (uint64_t)(c - '0');
        } else if (c == ' ' || c == '\0') {
            continue;
        } else {
            break;
        }
    }
    return v;
}

/* Walks a ustar archive already loaded into memory, calling cb(entry,
 * ctx) for each file/directory header. Returns 0 on a clean parse, -1
 * on a truncated/malformed archive. */
static int tar_walk(const uint8_t *buf, size_t len,
                     int (*cb)(const struct tar_entry *e, void *ctx),
                     void *ctx) {
    const uint8_t *p = buf;
    const uint8_t *end = buf + len;

    while (p + TAR_BLOCK <= end) {
        int zero = 1;
        for (int i = 0; i < TAR_BLOCK; i++) {
            if (p[i] != 0) {
                zero = 0;
                break;
            }
        }
        if (zero) {
            break;
        }

        const char *name = (const char *)p;
        size_t name_len = 0;
        while (name_len < 100 && name[name_len] != '\0') {
            name_len++;
        }
        while (name_len > 0 && name[name_len - 1] == '/') {
            name_len--;
        }
        if (name_len >= 2 && name[0] == '.' && name[1] == '/') {
            name += 2;
            name_len -= 2;
        }

        struct tar_entry e;
        e.name = name;
        e.name_len = name_len;
        e.type = (char)p[156];
        e.mode = (uint32_t)tar_octal((const char *)p + 100, 8);
        e.size = tar_octal((const char *)p + 124, 12);
        e.data = p + TAR_BLOCK;

        if (name_len > 0 && cb(&e, ctx) != 0) {
            return -1;
        }

        uint64_t padded = (e.size + TAR_BLOCK - 1) & ~(uint64_t)(TAR_BLOCK - 1);
        p += TAR_BLOCK + padded;
        if (p > end) {
            return -1; /* truncated archive */
        }
    }
    return 0;
}

static uint8_t *read_whole_file(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long sz = ftell(f);
    if (sz < 0 || fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return NULL;
    }
    uint8_t *buf = malloc((size_t)sz > 0 ? (size_t)sz : 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    size_t got = fread(buf, 1, (size_t)sz, f);
    fclose(f);
    if (got != (size_t)sz) {
        free(buf);
        return NULL;
    }
    *out_len = (size_t)sz;
    return buf;
}

/* mkdir -p: create every path component up to (not including) the
 * final element of `path`. */
static int mkdir_parents(const char *path) {
    char comp[512];
    size_t len = strlen(path);
    if (len >= sizeof(comp)) {
        return -1;
    }
    for (size_t i = 1; i < len; i++) {
        if (path[i] != '/') {
            continue;
        }
        memcpy(comp, path, i);
        comp[i] = '\0';
        if (mkdir(comp, 0755) < 0 && errno != EEXIST) {
            return -1;
        }
    }
    return 0;
}

/* ---- control file parsing ---- */

struct pkg_info {
    char name[64];
    char version[32];
    char description[256];
};

static void control_field(struct pkg_info *pk, const char *key, const char *val) {
    if (strcasecmp(key, "Name") == 0) {
        snprintf(pk->name, sizeof(pk->name), "%s", val);
    } else if (strcasecmp(key, "Version") == 0) {
        snprintf(pk->version, sizeof(pk->version), "%s", val);
    } else if (strcasecmp(key, "Description") == 0) {
        snprintf(pk->description, sizeof(pk->description), "%s", val);
    }
}

static void parse_control(const uint8_t *data, uint64_t size, struct pkg_info *pk) {
    memset(pk, 0, sizeof(*pk));
    char line[512];
    size_t li = 0;
    for (uint64_t i = 0; i <= size; i++) {
        char c = (i < size) ? (char)data[i] : '\n';
        if (c == '\n' || li + 1 >= sizeof(line)) {
            line[li] = '\0';
            char *s = trim(line);
            char *colon = strchr(s, ':');
            if (colon) {
                *colon = '\0';
                control_field(pk, trim(s), trim(colon + 1));
            }
            li = 0;
        } else if (c != '\r') {
            line[li++] = c;
        }
    }
}

struct find_control_ctx {
    struct pkg_info *pk;
    int found;
};

static int find_control_cb(const struct tar_entry *e, void *ctx_) {
    struct find_control_ctx *ctx = ctx_;
    if (e->type == '5') {
        return 0;
    }
    if (e->name_len == 7 && memcmp(e->name, "control", 7) == 0) {
        parse_control(e->data, e->size, ctx->pk);
        ctx->found = 1;
    }
    return 0;
}

/* ---- install ---- */

struct install_ctx {
    FILE *manifest;
    int count;
    int failed;
};

static int install_cb(const struct tar_entry *e, void *ctx_) {
    struct install_ctx *ctx = ctx_;
    static const char prefix[] = "data/";
    size_t plen = sizeof(prefix) - 1;

    if (e->name_len <= plen || memcmp(e->name, prefix, plen) != 0) {
        return 0; /* control, or a stray root-level entry: not installed */
    }
    if (e->type == '5') {
        return 0; /* directories are implied by the files inside them */
    }
    if (e->type != '0' && e->type != '\0') {
        return 0; /* symlinks etc: not supported yet */
    }

    char target[600];
    size_t rest_len = e->name_len - plen;
    if (rest_len + 1 >= sizeof(target)) {
        fprintf(stderr, "tpm: path too long in package, skipping\n");
        ctx->failed = 1;
        return 0;
    }
    target[0] = '/';
    memcpy(target + 1, e->name + plen, rest_len);
    target[1 + rest_len] = '\0';

    if (mkdir_parents(target) < 0) {
        fprintf(stderr, "tpm: %s: %s\n", target, strerror(errno));
        ctx->failed = 1;
        return 0;
    }

    unsigned mode = e->mode != 0 ? e->mode : 0644;
    int fd = open(target, O_CREAT | O_WRONLY | O_TRUNC, mode);
    if (fd < 0) {
        fprintf(stderr, "tpm: %s: %s\n", target, strerror(errno));
        ctx->failed = 1;
        return 0;
    }
    uint64_t off = 0;
    int write_failed = 0;
    while (off < e->size) {
        size_t chunk = (e->size - off) > 65536 ? 65536 : (size_t)(e->size - off);
        ssize_t w = write(fd, e->data + off, chunk);
        if (w <= 0) {
            write_failed = 1;
            break;
        }
        off += (uint64_t)w;
    }
    close(fd);
    if (write_failed) {
        fprintf(stderr, "tpm: %s: %s\n", target, strerror(errno));
        ctx->failed = 1;
        return 0;
    }

    printf("  installing %s\n", target);
    if (ctx->manifest) {
        fprintf(ctx->manifest, "%s\n", target);
    }
    ctx->count++;
    return 0;
}

static int cmd_install_tpkg(const char *path) {
    size_t len;
    uint8_t *buf = read_whole_file(path, &len);
    if (!buf) {
        fprintf(stderr, "tpm: %s: %s\n", path, strerror(errno));
        return 1;
    }

    struct pkg_info pk;
    struct find_control_ctx fctx = { .pk = &pk, .found = 0 };
    if (tar_walk(buf, len, find_control_cb, &fctx) < 0) {
        fprintf(stderr, "tpm: %s: not a valid .tpkg (bad ustar archive)\n", path);
        free(buf);
        return 1;
    }
    if (!fctx.found || pk.name[0] == '\0') {
        fprintf(stderr, "tpm: %s: missing or invalid control file\n", path);
        free(buf);
        return 1;
    }

    printf("Selecting previously unselected package %s.\n", pk.name);
    printf("Unpacking %s (%s) ...\n", pk.name,
           pk.version[0] ? pk.version : "unknown");

    if (mkdir_parents(STATUS_DIR "/x") < 0) {
        fprintf(stderr, "tpm: %s: %s\n", STATUS_DIR, strerror(errno));
        free(buf);
        return 1;
    }
    char manifest_path[300];
    snprintf(manifest_path, sizeof(manifest_path), "%s/%s.list", STATUS_DIR, pk.name);
    FILE *manifest = fopen(manifest_path, "w");
    if (!manifest) {
        fprintf(stderr, "tpm: %s: %s\n", manifest_path, strerror(errno));
        free(buf);
        return 1;
    }
    fprintf(manifest, "Name: %s\n", pk.name);
    fprintf(manifest, "Version: %s\n", pk.version);
    fprintf(manifest, "Description: %s\n", pk.description);
    fprintf(manifest, "Files:\n");

    struct install_ctx ictx = { .manifest = manifest, .count = 0, .failed = 0 };
    tar_walk(buf, len, install_cb, &ictx);
    fclose(manifest);
    free(buf);

    if (ictx.failed) {
        fprintf(stderr, "tpm: %s: install failed partway through\n", pk.name);
        return 1;
    }
    if (ictx.count == 0) {
        fprintf(stderr, "tpm: %s: package contains no files under data/\n", pk.name);
        return 1;
    }

    printf("Setting up %s (%s) ...\n", pk.name,
           pk.version[0] ? pk.version : "unknown");
    return 0;
}

static int has_suffix(const char *s, const char *suffix) {
    size_t sl = strlen(s), fl = strlen(suffix);
    return sl >= fl && strcmp(s + sl - fl, suffix) == 0;
}

static int cmd_install(const char *arg) {
    if (has_suffix(arg, ".tpkg")) {
        return cmd_install_tpkg(arg);
    }

    char repo_url[512], filename[256], version[32];
    if (find_package(arg, repo_url, sizeof(repo_url), filename, sizeof(filename),
                      version, sizeof(version)) < 0) {
        fprintf(stderr, "tpm: %s: not found (run 'tpm update' first)\n", arg);
        return 1;
    }

    char pkg_url[600];
    join_url(pkg_url, sizeof(pkg_url), repo_url, filename);
    printf("tpm: fetching %s\n", pkg_url);

    uint8_t *buf;
    size_t len;
    if (fetch_url(pkg_url, &buf, &len) < 0) {
        return 1;
    }

    char tmp_path[300];
    snprintf(tmp_path, sizeof(tmp_path), "/tmp/tpm-%s.tpkg", arg);
    if (write_buffer(tmp_path, buf, len) < 0) {
        fprintf(stderr, "tpm: %s: %s\n", tmp_path, strerror(errno));
        free(buf);
        return 1;
    }
    free(buf);

    int rc = cmd_install_tpkg(tmp_path);
    unlink(tmp_path);
    return rc;
}

/* ---- remove ---- */

static int cmd_remove(const char *name) {
    char manifest_path[300];
    snprintf(manifest_path, sizeof(manifest_path), "%s/%s.list", STATUS_DIR, name);
    FILE *f = fopen(manifest_path, "r");
    if (!f) {
        fprintf(stderr, "tpm: %s: not installed\n", name);
        return 1;
    }

    char line[600];
    int in_files = 0, removed = 0, failed = 0;
    while (fgets(line, sizeof(line), f)) {
        char *s = trim(line);
        if (!in_files) {
            if (strcmp(s, "Files:") == 0) {
                in_files = 1;
            }
            continue;
        }
        if (*s == '\0') {
            continue;
        }
        if (unlink(s) == 0) {
            printf("  removing %s\n", s);
            removed++;
        } else if (errno != ENOENT) {
            fprintf(stderr, "tpm: %s: %s\n", s, strerror(errno));
            failed = 1;
        }
    }
    fclose(f);

    if (unlink(manifest_path) < 0) {
        fprintf(stderr, "tpm: %s: %s\n", manifest_path, strerror(errno));
        failed = 1;
    }

    printf("Removing %s (%d file%s) ...\n", name, removed, removed == 1 ? "" : "s");
    return failed ? 1 : 0;
}

/* ---- list ---- */

#define SYS_READDIR 11
#define VFS_NAME_MAX 64

struct vfs_dirent {
    char name[VFS_NAME_MAX];
    unsigned type;
    unsigned size;
    unsigned mode;
};

static long tus_syscall3(long n, long a1, long a2, long a3) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "a"(n)
                     : "memory", "cc");
    return ret;
}

static int cmd_list_installed(void) {
    int fd = open(STATUS_DIR, O_RDONLY);
    if (fd < 0) {
        printf("tpm: no packages installed\n");
        return 0;
    }

    int any = 0;
    struct vfs_dirent ent;
    long n;
    while ((n = tus_syscall3(SYS_READDIR, fd, (long)&ent, sizeof(ent))) > 0) {
        if (ent.name[0] == '.' || !has_suffix(ent.name, ".list")) {
            continue;
        }
        char path[300];
        snprintf(path, sizeof(path), "%s/%s", STATUS_DIR, ent.name);
        FILE *f = fopen(path, "r");
        if (!f) {
            continue;
        }
        char name[128] = "", version[64] = "";
        char line[300];
        while (fgets(line, sizeof(line), f)) {
            char *s = trim(line);
            if (strncmp(s, "Name:", 5) == 0) {
                snprintf(name, sizeof(name), "%s", trim(s + 5));
            } else if (strncmp(s, "Version:", 8) == 0) {
                snprintf(version, sizeof(version), "%s", trim(s + 8));
            }
        }
        fclose(f);
        printf("  %-24s %s\n", name[0] ? name : ent.name, version);
        any = 1;
    }
    close(fd);
    if (!any) {
        printf("tpm: no packages installed\n");
    }
    return 0;
}

static int cmd_list(void) {
    printf("tpm: repositories in %s\n", SOURCE_LIST);
    int n = for_each_source(print_source);
    if (n < 0) {
        fprintf(stderr, "tpm: %s: %s\n", SOURCE_LIST, strerror(errno));
        return 1;
    }
    if (n == 0) {
        fprintf(stderr, "tpm: no repositories configured in %s\n", SOURCE_LIST);
        return 1;
    }
    return 0;
}

static void usage(void) {
    fprintf(stderr,
            "usage: tpm <command> [package]\n"
            "commands:\n"
            "  update              refresh repository list from %s\n"
            "  list                show configured repositories\n"
            "  list --installed    show installed packages\n"
            "  search <package>    look up a package\n"
            "  install <package>   install a package by name (needs a repo)\n"
            "  install <file.tpkg> install a local package file\n"
            "  remove <package>    remove an installed package\n",
            SOURCE_LIST);
}

static int cmd_search(const char *pkg) {
    char repo_url[512], filename[256], version[32];
    if (find_package(pkg, repo_url, sizeof(repo_url), filename, sizeof(filename),
                      version, sizeof(version)) < 0) {
        printf("tpm: %s: not found (run 'tpm update' first)\n", pkg);
        return 1;
    }
    printf("%-24s %-12s %s\n", pkg, version, repo_url);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
        return 1;
    }

    const char *cmd = argv[1];
    if (strcmp(cmd, "update") == 0) {
        return cmd_update();
    } else if (strcmp(cmd, "list") == 0) {
        if (argc >= 3 && strcmp(argv[2], "--installed") == 0) {
            return cmd_list_installed();
        }
        return cmd_list();
    } else if (strcmp(cmd, "search") == 0) {
        if (argc < 3) {
            fprintf(stderr, "usage: tpm search <package>\n");
            return 1;
        }
        return cmd_search(argv[2]);
    } else if (strcmp(cmd, "install") == 0) {
        if (argc < 3) {
            fprintf(stderr, "usage: tpm install <package|file.tpkg>\n");
            return 1;
        }
        return cmd_install(argv[2]);
    } else if (strcmp(cmd, "remove") == 0) {
        if (argc < 3) {
            fprintf(stderr, "usage: tpm remove <package>\n");
            return 1;
        }
        return cmd_remove(argv[2]);
    } else {
        usage();
        return 1;
    }
}
