#!/usr/bin/env python3
"""
test_ipv6.py - IPv6: SLAAC link-local addressing, NDP and ICMPv6

Boots tus.iso behind QEMU's user-mode (slirp) network, which acts as
an on-link IPv6 router at fec0::2 (QEMU's documented default), and
checks:

  1. TUS derives a link-local address (fe80::/64, modified EUI-64)
     from the NIC's MAC at boot and prints it - kernel/net/ipv6.c's
     ipv6_init().
  2. `ifconfig` reports that address.
  3. `ping6 fec0::2` gets real replies: this exercises Neighbor
     Solicitation/Advertisement (to resolve the gateway's MAC),
     ICMPv6 Echo Request/Reply, and the receive-side dispatch
     (eth_input -> ipv6_input -> icmpv6_input) end to end - not just
     that a packet went out.

What this does NOT check: SLAAC global address configuration (RA
Prefix Information), because QEMU's slirp does not appear to answer
Router Solicitation with a Router Advertisement in this QEMU version
(no RA was observed on the wire in manual testing) - that code path
(handle_ra() in kernel/net/ipv6.c) exists but is unexercised by any
router this test harness has available. See that file's header for
the rest of what is and is not in scope.

Usage: python3 tests/test_ipv6.py   (from the project root)
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
         "-pidfile", "/tmp/tus-qemu-ipv6.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


def main():
    proc = start_qemu()
    try:
        offset = wait_for(b"link-local", BOOT_TIMEOUT)
        ok("kernel derives and prints a link-local address at boot")

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

        type_text(sock, "ifconfig\r")
        offset = wait_for(b"Scope:Link", 15, offset)
        ok("ifconfig reports the link-local address")

        type_text(sock, "ping6 fec0::2 -c 2\r")
        offset = wait_for(b"0% packet loss", 15, offset)
        ok("ping6 to the on-link router gets real ICMPv6 Echo Replies "
           "(proves NDP resolution and the receive path)")

        type_text(sock, "exit\r")
        time.sleep(0.5)
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            proc.kill()

    print("\nAll IPv6 tests passed.")


if __name__ == "__main__":
    main()
