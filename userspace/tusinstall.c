/*
 * tusinstall - install TUS onto a disk
 *
 * What "installing" means here is the same thing it means anywhere
 * else: put the system somewhere the firmware can find it at power
 * on, and leave the machine able to boot without the medium it was
 * booted from.
 *
 * TUS is booted by Limine, and Limine's EFI executable boots off an
 * EFI System Partition - a FAT filesystem with a known layout. So the
 * installer does exactly that, plus one more thing:
 *
 *   1. an MBR with TWO partitions:
 *        - type 0xEF (EFI System), sized to comfortably hold the
 *          running kernel and root filesystem plus room to grow
 *        - type WRF_PART_TYPE (see include/wrf.h), everything else
 *          on the disk
 *   2. a FAT32 filesystem in the first partition, written by hand
 *      (mkfs is 300 lines when the only files it has to hold are
 *      four)
 *   3. /EFI/BOOT/BOOTX64.EFI      the bootloader the firmware runs
 *      /boot/limine.conf          which kernel to boot, and with what
 *      /boot/kernel.elf           the kernel  - copied from /dev/kernel
 *      /boot/rootfs.img           the system  - copied from /dev/rootfs
 *   4. a WRF filesystem (include/wrf.h) in the second partition -
 *      empty, formatted and ready: the kernel mounts it at /mnt on
 *      every boot from here on (kernel/fs/wrf.c), so unlike the FAT32
 *      partition above - which is only ever read from again, never
 *      written to after this install - anything created under /mnt
 *      genuinely persists across a reboot. The installed OS itself
 *      still boots from a fresh copy of rootfs.img in RAM every time
 *      (see kernel/vfs/rootfs.c); WRF is what makes a write anywhere
 *      else on the system actually stick.
 *
 * The kernel and the root filesystem come out of MEMORY, not out of
 * files: /dev/kernel and /dev/rootfs are the images the bootloader
 * loaded. What gets installed is therefore by definition the system
 * that is running - there is no second copy to keep in step.
 *
 * Everything is written through /dev/hdX, which is a byte stream (the
 * kernel's disk device does the sector arithmetic), so this program
 * is plain open/lseek/read/write from beginning to end. The WRF
 * partition's own layout math (superblock, bitmap and inode-table
 * sizing) is not reimplemented here - wrf_compute_layout()
 * (include/wrf.h) is shared verbatim with userspace/mkfs_wrf.c, the
 * standalone formatter, so the two writers of a WRF superblock cannot
 * quietly drift apart.
 */

#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "wrf.h"

#define SECTOR       512
#define PART_START   2048          /* 1 MiB in, where everyone starts */
#define RESERVED     32            /* FAT32 reserved sectors */
#define NUM_FATS     2
#define ROOT_CLUSTER 2
#define COPY_CHUNK   (64u * 1024)
#define TAR_BLOCK    512
#define LINE_MAX_TUS 512

#define SYS_POWER        53
#define TUS_POWER_HALT    0
#define TUS_POWER_REBOOT  1

/* Where the pieces come from on the running system. */
#define SRC_LOADER "/boot/BOOTX64.EFI"
#define SRC_CONFIG "/boot/limine.conf"
#define SRC_KERNEL "/dev/kernel"
#define SRC_ROOTFS "/dev/rootfs"

static long tus_syscall(long n, long a1) {
    long ret;
    /* Only RAX survives the trap, so every argument register is
     * declared read-write - including the ones this call does not
     * use (see musl's tus_syscall.c). */
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = 0;
    register long rdx __asm__("rdx") = 0;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"(n)
                     : "rcx", "r11", "memory");
    return ret;
}

/* ---- the disk ---- */

static int g_disk = -1;            /* fd on /dev/hdX */
static uint64_t g_disk_bytes;

static int dev_write(uint64_t off, const void *buf, size_t len) {
    if (lseek(g_disk, (long)off, SEEK_SET) < 0) {
        return -1;
    }
    const uint8_t *p = buf;
    while (len > 0) {
        long n = write(g_disk, p, len);
        if (n <= 0) {
            return -1;
        }
        p += n;
        len -= (size_t)n;
    }
    return 0;
}

static int dev_read(uint64_t off, void *buf, size_t len) {
    if (lseek(g_disk, (long)off, SEEK_SET) < 0) {
        return -1;
    }
    uint8_t *p = buf;
    while (len > 0) {
        long n = read(g_disk, p, len);
        if (n <= 0) {
            return -1;
        }
        p += n;
        len -= (size_t)n;
    }
    return 0;
}

/* ---- little-endian stores, because a filesystem on disk is not a
 * struct: every field here has a fixed offset and a fixed width ---- */

