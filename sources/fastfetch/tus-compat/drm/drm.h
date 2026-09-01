/* Stub for TUS: TUS has no DRM subsystem at all (VBE framebuffer
 * only, no /dev/dri/*), so gpu_drm.c's open(renderPath, ...) always
 * fails with ENOENT before any ioctl here is ever issued - the real
 * kernel never gets a chance to interpret these command numbers. That
 * makes this the same situation as nl80211.h in ../linux/: the ioctl
 * macro *mechanism* (DRM_IOWR/DRM_IOW/DRM_IO, matching real DRM's
 * _IOC encoding via <sys/ioctl.h>, which musl provides correctly) is
 * worth getting right since other code depends on it, but the
 * per-command numbers folded into DRM_COMMAND_BASE+nr just need to be
 * distinct, not byte-identical to a real kernel's. */
#ifndef _DRM_DRM_H
#define _DRM_DRM_H

#include <linux/types.h> /* __u8/__u32/__u64 etc, needed transitively by
                          * bundled headers like gpu/asahi_drm.h, same
                          * as on a real system where drm/drm.h pulls
                          * this in too. */
#include <sys/ioctl.h>

#define DRM_IOCTL_BASE 'd'
#define DRM_IO(nr)          _IO(DRM_IOCTL_BASE, nr)
#define DRM_IOR(nr, type)   _IOR(DRM_IOCTL_BASE, nr, type)
#define DRM_IOW(nr, type)   _IOW(DRM_IOCTL_BASE, nr, type)
#define DRM_IOWR(nr, type)  _IOWR(DRM_IOCTL_BASE, nr, type)

#define DRM_COMMAND_BASE 0x40

#endif
