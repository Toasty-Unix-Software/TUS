#!/usr/bin/env python3
"""
test_term.py - terminal sessions, the file manager and the wheel

Three things landed together and this drives all three in one boot:

  hxtsh    a terminal window that runs the KERNEL's tsh over a
           terminal session (include/tusterm.h) instead of carrying a
           shell of its own. The proof is not that text appears - it
           is that a kernel built-in (`ls`, `sysinfo`, which print
           with kprintf and never touch a file descriptor) shows up in
           the window, and that a command typed into the window
           leaves a file behind that the CONSOLE shell can read back.

  hxfiles  the file manager: a listing that scrolls, with folders in
           it (their icon colour is what the screendumps count).

  wheel    PS/2 IntelliMouse support in the driver, HX_PTR_WHEEL in
           the protocol, and clients that scroll on it.

Usage: python3 tests/test_term.py   (from the project root)
"""

import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from test_boot import (mouse_click, mouse_to, mouse_wheel, qmp_connect,
                       screendump, sendkey, start_qemu, type_text, wait_for)

TEXT   = (0xE8, 0xE8, 0xE8)   # what tsh prints (the kernel's COLOR_FG)
INK    = (0xC8, 0xD4, 0xE0)   # hxtsh's own default ink, before any SGR
PROMPT = (0xFF, 0xA0, 0x40)   # tsh's prompt (COLOR_ACCENT), in a window
FOLDER = (0xF9, 0xAB, 0x00)   # hxfiles' folder icons
TERM_BG = (0x0C, 0x12, 0x18)  # hxtsh's paper
PAGE   = (0xFF, 0xFF, 0xFF)   # Clint's page
ACCENT = (0x1A, 0x73, 0xE8)   # hxfiles' selection stripe and toolbar

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


def text_px(path, region=None):
    """Every colour the window writes text in: hxtsh's own default
    ink until the shell sends an SGR, and the kernel's COLOR_FG after
    it does."""
    return (count_color(path, TEXT, region=region) +
            count_color(path, INK, region=region))


def centroid(path, rgb, tolerance=6):
    """Middle of everything painted in `rgb` - which is how the test
    finds a window on screen without knowing where the window manager
    put it."""
    width, height, body = read_ppm(path)
    sx = sy = n = 0
    for y in range(height):
        row = y * width * 3
        for x in range(width):
            i = row + x * 3
            if (abs(body[i] - rgb[0]) <= tolerance and
                    abs(body[i + 1] - rgb[1]) <= tolerance and
                    abs(body[i + 2] - rgb[2]) <= tolerance):
                sx += x
                sy += y
                n += 1
    assert n > 0, f"nothing painted in {rgb}"
    return sx // n, sy // n


