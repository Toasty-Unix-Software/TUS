/*
 * mp4.c - minimal MP4 demuxer (see mp4.h)
 *
 * The layout this walks:
 *
 *   ftyp                       (checked, then skipped)
 *   moov
 *     trak                     (one per track; we want handler 'vide')
 *       mdia
 *         mdhd                 timescale + duration
 *         hdlr                 'vide' / 'soun' / ...
 *         minf/stbl
 *           stsd/avc1/avcC     picture size, SPS/PPS, NAL length size
 *           stts               sample durations
 *           stss               key frames
 *           stsc               samples per chunk
 *           stsz               sample sizes
 *           stco / co64        chunk file offsets
 *   mdat                       the coded frames themselves
 *
 * The sample tables are stored per chunk, so the file offset of a
 * sample is only known after walking stsc, stco and stsz together;
 * mp4_open() does that once and leaves a flat array behind.
 */

#include "mp4.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SAMPLES 200000u
#define MAX_TABLE   200000u

/* ---- byte access ---- */

static unsigned rd32(const unsigned char *p) {
    return ((unsigned)p[0] << 24) | ((unsigned)p[1] << 16) |
           ((unsigned)p[2] << 8) | (unsigned)p[3];
}

static unsigned rd16(const unsigned char *p) {
    return ((unsigned)p[0] << 8) | (unsigned)p[1];
}

static unsigned long rd64(const unsigned char *p) {
    return ((unsigned long)rd32(p) << 32) | (unsigned long)rd32(p + 4);
}

static long read_at(int fd, unsigned long off, void *buf, unsigned len) {
    if (lseek(fd, (long)off, SEEK_SET) < 0) {
        return -1;
    }
    unsigned got = 0;
    while (got < len) {
        long n = read(fd, (unsigned char *)buf + got, len - got);
        if (n <= 0) {
            break;
        }
        got += (unsigned)n;
    }
    return (long)got;
}

/* Read a whole box body into memory. Tables are bounded by
 * MAX_TABLE entries, so a damaged file cannot ask for gigabytes. */
static unsigned char *read_box(int fd, unsigned long off, unsigned len) {
    if (len == 0 || len > 8u * 1024u * 1024u) {
        return NULL;
    }
    unsigned char *buf = malloc(len);
    if (buf == NULL) {
        return NULL;
    }
    if (read_at(fd, off, buf, len) != (long)len) {
        free(buf);
        return NULL;
    }
    return buf;
}

/* ---- parser state ---- */

struct table {
    unsigned  count;
    unsigned *data; /* count * stride entries */
};

struct parser {
    int fd;
    struct mp4_video *v;

    int  in_video_track;   /* the trak being parsed is a video track */
    int  have_video;       /* a complete video track was captured */

    unsigned timescale;
    unsigned long duration;
    unsigned width, height;
    char codec[5];
    unsigned nal_size;
    unsigned profile, level;
    unsigned char params[MP4_MAX_PARAMS];
    unsigned params_len;

    struct table stts;     /* pairs: count, delta */
    struct table stsc;     /* triples: first_chunk, per_chunk, desc */
    struct table stsz;     /* sizes (or a single constant) */
    unsigned stsz_constant;
    unsigned stsz_count;
    struct table stco;     /* chunk offsets (co64 stores hi/lo pairs) */
    int stco_64;
    struct table stss;     /* sync sample numbers */
};

static void table_free(struct table *t) {
    free(t->data);
    t->data = NULL;
    t->count = 0;
}

static void parser_reset_track(struct parser *p) {
    p->in_video_track = 0;
    p->timescale = 0;
    p->duration = 0;
    p->width = 0;
    p->height = 0;
    p->codec[0] = '\0';
    p->nal_size = 4;
    p->profile = 0;
    p->level = 0;
    p->params_len = 0;
    table_free(&p->stts);
    table_free(&p->stsc);
    table_free(&p->stsz);
    table_free(&p->stco);
    table_free(&p->stss);
    p->stco_64 = 0;
    p->stsz_constant = 0;
    p->stsz_count = 0;
}

/* ---- individual boxes ---- */

static void parse_mdhd(struct parser *p, const unsigned char *b, unsigned len) {
    if (len < 20) {
        return;
    }
    unsigned version = b[0];
    if (version == 1) {
        if (len < 32) {
            return;
        }
        p->timescale = rd32(b + 20);
        p->duration = rd64(b + 24);
    } else {
        p->timescale = rd32(b + 12);
        p->duration = rd32(b + 16);
    }
}

