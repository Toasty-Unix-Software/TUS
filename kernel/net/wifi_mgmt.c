/*
 * wifi_mgmt.c - IEEE 802.11 management frame construction/parsing
 *
 * See wifi_mgmt.h for what this is and is not. Byte layouts follow
 * IEEE 802.11-2020 7.3.1 (frame formats) and 9.4.2.25 (RSNE).
 */

#include "wifi_mgmt.h"
#include "../core/klib.h"

/* klib has no memcmp - nothing else in the kernel needed one before
 * RSN IE suite comparison. */
static int wifi_memcmp(const void *a, const void *b, size_t n) {
    const uint8_t *pa = (const uint8_t *)a, *pb = (const uint8_t *)b;
    for (size_t i = 0; i < n; i++) {
        if (pa[i] != pb[i]) return (int)pa[i] - (int)pb[i];
    }
    return 0;
}
#define memcmp wifi_memcmp

/* ---- 802.11 element IDs we care about ---- */
#define IE_SSID   0
#define IE_RSN    48

/* The only OUI/suite combination TUS emits or expects: 00-0F-AC is
 * the IEEE 802.11 OUI, suite type 4 is CCMP, suite type 2 is PSK. */
static const uint8_t IEEE80211_OUI[3] = { 0x00, 0x0F, 0xAC };
#define SUITE_CCMP 4
#define SUITE_PSK  2

/* ---- little-endian helpers (802.11 fields are all LE on the wire) ---- */

static void put_u16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)(v & 0xff);
    p[1] = (uint8_t)(v >> 8);
}

static uint16_t get_u16(const uint8_t *p) {
    return (uint16_t)(p[0] | (p[1] << 8));
}

/* ---- generic mgmt header (7.3.1.3 - da/sa/bssid order fixed for
 * type=management: addr1=DA, addr2=SA, addr3=BSSID) ---- */

struct mgmt_hdr {
    uint16_t fctl;
    uint16_t duration;
    uint8_t da[6];
    uint8_t sa[6];
    uint8_t bssid[6];
    uint16_t seq_ctl;
};

#define MGMT_HDR_LEN 24

static void write_mgmt_hdr(uint8_t *buf, uint8_t subtype,
                            const uint8_t da[6], const uint8_t sa[6],
                            const uint8_t bssid[6]) {
    /* Frame Control: protocol version 0, type 0 (management),
     * subtype in bits 4-7. */
    uint16_t fctl = (uint16_t)((subtype & 0x0F) << 4);
    put_u16(buf + 0, fctl);
    put_u16(buf + 2, 0);           /* duration, unused here */
    memcpy(buf + 4, da, 6);
    memcpy(buf + 10, sa, 6);
    memcpy(buf + 16, bssid, 6);
    put_u16(buf + 22, 0);          /* seq/frag control */
}

static int mgmt_subtype(const uint8_t *frame, size_t len) {
    if (len < MGMT_HDR_LEN) return -1;
    uint16_t fctl = get_u16(frame);
    if (((fctl >> 2) & 0x3) != 0) return -1; /* not a management frame */
    return (fctl >> 4) & 0x0F;
}

/* ---- element (IE) writer/reader ---- */

static int put_ie(uint8_t *buf, size_t buf_len, size_t off,
                   uint8_t id, const uint8_t *data, uint8_t data_len) {
    if (off + 2u + data_len > buf_len) return -1;
    buf[off] = id;
    buf[off + 1] = data_len;
    if (data_len) memcpy(buf + off + 2, data, data_len);
    return (int)(off + 2 + data_len);
}

/* Finds element `id` in a tagged-parameters region; returns its data
 * pointer via *out and length via *out_len, 0 on success. */
static int find_ie(const uint8_t *p, size_t len, uint8_t id,
                    const uint8_t **out, uint8_t *out_len) {
    size_t i = 0;
    while (i + 2 <= len) {
        uint8_t this_id = p[i];
        uint8_t this_len = p[i + 1];
        if (i + 2 + this_len > len) return -1; /* truncated */
        if (this_id == id) {
            *out = p + i + 2;
            *out_len = this_len;
            return 0;
        }
        i += 2 + this_len;
    }
    return -1;
}

/* ---- RSN IE (9.4.2.25) ----
 *
 * Version(2) Group Cipher Suite(4) Pairwise Count(2) Pairwise
 * Suites(4*n) AKM Count(2) AKM Suites(4*n) [Capabilities(2)] ...
 * TUS only ever builds/expects exactly one pairwise suite (CCMP) and
 * one AKM (PSK), which is what every consumer WPA2-Personal AP does.
 */

