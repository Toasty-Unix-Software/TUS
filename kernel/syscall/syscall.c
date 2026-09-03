/*
 * syscall.c - system call dispatch
 *
 * The IDT gate at vector 0x80 enters syscall_entry(), a tiny naked
 * stub that pushes the seven argument registers and calls
 * syscall_dispatch() with a pointer to them. The dispatch table maps
 * POSIX-style numbers onto the VFS, timer and process APIs.
 *
 * Ring-3 enforcement: the stub also records the caller's CS (pushed
 * by the CPU as part of the interrupt frame), so the dispatcher knows
 * whether the call came from user mode. User callers may only pass
 * pointers into the canonical user half; anything in the kernel half
 * is rejected with -EFAULT. (The kernel shell itself calls the same
 * ABI from ring 0 and is exempt.)
 */

#include "syscall.h"

#include "../mm/kmalloc.h"

#include <stdbool.h>

#include "../arch/x86_64/io.h"
#include "../core/bootinfo.h"
#include "../core/errno.h"
#include "../core/console.h"
#include "../drivers/pit/pit.h"
#include "../elf/tus_elf.h"
#include "../mm/pmm.h"
#include "../mm/vmm.h"
#include "../highx/highx.h"
#include "../net/socket.h"
#include "../net/dhcp.h"
#include "../net/dns.h"
#include "../net/ip.h"
#include "../net/ipv6.h"
#include "../net/netif.h"
#include "../net/tcp.h"
#include "../core/random.h"
#include "../core/hostname.h"
#include "../drivers/rtc/rtc.h"
#include "../drivers/rtl8139/rtl8139.h"
#include "../drivers/serial/serial.h"
#include "../core/panic_screen.h"

#include <tusnet.h>
#include <tusvideo.h>
#include <tusinput.h>
#include "../drivers/keymap/keymap.h"
#include "../drivers/fb/fb.h"
#include "../drivers/vbe/vbe.h"
#include "../vfs/devices.h"
#include "../sched/sched.h"
#include "../sched/cap.h"
#include "../term/term.h"
#include "../vfs/vfs.h"
#include "../core/klib.h"

/* Register image as pushed by syscall_entry() (lowest address first).
 * The caller's CS is passed as a separate argument to the dispatcher
 * (it lives in the CPU-pushed frame, not among the registers). */
struct syscall_regs {
    uint64_t rax; /* syscall number */
    uint64_t rdi; /* arg 1 */
    uint64_t rsi; /* arg 2 */
    uint64_t rdx; /* arg 3 */
    uint64_t r10; /* arg 4 */
    uint64_t r8;  /* arg 5 */
    uint64_t r9;  /* arg 6 (unused for now) */
};

/* Vector 0x80 gate: save the registers, dispatch, restore, iretq.
 * After the seven pushes, the CPU-pushed frame starts at %rsp+56:
 * RIP at +56, CS at +64 (valid for both ring-3 and ring-0 callers).
 * CS is handed to the dispatcher in %rsi. */
__attribute__((naked)) void syscall_entry(void) {
    __asm__ volatile(
        "push %r9\n\t"
        "push %r8\n\t"
        "push %r10\n\t"
        "push %rdx\n\t"
        "push %rsi\n\t"
        "push %rdi\n\t"
        "push %rax\n\t"
        "mov %rsp, %rdi\n\t"
        "mov 64(%rsp), %rsi\n\t"   /* CS from the interrupt frame */
        "lea 56(%rsp), %rdx\n\t"   /* pointer to the interrupt frame */
        "call syscall_dispatch\n\t"
        "add $56, %rsp\n\t"
        "iretq\n");
}

/* Upper bound of the canonical user half (0x00007fffffffffff). The
 * kernel half starts at 0xffff800000000000; non-canonical addresses
 * sit in between and are rejected too. */
#define USER_HALF_MAX 0x00007fffffffffffull

/* True when a user-mode caller may reference [ptr, ptr+len). Ring-0
 * callers (the shell) pass kernel pointers freely. */
static bool access_ok(bool from_user, const void *ptr, size_t len) {
    if (!from_user) {
        return true;
    }
    uint64_t a = (uint64_t)(uintptr_t)ptr;
    uint64_t e = a + len;
    return e >= a && e <= USER_HALF_MAX;
}

/* arch_prctl operations (Linux-compatible subset). */
#define ARCH_SET_FS 0x1002
#define ARCH_GET_FS 0x1003

/* IA32_FS_BASE model-specific register. */
#define MSR_FS_BASE 0xC0000100

/* mmap flags (Linux subset understood by userspace). */
#define MAP_ANONYMOUS 0x20

/* Initial address for anonymous mmap allocations. Every task has its
 * own address space, so all tasks can use the same cursor; it grows
 * upward from here (ELF images live at 0x10000000, the user stack at
 * 0x60000000, so this range is free). */
#define MMAP_CURSOR_START 0x40000000ull

/* SIGKILL(9)/SIGSTOP(19) as sig_blocked bits: never blockable,
 * enforced wherever sigprocmask()/sigsuspend() write the mask. */
#define SIG_MASK_9_19_TUS ((1ULL << 8) | (1ULL << 18))

/* ------------------------------------------------------------------ */

static long sys_munmap(long addr, long len);

/* Bounds for copy_user_argv() below. A real compiler driver's argv is
 * much bigger than a typical shell command: PCC's cc passes ~90
 * predefine flags (-D__SIZEOF_INT__ and friends) to cpp for a single
 * file. 128 args of up to 255 chars comfortably covers that. */
#define MAX_ARGV     128
#define MAX_ARG_LEN  256

/* Copies argv out of user memory into one kmalloc'd block: up to
 * MAX_ARGV pointers into MAX_ARGV slots of MAX_ARG_LEN bytes each
 * (heap, not the 16 KiB kernel stack - shared by execve and spawn so
 * the bound only has to live in one place). On success *out_argv is
 * heap-owned and the caller must kfree() it; returns argc, or a
 * negative errno with *out_argv left untouched. */
static long copy_user_argv(bool from_user, char **uargv, char ***out_argv) {
    /* One block: (MAX_ARGV + 1) pointers, followed by MAX_ARGV string
     * slots of MAX_ARG_LEN bytes - a single kmalloc/kfree pair. */
    size_t ptrs_size = sizeof(char *) * (MAX_ARGV + 1);
    char **argv = kmalloc(ptrs_size + (size_t)MAX_ARGV * MAX_ARG_LEN);
    if (argv == NULL) {
        return -ENOMEM;
    }
    char *arg_data = (char *)argv + ptrs_size;

    int argc = 0;
    if (uargv != NULL) {
        while (argc < MAX_ARGV) {
            if (!access_ok(from_user, (void *)(uargv + argc),
                           sizeof(char *))) {
                kfree(argv);
                return -EFAULT;
            }
            const char *s = uargv[argc];
            if (s == NULL) {
                break;
            }
            if (!access_ok(from_user, s, 1)) {
                kfree(argv);
                return -EFAULT;
            }
            char *slot = arg_data + (size_t)argc * MAX_ARG_LEN;
            size_t j = 0;
            while (j + 1 < MAX_ARG_LEN) {
                char c = s[j];
                if (c == '\0') {
                    break;
                }
                slot[j++] = c;
            }
            slot[j] = '\0';
            argv[argc] = slot;
            argc++;
        }
    }
    argv[argc] = NULL;
    *out_argv = argv;
    return argc;
}

/* execve(path, argv, envp): replace the calling task with the ELF at
 * `path`. The path is copied out of user memory first (we are running
 * in the caller's address space, so a bounded direct read is fine
 * after access_ok). Only user-mode callers may exec; the shell spawns
 * tasks with elf_exec() instead. On success the rewritten IRETQ frame
 * lands in the new program. */
static long sys_execve(struct syscall_regs *r, bool from_user,
                       uint64_t frame_rsp) {
    if (!from_user) {
        return -EPERM;
    }
    if (!access_ok(from_user, (const void *)r->rdi, 1)) {
        return -EFAULT;
    }

    char path[256];
    size_t i = 0;
    const char *upath = (const char *)r->rdi;
    while (i + 1 < sizeof(path)) {
        char c = upath[i];
        if (c == '\0') {
            break;
        }
        path[i++] = c;
    }
    path[i] = '\0';
    if (i == 0) {
        return -ENOENT;
    }

    char **argv;
    long argc = copy_user_argv(from_user, (char **)r->rsi, &argv);
    if (argc < 0) {
        return argc;
    }

    /* execve's argv includes the program name; elf_exec_current
     * prepends `path` as argv[0] itself, so skip it. */
    long ret;
    if (argc > 0) {
        ret = elf_exec_current(path, (int)argc - 1, &argv[1], frame_rsp);
    } else {
        ret = elf_exec_current(path, 0, argv, frame_rsp);
    }
    kfree(argv);
    if (ret >= 0) {
        /* Real close-on-exec, now that the new image is committed
         * (a failed exec above leaves fds - and everything else -
         * unchanged, per elf_exec_current's own contract). The fd
         * table is task-scoped, not image-scoped, so it survived the
         * swap; this is what a CLOEXEC-marked fd (fcntl F_SETFD
         * above) was promising all along. See F_SETFD's comment for
         * why this specific gap mattered: it's what a fork+exec
         * error-detection pipe (every POSIX shell, ksh included)
         * depends on to unblock the parent's read() on successful
         * exec. */
        struct task *cur = sched_current();
        if (cur != NULL) {
            for (int fd = 0; fd < VFS_MAX_FDS; fd++) {
                if (cur->fd_cloexec[fd]) {
                    vfs_close(fd);
                    cur->fd_cloexec[fd] = false;
                }
            }
        }
    }
    return ret;
}

/* spawn(path, argv): load the ELF at `path` as a NEW ring-3 task and
 * return its pid. Unlike execve the caller keeps running, which is
 * what a window manager needs to launch applications: tusWM calls
 * this and stays alive to manage the window that shows up.
 *
 * The strings are copied out of user memory first (same bounded copy
 * execve does), so the loader never dereferences a user pointer. */
static long sys_spawn(struct syscall_regs *r, bool from_user) {
    if (!access_ok(from_user, (const void *)r->rdi, 1)) {
        return -EFAULT;
    }

    char path[256];
    size_t i = 0;
    const char *upath = (const char *)r->rdi;
    while (i + 1 < sizeof(path)) {
        char c = upath[i];
        if (c == '\0') {
            break;
        }
        path[i++] = c;
    }
    path[i] = '\0';
    if (i == 0) {
        return -ENOENT;
    }

    char **argv;
    long argc = copy_user_argv(from_user, (char **)r->rsi, &argv);
    if (argc < 0) {
        return argc;
    }

    /* elf_exec prepends the path as argv[0] itself, so hand it the
     * arguments after the program name (like execve does). */
    long ret;
    if (argc > 0) {
        ret = elf_exec(path, (int)argc - 1, &argv[1]);
    } else {
        ret = elf_exec(path, 0, argv);
    }
    kfree(argv);
    return ret;
}

