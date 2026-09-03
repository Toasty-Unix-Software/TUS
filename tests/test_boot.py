#!/usr/bin/env python3
"""
test_boot.py - automated boot test for TUS

Boots tus.iso in headless QEMU, waits for the TUS shell on the serial
log, then types commands through the virtual PS/2 keyboard (QMP
"sendkey") and verifies every response. Ends by triggering the kernel
panic handler and checking the register dump.

Usage: python3 tests/test_boot.py   (from the project root)
"""

import json
import os
import socket
import subprocess
import sys
import time

SERIAL_LOG = "/tmp/tus-serial.log"
QEMU_LOG   = "/tmp/tus-qemu.log"
QMP_SOCK   = "/tmp/tus-qmp.sock"
SCREEN_PPM = "/tmp/tus-screen.ppm"
BOOT_TIMEOUT = 60

PASS = 0


def ok(name):
    global PASS
    PASS += 1
    print(f"  [PASS] {name}")


def wait_for(needle, timeout=BOOT_TIMEOUT, offset=0):
    """Wait until `needle` appears in the serial log; return new offset.

    The search runs on BYTES and the offset is a byte offset. It used
    to decode first and then add a character index to a byte offset,
    which is the same number only while the log is pure ASCII - the
    moment TUS printed a Turkish letter every later offset drifted by
    one byte per non-ASCII character, and tests started reading the
    wrong line."""
    if isinstance(needle, str):
        needle = needle.encode("utf-8")
    deadline = time.time() + timeout
    while time.time() < deadline:
        try:
            with open(SERIAL_LOG, "rb") as f:
                f.seek(offset)
                data = f.read()
        except FileNotFoundError:
            time.sleep(0.1)  # QEMU has not created the log file yet
            continue
        pos = data.find(needle)
        if pos >= 0:
            return offset + pos + len(needle)
        time.sleep(0.1)
    with open(SERIAL_LOG, "rb") as f:
        tail = f.read().decode("utf-8", "replace")[-800:]
    raise AssertionError(
        f"timeout waiting for {needle.decode('utf-8', 'replace')!r}; "
        f"log tail:\n{tail}")


def qmp_connect():
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    deadline = time.time() + 15
    while time.time() < deadline:
        try:
            sock.connect(QMP_SOCK)
            break
        except (ConnectionRefusedError, FileNotFoundError):
            time.sleep(0.1)
    else:
        raise AssertionError("cannot connect to QEMU QMP socket")
    sock.recv(4096)  # greeting
    sock.sendall(b'{"execute":"qmp_capabilities"}\n')
    sock.recv(4096)
    return sock


def qmp_cmd(sock, execute, arguments=None):
    cmd = {"execute": execute}
    if arguments is not None:
        cmd["arguments"] = arguments
    sock.sendall((json.dumps(cmd) + "\n").encode())
    return sock.recv(65536)


def sendkey(sock, key):
    qmp_cmd(sock, "human-monitor-command",
            {"command-line": f"sendkey {key}"})
    time.sleep(0.03)


def type_text(sock, text):
    for ch in text:
        if ch == "\r":
            sendkey(sock, "ret")
        elif ch == "\b":
            sendkey(sock, "backspace")
        elif ch == " ":
            sendkey(sock, "spc")
        elif ch == "/":
            sendkey(sock, "slash")
        elif ch == "|":
            sendkey(sock, "shift-backslash")
        elif ch == "<":
            sendkey(sock, "shift-comma")
        elif ch == ">":
            sendkey(sock, "shift-dot")
        elif ch == ".":
            sendkey(sock, "dot")
        elif ch == "-":
            sendkey(sock, "minus")
        elif ch == "_":
            sendkey(sock, "shift-minus")
        elif ch == '"':
            sendkey(sock, "shift-apostrophe")
        elif ch == "'":
            sendkey(sock, "apostrophe")
        elif ch == ":":
            sendkey(sock, "shift-semicolon")
        elif ch == ";":
            sendkey(sock, "semicolon")
        elif ch == ",":
            sendkey(sock, "comma")
        elif ch == "=":
            sendkey(sock, "equal")
        elif ch == "+":
            sendkey(sock, "shift-equal")
        elif ch == "*":
            sendkey(sock, "shift-8")
        elif ch == "?":
            sendkey(sock, "shift-slash")
        elif ch == "!":
            sendkey(sock, "shift-1")
        elif ch == "@":
            sendkey(sock, "shift-2")
        elif ch.isupper():
            sendkey(sock, f"shift-{ch.lower()}")
        elif ch.isalpha() or ch.isdigit():
            sendkey(sock, ch)
        else:
            raise AssertionError(f"no sendkey mapping for {ch!r}")
        time.sleep(0.05)


