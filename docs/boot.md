# The boot sequence

This document walks through everything that happens between the firmware
handing control over and tsh printing its first prompt. The point is not
just to list function calls in order, it is to explain why they happen in
that order, because almost every ordering decision in `kernel/main.c` is
load bearing. Move something a few lines up or down and the machine either
hangs, panics, or silently loses a feature.

## Stage 0: the firmware and the bootloader

TUS does not have its own bootloader. It uses Limine, which is a separate
project entirely, and the handoff between them is defined by the Limine
boot protocol rather than anything TUS controls. `limine.conf` at the repo
root tells Limine which kernel ELF to load, which module to load alongside
it (`rootfs.img`), and what kind of framebuffer to ask the firmware for.

By the time Limine jumps into `_start()` in `kernel/main.c`, the following
is already true and TUS can rely on it without checking:

- The CPU is in 64 bit long mode.
- The kernel is mapped at its linked higher half address,
  `0xffffffff80000000`, with paging already active.
- A stack exists and a flat GDT is loaded (temporary, TUS replaces it with
  its own almost immediately).
- If a framebuffer was available, it is already mapped and its address,
  width, height, pitch and bits per pixel are ready to be read.

None of this is negotiated at runtime through some capability query. TUS
declares what it wants via static request structures placed in a special
`.requests` linker section (see the `__attribute__((used, section(".requests")))`
structs near the top of `main.c`), Limine fills in the response pointers
before jumping to `_start()`, and the kernel just reads them.

## Stage 1: collecting boot info

The very first thing `_start()` does, before touching any hardware, is call
`fill_bootinfo()`. This walks every one of those Limine response structs
(framebuffer, memory map, bootloader name and version, ACPI RSDP pointer,
CPU count from the MP feature, the rootfs module, the kernel's own ELF
file) and copies what it needs into one global struct, `g_bootinfo`. From
this point on nothing else in the kernel touches the raw Limine structures
directly. Everything downstream reads `g_bootinfo`. This matters because it
means the boot protocol details are contained to one function, and if TUS
ever needs to support a different bootloader, this is the only place that
has to change.

`park_aps()` runs right after. Limine starts every CPU core, not just the
boot processor, and each application processor sits there spinning waiting
for someone to publish a `goto_address` it can jump to. TUS is currently
single core, so every AP just gets handed a `cli; hlt` loop forever. This
has to happen early, before interrupts are enabled anywhere, because an AP
that is still executing Limine's own trampoline code when TUS starts
touching shared state is a source of very hard to debug races.

## Stage 2: the console comes up

`console_init()` brings up the serial port and, if a framebuffer exists,
the text console that draws characters into it. This is deliberately one
of the very first things to happen because everything after this point can
now report what it is doing. If something goes wrong three steps later,
there is already a way to see it.

