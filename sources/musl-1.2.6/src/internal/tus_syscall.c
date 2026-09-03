/*
 * tus_syscall.c - TUS kernel ABI translation for musl
 *
 * TUS (Toasty Unix Software) exposes a compact POSIX-style syscall
 * ABI through `int $0x80` with its own numbering (kernel/syscall/
 * syscall.h). Upstream musl source uses Linux x86_64 syscall numbers
 * and the `syscall` instruction. This file is the bridge:
 *
 *   - __syscall0..6 (arch/x86_64/syscall_arch.h) forward here,
 *   - __syscall_cp_asm (src/thread/x86_64/syscall_cp.s) forwards
 *     here too (TUS has no thread cancellation yet),
 *
 * and it translates the Linux number to the TUS number, emulates a
 * few calls in userspace (converting timespecs, ignoring madvise and
 * umask) and returns -ENOSYS (-38) for everything else, exactly like
 * an unknown Linux syscall.
 *
 * The kernel stub only restores RAX; every argument register is
 * clobbered by the trap, so all registers are declared read-write in
 * the asm (see TUS.md lesson 7/16).
 */

#include <stdint.h>

/* TUS syscall numbers (must match kernel/syscall/syscall.h). */
#define TUS_SYS_EXIT    0
#define TUS_SYS_READ    1
#define TUS_SYS_WRITE   2
#define TUS_SYS_OPEN    3
#define TUS_SYS_CLOSE   4
#define TUS_SYS_IOCTL   5
#define TUS_SYS_GETPID  6
#define TUS_SYS_SLEEP   8
#define TUS_SYS_MKDIR   9
#define TUS_SYS_UNLINK  10
#define TUS_SYS_READDIR 11
#define TUS_SYS_MMAP    12
#define TUS_SYS_MUNMAP  13
#define TUS_SYS_ARCH_PRCTL 14
#define TUS_SYS_WRITEV  15
#define TUS_SYS_READV   51
#define TUS_SYS_TIME    16
#define TUS_SYS_FTRUNCATE 17
#define TUS_SYS_LSEEK    40
#define TUS_SYS_EXECVE   18
#define TUS_SYS_CHMOD    19
#define TUS_SYS_GETUID   20
#define TUS_SYS_GETEUID  21
#define TUS_SYS_SETUID   22
#define TUS_SYS_GETGID   23
#define TUS_SYS_SETGID   24
#define TUS_SYS_PIPE     25
#define TUS_SYS_DUP2     26
#define TUS_SYS_DUP      27
#define TUS_SYS_SOCKET      28
#define TUS_SYS_BIND        29
#define TUS_SYS_LISTEN      30
#define TUS_SYS_ACCEPT      31
#define TUS_SYS_CONNECT     32
#define TUS_SYS_SOCKETPAIR  33
#define TUS_SYS_SHUTDOWN    34
#define TUS_SYS_GETSOCKNAME 35
#define TUS_SYS_POLL        36
#define TUS_SYS_SELECT      37
#define TUS_SYS_SENDTO      41
#define TUS_SYS_RECVFROM    42
#define TUS_SYS_GETPEERNAME 43
#define TUS_SYS_SETSOCKOPT  44
#define TUS_SYS_GETSOCKOPT  45
#define TUS_SYS_FCNTL       46
#define TUS_SYS_NETCTL      47
#define TUS_SYS_GETRANDOM   48
#define TUS_SYS_WAITPID     49
#define TUS_SYS_CLOCK       50
#define TUS_SYS_STATX       56
#define TUS_SYS_GETPPID     58
#define TUS_SYS_GETPGRP     59
#define TUS_SYS_GETPGID     60
#define TUS_SYS_SETPGID     61
#define TUS_SYS_SETSID      62
#define TUS_SYS_CHDIR       63
#define TUS_SYS_FCHDIR      64
#define TUS_SYS_GETCWD      65
#define TUS_SYS_ACCESS      66
#define TUS_SYS_RENAME      67
#define TUS_SYS_READLINK    68
#define TUS_SYS_SYMLINK     69
#define TUS_SYS_TIMES       70
#define TUS_SYS_GETRLIMIT   71
#define TUS_SYS_SETRLIMIT   72
#define TUS_SYS_SET_TID_ADDRESS 73
#define TUS_SYS_KILL        74
#define TUS_SYS_SIGACTION   75
#define TUS_SYS_SIGPROCMASK 76
#define TUS_SYS_SIGSUSPEND  77
#define TUS_SYS_ALARM       78
#define TUS_SYS_PAUSE       79
#define TUS_SYS_SETITIMER   80
#define TUS_SYS_UNAME        81
#define TUS_SYS_SETHOSTNAME  82
#define TUS_SYS_CHOWN        86

