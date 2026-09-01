/* Stub for TUS: see drm.h in this same directory - command numbers
 * don't need to match real Linux (the ioctl never reaches a real
 * kernel driver on TUS); struct field names/order do, since
 * gpu_drm.c's ffDrmDetectAmdgpu sets/reads them by name. AMDGPU_INFO_*
 * are real, stable Linux uapi values, used because they cost nothing
 * to get right. AMDGPU_FAMILY_GC_11_0_0 is deliberately left
 * undefined so drm_amdgpu_info_device takes the older _pad/_pad1
 * branch in gpu_drm.c instead of pcie_gen/pcie_num_lanes - one struct
 * shape to maintain instead of two, and both compile equally validly
 * since neither ever holds real data on TUS. */
#ifndef _DRM_AMDGPU_DRM_H
#define _DRM_AMDGPU_DRM_H

#include <stdint.h>

#include "drm.h"

#define DRM_AMDGPU_INFO 0x05
#define DRM_IOCTL_AMDGPU_INFO DRM_IOWR(DRM_COMMAND_BASE + DRM_AMDGPU_INFO, struct drm_amdgpu_info)

#define AMDGPU_INFO_DEV_INFO 0x16
#define AMDGPU_INFO_MEMORY   0x19
#define AMDGPU_INFO_SENSOR   0x1c

#define AMDGPU_INFO_SENSOR_GPU_TEMP 0x1
#define AMDGPU_INFO_SENSOR_GPU_LOAD 0x2

#define AMDGPU_IDS_FLAGS_FUSION 0x1

struct drm_amdgpu_info {
    uint64_t return_pointer;
    uint32_t return_size;
    uint32_t query;
    union {
        struct {
            uint32_t type;
            uint32_t ip_instance;
        } query_hw_ip;
        uint32_t query_bytes[2];
    };
};

struct drm_amdgpu_info_device {
    uint32_t device_id;
    uint32_t chip_rev;
    uint32_t external_rev;
    uint32_t pci_rev;
    uint32_t family;
    uint32_t num_shader_engines;
    uint32_t num_shader_arrays_per_engine;
    uint32_t gpu_counter_freq;
    uint64_t max_engine_clock;
    uint64_t max_memory_clock;
    uint32_t cu_active_number;
    uint32_t cu_ao_mask;
    uint32_t cu_bitmap[4][4];
    uint32_t vram_type;
    uint32_t vram_bit_width;
    uint32_t ce_ram_size;
    uint32_t vce_harvest_config;
    uint32_t pci_domain;
    uint32_t pci_bus;
    uint32_t pci_device;
    uint32_t pci_func;
    uint32_t ids_flags;
    uint32_t _pad;
    uint32_t _pad1;
};

struct drm_amdgpu_heap_info {
    uint64_t total_heap_size;
    uint64_t usable_heap_size;
    uint64_t heap_usage;
    uint64_t max_allocation;
};

struct drm_amdgpu_memory_info {
    struct drm_amdgpu_heap_info vram;
    struct drm_amdgpu_heap_info cpu_accessible_vram;
    struct drm_amdgpu_heap_info gtt;
};

#endif