# ---- the mouse ----
#
# QEMU's PS/2 mouse takes relative movement, so the harness tracks the
# cursor itself: mouse_reset() pins it in a corner (the server clamps,
# so a big enough push always lands at 0,0) and every later move is
# counted from there.

_MOUSE = [None, None]


def mouse_move(sock, dx, dy, step=48):
    """Relative movement, in steps a PS/2 packet can carry (nine bits,
    and QEMU sends one packet per event)."""
    while dx != 0 or dy != 0:
        sx = max(-step, min(step, dx))
        sy = max(-step, min(step, dy))
        dx -= sx
        dy -= sy
        qmp_cmd(sock, "input-send-event", {"events": [
            {"type": "rel", "data": {"axis": "x", "value": sx}},
            {"type": "rel", "data": {"axis": "y", "value": sy}},
        ]})
        time.sleep(0.02)
    time.sleep(0.4)  # let the display server pump the packet ring


def mouse_reset(sock):
    """Push the cursor into the top left corner, where it is pinned."""
    mouse_move(sock, -4000, -4000)
    _MOUSE[0], _MOUSE[1] = 0, 0


def mouse_to(sock, x, y):
    """Absolute position, by counting from the last known one."""
    if _MOUSE[0] is None:
        mouse_reset(sock)
    mouse_move(sock, x - _MOUSE[0], y - _MOUSE[1])
    _MOUSE[0], _MOUSE[1] = x, y


def mouse_button(sock, down, button="left"):
    qmp_cmd(sock, "input-send-event", {"events": [
        {"type": "btn", "data": {"down": down, "button": button}},
    ]})
    time.sleep(0.2)


def mouse_wheel(sock, steps):
    """Turn the wheel; positive is up (away from the user).

    QEMU models a wheel notch as a button press and release on
    wheel-up/wheel-down, which is what the PS/2 mouse it emulates
    reports in the fourth byte of an IntelliMouse packet."""
    button = "wheel-up" if steps > 0 else "wheel-down"
    for _ in range(abs(steps)):
        qmp_cmd(sock, "input-send-event", {"events": [
            {"type": "btn", "data": {"down": True, "button": button}},
        ]})
        qmp_cmd(sock, "input-send-event", {"events": [
            {"type": "btn", "data": {"down": False, "button": button}},
        ]})
        time.sleep(0.05)
    time.sleep(0.4)


def mouse_click(sock, button="left"):
    mouse_button(sock, True, button)
    mouse_button(sock, False, button)
    time.sleep(0.4)