static int build_rsn_ie(uint8_t *out, size_t out_len) {
    if (out_len < 20) return -1;
    size_t o = 0;
    put_u16(out + o, 1); o += 2;                    /* version */
    memcpy(out + o, IEEE80211_OUI, 3); out[o + 3] = SUITE_CCMP; o += 4;
    put_u16(out + o, 1); o += 2;                     /* pairwise count */
    memcpy(out + o, IEEE80211_OUI, 3); out[o + 3] = SUITE_CCMP; o += 4;
    put_u16(out + o, 1); o += 2;                     /* AKM count */
    memcpy(out + o, IEEE80211_OUI, 3); out[o + 3] = SUITE_PSK; o += 4;
    put_u16(out + o, 0); o += 2;                     /* RSN capabilities */
    return (int)o;
}

static int parse_rsn_ie(const uint8_t *ie, uint8_t ie_len,
                         struct wifi_rsn_info *out) {
    memset(out, 0, sizeof(*out));
    if (ie_len < 8) return -1;
    size_t o = 2; /* skip version */

    if (o + 4 > ie_len) return -1;
    out->group_is_ccmp = memcmp(ie + o, IEEE80211_OUI, 3) == 0 &&
                          ie[o + 3] == SUITE_CCMP;
    o += 4;

    if (o + 2 > ie_len) return -1;
    uint16_t pairwise_count = get_u16(ie + o); o += 2;
    for (uint16_t i = 0; i < pairwise_count; i++) {
        if (o + 4 > ie_len) return -1;
        if (memcmp(ie + o, IEEE80211_OUI, 3) == 0 && ie[o + 3] == SUITE_CCMP) {
            out->pairwise_has_ccmp = 1;
        }
        o += 4;
    }

    if (o + 2 > ie_len) return -1;
    uint16_t akm_count = get_u16(ie + o); o += 2;
    for (uint16_t i = 0; i < akm_count; i++) {
        if (o + 4 > ie_len) return -1;
        if (memcmp(ie + o, IEEE80211_OUI, 3) == 0 && ie[o + 3] == SUITE_PSK) {
            out->akm_has_psk = 1;
        }
        o += 4;
    }

    out->valid = 1;
    return 0;
}

/* ---- probe request / response ---- */

