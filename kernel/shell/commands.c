/*
 * commands.c - core tsh commands and dispatch
 *
 * Each command is a plain function `int fn(int argc, char **argv)`
 * with argv[0] being the command name itself, exactly like a UNIX
 * shell. The core commands live here; the file-system commands live
 * in cmd_fs.c (g_fs_commands) and are reached through the same table.
 */

#include "commands.h"

#include "tsh.h"
#include "arch/x86_64/cpu.h"
#include "arch/x86_64/io.h"
#include "arch/x86_64/ioapic.h"
#include "arch/x86_64/lapic.h"
#include "arch/x86_64/pic.h"
#include "arch/x86_64/smp.h"
#include "sched/cap.h"
#include "core/bootinfo.h"
#include "core/console.h"
#include "core/klib.h"
#include "drivers/fb/fb.h"
#include "drivers/ec/ec.h"
#include "drivers/pci/pci.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/pit/pit.h"
#include "drivers/hpet/hpet.h"
#include "drivers/pmtimer/pmtimer.h"
#include "drivers/ath9k/ath9k.h"
#include "drivers/ehci/ehci.h"
#include "drivers/xhci/xhci.h"
#include "drivers/usbhid/usbhid.h"
#include "elf/tus_elf.h"
#include "highx/highx.h"
#include "mm/pmm.h"
#include "mm/swap.h"
#include "mm/vmm.h"
#include "net/wpa_crypto.h"
#include "sched/sched.h"
#include "term/term.h"
#include "vfs/vfs.h"

#define MAX_ARGS 16

static int cmd_help(int argc, char **argv);
static int cmd_clear(int argc, char **argv);
static int cmd_ver(int argc, char **argv);
static int cmd_about(int argc, char **argv);
static int cmd_sysinfo(int argc, char **argv);
static int cmd_cpuinfo(int argc, char **argv);
static int cmd_caps(int argc, char **argv);
static int cmd_swaptest(int argc, char **argv);
static int cmd_reboot(int argc, char **argv);
static int cmd_shutdown(int argc, char **argv);
static int cmd_crash(int argc, char **argv);
static int cmd_ath9k_test(int argc, char **argv);
static int cmd_wpa_selftest(int argc, char **argv);
static int cmd_usb(int argc, char **argv);
static int cmd_apic(int argc, char **argv);
static int cmd_msi(int argc, char **argv);
static int cmd_timers(int argc, char **argv);
static int cmd_fan(int argc, char **argv);
static int cmd_highx(int argc, char **argv);
static int cmd_history(int argc, char **argv);
static int cmd_exit(int argc, char **argv);

static const struct shell_command g_core_commands[] = {
    { "help",       "list available commands",           cmd_help },
    { "clear",      "clear the screen",                  cmd_clear },
    { "ver",        "show the kernel version",           cmd_ver },
    { "about",      "show TUS information",              cmd_about },
    { "sysinfo",    "show system information",           cmd_sysinfo },
    { "cpuinfo",    "show detected CPUs (ACPI/MADT)",    cmd_cpuinfo },
    { "caps",       "show current task's capability bits",    cmd_caps },
    { "swaptest",   "exercise the disk-backed swap path end to end",  cmd_swaptest },
    { "reboot",     "restart the machine",               cmd_reboot },
    { "shutdown",   "halt the machine",                  cmd_shutdown },
    { "halt",       "halt the machine",                  cmd_shutdown },
    { "poweroff",   "halt the machine",                  cmd_shutdown },
    { "crash",      "raise a CPU exception (demo)",      cmd_crash },
    { "ath9k_test", "run ath9k-htc driver unit tests",   cmd_ath9k_test },
    { "wpaselftest", "check the WPA2-PSK crypto core against known test vectors", cmd_wpa_selftest },
    { "usb",        "show USB device information",       cmd_usb },
    { "apic",       "show Local APIC / I/O APIC status",  cmd_apic },
    { "msi",        "list/enable PCI Message Signaled Interrupts", cmd_msi },
    { "timers",     "show which hardware timers are active",      cmd_timers },
    { "fan",        "read/write ACPI Embedded Controller registers", cmd_fan },
    { "highx",      "start a highX session (--de desktop + login, --wm tusWM)", cmd_highx },
    { "history",    "list the command history",           cmd_history },
    { "exit",       "close this terminal window",        cmd_exit },
};

#define CORE_COMMAND_COUNT (sizeof(g_core_commands) / sizeof(g_core_commands[0]))

/* `exit` means something only inside a terminal window: the console
 * shell is the machine's last resort and has nowhere to exit to. */
static int cmd_exit(int argc, char **argv) {
    (void)argc;
    (void)argv;
    struct tsh_term *t = term_current();
    if (t == NULL) {
        console_write("exit: this is the console shell - "
                      "there is nothing to exit to.\n");
        return 1;
    }
    t->closing = true;
    return 0;
}

