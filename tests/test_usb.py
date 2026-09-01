#!/usr/bin/env python3
"""
test_usb.py - USB keyboard and mouse over xHCI

Boots tus.iso with a qemu-xhci controller and a USB keyboard and mouse
attached, and checks the whole path: the controller comes up, both
devices are enumerated and claimed, and - the part that matters -
keystrokes and pointer movement really arrive.

QEMU routes its input to the most recently attached device, so with a
usb-kbd present the QMP `sendkey` commands the other tests use go over
USB rather than the PS/2 port. That is what makes this test meaningful
rather than a test of the PS/2 driver with USB switched on nearby.

Usage: python3 tests/test_usb.py   (from the project root)
"""

import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from test_boot import (SERIAL_LOG, QMP_SOCK, ok, qmp_connect, screendump,
                       sendkey, type_text, wait_for)
from test_highx import ACCENT, count_color, read_ppm

SCREEN = "/tmp/tus-usb.ppm"


def start_qemu():
    for stale in (SERIAL_LOG, QMP_SOCK, SCREEN):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso", "-m", "512M",
         "-vga", "std",
         "-device", "qemu-xhci,id=xhci",
         "-device", "usb-kbd,bus=xhci.0",
         "-device", "usb-mouse,bus=xhci.0",
         "-display", "none", "-no-reboot",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-usb-qemu.pid"],
        stdout=open("/tmp/tus-usb-qemu.log", "w"), stderr=subprocess.STDOUT)


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"

    print("== TUS USB (xHCI + HID) test ==")
    qemu = start_qemu()
    try:
        sock = qmp_connect()

        off = wait_for("graphics test be performed", timeout=90)
        ok("kernel boots with an xHCI controller attached")

        off = wait_for("[xhci] version", timeout=30)
        ok("the xHCI controller is found and reset")

        # Enumeration runs from the polling task, which starts after
        # the boot splash hold, so it may land either side of the
        # prompt. Wait for it on its own.
        wait_for("claimed by usb-keyboard", timeout=30)
        ok("the USB keyboard is enumerated and claimed")

        wait_for("claimed by usb-mouse", timeout=30)
        ok("the USB mouse is enumerated and claimed")

        # Decline the graphics test - over USB, since that is the
        # device QEMU is now sending to.
        sendkey(sock, "n")
        off = wait_for("tus:/>", timeout=60, offset=off)
        ok("a USB keystroke reaches the kernel (the prompt answered)")

        # A whole command, including shifted characters: the modifier
        # byte of the boot report has to be decoded, not just the key
        # array.
        type_text(sock, "echo USB-KEYBOARD-WORKS\r")
        off = wait_for("USB-KEYBOARD-WORKS", timeout=20, offset=off)
        ok("typing a command over USB works (uppercase and '-' included)")

        # The `usb` command should describe what it found.
        type_text(sock, "usb\r")
        off = wait_for("usb-keyboard", timeout=20, offset=off)
        ok("`usb` lists the keyboard's slot and driver")

        # The decisive check. QEMU also emulates a PS/2 keyboard, so a
        # shell that answered proves only that SOME keyboard works.
        # The USB driver counts the keys it decoded; a non-zero count
        # is the only proof that these keystrokes came over USB.
        with open(SERIAL_LOG, "rb") as f:
            tail = f.read().decode("utf-8", "replace")
        line = [l for l in tail.splitlines() if "usb-keyboard:" in l][-1]
        keys = int(line.split("reports,")[1].split("keys")[0].strip())
        assert keys >= 20, f"the USB driver decoded only {keys} keys"
        ok(f"the keystrokes really came over USB ({keys} keys decoded)")

        # ---- the mouse ----
        #
        # A pointer only exists inside a highX session, so start one
        # and move the USB mouse. The cursor is drawn where the
        # compositor thinks the pointer is, so finding it away from
        # the centre proves movement arrived.
        type_text(sock, "highx\r")
        off = wait_for("/bin/tuswm started", timeout=40, offset=off)
        time.sleep(5)
        ok("a highX session starts")

        screendump(sock, SCREEN)
        bar = count_color(SCREEN, ACCENT, region=(0, 0, 1280, 24))
        assert bar > 200, f"tusWM did not paint ({bar} px)"
        ok("the desktop is up")

        # Push the cursor hard into the top left corner. The server
        # clamps, so a big enough push always lands at 0,0 - and a
        # cursor that is at 0,0 afterwards is a cursor that moved.
        for _ in range(40):
            qmp_cmd_move(sock, -100, -100)
        time.sleep(1.0)
        screendump(sock, "/tmp/tus-usb-corner.ppm")
        corner = white_ish("/tmp/tus-usb-corner.ppm", 0, 0, 24, 24)
        assert corner > 20, f"no cursor in the top left corner ({corner} px)"
        ok(f"USB mouse movement moves the pointer ({corner} cursor px at 0,0)")

        # And back out again, to show it is movement and not a cursor
        # that was drawn there all along.
        for _ in range(6):
            qmp_cmd_move(sock, 100, 100)
        time.sleep(1.0)
        screendump(sock, "/tmp/tus-usb-moved.ppm")
        corner2 = white_ish("/tmp/tus-usb-moved.ppm", 0, 0, 24, 24)
        assert corner2 < corner, \
            f"the cursor did not leave the corner ({corner2} px still there)"
        ok("the pointer follows further movement (the corner cleared)")

        print("\n== all USB checks passed ==")
        return 0
    finally:
        qemu.terminate()
        try:
            qemu.wait(timeout=5)
        except subprocess.TimeoutExpired:
            qemu.kill()


def qmp_cmd_move(sock, dx, dy):
    from test_boot import qmp_cmd
    qmp_cmd(sock, "input-send-event", {"events": [
        {"type": "rel", "data": {"axis": "x", "value": dx}},
        {"type": "rel", "data": {"axis": "y", "value": dy}},
    ]})
    time.sleep(0.02)


def white_ish(path, x0, y0, w, h):
    """Light pixels in a region - the cursor is a white arrow with a
    dark outline, and nothing else on the desktop there is light."""
    width, height, body = read_ppm(path)
    count = 0
    for y in range(y0, min(y0 + h, height)):
        for x in range(x0, min(x0 + w, width)):
            i = (y * width + x) * 3
            if body[i] > 180 and body[i + 1] > 180 and body[i + 2] > 180:
                count += 1
    return count


if __name__ == "__main__":
    try:
        sys.exit(main())
    except AssertionError as exc:
        print(f"\n  [FAIL] {exc}")
        sys.exit(1)
