#!/usr/bin/env python3
"""
test_tusde.py - automated test for tusDE, the TUS desktop environment

Boots tus.iso in headless QEMU, starts a desktop session (`highx --de`)
and then uses nothing but the mouse: it opens the launcher, starts a
program from it, drags a window by its title bar into an edge to tile
it, works the title bar buttons, toggles the window from the panel and
logs out with the panel's power button and ends the session from the
greeter it lands back on. Every step is checked
on the framebuffer (screendumps, by the colors tusDE paints) and on the
serial log (what the kernel and the programs report).

Usage: python3 tests/test_tusde.py   (from the project root)
"""

import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from test_boot import (mouse_click, mouse_drag, mouse_reset, mouse_to,
                       qmp_cmd, qmp_connect, screendump, sendkey, start_qemu,
                       type_text, wait_for)

# Colors tusDE and its clients paint (see userspace/tusde.c, hxclock.c).
# tusDE's own chrome went monochrome on 2026-08-23 ("siyah-beyaz olsun" -
# make it black and white) - see tusde.c's COL_* defines. ACCENT is pure
# white specifically so a +-6 colour search can never confuse it with
# the compositor's cursor (0xF0F0F0, unchanged). The three title bar
# button colours stay put on purpose: this test locates them by colour
# (find_color(BTN_CLOSE) etc.) to know where to click, so they could not
# become monochrome without a bigger rework of that location strategy.
ACCENT = (0xFF, 0xFF, 0xFF)   # the panel hairline, focus, menu marker
BTN_MIN = (0xF0, 0xB4, 0x5C)  # title bar: minimise
BTN_MAX = (0x5C, 0xD9, 0x8B)  # title bar: maximise
BTN_CLOSE = (0xF2, 0x60, 0x6B)  # title bar: close
CLOCK = (0x50, 0xD0, 0xA0)    # hxclock's lit segments

# Panel geometry, from the constants at the top of userspace/tusde.c.
# TOPBAR_H is new: a thin menu bar across the top (2026-08-23), which
# is also where the clock moved to - the dock (the old "panel") no
# longer reserves CLOCK_W of width for it, so task_button_x() below
# does not subtract it either.
PANEL_H = 44
TOPBAR_H = 26
LAUNCH_W = 84
TASK_GAP = 6
TASK_W = 190
POWER_W = 36

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


def near(body, i, rgb, tolerance):
    return (abs(body[i] - rgb[0]) <= tolerance and
            abs(body[i + 1] - rgb[1]) <= tolerance and
            abs(body[i + 2] - rgb[2]) <= tolerance)


def count_color(path, rgb, tolerance=6, region=None):
    """Count pixels close to `rgb`; region = (x0, y0, x1, y1)."""
    width, height, body = read_ppm(path)
    x0, y0, x1, y1 = region or (0, 0, width, height)
    hits = 0
    for y in range(max(y0, 0), min(y1, height)):
        row = y * width * 3
        for x in range(max(x0, 0), min(x1, width)):
            if near(body, row + x * 3, rgb, tolerance):
                hits += 1
    return hits


def find_color(path, rgb, tolerance=6, region=None):
    """The first pixel close to `rgb`, scanning top to bottom."""
    width, height, body = read_ppm(path)
    x0, y0, x1, y1 = region or (0, 0, width, height)
    for y in range(max(y0, 0), min(y1, height)):
        row = y * width * 3
        for x in range(max(x0, 0), min(x1, width)):
            if near(body, row + x * 3, rgb, tolerance):
                return x, y
    return None


