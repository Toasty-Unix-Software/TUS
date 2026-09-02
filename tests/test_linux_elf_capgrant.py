#!/usr/bin/env python3
"""
test_linux_elf_capgrant.py - CAP_LINUX_EXEC is real, per-session
capability persistence, not "root only" in disguise: a non-root user
with an explicit /etc/capabilities grant can run a Linux binary, and
the grant survives across multiple commands in the same login
session (not just the first one after login). The negative case - an
ungranted non-root user is refused - is covered separately by
test_linux_elf_capgate.py, in its own boot.
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

        type_text(sock, "useradd linuxuser\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)
        type_text(sock, "passwd linuxuser\r")
        wait_for("New password:", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        wait_for("Retype", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        # linuxuser is uid 1000 (first non-root useradd on a fresh
        # image); grant it CAP_LINUX_EXEC.
        type_text(sock, "echo 1000:0x8 >> /etc/capabilities\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        # --- granted user: works, and keeps working across multiple
        # commands in the same session (not just the first one). ---
        type_text(sock, "login\r")
        wait_for("login:", offset=offset, timeout=15)
        type_text(sock, "linuxuser\r")
        wait_for("Password:", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        type_text(sock, "caps\r")
        offset = wait_for("CAP_LINUX_EXEC : yes", offset=offset, timeout=15)

        # An unrelated command in between, to prove the grant isn't
        # something that only survives for exactly one exec.
        type_text(sock, "ver\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        run_start = offset
        type_text(sock, "lxhello\r")
        offset = wait_for("hello from linux elf on TUS", offset=offset,
                          timeout=15)
        with open(test_boot.SERIAL_LOG, "rb") as f:
            f.seek(run_start)
            since_run = f.read().decode("utf-8", "replace")
        assert "hello from linux elf on TUS" in since_run, \
            "granted user's write(1, ...) never reached the console"
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        # Second command after the grant, still in the same session -
        # this is the actual persistence proof.
        run_start = offset
        type_text(sock, "lxhello\r")
        offset = wait_for("hello from linux elf on TUS", offset=offset,
                          timeout=15)
        with open(test_boot.SERIAL_LOG, "rb") as f:
            f.seek(run_start)
            since_run = f.read().decode("utf-8", "replace")
        assert "hello from linux elf on TUS" in since_run, \
            "the grant did not survive to a second command in the session"
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        print("PASS: CAP_LINUX_EXEC is real per-session capability "
              "persistence - a non-root user granted it via "
              "/etc/capabilities keeps working across multiple "
              "commands in the same login session")
        return 0
    finally:
        qemu.terminate()


if __name__ == "__main__":
    sys.exit(main())