#define TUS_ENOSYS (-38)
#define TUS_EFAULT (-14)
#define TUS_EOPNOTSUPP (-95)

/* The one send()/recv() flag that is a no-op on TUS (no signals). */
#define TUS_MSG_NOSIGNAL 0x4000L

/* Userspace view of a struct timespec (fields match musl's layout). */
struct tus_timespec {
    long tv_sec;
    long tv_nsec;
};

/* The raw trap. Only RAX comes back; all argument registers are
 * read-write so the compiler reloads them around every call. */
static __inline long tus_raw(long n, long a1, long a2, long a3,
                             long a4, long a5, long a6) {
    long ret;
    register long r10 __asm__("r10") = a4;
    register long r8 __asm__("r8") = a5;
    register long r9 __asm__("r9") = a6;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"(n)
                     : "rcx", "r11", "memory");
    return ret;
}

/* fork(): its own gate (int $0x81, not the usual int $0x80) - see
 * kernel/syscall/syscall.h's SYS_FORK entry and fork_entry()/
 * sched_fork() in kernel/sched/sched.c for why. No arguments to load:
 * the kernel captures the caller's full register state itself, so
 * only the result matters - child pid in the parent, 0 in the child,
 * or a negative errno. */
static __inline long tus_fork(void) {
    long ret;
    __asm__ volatile("int $0x81" : "=a"(ret) : : "rcx", "r11", "memory");
    return ret;
}

/* struct timeval (musl layout). */
struct tus_timeval {
    long tv_sec;
    long tv_usec;
};

/* Mirrors the kernel's struct tus_statx (kernel/syscall/syscall.c) -
 * only used locally by the SYS_fstat translation below. */
struct tus_statx_mini {
    uint32_t stx_mask;
    uint32_t stx_blksize;
    uint64_t stx_attributes;
    uint32_t stx_nlink;
    uint32_t stx_uid;
    uint32_t stx_gid;
    uint16_t stx_mode;
    uint16_t pad1;
    uint64_t stx_ino;
    uint64_t stx_size;
    uint64_t stx_blocks;
    uint64_t stx_attributes_mask;
    struct {
        int64_t tv_sec;
        uint32_t tv_nsec;
        int32_t pad;
    } stx_atime, stx_btime, stx_ctime, stx_mtime;
    uint32_t stx_rdev_major, stx_rdev_minor, stx_dev_major, stx_dev_minor;
    uint64_t spare[14];
};

/* Classic x86_64 struct stat, matching musl's own src/internal/kstat.h
 * layout - what fstatat_kstat() in src/stat/fstatat.c fills in.
 * fstatat_kstat() has two branches that reach TUS: fd+AT_EMPTY_PATH
 * (what fstat() itself uses, SYS_fstat/case 5 below) and everything
 * path-based - stat(), lstat(), fstatat() with a real path - which
 * all fall to SYS_fstatat/SYS_newfstatat (case 262), since x86_64's
 * kstat time fields are already 64-bit and so never take the SYS_statx
 * branch fstatat_kstat() tries first. */
struct tus_kstat {
    uint64_t st_dev;
    uint64_t st_ino;
    uint64_t st_nlink;
    uint32_t st_mode;
    uint32_t st_uid;
    uint32_t st_gid;
    uint32_t __pad0;
    uint64_t st_rdev;
    int64_t st_size;
    int64_t st_blksize;
    int64_t st_blocks;
    int64_t st_atime_sec, st_atime_nsec;
    int64_t st_mtime_sec, st_mtime_nsec;
    int64_t st_ctime_sec, st_ctime_nsec;
    int64_t __unused[3];
};

/* Shared by cases 5 and 262 below: both get the kernel's answer as a
 * struct tus_statx and need it translated into the classic struct
 * stat shape musl asked for. */