static void put16(uint8_t *p, uint16_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

static void put32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

/* ---- the filesystem ---- */

struct fat32 {
    uint32_t part_start;    /* LBA of the partition */
    uint32_t part_sectors;
    uint32_t spc;           /* sectors per cluster */
    uint32_t fat_size;      /* sectors per FAT */
    uint32_t data_start;    /* LBA of cluster 2 */
    uint32_t clusters;      /* data clusters */
    uint32_t next_free;     /* linear allocator */
};

static struct fat32 g_fs;

static uint64_t cluster_off(uint32_t cluster) {
    return ((uint64_t)g_fs.data_start +
            (uint64_t)(cluster - 2) * g_fs.spc) * SECTOR;
}

/* One FAT sector, held back until another one is needed. A file of a
 * thousand clusters has a thousand FAT entries, and 128 of them share
 * a sector: without this, installing would be four disk operations
 * per cluster instead of four per 128.
 *
 * Both copies of the FAT are written from the cache, because a
 * filesystem with one good FAT and one stale one is one that another
 * system will "repair" the wrong way. */
static uint8_t g_fat_cache[SECTOR];
static uint32_t g_fat_sector = 0xFFFFFFFFu;
static int g_fat_dirty;

static int fat_flush(void) {
    if (!g_fat_dirty) {
        return 0;
    }
    for (int i = 0; i < NUM_FATS; i++) {
        uint64_t at = ((uint64_t)g_fs.part_start + RESERVED +
                       (uint64_t)i * g_fs.fat_size + g_fat_sector) * SECTOR;
        if (dev_write(at, g_fat_cache, SECTOR) != 0) {
            return -1;
        }
    }
    g_fat_dirty = 0;
    return 0;
}

static int fat_set(uint32_t cluster, uint32_t value) {
    uint32_t index = cluster * 4;
    uint32_t sec = index / SECTOR;
    uint32_t off = index % SECTOR;

    if (sec != g_fat_sector) {
        if (fat_flush() != 0) {
            return -1;
        }
        uint64_t at = ((uint64_t)g_fs.part_start + RESERVED + sec) * SECTOR;
        if (dev_read(at, g_fat_cache, SECTOR) != 0) {
            return -1;
        }
        g_fat_sector = sec;
    }
    put32(g_fat_cache + off, value & 0x0FFFFFFF);
    g_fat_dirty = 1;
    return 0;
}

static uint32_t cluster_alloc(void) {
    if (g_fs.next_free >= g_fs.clusters + 2) {
        return 0;
    }
    return g_fs.next_free++;
}

/* Fill a cluster with zeros - a fresh directory must not be read as
 * whatever the disk happened to hold. */
static int cluster_clear(uint32_t cluster) {
    static uint8_t zeros[SECTOR];
    for (uint32_t s = 0; s < g_fs.spc; s++) {
        if (dev_write(cluster_off(cluster) + (uint64_t)s * SECTOR, zeros,
                      SECTOR) != 0) {
            return -1;
        }
    }
    return 0;
}

/* 8.3 names, upper case, space padded: "BOOTX64 EFI". Everything TUS
 * installs has a name that fits, which is why there is no long-name
 * support here. */
static void name83(const char *name, char *out) {
    memset(out, ' ', 11);
    int i = 0;
    const char *dot = strchr(name, '.');
    int base = dot != NULL ? (int)(dot - name) : (int)strlen(name);
    for (i = 0; i < base && i < 8; i++) {
        char c = name[i];
        out[i] = (c >= 'a' && c <= 'z') ? (char)(c - 32) : c;
    }
    if (dot != NULL) {
        for (int j = 0; j < 3 && dot[1 + j] != '\0'; j++) {
            char c = dot[1 + j];
            out[8 + j] = (c >= 'a' && c <= 'z') ? (char)(c - 32) : c;
        }
    }
}

/* The 8.3 alias a long name hides behind: LIMINE~1.CON for
 * limine.conf. Nothing reads it (the long name is what programs
 * look for), but every entry must have one. */
static void short_alias(const char *name, char *out) {
    memset(out, ' ', 11);
    const char *dot = strrchr(name, '.');
    int base = dot != NULL ? (int)(dot - name) : (int)strlen(name);
    int n = base < 6 ? base : 6;
    for (int i = 0; i < n; i++) {
        char c = name[i];
        out[i] = (c >= 'a' && c <= 'z') ? (char)(c - 32) : c;
    }
    out[n] = '~';
    out[n + 1] = '1';
    if (dot != NULL) {
        for (int j = 0; j < 3 && dot[1 + j] != '\0'; j++) {
            char c = dot[1 + j];
            out[8 + j] = (c >= 'a' && c <= 'z') ? (char)(c - 32) : c;
        }
    }
}

/* The checksum that ties a long name to its 8.3 entry. Get it wrong
 * and every reader falls back to the alias - which is how a file
 * called limine.conf turns into LIMINE~1.CON and stops being found. */
static uint8_t sfn_checksum(const char *n11) {
    uint8_t sum = 0;
    for (int i = 0; i < 11; i++) {
        sum = (uint8_t)(((sum & 1) << 7) + (sum >> 1) + (uint8_t)n11[i]);
    }
    return sum;
}

static int name_is_83(const char *name) {
    const char *dot = strchr(name, '.');
    size_t base = dot != NULL ? (size_t)(dot - name) : strlen(name);
    size_t ext = dot != NULL ? strlen(dot + 1) : 0;
    if (base == 0 || base > 8 || ext > 3) {
        return 0;
    }
    return strchr(name, '.') == strrchr(name, '.'); /* one dot at most */
}

/*
 * Find `need` consecutive free entries in a directory, extending it
 * by a cluster when there is no room. Free slots at the end of a
 * sector that cannot hold the whole run are marked deleted (0xE5)
 * rather than left as zeros: a zero entry means "the directory ends
 * here", and leaving one in the middle hides everything after it.
 *
 * A run never has to straddle a sector: 16 entries fit in one, and
 * the longest name TUS installs needs two.
 */
static int dir_reserve(uint32_t dir_cluster, int need, uint32_t *out_cluster,
                       uint32_t *out_sector, int *out_index) {
    uint8_t sector[SECTOR];
    uint32_t cluster = dir_cluster;

    for (;;) {
        for (uint32_t s = 0; s < g_fs.spc; s++) {
            uint64_t at = cluster_off(cluster) + (uint64_t)s * SECTOR;
            if (dev_read(at, sector, SECTOR) != 0) {
                return -1;
            }
            for (int e = 0; e + need <= SECTOR / 32; e++) {
                int ok = 1;
                for (int i = 0; i < need; i++) {
                    uint8_t first = sector[(e + i) * 32];
                    if (first != 0x00 && first != 0xE5) {
                        ok = 0;
                        break;
                    }
                }
                if (!ok) {
                    continue;
                }
                *out_cluster = cluster;
                *out_sector = s;
                *out_index = e;
                return 0;
            }
            /* Room at the end of this sector, but not enough of it. */
            int dirty = 0;
            for (int e = 0; e < SECTOR / 32; e++) {
                if (sector[e * 32] == 0x00) {
                    sector[e * 32] = 0xE5;
                    dirty = 1;
                }
            }
            if (dirty && dev_write(at, sector, SECTOR) != 0) {
                return -1;
            }
        }

        uint32_t next = cluster_alloc();
        if (next == 0 || cluster_clear(next) != 0 ||
            fat_set(cluster, next) != 0 || fat_set(next, 0x0FFFFFFF) != 0 ||
            fat_flush() != 0) {
            return -1;
        }
        cluster = next;
    }
}

/* Fill in one 8.3 directory entry at `p`. */
static void fill_short(uint8_t *p, const char *n11, uint8_t attr,
                       uint32_t first_cluster, uint32_t size) {
    memset(p, 0, 32);
    memcpy(p, n11, 11);
    p[11] = attr;
    /* A date every tool accepts: 2026-01-01, 00:00. */
    put16(p + 22, 0);
    put16(p + 24, (uint16_t)(((2026 - 1980) << 9) | (1 << 5) | 1));
    put16(p + 20, (uint16_t)(first_cluster >> 16));
    put16(p + 26, (uint16_t)(first_cluster & 0xFFFF));
    put32(p + 28, size);
}

/*
 * Add an entry. A name that fits 8.3 gets one entry; anything longer
 * (limine.conf - four characters of extension, which 8.3 has no room
 * for) gets VFAT long-name entries in front of its alias, which is
 * what makes the bootloader find its configuration file by the name
 * it looks for.
 */
static int dir_add(uint32_t dir_cluster, const char *name, uint8_t attr,
                   uint32_t first_cluster, uint32_t size) {
    char n11[11];
    int longname = !name_is_83(name);
    size_t len = strlen(name);
    int lfn_entries = longname ? (int)((len + 12) / 13) : 0;
    int need = lfn_entries + 1;

    if (longname) {
        short_alias(name, n11);
    } else {
        name83(name, n11);
    }

    uint32_t cluster, sec;
    int index;
    if (dir_reserve(dir_cluster, need, &cluster, &sec, &index) != 0) {
        return -1;
    }

    uint8_t sector[SECTOR];
    uint64_t at = cluster_off(cluster) + (uint64_t)sec * SECTOR;
    if (dev_read(at, sector, SECTOR) != 0) {
        return -1;
    }

    if (longname) {
        uint8_t sum = sfn_checksum(n11);
        /* The parts are stored last first, so a reader walking
         * forwards sees them in reverse and reassembles the name. */
        for (int part = lfn_entries; part >= 1; part--) {
            uint8_t *p = sector + (index + (lfn_entries - part)) * 32;
            memset(p, 0, 32);
            p[0] = (uint8_t)(part | (part == lfn_entries ? 0x40 : 0));
            p[11] = 0x0F;      /* the long-name attribute */
            p[12] = 0;
            p[13] = sum;
            put16(p + 26, 0);  /* long-name entries carry no cluster */

            static const int slot[13] = { 1, 3, 5, 7, 9, 14, 16, 18, 20,
                                          22, 24, 28, 30 };
            for (int c = 0; c < 13; c++) {
                size_t at_char = (size_t)(part - 1) * 13 + (size_t)c;
                uint16_t ch;
                if (at_char < len) {
                    ch = (uint16_t)(unsigned char)name[at_char];
                } else if (at_char == len) {
                    ch = 0;          /* the terminator */
                } else {
                    ch = 0xFFFF;     /* padding */
                }
                put16(p + slot[c], ch);
            }
        }
    }

    fill_short(sector + (index + lfn_entries) * 32, n11, attr, first_cluster,
               size);
    return dev_write(at, sector, SECTOR);
}

/* Create a subdirectory and return its first cluster (0 on failure). */
static uint32_t dir_create(uint32_t parent, const char *name) {
    uint32_t cluster = cluster_alloc();
    if (cluster == 0 || cluster_clear(cluster) != 0 ||
        fat_set(cluster, 0x0FFFFFFF) != 0) {
        return 0;
    }
    if (dir_add(parent, name, 0x10, cluster, 0) != 0) {
        return 0;
    }

    /* "." and ".." - and ".." of a directory in the root is 0, not 2:
     * that is what the specification says and what checkers test. */
    uint8_t sector[SECTOR];
    memset(sector, 0, SECTOR);
    memset(sector, ' ', 11);
    sector[0] = '.';
    sector[11] = 0x10;
    put16(sector + 20, (uint16_t)(cluster >> 16));
    put16(sector + 26, (uint16_t)(cluster & 0xFFFF));
    memset(sector + 32, ' ', 11);
    sector[32] = '.';
    sector[33] = '.';
    sector[32 + 11] = 0x10;
    uint32_t up = parent == ROOT_CLUSTER ? 0 : parent;
    put16(sector + 32 + 20, (uint16_t)(up >> 16));
    put16(sector + 32 + 26, (uint16_t)(up & 0xFFFF));
    if (dev_write(cluster_off(cluster), sector, SECTOR) != 0) {
        return 0;
    }
    return cluster;
}

/* ---- progress ---- */

static void progress(const char *what, uint64_t done, uint64_t total) {
    int pct = total > 0 ? (int)((done * 100) / total) : 100;
    printf("\r  %-28s %3d%%", what, pct);
    fflush(stdout);
}

/* Copy a source file into the filesystem as `name` in `dir`.
 *
 * The allocator is linear and nothing else allocates while a file is
 * being written, so the clusters of one batch are consecutive on the
 * disk - which is why a batch can go out as ONE write instead of one
 * per cluster. */
static int copy_file(uint32_t dir, const char *name, const char *src,
                     const char *label) {
    int fd = open(src, O_RDONLY);
    if (fd < 0) {
        printf("\n  %s: cannot open\n", src);
        return -1;
    }
    long total = lseek(fd, 0, SEEK_END);
    if (total <= 0) {
        printf("\n  %s: empty\n", src);
        close(fd);
        return -1;
    }
    lseek(fd, 0, SEEK_SET);

    uint32_t cb = g_fs.spc * SECTOR;
    uint32_t per_batch = COPY_CHUNK / cb;
    if (per_batch == 0) {
        per_batch = 1;
    }
    size_t stage_size = (size_t)per_batch * cb;
    uint8_t *stage = malloc(stage_size);
    if (stage == NULL) {
        close(fd);
        return -1;
    }

    uint32_t first = 0, prev = 0;
    uint64_t written = 0;
    int rc = 0;

    for (;;) {
        size_t got = 0;
        while (got < stage_size) {
            long n = read(fd, stage + got, stage_size - got);
            if (n <= 0) {
                break;
            }
            got += (size_t)n;
        }
        if (got == 0) {
            break;
        }
        /* Round up to a cluster and pad: the tail of the last cluster
         * is zeros, not whatever the disk happened to hold. */
        size_t padded = ((got + cb - 1) / cb) * cb;
        memset(stage + got, 0, padded - got);

        uint32_t n_clusters = (uint32_t)(padded / cb);
        uint32_t run_start = 0;
        for (uint32_t i = 0; i < n_clusters; i++) {
            uint32_t cluster = cluster_alloc();
            if (cluster == 0) {
                printf("\n  %s: the disk is full\n", name);
                rc = -1;
                break;
            }
            if (i == 0) {
                run_start = cluster;
            }
            if (first == 0) {
                first = cluster;
            }
            if (prev != 0 && fat_set(prev, cluster) != 0) {
                rc = -1;
                break;
            }
            if (fat_set(cluster, 0x0FFFFFFF) != 0) {
                rc = -1;
                break;
            }
            prev = cluster;
        }
        if (rc != 0) {
            break;
        }
        if (dev_write(cluster_off(run_start), stage, padded) != 0) {
            rc = -1;
            break;
        }

        written += got;
        progress(label, written, (uint64_t)total);
        if (got < stage_size) {
            break; /* short read: end of file */
        }
    }

    free(stage);
    close(fd);
    if (rc != 0 || fat_flush() != 0) {
        return -1;
    }
    if (dir_add(dir, name, 0x20, first, (uint32_t)total) != 0) {
        return -1;
    }
    progress(label, (uint64_t)total, (uint64_t)total);
    printf("   ok\n");
    return 0;
}

/* Read a file back out of the filesystem and compare it with its
 * source: an install that says it worked should have looked. */
static int verify_file(uint32_t first_cluster, const char *src) {
    if (fat_flush() != 0) {
        return -1;
    }
    int fd = open(src, O_RDONLY);
    if (fd < 0) {
        return -1;
    }
    uint8_t *a = malloc(COPY_CHUNK);
    uint8_t *b = malloc(COPY_CHUNK);
    if (a == NULL || b == NULL) {
        free(a);
        free(b);
        close(fd);
        return -1;
    }

    uint32_t cluster = first_cluster;
    uint32_t cluster_bytes = g_fs.spc * SECTOR;
    int rc = 0;
    for (;;) {
        long n = read(fd, a, cluster_bytes > COPY_CHUNK ? COPY_CHUNK
                                                        : cluster_bytes);
        if (n <= 0) {
            break;
        }
        if (cluster < 2 || cluster >= g_fs.clusters + 2) {
            rc = -1;
            break;
        }
        if (dev_read(cluster_off(cluster), b, (size_t)n) != 0 ||
            memcmp(a, b, (size_t)n) != 0) {
            rc = -1;
            break;
        }
        /* Follow the chain through the first FAT. */
        uint8_t sector[SECTOR];
        uint32_t index = cluster * 4;
        uint64_t at = ((uint64_t)g_fs.part_start + RESERVED +
                       index / SECTOR) * SECTOR;
        if (dev_read(at, sector, SECTOR) != 0) {
            rc = -1;
            break;
        }
        uint32_t off = index % SECTOR;
        cluster = ((uint32_t)sector[off] | ((uint32_t)sector[off + 1] << 8) |
                   ((uint32_t)sector[off + 2] << 16) |
                   ((uint32_t)sector[off + 3] << 24)) & 0x0FFFFFFF;
        if (cluster >= 0x0FFFFFF8) {
            break; /* end of chain: the rest of the file must be gone */
        }
    }
    free(a);
    free(b);
    close(fd);
    return rc;
}

/* Write an in-memory buffer instead of streaming a source file - same
 * cluster-batching shape as copy_file() above, only the source
 * changes. This is how a rootfs.img customized in memory (see
 * "customizing the installed rootfs" below) ends up on disk without
 * ever being written back out to a real file first. */
static int copy_buffer(uint32_t dir, const char *name, const uint8_t *buf,
                       size_t total, const char *label) {
    uint32_t cb = g_fs.spc * SECTOR;
    uint32_t first = 0, prev = 0;
    uint64_t written = 0;
    int rc = 0;

    size_t off = 0;
    while (off < total) {
        size_t chunk = total - off < COPY_CHUNK ? total - off : COPY_CHUNK;
        size_t padded = ((chunk + cb - 1) / cb) * cb;

        uint8_t *stage = malloc(padded);
        if (stage == NULL) {
            return -1;
        }
        memcpy(stage, buf + off, chunk);
        memset(stage + chunk, 0, padded - chunk);

        uint32_t n_clusters = (uint32_t)(padded / cb);
        uint32_t run_start = 0;
        for (uint32_t i = 0; i < n_clusters; i++) {
            uint32_t cluster = cluster_alloc();
            if (cluster == 0) {
                printf("\n  %s: the disk is full\n", name);
                rc = -1;
                break;
            }
            if (i == 0) {
                run_start = cluster;
            }
            if (first == 0) {
                first = cluster;
            }
            if (prev != 0 && fat_set(prev, cluster) != 0) {
                rc = -1;
                break;
            }
            if (fat_set(cluster, 0x0FFFFFFF) != 0) {
                rc = -1;
                break;
            }
            prev = cluster;
        }
        if (rc == 0 && dev_write(cluster_off(run_start), stage, padded) != 0) {
            rc = -1;
        }
        free(stage);
        if (rc != 0) {
            break;
        }

        written += chunk;
        off += chunk;
        progress(label, written, (uint64_t)total);
    }

    if (rc != 0 || fat_flush() != 0) {
        return -1;
    }
    if (dir_add(dir, name, 0x20, first, (uint32_t)total) != 0) {
        return -1;
    }
    progress(label, (uint64_t)total, (uint64_t)total);
    printf("   ok\n");
    return 0;
}

/* verify_file()'s counterpart for an in-memory source. */
static int verify_buffer(uint32_t first_cluster, const uint8_t *buf,
                         size_t total) {
    if (fat_flush() != 0) {
        return -1;
    }
    uint8_t *b = malloc(COPY_CHUNK);
    if (b == NULL) {
        return -1;
    }
    uint32_t cluster = first_cluster;
    uint32_t cluster_bytes = g_fs.spc * SECTOR;
    size_t off = 0;
    int rc = 0;
    while (off < total) {
        size_t n = total - off < cluster_bytes ? total - off : cluster_bytes;
        n = n < COPY_CHUNK ? n : COPY_CHUNK;
        if (cluster < 2 || cluster >= g_fs.clusters + 2) {
            rc = -1;
            break;
        }
        if (dev_read(cluster_off(cluster), b, n) != 0 ||
            memcmp(buf + off, b, n) != 0) {
            rc = -1;
            break;
        }
        off += n;
        if (n < cluster_bytes) {
            break;
        }
        uint8_t sector[SECTOR];
        uint32_t index = cluster * 4;
        uint64_t at = ((uint64_t)g_fs.part_start + RESERVED +
                       index / SECTOR) * SECTOR;
        if (dev_read(at, sector, SECTOR) != 0) {
            rc = -1;
            break;
        }
        uint32_t soff = index % SECTOR;
        cluster = ((uint32_t)sector[soff] | ((uint32_t)sector[soff + 1] << 8) |
                   ((uint32_t)sector[soff + 2] << 16) |
                   ((uint32_t)sector[soff + 3] << 24)) & 0x0FFFFFFF;
        if (cluster >= 0x0FFFFFF8) {
            break;
        }
    }
    free(b);
    return (rc == 0 && off >= total) ? 0 : -1;
}

/*
 * ---- customizing the installed rootfs ----
 *
 * tusinstall used to copy /dev/rootfs (the in-memory ustar the running
 * system booted from) straight onto the disk - which is why every
 * install had root's password baked in at "toast". To let the
 * installer set a real root password and optionally add a user
 * account, specific members of that archive (etc/shadow, etc/passwd,
 * etc/group) have to change - and a tar member is a fixed-size,
 * contiguous run of 512-byte blocks, so "editing" one in place only
 * works if the replacement is exactly the same length. It essentially
 * never is (a SHA-512 hash and a new user's lines are not the same
 * size as what was there), so this rebuilds the whole archive into a
 * new buffer instead: every unmodified member's header and data are
 * copied byte for byte (untouched - same mode, uid, mtime, checksum,
 * everything), the three text members that changed get a header
 * reusing all their original fields except size/checksum
 * (recomputed) plus their new content, and - if a user account was
 * requested - one brand new directory entry for that account's home
 * is appended before the end-of-archive marker.
 *
 * ustar layout matches kernel/vfs/rootfs.c's own comment exactly
 * (that file is the authority on what TUS's parser actually expects);
 * this only has to also be valid enough for a real `tar` to read, in
 * case anyone ever pulls the installed image apart to look at it.
 */

#define TAR_NAME_OFF   0
#define TAR_NAME_LEN   100
#define TAR_MODE_OFF   100
#define TAR_MODE_LEN   8
#define TAR_UID_OFF    108
#define TAR_GID_OFF    116
#define TAR_SIZE_OFF   124
#define TAR_SIZE_LEN   12
#define TAR_MTIME_OFF  136
#define TAR_MTIME_LEN  12
#define TAR_CHKSUM_OFF 148
#define TAR_TYPE_OFF   156
#define TAR_MAGIC_OFF  257

static uint64_t tar_octal(const uint8_t *field, size_t len) {
    uint64_t v = 0;
    for (size_t i = 0; i < len; i++) {
        char c = (char)field[i];
        if (c >= '0' && c <= '7') {
            v = (v << 3) | (uint64_t)(c - '0');
        } else if (c == ' ' || c == '\0') {
            continue;
        } else {
            break;
        }
    }
    return v;
}

/* Right-justified octal, leading zeros, NUL-terminated in the last
 * byte of the field - the standard ustar numeric field encoding. */
static void tar_put_octal(uint8_t *field, size_t len, uint64_t v) {
    field[len - 1] = '\0';
    for (size_t i = len - 1; i-- > 0; ) {
        field[i] = (uint8_t)('0' + (v & 7));
        v >>= 3;
    }
}

/* Sum of every header byte with the checksum field itself treated as
 * eight spaces, written back as "NNNNNN\0 " (6 octal digits, NUL,
 * space) - the one field ustar readers disagree on the padding of,
 * which is why this exact shape (not a plain tar_put_octal) is what
 * every real tar implementation actually emits. */
static void tar_set_checksum(uint8_t *header) {
    memset(header + TAR_CHKSUM_OFF, ' ', 8);
    uint32_t sum = 0;
    for (int i = 0; i < TAR_BLOCK; i++) {
        sum += header[i];
    }
    for (int i = 5; i >= 0; i--) {
        header[TAR_CHKSUM_OFF + i] = (uint8_t)('0' + (sum & 7));
        sum >>= 3;
    }
    header[TAR_CHKSUM_OFF + 6] = '\0';
    header[TAR_CHKSUM_OFF + 7] = ' ';
}

/* Build a brand new header (the new user's home directory - nothing
 * else this file adds is a new tar member, just new content for
 * three that already exist). */
static void tar_make_dir_header(uint8_t *header, const char *name,
                                uint32_t mode) {
    memset(header, 0, TAR_BLOCK);
    snprintf((char *)header + TAR_NAME_OFF, TAR_NAME_LEN, "%s", name);
    tar_put_octal(header + TAR_MODE_OFF, TAR_MODE_LEN, mode);
    tar_put_octal(header + TAR_UID_OFF, 8, 0);
    tar_put_octal(header + TAR_GID_OFF, 8, 0);
    tar_put_octal(header + TAR_SIZE_OFF, TAR_SIZE_LEN, 0);
    tar_put_octal(header + TAR_MTIME_OFF, TAR_MTIME_LEN, 0);
    header[TAR_TYPE_OFF] = '5'; /* directory */
    memcpy(header + TAR_MAGIC_OFF, "ustar", 6);
    header[TAR_MAGIC_OFF + 6] = '0';
    header[TAR_MAGIC_OFF + 7] = '0';
    snprintf((char *)header + TAR_MAGIC_OFF + 8, 32, "root");   /* uname */
    snprintf((char *)header + TAR_MAGIC_OFF + 8 + 32, 32, "root"); /* gname */
    tar_set_checksum(header);
}

/* Same shape as tar_make_dir_header(), for a small regular file added
 * whole (the /etc/sshd.enable boot-service flag - see the file
 * comment at build_custom_rootfs() and kernel/main.c's
 * load_boot_services(), which is what actually reads it). */
static void tar_make_file_header(uint8_t *header, const char *name,
                                 uint32_t mode, size_t size) {
    memset(header, 0, TAR_BLOCK);
    snprintf((char *)header + TAR_NAME_OFF, TAR_NAME_LEN, "%s", name);
    tar_put_octal(header + TAR_MODE_OFF, TAR_MODE_LEN, mode);
    tar_put_octal(header + TAR_UID_OFF, 8, 0);
    tar_put_octal(header + TAR_GID_OFF, 8, 0);
    tar_put_octal(header + TAR_SIZE_OFF, TAR_SIZE_LEN, size);
    tar_put_octal(header + TAR_MTIME_OFF, TAR_MTIME_LEN, 0);
    header[TAR_TYPE_OFF] = '0'; /* regular file */
    memcpy(header + TAR_MAGIC_OFF, "ustar", 6);
    header[TAR_MAGIC_OFF + 6] = '0';
    header[TAR_MAGIC_OFF + 7] = '0';
    snprintf((char *)header + TAR_MAGIC_OFF + 8, 32, "root");
    snprintf((char *)header + TAR_MAGIC_OFF + 8 + 32, 32, "root");
    tar_set_checksum(header);
}

/* Append `text` (already newline-terminated) to a growable buffer,
 * reallocating as needed - the simplest possible line editor for the
 * three small text files this touches. */
struct grow_buf {
    char *data;
    size_t len;
    size_t cap;
};

static int grow_buf_append(struct grow_buf *g, const char *text, size_t n) {
    if (g->len + n + 1 > g->cap) {
        size_t newcap = g->cap == 0 ? 4096 : g->cap * 2;
        while (newcap < g->len + n + 1) {
            newcap *= 2;
        }
        char *p = realloc(g->data, newcap);
        if (p == NULL) {
            return -1;
        }
        g->data = p;
        g->cap = newcap;
    }
    memcpy(g->data + g->len, text, n);
    g->len += n;
    g->data[g->len] = '\0';
    return 0;
}

/* Header + content + zero padding to the next TAR_BLOCK boundary, in
 * one call - the three-step dance every new file member needs
 * (sshd.enable, and now the two host key files) written once. */
static int append_file_member(struct grow_buf *out, const char *name,
                              uint32_t mode, const void *content,
                              size_t len) {
    uint8_t header[TAR_BLOCK];
    tar_make_file_header(header, name, mode, len);
    if (grow_buf_append(out, (const char *)header, TAR_BLOCK) != 0) {
        return -1;
    }
    if (len > 0 && grow_buf_append(out, content, len) != 0) {
        return -1;
    }
    size_t pad = TAR_BLOCK - (len % TAR_BLOCK);
    if (pad == TAR_BLOCK) pad = 0;
    static const uint8_t zeropad[TAR_BLOCK];
    if (pad > 0 && grow_buf_append(out, (const char *)zeropad, pad) != 0) {
        return -1;
    }
    return 0;
}

/* Rebuild etc/shadow's content: copy every line through unchanged
 * except root's (replaced with the new hash), then optionally append
 * one line for a newly created user. Mirrors passwd.c's own shadow
 * line shape exactly (name:hash:0:0:99999:7::) so the account behaves
 * like any other TUS account afterward - `passwd` on it later works
 * the same as it would for any useradd-created one. */
static int rebuild_shadow(const char *orig, size_t orig_len,
                          const char *root_hash, const char *new_user,
                          const char *new_user_hash, struct grow_buf *out) {
    size_t i = 0;
    while (i < orig_len) {
        size_t start = i;
        while (i < orig_len && orig[i] != '\n') {
            i++;
        }
        size_t linelen = i - start;
        if (i < orig_len) {
            i++; /* consume the newline */
        }
        const char *colon = memchr(orig + start, ':', linelen);
        int is_root = colon != NULL &&
                      (size_t)(colon - (orig + start)) == 4 &&
                      memcmp(orig + start, "root", 4) == 0;
        if (is_root && root_hash != NULL) {
            char line[LINE_MAX_TUS];
            snprintf(line, sizeof(line), "root:%s:0:0:99999:7::\n", root_hash);
            if (grow_buf_append(out, line, strlen(line)) != 0) {
                return -1;
            }
        } else {
            if (grow_buf_append(out, orig + start, linelen) != 0) {
                return -1;
            }
            if (linelen == 0 || orig[start + linelen - 1] != '\n') {
                if (grow_buf_append(out, "\n", 1) != 0) {
                    return -1;
                }
            }
        }
    }
    if (new_user != NULL) {
        char line[LINE_MAX_TUS];
        snprintf(line, sizeof(line), "%s:%s:0:0:99999:7::\n", new_user,
                 new_user_hash);
        if (grow_buf_append(out, line, strlen(line)) != 0) {
            return -1;
        }
    }
    return 0;
}

/* etc/passwd and etc/group both just need one line appended (nothing
 * in either needs editing, unlike shadow) - same field shapes
 * useradd.c produces: "name:x:uid:gid:comment:home:shell" and
 * "name:x:gid:". */
static int append_line(const char *orig, size_t orig_len, const char *line,
                       struct grow_buf *out) {
    if (grow_buf_append(out, orig, orig_len) != 0) {
        return -1;
    }
    if (line != NULL) {
        return grow_buf_append(out, line, strlen(line));
    }
    return 0;
}

/* Slurp a whole source into one malloc'd buffer - /dev/rootfs is a
 * device (the boot module Limine loaded), not a disk, but it answers
 * lseek(SEEK_END) the same way copy_file() already relies on above. */
static uint8_t *read_all(const char *path, size_t *out_len) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }
    long total = lseek(fd, 0, SEEK_END);
    if (total <= 0) {
        close(fd);
        return NULL;
    }
    lseek(fd, 0, SEEK_SET);
    uint8_t *buf = malloc((size_t)total);
    if (buf == NULL) {
        close(fd);
        return NULL;
    }
    size_t got = 0;
    while (got < (size_t)total) {
        long n = read(fd, buf + got, (size_t)total - got);
        if (n <= 0) {
            free(buf);
            close(fd);
            return NULL;
        }
        got += (size_t)n;
    }
    close(fd);
    *out_len = got;
    return buf;
}

