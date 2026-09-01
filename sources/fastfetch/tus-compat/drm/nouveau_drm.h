/* Stub for TUS: see drm.h in this same directory - command number
 * doesn't need to match real Linux (the ioctl never reaches a real
 * kernel driver on TUS); struct fields do, since gpu_drm.c's
 * ffDrmDetectNouveau sets/reads .param/.value by name. Defining
 * DRM_IOCTL_NOUVEAU_GETPARAM here (real headers do) means gpu_drm.c's
 * own DRM_IOWR(DRM_COMMAND_BASE + DRM_NOUVEAU_GETPARAM, ...) fallback
 * for older headers never triggers. NOUVEAU_GETPARAM_* are real,
 * stable Linux uapi values, used because they cost nothing to get
 * right. */
#ifndef _DRM_NOUVEAU_DRM_H
#define _DRM_NOUVEAU_DRM_H

#include <stdint.h>

#include "drm.h"

#define DRM_NOUVEAU_GETPARAM 0x00
#define DRM_IOCTL_NOUVEAU_GETPARAM DRM_IOWR(DRM_COMMAND_BASE + DRM_NOUVEAU_GETPARAM, struct drm_nouveau_getparam)

#define NOUVEAU_GETPARAM_FB_SIZE     8
#define NOUVEAU_GETPARAM_AGP_SIZE    9
#define NOUVEAU_GETPARAM_GRAPH_UNITS 13

struct drm_nouveau_getparam {
    uint64_t param;
    uint64_t value;
};

#endif
