/*
 * wrf.c - WRF filesystem driver
 *
 * Talks straight to the disk with ata_read()/ata_write() (see
 * kernel/drivers/ata/ata.h) rather than through the VFS's fd-based API:
 * this runs at boot before the scheduler exists (kernel/main.c mounts
 * it right after the rootfs), and vfs_open()/vfs_read() need a
 * current task's fd table, which does not exist yet. mkfs.wrf, the
 * userspace formatter, is the other side of the same disk layout
 * (include/wrf.h) and goes through /dev/hdX like any other program -
 * it runs after boot, so it has no such constraint.
 *
 * Every metadata write (a bitmap bit, an inode, a directory entry) is
 * flushed to disk immediately - no write-back caching, no journal.
 * Simple and correct; a crash mid-operation can still leave the disk
 * inconsistent (e.g. a block marked allocated that no inode points
 * at, wasting space but never corrupting anything reachable), the
 * same honesty ext2 had before ext3's journal. Data blocks and bitmap
 * blocks alike are cached in RAM only for the two bitmaps themselves
 * (small - kilobytes - and hot on every allocation); inodes and file
 * data are read fresh from disk each call, deliberately: this is a
 * hobby OS's first persistent filesystem, and a stale cache silently
 * disagreeing with the disk is a worse bug than being slow.
 */

#include "wrf.h"

#include "../core/klib.h"
#include "../drivers/ata/ata.h"
#include "../drivers/rtc/rtc.h"
#include "../mm/kmalloc.h"
#include "../vfs/vfs.h"
#include "../../include/wrf.h"

static bool g_mounted = false;
static int g_disk = -1;
static struct wrf_superblock g_sb;
static uint8_t *g_inode_bitmap; /* g_sb.inode_bitmap_blocks * 512 bytes */
static uint8_t *g_block_bitmap; /* g_sb.block_bitmap_blocks * 512 bytes */

/* ---- bit twiddling ---- */

static int bit_test(const uint8_t *bm, uint32_t i) {
    return (bm[i / 8] >> (i % 8)) & 1;
}
static void bit_set(uint8_t *bm, uint32_t i) {
    bm[i / 8] |= (uint8_t)(1u << (i % 8));
}
static void bit_clear(uint8_t *bm, uint32_t i) {
    bm[i / 8] &= (uint8_t)~(1u << (i % 8));
}

/* Writes back just the one 512-byte sector `bit` lives in - every
 * alloc/free touches one bit, so this is one sector of I/O, not a
 * whole-bitmap flush. */
static void flush_bitmap_sector(const uint8_t *bm, uint32_t bit, uint32_t bitmap_lba) {
    uint32_t sector = (bit / 8) / WRF_BLOCK_SIZE;
    ata_write(g_disk, bitmap_lba + sector, 1, bm + (size_t)sector * WRF_BLOCK_SIZE);
}

/* Inode 0 is reserved (see include/wrf.h); the allocator never hands
 * it out, so a 0 inode number reliably means "no inode" everywhere
 * else in this file and in vfs_node.wrf_ino. */
static int32_t wrf_inode_alloc(void) {
    for (uint32_t i = 1; i < g_sb.inode_count; i++) {
        if (!bit_test(g_inode_bitmap, i)) {
            bit_set(g_inode_bitmap, i);
            flush_bitmap_sector(g_inode_bitmap, i, g_sb.inode_bitmap_lba);
            return (int32_t)i;
        }
    }
    return -1;
}

static void wrf_inode_free_bit(uint32_t ino) {
    bit_clear(g_inode_bitmap, ino);
    flush_bitmap_sector(g_inode_bitmap, ino, g_sb.inode_bitmap_lba);
}

/* Newly allocated data blocks come back zeroed - a read of a hole
 * past a file's old size (e.g. after O_TRUNC-then-grow reusing a
 * block wrf_truncate() left allocated) must see zero bytes, matching
 * ramfs's ftruncate()-grow, which memsets the same way. */
static int32_t wrf_block_alloc(void) {
    for (uint32_t i = 0; i < g_sb.total_blocks; i++) {
        if (!bit_test(g_block_bitmap, i)) {
            bit_set(g_block_bitmap, i);
            flush_bitmap_sector(g_block_bitmap, i, g_sb.block_bitmap_lba);
            uint8_t zero[WRF_BLOCK_SIZE];
            memset(zero, 0, sizeof(zero));
            ata_write(g_disk, g_sb.data_start_lba + i, 1, zero);
            return (int32_t)i;
        }
    }
    return -1;
}

