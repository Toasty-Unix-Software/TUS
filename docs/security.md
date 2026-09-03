# Security

TUS is a hobby operating system, not a hardened production kernel, and it
is worth saying that plainly rather than overselling what is here. That
said, the security decisions that do exist were made deliberately, and
this document explains what they are, why they exist, and what the actual
threat model is, so nobody mistakes the presence of a mitigation for a
guarantee it wasn't designed to give.

## The general philosophy: fail closed

Almost every security relevant piece of TUS follows the same shape: if a
check can't be performed, or a feature can't be verified as safe, the
default is to refuse rather than to proceed optimistically. The APIC
upgrade path in `boot.md` is a good non-security example of the same
philosophy applied to reliability: every step either succeeds fully or the
whole upgrade is abandoned and the system falls back to something known to
work. Security follows the identical shape. A syscall argument that fails
validation is rejected, not clamped or best-effort interpreted. A
permission check that can't determine the caller is root treats them as
not root. Nothing in the security path tries to be clever about partial
trust.

## Syscall pointer validation

Every syscall that touches a user supplied pointer runs it through
`access_ok()` in `kernel/syscall/syscall.c` before dereferencing it. This
checks that the address and length the user program handed over actually
lie within that task's own mapped user address space, not into the
kernel's higher half and not past the end of what's actually mapped. A
ring 3 program that hands the kernel a pointer into kernel memory, or a
completely unmapped address, gets `-EFAULT` back, not a page fault that
takes the whole machine down.

This has a deliberate carve out: the ring-0 kernel shell (tsh at the
console, as opposed to a userspace program calling into the kernel through
`int $0x80`) bypasses these checks, because it is trusted code running at
the kernel's own privilege level already, calling the same syscall ABI
directly rather than through the ring 3 boundary. The check exists
specifically to stop an untrusted ring 3 program from handing the kernel a
bad pointer, and a ring 0 caller by definition isn't the thing that check
is protecting against.

## What happens when a user program actually crashes

A fault at CPL 3, ring 3, kills that one task and nothing else.
`kernel/arch/x86_64/idt.c` checks the privilege level the exception came
from, and if it's ring 3 the response is: print one line describing what
happened, then `task_exit(128 + signal)`, which gives back that task's
windows, its keyboard grab if it held one, and its open file descriptors,
and hands control to the next task. A program that writes to unmapped
memory, executes an invalid opcode, or divides by zero loses only itself.

The one exception that is fatal no matter what ring it happens in is an
NMI, a double fault, or a machine check. Those aren't a program's mistake,
they're the CPU or the platform telling you something is actually wrong at
a level no amount of per-task isolation can paper over, so TUS treats them
as fatal system-wide, the same way any real kernel would.

## Spectre v1 and v2

TUS carries mitigations for both classic Spectre variants, implemented in
`kernel/arch/x86_64/spectre.{c,h}`.

### Spectre v1, bounds check bypass

The textbook Spectre v1 gadget is a bounds check immediately followed by
an array access using the just-checked, attacker-influenced index: the CPU
can speculatively execute past the branch before the check has actually
retired, read out of bounds, and leave a measurable trace in the cache
even though the architectural result of that speculation is thrown away.

TUS's concrete instance of this pattern is `fd_get()` in
`kernel/vfs/vfs.c`: a file descriptor number arrives straight from a
syscall argument, which is to say straight from user space, and after
being checked against `0 <= fd < VFS_MAX_FDS` it indexes directly into the
per-task file descriptor table. That's exactly the shape Spectre v1
targets. The fix is an `LFENCE` speculation barrier placed right after the
bounds check and right before the indexed read, via `spectre_v1_barrier()`
in `spectre.h`. LFENCE is part of the baseline x86-64 instruction set, so
this mitigation needs no CPUID feature check and is always active; the
boot banner and `/proc/cpuinfo` both report "LFENCE enabled" for that
reason rather than reporting a runtime detected state.

### Spectre v2, branch target injection