static __inline void statx_mini_to_kstat(const struct tus_statx_mini *stx, struct tus_kstat *ks) {
    ks->st_dev = 0;
    ks->st_ino = stx->stx_ino;
    ks->st_nlink = stx->stx_nlink;
    ks->st_mode = stx->stx_mode;
    ks->st_uid = stx->stx_uid;
    ks->st_gid = stx->stx_gid;
    ks->__pad0 = 0;
    ks->st_rdev = 0;
    ks->st_size = (int64_t)stx->stx_size;
    ks->st_blksize = (int64_t)stx->stx_blksize;
    ks->st_blocks = (int64_t)stx->stx_blocks;
    ks->st_atime_sec = stx->stx_atime.tv_sec;
    ks->st_atime_nsec = stx->stx_atime.tv_nsec;
    ks->st_mtime_sec = stx->stx_mtime.tv_sec;
    ks->st_mtime_nsec = stx->stx_mtime.tv_nsec;
    ks->st_ctime_sec = stx->stx_ctime.tv_sec;
    ks->st_ctime_nsec = stx->stx_ctime.tv_nsec;
}

/* Mirrors the kernel's struct vfs_dirent (kernel/vfs/vfs.h) - one
 * fixed-size record per entry, unlike Linux's variable-length packed
 * getdents format. */
struct tus_vfs_dirent {
    char name[64];
    uint32_t type;   /* VFS_DIR=1, VFS_FILE=2, VFS_DEVICE=3, VFS_SOCKET=4 */
    uint32_t size;
    uint32_t mode;
};

/* getdents(2): musl's opendir()/readdir() (src/dirent/readdir.c) read
 * raw kernel dirents straight into their own struct dirent - {d_ino,
 * d_off, d_reclen, d_type, d_name[]} on this arch (arch/generic/bits/
 * dirent.h) - so the kernel's answer has to already BE that shape.
 * TUS's own SYS_READDIR returns simpler fixed-size records instead
 * (ls, hxfiles etc. call it directly - see kernel/vfs/vfs.c's own
 * comment on why musl does not wrap it); this bridges the two by
 * reading one TUS record at a time and packing it into musl's layout.
 *
 * One real limitation: TUS_SYS_READDIR advances the directory's
 * position as it returns each entry, so an entry read here but not
 * yet copied out (because doing so would overflow the caller's
 * buffer) is still consumed - a directory with enough entries to
 * overflow a single getdents() call (musl's own DIR buffer is a few
 * KB) could silently skip the remainder. Fine for what TUS actually
 * has directories full of; a real fix would need TUS_SYS_READDIR to
 * support "peek without consuming", which does not exist yet. */
struct tus_linux_dirent {
    uint64_t d_ino;
    int64_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};

static long tus_getdents(long fd, char *buf, long count) {
    long total = 0;
    for (;;) {
        struct tus_vfs_dirent ent;
        long n = tus_raw(TUS_SYS_READDIR, fd, (long)&ent, (long)sizeof(ent), 0, 0, 0);
        if (n <= 0) {
            break; /* EOF, or an error on the very first entry (lost either way) */
        }
        unsigned char d_type;
        switch (ent.type) {
        case 1: d_type = 4; break;  /* VFS_DIR -> DT_DIR */
        case 2: d_type = 8; break;  /* VFS_FILE -> DT_REG */
        case 3: d_type = 2; break;  /* VFS_DEVICE -> DT_CHR (closest match) */
        case 4: d_type = 12; break; /* VFS_SOCKET -> DT_SOCK */
        default: d_type = 0; break; /* DT_UNKNOWN */
        }
        long namelen = 0;
        while (ent.name[namelen] != '\0' && namelen < (long)sizeof(ent.name) - 1) {
            namelen++;
        }
        long reclen = (long)(sizeof(struct tus_linux_dirent) + namelen + 1 + 7) & ~7L;
        if (total + reclen > count) {
            break; /* would not fit - see the function comment above */
        }
        struct tus_linux_dirent *d = (struct tus_linux_dirent *)(buf + total);
        d->d_ino = 1;   /* TUS has no real inode numbers; 0 would read as a deleted entry */
        d->d_off = total + reclen;
        d->d_reclen = (unsigned short)reclen;
        d->d_type = d_type;
        for (long i = 0; i < namelen; i++) {
            d->d_name[i] = ent.name[i];
        }
        d->d_name[namelen] = '\0';
        total += reclen;
    }
    return total;
}

