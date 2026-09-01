#!/usr/bin/env python3
"""
test_video.py - automated test for MP4 playback on TUS

Boots tus.iso, starts a highX session with hxvideo as the session
leader, and checks that the shipped MP4 is really decoded: the sample
clip is a colour-bar pattern, so the framebuffer must fill with
saturated colours that nothing else in TUS paints. Then it exercises
the player's controls (pause, restart, seek) and quits.

Usage: python3 tests/test_video.py   (from the project root)
"""

import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from test_boot import (qmp_cmd, qmp_connect, screendump, sendkey, start_qemu,
                       type_text, wait_for)

PASS = 0


def ok(name):
    global PASS
    PASS += 1
    print(f"  [PASS] {name}")


def read_ppm(path):
    with open(path, "rb") as f:
        data = f.read()
    assert data.startswith(b"P6"), f"{path}: not a P6 PPM"
    parts = data[:200].split()
    width, height = int(parts[1]), int(parts[2])
    body = data[data.index(b"\n255\n") + 5:]
    return width, height, body


def count_video_pixels(path):
    """Pixels of the clip's saturated colour bars (red, green, blue,
    yellow, magenta, cyan): one channel at full scale and another at
    zero. TUS's own chrome is grey, blue-grey or white, so it never
    passes this test - white text has no dark channel."""
    width, height, body = read_ppm(path)
    hits = 0
    for i in range(0, width * height * 3, 3):
        r, g, b = body[i], body[i + 1], body[i + 2]
        if max(r, g, b) > 200 and min(r, g, b) < 60:
            hits += 1
    return hits


def main():
    assert os.path.exists("tus.iso"), "tus.iso missing - run `make iso` first"
    print("== TUS video playback test ==")
    proc = start_qemu()

    try:
        offset = wait_for("tsh ready", timeout=90)
        sock = qmp_connect()
        time.sleep(2)
        sendkey(sock, "n")
        offset = wait_for("tus:/>", offset=offset)
        ok("kernel boots")

        # hxvideo as the session leader: highX starts, the player opens
        # the file shipped in the root filesystem.
        type_text(sock, "highx hxvideo\r")
        offset = wait_for("/bin/hxvideo started", offset=offset)
        ok("`highx hxvideo` starts the player")

        time.sleep(6)
        screendump(sock, "/tmp/tus-video-1.ppm")
        pixels = count_video_pixels("/tmp/tus-video-1.ppm")
        assert pixels > 20000, f"no decoded picture on screen ({pixels} px)"
        ok(f"the MP4 is demuxed, decoded and displayed ({pixels} video px)")

        # Pause: the picture stays on screen.
        sendkey(sock, "spc")
        time.sleep(2)
        screendump(sock, "/tmp/tus-video-2.ppm")
        paused = count_video_pixels("/tmp/tus-video-2.ppm")
        assert paused > 20000, f"picture vanished when paused ({paused} px)"
        ok(f"Space pauses and the frame stays on screen ({paused} px)")

        # Restart: seeking back to the first key frame and decoding on.
        sendkey(sock, "r")
        time.sleep(1)
        sendkey(sock, "spc")   # resume if the restart left it paused
        time.sleep(5)
        screendump(sock, "/tmp/tus-video-3.ppm")
        again = count_video_pixels("/tmp/tus-video-3.ppm")
        assert again > 20000, f"nothing decoded after restart ({again} px)"
        ok(f"R restarts playback from the first key frame ({again} px)")

        # Seek forward, then back.
        sendkey(sock, "right")
        time.sleep(3)
        sendkey(sock, "left")
        time.sleep(3)
        screendump(sock, "/tmp/tus-video-4.ppm")
        seeked = count_video_pixels("/tmp/tus-video-4.ppm")
        assert seeked > 20000, f"seeking broke playback ({seeked} px)"
        ok(f"the arrow keys seek and playback continues ({seeked} px)")

        # A file the decoder cannot handle must say so in the window
        # instead of dying silently behind the window system.
        sendkey(sock, "q")
        offset = wait_for("highx: session ended", offset=offset, timeout=30)
        type_text(sock, "highx hxvideo /video/high-profile.mp4\r")
        offset = wait_for("/bin/hxvideo started", offset=offset)
        time.sleep(5)
        screendump(sock, "/tmp/tus-video-6.ppm")
        refused = count_video_pixels("/tmp/tus-video-6.ppm")
        assert refused < 2000, \
            f"a high profile file should not decode ({refused} px)"
        ok("a non-baseline file is refused with a message, not a crash")

        # Quit: the player exits, so the session (it is the leader) ends.
        sendkey(sock, "q")
        offset = wait_for("highx: session ended", offset=offset, timeout=30)
        ok("Q quits the player and ends the session")

        time.sleep(2)
        screendump(sock, "/tmp/tus-video-5.ppm")
        left = count_video_pixels("/tmp/tus-video-5.ppm")
        assert left < 2000, f"video pixels survived the session ({left})"
        ok("the text console is back")

        print(f"\nALL {PASS} TESTS PASSED")
        return 0
    finally:
        try:
            qmp_cmd(sock, "quit")
        except Exception:
            pass
        proc.terminate()
        try:
            proc.wait(timeout=10)
        except Exception:
            proc.kill()


if __name__ == "__main__":
    sys.exit(main())
