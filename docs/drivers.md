# Writing a driver for TUS

This document is the practical guide for adding a new device driver to the
kernel. It uses the actual folder layout, the actual build system, and real
patterns pulled from the drivers that already exist, rather than a generic
"how operating systems work" explanation. If you want the theory, read the
source of an existing driver alongside this. If you want to know exactly
what files to create and what to wire up, this document is that.

## The folder layout

Every driver lives in its own directory under `kernel/drivers/`. Not a flat
pile of `.c` and `.h` files anymore, one folder per driver:

```
kernel/drivers/
  ahci/       ahci.c   ahci.h
  ata/        ata.c    ata.h
  ath9k/      ath9k.c  ath9k.h  ath9k_test.c
  e1000/      e1000.c  e1000.h
  ...
  xhci/       xhci.c   xhci.h
```

A driver that needs more than one source file just adds more files to its
own folder (`ath9k` is the example: it has a driver and a self test file
living side by side). There is no rule that a driver folder can only hold
one `.c` file.

Headers are referenced from anywhere else in the kernel with the full path
from `kernel/`, because `kernel/` itself is on the compiler's include
search path (`-Ikernel` in the project Makefile):

```c
#include "drivers/pci/pci.h"
#include "drivers/ata/ata.h"
```

This is true even for a driver including another driver's header, or a
driver's own `.c` including its own `.h`. There is no special casing for
"same folder" includes, everything is spelled out from `kernel/` so a file
can be moved without its includes silently breaking or silently still
working by accident.

## Creating a new driver, step by step

Say you are adding a driver for some device called `widget`.

1. Create the folder: `kernel/drivers/widget/`.
2. Write `kernel/drivers/widget/widget.h` with your public interface
   (init function, any structs other subsystems need to see) and
   `kernel/drivers/widget/widget.c` with the implementation.
3. Create `kernel/drivers/widget/Makefile`. This is not a real,
   independently invokable Makefile, it is a fragment that declares this
   driver's own source list and gets pulled in by
   `kernel/drivers/Makefile` via `include`. Copy the shape of any existing
   one, for example `kernel/drivers/rtc/Makefile`:

   ```makefile
   # Makefile fragment for kernel/drivers/widget/.
   # Included by kernel/drivers/Makefile - not meant to be run directly,
   # it only declares this driver's own source files. The actual compile
   # rule and the final link both live in the top-level Makefile.

   SRCS_DRIVERS_WIDGET := \
     kernel/drivers/widget/widget.c
   ```

   If your driver has more than one `.c` file, list all of them,
   backslash continued, one per line.

4. Add one line to `kernel/drivers/Makefile`:

   ```makefile
   include kernel/drivers/widget/Makefile
   ```

   and add `$(SRCS_DRIVERS_WIDGET)` to the `SRCS_DRIVERS` list right below
   it, in the same backslash continued style every other driver uses.

That is the entire build system integration. The project root Makefile
never needs to change, because it does not enumerate source files itself
anymore, it includes `kernel/Makefile`, which includes
`kernel/drivers/Makefile`, which now includes your new fragment. See
`build-system.md` for the full picture of how this include chain is wired
and why it replaced a single `find kernel -name '*.c'` wildcard.

5. Call your driver's init function from `kernel/main.c`, in the right
   place in the boot sequence (see `boot.md` for what "right place" means:
   ask what your driver depends on, and what depends on it).

## PCI drivers specifically

If your device sits on the PCI bus, the pattern is registration, not
manual probing. `kernel/drivers/pci/pci.c` walks every function on the bus
once, at boot, and for each one checks it against every driver that
registered itself with `pci_register_driver()`. A driver entry looks
roughly like this:

```c
static pci_driver_t widget_driver = {
    .name = "widget",
    .vendor_id = 0x1234,
    .device_id = 0x5678,
    .class_code = 0xFF,       /* 0xFFFF wildcards vendor/device */
    .subclass_code = 0xFF,
    .prog_if = 0xFF,
    .init = widget_pci_init,
};

void widget_register(void) {
    pci_register_driver(&widget_driver);
}
```

Call `widget_register()` before `pci_enumerate_devices()` runs in
`main.c`, so your driver is in the table by the time the bus gets walked.
The enumerator logs every device it finds (bus, device, function, vendor
and device ID, class code, BAR0, IRQ line and pin) to the serial log via
`klog()`, and separately logs a "matched driver" line when your `init`
function gets called. If your device isn't showing up, that serial log is
the first place to look, not a breakpoint.

## Interrupt driven versus polled

Look at how an existing driver in the same rough category handles this
before inventing a new pattern. Keyboard, mouse, serial, and the network
cards are interrupt driven: they register a handler with the IDT/PIC layer
and do their work inside the handler, keeping it short. xHCI is
deliberately polled instead, on a fixed interval, because with a real
transfer ring and event ring behind it a slow poll only costs latency, not
correctness, and it avoids the complexity of driving USB enumeration from
inside an interrupt context. Pick whichever fits your device rather than
defaulting to interrupts out of habit.

If your driver enables a hardware interrupt, do it through
`pic_enable_irq()` / `pic_send_eoi()`, never by touching the 8259 or the
APIC registers directly. Those two functions are the seam that lets a
driver work identically whether the machine ended up on the plain PIC or
the upgraded I/O APIC path (see `boot.md` stage 4), and every existing
driver goes through them for exactly that reason.

## DMA

If your driver needs physically contiguous memory it can hand to hardware,
use `kernel/mm/dma.c`, which hands out page aligned frames straight from
the physical memory allocator along with their real physical address. Do
not improvise a physical address by casting a higher half kernel pointer;
that trick happens to work for identity adjacent mappings but breaks the
moment a page isn't laid out the way you assumed, and it has bitten this
codebase before (the EHCI driver's `virt_to_phys` is a documented example
of exactly this mistake, kept around as a cautionary example rather than
fixed, because EHCI's only real device, the ath9k adapter, never actually
moves a byte through that path).

## Testing your driver

If your driver is amenable to it, look at how `tests/fb/` or
`tests/keymap/` compile the actual driver `.c` file straight into a host
side test binary (via `#include "../../kernel/drivers/fb/fb.c"` and a tiny
standalone Makefile) rather than linking against a build of the whole
kernel. This works well for drivers whose logic can be exercised without
real hardware behind it: framebuffer text rendering, keymap tables, the
highX compositor's pixel math. If your driver genuinely needs hardware or
QEMU's emulation of it, a `make test-*` target that boots the ISO and
drives it through QMP, the way `make test-usb` proves the USB HID path, is
the better fit.

Either way, write something that would actually fail if you broke the
driver, not a test that just confirms it compiles.
