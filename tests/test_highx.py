#!/usr/bin/env python3
"""
test_highx.py - automated test for the highX window system

Boots tus.iso in headless QEMU, starts a highX session (`highx`, which
brings up tusWM), drives the window manager through its keyboard
shortcuts - including the ones that need real modifier reporting from
the PS/2 driver (Super+D, Alt+arrows, Alt+Shift+arrows) - moves and
clicks the mouse, and checks the result on both channels: the serial log (what the kernel and the
programs report) and the framebuffer itself (screendumps, checked for
the colors highX, tusWM and its clients paint).

Usage: python3 tests/test_highx.py   (from the project root)
"""

import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from test_boot import (mouse_click, mouse_move, qmp_cmd, qmp_connect,
                       screendump, sendkey, start_qemu, type_text, wait_for)

# Colors tusWM and its clients paint (see userspace/tuswm.c, hxclock.c).
# tusWM's own chrome went monochrome on 2026-08-23 ("siyah-beyaz olsun" -
# make it black and white, not the old cyan/blue "cyber" palette) - see
# tuswm.c's COL_* defines. ACCENT is pure white specifically so a +-6
# colour search can never confuse it with CURSOR (0xF0F0F0, unchanged -
# the compositor's cursor body colour was already monochrome).
ACCENT = (0xFF, 0xFF, 0xFF)   # focused title bar + border, bar highlight
CLOCK  = (0x50, 0xD0, 0xA0)   # lit seven-segment digits
BOX    = (0xE0, 0xA0, 0x40)   # hxdemo's bouncing box
GRID   = (0x24, 0x24, 0x28)   # desktop grid lines
TEXT   = (0xC8, 0xD4, 0xE0)   # hxtsh's default ink (before the
                              # shell's first colour escape)
SHELL  = (0xE8, 0xE8, 0xE8)   # and the kernel's COLOR_FG after it
CURSOR = (0xF0, 0xF0, 0xF0)   # the body of the compositor's arrow

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
    """Count pixels close to `rgb`; region = (x0, y0, x1, y1)."""
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


def text_px(path):
    """Every colour a terminal window writes text in: its own default
    ink until the shell sends an SGR, the kernel's after."""
    return count_color(path, TEXT) + count_color(path, SHELL)


def cursor_at(path, x, y):
    """Cursor body pixels in the sprite's rectangle at (x, y)."""
    return count_color(path, CURSOR, region=(x, y, x + 12, y + 19))


