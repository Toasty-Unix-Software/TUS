/*
 * wrf.h - WRF filesystem driver: the kernel side
 *
 * See include/wrf.h for the on-disk format. This header is the seam
 * between kernel/vfs/vfs.c (which owns the in-memory node tree and
 * fd-based API) and kernel/fs/wrf.c (which owns the disk): vfs.c
 * calls these exactly at the points a WRF-backed node is created,
 * removed, moved, read, written or truncated - see the comments next
 * to each call site in vfs.c for why that point and not another.
 *
 * Every call here is a no-op unless the node/directory involved is
 * actually WRF-backed (struct vfs_node's wrf_ino field is nonzero),
 * so vfs.c can call them unconditionally at the right point without
 * a mount check of its own.
 */

#ifndef TUS_FS_WRF_H
#define TUS_FS_WRF_H

#include <stddef.h>
#include <stdint.h>

struct vfs_node;

/* Mounts /mnt from the first ATA disk that carries a valid WRF
 * superblock (format one first with mkfs.wrf). Creates /mnt if it
 * does not already exist. Always safe to call even with no disk
 * present or an unformatted one - logs why and leaves /mnt as a
 * normal empty (non-persistent) directory in that case. Called once,
 * at boot, from kernel/main.c right after the rootfs mount. */
void wrf_boot_mount(void);

/* dir is WRF-backed and node is a brand-new (node->wrf_ino == 0)
 * VFS_DIR or VFS_FILE just linked under it: allocate an inode and a
 * directory entry, and set node->wrf_ino. Called from
 * vfs_create_dir()/vfs_create_file(), never from vfs_create_device()/
 * vfs_create_socket() (device nodes and sockets are never persisted)
 * or from vfs_rename()'s reattachment (which already has a
 * wrf_ino and must not be allocated a second inode - see
 * wrf_notify_attach() below). */
void wrf_notify_create(struct vfs_node *dir, struct vfs_node *node);

/* Removes node's directory entry from dir on disk, WITHOUT freeing
 * its inode or data blocks - the node may still be reachable (an
 * open fd surviving unlink(), or about to be reattached elsewhere by
 * rename()). Call wrf_notify_free() separately once the node is
 * actually gone for good. No-op unless both dir and node are
 * WRF-backed. */
void wrf_notify_detach(struct vfs_node *dir, struct vfs_node *node);

/* The other half of a WRF-aware rename(): adds a directory entry in
 * dir for node's EXISTING inode (node->wrf_ino, already set) under
 * node's current (new) name - no inode is allocated. No-op unless
 * both dir and node are WRF-backed. */
void wrf_notify_attach(struct vfs_node *dir, struct vfs_node *node);

/* Frees node's inode and every data block it owns (direct, indirect
 * and doubly-indirect) once it is truly gone - the immediate-free
 * path in vfs_remove()/vfs_rename(), or file_unref() on the last
 * close of an unlinked, still-open node. Clears node->wrf_ino. No-op
 * if node is not WRF-backed. */
void wrf_notify_free(struct vfs_node *node);

/* Regular file I/O, called from vfs.c's file_read()/file_write()/
 * vfs_pread() in place of the ramfs node->data path whenever
 * node->wrf_ino != 0. Same shape and return convention as those
 * (bytes moved, or a negative errno). */
long wrf_file_read(struct vfs_node *node, void *buf, size_t count, size_t pos);
long wrf_file_write(struct vfs_node *node, const void *buf, size_t count, size_t pos);

/* ftruncate()/O_TRUNC for a WRF-backed file. Shrinking only updates
 * the recorded size - the blocks past it stay allocated (reused
 * as-is if the file grows again later) rather than being freed, a
 * deliberate v1 simplification: nothing about it is incorrect, it
 * just does not reclaim space a shrink-then-rewrite cycle could
 * have. Returns 0 (WRF never fails a truncate a real disk fits). */
long wrf_truncate(struct vfs_node *node, size_t length);

/* Walks every block the block bitmap marks allocated and re-checks it
 * against its stored checksum table entry (see the v2 integrity
 * comment in include/wrf.h) - the "wrfscrub" shell command's
 * implementation. Prints one line per mismatch found (a scrub is a
 * detector, not a repair tool: WRF keeps no redundant copy of file
 * data to reconstruct from) plus a one-line summary, and returns the
 * number of mismatches found (0 if /home is not WRF-mounted or the
 * volume is perfectly healthy). */
int wrf_scrub(void);

#endif /* TUS_FS_WRF_H */
