/*
 * cmd_fs.c - tsh commands for files, devices and time
 *
 * Every command in this file talks to the kernel through the POSIX
 * syscall ABI (see syscall.h) instead of calling kernel functions
 * directly. That keeps the shell honest as the first "program" that
 * exercises the system call interface, and every one of these
 * commands will keep working unchanged once user processes exist.
 */

#include <stdarg.h>

#include "commands.h"

#include "tsh.h"
#include "../core/console.h"
#include "../core/klib.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/rtc/rtc.h"
#include "../elf/tus_elf.h"
#include "../sched/sched.h"
#include "../term/term.h"
#include "../syscall/syscall.h"
#include "../vfs/devices.h"
#include "../vfs/vfs.h"

/* ---- helpers ---- */

/* Current working directory of the shell. The VFS itself only knows
 * absolute paths; the shell resolves every relative path against
 * this before calling the syscall ABI (a real UNIX shell would hand
 * the kernel a relative path and let it resolve - our VFS is still
 * absolute-only, so the shell does the resolution). */
static char g_cwd[128] = "/";
static char g_oldpwd[128] = ""; /* previous directory (cd -) */

/* ... of the CONSOLE shell. A terminal window runs a shell of its
 * own, and `cd` in one window must not move another: the buffers
 * follow the session the calling task belongs to (kernel/term). */
static char *cwd_buf(void) {
    struct tsh_term *t = term_current();
    return t != NULL ? t->cwd : g_cwd;
}

static char *oldpwd_buf(void) {
    struct tsh_term *t = term_current();
    return t != NULL ? t->oldpwd : g_oldpwd;
}

/* Longest normalized path we hand to the syscall ABI. */
#define PATH_BUF 256

const char *shell_cwd(void) {
    return cwd_buf();
}

/* Resolve `in` (absolute or relative) against the shell's own cwd.
 * The algorithm itself now lives in kernel/vfs/vfs.c as
 * vfs_path_resolve() (a ring-3 task's chdir()/open() reach the same
 * logic through vfs_lookup()); this is a thin wrapper supplying the
 * console/terminal-session cwd cmd_fs.c has always tracked. */
static void path_resolve(const char *in, char *out, size_t outsz) {
    vfs_path_resolve(cwd_buf(), in, out, outsz);
}

static void print_syscall_error(const char *what, long err) {
    kprintf("%s: error %ld\n", what, -err);
}

/* ---- pwd / cd ---- */

/*
 * Print to the shell's standard output instead of straight to the
 * console.
 *
 * A built-in that calls kprintf() writes to the screen no matter what
 * the command line said; one that writes to fd 1 obeys `> file` and
 * `|` exactly like a program in /bin does, because redirection is
 * nothing but a different file behind that descriptor. Error and
 * usage messages stay on the console on purpose - that is where a
 * user is looking when a command was typed wrong.
 */
static char g_out_buf[512];
static size_t g_out_len;

static void shell_out_sink(char c) {
    if (g_out_len < sizeof(g_out_buf)) {
        g_out_buf[g_out_len++] = c;
    }
    if (g_out_len == sizeof(g_out_buf)) {
        syscall(SYS_WRITE, 1, (long)g_out_buf, (long)g_out_len, 0, 0);
        g_out_len = 0;
    }
}

static void shell_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    g_out_len = 0;
    kvprintf(shell_out_sink, fmt, args);
    va_end(args);
    if (g_out_len > 0) {
        syscall(SYS_WRITE, 1, (long)g_out_buf, (long)g_out_len, 0, 0);
        g_out_len = 0;
    }
}

static int cmd_pwd(int argc, char **argv) {
    (void)argc;
    (void)argv;
    shell_printf("%s\n", cwd_buf());
    return 0;
}