/* Generate a fresh Ed25519 host keypair by spawning /bin/ssh-keygen
 * (real fork()+execve(), same as a shell would) into two temp files,
 * then read both back into malloc'd buffers via read_all() above.
 * This is the ONLY place a TUS sshd host key is meant to come from -
 * see sshd.c's ensure_host_key(), whose own generation is a
 * documented last-resort fallback for boots with no persistent
 * storage to bake one into at all (a live CD). Reusing ssh-keygen
 * here instead of duplicating Ed25519 keygen and OpenSSH's private
 * key file format a second time in this file. */
static int generate_ssh_host_key(uint8_t **priv, size_t *priv_len,
                                 uint8_t **pub, size_t *pub_len) {
    static const char *tmp_priv = "/tmp/tusinstall_host_key";
    static const char *tmp_pub = "/tmp/tusinstall_host_key.pub";

    /* ssh-keygen (ssh_keygen.c) refuses to overwrite an existing
     * file - clear out anything left from an earlier, abandoned
     * tusinstall run in this same session before spawning it. */
    unlink(tmp_priv);
    unlink(tmp_pub);

    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    }
    if (pid == 0) {
        char *argv[] = {(char *)"/bin/ssh-keygen", (char *)"-f",
                        (char *)tmp_priv, NULL};
        char *envp[] = {(char *)"PATH=/bin:/usr/bin", (char *)"HOME=/root",
                        NULL};
        execve("/bin/ssh-keygen", argv, envp);
        _exit(127);
    }
    int status = 0;
    if (waitpid(pid, &status, 0) != pid || !WIFEXITED(status) ||
        WEXITSTATUS(status) != 0) {
        unlink(tmp_priv);
        unlink(tmp_pub);
        return -1;
    }

    size_t plen = 0, publen = 0;
    uint8_t *pdata = read_all(tmp_priv, &plen);
    uint8_t *pubdata = pdata != NULL ? read_all(tmp_pub, &publen) : NULL;
    unlink(tmp_priv);
    unlink(tmp_pub);
    if (pdata == NULL || pubdata == NULL) {
        free(pdata);
        free(pubdata);
        return -1;
    }

    *priv = pdata;
    *priv_len = plen;
    *pub = pubdata;
    *pub_len = publen;
    return 0;
}

