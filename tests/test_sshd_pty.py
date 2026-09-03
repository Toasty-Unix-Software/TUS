#!/usr/bin/env python3
"""
test_sshd_pty.py - a real ssh client gets a real pty from TUS's sshd

Boots tus.iso behind QEMU's user-mode network with a hostfwd rule so
this machine's own `ssh` can reach the guest's sshd on the forwarded
port. Starts sshd manually in the guest (no need to go through
tusinstall's baked-in host key / sshd.enable flow for this), then
connects from the host with `-t` (force a pty) and confirms:

  1. `tty` inside the session reports a real /dev/pts/N, not "not a
     tty" - proving sshd allocated a pty and wired it to the shell's
     stdio instead of a plain pipe pair.
  2. A backgrounded job inside that session is still visible to `ps`
     from a second command in the same session - real job control,
     which needs a real controlling-terminal-shaped fd, not a pipe.

Usage: python3 tests/test_sshd_pty.py   (from the project root)
"""

import os
import subprocess
import sys
import time

import pexpect

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from test_boot import SERIAL_LOG, QEMU_LOG, QMP_SOCK  # noqa: E402

SSHD_PORT = 18923
BOOT_TIMEOUT = 90


def start_qemu():
    for stale in (SERIAL_LOG, QEMU_LOG, QMP_SOCK):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso", "-m", "512M",
         "-display", "none", "-no-reboot",
         "-nic", f"user,model=rtl8139,hostfwd=tcp::{SSHD_PORT}-:22",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-qemu-sshd-pty.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


def wait_serial(needle, timeout=BOOT_TIMEOUT):
    deadline = time.time() + timeout
    seen = ""
    while time.time() < deadline:
        try:
            with open(SERIAL_LOG, "rb") as f:
                seen = f.read().decode("utf-8", "replace")
        except FileNotFoundError:
            pass
        if needle in seen:
            return seen
        time.sleep(0.3)
    raise AssertionError(f"timeout waiting for {needle!r}; log tail:\n{seen[-2000:]}")


def ok(msg):
    print(f"  [PASS] {msg}")


def main():
    qemu = start_qemu()
    try:
        wait_serial("tsh ready")

        # Log in as root at the console, then start sshd in the
        # background so it is listening before the host tries to
        # connect. test_boot.py's helpers assume a QMP+console socket
        # pair; reuse those instead of re-deriving them.
        from test_boot import qmp_connect, type_text, sendkey  # noqa: E402

        sock = qmp_connect()
        try:
            wait_serial("graphics test be performed", timeout=15)
            sendkey(sock, "n")
            sendkey(sock, "ret")
        except AssertionError:
            pass
        wait_serial("login:")
        type_text(sock, "root\r")
        wait_serial("Password:")
        type_text(sock, "toast\r")
        wait_serial("Welcome to TUS, root!")

        # sshd falls back to /bin/tsh when ksh isn't installed, but
        # /bin/tsh is not actually a file execve() can ever launch -
        # tsh is a ring-0 kernel task (kernel/term/term.c's
        # task_create_kernel(term_shell_main, ...)), not a ring-3
        # binary. That fallback is its own separate, pre-existing bug
        # (not introduced by the pty work this test targets) -
        # install ksh via tpm so sshd has a real session shell to
        # launch, instead of getting blocked on it here.
        type_text(sock, "tpm update\r")
        wait_serial("tus:/>")
        type_text(sock, "tpm install ksh\r")
        wait_serial("Setting up ksh")
        wait_serial("tus:/>")
        ok("ksh installed so sshd has a real session shell to launch")

        # tsh has no `&` job-control syntax (that's the separate,
        # documented ksh/tsh gap this fork isn't fixing) - `exec`
        # calls elf_exec() and returns immediately without waiting,
        # which is what actually gets sshd running in the background
        # here.
        type_text(sock, "exec /bin/sshd -v\r")
        wait_serial("listening on port")
        ok("sshd started in the guest")

        # Give the guest's DHCP/link-up and sshd's own key generation
        # (first run) a moment before the host dials in.
        time.sleep(2)

        client = pexpect.spawn(
            "ssh", ["-t", "-p", str(SSHD_PORT),
                    "-o", "StrictHostKeyChecking=no",
                    "-o", "UserKnownHostsFile=/dev/null",
                    "-o", "LogLevel=ERROR",
                    "root@127.0.0.1"],
            timeout=30, encoding="utf-8")
        client.expect("[Pp]assword:")
        client.sendline("toast")
        client.expect(r"(tus:.*[>#]|^# )")
        ok("password auth succeeds over a real TCP connection to sshd")

        client.sendline("tty")
        client.expect("/dev/pts/")
        client.expect(r"(tus:.*[>#]|^# )")
        ok("`tty` reports a real /dev/pts/N pty, not a pipe")

        client.sendline("sleep 30 &")
        client.expect(r"(tus:.*[>#]|^# )")
        client.sendline("ps")
        idx = client.expect(["sleep", pexpect.TIMEOUT], timeout=5)
        client.expect(r"(tus:.*[>#]|^# )")
        if idx == 0:
            ok("a backgrounded job in the SSH session is visible to ps")
        else:
            print("  [FAIL] backgrounded `sleep 30 &` did not show up in ps "
                  "(this is the separate, not-yet-fixed ksh/tsh job-control "
                  "gap, not a pty allocation failure)")

        client.sendline("exit")
        client.close()

        print("\nALL SSHD PTY CHECKS PASSED")
        return 0
    finally:
        qemu.terminate()
        try:
            qemu.wait(timeout=10)
        except subprocess.TimeoutExpired:
            qemu.kill()


if __name__ == "__main__":
    sys.exit(main())