static int cmd_cd(int argc, char **argv) {
    const char *target = "/";
    bool print_target = false;

    if (argc > 1) {
        if (strcmp(argv[1], "-") == 0) {
            /* cd - : go to the previous directory and print it. */
            if (oldpwd_buf()[0] == '\0') {
                kprintf("cd: no previous directory\n");
                return 1;
            }
            target = oldpwd_buf();
            print_target = true;
        } else if (strcmp(argv[1], "~") == 0) {
            target = "/"; /* HOME; TUS has no per-user homes yet */
        } else {
            target = argv[1];
        }
    }

    char resolved[PATH_BUF];
    path_resolve(target, resolved, sizeof(resolved));

    /* Only enter real directories: open + readdir succeeds for dirs,
     * returns -ENOTDIR for files. */
    long fd = syscall(SYS_OPEN, (long)resolved, O_RDONLY, 0, 0, 0);
    if (fd < 0) {
        print_syscall_error("cd", fd);
        return 1;
    }
    struct vfs_dirent ent;
    long r = syscall(SYS_READDIR, fd, (long)&ent, sizeof(ent), 0, 0);
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    if (r < 0) {
        kprintf("cd: not a directory: %s\n", resolved);
        return 1;
    }

    strncpy(oldpwd_buf(), cwd_buf(), sizeof(g_oldpwd) - 1);
    oldpwd_buf()[sizeof(g_oldpwd) - 1] = '\0';
    strncpy(cwd_buf(), resolved, sizeof(g_cwd) - 1);
    cwd_buf()[sizeof(g_cwd) - 1] = '\0';
    if (print_target) {
        kprintf("%s\n", resolved);
    }
    return 0;
}

/* ---- ls ---- */

/* ---- ls ---- */

/* Format the permission bits the way ls -l does: -rwxr-xr-x, with
 * 's'/'S' for setuid/setgid and 't'/'T' for the sticky bit. */
static void mode_string(uint32_t mode, uint32_t type, char out[11]) {
    out[0] = (type == VFS_DIR)    ? 'd'
           : (type == VFS_DEVICE) ? 'c'
           : (type == VFS_SOCKET) ? 's'
                                  : '-';
    static const char rwx[10] = "rwxrwxrwx";
    for (int i = 0; i < 9; i++) {
        out[1 + i] = (mode & (0400u >> i)) ? rwx[i] : '-';
    }
    if (mode & 04000) {
        out[3] = (out[3] == 'x') ? 's' : 'S';
    }
    if (mode & 02000) {
        out[6] = (out[6] == 'x') ? 's' : 'S';
    }
    if (mode & 01000) {
        out[9] = (out[9] == 'x') ? 't' : 'T';
    }
    out[10] = '\0';
}

struct ls_entry {
    char name[VFS_NAME_MAX];
    uint32_t type;
    uint32_t size;
    uint32_t mode;
};

static int cmd_ls(int argc, char **argv) {
    bool long_fmt = false;
    bool all = false;
    const char *target = NULL;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            for (const char *p = argv[i] + 1; *p != '\0'; p++) {
                if (*p == 'l') {
                    long_fmt = true;
                } else if (*p == 'a') {
                    all = true;
                } else {
                    kprintf("ls: invalid option -- '%c'\n", *p);
                    return 1;
                }
            }
        } else if (target == NULL) {
            target = argv[i];
        }
    }

    char resolved[PATH_BUF];
    if (target != NULL) {
        path_resolve(target, resolved, sizeof(resolved));
    } else {
        strncpy(resolved, cwd_buf(), sizeof(resolved) - 1);
        resolved[sizeof(resolved) - 1] = '\0';
    }

    long fd = syscall(SYS_OPEN, (long)resolved, O_RDONLY, 0, 0, 0);
    if (fd < 0) {
        print_syscall_error("ls", fd);
        return 1;
    }

    /* Collect and sort the entries (UNIX ls sorts by name). */
    struct ls_entry ents[128];
    int count = 0;
    struct vfs_dirent ent;
    long n;
    while ((n = syscall(SYS_READDIR, fd, (long)&ent, sizeof(ent), 0, 0)) > 0
           && count < 128) {
        if (!all && ent.name[0] == '.') {
            continue;
        }
        strncpy(ents[count].name, ent.name, VFS_NAME_MAX - 1);
        ents[count].name[VFS_NAME_MAX - 1] = '\0';
        ents[count].type = ent.type;
        ents[count].size = ent.size;
        ents[count].mode = ent.mode;
        count++;
    }
    if (n < 0) {
        print_syscall_error("ls", n);
    }
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(ents[j].name, ents[i].name) < 0) {
                struct ls_entry tmp = ents[i];
                ents[i] = ents[j];
                ents[j] = tmp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        if (long_fmt) {
            char m[11];
            mode_string(ents[i].mode, ents[i].type, m);
            const char *kind = (ents[i].type == VFS_DIR)    ? "/"
                             : (ents[i].type == VFS_SOCKET) ? "=" : "";
            shell_printf("%s root root %8u %s%s\n", m, ents[i].size,
                         ents[i].name, kind);
        } else {
            shell_printf("%s\n", ents[i].name);
        }
    }
    return 0;
}