Spectre v2 is about an attacker training the CPU's indirect branch
predictor so that a victim's indirect call or jump gets speculatively
redirected somewhere the attacker chose. The standard software mitigation
is the retpoline: recompiling every indirect branch so it goes through a
sequence that defeats the predictor. That's a compiler code-generation
feature, not something you can bolt on after the fact, and TUS specifically
cannot lean on it, because this kernel is built with three different
compilers depending on the target (GCC, Clang, and TUS's own hosted port
of PCC), and PCC has no retpoline support at all.

The mitigation TUS actually uses is IBRS, Indirect Branch Restricted
Speculation, which is a hardware feature rather than a compiler one:
setting the IBRS bit in the `IA32_SPEC_CTRL` MSR tells the CPU itself to
stop predicting indirect branches across privilege levels. `spectre_init()`
checks CPUID leaf 7, sub-leaf 0, EDX bit 26 for IBRS/IBPB support, and if
it's present, sets the bit via `wrmsr`. This closes the same hole a
retpoline would close, without depending on which of the three compilers
actually built the kernel.

Whether this shows as active depends entirely on the CPU it's running on.
Real hardware from the last several years reports "IBRS enabled". QEMU's
default emulated CPU model does not advertise the feature at all, so
under plain `qemu-system-x86_64` with no `-cpu` override, both the boot
banner and `/proc/cpuinfo` will honestly report Spectre v2 as
"Disabled" / "Vulnerable" rather than lying about a mitigation that isn't
actually there. That's intentional: these fields report ground truth, not
an aspiration.

### Where the status is reported

Both mitigation states are printed on the boot banner (`Spectre v1: ...`,
`Spectre v2: ...`) and readable at runtime from `/proc/cpuinfo` under the
`spectre_v1` and `spectre_v2` fields, in the same shape real Linux systems
use for the equivalent information, so existing tooling that already knows
to look at `/proc/cpuinfo` doesn't need TUS-specific handling.

## The VFS permission model

`kernel/vfs/vfs.c` has a real `vfs_access_ok()` check, not just a mode bit
that's decorative. Password hashes in `/etc/shadow`, for instance, are
root only, and that's actually enforced by the permission model rather
than relying on a checkout's umask happening to leave the file
world-unreadable, which used to be the only thing standing between a
non-root user and the hash file before this was tightened up.

This check is now genuinely tied to *who is actually logged in*, which
was not always true. The ring-0 console shell (tsh) never execs to become
the logged-in user - it stays a permanently-euid-0 kernel task - so file
operations it performs directly, like `>`/`>>` redirection, used to check
nothing but that always-root identity, regardless of who was actually
logged in. A non-root session could write any file on the system just by
using redirection instead of a command that execs, including
self-granting itself capabilities (see below) via
`echo uid:caps >> /etc/capabilities`, or corrupting `/etc/passwd`
directly. This was found and fixed by threading the same real
session-identity plumbing the `doas` fix (below) introduced through to
`vfs_access_ok()`'s callers (`sched_set_session_ids()` /
`sched_session_ids()`, a shared `vfs_caller_ids()` helper), so shell
built-ins are checked against who is actually logged in, not the shell's
own ring-0 privilege level. Fixing this also surfaced two related bugs -
new files were always created `root:root` regardless of the real caller,
and the root `/` node had mode `0`, silently blocking non-root path
traversal - both fixed alongside it.

SUID binaries (`doas`, `passwd`) are the deliberate escalation path, and
it's narrow on purpose: the kernel stores the SUID bit on the inode the
same way any real Unix does, `ls -l` shows the `s`, and the actual
privilege TUS-wide configuration questions (who can run `res_set`, who can
change the keymap, and so on) live in one place, `/etc/doas.conf`, rather
than being scattered across individual setuid binaries. `res_set` and
`keymap` specifically are not setuid themselves, they go through a syscall
(`SYS_VIDEO`, `SYS_INPUT`) that refuses the privileged operation outright
unless the caller's effective uid is 0.

