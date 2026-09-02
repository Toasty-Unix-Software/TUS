/*
 * main.c - TUS kernel entry point
 *
 * Boot flow:
 *   1. Limine (see limine.conf) switches the CPU to 64-bit long mode,
 *      maps this ELF at its linked higher-half addresses, sets up a
 *      stack and a flat GDT, and jumps to _start().
 *   2. _start() collects the boot protocol responses into g_bootinfo.
 *   3. The console (serial + framebuffer), IDT, PIC and the keyboard
 *      driver are initialized.
 *   4. Interrupts are enabled and control passes to the TUS shell.
 */

#include <limine.h>

#include "arch/x86_64/acpi.h"
#include "arch/x86_64/cpu.h"
#include "arch/x86_64/spectre.h"
#include "arch/x86_64/gdt.h"
#include "arch/x86_64/idt.h"
#include "syscall/linux_syscall.h"
#include "arch/x86_64/io.h"
#include "arch/x86_64/lapic.h"
#include "arch/x86_64/pic.h"
#include "arch/x86_64/smp.h"
#include "boot/splash.h"
#include "core/bootinfo.h"
#include "core/console.h"
#include "core/klib.h"
#include "core/random.h"
#include "core/hostname.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "drivers/fb/fb.h"
#include "drivers/ata/ata.h"
#include "drivers/ahci/ahci.h"
#include "drivers/nvme/nvme.h"
#include "drivers/pit/pit.h"
#include "drivers/rtc/rtc.h"
#include "drivers/serial/serial.h"
#include "drivers/rtl8139/rtl8139.h"
#include "drivers/pci/pci.h"
#include "drivers/vbe/vbe.h"
#include "drivers/xhci/xhci.h"
#include "drivers/hda/hda.h"
#include "drivers/usbhid/usbhid.h"
#include "drivers/keymap/keymap.h"
#include "drivers/ehci/ehci.h"
#include "drivers/ath9k/ath9k.h"
#include "drivers/hpet/hpet.h"
#include "drivers/pmtimer/pmtimer.h"
#include "elf/tus_elf.h"
#include "mm/kmalloc.h"
#include "mm/pmm.h"
#include "mm/swap.h"
#include "mm/vmm.h"
#include "net/dns.h"
#include "net/ip.h"
#include "net/ipv6.h"
#include "net/netif.h"
#include "sched/sched.h"
#include "shell/tsh.h"
#include "term/term.h"
#include "vfs/devices.h"
#include "fs/wrf.h"
#include "vfs/rootfs.h"
#include "vfs/vfs.h"

/* ---- Limine boot protocol requests ----
 *
 * Each request is a static struct placed in the ".requests" section
 * (see kernel/linker.ld). Limine fills in the response pointer before
 * the kernel starts; the request structs themselves are left intact.
 */

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_mp_request mp_request = {
    .id = LIMINE_MP_REQUEST_ID,
    .revision = 0
};

/* The kernel's own image, as the bootloader loaded it. TUS installs
 * itself from memory (/dev/kernel), so what lands on a disk is by
 * definition the kernel that is running. */
__attribute__((used, section(".requests")))
static volatile struct limine_executable_file_request kernel_file_request = {
    .id = LIMINE_EXECUTABLE_FILE_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0
};

/* The ACPI RSDP: the starting point for finding the MADT, which is
 * where the Local APIC and I/O APIC live (see kernel/arch/x86_64/
 * acpi.c). Limine finds it itself - via the EFI configuration table
 * or by scanning the BIOS regions - which is more reliable than a
 * kernel doing that scan a second time. */
__attribute__((used, section(".requests")))
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST_ID,
    .revision = 0
};

/*
 * Base revision: tells Limine which protocol features we rely on.
 * Revision 2 means "higher-half kernel, HHDM offset, framebuffers
 * mapped in the higher half". Limine finds this symbol by name in the
 * kernel's symbol table.
 */
static volatile uint64_t limine_base_revision[3] = LIMINE_BASE_REVISION(2);

/* Single source of truth about what the bootloader gave us. */
struct bootinfo g_bootinfo;

/* How long the boot splash (toasts + boot log) stays on screen
 * before the shell clears it, in milliseconds. */
#define BOOT_SPLASH_HOLD_MS 2500