static void parse_hdlr(struct parser *p, const unsigned char *b, unsigned len) {
    if (len < 12) {
        return;
    }
    p->in_video_track = memcmp(b + 8, "vide", 4) == 0;
}

/* avcC (ISO/IEC 14496-15): the decoder configuration record. The SPS
 * and PPS it carries are copied out as an Annex B stream, which is
 * exactly what has to be fed to the decoder before the first frame. */
static void parse_avcc(struct parser *p, const unsigned char *b, unsigned len) {
    if (len < 7) {
        return;
    }
    p->profile = b[1]; /* profile_idc: 66 baseline, 77 main, 100 high */
    p->level = b[3];
    p->nal_size = (b[4] & 3u) + 1u;

    unsigned pos = 5;
    unsigned out = 0;
    unsigned count = b[pos++] & 0x1Fu; /* SPS entries */

    for (int round = 0; round < 2; round++) {
        for (unsigned i = 0; i < count && pos + 2 <= len; i++) {
            unsigned nal_len = rd16(b + pos);
            pos += 2;
            if (pos + nal_len > len || out + 4 + nal_len > MP4_MAX_PARAMS) {
                return;
            }
            p->params[out++] = 0;
            p->params[out++] = 0;
            p->params[out++] = 0;
            p->params[out++] = 1;
            memcpy(p->params + out, b + pos, nal_len);
            out += nal_len;
            pos += nal_len;
        }
        if (round == 0) {
            if (pos >= len) {
                break;
            }
            count = b[pos++]; /* PPS entries */
        }
    }
    p->params_len = out;
}

/* stsd -> the first sample entry: 'avc1' with the picture size, then
 * the nested avcC. */
static void parse_stsd(struct parser *p, const unsigned char *b, unsigned len) {
    if (len < 16) {
        return;
    }
    unsigned pos = 8; /* version/flags + entry count */
    unsigned entry_size = rd32(b + pos);
    if (entry_size < 86 || pos + entry_size > len) {
        return;
    }
    memcpy(p->codec, b + pos + 4, 4);
    p->codec[4] = '\0';
    p->width = rd16(b + pos + 32);
    p->height = rd16(b + pos + 34);

    /* Sub-boxes start after the 78-byte video sample entry header. */
    unsigned sub = pos + 8 + 78;
    while (sub + 8 <= pos + entry_size) {
        unsigned sz = rd32(b + sub);
        if (sz < 8 || sub + sz > pos + entry_size) {
            break;
        }
        if (memcmp(b + sub + 4, "avcC", 4) == 0) {
            parse_avcc(p, b + sub + 8, sz - 8);
        }
        sub += sz;
    }
}

/* Read a table of `stride` 32-bit words per entry. */
static int parse_table(struct table *t, const unsigned char *b, unsigned len,
                       unsigned stride) {
    if (len < 8) {
        return 0;
    }
    unsigned count = rd32(b + 4);
    if (count > MAX_TABLE || 8 + count * stride * 4 > len) {
        return 0;
    }
    table_free(t);
    if (count == 0) {
        return 1;
    }
    t->data = malloc((size_t)count * stride * sizeof(unsigned));
    if (t->data == NULL) {
        return 0;
    }
    for (unsigned i = 0; i < count * stride; i++) {
        t->data[i] = rd32(b + 8 + i * 4);
    }
    t->count = count;
    return 1;
}

static void parse_stsz(struct parser *p, const unsigned char *b, unsigned len) {
    if (len < 12) {
        return;
    }
    p->stsz_constant = rd32(b + 4);
    p->stsz_count = rd32(b + 8);
    if (p->stsz_constant != 0 || p->stsz_count == 0) {
        return;
    }
    if (p->stsz_count > MAX_SAMPLES || 12 + p->stsz_count * 4 > len) {
        p->stsz_count = 0;
        return;
    }
    table_free(&p->stsz);
    p->stsz.data = malloc((size_t)p->stsz_count * sizeof(unsigned));
    if (p->stsz.data == NULL) {
        p->stsz_count = 0;
        return;
    }
    for (unsigned i = 0; i < p->stsz_count; i++) {
        p->stsz.data[i] = rd32(b + 12 + i * 4);
    }
    p->stsz.count = p->stsz_count;
}