/* Rebuild /dev/rootfs's ustar archive with root's password hash
 * replaced and (optionally) one new user account added. Returns a
 * malloc'd buffer and its length via *out_len, or NULL on failure -
 * every unmodified member is copied byte for byte, only
 * etc/shadow, etc/passwd and etc/group change, and one new directory
 * entry is appended for the new account's home if there is one. */
static uint8_t *build_custom_rootfs(const char *root_hash,
                                    const char *new_user,
                                    const char *new_user_hash,
                                    int enable_sshd,
                                    size_t *out_len) {
    size_t orig_len = 0;
    uint8_t *orig = read_all(SRC_ROOTFS, &orig_len);
    if (orig == NULL || orig_len < TAR_BLOCK * 2) {
        free(orig);
        return NULL;
    }

    struct grow_buf out;
    memset(&out, 0, sizeof(out));
    out.cap = orig_len + 65536; /* generous headroom, grows anyway */
    out.data = malloc(out.cap);
    if (out.data == NULL) {
        free(orig);
        return NULL;
    }

    size_t p = 0;
    while (p + TAR_BLOCK <= orig_len) {
        int zero = 1;
        for (int i = 0; i < TAR_BLOCK; i++) {
            if (orig[p + i] != 0) {
                zero = 0;
                break;
            }
        }
        if (zero) {
            break; /* end of archive */
        }

        uint8_t *header = orig + p;
        uint64_t size = tar_octal(header + TAR_SIZE_OFF, TAR_SIZE_LEN);
        size_t padded = (size_t)(((size + TAR_BLOCK - 1) / TAR_BLOCK) * TAR_BLOCK);
        const char *data = (const char *)(header + TAR_BLOCK);

        /* Names in the archive carry the "./" prefix (see
         * kernel/vfs/rootfs.c's tar_normalise, which strips it the
         * same way) - compare past it so "etc/shadow" matches
         * "./etc/shadow". */
        const char *name = (const char *)header;
        if (name[0] == '.' && name[1] == '/') {
            name += 2;
        }

        int is_shadow = strcmp(name, "etc/shadow") == 0;
        int is_passwd = strcmp(name, "etc/passwd") == 0;
        int is_group = strcmp(name, "etc/group") == 0;

        if (is_shadow || is_passwd || is_group) {
            struct grow_buf content;
            memset(&content, 0, sizeof(content));
            int rc;
            if (is_shadow) {
                rc = rebuild_shadow(data, (size_t)size, root_hash, new_user,
                                    new_user_hash, &content);
            } else if (is_passwd) {
                char line[LINE_MAX_TUS];
                line[0] = '\0';
                if (new_user != NULL) {
                    snprintf(line, sizeof(line),
                             "%s:x:1000:1000::/home/%s:/bin/tsh\n",
                             new_user, new_user);
                }
                rc = append_line(data, (size_t)size,
                                 new_user != NULL ? line : NULL, &content);
            } else {
                char line[LINE_MAX_TUS];
                line[0] = '\0';
                if (new_user != NULL) {
                    snprintf(line, sizeof(line), "%s:x:1000:\n", new_user);
                }
                rc = append_line(data, (size_t)size,
                                 new_user != NULL ? line : NULL, &content);
            }
            if (rc != 0) {
                free(content.data);
                free(out.data);
                free(orig);
                return NULL;
            }

            uint8_t newheader[TAR_BLOCK];
            memcpy(newheader, header, TAR_BLOCK);
            tar_put_octal(newheader + TAR_SIZE_OFF, TAR_SIZE_LEN, content.len);
            tar_set_checksum(newheader);
            size_t newpadded =
                ((content.len + TAR_BLOCK - 1) / TAR_BLOCK) * TAR_BLOCK;

            if (grow_buf_append(&out, (char *)newheader, TAR_BLOCK) != 0) {
                free(content.data);
                free(out.data);
                free(orig);
                return NULL;
            }
            /* Pad content to a block boundary with zeros, same as the
             * original archive - grow_buf_append the real bytes, then
             * the pad separately since content.data isn't pre-padded. */
            if (grow_buf_append(&out, content.data, content.len) != 0) {
                free(content.data);
                free(out.data);
                free(orig);
                return NULL;
            }
            static const uint8_t zeros[TAR_BLOCK];
            size_t padlen = newpadded - content.len;
            if (padlen > 0 &&
                grow_buf_append(&out, (const char *)zeros, padlen) != 0) {
                free(content.data);
                free(out.data);
                free(orig);
                return NULL;
            }
            free(content.data);
        } else {
            if (grow_buf_append(&out, (const char *)header,
                                TAR_BLOCK + padded) != 0) {
                free(out.data);
                free(orig);
                return NULL;
            }
        }

        p += TAR_BLOCK + padded;
    }
    free(orig);

    if (new_user != NULL) {
        /* vfs_create_dir() (kernel/vfs/vfs.c) requires the parent to
         * already exist - it looks it up and fails (silently, per
         * rootfs.c's mount loop, which does not check) if not. The
         * base image ships no /home at all, so without this entry
         * "home/<user>/" mounts nowhere and the account's home
         * directory just never appears. */
        uint8_t homedirheader[TAR_BLOCK];
        tar_make_dir_header(homedirheader, "home/", 0755);
        if (grow_buf_append(&out, (const char *)homedirheader, TAR_BLOCK) !=
            0) {
            free(out.data);
            return NULL;
        }

        char homename[128];
        snprintf(homename, sizeof(homename), "home/%s/", new_user);
        uint8_t dirheader[TAR_BLOCK];
        tar_make_dir_header(dirheader, homename, 0700);
        if (grow_buf_append(&out, (const char *)dirheader, TAR_BLOCK) != 0) {
            free(out.data);
            return NULL;
        }
    }

    if (enable_sshd) {
        /* Presence is the whole flag - kernel/main.c's
         * load_boot_services() only checks that the file opens, never
         * reads its content. "etc/" already exists as a directory in
         * the base image (it holds passwd/shadow/... already), unlike
         * "home/" above, so no parent entry is needed for the flag
         * file itself. */
        static const char flag_content[] = "enabled\n";
        if (append_file_member(&out, "etc/sshd.enable", 0644, flag_content,
                               sizeof(flag_content) - 1) != 0) {
            free(out.data);
            return NULL;
        }

        /* The host key is what makes sshd's identity persist across
         * reboots at all: TUS has no write-through to disk anywhere
         * (every vfs_create_file() (kernel/vfs/vfs.c) call only
         * touches an in-memory node, and rootfs.c reparses the same
         * static rootfs.img fresh on every boot), so a key generated
         * at runtime by sshd itself lives only as long as that one
         * boot. Baking it in here, the same way the root password and
         * any new account are baked in above, is the only way an
         * installed machine's host identity is stable from one boot
         * to the next. "etc/ssh/" does not exist in the base image
         * (only "etc/" does), so it needs its own directory entry -
         * the same class of fix as "home/" above. */
        uint8_t *hostpriv = NULL, *hostpub = NULL;
        size_t hostpriv_len = 0, hostpub_len = 0;
        if (generate_ssh_host_key(&hostpriv, &hostpriv_len, &hostpub,
                                  &hostpub_len) != 0) {
            free(out.data);
            return NULL;
        }

        uint8_t sshdirheader[TAR_BLOCK];
        tar_make_dir_header(sshdirheader, "etc/ssh/", 0755);
        if (grow_buf_append(&out, (const char *)sshdirheader, TAR_BLOCK) !=
            0) {
            free(hostpriv);
            free(hostpub);
            free(out.data);
            return NULL;
        }

        /* 0600: the same private-key permission ssh-keygen itself
         * writes, and what sshd's own ensure_host_key() checks for
         * (ssh_key_read_private_file() in sshkey.c) before it will
         * trust a key file on disk. */
        int priv_ok = append_file_member(&out, "etc/ssh/ssh_host_ed25519_key",
                                         0600, hostpriv, hostpriv_len) == 0;
        int pub_ok =
            priv_ok && append_file_member(&out,
                                          "etc/ssh/ssh_host_ed25519_key.pub",
                                          0644, hostpub, hostpub_len) == 0;
        free(hostpriv);
        free(hostpub);
        if (!priv_ok || !pub_ok) {
            free(out.data);
            return NULL;
        }
    }

    static const uint8_t end_marker[TAR_BLOCK * 2];
    if (grow_buf_append(&out, (const char *)end_marker, TAR_BLOCK * 2) != 0) {
        free(out.data);
        return NULL;
    }

    *out_len = out.len;
    return (uint8_t *)out.data;
}

