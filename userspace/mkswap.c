/*
 * mkswap.c - formats a disk as swap space (see include/swapdisk.h)
 *
 * Writes a swap_header to LBA 0 with the slot count the device can
 * hold; kernel/mm/swap.c's swap_init() looks for this at boot and, if
 * found, claims the whole disk as swap. Mirrors mkfs_wrf.c: userspace
 * code writing through the plain byte-stream device node (/dev/hdX)
 * after boot, not the kernel's own direct ata_read()/ata_write() path.
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "swapdisk.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: mkswap <device>\n");
        return 2;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("mkswap: open");
        return 1;
    }
    long dev_size = lseek(fd, 0, SEEK_END);
    if (dev_size <= 0) {
        fprintf(stderr, "mkswap: %s: cannot determine device size\n", argv[1]);
        close(fd);
        return 1;
    }
    uint32_t total_sectors = (uint32_t)(dev_size / SWAP_SECTOR_SIZE);
    if (total_sectors <= SWAP_SECTORS_PER_SLOT) {
        fprintf(stderr, "mkswap: %s: too small\n", argv[1]);
        close(fd);
        return 1;
    }
    uint32_t slots = (total_sectors - 1) / SWAP_SECTORS_PER_SLOT;

    struct swap_header hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.magic = SWAP_MAGIC;
    hdr.version = SWAP_VERSION;
    hdr.total_slots = slots;

    uint8_t sector[SWAP_SECTOR_SIZE];
    memset(sector, 0, sizeof(sector));
    memcpy(sector, &hdr, sizeof(hdr));

    if (lseek(fd, 0, SEEK_SET) != 0 ||
        write(fd, sector, sizeof(sector)) != (ssize_t)sizeof(sector)) {
        perror("mkswap: write header");
        close(fd);
        return 1;
    }

    close(fd);
    printf("mkswap: %s: %u slots (%u MiB)\n", argv[1], slots,
           slots * 4096u / (1024u * 1024u));
    return 0;
}
