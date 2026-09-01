/*
 * kmalloc.c - kernel heap allocator implementation
 *
 * Block layout (all offsets relative to the block start):
 *
 *     +--------+-------------------+
 *     | header | user data         |
 *     +--------+-------------------+
 *
 * header = { size (payload, multiple of 16), next (free list link) }
 *
 * Allocation: first-fit over the free list; if a block is larger than
 * needed it is split into two. Freeing: the block is reinserted into
 * the list and merged with adjacent free blocks to fight fragmentation.
 *
 * When the free list is empty, the arena is grown by one or more
 * pages, which are mapped from fresh PMM frames.
 */

#include "kmalloc.h"

#include <stdbool.h>

#include "pmm.h"
#include "vmm.h"
#include "../core/klib.h"
#include "../sched/sched.h"

#define ALIGN 16
#define PAGE_SIZE 4096

struct block {
    size_t size;            /* payload size, multiple of ALIGN */
    size_t pages;           /* >0: multi-page block (arena-allocated) */
    struct block *next;     /* free list link (only when free) */
};

#define HEADER_SIZE (sizeof(struct block))

static struct block *g_free;         /* free list head */
static uint64_t g_arena_next;        /* next virtual address to map */
static size_t g_arena_bytes;         /* mapped bytes */

/*
 * Large (multi-page) blocks never go on g_free - kfree_locked() unmaps
 * their pages and returns the frames to the PMM immediately, which
 * means nothing is left at that address to hold a free-list link once
 * it happens. Without tracking the freed VIRTUAL ADDRESS RANGE
 * somewhere else, g_arena_next above only ever grows: every large
 * kmalloc()/kfree() pair permanently abandons its VA range instead of
 * reusing it. A window's pixel buffer (width*height*4) is a large
 * block for any real window size, so repeatedly resizing one - which
 * is exactly what a fast drag against a live tiling layout does -
 * burns through VA space on every single resize. Given enough of
 * them, g_arena_next walks straight past VMM_KHEAP_SIZE (there was no
 * check stopping it) and into whatever VA range comes next -
 * VMM_FB_BASE, in this kernel's layout - where kmalloc's own
 * vmm_map_page() calls silently steal PTEs the framebuffer driver had
 * already mapped to real VRAM. The result isn't a fault inside
 * kmalloc() itself; it surfaces later, as a not-present page fault
 * inside the compositor's screen blit, once enough of the
 * framebuffer's mapping has been quietly overwritten or left with
 * holes.
 *
 * The fix mirrors the small-block free list's shape (first-fit,
 * coalesce with a neighbour) but the entries live in a small bounded
 * table instead of inside the freed memory - the freed pages are
 * already unmapped by the time kfree_locked() would want to link them
 * in. A full table just falls back to the old grow-only behaviour
 * (still bounded now by the VMM_KHEAP_SIZE check below), not a
 * correctness problem, only a missed reuse opportunity.
 */
#define LARGE_FREE_MAX 256
struct large_free {
    uint64_t va;
    size_t pages;
};
static struct large_free g_large_free[LARGE_FREE_MAX];
static int g_large_free_n;

/* True if [va, va + pages*PAGE_SIZE) would still fit inside the
 * arena's reserved VA range - the hard backstop that turns "silently
 * corrupt whatever VA comes next" into a clean, already-handled
 * ENOMEM (every kmalloc() caller in this kernel checks for NULL). */
static bool arena_fits(uint64_t va, size_t pages) {
    return (va - VMM_KHEAP_BASE) + pages * PAGE_SIZE <= VMM_KHEAP_SIZE;
}

/* Record a freed large block's VA range for reuse, coalescing with an
 * adjacent recorded range on either side so the table doesn't fill up
 * with what is really one bigger hole split into pieces. */
static void large_free_add(uint64_t va, size_t pages) {
    for (int i = 0; i < g_large_free_n; i++) {
        struct large_free *e = &g_large_free[i];
        if (e->va + e->pages * PAGE_SIZE == va) {
            e->pages += pages;
            return;
        }
        if (va + pages * PAGE_SIZE == e->va) {
            e->va = va;
            e->pages += pages;
            return;
        }
    }
    if (g_large_free_n < LARGE_FREE_MAX) {
        g_large_free[g_large_free_n].va = va;
        g_large_free[g_large_free_n].pages = pages;
        g_large_free_n++;
    }
    /* Table full: the VA range is abandoned, same as before this fix
     * existed - rare (256 concurrently-fragmented holes), and still
     * bounded by arena_fits() rather than able to corrupt anything. */
}

/* First-fit a recorded hole of at least `pages`; NULL if none.
 * Splits from the front of a larger hole and keeps the remainder. */
