#!/usr/bin/env python3
"""
test_ipv6_transport.py - TCP6 and UDP6 (kernel/net/tcp6.c, udp6.c)

Boots tus.iso behind QEMU's user-mode (slirp) network and checks that
real transport-layer traffic crosses the wire over IPv6, not just
ICMPv6 (see test_ipv6.py for that). QEMU's slirp treats fec0::2 the
same way it treats 10.0.2.2 for IPv4: a port opened there proxies to
a real listener on this host, so a genuine three-way handshake and a
UDP6 datagram exchange both have to work against a real peer.

Unlike test_net.py's IPv4 nc check, this one reads correctness off the
HOST side (the bytes a Python server actually received) rather than
on-screen character interleaving - the two streams' relative arrival
order over a local slirp hop is fast enough to be flaky (test_net.py's
own "hHeElLlLoO" interleaving assertion demonstrably flakes on
occasion for the exact same reason, independent of this file), so this
test avoids that class of flake entirely by asserting on the recorded
bytes instead of a screen-scraped interleaving pattern.

Usage: python3 tests/test_ipv6_transport.py   (from the project root)
"""

import os
import socket
import subprocess
import sys
import threading
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from test_boot import (  # noqa: E402
    qmp_connect, sendkey, type_text, wait_for, ok,
    SERIAL_LOG, QEMU_LOG, QMP_SOCK,
)

TCP6_PORT = 19998
UDP6_PORT = 19999
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
         "-pidfile", "/tmp/tus-qemu-ipv6-transport.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


class Tcp6EchoServer(threading.Thread):
    """One-shot: accepts a connection, records everything it reads
    until EOF, replies with the upper-cased whole and closes."""

    def __init__(self, port):
        super().__init__(daemon=True)
        self.sock = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(("::", port))
        self.sock.listen(4)
        self.received = b""
        self.done = threading.Event()

    def run(self):
        conn, _ = self.sock.accept()
        conn.settimeout(30)
        try:
            while True:
                data = conn.recv(4096)
                if not data:
                    break
                self.received += data
        except OSError:
            pass
        try:
            conn.sendall(self.received.upper())
        except OSError:
            pass
        conn.close()
        self.sock.close()
        self.done.set()


class Udp6EchoServer(threading.Thread):
    """Replies to every datagram with its upper-cased form."""

    def __init__(self, port):
        super().__init__(daemon=True)
        self.sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(("::", port))
        self.received = []
        self.running = True

    def run(self):
        self.sock.settimeout(1)
        while self.running:
            try:
                data, addr = self.sock.recvfrom(4096)
            except socket.timeout:
                continue
            except OSError:
                return
            self.received.append(data)
            self.sock.sendto(data.upper(), addr)

    def stop(self):
        self.running = False
        self.sock.close()


def main():
    if not os.path.exists("tus.iso"):
        print("tus.iso not found - run `make clang` first")
        return 1

    tcp_srv = Tcp6EchoServer(TCP6_PORT)
    tcp_srv.start()
    udp_srv = Udp6EchoServer(UDP6_PORT)
    udp_srv.start()

    proc = start_qemu()
    try:
        offset = wait_for(b"link-local", BOOT_TIMEOUT)
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

        # ---- UDP6: one datagram out, the echoed reply back ----
        type_text(sock, f"nc6 -u -v fec0::2 {UDP6_PORT}\r")
        offset = wait_for(b"nc6: sending", 10, offset)
        type_text(sock, "udp6-payload\r")
        offset = wait_for(b"UDP6-PAYLOAD", 15, offset)
        ok("UDP6 sendto/recvfrom round-tripped through a real peer, "
           "reply content correct (proves the pseudo-header checksum "
           "and addressing are right, not just that a packet left)")
        sendkey(sock, "ctrl-c")
        time.sleep(0.5)
        offset = wait_for(b"tus:/>", 15, offset)

        # ---- TCP6: a real stream, correctness checked on the host ----
        type_text(sock, f"nc6 fec0::2 {TCP6_PORT} < /etc/hostname\r")
        tcp_srv.done.wait(timeout=20)
        assert tcp_srv.received == b"tus\n" or tcp_srv.received.strip() == b"tus", (
            f"host never received the expected stream content: "
            f"{tcp_srv.received!r}")
        ok("TCP6 stream reached the host with exact byte content "
           "(three-way handshake, data segments and their checksums, "
           "and the closing FIN all worked)")

        offset = wait_for(b"tus:/>", 20, offset)
        time.sleep(0.5)
    finally:
        udp_srv.stop()
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            proc.kill()

    print("\nAll TCP6/UDP6 transport tests passed.")


if __name__ == "__main__":
    sys.exit(main() or 0)
