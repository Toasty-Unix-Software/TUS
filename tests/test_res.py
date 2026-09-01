#!/usr/bin/env python3
"""
test_res.py - runtime display mode setting (res_set / SYS_VIDEO)

Boots tus.iso headless, then drives `res_set` through the virtual PS/2
keyboard and checks two independent things for every mode change:

  - what TUS says (the serial log)
  - what the machine actually shows (a QEMU screendump, whose PPM
    header carries the real framebuffer geometry)

The second one is the point. A mode change that only updates the
kernel's idea of the screen would pass every text check and leave the
display untouched, so every assertion about a resolution is made
against the pixels QEMU hands back.

Usage: python3 tests/test_res.py   (from the project root)
"""

import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from test_boot import (SERIAL_LOG, QMP_SOCK, ok, qmp_connect, screendump,
                       sendkey, type_text, wait_for)
from test_highx import ACCENT, count_color, read_ppm, text_px

SCREEN = "/tmp/tus-res.ppm"


def start_qemu():
    """Same machine as the boot test, with one difference that matters:
    -vga std is named explicitly. It is QEMU's default on x86-64, but
    this test is about the Bochs VBE registers that adapter provides,
    and a default is a bad thing to depend on silently."""
    for stale in (SERIAL_LOG, QMP_SOCK, SCREEN):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso", "-m", "512M",
         "-vga", "std",
         "-display", "none", "-no-reboot",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-res-qemu.pid"],
        stdout=open("/tmp/tus-res-qemu.log", "w"), stderr=subprocess.STDOUT)


def dump(sock, path):
    """Take a screendump and wait until the file is COMPLETE.

    test_boot.screendump waits for a non-empty file, which is enough
    at 1280x800 and is not at 1920x1080: a six-megabyte PPM takes
    QEMU long enough on this host that a reader can arrive between the
    header and the pixels. Wait for the byte count the header itself
    promises."""
    try:
        os.unlink(path)
    except FileNotFoundError:
        pass

    from test_boot import qmp_cmd
    qmp_cmd(sock, "screendump", {"filename": path})

    deadline = time.time() + 40
    while time.time() < deadline:
        try:
            with open(path, "rb") as f:
                data = f.read()
        except FileNotFoundError:
            time.sleep(0.2)
            continue
        if data.startswith(b"P6") and b"\n255\n" in data[:64]:
            parts = data[:64].split()
            w, h = int(parts[1]), int(parts[2])
            body = data[data.index(b"\n255\n") + 5:]
            if len(body) >= w * h * 3:
                return w, h, body
        time.sleep(0.2)
    raise AssertionError(f"screendump {path} never completed")


def screen_size(sock):
    """The framebuffer geometry QEMU is actually scanning out."""
    w, h, _ = dump(sock, SCREEN)
    return w, h


def black_px(path):
    """Pixels no one painted. The compositor covers the whole screen -
    background, then windows - so a black pixel in a highX screenshot
    means a region something believed was off-screen."""
    w, h, body = read_ppm(path)
    return sum(1 for i in range(0, w * h * 3, 3)
               if body[i] < 8 and body[i + 1] < 8 and body[i + 2] < 8)