/* statfs(2)/fstatfs(2): TUS's whole filesystem genuinely IS what
 * these ask about - kernel/vfs/vfs.c's own header comment calls it
 * "a small in-memory UNIX-like filesystem (ramfs)" - so answering as
 * tmpfs is not a portability fib, it is the honest answer. Synthesized
 * entirely in userspace (no kernel round trip needed, the answer
 * never varies): libast's sftmp.c checks this to decide whether a
 * temp file can skip a real disk write, and having it return -ENOSYS
 * previously was harmless there (checked with "< 0") but left other,
 * less careful callers on the wrong end of an unchecked assumption
 * that a normal Linux program is entitled to make. */
#define TUS_TMPFS_MAGIC 0x01021994L
static long tus_statfs_common(long buf_ptr) {
    /* struct statfs (musl's bits/statfs.h): f_type, f_bsize, f_blocks,
     * f_bfree, f_bavail, f_files, f_ffree, f_fsid, f_namelen, f_frsize,
     * f_flags, f_spare[4] - all "unsigned long"-or-8-byte fields on
     * this arch, hence indexing it as one flat array of longs. Real
     * free-space numbers matter here, not just a non-error return:
     * libast's own xaccess() (src/lib/libast/path/pathtemp.c) computes
     * f_frsize*f_bavail itself and synthesizes ENOSPC when that comes
     * out to 0 - which an all-zeroed stub always does. kmalloc's 64
     * MiB heap (kernel/mm/kmalloc.c) is the real, honest ceiling. */
    long *b = (long *)buf_ptr;
    for (int i = 0; i < 15; i++) {
        b[i] = 0;
    }
    b[0] = TUS_TMPFS_MAGIC; /* f_type */
    b[1] = 4096;             /* f_bsize */
    b[2] = 16384;            /* f_blocks: 64 MiB / 4096 */
    b[3] = 16384;            /* f_bfree */
    b[4] = 16384;            /* f_bavail */
    b[5] = 65536;            /* f_files: no real inode ceiling, a big round number */
    b[6] = 65536;            /* f_ffree */
    b[8] = 64;               /* f_namelen (VFS_NAME_MAX, kernel/vfs/vfs.h) */
    b[9] = 4096;             /* f_frsize */
    return 0;
}

/* clock_gettime / clock_gettime64: CLOCK_REALTIME comes from the CMOS
 * clock (SYS_CLOCK), CLOCK_MONOTONIC from the tick counter (SYS_TIME).
 * They were the same value until TUS had an RTC, and telling them
 * apart matters the moment something records a date - a git commit
 * stamped with seconds-since-boot is dated 1970. */
static __inline long tus_clock_gettime(long clk, const void *ts) {
    if (ts == 0 || (clk != 0 && clk != 1)) {
        return -22; /* -EINVAL */
    }
    long secs = tus_raw(clk == 0 ? TUS_SYS_CLOCK : TUS_SYS_TIME,
                        0, 0, 0, 0, 0, 0);
    if (secs < 0) {
        return secs;
    }
    struct tus_timespec *t = (struct tus_timespec *)ts;
    t->tv_sec = secs;
    t->tv_nsec = 0;
    return 0;
}

/* Convert a userspace timespec to whole seconds, rounding up (the
 * TUS sleep syscall takes seconds). NULL maps to an immediate
 * return. */
static __inline long tus_sleep_from_timespec(const void *ts) {
    const struct tus_timespec *t = (const struct tus_timespec *)ts;
    if (t == 0) {
        return 0;
    }
    long secs = t->tv_sec;
    if (t->tv_nsec > 0) {
        secs += 1;
    }
    return tus_raw(TUS_SYS_SLEEP, secs, 0, 0, 0, 0, 0);
}

