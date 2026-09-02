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
 *   [inode_table_lba   ..]       inode table, 4 inodes per sector
 *   [block_bitmap_lba  ..]       data block bitmap, 1 bit per block
 *   [checksum_table_lba ..]      per-block CRC32, 128 entries per sector (v2)
 *   [data_start_lba    ..]       data blocks (files, directory data)
 *   [last sector]                backup superblock (v2)
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
#define WRF_VERSION     2
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

/* v2 integrity additions (ZFS-inspired, scoped to what a single-disk,
 * no-journal filesystem can honestly deliver - see the block comment
 * above wrf_crc32() for exactly what these do and don't protect
 * against):
 *
 *   - checksum_table_lba/blocks: a table of one CRC32 per block in the
 *     data address space (which the direct/indirect/dindirect index
 *     blocks live in too, not just file data - see wrf_bmap() in
 *     wrf.c), so silent corruption of any block already on disk is
 *     detected the next time it is read, not just "trusted".
 *   - generation/sb_checksum: the superblock itself is checksummed,
 *     and written TWICE - once at LBA 0 of the volume (the classic
 *     spot) and once more at the volume's last sector (a "backup
 *     uberblock", ZFS's term for the same idea). wrf_boot_mount()
 *     reads both, accepts whichever has a valid magic/version/
 *     checksum, and prefers the higher `generation` if both are
 *     valid - a single flipped bit or dead sector on either copy no
 *     longer means the whole volume is unreadable. `generation` is
 *     always 1 for a freshly-formatted volume (mkfs.wrf and
 *     tusinstall.c both write two identical copies); WRF v2 never
 *     rewrites the superblock after mkfs time, so there is no
 *     runtime uberblock ring the way ZFS has one - the second copy
 *     exists purely as insurance against one copy's medium going
 *     bad, not as a transaction log.
 */
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
    uint32_t checksum_table_lba;
    uint32_t checksum_table_blocks;
    uint64_t generation;
    uint32_t sb_checksum;         /* CRC32 of this struct with this field zeroed */
    uint8_t  reserved[WRF_BLOCK_SIZE -
                       (15 * (int)sizeof(uint32_t) + (int)sizeof(uint64_t))];
} __attribute__((packed));

/* Every reader of a superblock sector (wrf.c's try_mount_at()) reads
 * exactly WRF_BLOCK_SIZE bytes into a WRF_BLOCK_SIZE-byte stack buffer
 * and then memcpy's sizeof(struct wrf_superblock) out of it - a
 * `reserved[]` sized even one field short of exactly filling the rest
 * of the sector (as v2 briefly was: `sb_checksum` got added without
 * updating the reserved-array size math, making the struct 516 bytes)
 * turns that memcpy into a 4-byte stack over-read with no compiler
 * warning, corrupting whatever local variable happened to sit next to
 * the buffer - which is exactly the bug a scoped-down "make it more
 * secure" pass must not itself introduce. This fires at compile time,
 * not boot time, the moment the struct's true size drifts from 512. */
_Static_assert(sizeof(struct wrf_superblock) == WRF_BLOCK_SIZE,
               "wrf_superblock must be exactly one block - fix the reserved[] size math");

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

/* Bitwise (table-free, on purpose - this header is included by both
 * the freestanding kernel and userspace formatters, and a static
 * 1 KiB CRC table is a worse tradeoff than ~4000 extra branches per
 * 512-byte block on hardware this small) CRC32/IEEE-802.3, the same
 * polynomial ext4 and ZFS's fletcher-free "crc32" checksum use. This
 * is what WRF v2 leans on for its whole integrity story: every block
 * in the data address space (file data AND the indirect/dindirect
 * index blocks that address it - see wrf_bmap() in wrf.c) has its
 * checksum stored OUT OF BAND in a separate checksum table rather
 * than next to the data, exactly ZFS's point about self-checksumming
 * data being useless (a torn write corrupts the data and its
 * checksum together) - here, a write only updates a block's stored
 * checksum after the block write itself succeeds (wrf_data_write() in
 * wrf.c), so a read through wrf_data_read() catches bit rot, a
 * misdirected write, or any other silent corruption between the two
 * writes. What this does NOT provide is transactional atomicity
 * across MULTIPLE blocks: WRF v2 still writes each block in place,
 * immediately, with no copy-on-write and no journal (see the file
 * header comment in wrf.c) - a crash between two block writes of the
 * same logical operation (e.g. growing a file past a block boundary)
 * can still leave that operation half-done, exactly as v1 could. Full
 * copy-on-write would mean never overwriting a live block in place -
 * a bigger redesign of the allocator and every metadata pointer -
 * deliberately left for a future WRF version rather than attempted
 * half-verified here. */
