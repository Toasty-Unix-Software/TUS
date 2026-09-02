/*
 * swap.c - disk-backed swap space (see swap.h)
 */

#include "swap.h"

#include <stddef.h>
#include <string.h>

#include "../core/klib.h"
#include "../drivers/ata/ata.h"
#include "../../include/swapdisk.h"
#include "kmalloc.h"
#include "pmm.h"
#include "vmm.h"

_Static_assert(SWAP_SECTOR_SIZE == ATA_SECTOR_SIZE,
               "swapdisk.h's sector size must match the ATA driver's");

/* Software encoding of an evicted PTE (hardware ignores every bit
 * below when P=0, so these are ours to define). */
#define SWAP_PTE_MARKER (1ull << 9)
#define SWAP_PTE_FLAGS_MASK (VMM_WRITE | VMM_USER)
#define SWAP_PTE_SLOT_SHIFT 12

static int      g_swap_disk = -1;
static uint32_t g_total_slots = 0;
static uint8_t *g_bitmap = NULL; /* one bit per slot, 1 = in use */

static bool bitmap_test(uint32_t slot) {
    return (g_bitmap[slot / 8] & (1u << (slot % 8))) != 0;
}
static void bitmap_set(uint32_t slot) {
    g_bitmap[slot / 8] |= (uint8_t)(1u << (slot % 8));
}
static void bitmap_clear(uint32_t slot) {
    g_bitmap[slot / 8] &= (uint8_t)~(1u << (slot % 8));
}

void swap_init(void) {
    for (int i = 0; i < ata_disk_count() + 4 && i < 8; i++) {
        const struct ata_disk *d = ata_disk(i);
        if (d == NULL || !d->present || d->atapi) {
            continue;
        }
        struct swap_header hdr;
        uint8_t sector[ATA_SECTOR_SIZE];
        if (ata_read(i, 0, 1, sector) != 0) {
            continue;
        }
        memcpy(&hdr, sector, sizeof(hdr));
        if (hdr.magic != SWAP_MAGIC) {
            continue;
        }
        uint32_t max_slots = (d->sectors - 1) / SWAP_SECTORS_PER_SLOT;
        uint32_t slots = hdr.total_slots;
        if (slots == 0 || slots > max_slots) {
            slots = max_slots;
        }
        if (slots == 0) {
            continue;
        }
        g_bitmap = kmalloc((slots + 7) / 8);
        if (g_bitmap == NULL) {
            continue;
        }
        memset(g_bitmap, 0, (slots + 7) / 8);
        g_swap_disk = i;
        g_total_slots = slots;
        klog("swap         : /dev/%s  %u slots (%u MiB)\n", d->name,
             (unsigned)slots, (unsigned)(slots * 4096 / (1024 * 1024)));
        return;
    }
}

bool swap_available(void) {
    return g_swap_disk >= 0;
}

void swap_get_stats(uint32_t *total_slots, uint32_t *used_slots) {
    if (total_slots) *total_slots = g_total_slots;
    if (used_slots == NULL) {
        return;
    }
    uint32_t used = 0;
    for (uint32_t i = 0; i < g_total_slots; i++) {
        if (bitmap_test(i)) used++;
    }
    *used_slots = used;
}

static int32_t alloc_slot(void) {
    for (uint32_t i = 0; i < g_total_slots; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            return (int32_t)i;
        }
    }
    return -1;
}

uint32_t swap_out_page(uint64_t cr3, uint64_t virt) {
    if (!swap_available()) {
        return 0;
    }
    virt &= ~0xfffull;

    uint64_t *pte = vmm_walk_pte(cr3, virt, false);
    if (pte == NULL || (*pte & VMM_PRESENT) == 0) {
        return 0;
    }
    uint64_t phys = *pte & 0x000ffffffffff000ull;
    uint64_t orig_flags = *pte & SWAP_PTE_FLAGS_MASK;

    int32_t slot = alloc_slot();
    if (slot < 0) {
        return 0;
    }

    const void *page = (const void *)(uintptr_t)pmm_phys_to_virt(phys);
    if (ata_write(g_swap_disk, (uint32_t)slot * SWAP_SECTORS_PER_SLOT + 1,
                  SWAP_SECTORS_PER_SLOT, page) != 0) {
        bitmap_clear((uint32_t)slot);
        return 0;
    }

    *pte = SWAP_PTE_MARKER | orig_flags |
           ((uint64_t)(uint32_t)slot << SWAP_PTE_SLOT_SHIFT);
    __asm__ volatile("invlpg (%0)" : : "r"(virt) : "memory");

    pmm_free_frame(phys);
    return (uint32_t)slot + 1;
}

bool swap_fault(uint64_t cr3, uint64_t virt) {
    if (!swap_available()) {
        return false;
    }
    uint64_t page = virt & ~0xfffull;

    uint64_t *pte = vmm_walk_pte(cr3, page, false);
    if (pte == NULL || (*pte & VMM_PRESENT) != 0 ||
        (*pte & SWAP_PTE_MARKER) == 0) {
        return false;
    }

    uint32_t slot = (uint32_t)(*pte >> SWAP_PTE_SLOT_SHIFT);
    uint64_t orig_flags = *pte & SWAP_PTE_FLAGS_MASK;

    uint64_t phys = pmm_alloc_frame();
    if (phys == 0) {
        return false; /* genuinely out of memory: let the real fault path kill it */
    }
    void *dst = (void *)(uintptr_t)pmm_phys_to_virt(phys);
    if (ata_read(g_swap_disk, slot * SWAP_SECTORS_PER_SLOT + 1,
                 SWAP_SECTORS_PER_SLOT, dst) != 0) {
        pmm_free_frame(phys);
        return false;
    }

    *pte = phys | VMM_PRESENT | orig_flags;
    __asm__ volatile("invlpg (%0)" : : "r"(page) : "memory");

    bitmap_clear(slot);
    return true;
}
