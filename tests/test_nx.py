#!/usr/bin/env python3
"""
test_nx.py - proves NX/W^X enforcement is real, not just present in
the source.

Regression coverage for the NX hardening pass (EFER.NXE + VMM_NX on
ELF data segments, user stacks, and anonymous mmap() without
PROT_EXEC): `nxtest` (tests/test_nx.c) mmaps an anonymous page,
writes a `ret` opcode into it, and jumps to it. On a correctly
hardened kernel the CPU raises a page fault (NX violation) the
instant execution reaches that page, and TUS kills the task instead
of running attacker-controlled bytes as code. If nxtest instead
prints "UNSAFE", the mmap NX default regressed.
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

        type_text(sock, "nxtest\r")
        offset = wait_for("about to jump to a non-executable page",
                          offset=offset, timeout=15)
        offset = wait_for("killed:", offset=offset, timeout=15)
        print("PASS: an anonymous mmap() page without PROT_EXEC is "
              "genuinely non-executable - jumping to it faults and "
              "kills the task")
        return 0
    finally:
        qemu.terminate()
        qemu.wait(timeout=10)


if __name__ == "__main__":
    sys.exit(main())
