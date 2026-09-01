#!/usr/bin/env python3
"""
test_install.py - the greeter, the installer, and booting what it wrote

Two things land here and the second one is only believable end to end:

  hxlogin     the greeter a desktop session starts at. It is checked
              by driving it: a wrong password is refused, the right
              (empty, on a fresh image) one starts tusDE, and the
              desktop exiting brings the greeter back.

  tusinstall  writes an EFI system partition onto /dev/hda and copies
              the running system into it. The test then boots THAT
              DISK with no CD attached, under OVMF, and waits for the
              kernel it just installed to say it is ready.

The second half needs UEFI firmware for QEMU (/usr/share/OVMF). When
it is missing the install itself is still checked - only the boot from
the disk is skipped, and the test says so.

Usage: python3 tests/test_install.py   (from the project root)
"""

import os
import socket
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import test_boot
from test_boot import (qmp_connect, screendump, sendkey, start_qemu,
                       type_text, wait_for)


def qmp_connect_at(path):
    """qmp_connect(), but against an arbitrary socket path - qmp_connect()
    itself is hardcoded to test_boot's own QMP_SOCK."""
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    deadline = time.time() + 15
    while time.time() < deadline:
        try:
            sock.connect(path)
            break
        except (ConnectionRefusedError, FileNotFoundError):
            time.sleep(0.1)
    else:
        raise AssertionError("cannot connect to QEMU QMP socket")
    sock.recv(4096)
    sock.sendall(b'{"execute":"qmp_capabilities"}\n')
    sock.recv(4096)
    return sock

DISK = "/tmp/tus-install-disk.img"
DISK_MB = 512
OVMF_CODE = "/usr/share/OVMF/OVMF_CODE_4M.fd"
OVMF_VARS = "/usr/share/OVMF/OVMF_VARS_4M.fd"
VARS_COPY = "/tmp/tus-ovmf-vars.fd"
DISK_LOG = "/tmp/tus-diskboot.log"

ACCENT = (0x4F, 0xA3, 0xD1)   # the greeter's rule and its Log in button
CARD   = (0x13, 0x1C, 0x2B)   # the greeter's card
TERM   = (0x0C, 0x12, 0x18)   # the terminal tusDE opens: only there
                              # once a session is actually running

PASS = 0


def ok(name):
    global PASS
    PASS += 1
    print(f"  [PASS] {name}")


def read_ppm(path):
    with open(path, "rb") as f:
        data = f.read()
    assert data.startswith(b"P6"), f"{path}: not a P6 PPM"
    parts = data[:200].split()
    width, height = int(parts[1]), int(parts[2])
    body = data[data.index(b"\n255\n") + 5:]
    return width, height, body


def count_color(path, rgb, tolerance=6, region=None):
    width, height, body = read_ppm(path)
    x0, y0, x1, y1 = region or (0, 0, width, height)
    hits = 0
    for y in range(y0, min(y1, height)):
        row = y * width * 3
        for x in range(x0, min(x1, width)):
            i = row + x * 3
            if (abs(body[i] - rgb[0]) <= tolerance and
                    abs(body[i + 1] - rgb[1]) <= tolerance and
                    abs(body[i + 2] - rgb[2]) <= tolerance):
                hits += 1
    return hits


def start_qemu_with_disk():
    """The boot test's QEMU, plus an empty disk to install onto."""
    for stale in (test_boot.SERIAL_LOG, test_boot.QEMU_LOG,
                  test_boot.QMP_SOCK):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    subprocess.run(["dd", "if=/dev/zero", "of=" + DISK, "bs=1M",
                    "count=%d" % DISK_MB], capture_output=True, check=True)
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso", "-m", "512M",
         "-drive", "file=%s,format=raw,if=ide,index=0,media=disk" % DISK,
         "-display", "none", "-no-reboot",
         "-serial", "file:" + test_boot.SERIAL_LOG,
         "-qmp", "unix:%s,server=on,wait=off" % test_boot.QMP_SOCK],
        stdout=open(test_boot.QEMU_LOG, "w"), stderr=subprocess.STDOUT)


DISK_QMP = "/tmp/tus-diskboot-qmp.sock"


