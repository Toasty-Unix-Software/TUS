#!/usr/bin/env python3
"""
test_wrf_persist.py - proves WRF actually persists /home across a reboot

kernel/fs/wrf.c mounts TUS's own on-disk filesystem (WRF) at /home
instead of a plain ramfs directory, and vfs.c's create/write/read
paths route through it whenever a node's wrf_ino is set (see wrf.h).
Correctness here isn't "it compiles" or even "one boot works" - a
ramfs directory also happily holds a file for the life of one boot.
The only real proof is: format the disk, reboot, write a file under
/home, reboot again with the SAME disk image, and see the file still
there. This test does exactly that, across three real QEMU boots
sharing one disk image.

Usage: python3 tests/test_wrf_persist.py   (from the project root)
"""

import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
from test_boot import (SERIAL_LOG, QMP_SOCK, wait_for, qmp_connect,
                        type_text, sendkey)

QEMU_LOG = "/tmp/tus-wrf-qemu.log"


def start_qemu(disk):
    for stale in (SERIAL_LOG, QEMU_LOG, QMP_SOCK):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso",
         "-drive", f"file={disk},format=raw,if=ide",
         "-m", "512M", "-display", "none", "-no-reboot",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-wrf-qemu.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


def boot_and_run(disk, cmds):
    """Boot the ISO against `disk`, get past the graphics prompt and
    login gate (skipped gracefully on older kernels, same as
    test_boot.py), type `cmds` at the shell, and return the full
    serial log."""
    proc = start_qemu(disk)
    try:
        offset = wait_for("tsh ready")
        sock = qmp_connect()
        try:
            offset = wait_for("graphics test be performed", timeout=15, offset=offset)
            sendkey(sock, "n")
            sendkey(sock, "ret")
        except AssertionError:
            pass
        try:
            offset = wait_for("login:", timeout=15, offset=offset)
            type_text(sock, "root\r")
            offset = wait_for("Password:", offset=offset, timeout=15)
            type_text(sock, "toast\r")
        except AssertionError:
            pass
        offset = wait_for("tus:/>", offset=offset)
        for line in cmds:
            type_text(sock, line + "\r")
            offset = wait_for("tus:/>", offset=offset)
        with open(SERIAL_LOG, "rb") as f:
            return f.read().decode("utf-8", "replace")
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except Exception:
            proc.kill()


def main():
    with tempfile.TemporaryDirectory() as tmp:
        disk = os.path.join(tmp, "wrf_test.img")
        subprocess.run(["qemu-img", "create", "-f", "raw", disk, "64M"],
                        check=True, stdout=subprocess.DEVNULL)

        print("== boot 1: format the disk (mkfs.wrf mounts nothing itself -")
        print("   see its own header comment; the mount only happens at the")
        print("   NEXT boot, from kernel/fs/wrf.c's wrf_boot_mount()) ==")
        boot_and_run(disk, ["mkfs.wrf /dev/hda"])
        print("  OK")

        print("== boot 2: /home is now WRF-backed - create a user and a file ==")
        log2 = boot_and_run(disk, [
            "useradd -m -s /bin/tsh persisttest",
            "echo hello-from-disk > /home/persisttest/marker.txt",
            "cat /home/persisttest/marker.txt",
        ])
        assert "mounted /home" in log2, \
            "WRF did not mount /home in boot 2:\n" + log2[-2000:]
        assert "hello-from-disk" in log2, \
            "write did not read back in boot 2:\n" + log2[-2000:]
        print("  OK (mounted /home from disk, wrote marker.txt)")

        print("== boot 3: same disk image, fresh boot - the real test ==")
        log3 = boot_and_run(disk, ["cat /home/persisttest/marker.txt"])
        assert "mounted /home" in log3, \
            "WRF did not remount /home in boot 3:\n" + log3[-2000:]
        assert "hello-from-disk" in log3, \
            "marker.txt did NOT survive a reboot:\n" + log3[-2000:]
        print("  OK - /home/persisttest/marker.txt survived a reboot")

    print("PASS: WRF /home persistence verified across three real boots")
    return 0


if __name__ == "__main__":
    sys.exit(main())