/* exit(status): terminate the current task and switch to the next.
 * Never returns. */
__attribute__((noreturn)) static long sys_exit(int status) {
    task_exit(status);
}

/* mmap(addr, len, prot, flags): anonymous private mapping of zeroed
 * pages inside the calling task's address space. If addr is 0 a free
 * region is chosen (per-task cursor); file-backed mappings are not
 * supported yet and return -ENODEV. Returns the mapping address or a
 * negative errno. */
/* Not static: kernel/syscall/linux_syscall.c's Linux-compat mmap(2)
 * (syscall #9) reuses this directly - Linux's MAP_ANONYMOUS is the
 * same bit value (0x20) and the anonymous-only restriction already
 * matches what a Linux-compat task needs. */
#define TUS_PROT_EXEC 0x4 /* matches Linux PROT_EXEC, kept in sync for linux_syscall.c's passthrough */

long sys_mmap(long addr, long len, long prot, long flags) {
    struct task *cur = sched_current();
    if (cur == NULL || len <= 0) {
        return -EINVAL;
    }
    if ((flags & MAP_ANONYMOUS) == 0) {
        return -EINVAL; /* file-backed mmap not implemented yet */
    }

    uint64_t start = (uint64_t)addr;
    uint64_t bytes = (uint64_t)len;
    if (start == 0) {
        start = cur->mmap_cur;
    }
    if ((start & 0xFFF) != 0) {
        return -EINVAL;
    }
    bytes = (bytes + 0xFFF) & ~0xFFFull;
    if (start + bytes > USER_HALF_MAX || start + bytes < start) {
        return -ENOMEM;
    }

    for (uint64_t page = start; page < start + bytes; page += 0x1000) {
        uint64_t frame = pmm_alloc_frame();
        if (frame == 0) {
            /* Roll back what we mapped so far. */
            sys_munmap(start, (long)(page - start));
            return -ENOMEM;
        }
        /* Fresh anonymous pages must read as zeros. */
        memset((void *)pmm_phys_to_virt(frame), 0, 4096);
        /* Anonymous mmap is data unless the caller explicitly asked
         * for PROT_EXEC (a real JIT/trampoline use case) - default to
         * NX so an unrelated caller's mmap'd buffer can't be used as
         * a landing pad for injected code. */
        uint64_t map_flags = VMM_PRESENT | VMM_WRITE | VMM_USER;
        if (!(prot & TUS_PROT_EXEC)) {
            map_flags |= VMM_NX;
        }
        if (vmm_map_page_in(cur->cr3, page, frame, map_flags) != 0) {
            pmm_free_frame(frame);
            sys_munmap(start, (long)(page - start));
            return -ENOMEM;
        }
    }
    cur->mmap_cur = start + bytes;
    return (long)start;
}

/* munmap(addr, len): unmap anonymous pages in the calling task's
 * space and return their frames to the PMM. */
static long sys_munmap(long addr, long len) {
    struct task *cur = sched_current();
    if (cur == NULL || len <= 0 || (addr & 0xFFF) != 0) {
        return -EINVAL;
    }
    uint64_t start = (uint64_t)addr;
    uint64_t bytes = ((uint64_t)len + 0xFFF) & ~0xFFFull;
    if (start + bytes > USER_HALF_MAX || start + bytes < start) {
        return -EINVAL;
    }
    for (uint64_t page = start; page < start + bytes; page += 0x1000) {
        uint64_t phys = vmm_translate_in(cur->cr3, page);
        if (phys == 0) {
            continue; /* not mapped: nothing to do */
        }
        vmm_unmap_page_in(cur->cr3, page);
        pmm_free_frame(phys & ~0xFFFull);
    }
    return 0;
}

/* arch_prctl(op, addr): thread-control operations. Only the FS base
 * (the thread pointer that the C library stores its TLS in) is
 * supported: ARCH_SET_FS writes the MSR immediately (the scheduler
 * reloads it on every task switch), ARCH_GET_FS reads it back. */
static long sys_arch_prctl(long op, long addr, bool from_user) {
    struct task *cur = sched_current();
    if (cur == NULL) {
        return -EINVAL;
    }
    if (op == ARCH_SET_FS) {
        cur->fs_base = (uint64_t)addr;
        wrmsr(MSR_FS_BASE, (uint64_t)addr);
        return 0;
    }
    if (op == ARCH_GET_FS) {
        if (!access_ok(from_user, (void *)addr, 8)) {
            return -EFAULT;
        }
        *(uint64_t *)(uintptr_t)addr = cur->fs_base;
        return 0;
    }
    return -EINVAL;
}

/* writev(fd, iov, count): scatter write. The iovec array lives in
 * user memory and each segment is validated before it is written. */
/* struct statx, matching musl's local definition in
 * sources/musl-1.2.6/src/stat/fstatat.c byte for byte - this is the
 * ABI musl's fstatat_statx() reads, not a TUS invention. */
struct tus_statx {
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
    uint32_t stx_rdev_major;
    uint32_t stx_rdev_minor;
    uint32_t stx_dev_major;
    uint32_t stx_dev_minor;
    uint64_t spare[14];
};

#define TUS_AT_EMPTY_PATH 0x1000
#define TUS_S_IFDIR 0040000
#define TUS_S_IFCHR 0020000
#define TUS_S_IFIFO 0010000
#define TUS_S_IFREG 0100000

/* statx(fd, path, flags, mask, statxbuf): TUS has no stat/fstat/
 * fstatat, so musl's fstat() always ends up here (see SYS_STATX in
 * syscall.h). Two cases: AT_EMPTY_PATH with a valid fd is what
 * musl's fstat() produces (path ignored, looked up via the fd
 * table); anything else is a real path-based lookup (bare stat(),
 * fstatat() with a real path) and goes through vfs_lookup() instead -
 * absolute paths only, same limitation as every other path syscall
 * here (vfs_open_mode, vfs_mkdir, ...), since TUS has no cwd
 * resolution at the syscall layer. */
static long sys_statx(struct syscall_regs *r, bool from_user) {
    long fd = (long)r->rdi;
    const char *upath = (const char *)r->rsi;
    int flags = (int)r->rdx;
    void *ubuf = (void *)r->r8;

    if (!access_ok(from_user, ubuf, sizeof(struct tus_statx))) {
        return -EFAULT;
    }

    size_t size = 0;
    uint32_t mode = 0;
    int type = 0;
    uint64_t ino = 0;
    long ret;
    if (flags & TUS_AT_EMPTY_PATH) {
        if (fd < 0) {
            return -ENOSYS;
        }
        ret = vfs_fstat(fd, &size, &mode, &type, &ino);
    } else {
        if (!access_ok(from_user, (const void *)upath, 1)) {
            return -EFAULT;
        }
        ret = vfs_stat_path(upath, &size, &mode, &type, &ino);
    }
    if (ret < 0) {
        return ret;
    }

    struct tus_statx stx;
    memset(&stx, 0, sizeof(stx));
    uint16_t ifmt;
    switch (type) {
    case VFS_DIR:
        ifmt = TUS_S_IFDIR;
        break;
    case VFS_DEVICE:
        ifmt = TUS_S_IFCHR;
        break;
    case VFS_PIPE:
        ifmt = TUS_S_IFIFO;
        break;
    default:
        ifmt = TUS_S_IFREG;
        break;
    }
    stx.stx_mask = 0x7ff; /* STATX_BASIC_STATS: every field below is filled */
    stx.stx_mode = ifmt | (uint16_t)(mode & 07777);
    stx.stx_nlink = 1;
    stx.stx_size = size;
    stx.stx_blksize = 4096;
    stx.stx_blocks = (size + 511) / 512;
    stx.stx_ino = ino;
    /* One constant "device" for TUS's whole tree (there is only ever
     * one - the rootfs tar plus device nodes) - stx_ino alone already
     * gives every node a distinct identity, which is what mattered
     * (see vfs_fstat()'s comment). */
    stx.stx_dev_major = 0;
    stx.stx_dev_minor = 1;

    memcpy(ubuf, &stx, sizeof(stx));
    return 0;
}

static long sys_writev(long fd, void *iov, long count, bool from_user) {
    if (count < 0 || count > 256) {
        return -EINVAL;
    }
    size_t bytes = (size_t)count * 16; /* struct iovec { base; len; } */
    if (!access_ok(from_user, iov, bytes)) {
        return -EFAULT;
    }
    long total = 0;
    for (long i = 0; i < count; i++) {
        uint64_t base = ((uint64_t *)iov)[2 * i];
        uint64_t len = ((uint64_t *)iov)[2 * i + 1];
        if (!access_ok(from_user, (void *)base, (size_t)len)) {
            return total > 0 ? total : -EFAULT;
        }
        long n = vfs_write(fd, (const void *)base, (size_t)len);
        if (n < 0) {
            return total > 0 ? total : n;
        }
        total += n;
    }
    return total;
}

/*
 * readv(fd, iov, count): the mirror of writev, and the one musl's
 * stdio actually depends on. Every buffered read - fgets(), fread(),
 * fscanf() - goes through __stdio_read, which issues readv with two
 * buffers: the caller's, and the FILE's own read-ahead. Without it
 * those calls quietly return nothing at all, which looks like an
 * empty file rather than a missing system call.
 *
 * A short read stops the walk: filling later buffers after an earlier
 * one came up short would put a hole in the middle of the data.
 */
static long sys_readv(long fd, void *iov, long count, bool from_user) {
    if (count < 0 || count > 256) {
        return -EINVAL;
    }
    size_t bytes = (size_t)count * 16; /* struct iovec { base; len; } */
    if (!access_ok(from_user, iov, bytes)) {
        return -EFAULT;
    }
    long total = 0;
    for (long i = 0; i < count; i++) {
        uint64_t base = ((uint64_t *)iov)[2 * i];
        uint64_t len = ((uint64_t *)iov)[2 * i + 1];
        if (len == 0) {
            continue;
        }
        if (!access_ok(from_user, (void *)base, (size_t)len)) {
            return total > 0 ? total : -EFAULT;
        }
        long n = vfs_read(fd, (void *)base, (size_t)len);
        if (n < 0) {
            return total > 0 ? total : n;
        }
        total += n;
        if ((uint64_t)n < len) {
            break; /* end of file, or all that was ready */
        }
    }
    return total;
}