/* ---- cat ---- */

static int cmd_cat(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: cat <path>\n");
        return 1;
    }

    char resolved[PATH_BUF];
    path_resolve(argv[1], resolved, sizeof(resolved));

    long fd = syscall(SYS_OPEN, (long)resolved, O_RDONLY, 0, 0, 0);
    if (fd < 0) {
        print_syscall_error("cat", fd);
        return 1;
    }
    char buf[256];
    long n;
    while ((n = syscall(SYS_READ, fd, (long)buf, sizeof(buf), 0, 0)) > 0) {
        syscall(SYS_WRITE, 1, (long)buf, n, 0, 0);
    }
    if (n < 0) {
        print_syscall_error("cat", n);
    }
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    return 0;
}

/* ---- echo (with `> file` redirection) ---- */

static int cmd_echo(int argc, char **argv) {
    /* Detect a ">" / ">>" redirection: echo a b > path */
    long fd = 1; /* stdout by default */
    int end_arg = argc;
    char resolved[PATH_BUF];

    for (int i = 1; i < argc; i++) {
        int flags = -1;
        if (strcmp(argv[i], ">") == 0 && i + 1 < argc) {
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        } else if (strcmp(argv[i], ">>") == 0 && i + 1 < argc) {
            flags = O_WRONLY | O_CREAT | O_APPEND;
        }
        if (flags >= 0) {
            path_resolve(argv[i + 1], resolved, sizeof(resolved));
            fd = syscall(SYS_OPEN, (long)resolved, flags, 0, 0, 0);
            if (fd < 0) {
                print_syscall_error("echo", fd);
                return 1;
            }
            end_arg = i;
            break;
        }
    }

    for (int i = 1; i < end_arg; i++) {
        if (i > 1) {
            syscall(SYS_WRITE, fd, (long)" ", 1, 0, 0);
        }
        syscall(SYS_WRITE, fd, (long)argv[i], (long)strlen(argv[i]), 0, 0);
    }
    syscall(SYS_WRITE, fd, (long)"\n", 1, 0, 0);

    if (fd != 1) {
        syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    }
    return 0;
}

/* ---- mkdir / touch / rm ---- */

static int cmd_mkdir(int argc, char **argv) {
    bool parents = false;
    bool verbose = false;
    uint32_t mode = 0;
    const char *target = NULL;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0' && target == NULL) {
            for (const char *p = argv[i] + 1; *p != '\0'; p++) {
                if (*p == 'p') {
                    parents = true;
                } else if (*p == 'v') {
                    verbose = true;
                } else if (*p == 'm') {
                    if (i + 1 < argc) {
                        mode = (uint32_t)strtoul(argv[++i], NULL, 8);
                    } else {
                        kprintf("mkdir: option requires an argument -- 'm'\n");
                        return 1;
                    }
                } else {
                    kprintf("mkdir: invalid option -- '%c'\n", *p);
                    return 1;
                }
            }
        } else if (target == NULL) {
            target = argv[i];
        }
    }
    if (target == NULL) {
        console_write("usage: mkdir [-p] [-v] [-m mode] <directory>\n");
        return 1;
    }

    char resolved[PATH_BUF];
    path_resolve(target, resolved, sizeof(resolved));

    long r;
    if (parents) {
        /* Create every missing component of the path. */
        char comp[PATH_BUF];
        size_t len = strlen(resolved);
        r = 0;
        for (size_t i = 1; i <= len; i++) {
            if (i == len || resolved[i] == '/') {
                size_t n = i;
                if (i == len && n > 1 && resolved[n - 1] == '/') {
                    n--;
                }
                memcpy(comp, resolved, n);
                comp[n] = '\0';
                if (n > 1) {
                    long rr = syscall(SYS_MKDIR, (long)comp, mode, 0, 0, 0);
                    if (rr < 0 && rr != -17 /* EEXIST */) {
                        r = rr;
                        break;
                    } else if (verbose && rr == 0) {
                        kprintf("mkdir: created directory '%s'\n", comp);
                    }
                }
            }
        }
    } else {
        r = syscall(SYS_MKDIR, (long)resolved, mode, 0, 0, 0);
        if (r == 0 && verbose) {
            kprintf("mkdir: created directory '%s'\n", resolved);
        }
    }
    if (r < 0) {
        print_syscall_error("mkdir", r);
        return 1;
    }
    return 0;
}