static void wrf_block_free(uint32_t blk) {
    bit_clear(g_block_bitmap, blk);
    flush_bitmap_sector(g_block_bitmap, blk, g_sb.block_bitmap_lba);
}

/* ---- inode table ---- */

static void wrf_inode_read(uint32_t ino, struct wrf_inode *out) {
    uint8_t sector[WRF_BLOCK_SIZE];
    ata_read(g_disk, g_sb.inode_table_lba + ino / 4, 1, sector);
    memcpy(out, sector + (ino % 4) * sizeof(*out), sizeof(*out));
}

static void wrf_inode_write(uint32_t ino, const struct wrf_inode *in) {
    uint8_t sector[WRF_BLOCK_SIZE];
    ata_read(g_disk, g_sb.inode_table_lba + ino / 4, 1, sector);
    memcpy(sector + (ino % 4) * sizeof(*in), in, sizeof(*in));
    ata_write(g_disk, g_sb.inode_table_lba + ino / 4, 1, sector);
}

/* ---- block mapping ----
 *
 * Translates a file-relative block index into an on-disk block
 * number, walking direct pointers, then the indirect block, then the
 * doubly-indirect block (see include/wrf.h's layout comment). With
 * `alloc`, a hole is filled in: a fresh block is allocated and the
 * pointer that was 0 is written back to disk immediately (the
 * indirect/doubly-indirect index blocks are themselves ordinary
 * allocated blocks, addressed the same way as data blocks).
 *
 * The return value is the on-disk block number PLUS ONE, not the raw
 * block number - the same +1 bias every pointer field (direct[],
 * indirect, dindirect, and every entry of an indirect/doubly-indirect
 * table) is stored with. Block 0 is a perfectly valid, real data
 * block wrf_block_alloc() can and does hand out, so a return value of
 * plain 0 would be ambiguous between "the real block is 0" and "there
 * is no block" (a hole, or allocation failed) - the two cases a
 * caller most needs to tell apart. With the bias, 0 unambiguously
 * means "no block" and every real answer is nonzero; the caller
 * subtracts 1 before using it as an LBA offset. */
static uint32_t wrf_bmap(uint32_t ino, struct wrf_inode *inode, uint32_t lblk, bool alloc) {
    if (lblk < WRF_DIRECT) {
        if (inode->direct[lblk] == 0 && alloc) {
            int32_t blk = wrf_block_alloc();
            if (blk < 0) {
                return 0;
            }
            inode->direct[lblk] = (uint32_t)(blk + 1); /* +1: 0 stays "none" */
            wrf_inode_write(ino, inode);
        }
        return inode->direct[lblk];
    }
    lblk -= WRF_DIRECT;

    const uint32_t per_block = WRF_BLOCK_SIZE / sizeof(uint32_t); /* 128 */

    if (lblk < per_block) {
        if (inode->indirect == 0) {
            if (!alloc) {
                return 0;
            }
            int32_t blk = wrf_block_alloc();
            if (blk < 0) {
                return 0;
            }
            inode->indirect = (uint32_t)(blk + 1);
            wrf_inode_write(ino, inode);
        }
        uint32_t table[128];
        ata_read(g_disk, g_sb.data_start_lba + (inode->indirect - 1), 1, table);
        if (table[lblk] == 0 && alloc) {
            int32_t blk = wrf_block_alloc();
            if (blk < 0) {
                return 0;
            }
            table[lblk] = (uint32_t)(blk + 1);
            ata_write(g_disk, g_sb.data_start_lba + (inode->indirect - 1), 1, table);
        }
        return table[lblk];
    }
    lblk -= per_block;

    uint32_t outer_idx = lblk / per_block;
    uint32_t inner_idx = lblk % per_block;
    if (outer_idx >= per_block) {
        return 0; /* past the largest file WRF can represent */
    }

    if (inode->dindirect == 0) {
        if (!alloc) {
            return 0;
        }
        int32_t blk = wrf_block_alloc();
        if (blk < 0) {
            return 0;
        }
        inode->dindirect = (uint32_t)(blk + 1);
        wrf_inode_write(ino, inode);
    }
    uint32_t outer[128];
    ata_read(g_disk, g_sb.data_start_lba + (inode->dindirect - 1), 1, outer);
    if (outer[outer_idx] == 0) {
        if (!alloc) {
            return 0;
        }
        int32_t blk = wrf_block_alloc();
        if (blk < 0) {
            return 0;
        }
        outer[outer_idx] = (uint32_t)(blk + 1);
        ata_write(g_disk, g_sb.data_start_lba + (inode->dindirect - 1), 1, outer);
    }
    uint32_t inner[128];
    ata_read(g_disk, g_sb.data_start_lba + (outer[outer_idx] - 1), 1, inner);
    if (inner[inner_idx] == 0 && alloc) {
        int32_t blk = wrf_block_alloc();
        if (blk < 0) {
            return 0;
        }
        inner[inner_idx] = (uint32_t)(blk + 1);
        ata_write(g_disk, g_sb.data_start_lba + (outer[outer_idx] - 1), 1, inner);
    }
    return inner[inner_idx];
}