/* co64 holds 64-bit offsets; they are stored as high/low pairs so the
 * rest of the code can treat both tables the same way. */
static void parse_co64(struct parser *p, const unsigned char *b, unsigned len) {
    if (len < 8) {
        return;
    }
    unsigned count = rd32(b + 4);
    if (count > MAX_TABLE || 8 + count * 8 > len) {
        return;
    }
    table_free(&p->stco);
    if (count == 0) {
        return;
    }
    p->stco.data = malloc((size_t)count * 2 * sizeof(unsigned));
    if (p->stco.data == NULL) {
        return;
    }
    for (unsigned i = 0; i < count; i++) {
        p->stco.data[i * 2] = rd32(b + 8 + i * 8);
        p->stco.data[i * 2 + 1] = rd32(b + 8 + i * 8 + 4);
    }
    p->stco.count = count;
    p->stco_64 = 1;
}

/* Chunk offsets come from stco (32-bit) or co64 (64-bit); this hides
 * the difference from the sample-table builder. */
static unsigned long chunk_offset(const struct parser *p, unsigned chunk) {
    if (chunk == 0 || chunk > p->stco.count || p->stco.data == NULL) {
        return 0;
    }
    if (p->stco_64) {
        return ((unsigned long)p->stco.data[(chunk - 1) * 2] << 32) |
               p->stco.data[(chunk - 1) * 2 + 1];
    }
    return p->stco.data[chunk - 1];
}

/* ---- box tree walk ---- */

static int is_container(const unsigned char *type) {
    static const char *const names[] = { "moov", "trak", "mdia", "minf",
                                         "stbl", "edts", "udta" };
    for (unsigned i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        if (memcmp(type, names[i], 4) == 0) {
            return 1;
        }
    }
    return 0;
}

static int capture_track(struct parser *p);

static int walk(struct parser *p, unsigned long start, unsigned long end,
                int depth) {
    unsigned char head[16];

    if (depth > 8) {
        return 0;
    }
    unsigned long pos = start;
    while (pos + 8 <= end) {
        if (read_at(p->fd, pos, head, 8) != 8) {
            return 0;
        }
        unsigned long size = rd32(head);
        unsigned long body = pos + 8;

        if (size == 1) {
            if (read_at(p->fd, pos + 8, head + 8, 8) != 8) {
                return 0;
            }
            size = rd64(head + 8);
            body = pos + 16;
        } else if (size == 0) {
            size = end - pos; /* "to the end of the file" */
        }
        if (size < 8 || pos + size > end) {
            return 0;
        }

        const unsigned char *type = head + 4;
        unsigned body_len = (unsigned)(pos + size - body);

        if (memcmp(type, "trak", 4) == 0) {
            parser_reset_track(p);
            walk(p, body, pos + size, depth + 1);
            if (p->in_video_track && !p->have_video) {
                if (!capture_track(p)) {
                    return 0;
                }
            }
            parser_reset_track(p);
        } else if (is_container(type)) {
            walk(p, body, pos + size, depth + 1);
        } else {
            static const struct {
                const char *name;
                int kind; /* 0 mdhd, 1 hdlr, 2 stsd, 3 stts, 4 stsc,
                           * 5 stsz, 6 stco, 7 co64, 8 stss */
            } leaves[] = {
                { "mdhd", 0 }, { "hdlr", 1 }, { "stsd", 2 }, { "stts", 3 },
                { "stsc", 4 }, { "stsz", 5 }, { "stco", 6 }, { "co64", 7 },
                { "stss", 8 },
            };
            for (unsigned i = 0; i < sizeof(leaves) / sizeof(leaves[0]); i++) {
                if (memcmp(type, leaves[i].name, 4) != 0) {
                    continue;
                }
                unsigned char *b = read_box(p->fd, body, body_len);
                if (b == NULL) {
                    break;
                }
                switch (leaves[i].kind) {
                case 0: parse_mdhd(p, b, body_len); break;
                case 1: parse_hdlr(p, b, body_len); break;
                case 2: parse_stsd(p, b, body_len); break;
                case 3: parse_table(&p->stts, b, body_len, 2); break;
                case 4: parse_table(&p->stsc, b, body_len, 3); break;
                case 5: parse_stsz(p, b, body_len); break;
                case 6:
                    parse_table(&p->stco, b, body_len, 1);
                    p->stco_64 = 0;
                    break;
                case 7: parse_co64(p, b, body_len); break;
                case 8: parse_table(&p->stss, b, body_len, 1); break;
                default: break;
                }
                free(b);
                break;
            }
        }
        pos += size;
    }
    return 1;
}

