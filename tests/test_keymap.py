#!/usr/bin/env python3
"""
test_keymap.py - keyboard layouts, UTF-8 and dead keys

QEMU's `sendkey` names a PHYSICAL KEY, not a character: "sendkey a"
presses the key one to the right of Caps Lock whatever is printed on
it. That is exactly what a layout test needs - the same key press has
to produce a different letter on each layout, and the only way to see
that is to press keys by position and read what came out.

What is checked:

  - /etc/keymap decides the layout the machine boots with
  - Turkish Q, including the two letters the language is usually
    broken by: dotted capital I and dotless lowercase i
  - Turkish F, German and French put different letters on the same keys
  - dead keys compose (German acute + e = e-acute)
  - the letters survive the whole path - keyboard, shell line editor,
    /bin/echo, the tty - and come back as UTF-8 on the serial log
  - the same typing works inside a highX terminal window, which is a
    different path entirely (highX key event -> hxtsh -> a pipe)

Usage: python3 tests/test_keymap.py   (from the project root)
"""

import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import test_boot
from test_boot import ok, qmp_connect, sendkey, type_text, wait_for

SERIAL_LOG = "/tmp/tus-keymap.log"
QMP_SOCK = "/tmp/tus-keymap-qmp.sock"
test_boot.SERIAL_LOG = SERIAL_LOG
test_boot.QMP_SOCK = QMP_SOCK


def start_qemu():
    for stale in (SERIAL_LOG, QMP_SOCK):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso", "-m", "512M",
         "-vga", "std", "-display", "none", "-no-reboot",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-keymap-qemu.pid"],
        stdout=open("/tmp/tus-keymap-qemu.log", "w"),
        stderr=subprocess.STDOUT)


LETTER_KEYS = [chr(c) for c in range(ord("a"), ord("z") + 1)]

# Keys that carry a letter or a dash on SOME layout but not on a US
# one. Turkish F puts 'y' on the apostrophe key and AZERTY puts 'm' on
# the semicolon key, so a calibration that only pressed a..z would
# leave the harness unable to type "keymap".
EXTRA_KEYS = ["apostrophe", "semicolon", "bracket_left", "bracket_right",
              "comma", "dot", "backslash", "minus", "equal", "slash", "6",
              # '/' is shifted on most European layouts: Turkish Q and
              # German put it on Shift+7, AZERTY on Shift+'.'.
              "shift-7", "shift-dot", "shift-slash"]

# Every character this test has to type in a command.
NEEDED = set("doaskeympulhigxnrtfc-/")

# char -> QEMU key name for the layout currently loaded. Rebuilt by
# calibrate() after every switch; see the note there.
KEYS = {}


def prompt(offset):
    """Advance to just past the next shell prompt.

    Every step in this test leaves the offset sitting at a prompt that
    is waiting for input. Without that invariant a wait_for() on part
    of a command's output leaves the offset mid-output, and the NEXT
    step's wait for a prompt matches the one that command already
    printed - so it reads the previous command's last line and
    compares it against what it just typed."""
    return wait_for("tus:/>", timeout=25, offset=offset)


def press(sock, keys):
    """Press a list of physical keys, by QEMU's names for them."""
    for k in keys:
        sendkey(sock, k)
        time.sleep(0.05)


def log_upto(offset):
    """The log decoded up to a BYTE offset (see test_boot.wait_for -
    offsets are byte offsets, and this log is UTF-8)."""
    with open(SERIAL_LOG, "rb") as f:
        return f.read(offset).decode("utf-8", "replace")


def read_output(offset):
    """The line the shell printed, given the offset just past the
    prompt that followed it."""
    lines = [l for l in log_upto(offset).splitlines() if l]
    return lines[-2] if len(lines) >= 2 else ""


def echo_keys(sock, keys, offset):
    """Type `echo <keys>` by PHYSICAL key and return what came back.

    Reading the answer off the serial log rather than predicting it is
    the whole point: the harness cannot type a letter it does not know
    the layout produces, so it asks the machine."""
    type_cmd(sock, "echo ")
    press(sock, keys)
    sendkey(sock, "ret")
    offset = wait_for("tus:/>", timeout=20, offset=offset)
    return read_output(offset), offset


def type_cmd(sock, text):
    """Type a command through the CURRENT layout.

    test_boot.type_text presses the key a US keyboard has that
    character on, which is wrong the moment the layout is not US -
    "keymap -l" comes out as "keymap *l" on Turkish Q, and "doas" as
    "doqs" on AZERTY. KEYS maps each character back to the physical
    key that produces it here."""
    for ch in text:
        if ch == " ":
            sendkey(sock, "spc")
        elif ch == "\r":
            sendkey(sock, "ret")
        elif ch in KEYS:
            sendkey(sock, KEYS[ch])
        else:
            type_text(sock, ch)  # digits and anything not calibrated
            continue
        time.sleep(0.05)