Two things are worth knowing about the console from the start. First,
serial output is TUS's real debug channel, and it always gets written to
regardless of whether a framebuffer console exists. Second, as of the
recent boot log rework, only a short human readable summary goes to the
screen (bootloader name, "detecting hardware", and eventually "tsh
ready"). Every technical detail, including the full memory map dump and
the per device PCI enumeration log with BAR and IRQ information, goes
through `klog()` straight to serial only. If you are debugging boot and
the framebuffer isn't showing enough, that is on purpose: check the serial
log, not the screen.

## Stage 3: interrupt and memory infrastructure

`gdt_init()` replaces Limine's temporary GDT with TUS's own. `idt_init()`
installs the interrupt descriptor table and every exception handler TUS
knows about (page fault, general protection fault, divide by zero, invalid
opcode, and so on). Nothing has unmasked interrupts yet, this is just
getting the table ready so that when interrupts do get turned on later, the
CPU has somewhere sane to jump.

Then the memory system comes up in three steps that have to happen in this
exact order:

1. `pmm_init()` builds the physical frame allocator out of the Limine
   memory map, marking every USABLE region as available and everything
   else (reserved, ACPI, bootloader reclaimable, and so on) as off limits.
2. `vmm_init()` sets up the virtual memory manager on top of the page
   tables Limine already built, giving TUS the ability to map and unmap
   pages on demand rather than only using whatever Limine set up.
3. `kmalloc_init()` builds the kernel heap on top of that, at a fixed
   virtual address with a 64 MiB cap, using a classic split and coalesce
   free list allocator.

Nothing before this point could allocate memory dynamically. Nothing after
it has to worry about that constraint again.

## Stage 4: interrupt controller and timers

`pic_init()` brings up the legacy 8259 PIC unconditionally, every single
boot, with no exceptions. Only after that does TUS attempt to move IRQ
routing onto the Local APIC and I/O APIC, and that attempt is allowed to
fail silently at any step: no APIC support in CPUID, no MADT in the ACPI
tables, no I/O APIC entry, a failed MMIO mapping, any of it just leaves
`g_apic_active` false and every driver keeps calling the exact same
`pic_enable_irq()` / `pic_send_eoi()` functions they always did, because
those functions branch internally on `g_apic_active`. This fail closed
design is deliberate: a machine that would have worked fine on the plain
PIC should never fail to boot just because the fancier IRQ routing didn't
pan out.

`pit_init()` brings up the Programmable Interval Timer at 100 Hz, which is
what drives the scheduler's preemption tick later. Keyboard and mouse
(`kbd_init()`, `mouse_init()`) come up right after, still with interrupts
globally masked, so their interrupt handlers are registered but nothing
will fire until `sti()` much later.

## Stage 5: storage and the filesystem tree

`ata_init()` probes for IDE/PATA disks. `vfs_init()` builds the virtual
filesystem tree and mounts `rootfs.img`, the ustar tar archive Limine
loaded as a module, as a read only tree. This is the point where `/bin`,
`/etc`, `/dev` (empty for now) and everything else in the shipped root
filesystem becomes visible to the kernel.

`wrf_boot_mount()` looks for a WRF formatted disk to mount at `/mnt`. WRF
is TUS's own simple on disk filesystem, separate from the tar based
rootfs, meant for persistent user data on an actual disk rather than the
read only image that ships in the ISO.

## Stage 6: the scheduler and device nodes

`sched_init()` brings the round robin preemptive scheduler online, though
nothing is running on it yet besides the boot thread itself. Immediately
after, `vfs_devices_init()` creates the device nodes under `/dev`:
framebuffer, console tty, keyboard, serial, null, zero, random. This has
to happen after `sched_init()` because device file descriptors live in a
task's fd table, and before the scheduler exists there is no task struct
to hold one.

AHCI and NVMe storage drivers (`ahci_init()`, `nvme_init()`) come up here
too, adding any SATA or NVMe disks they find to the same device namespace
the ATA driver already populated.

## Stage 7: configuration files and services

With a real filesystem and real device nodes available, TUS can now read
its own configuration. `load_keymap()` reads `/etc/keymap` and switches the
keyboard layout away from the built in US default if something else is
configured. `load_hostname()` reads `/etc/hostname`. `load_tussm()` starts
tusSM, the service manager, as a real ring 3 process, which is what
actually starts most of userspace from here on rather than the kernel
spawning things directly.

`term_init()` sets up the terminal session infrastructure that `hxtsh`
windows and SSH sessions both build on. `random_init()` seeds the kernel's
RNG from timing jitter (there is no hardware RNG dependency assumed).
`rtc_init()` reads the real time clock so `date` and file timestamps have
something real to report.

## Stage 8: networking

`net_init()` brings up the RTL8139 or E1000 driver, whichever is present,
and the IP stack on top of it. `load_resolv_conf()` reads `/etc/resolv.conf`
for a DNS resolver address. `load_boot_services()` starts anything else
configured to run at boot through tusSM.

## Stage 9: the rest of the hardware

This is the point where the more exotic drivers come up: `hda_register()`
for the HD Audio codec and `/dev/dsp`, `ehci_init()` for the EHCI USB 2.0
controller (used specifically for the ath9k wifi adapter's high speed
transfers), and then `pci_enumerate_devices()`, which walks every PCI
function on the bus, logs vendor and device ID, class code, BAR0 and IRQ
line to the serial log, and matches each one against the table of drivers
that registered themselves as PCI capable.

`vbe_init()` checks for Bochs VBE display mode setting support, which is
what `res_set` uses later to change resolution on QEMU, Bochs, or
VirtualBox. On real hardware with no VBE registers this just quietly does
nothing and the machine keeps whatever mode the firmware negotiated.

## Stage 10: SSE, Spectre mitigations, and the banner

`cpu_enable_sse()` clears CR0.EM and sets the OSFXSR and OSXMMEXCPT bits in
CR4, which is what lets user space programs use SSE instructions. The
kernel itself never touches the FPU or SSE state directly, the scheduler
saves and restores it per task on every context switch.

`spectre_init()` runs right before the boot banner, which matters because
the banner reports the result. It sets the Spectre v1 mitigation flag
unconditionally (LFENCE is baseline x86-64, always available) and checks
CPUID leaf 7 for IBRS/IBPB support to decide the Spectre v2 status. See
`security.md` for the actual reasoning behind these mitigations and why v2
uses IBRS rather than compiler level retpolines.

`print_boot_banner()` prints the short summary to the screen and the full
technical detail to serial, as described in stage 2 above.

## Stage 11: interrupts go live

`sti()` is the single most consequential line in the whole boot sequence.
Every interrupt handler that was registered but dormant since stage 3 and
stage 4 can now actually fire: the PIT tick, the keyboard, the mouse, PCI
device interrupts, all of it. Right after, `serial_start_async()` switches
the serial mirror from synchronous byte by byte writes (used during boot
so that a kernel that dies mid init still gets its last words out over the
wire) to an interrupt driven queue drained by COM1's transmit interrupt.

`lapic_timer_calibrate()` runs after `sti()` specifically because
calibrating the Local APIC's onboard timer means timing against something
else, and that wait has to actually be able to receive an interrupt to
complete. This calibration is a masked one shot, it cannot accidentally
fire a stray interrupt into a handler that isn't expecting it.

USB HID and xHCI (`usbhid_init()`, `xhci_init()`) come up after `sti()` for
the same reason ath9k and EHCI don't: bringing up a USB controller means
waiting on it, and TUS's only way to wait is `timer_sleep_ms()`, which
halts the CPU until the next timer interrupt arrives. Call that with
interrupts still masked and the machine just halts forever.

## Stage 12: login and the shell

`console_login_gate()` is the last thing standing between boot completing
and someone actually being able to type a command. On the current kernel
this is a real login prompt backed by `/etc/shadow` and `crypt()`, not a
free pass. Once that gate is satisfied, `tsh_run()` starts the interactive
kernel shell and the boot sequence, as such, is over. Everything after
this point is just normal running system behavior: user tasks spawning,
the scheduler doing its job, network packets arriving, and so on.

## Why the ordering matters, in one paragraph

If you take nothing else away from this document, take this: TUS's boot
order is a dependency chain, not a checklist. Memory management has to
exist before anything allocates. The scheduler has to exist before device
nodes that need a task's fd table. The filesystem has to exist before
configuration files can be read. Interrupts have to stay masked until
every handler that could fire is actually ready for it, and specifically
have to be unmasked before anything that waits on a timer interrupt tries
to wait. When you add a new `_init()` call to `main.c`, ask what it needs
that came before it, and what would break if it ran even one line earlier.