static uint64_t large_free_take(size_t pages) {
    for (int i = 0; i < g_large_free_n; i++) {
        struct large_free *e = &g_large_free[i];
        if (e->pages < pages) {
            continue;
        }
        uint64_t va = e->va;
        if (e->pages == pages) {
            g_large_free[i] = g_large_free[--g_large_free_n];
        } else {
            e->va += pages * PAGE_SIZE;
            e->pages -= pages;
        }
        return va;
    }
    return 0;
}

static size_t align_up(size_t n) {
    return (n + ALIGN - 1) & ~(size_t)(ALIGN - 1);
}

void kmalloc_init(void) {
    g_free = NULL;
    g_arena_next = VMM_KHEAP_BASE;
    g_arena_bytes = 0;

    /* Pre-create the intermediate page tables for the whole heap
     * region in the root space. This runs before any user task can
     * exist; afterwards, growing the heap only writes shared leaf
     * PTEs, which every per-task address space sees. (Without this,
     * a table allocation triggered while a task's space is active
     * could end up private to that space and the heap would break.) */
    vmm_reserve_tables(VMM_KHEAP_BASE, VMM_KHEAP_SIZE);
}

/* Map one more 4 KiB frame and add it to the free list. */
static void heap_grow_page(void) {
    if (!arena_fits(g_arena_next, 1)) {
        return; /* arena exhausted - fail like "out of physical memory" */
    }
    uint64_t phys = pmm_alloc_frame();
    if (phys == 0) {
        return; /* out of physical memory */
    }
    vmm_map_page(g_arena_next, phys, VMM_WRITE);

    struct block *b = (struct block *)g_arena_next;
    b->size = PAGE_SIZE - HEADER_SIZE;
    /* `pages` distinguishes a multi-page block from an ordinary one
     * in kfree(). A fresh frame carries whatever the last user of
     * that memory left behind, so it has to be cleared here and
     * everywhere else a header is created - a stale non-zero value
     * makes kfree() unmap pages that belong to somebody else. */
    b->pages = 0;
    b->next = g_free;
    g_free = b;

    g_arena_next += PAGE_SIZE;
    g_arena_bytes += PAGE_SIZE;
}

static void heap_grow(size_t payload) {
    while (payload > 0) {
        heap_grow_page();
        payload -= payload > PAGE_SIZE ? PAGE_SIZE : payload;
    }
}

/* The actual allocator, called only with preemption disabled (see
 * kmalloc() below) - see the long comment there for why. */
static void *kmalloc_locked(size_t size) {
    if (size == 0) {
        size = ALIGN;
    }
    size = align_up(size);
    if (size > PAGE_SIZE - HEADER_SIZE) {
        /* Large allocation: whole arena pages, never on the small free
         * list (see the g_large_free comment above for why not, and
         * for the reuse this checks first). The header records the
         * page count so kfree() can release the frames again. */
        size_t pages = (size + HEADER_SIZE + PAGE_SIZE - 1) / PAGE_SIZE;

        uint64_t reused = large_free_take(pages);
        if (reused != 0) {
            struct block *b = (struct block *)reused;
            for (size_t i = 0; i < pages; i++) {
                uint64_t phys = pmm_alloc_frame();
                if (phys == 0) {
                    /* Put the VA range back; nothing else was touched. */
                    large_free_add(reused, pages);
                    return NULL;
                }
                vmm_map_page(reused + i * PAGE_SIZE, phys, VMM_WRITE);
            }
            b->size = size;
            b->pages = pages;
            b->next = NULL;
            return (void *)((uint8_t *)b + HEADER_SIZE);
        }

        if (!arena_fits(g_arena_next, pages)) {
            return NULL; /* arena exhausted, not a silent VA overrun */
        }
        struct block *b = (struct block *)g_arena_next;
        for (size_t i = 0; i < pages; i++) {
            uint64_t phys = pmm_alloc_frame();
            if (phys == 0) {
                return NULL;
            }
            vmm_map_page(g_arena_next + i * PAGE_SIZE, phys, VMM_WRITE);
        }
        b->size = size;
        b->pages = pages;
        b->next = NULL;
        g_arena_next += pages * PAGE_SIZE;
        g_arena_bytes += pages * PAGE_SIZE;
        return (void *)((uint8_t *)b + HEADER_SIZE);
    }

    /* First-fit with splitting. */
    struct block **link = &g_free;
    while (*link != NULL) {
        struct block *b = *link;
        if (b->size >= size) {
            if (b->size >= size + HEADER_SIZE + ALIGN) {
                /* Split: the remainder becomes a new free block. */
                struct block *rest = (struct block *)((uint8_t *)b + HEADER_SIZE + size);
                rest->size = b->size - size - HEADER_SIZE;
                rest->pages = 0;
                rest->next = b->next;
                *link = rest;
                b->size = size;
                b->pages = 0;
            } else {
                *link = b->next;
                b->pages = 0;
            }
            return (void *)((uint8_t *)b + HEADER_SIZE);
        }
        link = &b->next;
    }

    /* Out of free blocks: grow the arena and retry once. */
    size_t before = g_arena_bytes;
    heap_grow(size);
    if (g_arena_bytes == before) {
        return NULL; /* could not grow (out of physical memory) */
    }
    return kmalloc_locked(size);
}

