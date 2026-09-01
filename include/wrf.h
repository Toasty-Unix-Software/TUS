/*
 * wrf.h - WRF (Writable-Readable Filesystem): on-disk format
 *
 * TUS's own persistent filesystem, from scratch. Everything else in
 * TUS keeps its root filesystem in RAM (rootfs.img, a read-only
 * ustar tar parsed at boot - see kernel/vfs/rootfs.c); WRF is the
 * first thing a write to actually reaches a disk and survives a
 * reboot. It is mounted at /mnt (kernel/fs/wrf.c, wrf_boot_mount())
 * on the first ATA disk found, if that disk carries a valid WRF
 * superblock - format one with `mkfs.wrf <device>` first.
 *
 * This header is included verbatim by the kernel (kernel/fs/wrf.c,
 * which mounts the filesystem and backs real VFS operations with it)
 * and by userspace (userspace/mkfs_wrf.c, the formatter) - the same
 * pattern include/highx.h uses so the two sides of a protocol cannot
 * drift apart; here the "protocol" is the disk layout.
 *
 * Layout, in LBA (512-byte sector) order from the start of the WRF
 * volume:
 *
 *   [0]                          superblock (struct wrf_superblock)
 *   [inode_bitmap_lba  ..]       inode bitmap, 1 bit per inode
 *   [block_bitmap_lba  ..]       data block bitmap, 1 bit per block
 *   [inode_table_lba   ..]       inode table, 4 inodes per sector
 *   [data_start_lba    ..]       data blocks (files, directory data)
 *
 * A block IS a sector (512 bytes) - there is no separate block size,
 * which is what lets every block reference double as a plain LBA
 * offset from data_start_lba with no shift/multiply to get wrong.
 *
 * Inode 0 is reserved (never allocated - "no inode", the same role
 * NULL plays for a pointer); inode 1 is the root directory, always.
 *
 * A directory's contents are exactly a file's: a byte stream (read
 * and written through the same block-mapped I/O every regular file
 * uses - see wrf_raw_io() in wrf.c), holding a flat array of struct
 * wrf_dirent. A removed entry is zeroed in place (inode 0) rather
 * than compacted, and a create reuses the first zeroed slot it finds
 * before growing the directory - the same free-list-by-tombstone idea
 * ustar-style archives don't need but a real filesystem does.
 *
 * File data is addressed the classic Unix way: 12 direct block
 * pointers, then one indirect block (128 more pointers), then one
 * doubly-indirect block (128 pointers to indirect blocks, 128 more
 * pointers each) - 12 + 128 + 128*128 = 16,524 blocks, a hair over
 * 8 MiB, the largest a single WRF file can be in v1.
 */

#ifndef TUS_WRF_H
#define TUS_WRF_H

#include <stddef.h>
#include <stdint.h>

#define WRF_MAGIC       0x31465257u /* "WRF1", little-endian on disk */
#define WRF_VERSION     1
#define WRF_BLOCK_SIZE  512
#define WRF_ROOT_INO    1
#define WRF_DIRECT      12
#define WRF_NAME_MAX    60 /* matches VFS_NAME_MAX (64) minus this header */

/* MBR partition type byte for a WRF volume that shares a disk with
 * other partitions (tusinstall writes one next to the boot ESP; see
 * its own comment for why). 0xDA is the standard MBR registry's
 * generic "non-FS data" type - unclaimed by any real filesystem
 * driver, which is exactly the property a homemade format needs from
 * a byte no other OS's installer or fdisk will treat specially.
 * kernel/fs/wrf.c looks for this byte at boot; a WRF volume that owns
 * a whole disk with no partition table at all (the original,
 * still-supported `mkfs.wrf /dev/hdX` workflow) needs no such marker
 * - its superblock simply starts at LBA 0. */
#define WRF_PART_TYPE   0xDAu

/* Type bits packed into wrf_inode.mode, the same idea as S_IFDIR/
 * S_IFREG - kept out of the low 12 bits so permission bits (0644,
 * 0755, ...) never collide with them. */
#define WRF_IFDIR 0040000u
#define WRF_IFREG 0100000u
#define WRF_IFMT  0170000u

/* wrf_dirent.type - stored redundantly with the inode's own mode bits
 * so a directory listing never has to read every child's inode just
 * to tell a file from a subdirectory. */
#define WRF_DT_REG 1
#define WRF_DT_DIR 2

struct wrf_superblock {
    uint32_t magic;
    uint32_t version;
    uint32_t total_blocks;        /* data blocks covered by the block bitmap */
    uint32_t inode_count;         /* inodes covered by the inode bitmap/table */
    uint32_t inode_bitmap_lba;
    uint32_t inode_bitmap_blocks;
    uint32_t block_bitmap_lba;
    uint32_t block_bitmap_blocks;
    uint32_t inode_table_lba;
    uint32_t inode_table_blocks;
    uint32_t data_start_lba;
    uint32_t root_ino;
    uint8_t  reserved[WRF_BLOCK_SIZE - 12 * (int)sizeof(uint32_t)];
} __attribute__((packed));

