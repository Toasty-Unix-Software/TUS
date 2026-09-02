#!/usr/bin/env python3
"""
test_linux_elf.py - boot-level check that a real, unmodified static
Linux x86_64 ELF binary runs under TUS's own kernel.

`rootfs/bin/lxhello` is `clang -target x86_64-linux-gnu -static
-nostdlib` output: it links at ~0x400000 (not TUS's 0x10000000
convention), uses the real `syscall` (0f 05) instruction with Linux's
x86_64 syscall numbers (1=write, 60=exit), and was verified to run
correctly under plain `qemu-x86_64` on the build host - i.e. it is a
genuine, unmodified Linux binary, not something built for TUS.

This is the end-to-end proof for kernel/syscall/linux_syscall.c: SYSCALL/
SYSRET MSR setup, the foreign-ELF detection in tus_elf.c, the
CAP_LINUX_EXEC gate, and the minimal Linux syscall table.
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

        run_start = offset
        type_text(sock, "lxhello\r")
        offset = wait_for("hello from linux elf on TUS", offset=offset,
                          timeout=15)

        with open(test_boot.SERIAL_LOG, "rb") as f:
            f.seek(run_start)
            since_run = f.read().decode("utf-8", "replace")

        assert "Invalid Opcode" not in since_run, \
            "the Linux binary's `syscall` instruction faulted"
        assert "hello from linux elf on TUS" in since_run, \
            "the Linux binary's write(1, ...) never reached the console"

        offset = wait_for("tus:/>", offset=offset, timeout=15)
        print("PASS: a real static Linux x86_64 ELF binary ran under TUS")
        return 0
    finally:
        qemu.terminate()


if __name__ == "__main__":
    sys.exit(main())