/* ---- asking for the root password and an optional user account ---- */

/* Same shape as passwd.c's read_line(): open the console tty
 * directly (fd 0 is already it, but this mirrors the real tool
 * rather than assuming), disable ECHO for the duration, restore it
 * after. tusinstall's own ask() above is deliberately the visible
 * variant (disk names, yes/no); this is only for secrets. */
static int ask_secret(const char *prompt, char *out, size_t outsz) {
    struct termios tio, orig;
    int have_tio = tcgetattr(0, &orig) == 0;
    if (have_tio) {
        tio = orig;
        tio.c_lflag &= ~(tcflag_t)ECHO;
        tcsetattr(0, TCSAFLUSH, &tio);
    }
    printf("%s", prompt);
    fflush(stdout);
    size_t n = 0;
    char c;
    while (n + 1 < outsz && read(0, &c, 1) == 1) {
        if (c == '\n' || c == '\r') {
            break;
        }
        out[n++] = c;
    }
    out[n] = '\0';
    if (have_tio) {
        tcsetattr(0, TCSAFLUSH, &orig);
    }
    printf("\n");
    return (int)n;
}

/* SHA-512 ($6$), same recipe as passwd.c and useradd.c: a salt seeded
 * from the wall clock plus two random characters. Returns 0 and fills
 * `out` on success, -1 if crypt() itself failed (out gets "!", a
 * locked-account hash, so a crypt() failure can never silently ship
 * an account nothing can authenticate against but also isn't locked). */