static inline uint32_t wrf_crc32(const void *data, size_t len) {
    const uint8_t *p = data;
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; i++) {
        crc ^= p[i];
        for (int bit = 0; bit < 8; bit++) {
            uint32_t mask = (uint32_t)(-(int32_t)(crc & 1u));
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }
    return crc ^ 0xFFFFFFFFu;
}

/* The superblock's own self-check: CRC32 of the whole struct with
 * sb_checksum zeroed out first (it can't include itself). Shared by
 * every writer (mkfs.wrf, tusinstall.c) and reader (wrf.c's
 * wrf_boot_mount()) so a bit flip anywhere in the superblock -
 * including in a field a future version adds - is caught the same
 * way on every path. */
static inline uint32_t wrf_sb_checksum(const struct wrf_superblock *sb) {
    struct wrf_superblock tmp = *sb;
    tmp.sb_checksum = 0;
    return wrf_crc32(&tmp, sizeof(tmp));
}

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
    if (total_sectors < 66) { /* 64 (v1 floor) + the backup superblock sector's neighborhood */
        return -1;
    }
    /* The volume's very last sector is reserved for the backup
     * superblock (see the v2 integrity comment above struct
     * wrf_superblock); every other LBA in this function is computed
     * against `usable`, not `total_sectors`, so the data region never
     * grows into it. */
    uint32_t usable = total_sectors - 1;

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

    /* The block bitmap (1 bit/block) and the checksum table (one
     * 4-byte CRC32/block, so 128 entries per sector) both have to
     * cover the data region, whose size in turn depends on how big
     * *they* are - solved the same way v1 solved it for the bitmap
     * alone: assume every remaining sector is data first, size both
     * regions for that upper bound (each ends up at most one sector
     * bigger than strictly necessary), then shrink the data region by
     * their combined real size. Both estimates only shrink as the data
     * region shrinks, so sizing them against the overestimate still
     * leaves both regions large enough for the real, smaller
     * total_blocks computed below. */
    uint32_t block_bitmap_lba = inode_table_lba + inode_table_blocks;
    if (usable <= block_bitmap_lba) {
        return -1;
    }
    uint32_t remaining = usable - block_bitmap_lba;
    uint32_t block_bitmap_blocks = (remaining + bits_per_sector - 1) / bits_per_sector;
    uint32_t csum_entries_per_sector = WRF_BLOCK_SIZE / (uint32_t)sizeof(uint32_t); /* 128 */
    uint32_t checksum_table_blocks = (remaining + csum_entries_per_sector - 1) / csum_entries_per_sector;
    uint32_t checksum_table_lba = block_bitmap_lba + block_bitmap_blocks;
    uint32_t data_start_lba = checksum_table_lba + checksum_table_blocks;
    if (data_start_lba >= usable) {
        return -1;
    }

    out->magic = WRF_MAGIC;
    out->version = WRF_VERSION;
    out->total_blocks = usable - data_start_lba;
    out->inode_count = inode_count;
    out->inode_bitmap_lba = inode_bitmap_lba;
    out->inode_bitmap_blocks = inode_bitmap_blocks;
    out->block_bitmap_lba = block_bitmap_lba;
    out->block_bitmap_blocks = block_bitmap_blocks;
    out->inode_table_lba = inode_table_lba;
    out->inode_table_blocks = inode_table_blocks;
    out->data_start_lba = data_start_lba;
    out->root_ino = WRF_ROOT_INO;
    out->checksum_table_lba = checksum_table_lba;
    out->checksum_table_blocks = checksum_table_blocks;
    out->generation = 1;
    out->sb_checksum = 0;
    for (size_t i = 0; i < sizeof(out->reserved); i++) {
        out->reserved[i] = 0;
    }
    out->sb_checksum = wrf_sb_checksum(out);
    return 0;
}

#endif /* TUS_WRF_H */
