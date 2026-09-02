/*
 * swap.h - disk-backed swap space
 *
 * A page evicted with swap_out_page() is written to a dedicated swap
 * disk and its PTE rewritten to a not-present, software-defined
 * encoding (marker bit + slot number + the flags needed to restore
 * it). The page fault handler calls swap_fault() on every #PF before
 * deciding whether the fault is real; if the faulting PTE carries the
 * marker, the page is read back from disk and the instruction that
 * faulted simply retries - transparent to whatever touched the
 * address.
 *
 * Slot bookkeeping lives only in RAM: swap content has no reason to
 * survive a reboot (nothing running survives one either), so the free
 * bitmap starts all-free every boot regardless of what a previous
 * boot left on disk.
 */

#ifndef TUS_MM_SWAP_H
#define TUS_MM_SWAP_H

#include <stdbool.h>
#include <stdint.h>

/* Scan attached ATA disks for one carrying SWAP_MAGIC at LBA 0
 * (written by the `mkswap` userspace tool) and claim it. No-op if
 * none is found. Must run after ata_init(). */
void swap_init(void);

bool swap_available(void);

/* Evict the page mapped at `virt` in address space `cr3`: write its
 * content to a free swap slot, free the physical frame, and rewrite
 * the PTE to the not-present swap encoding. `virt` must be page
 * aligned and currently present. Returns the 1-based slot number used,
 * or 0 on failure (no swap disk, no free slot, or nothing mapped
 * there). */
uint32_t swap_out_page(uint64_t cr3, uint64_t virt);

/* Called from the #PF handler before any other handling: if the PTE
 * for `virt` in `cr3` carries the swap marker, read the page back
 * from disk into a fresh frame, restore the PTE and return true (the
 * caller should just return from the fault). Returns false for any
 * other fault (the caller proceeds with its normal handling). */
bool swap_fault(uint64_t cr3, uint64_t virt);

/* Stats for the `swaptest`/debug path: total slots and slots in use. */
void swap_get_stats(uint32_t *total_slots, uint32_t *used_slots);

#endif /* TUS_MM_SWAP_H */
