/*
 * vmm.h - Virtual Memory Manager (x86-64, 4-level paging)
 *
 * The kernel boots on the page tables that Limine set up (kernel image
 * mapped in the higher half, HHDM covering all physical memory). This
 * module owns those tables: it can walk them and add new mappings on
 * demand. The kernel heap lives in a dedicated higher-half region that
 * is mapped frame by frame as the allocator grows.
 */

#ifndef TUS_MM_VMM_H
#define TUS_MM_VMM_H

#include <stddef.h>
#include <stdint.h>

/* Page table entry flags. */
#define VMM_PRESENT (1ull << 0)
#define VMM_WRITE   (1ull << 1)
#define VMM_USER    (1ull << 2)
/* Page-level cache disable. Device registers are not memory: a cached
 * read of a status register returns whatever the line held last, and
 * a write may sit in the cache while the driver waits for the device
 * to react to it. Every MMIO mapping needs this. */
#define VMM_NOCACHE (1ull << 4)

/* Virtual address of the kernel heap arena (reserved address space). */
#define VMM_KHEAP_BASE 0xffffffff81000000ull
#define VMM_KHEAP_SIZE (64ull * 1024 * 1024)

/* Virtual window onto the display adapter's linear framebuffer.
 *
 * Limine maps the framebuffer for the mode the machine booted in and
 * not one byte more, so a runtime mode change to a larger resolution
 * would paint past the end of that mapping. The framebuffer therefore
 * gets a reservation of its own, sized for the largest mode the VBE
 * driver will program (1920x1200x4 is 9 MiB; 32 MiB leaves room for
 * a wider pitch and for a second buffer later).
 *
 * The address is deliberately inside the same 1 GiB PDPT slot as the
 * kernel image: that page directory already exists in every address
 * space and is shared by reference, so leaf mappings added here after
 * tasks have been created are visible to all of them. */
#define VMM_FB_BASE 0xffffffff88000000ull
#define VMM_FB_SIZE (32ull * 1024 * 1024)

/* Window for device register blocks (PCI memory BARs).
 *
 * The HHDM covers physical *memory*; a BAR usually sits above it, and
 * on a machine with a lot of RAM it can sit above 4 GiB entirely.
 * Rather than guess whether a given BAR happens to be inside the
 * HHDM, drivers map theirs explicitly with vmm_map_mmio(), which
 * hands out pages from this reservation. Same PDPT slot as the kernel
 * image, for the same reason the framebuffer window is. */
#define VMM_MMIO_BASE 0xffffffff8c000000ull
#define VMM_MMIO_SIZE (16ull * 1024 * 1024)

/* Initialize: record the active page tables (CR3). */
void vmm_init(void);

/* Physical address of the boot (root) address space - the one the
 * kernel shell runs in. All per-task spaces clone its kernel half. */
uint64_t vmm_root_cr3(void);

/* Allocate a fresh address space: a new PML4 whose kernel half
 * (indices 256..511) is copied from the root space, so every task
 * sees the same kernel mappings. The user half starts empty.
 * Returns the new CR3, or 0 on failure. */
uint64_t vmm_space_clone(void);

/* fork()'s address-space half: a vmm_space_clone(), with every
 * present page in `src_cr3`'s user half (indices 0..255) - stack,
 * heap, loaded ELF segments - eagerly copied into the new space at
 * the same virtual addresses. TUS's mmap is anonymous-only, so there
 * is no shared-mapping case to preserve; a real copy-on-write scheme
 * would defer this to the first write instead, but that needs
 * page-fault-time allocation this kernel does not have yet, and fork
 * is not a hot path here. Returns the new CR3, or 0 on failure. */
uint64_t vmm_space_fork(uint64_t src_cr3);

/* Physical address of the address space that is active right now.
 * elf_exec() uses it to put the caller's space back after loading a
 * program into a different one - the caller is not always the kernel
 * shell (SYS_SPAWN runs in a ring-3 task's own space). */
uint64_t vmm_current_cr3(void);

/* Make `cr3` the active address space (g_cr3 + CR3 reload, which
 * also flushes the TLB). The caller must be in a context where the
 * current kernel stack is mapped in the new space (always true:
 * kernel stacks live in the shared kernel heap). */
void vmm_space_switch(uint64_t cr3);

/* Map a single 4 KiB page into an explicit address space.
 * `virt` must be page aligned. Returns 0 on success, -1 on failure. */
int vmm_map_page_in(uint64_t cr3, uint64_t virt, uint64_t phys,
                    uint64_t flags);

/* Ensure the intermediate page tables for [virt, virt+bytes) exist
 * in the root space (no frames mapped). Call before any task can
 * exist, so later runtime mappings only touch shared leaf tables
 * and every address space always sees them. */
void vmm_reserve_tables(uint64_t virt, size_t bytes);

/* Map a single 4 KiB page in the CURRENT address space. Kernel-half
 * addresses (>= 0xffff800000000000) are mapped in the root space
 * instead: the kernel half is shared by reference, and mapping there
 * keeps every task's view consistent. Returns 0 or -1. */
int vmm_map_page(uint64_t virt, uint64_t phys, uint64_t flags);

/* Map `bytes` (page aligned) of physical memory at `virt`. */
int vmm_map_region(uint64_t virt, uint64_t phys, size_t bytes, uint64_t flags);

/* Map a device's register block and return a pointer to it.
 *
 * `phys` need not be page aligned: the mapping is made from the page
 * that contains it and the returned pointer carries the offset back,
 * because a BAR is allowed to start mid-page and a driver should not
 * have to care. Mappings are uncached (VMM_NOCACHE) and are never
 * released - a driver that has its registers keeps them for the life
 * of the machine. Returns NULL if the reservation is exhausted. */
void *vmm_map_mmio(uint64_t phys, size_t bytes);

/* Unmap a single page (PTE cleared, TLB flushed). */
void vmm_unmap_page(uint64_t virt);

/* Unmap a single page in an explicit address space (PTE cleared,
 * TLB flushed). The target space does not need to be loaded. */
void vmm_unmap_page_in(uint64_t cr3, uint64_t virt);

/* Translate a virtual address in the current space; 0 if unmapped. */
uint64_t vmm_translate(uint64_t virt);

/* Translate a virtual address in an explicit space; 0 if unmapped. */
uint64_t vmm_translate_in(uint64_t cr3, uint64_t virt);

/* Return the raw page-table entry for a virtual address (0 if any
 * level is unmapped). Debug helper. */
uint64_t vmm_pte(uint64_t virt);

/* Return bitmask of NX bits set in the upper levels (PML4/PDPT/PD)
 * for a virtual address. Debug helper: 0 means executable. */
uint64_t vmm_level_nx(uint64_t virt);

/* Return the raw page-table entry at `level` (1=PD, 2=PDPT, 3=PML4).
 * Debug helper to inspect Limine's large-page mappings. */
uint64_t vmm_level_entry(uint64_t virt, int level);

#endif /* TUS_MM_VMM_H */
