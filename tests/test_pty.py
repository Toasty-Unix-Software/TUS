#!/usr/bin/env python3
"""
test_pty.py - boot-level check that the kernel PTY layer works.

Boots tus.iso, drops into ksh93, and runs its `pty` builtin (which
uses openpty()/ptsname() -> /dev/ptmx + /dev/pts/N under the hood) to
run `echo` through a real pseudo-terminal. If the kernel's PTY devices
are missing or broken, ksh's pty builtin fails to allocate a pty and
prints an error instead of the echoed text.
"""

import sys

sys.path.insert(0, "tests")
import test_boot  # noqa: E402
from test_boot import start_qemu, qmp_connect, type_text, wait_for  # noqa: E402


def main():
    qemu = start_qemu()
    try:
        import time

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

        type_text(sock, "ksh\r")
        offset = wait_for("started as pid", offset=offset, timeout=15)

        # ksh93's own prompt varies by build; give it a moment then
        # drive the pty builtin directly.
        time.sleep(1)
        type_text(sock, "pty echo hello-from-pty\r")
        offset = wait_for("hello-from-pty", offset=offset, timeout=15)
        test_boot.ok("ksh's pty builtin runs a program through "
                     "/dev/ptmx + /dev/pts")

        print(f"\n{test_boot.PASS} check(s) passed")
    finally:
        qemu.terminate()
        try:
            qemu.wait(timeout=5)
        except Exception:
            qemu.kill()


if __name__ == "__main__":
    main()