def run(sock, cmd, expect, offset, timeout=20):
    """Type a command, wait for a phrase in its output, return the new
    serial-log offset."""
    type_text(sock, cmd + "\r")
    return wait_for(expect, timeout=timeout, offset=offset)


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"

    print("== TUS display mode test ==")
    qemu = start_qemu()
    try:
        sock = qmp_connect()

        # The kernel offers a graphics test before the shell starts and
        # blocks on a keypress; decline it (anything but 'y'), then
        # send the Enter it drains for before continuing (kernel/main.c).
        off = wait_for("graphics test be performed", timeout=60)
        sendkey(sock, "n")
        sendkey(sock, "ret")

        # The console now requires a real login (console_login_gate()
        # in kernel/main.c) before anything else - root's password is
        # "toast" (see rootfs/etc/shadow). Older kernels with no login
        # gate boot straight to "tus:/>" and never print "login:", so
        # this is skipped the same way the graphics prompt above is.
        try:
            wait_for("login:", timeout=15, offset=off)
            type_text(sock, "root\r")
            wait_for("Password:", offset=off, timeout=15)
            type_text(sock, "toast\r")
        except AssertionError:
            pass

        off = wait_for("tus:/>", timeout=60, offset=off)
        ok("booted to the shell")
        time.sleep(0.5)

        boot_w, boot_h = screen_size(sock)
        print(f"  boot mode: {boot_w}x{boot_h}")

        # ---- what the kernel reports must match what QEMU shows ----
        type_text(sock, "res_set\r")
        off = wait_for(f"{boot_w}x{boot_h} at 32 bpp", timeout=20, offset=off)
        ok("res_set reports the mode QEMU is scanning out")

        off = wait_for("mode setting: available", timeout=10, offset=off)
        ok("the Bochs VBE adapter was detected")

        # ---- listing ----
        off = run(sock, "res_set -l", "1280x800", off)
        ok("res_set -l lists the mode table")

        # ---- a real mode change, checked in the pixels ----
        target = (1280, 800) if (boot_w, boot_h) != (1280, 800) else (1024, 768)
        off = run(sock, f"doas res_set {target[0]}x{target[1]}",
                  f"now {target[0]}x{target[1]} at 32 bpp", off)
        ok(f"doas res_set {target[0]}x{target[1]} reports success")

        time.sleep(1.0)
        got = screen_size(sock)
        assert got == target, f"screen is {got}, expected {target}"
        ok(f"the display really changed to {target[0]}x{target[1]}")

        # ---- the console still works at the new size ----
        off = run(sock, "res_set", f"{target[0]}x{target[1]} at 32 bpp", off)
        ok("the console is usable after the mode change")

        # ---- growing PAST the boot mode ----
        #
        # This is the case the framebuffer reservation exists for.
        # Limine maps exactly the boot mode's bytes (1280x800x4 is
        # 4 MiB); 1920x1080x4 is 8.3 MiB, so painting it on Limine's
        # mapping would fault or corrupt whatever follows. The kernel
        # maps the adapter's memory at VMM_FB_BASE instead, and a
        # console that comes back readable at 1920x1080 is the proof.
        off = run(sock, "doas res_set 1920x1080", "now 1920x1080 at 32 bpp", off)
        time.sleep(1.0)
        got = screen_size(sock)
        assert got == (1920, 1080), f"screen is {got}, expected (1920, 1080)"
        ok("a mode larger than the boot mode works (remapped framebuffer)")

        off = run(sock, "ls /bin | grep hostname", "hostname", off)
        ok("the console still runs commands at 1920x1080")

        # ---- rejections ----
        off = run(sock, "res_set 4096x4096",
                  "is not a mode this machine accepts", off)
        ok("a mode past the cap is refused")

        off = run(sock, "res_set nonsense",
                  "is not a WIDTHxHEIGHT mode", off)
        ok("a malformed argument is refused")

        # ---- back to the boot mode ----
        off = run(sock, f"doas res_set {boot_w}x{boot_h}",
                  f"now {boot_w}x{boot_h} at 32 bpp", off)
        time.sleep(1.0)
        got = screen_size(sock)
        assert got == (boot_w, boot_h), f"screen is {got}, expected boot mode"
        ok("changing back to the boot mode works")

        # ---- the shell survived all of it ----
        off = run(sock, "ls /bin | grep res_set", "res_set", off)
        ok("the shell still runs commands afterwards")

        # ---- the hard case: a mode change UNDER a live highX session
        #
        # Everything the display server cached about the screen is
        # wrong after a mode change - the compositor's pixel pointer,
        # pitch and bounds, and the position of every window that now
        # hangs off the edge. highx_rebind() is what fixes it, and the
        # way to see that it worked is that the desktop is still
        # painted at the new size afterwards.
        type_text(sock, "highx\r")
        off = wait_for("/bin/tuswm started", timeout=30, offset=off)
        off = wait_for("/bin/hxtsh started", timeout=30, offset=off)
        time.sleep(5)
        ok("a highX session starts")

        shot = "/tmp/tus-res-highx-1.ppm"
        dump(sock, shot)
        bar = count_color(shot, ACCENT, region=(0, 0, boot_w, 24))
        assert bar > 200, f"tusWM status bar not painted ({bar} px)"
        ok(f"the desktop is painted at {boot_w}x{boot_h} ({bar} accent px)")

        # Keystrokes now go to the terminal window the WM opened, so
        # this runs inside the session rather than at the console.
        type_text(sock, f"doas res_set {target[0]}x{target[1]}\r")
        time.sleep(4)

        got = screen_size(sock)
        assert got == target, f"screen is {got}, expected {target}"
        ok(f"the display changed to {target[0]}x{target[1]} under highX")

        shot = "/tmp/tus-res-highx-2.ppm"
        dump(sock, shot)
        bar = count_color(shot, ACCENT, region=(0, 0, target[0], 24))
        assert bar > 200, f"status bar gone after the rebind ({bar} px)"
        ok(f"tusWM repainted its bar at the new width ({bar} accent px)")

        # Every pixel of the NEW mode must have been painted. An
        # unpainted region is the failure this rebind exists to
        # prevent: the framebuffer got bigger and the compositor kept
        # painting the old bounds, leaving a black L along two edges.
        # (The desktop background itself may be entirely hidden - the
        # terminal window keeps its size when the screen shrinks, and
        # at 1024x768 a window laid out for 1280x800 covers the lot.)
        black = black_px(shot)
        assert black == 0, f"{black} unpainted pixels after the rebind"
        ok("the compositor painted every pixel of the new mode")

        text = text_px(shot)
        assert text > 500, f"the terminal window is blank ({text} text px)"
        ok(f"the terminal window survived the mode change ({text} text px)")

        # And it is still a working session: type into that window and
        # the shell behind it answers.
        type_text(sock, "ls /bin\r")
        time.sleep(3)
        shot = "/tmp/tus-res-highx-3.ppm"
        dump(sock, shot)
        assert text_px(shot) > text, "the shell stopped answering after rebind"
        ok("the shell in the window still runs commands")

        print("\n== all display mode checks passed ==")
        return 0
    finally:
        qemu.terminate()
        try:
            qemu.wait(timeout=5)
        except subprocess.TimeoutExpired:
            qemu.kill()


if __name__ == "__main__":
    try:
        sys.exit(main())
    except AssertionError as exc:
        print(f"\n  [FAIL] {exc}")
        sys.exit(1)