This escalation path used to be narrower than it looked: `setuid()` did
not enforce real POSIX rules (any task could call `setuid(0)` on itself),
`login` authenticated a user but never actually dropped privileges
afterward, and the kernel never acted on a binary's setuid bit at exec
time at all - so `doas`'s "already root, skip the password" check was
comparing against an effective uid that was *always* 0 no matter who was
logged in, and silently never prompted. All of that is now real: `setuid`/
`setgid` enforce POSIX semantics, `login` drops privileges via
`setuid`/`setgid` after authenticating, setuid-on-exec is implemented for
the first time (the `4555` mode bit on `/bin/doas`/`/bin/passwd` existed
before this but the kernel never honored it), and `doas`'s password check
uses the real invoking uid rather than its own (now genuinely
setuid-root) effective uid. The only way a non-root user reaches uid 0 is
through `doas`, whose rules are all in that one auditable file, and it now
actually asks for a password to get there.

## Capabilities

Alongside the classic rwx/setuid model above, TUS has a small POSIX-style
capabilities bitmask (`kernel/sched/cap.h`) for privileges that don't map
cleanly onto "root or not" - `CAP_NET_ADMIN` (interface configuration via
`netctl`), `CAP_NET_RAW`, `CAP_SETUID`, and `CAP_LINUX_EXEC` (see the
Linux compatibility layer note below). `has_cap()` gates the relevant
syscalls instead of a raw `euid == 0` check. Root implicitly has every
capability; a non-root user only has what's explicitly granted.

Capability grants are per-user, stored in `/etc/capabilities`
(`uid:hexbits` per line, root-editable, read at login) and threaded
through the same real session-identity mechanism as the `doas`/VFS fixes
above, so a capability granted to a user persists across every command
in their login session rather than resetting on each new task spawn. A
`caps` shell command reports both the running shell's own (always-root)
task capabilities and the real session's effective set, for inspecting
what a given login actually has.

## NX / W^X

`EFER.NXE` is enabled and enforced: ELF data/rodata/bss segments, user
stacks, and anonymous `mmap()` regions created without `PROT_EXEC` are
genuinely non-executable, not just conventionally treated that way. A
task that writes code into a non-executable page and jumps to it takes a
page fault and is killed, the same as any other ring-3 fault (see above),
rather than executing attacker-controlled memory.

## The Linux binary compatibility layer

TUS can run *some* real, unmodified Linux x86-64 ELF binaries -
statically linked ones, with no dynamic linker support (`PT_INTERP`
fails outright) and only a small, explicit subset of the Linux syscall
table implemented (translated to TUS's own existing kernel functionality
under the hood, everything else returns `-ENOSYS` rather than crashing).
This is real new attack surface - a second syscall entry path
(`SYSCALL`/`SYSRET`, via `EFER.SCE` and the `STAR`/`LSTAR`/`SFMASK` MSRs,
alongside TUS's native `int $0x80` ABI) and a second binary format the
kernel will exec - so it's deliberately gated behind `CAP_LINUX_EXEC`
rather than available to every user by default. Root always has it
implicitly; a non-root user needs an explicit grant in
`/etc/capabilities`, wired through the same real session-identity
mechanism described above rather than a per-task flag that would reset
on every spawn.

## What TUS does not claim to defend against

Being direct about the boundaries matters as much as documenting the
mitigations themselves. TUS has no kernel address space layout
randomization - the kernel links at a fixed virtual base, and true KASLR
would need it built relocatable with a boot-time relocation step, which
hasn't been done. There is no userspace ASLR either (stack/heap/mmap
addresses are not randomized). There is no stack canary in the kernel
(`-fno-stack-protector` is a deliberate build flag, not an oversight,
kernel mode code doesn't get the canary support most user space runtimes
assume). There's no code signing, no secure boot chain past whatever the
firmware itself does, and no sandboxing model beyond the one process, one
address space, ring 3 isolation described above - plus, now, capability
gating for the handful of things listed above. This is a project built to
understand how an operating system works end to end, and the security
work reflects specific, understood threats, like Spectre, like an
untrusted syscall pointer, and like the real privilege-propagation bugs
described above, rather than a general claim of hardened-ness. Treat it
accordingly.