/* ---- Unix domain sockets ---- */

/* Copy the path out of a user struct sockaddr_un. Abstract (leading
 * NUL) and unnamed addresses are rejected: TUS only supports sockets
 * that live in the filesystem. */
static long sockaddr_path(bool from_user, const void *uaddr, long addrlen,
                          char *out, size_t size) {
    if (uaddr == NULL) {
        return -EFAULT;
    }
    if (addrlen <= (long)sizeof(uint16_t) ||
        addrlen > (long)sizeof(struct sockaddr_un)) {
        return -EINVAL;
    }
    if (!access_ok(from_user, uaddr, (size_t)addrlen)) {
        return -EFAULT;
    }

    const struct sockaddr_un *sa = (const struct sockaddr_un *)uaddr;
    if (sa->sun_family != AF_UNIX) {
        return -EAFNOSUPPORT;
    }
    size_t max = (size_t)addrlen - sizeof(uint16_t);
    if (max > UNIX_PATH_MAX) {
        max = UNIX_PATH_MAX;
    }
    size_t i = 0;
    while (i < max && i + 1 < size && sa->sun_path[i] != '\0') {
        out[i] = sa->sun_path[i];
        i++;
    }
    out[i] = '\0';
    if (i == 0) {
        return -EINVAL; /* unnamed or abstract socket */
    }
    return 0;
}

/* Write a struct sockaddr_un back to the caller (accept/getsockname).
 * A NULL address pointer means the caller does not want it. As POSIX
 * requires, *ulen receives the untruncated length even when the copy
 * had to be cut short. */
static long sockaddr_store(bool from_user, void *uaddr, void *ulen,
                           const char *path) {
    if (uaddr == NULL || ulen == NULL) {
        return 0;
    }
    if (!access_ok(from_user, ulen, sizeof(uint32_t))) {
        return -EFAULT;
    }
    uint32_t cap = *(uint32_t *)ulen;

    struct sockaddr_un sa;
    memset(&sa, 0, sizeof(sa));
    sa.sun_family = AF_UNIX;
    size_t len = strlen(path);
    if (len >= UNIX_PATH_MAX) {
        len = UNIX_PATH_MAX - 1;
    }
    memcpy(sa.sun_path, path, len);

    /* An unnamed socket is just the family field, like Linux. */
    uint32_t full = (uint32_t)sizeof(uint16_t);
    if (len > 0) {
        full += (uint32_t)len + 1;
    }
    uint32_t n = cap < full ? cap : full;
    if (n > 0 && !access_ok(from_user, uaddr, n)) {
        return -EFAULT;
    }
    memcpy(uaddr, &sa, n);
    *(uint32_t *)ulen = full;
    return 0;
}

static long sys_socket(long domain, long type, long protocol) {
    long err = 0;

    if (domain == AF_UNIX) {
        struct unix_sock *s = unix_sock_create((int)domain, (int)type,
                                               (int)protocol, &err);
        if (s == NULL) {
            return err;
        }
        return vfs_sock_install(s);
    } else if (domain == AF_INET) {
        struct inet_sock *s = inet_sock_create((int)type, (int)protocol, &err);
        if (s == NULL) {
            return err;
        }
        return vfs_inet_sock_install(s);
    }

    return -EAFNOSUPPORT;
}

/* ---- AF_INET addresses ----
 *
 * struct sockaddr_in is byte-identical to musl's, and its sin_addr and
 * sin_port are already in network byte order - the same order the
 * stack works in - so neither crossing needs a swap. */
static long sockaddr_in_get(bool from_user, const void *uaddr, long addrlen,
                            uint32_t *ip, uint16_t *port) {
    if (uaddr == NULL) {
        return -EFAULT;
    }
    if (addrlen < (long)sizeof(struct sockaddr_in)) {
        return -EINVAL;
    }
    if (!access_ok(from_user, uaddr, sizeof(struct sockaddr_in))) {
        return -EFAULT;
    }

    const struct sockaddr_in *sa = (const struct sockaddr_in *)uaddr;
    if (sa->sin_family != AF_INET) {
        return -EAFNOSUPPORT;
    }
    *ip = sa->sin_addr;
    *port = ntohs(sa->sin_port);
    return 0;
}

static long sockaddr_in_store(bool from_user, void *uaddr, void *ulen,
                              uint32_t ip, uint16_t port) {
    if (uaddr == NULL || ulen == NULL) {
        return 0;
    }
    if (!access_ok(from_user, ulen, sizeof(uint32_t))) {
        return -EFAULT;
    }
    uint32_t cap = *(uint32_t *)ulen;

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr = ip;
    sa.sin_port = htons(port);

    uint32_t full = (uint32_t)sizeof(sa);
    uint32_t n = cap < full ? cap : full;
    if (n > 0 && !access_ok(from_user, uaddr, n)) {
        return -EFAULT;
    }
    memcpy(uaddr, &sa, n);
    *(uint32_t *)ulen = full;
    return 0;
}

static long sys_bind(long fd, const void *uaddr, long addrlen, bool from_user) {
    struct inet_sock *inet = vfs_fd_inet_sock(fd);
    if (inet != NULL) {
        uint32_t ip = 0;
        uint16_t port = 0;
        long ret = sockaddr_in_get(from_user, uaddr, addrlen, &ip, &port);
        if (ret < 0) {
            return ret;
        }
        return inet_sock_bind(inet, ip, port);
    }

    struct unix_sock *s = vfs_fd_sock(fd);
    if (s == NULL) {
        return -ENOTSOCK;
    }
    char path[UNIX_PATH_MAX];
    long ret = sockaddr_path(from_user, uaddr, addrlen, path, sizeof(path));
    if (ret < 0) {
        return ret;
    }
    return unix_sock_bind(s, path);
}

static long sys_connect(long fd, const void *uaddr, long addrlen,
                        bool from_user) {
    struct inet_sock *inet = vfs_fd_inet_sock(fd);
    if (inet != NULL) {
        uint32_t ip = 0;
        uint16_t port = 0;
        long ret = sockaddr_in_get(from_user, uaddr, addrlen, &ip, &port);
        if (ret < 0) {
            return ret;
        }
        return inet_sock_connect(inet, ip, port);
    }

    struct unix_sock *s = vfs_fd_sock(fd);
    if (s == NULL) {
        return -ENOTSOCK;
    }
    char path[UNIX_PATH_MAX];
    long ret = sockaddr_path(from_user, uaddr, addrlen, path, sizeof(path));
    if (ret < 0) {
        return ret;
    }
    return unix_sock_connect(s, path);
}

static long sys_accept(long fd, void *uaddr, void *ulen, bool from_user) {
    struct inet_sock *inet = vfs_fd_inet_sock(fd);
    if (inet != NULL) {
        long err = 0;
        struct inet_sock *client = inet_sock_accept(inet, &err);
        if (client == NULL) {
            return err ? err : -EAGAIN;
        }

        uint32_t ip = 0;
        uint16_t port = 0;
        inet_sock_getname(client, &ip, &port, true);
        long ret = sockaddr_in_store(from_user, uaddr, ulen, ip, port);
        if (ret < 0) {
            inet_sock_unref(client);
            return ret;
        }
        return vfs_inet_sock_install(client);
    }

    struct unix_sock *s = vfs_fd_sock(fd);
    if (s == NULL) {
        return -ENOTSOCK;
    }
    long err = 0;
    struct unix_sock *client = unix_sock_accept(s, &err);
    if (client == NULL) {
        return err;
    }

    /* The peer of an accepted AF_UNIX connection is unnamed unless the
     * client bound a path of its own; report what it has. */
    char path[UNIX_PATH_MAX];
    if (unix_sock_getname(client, path, sizeof(path)) < 0) {
        path[0] = '\0';
    }
    long ret = sockaddr_store(from_user, uaddr, ulen, path);
    if (ret < 0) {
        unix_sock_unref(client);
        return ret;
    }
    return vfs_sock_install(client);
}

static long sys_socketpair(long domain, long type, long protocol, void *usv,
                           bool from_user) {
    if (!access_ok(from_user, usv, 2 * sizeof(int))) {
        return -EFAULT;
    }
    /* Validate the arguments on a throwaway socket so socketpair
     * rejects exactly what socket() rejects. */
    long err = 0;
    struct unix_sock *probe = unix_sock_create((int)domain, (int)type,
                                               (int)protocol, &err);
    if (probe == NULL) {
        return err;
    }
    unix_sock_unref(probe);

    struct unix_sock *a = NULL, *b = NULL;
    long ret = unix_sock_pair(&a, &b);
    if (ret < 0) {
        return ret;
    }
    long fd0 = vfs_sock_install(a);
    if (fd0 < 0) {
        unix_sock_unref(b);
        return fd0;
    }
    long fd1 = vfs_sock_install(b);
    if (fd1 < 0) {
        vfs_close(fd0);
        return fd1;
    }
    ((int *)usv)[0] = (int)fd0;
    ((int *)usv)[1] = (int)fd1;
    return 0;
}

/* getsockname() and getpeername() differ only in which end they
 * report, so both land here. */
static long sys_getname(long fd, void *uaddr, void *ulen, bool from_user,
                        bool peer) {
    struct inet_sock *inet = vfs_fd_inet_sock(fd);
    if (inet != NULL) {
        uint32_t ip = 0;
        uint16_t port = 0;
        inet_sock_getname(inet, &ip, &port, peer);
        return sockaddr_in_store(from_user, uaddr, ulen, ip, port);
    }

    struct unix_sock *s = vfs_fd_sock(fd);
    if (s == NULL) {
        return -ENOTSOCK;
    }
    char path[UNIX_PATH_MAX];
    if (unix_sock_getname(s, path, sizeof(path)) < 0) {
        path[0] = '\0';
    }
    return sockaddr_store(from_user, uaddr, ulen, path);
}

static long sys_getsockname(long fd, void *uaddr, void *ulen, bool from_user) {
    return sys_getname(fd, uaddr, ulen, from_user, false);
}

static long sys_getpeername(long fd, void *uaddr, void *ulen, bool from_user) {
    return sys_getname(fd, uaddr, ulen, from_user, true);
}

/* ---- datagram send/receive ---- */

