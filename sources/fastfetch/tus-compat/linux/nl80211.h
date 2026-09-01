/* Stub for TUS: see linux/netlink.h in this same compat tree - TUS
 * has no netlink sockets (and no wifi hardware at all), so
 * wifi_linux.c's socket(AF_NETLINK, ...) always fails first and none
 * of this ever parses real scan/station data. Every symbol below is
 * only ever used as an attribute-index constant (building or
 * switching on netlink attribute types), never dereferenced as real
 * kernel-supplied data, so - unlike rtnetlink.h/genetlink.h, which
 * use the real stable uapi values because they cost nothing - exact
 * values here don't matter, only that each name is distinct. */
#ifndef _LINUX_NL80211_H
#define _LINUX_NL80211_H

enum nl80211_commands {
    NL80211_CMD_GET_STATION = 1,
    NL80211_CMD_GET_INTERFACE,
    NL80211_CMD_GET_SCAN,
};

enum nl80211_attrs {
    NL80211_ATTR_IFINDEX = 1,
    NL80211_ATTR_STA_INFO,
    NL80211_ATTR_BSS,
    NL80211_ATTR_CHANNEL_WIDTH,
};

enum nl80211_bss {
    NL80211_BSS_BSSID = 1,
    NL80211_BSS_FREQUENCY,
    NL80211_BSS_CAPABILITY,
    NL80211_BSS_INFORMATION_ELEMENTS,
    NL80211_BSS_SIGNAL_MBM,
    NL80211_BSS_STATUS,
    NL80211_BSS_BEACON_IES,
};

enum nl80211_bss_status {
    NL80211_BSS_STATUS_ASSOCIATED = 1,
};

enum nl80211_sta_info {
    NL80211_STA_INFO_SIGNAL = 1,
    NL80211_STA_INFO_TX_BITRATE,
    NL80211_STA_INFO_RX_BITRATE,
};

/* Unlike the enums above, wifi_linux.c mixes named references to
 * some of these with bare bumeric literals for others (newer ones,
 * commented with their real name, e.g. "case 18 [> ..._320_MHZ_WIDTH
 * <]") in the SAME switch statements - so these two enums are the
 * exception to this file's "self-consistent values are fine" rule:
 * they must carry their real kernel values, verified directly against
 * wifi_linux.c's own literal-with-comment usages, or the named and
 * literal cases collide. */
enum nl80211_rate_info {
    NL80211_RATE_INFO_BITRATE = 0,
    NL80211_RATE_INFO_MCS = 1,
    NL80211_RATE_INFO_40_MHZ_WIDTH = 2,
    NL80211_RATE_INFO_BITRATE32 = 4,
    NL80211_RATE_INFO_VHT_MCS = 5,
    NL80211_RATE_INFO_80_MHZ_WIDTH = 7,
    NL80211_RATE_INFO_80P80_MHZ_WIDTH = 8,
    NL80211_RATE_INFO_160_MHZ_WIDTH = 9,
    NL80211_RATE_INFO_10_MHZ_WIDTH = 10,
    NL80211_RATE_INFO_5_MHZ_WIDTH = 11,
    NL80211_RATE_INFO_HE_MCS = 13, /* verified: wifi_linux.c:270 */
    NL80211_RATE_INFO_320_MHZ_WIDTH = 18, /* verified: wifi_linux.c:237 */
    NL80211_RATE_INFO_EHT_MCS = 19, /* verified: wifi_linux.c:267 */
    NL80211_RATE_INFO_S1G_MCS = 23, /* verified: wifi_linux.c:264 */
    NL80211_RATE_INFO_1_MHZ_WIDTH = 25, /* verified: wifi_linux.c:246 */
    NL80211_RATE_INFO_2_MHZ_WIDTH = 26, /* verified: wifi_linux.c:249 */
    NL80211_RATE_INFO_4_MHZ_WIDTH = 27, /* verified: wifi_linux.c:252 */
    NL80211_RATE_INFO_8_MHZ_WIDTH = 28, /* verified: wifi_linux.c:255 */
    NL80211_RATE_INFO_16_MHZ_WIDTH = 29, /* verified: wifi_linux.c:258 */
    NL80211_RATE_INFO_UHR_MCS = 30, /* verified: wifi_linux.c:261 */
};

enum nl80211_chan_width {
    NL80211_CHAN_WIDTH_20_NOHT = 0,
    NL80211_CHAN_WIDTH_20 = 1,
    NL80211_CHAN_WIDTH_40 = 2,
    NL80211_CHAN_WIDTH_80 = 3,
    NL80211_CHAN_WIDTH_80P80 = 4,
    NL80211_CHAN_WIDTH_160 = 5,
    NL80211_CHAN_WIDTH_5 = 6,
    NL80211_CHAN_WIDTH_10 = 7,
    NL80211_CHAN_WIDTH_1 = 8,
    NL80211_CHAN_WIDTH_2 = 9,
    NL80211_CHAN_WIDTH_4 = 10,
    NL80211_CHAN_WIDTH_8 = 11,
    NL80211_CHAN_WIDTH_16 = 12,
    NL80211_CHAN_WIDTH_320 = 13, /* verified: wifi_linux.c:325 */
};

#endif
