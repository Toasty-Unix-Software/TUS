/* Stub for TUS: real Linux kernel fixed-width type aliases (from
 * <linux/types.h>), which several bundled/vendored DRM headers
 * (src/detection/gpu/asahi_drm.h etc.) assume are already available -
 * on a real system these come transitively from <linux/types.h> via
 * <drm/drm.h> or similar; TUS has none of that, so this just aliases
 * them to the equivalent stdint.h types, which is exactly what the
 * real header does too. */
#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

#include <stdint.h>

typedef uint8_t  __u8;
typedef int8_t   __s8;
typedef uint16_t __u16;
typedef int16_t  __s16;
typedef uint32_t __u32;
typedef int32_t  __s32;
typedef uint64_t __u64;
typedef int64_t  __s64;

#endif