static int cmd_touch(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: touch <path>\n");
        return 1;
    }
    char resolved[PATH_BUF];
    path_resolve(argv[1], resolved, sizeof(resolved));
    long fd = syscall(SYS_OPEN, (long)resolved, O_CREAT | O_RDWR, 0, 0, 0);
    if (fd < 0) {
        print_syscall_error("touch", fd);
        return 1;
    }
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    return 0;
}

static int cmd_rm(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: rm <path>\n");
        return 1;
    }
    char resolved[PATH_BUF];
    path_resolve(argv[1], resolved, sizeof(resolved));
    long r = syscall(SYS_UNLINK, (long)resolved, 0, 0, 0, 0);
    if (r < 0) {
        print_syscall_error("rm", r);
        return 1;
    }
    return 0;
}

/* ---- uptime / sleep ---- */

static int cmd_uptime(int argc, char **argv) {
    (void)argc;
    (void)argv;
    long ms = syscall(SYS_UPTIME, 0, 0, 0, 0, 0);
    shell_printf("uptime: %ld.%03ld s\n", ms / 1000, ms % 1000);
    return 0;
}

static int cmd_sleep(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: sleep <milliseconds>\n");
        return 1;
    }
    syscall(SYS_SLEEP, (long)strtoul(argv[1], NULL, 10), 0, 0, 0, 0);
    return 0;
}

/* ---- fbfill: paint the whole framebuffer via an ioctl ---- */

static int cmd_fbfill(int argc, char **argv) {
    uint32_t color = 0xFFFFFF; /* default: white */
    if (argc > 1) {
        color = (uint32_t)strtoul(argv[1], NULL, 16) & 0xFFFFFF;
    }

    long fd = syscall(SYS_OPEN, (long)"/dev/fb0", O_WRONLY, 0, 0, 0);
    if (fd < 0) {
        print_syscall_error("fbfill", fd);
        return 1;
    }
    long r = syscall(SYS_IOCTL, fd, FB_IOCTL_FILL, (long)&color, 0, 0);
    if (r < 0) {
        print_syscall_error("fbfill", r);
    } else {
        kprintf("fb0: filled with #%06x\n", color);
    }
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    return 0;
}

/* ---- exec: run a static ELF image ---- */

static int cmd_exec(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: exec <static-elf-path> [args...]\n");
        return 1;
    }
    char resolved[PATH_BUF];
    path_resolve(argv[1], resolved, sizeof(resolved));
    /* Hand the console keyboard to the new program: it may be a
     * foreground application (kilo) that wants the tty in raw mode.
     * The new task claims ownership on its first read and gives it
     * back when it exits. */
    kbd_input_release(kbd_input_owner());
    elf_exec(resolved, argc - 2, &argv[2]);
    return 0;
}

/* ---- ps: list tasks ---- */

static int cmd_ps(int argc, char **argv) {
    (void)argc;
    (void)argv;
    console_write("PID  STATE   CR3       NAME\n");
    task_list_all();
    return 0;
}

/* ---- date, whoami, id: wall clock and identity ---- */

/* /etc/passwd's numeric uid field, matched against the one the kernel
 * knows about (SYS_GETUID etc.) - the only lookup `whoami` and `id`
 * need, and small enough not to be worth a general passwd-parsing
 * library for. */
