# Makefile fragment for kernel/.
#
# Every kernel subsystem has its own Makefile (kernel/core/Makefile,
# kernel/drivers/Makefile, and so on, down to one per individual driver
# under kernel/drivers/*/Makefile) that only declares that directory's
# own source files. None of these fragments compile or link anything
# themselves - they are `include`d, bottom to top, until this file sums
# every one of them into KERNEL_SRCS. The actual compile rule and the
# kernel.elf link step both stay in the top-level project Makefile,
# which includes this file instead of scanning the tree with `find`.
#
# Adding a new file to an existing driver or subsystem: add it to that
# directory's own Makefile, nothing here needs to change.
# Adding a whole new subsystem directory under kernel/: give it a
# Makefile following the pattern of any of the existing ones, then add
# one line below.

include kernel/core/Makefile
include kernel/arch/Makefile
include kernel/boot/Makefile
include kernel/drivers/Makefile
include kernel/vfs/Makefile
include kernel/mm/Makefile
include kernel/elf/Makefile
include kernel/sched/Makefile
include kernel/term/Makefile
include kernel/highx/Makefile
include kernel/fs/Makefile
include kernel/syscall/Makefile
include kernel/shell/Makefile
include kernel/net/Makefile

# kernel/main.c itself has no subdirectory of its own.
SRCS_MAIN := kernel/main.c

KERNEL_SRCS := \
  $(SRCS_MAIN) \
  $(SRCS_CORE) \
  $(SRCS_ARCH) \
  $(SRCS_BOOT) \
  $(SRCS_DRIVERS) \
  $(SRCS_VFS) \
  $(SRCS_MM) \
  $(SRCS_ELF) \
  $(SRCS_SCHED) \
  $(SRCS_TERM) \
  $(SRCS_HIGHX) \
  $(SRCS_FS) \
  $(SRCS_SYSCALL) \
  $(SRCS_SHELL) \
  $(SRCS_NET)