static long sys_sendto(struct syscall_regs *r, bool from_user) {
    long fd = (long)r->rdi;
    const void *buf = (const void *)r->rsi;
    size_t len = (size_t)r->rdx;
    const void *uaddr = (const void *)r->r8;
    long addrlen = (long)r->r9;

    if (!access_ok(from_user, buf, len)) {
        return -EFAULT;
    }

    struct inet_sock *inet = vfs_fd_inet_sock(fd);
    if (inet == NULL) {
        /* Without an address this is write(), which is what send() on
         * an AF_UNIX stream has always been. */
        if (uaddr != NULL) {
            return -EOPNOTSUPP;
        }
        return vfs_write(fd, buf, len);
    }

    if (uaddr == NULL) {
        return inet_sock_write(inet, buf, len);
    }

    uint32_t ip = 0;
    uint16_t port = 0;
    long ret = sockaddr_in_get(from_user, uaddr, addrlen, &ip, &port);
    if (ret < 0) {
        return ret;
    }
    return inet_sock_sendto(inet, buf, len, ip, port);
}

static long sys_recvfrom(struct syscall_regs *r, bool from_user) {
    long fd = (long)r->rdi;
    void *buf = (void *)r->rsi;
    size_t len = (size_t)r->rdx;
    void *uaddr = (void *)r->r8;
    void *ulen = (void *)r->r9;

    if (!access_ok(from_user, buf, len)) {
        return -EFAULT;
    }

    struct inet_sock *inet = vfs_fd_inet_sock(fd);
    if (inet == NULL) {
        if (uaddr != NULL) {
            return -EOPNOTSUPP;
        }
        return vfs_read(fd, buf, len);
    }

    uint32_t ip = 0;
    uint16_t port = 0;
    long n = inet_sock_recvfrom(inet, buf, len, &ip, &port, 30000);
    if (n < 0) {
        return n;
    }
    if (uaddr != NULL) {
        long ret = sockaddr_in_store(from_user, uaddr, ulen, ip, port);
        if (ret < 0) {
            return ret;
        }
    }
    return n;
}

/* ---- socket options ----
 *
 * Only the options that change behaviour TUS actually has are acted
 * on. The rest are accepted and ignored rather than refused, because a
 * program that cannot set SO_KEEPALIVE usually gives up entirely, and
 * its absence changes nothing about whether the connection works. */
#define SOL_SOCKET   1
#define SO_REUSEADDR 2
#define SO_ERROR     4
#define SO_RCVBUF    8
#define SO_SNDBUF    7

static long sys_setsockopt(long fd, long level, long opt, const void *val,
                           long len, bool from_user) {
    (void)level; (void)opt;
    if (val != NULL && !access_ok(from_user, val, (size_t)len)) {
        return -EFAULT;
    }
    if (vfs_fd_inet_sock(fd) == NULL && vfs_fd_sock(fd) == NULL) {
        return -ENOTSOCK;
    }
    return 0;
}

static long sys_getsockopt(long fd, long level, long opt, void *val,
                           void *ulen, bool from_user) {
    if (vfs_fd_inet_sock(fd) == NULL && vfs_fd_sock(fd) == NULL) {
        return -ENOTSOCK;
    }
    if (val == NULL || ulen == NULL) {
        return -EFAULT;
    }
    if (!access_ok(from_user, ulen, sizeof(uint32_t))) {
        return -EFAULT;
    }
    uint32_t cap = *(uint32_t *)ulen;
    if (cap < sizeof(int) || !access_ok(from_user, val, sizeof(int))) {
        return -EINVAL;
    }

    int result = 0;
    if (level == SOL_SOCKET && opt == SO_RCVBUF) {
        result = TCP_RX_BUF_SIZE;
    } else if (level == SOL_SOCKET && opt == SO_SNDBUF) {
        result = TCP_TX_BUF_SIZE;
    }
    /* SO_ERROR reports 0: a failed connect() already returned its
     * errno directly, because TUS has no non-blocking connect. */

    *(int *)val = result;
    *(uint32_t *)ulen = sizeof(int);
    return 0;
}

/* ---- fcntl ---- */

#define F_DUPFD 0
#define F_GETFD 1
#define F_SETFD 2
#define F_GETFL 3
#define F_SETFL 4
#define F_DUPFD_CLOEXEC 1030
#define O_NONBLOCK_FLAG 04000
#define TUS_FD_CLOEXEC 1

static long sys_fcntl(long fd, long cmd, long arg) {
    struct inet_sock *inet = vfs_fd_inet_sock(fd);

    switch (cmd) {
    case F_DUPFD:
    case F_DUPFD_CLOEXEC: {
        long newfd = vfs_fcntl_dupfd(fd, arg);
        /* A dup'd fd starts with its OWN cloexec bit clear regardless
         * of the source fd's (POSIX) - only _CLOEXEC variant sets it. */
        if (newfd >= 0) {
            struct task *cur = sched_current();
            if (cur != NULL && newfd < VFS_MAX_FDS) {
                cur->fd_cloexec[newfd] = (cmd == F_DUPFD_CLOEXEC);
            }
        }
        return newfd;
    }
    case F_GETFD: {
        /* Must fail -EBADF for an fd that was never opened (or already
         * closed), not just succeed unconditionally: ksh's startup
         * closes every fd above stderr by probing fcntl(fd, F_GETFD)
         * upward until it errors - with no error ever returned, that
         * loop never stops (found by booting a real ksh binary on
         * TUS: RDI/RBP climbed past 900,000 with no syscall trace
         * gap, i.e. a tight in-kernel-looking spin that was actually
         * this fcntl call always "succeeding"). */
        long flags = vfs_fd_flags(fd);
        if (flags < 0) {
            return flags;
        }
        struct task *cur = sched_current();
        if (cur != NULL && fd >= 0 && fd < VFS_MAX_FDS && cur->fd_cloexec[fd]) {
            return TUS_FD_CLOEXEC;
        }
        return 0;
    }
    case F_SETFD: {
        /* Same existence check as F_GETFD above. Real close-on-exec:
         * without this, ksh's (and every POSIX shell's) fork+exec
         * error-detection pipe hangs forever - it works by the child
         * exec()ing with the pipe's write end marked CLOEXEC, so a
         * successful exec silently closes it and the parent's read()
         * sees EOF; when TUS never actually closed it, that read()
         * blocked forever waiting for an EOF that could never come.
         * The actual close happens in sys_execve() below. */
        long flags = vfs_fd_flags(fd);
        if (flags < 0) {
            return flags;
        }
        struct task *cur = sched_current();
        if (cur != NULL && fd >= 0 && fd < VFS_MAX_FDS) {
            cur->fd_cloexec[fd] = (arg & TUS_FD_CLOEXEC) != 0;
        }
        return 0;
    }
    case F_GETFL:
        return vfs_fd_flags(fd);
    case F_SETFL:
        if (inet != NULL) {
            inet_sock_set_nonblock(inet, (arg & O_NONBLOCK_FLAG) != 0);
        }
        return vfs_fd_set_flags(fd, arg);
    default:
        return -EINVAL;
    }
}

/* ---- waitpid ----
 *
 * Block until one child has exited and report how. A zombie's slot
 * survives until the scheduler reuses it, which is what makes the
 * status still there to collect. `pid == -1` (ksh's plain `wait`
 * with no arguments waits for ANY child) scans the task table for a
 * zombie whose ppid is the caller - there is no separate "reap
 * anything" bookkeeping, since a zombie already stays in the table
 * exactly for this. `pid == 0` (any child in the caller's own
 * process group) is not supported - real pid targets and -1 cover
 * every case TUS actually exercises. */
static long sys_waitpid(long pid, void *ustatus, long options,
                        bool from_user) {
    #define WNOHANG 1

    if (pid == 0 || pid < -1) {
        return -ECHILD; /* process-group targets: not supported */
    }
    if (ustatus != NULL && !access_ok(from_user, ustatus, sizeof(int))) {
        return -EFAULT;
    }

    struct task *cur = sched_current();
    uint32_t caller_pid = cur != NULL ? cur->pid : 0;

    for (;;) {
        int status = 0;
        long reaped_pid = -1;

        if (pid == -1) {
            uint32_t zpid = sched_find_zombie_child(caller_pid);
            if (zpid != 0 && sched_task_reap(zpid, &status) == 1) {
                reaped_pid = zpid;
            } else if (!sched_has_child(caller_pid)) {
                return -ECHILD; /* no children at all, living or dead */
            }
        } else {
            int r = sched_task_reap((uint32_t)pid, &status);
            if (r < 0) {
                return -ECHILD;
            }
            if (r == 1) {
                reaped_pid = pid;
            }
        }

        if (reaped_pid >= 0) {
            if (ustatus != NULL) {
                /* wait(2)'s status has two incompatible layouts and
                 * nothing in the raw int says which one applies - a
                 * real exit(2) encodes the code in the high byte
                 * (WIFEXITED/WEXITSTATUS), a signal death encodes the
                 * signal number directly in the low 7 bits
                 * (WIFSIGNALED/WTERMSIG). TUS's own exit_status is
                 * always "128 + signal" for the latter (matching what
                 * a shell's $? already shows), which
                 * sched_task_was_signaled() is what tells apart from
                 * a real program that happened to exit(143). */
                if (sched_task_was_signaled((uint32_t)reaped_pid)) {
                    *(int *)ustatus = status - 128; /* WTERMSIG: low byte, no shift */
                } else {
                    *(int *)ustatus = (status & 0xff) << 8; /* WEXITSTATUS */
                }
            }
            return reaped_pid;
        }
        if (options & WNOHANG) {
            return 0;
        }
        if (sched_signal_pending()) {
            return -EINTR;
        }
        hlt();
    }
}

/* ---- netctl ---- */