def calibrate(sock, offset):
    """Learn what each physical key types on the layout now loaded.

    Rather than keep a second copy of every layout table in the test -
    which would only prove the two copies agree - the harness presses
    the 26 US letter keys and reads back what the machine produced.
    The layout table itself is under test the whole time.

    The readback does not go through `echo`, because typing the word
    "echo" already needs the map we are trying to build: on Turkish F
    those four keys produce "ğvth". It goes through the SHELL'S OWN
    ERROR instead - press the keys, press Enter, and tsh says

        tsh: <whatever those keys produced>: command not found

    which needs no command to exist and no layout to be known."""
    global KEYS

    def probe(keys):
        nonlocal_offset[0] = nonlocal_offset[0]
        press(sock, keys)
        sendkey(sock, "ret")
        nonlocal_offset[0] = wait_for("command not found", timeout=25,
                                      offset=nonlocal_offset[0])
        line = log_upto(nonlocal_offset[0]).splitlines()[-1]
        # "tsh: XYZ: command not found"
        if ": command not found" not in line:
            raise AssertionError(
                f"calibration line was {line!r}, not a not-found message")
        body = line[len("tsh: "):line.rindex(": command not found")]
        nonlocal_offset[0] = wait_for("tus:/>", timeout=20,
                                      offset=nonlocal_offset[0])
        return body

    nonlocal_offset = [offset]

    produced = probe(LETTER_KEYS)
    KEYS = {}
    if len(produced) == len(LETTER_KEYS):
        for key, ch in zip(LETTER_KEYS, produced):
            KEYS.setdefault(ch, key)
    else:
        KEYS = {c: c for c in LETTER_KEYS}

    # Anything still missing is on a key a US keyboard does not have a
    # letter on. Those go through `echo` one key at a time rather than
    # through the not-found trick: several of them produce '/', and a
    # token with a slash in it is a PATH to tsh, which answers "cannot
    # execute" instead of "command not found". Typing `echo` is safe
    # by now - the letters are calibrated.
    off = nonlocal_offset[0]
    for key in EXTRA_KEYS:
        if NEEDED <= set(KEYS):
            break
        type_cmd(sock, "echo ")
        sendkey(sock, key)
        sendkey(sock, "ret")
        off = wait_for("tus:/>", timeout=25, offset=off)
        produced = read_output(off).strip()
        if len(produced) == 1:
            KEYS.setdefault(produced, key)

    missing = NEEDED - set(KEYS)
    assert not missing, \
        f"cannot type {sorted(missing)} on this layout - calibration failed"
    return off


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"

    print("== TUS keyboard layout test ==")
    qemu = start_qemu()
    try:
        sock = qmp_connect()
        off = wait_for("graphics test be performed", timeout=90)
        sendkey(sock, "n")
        sendkey(sock, "ret")
        # The console now requires a real login (console_login_gate()
        # in kernel/main.c) before anything else - root's password is
        # "toast" (see rootfs/etc/shadow), typed here on the default
        # 'us' boot layout before any layout switching happens. Older
        # kernels with no login gate boot straight to "tus:/>" and
        # never print "login:", so this is skipped the same way the
        # graphics prompt above is.
        try:
            wait_for("login:", timeout=15, offset=off)
            type_text(sock, "root\r")
            wait_for("Password:", offset=off, timeout=15)
            type_text(sock, "toast\r")
        except AssertionError:
            pass
        off = wait_for("tus:/>", timeout=60, offset=off)
        ok("booted to the shell")
        off = calibrate(sock, off)

        # ---- /etc/keymap picks the boot layout ----
        with open(SERIAL_LOG, "rb") as f:
            boot = f.read().decode("utf-8", "replace")
        assert "keymap      : us (from /etc/keymap)" in boot, \
            "the kernel did not load the layout /etc/keymap names"
        ok("/etc/keymap decides the layout at boot")

        type_cmd(sock, "keymap\r")
        off = wait_for("us - US English", timeout=20, offset=off)
        off = prompt(off)
        ok("`keymap` reports the loaded layout")

        type_cmd(sock, "keymap -l\r")
        off = wait_for("tr-f   Turkish F", timeout=20, offset=off)
        off = prompt(off)
        ok("`keymap -l` lists the layouts")

        # The image ships the US layout, which is the right default for
        # the same reason the kernel falls back to it: it is the one
        # whose characters you can still type the name of another
        # layout with. Turkish is one command away.
        type_cmd(sock, "doas keymap -s tr\r")
        off = wait_for("keymap: tr", timeout=20, offset=off)
        off = wait_for("/etc/keymap updated", timeout=20, offset=off)
        off = prompt(off)
        off = calibrate(sock, off)
        ok("`keymap -s` loads a layout and makes it the default")

        type_cmd(sock, "cat /etc/keymap\r")
        off = wait_for("tr", timeout=20, offset=off)
        off = prompt(off)
        ok("/etc/keymap now names the new layout")

        # ---- Turkish Q ----
        #
        # These are physical keys. On a US keyboard they would type
        # i ; [ , . ] - on Turkish Q they are six Turkish letters.
        out, off = echo_keys(sock, ["i", "semicolon", "bracket_left",
                                    "comma", "dot", "bracket_right"], off)
        assert out.endswith("ışğöçü"), f"got {out!r}, expected 'ışğöçü'"
        ok("Turkish Q types ışğöçü on the US i ; [ , . ] keys")

        # The two i's. Shift+i is a PLAIN capital I (the dotless i's
        # capital); the dotted capital İ lives on the apostrophe key.
        # A system that upper-cases 'i' to 'I' gets this wrong, which
        # is why the layout carries both cases itself.
        out, off = echo_keys(sock, ["shift-apostrophe", "shift-i",
                                    "shift-semicolon",
                                    "shift-bracket_left"], off)
        assert out.endswith("İIŞĞ"), f"got {out!r}, expected 'İIŞĞ'"
        ok("the dotted and dotless capital I are both right")

        # ---- switching layouts ----
        type_cmd(sock, "doas keymap tr-f\r")
        off = wait_for("keymap: tr-f", timeout=20, offset=off)
        off = prompt(off)
        off = calibrate(sock, off)
        out, off = echo_keys(sock, ["a", "s", "d"], off)
        assert out.endswith("uie"), f"got {out!r}, expected 'uie' on Turkish F"
        ok("Turkish F puts u i e where Q has a s d")

        type_cmd(sock, "doas keymap de\r")
        off = wait_for("keymap: de", timeout=20, offset=off)
        off = prompt(off)
        off = calibrate(sock, off)
        out, off = echo_keys(sock, ["y", "z", "semicolon"], off)
        assert out.endswith("zyö"), f"got {out!r}, expected 'zyö' on German"
        ok("German swaps y and z and puts ö on the ; key")

        # Dead keys: the ´ key (US '=') types nothing, and then an 'e'
        # becomes é. Pressed twice it types the accent on its own.
        out, off = echo_keys(sock, ["equal", "e", "equal", "equal"], off)
        assert out.endswith("é´"), f"got {out!r}, expected 'é´'"
        ok("a dead key composes, and doubled types the accent itself")

        # A dead key before something that does not combine must show
        # BOTH characters rather than swallow one.
        out, off = echo_keys(sock, ["equal", "x"], off)
        assert out.endswith("´x"), f"got {out!r}, expected '´x'"
        ok("a dead key that cannot combine loses nothing")

        type_cmd(sock, "doas keymap fr\r")
        off = wait_for("keymap: fr", timeout=20, offset=off)
        off = prompt(off)
        off = calibrate(sock, off)
        out, off = echo_keys(sock, ["a", "q", "w"], off)
        assert out.endswith("qaz"), f"got {out!r}, expected 'qaz' on AZERTY"
        ok("French AZERTY moves a, q and w")

        type_cmd(sock, "doas keymap us\r")
        off = wait_for("keymap: us", timeout=20, offset=off)
        off = prompt(off)
        off = calibrate(sock, off)
        out, off = echo_keys(sock, ["a", "s", "d"], off)
        assert out.endswith("asd"), f"got {out!r}, expected 'asd'"
        ok("US is still US")

        # ---- an unknown layout is refused ----
        type_cmd(sock, "doas keymap klingon\r")
        off = wait_for("no layout called 'klingon'", timeout=20, offset=off)
        off = prompt(off)
        ok("an unknown layout is refused by name")

        # ---- the same thing inside a window ----
        #
        # A highX terminal is a different path end to end: the key
        # becomes an hx_event, hxtsh encodes it as UTF-8 and writes it
        # down a pipe, and the shell reads bytes. Nothing here is
        # shared with the console except the layout itself.
        type_cmd(sock, "doas keymap tr\r")
        off = wait_for("keymap: tr", timeout=20, offset=off)
        off = prompt(off)
        off = calibrate(sock, off)

        type_cmd(sock, "highx\r")
        off = wait_for("/bin/hxtsh started", timeout=40, offset=off)
        time.sleep(5)
        ok("a highX session starts")

        type_cmd(sock, "echo ")
        press(sock, ["i", "semicolon", "bracket_left"])
        sendkey(sock, "ret")
        time.sleep(3)
        with open(SERIAL_LOG, "rb") as f:
            tail = f.read().decode("utf-8", "replace")
        assert "ışğ" in tail[off:], \
            "the Turkish letters did not reach the shell in the window"
        ok("typing Turkish in a highX terminal window works")

        print("\n== all keyboard layout checks passed ==")
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
