#!/usr/bin/env python3
"""
test_ssh.py - TUS logs in to a real OpenSSH server

Boots tus.iso behind QEMU's user-mode network and has the guest ssh
out to an sshd running on this host, which the guest reaches at
10.0.2.2. Nothing about the connection is simulated: the guest
generates its own key, the host's sshd authenticates it, and a command
runs on the far end.

The key travels the only way it safely can - the guest makes it and
prints the public half, the test scrapes that off the serial console
and hands it to sshd as an authorized key. That also means ssh-keygen
is under test rather than assumed.

Usage: python3 tests/test_ssh.py   (from the project root)
"""

import os
import re
import shutil
import subprocess
import sys
import tempfile
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from test_boot import (  # noqa: E402
    qmp_connect, sendkey, type_text, wait_for, ok,
    SERIAL_LOG, QEMU_LOG, QMP_SOCK,
)

SSHD_PORT = 18922
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
         "-nic", "user,model=rtl8139",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-qemu-ssh.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


def run_cmd(qmp, text, expect, timeout=60, offset=0):
    type_text(qmp, text)
    sendkey(qmp, "ret")
    return wait_for(expect, timeout=timeout, offset=offset)


def serial_text():
    with open(SERIAL_LOG, "rb") as f:
        return f.read().decode("utf-8", "replace")


def start_sshd(work, authorized_keys):
    """One sshd, one connection. Debug mode keeps it in the
    foreground and avoids needing root for a pid file or a log."""
    hostkey = os.path.join(work, "hostkey")
    subprocess.run(["ssh-keygen", "-q", "-t", "ed25519", "-f", hostkey,
                    "-N", ""], check=True)
    log = open(os.path.join(work, "sshd.log"), "w")
    return subprocess.Popen(
        ["/usr/sbin/sshd", "-d", "-p", str(SSHD_PORT), "-h", hostkey,
         "-o", f"PidFile={work}/sshd.pid",
         "-o", f"AuthorizedKeysFile={authorized_keys}",
         "-o", "PasswordAuthentication=no",
         # The keys live under /tmp, which sshd would otherwise refuse
         # to trust because /tmp is world-writable.
         "-o", "StrictModes=no",
         "-o", "ListenAddress=127.0.0.1"],
        stdout=log, stderr=subprocess.STDOUT)


def main():
    if not os.path.exists("tus.iso"):
        print("tus.iso not found - build the ISO first")
        return 1
    for tool in ("ssh-keygen", "/usr/sbin/sshd"):
        if shutil.which(tool) is None and not os.path.exists(tool):
            print(f"SKIP: {tool} is not installed")
            return 0

    print("== TUS ssh test ==")
    work = tempfile.mkdtemp(prefix="tus-ssh-")
    proc = start_qemu()
    sshd = None

    try:
        offset = wait_for("tsh ready", timeout=BOOT_TIMEOUT)
        qmp = qmp_connect()
        time.sleep(2)
        sendkey(qmp, "n")           # skip the boot splash
        # The kernel drains up to the next newline before continuing
        # (kernel/main.c) - "n" alone leaves it blocked forever.
        sendkey(qmp, "ret")
        # The console now requires a real login (console_login_gate()
        # in kernel/main.c) before anything else - root's password is
        # "toast" (see rootfs/etc/shadow). Older kernels with no login
        # gate boot straight to "tus:/>" and never print "login:", so
        # this is skipped the same way the graphics prompt above is.
        try:
            wait_for("login:", timeout=15, offset=offset)
            type_text(qmp, "root\r")
            wait_for("Password:", offset=offset, timeout=15)
            type_text(qmp, "toast\r")
        except AssertionError:
            pass
        offset = wait_for("tus:/>", offset=offset)
        ok("kernel boots with the network up")

        offset = run_cmd(qmp, "ifconfig", "10.0.2.15", offset=offset)
        ok("the guest has an address")

        # ---- ssh-keygen ----

        offset = run_cmd(qmp, "ssh-keygen -f /tmp/id -C tus@test",
                         "The key fingerprint is:", offset=offset)
        ok("ssh-keygen writes a key pair")

        offset = run_cmd(qmp, "ssh-keygen -l -f /tmp/id", "SHA256:",
                         offset=offset)
        ok("ssh-keygen reads its own key back")

        offset = run_cmd(qmp, "cat /tmp/id.pub", "tus@test", offset=offset)
        match = re.search(r"(ssh-ed25519 [A-Za-z0-9+/=]+ tus@test)",
                          serial_text())
        assert match, "the public key line never appeared on the console"
        pubkey = match.group(1)
        ok("the public key is in OpenSSH's one-line form")

        # ---- a real login ----

        authorized_keys = os.path.join(work, "authorized_keys")
        with open(authorized_keys, "w") as f:
            f.write(pubkey + "\n")
        os.chmod(authorized_keys, 0o600)

        sshd = start_sshd(work, authorized_keys)
        time.sleep(2)
        assert sshd.poll() is None, "sshd exited before the guest connected"

        user = os.environ.get("USER", "root")
        type_text(qmp, f"ssh -p {SSHD_PORT} -i /tmp/id {user}@10.0.2.2 "
                       "'echo TUS-SSH-WORKS; id -un'")
        sendkey(qmp, "ret")

        # The host is unknown, so the client asks before trusting it.
        offset = wait_for("Continue connecting", offset=offset, timeout=60)
        ok("an unknown host key is queried, not silently accepted")

        type_text(qmp, "yes")
        sendkey(qmp, "ret")

        offset = wait_for("TUS-SSH-WORKS", offset=offset, timeout=90)
        ok("public key authentication succeeds against OpenSSH")

        offset = wait_for(user, offset=offset, timeout=30)
        ok("the remote command runs as the expected user")

        # A second connection must not ask again: the key was recorded.
        offset = run_cmd(qmp,
                         f"ssh -p {SSHD_PORT} -i /tmp/id {user}@10.0.2.2 true",
                         "tus:", offset=offset, timeout=60)
        text = serial_text()[offset - 200:] if offset > 200 else serial_text()
        assert "Continue connecting" not in text, \
            "the host key was not remembered"
        ok("known_hosts is written and reused")

        print("\nall ssh checks passed")
        return 0

    except AssertionError as e:
        print(f"  [FAIL] {e}")
        print("\n--- last of the serial log ---")
        print(serial_text()[-3000:])
        return 1
    finally:
        if sshd and sshd.poll() is None:
            sshd.terminate()
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            proc.kill()
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