static int cmd_help(int argc, char **argv) {
    (void)argc;
    (void)argv;
    console_write("Built-in commands:\n");
    for (size_t i = 0; i < CORE_COMMAND_COUNT; i++) {
        kprintf("  %-10s %s\n", g_core_commands[i].name,
                g_core_commands[i].description);
    }
    for (size_t i = 0; i < g_fs_command_count; i++) {
        kprintf("  %-10s %s\n", g_fs_commands[i].name,
                g_fs_commands[i].description);
    }
    console_write("\nNetwork tools (in /bin):\n");
    console_write("  ping       send ICMP echo requests to a host\n");
    console_write("  ifconfig   show network interface information\n");
    console_write("  netstat    show network statistics (-i for interfaces, -s for stats)\n");
    console_write("  arp        show/manage ARP table (-a to list all)\n");
    console_write("  route      show/manipulate routing table (-n for numeric)\n");
    console_write("  hostname   show or set system hostname\n");
    console_write("\nUser programs (in /bin):\n");
    console_write("  kilo       text editor\n");
    console_write("  doas       privilege elevation (run as root)\n");
    console_write("  login      user login\n");
    console_write("  useradd    create new user accounts\n");
    console_write("  passwd     change user password\n");
    console_write("  grep       search text patterns\n");
    console_write("  sed        stream editor\n");
    console_write("  echo       print text\n");
    console_write("\nExamples:\n");
    console_write("  ping 192.168.1.1        # Test connectivity\n");
    console_write("  ping -c 4 8.8.8.8       # Send 4 packets\n");
    console_write("  ifconfig                # Show network config\n");
    console_write("  exec /bin/kilo file.txt # Edit file\n");
    console_write("  help                    # Show this message\n");
    return 0;
}

/* history - the lines the shell has run, oldest first. The Up and
 * Down keys walk the same list (see tsh.c). */
static int cmd_history(int argc, char **argv) {
    (void)argc;
    (void)argv;
    for (int i = 0; i < tsh_history_count(); i++) {
        kprintf("%4d  %s\n", i + 1, tsh_history_get(i));
    }
    return 0;
}

static int cmd_clear(int argc, char **argv) {
    (void)argc;
    (void)argv;
    console_clear();
    return 0;
}

static int cmd_ver(int argc, char **argv) {
    (void)argc;
    (void)argv;
    kprintf("TUS kernel 1.0.0-unstable, built with %s\n", __VERSION__);
    return 0;
}

static int cmd_about(int argc, char **argv) {
    (void)argc;
    (void)argv;
    console_write("Toasty Unix Software (TUS)\n");
    console_write("\"Work everywhere, but work right.\"\n");
    console_write("Architecture: x86_64 (AMD64)\n");
    kprintf("Bootloader  : %s %s\n",
            g_bootinfo.bootloader_name ? g_bootinfo.bootloader_name : "unknown",
            g_bootinfo.bootloader_version ? g_bootinfo.bootloader_version : "");
    return 0;
}

static int cmd_sysinfo(int argc, char **argv) {
    (void)argc;
    (void)argv;

    char vendor[13];
    char brand[49];
    cpu_get_vendor(vendor);
    cpu_get_brand(brand);
    kprintf("CPU vendor  : %s\n", vendor);
    kprintf("CPU model   : %s\n", brand);

    kprintf("Memory      : %llu MiB usable\n",
            (unsigned long long)(g_bootinfo.usable_memory_bytes / (1024 * 1024)));

    uint64_t total_frames = 0, free_frames = 0;
    pmm_get_stats(&total_frames, &free_frames);
    kprintf("PMM         : %llu free / %llu frames (%llu MiB)\n",
            (unsigned long long)free_frames, (unsigned long long)total_frames,
            (unsigned long long)(free_frames * 4 / 1024));

    kprintf("Uptime      : %llu.%03llu s\n",
            (unsigned long long)(pit_uptime_ms() / 1000),
            (unsigned long long)(pit_uptime_ms() % 1000));

    uint32_t width = 0, height = 0, bpp = 0;
    uint64_t pitch = 0;
    void *address = NULL;
    fb_get_info(&width, &height, &bpp, &pitch, &address);
    if (width > 0 && height > 0) {
        kprintf("Framebuffer : %ux%u, %u bpp, pitch %llu @ %p\n",
                width, height, bpp, (unsigned long long)pitch, address);
    } else {
        console_write("Framebuffer : none (serial console only)\n");
    }
    return 0;
}

static int cmd_cpuinfo(int argc, char **argv) {
    (void)argc;
    (void)argv;

    int n = smp_cpu_count();
    kprintf("CPUs        : %d detected (%d enabled), 1 running (no SMP scheduling)\n",
            n, smp_cpu_enabled_count());
    for (int i = 0; i < n; i++) {
        const struct cpu_state *c = smp_cpu_get(i);
        if (c == NULL) {
            break;
        }
        kprintf("  cpu%-2d apic_id=%-3u acpi_id=%-3u %s%s\n", i, c->apic_id,
                c->acpi_processor_id, c->enabled ? "enabled" : "disabled",
                c->is_bsp ? " (BSP, running)" : "");
    }
    return 0;
}

static int cmd_caps(int argc, char **argv) {
    (void)argc;
    (void)argv;

    struct task *cur = sched_current();
    if (cur == NULL) {
        console_write("caps: no current task\n");
        return 1;
    }

    kprintf("pid=%u uid=%u euid=%u caps=0x%x%s\n", cur->pid, cur->uid,
            cur->euid, cur->caps, cur->euid == 0 ? " (root: all implied)" : "");
    kprintf("  CAP_NET_ADMIN : %s\n", has_cap(cur, CAP_NET_ADMIN) ? "yes" : "no");
    kprintf("  CAP_NET_RAW   : %s\n", has_cap(cur, CAP_NET_RAW) ? "yes" : "no");
    kprintf("  CAP_SETUID    : %s\n", has_cap(cur, CAP_SETUID) ? "yes" : "no");
    return 0;
}

