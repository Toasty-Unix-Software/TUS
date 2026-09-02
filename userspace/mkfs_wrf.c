/*
 * mkfs_wrf.c - formats a disk with WRF (see include/wrf.h)
 *
 * Lays out the superblock, an all-free inode bitmap, an all-free
 * block bitmap, a zeroed inode table, and one inode: the empty root
 * directory. Everything else - every other file and directory a WRF
 * volume will ever hold - is created later, by the kernel, the first
 * time something is written under /home (kernel/fs/wrf.c mounts this
 * volume at boot; see its wrf_notify_create()).
 *
 * Writes through the ordinary byte-stream device node (/dev/hdX),
 * exactly like tusinstall.c writes a FAT32 filesystem to the boot
 * disk - this is userspace code running after boot, so unlike
 * kernel/fs/wrf.c (which mounts before the scheduler exists and talks
 * to the ATA driver directly) it has a real fd table to use.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wrf.h"

static int write_all(int fd, const void *buf, size_t len) {
    const unsigned char *p = buf;
    size_t off = 0;
    while (off < len) {
        ssize_t n = write(fd, p + off, len - off);
        if (n <= 0) {
            return -1;
        }
        off += (size_t)n;
    }
    return 0;
}

static int write_zero_sectors(int fd, uint32_t count) {
    uint8_t zero[WRF_BLOCK_SIZE];
    memset(zero, 0, sizeof(zero));
    for (uint32_t i = 0; i < count; i++) {
        if (write_all(fd, zero, sizeof(zero)) != 0) {
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: mkfs.wrf <device>\n");
        return 2;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("mkfs.wrf: open");
        return 1;
    }
    long dev_size = lseek(fd, 0, SEEK_END);
    if (dev_size <= 0) {
        fprintf(stderr, "mkfs.wrf: %s: cannot determine device size\n", argv[1]);
        close(fd);
        return 1;
    }
    uint32_t total_sectors = (uint32_t)(dev_size / WRF_BLOCK_SIZE);

    struct wrf_superblock sb;
    if (wrf_compute_layout(total_sectors, &sb) != 0) {
        fprintf(stderr, "mkfs.wrf: %s: too small (%u sectors)\n", argv[1], total_sectors);
        close(fd);
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) != 0 || write_all(fd, &sb, sizeof(sb)) != 0) {
        perror("mkfs.wrf: write superblock");
        close(fd);
        return 1;
    }
    /* The backup superblock (see the v2 integrity comment in wrf.h):
     * an identical copy at the volume's last sector, so a dead or
     * corrupted LBA 0 doesn't take the whole filesystem down with it. */
    if (lseek(fd, (long)(total_sectors - 1) * WRF_BLOCK_SIZE, SEEK_SET) < 0 ||
        write_all(fd, &sb, sizeof(sb)) != 0) {
        perror("mkfs.wrf: write backup superblock");
        close(fd);
        return 1;
    }
    /* Zero every metadata sector: an all-zero inode bitmap and block
     * bitmap mean "everything free," an all-zero inode table means
     * "every inode's mode is 0" - neither WRF_IFDIR nor WRF_IFREG,
     * which is exactly what makes inode 0 (reserved) and every other
     * not-yet-allocated inode inert - and an all-zero checksum table
     * is harmless too: every entry in it belongs to a block the block
     * bitmap says is free, so nothing ever reads it before
     * wrf_block_alloc() (kernel/fs/wrf.c) overwrites it for real. */
    if (lseek(fd, (long)WRF_BLOCK_SIZE, SEEK_SET) < 0 ||
        write_zero_sectors(fd, sb.inode_bitmap_blocks + sb.inode_table_blocks +
                                    sb.block_bitmap_blocks + sb.checksum_table_blocks) != 0) {
        perror("mkfs.wrf: zero metadata");
        close(fd);
        return 1;
    }

    /* Inode 0 (reserved) and inode 1 (root) both start allocated. */
    uint8_t ibmap0[WRF_BLOCK_SIZE];
    memset(ibmap0, 0, sizeof(ibmap0));
    ibmap0[0] = 0x03;
    if (lseek(fd, (long)sb.inode_bitmap_lba * WRF_BLOCK_SIZE, SEEK_SET) < 0 ||
        write_all(fd, ibmap0, sizeof(ibmap0)) != 0) {
        perror("mkfs.wrf: write inode bitmap");
        close(fd);
        return 1;
    }

    /* The root directory: an empty WRF_IFDIR inode, in the inode
     * table's first sector at slot WRF_ROOT_INO (slot 0 is inode 0,
     * left all-zero on purpose - reserved means never read). */
    struct wrf_inode root;
    memset(&root, 0, sizeof(root));
    root.mode = WRF_IFDIR | 0755;
    root.nlink = 1;
    uint8_t first_inode_sector[WRF_BLOCK_SIZE];
    memset(first_inode_sector, 0, sizeof(first_inode_sector));
    memcpy(first_inode_sector + WRF_ROOT_INO * sizeof(root), &root, sizeof(root));
    if (lseek(fd, (long)sb.inode_table_lba * WRF_BLOCK_SIZE, SEEK_SET) < 0 ||
        write_all(fd, first_inode_sector, sizeof(first_inode_sector)) != 0) {
        perror("mkfs.wrf: write root inode");
        close(fd);
        return 1;
    }

    close(fd);
    printf("mkfs.wrf: %s: %u KiB total, %u inodes, %u KiB data\n", argv[1],
           total_sectors / 2, sb.inode_count, sb.total_blocks / 2);
    return 0;
}