/* Reads or writes `count` bytes at byte offset `pos` of inode `ino`
 * (`inode`: its already-loaded on-disk struct, mutated and written
 * back here as blocks are allocated or the file grows). Shared by
 * regular file I/O and directory entry I/O - a directory's contents
 * are just bytes, exactly like a file's (see include/wrf.h). Returns
 * bytes moved. */
static long wrf_raw_io(uint32_t ino, struct wrf_inode *inode, void *buf_, size_t count,
                        size_t pos, bool do_write) {
    uint8_t *buf = buf_;
    size_t done = 0;
    while (done < count) {
        uint32_t lblk = (uint32_t)((pos + done) / WRF_BLOCK_SIZE);
        uint32_t off = (uint32_t)((pos + done) % WRF_BLOCK_SIZE);
        size_t chunk = WRF_BLOCK_SIZE - off;
        if (chunk > count - done) {
            chunk = count - done;
        }

        if (do_write) {
            uint32_t biased = wrf_bmap(ino, inode, lblk, true);
            if (biased == 0) {
                break; /* disk full: stop, report the bytes moved so far */
            }
            uint32_t blk = biased - 1;
            uint8_t sector[WRF_BLOCK_SIZE];
            if (chunk != WRF_BLOCK_SIZE) {
                ata_read(g_disk, g_sb.data_start_lba + blk, 1, sector);
            }
            memcpy(sector + off, buf + done, chunk);
            ata_write(g_disk, g_sb.data_start_lba + blk, 1, sector);
        } else {
            uint32_t biased = wrf_bmap(ino, inode, lblk, false);
            if (biased == 0) {
                memset(buf + done, 0, chunk); /* a hole reads as zero */
            } else {
                uint8_t sector[WRF_BLOCK_SIZE];
                ata_read(g_disk, g_sb.data_start_lba + (biased - 1), 1, sector);
                memcpy(buf + done, sector + off, chunk);
            }
        }
        done += chunk;
    }

    if (do_write && pos + done > inode->size) {
        inode->size = pos + done;
        wrf_inode_write(ino, inode);
    }
    return (long)done;
}

/* ---- directory entries ----
 *
 * A directory's data is a flat array of struct wrf_dirent - see
 * include/wrf.h. No hashing, no sorting: every lookup is a linear
 * scan, which is fine at the scale a hand-written OS's filesystem
 * actually sees and is what keeps this simple enough to have gotten
 * right in one pass. */

static int wrf_dir_find(uint32_t dir_ino, const char *name, size_t *out_pos,
                         uint32_t *out_child) {
    struct wrf_inode di;
    wrf_inode_read(dir_ino, &di);
    struct wrf_dirent de;
    for (size_t pos = 0; pos + sizeof(de) <= di.size; pos += sizeof(de)) {
        wrf_raw_io(dir_ino, &di, &de, sizeof(de), pos, false);
        if (de.inode != 0 && strcmp(de.name, name) == 0) {
            if (out_pos) {
                *out_pos = pos;
            }
            if (out_child) {
                *out_child = de.inode;
            }
            return 0;
        }
    }
    return -1;
}

