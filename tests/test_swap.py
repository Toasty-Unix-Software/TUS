#!/usr/bin/env python3
"""
test_swap.py - proves disk-backed swap actually round-trips through a
real disk and a real page fault.

kernel/mm/swap.c evicts a page to a dedicated swap disk and rewrites
its PTE to a not-present, software-encoded entry; kernel/arch/x86_64/
idt.c's #PF handler reads it back on the very next touch, transparently.
"it compiles" proves nothing here - the only real proof is booting with
a formatted swap disk attached, running the `swaptest` shell command
(which writes a pattern, evicts it for real, confirms the PTE is gone,
then just reads the address and lets the page fault do the rest), and
checking the pattern survived the whole round trip.

Usage: python3 tests/test_swap.py   (from the project root)
"""

import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
from test_boot import (SERIAL_LOG, QMP_SOCK, wait_for, qmp_connect,
                        type_text, sendkey)

QEMU_LOG = "/tmp/tus-swap-qemu.log"


def start_qemu(swap_disk):
    for stale in (SERIAL_LOG, QEMU_LOG, QMP_SOCK):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso",
         "-drive", f"file={swap_disk},format=raw,if=ide",
         "-m", "512M", "-display", "none", "-no-reboot",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-swap-qemu.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


def boot_and_run(swap_disk, cmds):
    proc = start_qemu(swap_disk)
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
            offset = wait_for("tus:/>", offset=offset, timeout=20)
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
        disk = os.path.join(tmp, "swap_test.img")
        subprocess.run(["qemu-img", "create", "-f", "raw", disk, "16M"],
                        check=True, stdout=subprocess.DEVNULL)

        print("== boot 1: format the second disk as swap ==")
        log1 = boot_and_run(disk, ["mkswap /dev/hda"])
        assert "mkswap: /dev/hda:" in log1, \
            "mkswap did not report success:\n" + log1[-2000:]
        print("  OK")

        print("== boot 2: fresh boot picks up the formatted disk and "
              "swaptest exercises it ==")
        log2 = boot_and_run(disk, ["swaptest"])
        assert "swap         : /dev/hda" in log2, \
            "swap_init() did not claim /dev/hda at boot:\n" + log2[-2000:]
        assert "PTE not-present immediately after eviction: yes" in log2, \
            "eviction did not actually remove the mapping:\n" + log2[-2000:]
        assert "page fault transparently restored the pattern: yes" in log2, \
            "the page-fault-driven swap-in did not restore the data:\n" + log2[-2000:]
        assert "swaptest: PASS" in log2, \
            "swaptest reported failure:\n" + log2[-2000:]
        print("  OK - evicted a real page to disk and faulted it back in")

    print("PASS: disk-backed swap verified (real disk write, real disk "
          "read, real page fault)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
