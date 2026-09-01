# TUS Documentation

This folder is the real documentation for Toasty Unix Software. The top level
README is a short introduction for someone landing on the repository for the
first time. Everything past that belongs here.

The goal of these pages is to let someone who has never touched TUS before
sit down, read through them, and come out the other side able to boot the
system, understand what happens between power on and the shell prompt, add
a driver without guessing at the conventions, and reason about the security
decisions that were made and why they were made that way. Nothing here is
written for a machine to parse. It is written for a person, the same way you
would explain the project to a new engineer joining the team over coffee.

## Where to start

If you are completely new, read `boot.md` first. It walks through exactly
what happens from the moment the firmware hands control to Limine until tsh
prints its prompt, and it explains the reasoning behind the ordering of
every subsystem init call, not just the mechanics.

If you already understand roughly how the kernel boots and you want to
write a driver, go straight to `drivers.md`. It documents the actual folder
layout under `kernel/drivers/`, the build system hooks a new driver needs,
and the patterns TUS drivers follow (polling versus interrupts, DMA,
how a driver registers itself with the PCI enumerator, and so on).

If you care about how TUS thinks about security, `security.md` covers the
syscall pointer validation model, the Spectre v1 and v2 mitigations, the
permission model in the VFS, and the general philosophy of failing closed
rather than trusting user space.

`architecture.md` is the big picture document. It is the one to read if you
want to understand how the pieces fit together at a system level: memory
management, the scheduler, the VFS, the network stack, highX, and how they
talk to each other.

`build-system.md` explains the Makefile layout, which is deliberately split
into one Makefile per subdirectory instead of one giant file at the root
that globs everything with `find`. If you are adding a new source file or a
whole new subsystem directory, this is where you learn how the pieces wire
together.

## Table of contents

- `boot.md` - the boot sequence, stage by stage, with the reasoning behind
  the init order
- `drivers.md` - how to write a driver for TUS, using the actual folder
  layout and real examples from the existing drivers
- `security.md` - the security model: syscall validation, Spectre
  mitigations, permissions, and the general threat model TUS assumes
- `architecture.md` - the system-level picture: memory, scheduling, VFS,
  networking, highX
- `build-system.md` - how the Makefile tree is organized and how to extend
  it
- `INSTALL.md` - installing TUS onto a real disk with tusinstall

None of these documents assume you have read the source code first. They
assume you are about to, and they are here to make that easier.