struct wrf_inode {
    uint32_t mode;   /* WRF_IFDIR/WRF_IFREG | rwxrwxrwx-style bits */
    uint32_t uid;
    uint32_t gid;
    uint64_t size;   /* bytes */
    uint32_t atime;
    uint32_t mtime;
    uint32_t ctime;
    uint32_t nlink;  /* always 1 in v1 - no hard links */
    uint32_t direct[WRF_DIRECT];
    uint32_t indirect;
    uint32_t dindirect;
    uint8_t  reserved[128 - (3 * (int)sizeof(uint32_t) + (int)sizeof(uint64_t) +
                              4 * (int)sizeof(uint32_t) +
                              WRF_DIRECT * (int)sizeof(uint32_t) +
                              2 * (int)sizeof(uint32_t))];
} __attribute__((packed));

struct wrf_dirent {
    uint32_t inode;  /* 0 = free/tombstoned slot */
    uint8_t  type;   /* WRF_DT_REG / WRF_DT_DIR */
    uint8_t  reserved[3];
    char     name[64];
} __attribute__((packed));

/* Computes the layout for a WRF volume of `total_sectors` 512-byte
 * sectors and fills every field of `out`. The LBA fields are relative
 * to the START of the volume (sector 0 there, not sector 0 of
 * whatever disk it lives on) - kernel/fs/wrf.c's wrf_boot_mount()
 * adds the volume's actual starting LBA on the physical disk exactly
 * once, right after reading the superblock, rather than baking a
 * disk-absolute address into what gets stored on disk. That is what
 * lets the same superblock content be correct whether the volume is
 * a whole dedicated disk (starts at LBA 0) or one partition of a
 * larger one written by tusinstall (starts wherever the MBR says).
 *
 * Shared verbatim by every writer of a WRF superblock -
 * userspace/mkfs_wrf.c and userspace/tusinstall.c - so the layout
 * math exists in exactly one place; two independent reimplementations
 * of the same bitmap/inode-table sizing arithmetic are exactly the
 * kind of thing that quietly drifts apart one rounding tweak at a
 * time. Returns 0 with `out` filled in, or -1 if `total_sectors` is
 * too small to hold even the smallest layout (`out` is left
 * untouched in that case). */
static inline int wrf_compute_layout(uint32_t total_sectors, struct wrf_superblock *out) {
    if (total_sectors < 64) {
        return -1;
    }

    /* One inode per 4 KiB of volume, clamped to a sane range - plenty
     * for a package manager's files and a few dozen directories
     * without spending the whole volume on an oversized inode table. */
    uint32_t inode_count = total_sectors / 8;
    if (inode_count < 16) {
        inode_count = 16;
    }
    if (inode_count > 65536) {
        inode_count = 65536;
    }

    uint32_t bits_per_sector = WRF_BLOCK_SIZE * 8;
    uint32_t inodes_per_sector = WRF_BLOCK_SIZE / (uint32_t)sizeof(struct wrf_inode);

    uint32_t inode_bitmap_lba = 1; /* right after the superblock */
    uint32_t inode_bitmap_blocks = (inode_count + bits_per_sector - 1) / bits_per_sector;
    uint32_t inode_table_lba = inode_bitmap_lba + inode_bitmap_blocks;
    uint32_t inode_table_blocks = (inode_count + inodes_per_sector - 1) / inodes_per_sector;

    /* The block bitmap has to cover the data region, whose size in
     * turn depends on how big the bitmap itself is - solved by
     * assuming every remaining sector is data first, sizing the
     * bitmap for that (an overestimate by at most one bitmap sector's
     * worth of blocks), then shrinking the data region by exactly the
     * bitmap's real size. */
    uint32_t block_bitmap_lba = inode_table_lba + inode_table_blocks;
    uint32_t remaining = total_sectors - block_bitmap_lba;
    uint32_t block_bitmap_blocks = (remaining + bits_per_sector - 1) / bits_per_sector;
    uint32_t data_start_lba = block_bitmap_lba + block_bitmap_blocks;
    if (data_start_lba >= total_sectors) {
        return -1;
    }

    out->magic = WRF_MAGIC;
    out->version = WRF_VERSION;
    out->total_blocks = total_sectors - data_start_lba;
    out->inode_count = inode_count;
    out->inode_bitmap_lba = inode_bitmap_lba;
    out->inode_bitmap_blocks = inode_bitmap_blocks;
    out->block_bitmap_lba = block_bitmap_lba;
    out->block_bitmap_blocks = block_bitmap_blocks;
    out->inode_table_lba = inode_table_lba;
    out->inode_table_blocks = inode_table_blocks;
    out->data_start_lba = data_start_lba;
    out->root_ino = WRF_ROOT_INO;
    for (size_t i = 0; i < sizeof(out->reserved); i++) {
        out->reserved[i] = 0;
    }
    return 0;
}

#endif /* TUS_WRF_H */
