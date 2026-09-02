/*
 * syscall.h - POSIX-style system call interface
 *
 * Syscalls are invoked with `int $0x80`:
 *
 *     RAX = syscall number
 *     RDI, RSI, RDX, R10, R8 = arguments (up to five)
 *     RAX = return value (>= 0, or negative errno on failure)
 *
 * The IDT gate at vector 0x80 is a DPL-3 trap gate, so the same ABI
 * will be used by user-mode processes once the scheduler exists. Until
 * then the kernel shell calls these directly.
 */

#ifndef TUS_SYSCALL_SYSCALL_H
#define TUS_SYSCALL_SYSCALL_H

#include <stdint.h>

/* Syscall numbers (documented ABI; keep stable). */
#define SYS_EXIT    0
#define SYS_READ    1
#define SYS_WRITE   2
#define SYS_OPEN    3
#define SYS_CLOSE   4
#define SYS_IOCTL   5
#define SYS_GETPID  6
#define SYS_UPTIME  7
#define SYS_SLEEP   8
#define SYS_MKDIR   9
#define SYS_UNLINK  10
#define SYS_READDIR 11
/* v0.5.0: userspace libc (musl) support. */
#define SYS_MMAP     12 /* mmap(addr, len, prot, flags) - anonymous only */
#define SYS_MUNMAP   13 /* munmap(addr, len) */
#define SYS_ARCH_PRCTL 14 /* arch_prctl(op, addr) - ARCH_SET_FS/ARCH_GET_FS */
#define SYS_WRITEV   15 /* writev(fd, iovec*, count) */

/* readv(fd, iovec*, count): writev's mirror. musl's buffered input
 * (fgets, fread) is built on it, so without it every stdio read
 * returns nothing and a present file looks like an empty one. */
#define SYS_READV    51
/* v0.6.0: kilo (a real terminal application) needs a clock and file
 * truncation. */
#define SYS_TIME     16 /* time(NULL): seconds since boot */
#define SYS_FTRUNCATE 17 /* ftruncate(fd, length) */
/* v0.8.0: execve + uid/gid (doas, passwd, login, useradd). */
#define SYS_EXECVE   18 /* execve(path, argv, envp) - replaces the task */
#define SYS_CHMOD    19 /* chmod(path, mode) */
#define SYS_GETUID   20 /* getuid() */
#define SYS_GETEUID  21 /* geteuid() */
#define SYS_SETUID   22 /* setuid(uid) */
#define SYS_GETGID   23 /* getgid() */
#define SYS_SETGID   24 /* setgid(gid) */
/* v2.0 (2026-08-13): pipes + dup for I/O redirection. */
#define SYS_PIPE     25 /* pipe(int fds[2]) */
#define SYS_DUP2     26 /* dup2(oldfd, newfd) */
#define SYS_DUP      27 /* dup(oldfd) */
/* Unix domain sockets (AF_UNIX, SOCK_STREAM) and I/O multiplexing.
 * The address argument is a struct sockaddr_un (see net/socket.h). */
#define SYS_SOCKET      28 /* socket(domain, type, protocol) */
#define SYS_BIND        29 /* bind(fd, addr, addrlen) */
#define SYS_LISTEN      30 /* listen(fd, backlog) */
#define SYS_ACCEPT      31 /* accept(fd, addr, addrlen*) */
#define SYS_CONNECT     32 /* connect(fd, addr, addrlen) */
#define SYS_SOCKETPAIR  33 /* socketpair(domain, type, protocol, int sv[2]) */
#define SYS_SHUTDOWN    34 /* shutdown(fd, how) */
#define SYS_GETSOCKNAME 35 /* getsockname(fd, addr, addrlen*) */
#define SYS_POLL        36 /* poll(struct pollfd*, nfds, timeout_ms) */
#define SYS_SELECT      37 /* select(nfds, rfds, wfds, efds, timeval*) */

/* highX window system (v1.0). One multiplexed call carries the whole
 * protocol: `op` is an HX_OP_* opcode, `arg` points at the matching
 * request structure and `len` is its size. See include/highx.h. */
#define SYS_HIGHX       38 /* highx(op, void *arg, size_t len) */
/* spawn(path, argv): start a program as a NEW task and return its pid
 * without waiting - execve replaces the caller, this one does not.
 * The window manager launches its applications with it. */
#define SYS_SPAWN       39 /* spawn(const char *path, char *const argv[]) */
/* lseek(fd, offset, whence): reposition a regular file. Needed by
 * anything that reads a container format instead of a stream - the
 * MP4 demuxer jumps between the sample tables and the media data. */
#define SYS_LSEEK       40 /* lseek(fd, offset, whence) */

