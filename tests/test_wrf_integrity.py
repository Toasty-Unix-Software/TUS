#!/usr/bin/env python3
"""
test_wrf_integrity.py - proves WRF v2's ZFS-inspired integrity features
actually catch corruption, not just that mount/read/write still work.

Two real properties are exercised here, each by actually damaging bytes
on a raw disk image between two QEMU boots and checking the NEXT boot
notices:

  1. Per-block checksums: flip a bit in an already-written data block
     (simulating bit rot / a misdirected write) WITHOUT touching its
     checksum table entry, then run the `wrfscrub` shell command on the
     next boot and confirm it reports the corruption.

  2. Dual superblock: zero out the PRIMARY superblock sector (simulating
     a dead LBA 0) on an otherwise-valid volume, then boot and confirm
     wrf.c falls back to the backup copy at the volume's last sector and
     /home still mounts.

See include/wrf.h's v2 integrity comment and kernel/fs/wrf.c's
wrf_scrub()/try_mount_at() for what's actually being tested.

Usage: python3 tests/test_wrf_integrity.py   (from the project root)
"""

import os
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
from test_wrf_persist import boot_and_run

SB_FMT = "<14I Q I"  # magic..root_ino (12), checksum_table_lba/blocks (2), generation, sb_checksum
SB_NAMES = ["magic", "version", "total_blocks", "inode_count", "inode_bitmap_lba",
            "inode_bitmap_blocks", "block_bitmap_lba", "block_bitmap_blocks",
            "inode_table_lba", "inode_table_blocks", "data_start_lba", "root_ino",
            "checksum_table_lba", "checksum_table_blocks", "generation", "sb_checksum"]


def read_superblock(path, lba=0):
    with open(path, "rb") as f:
        f.seek(lba * 512)
        raw = f.read(512)
    return dict(zip(SB_NAMES, struct.unpack_from(SB_FMT, raw, 0)))


def test_checksum_catches_corruption():
    print("== test 1: wrfscrub catches a corrupted data block ==")
    with tempfile.TemporaryDirectory() as tmp:
        disk = os.path.join(tmp, "csum_test.img")
        subprocess.run(["qemu-img", "create", "-f", "raw", disk, "64M"],
                        check=True, stdout=subprocess.DEVNULL)

        boot_and_run(disk, ["mkfs.wrf /dev/hda"])
        log = boot_and_run(disk, [
            "echo integrity-marker > /home/scrubme.txt",
            "cat /home/scrubme.txt",
        ])
        assert "mounted /home" in log, "WRF did not mount:\n" + log[-2000:]
        assert "integrity-marker" in log, "write/read failed:\n" + log[-2000:]

        sb = read_superblock(disk)
        data_start = sb["data_start_lba"]

        # Flip one bit in data block 0 (scrubme.txt's data - the only
        # block written so far on a volume this fresh) WITHOUT touching
        # its checksum table entry, exactly what a stray cosmic ray /
        # bad sector / misdirected write does on real hardware.
        with open(disk, "r+b") as f:
            f.seek(data_start * 512)
            byte = f.read(1)
            f.seek(data_start * 512)
            f.write(bytes([byte[0] ^ 0xFF]))

        log2 = boot_and_run(disk, ["wrfscrub"])
        assert "mounted /home" in log2, "WRF did not remount:\n" + log2[-2000:]
        assert "CHECKSUM MISMATCH" in log2, \
            "scrub did not detect the corrupted block:\n" + log2[-3000:]
        assert "1 corrupt" in log2 or "corrupt" in log2, \
            "scrub summary missing:\n" + log2[-2000:]
        print("  OK - wrfscrub reported the corrupted block")


def test_backup_superblock_recovery():
    print("== test 2: a dead primary superblock falls back to the backup ==")
    with tempfile.TemporaryDirectory() as tmp:
        disk = os.path.join(tmp, "sb_test.img")
        subprocess.run(["qemu-img", "create", "-f", "raw", disk, "64M"],
                        check=True, stdout=subprocess.DEVNULL)

        boot_and_run(disk, ["mkfs.wrf /dev/hda"])
        log = boot_and_run(disk, [
            "echo still-here > /home/survivor.txt",
            "cat /home/survivor.txt",
        ])
        assert "mounted /home" in log, "WRF did not mount:\n" + log[-2000:]

        # Zero out the primary superblock sector - simulates a dead or
        # corrupted LBA 0. The backup copy (last sector of the volume)
        # is untouched.
        with open(disk, "r+b") as f:
            f.seek(0)
            f.write(b"\x00" * 512)

        log2 = boot_and_run(disk, ["cat /home/survivor.txt"])
        assert "using the backup copy" in log2 or "backup copy" in log2, \
            "wrf.c did not report falling back to the backup superblock:\n" + log2[-3000:]
        assert "mounted /home" in log2, \
            "WRF did not mount from the backup superblock:\n" + log2[-3000:]
        assert "still-here" in log2, \
            "survivor.txt did not read back after backup-superblock recovery:\n" + log2[-2000:]
        print("  OK - mounted from the backup superblock, data intact")


def main():
    test_checksum_catches_corruption()
    test_backup_superblock_recovery()
    print("PASS: WRF v2 integrity features verified against real corruption")
    return 0


if __name__ == "__main__":
    sys.exit(main())
