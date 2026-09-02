/*
 * tus_elf.h - running static ELF images on TUS
 *
 * Only ET_EXEC binaries (linked with `-static`) are supported. Two
 * entry points: elf_exec() spawns the image as a NEW ring-3 task
 * (used by the shell's exec command and the PATH lookup), while
 * elf_exec_current() implements execve - it replaces the calling
 * task's program image in place.
 */

#ifndef TUS_ELF_TUS_ELF_H
#define TUS_ELF_TUS_ELF_H

#include <stdint.h>

/* Load the static ELF at `path` and spawn it as a new ring-3 task.
 * Returns the new PID (>= 0) on success, a negative errno otherwise
 * (the shell uses the pid to wait for the program to exit). */
long elf_exec(const char *path, int argc, char **argv);

/* Same as elf_exec(), but the new task starts at the given uid/gid
 * (both real and effective) instead of always root, and `caps` is
 * the capability bitmask (see kernel/sched/cap.h) actually granted
 * to that session - used to gate exec of a foreign Linux-ABI binary
 * (CAP_LINUX_EXEC) by real per-session identity instead of a
 * uid==0 shortcut. Used by the shell to run a typed command as
 * whoever is actually logged into that session, rather than as
 * root regardless of who is "logged in" - see g_session_uid/
 * g_session_caps in kernel/shell/commands.c. */
long elf_exec_as(const char *path, int argc, char **argv,
                 uint32_t uid, uint32_t gid, uint32_t caps);

/* execve: replace the CURRENT task's image with the ELF at `path`.
 * `path`/`argv` must be kernel pointers; `frame_rsp` is the address
 * of the live interrupt frame on the calling task's kernel stack
 * (from syscall_entry), which is rewritten so the syscall epilogue
 * IRETQs into the new program. Returns a negative errno on failure
 * (the task is then unchanged). */
long elf_exec_current(const char *path, int argc, char **argv,
                      uint64_t frame_rsp);

#endif /* TUS_ELF_TUS_ELF_H */