static long sys_netctl(long op, void *arg, long len, bool from_user) {
    if (op == NETCTL_DHCP) {
        struct task *cur = sched_current();
        if (from_user && !has_cap(cur, CAP_NET_ADMIN)) {
            return -EPERM;
        }
        return dhcp_configure() == 0 ? 0 : -ETIMEDOUT;
    }

    if (arg == NULL || !access_ok(from_user, arg, (size_t)len)) {
        return -EFAULT;
    }

    switch (op) {
    case NETCTL_GET_IF: {
        if ((size_t)len < sizeof(struct tus_ifinfo)) return -EINVAL;
        struct tus_ifinfo *info = (struct tus_ifinfo *)arg;

        memset(info, 0, sizeof(*info));
        memcpy(info->name, g_netif.name, sizeof(info->name));
        memcpy(info->mac, g_netif.mac, 6);
        info->up = g_netif.up ? 1 : 0;
        info->ip = g_netif.ip;
        info->netmask = g_netif.netmask;
        info->gateway = g_netif.gateway;
        info->dns = g_netif.dns;

        struct rtl8139_device dev;
        rtl8139_get_stats(&dev);
        info->rx_packets = dev.rx_packets;
        info->tx_packets = dev.tx_packets;
        info->rx_errors = dev.rx_errors;
        info->tx_dropped = dev.tx_dropped;
        info->rx_bytes = dev.rx_bytes;
        info->tx_bytes = dev.tx_bytes;
        info->mtu = 1500;
        netif_get_stats(NULL, &info->rx_dropped);
        return 0;
    }

    case NETCTL_SET_IF: {
        if ((size_t)len < sizeof(struct tus_ifinfo)) return -EINVAL;

        struct task *cur = sched_current();
        if (from_user && !has_cap(cur, CAP_NET_ADMIN)) {
            return -EPERM; /* reconfiguring the interface needs root or CAP_NET_ADMIN */
        }

        const struct tus_ifinfo *info = (const struct tus_ifinfo *)arg;
        if (info->ip) g_netif.ip = info->ip;
        if (info->netmask) g_netif.netmask = info->netmask;
        g_netif.gateway = info->gateway;
        if (info->dns) g_netif.dns = info->dns;
        return 0;
    }

    case NETCTL_PING: {
        if ((size_t)len < sizeof(struct tus_ping)) return -EINVAL;
        struct tus_ping *p = (struct tus_ping *)arg;

        uint8_t payload[64];
        uint32_t plen = p->payload_len > sizeof(payload) ? sizeof(payload)
                                                         : p->payload_len;
        for (uint32_t i = 0; i < plen; i++) payload[i] = (uint8_t)('a' + i % 26);

        if (icmp_send_echo(p->dst_ip, p->id, p->seq, payload,
                           (uint16_t)plen) < 0) {
            p->rtt_ms = -1;
            return -EHOSTUNREACH;
        }
        long rtt = icmp_wait_reply(p->dst_ip, p->id, p->seq,
                                   p->timeout_ms ? p->timeout_ms : 1000);
        p->rtt_ms = (int32_t)rtt;
        return rtt < 0 ? -ETIMEDOUT : 0;
    }

    case NETCTL_ARP_DUMP: {
        int max = (int)((size_t)len / sizeof(struct tus_arp_row));
        if (max <= 0) return -EINVAL;

        struct arp_entry_info rows[32];
        if (max > 32) max = 32;
        int n = arp_cache_dump(rows, max);

        struct tus_arp_row *out = (struct tus_arp_row *)arg;
        for (int i = 0; i < n; i++) {
            out[i].ip = rows[i].ip;
            memcpy(out[i].mac, rows[i].mac, 6);
        }
        return n;
    }

    case NETCTL_TCP_DUMP: {
        int max = (int)((size_t)len / sizeof(struct tus_tcp_row));
        if (max <= 0) return -EINVAL;

        struct tcp_conn_info rows[64];
        if (max > 64) max = 64;
        int n = tcp_dump(rows, max);

        struct tus_tcp_row *out = (struct tus_tcp_row *)arg;
        for (int i = 0; i < n; i++) {
            out[i].state = rows[i].state;
            out[i].local_ip = rows[i].local_ip;
            out[i].local_port = rows[i].local_port;
            out[i].remote_ip = rows[i].remote_ip;
            out[i].remote_port = rows[i].remote_port;
            out[i].rx_queued = rows[i].rx_queued;
            out[i].tx_queued = rows[i].tx_queued;
        }
        return n;
    }

    case NETCTL_RESOLVE: {
        if ((size_t)len < sizeof(struct tus_resolve)) return -EINVAL;
        struct tus_resolve *req = (struct tus_resolve *)arg;

        req->name[sizeof(req->name) - 1] = '\0';
        int n = dns_resolve(req->name, req->addr, DNS_MAX_ADDRS);
        if (n < 0) {
            req->count = 0;
            return n;
        }
        req->count = n;
        return n;
    }

    case NETCTL_GET_IF6: {
        if ((size_t)len < sizeof(struct tus_if6info)) return -EINVAL;
        struct tus_if6info *info = (struct tus_if6info *)arg;
        info->have_link_local = ipv6_get_link_local(info->link_local) ? 1 : 0;
        info->have_global = ipv6_get_global(info->global) ? 1 : 0;
        return 0;
    }

    case NETCTL_PING6: {
        if ((size_t)len < sizeof(struct tus_ping6)) return -EINVAL;
        struct tus_ping6 *p = (struct tus_ping6 *)arg;

        uint8_t payload[64];
        uint32_t plen = p->payload_len > sizeof(payload) ? sizeof(payload)
                                                         : p->payload_len;
        for (uint32_t i = 0; i < plen; i++) payload[i] = (uint8_t)('a' + i % 26);

        if (icmpv6_send_echo(p->dst, p->id, p->seq, payload,
                             (uint16_t)plen) < 0) {
            p->rtt_ms = -1;
            return -EHOSTUNREACH;
        }
        long rtt = icmpv6_wait_reply(p->dst, p->id, p->seq,
                                     p->timeout_ms ? p->timeout_ms : 1000);
        p->rtt_ms = (int32_t)rtt;
        return rtt < 0 ? -ETIMEDOUT : 0;
    }

    case NETCTL_NDP_DUMP: {
        int max = (int)((size_t)len / sizeof(struct tus_ndp_row));
        if (max <= 0) return -EINVAL;

        struct ndp_entry_info rows[16];
        if (max > 16) max = 16;
        int n = ndp_cache_dump(rows, max);

        struct tus_ndp_row *out = (struct tus_ndp_row *)arg;
        for (int i = 0; i < n; i++) {
            memcpy(out[i].addr, rows[i].addr, 16);
            memcpy(out[i].mac, rows[i].mac, 6);
        }
        return n;
    }

    default:
        return -EINVAL;
    }
}

/* ---- I/O multiplexing ---- */

/* Upper bound on a single poll() array. VFS_MAX_FDS is 16, but the
 * same fd may legitimately appear several times, so allow some room. */
#define POLL_MAX_FDS 64

/* Widest fd number select() will consider (one fd_set is 1024 bits in
 * musl; only the first VFS_MAX_FDS can ever be open). */
#define SELECT_MAX_FDS 1024
#define SELECT_WORDS   (SELECT_MAX_FDS / 64)

/* Userspace struct pollfd (musl's include/poll.h layout). */
struct tus_pollfd {
    int fd;
    short events;
    short revents;
};

/* True once `timeout_ms` have elapsed since `start`. A negative
 * timeout means "block forever". */
static bool poll_expired(uint64_t start, long timeout_ms) {
    return timeout_ms >= 0 &&
           pit_uptime_ms() - start >= (uint64_t)timeout_ms;
}

static long sys_poll(void *ufds, long nfds, long timeout_ms, bool from_user) {
    if (nfds < 0 || nfds > POLL_MAX_FDS) {
        return -EINVAL;
    }
    if (nfds > 0 &&
        !access_ok(from_user, ufds, (size_t)nfds * sizeof(struct tus_pollfd))) {
        return -EFAULT;
    }
    struct tus_pollfd *fds = (struct tus_pollfd *)ufds;

    uint64_t start = pit_uptime_ms();
    for (;;) {
        long ready = 0;
        for (long i = 0; i < nfds; i++) {
            /* A negative fd is skipped and its revents cleared, which
             * is how callers disable a slot without shrinking the
             * array. */
            if (fds[i].fd < 0) {
                fds[i].revents = 0;
                continue;
            }
            short re = vfs_poll(fds[i].fd, fds[i].events);
            fds[i].revents = re;
            if (re != 0) {
                ready++;
            }
        }
        if (ready > 0) {
            return ready;
        }
        if (timeout_ms == 0 || poll_expired(start, timeout_ms)) {
            return 0; /* timed out with nothing ready */
        }
        /* Drain the network before sleeping. A frame the card's
         * interrupt queued is not readable until the stack has walked
         * it, and nothing else here would ever do that: a task
         * blocked in poll() is exactly the case where no TCP call is
         * on the stack to pump it. */
        net_poll();
        hlt(); /* the PIT tick wakes us to look again */
    }
}

static long sys_select(long nfds, void *urfds, void *uwfds, void *uefds,
                       void *utv, bool from_user) {
    if (nfds < 0 || nfds > SELECT_MAX_FDS) {
        return -EINVAL;
    }
    size_t words = ((size_t)nfds + 63) / 64;
    size_t bytes = words * sizeof(uint64_t);
    if ((urfds != NULL && !access_ok(from_user, urfds, bytes)) ||
        (uwfds != NULL && !access_ok(from_user, uwfds, bytes)) ||
        (uefds != NULL && !access_ok(from_user, uefds, bytes))) {
        return -EFAULT;
    }

    /* struct timeval { long tv_sec; long tv_usec; }; NULL blocks. */
    long timeout_ms = -1;
    if (utv != NULL) {
        if (!access_ok(from_user, utv, 2 * sizeof(long))) {
            return -EFAULT;
        }
        long sec = ((const long *)utv)[0];
        long usec = ((const long *)utv)[1];
        if (sec < 0 || usec < 0) {
            return -EINVAL;
        }
        /* Clamp instead of overflowing: anything beyond ~24 days is
         * indistinguishable from "wait forever" here anyway. */
        if (sec > 2000000) {
            timeout_ms = 2000000000L;
        } else {
            timeout_ms = sec * 1000 + usec / 1000;
        }
    }

    /* The result sets overwrite the request sets, so keep a copy. */
    uint64_t in[3][SELECT_WORDS];
    uint64_t out[3][SELECT_WORDS];
    memset(in, 0, sizeof(in));
    if (urfds != NULL) {
        memcpy(in[0], urfds, bytes);
    }
    if (uwfds != NULL) {
        memcpy(in[1], uwfds, bytes);
    }
    if (uefds != NULL) {
        memcpy(in[2], uefds, bytes);
    }

    uint64_t start = pit_uptime_ms();
    for (;;) {
        memset(out, 0, sizeof(out));
        long ready = 0;

        for (long fd = 0; fd < nfds; fd++) {
            size_t w = (size_t)fd / 64;
            uint64_t bit = 1ull << (fd % 64);
            bool want_r = (in[0][w] & bit) != 0;
            bool want_w = (in[1][w] & bit) != 0;
            bool want_e = (in[2][w] & bit) != 0;
            if (!want_r && !want_w && !want_e) {
                continue;
            }

            short events = 0;
            if (want_r) {
                events |= POLLIN;
            }
            if (want_w) {
                events |= POLLOUT;
            }
            if (want_e) {
                events |= POLLPRI;
            }
            short re = vfs_poll(fd, events);
            if (re & POLLNVAL) {
                return -EBADF; /* select(2) fails outright on a bad fd */
            }
            /* select() promises "will not block", not "will succeed":
             * a hung-up or errored fd qualifies for both sets, because
             * the read returns EOF and the write fails immediately
             * instead of waiting. */
            if (want_r && (re & (POLLIN | POLLHUP | POLLERR))) {
                out[0][w] |= bit;
                ready++;
            }
            if (want_w && (re & (POLLOUT | POLLHUP | POLLERR))) {
                out[1][w] |= bit;
                ready++;
            }
            if (want_e && (re & POLLPRI)) {
                out[2][w] |= bit;
                ready++;
            }
        }

        if (ready > 0 || timeout_ms == 0 || poll_expired(start, timeout_ms)) {
            /* On timeout every set is cleared, which `out` already is. */
            if (urfds != NULL) {
                memcpy(urfds, out[0], bytes);
            }
            if (uwfds != NULL) {
                memcpy(uwfds, out[1], bytes);
            }
            if (uefds != NULL) {
                memcpy(uefds, out[2], bytes);
            }
            return ready;
        }
        net_poll(); /* same reason as in sys_poll() */
        hlt();
    }
}

