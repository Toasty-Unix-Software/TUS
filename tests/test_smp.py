#!/usr/bin/env python3
"""
test_smp.py - boot-level check that ACPI/MADT CPU enumeration works.

Boots tus.iso and runs the `cpuinfo` shell command, which reports the
CPU list smp_init() built from the MADT's Processor Local APIC
entries. TUS still executes only on the boot CPU (no AP trampoline),
so this checks topology discovery, not parallel execution: at least
one CPU entry, marked enabled and BSP/running.
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

        type_text(sock, "cpuinfo\r")
        offset = wait_for("CPUs        :", offset=offset, timeout=15)
        test_boot.ok("cpuinfo reports a CPU count from ACPI/MADT")

        offset = wait_for("(BSP, running)", offset=offset, timeout=15)
        test_boot.ok("cpuinfo identifies the boot CPU")

        print(f"\n{test_boot.PASS} check(s) passed")
    finally:
        qemu.terminate()
        try:
            qemu.wait(timeout=5)
        except Exception:
            qemu.kill()


if __name__ == "__main__":
    main()