/*
 * g_free/g_arena_next/g_arena_bytes are read-modified-written across
 * several steps each (walk the free list, maybe grow the arena, map a
 * page, THEN advance g_arena_next) with no lock of their own. TUS is
 * single-core, so the only thing that can interleave two callers is
 * the 100 Hz preemptive tick landing inside one of those steps and
 * switching to a task whose kernel path also calls kmalloc()/kfree() -
 * which then sees the shared state half updated. The result is not a
 * clean crash: a `struct block` header gets written at a virtual
 * address the writer THINKS it just mapped but that the interleaved
 * caller's own bookkeeping walked past differently, so the page
 * genuinely is not present and the kernel takes a page fault deep
 * inside kmalloc(), CR2 pointing at kernel-heap memory. This was rare
 * enough with TUS's earlier userspace to go unnoticed - LVGL's
 * heavier allocation traffic during a highX client's first frame
 * makes the window far more likely to be hit, not a new bug LVGL
 * introduced. Every other kmalloc.c-adjacent piece of shared kernel
 * state (klib.c's kprintf, sched.c's own bookkeeping, tus_elf.c's
 * per-space page tables, term.c, highx.c) already disables preemption
 * around its own critical section for exactly this reason; this file
 * simply never had.
 */
void *kmalloc(size_t size) {
    preempt_disable();
    void *ret = kmalloc_locked(size);
    preempt_enable();
    return ret;
}

static void kfree_locked(void *ptr) {
    struct block *b = (struct block *)((uint8_t *)ptr - HEADER_SIZE);

    /* Multi-page block: release its frames back to the PMM. The page
     * count is read ONCE before the loop - the first iteration unmaps
     * the page that holds the header, so re-reading b->pages from
     * memory afterwards would fault. */
    if (b->pages > 0) {
        size_t pages = b->pages;
        uint64_t virt = (uint64_t)(uintptr_t)b;
        for (size_t i = 0; i < pages; i++) {
            uint64_t phys = vmm_translate(virt + i * PAGE_SIZE);
            if (phys != 0) {
                vmm_unmap_page(virt + i * PAGE_SIZE);
                pmm_free_frame(phys);
            }
        }
        /* Physical frames are gone, but the VA range is still good -
         * record it so the next large allocation can reuse it instead
         * of g_arena_next marching further into whatever comes after
         * the heap (see the g_large_free comment above). */
        large_free_add(virt, pages);
        return;
    }

    /* Insert sorted by address, merging with both neighbours. The
     * predecessor is remembered during the walk: looking it up again
     * afterwards used to compare the freshly inserted block with
     * itself, so the merge never happened and the heap fragmented one
     * block at a time. */
    struct block *prev = NULL;
    struct block **link = &g_free;
    while (*link != NULL && *link < b) {
        prev = *link;
        link = &(*link)->next;
    }
    b->pages = 0;
    b->next = *link;
    *link = b;

    /* Merge with the successor (if adjacent). */
    if (b->next != NULL &&
        (uint8_t *)b + HEADER_SIZE + b->size == (uint8_t *)b->next) {
        b->size += HEADER_SIZE + b->next->size;
        b->next = b->next->next;
    }
    /* Merge with the predecessor (if adjacent). */
    if (prev != NULL &&
        (uint8_t *)prev + HEADER_SIZE + prev->size == (uint8_t *)b) {
        prev->size += HEADER_SIZE + b->size;
        prev->next = b->next;
    }
}

void kfree(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    preempt_disable();
    kfree_locked(ptr);
    preempt_enable();
}

void *krealloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return kmalloc(size);
    }
    size = align_up(size);

    /* One critical section for the whole operation, not "realloc calls
     * the already-locked kmalloc/kfree back to back": b->size is read
     * before kmalloc_locked() can possibly grow the arena and move
     * things around it, and the old block must not be freed by anyone
     * else between the copy and kfree_locked() below. */
    preempt_disable();
    struct block *b = (struct block *)((uint8_t *)ptr - HEADER_SIZE);
    if (size <= b->size) {
        preempt_enable();
        return ptr; /* fits already */
    }
    void *fresh = kmalloc_locked(size);
    if (fresh == NULL) {
        preempt_enable();
        return NULL;
    }
    memcpy(fresh, ptr, b->size);
    kfree_locked(ptr);
    preempt_enable();
    return fresh;
}

size_t kmalloc_arena_bytes(void) {
    return g_arena_bytes;
}
