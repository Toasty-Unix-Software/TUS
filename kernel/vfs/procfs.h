#ifndef TUS_VFS_PROCFS_H
#define TUS_VFS_PROCFS_H

/* Create /proc and its synthetic files. Called by vfs_devices_init(),
 * right after devices_init() (needs PMM/scheduler already up). */
void procfs_init(void);

#endif /* TUS_VFS_PROCFS_H */
