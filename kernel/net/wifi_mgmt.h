/*
 * wifi_mgmt.h - IEEE 802.11 management frame construction/parsing
 *
 * This is the piece wpa_crypto.h explicitly said TUS didn't have:
 * building and parsing probe request/response, open-system
 * authentication, and association request/response frames, plus the
 * RSN information element WPA2-PSK association needs to advertise
 * (CCMP pairwise/group cipher, PSK AKM).
 *
 * What this is NOT: a full join. Actually sending these frames needs
 * the ath9k-htc USB command/data protocol (kernel/drivers/ath9k/ only
 * loads firmware today - see its header, there is no TX/RX path at
 * all), and there is still no way to boot-test a real association in
 * this environment: QEMU emulates no 802.11 hardware and no virtual
 * AP, so nothing here could send a frame to anything real even with
 * that protocol written. Building the USB command/data path is a
 * separate, large prerequisite (see kernel/net/wpa_crypto.h's own
 * note - the same limitation, one layer up).
 *
 * What IS real and boot-verified: the frame layer itself. Every
 * builder here produces bytes matching the IEEE 802.11-2020 frame
 * formats (7.3.1 management frame body, 9.4.2.25 RSNE), and every
 * parser round-trips them, checked by wifi_mgmt_selftest() against
 * fixed reference frames (including a real-world RSN IE byte
 * sequence: CCMP/CCMP/PSK, the overwhelmingly common WPA2-Personal
 * configuration) the same way wpa_selftest() checks the crypto core
 * against IEEE 802.11i-2004 Annex H.4. When the USB layer exists,
 * this is the piece that turns received bytes into structured frames
 * and wpa_crypto's PMK/PTK into bytes to send - it does not need to
 * change to plug in.
 */

#ifndef TUS_NET_WIFI_MGMT_H
#define TUS_NET_WIFI_MGMT_H

#include <stddef.h>
#include <stdint.h>

#define WIFI_MGMT_MAX_SSID   32
#define WIFI_MGMT_MAX_FRAME  256

/* 802.11 frame control subtypes we build/parse (type = 0, management). */
#define WIFI_SUBTYPE_ASSOC_REQ    0x00
#define WIFI_SUBTYPE_ASSOC_RESP   0x01
#define WIFI_SUBTYPE_PROBE_REQ    0x04
#define WIFI_SUBTYPE_PROBE_RESP   0x05
#define WIFI_SUBTYPE_AUTH         0x0B

/* Parsed RSN information element (802.11-2020 9.4.2.25). Only the
 * fields WPA2-PSK association needs: cipher suites and AKM. Every
 * suite is identified by its 4-byte OUI+type, but since TUS only
 * ever emits/expects the 00-0F-AC (IEEE 802.11) OUI with CCMP (4) and
 * PSK (2), those are exposed as booleans rather than raw suite lists. */
struct wifi_rsn_info {
    int valid;
    int group_is_ccmp;
    int pairwise_has_ccmp;
    int akm_has_psk;
};

/* Builds an 802.11 probe request: broadcast destination, given SSID
 * (open, no BSSID filter). Returns the frame length, or -1 if `ssid`
 * is too long for `buf`. */
int wifi_build_probe_req(uint8_t *buf, size_t buf_len,
                          const uint8_t sa[6],
                          const char *ssid, size_t ssid_len);

/* Parses a probe response (or beacon - same body layout from the
 * fixed fields on) far enough to pull the SSID, BSSID and RSN IE.
 * Returns 0 on a well-formed frame, -1 otherwise. */
int wifi_parse_probe_resp(const uint8_t *frame, size_t len,
                           uint8_t bssid_out[6],
                           char ssid_out[WIFI_MGMT_MAX_SSID + 1],
                           struct wifi_rsn_info *rsn_out);

/* Builds an open-system authentication frame (seq 1, the station's
 * half - the only kind a client sends; WPA2-PSK does the real auth
 * in the 4-way handshake, not here, so this is always "open"). */
int wifi_build_auth(uint8_t *buf, size_t buf_len,
                     const uint8_t da[6], const uint8_t sa[6],
                     const uint8_t bssid[6], uint16_t seq_num);

/* Parses an authentication frame, returning its status code (0 =
 * successful, per 802.11 Table 9-50) via *status_out. Returns 0 if
 * the frame parsed, -1 otherwise (status_out is still meaningful only
 * when this returns 0). */
int wifi_parse_auth(const uint8_t *frame, size_t len, uint16_t *status_out);

/* Builds an association request advertising WPA2-PSK support: an RSN
 * IE with CCMP group+pairwise and PSK AKM, the configuration every
 * ath9k-compatible WPA2-Personal AP expects. */
int wifi_build_assoc_req(uint8_t *buf, size_t buf_len,
                          const uint8_t da[6], const uint8_t sa[6],
                          const uint8_t bssid[6],
                          const char *ssid, size_t ssid_len);

/* Parses an association response, returning its status code the same
 * way wifi_parse_auth() does. */
int wifi_parse_assoc_resp(const uint8_t *frame, size_t len,
                           uint16_t *status_out);

/* Round-trips every builder/parser above against fixed reference
 * frames, including a real-world CCMP/CCMP/PSK RSN IE byte sequence.
 * Returns 0 if every check matches, -1 otherwise (prints which). */
int wifi_mgmt_selftest(void);

#endif /* TUS_NET_WIFI_MGMT_H */
