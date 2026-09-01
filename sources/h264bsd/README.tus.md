# h264bsd in TUS

This is the H.264 decoder `hxvideo` uses to play MP4 files.

* Upstream: <https://github.com/oneam/h264bsd>
* Upstream commit: `42bcb5d753ad86d84903354bf3c68423c28adb7b` (2022-04-26)
* Vendored: `src/` only (26 C files), **unmodified**.
* License: Apache-2.0 for the decoder itself (it comes from the
  Android Open Source Project) plus MIT for the wrappers - see
  `LICENSE.md`. TUS itself is BSD-3-Clause; both licenses are
  permissive and keep their own terms inside this directory.

## Why this decoder

It is a *baseline* profile decoder: no CABAC, no B-frames, integer
arithmetic only, and it allocates with plain `malloc`/`free`. That is
exactly what a freestanding TUS program can link against - the same
`-ffreestanding -mgeneral-regs-only` build as every other userspace
tool, on top of the ported musl.

The consequence is visible to users: files must be H.264 **baseline**.
`hxvideo` detects anything else and says so in its window, with the
`ffmpeg` command that converts a file (`make video FILE=...` does it
for you).

## How it is built

The Makefile compiles `sources/h264bsd/src/*.c` into
`build/h264bsd/*.o` and links them into `rootfs/bin/hxvideo`. Nothing
else in TUS depends on it.

## How it is tested

`make test-mp4` builds the decoder **and** the TUS demuxer
(`userspace/mp4.c`) on the build host and decodes the file shipped in
`rootfs/video/`, checking the frame count, the key frames, seeking and
that the pictures are not blank. `make test-video` then plays the same
file inside a booted TUS and checks the framebuffer.