static int hash_password(const char *plain, char *out, size_t outsz) {
    char salt[32];
    snprintf(salt, sizeof(salt), "$6$%ld%c%c$", (long)time(NULL),
             'a' + (rand() % 26), '0' + (rand() % 10));
    char *h = crypt(plain, salt);
    snprintf(out, outsz, "%s", h != NULL ? h : "!");
    return h != NULL ? 0 : -1;
}

/* ---- laying out the disk ---- */

/* Writes the ESP's partition entry, and - when wrf_sectors is nonzero
 * - a second entry for a WRF partition right after it, spanning
 * wrf_start..wrf_start+wrf_sectors. A disk too small to spare room
 * for WRF (see main()) gets wrf_sectors == 0 and only the one
 * partition, exactly as before WRF existed - the installed system
 * still boots fine, it just has no /mnt to persist anything in. */
static int write_mbr(uint32_t part_sectors, uint32_t wrf_start, uint32_t wrf_sectors) {
    uint8_t mbr[SECTOR];
    memset(mbr, 0, sizeof(mbr));

    /* No boot code: the firmware that boots this disk is UEFI, and it
     * reads the partition table, not sector 0's instructions. */
    uint8_t *e = mbr + 446;
    e[0] = 0x80;                    /* bootable */
    e[1] = 0xFE;                    /* CHS: the "use LBA" filler */
    e[2] = 0xFF;
    e[3] = 0xFF;
    e[4] = 0xEF;                    /* EFI System Partition */
    e[5] = 0xFE;
    e[6] = 0xFF;
    e[7] = 0xFF;
    put32(e + 8, PART_START);
    put32(e + 12, part_sectors);

    if (wrf_sectors != 0) {
        uint8_t *e2 = mbr + 446 + 16;
        e2[0] = 0x00;                 /* not the boot partition */
        e2[1] = 0xFE;
        e2[2] = 0xFF;
        e2[3] = 0xFF;
        e2[4] = WRF_PART_TYPE;
        e2[5] = 0xFE;
        e2[6] = 0xFF;
        e2[7] = 0xFF;
        put32(e2 + 8, wrf_start);
        put32(e2 + 12, wrf_sectors);
    }

    mbr[510] = 0x55;
    mbr[511] = 0xAA;
    return dev_write(0, mbr, SECTOR);
}

/* Formats the second partition (start_lba..+part_sectors) as WRF -
 * the same on-disk result `mkfs.wrf` produces, written here instead
 * through dev_write() at disk-absolute byte offsets rather than
 * through a separate program and a partition-level device node
 * (neither exists in TUS - a disk is one byte stream end to end, see
 * kernel/vfs/devices.c). wrf_compute_layout() (include/wrf.h) does
 * the actual sizing math, shared with mkfs_wrf.c; every field it
 * fills in is relative to start_lba, exactly what kernel/fs/wrf.c
 * expects to find in a WRF superblock that does not own the whole
 * disk (see its wrf_boot_mount()). Returns 0, or -1 on any write
 * failure or if part_sectors is too small for even the smallest WRF
 * volume. */
static int format_wrf_partition(uint32_t start_lba, uint32_t part_sectors) {
    struct wrf_superblock sb;
    if (wrf_compute_layout(part_sectors, &sb) != 0) {
        return -1;
    }

    uint64_t base = (uint64_t)start_lba * SECTOR;
    if (dev_write(base, &sb, sizeof(sb)) != 0) {
        return -1;
    }

    /* Zero every metadata sector: see mkfs_wrf.c's matching comment -
     * an all-zero inode bitmap and block bitmap mean "everything
     * free," and an all-zero inode table means "every inode's mode is
     * 0," inert for both the reserved inode 0 and every inode not yet
     * allocated. */
    uint8_t zero[SECTOR];
    memset(zero, 0, sizeof(zero));
    uint32_t meta_sectors = sb.inode_bitmap_blocks + sb.inode_table_blocks +
                             sb.block_bitmap_blocks;
    for (uint32_t i = 0; i < meta_sectors; i++) {
        if (dev_write(base + (uint64_t)(1 + i) * SECTOR, zero, SECTOR) != 0) {
            return -1;
        }
    }

    /* Inode 0 (reserved) and inode 1 (root) both start allocated. */
    uint8_t ibmap0[SECTOR];
    memset(ibmap0, 0, sizeof(ibmap0));
    ibmap0[0] = 0x03;
    if (dev_write(base + (uint64_t)sb.inode_bitmap_lba * SECTOR, ibmap0, SECTOR) != 0) {
        return -1;
    }

    /* The root directory: an empty WRF_IFDIR inode, in the inode
     * table's first sector at slot WRF_ROOT_INO (slot 0 is inode 0,
     * left all-zero - reserved means never read). */
    struct wrf_inode root;
    memset(&root, 0, sizeof(root));
    root.mode = WRF_IFDIR | 0755;
    root.nlink = 1;
    uint8_t first_inode_sector[SECTOR];
    memset(first_inode_sector, 0, sizeof(first_inode_sector));
    memcpy(first_inode_sector + WRF_ROOT_INO * sizeof(root), &root, sizeof(root));
    if (dev_write(base + (uint64_t)sb.inode_table_lba * SECTOR, first_inode_sector,
                  SECTOR) != 0) {
        return -1;
    }
    return 0;
}

