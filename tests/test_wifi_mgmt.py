#!/usr/bin/env python3
"""
test_wifi_mgmt.py - boot-verify the 802.11 management frame layer

Runs `wifimgmtselftest` in a real booted kernel and checks every
sub-check passed. This is the frame-layer sibling of wpaselftest
(the crypto core): see kernel/net/wifi_mgmt.h for why this is a
byte-level round-trip test and not a real over-the-air association -
QEMU has no 802.11 hardware to associate with.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import test_boot  # noqa: E402
from test_boot import (  # noqa: E402
    qmp_connect, sendkey, type_text, wait_for, ok, start_qemu,
    SERIAL_LOG, BOOT_TIMEOUT,
)


def boot_to_shell():
    proc = start_qemu()
    offset = wait_for("tsh ready", timeout=BOOT_TIMEOUT)
    sock = qmp_connect()

    try:
        wait_for("graphics test be performed", timeout=15, offset=offset)
        sendkey(sock, "n")
        sendkey(sock, "ret")
    except AssertionError:
        pass

    try:
        wait_for("login:", timeout=15, offset=offset)
        type_text(sock, "root\r")
        wait_for("Password:", offset=offset, timeout=15)
        type_text(sock, "toast\r")
    except AssertionError:
        pass

    offset = wait_for("tus:/>", offset=offset)
    return proc, sock, offset


def main():
    print("== 802.11 management frame test ==")
    proc, sock, offset = boot_to_shell()
    try:
        type_text(sock, "wifimgmtselftest\r")
        offset = wait_for("selftest:", offset=offset, timeout=15)
        with open(SERIAL_LOG, "rb") as f:
            f.seek(max(0, offset - 1600))
            tail = f.read().decode("utf-8", "replace")

        assert "FAIL" not in tail, f"a wifi_mgmt selftest check failed:\n{tail}"
        assert "all checks passed" in tail, f"selftest did not report success:\n{tail}"
        ok("wifimgmtselftest: all 802.11 frame build/parse checks passed")

        print(f"\n{test_boot.PASS} checks passed")
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=5)
        except Exception:
            proc.kill()


if __name__ == "__main__":
    main()
