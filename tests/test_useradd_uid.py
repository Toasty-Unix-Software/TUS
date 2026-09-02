#!/usr/bin/env python3
"""
test_useradd_uid.py - useradd assigns distinct, correctly incrementing
uids to consecutively created users. Regression test for a bug where
max_field() was passed the /etc/passwd username field (0) instead of
the uid field (2), so atoi() on a name always yielded 0 and every new
user landed on the same uid (1000).
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

        for name in ("alice", "bob", "carol"):
            type_text(sock, "useradd %s\r" % name)
            offset = wait_for("tus:/>", offset=offset, timeout=15)

        type_text(sock, "cat /etc/passwd\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        with open(test_boot.SERIAL_LOG, "r", errors="replace") as f:
            full = f.read()

        uids = {}
        for line in full.splitlines():
            for name in ("alice", "bob", "carol"):
                if line.startswith(name + ":"):
                    fields = line.split(":")
                    if len(fields) >= 3:
                        uids[name] = fields[2]

        assert set(uids.keys()) == {"alice", "bob", "carol"}, \
            "expected all three users in /etc/passwd output, got %r" % uids

        ints = {name: int(uid) for name, uid in uids.items()}
        assert len(set(ints.values())) == 3, \
            "uids are not distinct: %r (bug: max_field() read the wrong column)" % ints
        assert ints["alice"] == 1000, ints
        assert ints["bob"] == 1001, ints
        assert ints["carol"] == 1002, ints

        print("OK: alice=%d bob=%d carol=%d (all distinct, correctly incrementing)"
              % (ints["alice"], ints["bob"], ints["carol"]))
    finally:
        qemu.terminate()


if __name__ == "__main__":
    main()
