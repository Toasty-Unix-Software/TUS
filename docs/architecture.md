# System architecture

This is the big picture document: how TUS's major subsystems fit together,
what each one owns, and how they talk to each other. Read `boot.md` first
if you want the chronological version of how these pieces come online.
This document is the structural version instead, organized by subsystem
rather than by boot order.

## The higher half kernel layout

TUS is mapped at `0xffffffff80000000`, the top of the 64-bit address
space, the classic "higher half" layout. User space lives in the lower
canonical half, `0x0000000000000000` through
`0x00007fffffffffff`. Every task has its own private mapping for the user
half of its address space, but the kernel half is shared across every task
by construction, which is what lets TUS get away with one single kernel
heap rather than needing per-address-space kernel allocations. The linker
script that sets this all up is `kernel/linker.ld`.

## Memory management

Three layers, built in a strict dependency order during boot (see
`boot.md` stage 3):

The physical memory manager, `kernel/mm/pmm.c`, is a bitmap frame
allocator built directly from the Limine memory map. Only frames marked
USABLE by the bootloader ever get handed out; everything else (reserved
regions, ACPI tables, the kernel image itself, bootloader reclaimable
regions) stays off the table permanently.

The virtual memory manager, `kernel/mm/vmm.c`, extends whatever page
tables Limine already built, on demand, rather than replacing them wholesale.
`map_page()`, `map_region()` and `unmap_page()` are the primitives
everything else in the kernel builds on when it needs to map something
into virtual memory: MMIO regions for a device driver, a new task's
address space, the framebuffer for a resolution change.

`kmalloc`, in `kernel/mm/kmalloc.c`, is a free-list heap living at a fixed
virtual address with a 64 MiB ceiling, splitting and coalescing blocks on
allocation and free the way any classic C heap allocator does. It does not
zero memory on allocation, which is a detail that has caused at least one
real bug in this codebase: `vfs_pipe()` used to leave garbage in a
freshly-allocated pipe's `inet_sock`/`socket_domain` fields, and that
garbage occasionally looked enough like a valid pointer that a pipe read
got misrouted into the socket read path, causing a panic. If you allocate
something with `kmalloc` and don't immediately fill in every field, assume
whatever was there before you is still there.

## The scheduler