long tus_syscall(long n, long a1, long a2, long a3,
                 long a4, long a5, long a6) {
    switch (n) {
    /* Straight number translations (Linux x86_64 -> TUS). */
    case 0:   return tus_raw(TUS_SYS_READ, a1, a2, a3, 0, 0, 0);    /* read */
    case 1:   return tus_raw(TUS_SYS_WRITE, a1, a2, a3, 0, 0, 0);   /* write */
    case 2:   return tus_raw(TUS_SYS_OPEN, a1, a2, a3, 0, 0, 0);    /* open */
    case 3:   return tus_raw(TUS_SYS_CLOSE, a1, 0, 0, 0, 0, 0);     /* close */
    case 9:   return tus_raw(TUS_SYS_MMAP, a1, a2, a3, a4, 0, 0);   /* mmap */
    case 11:  return tus_raw(TUS_SYS_MUNMAP, a1, a2, 0, 0, 0, 0);   /* munmap */
    case 16:  return tus_raw(TUS_SYS_IOCTL, a1, a2, a3, 0, 0, 0);   /* ioctl */
    case 20:  return tus_raw(TUS_SYS_WRITEV, a1, a2, a3, 0, 0, 0);  /* writev */
    /* readv: musl's buffered input is built on it, so every fgets()
     * and fread() in TUS arrives here. */
    case 19:  return tus_raw(TUS_SYS_READV, a1, a2, a3, 0, 0, 0);   /* readv */
    case 7:   return tus_raw(TUS_SYS_POLL, a1, a2, a3, 0, 0, 0);    /* poll(fds, n, timeout) */
    case 22:  return tus_raw(TUS_SYS_PIPE, a1, 0, 0, 0, 0, 0);      /* pipe(fds) */
    case 23:  return tus_raw(TUS_SYS_SELECT, a1, a2, a3, a4, a5, 0); /* select(n, r, w, e, tv) */
    case 41:  return tus_raw(TUS_SYS_SOCKET, a1, a2, a3, 0, 0, 0);  /* socket */
    case 42:  return tus_raw(TUS_SYS_CONNECT, a1, a2, a3, 0, 0, 0); /* connect */
    case 43:  return tus_raw(TUS_SYS_ACCEPT, a1, a2, a3, 0, 0, 0);  /* accept */
    case 48:  return tus_raw(TUS_SYS_SHUTDOWN, a1, a2, 0, 0, 0, 0); /* shutdown */
    case 49:  return tus_raw(TUS_SYS_BIND, a1, a2, a3, 0, 0, 0);    /* bind */
    case 50:  return tus_raw(TUS_SYS_LISTEN, a1, a2, 0, 0, 0, 0);   /* listen */
    case 51:  return tus_raw(TUS_SYS_GETSOCKNAME, a1, a2, a3, 0, 0, 0); /* getsockname */
    case 53:  return tus_raw(TUS_SYS_SOCKETPAIR, a1, a2, a3, a4, 0, 0); /* socketpair */
    case 288: return tus_raw(TUS_SYS_ACCEPT, a1, a2, a3, 0, 0, 0);  /* accept4: flags ignored (no O_NONBLOCK/CLOEXEC) */
    case 32:  return tus_raw(TUS_SYS_DUP, a1, 0, 0, 0, 0, 0);       /* dup */
    case 33:  return tus_raw(TUS_SYS_DUP2, a1, a2, 0, 0, 0, 0);     /* dup2 */
    case 39:  return tus_raw(TUS_SYS_GETPID, 0, 0, 0, 0, 0, 0);     /* getpid */
    case 57:  return tus_fork();                                    /* fork (see tus_fork() above) */
    case 59:  return tus_raw(TUS_SYS_EXECVE, a1, a2, a3, 0, 0, 0);  /* execve */
    case 60:  return tus_raw(TUS_SYS_EXIT, a1, 0, 0, 0, 0, 0);      /* exit */
    case 8:   return tus_raw(TUS_SYS_LSEEK, a1, a2, a3, 0, 0, 0);   /* lseek(fd, off, whence) */
    case 77:  return tus_raw(TUS_SYS_FTRUNCATE, a1, a2, 0, 0, 0, 0); /* ftruncate */
    case 83:  return tus_raw(TUS_SYS_MKDIR, a1, a2, 0, 0, 0, 0);    /* mkdir(path, mode) */
    case 87:  return tus_raw(TUS_SYS_UNLINK, a1, 0, 0, 0, 0, 0);    /* unlink */
    case 137: return tus_statfs_common(a2);                         /* statfs(path, buf) - see tus_statfs_common() above */
    case 138: return tus_statfs_common(a2);                         /* fstatfs(fd, buf) */
    case 78:  return tus_getdents(a1, (char *)a2, a3);              /* getdents - see tus_getdents() above */
    case 217: return tus_getdents(a1, (char *)a2, a3);              /* getdents64: same layout on this arch */
    case 90:  return tus_raw(TUS_SYS_CHMOD, a1, a2, 0, 0, 0, 0);    /* chmod */
    case 92:  return tus_raw(TUS_SYS_CHOWN, a1, a2, a3, 0, 0, 0);   /* chown(path, uid, gid) */
    case 102: return tus_raw(TUS_SYS_GETUID, 0, 0, 0, 0, 0, 0);     /* getuid */
    case 104: return tus_raw(TUS_SYS_GETGID, 0, 0, 0, 0, 0, 0);     /* getgid */
    case 105: return tus_raw(TUS_SYS_SETUID, a1, 0, 0, 0, 0, 0);    /* setuid */
    case 106: return tus_raw(TUS_SYS_SETGID, a1, 0, 0, 0, 0, 0);    /* setgid */
    case 107: return tus_raw(TUS_SYS_GETEUID, 0, 0, 0, 0, 0, 0);    /* geteuid */
    case 108: return tus_raw(TUS_SYS_GETGID, 0, 0, 0, 0, 0, 0);    /* getegid */
    case 158: return tus_raw(TUS_SYS_ARCH_PRCTL, a1, a2, 0, 0, 0, 0); /* arch_prctl */
    case 201: return tus_raw(TUS_SYS_CLOCK, 0, 0, 0, 0, 0, 0);      /* time */
    case 231: return tus_raw(TUS_SYS_EXIT, a1, 0, 0, 0, 0, 0);      /* exit_group */
    case 257: return tus_raw(TUS_SYS_OPEN, a2, a3, 0, 0, 0, 0);     /* openat(dirfd, path, flags, mode) */
    case 258: return tus_raw(TUS_SYS_MKDIR, a2, a3, 0, 0, 0, 0);    /* mkdirat(dirfd, path, mode) */
    case 263: return tus_raw(TUS_SYS_UNLINK, a2, 0, 0, 0, 0, 0);    /* unlinkat(dirfd, path, flags) */

    /* Process identity, working directory, and the handful of other
     * POSIX calls a real ksh93 makes directly (see kernel/syscall/
     * syscall.c's dispatch for what each one actually does on TUS -
     * no cwd-relative dirfd support, same simplification openat/
     * mkdirat/unlinkat above already make: only AT_FDCWD is ever
     * seen from a task with no dirent-derived directory fds). */
    case 110: return tus_raw(TUS_SYS_GETPPID, 0, 0, 0, 0, 0, 0);    /* getppid */
    case 111: return tus_raw(TUS_SYS_GETPGRP, 0, 0, 0, 0, 0, 0);    /* getpgrp */
    case 121: return tus_raw(TUS_SYS_GETPGID, a1, 0, 0, 0, 0, 0);   /* getpgid */
    case 109: return tus_raw(TUS_SYS_SETPGID, a1, a2, 0, 0, 0, 0);  /* setpgid */
    case 112: return tus_raw(TUS_SYS_SETSID, 0, 0, 0, 0, 0, 0);     /* setsid */
    case 80:  return tus_raw(TUS_SYS_CHDIR, a1, 0, 0, 0, 0, 0);     /* chdir */
    case 81:  return tus_raw(TUS_SYS_FCHDIR, a1, 0, 0, 0, 0, 0);    /* fchdir */
    case 79:  return tus_raw(TUS_SYS_GETCWD, a1, a2, 0, 0, 0, 0);   /* getcwd */
    case 21:  return tus_raw(TUS_SYS_ACCESS, a1, a2, 0, 0, 0, 0);   /* access */
    case 269: return tus_raw(TUS_SYS_ACCESS, a2, a3, 0, 0, 0, 0);   /* faccessat(dirfd, path, mode, flags) */
    case 82:  return tus_raw(TUS_SYS_RENAME, a1, a2, 0, 0, 0, 0);   /* rename */
    case 89:  return tus_raw(TUS_SYS_READLINK, a1, a2, a3, 0, 0, 0); /* readlink */
    case 267: return tus_raw(TUS_SYS_READLINK, a2, a3, a4, 0, 0, 0); /* readlinkat(dirfd, path, buf, size) */
    case 88:  return tus_raw(TUS_SYS_SYMLINK, a1, a2, 0, 0, 0, 0);  /* symlink */
    case 266: return tus_raw(TUS_SYS_SYMLINK, a1, a3, 0, 0, 0, 0);  /* symlinkat(target, newdirfd, path) */
    case 100: return tus_raw(TUS_SYS_TIMES, a1, 0, 0, 0, 0, 0);     /* times */
    case 97:  return tus_raw(TUS_SYS_GETRLIMIT, a1, a2, 0, 0, 0, 0); /* getrlimit */
    case 160: return tus_raw(TUS_SYS_SETRLIMIT, a1, a2, 0, 0, 0, 0); /* setrlimit */
    case 302: /* prlimit64(pid, resource, new, old): TUS tasks have no
               * real limits to set; only the "read back" direction
               * (old != NULL) is answered, matching getrlimit. */
        if (a4 != 0) {
            return tus_raw(TUS_SYS_GETRLIMIT, a2, a4, 0, 0, 0, 0);
        }
        return 0;
    case 218: return tus_raw(TUS_SYS_SET_TID_ADDRESS, 0, 0, 0, 0, 0, 0); /* set_tid_address */

    /* Real POSIX signal delivery (kernel/sched/sched.c). rt_sigreturn
     * (15) does NOT arrive here at all - musl's own restorer traps
     * straight to its dedicated IDT vector (int $0x82, see arch/
     * x86_64/src/signal/restore.s), the same reason fork() bypasses
     * this dispatch too. */
    case 62:  return tus_raw(TUS_SYS_KILL, a1, a2, 0, 0, 0, 0);        /* kill */
    case 200: return tus_raw(TUS_SYS_KILL, a1, a2, 0, 0, 0, 0);        /* tkill(tid, sig): tid==pid, no threads */
    case 234: return tus_raw(TUS_SYS_KILL, a1, a3, 0, 0, 0, 0);        /* tgkill(tgid, tid, sig) */
    case 13:  return tus_raw(TUS_SYS_SIGACTION, a1, a2, a3, 0, 0, 0);  /* rt_sigaction */
    case 14:  return tus_raw(TUS_SYS_SIGPROCMASK, a1, a2, a3, 0, 0, 0); /* rt_sigprocmask */
    case 130: return tus_raw(TUS_SYS_SIGSUSPEND, a1, 0, 0, 0, 0, 0);   /* rt_sigsuspend */
    case 37:  return tus_raw(TUS_SYS_ALARM, a1, 0, 0, 0, 0, 0);        /* alarm */
    case 34:  return tus_raw(TUS_SYS_PAUSE, 0, 0, 0, 0, 0, 0);         /* pause */
    case 38:  return tus_raw(TUS_SYS_SETITIMER, a1, a2, a3, 0, 0, 0);  /* setitimer - what musl's alarm() actually calls */
    case 63:  return tus_raw(TUS_SYS_UNAME, a1, 0, 0, 0, 0, 0);        /* uname - what musl's gethostname() actually calls */
    case 170: return tus_raw(TUS_SYS_SETHOSTNAME, a1, a2, 0, 0, 0, 0); /* sethostname */

    /* send()/recv() are sendto()/recvfrom() with a NULL address, and
     * the kernel handles both for AF_UNIX and AF_INET. MSG_NOSIGNAL is
     * a no-op here (no signals); MSG_PEEK, MSG_DONTWAIT and MSG_WAITALL
     * would change the semantics and are refused rather than ignored. */
    case 44: /* sendto(fd, buf, len, flags, dest_addr, addrlen) */
        if ((a4 & ~TUS_MSG_NOSIGNAL) != 0) {
            return TUS_EOPNOTSUPP;
        }
        return tus_raw(TUS_SYS_SENDTO, a1, a2, a3, a4, a5, a6);
    case 45: /* recvfrom(fd, buf, len, flags, src_addr, addrlen) */
        if ((a4 & ~TUS_MSG_NOSIGNAL) != 0) {
            return TUS_EOPNOTSUPP;
        }
        return tus_raw(TUS_SYS_RECVFROM, a1, a2, a3, a4, a5, a6);
    case 52: return tus_raw(TUS_SYS_GETPEERNAME, a1, a2, a3, 0, 0, 0);
    case 54: return tus_raw(TUS_SYS_SETSOCKOPT, a1, a2, a3, a4, a5, 0);
    case 55: return tus_raw(TUS_SYS_GETSOCKOPT, a1, a2, a3, a4, a5, 0);
    case 72: return tus_raw(TUS_SYS_FCNTL, a1, a2, a3, 0, 0, 0); /* fcntl */
    case 318: /* getrandom(buf, len, flags) */
        return tus_raw(TUS_SYS_GETRANDOM, a1, a2, a3, 0, 0, 0);
    case 61: /* wait4(pid, status, options, rusage) */
        if (a4 != 0) {
            return TUS_EOPNOTSUPP; /* no rusage accounting */
        }
        return tus_raw(TUS_SYS_WAITPID, a1, a2, a3, 0, 0, 0);
    case 332: /* statx(fd, path, flags, mask, statxbuf) */
        return tus_raw(TUS_SYS_STATX, a1, a2, a3, a4, a5, 0);
    case 5: { /* fstat(fd, kstatbuf) - see struct tus_kstat above for
               * why this exists: fstat() ends up here, not at 332. */
        struct tus_statx_mini stx;
        long ret = tus_raw(TUS_SYS_STATX, a1, (long)"", 0x1800, 0x7ff,
                           (long)&stx, 0);
        if (ret) {
            return ret;
        }
        statx_mini_to_kstat(&stx, (struct tus_kstat *)a2);
        return 0;
    }
    /* fstatat_kstat() in musl's fstatat.c (reached from stat(),
     * lstat() and fstatat() with a real path - x86_64's kstat time
     * fields are already 64-bit, so it never takes the SYS_statx
     * branch it tries first) picks one of three raw syscalls
     * depending on fd/flag: SYS_stat for a plain absolute-path
     * stat(), SYS_lstat likewise for lstat(), and SYS_fstatat
     * (aliased to SYS_newfstatat) for everything else, e.g. a real
     * dirfd or fstatat()'s own flags. TUS has no symlinks, so lstat()
     * needs nothing lstat-specific, and no cwd resolution, so a dirfd
     * other than AT_FDCWD is not meaningfully different from a bare
     * absolute path - all three collapse to the same path-based
     * TUS_SYS_STATX lookup vfs_stat_path() (see sys_statx() in
     * kernel/syscall/syscall.c) implements. */
    case 4:   /* stat(path, kstatbuf) */
    case 6: { /* lstat(path, kstatbuf) */
        struct tus_statx_mini stx;
        long ret = tus_raw(TUS_SYS_STATX, 0, a1, 0, 0x7ff, (long)&stx, 0);
        if (ret) {
            return ret;
        }
        statx_mini_to_kstat(&stx, (struct tus_kstat *)a2);
        return 0;
    }
    case 262: { /* newfstatat(dirfd, path, kstatbuf, flags) */
        struct tus_statx_mini stx;
        long ret = tus_raw(TUS_SYS_STATX, a1, a2, a4, 0x7ff, (long)&stx, 0);
        if (ret) {
            return ret;
        }
        statx_mini_to_kstat(&stx, (struct tus_kstat *)a3);
        return 0;
    }

    /* Emulated in userspace. */
    case 28:  return 0;   /* madvise: allocator hint, safe to ignore */
    case 95:  return 0;   /* umask: TUS has no permission bits */
    case 35:  return tus_sleep_from_timespec((const void *)a1); /* nanosleep(req, rem) */
    case 96: { /* gettimeofday(tv, tz) */
        if (a1 == 0) {
            return 0;
        }
        long secs = tus_raw(TUS_SYS_CLOCK, 0, 0, 0, 0, 0, 0);
        if (secs < 0) {
            return secs;
        }
        struct tus_timeval *tv = (struct tus_timeval *)a1;
        tv->tv_sec = secs;
        tv->tv_usec = 0;
        return 0;
    }
    case 228: /* clock_gettime(clk, ts) */
        return tus_clock_gettime(a1, (const void *)a2);
    case 403: /* clock_gettime64(clk, ts) */
        return tus_clock_gettime(a1, (const void *)a2);
    case 115: /* clock_nanosleep_time64(clk, flags, req, rem) */
    case 230: /* clock_nanosleep(clk, flags, req, rem) */
        if (a2 != 0) {
            return TUS_EFAULT; /* no TIMER_ABSTIME support */
        }
        return tus_sleep_from_timespec((const void *)a3);

    /* Not implemented by TUS yet: behave like an unknown syscall. */
    default:
        return TUS_ENOSYS;
    }
}