/* ---- SYS_VIDEO: the display mode ---- */

/* Fill in everything GET and CAPS report about the mode on screen. */
static void video_describe(struct tus_video_mode *m) {
    uint32_t w = 0, h = 0, bpp = 0;
    uint64_t pitch = 0;
    void *addr = NULL;
    fb_get_info(&w, &h, &bpp, &pitch, &addr);

    m->width = w;
    m->height = h;
    m->bpp = bpp;
    m->pitch = (uint32_t)pitch;
    m->flags = 0;
    if (vbe_available()) {
        m->flags |= TUS_VIDEO_F_MODESET;
    }
    if (highx_active()) {
        m->flags |= TUS_VIDEO_F_HIGHX;
    }
    m->max_width = vbe_available() ? VBE_MAX_WIDTH : w;
    m->max_height = vbe_available() ? VBE_MAX_HEIGHT : h;
}

static long sys_video(long op, void *arg, size_t len, bool from_user) {
    if (arg == NULL || len < sizeof(struct tus_video_mode)) {
        return -EINVAL;
    }
    if (!access_ok(from_user, arg, len)) {
        return -EFAULT;
    }
    struct tus_video_mode *m = (struct tus_video_mode *)arg;

    switch (op) {
    case TUS_VIDEO_GET_MODE:
    case TUS_VIDEO_CAPS:
        video_describe(m);
        return 0;

    case TUS_VIDEO_LIST_MODE: {
        uint32_t w = 0, h = 0;
        if (!vbe_mode_at((int)m->index, &w, &h)) {
            return -ENOENT; /* the caller walked off the end: done */
        }
        m->width = w;
        m->height = h;
        m->bpp = 32;
        m->pitch = w * 4;
        return 0;
    }

    case TUS_VIDEO_SET_MODE: {
        struct task *cur = sched_current();
        if (from_user && cur != NULL && cur->euid != 0) {
            /* One screen, shared by every program on the machine.
             * Resizing it is an administrative act - `doas res_set`. */
            return -EPERM;
        }

        uint32_t want_w = m->width;
        uint32_t want_h = m->height;

        int rc = fb_set_mode(want_w, want_h);
        if (rc != 0) {
            return rc;
        }

        /* Everything that cached the old geometry has to be told, in
         * this order: the device node first (it is what a program
         * would consult next), then the display server, which
         * repaints the screen and so should be the last thing to
         * touch it. */
        devices_refresh_fb();
        if (highx_active()) {
            (void)highx_rebind();
        } else {
            fb_repaint();
        }

        video_describe(m);
        return 0;
    }

    default:
        return -EINVAL;
    }
}


/* ---- SYS_INPUT: the keyboard layout ---- */

static long sys_input(long op, void *arg, size_t len, bool from_user) {
    if (arg == NULL || len < sizeof(struct tus_input_keymap)) {
        return -EINVAL;
    }
    if (!access_ok(from_user, arg, len)) {
        return -EFAULT;
    }
    struct tus_input_keymap *km = (struct tus_input_keymap *)arg;

    switch (op) {
    case TUS_INPUT_GET_KEYMAP: {
        const char *name = keymap_name();
        const char *desc = "";
        for (int i = 0; keymap_at(i, NULL, NULL); i++) {
            const char *n = NULL, *d = NULL;
            keymap_at(i, &n, &d);
            if (strcmp(n, name) == 0) {
                desc = d;
                break;
            }
        }
        strncpy(km->name, name, TUS_KEYMAP_NAME_MAX - 1);
        km->name[TUS_KEYMAP_NAME_MAX - 1] = '\0';
        strncpy(km->description, desc, TUS_KEYMAP_DESC_MAX - 1);
        km->description[TUS_KEYMAP_DESC_MAX - 1] = '\0';
        return 0;
    }

    case TUS_INPUT_LIST_KEYMAP: {
        const char *n = NULL, *d = NULL;
        if (!keymap_at((int)km->index, &n, &d)) {
            return -ENOENT; /* the caller walked off the end: done */
        }
        strncpy(km->name, n, TUS_KEYMAP_NAME_MAX - 1);
        km->name[TUS_KEYMAP_NAME_MAX - 1] = '\0';
        strncpy(km->description, d, TUS_KEYMAP_DESC_MAX - 1);
        km->description[TUS_KEYMAP_DESC_MAX - 1] = '\0';
        return 0;
    }

    case TUS_INPUT_SET_KEYMAP: {
        struct task *cur = sched_current();
        if (from_user && cur != NULL && cur->euid != 0) {
            /* One keyboard, every user - `doas keymap tr`. */
            return -EPERM;
        }
        km->name[TUS_KEYMAP_NAME_MAX - 1] = '\0';
        if (keymap_set(km->name) != 0) {
            return -ENOENT;
        }
        return 0;
    }

    default:
        return -EINVAL;
    }
}