int wifi_build_probe_req(uint8_t *buf, size_t buf_len,
                          const uint8_t sa[6],
                          const char *ssid, size_t ssid_len) {
    if (ssid_len > WIFI_MGMT_MAX_SSID) return -1;
    if (buf_len < MGMT_HDR_LEN + 2 + ssid_len) return -1;

    static const uint8_t bcast[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    write_mgmt_hdr(buf, WIFI_SUBTYPE_PROBE_REQ, bcast, sa, bcast);

    int off = put_ie(buf, buf_len, MGMT_HDR_LEN, IE_SSID,
                      (const uint8_t *)ssid, (uint8_t)ssid_len);
    return off;
}

int wifi_parse_probe_resp(const uint8_t *frame, size_t len,
                           uint8_t bssid_out[6],
                           char ssid_out[WIFI_MGMT_MAX_SSID + 1],
                           struct wifi_rsn_info *rsn_out) {
    int subtype = mgmt_subtype(frame, len);
    if (subtype != WIFI_SUBTYPE_PROBE_RESP) return -1;

    memcpy(bssid_out, frame + 16, 6);

    /* Fixed fields: timestamp(8) beacon interval(2) capability(2)
     * before the tagged parameters begin. */
    size_t fixed_off = MGMT_HDR_LEN + 12;
    if (fixed_off > len) return -1;

    const uint8_t *ssid_ie;
    uint8_t ssid_len;
    if (find_ie(frame + fixed_off, len - fixed_off, IE_SSID,
                &ssid_ie, &ssid_len) != 0) {
        return -1;
    }
    if (ssid_len > WIFI_MGMT_MAX_SSID) return -1;
    memcpy(ssid_out, ssid_ie, ssid_len);
    ssid_out[ssid_len] = '\0';

    const uint8_t *rsn_ie;
    uint8_t rsn_len;
    if (find_ie(frame + fixed_off, len - fixed_off, IE_RSN,
                &rsn_ie, &rsn_len) == 0) {
        parse_rsn_ie(rsn_ie, rsn_len, rsn_out);
    } else {
        memset(rsn_out, 0, sizeof(*rsn_out));
    }

    return 0;
}

/* ---- authentication (open system, 802.11-2020 9.3.3.12) ---- */

int wifi_build_auth(uint8_t *buf, size_t buf_len,
                     const uint8_t da[6], const uint8_t sa[6],
                     const uint8_t bssid[6], uint16_t seq_num) {
    if (buf_len < MGMT_HDR_LEN + 6) return -1;
    write_mgmt_hdr(buf, WIFI_SUBTYPE_AUTH, da, sa, bssid);

    put_u16(buf + MGMT_HDR_LEN + 0, 0);        /* auth algorithm: open */
    put_u16(buf + MGMT_HDR_LEN + 2, seq_num);  /* transaction seq */
    put_u16(buf + MGMT_HDR_LEN + 4, 0);        /* status: successful */
    return MGMT_HDR_LEN + 6;
}

int wifi_parse_auth(const uint8_t *frame, size_t len, uint16_t *status_out) {
    int subtype = mgmt_subtype(frame, len);
    if (subtype != WIFI_SUBTYPE_AUTH) return -1;
    if (len < MGMT_HDR_LEN + 6) return -1;
    *status_out = get_u16(frame + MGMT_HDR_LEN + 4);
    return 0;
}

/* ---- association request / response ---- */

int wifi_build_assoc_req(uint8_t *buf, size_t buf_len,
                          const uint8_t da[6], const uint8_t sa[6],
                          const uint8_t bssid[6],
                          const char *ssid, size_t ssid_len) {
    if (ssid_len > WIFI_MGMT_MAX_SSID) return -1;
    if (buf_len < MGMT_HDR_LEN + 4) return -1;
    write_mgmt_hdr(buf, WIFI_SUBTYPE_ASSOC_REQ, da, sa, bssid);

    size_t o = MGMT_HDR_LEN;
    put_u16(buf + o, 0x0431); o += 2; /* capability: ESS, privacy, short-slot */
    put_u16(buf + o, 0);      o += 2; /* listen interval */

    int r = put_ie(buf, buf_len, o, IE_SSID,
                    (const uint8_t *)ssid, (uint8_t)ssid_len);
    if (r < 0) return -1;
    o = (size_t)r;

    uint8_t rsn_body[20];
    int rsn_len = build_rsn_ie(rsn_body, sizeof(rsn_body));
    if (rsn_len < 0) return -1;
    r = put_ie(buf, buf_len, o, IE_RSN, rsn_body, (uint8_t)rsn_len);
    if (r < 0) return -1;
    return r;
}

int wifi_parse_assoc_resp(const uint8_t *frame, size_t len,
                           uint16_t *status_out) {
    int subtype = mgmt_subtype(frame, len);
    if (subtype != WIFI_SUBTYPE_ASSOC_RESP) return -1;
    /* capability(2) status(2) AID(2) ... */
    if (len < MGMT_HDR_LEN + 6) return -1;
    *status_out = get_u16(frame + MGMT_HDR_LEN + 2);
    return 0;
}

/* ---- self-test ---- */

static int check(const char *name, int cond) {
    if (cond) {
        kprintf("[wifi_mgmt] PASS %s\n", name);
        return 0;
    }
    kprintf("[wifi_mgmt] FAIL %s\n", name);
    return 1;
}

int wifi_mgmt_selftest(void) {
    int failures = 0;
    static const uint8_t sa[6] = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x01 };
    static const uint8_t bssid[6] = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x02 };
    uint8_t buf[WIFI_MGMT_MAX_FRAME];

    /* probe request round-trip: build, then re-derive the subtype
     * and SSID element by hand (there is no probe-request parser -
     * a station never receives its own probe requests - so this
     * checks the bytes directly). */
    int n = wifi_build_probe_req(buf, sizeof(buf), sa, "TUSNET", 6);
    failures += check("probe request builds", n == MGMT_HDR_LEN + 2 + 6);
    failures += check("probe request subtype",
                       mgmt_subtype(buf, (size_t)n) == WIFI_SUBTYPE_PROBE_REQ);
    failures += check("probe request SSID IE",
                       n > 0 && buf[MGMT_HDR_LEN] == IE_SSID &&
                       buf[MGMT_HDR_LEN + 1] == 6 &&
                       memcmp(buf + MGMT_HDR_LEN + 2, "TUSNET", 6) == 0);

    /* probe response parse: hand-build a reference frame (fixed
     * fields + SSID IE + a real-world CCMP/CCMP/PSK RSN IE) and parse
     * it back. */
    {
        uint8_t frame[128];
        write_mgmt_hdr(frame, WIFI_SUBTYPE_PROBE_RESP, sa, bssid, bssid);
        size_t o = MGMT_HDR_LEN;
        memset(frame + o, 0, 8); o += 8;   /* timestamp */
        put_u16(frame + o, 100); o += 2;   /* beacon interval */
        put_u16(frame + o, 0x0431); o += 2; /* capability */
        int r = put_ie(frame, sizeof(frame), o, IE_SSID,
                        (const uint8_t *)"TUSNET", 6);
        o = (size_t)r;
        uint8_t rsn_body[20];
        int rsn_len = build_rsn_ie(rsn_body, sizeof(rsn_body));
        r = put_ie(frame, sizeof(frame), o, IE_RSN, rsn_body, (uint8_t)rsn_len);
        o = (size_t)r;

        uint8_t got_bssid[6];
        char got_ssid[WIFI_MGMT_MAX_SSID + 1];
        struct wifi_rsn_info rsn;
        int pr = wifi_parse_probe_resp(frame, o, got_bssid, got_ssid, &rsn);
        failures += check("probe response parses", pr == 0);
        failures += check("probe response BSSID",
                           memcmp(got_bssid, bssid, 6) == 0);
        failures += check("probe response SSID",
                           strcmp(got_ssid, "TUSNET") == 0);
        failures += check("probe response RSN: CCMP group",
                           rsn.valid && rsn.group_is_ccmp);
        failures += check("probe response RSN: CCMP pairwise",
                           rsn.pairwise_has_ccmp);
        failures += check("probe response RSN: PSK AKM", rsn.akm_has_psk);
    }

    /* auth round-trip */
    n = wifi_build_auth(buf, sizeof(buf), bssid, sa, bssid, 1);
    failures += check("auth frame builds", n == MGMT_HDR_LEN + 6);
    {
        uint16_t status = 0xffff;
        int pr = wifi_parse_auth(buf, (size_t)n, &status);
        failures += check("auth frame parses", pr == 0);
        failures += check("auth status is 'successful'", status == 0);
    }

    /* assoc request: build, then confirm the RSN IE it embeds is
     * exactly the CCMP/CCMP/PSK one a real WPA2-Personal AP expects
     * (byte-for-byte, not just "an IE exists"). */
    n = wifi_build_assoc_req(buf, sizeof(buf), bssid, sa, bssid, "TUSNET", 6);
    failures += check("assoc request builds", n > 0);
    if (n > 0) {
        static const uint8_t expect_rsn[] = {
            48, 20,                         /* IE id, len */
            0x01, 0x00,                     /* version 1 */
            0x00, 0x0F, 0xAC, 0x04,         /* group: CCMP */
            0x01, 0x00,                     /* pairwise count */
            0x00, 0x0F, 0xAC, 0x04,         /* pairwise: CCMP */
            0x01, 0x00,                     /* AKM count */
            0x00, 0x0F, 0xAC, 0x02,         /* AKM: PSK */
            0x00, 0x00,                     /* capabilities */
        };
        size_t rsn_off = (size_t)n - sizeof(expect_rsn);
        failures += check("assoc request RSN IE matches WPA2-Personal wire format",
                           n >= (int)sizeof(expect_rsn) &&
                           memcmp(buf + rsn_off, expect_rsn, sizeof(expect_rsn)) == 0);
    }

    /* assoc response: hand-build and parse back */
    {
        uint8_t frame[MGMT_HDR_LEN + 6];
        write_mgmt_hdr(frame, WIFI_SUBTYPE_ASSOC_RESP, sa, bssid, bssid);
        put_u16(frame + MGMT_HDR_LEN, 0x0431);
        put_u16(frame + MGMT_HDR_LEN + 2, 0); /* status: successful */
        put_u16(frame + MGMT_HDR_LEN + 4, 1); /* AID */
        uint16_t status = 0xffff;
        int pr = wifi_parse_assoc_resp(frame, sizeof(frame), &status);
        failures += check("assoc response parses", pr == 0);
        failures += check("assoc status is 'successful'", status == 0);
    }

    if (failures == 0) {
        kprintf("[wifi_mgmt] selftest: all checks passed\n");
    } else {
        kprintf("[wifi_mgmt] selftest: %d check(s) FAILED\n", failures);
    }
    return failures == 0 ? 0 : -1;
}
