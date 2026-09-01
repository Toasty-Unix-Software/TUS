/* Stub for TUS: see drm.h in this same directory - the DRM_IOCTL_*
 * command numbers here don't need to match real Linux since the
 * ioctl never reaches a real kernel driver on TUS, only the two
 * structs' field names/order need to be right (gpu_drm.c's
 * ffDrmDetectRadeon accesses .request/.value on drm_radeon_info and
 * .vram_size/.gart_size on drm_radeon_gem_info). RADEON_INFO_* are
 * real, stable Linux uapi values (radeon_drm.h), used because they
 * cost nothing to get right. */
#ifndef _DRM_RADEON_DRM_H
#define _DRM_RADEON_DRM_H

#include <stdint.h>

#include "drm.h"

#define DRM_RADEON_INFO     0x27
#define DRM_RADEON_GEM_INFO 0x1c

#define DRM_IOCTL_RADEON_INFO     DRM_IOWR(DRM_COMMAND_BASE + DRM_RADEON_INFO, struct drm_radeon_info)
#define DRM_IOCTL_RADEON_GEM_INFO DRM_IOWR(DRM_COMMAND_BASE + DRM_RADEON_GEM_INFO, struct drm_radeon_gem_info)

#define RADEON_INFO_ACTIVE_CU_COUNT  0x1a
#define RADEON_INFO_CURRENT_GPU_TEMP 0x2b
#define RADEON_INFO_MAX_SCLK         0x1c
#define RADEON_INFO_VRAM_USAGE       0x1e
#define RADEON_INFO_GTT_USAGE        0x1f

struct drm_radeon_info {
    uint32_t request;
    uint32_t pad;
    uint64_t value;
};

struct drm_radeon_gem_info {
    uint64_t gart_size;
    uint64_t vram_size;
    uint64_t vram_visible;
};

#endif