def boot_the_disk():
    """Boot the installed disk under UEFI, with no CD in the machine."""
    subprocess.run(["cp", OVMF_VARS, VARS_COPY], check=True)
    for stale in (DISK_LOG, DISK_QMP):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64",
         "-drive", "if=pflash,format=raw,unit=0,file=%s,readonly=on" % OVMF_CODE,
         "-drive", "if=pflash,format=raw,unit=1,file=%s" % VARS_COPY,
         "-drive", "file=%s,format=raw,if=ide,index=0,media=disk" % DISK,
         "-m", "512M", "-display", "none", "-no-reboot",
         "-serial", "file:" + DISK_LOG,
         "-qmp", "unix:%s,server=on,wait=off" % DISK_QMP],
        stdout=open("/tmp/tus-diskboot-qemu.log", "w"),
        stderr=subprocess.STDOUT)


def wait_for_in(path, needle, timeout=90):
    deadline = time.time() + timeout
    while time.time() < deadline:
        try:
            with open(path, "rb") as f:
                data = f.read()
            if needle.encode() in data:
                return data
        except FileNotFoundError:
            pass
        time.sleep(0.5)
    raise AssertionError(f"timeout waiting for {needle!r} in {path}")


def wait_for_disk(needle, timeout=90, offset=0):
    """Byte-offset wait_for() for DISK_LOG (test_boot.wait_for is hardcoded
    to its own SERIAL_LOG, a different file)."""
    if isinstance(needle, str):
        needle = needle.encode()
    deadline = time.time() + timeout
    while time.time() < deadline:
        try:
            with open(DISK_LOG, "rb") as f:
                f.seek(offset)
                data = f.read()
        except FileNotFoundError:
            time.sleep(0.1)
            continue
        pos = data.find(needle)
        if pos >= 0:
            return offset + pos + len(needle)
        time.sleep(0.1)
    with open(DISK_LOG, "rb") as f:
        tail = f.read().decode("utf-8", "replace")[-800:]
    raise AssertionError(f"timeout waiting for {needle!r}; log tail:\n{tail}")


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"

    print("== greeter and disk install test ==")
    proc = start_qemu_with_disk()

    try:
        offset = wait_for("tsh ready", timeout=90)
        sock = qmp_connect()
        ok("kernel boots with a disk attached")

        offset = wait_for("disk         : /dev/hda", offset=0)
        ok("the ATA driver finds the disk and /dev/hda exists")

        time.sleep(2)
        sendkey(sock, "n")
        offset = wait_for("tus:/>", offset=offset)

        # ---- the greeter ----

        type_text(sock, "highx --de\r")
        offset = wait_for("/bin/hxlogin started", offset=offset, timeout=30)
        ok("`highx --de` starts the greeter, not the desktop")

        time.sleep(5)
        screendump(sock, "/tmp/tus-login-1.ppm")
        width, height, _ = read_ppm("/tmp/tus-login-1.ppm")
        card = count_color("/tmp/tus-login-1.ppm", CARD)
        assert card > 20000, f"no login card on screen ({card} px)"
        ok(f"the greeter paints its card ({card} px)")

        # A password on an account that has none is refused, and the
        # greeter says why rather than just blinking.
        type_text(sock, "wrong\r")
        time.sleep(2)
        screendump(sock, "/tmp/tus-login-2.ppm")
        desktop = count_color("/tmp/tus-login-2.ppm", TERM)
        assert desktop == 0, "the greeter let a wrong password through"
        ok("a password that is not the account's is refused")

        # root has no password on a fresh image: an empty one gets in.
        sendkey(sock, "ret")
        offset = wait_for("/bin/tusde started", offset=offset, timeout=30)
        time.sleep(6)
        screendump(sock, "/tmp/tus-login-3.ppm")
        desktop = count_color("/tmp/tus-login-3.ppm", TERM)
        assert desktop > 50000, f"the desktop is not on screen ({desktop} px)"
        ok(f"logging in starts tusDE and its terminal ({desktop} px)")

        # tusDE's power button ends the desktop - and the greeter,
        # which is the session leader, comes back instead of the
        # session ending.
        from test_boot import mouse_click, mouse_to
        mouse_to(sock, width - 26, height - 23)
        mouse_click(sock)
        time.sleep(5)
        screendump(sock, "/tmp/tus-login-4.ppm")
        desktop = count_color("/tmp/tus-login-4.ppm", TERM)
        card = count_color("/tmp/tus-login-4.ppm", CARD)
        assert desktop == 0 and card > 20000, \
            f"the greeter did not come back (desktop {desktop}, card {card})"
        ok(f"logging out returns to the greeter ({card} px of card)")

        # "Exit to console" is what ends the session for real.
        mouse_to(sock, width - 3 * 140 + 60, height - 30)
        mouse_click(sock)
        offset = wait_for("highx: session ended", offset=offset, timeout=30)
        ok("the greeter's console button ends the session")

        # ---- the installer ----

        type_text(sock, "tusinstall\r")
        offset = wait_for("Which disk is the root disk?", offset=offset,
                          timeout=30)
        ok("the installer finds the disk and asks which one")

        type_text(sock, "\r")
        offset = wait_for("Are you sure?", offset=offset, timeout=20)
        type_text(sock, "no\r")
        offset = wait_for("Nothing was written", offset=offset, timeout=20)
        ok("answering anything but yes writes nothing")

        type_text(sock, "tusinstall\r")
        offset = wait_for("Which disk is the root disk?", offset=offset,
                          timeout=30)
        type_text(sock, "\r")
        offset = wait_for("Are you sure?", offset=offset, timeout=20)
        type_text(sock, "yes\r")

        # A real root password and a real user account - not the
        # defaults - is the actual point of this pass: the installed
        # disk is checked against them below, after a real reboot.
        offset = wait_for("Set root password", offset=offset, timeout=20)
        type_text(sock, "newpass123\r")
        offset = wait_for("Retype root password", offset=offset, timeout=20)
        type_text(sock, "newpass123\r")
        ok("the installer accepts a new root password")

        offset = wait_for("Create a user account?", offset=offset,
                          timeout=20)
        type_text(sock, "y\r")
        offset = wait_for("Username:", offset=offset, timeout=20)
        type_text(sock, "alice\r")
        offset = wait_for("Password:", offset=offset, timeout=20)
        type_text(sock, "alicepass\r")
        offset = wait_for("Retype password", offset=offset, timeout=20)
        type_text(sock, "alicepass\r")
        ok("the installer accepts a new user account")

        offset = wait_for("verifying", offset=offset, timeout=600)
        offset = wait_for("CONGRATULATIONS", offset=offset, timeout=120)
        ok("the install completes and verifies what it wrote")

        offset = wait_for("Exit to (S)hell, (H)alt or (R)eboot?",
                          offset=offset, timeout=20)
        ok("it ends the way an installer ends")

        type_text(sock, "s\r")
        offset = wait_for("tus:/>", offset=offset, timeout=30)
        ok("(S)hell goes back to the shell")

        # The mail the message tells you to read is really there.
        type_text(sock, "mail\r")
        offset = wait_for("Welcome to TUS", offset=offset, timeout=20)
        ok("`mail` prints the message the installer points at")

    finally:
        try:
            proc.terminate()
            proc.wait(timeout=5)
        except Exception:
            proc.kill()

    # ---- boot what was installed ----

    if not os.path.exists(OVMF_CODE) or not os.path.exists(OVMF_VARS):
        print("  [SKIP] booting the installed disk (no OVMF firmware here)")
    else:
        disk_proc = boot_the_disk()
        try:
            data = wait_for_in(DISK_LOG, "tsh ready", timeout=120)
            ok("the installed disk boots on its own, with no CD attached")
            assert b"/dev/hda" in data, "the booted system lost its disk"
            ok("the system booted from the disk sees the disk it is on")

            # The whole point of this pass: log into the INSTALLED disk
            # with the password set during install, not "toast" - and
            # confirm the user account it was told to create is really
            # there too. Both only work if build_custom_rootfs() (
            # tusinstall.c) actually rewrote etc/shadow/etc/passwd
            # inside the archive it wrote to disk, not just baked in
            # the running system's own defaults.
            doff = len(data)
            dsock = qmp_connect_at(DISK_QMP)
            doff = wait_for_disk("graphics test be performed", offset=doff,
                                 timeout=20)
            type_text(dsock, "n\r")
            doff = wait_for_disk("login:", offset=doff, timeout=20)
            type_text(dsock, "root\r")
            doff = wait_for_disk("Password:", offset=doff, timeout=20)
            type_text(dsock, "newpass123\r")
            doff = wait_for_disk("Welcome to TUS, root!", offset=doff,
                                 timeout=20)
            ok("the installed disk's root account has the password set "
               "during install, not the default")

            type_text(dsock, "cat /etc/passwd\r")
            time.sleep(1)
            with open(DISK_LOG, "rb") as f:
                f.seek(doff)
                passwd_out = f.read()
            assert b"alice" in passwd_out, \
                "the user account created during install is missing"
            ok("the user account created during install exists on disk")
        finally:
            try:
                disk_proc.terminate()
                disk_proc.wait(timeout=5)
            except Exception:
                disk_proc.kill()

    print(f"\nALL {PASS} TESTS PASSED")
    return 0


if __name__ == "__main__":
    sys.exit(main())