static int passwd_name_for_uid(uint32_t uid, char *out, size_t outsz) {
    long fd = syscall(SYS_OPEN, (long)"/etc/passwd", O_RDONLY, 0, 0, 0);
    if (fd < 0) {
        return -1;
    }
    static char buf[4096];
    long n = syscall(SYS_READ, fd, (long)buf, sizeof(buf) - 1, 0, 0);
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    if (n <= 0) {
        return -1;
    }
    buf[n] = '\0';

    char *line = buf;
    while (line < buf + n) {
        char *nl = line;
        while (nl < buf + n && *nl != '\n') {
            nl++;
        }
        char *name_end = line;
        while (name_end < nl && *name_end != ':') {
            name_end++;
        }
        char *pass_end = (name_end < nl) ? name_end + 1 : nl;
        while (pass_end < nl && *pass_end != ':') {
            pass_end++;
        }
        if (name_end < nl && pass_end < nl) {
            unsigned long line_uid = strtoul(pass_end + 1, NULL, 10);
            if (line_uid == uid) {
                size_t namelen = (size_t)(name_end - line);
                if (namelen >= outsz) {
                    namelen = outsz - 1;
                }
                memcpy(out, line, namelen);
                out[namelen] = '\0';
                return 0;
            }
        }
        line = nl + 1;
    }
    return -1;
}

static void fmt2(char *out, int v) {
    if (v < 0) {
        v = 0;
    }
    out[0] = (char)('0' + (v / 10) % 10);
    out[1] = (char)('0' + v % 10);
    out[2] = '\0';
}

static int cmd_date(int argc, char **argv) {
    (void)argc;
    (void)argv;
    long epoch = syscall(SYS_CLOCK, 0, 0, 0, 0, 0);
    int y, mo, d, h, mi, s;
    rtc_civil_from_epoch((uint64_t)epoch, &y, &mo, &d, &h, &mi, &s);
    char mm[3], dd[3], hh[3], mn[3], ss[3];
    fmt2(mm, mo);
    fmt2(dd, d);
    fmt2(hh, h);
    fmt2(mn, mi);
    fmt2(ss, s);
    shell_printf("%d-%s-%s %s:%s:%s UTC\n", y, mm, dd, hh, mn, ss);
    return 0;
}

static int cmd_whoami(int argc, char **argv) {
    (void)argc;
    (void)argv;
    long uid = syscall(SYS_GETUID, 0, 0, 0, 0, 0);
    char name[64];
    if (passwd_name_for_uid((uint32_t)uid, name, sizeof(name)) == 0) {
        shell_printf("%s\n", name);
    } else {
        shell_printf("%ld\n", uid);
    }
    return 0;
}

static int cmd_id(int argc, char **argv) {
    (void)argc;
    (void)argv;
    long uid = syscall(SYS_GETUID, 0, 0, 0, 0, 0);
    long gid = syscall(SYS_GETGID, 0, 0, 0, 0, 0);
    long euid = syscall(SYS_GETEUID, 0, 0, 0, 0, 0);
    char name[64];
    if (passwd_name_for_uid((uint32_t)uid, name, sizeof(name)) == 0) {
        shell_printf("uid=%ld(%s) gid=%ld euid=%ld\n", uid, name, gid, euid);
    } else {
        shell_printf("uid=%ld gid=%ld euid=%ld\n", uid, gid, euid);
    }
    return 0;
}

/* ---- head / tail / wc / cp / df ---- */

static int cmd_head(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: head <path> [lines]\n");
        return 1;
    }
    long want = argc >= 3 ? (long)strtoul(argv[2], NULL, 10) : 10;
    char resolved[PATH_BUF];
    path_resolve(argv[1], resolved, sizeof(resolved));
    long fd = syscall(SYS_OPEN, (long)resolved, O_RDONLY, 0, 0, 0);
    if (fd < 0) {
        print_syscall_error("head", fd);
        return 1;
    }
    char buf[256];
    long n;
    long lines = 0;
    while (lines < want &&
          (n = syscall(SYS_READ, fd, (long)buf, sizeof(buf), 0, 0)) > 0) {
        long i;
        for (i = 0; i < n && lines < want; i++) {
            if (buf[i] == '\n') {
                lines++;
            }
        }
        syscall(SYS_WRITE, 1, (long)buf, i, 0, 0);
    }
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    return 0;
}