/* ---- AF_INET additions ----
 *
 * send()/recv() used to be write()/read() with the address argument
 * refused outright, which is fine for a stream and useless for a
 * datagram. With UDP in the kernel they need the real thing. */
#define SYS_SENDTO      41 /* sendto(fd, buf, len, flags, addr, addrlen) */
#define SYS_RECVFROM    42 /* recvfrom(fd, buf, len, flags, addr, addrlen*) */
#define SYS_GETPEERNAME 43 /* getpeername(fd, addr, addrlen*) */
#define SYS_SETSOCKOPT  44 /* setsockopt(fd, level, opt, val, len) */
#define SYS_GETSOCKOPT  45 /* getsockopt(fd, level, opt, val, len*) */

/* fcntl(fd, cmd, arg): F_GETFL/F_SETFL, which is how a program asks
 * for O_NONBLOCK on a socket. */
#define SYS_FCNTL       46

/* netctl(op, arg, len): the interface, ARP and connection tables, plus
 * ping and the resolver. See include/tusnet.h for the ABI. */
#define SYS_NETCTL      47

/* getrandom(buf, len, flags): bytes from the kernel's CSPRNG. Key
 * exchange and host key generation need these; nothing in userspace is
 * in a position to collect entropy for itself. */
#define SYS_GETRANDOM   48

/* waitpid(pid, status*, options): block until a spawned task exits.
 * sshd and git both start children and need to know how they ended. */
#define SYS_WAITPID     49

/* clock(): wall-clock seconds since the epoch, from the CMOS RTC.
 * SYS_TIME reports uptime, which is not a date a commit can carry. */
#define SYS_CLOCK       50

/* term(op, arg, len): terminal sessions - a real tsh, running as its
 * own ring-0 task, wired to a terminal window instead of the screen.
 * One multiplexed call carries the protocol, as SYS_HIGHX does; see
 * include/tusterm.h. */
#define SYS_TERM        52

/* power(op): 0 halts the machine, 1 reboots it. The installer needs
 * it (an install ends in a reboot, like every installer), and so does
 * anything else that offers to shut the machine down. */
#define SYS_POWER       53
#define TUS_POWER_HALT   0
#define TUS_POWER_REBOOT 1

/* video(op, arg, len): read, list and change the display mode.
 * See include/tusvideo.h for the ABI. Changing the mode is root's
 * job; reading it is not. */
#define SYS_VIDEO       54

/* input(op, arg, len): the keyboard layout, and the input settings
 * that will live next to it. See include/tusinput.h. Reading is open;
 * changing the layout is root's job - one keyboard, every user. */
#define SYS_INPUT       55

/* statx(fd, path, flags, mask, statxbuf): TUS has no stat/fstat/
 * fstatat, so musl's fstat() always falls back to this (see
 * sources/musl-1.2.6/src/stat/fstatat.c's fstatat_statx()). Only the
 * fd-based case musl actually uses for fstat() is implemented -
 * AT_EMPTY_PATH with a valid fd, path ignored - not general path
 * lookups with flags. */
#define SYS_STATX       56

/* fork(): NOT dispatched through this file's int-0x80 switch at all.
 * It needs to capture and replay the full CPU register state, which
 * the ordinary 7-register syscall gate has no room for - see
 * fork_entry() and sched_fork() in kernel/sched/sched.c. It has its
 * own IDT vector (0x81, kernel/arch/x86_64/idt.c) and its own musl
 * bridge (sources/musl-1.2.6/src/internal/tus_syscall.c: tus_fork()).
 * The number is listed here only so the syscall ledger stays
 * complete. */
#define SYS_FORK        57

/* v0.9.0: the process/filesystem surface a real ksh93 needs -
 * per-task identity (ppid/pgid/sid) and working directory, plus the
 * handful of POSIX calls it makes directly. See kernel/syscall/
 * syscall.c's dispatch for each one's semantics. */
#define SYS_GETPPID     58 /* getppid() */
#define SYS_GETPGRP     59 /* getpgrp() */
#define SYS_GETPGID     60 /* getpgid(pid) */
#define SYS_SETPGID     61 /* setpgid(pid, pgid) */
#define SYS_SETSID      62 /* setsid() */
#define SYS_CHDIR       63 /* chdir(path) */
#define SYS_FCHDIR      64 /* fchdir(fd) */
#define SYS_GETCWD      65 /* getcwd(buf, size) */
#define SYS_ACCESS      66 /* access(path, mode) */
#define SYS_RENAME      67 /* rename(old, new) */
#define SYS_READLINK    68 /* readlink(path, buf, size) - TUS has no
                             * symlinks, always -ENOENT (see vfs.c) */
