/*
 * swapdisk.h - on-disk layout for a swap device
 *
 * Shared between the kernel (kernel/mm/swap.c, which reads this at
 * boot) and the `mkswap` userspace tool (which writes it). LBA 0
 * holds the header; slot N's 4 KiB lives at sectors
 * [1 + N*SWAP_SECTORS_PER_SLOT, 1 + (N+1)*SWAP_SECTORS_PER_SLOT).
 */

#ifndef TUS_SWAPDISK_H
#define TUS_SWAPDISK_H

#include <stdint.h>

#define SWAP_MAGIC   0x50415753u /* "SWAP" */
#define SWAP_VERSION 1u
#define SWAP_SECTOR_SIZE 512
#define SWAP_SECTORS_PER_SLOT (4096 / SWAP_SECTOR_SIZE) /* 8 */

struct swap_header {
    uint32_t magic;
    uint32_t version;
    uint32_t total_slots;
    uint32_t reserved;
};

#endif /* TUS_SWAPDISK_H */