static int cmd_tail(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: tail <path> [lines]\n");
        return 1;
    }
    long want = argc >= 3 ? (long)strtoul(argv[2], NULL, 10) : 10;
    char resolved[PATH_BUF];
    path_resolve(argv[1], resolved, sizeof(resolved));
    long fd = syscall(SYS_OPEN, (long)resolved, O_RDONLY, 0, 0, 0);
    if (fd < 0) {
        print_syscall_error("tail", fd);
        return 1;
    }
    /* Read the whole file (capped at 64 KiB - plenty for anything on
     * this rootfs) rather than seeking blind: the last N lines are
     * only knowable once every '\n' between here and the end has been
     * counted. */
    static char buf[65536];
    long total = 0;
    long n;
    while (total < (long)sizeof(buf) &&
          (n = syscall(SYS_READ, fd, (long)(buf + total),
                      sizeof(buf) - (size_t)total, 0, 0)) > 0) {
        total += n;
    }
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    if (total == 0) {
        return 0;
    }

    long total_lines = 0;
    for (long k = 0; k < total; k++) {
        if (buf[k] == '\n') {
            total_lines++;
        }
    }
    if (buf[total - 1] != '\n') {
        total_lines++; /* a trailing partial line still counts */
    }
    long skip = total_lines - want;
    if (skip < 0) {
        skip = 0;
    }

    long start = 0, seen = 0;
    for (long k = 0; k < total && seen < skip; k++) {
        if (buf[k] == '\n') {
            seen++;
            start = k + 1;
        }
    }
    syscall(SYS_WRITE, 1, (long)(buf + start), total - start, 0, 0);
    return 0;
}

static int cmd_wc(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: wc <path>\n");
        return 1;
    }
    char resolved[PATH_BUF];
    path_resolve(argv[1], resolved, sizeof(resolved));
    long fd = syscall(SYS_OPEN, (long)resolved, O_RDONLY, 0, 0, 0);
    if (fd < 0) {
        print_syscall_error("wc", fd);
        return 1;
    }
    char buf[256];
    long n;
    long lines = 0, words = 0, bytes = 0;
    int in_word = 0;
    while ((n = syscall(SYS_READ, fd, (long)buf, sizeof(buf), 0, 0)) > 0) {
        bytes += n;
        for (long i = 0; i < n; i++) {
            char c = buf[i];
            if (c == '\n') {
                lines++;
            }
            if (c == ' ' || c == '\t' || c == '\n') {
                in_word = 0;
            } else if (!in_word) {
                in_word = 1;
                words++;
            }
        }
    }
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    shell_printf("%ld %ld %ld %s\n", lines, words, bytes, argv[1]);
    return 0;
}

static int cmd_cp(int argc, char **argv) {
    if (argc < 3) {
        console_write("usage: cp <src> <dst>\n");
        return 1;
    }
    char rsrc[PATH_BUF], rdst[PATH_BUF];
    path_resolve(argv[1], rsrc, sizeof(rsrc));
    path_resolve(argv[2], rdst, sizeof(rdst));

    long sfd = syscall(SYS_OPEN, (long)rsrc, O_RDONLY, 0, 0, 0);
    if (sfd < 0) {
        print_syscall_error("cp", sfd);
        return 1;
    }
    long dfd = syscall(SYS_OPEN, (long)rdst, O_CREAT | O_WRONLY | O_TRUNC, 0, 0, 0);
    if (dfd < 0) {
        print_syscall_error("cp", dfd);
        syscall(SYS_CLOSE, sfd, 0, 0, 0, 0);
        return 1;
    }

    char buf[512];
    long n;
    while ((n = syscall(SYS_READ, sfd, (long)buf, sizeof(buf), 0, 0)) > 0) {
        long off = 0;
        while (off < n) {
            long w = syscall(SYS_WRITE, dfd, (long)(buf + off), n - off, 0, 0);
            if (w <= 0) {
                break;
            }
            off += w;
        }
    }
    syscall(SYS_CLOSE, sfd, 0, 0, 0, 0);
    syscall(SYS_CLOSE, dfd, 0, 0, 0, 0);
    return 0;
}

static int cmd_df(int argc, char **argv) {
    (void)argc;
    (void)argv;
    static const char *disks[] = { "/dev/hda", "/dev/hdb", "/dev/hdc", "/dev/hdd" };
    int found = 0;
    shell_printf("DEVICE      SIZE\n");
    for (int i = 0; i < 4; i++) {
        long fd = syscall(SYS_OPEN, (long)disks[i], O_RDONLY, 0, 0, 0);
        if (fd < 0) {
            continue;
        }
        long size = syscall(SYS_LSEEK, fd, 0, SEEK_END, 0, 0);
        syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
        if (size > 0) {
            found = 1;
            shell_printf("%-11s %ld MiB\n", disks[i], size / (1024 * 1024));
        }
    }
    if (!found) {
        console_write("no disks attached\n");
    }
    return 0;
}

