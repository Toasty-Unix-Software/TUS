# The build system

TUS is built with GNU Make, and the tree is organized as one Makefile per
directory rather than a single flat Makefile that globs the whole source
tree with `find`. This document explains how that's wired together, why it
changed from the old flat layout, and what to do when you add a new file
or a whole new subsystem.

## Why not just `find kernel -name '*.c'`

The kernel source used to be enumerated with a single line at the top of
the project Makefile:

```makefile
KERNEL_SRCS := $(shell find kernel -name '*.c')
```

This worked, and it is genuinely the simplest possible thing that could
work. Its downside is that it makes the source tree structurally invisible
to the build system: nothing about the Makefile tells you what's in
`kernel/drivers/` or which files belong to the scheduler versus the VFS,
you'd have to go read the directory yourself. Once `kernel/drivers/` grew
into 23 separate driver folders (one per device, see `drivers.md`), the
wildcard still worked mechanically, but the project no longer had any
Makefile-level structure that mirrored the actual code structure. Splitting
the build into one Makefile per directory fixes that: the build system
now documents the tree the same way the folders themselves do, and a new
driver's Makefile fragment is the same three lines regardless of how deep
in the tree it lives.

## The include chain

Nothing below the top-level project Makefile actually compiles or links
anything. Every fragment Makefile below it does exactly one thing: declare
a variable listing that directory's own source files, nothing more. Here's
the actual shape, top to bottom:

```
Makefile                          (project root: compiles, links, builds
                                    the ISO, everything else - unchanged)
  includes kernel/Makefile
    includes kernel/core/Makefile         -> SRCS_CORE
    includes kernel/arch/Makefile
      includes kernel/arch/x86_64/Makefile -> SRCS_ARCH_X86_64
    includes kernel/boot/Makefile         -> SRCS_BOOT
    includes kernel/drivers/Makefile
      includes kernel/drivers/ahci/Makefile    -> SRCS_DRIVERS_AHCI
      includes kernel/drivers/ata/Makefile     -> SRCS_DRIVERS_ATA
      ... one include per driver folder ...
      includes kernel/drivers/xhci/Makefile    -> SRCS_DRIVERS_XHCI
      (sums all of the above into SRCS_DRIVERS)
    includes kernel/vfs/Makefile          -> SRCS_VFS
    includes kernel/mm/Makefile           -> SRCS_MM
    includes kernel/elf/Makefile          -> SRCS_ELF
    includes kernel/sched/Makefile        -> SRCS_SCHED
    includes kernel/term/Makefile         -> SRCS_TERM
    includes kernel/highx/Makefile        -> SRCS_HIGHX
    includes kernel/fs/Makefile           -> SRCS_FS
    includes kernel/syscall/Makefile      -> SRCS_SYSCALL
    includes kernel/shell/Makefile        -> SRCS_SHELL
    includes kernel/net/Makefile          -> SRCS_NET
    (sums everything above, plus kernel/main.c itself, into KERNEL_SRCS)
```

The project root Makefile picks up `KERNEL_SRCS` from that chain exactly
where it used to pick up the result of the `find` wildcard, and everything
downstream of that (`KERNEL_OBJS`, the pattern rule that compiles a `.c`
into a `.o`, the final `ld` invocation that links `kernel.elf`) is
completely unchanged. This split only touches how the source list gets
built, not how it gets compiled or linked.

## Adding a file to an existing driver or subsystem

Add it to that directory's own Makefile. For example, adding a second
source file to the `rtc` driver:

```makefile
# kernel/drivers/rtc/Makefile
SRCS_DRIVERS_RTC := \
  kernel/drivers/rtc/rtc.c \
  kernel/drivers/rtc/rtc_alarm.c
```

Nothing else needs to change. `kernel/drivers/Makefile` already includes
this fragment and already sums `$(SRCS_DRIVERS_RTC)` into `SRCS_DRIVERS`.

## Adding a whole new driver

See `drivers.md` for the full walkthrough with a worked example. In short:
new folder under `kernel/drivers/`, a `Makefile` fragment inside it
declaring `SRCS_DRIVERS_<NAME>`, one `include` line and one addition to
the sum in `kernel/drivers/Makefile`, done.

## Adding a whole new kernel subsystem directory

Say you're adding `kernel/audio/` as a new top-level subsystem, separate
from the drivers tree. Create `kernel/audio/Makefile` following the same
pattern as any existing subsystem fragment (`kernel/mm/Makefile` is a good
short example to copy), defining `SRCS_AUDIO`, then in `kernel/Makefile`
add:

```makefile
include kernel/audio/Makefile
```

and add `$(SRCS_AUDIO)` to the `KERNEL_SRCS` sum at the bottom of that same
file. Two lines, in the one file that owns the top-level subsystem list.

## Verifying the source list is still correct

Because this is just a chain of `include` statements building up plain
Make variables, you can check the result matches reality without compiling
anything, using a dry run:

```
make -n kernel.elf
```

This prints every command Make would run, including the full `cc ...` line
for each source file and the final `ld` line listing every object file it
would link, without actually invoking the compiler or the linker. If a new
driver's source file is missing from that output, the Makefile fragment
chain has a break in it somewhere between that driver and
`kernel/Makefile`, and `-n` will show you exactly what got compiled
instead of what you expected.

## What did not change

The rest of the project Makefile: musl, userspace tools, the vendored
third-party builds (PCC, NASM, elftoolchain, mbedtls, lvgl, fastfetch,
ksh93), the rootfs image assembly, and the ISO build, are all still one
flat set of rules in the root Makefile, the same as before this reorganization.
Only the kernel source enumeration moved into the per-directory fragment
chain described above. If a similarly large restructuring of the userspace
or vendored-source build rules is ever worth doing, it should follow the
same pattern: one Makefile per directory, each one declaring only its own
sources, summed upward through `include`, with the actual compile and link
commands staying exactly where they are now.
