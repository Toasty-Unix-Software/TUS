#!/usr/bin/env python3
"""
test_doas_auth.py - boot-level check that doas actually requires a
password for a non-root, non-nopass user.

Security regression test: login used to never drop the session from
root, and doas's own default target (no -u) silently defaulted to
"self" instead of "root", and elf_exec() always restarted every task
at uid 0 regardless of who was logged in - any combination of which
made doas's password prompt either never fire, or fire but not
actually gate anything. This boots, creates a non-root user, adds
them to /etc/doas.conf with a password-required rule, logs in as
them, and checks that: doas prompts for a password, a wrong password
is rejected (and the elevated command does NOT run), and the right
password is accepted (and it does).
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

        # Create a non-root account, give it a password, and let it
        # invoke doas for exactly one command (password required -
        # no "nopass").
        type_text(sock, "useradd -m -s /bin/tsh ahmet\r")
        offset = wait_for("user ahmet added", offset=offset)
        type_text(sock, "passwd ahmet\r")
        offset = wait_for("New password:", offset=offset)
        type_text(sock, "secret1\r")
        offset = wait_for("Retype new password:", offset=offset)
        type_text(sock, "secret1\r")
        offset = wait_for("password for ahmet updated", offset=offset)
        type_text(sock, "echo 'permit ahmet as root useradd' >> /etc/doas.conf\r")
        offset = wait_for("tus:/>", offset=offset)

        # Log in as ahmet: the session must actually drop to ahmet's
        # uid, not stay root.
        type_text(sock, "login ahmet\r")
        offset = wait_for("Password:", offset=offset)
        type_text(sock, "secret1\r")
        offset = wait_for("Welcome to TUS, ahmet!", offset=offset)
        offset = wait_for("uid=1000", offset=offset)
        offset = wait_for("tus:/>", offset=offset)
        test_boot.ok("login drops the session to the authenticated uid")

        # doas with the WRONG password: must prompt, must reject, and
        # the privileged command must NOT have run.
        type_text(sock, "doas useradd -m mehmet\r")
        offset = wait_for("password:", offset=offset, timeout=15)
        test_boot.ok("doas prompts for a password for a non-root, non-nopass user")
        type_text(sock, "wrongpass\r")
        offset = wait_for("authentication failed", offset=offset, timeout=15)
        test_boot.ok("doas rejects a wrong password")
        cat_start = offset
        type_text(sock, "cat /etc/passwd\r")
        offset = wait_for("tus:/>", offset=offset)
        with open(test_boot.SERIAL_LOG, "rb") as f:
            f.seek(cat_start)
            tail = f.read(offset - cat_start).decode("utf-8", "replace")
        assert "mehmet:x:" not in tail, (
            "doas ran the command despite a wrong password:\n" + tail)
        test_boot.ok("a rejected doas password does not run the elevated command")

        # doas with the RIGHT password: must succeed and actually run
        # the command as root.
        type_text(sock, "doas useradd -m mehmet\r")
        offset = wait_for("password:", offset=offset, timeout=15)
        type_text(sock, "secret1\r")
        offset = wait_for("user mehmet added", offset=offset, timeout=15)
        test_boot.ok("doas accepts the correct password and runs the command")

        print(f"\n{test_boot.PASS} check(s) passed")
    finally:
        qemu.terminate()
        try:
            qemu.wait(timeout=5)
        except Exception:
            qemu.kill()


if __name__ == "__main__":
    main()