/* ---- kill / pkill: no real signal delivery, so every kill is a
 * SIGKILL - the task's console, windows, terminal sessions and open
 * files are released and it is marked a zombie right away. ---- */

static int cmd_kill(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: kill <pid> [pid...]\n");
        return 1;
    }
    int failed = 0;
    for (int i = 1; i < argc; i++) {
        uint32_t pid = (uint32_t)strtoul(argv[i], NULL, 10);
        if (sched_current() != NULL && pid == sched_current()->pid) {
            shell_printf("kill: %u: refusing to kill this shell\n", pid);
            failed = 1;
            continue;
        }
        if (task_kill(pid, 137) != 0) {
            shell_printf("kill: %u: no such process\n", pid);
            failed = 1;
        }
    }
    return failed;
}

static int cmd_pkill(int argc, char **argv) {
    if (argc < 2) {
        console_write("usage: pkill <name-substring>\n");
        return 1;
    }
    int n = task_kill_by_name(argv[1], 137);
    shell_printf("pkill: %d process%s killed\n", n, n == 1 ? "" : "es");
    return n > 0 ? 0 : 1;
}

/* ---- sm (tusSM status) ----
 *
 * tussm (userspace/tussm.c) republishes /var/run/tussm.status every
 * time a service's state changes: "name pid state restarts", one line
 * per entry in its hardcoded service table. `sm` (and `service`, the
 * more familiar alias) just cats it with a header - the same "read a
 * small file tusSM already keeps current" shape procfs.c uses for
 * /proc/uptime etc., not a live IPC query, so tusSM does not need to
 * be interrupted to answer this. A missing file (tusSM has not
 * started yet, or could not start) is reported plainly rather than as
 * a raw ENOENT. */
static int cmd_sm(int argc, char **argv) {
    (void)argc;
    (void)argv;

    long fd = syscall(SYS_OPEN, (long)"/var/run/tussm.status", O_RDONLY, 0, 0, 0);
    if (fd < 0) {
        console_write("sm: tusSM has not published a status yet "
                      "(not started, or still starting)\n");
        return 1;
    }

    console_write("SERVICE      PID    STATE      RESTARTS\n");
    char buf[256];
    long n;
    while ((n = syscall(SYS_READ, fd, (long)buf, sizeof(buf), 0, 0)) > 0) {
        syscall(SYS_WRITE, 1, (long)buf, n, 0, 0);
    }
    syscall(SYS_CLOSE, fd, 0, 0, 0, 0);
    return 0;
}

/* Command table additions, referenced from commands.c. */
const struct shell_command g_fs_commands[] = {
    { "ls",      "list a directory",            cmd_ls },
    { "cat",     "print a file or device",      cmd_cat },
    { "echo",    "print text (supports > file)", cmd_echo },
    { "mkdir",   "create a directory",          cmd_mkdir },
    { "touch",   "create an empty file",        cmd_touch },
    { "rm",      "remove a file",               cmd_rm },
    { "cd",      "change the working directory", cmd_cd },
    { "pwd",     "print the working directory", cmd_pwd },
    { "uptime",  "time since boot",             cmd_uptime },
    { "sleep",   "wait N milliseconds",         cmd_sleep },
    { "fbfill",  "fill the framebuffer with a color", cmd_fbfill },
    { "exec",    "run a static ELF binary",     cmd_exec },
    { "ps",      "list running tasks",          cmd_ps },
    { "kill",    "terminate a task by pid",      cmd_kill },
    { "pkill",   "terminate tasks by name",      cmd_pkill },
    { "date",    "show the wall clock (UTC)",    cmd_date },
    { "whoami",  "print the current user name",  cmd_whoami },
    { "id",      "print uid/gid/euid",           cmd_id },
    { "head",    "print the first lines of a file", cmd_head },
    { "tail",    "print the last lines of a file", cmd_tail },
    { "wc",      "count lines/words/bytes",      cmd_wc },
    { "cp",      "copy a file",                  cmd_cp },
    { "df",      "show attached disk sizes",     cmd_df },
    { "sm",      "show tusSM service status",    cmd_sm },
    { "service", "show tusSM service status",    cmd_sm },
};

const size_t g_fs_command_count =
    sizeof(g_fs_commands) / sizeof(g_fs_commands[0]);