def cursor_probe(sock, path, x, y, tries=3):
    """Screendump until the cursor shows up at (x, y).

    QEMU converts only the guest scanlines its dirty bitmap says have
    changed, and a write that lands while it is taking that snapshot is
    missed until the same lines are written again - a capture artefact
    on the host, not something the guest can tell you about. A one
    pixel round trip re-dirties the sprite and leaves it where it was.
    """
    for _ in range(tries):
        screendump(sock, path)
        hits = cursor_at(path, x, y)
        if hits > 0:
            return hits
        mouse_move(sock, 1, 1)
        mouse_move(sock, -1, -1)
    return 0


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"

    print("== highX window system test ==")
    proc = start_qemu()

    try:
        offset = wait_for("tsh ready", timeout=90)
        sock = qmp_connect()
        ok("kernel boots")

        # The boot menu asks whether to run the graphics test; decline.
        # It then drains up to the next newline (kernel/main.c) before
        # continuing, so "n" alone with no Enter leaves it blocked
        # forever - a real Enter has to follow.
        time.sleep(2)
        sendkey(sock, "n")
        sendkey(sock, "ret")

        # The console now requires a real login (console_login_gate()
        # in kernel/main.c) before anything else - root's password is
        # "toast" (see rootfs/etc/shadow). Older kernels with no login
        # gate boot straight to "tus:/>" and never print "login:", so
        # this is skipped the same way the graphics prompt above is.
        try:
            offset = wait_for("login:", timeout=15, offset=offset)
            type_text(sock, "root\r")
            offset = wait_for("Password:", offset=offset, timeout=15)
            type_text(sock, "toast\r")
        except AssertionError:
            pass

        offset = wait_for("tus:/>", offset=offset)

        # 1. The server is not running before anything asks for it.
        type_text(sock, "highx info\r")
        offset = wait_for("highX: not running", offset=offset)
        ok("`highx info` reports no session before one is started")

        # 2. Starting a session brings up the window manager, which
        #    opens a terminal of its own.
        type_text(sock, "highx\r")
        offset = wait_for("/bin/tuswm started", offset=offset)
        offset = wait_for("/bin/hxtsh started", offset=offset)
        ok("`highx` starts the display server, tusWM and a terminal")

        time.sleep(5)
        screendump(sock, "/tmp/tus-highx-1.ppm")
        width, height, _ = read_ppm("/tmp/tus-highx-1.ppm")

        bar = count_color("/tmp/tus-highx-1.ppm", ACCENT,
                          region=(0, 0, width, 24))
        assert bar > 200, f"status bar not painted ({bar} accent px)"
        ok(f"tusWM paints its status bar ({bar} accent px in the top rows)")

        text = text_px("/tmp/tus-highx-1.ppm")
        assert text > 500, f"the terminal printed nothing ({text} text px)"
        ok(f"hxtsh renders the shell's banner and prompt ({text} text px)")

        grid = count_color("/tmp/tus-highx-1.ppm", GRID)
        assert grid > 0, "desktop background not painted"
        ok("the compositor paints the desktop background")

        # 3. The built-ins are the KERNEL's: hxtsh has no shell of its
        #    own, so `ls` here is the same `ls` the console runs.
        type_text(sock, "ls /bin\r")
        time.sleep(3)
        screendump(sock, "/tmp/tus-highx-2.ppm")
        listed = text_px("/tmp/tus-highx-2.ppm")
        assert listed > text, "ls printed nothing into the terminal"
        ok(f"the kernel's tsh runs in the window: `ls /bin` ({listed} px)")

        # 4. Anything else is a real program, spawned by that shell -
        #    and it prints into the window because a spawned task
        #    inherits the session.
        type_text(sock, "grep -i -n toast /etc/motd\r")
        offset = wait_for("/bin/grep started", offset=offset)
        time.sleep(3)
        ok("the session runs /bin programs and shows their output")

        # 5. Super+D opens the launcher; typing filters it and Enter
        #    starts the entry - all of it needs modifier reporting.
        sendkey(sock, "meta_l-d")
        offset = wait_for("/bin/hxmenu started", offset=offset)
        time.sleep(3)
        screendump(sock, "/tmp/tus-highx-3.ppm")
        menu = count_color("/tmp/tus-highx-3.ppm", ACCENT,
                           region=(0, 24, width, height))
        assert menu > 500, f"launcher not on screen ({menu} accent px)"
        ok(f"Super+D opens the hxmenu launcher ({menu} accent px)")

        type_text(sock, "clo")
        time.sleep(1)
        sendkey(sock, "ret")
        offset = wait_for("/bin/hxclock started", offset=offset)
        time.sleep(4)
        screendump(sock, "/tmp/tus-highx-4.ppm")
        digits = count_color("/tmp/tus-highx-4.ppm", CLOCK)
        assert digits > 500, f"hxclock digits missing ({digits} px)"
        ok(f"the launcher filters and starts a program ({digits} px of clock)")

        # 6. Alt+arrows move focus, and the focus is visible: the
        #    focused window carries the accent title bar and border.
        def accent_halves(path):
            left = count_color(path, ACCENT, region=(0, 24, width // 2, height))
            right = count_color(path, ACCENT,
                                region=(width // 2, 24, width, height))
            return left, right

        left, right = accent_halves("/tmp/tus-highx-4.ppm")
        assert right > left, f"new window not focused (l={left} r={right})"
        ok(f"the newest window is focused and highlighted (l={left} r={right})")

        sendkey(sock, "alt-left")
        time.sleep(3)
        screendump(sock, "/tmp/tus-highx-5.ppm")
        left, right = accent_halves("/tmp/tus-highx-5.ppm")
        assert left > right, f"Alt+Left did not move focus (l={left} r={right})"
        ok(f"Alt+Left focuses the window to the left (l={left} r={right})")

        sendkey(sock, "alt-right")
        time.sleep(3)
        screendump(sock, "/tmp/tus-highx-6.ppm")
        left, right = accent_halves("/tmp/tus-highx-6.ppm")
        assert right > left, "Alt+Right did not move focus back"
        ok("Alt+Right focuses the window to the right")

        # 7. Alt+Shift+arrow moves the focused window: the clock's
        #    digits change sides.
        before_left = count_color("/tmp/tus-highx-6.ppm", CLOCK,
                                  region=(0, 24, width // 2, height))
        sendkey(sock, "alt-shift-left")
        time.sleep(4)
        screendump(sock, "/tmp/tus-highx-7.ppm")
        after_left = count_color("/tmp/tus-highx-7.ppm", CLOCK,
                                 region=(0, 24, width // 2, height))
        assert after_left > before_left + 200, \
            f"window did not move ({before_left} -> {after_left} px left)"
        ok(f"Alt+Shift+Left moves the focused window ({before_left} -> "
           f"{after_left} px in the left half)")

        # 8. The mouse: PS/2 packets move the compositor's cursor, and
        #    the position is clamped to the screen - push it into a
        #    corner and it is still there, whole, when it comes back.
        mouse_move(sock, -3000, -3000)   # pin it to the top left
        corner = cursor_probe(sock, "/tmp/tus-highx-9.ppm", 0, 0)
        assert corner > 20, f"no cursor in the top left corner ({corner} px)"
        ok(f"the cursor follows the mouse to the top left ({corner} px)")

        mouse_move(sock, 300, 300)
        moved = cursor_probe(sock, "/tmp/tus-highx-10.ppm", 300, 300)
        gone = cursor_at("/tmp/tus-highx-10.ppm", 0, 0)
        assert moved > 20, f"cursor not at 300,300 ({moved} px)"
        assert gone == 0, f"cursor left a trail behind it ({gone} px)"
        ok(f"the cursor moves and the compositor erases the old one "
           f"({moved} px at 300,300, {gone} left behind)")

        mouse_move(sock, 4000, 4000)     # far past the bottom right
        mouse_move(sock, -60, -60)       # and back onto the screen
        back = cursor_probe(sock, "/tmp/tus-highx-11.ppm", width - 61,
                            height - 61)
        assert back > 20, \
            f"the cursor was not clamped to the screen ({back} px)"
        ok(f"movement past an edge is clamped, not lost ({back} px)")

        # 9. Clicking a window focuses it: the clock is on the left and
        #    focused (step 7), so a click on the right half moves the
        #    accent border and title bar over there.
        left, right = accent_halves("/tmp/tus-highx-11.ppm")
        assert left > right, f"the clock lost focus early (l={left} r={right})"
        mouse_move(sock, -(width - 61) + 3 * width // 4,
                   -(height - 61) + height // 2)
        mouse_click(sock)
        time.sleep(2)
        screendump(sock, "/tmp/tus-highx-12.ppm")
        left, right = accent_halves("/tmp/tus-highx-12.ppm")
        assert right > left, f"the click did not focus (l={left} r={right})"
        ok(f"clicking a window focuses it (l={left} r={right})")

        # 10. Super+Enter starts another terminal.
        sendkey(sock, "meta_l-ret")
        offset = wait_for("/bin/hxtsh started", offset=offset)
        ok("Super+Enter opens a new terminal")

        # 11. Ctrl+Q ends the session and hands the screen back to tsh.
        sendkey(sock, "ctrl-q")
        offset = wait_for("highx: session ended", offset=offset, timeout=30)
        ok("Ctrl+Q quits tusWM and the session ends")

        time.sleep(2)
        screendump(sock, "/tmp/tus-highx-8.ppm")
        leftover = count_color("/tmp/tus-highx-8.ppm", ACCENT)
        assert leftover < 50, f"window pixels survived the session ({leftover})"
        ok("the text console is repainted (no window pixels left)")

        # 12. The shell works again, and the server reports itself gone.
        type_text(sock, "highx info\r")
        offset = wait_for("highX: not running", offset=offset)
        ok("the shell is usable again and the server released the screen")

        print(f"\nALL {PASS} TESTS PASSED")
        return 0
    finally:
        try:
            qmp_cmd(sock, "quit")
        except Exception:
            pass
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except Exception:
            proc.kill()


if __name__ == "__main__":
    sys.exit(main())