static void fill_bootinfo(void) {
    g_bootinfo.bootloader_name = bootloader_info_request.response != NULL
        ? bootloader_info_request.response->name : NULL;
    g_bootinfo.bootloader_version = bootloader_info_request.response != NULL
        ? bootloader_info_request.response->version : NULL;

    g_bootinfo.framebuffer = (framebuffer_request.response != NULL &&
                              framebuffer_request.response->framebuffer_count > 0)
        ? framebuffer_request.response->framebuffers[0] : NULL;

    g_bootinfo.hhdm_offset = hhdm_request.response != NULL
        ? hhdm_request.response->offset : 0;

    g_bootinfo.rsdp = rsdp_request.response != NULL
        ? rsdp_request.response->address : NULL;

    /* CPU count from the MP feature (includes the BSP). TUS itself is
     * single-CPU for now; the count drives the boot splash (one toast
     * per core) and the banner. */
    g_bootinfo.cpu_count = (mp_request.response != NULL)
        ? mp_request.response->cpu_count : 1;

    /* rootfs.img, loaded by Limine as the first module. */
    if (module_request.response != NULL &&
        module_request.response->module_count > 0) {
        g_bootinfo.rootfs_module = module_request.response->modules[0];
    } else {
        g_bootinfo.rootfs_module = NULL;
    }

    g_bootinfo.kernel_file = (kernel_file_request.response != NULL)
        ? kernel_file_request.response->executable_file : NULL;

    uint64_t total = 0;
    if (memmap_request.response != NULL) {
        for (uint64_t i = 0; i < memmap_request.response->entry_count; i++) {
            struct limine_memmap_entry *entry = memmap_request.response->entries[i];
            if (entry->type == LIMINE_MEMMAP_USABLE) {
                total += entry->length;
            }
        }
    }
    g_bootinfo.usable_memory_bytes = total;
}

/* Park every application processor: the bootloader starts the APs and
 * they spin on their goto_address until we publish one. TUS does not
 * use the APs yet, so we hand them a trivial cli/hlt loop - they stay
 * out of the way (and out of the TCG's way) instead of burning cycles. */
static void ap_park(struct limine_mp_info *cpu) {
    (void)cpu;
    for (;;) {
        asm volatile("cli; hlt");
    }
}

static void park_aps(void) {
    if (mp_request.response == NULL) {
        return;
    }
    for (uint64_t i = 0; i < mp_request.response->cpu_count; i++) {
        struct limine_mp_info *cpu = mp_request.response->cpus[i];
        if (cpu == NULL) {
            continue;
        }
        /* The BSP's goto_address is unused by the bootloader; setting
         * it is harmless and keeps the loop simple. */
        __atomic_store_n(&cpu->goto_address, ap_park, __ATOMIC_SEQ_CST);
    }
}

/* Every Limine memory map entry (type, base, length) - the raw E820-
 * style table the PMM's usable-frame total is computed from. Serial
 * only: at a few dozen entries on real hardware this would swamp the
 * framebuffer text console for no reader standing in front of the
 * screen to use. */
static const char *memmap_type_name(uint64_t type) {
    switch (type) {
    case LIMINE_MEMMAP_USABLE:                 return "usable";
    case LIMINE_MEMMAP_RESERVED:               return "reserved";
    case LIMINE_MEMMAP_ACPI_RECLAIMABLE:       return "acpi-reclaimable";
    case LIMINE_MEMMAP_ACPI_NVS:               return "acpi-nvs";
    case LIMINE_MEMMAP_BAD_MEMORY:             return "bad";
    case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE: return "bootloader-reclaimable";
    case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES: return "kernel-and-modules";
    case LIMINE_MEMMAP_FRAMEBUFFER:            return "framebuffer";
    default:                                   return "unknown";
    }
}

static void log_memory_map(void) {
    if (memmap_request.response == NULL) {
        klog("memmap       : none provided by bootloader\n");
        return;
    }
    uint64_t n = memmap_request.response->entry_count;
    klog("memmap       : %llu entries (Limine E820-style map)\n",
         (unsigned long long)n);
    for (uint64_t i = 0; i < n; i++) {
        struct limine_memmap_entry *e = memmap_request.response->entries[i];
        klog("  [%llu] 0x%llx - 0x%llx  %llu KiB  %s\n",
             (unsigned long long)i, (unsigned long long)e->base,
             (unsigned long long)(e->base + e->length),
             (unsigned long long)(e->length / 1024),
             memmap_type_name(e->type));
    }
}