`kernel/sched/sched.c` implements round-robin preemptive multitasking,
driven by the 100 Hz PIT interrupt (or the HPET/Local APIC timer path
where those are available, see `boot.md` for how that upgrade path works
the same way the I/O APIC one does: fail closed, fall back to PIT if
anything about the fancier timer isn't there).

Every task gets its own private user-space address mapping and shares the
kernel half, as described above. FPU and SSE state is saved and restored
per task on every context switch, which is what lets user space use SSE
freely even though the kernel itself is built with `-mgeneral-regs-only`
and never touches the FPU registers on its own behalf.

TUS is currently single core in practice: Limine starts every application
processor, but they're immediately parked in a `cli; hlt` loop during boot
(see `boot.md` stage 1) and the scheduler only ever runs tasks on the boot
processor. Multi-core scheduling is a real gap, not a design choice that's
been finished and hidden.

`ps`, `kill`, and `pkill` all work through `task_kill()`, which does for a
task other than the caller everything `task_exit()` does for a task
exiting itself: release the console keyboard if it's held, tear down any
highX windows, close every terminal session, close every open file
descriptor, then mark the task a zombie so the scheduler stops considering
it. This is provably safe specifically because TUS is single core: the
target task can never be mid-instruction while `task_kill()` runs against
it, it can only be sitting at whatever `hlt()` or preemption point its
last context switch left it at.

## The virtual filesystem

The VFS, in `kernel/vfs/`, is a tree of `struct vfs_node`, each one typed
as a regular file, a directory, a device node, or an AF_UNIX socket, with
operation pointers (`read`, `write`, `ioctl`) that get filled in
differently depending on the type.

The root filesystem itself is not built by the kernel walking a real disk
partition. It's a ustar tar archive, `rootfs.img`, that Limine loads as a
boot module, and `kernel/vfs/rootfs.c` parses that tar structure directly
into VFS tree nodes at boot, mounted read only. Anything you drop into the
`rootfs/` staging directory in the repo shows up in the booted system
automatically the next time the image is rebuilt, with zero kernel code
changes required. Directories that need to exist but can't be represented
in a tar archive as empty (`/dev`, `/tmp`, parts of `/etc`) get created
explicitly by the kernel at boot instead.

Device nodes live under `/dev` and are created by `vfs_devices_init()`
during boot: the framebuffer, the console tty, the keyboard, the serial
port, `/dev/null`, `/dev/zero`, `/dev/random`, and one node per detected
disk. A disk device node is deliberately treated as nothing more than a
byte stream: any offset, any length, `lseek(SEEK_END)` works, `ls -l`
reports its real size, and the disk installer is built entirely out of
plain `open`/`lseek`/`read`/`write` calls as a result, with no special
disk-aware API needed anywhere in userspace.

`/proc` is a small set of synthetic files, not a full Linux-style procfs
with per-process directories. `kernel/vfs/procfs.c` generates
`/proc/uptime`, `/proc/meminfo`, `/proc/loadavg`, `/proc/stat`,
`/proc/mounts`, and `/proc/cpuinfo` on read, each one built from real
kernel state at the moment it's read, with one honest exception:
`/proc/loadavg`'s three averages always read `0.00`, because TUS's
scheduler keeps no decayed running average to report, and that field is
an admitted "not tracked" rather than a fabricated number.

## The syscall boundary

One vector, `int $0x80`. Arguments arrive in `rdi`, `rsi`, `rdx`, `r10`,
`r8`, `r9`, matching the System V AMD64 calling convention rather than the
Linux x86-64 syscall convention (which uses a slightly different register
set and vector). `kernel/syscall/syscall.c` is the dispatch table, and
every syscall that touches a user pointer runs it through `access_ok()`
first; see `security.md` for exactly what that checks and why. The
ring-0 kernel shell calls the same dispatch table directly, without going
through the `int $0x80` gate at all, since it's already running at the
kernel's own privilege level.

## Networking

The current stack is Ethernet plus IPv4 plus ICMP, built on either an
RTL8139 or an E1000 driver depending on what QEMU (or real hardware) is
presenting. There is no TCP or UDP implementation yet in the general
sense; the socket layer that does exist, `AF_UNIX`, is fully implemented
for local inter-process communication: `bind`, `listen`, `accept`,
`connect`, `socketpair`, `poll`, and `select` all work. Each socket owns a
4 KiB ring buffer for its receive direction, writes copy directly into the
peer's buffer, and a reader blocking on an empty buffer just spins on
`hlt()` until the scheduler wakes it back up when the peer writes.

## highX, the window system

highX is TUS's own display server, and unlike X11 or Wayland it lives
inside the kernel itself (`kernel/highx/`), not as a separate userspace
process. The entire protocol is one syscall, `SYS_HIGHX`, which takes an
opcode plus a pointer to a matching request structure defined in
`include/highx.h`, a header the kernel and every client both include
directly so the protocol can't drift out of sync between server and
client builds.

Every window owns its own ARGB backing store allocated in the kernel heap;
clients never touch the framebuffer directly, they only ever composite
into their own buffer and ask the server to blit it. Damage tracking means
`HX_OP_COMMIT` only repaints the rectangle that actually changed, clipped
against whatever's on top of it in the window stack, which is what keeps
a full-screen application redrawing on every keystroke from turning into a
full-screen repaint on every keystroke.

Because the compositor is kernel code built with `-mgeneral-regs-only`,
its pixel loops (`kernel/highx/compositor.c`) are written to avoid
per-pixel branching rather than reaching for SIMD intrinsics: borders are
four rectangles, not four thousand bounds checks, and the software
rasterizer for text walks the set bits of a glyph row directly rather than
testing each pixel individually.

Two window managers exist on top of the protocol, tusWM (a manual, keybound
tiling and floating hybrid) and tusDE (a mouse-driven desktop), both built
purely against the client-side highX API with no special kernel
privileges beyond what any other highX client has.

## The shells

TUS actually ships two different shells, and it's worth being clear about
which is which. tsh is the kernel's own shell, and it runs as a ring 0
task at the console; its built-in commands print through `kprintf()`
directly rather than through a syscall, which is exactly why the terminal
session redirection layer (`kernel/term/term.c`) has to intercept
`kprintf()` output specifically to make a terminal window showing tsh
work correctly rather than only being visible on the console. hxterm is a
separate, ring 3, userspace shell with its own line editor, history, and
job control, built for the case where you want a real process running a
real shell rather than a view into the kernel's own one; it's still the
only way to get a shell in a highX window if you'd rather not use the
kernel's, though the default terminal window in a highX session is now
`hxtsh`, which is a thin client showing a real tsh instance rather than
its own separate shell implementation.

## Where the pieces actually connect

If you're trying to trace how a keystroke becomes a character on screen,
here's the short version: the PS/2 or USB HID driver decodes a scancode
into a `kbd_event` with both an ASCII byte and a real Unicode codepoint,
`kernel/drivers/keymap/keymap.c` is what turns "the key right of Caps
Lock" into an actual letter based on the loaded layout, and from there the
event either goes to the console's tty layer (which UTF-8 encodes the
codepoint into a byte queue a program reads one byte at a time), or to
highX's event queue if a highX session owns the keyboard, which delivers
the codepoint directly as `hx_event.key` rather than re-deriving it from
raw bytes. One decode, two different delivery paths depending on who's
listening.