/* ---- sample table assembly ---- */

/* Turn stsc + stco + stsz + stts + stss into one flat array: for each
 * sample, where it is, how big it is, how long it lasts and whether
 * it can be decoded on its own. */
static int capture_track(struct parser *p) {
    struct mp4_video *v = p->v;
    unsigned n = p->stsz_count; /* stsz always carries the sample count */

    if (n == 0 || p->stco.count == 0 || p->stsc.count == 0) {
        return 1; /* not a track we can play; keep looking */
    }
    if (n > MAX_SAMPLES) {
        return 0;
    }

    struct mp4_sample *samples = calloc(n, sizeof(*samples));
    if (samples == NULL) {
        return 0;
    }

    /* Walk the chunks: stsc says "from chunk X on, every chunk holds
     * N samples", so each entry covers a run of chunks. */
    unsigned sample = 0;
    unsigned chunks = p->stco.count;
    for (unsigned entry = 0; entry < p->stsc.count && sample < n; entry++) {
        unsigned first = p->stsc.data[entry * 3];          /* 1-based */
        unsigned per_chunk = p->stsc.data[entry * 3 + 1];
        unsigned last = (entry + 1 < p->stsc.count)
                            ? p->stsc.data[(entry + 1) * 3] - 1
                            : chunks;
        if (first == 0 || per_chunk == 0) {
            continue;
        }
        for (unsigned chunk = first; chunk <= last && sample < n; chunk++) {
            unsigned long offset = chunk_offset(p, chunk);
            if (offset == 0) {
                break;
            }
            for (unsigned i = 0; i < per_chunk && sample < n; i++) {
                unsigned size = p->stsz_constant != 0 ? p->stsz_constant
                                                      : p->stsz.data[sample];
                samples[sample].offset = offset;
                samples[sample].size = size;
                offset += size;
                sample++;
            }
        }
    }

    /* Durations from stts (run-length encoded). */
    unsigned idx = 0;
    for (unsigned e = 0; e < p->stts.count && idx < n; e++) {
        unsigned count = p->stts.data[e * 2];
        unsigned delta = p->stts.data[e * 2 + 1];
        for (unsigned i = 0; i < count && idx < n; i++) {
            samples[idx++].duration = delta;
        }
    }
    while (idx < n) {
        samples[idx].duration = p->stts.count ? p->stts.data[1] : 0;
        idx++;
    }

    /* Key frames: stss lists them (1-based). No stss means every
     * sample is a key frame. */
    if (p->stss.count == 0) {
        for (unsigned i = 0; i < n; i++) {
            samples[i].sync = 1;
        }
    } else {
        for (unsigned i = 0; i < p->stss.count; i++) {
            unsigned s = p->stss.data[i];
            if (s >= 1 && s <= n) {
                samples[s - 1].sync = 1;
            }
        }
    }

    unsigned max_size = 0;
    for (unsigned i = 0; i < n; i++) {
        if (samples[i].size > max_size) {
            max_size = samples[i].size;
        }
    }

    v->samples = samples;
    v->sample_count = n;
    v->max_sample_size = max_size;
    v->width = p->width;
    v->height = p->height;
    v->timescale = p->timescale ? p->timescale : 1000;
    v->duration = p->duration;
    v->nal_length_size = p->nal_size;
    v->profile = p->profile;
    v->level = p->level;
    memcpy(v->codec, p->codec, sizeof(v->codec));
    memcpy(v->params, p->params, p->params_len);
    v->params_len = p->params_len;
    p->have_video = 1;
    return 1;
}

/* ---- public API ---- */