static void print_boot_banner(void) {
    const char *name = g_bootinfo.bootloader_name
        ? g_bootinfo.bootloader_name : "unknown";
    const char *version = g_bootinfo.bootloader_version
        ? g_bootinfo.bootloader_version : "";

    /* The framebuffer keeps the splash and a short human-readable
     * summary; every technical detail (exact addresses, per-entry
     * memory map, disk model strings, mitigation status) goes to the
     * serial log only via klog() - the reliable, high-bandwidth debug
     * channel documented in kernel/core/console.h, not the 80-column
     * text console someone might actually be reading during boot. */
    console_set_color(0x00FFA040, 0x00000000); /* toast orange */
    console_write("Toasty Unix Software (TUS)\n");
    console_set_color(0x00E8E8E8, 0x00000000);
    console_write("\"Work everywhere, but work right.\"\n");
    console_write("------------------------------------------------\n");
    console_write("Detecting hardware... (full detail on the serial log)\n");

    klog("------------------------------------------------\n");
    klog("bootloader   : %s %s\n", name, version);
    klog("architecture : x86_64 (AMD64)\n");
    klog("cpu count    : %llu\n", (unsigned long long)g_bootinfo.cpu_count);
    klog("memory       : %llu MiB usable\n",
         (unsigned long long)(g_bootinfo.usable_memory_bytes / (1024 * 1024)));
    log_memory_map();
    if (g_bootinfo.framebuffer != NULL) {
        klog("framebuffer  : %llux%llu, %u bpp, pitch %llu @ %p\n",
             (unsigned long long)g_bootinfo.framebuffer->width,
             (unsigned long long)g_bootinfo.framebuffer->height,
             g_bootinfo.framebuffer->bpp,
             (unsigned long long)g_bootinfo.framebuffer->pitch,
             g_bootinfo.framebuffer->address);
    } else {
        klog("framebuffer  : unavailable (serial console only)\n");
    }
    klog("serial       : COM1 @ 115200 8N1 (debug mirror)\n");
    klog("keyboard     : PS/2 scancode set 1, IRQ1\n");
    klog(mouse_present()
             ? (mouse_has_wheel()
                    ? "mouse        : PS/2 aux port, IRQ12, wheel\n"
                    : "mouse        : PS/2 aux port, IRQ12\n")
             : "mouse        : not detected\n");
    klog("timer        : PIT 100 Hz (IRQ0)\n");
    if (ata_disk_count() > 0) {
        for (int i = 0; i < ATA_MAX_DISKS; i++) {
            const struct ata_disk *d = ata_disk(i);
            if (d == NULL || !d->present || d->atapi) {
                continue;
            }
            klog("disk         : /dev/%s  %u MiB  %s  "
                 "(`tusinstall` installs onto it)\n", d->name,
                 (unsigned)(d->sectors / 2048), d->model);
        }
    } else {
        klog("disk         : none (install needs one)\n");
    }
    uint64_t total_frames = 0, free_frames = 0;
    pmm_get_stats(&total_frames, &free_frames);
    klog("pmm          : %llu free / %llu frames (%llu MiB usable)\n",
         (unsigned long long)free_frames,
         (unsigned long long)total_frames,
         (unsigned long long)(g_bootinfo.usable_memory_bytes / (1024 * 1024)));
    klog("vfs          : /dev/fb0 tty0 kbd0 serial0 null zero "
         "random urandom\n");
    klog("Spectre v1   : %s\n",
         spectre_v1_mitigation == SPECTRE_V1_MITIGATION_FENCE
             ? "LFENCE enabled" : "Disabled");
    klog("Spectre v2   : %s\n",
         spectre_v2_mitigation == SPECTRE_V2_MITIGATION_RETPOLINE
             ? "Retpoline active" : "Disabled");
    klog("------------------------------------------------\n");

    console_write("------------------------------------------------\n");
    console_write("tsh ready. Type 'help' to list the commands.\n\n");
}

/* Kernel entry point. Limine provides the stack; never returns. */
/* Read /etc/keymap and load the layout it names. One short line, so
 * the whole file is read into a small buffer and trimmed. */
static void load_keymap(void) {
    int fd = vfs_open("/etc/keymap", 0);
    if (fd < 0) {
        return; /* no file: keep the built-in US layout */
    }
    char buf[32];
    long n = vfs_read(fd, buf, sizeof(buf) - 1);
    vfs_close(fd);
    if (n <= 0) {
        return;
    }
    buf[n] = '\0';
    for (long i = 0; i < n; i++) {
        if (buf[i] == '\n' || buf[i] == '\r' || buf[i] == ' ') {
            buf[i] = '\0';
            break;
        }
    }
    if (buf[0] == '\0') {
        return;
    }
    if (keymap_set(buf) != 0) {
        kprintf("keymap      : /etc/keymap names '%s', which is not a "
                "layout - keeping %s\n", buf, keymap_name());
        return;
    }
    kprintf("keymap      : %s (from /etc/keymap)\n", keymap_name());
}