/* Exercises kernel/mm/swap.c end to end against a real disk: map a
 * scratch page, write a pattern, evict it (a real disk write + the
 * frame is actually freed), confirm the PTE is gone, then just READ
 * the address - the resulting #PF is handled transparently by
 * swap_fault() in idt.c, which reads the page back from disk into a
 * fresh frame and restores the mapping. If the pattern still matches
 * afterwards, the whole eviction/fault-in round trip is real, not
 * simulated. */
static int cmd_swaptest(int argc, char **argv) {
    (void)argc;
    (void)argv;

    if (!swap_available()) {
        console_write("swaptest: no swap disk (run `mkswap /dev/hdX` on a "
                       "second disk and reboot)\n");
        return 1;
    }

    uint64_t phys = pmm_alloc_frame();
    if (phys == 0) {
        console_write("swaptest: out of memory\n");
        return 1;
    }
    uint64_t cr3 = vmm_current_cr3();
    if (vmm_map_page_in(cr3, VMM_SWAPTEST_VA, phys,
                        VMM_PRESENT | VMM_WRITE) != 0) {
        pmm_free_frame(phys);
        console_write("swaptest: could not map the scratch page\n");
        return 1;
    }

    volatile uint32_t *page = (volatile uint32_t *)VMM_SWAPTEST_VA;
    for (int i = 0; i < 1024; i++) {
        page[i] = 0xdeadbe00u + (uint32_t)i;
    }

    uint32_t before_total, before_used;
    swap_get_stats(&before_total, &before_used);

    uint32_t slot = swap_out_page(cr3, VMM_SWAPTEST_VA);
    if (slot == 0) {
        vmm_unmap_page(VMM_SWAPTEST_VA);
        pmm_free_frame(phys);
        console_write("swaptest: swap_out_page failed\n");
        return 1;
    }
    bool now_absent = vmm_translate(VMM_SWAPTEST_VA) == 0;

    /* Just touching the address faults - swap_fault() in idt.c does
     * the rest and this read proceeds normally once it has. */
    uint32_t readback_first = page[0];
    bool ok = now_absent && readback_first == 0xdeadbe00u;
    for (int i = 0; ok && i < 1024; i++) {
        if (page[i] != 0xdeadbe00u + (uint32_t)i) {
            ok = false;
        }
    }

    uint32_t after_total, after_used;
    swap_get_stats(&after_total, &after_used);

    kprintf("swaptest: slot %u used during eviction (swap %u/%u -> %u/%u "
            "slots busy)\n", slot, before_used, before_total, after_used,
            after_total);
    kprintf("swaptest: PTE not-present immediately after eviction: %s\n",
            now_absent ? "yes" : "no");
    kprintf("swaptest: page fault transparently restored the pattern: %s\n",
            ok ? "yes" : "no");
    console_write(ok ? "swaptest: PASS\n" : "swaptest: FAIL\n");

    /* The frame backing this address now is a fresh one swap_fault()
     * allocated on the way back in - not the `phys` from above, which
     * swap_out_page() already returned to the allocator. */
    uint64_t final_phys = vmm_translate(VMM_SWAPTEST_VA);
    vmm_unmap_page(VMM_SWAPTEST_VA);
    if (final_phys != 0) {
        pmm_free_frame(final_phys);
    }
    return ok ? 0 : 1;
}

static int cmd_reboot(int argc, char **argv) {
    (void)argc;
    (void)argv;
    console_write("Rebooting...\n");
    /* Ask the 8042 keyboard controller to pulse the reset line. */
    outb(0x64, 0xFE);
    /* If the reset never fires, stop here instead of continuing. */
    for (;;) {
        cli();
        hlt();
    }
    return 0;
}

static int cmd_shutdown(int argc, char **argv) {
    (void)argc;
    (void)argv;
    console_write("The system is halted.\n");
    /* Same shape as SYS_POWER's TUS_POWER_HALT (the console shell is
     * ring 0 and reaches this directly rather than through the
     * syscall ABI, same as cmd_reboot above). */
    for (;;) {
        cli();
        hlt();
    }
    return 0;
}

static int cmd_crash(int argc, char **argv) {
    (void)argc;
    (void)argv;
    console_write("Raising invalid opcode (#UD)...\n");
    __asm__ volatile("ud2"); /* never reached */
    return 0;
}

static int cmd_ath9k_test(int argc, char **argv) {
    (void)argc;
    (void)argv;
    kprintf("Running ath9k-htc driver unit tests...\n");
    ath9k_run_tests();
    return 0;
}

static int cmd_wpa_selftest(int argc, char **argv) {
    (void)argc;
    (void)argv;
    return wpa_selftest() == 0 ? 0 : 1;
}

static int cmd_apic(int argc, char **argv) {
    (void)argc;
    (void)argv;
    if (!pic_using_apic()) {
        console_write("routing:    8259 PIC (Local APIC/I/O APIC not "
                      "available or not found)\n");
        return 0;
    }
    kprintf("routing:    Local APIC + I/O APIC\n");
    kprintf("lapic id:   %u\n", lapic_id());
    kprintf("lapic:      mapped, software-enabled, spurious vector 0xFF\n");
    kprintf("ioapic:     mapped\n");
    console_write("irq0 (timer), irq1 (keyboard), irq4 (serial), irq12 "
                 "(mouse) and any PCI device IRQ are\n"
                 "delivered through I/O APIC redirection entries "
                 "rather than the 8259 cascade.\n");
    return 0;
}