#define SYS_SYMLINK     69 /* symlink(target, path) - always -EPERM */
#define SYS_TIMES       70 /* times(struct tms *) */
#define SYS_GETRLIMIT   71 /* getrlimit(resource, rlim*) - RLIM_INFINITY */
#define SYS_SETRLIMIT   72 /* setrlimit(resource, rlim*) - accepted, unenforced */
#define SYS_SET_TID_ADDRESS 73 /* set_tid_address(int*) - returns getpid() */

/* Real POSIX signal delivery (see kernel/sched/sched.c's
 * sched_deliver_signal()/sched_raise() and sigreturn's own IDT vector
 * 0x82 - rt_sigreturn does NOT go through this int-0x80 dispatch at
 * all, for the same reason fork() has its own vector). */
#define SYS_KILL        74 /* kill(pid, sig) */
#define SYS_SIGACTION   75 /* rt_sigaction(sig, act, oldact, sigsetsize) */
#define SYS_SIGPROCMASK 76 /* rt_sigprocmask(how, set, oldset, sigsetsize) */
#define SYS_SIGSUSPEND  77 /* rt_sigsuspend(mask, sigsetsize) */
#define SYS_ALARM       78 /* alarm(seconds) */
#define SYS_PAUSE       79 /* pause() */

/* setitimer(ITIMER_REAL, ...): musl's own alarm() (src/unistd/
 * alarm.c) does NOT call the raw alarm(2) syscall at all - it is
 * implemented in terms of this instead, so it is the one that
 * actually has to work for alarm() to do anything. Only ITIMER_REAL,
 * one-shot (it_interval is read but a periodic repeat is not
 * supported - alarm() itself never sets one), same alarm_deadline_ms
 * mechanism as SYS_ALARM. */
#define SYS_SETITIMER   80

/* uname(struct utsname*) / sethostname(name, len): musl's own
 * gethostname() (src/unistd/gethostname.c) is implemented in terms of
 * uname() - it is NOT a separate syscall - so uname() filling
 * nodename is what makes both work. See kernel/core/hostname.c for
 * the single buffer both syscalls read/write, and main.c's
 * load_hostname() for how /etc/hostname seeds it at boot. */
#define SYS_UNAME       81 /* uname(struct utsname*) */
#define SYS_SETHOSTNAME 82 /* sethostname(name, len) - root only */

/* getprocs(buf, bufsize): TUS's own ABI (no Linux equivalent, so musl
 * has no wrapper - userspace calls this directly via int $0x80, same
 * as SYS_READDIR). Fills buf with one struct tus_procinfo per live
 * task (see kernel/sched/sched.h) and returns the count, or a
 * negative errno. This is what let /bin/ps and /bin/pkill exist at
 * all: before this, only tsh's own cmd_ps (ring 0) could see the task
 * table, via task_list_all() directly. */
#define SYS_GETPROCS    83 /* getprocs(struct tus_procinfo *buf, size_t bufsize) */

/* panic(kind): root-only, no return - paints the full-screen coloured
 * panic display (kernel/core/panic_screen.h) and halts, exactly like
 * SYS_POWER halts/reboots. This is tusSM's escalation path: when a
 * service marked critical crashes past its restart budget, tussm
 * (userspace/tussm.c) calls this with TUS_PANIC_BSOD instead of just
 * logging the failure. No musl wrapper - same raw int $0x80 pattern
 * as SYS_GETPROCS/SYS_READDIR above. */
#define SYS_PANIC       84
#define SYS_CAPSET      85 /* capset(pid, uint32_t caps) - root only;
                             * grants exactly `caps` (see kernel/sched/cap.h)
                             * to task `pid` (0 = self) */
#define TUS_PANIC_BSOD   0
#define TUS_PANIC_GSOD   1
#define TUS_PANIC_RSOD   2

/* IDT entry stub (vector 0x80). Installed by idt_init(). */
void syscall_entry(void);

/* Perform a syscall from kernel mode (what tsh uses today).
 *
 * The kernel stub (syscall_entry) uses the argument registers as
 * scratch and never restores them - only RAX comes back. Every
 * argument register is therefore declared read-write ("+"): GCC
 * knows the asm may clobber them and reloads them before every call
 * instead of assuming they survived the previous one. */
static inline long syscall(long number, long a1, long a2, long a3,
                           long a4, long a5) {
    long ret;
    register long r10 __asm__("r10") = a4;
    register long r8 __asm__("r8") = a5;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = a1;
    register long rsi __asm__("rsi") = a2;
    register long rdx __asm__("rdx") = a3;
    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"(number)
                     : "rcx", "r11", "memory");
    return ret;
}

#endif /* TUS_SYSCALL_SYSCALL_H */
