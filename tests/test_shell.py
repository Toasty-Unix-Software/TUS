#!/usr/bin/env python3
"""
test_shell.py - shell behaviour of tsh, in a window and at the console

Both TUS shells are expected to behave the way a UNIX user types:
quotes group arguments, `>` and `>>` and `<` redirect, `|` builds a
pipeline out of built-ins and programs alike, and the command history
is reachable with the `history` command and the Up/Down keys.

tsh is checked on the serial log directly. hxterm draws into a highX
window, so its results are checked through their side effects: the
files it writes are read back with tsh once the session ends.

Usage: python3 tests/test_shell.py   (from the project root)
"""

import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from test_boot import (qmp_cmd, qmp_connect, sendkey, start_qemu, type_text,
                       wait_for)

PASS = 0


def ok(name):
    global PASS
    PASS += 1
    print(f"  [PASS] {name}")


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"
    print("== TUS shell test (tsh + hxterm) ==")
    proc = start_qemu()

    try:
        offset = wait_for("tsh ready", timeout=90)
        sock = qmp_connect()
        time.sleep(2)
        sendkey(sock, "n")
        offset = wait_for("tus:/>", offset=offset)
        ok("kernel boots")

        # ---- tsh ----

        # 1. Quoting: the quotes group the words and are removed.
        type_text(sock, 'echo "hello tus" > /tmp/q.txt\r')
        time.sleep(1)
        type_text(sock, "cat /tmp/q.txt\r")
        offset = wait_for("hello tus", offset=offset)
        ok("tsh: echo \"two words\" > file writes the text without quotes")

        # 2. Appending.
        type_text(sock, "echo second >> /tmp/q.txt\r")
        time.sleep(1)
        type_text(sock, "cat /tmp/q.txt\r")
        offset = wait_for("second", offset=offset)
        ok("tsh: >> appends to the file")

        # 3. A built-in feeding a program: ls now writes to fd 1, so the
        #    pipe carries its output.
        type_text(sock, "ls /bin | grep hxterm\r")
        offset = wait_for("hxterm", offset=offset)
        ok("tsh: `ls /bin | grep hxterm` pipes a built-in into a program")

        # 4. Input redirection into a program.
        type_text(sock, "grep -i -n toast < /etc/motd\r")
        offset = wait_for("1:Welcome to TUS", offset=offset)
        ok("tsh: `< file` feeds a program's standard input")

        # 5. The history command lists what was run.
        type_text(sock, "history\r")
        offset = wait_for("1  echo \"hello tus\" > /tmp/q.txt", offset=offset)
        ok("tsh: `history` lists the command lines")

        # 6. The Up key recalls; Enter runs it again.
        type_text(sock, "echo marker-one\r")
        offset = wait_for("marker-one", offset=offset)
        sendkey(sock, "up")
        time.sleep(0.5)
        sendkey(sock, "ret")
        offset = wait_for("marker-one", offset=offset)
        ok("tsh: Up recalls the previous line and it runs again")

        # 7. Walking further back and returning with Down.
        sendkey(sock, "up")
        sendkey(sock, "up")
        sendkey(sock, "down")
        time.sleep(0.5)
        sendkey(sock, "ret")
        offset = wait_for("marker-one", offset=offset)
        ok("tsh: Up/Down walk the history in both directions")

        # ---- the terminal window ----
        #
        # The window a highX session opens is hxtsh, which has no
        # shell of its own: what answers here is the same tsh that
        # answered above, reached through a terminal session.

        type_text(sock, "highx\r")
        offset = wait_for("/bin/hxtsh started", offset=offset)
        time.sleep(5)

        # 8. Quoting and redirection inside the window.
        type_text(sock, 'echo "gui shell" > /tmp/hx1.txt\r')
        time.sleep(2)
        # 9. A pipeline whose result is redirected into a file.
        type_text(sock, "ls /bin | grep hxvideo > /tmp/hx2.txt\r")
        time.sleep(4)
        # 10. History: two Ups reach the echo again, Enter re-runs it,
        #     so the file ends up with the line twice.
        type_text(sock, "echo again >> /tmp/hx1.txt\r")
        time.sleep(2)
        sendkey(sock, "up")
        time.sleep(0.5)
        sendkey(sock, "ret")
        time.sleep(2)

        # hxterm still ships (a terminal with its own shell in it, and
        # the only one that works without terminal sessions); start it
        # from the window and let its shell write a file too. tusWM
        # focuses the new window, so the typing lands in hxterm.
        type_text(sock, "hxterm\r")
        offset = wait_for("/bin/hxterm started", offset=offset)
        time.sleep(4)
        type_text(sock, 'echo "own shell" > /tmp/hx3.txt\r')
        time.sleep(2)

        sendkey(sock, "ctrl-q")
        offset = wait_for("highx: session ended", offset=offset, timeout=30)
        ok("the terminal window ran a session's worth of commands")

        type_text(sock, "cat /tmp/hx1.txt\r")
        offset = wait_for("gui shell", offset=offset)
        ok("window: quoting and `>` wrote the file")

        offset = wait_for("again", offset=offset)
        offset = wait_for("again", offset=offset)
        ok("window: Up re-ran the append, so the line is there twice")

        type_text(sock, "cat /tmp/hx2.txt\r")
        offset = wait_for("hxvideo", offset=offset)
        ok("window: `ls | grep ... > file` piped and redirected")

        type_text(sock, "cat /tmp/hx3.txt\r")
        offset = wait_for("own shell", offset=offset)
        ok("hxterm's own shell still runs commands in its own window")

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