/* Sectors per cluster, the way every mkfs picks it: by disk size,
 * then checked against FAT32's lower limit on the cluster count. */
static uint32_t pick_spc(uint32_t part_sectors) {
    uint32_t mib = part_sectors / 2048;
    uint32_t spc = mib <= 260 ? 1 : mib <= 8192 ? 8 : mib <= 16384 ? 16
                 : mib <= 32768 ? 32 : 64;
    while (spc > 1) {
        uint32_t divisor = spc * (SECTOR / 4) + NUM_FATS;
        uint32_t fat = (part_sectors - RESERVED + divisor - 1) / divisor;
        uint32_t clusters =
            (part_sectors - RESERVED - NUM_FATS * fat) / spc;
        if (clusters >= 65525) {
            break;
        }
        spc /= 2;
    }
    return spc;
}

static int mkfs(uint32_t part_sectors) {
    memset(&g_fs, 0, sizeof(g_fs));
    g_fs.part_start = PART_START;
    g_fs.part_sectors = part_sectors;
    g_fs.spc = pick_spc(part_sectors);

    uint32_t divisor = g_fs.spc * (SECTOR / 4) + NUM_FATS;
    g_fs.fat_size = (part_sectors - RESERVED + divisor - 1) / divisor;
    g_fs.data_start = PART_START + RESERVED + NUM_FATS * g_fs.fat_size;
    g_fs.clusters = (part_sectors - RESERVED - NUM_FATS * g_fs.fat_size) /
                    g_fs.spc;
    g_fs.next_free = ROOT_CLUSTER;

    if (g_fs.clusters < 65525) {
        printf("  the partition is too small for FAT32 "
               "(needs about 40 MiB)\n");
        return -1;
    }

    /* The boot sector: a BPB with a jump instruction in front of it,
     * because that is what "a FAT filesystem" means to firmware. */
    uint8_t bs[SECTOR];
    memset(bs, 0, sizeof(bs));
    bs[0] = 0xEB;
    bs[1] = 0x58;
    bs[2] = 0x90;
    memcpy(bs + 3, "TUS  1.0", 8);
    put16(bs + 11, SECTOR);
    bs[13] = (uint8_t)g_fs.spc;
    put16(bs + 14, RESERVED);
    bs[16] = NUM_FATS;
    put16(bs + 17, 0);              /* root entries: FAT32 has none */
    put16(bs + 19, 0);              /* small total sectors: unused */
    bs[21] = 0xF8;                  /* fixed disk */
    put16(bs + 22, 0);              /* FAT size 16: unused */
    put16(bs + 24, 32);             /* sectors per track */
    put16(bs + 26, 8);              /* heads */
    put32(bs + 28, PART_START);     /* hidden sectors */
    put32(bs + 32, part_sectors);
    put32(bs + 36, g_fs.fat_size);
    put16(bs + 40, 0);              /* flags: both FATs live */
    put16(bs + 42, 0);              /* version 0.0 */
    put32(bs + 44, ROOT_CLUSTER);
    put16(bs + 48, 1);              /* FSInfo sector */
    put16(bs + 50, 6);              /* backup boot sector */
    bs[64] = 0x80;                  /* BIOS drive number */
    bs[66] = 0x29;                  /* extended boot signature */
    put32(bs + 67, 0x54555300);     /* volume id */
    memcpy(bs + 71, "TUS        ", 11);
    memcpy(bs + 82, "FAT32   ", 8);
    put16(bs + 510, 0xAA55);
    if (dev_write((uint64_t)PART_START * SECTOR, bs, SECTOR) != 0 ||
        dev_write((uint64_t)(PART_START + 6) * SECTOR, bs, SECTOR) != 0) {
        return -1;
    }

    /* FSInfo: two signatures, a free-cluster hint and a third. */
    uint8_t fsi[SECTOR];
    memset(fsi, 0, sizeof(fsi));
    put32(fsi + 0, 0x41615252);
    put32(fsi + 484, 0x61417272);
    put32(fsi + 488, 0xFFFFFFFF);   /* free count: unknown */
    put32(fsi + 492, 0xFFFFFFFF);   /* next free: unknown */
    put16(fsi + 510, 0xAA55);
    if (dev_write((uint64_t)(PART_START + 1) * SECTOR, fsi, SECTOR) != 0 ||
        dev_write((uint64_t)(PART_START + 7) * SECTOR, fsi, SECTOR) != 0) {
        return -1;
    }

    /* Both FATs, zeroed. This is the only part of the install that
     * writes a lot of nothing, so it is the only part with a bar. */
    uint8_t *zeros = calloc(1, COPY_CHUNK);
    if (zeros == NULL) {
        return -1;
    }
    uint64_t fat_bytes = (uint64_t)g_fs.fat_size * NUM_FATS * SECTOR;
    uint64_t at = (uint64_t)(PART_START + RESERVED) * SECTOR;
    for (uint64_t done = 0; done < fat_bytes; done += COPY_CHUNK) {
        size_t n = fat_bytes - done < COPY_CHUNK
                       ? (size_t)(fat_bytes - done) : COPY_CHUNK;
        if (dev_write(at + done, zeros, n) != 0) {
            free(zeros);
            return -1;
        }
        progress("formatting", done + n, fat_bytes);
    }
    free(zeros);
    progress("formatting", fat_bytes, fat_bytes);
    printf("   ok\n");

    /* Cluster 0 and 1 are the media descriptor and the end marker;
     * cluster 2 is the root directory, which starts out empty. */
    g_fs.next_free = ROOT_CLUSTER;
    uint32_t root = cluster_alloc();
    if (root != ROOT_CLUSTER || cluster_clear(root) != 0) {
        return -1;
    }
    if (fat_set(0, 0x0FFFFFF8) != 0 || fat_set(1, 0x0FFFFFFF) != 0 ||
        fat_set(ROOT_CLUSTER, 0x0FFFFFFF) != 0 || fat_flush() != 0) {
        return -1;
    }
    return 0;
}

/* ---- asking ---- */

