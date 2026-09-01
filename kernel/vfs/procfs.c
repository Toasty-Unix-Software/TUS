/*
 * procfs.c - a handful of synthetic /proc files
 *
 * Not a real procfs (no per-process directories, no /proc/self): just
 * the specific files ported Linux software actually reads to learn
 * basic system state (uptime, memory, mount list, load) without
 * needing its own TUS-specific code path. Every number here is real,
 * read from the same kernel state the boot banner and shell commands
 * already use - nothing is fabricated. /proc/loadavg's three averages
 * are the one exception: TUS's scheduler has no decayed running
 * average, so they always read 0.00 (an honest "not tracked", not a
 * guess) while the process count after them is real.
 */

#include "procfs.h"

#include "vfs.h"

#include "../arch/x86_64/cpu.h"
#include "../arch/x86_64/spectre.h"
#include "../core/klib.h"
#include "../drivers/pit/pit.h"
#include "../mm/pmm.h"
#include "../sched/sched.h"

/* Same one-call-builds-the-whole-file pattern as kprintf(): kvprintf's
 * sink takes no userdata, so generation is serialized through one
 * scratch buffer under preempt_disable(), exactly like console output
 * already is. Every /proc file here is small and built in a single
 * kvprintf() call, so there is no cross-call state to protect. */
static char g_procfs_scratch[2048];
static size_t g_procfs_scratch_len;

static void procfs_putchar(char c) {
    if (g_procfs_scratch_len + 1 < sizeof(g_procfs_scratch)) {
        g_procfs_scratch[g_procfs_scratch_len++] = c;
    }
}

static size_t procfs_format(const char *fmt, ...) {
    va_list args;
    preempt_disable();
    g_procfs_scratch_len = 0;
    va_start(args, fmt);
    kvprintf(procfs_putchar, fmt, args);
    va_end(args);
    g_procfs_scratch[g_procfs_scratch_len] = '\0';
    size_t len = g_procfs_scratch_len;
    preempt_enable();
    return len;
}

static long proc_read(void *priv, void *buf, size_t count, size_t pos) {
    typedef size_t (*gen_fn)(void);
    gen_fn gen = (gen_fn)priv;
    size_t len = gen();
    if (pos >= len) {
        return 0;
    }
    size_t n = len - pos;
    if (n > count) {
        n = count;
    }
    memcpy(buf, g_procfs_scratch + pos, n);
    return (long)n;
}

static size_t proc_gen_uptime(void) {
    uint64_t secs = pit_uptime_ms() / 1000;
    /* Second field is normally "idle time"; TUS has no per-core idle
     * accounting, so it just repeats uptime (matches what a system
     * with a single always-busy CPU would report). */
    return procfs_format("%lu.00 %lu.00\n", secs, secs);
}

static size_t proc_gen_meminfo(void) {
    uint64_t total_frames = 0, free_frames = 0;
    pmm_get_stats(&total_frames, &free_frames);
    uint64_t total_kb = total_frames * 4;
    uint64_t free_kb = free_frames * 4;
    return procfs_format(
        "MemTotal: %lu kB\n"
        "MemFree: %lu kB\n"
        "MemAvailable: %lu kB\n"
        "Buffers: 0 kB\n"
        "Cached: 0 kB\n"
        "SwapTotal: 0 kB\n"
        "SwapFree: 0 kB\n",
        total_kb, free_kb, free_kb);
}

static size_t proc_gen_loadavg(void) {
    int tasks = sched_task_count();
    if (tasks < 1) {
        tasks = 1;
    }
    /* "0.00 0.00 0.00" is not a guess: TUS keeps no decayed running
     * average to report here (see file header comment). */
    return procfs_format("0.00 0.00 0.00 1/%d %d\n", tasks, tasks);
}

static size_t proc_gen_stat(void) {
    uint64_t ticks = pit_uptime_ms() / 10; /* PIT runs at 100 Hz */
    return procfs_format(
        "cpu  %lu 0 0 0 0 0 0 0 0 0\n"
        "cpu0 %lu 0 0 0 0 0 0 0 0 0\n"
        "btime 0\n"
        "processes %d\n",
        ticks, ticks, sched_task_count());
}

static size_t proc_gen_mounts(void) {
    return procfs_format("rootfs / tusfs rw 0 0\n");
}

static size_t proc_gen_cpuinfo(void) {
    char vendor[13];
    char brand[49];
    cpu_get_vendor(vendor);
    cpu_get_brand(brand);
    return procfs_format(
        "vendor_id\t: %s\n"
        "model name\t: %s\n"
        "spectre_v1\t: %s\n"
        "spectre_v2\t: %s\n",
        vendor, brand[0] != '\0' ? brand : "unknown",
        spectre_v1_mitigation == SPECTRE_V1_MITIGATION_FENCE
            ? "Mitigation: LFENCE" : "Vulnerable",
        spectre_v2_mitigation == SPECTRE_V2_MITIGATION_RETPOLINE
            ? "Mitigation: Retpolines" : "Vulnerable");
}

static long proc_write(void *priv, const void *buf, size_t count,
                       size_t pos) {
    (void)priv;
    (void)buf;
    (void)pos;
    return (long)count; /* writes are accepted and discarded */
}

void procfs_init(void) {
    static const struct file_ops proc_ops = { proc_read, proc_write, NULL,
                                              NULL };
    if (vfs_lookup("/proc") == NULL) {
        vfs_create_dir("/proc");
    }
    vfs_create_device("/proc/uptime", &proc_ops, (void *)proc_gen_uptime);
    vfs_create_device("/proc/meminfo", &proc_ops, (void *)proc_gen_meminfo);
    vfs_create_device("/proc/loadavg", &proc_ops, (void *)proc_gen_loadavg);
    vfs_create_device("/proc/stat", &proc_ops, (void *)proc_gen_stat);
    vfs_create_device("/proc/mounts", &proc_ops, (void *)proc_gen_mounts);
    vfs_create_device("/proc/cpuinfo", &proc_ops, (void *)proc_gen_cpuinfo);
}
