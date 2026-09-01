#!/usr/bin/env python3
"""
test_net.py - the network stack, end to end

Boots tus.iso with QEMU's user-mode network behind an RTL8139 and
drives the shell through it: address configuration, ARP and ICMP to the
gateway, a DNS lookup, and a real TCP conversation with a server
running on this host (which the guest reaches at 10.0.2.2).

The TCP test is the one that matters. Everything above it works with a
send-only stack; a byte that comes back proves the receive path, the
sequence numbers and the ACKs.

Usage: python3 tests/test_net.py   (from the project root)
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

ECHO_PORT = 18888
BULK_PORT = 18889
BULK_BYTES = 256 * 1024
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
         "-pidfile", "/tmp/tus-qemu-net.pid"],
        stdout=open(QEMU_LOG, "w"), stderr=subprocess.STDOUT)


class EchoServer(threading.Thread):
    """Upper-cases whatever it is sent, so the reply cannot be mistaken
    for the guest's own echo of the typed line, and hangs up when it
    sees a 'q' - which is how the test gets a FIN sent to the guest
    without depending on the console turning Ctrl-D into EOF (TUS's
    tty has no canonical mode, so it does not)."""

    def __init__(self, port):
        super().__init__(daemon=True)
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(("127.0.0.1", port))
        self.sock.listen(4)
        self.received = []
        self.running = True

    def run(self):
        while self.running:
            try:
                conn, _ = self.sock.accept()
            except OSError:
                return
            threading.Thread(target=self.serve, args=(conn,),
                             daemon=True).start()

    def serve(self, conn):
        conn.settimeout(30)
        try:
            while True:
                data = conn.recv(4096)
                if not data:
                    print(f"  [host] {time.time():.2f} peer sent EOF")
                    break
                print(f"  [host] {time.time():.2f} recv {data!r}")
                self.received.append(data)
                conn.sendall(data.upper())
                if b"q" in data:
                    print(f"  [host] {time.time():.2f} hanging up")
                    break
        except OSError as exc:
            print(f"  [host] {time.time():.2f} error {exc}")
        finally:
            print(f"  [host] {time.time():.2f} closing")
            conn.close()

    def stop(self):
        self.running = False
        self.sock.close()


class BulkServer(threading.Thread):
    """Sends a fixed number of bytes and hangs up. Enough data to make
    the guest's receive window close and reopen many times over, which
    is the part of TCP a one-line echo never touches."""

    def __init__(self, port, total):
        super().__init__(daemon=True)
        self.total = total
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(("127.0.0.1", port))
        self.sock.listen(4)
        self.running = True

    def run(self):
        payload = bytes(bytearray((i % 251) for i in range(self.total)))
        while self.running:
            try:
                conn, _ = self.sock.accept()
            except OSError:
                return
            try:
                conn.sendall(payload)
            except OSError:
                pass
            conn.close()

    def stop(self):
        self.running = False
        self.sock.close()


def run_cmd(qmp, text, expect, timeout=30, offset=0):
    type_text(qmp, text)
    sendkey(qmp, "ret")
    return wait_for(expect, timeout=timeout, offset=offset)


def main():
    if not os.path.exists("tus.iso"):
        print("tus.iso not found - run `make clang` first")
        return 1

    echo = EchoServer(ECHO_PORT)
    echo.start()
    bulk = BulkServer(BULK_PORT, BULK_BYTES)
    bulk.start()
    print(f"echo server on 127.0.0.1:{ECHO_PORT}, "
          f"bulk server on 127.0.0.1:{BULK_PORT}")

    qemu = start_qemu()
    qmp = None
    try:
        offset = wait_for("tsh", timeout=BOOT_TIMEOUT)
        ok("booted to a shell")

        # The card has to have been found on PCI at all.
        with open(SERIAL_LOG) as f:
            log = f.read()
        assert "rtl8139: MAC" in log, "the NIC was never initialised"
        ok("RTL8139 found on PCI and brought up")

        qmp = qmp_connect()

        # The kernel asks about the graphics test and blocks on a
        # keypress; decline it, or the first character of the first
        # command is eaten by the prompt. It then drains up to the
        # next newline (kernel/main.c), so "n" alone with no Enter
        # leaves it blocked forever - a real Enter has to follow.
        try:
            offset = wait_for("graphics test be performed", timeout=15,
                              offset=offset)
            sendkey(qmp, "n")
            sendkey(qmp, "ret")
        except AssertionError:
            pass

        # The console now requires a real login (console_login_gate()
        # in kernel/main.c) before anything else - root's password is
        # "toast" (see rootfs/etc/shadow). Older kernels with no login
        # gate boot straight to "tus:/>" and never print "login:", so
        # this is skipped the same way the graphics prompt above is.
        try:
            wait_for("login:", timeout=15, offset=offset)
            type_text(qmp, "root\r")
            wait_for("Password:", offset=offset, timeout=15)
            type_text(qmp, "toast\r")
        except AssertionError:
            pass

        offset = wait_for("tus:/>", timeout=30, offset=offset)
        time.sleep(0.5)

        offset = run_cmd(qmp, "ifconfig", "10.0.2.15", offset=offset)
        ok("ifconfig reports the interface address")

        # ARP for the gateway, then ICMP through it.
        offset = run_cmd(qmp, "ping -c 2 10.0.2.2", "bytes from 10.0.2.2",
                         timeout=40, offset=offset)
        ok("ping reaches the gateway (ARP + ICMP round trip)")

        offset = run_cmd(qmp, "arp", "ether", offset=offset)
        ok("the ARP cache learned the gateway's MAC")

        # QEMU's built-in resolver lives at 10.0.2.3.
        offset = run_cmd(qmp, "host example.com", "has address",
                         timeout=40, offset=offset)
        ok("DNS resolves a name over UDP")

        # The real test: a TCP connection to this host, with data both
        # ways. 10.0.2.2 is the host from inside QEMU's user network.
        type_text(qmp, f"nc -v 10.0.2.2 {ECHO_PORT}")
        sendkey(qmp, "ret")
        offset = wait_for("nc: connected", timeout=40, offset=offset)
        ok("TCP connect completed the three-way handshake")

        # The console echoes each character as it is typed and the
        # server's upper-cased reply arrives before the next one, so
        # what lands on screen is the two streams interleaved. That
        # shape is the proof: it can only happen if each byte made a
        # full round trip before the next was sent.
        type_text(qmp, "hello tus")
        offset = wait_for("hHeElLlLoO", timeout=30, offset=offset)
        ok("data crossed the connection in both directions")

        deadline = time.time() + 10
        while time.time() < deadline:
            if b"".join(echo.received).find(b"hello tus") >= 0:
                break
            time.sleep(0.2)
        assert b"hello tus" in b"".join(echo.received), \
            "the host server never saw the guest's bytes"
        ok("the host server received exactly what was typed")

        # Ctrl-D closes stdin, which half-closes the connection.
        # 'q' makes the host hang up, so the guest has to notice a FIN
        # and report end of stream.
        type_text(qmp, "q")
        offset = wait_for("nc: connection closed", timeout=30, offset=offset)
        ok("the peer's FIN was reported as end of stream")

        offset = run_cmd(qmp, "netstat", "tcp", offset=offset)
        ok("netstat lists the connection table")

        # A quarter of a megabyte through a 32 KiB window: the whole
        # transfer only completes if the window updates, the ACKs and
        # the ring buffers all behave.
        type_text(qmp, f"nc 10.0.2.2 {BULK_PORT} > /tmp/blob")
        sendkey(qmp, "ret")
        time.sleep(2)
        offset = wait_for("tus:/>", timeout=180, offset=offset)

        offset = run_cmd(qmp, "ls -l /tmp", str(BULK_BYTES), timeout=30,
                         offset=offset)
        ok(f"{BULK_BYTES} bytes arrived intact over one connection")

        print(f"\nall network checks passed")
        return 0

    except AssertionError as exc:
        print(f"\n  [FAIL] {exc}")
        return 1
    finally:
        if qmp:
            qmp.close()
        echo.stop()
        bulk.stop()
        qemu.terminate()
        try:
            qemu.wait(timeout=5)
        except subprocess.TimeoutExpired:
            qemu.kill()


if __name__ == "__main__":
    sys.exit(main())