int mp4_open(struct mp4_video *v, const char *path) {
    memset(v, 0, sizeof(*v));
    v->fd = open(path, O_RDONLY);
    if (v->fd < 0) {
        return MP4_E_OPEN;
    }

    unsigned long size = (unsigned long)lseek(v->fd, 0, SEEK_END);
    if ((long)size <= 0) {
        close(v->fd);
        v->fd = -1;
        return MP4_E_FORMAT;
    }

    unsigned char head[8];
    if (read_at(v->fd, 0, head, 8) != 8 || memcmp(head + 4, "ftyp", 4) != 0) {
        /* Some files start with another box; only refuse if the first
         * box header is not a box at all. */
        if (rd32(head) < 8) {
            close(v->fd);
            v->fd = -1;
            return MP4_E_FORMAT;
        }
    }

    struct parser p;
    memset(&p, 0, sizeof(p));
    p.fd = v->fd;
    p.v = v;
    p.nal_size = 4;

    int ok = walk(&p, 0, size, 0);
    parser_reset_track(&p);

    if (!ok && !p.have_video) {
        mp4_close(v);
        return MP4_E_FORMAT;
    }
    if (!p.have_video || v->sample_count == 0) {
        mp4_close(v);
        return MP4_E_NOVIDEO;
    }
    if (memcmp(v->codec, "avc1", 4) != 0 || v->params_len == 0) {
        return MP4_E_CODEC; /* the caller may still want the details */
    }
    return MP4_OK;
}

long mp4_read_sample(struct mp4_video *v, unsigned index, unsigned char *buf,
                     unsigned buf_size) {
    if (v == NULL || index >= v->sample_count || buf == NULL) {
        return -1;
    }
    struct mp4_sample *s = &v->samples[index];
    if (s->size > buf_size) {
        return -1;
    }
    if (read_at(v->fd, s->offset, buf, s->size) != (long)s->size) {
        return -1;
    }

    /* MP4 stores every NAL unit with a length prefix; H.264 decoders
     * want Annex B start codes. Both are the same width when the
     * length field is 4 bytes, so the rewrite happens in place. */
    unsigned pos = 0;
    unsigned lsz = v->nal_length_size;
    if (lsz == 4) {
        while (pos + 4 <= s->size) {
            unsigned nal = rd32(buf + pos);
            if (nal == 0 || pos + 4 + nal > s->size) {
                break;
            }
            buf[pos] = 0;
            buf[pos + 1] = 0;
            buf[pos + 2] = 0;
            buf[pos + 3] = 1;
            pos += 4 + nal;
        }
        return (long)s->size;
    }

    /* Shorter length fields need the data moved: walk backwards so the
     * expansion never overwrites data we still have to read. */
    unsigned char *tmp = malloc(s->size);
    if (tmp == NULL) {
        return -1;
    }
    memcpy(tmp, buf, s->size);
    unsigned out = 0;
    pos = 0;
    while (pos + lsz <= s->size) {
        unsigned nal = 0;
        for (unsigned i = 0; i < lsz; i++) {
            nal = (nal << 8) | tmp[pos + i];
        }
        pos += lsz;
        if (nal == 0 || pos + nal > s->size || out + 4 + nal > buf_size) {
            break;
        }
        buf[out++] = 0;
        buf[out++] = 0;
        buf[out++] = 0;
        buf[out++] = 1;
        memcpy(buf + out, tmp + pos, nal);
        out += nal;
        pos += nal;
    }
    free(tmp);
    return (long)out;
}

unsigned mp4_prev_sync(const struct mp4_video *v, unsigned index) {
    if (index >= v->sample_count) {
        index = v->sample_count ? v->sample_count - 1 : 0;
    }
    for (unsigned i = index + 1; i-- > 0;) {
        if (v->samples[i].sync) {
            return i;
        }
    }
    return 0;
}

unsigned long mp4_sample_time_ms(const struct mp4_video *v, unsigned index) {
    unsigned long ticks = 0;
    for (unsigned i = 0; i < index && i < v->sample_count; i++) {
        ticks += v->samples[i].duration;
    }
    return v->timescale ? ticks * 1000ul / v->timescale : 0;
}

void mp4_close(struct mp4_video *v) {
    if (v->fd >= 0) {
        close(v->fd);
        v->fd = -1;
    }
    free(v->samples);
    v->samples = NULL;
    v->sample_count = 0;
}

const char *mp4_strerror(int code) {
    switch (code) {
    case MP4_OK:        return "ok";
    case MP4_E_OPEN:    return "cannot open file";
    case MP4_E_FORMAT:  return "not an MP4 file";
    case MP4_E_NOVIDEO: return "no video track";
    case MP4_E_CODEC:   return "video track is not H.264 (avc1)";
    case MP4_E_MEMORY:  return "out of memory";
    case MP4_E_TOOBIG:  return "sample tables too large";
    default:            return "unknown error";
    }
}