/* Read /etc/hostname and adopt the name it holds - same shape as
 * load_keymap() just above: one short line from the root filesystem,
 * a missing or unreadable file leaves the built-in default ("tus")
 * rather than failing boot. */
static void load_hostname(void) {
    int fd = vfs_open("/etc/hostname", 0);
    if (fd < 0) {
        return;
    }
    char buf[HOSTNAME_MAX + 1];
    long n = vfs_read(fd, buf, sizeof(buf) - 1);
    vfs_close(fd);
    if (n <= 0) {
        return;
    }
    buf[n] = '\0';
    for (long i = 0; i < n; i++) {
        if (buf[i] == '\n' || buf[i] == '\r' || buf[i] == ' ') {
            buf[i] = '\0';
            n = i;
            break;
        }
    }
    if (buf[0] == '\0') {
        return;
    }
    hostname_set(buf, (size_t)n);
    kprintf("hostname     : %s (from /etc/hostname)\n", hostname_get());
}

/* Start any background service the image was told to enable - the
 * smallest thing that works, not a general service manager nobody
 * asked for: presence of a flag file is the whole config format,
 * checked the same way load_keymap()/load_hostname() check theirs.
 * Called after net_init() (unlike those two), since the one service
 * this starts today - sshd - needs a working network stack to be any
 * use at all. tusinstall (userspace/tusinstall.c) is what actually
 * writes this file into an installed image, by prompting for it. */
static void load_boot_services(void) {
    int fd = vfs_open("/etc/sshd.enable", 0);
    if (fd < 0) {
        return;
    }
    vfs_close(fd);
    long pid = elf_exec("/bin/sshd", 0, NULL);
    if (pid < 0) {
        kprintf("sshd         : /etc/sshd.enable present, but /bin/sshd "
                "could not be started\n");
        return;
    }
    kprintf("sshd         : started (pid %ld, from /etc/sshd.enable)\n", pid);
}

/* Start tusSM (the Toasty Unix Software Service Manager,
 * userspace/tussm.c) - unlike load_boot_services() just above, this
 * is not opt-in behind a flag file: tusSM owns errorD/bootD, which
 * every other service's crash gets logged/journaled through, so the
 * system wants it up unconditionally. Needs the VFS and scheduler
 * (elf_exec()) but nothing from the network stack, so it starts
 * before net_init() rather than after, unlike sshd. */
static void load_tussm(void) {
    long pid = elf_exec("/bin/tussm", 0, NULL);
    if (pid < 0) {
        kprintf("tusSM        : /bin/tussm could not be started\n");
        return;
    }
    kprintf("tusSM        : started (pid %ld)\n", pid);
}

/* Read /etc/resolv.conf and take its first "nameserver" line as the
 * DNS resolver address - net_init() has already set g_netif.dns to
 * QEMU's usermode-network relay (10.0.2.3), which is right for the
 * default test environment and nowhere else. Real hardware, or a
 * different virtual network, needs a different server, and a config
 * file beats hardcoding one address into the kernel for that. A
 * missing file, or one with no valid nameserver line, leaves
 * whatever net_init() already set. */
static void load_resolv_conf(void) {
    int fd = vfs_open("/etc/resolv.conf", 0);
    if (fd < 0) {
        return;
    }
    char buf[512];
    long n = vfs_read(fd, buf, sizeof(buf) - 1);
    vfs_close(fd);
    if (n <= 0) {
        return;
    }
    buf[n] = '\0';

    char *line = buf;
    while (line < buf + n) {
        char *nl = strchr(line, '\n');
        if (nl) {
            *nl = '\0';
        }
        while (*line == ' ' || *line == '\t') {
            line++;
        }
        if (strncmp(line, "nameserver", 10) == 0 &&
            (line[10] == ' ' || line[10] == '\t')) {
            char *p = line + 10;
            while (*p == ' ' || *p == '\t') {
                p++;
            }
            char *end = p;
            while (*end == '.' || (*end >= '0' && *end <= '9')) {
                end++;
            }
            *end = '\0';
            uint32_t addr = dns_parse_ipv4(p);
            if (addr != 0) {
                g_netif.dns = addr;
                kprintf("resolver    : %s (from /etc/resolv.conf)\n", p);
                return;
            }
        }
        if (!nl) {
            break;
        }
        line = nl + 1;
    }
}

