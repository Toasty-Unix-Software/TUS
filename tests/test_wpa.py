#!/usr/bin/env python3
"""
test_wpa.py - WPA2-PSK crypto core (kernel/net/wpa_crypto.c)

Boots tus.iso and runs `wpaselftest`, which checks SHA-1, HMAC-SHA1
and PBKDF2-HMAC-SHA1 against their published test vectors (FIPS
180-1, RFC 2202, RFC 6070) and then exercises the full PSK -> PTK ->
EAPOL-MIC pipeline for determinism.

Scope note (see kernel/net/wpa_crypto.h's header comment): this is the
crypto core only. Actually joining a WPA2 network needs 802.11
management frames and the ath9k-htc USB command protocol, neither of
which exists yet, and there is no way to boot-test a real handshake
in this environment (QEMU has no ath9k-htc emulation and no virtual
AP) - so that part isn't attempted or claimed done here.

Usage: python3 tests/test_wpa.py   (from the project root)
"""

import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from test_boot import (  # noqa: E402
    qmp_connect, sendkey, type_text, wait_for, ok,
    SERIAL_LOG, QEMU_LOG, QMP_SOCK,
)

BOOT_TIMEOUT = 90


def start_qemu():
    for stale in (SERIAL_LOG, QEMU_LOG, QMP_SOCK):
        try:
            os.unlink(stale)
        except FileNotFoundError:
            pass
    return subprocess.Popen(
        ["qemu-system-x86_64", "-cdrom", "tus.iso", "-m", "512M",
         "-display", "none", "-no-reboot",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-qemu-wpa.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


def main():
    proc = start_qemu()
    try:
        sock = qmp_connect()
        offset = 0

        try:
            offset = wait_for(b"graphics test be performed", 15, offset)
            sendkey(sock, "n")
            sendkey(sock, "ret")
        except AssertionError:
            pass

        try:
            offset = wait_for(b"login:", 30, offset)
            type_text(sock, "root\r")
            offset = wait_for(b"Password:", 15, offset)
            type_text(sock, "toast\r")
        except AssertionError:
            pass

        offset = wait_for(b"tus:/>", 30, offset)

        type_text(sock, "wpaselftest\r")
        offset = wait_for(b"PASS sha1", 15, offset)
        offset = wait_for(b"PASS hmac_sha1", 15, offset)
        offset = wait_for(b"PASS pbkdf2(RFC6070, c=1)", 15, offset)
        offset = wait_for(b"PASS pbkdf2(RFC6070, c=4096)", 15, offset)
        offset = wait_for(b"selftest: all checks passed", 15, offset)
        ok("wpaselftest: SHA-1/HMAC-SHA1/PBKDF2 match published test "
           "vectors, and the PSK->PTK->MIC pipeline is deterministic")

        type_text(sock, "exit\r")
        time.sleep(0.5)
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            proc.kill()

    print("\nAll WPA crypto-core tests passed.")


if __name__ == "__main__":
    main()