def task_button_x(index, count, width):
    """Where tusDE puts a task button (userspace/tusde.c:panel_task_x)."""
    room = width - LAUNCH_W - 24 - POWER_W - 24
    tw = min(TASK_W, (room - (count - 1) * TASK_GAP) // count)
    return LAUNCH_W + 16 + index * (tw + TASK_GAP) + tw // 2


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"

    print("== tusDE desktop test ==")
    proc = start_qemu()

    try:
        offset = wait_for("tsh ready", timeout=90)
        sock = qmp_connect()
        ok("kernel boots")

        # The boot menu asks whether to run the graphics test; decline.
        time.sleep(2)
        sendkey(sock, "n")
        offset = wait_for("tus:/>", offset=offset)

        # 1. `highx --de` starts the greeter; logging in starts the
        #    desktop. root has no password on a fresh image, so Enter
        #    on the empty password field is the whole login.
        type_text(sock, "highx --de\r")
        offset = wait_for("/bin/hxlogin started", offset=offset)
        time.sleep(4)
        sendkey(sock, "ret")
        offset = wait_for("/bin/tusde started", offset=offset)
        offset = wait_for("/bin/hxtsh started", offset=offset)
        ok("the greeter logs in and starts tusDE with its first terminal")

        time.sleep(6)
        screendump(sock, "/tmp/tus-de-1.ppm")
        width, height, _ = read_ppm("/tmp/tus-de-1.ppm")

        panel = count_color("/tmp/tus-de-1.ppm", ACCENT,
                            region=(0, height - PANEL_H, width, height))
        assert panel > 200, f"the panel is not on screen ({panel} accent px)"
        ok(f"tusDE paints its panel along the bottom ({panel} accent px)")

        # The wallpaper is a gradient: the top and the bottom of the
        # desktop must not be the same color. Sampled just below the
        # new topbar (TOPBAR_H), not at y=4 any more - the topbar's
        # own translucent glass now covers that row.
        _, _, body = read_ppm("/tmp/tus-de-1.ppm")
        top_y = TOPBAR_H + 4
        top = body[(top_y * width + 4) * 3:(top_y * width + 4) * 3 + 3]
        low = body[((height - PANEL_H - 8) * width + 4) * 3:]
        assert abs(top[2] - low[2]) > 4 or abs(top[1] - low[1]) > 4, \
            "the wallpaper is flat, not a gradient"
        ok("the wallpaper is a gradient, not a flat fill")

        # 2. The terminal came up decorated: three title bar buttons.
        close = find_color("/tmp/tus-de-1.ppm", BTN_CLOSE)
        assert close is not None, "no close button on the terminal"
        ok(f"windows get a title bar with buttons (close at {close})")

        # 3. The launcher opens from the panel, with the mouse only.
        #
        # The menu's geometry (userspace/tusde.c: MENU_PAD, MENU_ROW,
        # g_menu_h) depends on how many entries /etc/highx/menu lists -
        # eleven now (Clint, Terminal, Files, Shell (hxterm), Video,
        # Clock, Demo, Fonts, LVGL, Menu, Wav Player), grown from five
        # over the life of the project, which is also why the menu is
        # tall enough that
        # its top edge sits above the screen's vertical midpoint: a
        # region check hardcoded to "the bottom half" would miss most
        # of it. Reconstructing the real geometry here, rather than
        # guessing a region generous enough to work by luck, is what
        # keeps this test meaningful the next time an entry is added.
        MENU_PAD = 10
        MENU_ROW = 34
        MENU_ENTRIES = 11
        CLOCK_ROW = 5  # 0-based: Clint, Terminal, Files, Shell, Video, Clock
        menu_h = MENU_PAD * 2 + 24 + MENU_ENTRIES * MENU_ROW
        menu_top = height - PANEL_H - menu_h - 8
        menu_region = (0, menu_top, 300, height - PANEL_H)

        mouse_reset(sock)
        mouse_to(sock, 8 + LAUNCH_W // 2, height - PANEL_H // 2)
        mouse_click(sock)
        time.sleep(1)
        screendump(sock, "/tmp/tus-de-2.ppm")
        menu = count_color("/tmp/tus-de-2.ppm", ACCENT, region=menu_region)
        assert menu > 100, f"the launcher did not open ({menu} accent px)"
        ok(f"clicking the panel launcher opens the menu ({menu} accent px)")

        # 4. Clicking an entry starts it - Clock, the sixth row.
        row_y = menu_top + MENU_PAD + 24 + CLOCK_ROW * MENU_ROW + 17
        mouse_to(sock, 80, row_y)
        mouse_click(sock)
        offset = wait_for("/bin/hxclock started", offset=offset)
        time.sleep(4)
        screendump(sock, "/tmp/tus-de-3.ppm")
        digits = count_color("/tmp/tus-de-3.ppm", CLOCK)
        assert digits > 500, f"the clock did not come up ({digits} px)"
        ok(f"the menu launches a program with one click ({digits} px of clock)")

        gone = count_color("/tmp/tus-de-3.ppm", ACCENT, region=menu_region)
        assert gone < 100, f"the menu stayed open after the click ({gone} px)"
        ok("the menu closes itself once it has launched something")

        # 5. Drag the new window by its title bar into the left edge:
        #    tusDE half-tiles whatever is dropped against a side.
        close = find_color("/tmp/tus-de-3.ppm", BTN_CLOSE)
        assert close is not None, "the clock has no title bar"
        # The three buttons and the title are at the LEFT of the bar
        # now (userspace/tusde.c draw_frame(): macOS puts them there,
        # not on the right) - so a drag point clear of all of them is
        # to the RIGHT of the close button, not to its left. Going
        # left, as this used to, walks off the left edge of the screen
        # for a window whose close button is already near x=0, and
        # mouse_to()'s relative-from-last-known-position tracking has
        # no way to notice the guest clamped that request: every
        # absolute position after this line would silently drift by
        # however far off-screen the request went.
        bar_x, bar_y = close[0] + 120, close[1]
        mouse_drag(sock, bar_x, bar_y, 0, height // 2)
        time.sleep(1)
        screendump(sock, "/tmp/tus-de-4.ppm")
        left = count_color("/tmp/tus-de-4.ppm", CLOCK,
                           region=(0, 0, width // 2, height - PANEL_H))
        right = count_color("/tmp/tus-de-4.ppm", CLOCK,
                            region=(width // 2, 0, width, height - PANEL_H))
        assert left > 500 and right < 200, \
            f"the window did not tile left (l={left} r={right})"
        ok(f"dragging a window into an edge tiles it (l={left} r={right})")

        # 6. The title bar buttons: maximise, then minimise.
        close = find_color("/tmp/tus-de-4.ppm", BTN_CLOSE)
        maximise = find_color("/tmp/tus-de-4.ppm", BTN_MAX)
        assert close is not None and maximise is not None, "buttons missing"
        mouse_to(sock, maximise[0], maximise[1])
        mouse_click(sock)
        time.sleep(1.5)
        screendump(sock, "/tmp/tus-de-5.ppm")
        right = count_color("/tmp/tus-de-5.ppm", CLOCK,
                            region=(width // 2, 0, width, height - PANEL_H))
        panel = count_color("/tmp/tus-de-5.ppm", ACCENT,
                            region=(0, height - PANEL_H, width, height))
        assert right > 200, f"maximise did not fill the screen ({right} px)"
        assert panel > 200, "a maximised window covered the panel"
        ok(f"the maximise button fills the work area, panel intact ({right} px)")

        minimise = find_color("/tmp/tus-de-5.ppm", BTN_MIN)
        assert minimise is not None, "no minimise button"
        mouse_to(sock, minimise[0], minimise[1])
        mouse_click(sock)
        time.sleep(1.5)
        screendump(sock, "/tmp/tus-de-6.ppm")
        digits = count_color("/tmp/tus-de-6.ppm", CLOCK)
        assert digits < 100, f"the window is still on screen ({digits} px)"
        ok("the minimise button puts the window away")

        # 7. Its panel button brings it back - the terminal is task 0
        #    on the panel, the clock task 1.
        mouse_to(sock, task_button_x(1, 2, width), height - PANEL_H // 2)
        mouse_click(sock)
        time.sleep(1.5)
        screendump(sock, "/tmp/tus-de-7.ppm")
        digits = count_color("/tmp/tus-de-7.ppm", CLOCK)
        assert digits > 500, f"the panel did not restore it ({digits} px)"
        ok(f"the panel task button brings a window back ({digits} px)")

        # 8. The close button really closes the application.
        close = find_color("/tmp/tus-de-7.ppm", BTN_CLOSE)
        assert close is not None, "the restored window has no title bar"
        mouse_to(sock, close[0], close[1])
        mouse_click(sock)
        time.sleep(2)
        screendump(sock, "/tmp/tus-de-8.ppm")
        digits = count_color("/tmp/tus-de-8.ppm", CLOCK)
        assert digits < 100, f"the clock survived its close button ({digits})"
        ok("the close button closes the window")

        # 9. The power button logs out: the desktop goes and the
        #    greeter (the session's leader) comes back, which is what
        #    a log out means now that a session starts at one.
        mouse_to(sock, width - POWER_W // 2 - 8, height - PANEL_H // 2)
        mouse_click(sock)
        time.sleep(5)
        screendump(sock, "/tmp/tus-de-8b.ppm")
        card = count_color("/tmp/tus-de-8b.ppm", (0x13, 0x1C, 0x2B))
        assert card > 20000, f"the greeter did not come back ({card} px)"
        ok("the panel's power button logs out, back to the greeter")

        # ... and the greeter's own button is what ends the session.
        mouse_to(sock, width - 3 * 140 + 60, height - 30)
        mouse_click(sock)
        offset = wait_for("highx: session ended", offset=offset, timeout=30)
        ok("the greeter ends the session and hands the screen back")

        time.sleep(2)
        screendump(sock, "/tmp/tus-de-9.ppm")
        leftover = count_color("/tmp/tus-de-9.ppm", ACCENT)
        assert leftover < 50, f"desktop pixels survived the session ({leftover})"
        ok("the text console is repainted (no desktop pixels left)")

        # 10. The other session is still one flag away.
        type_text(sock, "highx --wm\r")
        offset = wait_for("/bin/tuswm started", offset=offset)
        time.sleep(4)
        sendkey(sock, "ctrl-q")
        offset = wait_for("highx: session ended", offset=offset, timeout=30)
        ok("`highx --wm` starts tusWM instead")

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
