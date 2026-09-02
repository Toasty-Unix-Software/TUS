#!/usr/bin/env python3
"""
test_dhcp.py - DHCPv4 client (kernel/net/dhcp.c)

Boots tus.iso behind QEMU's user-mode (slirp) network, which runs a
real DHCP server on 10.0.2.2 handing out 10.0.2.15 by default, and
checks that `doas dhcp` actually runs DISCOVER/OFFER/REQUEST/ACK
against it and that ifconfig reflects the leased address afterward -
not just that the command returns 0.

Usage: python3 tests/test_dhcp.py   (from the project root)
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
         "-nic", "user,model=rtl8139",
         "-serial", f"file:{SERIAL_LOG}",
         "-qmp", f"unix:{QMP_SOCK},server=on,wait=off",
         "-pidfile", "/tmp/tus-qemu-dhcp.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


def main():
    proc = start_qemu()
    try:
        offset = wait_for(b"network: eth0", BOOT_TIMEOUT)
        ok("kernel brings up eth0 with its static default")

        sock = qmp_connect()

        try:
            offset = wait_for(b"graphics test be performed", 15, offset)
            sendkey(sock, "n")
            sendkey(sock, "ret")
        except AssertionError:
            pass

        try:
            offset = wait_for(b"login:", 15, offset)
            type_text(sock, "root\r")
            offset = wait_for(b"Password:", 15, offset)
            type_text(sock, "toast\r")
        except AssertionError:
            pass

        offset = wait_for(b"tus:/>", 20, offset)

        # Change the static address first, so a successful lease
        # afterward can only be explained by a real DHCP exchange
        # having overwritten it - not by the boot default already
        # matching.
        type_text(sock, "ifconfig eth0 10.0.2.77 netmask 255.255.255.0\r")
        offset = wait_for(b"10.0.2.77", 15, offset)
        ok("ifconfig sets a throwaway static address first")

        type_text(sock, "dhcp\r")
        offset = wait_for(b"dhcp: leased 10.0.2.15", 15, offset)
        ok("doas dhcp completes DISCOVER/OFFER/REQUEST/ACK against "
           "slirp's DHCP server and installs the leased address")

        type_text(sock, "ifconfig\r")
        offset = wait_for(b"inet addr:10.0.2.15", 15, offset)
        offset = wait_for(b"Mask:255.255.255.0", 15, offset)
        ok("ifconfig reflects the leased address and netmask")

        type_text(sock, "exit\r")
        time.sleep(0.5)
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            proc.kill()

    print("\nAll DHCP tests passed.")


if __name__ == "__main__":
    main()
