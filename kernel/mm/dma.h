/*
 * dma.h - memory a device can read and write
 *
 * A device master does not walk page tables. It puts a *physical*
 * address on the bus, so every structure a controller reads - a ring,
 * a descriptor table, a transfer buffer - has to be physically
 * contiguous and its physical address has to be known.
 *
 * kmalloc cannot promise either. It hands out pieces of a virtual
 * arena that is mapped frame by frame, so two adjacent kmalloc bytes
 * may be a gigabyte apart in physical memory, and the kernel's own
 * addresses are in the higher half where no 32-bit register can reach.
 * (The EHCI driver's `virt_to_phys` casts a kernel pointer to
 * uint32_t, which is how one can tell it has never moved a byte to a
 * real device.)
 *
 * This allocator hands out whole physical frames from the PMM and
 * gives back both views of them: the physical address for the device,
 * and the HHDM address for the kernel. Allocations are page granular
 * and page aligned, which is what controller data structures want
 * anyway - xHCI rings must not cross a 64 KiB boundary, and a
 * page-aligned allocation of at most one page never does.
 */

#ifndef TUS_MM_DMA_H
#define TUS_MM_DMA_H

#include <stddef.h>
#include <stdint.h>

struct dma_buf {
    void    *virt;  /* kernel view (HHDM); NULL if the allocation failed */
    uint64_t phys;  /* what the device is told */
    size_t   size;  /* bytes actually reserved (a multiple of 4096) */
};

/* Reserve `bytes` of physically contiguous, page-aligned memory and
 * zero it. Returns a buffer whose .virt is NULL on failure. */
struct dma_buf dma_alloc(size_t bytes);

/* Give it back. Safe on a failed allocation. */
void dma_free(struct dma_buf *buf);

/* Physical address of a pointer inside a dma_buf. The offset is
 * checked: a pointer that is not inside the buffer returns 0, because
 * handing a device a wrong physical address is the kind of bug that
 * corrupts memory somewhere else entirely. */
uint64_t dma_phys_of(const struct dma_buf *buf, const void *ptr);

#endif /* TUS_MM_DMA_H */
