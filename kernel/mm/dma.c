/*
 * dma.c - memory a device can read and write
 *
 * See dma.h. The implementation is deliberately thin: whole frames
 * from the PMM, addressed through the HHDM. There is no sub-page
 * suballocator because there is no caller that wants one - every
 * structure a host controller reads is at least a page's worth of
 * ring or table, and rounding up costs a few frames on a machine that
 * has hundreds of thousands.
 */

#include "dma.h"

#include "pmm.h"
#include "../core/klib.h"

#define DMA_PAGE 4096u

struct dma_buf dma_alloc(size_t bytes) {
    struct dma_buf buf = { NULL, 0, 0 };
    if (bytes == 0) {
        return buf;
    }

    size_t pages = (bytes + DMA_PAGE - 1) / DMA_PAGE;
    uint64_t phys = pmm_alloc_frames(pages);
    if (phys == 0) {
        return buf;
    }

    buf.phys = phys;
    buf.size = pages * DMA_PAGE;
    buf.virt = (void *)(uintptr_t)pmm_phys_to_virt(phys);

    /* A controller reads these structures the moment it is told about
     * them; a reserved field left holding whatever the last owner of
     * the frame wrote is a fault waiting to be blamed on the driver. */
    memset(buf.virt, 0, buf.size);
    return buf;
}

void dma_free(struct dma_buf *buf) {
    if (buf == NULL || buf->virt == NULL) {
        return;
    }
    pmm_free_frames(buf->phys, buf->size / DMA_PAGE);
    buf->virt = NULL;
    buf->phys = 0;
    buf->size = 0;
}

uint64_t dma_phys_of(const struct dma_buf *buf, const void *ptr) {
    if (buf == NULL || buf->virt == NULL || ptr == NULL) {
        return 0;
    }
    const uint8_t *base = (const uint8_t *)buf->virt;
    const uint8_t *p = (const uint8_t *)ptr;
    if (p < base || p >= base + buf->size) {
        return 0;
    }
    return buf->phys + (uint64_t)(p - base);
}