/* Reports every hardware timer source this kernel knows about, and
 * which one pit_uptime_ms() (SYS_UPTIME, and everything built on it)
 * is actually reading from right now. The PIT itself is not "detected"
 * - it is programmed unconditionally at boot (pit_init(), see
 * pit.c's own header comment: it stays the backward-compatible
 * baseline every one of the other three is additive on top of. */
static int cmd_timers(int argc, char **argv) {
    (void)argc;
    (void)argv;
    kprintf("PIT:        100 Hz (IRQ0), always on - the scheduler tick "
           "and the backward-compatible\n"
           "            baseline every timer below is additive on top "
           "of\n");

    if (lapic_timer_available()) {
        kprintf("LAPIC timer: calibrated, %llu Hz (divide-by-1)\n",
               (unsigned long long)lapic_timer_hz());
    } else {
        console_write("LAPIC timer: not available (no Local APIC, or "
                      "calibration measured 0)\n");
    }

    if (pmtimer_available()) {
        uint32_t a = pmtimer_read();
        uint32_t b = pmtimer_read();
        kprintf("ACPI PM timer: %u Hz fixed, port present - two back-to-back "
               "reads: %u, %u\n",
               ACPI_PM_TIMER_HZ, a, b);
    } else {
        console_write("ACPI PM timer: not available (no FADT, or "
                      "PM_TMR_BLK is 0)\n");
    }

    if (hpet_available()) {
        kprintf("HPET:       mapped, %u fs/tick, counter=%llu, uptime=%llu ms "
               "(SOURCE for pit_uptime_ms())\n",
               hpet_period_fs(), (unsigned long long)hpet_read_counter(),
               (unsigned long long)hpet_uptime_ms());
    } else {
        console_write("HPET:       not available (no HPET table, or the "
                      "MMIO mapping failed) - "
                      "pit_uptime_ms() is reading the PIT tick count\n");
    }
    return 0;
}

/* No args: lists which PCI functions on bus 0 (see pci_enumerate_devices's
 * own scan bounds) have an MSI capability at all. Four args (bus dev
 * func vector): programs it - real capability-list writes, read back
 * afterwards to prove they landed - but does not touch anything else
 * in the kernel, since no TUS driver today has an ISR waiting on the
 * vector this would redirect a device's interrupts to (see the long
 * comment on pci_msi_enable() in drivers/pci.h). */
static int cmd_msi(int argc, char **argv) {
    if (argc == 1) {
        console_write("BUS DEV FUNC VENDOR:DEVICE  MSI  MSI-X\n");
        for (int dev = 0; dev < 32; dev++) {
            for (int fn = 0; fn < 8; fn++) {
                uint32_t vd = pci_config_read(0, dev, fn, PCI_VENDOR_ID);
                if ((vd & 0xFFFF) == 0xFFFF) {
                    continue;
                }
                int msi = pci_find_capability(0, dev, fn, PCI_CAP_ID_MSI);
                int msix = pci_find_capability(0, dev, fn, PCI_CAP_ID_MSIX);
                kprintf("0   %-3d %-4d %04x:%04x        %-4s %s\n", dev, fn,
                       vd & 0xFFFF, vd >> 16,
                       msi >= 0 ? "yes" : "no",
                       msix >= 0 ? "yes (not programmable by this kernel yet)" : "no");
            }
        }
        return 0;
    }

    if (argc != 5) {
        console_write("usage: msi                       "
                     "(list MSI capability on every PCI function)\n"
                     "       msi <bus> <dev> <func> <vector>  "
                     "(program and enable it)\n");
        return 1;
    }

    uint8_t bus = (uint8_t)strtoul(argv[1], NULL, 0);
    uint8_t dev = (uint8_t)strtoul(argv[2], NULL, 0);
    uint8_t fn = (uint8_t)strtoul(argv[3], NULL, 0);
    uint8_t vector = (uint8_t)strtoul(argv[4], NULL, 0);

    if (pci_msi_enable(bus, dev, fn, lapic_id(), vector) != 0) {
        kprintf("msi: %02x:%02x.%x has no MSI capability\n", bus, dev, fn);
        return 1;
    }

    int cap = pci_find_capability(bus, dev, fn, PCI_CAP_ID_MSI);
    uint32_t header = pci_config_read(bus, dev, fn, (uint8_t)cap);
    uint32_t addr = pci_config_read(bus, dev, fn, (uint8_t)cap + 4);
    kprintf("msi: enabled on %02x:%02x.%x - control %04x, address %08x, "
           "vector %u -> lapic %u\n",
           bus, dev, fn, header >> 16, addr, vector, lapic_id());
    return 0;
}

/* Fan/PWM control has no equivalent of VBE or xHCI to program against
 * directly: real hardware wires it behind the ACPI Embedded
 * Controller's byte registers (see drivers/ec.h), and WHICH register
 * means what is machine-specific, defined in a DSDT's AML that TUS
 * has no interpreter for. This is real, honest infrastructure up to
 * that point - raw EC register access, same as Linux's `ectool` or
 * `/sys/kernel/debug/ec/*` - and no further: there is no universal
 * "set fan speed" this could safely claim to be. QEMU implements no
 * EC at all, so `fan` on a machine booted under QEMU always reports
 * none found - that is `ec_probe()` working correctly, not a driver
 * bug. */
