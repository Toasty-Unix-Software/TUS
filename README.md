# TUS - Toasty Unix Software

*"Work everywhere, but work right."*

<img width="713" height="789" alt="tus" src="https://github.com/user-attachments/assets/f03ba14a-8b01-4714-bae9-363c47f29ded" />

TUS is a UNIX-like operating system for the AMD64 (x86-64) architecture,
written from scratch and booted with the [Limine](https://limine-bootloader.org)
bootloader. It has its own kernel, its own ported C library, its own
window system, its own web browser, and its own shell port - no Linux,
no BSD, nothing borrowed except a handful of vendored, clearly-marked
third-party libraries (musl, h264bsd, LVGL, mbed TLS, AST ksh93,
and the toolchain pieces under `sources/`).
Everything else - the VFS, the network stack, the display server, the
browser's HTML/CSS/JS engine, the font rasterizer, the software 3D
pipeline - is original code written for this project.

`ver` reports `TUS kernel 1.0.0-unstable`. Development happens directly
against `main`; 1.0.0 is the version currently in progress and has not
been tagged as a stable release yet.

## What's actually in it

### Boot and core architecture

- Boots via Limine on both BIOS and UEFI, into 64-bit long mode, as a
  higher-half kernel (`0xffffffff80000000`).
- **Boot splash** - one toast per CPU (Limine's MP feature reports the
  count), drawn from `/logo.ppm` (a runtime-decoded PPM, P3 or P6 -
  swap the splash by editing one file, no rebuild of kernel code
  needed) with the boot log scrolling below.
- **Full IDT**, remapped 8259 PIC as the safe baseline, and a real
  register dump (including CR2) on every CPU exception. A fault at
  ring 3 kills only that task (`fault: /bin/x (pid N) killed: ...`);
  the same fault at ring 0, or NMI/double-fault/machine-check at any
  ring, is a genuine kernel panic.
- **Local APIC + I/O APIC**, a strict, fails-closed upgrade over the
  PIC for interrupt routing - every driver keeps calling the same
  `pic_enable_irq()`/`pic_send_eoi()` API, which branches internally on
  whether the upgrade succeeded. ACPI MADT parsing (including
  Interrupt Source Override entries - IRQ0 is not GSI 0 on real
  chipsets) makes this correct on real hardware, not just QEMU's
  default wiring. **PCI MSI** capability-list programming
  (`pci_find_capability()`, `pci_msi_enable()`) is implemented and
  tested against a real MSI-capable device, though not wired into any
  in-tree driver yet.
- **Four hardware timers, deliberately kept together**: the 8254 PIT
  (100 Hz IRQ0, the scheduler's tick and the backward-compatible
  baseline every other timer sits on top of), a calibrated Local APIC
  one-shot timer, the ACPI Power Management Timer (fixed 3.579545 MHz,
  read via the FADT's `PM_TMR_BLK` port), and HPET (its free-running
  counter is what `uptime`/`SYS_UPTIME` actually reads when present,
  for sub-PIT-tick resolution with no calibration needed). `timers`
  shows what's detected and active; PIT still drives preemption
  unconditionally, everything else is additive.
- **ACPI Embedded Controller** access (`kernel/drivers/ec/ec.c`, `fan`
  shell command) - the real mechanism fan/battery/thermal live behind
  on laptops, at the same raw-register level Linux's `acpi_ec` debugfs
  sits at (no AML interpreter, so no "set fan speed" - that's
  machine-specific and undiscoverable without one).

### Memory and scheduling

- Bitmap physical frame allocator over Limine's memory map.
- Demand-extended paging on top of Limine's own page tables; each task
  gets a private user-half address space over a page directory shared
  with the kernel (so a single 1 GiB slot serves every task).
- `kmalloc`: a free-list heap with split/coalesce for small blocks and
  a virtual-address-reusing allocator for large ones (a real leak here
  - VA space abandoned on `kfree()` - was found and fixed after it let
  window-resize churn walk the heap arena into the framebuffer's own
  mapping; there's now a hard backstop that returns `ENOMEM` instead
  of silently corrupting anything if the arena is ever exhausted).
- Round-robin preemptive scheduler, ring-3 tasks with per-task address
  spaces, FPU/SSE state and FS-base (TLS) switched per context switch.
  Real **fork() + execve() + waitpid() + setpgid()**, which is what
  makes ksh's job control and sshd's accept-fork-per-connection model
  work. Real **POSIX signal delivery** (`kill(2)` reaching
  `sched_raise()`/`sched_raise_pgid()`), not just the older
  "every kill is a SIGKILL" model tsh's own built-in `kill` predates.
  `ps`/`kill`/`pkill` exist both as ring-0 tsh built-ins and as real
  ring-3 `/bin` binaries backed by a dedicated `SYS_GETPROCS` syscall -
  the first way a ring-3 program could ever see the task table.
  Currently BSP-only; Limine parks the other CPUs. **ACPI/MADT parses
  the real CPU topology** (`cpuinfo` shows what's detected) and there's
  a correct test-and-set spinlock primitive, but there is no AP
  trampoline yet - every core but the boot CPU stays parked and every
  task still runs on one core. Topology discovery, not multi-core
  execution.
- **NX/W^X**: `EFER.NXE` is enabled and enforced on ELF data/rodata/bss,
  user stacks, and anonymous `mmap()` without `PROT_EXEC` - writing code
  into a non-executable page and jumping to it faults and kills the
  task, it doesn't run.
- A small **POSIX-style capabilities bitmask** (`kernel/sched/cap.h`) -
  `CAP_NET_ADMIN`, `CAP_NET_RAW`, `CAP_SETUID`, `CAP_LINUX_EXEC` - for
  privileges that don't map onto plain "root or not". Grants live in
  `/etc/capabilities` (root-editable, read at login) and persist for a
  whole login session, not just one command; see `docs/security.md`.
- A **Linux x86-64 binary compatibility layer**: TUS can run real,
  unmodified, *statically linked* Linux ELF binaries - a second syscall
  entry path (`SYSCALL`/`SYSRET`, alongside TUS's native `int $0x80`
  ABI) and a small, explicit subset of the Linux syscall table
  translated onto TUS's own kernel functionality. No dynamic linker
  (`PT_INTERP` fails outright), no signals/threading for these
  binaries, and it's gated behind `CAP_LINUX_EXEC` since it's new attack
  surface - root has it implicitly, everyone else needs an explicit
  grant.

### Filesystem and storage

- VFS tree with regular files, directories, device nodes, and
  `AF_UNIX` sockets as real filesystem entries.
- The root filesystem is a **ustar tar image** (`rootfs.img`), parsed
  into the VFS at boot with no hardcoded layout - drop a file in
  `rootfs/` and rebuild, no kernel changes needed.
- `stat`/`fstat`/`statx` all work now (routed through a real
  `SYS_STATX`), `readdir`, `mkdir`, `unlink`, `rename`, `chmod`.
- **A disk is just a byte stream**: `/dev/hda`..`hdd` (a polled 28-bit
  LBA IDE/ATA driver) take any offset and length, so the disk
  installer is plain `open`/`lseek`/`read`/`write` - no special-cased
  disk API.
- **Installing to a disk** (`tusinstall`): writes an MBR with one EFI
  system partition and a hand-written FAT32 filesystem (including VFAT
  long-name entries, since `limine.conf`'s name needs them), copying
  the bootloader, kernel and rootfs straight out of the memory Limine
  already loaded them into - what's installed is, by definition, what
  was running. Prompts for a real root password and an optional user
  account (hashed the same way `passwd` does, baked into a rebuilt
  `/etc/shadow` inside the image), and optionally bakes in an SSH host
  key and an `sshd`-at-boot flag. Verifies what it wrote and ends with
  `Exit to (S)hell, (H)alt or (R)eboot?`.
- **A real, persistent disk filesystem: WRF** (`kernel/fs/wrf.c`),
  mounted read-write at `/home` on every boot when `tusinstall` laid
  down a WRF partition (or `mkfs.wrf` formatted a standalone disk) -
  see `docs/INSTALL.md`. `/` itself is still `rootfs.img`, read fresh
  into RAM every boot, so a file created outside `/home` (or a package
  installed with `tpm` without redirecting it there) is still gone on
  reboot exactly like running from the CD - but anything under `/home`
  genuinely survives, which is what makes real user data, and
  `tpm`-installed packages kept there, actually persistent now.
  WRF is hardened ZFS-style, not just "a filesystem that happens to be
  on disk": every data and index block carries an out-of-band CRC32
  checksum (a `wrfscrub` command walks the volume and reports
  mismatches), and the superblock is checksummed and duplicated
  (primary + a backup copy at the end of the volume, uberblock-style),
  so a dead or corrupted primary is recovered from the backup
  automatically. Copy-on-write is not implemented (documented as a
  known gap in `include/wrf.h`) - a torn write mid-block is not yet
  survivable the way a corrupted superblock is.
- **Disk-backed swap**: real page eviction to disk and page-fault-driven
  swap-in, so usable memory isn't hard-capped at physical RAM.

### Shells and the command line

- **tsh**, the ring-0 console shell: quoting, pipelines, `>`/`>>`/`<`/
  `2>` redirection on built-ins and programs alike, 32-line history,
  and a large built-in command set (`ls`, `cat`, `echo`, `mkdir`,
  `touch`, `rm`, `cd`, `pwd`, `uptime`, `sleep`, `ps`, `kill`, `pkill`,
  `date`, `whoami`, `id`, `head`, `tail`, `wc`, `cp`, `df`, `clear`,
  `help`, `ver`, `about`, `sysinfo`, `reboot`/`shutdown`/`halt`/
  `poweroff`, `apic`, `msi`, `timers`, `fan`, `usb`, `highx`, `exec`,
  and more - `help` lists the live table). Runs at ring 0 so it can
  reach hardware-introspection commands no ring-3 program could.
- **A real login gate** (`console_login_gate()`) now sits in front of
  even the console shell - `login:`/`Password:` against `/etc/shadow`,
  same `crypt()` path as everywhere else.
- **hxterm** and **hxtsh**: two different terminal-in-a-window designs.
  hxterm has its own line-editing shell built in. hxtsh is a *view* of
  the kernel's own tsh - `SYS_TERM` starts a real tsh as its own ring-0
  task and hands the window two ring buffers, so a terminal window, the
  text console, and an SSH session all run literally the same shell.
- **A real PTY layer**: `/dev/ptmx` + `/dev/pts/N` (an 8-slot pool of
  ring-buffer pty pairs), so a program that actually needs a pseudo-
  terminal (job control inside a terminal-in-a-window, `ksh`'s own
  `pty` builtin) gets real POSIX PTY semantics rather than nothing.
- **ksh (AST ksh93, a real port)** - boots and runs as an ordinary
  ring-3 program, with real builtins, job control (`sleep 60 &` reports
  `[1] pid`, though a background job currently doesn't stay visible to
  `ps` afterward - a known ksh-side gap, not a bug in `ps` itself), and
  `whence -a` correctly telling a builtin from a real binary. **Not part
  of the base image any more** - like `pcc`, `nasm`, and `fastfetch`
  (below), it ships as a `tpm` package (`tpm install ksh`) rather than
  baked into `rootfs.img`, the same pattern used for `tree`.
- **Coreutils as real `/bin` binaries**, not just tsh built-ins - the
  thing that makes ksh (or any other real shell) actually usable:
  `ls`, `cat`, `mkdir`, `touch`, `rm`, `mv`, `cp`, `head`, `tail`, `wc`,
  `pwd`, `uptime`, `sleep`, `date`, `whoami`, `id`, `df`, `ps`, `kill`,
  `pkill`, `echo`, `clear`. `pwd`/`sleep`/`kill` are deliberately *both*
  a ksh builtin and a real binary, exactly like real bash/ksh.
- `grep` (a real BRE/ERE engine: `-i -v -n -c -l -w -x -E -F -e -f -m
  -A/-B/-C`) and `sed` (`s///`, addresses/ranges, `y d p q =`, `a i c`,
  hold space, `-n -e -f -i -E`) as real binaries too.
- `doas` (OpenBSD-style privilege elevation via `/etc/doas.conf`) -
  genuinely requires a password now: real per-session identity
  propagation, `setuid`/`setgid` enforcing actual POSIX rules, and
  setuid-on-exec implemented for the first time (see
  `docs/security.md` for what was silently broken before this).
  `useradd` (uid allocation actually increments now - it used to
  hand every new user the same uid), `passwd`, `login`, `hostname`
  (real `gethostname()`/`sethostname()`, `/etc/hostname` read at boot).

### Networking

- Real Ethernet (RTL8139/e1000), ARP, IPv4, ICMP (`ping`).
- **Real TCP** (`kernel/net/tcp.c`) and UDP - connect/listen/accept,
  a genuine 3-way handshake, retransmission, a real receive window;
  DNS resolution over UDP. `AF_UNIX` local sockets (`socket`/`bind`/
  `listen`/`accept`/`connect`/`socketpair`/`poll`/`select`) predate the
  network stack and still work exactly as before.
- **A DHCPv4 client** - real DISCOVER/OFFER/REQUEST/ACK, boot-verified
  against QEMU's own DHCP server. Static IP configuration still works
  too.
- **IPv6**: SLAAC link-local addressing (RFC 4291 EUI-64 from the MAC),
  Neighbor Discovery (NS/NA, RS) and ICMPv6 echo (`ping6`), plus real
  **TCP6 and UDP6 transport** (`AF_INET6`, `SOCK_STREAM`/`SOCK_DGRAM`)
  built on the same TCP state machine and UDP logic as the IPv4 stack,
  with the IPv6 pseudo-header checksum. SLAAC global-address
  autoconfiguration exists in code but is unexercised - QEMU's slirp
  gateway doesn't answer Router Solicitations in testing. No AF_INET6
  path for anything beyond TCP/UDP (no raw sockets, etc.).
- Userspace tools: `ifconfig` (real RX/TX byte counters and MTU, IPv4
  and IPv6 addresses), `netstat`, `arp`, `route`, `nc`/`nc6`, `host`,
  `ping`/`ping6`, `fetch`/`wget` (HTTP/1.1 client).
- **ath9k-htc** USB Wi-Fi driver infrastructure (firmware for the
  AR7010/AR9271 chipset ships in `/lib/firmware/`), plus a **WPA2-PSK
  crypto core** (PBKDF2, PTK derivation, EAPOL MIC - verified against
  published FIPS/RFC test vectors) and an **IEEE 802.11 management
  frame layer** (probe/auth/assoc construction and parsing, RSN IE for
  CCMP/PSK). Honest gap: this has never associated with a real access
  point - `ath9k-htc` has no USB command/data path implemented yet, and
  QEMU emulates no 802.11 hardware or AP to test against, so the crypto
  and frame-format correctness are proven standalone, not the actual
  over-the-air handshake.
- **SSH, both directions, for real**: `userspace/ssh/` implements the
  transport and channel layers from scratch (`sshtrans.c`, `sshchan.c`,
  `sshbuf.c`, `sshkey.c`) and both ends use them - `ssh`/`ssh-keygen`
  as a real client (confirmed working against the actual OpenSSH on
  this machine), and `sshd` as a real server (`accept()` -> `fork()` ->
  handle in the child, password auth against `/etc/shadow`, spawns
  `/bin/ksh`). A real host machine's own OpenSSH client has
  successfully authenticated and run commands against TUS's `sshd`.
  `sshd` starts at boot only if `/etc/sshd.enable` exists, which
  `tusinstall` writes when asked; its host key is generated once at
  install time and baked into the image so it survives reboots (a key
  generated at runtime by `sshd` itself would not - or better, on
  `/home` now that WRF persistence exists). `sshd` falls back to
  `/bin/tsh` if `ksh` isn't installed via `tpm`. `sshd` now allocates a
  real per-session PTY when a client asks for one via pty-req, but a
  live `ssh -t` shell session over it still doesn't produce visible
  output end-to-end yet - see Known Issues below.

### USB

- **xHCI** (USB 3, every speed natively) for keyboards and mice - real
  DMA (physically contiguous frames from the PMM), a real command
  ring/event ring/transfer rings with proper cycle-bit handling, and
  the HID boot protocol (report diffing turned back into PS/2-style
  scancodes, so the rest of the input stack - Caps Lock LED, layouts,
  everything - is shared with the PS/2 driver unmodified).
- **EHCI** (USB 2) kept specifically for the ath9k-htc Wi-Fi adapter,
  which is high-speed - the one case EHCI (rather than xHCI or a
  low/full-speed companion controller) actually handles.

### The display server, window managers, and UI toolkit

- **highX**: a display server living in the kernel (`kernel/highx/`) -
  window table, stacking order, per-client event queues, keyboard
  focus and modifiers, a mouse cursor with wheel support, server-drawn
  window borders, damage-based repainting. One syscall
  (`SYS_HIGHX`) carries the entire protocol, shared verbatim between
  kernel and userspace via `include/highx.h` (see `docs/architecture.md`
  for how it fits together with the rest of the kernel).
- **tusWM** (keyboard-driven tiling) and **tusDE** (mouse-driven, with
  a dock/top-bar) are ordinary highX clients with zero special
  privilege - the same API any program gets. Both now use a shared
  **macOS-style UI toolkit** (`userspace/hglui/`) for anti-aliased
  rounded window chrome, three traffic-light buttons, and TrueType
  title text, built on **LVGL** underneath the actual `tuswm`/`tusde`
  windows (not a separate demo app).
- **hxfiles** (a file manager), **hxmenu** (a rofi-style Super+D
  launcher), **hxlogin** (the session greeter, session-leader so
  logging out returns to it), **hxclock**, **hxdemo**.
- **HighGL** (`userspace/highgl/`): a from-scratch GL 1.x
  fixed-function pipeline - immediate mode, matrix stacks, per-vertex
  lighting, one texture unit, depth test, blending, plus vertex/
  fragment hooks that stand in for shaders. Software, and shaped for
  it (branch-free inner span loop, barycentric interpolation,
  perspective-correct by construction, near-plane-only clipping).
  Verified against a brute-force reference model, not just "looks
  right."
- **TrueType** (`userspace/tusfont/`): reads real `.ttf` files -
  `cmap`, `glyf` (simple and composite), `kern` - and rasterizes with
  no floating point (26.6 fixed-point, matching how TrueType's own
  grid is defined), exact horizontally / 5x-supersampled vertically,
  non-zero winding fill. Used for UI title bars and the `hxfont`
  specimen viewer; not used for the fast, bitmap 8x16 text console.

### Clint - the web browser

`userspace/clint/`: HTTP/1.1 (chunked, redirects, gzip) → HTML parsing
→ CSS cascade → layout → paint, plus PNG images and a real
**tree-walking JavaScript interpreter** (ES5 + `let`/`const`/arrows/
template literals, its own backtracking regex engine, a 12 MB / 4M-step
budget instead of a garbage collector). No `<script src>` fetching, no
floats/absolute positioning, no aligned-grid tables, and nothing that
requires a modern JS engine (Google Search and YouTube both refuse it)
- documented limitations, not silent failures. Video is handed off to
`hxvideo` rather than decoded in-browser.

### Audio and video

- **HD Audio** driver (`kernel/drivers/hda/hda.c`) - `/dev/dsp`, a CLI
  player (`wavplay`) and an LVGL GUI one (`hxwavplayer`), play/pause
  verified against a real captured audio stream.
- **MP4 / H.264 playback** (`hxvideo`): a from-scratch MP4 demuxer
  (box tree, sample tables, `avcC` → Annex B rewriting) feeding the
  vendored `h264bsd` baseline decoder. Pause, seek, a real achieved
  frame-rate readout. Baseline profile only.

### Internationalization

Real keyboard **layouts** (US, Turkish Q, Turkish F, German, French,
Spanish) live in one shared table (`kernel/drivers/keymap/keymap.c`) used by
*both* PS/2 and USB HID, with AltGr as a genuine third shift level
(not Alt), dead-key composition, and Caps-Lock-aware letter mapping
(this is what makes Turkish dotted/dotless I actually correct). The
console and highX both understand UTF-8 codepoints end to end - a
`struct fb_cell` holds a codepoint, not a byte, so a Turkish `ş` is one
column, not a mangled two.

### Display mode setting

`res_set` reprograms the Bochs VBE DISPI registers (real hardware with
no such interface just keeps the firmware's mode); the framebuffer,
`/dev/fb0`, the console, and a running highX session are all correctly
rebound and repainted on a live mode change, windows moved rather than
resized so a client's backing store is never touched behind its back.

## Project layout

```
TUS/
├── kernel/
│   ├── main.c            boot sequence
│   ├── arch/x86_64/      IDT, GDT, CPU, PIC, ACPI, Local/IO APIC, port I/O
│   ├── core/             klib, console, errno, bootinfo
│   ├── boot/             boot splash (toast-per-CPU) + PPM decoding
│   ├── drivers/          serial, PS/2 kbd+mouse, keymap, framebuffer,
│   │                     PIT/HPET/PM-timer, RTL8139, IDE/ATA, xHCI/EHCI/
│   │                     HID, ath9k, PCI, VBE, HD Audio, ACPI EC
│   ├── mm/               PMM, VMM, kmalloc
│   ├── elf/               ELF loader
│   ├── vfs/               VFS tree, fd table, rootfs (tar) mount, /dev
│   ├── net/               ARP/IP/ICMP/TCP/UDP, AF_UNIX sockets
│   ├── highx/             the highX display server (compositor, windows)
│   ├── term/              terminal sessions (SYS_TERM)
│   ├── syscall/           int $0x80 dispatch table
│   ├── sched/             scheduler, task/address-space state
│   └── shell/             tsh + its built-in commands
├── userspace/             system tools and every highX client
│   ├── highapi/           the highX client library
│   ├── hglui/             macOS-style UI toolkit (tuswm/tusde chrome)
│   ├── highgl/            software GL 1.x pipeline
│   ├── tusfont/           TrueType rasterizer
│   ├── clint/             the web browser (HTTP/HTML/CSS/JS/PNG)
│   ├── ssh/               SSH transport/channel/key code (client + sshd)
│   ├── tuscrypt/          crypto primitives backing SSH/TLS
│   ├── ls.c cat.c mkdir.c ... echo.c clear.c   real /bin coreutils
│   ├── tuswm.c tusde.c hxlogin.c hxfiles.c hxmenu.c hxterm.c hxtsh.c
│   ├── hxvideo.c mp4.c hxwavplayer.c wavplay.c
│   └── doas.c useradd.c passwd.c login.c grep.c sed.c tusinstall.c
├── sources/               vendored: musl-1.2.6, h264bsd, ksh (AST 93u+m),
│                          lvgl, mbedtls, kilo, a small toolchain
│                          (pcc/nasm/elftoolchain)
├── rootfs/                root filesystem staging dir -> rootfs.img
├── tests/                 test_boot.py + one test_*.py per subsystem,
│                          plus host-only unit test suites (highx/, mp4/)
├── include/               highx.h, tusterm.h, tusvideo.h, tusinput.h -
│                          protocol headers shared by kernel and userspace
├── Makefile
├── limine.conf
└── docs/                  full documentation: boot sequence, architecture,
                           writing a driver, the security model, and how
                           the (per-directory) build system fits together
```

## Building and running

This has been developed and is regularly built on an ARM64 Raspberry
Pi cross-compiling to x86-64, so the toolchain choice matters:

```
make clang     # clean rebuild with Clang - use this on an ARM host
make gcc       # clean rebuild with GCC - needs a real x86-64 GCC
make           # plain make; may fail on a non-x86-64 host
```

Then:

```
make run          # build + boot in a QEMU window
make run-smp       # same, with 4 vCPUs (splash draws 4 toasts)
make test          # headless automated boot test (many checks)
```

Once booted, log in (`root` / `toast` on a fresh image) and try:

```
help
ls -l /bin
cat /etc/motd
tpm install ksh           # ksh (and pcc/nasm/fastfetch) aren't in the base image
ksh                       # a real POSIX-ish shell, not just tsh
cpuinfo                   # detected CPU topology (ACPI/MADT)
timers                    # PIT / LAPIC timer / ACPI PM timer / HPET
apic                      # Local/IO APIC routing status
useradd -m -s /bin/tsh john
doas useradd -m jane      # privilege elevation via /etc/doas.conf, real password prompt
ifconfig
ping -c 3 10.0.2.2
ping6 fec0::2
ssh-keygen -f /tmp/id
sshd &                    # then, from the host: ssh -p 22 root@<guest-ip>
highx --wm                # or --de for the mouse-driven desktop
clint http://example.com  # from inside a highx session
hxvideo /video/clip.mp4
hxcube                    # a rotating 3D cube via HighGL's fixed-function pipeline
```

## Test suite

Each of these is a real, end-to-end test driving actual QEMU through
its virtual keyboard/QMP and checking real output - not a mock:

| Command | Covers |
|---|---|
| `make test` | Console boot, shell built-ins, ELF exec, crash isolation, users, `doas` |
| `make test-highx` / `test-de` | The window system, tusWM / tusDE, input, focus, damage |
| `make test-shell` / `test-term` | tsh quoting/pipes/history; hxtsh + hxfiles + the wheel |
| `make test-net` | Ethernet, ARP, ICMP, DNS, a real TCP conversation |
| `make test-ssh` / `test-ssh-interop` | TUS's SSH client against a host sshd, and vs. real OpenSSH |
| `make test-usb` | USB keyboard/mouse over xHCI |
| `make test-install` | The installer, and booting the disk it wrote, UEFI, no CD |
| `make test-keymap` | Every keyboard layout, dead keys, UTF-8, in-window too |
| `make test-res` | Display mode changes at the console and under highX |
| `make test-video` | Booting and playing the shipped MP4 |
| `make test-crypto` | tuscrypt against RFC test vectors (host-only) |
| `make test-clint` / `test-js` | Clint's HTML/CSS/layout and JS engine (host-only) |
| `make test-highgl` | HighGL vs. a brute-force reference model (host-only) |
| `make test-font` | TrueType under ASan, fuzzed with truncated/corrupted fonts (host-only) |
| `make test-compositor` / `test-fb` | The highX compositor and text console (host-only unit tests) |
| `make test-mp4` | The MP4 demuxer + H.264 decoder (host-only) |

## Known issues

- Background jobs started in `ksh` (`sleep 60 &`) don't stay visible to
  `ps` afterward - a ksh-side job-control gap, not a bug in the new
  `ps`/`SYS_GETPROCS` path, which correctly reports what the task table
  actually contains.
- `sshd` allocates a real per-session PTY (`/dev/ptmx`/`/dev/pts/N`)
  when a client asks for one via pty-req, instead of the old plain
  pipe pair - but a live shell session over it still doesn't actually
  work yet: a real `ssh -t` client authenticates fine and the shell is
  reported started, but no shell output ever reaches the client within
  the test timeout. One real, confirmed root cause on this path has
  already been found and fixed (`slave_open` on a `/dev/pts/N` node
  was never actually set true, so the pty master's read a session's
  own sshd hands to `ssh_chan_pump()` saw an instantly-EOF ring instead
  of blocking for real shell output) - but that alone wasn't enough to
  get a full session working, so a second issue remains, not yet
  root-caused (candidates ruled in/out are in that commit's message
  and the `sshd:` commit history; `tests/test_sshd_pty.py` is the
  repro). The old pipe-based path (no pty-req) is unaffected.
- `fan` reports "not found" under QEMU (which implements no ACPI EC at
  all) - expected there; unverified on real EC-equipped hardware.
- The published `ksh` `tpm` package predates the fix for a crash in any
  ksh command that spawns a child process (`clear`, external commands,
  etc. - musl's raw `syscall`-instruction `clone`/`vfork` paths, invalid
  on TUS's `int $0x80` ABI, now fixed in libc). Rebuilding and
  republishing that package needs a genuine x86-64 cross-build
  environment (ksh93's own build system executes target-architecture
  probe binaries during configuration, which qemu-user can't do for
  TUS's non-Linux syscall ABI) - not yet done.

## Roadmap

- **Bringing up the parked application processors for real.** ACPI/MADT
  topology discovery and a spinlock primitive exist; the AP trampoline,
  per-CPU state, and fine-grained locking across the scheduler/VFS/page
  tables that real concurrent execution needs do not yet.
- Threads (`clone`, futexes) - TUS has real processes now, not yet
  real threads.
- **ext4/ext2** as a second, standard-format filesystem alongside WRF
  (which already handles `/home` persistence) - not started.
- Copy-on-write for WRF, so a crash mid-write can't leave a block torn
  (checksumming and dual-superblock recovery exist; COW doesn't yet).
- A real 802.11 association (ath9k-htc's USB command/data path, tested
  against an actual or virtual AP) to go with the WPA2 crypto core and
  management-frame layer that already exist.
- A dynamic linker for the Linux binary compatibility layer - only
  statically linked Linux binaries run today.
- Kernel KASLR (the kernel links at a fixed base) and userspace ASLR -
  neither implemented yet.
- `sshd` allocating a real PTY per session, to give SSH shells actual
  job control and window-resize forwarding.
- Moving the display server out of the kernel and into a userspace
  daemon, once job control makes that practical (the protocol is
  already transport-independent).
- Wiring MSI into an actual in-tree driver (it's implemented and
  tested standalone, but nothing uses it yet).