static void wrf_dir_append(uint32_t dir_ino, uint32_t child_ino, uint8_t type,
                            const char *name) {
    struct wrf_dirent de;
    memset(&de, 0, sizeof(de));
    de.inode = child_ino;
    de.type = type;
    size_t nlen = strlen(name);
    if (nlen >= sizeof(de.name)) {
        nlen = sizeof(de.name) - 1;
    }
    memcpy(de.name, name, nlen);

    struct wrf_inode di;
    wrf_inode_read(dir_ino, &di);
    struct wrf_dirent cur;
    for (size_t pos = 0; pos + sizeof(cur) <= di.size; pos += sizeof(cur)) {
        wrf_raw_io(dir_ino, &di, &cur, sizeof(cur), pos, false);
        if (cur.inode == 0) {
            wrf_raw_io(dir_ino, &di, &de, sizeof(de), pos, true);
            return;
        }
    }
    wrf_raw_io(dir_ino, &di, &de, sizeof(de), di.size, true);
}

static void wrf_dir_remove(uint32_t dir_ino, const char *name) {
    size_t pos;
    if (wrf_dir_find(dir_ino, name, &pos, NULL) != 0) {
        return;
    }
    struct wrf_inode di;
    wrf_inode_read(dir_ino, &di);
    struct wrf_dirent tomb;
    memset(&tomb, 0, sizeof(tomb));
    wrf_raw_io(dir_ino, &di, &tomb, sizeof(tomb), pos, true);
}

/* ---- vfs.c hooks (see wrf.h for what each is called for) ---- */

void wrf_notify_create(struct vfs_node *dir, struct vfs_node *node) {
    if (dir->wrf_ino == 0) {
        return;
    }
    int32_t ino = wrf_inode_alloc();
    if (ino < 0) {
        kprintf("wrf: out of inodes, %s not persisted\n", node->name);
        return;
    }
    struct wrf_inode di;
    memset(&di, 0, sizeof(di));
    di.mode = (node->type == VFS_DIR ? WRF_IFDIR : WRF_IFREG) | (node->mode & 07777u);
    di.uid = node->uid;
    di.gid = node->gid;
    di.nlink = 1;
    di.atime = di.mtime = di.ctime = (uint32_t)rtc_now();
    wrf_inode_write((uint32_t)ino, &di);

    node->wrf_ino = (uint32_t)ino;
    wrf_dir_append(dir->wrf_ino, (uint32_t)ino,
                   node->type == VFS_DIR ? WRF_DT_DIR : WRF_DT_REG, node->name);
}

void wrf_notify_detach(struct vfs_node *dir, struct vfs_node *node) {
    if (dir->wrf_ino == 0 || node->wrf_ino == 0) {
        return;
    }
    wrf_dir_remove(dir->wrf_ino, node->name);
}

void wrf_notify_attach(struct vfs_node *dir, struct vfs_node *node) {
    if (dir->wrf_ino == 0 || node->wrf_ino == 0) {
        return;
    }
    wrf_dir_append(dir->wrf_ino, node->wrf_ino,
                   node->type == VFS_DIR ? WRF_DT_DIR : WRF_DT_REG, node->name);
}

void wrf_notify_free(struct vfs_node *node) {
    if (node->wrf_ino == 0) {
        return;
    }
    struct wrf_inode di;
    wrf_inode_read(node->wrf_ino, &di);

    for (int i = 0; i < WRF_DIRECT; i++) {
        if (di.direct[i]) {
            wrf_block_free(di.direct[i] - 1);
        }
    }
    if (di.indirect) {
        uint32_t table[128];
        ata_read(g_disk, g_sb.data_start_lba + (di.indirect - 1), 1, table);
        for (int i = 0; i < 128; i++) {
            if (table[i]) {
                wrf_block_free(table[i] - 1);
            }
        }
        wrf_block_free(di.indirect - 1);
    }
    if (di.dindirect) {
        uint32_t outer[128];
        ata_read(g_disk, g_sb.data_start_lba + (di.dindirect - 1), 1, outer);
        for (int i = 0; i < 128; i++) {
            if (!outer[i]) {
                continue;
            }
            uint32_t inner[128];
            ata_read(g_disk, g_sb.data_start_lba + (outer[i] - 1), 1, inner);
            for (int j = 0; j < 128; j++) {
                if (inner[j]) {
                    wrf_block_free(inner[j] - 1);
                }
            }
            wrf_block_free(outer[i] - 1);
        }
        wrf_block_free(di.dindirect - 1);
    }

    wrf_inode_free_bit(node->wrf_ino);
    node->wrf_ino = 0;
}