/* Require a real login before handing the console over - spawns the
 * same /bin/login (userspace/login.c) a terminal session authenticates
 * with, waits for it (the same sched_task_alive()/hlt() poll
 * exec_pipeline() in kernel/shell/commands.c uses), and only proceeds
 * once it exits 0. login itself allows LOGIN_RETRIES wrong passwords
 * before giving up and exiting 1; unlike a terminal window, which
 * just stays closed, a plain console has nothing else to fall back
 * to, so a failure here starts login over rather than accepting it -
 * this loop, not login's own retry count, is what actually gates the
 * console.
 *
 * If /bin/login cannot even be started (elf_exec fails: a missing
 * binary, a stripped-down rootfs, memory pressure this early), the
 * console falls back to starting the shell directly with a warning,
 * rather than becoming permanently unusable over a problem that has
 * nothing to do with anyone's password - a real authentication
 * failure never takes this path, only a failure to run login at
 * all. */
static void console_login_gate(void) {
    for (;;) {
        long pid = elf_exec("/bin/login", 0, NULL);
        if (pid < 0) {
            console_write("login: /bin/login could not be started; "
                          "continuing without authentication\n");
            return;
        }
        while (sched_task_alive((uint32_t)pid)) {
            hlt();
        }
        int status = 0;
        sched_task_reap((uint32_t)pid, &status);
        if (status == 0) {
            return;
        }
    }
}

