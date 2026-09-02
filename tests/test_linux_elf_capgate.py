#!/usr/bin/env python3
"""
test_linux_elf_capgate.py - CAP_LINUX_EXEC actually gates the Linux
compat layer: a non-root user without the capability must be refused,
not silently granted it.
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

        type_text(sock, "useradd nolinux\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)
        type_text(sock, "passwd nolinux\r")
        wait_for("New password:", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        wait_for("Retype", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        type_text(sock, "login\r")
        wait_for("login:", offset=offset, timeout=15)
        type_text(sock, "nolinux\r")
        wait_for("Password:", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        run_start = offset
        type_text(sock, "lxhello\r")
        offset = wait_for("CAP_LINUX_EXEC required", offset=offset,
                          timeout=15)

        with open(test_boot.SERIAL_LOG, "rb") as f:
            f.seek(run_start)
            since_run = f.read().decode("utf-8", "replace")

        assert "hello from linux elf on TUS" not in since_run, \
            "non-root ran the Linux binary without CAP_LINUX_EXEC"

        print("PASS: CAP_LINUX_EXEC gate refuses a non-root, uncapable user")
        return 0
    finally:
        qemu.terminate()


if __name__ == "__main__":
    sys.exit(main())