long syscall_dispatch(struct syscall_regs *r, uint64_t cs, uint64_t frame_rsp) {
    bool from_user = (cs & 3) == 3;

    switch (r->rax) {
    case SYS_EXIT:
        return sys_exit((int)r->rdi);
    case SYS_EXECVE:
        return sys_execve(r, from_user, frame_rsp);
    case SYS_SPAWN:
        return sys_spawn(r, from_user);
    case SYS_HIGHX:
        /* The request structure is validated here; highx_request()
         * checks the opcode and the length itself. */
        if (!access_ok(from_user, (void *)r->rsi, (size_t)r->rdx)) {
            return -EFAULT;
        }
        return highx_request((long)r->rdi, (void *)r->rsi, r->rdx, from_user);
    case SYS_PIPE: {
        int fds[2];
        long ret = vfs_pipe(fds);
        if (ret < 0) {
            return ret;
        }
        if (!access_ok(from_user, (void *)r->rdi, 2 * sizeof(int))) {
            return -EFAULT;
        }
        ((int *)(uintptr_t)r->rdi)[0] = fds[0];
        ((int *)(uintptr_t)r->rdi)[1] = fds[1];
        return 0;
    }
    case SYS_DUP2:
        return vfs_dup2(r->rdi, r->rsi);
    case SYS_DUP:
        return vfs_dup(r->rdi);
    case SYS_SOCKET:
        return sys_socket(r->rdi, r->rsi, r->rdx);
    case SYS_BIND:
        return sys_bind(r->rdi, (const void *)r->rsi, (long)r->rdx, from_user);
    case SYS_LISTEN: {
        struct inet_sock *inet = vfs_fd_inet_sock(r->rdi);
        if (inet != NULL) {
            return inet_sock_listen(inet, (int)r->rsi);
        }
        struct unix_sock *s = vfs_fd_sock(r->rdi);
        return s != NULL ? unix_sock_listen(s, (int)r->rsi) : -ENOTSOCK;
    }
    case SYS_ACCEPT:
        return sys_accept(r->rdi, (void *)r->rsi, (void *)r->rdx, from_user);
    case SYS_CONNECT:
        return sys_connect(r->rdi, (const void *)r->rsi, (long)r->rdx,
                           from_user);
    case SYS_SOCKETPAIR:
        return sys_socketpair(r->rdi, r->rsi, r->rdx, (void *)r->r10,
                              from_user);
    case SYS_SHUTDOWN: {
        struct inet_sock *inet = vfs_fd_inet_sock(r->rdi);
        if (inet != NULL) {
            return inet_sock_shutdown(inet, (int)r->rsi);
        }
        struct unix_sock *s = vfs_fd_sock(r->rdi);
        return s != NULL ? unix_sock_shutdown(s, (int)r->rsi) : -ENOTSOCK;
    }
    case SYS_GETSOCKNAME:
        return sys_getsockname(r->rdi, (void *)r->rsi, (void *)r->rdx,
                               from_user);
    case SYS_GETPEERNAME:
        return sys_getpeername(r->rdi, (void *)r->rsi, (void *)r->rdx,
                               from_user);
    case SYS_SENDTO:
        return sys_sendto(r, from_user);
    case SYS_RECVFROM:
        return sys_recvfrom(r, from_user);
    case SYS_SETSOCKOPT:
        return sys_setsockopt(r->rdi, r->rsi, r->rdx, (const void *)r->r10,
                              (long)r->r8, from_user);
    case SYS_GETSOCKOPT:
        return sys_getsockopt(r->rdi, r->rsi, r->rdx, (void *)r->r10,
                              (void *)r->r8, from_user);
    case SYS_FCNTL:
        return sys_fcntl(r->rdi, r->rsi, r->rdx);
    case SYS_NETCTL:
        return sys_netctl(r->rdi, (void *)r->rsi, (long)r->rdx, from_user);
    case SYS_GETRANDOM: {
        void *buf = (void *)r->rdi;
        size_t len = (size_t)r->rsi;
        if (!access_ok(from_user, buf, len)) {
            return -EFAULT;
        }
        /* No short reads: the pool is a CSPRNG, not a blocking device,
         * so there is never a reason to hand back less than asked. */
        random_bytes(buf, len);
        return (long)len;
    }
    case SYS_WAITPID:
        return sys_waitpid((long)r->rdi, (void *)r->rsi, (long)r->rdx,
                           from_user);
    case SYS_INPUT:
        return sys_input((long)r->rdi, (void *)r->rsi,
                        (size_t)r->rdx, from_user);
    case SYS_VIDEO:
        return sys_video((long)r->rdi, (void *)r->rsi,
                        (size_t)r->rdx, from_user);
    case SYS_POWER:
        /* No return: the machine either resets or stops. */
        if (r->rdi == TUS_POWER_REBOOT) {
            console_write("\nRebooting...\n");
            serial_sync();
            outb(0x64, 0xFE); /* pulse the 8042's reset line */
        } else {
            console_write("\nThe system is halted.\n");
            serial_sync();
        }
        for (;;) {
            cli();
            hlt();
        }
    case SYS_TERM:
        /* The request structure is read AND written by the kernel;
         * both directions are covered by one check. */
        if (!access_ok(from_user, (const void *)r->rsi, (size_t)r->rdx)) {
            return -EFAULT;
        }
        /* TERM_OP_READ/WRITE carry a second pointer inside the
         * structure, which term_syscall cannot check without knowing
         * whether it came from ring 3 - so it is told. */
        return term_syscall((long)r->rdi, (void *)r->rsi, (size_t)r->rdx,
                            from_user);
    case SYS_CLOCK:
        return (long)rtc_now();
    case SYS_POLL:
        return sys_poll((void *)r->rdi, (long)r->rsi, (long)r->rdx, from_user);
    case SYS_SELECT:
        return sys_select((long)r->rdi, (void *)r->rsi, (void *)r->rdx,
                          (void *)r->r10, (void *)r->r8, from_user);
    case SYS_READ:
        if (!access_ok(from_user, (void *)r->rsi, (size_t)r->rdx)) {
            return -EFAULT;
        }
        return vfs_read((int)r->rdi, (void *)r->rsi, (size_t)r->rdx);
    case SYS_WRITE:
        if (!access_ok(from_user, (const void *)r->rsi, (size_t)r->rdx)) {
            return -EFAULT;
        }
        return vfs_write((int)r->rdi, (const void *)r->rsi, (size_t)r->rdx);
    case SYS_OPEN:
        if (!access_ok(from_user, (const void *)r->rdi, 1)) {
            return -EFAULT;
        }
        return vfs_open_mode((const char *)r->rdi, (int)r->rsi,
                             (uint32_t)r->rdx);
    case SYS_CLOSE:
        return vfs_close((int)r->rdi);
    case SYS_IOCTL:
        /* The ioctl arg blob may be a termios (57 bytes), a winsize
         * (8 bytes) or a device info struct; check the worst case. */
        if (!access_ok(from_user, (void *)r->rdx, 64)) {
            return -EFAULT;
        }
        return vfs_ioctl((int)r->rdi, r->rsi, (void *)r->rdx);
    case SYS_GETPID: {
        struct task *cur = sched_current();
        return cur != NULL ? (long)cur->pid : 1;
    }
    case SYS_UPTIME:
        return (long)pit_uptime_ms();
    case SYS_SLEEP:
        timer_sleep_ms((uint32_t)r->rdi);
        return 0;
    case SYS_MKDIR:
        if (!access_ok(from_user, (const void *)r->rdi, 1)) {
            return -EFAULT;
        }
        return vfs_mkdir((const char *)r->rdi, (uint32_t)r->rsi);
    case SYS_CHMOD:
        if (!access_ok(from_user, (const void *)r->rdi, 1)) {
            return -EFAULT;
        }
        return vfs_chmod((const char *)r->rdi, (uint32_t)r->rsi);
    case SYS_CHOWN:
        if (!access_ok(from_user, (const void *)r->rdi, 1)) {
            return -EFAULT;
        }
        return vfs_chown((const char *)r->rdi, (uint32_t)r->rsi, (uint32_t)r->rdx);
    case SYS_UNLINK:
        if (!access_ok(from_user, (const void *)r->rdi, 1)) {
            return -EFAULT;
        }
        return vfs_unlink((const char *)r->rdi);
    case SYS_READDIR:
        if (!access_ok(from_user, (void *)r->rsi, (size_t)r->rdx)) {
            return -EFAULT;
        }
        return vfs_readdir((int)r->rdi, (void *)r->rsi, (size_t)r->rdx);
    case SYS_MMAP:
        return sys_mmap(r->rdi, r->rsi, r->rdx, r->r10);
    case SYS_MUNMAP:
        return sys_munmap(r->rdi, r->rsi);
    case SYS_ARCH_PRCTL:
        return sys_arch_prctl(r->rdi, r->rsi, from_user);
    case SYS_WRITEV:
        return sys_writev(r->rdi, (void *)r->rsi, r->rdx, from_user);
    case SYS_READV:
        return sys_readv(r->rdi, (void *)r->rsi, r->rdx, from_user);
    case SYS_TIME:
        return (long)(pit_uptime_ms() / 1000);
    case SYS_FTRUNCATE:
        return vfs_ftruncate((int)r->rdi, (long)r->rsi);
    case SYS_LSEEK:
        return vfs_lseek((long)r->rdi, (long)r->rsi, (int)r->rdx);
    case SYS_STATX:
        return sys_statx(r, from_user);
    case SYS_GETUID: {
        struct task *cur = sched_current();
        return cur != NULL ? (long)cur->uid : 0;
    }
    case SYS_GETEUID: {
        struct task *cur = sched_current();
        return cur != NULL ? (long)cur->euid : 0;
    }
    case SYS_SETUID: {
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        uint32_t target = (uint32_t)r->rdi;
        /* POSIX semantics: a privileged (euid 0) caller may become
         * any uid; an unprivileged one may only set its real, saved
         * or effective uid to its own real uid (a no-op here, since
         * TUS tracks no saved-uid) - never to an arbitrary account,
         * root included. Without this check any process could
         * escalate itself with a bare setuid(0). */
        if (cur->euid != 0 && target != cur->uid) {
            return -EPERM;
        }
        cur->uid = target;
        cur->euid = target;
        return 0;
    }
    case SYS_GETGID: {
        struct task *cur = sched_current();
        return cur != NULL ? (long)cur->gid : 0;
    }
    case SYS_SETGID: {
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        uint32_t target = (uint32_t)r->rdi;
        if (cur->euid != 0 && target != cur->gid) {
            return -EPERM;
        }
        cur->gid = target;
        cur->egid = target;
        return 0;
    }

    /* ---- process identity (ksh: $PPID, job control's process
     * groups) ---- */
    case SYS_GETPPID: {
        struct task *cur = sched_current();
        return cur != NULL ? (long)cur->ppid : 0;
    }
    case SYS_GETPGRP: {
        struct task *cur = sched_current();
        return cur != NULL ? (long)cur->pgid : 0;
    }
    case SYS_GETPGID: {
        uint32_t pid = (uint32_t)r->rdi;
        struct task *t = pid == 0 ? sched_current() : sched_find_pid(pid);
        if (t == NULL) {
            return -ESRCH;
        }
        return (long)t->pgid;
    }
    case SYS_SETPGID: {
        uint32_t pid = (uint32_t)r->rdi;
        uint32_t pgid = (uint32_t)r->rsi;
        struct task *cur = sched_current();
        struct task *t = pid == 0 ? cur : sched_find_pid(pid);
        if (t == NULL) {
            return -ESRCH;
        }
        t->pgid = (pgid == 0) ? t->pid : pgid;
        return 0;
    }
    case SYS_SETSID: {
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        /* POSIX refuses this when the caller is already a process
         * group leader; TUS has no separate "leader" bookkeeping, so
         * pgid == pid (true for every session leader, since setsid
         * always sets both together) stands in for it. */
        if (cur->pgid == cur->pid && cur->sid == cur->pid) {
            return -EPERM;
        }
        cur->sid = cur->pid;
        cur->pgid = cur->pid;
        return (long)cur->sid;
    }

    /* ---- working directory ---- */
    case SYS_CHDIR: {
        if (!access_ok(from_user, (const void *)r->rdi, 1)) {
            return -EFAULT;
        }
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        struct vfs_node *node = vfs_lookup((const char *)r->rdi);
        if (node == NULL) {
            return -ENOENT;
        }
        if (node->type != VFS_DIR) {
            return -ENOTDIR;
        }
        char resolved[TASK_CWD_MAX];
        vfs_path_resolve(cur->cwd, (const char *)r->rdi, resolved, sizeof(resolved));
        strncpy(cur->cwd, resolved, TASK_CWD_MAX - 1);
        cur->cwd[TASK_CWD_MAX - 1] = '\0';
        return 0;
    }
    case SYS_FCHDIR: {
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        char resolved[TASK_CWD_MAX];
        long rc = vfs_fchdir((long)r->rdi, resolved, sizeof(resolved));
        if (rc != 0) {
            return rc;
        }
        strncpy(cur->cwd, resolved, TASK_CWD_MAX - 1);
        cur->cwd[TASK_CWD_MAX - 1] = '\0';
        return 0;
    }
    case SYS_GETCWD: {
        if (!access_ok(from_user, (void *)r->rdi, (size_t)r->rsi)) {
            return -EFAULT;
        }
        struct task *cur = sched_current();
        const char *cwd = cur != NULL ? cur->cwd : "/";
        size_t len = strlen(cwd);
        if (len + 1 > r->rsi) {
            return -ERANGE;
        }
        memcpy((void *)r->rdi, cwd, len + 1);
        return (long)(len + 1); /* real getcwd(2)'s return: bytes incl. NUL */
    }

    /* ---- misc POSIX ksh calls directly ---- */
    case SYS_ACCESS: {
        if (!access_ok(from_user, (const void *)r->rdi, 1)) {
            return -EFAULT;
        }
        return vfs_access_path((const char *)r->rdi, (int)r->rsi);
    }
    case SYS_RENAME: {
        if (!access_ok(from_user, (const void *)r->rdi, 1) ||
            !access_ok(from_user, (const void *)r->rsi, 1)) {
            return -EFAULT;
        }
        return vfs_rename((const char *)r->rdi, (const char *)r->rsi);
    }
    case SYS_READLINK:
        /* TUS has no symlinks - vfs_lookup() never returns anything
         * a real readlink() would follow, so any path is simply "not
         * a symlink", the same answer musl's own callers (realpath(),
         * libast's path canonicalizer) already treat as "keep going"
         * rather than "unknown syscall, abort". */
        return -EINVAL;
    case SYS_SYMLINK:
        return -EPERM; /* no write support for a node type TUS has none of */
    case SYS_TIMES: {
        if (r->rdi != 0 && !access_ok(from_user, (void *)r->rdi, 32)) {
            return -EFAULT;
        }
        /* No separate user/system accounting exists; approximate with
         * the uptime tick count (good enough for $SECONDS and `time`,
         * not for real accounting - same honesty as SYS_GETRLIMIT
         * below). */
        long ticks = (long)(pit_uptime_ms() / 10); /* CLK_TCK-ish */
        if (r->rdi != 0) {
            long *tms = (long *)r->rdi;
            tms[0] = ticks; /* tms_utime */
            tms[1] = 0;     /* tms_stime */
            tms[2] = 0;     /* tms_cutime */
            tms[3] = 0;     /* tms_cstime */
        }
        return ticks;
    }
    case SYS_GETRLIMIT: {
        if (!access_ok(from_user, (void *)r->rsi, 16)) {
            return -EFAULT;
        }
        uint64_t *lim = (uint64_t *)r->rsi;
        if (r->rdi == 7) { /* RLIMIT_NOFILE: a real, honest limit, not
                             * infinity - VFS_MAX_FDS is a genuine
                             * per-task ceiling (kernel/vfs/vfs.h), and
                             * musl's own sysconf(_SC_OPEN_MAX) (src/
                             * conf/sysconf.c) returns -1 for
                             * RLIM_INFINITY, which real ksh93 reads as
                             * "couldn't determine the limit" and
                             * refuses to start ("open files limit
                             * insufficient") rather than as "no
                             * limit". */
            lim[0] = VFS_MAX_FDS;
            lim[1] = VFS_MAX_FDS;
        } else {
            lim[0] = ~0ULL; /* rlim_cur = RLIM_INFINITY */
            lim[1] = ~0ULL; /* rlim_max = RLIM_INFINITY */
        }
        return 0;
    }
    case SYS_SETRLIMIT:
        /* Accepted and ignored: TUS enforces no resource limits.
         * Matches how umask() is already a no-op in tus_syscall.c. */
        return 0;
    case SYS_SET_TID_ADDRESS: {
        /* TUS has no threads - the main (only) thread's tid is always
         * its pid, exactly like Linux's main-thread case. musl stores
         * whatever this returns as self->tid and later syscalls
         * (raise() -> tkill(tid, sig)) depend on it being right. */
        struct task *cur = sched_current();
        return cur != NULL ? (long)cur->pid : 1;
    }

    /* ---- real (POSIX) signal delivery, see kernel/sched/sched.c ---- */
    case SYS_KILL: {
        long pid = (long)r->rdi;
        int sig = (int)r->rsi;
        if (pid > 0) {
            return sched_raise((uint32_t)pid, sig);
        }
        if (pid == 0) { /* the caller's own process group */
            struct task *cur = sched_current();
            if (cur == NULL) {
                return -ESRCH;
            }
            sched_raise_pgid(cur->pgid, sig);
            return 0;
        }
        if (pid == -1) {
            return -EPERM; /* "every process": not offered */
        }
        sched_raise_pgid((uint32_t)(-pid), sig); /* pid < -1: group -pid */
        return 0;
    }
    case SYS_SIGACTION: {
        int sig = (int)r->rdi;
        if (sig < 1 || sig > 31) {
            return -EINVAL;
        }
        if (sig == 9 || sig == 19) { /* SIGKILL/SIGSTOP: never catchable */
            return -EINVAL;
        }
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        if (r->rdx != 0) { /* oldact */
            if (!access_ok(from_user, (void *)r->rdx, 32)) {
                return -EFAULT;
            }
            uint64_t *old = (uint64_t *)r->rdx;
            old[0] = cur->sig_action[sig].handler;
            old[1] = cur->sig_action[sig].flags;
            old[2] = cur->sig_action[sig].restorer;
            old[3] = cur->sig_action[sig].mask;
        }
        if (r->rsi != 0) { /* act */
            if (!access_ok(from_user, (const void *)r->rsi, 32)) {
                return -EFAULT;
            }
            const uint64_t *ksa = (const uint64_t *)r->rsi;
            cur->sig_action[sig].handler = ksa[0];
            cur->sig_action[sig].flags = ksa[1];
            cur->sig_action[sig].restorer = ksa[2];
            cur->sig_action[sig].mask = ksa[3];
        }
        return 0;
    }
    case SYS_SIGPROCMASK: {
        long how = r->rdi;
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        if (r->rdx != 0) { /* oldset */
            if (!access_ok(from_user, (void *)r->rdx, 8)) {
                return -EFAULT;
            }
            *(uint64_t *)r->rdx = cur->sig_blocked;
        }
        if (r->rsi != 0) {
            if (!access_ok(from_user, (const void *)r->rsi, 8)) {
                return -EFAULT;
            }
            uint64_t set = *(const uint64_t *)r->rsi;
            switch (how) {
            case 0: cur->sig_blocked |= set; break;    /* SIG_BLOCK */
            case 1: cur->sig_blocked &= ~set; break;   /* SIG_UNBLOCK */
            case 2: cur->sig_blocked = set; break;     /* SIG_SETMASK */
            default: return -EINVAL;
            }
            /* SIGKILL/SIGSTOP can never be blocked. */
            cur->sig_blocked &= ~(SIG_MASK_9_19_TUS);
        }
        return 0;
    }
    case SYS_SIGSUSPEND: {
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        if (r->rdi != 0 && !access_ok(from_user, (const void *)r->rdi, 8)) {
            return -EFAULT;
        }
        uint64_t saved = cur->sig_blocked;
        if (r->rdi != 0) {
            cur->sig_blocked = (*(const uint64_t *)r->rdi) & ~(SIG_MASK_9_19_TUS);
        }
        while (!sched_signal_pending()) {
            hlt();
        }
        cur->sig_blocked = saved;
        return -EINTR; /* sigsuspend() never returns anything else */
    }
    case SYS_ALARM: {
        struct task *cur = sched_current();
        if (cur == NULL) {
            return 0;
        }
        uint64_t now = pit_uptime_ms();
        long remaining = 0;
        if (cur->alarm_deadline_ms != 0 && cur->alarm_deadline_ms > now) {
            remaining = (long)((cur->alarm_deadline_ms - now) / 1000);
        }
        uint32_t secs = (uint32_t)r->rdi;
        cur->alarm_deadline_ms = secs != 0 ? now + (uint64_t)secs * 1000 : 0;
        return remaining;
    }
    case SYS_SETITIMER: {
        if (r->rdi != 0) { /* only ITIMER_REAL exists here */
            return -EINVAL;
        }
        struct task *cur = sched_current();
        if (cur == NULL) {
            return -EPERM;
        }
        if (r->rsi != 0 && !access_ok(from_user, (const void *)r->rsi, 32)) {
            return -EFAULT;
        }
        if (r->rdx != 0 && !access_ok(from_user, (void *)r->rdx, 32)) {
            return -EFAULT;
        }
        uint64_t now = pit_uptime_ms();
        if (r->rdx != 0) { /* old itimerval: interval always 0 (one-shot) */
            long *old = (long *)r->rdx;
            old[0] = 0; old[1] = 0; /* it_interval */
            if (cur->alarm_deadline_ms != 0 && cur->alarm_deadline_ms > now) {
                uint64_t left_ms = cur->alarm_deadline_ms - now;
                old[2] = (long)(left_ms / 1000);       /* it_value.tv_sec */
                old[3] = (long)(left_ms % 1000) * 1000; /* it_value.tv_usec */
            } else {
                old[2] = 0; old[3] = 0;
            }
        }
        if (r->rsi != 0) {
            const long *new_it = (const long *)r->rsi;
            long value_sec = new_it[2];
            long value_usec = new_it[3];
            cur->alarm_deadline_ms = (value_sec != 0 || value_usec != 0)
                ? now + (uint64_t)value_sec * 1000 + (uint64_t)value_usec / 1000
                : 0;
        }
        return 0;
    }
    case SYS_PAUSE: {
        while (!sched_signal_pending()) {
            hlt();
        }
        return -EINTR;
    }
    case SYS_UNAME: {
        /* struct utsname: 6 fields of 65 bytes each (sys/utsname.h) -
         * sysname/nodename/release/version/machine/domainname. musl's
         * gethostname() is implemented in terms of this (not a
         * separate syscall), so nodename is the one field real
         * programs actually depend on being right. */
        if (!access_ok(from_user, (void *)r->rdi, 65 * 6)) {
            return -EFAULT;
        }
        char *u = (char *)r->rdi;
        const char *fields[6];
        fields[0] = "TUS";
        fields[1] = hostname_get();
        fields[2] = "1.0";
        fields[3] = "TUS";
        fields[4] = "x86_64";
        fields[5] = "";
        for (int i = 0; i < 6; i++) {
            char *dst = u + i * 65;
            const char *src = fields[i];
            size_t j = 0;
            for (; j < 64 && src[j] != '\0'; j++) {
                dst[j] = src[j];
            }
            dst[j] = '\0';
        }
        return 0;
    }
    case SYS_SETHOSTNAME: {
        struct task *cur = sched_current();
        if (from_user && cur != NULL && cur->euid != 0) {
            /* Same shape as SYS_VIDEO's TUS_VIDEO_SET_MODE check: one
             * machine-wide value, changing it is administrative. */
            return -EPERM;
        }
        size_t len = (size_t)r->rsi;
        if (!access_ok(from_user, (const void *)r->rdi, len)) {
            return -EFAULT;
        }
        return hostname_set((const char *)r->rdi, len);
    }
    case SYS_GETPROCS: {
        size_t bufsize = (size_t)r->rsi;
        if (!access_ok(from_user, (void *)r->rdi, bufsize)) {
            return -EFAULT;
        }
        int max = (int)(bufsize / sizeof(struct tus_procinfo));
        return task_snapshot((struct tus_procinfo *)r->rdi, max);
    }

    case SYS_PANIC: {
        struct task *cur = sched_current();
        if (from_user && cur != NULL && cur->euid != 0) {
            return -EPERM;
        }
        enum panic_kind kind;
        const char *title;
        switch ((long)r->rdi) {
        case TUS_PANIC_RSOD: kind = PANIC_RSOD; title = "Hardware error reported"; break;
        case TUS_PANIC_GSOD: kind = PANIC_GSOD; title = "System error reported"; break;
        case TUS_PANIC_BSOD:
        default:             kind = PANIC_BSOD; title = "Critical service crashed"; break;
        }
        serial_sync();
        panic_screen_show(kind, title);
        console_write("A critical failure was reported by userspace "
                      "(tusSM) - see /var/log/errors and /var/log/journal.\n");
        console_write("System halted.\n");
        for (;;) {
            cli();
            hlt();
        }
    }

    case SYS_CAPSET: {
        struct task *cur = sched_current();
        if (from_user && !has_cap(cur, ~0u)) {
            /* Only euid == 0 may grant capabilities - has_cap() with
             * an all-bits mask is true only for that implicit-root
             * case, never via a previously granted subset. */
            return -EPERM;
        }
        uint32_t pid = (uint32_t)r->rdi;
        uint32_t caps = (uint32_t)r->rsi & CAP_ALL_KNOWN;
        struct task *target = (pid == 0) ? cur : sched_find_pid(pid);
        if (target == NULL) {
            return -ESRCH;
        }
        target->caps = caps;
        return 0;
    }

    default:
        return -ENOSYS;
    }
}