static int cmd_fan(int argc, char **argv) {
    if (argc == 1) {
        if (ec_probe() == 0) {
            console_write("embedded controller: found\n");
        } else {
            console_write("embedded controller: not found (expected under "
                         "QEMU - no EC is emulated; real hardware should "
                         "answer)\n");
        }
        console_write("usage: fan read <reg>\n"
                     "       fan write <reg> <value>\n"
                     "(reg/value: EC byte register offsets - machine "
                     "specific, not auto-detected)\n");
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "read") == 0) {
        uint8_t reg = (uint8_t)strtoul(argv[2], NULL, 0);
        uint8_t value;
        if (ec_read(reg, &value) != 0) {
            console_write("fan: no response from the embedded controller\n");
            return 1;
        }
        kprintf("EC[0x%x] = 0x%x\n", reg, value);
        return 0;
    }

    if (argc == 4 && strcmp(argv[1], "write") == 0) {
        uint8_t reg = (uint8_t)strtoul(argv[2], NULL, 0);
        uint8_t value = (uint8_t)strtoul(argv[3], NULL, 0);
        if (ec_write(reg, value) != 0) {
            console_write("fan: no response from the embedded controller\n");
            return 1;
        }
        kprintf("EC[0x%x] <- 0x%x\n", reg, value);
        return 0;
    }

    console_write("usage: fan read <reg>\n       fan write <reg> <value>\n");
    return 1;
}

static int cmd_usb(int argc, char **argv) {
    (void)argc;
    (void)argv;

    extern ehci_controller_t ehci_ctrl;

    /* Two controllers, two stories. xHCI is where the keyboard and
     * the mouse are (see drivers/xhci.h for why they cannot be on
     * EHCI); EHCI is where the ath9k adapter is. */
    xhci_print_state();
    usbhid_print_state();

    if (ehci_ctrl.num_devices == 0) {
        console_write("EHCI: no devices\n");
        return 0;
    }

    kprintf("USB Devices (%d total):\n", ehci_ctrl.num_devices);
    kprintf("%-4s %-4s %-6s %-6s %-10s %-4s %-15s\n",
        "Addr", "Port", "Vendor", "Product", "Class", "Conf", "Status");
    kprintf("----+----+------+------+----------+----+---------------\n");

    for (int i = 0; i < ehci_ctrl.num_devices; i++) {
        usb_device_t *dev = &ehci_ctrl.devices[i];
        if (dev->descriptor) {
            const char *status = "unconfigured";
            if (dev->configured) {
                status = "configured";
            }

            kprintf("%-4d %-4d %04x   %04x   %02x.%02x      ",
                dev->device_address, dev->port,
                dev->vendor_id, dev->device_id,
                dev->device_class, dev->device_subclass);

            if (dev->config_descriptor) {
                kprintf("%-4d ", dev->config_descriptor->configuration_value);
            } else {
                kprintf("N/A  ");
            }

            kprintf("%s\n", status);

            if (dev->config_descriptor && dev->num_endpoints > 0) {
                kprintf("       Endpoints: %d\n", dev->num_endpoints);
            }
        }
    }

    return 0;
}

/*
 * tsh v2.0: pipes and I/O redirection.
 *
 * Operators (space-separated or glued, `2>` must touch):
 *
 *   |      pipeline   a | b | c   (stdout of a feeds stdin of b)
 *   >      stdout     cmd > file  (create/truncate)
 *   >>     stdout     cmd >> file (append)
 *   <      stdin      cmd < file
 *   2>     2>>        stderr redirection
 *
 * How it works: TUS keeps the fd table per task, and a spawned task
 * inherits a refcounted copy of the shell's table. The shell rewires
 * its OWN slots 0/1/2 (vfs_dup2), spawns the child, puts its own
 * slots back, and waits for the child's pid. External programs are
 * the real pipe users; builtins (echo writes through fd 1, so
 * `echo hi | grep hi` works) run inline with the same slot setup.
 * Commands that print through the console directly (help, ls, ...)
 * cannot be piped yet - only fd-1 writers can.
 */

#define MAX_PIPE_SEGS 8
#define MAX_TOKENS (TSH_LINE_MAX / 2 + 8)

struct pipeline_seg {
    char *argv[MAX_ARGS];
    int argc;
    int redir[3];   /* fd 0/1/2 redirected? */
    int rmode[3];   /* 0 = trunc, 1 = append, 2 = input */
    char *rfile[3];
    int builtin;    /* 0 = external program, else builtin_find() id */
};

/* Split a line into tokens, turning the operators into tokens of
 * their own. `2>x` merges into a single `2>` token (stderr). */
/*
 * Split a line into words and operators.
 *
 * Quotes group what is inside them into one word and are removed, so
 * `echo "one two" > f` passes a single argument, exactly as a UNIX
 * shell does. Tokens are copied into `scratch` rather than carved out
 * of the line in place: `x>y` has no room for a terminator between
 * the word and the operator, and quote removal shortens words.
 *
 * Returns the token count, or -1 if the line has more tokens than fit.
 */