def differs(a, b, region=None):
    """How many pixels changed between two screendumps."""
    wa, ha, ba = read_ppm(a)
    wb, hb, bb = read_ppm(b)
    assert (wa, ha) == (wb, hb), "screen size changed between dumps"
    x0, y0, x1, y1 = region or (0, 0, wa, ha)
    changed = 0
    for y in range(y0, min(y1, ha)):
        row = y * wa * 3
        for x in range(x0, min(x1, wa)):
            i = row + x * 3
            if ba[i:i + 3] != bb[i:i + 3]:
                changed += 1
    return changed


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"

    print("== terminal sessions, file manager and wheel test ==")
    proc = start_qemu()

    try:
        offset = wait_for("tsh ready", timeout=90)
        sock = qmp_connect()
        ok("kernel boots")

        # The boot menu asks whether to run the graphics test; decline.
        time.sleep(2)
        sendkey(sock, "n")
        offset = wait_for("tus:/>", offset=offset)

        # ---- hxtsh ----

        type_text(sock, "highx\r")
        offset = wait_for("/bin/tuswm started", offset=offset)
        offset = wait_for("/bin/hxtsh started", offset=offset)
        ok("`highx` starts tusWM, which opens hxtsh - not hxterm")

        time.sleep(5)
        screendump(sock, "/tmp/tus-term-1.ppm")
        width, height, _ = read_ppm("/tmp/tus-term-1.ppm")

        banner = text_px("/tmp/tus-term-1.ppm")
        assert banner > 300, f"the shell printed nothing ({banner} px)"
        ok(f"the kernel's tsh runs inside the window ({banner} text px)")

        prompt = count_color("/tmp/tus-term-1.ppm", PROMPT)
        assert prompt > 50, f"no prompt in the window ({prompt} px)"
        ok(f"the prompt arrives in the shell's own colour ({prompt} px)")

        # A kernel built-in: `ls` prints with kprintf, which never
        # passes a file descriptor - only the console capture puts it
        # in a window.
        type_text(sock, "ls /bin\r")
        time.sleep(3)
        screendump(sock, "/tmp/tus-term-2.ppm")
        listed = text_px("/tmp/tus-term-2.ppm")
        assert listed > banner, f"`ls` printed nothing ({listed} px)"
        ok(f"a kernel built-in prints into the window ({listed} px)")

        # A program in /bin, started by the kernel's shell, inherits
        # the session: its stdout is the window.
        type_text(sock, "grep -i -n toast /etc/motd\r")
        offset = wait_for("/bin/grep started", offset=offset)
        time.sleep(2)
        ok("the session's shell spawns /bin programs")

        # Redirection and pipelines are the shell's, not the
        # terminal's: both leave files the console can read back.
        type_text(sock, "echo terminal-works > /tmp/t1\r")
        time.sleep(2)
        type_text(sock, "ls /bin | grep hxfiles > /tmp/t2\r")
        time.sleep(3)
        ok("typed a redirection and a pipeline into the window")

        # `cd` in the window must not move the console shell.
        type_text(sock, "cd /etc\r")
        time.sleep(2)
        screendump(sock, "/tmp/tus-term-3.ppm")
        ok("`cd` inside the window changes that shell's directory")

        # ---- the wheel ----

        # Fill the window, then wheel back into the scrollback.
        type_text(sock, "help\r")
        time.sleep(3)
        screendump(sock, "/tmp/tus-term-4.ppm")
        mouse_to(sock, width // 2, height // 2)
        mouse_wheel(sock, 4)          # four notches up
        time.sleep(1)
        screendump(sock, "/tmp/tus-term-5.ppm")
        scrolled = differs("/tmp/tus-term-4.ppm", "/tmp/tus-term-5.ppm")
        assert scrolled > 2000, f"the wheel did nothing ({scrolled} px)"
        ok(f"the wheel scrolls the terminal's scrollback ({scrolled} px)")

        mouse_wheel(sock, -4)
        time.sleep(1)
        screendump(sock, "/tmp/tus-term-6.ppm")
        back = differs("/tmp/tus-term-4.ppm", "/tmp/tus-term-6.ppm")
        assert back < scrolled // 4, f"wheeling back did not return ({back})"
        ok(f"wheeling the other way comes back to the live text ({back} px)")

        # ---- hxfiles ----

        sendkey(sock, "meta_l-f")
        offset = wait_for("/bin/hxfiles started", offset=offset)
        time.sleep(4)
        screendump(sock, "/tmp/tus-files-1.ppm")
        folders = count_color("/tmp/tus-files-1.ppm", FOLDER)
        assert folders > 200, f"no folder icons on screen ({folders} px)"
        ok(f"Super+F opens the file manager, listing / ({folders} folder px)")

        # The pointer has to be over the file manager for the wheel
        # to reach it: highX delivers a wheel turn to the window under
        # the cursor, and the window manager tiles, so which half of
        # the screen the listing landed on is not the test's to guess.
        fx, fy = centroid("/tmp/tus-files-1.ppm", FOLDER)
        mouse_to(sock, fx, fy)
        time.sleep(0.5)

        # The first row of / is `bin` and it starts out selected, so
        # Enter walks into it - a directory full of files and no
        # folders at all, which the icon count can see.
        sendkey(sock, "ret")
        time.sleep(3)
        screendump(sock, "/tmp/tus-files-2.ppm")
        in_bin = count_color("/tmp/tus-files-2.ppm", FOLDER)
        assert in_bin < folders // 4, f"still in / ({in_bin} folder px)"
        ok(f"Enter opens the selected directory ({in_bin} folder px in /bin)")

        # /bin is longer than the window, so the wheel has somewhere
        # to go - and the scrollbar has a thumb to move.
        mouse_wheel(sock, -3)
        time.sleep(1)
        screendump(sock, "/tmp/tus-files-3.ppm")
        moved = differs("/tmp/tus-files-2.ppm", "/tmp/tus-files-3.ppm")
        assert moved > 500, f"the list did not scroll ({moved} px)"
        ok(f"the wheel scrolls the file list ({moved} px changed)")

        # Backspace is "up one level", back to the folders of /.
        sendkey(sock, "backspace")
        time.sleep(2)
        screendump(sock, "/tmp/tus-files-4.ppm")
        back_up = count_color("/tmp/tus-files-4.ppm", FOLDER)
        assert back_up > folders // 2, f"did not go back up ({back_up} px)"
        ok(f"Backspace goes back up to / ({back_up} folder px)")

        # ---- the browser scrolls on the wheel ----
        #
        # Clint is where the wheel matters most: its welcome page is
        # taller than its window, and until now nothing but the
        # keyboard could move it. The file manager is closed first -
        # it paints on white paper too, and the test finds Clint's
        # window by looking for white.
        sendkey(sock, "ctrl-w")   # tusWM: close the focused window
        time.sleep(3)
        screendump(sock, "/tmp/tus-term-cl0.ppm")
        tx, ty = centroid("/tmp/tus-term-cl0.ppm", TERM_BG)
        mouse_to(sock, tx, ty)
        mouse_click(sock)
        time.sleep(1)
        type_text(sock, "clint\r")
        offset = wait_for("/bin/clint started", offset=offset)
        time.sleep(6)
        screendump(sock, "/tmp/tus-clint-1.ppm")
        px, py = centroid("/tmp/tus-clint-1.ppm", PAGE)
        mouse_to(sock, px, py)
        mouse_wheel(sock, -5)
        time.sleep(1)
        screendump(sock, "/tmp/tus-clint-2.ppm")
        moved = differs("/tmp/tus-clint-1.ppm", "/tmp/tus-clint-2.ppm")
        assert moved > 5000, f"the page did not scroll ({moved} px)"
        ok(f"the wheel scrolls the browser's page ({moved} px changed)")

        mouse_wheel(sock, 5)
        time.sleep(1)
        screendump(sock, "/tmp/tus-clint-3.ppm")
        home = differs("/tmp/tus-clint-1.ppm", "/tmp/tus-clint-3.ppm")
        assert home < moved // 8, f"wheeling back did not return ({home} px)"
        ok(f"wheeling up returns to the top of the page ({home} px)")

        sendkey(sock, "ctrl-w")   # tusWM: close the focused window
        time.sleep(3)

        # ---- `exit` closes the window ----
        #
        # The terminal is the other half of the tiled screen; click it
        # to focus it (tusWM focuses what you click), then type the
        # built-in that ends a session.
        screendump(sock, "/tmp/tus-term-7.ppm")
        tx, ty = centroid("/tmp/tus-term-7.ppm", TERM_BG)
        mouse_to(sock, tx, ty)
        mouse_click(sock)
        time.sleep(1)
        type_text(sock, "exit\r")
        time.sleep(3)
        screendump(sock, "/tmp/tus-term-8.ppm")
        left = count_color("/tmp/tus-term-8.ppm", TERM_BG)
        assert left < 500, f"the terminal window is still there ({left} px)"
        ok("`exit` ends the session's shell and the window closes")

        # ---- back to the console ----

        # Ctrl+Q ends the session (tusWM's quit binding).
        sendkey(sock, "ctrl-q")
        offset = wait_for("tus:/>", offset=offset, timeout=30)
        ok("the session ends and the text console comes back")

        # The files the WINDOW's shell wrote are on the real VFS.
        type_text(sock, "cat /tmp/t1\r")
        offset = wait_for("terminal-works", offset=offset)
        ok("`echo ... > /tmp/t1` in the window really wrote the file")

        type_text(sock, "cat /tmp/t2\r")
        offset = wait_for("hxfiles", offset=offset)
        ok("`ls /bin | grep hxfiles > /tmp/t2` piped and redirected")

        # The console shell's own directory was never touched by the
        # `cd /etc` typed into the window.
        type_text(sock, "pwd\r")
        offset = wait_for("/\n", offset=offset)
        ok("the console shell kept its own working directory")

        print(f"\nALL {PASS} TESTS PASSED")
        return 0

    finally:
        try:
            proc.terminate()
            proc.wait(timeout=5)
        except Exception:
            proc.kill()


if __name__ == "__main__":
    sys.exit(main())
