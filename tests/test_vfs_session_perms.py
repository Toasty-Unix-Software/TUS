#!/usr/bin/env python3
"""
test_vfs_session_perms.py - proves the shell's own file operations
(redirection, mkdir, ...) are checked against the real logged-in
session, not the kernel shell's own permanently-root task identity.

This is the regression test for a genuine local privilege-escalation
hole found while auditing the security-hardening pass: vfs_access_ok()
checked the CALLING TASK's euid/egid, but the kernel shell (tsh) is
itself a ring-0 task that is permanently euid 0 - it never execs to
become the logged-in user, it just displays their identity and
threads it through exec'd children (doas fix, commit e43abbe1).
Nothing checked THAT identity for operations tsh performs directly
without spawning a task - `>`/`>>` redirection chief among them. Any
non-root login could therefore write ANY file - including
self-granting itself root-level capabilities via
`echo ... >> /etc/capabilities`, or corrupting /etc/passwd - just by
using shell redirection instead of a command that execs.

Two related, previously-exposed bugs are covered in the same boot:
- vfs_create_dir()/vfs_create_file() left new nodes owned by
  root:root (memset default) regardless of who actually created
  them, and vfs_mkdir()/vfs_open()'s O_CREAT path each had their own
  duplicate (and buggy, same-task-identity) ownership-assignment code
  that clobbered the correct value moments after; this meant even a
  legitimate mkdir *inside your own home directory* failed once
  permissions were enforced for real.
- the root VFS node ("/") itself had mode 0 (uninitialized), which
  is invisible under euid-0-bypasses-everything but blocks real
  non-root path traversal (e.g. `cd -` back to "/").
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

        type_text(sock, "useradd -m mallory\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)
        type_text(sock, "passwd mallory\r")
        wait_for("New password:", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        wait_for("Retype", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        type_text(sock, "login\r")
        wait_for("login:", offset=offset, timeout=15)
        type_text(sock, "mallory\r")
        wait_for("Password:", offset=offset, timeout=15)
        type_text(sock, "toast\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)

        # --- attack: self-grant a capability via shell redirection,
        # a builtin the shell performs directly without spawning a
        # task. Must be refused. ---
        type_text(sock, "echo 1000:0xf >> /etc/capabilities\r")
        offset = wait_for("/etc/capabilities: cannot open", offset=offset,
                          timeout=15)

        # --- attack: corrupt /etc/passwd the same way. ---
        type_text(sock, "echo pwned >> /etc/passwd\r")
        offset = wait_for("/etc/passwd: cannot open", offset=offset,
                          timeout=15)

        # --- legitimate: mallory can still create and use nested
        # directories inside her OWN home directory - proves the fix
        # didn't just lock everyone out of everything. ---
        type_text(sock, "mkdir -p /home/mallory/x/y/z\r")
        offset = wait_for("tus:/>", offset=offset, timeout=15)
        type_text(sock, "ls /home/mallory/x/y\r")
        offset = wait_for("z", offset=offset, timeout=15)

        print("PASS: shell-builtin file operations (redirection, mkdir) "
              "are checked against the real session identity, not the "
              "shell task's permanently-root euid - a non-root login "
              "can no longer self-grant capabilities or corrupt system "
              "files via redirection, while still working normally "
              "inside its own home directory")
        return 0
    finally:
        qemu.terminate()
        qemu.wait(timeout=10)


if __name__ == "__main__":
    sys.exit(main())