void _start(void) {
    cli(); /* build a clean interrupt environment */

    fill_bootinfo();
    park_aps(); /* APs halt in cli/hlt until the (single-CPU) kernel */
    console_init(g_bootinfo.framebuffer);

    /* Own GDT (kernel + user segments + TSS) before any interrupt
     * can fire; IDT selectors point at 0x08. */
    gdt_init();
    idt_init();
    linux_syscall_init(); /* SYSCALL/SYSRET MSRs for Linux-ABI tasks */

    /* Memory: physical frames, page tables, kernel heap. */
    pmm_init(memmap_request.response, g_bootinfo.hhdm_offset);
    vmm_init();
    kmalloc_init();

    /* Reserve the address space the framebuffer window will use.
     * Runtime mode changes map the display adapter's memory there
     * (see VMM_FB_BASE in mm/vmm.h); reserving the page tables now,
     * before the scheduler exists, is what makes those mappings
     * visible to every task created later. */
    vmm_reserve_tables(VMM_FB_BASE, VMM_FB_SIZE);
    vmm_reserve_tables(VMM_MMIO_BASE, VMM_MMIO_SIZE);

    /* pic_init() always brings up the 8259 first (the safe baseline
     * every IRQ line falls back to), then tries to move interrupt
     * routing on to the Local APIC and I/O APIC - which needs
     * vmm_map_mmio(), hence waiting until here rather than running
     * right after idt_init() as it used to. Interrupts stay masked
     * globally either way until sti() far below, so nothing in
     * between depends on the ordering. */
    pic_init();

    /* smp_init() reads the same MADT pic_init() just consulted for the
     * I/O APIC, this time for Processor Local APIC entries - i.e. the
     * CPU list. Must run after pic_init() so lapic_available()/
     * lapic_id() (used to identify which entry is the BSP) are already
     * set up; TUS still only executes on this one core (no AP
     * trampoline), so this is topology discovery, not a core count the
     * scheduler acts on yet. */
    smp_init();

    /* ACPI PM Timer and HPET: two more clock sources than the PIT,
     * neither needed for interrupt routing (that is pic_init() above),
     * so both can be brought up right here - no calibration delay, no
     * dependency on sti() having run yet, just table parsing and (for
     * HPET) an MMIO mapping, exactly the ingredients pic_init() above
     * already needed for the Local/IO APIC. Each fails closed on its
     * own: a machine with no FADT, no PM_TMR_BLK, no HPET table, or a
     * HPET table pointing somewhere vmm_map_mmio() cannot use is
     * simply left without that source - pit_uptime_ms() (pit.c) below
     * only prefers HPET when hpet_available() is true, and nothing
     * here is required for anything else to work. */
    struct acpi_fadt_info fadt;
    if (acpi_parse_fadt(g_bootinfo.rsdp, &fadt) == 0) {
        pmtimer_init(fadt.pm_tmr_blk, fadt.tmr_val_ext);
    }
    struct acpi_hpet_info hpet_info;
    if (acpi_parse_hpet(g_bootinfo.rsdp, &hpet_info) == 0) {
        hpet_init(hpet_info.phys_addr);
    }

    /* Devices and services. */
    pit_init();
    kbd_init();
    mouse_init();
    /* IDE disks. Probed before the device nodes are registered, since
     * /dev/hd* is created from what this finds. */
    ata_init();
    vfs_init();

    /* Mount the root filesystem (rootfs.img, a Limine module): the
     * directory tree (/dev, /tmp, /etc, /boot) and the OS files
     * (user programs, motd, boot logo) come from the image, not from
     * hardcoded kernel code. Device nodes are registered afterwards,
     * once /dev exists. */
    if (g_bootinfo.rootfs_module != NULL) {
        vfs_mount_rootfs(g_bootinfo.rootfs_module->address,
                         g_bootinfo.rootfs_module->size);
    } else {
        console_write("rootfs       : not loaded (no Limine module)\n");
    }

    /* WRF (kernel/fs/wrf.c): TUS's own persistent, writable filesystem
     * - unlike rootfs.img above, this one is not read back into
     * memory from a Limine module, it is read live off a real disk
     * every time a WRF-backed file or directory is touched, so a
     * write survives a reboot. Mounted at /home from the first ATA
     * disk that carries a valid WRF superblock (format one first with
     * mkfs.wrf); harmless no-op with nothing but rootfs.img's ramfs
     * if no such disk is attached. Must run after vfs_init()/the
     * rootfs mount (it needs "/" to exist so it can create /home) and
     * can run before the scheduler: it talks to the disk directly via
     * ata_read()/ata_write(), not through vfs_open()'s fd table. */
    wrf_boot_mount();

    /* Disk-backed swap (kernel/mm/swap.c): claims the first attached
     * ATA disk carrying a SWAP_MAGIC header (written by the `mkswap`
     * userspace tool) as raw page storage. No-op if none is found -
     * same "harmless without the disk" shape as wrf_boot_mount()
     * above. Must run after ata_init(); before the scheduler is fine
     * since it only touches the disk directly, not the fd table. */
    swap_init();

    /* The scheduler: tsh becomes task 0; the PIT (IRQ0) drives
     * round-robin switching. This runs BEFORE the device nodes are
     * registered so the standard descriptors land in task 0's own
     * fd table (vfs_devices_init opens them into the current task). */
    sched_init();
    vfs_devices_init();

    /* AHCI SATA and NVMe: PCI-discovered storage controllers, unlike
     * the fixed-port ata_init() above. Both scan the bus themselves
     * (same pattern rtl8139_probe_pci() uses) and register their own
     * /dev nodes directly (vfs_create_device()), so they only need to
     * run after /dev exists - hence right here, alongside ata_init()
     * in spirit even though they run a few lines later. Both poll
     * with bounded spin loops rather than timer_sleep_ms(), so unlike
     * xHCI they do not need to wait for sti() below. */
    ahci_init();
    nvme_init();

    /* The keyboard layout. /etc/keymap holds one line - a layout name
     * - and comes from the root filesystem, so changing the layout a
     * machine boots with is a one-file change and needs no rebuild.
     * A missing or unreadable file leaves the built-in US layout,
     * which is the only sane fallback: it is the one whose characters
     * a user can still type the file's name with.
     *
     * After sched_init(), not before: file descriptors live in a
     * task's own table, and until the scheduler has made tsh task 0
     * there is no table for vfs_open() to put one in. */
    load_keymap();
    load_hostname();
    load_tussm();

    /* Terminal sessions: from here on, console output belongs to the
     * task that produced it - a terminal window's shell prints into
     * that window, everything else keeps the screen. */
    term_init();

    /* The random pool and the wall clock. Both want the PIT running
     * (the pool times jitter against it, the clock anchors to it), so
     * they come after the timer and before anything that draws on
     * them - TCP's initial sequence numbers, for one. */
    random_init();
    rtc_init();

    /* Network stack initialization (optional, may fail silently). */
    net_init();
    ipv6_init();
    load_resolv_conf();
    load_boot_services();

    /* Wireless driver initialization (ath9k-htc). */
    ath9k_init();

    /* Register USB class drivers for device probing. */
    static usb_class_driver_t ath9k_usb_driver = {
        .class_code = 0xFF,
        .subclass_code = 0xFF,
        .name = "ath9k-usb",
        .probe = (usb_driver_probe_t)ath9k_usb_probe
    };
    ehci_register_class_driver(&ath9k_usb_driver);

    /* HD Audio: registers a class driver by PCI class/subclass, same
     * shape as every other one here - pci_enumerate_devices() below
     * finds and inits it if a controller actually exists. */
    hda_register();

    /* Register and enumerate PCI devices. */
    ehci_init();
    pci_enumerate_devices();

    /* Intel E1000/e1000e Ethernet is brought up inside net_init()
     * above, as the fallback when no RTL8139 is present - see
     * kernel/net/netif.c. Nothing to do here. */

    /* Runtime display mode setting. Needs PCI: the linear
     * framebuffer is the display adapter's BAR0. On hardware with a
     * real GPU there is no Bochs VBE register set, this reports
     * nothing and the machine keeps the mode Limine negotiated. */
    vbe_init();

    /* Default to 1920x1080 when the adapter can do runtime mode
     * setting at all - otherwise a user has to know to run `doas
     * res_set 1920x1080` by hand every boot just to get a reasonable
     * desktop size. Goes through the exact same fb_set_mode() /
     * devices_refresh_fb() pair SYS_VIDEO's TUS_VIDEO_SET_MODE calls
     * (no highx_rebind() - no session exists yet), so the console,
     * the splash drawn right after this, and /dev/fb0 all agree from
     * the very first frame. A failure here (no VBE, or 1920x1080 not
     * accepted) is silent and harmless: fb_set_mode() touches nothing
     * on error, so the machine simply keeps whatever mode Limine
     * negotiated, exactly as it always did before this existed. */
    if (vbe_available()) {
        if (fb_set_mode(1920, 1080) == 0) {
            devices_refresh_fb();
        }
    }

    /* Draw the boot splash: one toast per CPU, boot logs below. */
    splash_show(g_bootinfo.cpu_count);

    /* SSE for user programs: the C library uses SSE2 instructions
     * (memcpy, string ops, math). The scheduler now saves/restores
     * FPU state, so user SSE survives task switches. */
    cpu_enable_sse();

    /* Spectre v1/v2 mitigations: must run before any user task is
     * scheduled and before the banner reports their status. */
    spectre_init();

    print_boot_banner();

    sti();

    /* From here on the serial mirror is queued rather than written a
     * byte at a time: everything above this line went out
     * synchronously, so a kernel that dies during init still says so
     * on the wire. */
    serial_start_async();

    /* The Local APIC's own onboard timer (LVT Timer - separate
     * hardware from the interrupt-routing role lapic_init() already
     * set up inside pic_init() above). Calibrating it means measuring
     * how many of its ticks elapse during a known PIT interval via
     * timer_sleep_ms(), which halts waiting for IRQ0 - exactly the
     * same "must come after sti()" requirement xhci_init() has below,
     * for the same reason. A no-op if the Local APIC was never brought
     * up at all (lapic_available() false - the 8259 fallback path). */
    lapic_timer_calibrate();

    /* USB 3 (xHCI) and the HID class drivers on top of it - what
     * makes a USB keyboard and mouse work. The EHCI driver above
     * stays for the ath9k adapter, which is a high-speed device.
     *
     * This has to come AFTER sti(). Bringing an xHCI controller up
     * means waiting on it - for a reset to finish, for a port to
     * settle - and every one of those waits is timer_sleep_ms(),
     * which halts until IRQ0. Called with interrupts still masked it
     * would halt the machine on the first wait instead of the first
     * millisecond.
     *
     * Class drivers register first: xhci_init() probes each device as
     * it finds it, and a driver registered afterwards is too late for
     * anything that was already plugged in. */
    usbhid_init();
    xhci_init();

    /* Keep the boot splash (toast logos + boot log) on screen for a
     * moment before the shell takes over and clears it. The pause is
     * preempt-disabled so the timer tick cannot switch tasks while we
     * are still inside kernel boot code; pit_tick() still advances
     * the counter, so the sleep terminates normally. */
    preempt_disable();
    timer_sleep_ms(BOOT_SPLASH_HOLD_MS);
    preempt_enable();

    /* Graphics test prompt */
    console_set_color(0x00FFA040, 0x00000000);
    console_write("\n═════════════════════════════════════════════\n");
    console_write("Should a graphics test be performed? (y/N): ");
    console_set_color(0x00E8E8E8, 0x00000000);

    struct kbd_event ev = kbd_get_event();
    int run_graphics_test = 0;

    if (ev.type == KBD_EVENT_CHAR) {
        if (ev.c == 'y' || ev.c == 'Y') {
            run_graphics_test = 1;
        }
        /* This is a single-character prompt, not a line editor: a real
         * answer ("y", "n", even "yes") still arrives with a trailing
         * Enter keystroke the code above never reads. Left in the
         * keyboard queue, that Enter becomes the FIRST byte the next
         * reader sees - which used to be /bin/login's username prompt,
         * making its very first read_line() return an empty line and
         * exit before anyone could type anything. Drain up to the
         * newline so the queue is clean for whatever runs next. A bare
         * Enter (the y/N default) needs no draining - it already WAS
         * this character. */
        if (ev.c != '\n' && ev.c != '\r') {
            for (;;) {
                struct kbd_event more = kbd_get_event();
                if (more.type == KBD_EVENT_CHAR &&
                    (more.c == '\n' || more.c == '\r')) {
                    break;
                }
            }
        }
    }

    if (run_graphics_test) {
        console_set_color(0x00E8E8E8, 0x00000000);
        console_write("Yes\n\n");
        console_write("Running comprehensive graphics test...\n\n");

        if (g_bootinfo.framebuffer != NULL) {
            uint32_t *fb = (uint32_t *)g_bootinfo.framebuffer->address;
            uint64_t pitch_words = g_bootinfo.framebuffer->pitch / 4;
            uint32_t w = g_bootinfo.framebuffer->width;
            uint32_t h = g_bootinfo.framebuffer->height;

            console_set_color(0x00E8E8E8, 0x00000000);
            kprintf("Framebuffer: %u x %u @ %u bpp, pitch %llu\n",
                    w, h, g_bootinfo.framebuffer->bpp,
                    g_bootinfo.framebuffer->pitch);

            /* Test 1: Color gradient (red -> green -> blue) */
            console_write("\n[Test 1/4] Color gradient... ");
            for (uint32_t y = 0; y < h; y++) {
                for (uint32_t x = 0; x < w; x++) {
                    uint32_t r = (x * 255) / w;
                    uint32_t g = (y * 255) / h;
                    uint32_t b = ((x + y) * 255) / (w + h);
                    uint32_t color = 0xFF000000 | (r << 16) | (g << 8) | b;
                    fb[y * pitch_words + x] = color;
                }
            }
            console_write("Done\n");
            timer_sleep_ms(1500);

            /* Test 2: Checkerboard pattern */
            console_write("[Test 2/4] Checkerboard... ");
            uint32_t checker_size = 32;
            for (uint32_t y = 0; y < h; y++) {
                for (uint32_t x = 0; x < w; x++) {
                    uint32_t checker = ((x / checker_size) + (y / checker_size)) % 2;
                    uint32_t color = checker ? 0xFFFFFFFF : 0xFF000000;
                    fb[y * pitch_words + x] = color;
                }
            }
            console_write("Done\n");
            timer_sleep_ms(1500);

            /* Test 3: Primary colors (RGBCMYW) */
            console_write("[Test 3/4] Color bars... ");
            uint32_t colors[] = {
                0xFFFF0000, 0xFF00FF00, 0xFF0000FF,  /* RGB */
                0xFFFFFF00, 0xFF00FFFF, 0xFFFF00FF   /* CYM */
            };
            uint32_t bar_width = w / 6;
            for (uint32_t y = 0; y < h; y++) {
                for (uint32_t x = 0; x < w; x++) {
                    uint32_t bar = x / bar_width;
                    if (bar >= 6) bar = 5;
                    fb[y * pitch_words + x] = colors[bar];
                }
            }
            console_write("Done\n");
            timer_sleep_ms(1500);

            /* Test 4: Grayscale ramp */
            console_write("[Test 4/4] Grayscale... ");
            for (uint32_t y = 0; y < h; y++) {
                for (uint32_t x = 0; x < w; x++) {
                    uint32_t gray = (x * 255) / w;
                    uint32_t color = 0xFF000000 | (gray << 16) | (gray << 8) | gray;
                    fb[y * pitch_words + x] = color;
                }
            }
            console_write("Done\n");
            timer_sleep_ms(1500);

            console_write("\n✓ Graphics test completed successfully!\n");
            console_set_color(0x00FFA040, 0x00000000);
            console_write("═════════════════════════════════════════════\n\n");
            console_set_color(0x00E8E8E8, 0x00000000);
        } else {
            console_write("✗ Framebuffer not available\n");
        }
    } else {
        console_set_color(0x00E8E8E8, 0x00000000);
        console_write("No\n\n");
    }

    console_login_gate();
    tsh_run();

    /* tsh_run() never returns; this is just a safety net. */
    for (;;) {
        hlt();
    }
}
