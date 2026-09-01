# Installing TUS on a disk

TUS normally boots from `tus.iso`, running entirely out of RAM: the
kernel and the root filesystem (`rootfs.img`, a read-only tar image)
are Limine modules loaded fresh on every boot. `tusinstall` is what
turns that into something a machine boots on its own, with no CD
attached - and it handles the whole disk, not just the OS: partitioning,
formatting, copying the system across, **and** setting up a real
persistent filesystem, all in one run, with no separate steps or tools
needed afterward.

Specifically, `tusinstall` lays down **two partitions**:

- An **EFI System Partition** (FAT32) holding the bootloader, this
  kernel and this root filesystem - copied straight out of the running
  system's own memory (`/dev/kernel`, `/dev/rootfs`), so the disk ends
  up with exactly what booted it. This is what the machine actually
  boots from; it is only ever read again after this install, never
  written to.
- A **WRF partition** - TUS's own on-disk filesystem, mounted
  read-write at `/mnt` on every boot from here on
  (`kernel/fs/wrf.c`). Unlike `/`, which is still `rootfs.img` read
  fresh into RAM every boot (so a file created at the shell, or a
  package installed with `tpm`, is normally gone on reboot exactly
  like it is running from the CD), anything created under `/mnt`
  genuinely persists - it is read live off the disk on every access.

## 1. Build the ISO

From the project root:

```bash
make clang iso    # or: make gcc iso
```

(`make clang` is the reliable path on this Raspberry Pi - see
CLAUDE.md's "Compilation Notes on This Pi" if plain `make`/`make gcc`
fail with cross-compilation errors.) This produces `tus.iso` in the
project root.

## 2. Create a target disk

Any raw disk image works. 512 MiB is comfortably enough for the
current image with room left over for `/mnt` too - `tusinstall` sizes
the EFI partition itself (generous headroom over the kernel and
rootfs it's about to copy, floored at 64 MiB) and gives everything
else on the disk to WRF, only falling back to one plain, unpartitioned
disk (no `/mnt`) if there truly isn't room to spare:

```bash
qemu-img create -f raw tus-disk.img 512M
```

To install onto **real hardware** instead of a QEMU disk image, skip
this step and boot the ISO on the machine with the target drive
attached (a USB stick with the ISO written to it via `dd`, or a
physical optical drive) - `tusinstall` talks to whatever `/dev/hdX`
the ATA driver finds, image file or real disk alike, the same way.

## 3. Boot the ISO with the disk attached, and install

```bash
qemu-system-x86_64 -cdrom tus.iso -m 512M \
    -drive file=tus-disk.img,format=raw,if=ide,index=0,media=disk
```

At the console:

```
login: root
Password: toast
tus:/> tusinstall
```

`tusinstall` is interactive and walks through everything itself:

1. Lists the disks it found and asks which one to install onto
   (defaults to the first). **Everything on that disk is erased** -
   it asks for confirmation before writing anything.
2. Offers to set a new root password (the image ships with root's
   password baked in as `toast` - CLAUDE.md's own testing instructions
   use it - so changing it here is worth doing for anything other than
   a throwaway VM).
3. Optionally creates a regular user account.
4. Optionally enables the SSH daemon at startup.
5. Partitions the disk (the EFI System Partition, and - space
   permitting - a WRF partition right after it; see
   CLAUDE.md's "Installing to a disk (tusinstall)" for exactly how each
   is laid out), formats both, copies the bootloader, `/dev/kernel` and
   `/dev/rootfs` onto the first, and **verifies the copy by reading
   every byte back**.
6. Ends with `Exit to (S)hell, (H)alt or (R)eboot? [reboot]` - just
   press Enter, or type `s`/`h`/`r`.

## 4. Boot from the disk

The installed disk boots via **UEFI only** (Limine's EFI executable -
there is no legacy BIOS boot path on the installed disk the way there
is on the hybrid `tus.iso`), so QEMU needs OVMF firmware:

```bash
cp /usr/share/OVMF/OVMF_VARS_4M.fd /tmp/tus-ovmf-vars.fd
qemu-system-x86_64 -m 512M \
    -drive if=pflash,format=raw,unit=0,file=/usr/share/OVMF/OVMF_CODE_4M.fd,readonly=on \
    -drive if=pflash,format=raw,unit=1,file=/tmp/tus-ovmf-vars.fd \
    -drive file=tus-disk.img,format=raw,if=ide,index=0,media=disk
```

(Copy `OVMF_VARS` rather than passing it directly - QEMU writes to it,
and you generally don't want to keep re-mutating the shared system
copy.) No `-cdrom` this time: nothing should be attached but the disk
that was just installed. On real hardware, this just means: turn the
machine on with the drive attached and UEFI boot enabled in the
firmware - no OVMF needed there, that's only QEMU's stand-in for real
UEFI firmware.

If `/usr/share/OVMF/OVMF_CODE_4M.fd` doesn't exist on this machine,
install `ovmf` (or your distro's equivalent package) first; the
project's own `make test-install` checks for it the same way and
skips the disk-boot half of the test with a message when it's absent.

On the boot log (serial output, or scroll back at the console) you
should see:

```
wrf          : mounted /mnt from disk 0, LBA <N> (... inodes, ... KiB data)
```

confirming `/mnt` is the WRF partition `tusinstall` just wrote, not a
fallback. From here, anything under `/mnt` - `mkdir`, writing a file,
`tpm install`ing a package there - is real, persistent storage that
survives every future reboot.

## A disk too small for both partitions

If a target disk doesn't have room for a sensibly-sized WRF partition
on top of the EFI System Partition, `tusinstall` just gives the whole
disk to the ESP and skips WRF - exactly what it always did before WRF
existed. The install still succeeds and the system still boots; there
is simply no `/mnt` (the boot log has no `wrf : mounted ...` line, and
writes everywhere on the system stay non-persistent, same as running
from the CD). A 512 MiB disk, the size used above, is comfortably
enough for both.

## A separate, dedicated WRF disk (the old way)

`tusinstall`'s automatic partition is normally all you need, but WRF
itself doesn't require it - `mkfs.wrf <device>` still works standalone
on any whole disk with no partition table at all (`kernel/fs/wrf.c`
checks for a WRF partition in the MBR first, then falls back to a bare
WRF superblock at the very start of the disk). This is still the
right tool if you want a *second*, independent volume - a data disk
you can move between installs, say - rather than the one `tusinstall`
carves out of the boot disk automatically:

```bash
qemu-img create -f raw tus-data.img 64M
```

Attach it as an extra IDE drive (e.g. `-drive
file=tus-data.img,format=raw,if=ide,index=1,media=disk`), then at the
TUS shell:

```
tus:/> mkfs.wrf /dev/hdb
```

(Check `ls /dev` or the `disk :` line the kernel prints at boot if
you're not sure which name is which disk.) Reboot, and the kernel
mounts it at `/mnt` exactly the same way - though if the boot disk
*also* has its own `tusinstall`-written WRF partition, that one is
found first and mounted instead; only one WRF volume is mounted at a
time in v1.