static void ask(const char *prompt, const char *def, char *out, size_t outsz) {
    printf("%s", prompt);
    fflush(stdout);
    size_t n = 0;
    char c;
    while (n + 1 < outsz && read(0, &c, 1) == 1) {
        if (c == '\n' || c == '\r') {
            break;
        }
        if (c == '\b' || c == 0x7F) {
            if (n > 0) {
                n--;
                /* The tty echoes printable bytes only, so the
                 * erasing is this program's to do. */
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }
        out[n++] = c;
    }
    out[n] = '\0';
    if (n == 0 && def != NULL) {
        snprintf(out, outsz, "%s", def);
    }
}

/* ---- the installer ---- */

struct disk_choice {
    char path[32];
    char name[16];
    uint64_t bytes;
};

static int list_disks(struct disk_choice *out, int max) {
    static const char *names[] = { "hda", "hdb", "hdc", "hdd" };
    int n = 0;
    for (unsigned i = 0; i < sizeof(names) / sizeof(names[0]) && n < max; i++) {
        char path[32];
        snprintf(path, sizeof(path), "/dev/%s", names[i]);
        int fd = open(path, O_RDONLY);
        if (fd < 0) {
            continue;
        }
        long size = lseek(fd, 0, SEEK_END);
        close(fd);
        if (size <= 0) {
            continue;
        }
        snprintf(out[n].path, sizeof(out[n].path), "%s", path);
        snprintf(out[n].name, sizeof(out[n].name), "%s", names[i]);
        out[n].bytes = (uint64_t)size;
        n++;
    }
    return n;
}

static const char CONGRATS[] =
"\nCONGRATULATIONS! Your TUS install has been successfully completed!\n"
"\n"
"When you login to your new system the first time, please read your mail\n"
"using the 'mail' command.\n"
"\n";

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    printf("\nWelcome to the TUS installer.\n\n");
    printf("It will put a bootable copy of the running system on a disk:\n");
    printf("an EFI system partition with the bootloader, this kernel and\n");
    printf("this root filesystem in it, plus a WRF partition (see wrf.h) for\n");
    printf("everything else - /mnt, where a write actually persists across a\n");
    printf("reboot, unlike the rest of the system. Everything on that disk is\n");
    printf("lost.\n\n");

    struct disk_choice disks[4];
    int ndisks = list_disks(disks, 4);
    if (ndisks == 0) {
        printf("No disks found. TUS needs one to install onto - start the\n");
        printf("machine with a hard disk attached (qemu: -drive "
               "file=disk.img,format=raw,if=ide).\n");
        return 1;
    }

    printf("Available disks:\n");
    for (int i = 0; i < ndisks; i++) {
        printf("  %-4s  %llu MiB\n", disks[i].name,
               (unsigned long long)(disks[i].bytes / (1024 * 1024)));
    }
    printf("\n");

    char answer[64];
    char prompt[64];
    snprintf(prompt, sizeof(prompt), "Which disk is the root disk? [%s] ",
             disks[0].name);
    ask(prompt, disks[0].name, answer, sizeof(answer));

    struct disk_choice *chosen = NULL;
    for (int i = 0; i < ndisks; i++) {
        if (strcmp(answer, disks[i].name) == 0 ||
            strcmp(answer, disks[i].path) == 0) {
            chosen = &disks[i];
        }
    }
    if (chosen == NULL) {
        printf("%s: no such disk\n", answer);
        return 1;
    }

    printf("\nWARNING: everything on %s will be erased.\n", chosen->name);
    ask("Are you sure? [no] ", "no", answer, sizeof(answer));
    if (strcmp(answer, "yes") != 0 && strcmp(answer, "y") != 0) {
        printf("Nothing was written.\n");
        return 1;
    }

    /* Root's password and an optional user account - the running
     * system's own /etc/shadow (root:...:toast) would otherwise be
     * baked into the disk verbatim, so this is the last chance to
     * change it before the copy that does that. Blank input keeps
     * whatever hash is already there rather than forcing a choice. */
    char root_hash_buf[128];
    const char *root_hash = NULL;
    printf("\n");
    for (;;) {
        char p1[128], p2[128];
        ask_secret("Set root password (blank to keep the current one): ",
                   p1, sizeof(p1));
        if (p1[0] == '\0') {
            printf("Keeping the current root password.\n");
            break;
        }
        ask_secret("Retype root password: ", p2, sizeof(p2));
        if (strcmp(p1, p2) != 0) {
            printf("Passwords did not match. Try again.\n");
            continue;
        }
        if (hash_password(p1, root_hash_buf, sizeof(root_hash_buf)) != 0) {
            printf("Could not hash that password. Try again.\n");
            continue;
        }
        root_hash = root_hash_buf;
        break;
    }

    char new_user[64];
    new_user[0] = '\0';
    char new_user_hash[128];
    new_user_hash[0] = '\0';
    ask("Create a user account? [y/N] ", "n", answer, sizeof(answer));
    if (answer[0] == 'y' || answer[0] == 'Y') {
        for (;;) {
            ask("Username: ", "", new_user, sizeof(new_user));
            if (new_user[0] == '\0' || strchr(new_user, ':') != NULL ||
                strchr(new_user, '/') != NULL) {
                printf("Not a usable username. Try again.\n");
                continue;
            }
            char p1[128], p2[128];
            ask_secret("Password: ", p1, sizeof(p1));
            ask_secret("Retype password: ", p2, sizeof(p2));
            if (strcmp(p1, p2) != 0 || p1[0] == '\0') {
                printf("Passwords did not match (or were empty). Try "
                       "again.\n");
                new_user[0] = '\0';
                continue;
            }
            if (hash_password(p1, new_user_hash, sizeof(new_user_hash)) !=
                0) {
                printf("Could not hash that password. Try again.\n");
                new_user[0] = '\0';
                continue;
            }
            break;
        }
    }

    ask("Enable SSH daemon at startup? [y/N] ", "n", answer, sizeof(answer));
    int enable_sshd = (answer[0] == 'y' || answer[0] == 'Y');

    size_t custom_rootfs_len = 0;
    uint8_t *custom_rootfs = build_custom_rootfs(
        root_hash, new_user[0] != '\0' ? new_user : NULL, new_user_hash,
        enable_sshd, &custom_rootfs_len);
    if (custom_rootfs == NULL) {
        printf("\nCould not prepare the root filesystem for install.\n");
        return 1;
    }

    g_disk = open(chosen->path, O_RDWR);
    if (g_disk < 0) {
        printf("%s: cannot open for writing\n", chosen->path);
        return 1;
    }
    g_disk_bytes = chosen->bytes;

    uint32_t total_sectors = (uint32_t)(g_disk_bytes / SECTOR);
    if (total_sectors <= PART_START + 2048) {
        printf("%s is too small to install onto.\n", chosen->name);
        return 1;
    }
    uint32_t avail_sectors = total_sectors - PART_START;

    /* Size the ESP for what it actually has to hold - the kernel plus
     * the root filesystem just built, doubled for headroom (a future
     * `make iso` growing rootfs.img, or reinstalling in place without
     * repartitioning) - floored at 64 MiB so a small system still
     * gets sensible slack. Whatever's left over on the disk, if
     * enough to be worth it, becomes the WRF partition; a disk too
     * tight for both keeps the old behavior exactly - one partition,
     * the ESP gets everything, no /mnt. */
    long kernel_bytes = 0;
    int kf = open(SRC_KERNEL, O_RDONLY);
    if (kf >= 0) {
        kernel_bytes = lseek(kf, 0, SEEK_END);
        close(kf);
    }
    uint64_t need_bytes = (uint64_t)(kernel_bytes > 0 ? kernel_bytes : 0) +
                           custom_rootfs_len + (4u << 20); /* loader+config+FAT overhead */
    uint32_t esp_wanted = (uint32_t)((need_bytes * 2 + SECTOR - 1) / SECTOR);
    uint32_t esp_floor = (64u << 20) / SECTOR;
    if (esp_wanted < esp_floor) {
        esp_wanted = esp_floor;
    }
    uint32_t wrf_floor = (8u << 20) / SECTOR;

    uint32_t esp_sectors = avail_sectors;
    uint32_t wrf_start = 0, wrf_sectors = 0;
    if (avail_sectors > esp_wanted + wrf_floor) {
        esp_sectors = esp_wanted;
        wrf_start = PART_START + esp_sectors;
        wrf_sectors = avail_sectors - esp_sectors;
    }

    printf("\nInstalling on %s:\n", chosen->name);
    if (write_mbr(esp_sectors, wrf_start, wrf_sectors) != 0) {
        printf("  writing the partition table failed\n");
        return 1;
    }
    printf("  %-28s   ok\n", "partition table");

    if (mkfs(esp_sectors) != 0) {
        return 1;
    }

    uint32_t efi = dir_create(ROOT_CLUSTER, "EFI");
    uint32_t boot_efi = efi != 0 ? dir_create(efi, "BOOT") : 0;
    uint32_t boot = dir_create(ROOT_CLUSTER, "boot");
    if (efi == 0 || boot_efi == 0 || boot == 0 || fat_flush() != 0) {
        printf("  creating the directories failed\n");
        return 1;
    }

    /* The order is the order they matter in: without the loader
     * nothing else on the disk is reachable. */
    uint32_t first_loader = g_fs.next_free;
    if (copy_file(boot_efi, "BOOTX64.EFI", SRC_LOADER, "bootloader") != 0) {
        return 1;
    }
    uint32_t first_config = g_fs.next_free;
    if (copy_file(boot, "limine.conf", SRC_CONFIG, "boot configuration") != 0) {
        return 1;
    }
    uint32_t first_kernel = g_fs.next_free;
    if (copy_file(boot, "kernel.elf", SRC_KERNEL, "kernel") != 0) {
        return 1;
    }
    uint32_t first_rootfs = g_fs.next_free;
    if (copy_buffer(boot, "rootfs.img", custom_rootfs, custom_rootfs_len,
                    "root filesystem") != 0) {
        return 1;
    }

    printf("  %-28s", "verifying");
    fflush(stdout);
    if (verify_file(first_loader, SRC_LOADER) != 0 ||
        verify_file(first_config, SRC_CONFIG) != 0 ||
        verify_file(first_kernel, SRC_KERNEL) != 0 ||
        verify_buffer(first_rootfs, custom_rootfs, custom_rootfs_len) != 0) {
        printf("   FAILED\n\nWhat was written back does not match what was "
               "sent. The disk is not usable.\n");
        free(custom_rootfs);
        return 1;
    }
    free(custom_rootfs);
    printf("   ok\n");

    if (wrf_sectors != 0) {
        printf("  %-28s", "persistent storage (/mnt)");
        fflush(stdout);
        if (format_wrf_partition(wrf_start, wrf_sectors) != 0) {
            /* Not fatal: the boot partition is already written and
             * verified above, so the system installed is still fully
             * bootable - it just starts with no /mnt, same as an
             * install on a disk too small to spare room for one. */
            printf("   FAILED (the system will still boot; /mnt will "
                   "be empty)\n");
        } else {
            printf("   ok\n");
        }
    }

    close(g_disk);
    g_disk = -1;

    printf("%s", CONGRATS);
    if (wrf_sectors != 0) {
        printf("A persistent /mnt is ready too - anything created there\n"
               "(unlike the rest of the system) survives a reboot.\n\n");
    }

    for (;;) {
        ask("Exit to (S)hell, (H)alt or (R)eboot? [reboot] ", "reboot",
            answer, sizeof(answer));
        char c = answer[0];
        if (c == 's' || c == 'S') {
            printf("\nThe disk is installed. `reboot` when you are ready;\n"
                   "take the CD out first, or the machine will boot it "
                   "again.\n");
            return 0;
        }
        if (c == 'h' || c == 'H') {
            tus_syscall(SYS_POWER, TUS_POWER_HALT);
            return 0;
        }
        if (c == 'r' || c == 'R') {
            tus_syscall(SYS_POWER, TUS_POWER_REBOOT);
            return 0;
        }
    }
}