static int tokenize(const char *line, char *scratch, size_t scratch_size,
                    char **tok, int max) {
    size_t out = 0;
    int n = 0;
    const char *p = line;

    while (*p != '\0') {
        while (*p == ' ' || *p == '\t') {
            p++;
        }
        if (*p == '\0') {
            break;
        }
        if (n >= max || out + 4 >= scratch_size) {
            return -1;
        }
        char *start = scratch + out;

        /* Operators are tokens of their own: | < > >> */
        if (*p == '|' || *p == '<' || *p == '>') {
            char op = *p++;
            scratch[out++] = op;
            if (op == '>' && *p == '>') {
                scratch[out++] = '>';
                p++;
            }
            scratch[out++] = '\0';
            tok[n++] = start;
            continue;
        }
        /* Stderr redirection, with or without a space: 2> and 2>> */
        if (p[0] == '2' && p[1] == '>') {
            scratch[out++] = '2';
            scratch[out++] = '>';
            p += 2;
            if (*p == '>') {
                scratch[out++] = '>';
                p++;
            }
            scratch[out++] = '\0';
            tok[n++] = start;
            continue;
        }

        /* A word. Quoted runs keep their spaces and lose the quotes. */
        char quote = 0;
        while (*p != '\0') {
            char c = *p;
            if (quote == 0 && (c == ' ' || c == '\t' || c == '|' ||
                               c == '<' || c == '>')) {
                break;
            }
            if (quote == 0 && (c == '"' || c == '\'')) {
                quote = c;
                p++;
                continue;
            }
            if (quote != 0 && c == quote) {
                quote = 0;
                p++;
                continue;
            }
            if (out + 2 >= scratch_size) {
                return -1;
            }
            scratch[out++] = c;
            p++;
        }
        scratch[out++] = '\0';
        tok[n++] = start;
    }
    return n;
}

/* Turn the token stream into segments. Returns the segment count or
 * -1 on a syntax error (empty command, missing file after an
 * operator, too many stages). */
static int parse_pipeline(char **tok, int ntok, struct pipeline_seg *segs) {
    memset(segs, 0, sizeof(struct pipeline_seg) * MAX_PIPE_SEGS);
    int nseg = 0;
    struct pipeline_seg *cur = &segs[0];
    int t = 0;
    while (t < ntok) {
        const char *s = tok[t];
        if (strcmp(s, "|") == 0) {
            if (cur->argc == 0) {
                return -1;
            }
            if (++nseg >= MAX_PIPE_SEGS) {
                return -1;
            }
            cur = &segs[nseg];
            t++;
            continue;
        }
        int target = -1;
        int mode = -1;
        if (strcmp(s, ">") == 0) {
            target = 1;
            mode = 0;
        } else if (strcmp(s, ">>") == 0) {
            target = 1;
            mode = 1;
        } else if (strcmp(s, "<") == 0) {
            target = 0;
            mode = 2;
        } else if (strcmp(s, "2>") == 0) {
            target = 2;
            mode = 0;
        } else if (strcmp(s, "2>>") == 0) {
            target = 2;
            mode = 1;
        }
        if (target >= 0) {
            if (t + 1 >= ntok) {
                return -1; /* operator at the end: missing file */
            }
            cur->redir[target] = 1;
            cur->rmode[target] = mode;
            cur->rfile[target] = tok[t + 1];
            t += 2;
            continue;
        }
        if (cur->argc >= MAX_ARGS) {
            return -1;
        }
        cur->argv[cur->argc++] = tok[t];
        t++;
    }
    if (cur->argc == 0) {
        return -1; /* trailing | */
    }
    return nseg + 1;
}

/* Builtin lookup: > 0 = core table, < 0 = fs table, 0 = external. */
static int builtin_find(const char *name) {
    for (size_t i = 0; i < CORE_COMMAND_COUNT; i++) {
        if (strcmp(name, g_core_commands[i].name) == 0) {
            return (int)i + 1;
        }
    }
    for (size_t i = 0; i < g_fs_command_count; i++) {
        if (strcmp(name, g_fs_commands[i].name) == 0) {
            return -(int)i - 1;
        }
    }
    return 0;
}

static void builtin_run(int id, int argc, char **argv) {
    if (id > 0) {
        g_core_commands[id - 1].run(argc, argv);
    } else {
        g_fs_commands[-id - 1].run(argc, argv);
    }
}

/* Resolve a command name to a path: absolute paths pass through,
 * bare names are searched in /bin (executability = the x bit). */
static int resolve_prog(const char *name, char *out, size_t out_size) {
    if (strchr(name, '/') != NULL) {
        if (strlen(name) >= out_size) {
            return -1;
        }
        memcpy(out, name, strlen(name) + 1);
        return 0;
    }
    if (strlen(name) + 6 >= out_size) { /* "/bin/" + name */
        return -1;
    }
    memcpy(out, "/bin/", 5);
    memcpy(out + 5, name, strlen(name) + 1);
    struct vfs_node *node = vfs_lookup(out);
    if (node == NULL || node->type != VFS_FILE || (node->mode & 0111) == 0) {
        return -1;
    }
    return 0;
}

/*
 * highx [--wm | --de | program [args...] | info] - run a highX session.
 *
 * The display server lives in the kernel (kernel/highx/); this
 * command starts it, hands the screen over and spawns the session
 * leader. Two sessions ship with TUS: `--wm` is tusWM, the keyboard
 * driven tiling window manager (still the default), and `--de` is
 * tusDE, the mouse driven desktop. Any other name is a program to run
 * as the leader instead. The shell then waits exactly as it does for
 * any foreground program: when the leader exits, the server is
 * stopped and the text console is repainted.
 */