def mouse_drag(sock, x0, y0, x1, y1, steps=6):
    """Press at one point, walk to another, release there."""
    mouse_to(sock, x0, y0)
    mouse_button(sock, True)
    for i in range(1, steps + 1):
        mouse_to(sock, x0 + (x1 - x0) * i // steps,
                 y0 + (y1 - y0) * i // steps)
    mouse_button(sock, False)
    time.sleep(0.5)


def start_qemu():
    for stale in (SERIAL_LOG, QEMU_LOG, QMP_SOCK, SCREEN_PPM):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso", "-m", "512M",
         "-smp", "4",
         "-display", "none", "-no-reboot",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-qemu.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


def screendump(sock, name=None):
    """Take a screendump and wait until the file is actually COMPLETE.

    A bare "non-empty file" check is enough at a small boot resolution
    (1280x800 and similar - the whole PPM lands in one write QEMU
    finishes well inside the polling interval) and is not at TUS's
    current 1920x1080 default: a ~6.2 MB P6 PPM can take long enough
    on a loaded host that a reader arrives between the header and the
    pixels, or before QEMU has even created the file. Parse the header
    for the byte count it promises and wait for that many bytes, the
    same fix tests/test_res.py already needed for the same reason."""
    path = name or SCREEN_PPM
    try:
        os.unlink(path)
    except FileNotFoundError:
        pass
    qmp_cmd(sock, "screendump", {"filename": path})
    deadline = time.time() + 30
    while time.time() < deadline:
        try:
            with open(path, "rb") as f:
                data = f.read()
        except FileNotFoundError:
            time.sleep(0.1)
            continue
        if data.startswith(b"P6") and b"\n255\n" in data[:64]:
            parts = data[:64].split()
            w, h = int(parts[1]), int(parts[2])
            body_len = len(data) - (data.index(b"\n255\n") + 5)
            if body_len >= w * h * 3:
                return
        time.sleep(0.1)
    assert os.path.exists(path), "screendump produced no file"


def count_nonblack_pixels(path):
    """P6 PPM: count pixels whose RGB channels are not all near zero."""
    with open(path, "rb") as f:
        data = f.read()
    assert data.startswith(b"P6"), "not a P6 PPM"
    parts = data[:200].split()
    width, height = int(parts[1]), int(parts[2])
    body = data[data.index(b"\n255\n") + 5:]
    assert len(body) >= width * height * 3, "truncated PPM payload"
    count = 0
    for i in range(0, width * height * 3, 3):
        r, g, b = body[i], body[i + 1], body[i + 2]
        if r > 8 or g > 8 or b > 8:
            count += 1
    return count


def wait_for_stable_screen(sock, name=None, settle_reads=2, interval=0.3,
                            max_wait=6):
    """Repeatedly screendump until the non-black pixel count is the same
    on `settle_reads` consecutive reads, then return that count.

    A single screendump taken shortly after a keypress can land mid-
    repaint under TCG (verified reproducibly slow elsewhere in this
    file: the 1.5s settle comment above the scrollback test), so a
    fixed sleep-then-dump is a timing gamble. Polling until the count
    stops changing is a real wait-for-condition instead of a guess at
    "long enough"."""
    path = name or SCREEN_PPM
    deadline = time.time() + max_wait
    last = None
    stable = 0
    while time.time() < deadline:
        screendump(sock, path)
        count = count_nonblack_pixels(path)
        if count == last:
            stable += 1
            if stable >= settle_reads:
                return count
        else:
            stable = 1
        last = count
        time.sleep(interval)
    return last


def count_white_pixels(path):
    """P6 PPM: count pixels that are pure white (255,255,255)."""
    with open(path, "rb") as f:
        data = f.read()
    parts = data[:200].split()
    width, height = int(parts[1]), int(parts[2])
    body = data[data.index(b"\n255\n") + 5:]
    count = 0
    for i in range(0, width * height * 3, 3):
        if body[i] == 255 and body[i + 1] == 255 and body[i + 2] == 255:
            count += 1
    return count


def count_toast_pixels(path):
    """P6 PPM: count warm toast-brown/orange pixels (the boot logo)."""
    with open(path, "rb") as f:
        data = f.read()
    parts = data[:200].split()
    width, height = int(parts[1]), int(parts[2])
    body = data[data.index(b"\n255\n") + 5:]
    count = 0
    for i in range(0, width * height * 3, 3):
        r, g, b = body[i], body[i + 1], body[i + 2]
        if r > 100 and 60 < g < 220 and b < 130 and r > g > b:
            count += 1
    return count


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"

    print("== TUS boot test ==")
    proc = start_qemu()

    try:
        offset = wait_for("tsh ready", timeout=BOOT_TIMEOUT)
        ok("kernel boots and shell banner appears")

        sock = qmp_connect()
        ok("QMP connected")

        # 0a. The boot banner reports the CPU count from the Limine MP
        #     feature (the test boots with -smp 4).
        with open(SERIAL_LOG, "rb") as f:
            bootlog = f.read().decode("utf-8", "replace")
        assert "cpu count    : 4" in bootlog, \
            "banner does not report 4 CPUs"
        ok("banner reports 4 CPUs (Limine MP feature)")

        # 0b. Boot splash: "tsh ready" is the last boot log line, printed
        #     right before the splash hold; grab the framebuffer during
        #     the hold. Four CPUs must draw four toasts (~50k warm px).
        screendump(sock, "/tmp/tus-splash.ppm")
        splash_lit = count_toast_pixels("/tmp/tus-splash.ppm")
        assert splash_lit > 10000, \
            f"boot splash toasts not drawn ({splash_lit} toast px)"
        ok(f"boot splash draws one toast per CPU ({splash_lit} px)")

        # The kernel asks whether to run the graphics test before the
        # shell starts and blocks on a keypress; decline it so the
        # boot continues (any key other than 'y' means no). It then
        # drains up to the next newline (kernel/main.c) so a trailing
        # Enter cannot leak into login's username prompt - an answer
        # of just "n" with no Enter leaves the kernel blocked forever
        # in that drain loop, so a real Enter has to follow.
        try:
            wait_for("graphics test be performed", timeout=15, offset=offset)
            sendkey(sock, "n")
            sendkey(sock, "ret")
        except AssertionError:
            pass  # older kernels boot straight into the shell

        # The console now requires a real login (console_login_gate()
        # in kernel/main.c, ahead of tsh_run()) before anything else -
        # root's password is "toast" (see rootfs/etc/shadow). Older
        # kernels with no login gate boot straight to "tus:/>" and
        # never print "login:", so this is skipped the same way the
        # graphics prompt above is.
        try:
            wait_for("login:", timeout=15, offset=offset)
            type_text(sock, "root\r")
            wait_for("Password:", offset=offset, timeout=15)
            type_text(sock, "toast\r")
        except AssertionError:
            pass

        # Wait for the shell prompt (the splash hold ends, console is
        # cleared and tsh takes over) before typing anything.
        offset = wait_for("tus:/>", offset=offset)

        # 1. help
        type_text(sock, "help\r")
        offset = wait_for("Built-in commands:", offset=offset)
        offset = wait_for("fbfill", offset=offset)
        ok("help lists the command table (incl. new fs commands)")

        # 2. echo + backspace editing (types "echoo" then fixes it)
        type_text(sock, "echoo\b hi\r")
        offset = wait_for("hi", offset=offset)
        ok("echo prints its arguments")

        # 3. ver (0.8.0 now)
        type_text(sock, "ver\r")
        offset = wait_for("TUS kernel 1.0.0-unstable", offset=offset)
        ok("ver reports the kernel version")

        # 4. sysinfo (now with PMM stats and uptime)
        type_text(sock, "sysinfo\r")
        offset = wait_for("MiB usable", offset=offset)
        offset = wait_for("PMM", offset=offset)
        offset = wait_for("Uptime", offset=offset)
        offset = wait_for("Framebuffer", offset=offset)
        ok("sysinfo reports memory, PMM, uptime and framebuffer")

        # 5. about
        type_text(sock, "about\r")
        offset = wait_for("Toasty Unix Software", offset=offset)
        ok("about shows the TUS identity")

        # 6. uptime (PIT timer through the syscall ABI)
        type_text(sock, "uptime\r")
        offset = wait_for("uptime:", offset=offset)
        ok("uptime reports elapsed time via the PIT")

        # 7. VFS: ls / (the tree comes from rootfs.img; ls now sorts
        #    alphabetically like a real UNIX ls)
        type_text(sock, "ls /\r")
        offset = wait_for("bin", offset=offset)
        offset = wait_for("dev", offset=offset)
        offset = wait_for("etc", offset=offset)
        offset = wait_for("logo.ppm", offset=offset)
        ok("ls / lists the rootfs tree (bin dev etc tmp + logo.ppm)")

        # 7b. ls -l shows real permission bits incl. the SUID 's' on
        #     doas/passwd (set in the image build, like an initramfs).
        type_text(sock, "ls -l /bin\r")
        offset = wait_for("-r-sr-xr-x", offset=offset)
        offset = wait_for("doas", offset=offset)
        offset = wait_for("passwd", offset=offset)
        ok("ls -l shows permission bits and the SUID bit on doas/passwd")

        # 8. VFS: ls /dev shows the device nodes (sorted now).
        type_text(sock, "ls /dev\r")
        offset = wait_for("fb0", offset=offset)
        offset = wait_for("tty0", offset=offset)
        offset = wait_for("zero", offset=offset)
        ok("ls /dev lists the built-in devices")

        # 9. cat /etc/motd
        type_text(sock, "cat /etc/motd\r")
        offset = wait_for("Welcome to TUS", offset=offset)
        offset = wait_for("Work everywhere", offset=offset)
        ok("cat /etc/motd reads a seeded file")

        # 10. echo with redirection, then cat it back
        type_text(sock, "echo hello world > /tmp/greet\r")
        type_text(sock, "cat /tmp/greet\r")
        offset = wait_for("hello world", offset=offset)
        ok("echo > file writes through the syscall ABI")

        # 11. mkdir + ls
        type_text(sock, "mkdir /tmp/sub\r")
        type_text(sock, "ls /tmp\r")
        offset = wait_for("sub", offset=offset)
        ok("mkdir creates a directory")

        # 11b. cd/pwd: a real working directory. Relative paths are
        #      resolved against it, .. walks up, and the prompt shows
        #      the current directory (tus:/tmp> ).
        type_text(sock, "pwd\r")
        offset = wait_for("/\n", offset=offset)
        type_text(sock, "cd /tmp\r")
        type_text(sock, "pwd\r")
        offset = wait_for("/tmp\n", offset=offset)
        ok("cd changes the working directory, pwd reports it")
        type_text(sock, "touch rel.txt\r")
        type_text(sock, "ls\r")
        offset = wait_for("rel.txt", offset=offset)
        ok("relative paths resolve against the working directory")
        type_text(sock, "cd sub\r")
        type_text(sock, "pwd\r")
        offset = wait_for("/tmp/sub\n", offset=offset)
        type_text(sock, "cd ..\r")
        type_text(sock, "pwd\r")
        offset = wait_for("/tmp\n", offset=offset)
        type_text(sock, "cd /\r")
        type_text(sock, "pwd\r")
        offset = wait_for("/\n", offset=offset)
        ok("cd .. walks up, cd / returns to the root")

        # 12. UNIX PATH lookup: a bare command name runs /bin/<name>
        #     (executability comes from the x bit, not an extension).
        type_text(sock, "ls /bin\r")
        offset = wait_for("hello", offset=offset)
        offset = wait_for("kilo", offset=offset)
        offset = wait_for("useradd", offset=offset)
        type_text(sock, "hello\r")
        offset = wait_for("started as pid", offset=offset)
        offset = wait_for("Hello from a static ELF", offset=offset)
        ok("bare 'hello' runs /bin/hello via the PATH lookup")

        # 12b. Address-space isolation: a second instance loads at the
        #      SAME link address (0x10000000) in its own private
        #      address space. With per-task CR3 this works; a shared
        #      address space would collide on the second load.
        type_text(sock, "hello\r")
        offset = wait_for("started as pid", offset=offset)
        offset = wait_for("Hello from a static ELF", offset=offset)
        ok("second exec runs at the same link address (per-task address space)")

        # 12c. ps shows every task with its own address space (CR3).
        type_text(sock, "ps\r")
        offset = wait_for("CR3", offset=offset)
        offset = wait_for("tsh", offset=offset)
        ok("ps lists tasks with per-task address spaces")

        # 12d. Ring-3 enforcement: a user program passing a KERNEL
        #      address as a write() buffer must get -EFAULT (-14),
        #      never a write into kernel memory.
        type_text(sock, "enforce\r")
        offset = wait_for("started as pid", offset=offset)
        offset = wait_for("-14", offset=offset)
        ok("ring-3 syscall rejects kernel pointers with -EFAULT")

        # 12d2. A program that faults is killed, and the machine keeps
        #       running: the fault is the program's, not the kernel's.
        #       (`crash`, checked at the end, is the ring-0 half of
        #       this: a kernel fault still panics.)
        type_text(sock, "fault\r")
        offset = wait_for("started as pid", offset=offset)
        offset = wait_for("killed: writing to unmapped memory at 0x0",
                          offset=offset)
        ok("a null write in a user program kills the task, not the system")

        type_text(sock, "fault opcode\r")
        offset = wait_for("killed: Invalid Opcode", offset=offset)
        ok("an invalid opcode in ring 3 kills the task")

        type_text(sock, "fault kernel\r")
        offset = wait_for("killed: writing to protected memory", offset=offset)
        ok("a write into the kernel's half kills the task")

        type_text(sock, "echo survived\r")
        offset = wait_for("survived", offset=offset)
        ok("the shell still works after three crashed programs")

        # 12e. The ported musl C library: a real libc program linked
        #      against musl-1.2.6. printf goes through writev, malloc
        #      through mmap, strlen through SSE2 asm (FPU context
        #      switching), getpid through the ABI, TLS/errno through
        #      arch_prctl(ARCH_SET_FS), fopen through openat.
        type_text(sock, "musl_hello\r")
        offset = wait_for("started as pid", offset=offset)
        offset = wait_for("musl 1.2.6 on TUS: hello from libc", offset=offset)
        offset = wait_for("argc=1", offset=offset)
        offset = wait_for("argv0=/bin/musl_hello", offset=offset)
        offset = wait_for("pid=", offset=offset)
        offset = wait_for("malloc: heap string", offset=offset)
        offset = wait_for("free ok", offset=offset)
        offset = wait_for("all good", offset=offset)
        ok("musl libc program runs: printf, malloc, SSE strlen, fopen, getpid")

        # 12f. kilo: a real terminal application linked against musl.
        #      Exercises termios (TCGETS/TCSETS via ioctl), TIOCGWINSZ
        #      window size, raw-mode input with escape sequences, the
        #      ANSI/VT100 output path (cursor, erase, SGR, status bar),
        #      argv forwarding through exec, and file save via
        #      ftruncate + write. kilo itself needed NO source changes.
        type_text(sock, "kilo /kilo.txt\r")
        offset = wait_for("started as pid", offset=offset)
        offset = wait_for("/kilo.txt - 0 lines", offset=offset)
        ok("kilo starts with the filename in the status bar")

        # The editor screen is rendered on the framebuffer (tildes +
        # status bar lit pixels), not just on the serial log.
        time.sleep(1.0)
        screendump(sock, "/tmp/tus-kilo0.ppm")
        kilo_lit = count_nonblack_pixels("/tmp/tus-kilo0.ppm")
        assert kilo_lit > 5000, \
            f"kilo did not render on the framebuffer ({kilo_lit} px)"
        ok(f"kilo renders the editor screen on the framebuffer ({kilo_lit} px)")

        # Type text, then save with Ctrl-S (raw mode: the bytes reach
        # the editor, not the shell's line editor).
        type_text(sock, "hello")
        offset = wait_for("/kilo.txt - 1 lines (modified)", offset=offset)
        sendkey(sock, "ctrl-s")
        offset = wait_for("bytes written on disk", offset=offset)
        ok("kilo edits and saves via ftruncate + write")

        # Quit with Ctrl-Q; the console keyboard must return to the
        # shell, which can then read the saved file back.
        sendkey(sock, "ctrl-q")
        time.sleep(1.0)
        type_text(sock, "cat /kilo.txt\r")
        offset = wait_for("hello", offset=offset)
        ok("kilo-saved file reads back through the shell")

        # 12g. UNIX tools: grep (BRE/ERE engine, options).
        type_text(sock, "echo hello world > /tmp/t.txt\r")
        type_text(sock, "grep world /tmp/t.txt\r")
        offset = wait_for("hello world", offset=offset)
        type_text(sock, "grep -n -i HELLO /tmp/t.txt\r")
        offset = wait_for("1:hello world", offset=offset)
        type_text(sock, "grep -c o /tmp/t.txt\r")
        offset = wait_for("1\n", offset=offset)
        type_text(sock, "grep -v nope /tmp/t.txt\r")
        offset = wait_for("hello world", offset=offset)
        ok("grep searches lines: plain, -n -i, -c, -v")

        # 12h. sed (stream editor).
        type_text(sock, "sed s/world/tus/ /tmp/t.txt\r")
        offset = wait_for("hello tus", offset=offset)
        type_text(sock, "sed -n 1p /etc/motd\r")
        offset = wait_for("Welcome to TUS", offset=offset)
        ok("sed substitutes and prints with addresses")

        # 12i. useradd: real /etc/passwd + /etc/group + home dir.
        type_text(sock, "useradd -m -s /bin/tsh ahmet\r")
        offset = wait_for("user ahmet added (uid 1000", offset=offset)
        type_text(sock, "cat /etc/passwd\r")
        offset = wait_for("ahmet:x:1000:1000::/home/ahmet:/bin/tsh", offset=offset)
        ok("useradd creates the account in /etc/passwd")

        # 12j. doas: config check, PATH lookup, execve to root command.
        type_text(sock, "doas useradd -m veli\r")
        offset = wait_for("user veli added", offset=offset)
        type_text(sock, "cat /etc/passwd\r")
        offset = wait_for("veli:x:1001:1001::/home/veli", offset=offset)
        ok("doas runs a command as root (execve + /bin PATH)")

        # 12k. passwd: crypt() hashing into /etc/shadow, then login
        #      authenticates against it (echo-off password prompt).
        type_text(sock, "passwd ahmet\r")
        offset = wait_for("New password:", offset=offset)
        type_text(sock, "test123\r")
        offset = wait_for("Retype new password:", offset=offset)
        type_text(sock, "test123\r")
        offset = wait_for("password for ahmet updated", offset=offset)
        type_text(sock, "passwd -S ahmet\r")
        offset = wait_for("ahmet P", offset=offset)
        ok("passwd hashes and stores the password (status P)")

        # 12l. login: verifies /etc/shadow and starts the session.
        type_text(sock, "login ahmet\r")
        offset = wait_for("Password:", offset=offset)
        type_text(sock, "test123\r")
        offset = wait_for("Welcome to TUS, ahmet!", offset=offset)
        offset = wait_for("uid=1000", offset=offset)
        offset = wait_for("Work everywhere", offset=offset)
        ok("login authenticates and prints the motd")

        # 12m. cd - (OLDPWD) and mkdir -p.
        type_text(sock, "cd /tmp\r")
        type_text(sock, "cd -\r")
        offset = wait_for("/tmp\n", offset=offset)
        type_text(sock, "mkdir -p /tmp/a/b/c\r")
        type_text(sock, "ls /tmp/a/b\r")
        offset = wait_for("c", offset=offset)
        type_text(sock, "cd /\r")
        ok("cd - returns to the previous directory, mkdir -p creates parents")

        # 12n. tsh v2.0: pipes and I/O redirection. The shell rewires
        #      its own fd table (per-task, inherited at spawn), so
        #      `a | b`, `> file`, `>> file` and `< file` work exactly
        #      like a real UNIX shell. /bin/echo writes through fd 1,
        #      which is what makes it a usable pipeline producer.
        type_text(sock, "echo hello pipe world | grep -c pipe\r")
        offset = wait_for("1\n", offset=offset)
        type_text(sock, "echo a1 | sed s/a/b/ | grep -c b1\r")
        offset = wait_for("1\n", offset=offset)
        ok("pipes chain stages (echo | grep, echo | sed | grep)")

        type_text(sock, "echo line1 > /tmp/p.txt\r")
        type_text(sock, "grep -c line1 /tmp/p.txt\r")
        offset = wait_for("1\n", offset=offset)
        type_text(sock, "echo line2 >> /tmp/p.txt\r")
        type_text(sock, "grep -c line /tmp/p.txt\r")
        offset = wait_for("2\n", offset=offset)
        ok("> truncates and >> appends (both verified via grep -c)")

        type_text(sock, "sed -n p < /tmp/p.txt | grep -c line2\r")
        offset = wait_for("1\n", offset=offset)
        ok("stdin redirection feeds a pipeline")

        type_text(sock, "grep zzz /nonexistent 2> /tmp/e.txt\r")
        type_text(sock, "cat /tmp/e.txt\r")
        offset = wait_for("grep: /nonexistent: No such file", offset=offset)
        ok("2> redirects stderr to a file")

        # 12o. Unix domain sockets + poll/select. socktest is a musl
        #      program that drives the whole surface from ring 3:
        #      socketpair, bind/listen/connect/accept over a filesystem
        #      path, send/recv, shutdown, getsockname, then poll() and
        #      select() over sockets and pipes (readiness, hang-up,
        #      timeouts, EBADF). It prints one "ok:" line per check and
        #      "all good" only if every one passed.
        type_text(sock, "socktest\r")
        offset = wait_for("socktest: AF_UNIX sockets", offset=offset)
        offset = wait_for("ok: socketpair is bidirectional", offset=offset)
        offset = wait_for("ok: connect", offset=offset)
        offset = wait_for("ok: accept", offset=offset)
        offset = wait_for("ok: server recv sees", offset=offset)
        offset = wait_for("ok: half close gives the peer EOF", offset=offset)
        offset = wait_for("ok: connect to a stale socket path is refused",
                          offset=offset)
        offset = wait_for("ok: poll: socket with data is readable",
                          offset=offset)
        offset = wait_for("ok: poll: pipe with no writers reports POLLHUP",
                          offset=offset)
        offset = wait_for("ok: select: reports only the ready fd of two",
                          offset=offset)
        offset = wait_for("ok: select: times out on an idle socket",
                          offset=offset)
        offset = wait_for("socktest: all good", offset=offset)
        ok("AF_UNIX sockets, poll() and select() work from ring 3")

        # 12p. Run it a second time. This is the leak check: the fd
        #      table is only 16 slots and the socket node stays in the
        #      filesystem until unlink(), so a missed close or a
        #      leaked node would make the second run fail at bind()
        #      or run out of descriptors.
        type_text(sock, "socktest\r")
        offset = wait_for("socktest: all good", offset=offset)
        ok("a second socktest run passes (no fd or socket-node leak)")

        # 13. scrollback: overflow the screen, PageUp shows older
        #     lines, PageDown returns to the exact live view.
        #     (Runs BEFORE fbfill: that test paints the screen and
        #     would leave white remnants that skew the pixel counts.)
        for _ in range(3):
            type_text(sock, "help\r")
        time.sleep(1.5)  # let all output settle (TCG is slow)
        live_lit = wait_for_stable_screen(sock, "/tmp/tus-live.ppm")
        sendkey(sock, "pgup")
        time.sleep(0.25)
        sendkey(sock, "pgup")
        time.sleep(0.25)
        sendkey(sock, "pgup")
        back_lit = wait_for_stable_screen(sock, "/tmp/tus-back.ppm")
        assert back_lit != live_lit, \
            f"PageUp did not change the screen ({live_lit} == {back_lit})"
        sendkey(sock, "pgdn")
        time.sleep(0.25)
        sendkey(sock, "pgdn")
        time.sleep(0.25)
        sendkey(sock, "pgdn")
        restored_lit = wait_for_stable_screen(sock, "/tmp/tus-restored.ppm")
        assert restored_lit == live_lit, \
            f"PageDown did not restore the live view ({restored_lit} != {live_lit})"
        ok(f"scrollback: PageUp/PageDown navigate history "
           f"({live_lit} live, {back_lit} back, {restored_lit} restored)")

        # 14. fbfill paints the whole framebuffer white (ioctl). /dev/fb0
        #     is mode 0600 root:root, and the session is "ahmet" (non-
        #     root, and not in :wheel either) since the login test
        #     above - real session-identity enforcement (see the
        #     vfs_access_ok fix) now actually applies here, and ahmet
        #     has no doas.conf rule, so this logs back in as root
        #     first (password "toast", same as the initial console
        #     login) rather than trying doas.
        type_text(sock, "login root\r")
        offset = wait_for("Password:", offset=offset)
        type_text(sock, "toast\r")
        offset = wait_for("Welcome to TUS, root!", offset=offset)
        type_text(sock, "fbfill ffffff\r")
        offset = wait_for("filled with #ffffff", offset=offset)
        screendump(sock)
        white = count_white_pixels(SCREEN_PPM)
        assert white > 900000, f"fbfill did not paint the screen ({white} white)"
        ok(f"fbfill fills the framebuffer via /dev/fb0 ioctl ({white} px)")

        # 15. unknown command error path
        type_text(sock, "nosuchcmd\r")
        offset = wait_for("command not found", offset=offset)
        ok("unknown command produces an error")

        # 16. crash -> kernel panic handler with register dump
        type_text(sock, "crash\r")
        offset = wait_for("KERNEL PANIC", offset=offset)
        offset = wait_for("Invalid Opcode", offset=offset)
        wait_for("System halted", offset=offset)
        ok("crash triggers the panic handler with a register dump")

        print(f"\nALL {PASS} TESTS PASSED")
        return 0
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            proc.kill()


if __name__ == "__main__":
    sys.exit(main())
