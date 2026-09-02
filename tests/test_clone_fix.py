#!/usr/bin/env python3
"""
test_clone_fix.py - direct boot-level check that musl's posix_spawn()
-> __clone() path works on TUS.

This is the root-cause regression test for the ksh `clear` Invalid
Opcode crash: `clonetest` (tests/test_clone_fix.c) calls posix_spawn()
directly, which is exactly the code path ksh's job control used to
run any external command. Before the fix, musl's x86_64 clone.s used
the raw `syscall` instruction (unimplemented on TUS, which traps
through `int $0x80`/`$0x81`) and crashed with Invalid Opcode the
moment any program - not just ksh - called posix_spawn, vfork, or
pthread_create.
"""

import sys

sys.path.insert(0, "tests")
import test_boot  # noqa: E402
from test_boot import start_qemu, qmp_connect, type_text, wait_for  # noqa: E402


def main():
    qemu = start_qemu()
    try:
        sock = qmp_connect()

        offset = wait_for("tsh ready", timeout=60)
        offset = wait_for("graphics test be performed", offset=offset,
                          timeout=15)
        type_text(sock, "n\r")
        try:
            wait_for("login:", timeout=15, offset=offset)
            type_text(sock, "root\r")
            wait_for("Password:", offset=offset, timeout=15)
            type_text(sock, "toast\r")
        except AssertionError:
            pass
        offset = wait_for("tus:/>", offset=offset)

        run_start = offset
        type_text(sock, "clonetest\r")
        offset = wait_for("after spawn", offset=offset, timeout=15)

        with open(test_boot.SERIAL_LOG, "rb") as f:
            f.seek(run_start)
            since_run = f.read().decode("utf-8", "replace")

        assert "Invalid Opcode" not in since_run, \
            "posix_spawn -> __clone crashed (Invalid Opcode regression)"
        assert "CLONE_TEST: before spawn" in since_run
        assert "CLONE_TEST: child ran via posix_spawn" in since_run, \
            "child process never actually ran"
        assert "CLONE_TEST: after spawn" in since_run, \
            "parent never got the child's exit status back"

        offset = wait_for("tus:/>", offset=offset, timeout=15)
        print("PASS: posix_spawn()/__clone() works, no Invalid Opcode fault")
        return 0
    finally:
        qemu.terminate()


if __name__ == "__main__":
    sys.exit(main())