static int cmd_highx(int argc, char **argv) {
    if (argc >= 2 && strcmp(argv[1], "info") == 0) {
        highx_print_state();
        return 0;
    }
    if (highx_active()) {
        kprintf("highx: a session is already running\n");
        return 1;
    }

    char path[128];
    const char *leader = argc >= 2 ? argv[1] : "tuswm";
    /* The two shipped sessions have names of their own, so a user
     * never has to know which binary is behind them. */
    if (strcmp(leader, "--de") == 0 || strcmp(leader, "de") == 0) {
        /* The desktop session starts at its greeter, which is what
         * runs tusDE once someone has logged in - and what comes back
         * when they log out. `highx tusde` still starts the desktop
         * on its own, for a machine that wants no login screen. */
        leader = "hxlogin";
    } else if (strcmp(leader, "--wm") == 0 || strcmp(leader, "wm") == 0) {
        leader = "tuswm";
    }
    if (strchr(leader, '/') != NULL) {
        strncpy(path, leader, sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
    } else {
        strcpy(path, "/bin/");
        strncpy(path + 5, leader, sizeof(path) - 6);
        path[sizeof(path) - 1] = '\0';
    }

    struct vfs_node *node = vfs_lookup(path);
    if (node == NULL || node->type != VFS_FILE || (node->mode & 0111) == 0) {
        kprintf("highx: %s: not executable\n", path);
        return 1;
    }

    int rc = highx_start();
    if (rc < 0) {
        kprintf("highx: cannot take over the screen (errno %d) - a 32bpp "
                "framebuffer is required\n", -rc);
        return 1;
    }

    kprintf("highx: session starting, leader %s\n", path);

    /* Everything after the program name belongs to the session
     * leader: `highx hxvideo /video/clip.mp4` runs the player on that
     * file, exactly as the shell would have. */
    long pid = elf_exec(path, argc > 2 ? argc - 2 : 0,
                        argc > 2 ? &argv[2] : NULL);
    if (pid < 0) {
        highx_stop();
        kprintf("highx: %s: cannot execute\n", path);
        return 1;
    }

    /* The session leader owns the session: when it exits (tusWM quits
     * on Ctrl+Q) the display goes back to the text console. */
    while (sched_task_alive((uint32_t)pid)) {
        hlt();
    }
    highx_stop();

    /* Give the remaining clients a moment to notice that the display
     * is gone (their next request returns -ENODEV) before the shell
     * prints again. */
    for (int i = 0; i < 40; i++) {
        hlt();
    }
    kprintf("highx: session ended\n");
    return 0;
}

/* Run one pipeline. Every external stage is spawned as a task; the
 * shell waits (hlt) until all pids have exited. See the comment at
 * the top of this section for the fd choreography. */
static void exec_pipeline(struct pipeline_seg *segs, int nseg) {
    /* Pre-validate the external programs so a bad name aborts before
     * any task is spawned (and the shell's fds get tangled). */
    char resolved[MAX_PIPE_SEGS][160];
    for (int i = 0; i < nseg; i++) {
        if (segs[i].builtin == 0 &&
            resolve_prog(segs[i].argv[0], resolved[i], sizeof(resolved[i])) != 0) {
            kprintf("tsh: %s: command not found\n", segs[i].argv[0]);
            return;
        }
    }

    /* Save the standard descriptors (the shell's console) so the
     * shell's own slots can be restored after every segment. */
    long saves[3];
    saves[0] = vfs_dup(0);
    saves[1] = vfs_dup(1);
    saves[2] = vfs_dup(2);
    if (saves[0] < 0 || saves[1] < 0 || saves[2] < 0) {
        kprintf("tsh: too many open files\n");
        return;
    }

    int pids[MAX_PIPE_SEGS];
    int npids = 0;
    int prev_r = -1; /* read end of the pipe between the last two stages */
    int failed = 0;

    for (int i = 0; i < nseg; i++) {
        struct pipeline_seg *s = &segs[i];
        int w = -1;
        long filefds[3] = { -1, -1, -1 };

        if (!failed) {
            /* stdin: read end of the previous pipe (if any). */
            if (i > 0) {
                vfs_dup2(prev_r, 0);
                vfs_close(prev_r);
                prev_r = -1;
            }
            /* stdout: fresh pipe to the next stage (if any). */
            if (i < nseg - 1) {
                int pp[2];
                if (vfs_pipe(pp) != 0) {
                    failed = 1;
                } else {
                    vfs_dup2(pp[1], 1);
                    prev_r = pp[0];
                    w = pp[1];
                }
            }
            /* file redirections (> / >> / < / 2> / 2>>). */
            for (int tgt = 0; tgt < 3 && !failed; tgt++) {
                if (!s->redir[tgt]) {
                    continue;
                }
                int flags = s->rmode[tgt] == 2
                                ? O_RDONLY
                                : O_WRONLY | O_CREAT |
                                      (s->rmode[tgt] == 1 ? O_APPEND : O_TRUNC);
                filefds[tgt] = vfs_open(s->rfile[tgt], flags);
                if (filefds[tgt] < 0) {
                    kprintf("tsh: %s: cannot open\n", s->rfile[tgt]);
                    failed = 1;
                } else {
                    vfs_dup2(filefds[tgt], tgt);
                }
            }
            if (!failed) {
                if (s->builtin != 0) {
                    builtin_run(s->builtin, s->argc, s->argv);
                } else {
                    int pid = (int)elf_exec(resolved[i], s->argc - 1,
                                            &s->argv[1]);
                    if (pid < 0) {
                        kprintf("tsh: %s: cannot execute\n", s->argv[0]);
                        failed = 1;
                    } else {
                        pids[npids++] = pid;
                    }
                }
            }
        }

        /* The child (or inline builtin) is done with this setup: put
         * the shell's own descriptors back and drop the temporary
         * copies. The spawned task keeps its inherited table. */
        vfs_dup2(saves[0], 0);
        vfs_dup2(saves[1], 1);
        vfs_dup2(saves[2], 2);
        for (int tgt = 0; tgt < 3; tgt++) {
            if (filefds[tgt] >= 0) {
                vfs_close(filefds[tgt]);
            }
        }
        if (w >= 0) {
            vfs_close(w);
        }
        if (failed) {
            break;
        }
    }
    if (prev_r >= 0) {
        vfs_close(prev_r);
    }

    /* Wait for every stage: the shell idles in hlt, the PIT tick
     * preempts it and the children get CPU time. A pipe reader only
     * sees EOF once its writer has exited and closed its fds.
     *
     * This loop is the only place a foreground job's pids exist, and
     * it never touches the keyboard queue itself (it's a plain
     * sched_task_alive()/hlt() poll) - so Ctrl+C (kernel/drivers/keyboard/
     * keyboard.c) has nothing to find without sched_set_foreground()
     * publishing them first. Cleared once every stage has exited,
     * successfully or by that same Ctrl+C: task_kill() marks a task
     * ZOMBIE, which is exactly what makes sched_task_alive() false. */
    uint32_t fg_pids[MAX_PIPE_SEGS];
    for (int k = 0; k < npids; k++) {
        fg_pids[k] = (uint32_t)pids[k];
    }
    sched_set_foreground(fg_pids, npids);

    /* The shell itself was the console keyboard's owner (it just read
     * this command line), and ownership is claimed on first read and
     * never re-checked - a foreground child that tries to read the
     * keyboard directly (an interactive program with no highX/SYS_TERM
     * session, e.g. ksh's line editor) would find g_kbd_owner still
     * set to the shell's own pid and hlt() forever, since the shell
     * itself never reads again while it is just polling here. Release
     * it before waiting so the child's first read can claim it; if the
     * shell holds no claim (npids==0's caller path, or it already let
     * go) this is a no-op. Ownership comes back to the shell for free
     * on its own next read, since a task_exit() release (or nothing
     * ever claiming it) leaves g_kbd_owner at 0. */
    if (npids > 0) {
        kbd_input_release(sched_current()->pid);
    }

    for (int k = 0; k < npids; k++) {
        while (sched_task_alive((uint32_t)pids[k])) {
            hlt();
        }
    }
    sched_clear_foreground();

    vfs_close(saves[0]);
    vfs_close(saves[1]);
    vfs_close(saves[2]);
}

/*
 * Tokenize the line into argv (space separated) and dispatch. The line
 * is copied first so the caller's buffer stays untouched. Lines with
 * pipe/redirection operators go to the pipeline executor; everything
 * else is a single command (builtin or external program).
 */
void command_execute(const char *line) {
    char buffer[TSH_LINE_MAX];
    char scratch[TSH_LINE_MAX * 2];
    char *tokens[MAX_TOKENS];
    struct pipeline_seg segs[MAX_PIPE_SEGS];

    size_t len = strlen(line);
    if (len >= sizeof(buffer)) {
        len = sizeof(buffer) - 1;
    }
    memcpy(buffer, line, len);
    buffer[len] = '\0';

    int ntok = tokenize(buffer, scratch, sizeof(scratch), tokens, MAX_TOKENS);
    if (ntok < 0) {
        kprintf("tsh: line too long\n");
        return;
    }
    if (ntok == 0) {
        return; /* empty line */
    }

    /* Do any of the tokens look like an operator? */
    int has_ops = 0;
    for (int i = 0; i < ntok; i++) {
        const char *s = tokens[i];
        if (strcmp(s, "|") == 0 || strcmp(s, ">") == 0 ||
            strcmp(s, ">>") == 0 || strcmp(s, "<") == 0 ||
            strcmp(s, "2>") == 0 || strcmp(s, "2>>") == 0) {
            has_ops = 1;
            break;
        }
    }

    if (!has_ops) {
        /* Plain single command: builtin first, then the /bin PATH
         * lookup (a bare name becomes /bin/<name>; executability is
         * decided by the x permission bit, never an extension). */
        for (size_t i = 0; i < CORE_COMMAND_COUNT; i++) {
            if (strcmp(tokens[0], g_core_commands[i].name) == 0) {
                g_core_commands[i].run(ntok, tokens);
                return;
            }
        }
        for (size_t i = 0; i < g_fs_command_count; i++) {
            if (strcmp(tokens[0], g_fs_commands[i].name) == 0) {
                g_fs_commands[i].run(ntok, tokens);
                return;
            }
        }
        char pbin[128];
        if (resolve_prog(tokens[0], pbin, sizeof(pbin)) == 0) {
            /* A spawned task takes over the console keyboard while it
             * runs (ownership is claimed on first read, released in
             * task_exit); the shell waits for the task to exit. */
            memset(&segs[0], 0, sizeof(segs[0]));
            for (int i = 0; i < ntok && i < MAX_ARGS; i++) {
                segs[0].argv[i] = tokens[i];
            }
            segs[0].argc = ntok;
            segs[0].builtin = 0;
            exec_pipeline(segs, 1);
            return;
        }
        kprintf("tsh: %s: command not found\n", tokens[0]);
        return;
    }

    /* Operators present: parse into segments and run the pipeline. */
    int nseg = parse_pipeline(tokens, ntok, segs);
    if (nseg < 0) {
        kprintf("tsh: syntax error\n");
        return;
    }
    for (int i = 0; i < nseg; i++) {
        segs[i].builtin = builtin_find(segs[i].argv[0]);
    }
    exec_pipeline(segs, nseg);
}