long wrf_file_read(struct vfs_node *node, void *buf, size_t count, size_t pos) {
    if (pos >= node->size) {
        return 0;
    }
    if (count > node->size - pos) {
        count = node->size - pos;
    }
    struct wrf_inode di;
    wrf_inode_read(node->wrf_ino, &di);
    return wrf_raw_io(node->wrf_ino, &di, buf, count, pos, false);
}

long wrf_file_write(struct vfs_node *node, const void *buf, size_t count, size_t pos) {
    struct wrf_inode di;
    wrf_inode_read(node->wrf_ino, &di);
    long n = wrf_raw_io(node->wrf_ino, &di, (void *)buf, count, pos, true);
    if (n > 0 && pos + (size_t)n > node->size) {
        node->size = pos + (size_t)n;
    }
    return n;
}

long wrf_truncate(struct vfs_node *node, size_t length) {
    struct wrf_inode di;
    wrf_inode_read(node->wrf_ino, &di);
    di.size = length;
    di.mtime = (uint32_t)rtc_now();
    wrf_inode_write(node->wrf_ino, &di);
    node->size = length;
    return 0;
}

/* ---- mount ---- */

/* Recursively populates `parent`'s in-memory subtree from dir_ino's
 * on-disk entries - the same "the whole tree lives in memory, backed
 * by wrf_ino" model kernel/vfs/rootfs.c uses for the ustar image,
 * just built from a real filesystem instead of a tar blob. Links
 * nodes directly (dir_attach() is private to vfs.c) rather than
 * through vfs_create_dir()/vfs_create_file(), which would re-parse a
 * path string and re-check for a duplicate for every entry - wasted
 * work when the tree is being built fresh. */
static void wrf_walk_dir(struct vfs_node *parent, uint32_t dir_ino) {
    struct wrf_inode di;
    wrf_inode_read(dir_ino, &di);
    struct wrf_dirent de;
    for (size_t pos = 0; pos + sizeof(de) <= di.size; pos += sizeof(de)) {
        wrf_raw_io(dir_ino, &di, &de, sizeof(de), pos, false);
        if (de.inode == 0) {
            continue;
        }
        struct wrf_inode ci;
        wrf_inode_read(de.inode, &ci);

        struct vfs_node *node = kmalloc(sizeof(*node));
        if (node == NULL) {
            continue;
        }
        memset(node, 0, sizeof(*node));
        node->type = (ci.mode & WRF_IFMT) == WRF_IFDIR ? VFS_DIR : VFS_FILE;
        node->mode = ci.mode & 07777u;
        node->uid = ci.uid;
        node->gid = ci.gid;
        node->size = (size_t)ci.size;
        node->wrf_ino = de.inode;
        size_t nlen = strlen(de.name);
        if (nlen >= VFS_NAME_MAX) {
            nlen = VFS_NAME_MAX - 1;
        }
        memcpy(node->name, de.name, nlen);
        node->name[nlen] = '\0';

        node->parent = parent;
        node->sibling = parent->child;
        parent->child = node;

        if (node->type == VFS_DIR) {
            wrf_walk_dir(node, de.inode);
        }
    }
}

static uint32_t get_le32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

/* Tries to read a valid WRF superblock starting at `start_lba` on
 * `disk` and, if found, finishes mounting it there. Returns true on
 * success. On success every subsequent wrf.c function can go on
 * treating g_sb's LBA fields as plain disk-absolute sector numbers
 * without knowing anything about partitions - that's what folding
 * `start_lba` into them once, right here, buys the rest of the file. */
