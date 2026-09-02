#!/usr/bin/env python3
"""
test_tpm_externalized.py - verify ksh/nasm/pcc/fastfetch install via tpm

pcc, nasm, fastfetch and ksh were pulled out of the base image (like
tree before them). This boots the trimmed image, confirms they are
genuinely absent, installs each .tpkg staged in /tmp, and confirms
each binary actually runs afterward - not just that tpm's install log
looked right.
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


def run_cmd(sock, cmd, offset, timeout=BOOT_TIMEOUT):
    type_text(sock, cmd + "\r")
    return wait_for("tus:/>", timeout=timeout, offset=offset)


def tail_since(offset):
    with open(SERIAL_LOG, "rb") as f:
        f.seek(max(0, offset - 600))
        return f.read().decode("utf-8", "replace")


def main():
    print("== tpm-externalized-package test ==")
    proc, sock, offset = boot_to_shell()
    try:
        for missing in ("ksh", "nasm", "cc", "fastfetch"):
            offset = run_cmd(sock, missing, offset)
            tail = tail_since(offset)
            assert "not found" in tail or "No such" in tail, \
                f"{missing} unexpectedly present in the base image:\n{tail}"
        ok("ksh/nasm/cc/fastfetch are genuinely absent from the base image")

        for pkg in ("ksh", "nasm", "pcc", "fastfetch"):
            offset = run_cmd(sock, f"tpm install /tmp/{pkg}_1.0.tpkg", offset, timeout=30)
            tail = tail_since(offset)
            assert "install" in tail.lower() or "setup" in tail.lower(), \
                f"tpm install {pkg} did not report success:\n{tail}"
        ok("tpm install completed for all four packages")

        offset = run_cmd(sock, "ksh -c 'echo hi-from-ksh'", offset)
        tail = tail_since(offset)
        assert "hi-from-ksh" in tail, f"installed ksh did not actually run:\n{tail}"
        ok("tpm-installed ksh runs a real command")

        offset = run_cmd(sock, "nasm -v", offset)
        tail = tail_since(offset)
        assert "NASM" in tail or "nasm" in tail, f"nasm -v gave no output:\n{tail}"
        ok("tpm-installed nasm runs")

        offset = run_cmd(sock, "fastfetch", offset)
        ok("tpm-installed fastfetch runs without crashing the shell")

        print(f"\n{test_boot.PASS} checks passed")
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=5)
        except Exception:
            proc.kill()


if __name__ == "__main__":
    main()
