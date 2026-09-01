/* Stub for TUS: see drm.h in this same directory - TUS has no DRM at
 * all (VBE framebuffer only), so drm.c's open("/dev/dri/...") always
 * fails with ENOENT before any ioctl here is ever issued. Unlike the
 * vendor GPU-info ioctls in radeon_drm.h/amdgpu_drm.h/nouveau_drm.h,
 * these seven DRM_IOCTL_MODE_* are core, decades-stable DRM ABI (not
 * driver-specific), so the real command numbers and struct layouts
 * are used throughout - they cost nothing to get right and several
 * fields (drm_mode_modeinfo especially) are exactly as real Linux
 * defines them.
 *
 * DRM_MODE_CONNECTOR_SPI/USB are the one place this matters
 * functionally, not just for tidiness: drm.c mixes named references
 * for most connector types with two bare literals commented with
 * their real name ("case 19 [>DRM_MODE_CONNECTOR_SPI<]" at
 * displayserver/linux/drm.c:169-171), so - same lesson as
 * ../linux/nl80211.h - SPI and USB must carry their real values or
 * the named and literal switch cases collide. */
#ifndef _DRM_DRM_MODE_H
#define _DRM_DRM_MODE_H

#include <stdint.h>

#include "drm.h"

#define DRM_DISPLAY_MODE_LEN 32
#define DRM_PROP_NAME_LEN    32

#define DRM_IOCTL_MODE_GETRESOURCES DRM_IOWR(0xA0, struct drm_mode_card_res)
#define DRM_IOCTL_MODE_GETCRTC      DRM_IOWR(0xA1, struct drm_mode_crtc)
#define DRM_IOCTL_MODE_GETENCODER   DRM_IOWR(0xA6, struct drm_mode_get_encoder)
#define DRM_IOCTL_MODE_GETCONNECTOR DRM_IOWR(0xA7, struct drm_mode_get_connector)
#define DRM_IOCTL_MODE_GETPROPERTY  DRM_IOWR(0xAA, struct drm_mode_get_property)
#define DRM_IOCTL_MODE_GETPROPBLOB  DRM_IOWR(0xAC, struct drm_mode_get_blob)
#define DRM_IOCTL_MODE_GETFB        DRM_IOWR(0xAD, struct drm_mode_fb_cmd)

#define DRM_MODE_TYPE_PREFERRED (1 << 3)

#define DRM_MODE_PROP_PENDING    (1 << 0)
#define DRM_MODE_PROP_RANGE      (1 << 1)
#define DRM_MODE_PROP_IMMUTABLE  (1 << 2)
#define DRM_MODE_PROP_ENUM       (1 << 3)
#define DRM_MODE_PROP_BLOB       (1 << 4)
#define DRM_MODE_PROP_BITMASK    (1 << 5)
#define DRM_MODE_PROP_LEGACY_TYPE \
    (DRM_MODE_PROP_RANGE | DRM_MODE_PROP_ENUM | DRM_MODE_PROP_BLOB | DRM_MODE_PROP_BITMASK)
#define DRM_MODE_PROP_EXTENDED_TYPE 0x0000ffc0

#define DRM_MODE_CONNECTOR_Unknown     0
#define DRM_MODE_CONNECTOR_VGA         1
#define DRM_MODE_CONNECTOR_DVII        2
#define DRM_MODE_CONNECTOR_DVID        3
#define DRM_MODE_CONNECTOR_DVIA        4
#define DRM_MODE_CONNECTOR_Composite   5
#define DRM_MODE_CONNECTOR_SVIDEO      6
#define DRM_MODE_CONNECTOR_LVDS        7
#define DRM_MODE_CONNECTOR_Component   8
#define DRM_MODE_CONNECTOR_9PinDIN     9
#define DRM_MODE_CONNECTOR_DisplayPort 10
#define DRM_MODE_CONNECTOR_HDMIA       11
#define DRM_MODE_CONNECTOR_HDMIB       12
#define DRM_MODE_CONNECTOR_TV          13
#define DRM_MODE_CONNECTOR_eDP         14
#define DRM_MODE_CONNECTOR_VIRTUAL     15
#define DRM_MODE_CONNECTOR_DSI         16
#define DRM_MODE_CONNECTOR_DPI         17
#define DRM_MODE_CONNECTOR_WRITEBACK   18
#define DRM_MODE_CONNECTOR_SPI         19 /* verified: drm.c:169 */
#define DRM_MODE_CONNECTOR_USB         20 /* verified: drm.c:171 */

struct drm_mode_card_res {
    uint64_t fb_id_ptr;
    uint64_t crtc_id_ptr;
    uint64_t connector_id_ptr;
    uint64_t encoder_id_ptr;
    uint32_t count_fbs;
    uint32_t count_crtcs;
    uint32_t count_connectors;
    uint32_t count_encoders;
    uint32_t min_width, max_width;
    uint32_t min_height, max_height;
};

struct drm_mode_modeinfo {
    uint32_t clock;
    uint16_t hdisplay, hsync_start, hsync_end, htotal, hskew;
    uint16_t vdisplay, vsync_start, vsync_end, vtotal, vscan;
    uint32_t vrefresh;
    uint32_t flags;
    uint32_t type;
    char name[DRM_DISPLAY_MODE_LEN];
};

struct drm_mode_get_connector {
    uint64_t encoders_ptr;
    uint64_t modes_ptr;
    uint64_t props_ptr;
    uint64_t prop_values_ptr;
    uint32_t count_modes;
    uint32_t count_props;
    uint32_t count_encoders;
    uint32_t encoder_id;
    uint32_t connector_id;
    uint32_t connector_type;
    uint32_t connector_type_id;
    uint32_t connection;
    uint32_t mm_width, mm_height;
    uint32_t subpixel;
    uint32_t pad;
};

struct drm_mode_crtc {
    uint64_t set_connectors_ptr;
    uint32_t count_connectors;
    uint32_t crtc_id;
    uint32_t fb_id;
    uint32_t x, y;
    uint32_t gamma_size;
    uint32_t mode_valid;
    struct drm_mode_modeinfo mode;
};

struct drm_mode_get_encoder {
    uint32_t encoder_id;
    uint32_t encoder_type;
    uint32_t crtc_id;
    uint32_t possible_crtcs;
    uint32_t possible_clones;
};

struct drm_mode_get_property {
    uint64_t values_ptr;
    uint64_t enum_blob_ptr;
    uint32_t prop_id;
    uint32_t flags;
    char name[DRM_PROP_NAME_LEN];
    uint32_t count_values;
    uint32_t count_enum_blobs;
};

struct drm_mode_get_blob {
    uint32_t blob_id;
    uint32_t length;
    uint64_t data;
};

struct drm_mode_fb_cmd {
    uint32_t fb_id;
    uint32_t width, height;
    uint32_t pitch;
    uint32_t bpp;
    uint32_t depth;
    uint32_t handle;
};

#endif