static bool try_mount_at(struct vfs_node *mnt, int disk, uint32_t start_lba) {
    uint8_t buf[WRF_BLOCK_SIZE];
    if (ata_read(disk, start_lba, 1, buf) != 0) {
        return false;
    }
    struct wrf_superblock sb;
    memcpy(&sb, buf, sizeof(sb));
    if (sb.magic != WRF_MAGIC || sb.version != WRF_VERSION) {
        return false;
    }

    g_disk = disk;
    g_sb = sb;
    g_sb.inode_bitmap_lba += start_lba;
    g_sb.block_bitmap_lba += start_lba;
    g_sb.inode_table_lba += start_lba;
    g_sb.data_start_lba += start_lba;

    size_t ib_bytes = (size_t)g_sb.inode_bitmap_blocks * WRF_BLOCK_SIZE;
    size_t bb_bytes = (size_t)g_sb.block_bitmap_blocks * WRF_BLOCK_SIZE;
    g_inode_bitmap = kmalloc(ib_bytes);
    g_block_bitmap = kmalloc(bb_bytes);
    if (g_inode_bitmap == NULL || g_block_bitmap == NULL) {
        kprintf("wrf: out of memory loading bitmaps, /home not mounted\n");
        g_disk = -1;
        return false;
    }
    for (uint32_t i = 0; i < g_sb.inode_bitmap_blocks; i++) {
        ata_read(g_disk, g_sb.inode_bitmap_lba + i, 1, g_inode_bitmap + (size_t)i * WRF_BLOCK_SIZE);
    }
    for (uint32_t i = 0; i < g_sb.block_bitmap_blocks; i++) {
        ata_read(g_disk, g_sb.block_bitmap_lba + i, 1, g_block_bitmap + (size_t)i * WRF_BLOCK_SIZE);
    }

    g_mounted = true;
    mnt->wrf_ino = g_sb.root_ino;
    struct wrf_inode root_di;
    wrf_inode_read(g_sb.root_ino, &root_di);
    mnt->mode = root_di.mode & 07777u;
    wrf_walk_dir(mnt, g_sb.root_ino);

    kprintf("wrf          : mounted /home from disk %d, LBA %u (%u inodes, %u KiB data)\n",
            disk, start_lba, g_sb.inode_count, g_sb.total_blocks / 2);
    return true;
}

/* Tries `disk` for WRF: first a partition of type WRF_PART_TYPE in
 * its MBR (tusinstall writes one next to the boot ESP, sharing the
 * disk the system itself boots from), then - whether or not that disk
 * even has a partition table - a bare WRF superblock at LBA 0 (a
 * whole disk dedicated to WRF, formatted directly with `mkfs.wrf
 * /dev/hdX`, no partition table involved). A disk holding neither is
 * simply not WRF's; reading its boot sector as a candidate superblock
 * in the second attempt is safe even when it holds something else
 * entirely (an MBR, a different filesystem's own superblock) because
 * WRF_MAGIC matching that by coincidence is not a real concern. */
static bool try_mount_disk(struct vfs_node *mnt, int disk) {
    uint8_t mbr[WRF_BLOCK_SIZE];
    if (ata_read(disk, 0, 1, mbr) == 0 && mbr[510] == 0x55 && mbr[511] == 0xAA) {
        for (int i = 0; i < 4; i++) {
            const uint8_t *e = mbr + 446 + i * 16;
            if (e[4] == WRF_PART_TYPE) {
                uint32_t start = get_le32(e + 8);
                if (try_mount_at(mnt, disk, start)) {
                    return true;
                }
            }
        }
    }
    return try_mount_at(mnt, disk, 0);
}

void wrf_boot_mount(void) {
    struct vfs_node *mnt = vfs_lookup("/home");
    if (mnt == NULL) {
        mnt = vfs_create_dir("/home");
    }
    if (mnt == NULL) {
        kprintf("wrf: could not create /home, persistent storage unavailable\n");
        return;
    }

    if (ata_disk_count() == 0) {
        kprintf("wrf: no ATA disk present, /home not mounted\n");
        return;
    }
    /* ata_disk_count() is a COUNT, not a bound on valid indices - the
     * CD-ROM TUS itself booted from is usually ATAPI at index 0
     * (primary master), so the one real disk this loop wants can sit
     * at any slot up to ATA_MAX_DISKS. Every slot is checked and
     * ata_read()/ata_transfer() already refuse an absent or ATAPI
     * one (-ENODEV), so trying all of them is simply safe. */
    for (int disk = 0; disk < ATA_MAX_DISKS; disk++) {
        const struct ata_disk *d = ata_disk(disk);
        if (d == NULL || !d->present || d->atapi) {
            continue;
        }
        if (try_mount_disk(mnt, disk)) {
            return;
        }
    }
    kprintf("wrf: no WRF filesystem found on any disk (run mkfs.wrf, or "
            "install with tusinstall, to create one) - /home not mounted\n");
}
