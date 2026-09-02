#!/usr/bin/env python3
"""
test_ksh_clear.py - boot-level repro/regression check for the ksh
Invalid Opcode crash.

`ksh` was installed via `tpm install ksh`, launched, and running its
`clear` builtin crashed the kernel with "Invalid Opcode" at the
`syscall` instruction inside musl's __clone (reached via
posix_spawn, which ksh's job control uses to run any external
command). TUS traps syscalls through `int $0x80`/`$0x81`, not the
raw `syscall` instruction, and musl's x86_64 clone.s/vfork.s never
got patched for that - see sources/musl-1.2.6/src/thread/x86_64/
clone.s and src/process/x86_64/vfork.s.

This boots, installs ksh via tpm, launches it, and runs `clear`
(and a plain external command for good measure) - the whole point
being to actually exercise posix_spawn's __clone path, not just
that ksh starts.

KNOWN CURRENT STATUS: the musl-side fix is done and independently
verified by test_clone_fix.py (a tiny posix_spawn() test program
built and linked against the just-fixed libc), but the *published*
ksh.tpkg in /home/pi/projects/packages still contains a ksh93 binary
built before the fix - AST/ksh93's build system (bin/package) refuses
to cross-compile from this aarch64 host ("Cross-compiling is not
supported"), so ksh93 can only be rebuilt on an x86_64 machine. This
test will keep failing until someone rebuilds and republishes
ksh.tpkg from an x86_64 host with the corrected
sources/musl-1.2.6/src/thread/x86_64/clone.s and
src/process/x86_64/vfork.s. Kept here as the exact end-to-end repro
for whoever does that rebuild to verify against.
"""

import sys

sys.path.insert(0, "tests")
import test_boot  # noqa: E402
from test_boot import start_qemu, qmp_connect, type_text, wait_for  # noqa: E402


def main():
    qemu = start_qemu()
    try:
        sock = qmp_connect()

        offset = wait_for("tsh ready", timeout=60)
        offset = wait_for("graphics test be performed", offset=offset,
                          timeout=15)
        type_text(sock, "n\r")
        try:
            wait_for("login:", timeout=15, offset=offset)
            type_text(sock, "root\r")
            wait_for("Password:", offset=offset, timeout=15)
            type_text(sock, "toast\r")
        except AssertionError:
            pass
        offset = wait_for("tus:/>", offset=offset)

        type_text(sock, "tpm update\r")
        offset = wait_for("tus:/>", offset=offset, timeout=30)

        type_text(sock, "tpm install ksh\r")
        offset = wait_for("Setting up ksh", offset=offset, timeout=60)
        offset = wait_for("tus:/>", offset=offset)

        type_text(sock, "ksh\r")
        offset = wait_for("started as pid", offset=offset, timeout=15)

        # This is the actual repro: `clear` runs as an external
        # command via posix_spawn -> __clone. Before the fix this
        # faulted with Invalid Opcode and killed the ksh process.
        clear_start = offset
        type_text(sock, "clear\r")
        offset = wait_for("#", offset=offset, timeout=15)

        with open(test_boot.SERIAL_LOG, "rb") as f:
            f.seek(clear_start)
            since_clear = f.read().decode("utf-8", "replace")
        assert "Invalid Opcode" not in since_clear, \
            "ksh crashed running 'clear' (Invalid Opcode regression)"
        assert "fault:" not in since_clear, \
            f"unexpected fault after 'clear': {since_clear!r}"

        # A second external command, to make sure __clone survives
        # being used more than once (fork/exit reuse, not just a
        # one-shot fluke).
        type_text(sock, "echo clone-ok\r")
        offset = wait_for("clone-ok", offset=offset, timeout=15)

        type_text(sock, "exit\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        print("PASS: ksh + posix_spawn(clear) survived, no Invalid Opcode fault")
        return 0
    finally:
        qemu.terminate()


if __name__ == "__main__":
    sys.exit(main())
