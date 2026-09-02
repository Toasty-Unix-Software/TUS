# TUS - Toasty Unix Software
#
# Targets:
#   make          - build kernel.elf
#   make iso      - build tus.iso (bootable hybrid BIOS/UEFI image)
#   make run      - build and boot in QEMU (windowed)
#   make run-smp  - boot with 4 virtual CPUs (boot splash shows 4 toasts)
#   make test     - build and run the automated boot test
#   make test-highx      - boot and drive a highX/tusWM session
#   make test-de         - boot and drive a tusDE desktop with the mouse
#   make test-shell      - tsh + hxterm: quoting, pipes, redirection, history
#   make test-term       - hxtsh (a window on the kernel's tsh), hxfiles,
#                          and the mouse wheel
#   make test-install    - the greeter, the disk installer, and booting
#                          the disk it wrote (needs OVMF for the last part)
#   make test-compositor - host unit tests for the highX compositor
#   make test-fb         - host unit tests for the framebuffer console
#   make test-mp4        - host test of the MP4 demuxer + H.264 decoder
#   make test-video      - boot and play the shipped MP4 with hxvideo
#   make video FILE=x    - convert a video into rootfs/video/ (needs ffmpeg)
#   make clean    - remove build artifacts
#   make gcc      - clean rebuild with GCC
#   make clang    - clean rebuild with Clang
#
# Layout:
#   kernel/       - kernel sources (compiled into build/kernel/)
#   rootfs/       - staging dir for the root filesystem image
#   rootfs.img    - ustar tar of rootfs/, loaded by Limine as a module
#   build/        - all compiler output (objects, deps, test objects)
#   musl-out/     - ported musl libc (headers + libc.a + crt)

# ---- Quiet mode (V=1 to see commands) ----
ifneq ($(V),1)
  Q := @
  QUIET_CC   = @echo '  CC      ' $<
  QUIET_LD   = @echo '  LD      ' $@
  QUIET_AS   = @echo '  AS      ' $<
  QUIET_GEN  = @echo '  GEN     ' $@
else
  Q :=
  QUIET_CC   =
  QUIET_LD   =
  QUIET_AS   =
  QUIET_GEN  =
endif

CC      := gcc
LD      := ld
EXTRA_CFLAGS :=
EXTRA_LDFLAGS :=

# musl libc (userspace C library, ported to the TUS syscall ABI).
# `make musl` (or the first kernel build) configures, builds and
# installs musl-1.2.6 into musl-out/ (headers + libc.a + crt).
MUSL_DIR := sources/musl-1.2.6
MUSL_OUT := musl-out
MUSL_INC := $(MUSL_OUT)/usr/include
MUSL_LIB := $(MUSL_OUT)/usr/lib

# -m64            64-bit code (long mode, no paging setup by us)
# -ffreestanding  no hosted runtime assumptions
# -fno-stack-protector  no SSP (no canaries in kernel mode)
# -fno-pic        fixed higher-half addresses
# -mcmodel=kernel   kernel runs in the negative 2 GiB (0xffffffff80000000)
# -mno-red-zone   interrupt handlers must not clobber the red zone
# -mgeneral-regs-only  keep FPU/SSE state untouched in kernel mode
CFLAGS  := -m64 -ffreestanding -fno-stack-protector -fno-pic \
           -mcmodel=kernel -mno-red-zone -mgeneral-regs-only -O2 -Wall -Wextra \
           -std=gnu11 -Iinclude -Ikernel $(EXTRA_CFLAGS)

LDFLAGS := -m elf_x86_64 -T kernel/linker.ld $(EXTRA_LDFLAGS)

BUILD       := build
# kernel/Makefile pulls in one Makefile per kernel subdirectory (down to
# one per individual driver under kernel/drivers/*/) and sums them into
# KERNEL_SRCS - see kernel/Makefile's own comment for how a new file or
# subsystem gets added. This project Makefile stays the only place that
# actually compiles or links anything.
include kernel/Makefile
KERNEL_OBJS := $(patsubst kernel/%.c,$(BUILD)/kernel/%.o,$(KERNEL_SRCS))
DEPS        := $(KERNEL_OBJS:.o=.d)

# Root filesystem: rootfs/ is tarred into rootfs.img (ustar format,
# parsed by kernel/vfs/rootfs.c). User programs are built straight
# into rootfs/bin/ - no file extensions, like real UNIX: executability
# comes from the x permission bit, and the image stores the modes
# (doas/passwd ship setuid 4555). The empty base directories (dev,
# tmp) are created here - git does not track empty dirs - and end up
# in the image, so the kernel does not hardcode the directory tree.
ROOTFS_DIR   := rootfs
ROOTFS_IMG   := rootfs.img
ROOTFS_FILES := $(shell find $(ROOTFS_DIR) -type f 2>/dev/null)

# Static x86-64 user programs (linked at 0x10000000, entry _start).
USER_CFLAGS := -m64 -ffreestanding -fno-stack-protector -fno-pic \
               -mno-red-zone -mgeneral-regs-only -O2 -Wno-shift-op-parentheses $(EXTRA_CFLAGS)
USER_LDFLAGS := -m elf_x86_64 -static -e _start -Ttext 0x10000000 $(EXTRA_LDFLAGS)
MUSL_LINK := -L$(MUSL_LIB) -lc $(MUSL_LIB)/crtn.o

# Network tools include <tusnet.h> (the netctl ABI, shared verbatim
# with the kernel) and userspace/tusnetutil.h next to it.
NET_CFLAGS := $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -Iuserspace

# Bare command names in /bin (the shell finds them via its PATH
# lookup: typing `kilo` runs /bin/kilo).
USER_ELFS := $(ROOTFS_DIR)/bin/hello \
             $(ROOTFS_DIR)/bin/enforce \
             $(ROOTFS_DIR)/bin/fault \
             $(ROOTFS_DIR)/bin/musl_hello \
             $(ROOTFS_DIR)/bin/socktest \
             $(ROOTFS_DIR)/bin/kilo \
             $(ROOTFS_DIR)/bin/clonetest

# TUS system tools (userspace/), all linked against musl.
USER_TOOLS := $(ROOTFS_DIR)/bin/doas \
              $(ROOTFS_DIR)/bin/useradd \
              $(ROOTFS_DIR)/bin/passwd \
              $(ROOTFS_DIR)/bin/login \
              $(ROOTFS_DIR)/bin/grep \
              $(ROOTFS_DIR)/bin/sed \
              $(ROOTFS_DIR)/bin/echo \
              $(ROOTFS_DIR)/bin/ping \
              $(ROOTFS_DIR)/bin/ping6 \
              $(ROOTFS_DIR)/bin/nc \
              $(ROOTFS_DIR)/bin/ifconfig \
              $(ROOTFS_DIR)/bin/dhcp \
              $(ROOTFS_DIR)/bin/netstat \
              $(ROOTFS_DIR)/bin/arp \
              $(ROOTFS_DIR)/bin/route \
              $(ROOTFS_DIR)/bin/hostname \
              $(ROOTFS_DIR)/bin/host \
              $(ROOTFS_DIR)/bin/res_set \
              $(ROOTFS_DIR)/bin/keymap \
              $(ROOTFS_DIR)/bin/hxfont \
              $(ROOTFS_DIR)/bin/lvgldemo \
              $(ROOTFS_DIR)/bin/ssh \
              $(ROOTFS_DIR)/bin/ssh-keygen \
              $(ROOTFS_DIR)/bin/sshd \
              $(ROOTFS_DIR)/bin/fetch \
              $(ROOTFS_DIR)/bin/wget \
              $(ROOTFS_DIR)/bin/clint \
              $(ROOTFS_DIR)/bin/wavplay \
              $(ROOTFS_DIR)/bin/hxwavplayer \
              $(ROOTFS_DIR)/bin/ls \
              $(ROOTFS_DIR)/bin/cat \
              $(ROOTFS_DIR)/bin/mkdir \
              $(ROOTFS_DIR)/bin/touch \
              $(ROOTFS_DIR)/bin/rm \
              $(ROOTFS_DIR)/bin/mv \
              $(ROOTFS_DIR)/bin/cp \
              $(ROOTFS_DIR)/bin/head \
              $(ROOTFS_DIR)/bin/tail \
              $(ROOTFS_DIR)/bin/wc \
              $(ROOTFS_DIR)/bin/pwd \
              $(ROOTFS_DIR)/bin/uptime \
              $(ROOTFS_DIR)/bin/sleep \
              $(ROOTFS_DIR)/bin/date \
              $(ROOTFS_DIR)/bin/whoami \
              $(ROOTFS_DIR)/bin/id \
              $(ROOTFS_DIR)/bin/df \
              $(ROOTFS_DIR)/bin/kill \
              $(ROOTFS_DIR)/bin/ps \
              $(ROOTFS_DIR)/bin/pkill \
              $(ROOTFS_DIR)/bin/tussm \
              $(ROOTFS_DIR)/bin/errord \
              $(ROOTFS_DIR)/bin/bootd \
              $(ROOTFS_DIR)/bin/clear \
              $(ROOTFS_DIR)/bin/pty \
              $(ROOTFS_DIR)/bin/tpm \
              $(ROOTFS_DIR)/bin/mkfs.wrf \
              $(ROOTFS_DIR)/bin/mkswap

# ---- Mbed TLS ----
#
# sources/mbedtls is Mbed TLS 4.2.0, built with the same freestanding
# x86-64 flags as the rest of userspace and against the ported musl
# rather than the host's C library. CMake drives it because that is
# what Mbed TLS 4 supports; userspace/tls/tus_toolchain.cmake is what
# turns a host CMake into this cross build, and the two config headers
# beside it take away the parts of the library that assume an
# operating system TUS is not (files, clocks, threads, SSE).
#
# The build is a separate step from the kernel's: it is slow, it
# changes only when the vendored tree does, and nothing else in TUS
# needs it. `make mbedtls` runs it; the rules below depend on the
# libraries existing, not on rebuilding them.
MBEDTLS_SRC   := sources/mbedtls
MBEDTLS_BUILD := $(BUILD)/mbedtls
TLS_DIR       := userspace/tls
MBEDTLS_LIBS  := $(MBEDTLS_BUILD)/library/libmbedtls.a \
                 $(MBEDTLS_BUILD)/library/libmbedx509.a \
                 $(MBEDTLS_BUILD)/tf-psa-crypto/core/libtfpsacrypto.a

# Both config headers have to reach every file that includes an Mbed
# TLS header, not just the library's own sources: a caller compiled
# against a different configuration sees different struct layouts.
TLS_DEFS := -DMBEDTLS_USER_CONFIG_FILE='"$(CURDIR)/$(TLS_DIR)/tus_mbedtls_config.h"' \
            -DTF_PSA_CRYPTO_USER_CONFIG_FILE='"$(CURDIR)/$(TLS_DIR)/tus_psa_config.h"'
TLS_INC  := -isystem $(MBEDTLS_SRC)/include \
            -isystem $(MBEDTLS_SRC)/tf-psa-crypto/include \
            -isystem $(MBEDTLS_SRC)/tf-psa-crypto/drivers/builtin/include
TLS_CFLAGS := $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -Iuserspace \
              $(TLS_INC) $(TLS_DEFS)

mbedtls: $(MBEDTLS_LIBS)

$(MBEDTLS_LIBS): $(TLS_DIR)/tus_toolchain.cmake \
                 $(TLS_DIR)/tus_mbedtls_config.h $(TLS_DIR)/tus_psa_config.h \
                 | $(MUSL_LIB)/libc.a
	$(QUIET_GEN)
	$(Q)cmake -S $(MBEDTLS_SRC) -B $(MBEDTLS_BUILD) -G Ninja \
                -DCMAKE_TOOLCHAIN_FILE=$(CURDIR)/$(TLS_DIR)/tus_toolchain.cmake \
                -DTUS_MUSL_INC=$(CURDIR)/$(MUSL_INC) \
                -DENABLE_TESTING=OFF -DENABLE_PROGRAMS=OFF \
                -DUSE_STATIC_MBEDTLS_LIBRARY=ON -DUSE_SHARED_MBEDTLS_LIBRARY=OFF \
                -DMBEDTLS_USER_CONFIG_FILE=$(CURDIR)/$(TLS_DIR)/tus_mbedtls_config.h \
                -DTF_PSA_CRYPTO_USER_CONFIG_FILE=$(CURDIR)/$(TLS_DIR)/tus_psa_config.h
	$(Q)ninja -C $(MBEDTLS_BUILD)

$(BUILD)/userspace/tls/%.o: $(TLS_DIR)/%.c $(MBEDTLS_LIBS)
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(TLS_CFLAGS) -c $< -o $@

# ---- Clint, the browser ----
#
# The fetching half (URLs, HTTP, TLS) is shared between the browser
# and the `fetch` command, so it builds once into objects here.
# highAPI, defined here rather than with the highX rules below because
# a prerequisite list is expanded when the rule is read: Clint links
# the client library, and an $(HIGHAPI_OBJ) that is still empty at that
# point is a dependency make cannot see - which is exactly how a fixed
# library sat unlinked in a browser that kept crashing.
HIGHX_CFLAGS := $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -Iuserspace
HIGHAPI_OBJ  := $(BUILD)/userspace/highapi.o

CLINT_DIR    := userspace/clint
CLINT_CFLAGS := $(TLS_CFLAGS) -I$(TLS_DIR) -I$(CLINT_DIR)
CLINT_NET_OBJS := $(BUILD)/userspace/clint/http.o \
                  $(BUILD)/userspace/clint/inflate.o \
                  $(BUILD)/userspace/tls/tustls.o \
                  $(BUILD)/userspace/tls/tus_entropy.o

# Mbed TLS's bignum divides 128-bit integers, and the compiler turns
# that into a call to libgcc's __udivti3. Nothing else in TUS needs a
# compiler runtime, so it is linked here rather than everywhere.
LIBGCC := $(shell x86_64-linux-gnu-gcc -print-libgcc-file-name)

$(BUILD)/userspace/clint/%.o: $(CLINT_DIR)/%.c $(MBEDTLS_LIBS)
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CLINT_CFLAGS) -c $< -o $@

# The JavaScript interpreter is the one part of TUS that needs
# floating point: a JavaScript number *is* a double, and the language
# is defined in terms of one. Userspace may use SSE - the scheduler
# saves and restores the FPU per task - so the flag the kernel needs
# is filtered back out here.
JS_CFLAGS := $(filter-out -mgeneral-regs-only,$(CLINT_CFLAGS))

$(BUILD)/userspace/clint/js.o: $(CLINT_DIR)/js.c $(MBEDTLS_LIBS)
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(JS_CFLAGS) -c $< -o $@

$(BUILD)/userspace/clint/jsdom.o: $(CLINT_DIR)/jsdom.c $(MBEDTLS_LIBS)
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(JS_CFLAGS) -c $< -o $@

# The browser proper: the fetching objects above, plus the engine
# (parser, cascade, layout, canvas) and the highX client library.
CLINT_ENGINE_OBJS := $(BUILD)/userspace/clint/html.o \
                     $(BUILD)/userspace/clint/css.o \
                     $(BUILD)/userspace/clint/layout.o \
                     $(BUILD)/userspace/clint/paint.o \
                     $(BUILD)/userspace/clint/image.o \
                     $(BUILD)/userspace/clint/font.o \
                     $(BUILD)/userspace/clint/charset.o \
                     $(BUILD)/userspace/clint/js.o \
                     $(BUILD)/userspace/clint/jsdom.o \
                     $(BUILD)/userspace/clint/jsregex.o

# font.c includes the kernel's two font tables directly - one ASCII
# table and one accented table, shared so the browser and the console
# can never disagree about a glyph.
$(BUILD)/userspace/clint/font.o: kernel/drivers/fb/font8x16.h \
                                 kernel/drivers/fb/font_latin.h

$(ROOTFS_DIR)/bin/clint: $(BUILD)/userspace/clint/clint.o \
                         $(CLINT_ENGINE_OBJS) $(CLINT_NET_OBJS) \
                         $(HIGHAPI_OBJ) $(MBEDTLS_LIBS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/userspace/clint/clint.o $(CLINT_ENGINE_OBJS) \
                $(CLINT_NET_OBJS) $(HIGHAPI_OBJ) \
                $(MBEDTLS_LIBS) $(MUSL_LINK) $(LIBGCC)

$(ROOTFS_DIR)/bin/fetch: $(BUILD)/userspace/clint/fetch.o $(CLINT_NET_OBJS) \
                         $(MBEDTLS_LIBS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/userspace/clint/fetch.o $(CLINT_NET_OBJS) \
                $(MBEDTLS_LIBS) $(MUSL_LINK) $(LIBGCC)

$(ROOTFS_DIR)/bin/wget: $(BUILD)/userspace/clint/wget.o $(CLINT_NET_OBJS) \
                        $(MBEDTLS_LIBS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/userspace/clint/wget.o $(CLINT_NET_OBJS) \
                $(MBEDTLS_LIBS) $(MUSL_LINK) $(LIBGCC)

# ssh: the transport, the key formats and the crypto are one library
# shared by every ssh program, so they are compiled once here rather
# than rebuilt per binary. -Iuserspace picks up tusnetutil.h, which is
# how the client reaches the kernel's resolver.
SSH_CFLAGS := $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -Iuserspace \
              -Iuserspace/ssh -Iuserspace/tuscrypt
SSH_LIB_SRCS := userspace/tuscrypt/hash.c \
                userspace/tuscrypt/cipher.c \
                userspace/tuscrypt/curve25519.c \
                userspace/tuscrypt/util.c \
                userspace/ssh/sshbuf.c \
                userspace/ssh/sshkey.c \
                userspace/ssh/sshtrans.c \
                userspace/ssh/sshchan.c
SSH_LIB_OBJS := $(patsubst userspace/%.c,$(BUILD)/userspace/%.o,$(SSH_LIB_SRCS))

# highX window system: the client library (highAPI), the window
# manager and the demo applications. They compile against the shared
# protocol header in include/highx.h - the same file the kernel-side
# display server uses - so client and server can never drift apart.
# h264bsd: the vendored H.264 baseline decoder (sources/h264bsd,
# Apache-2.0 + MIT, see its LICENSE.md). It is plain integer C, so it
# builds with the same freestanding flags as every other TUS program.
H264_DIR  := sources/h264bsd/src
H264_SRCS := $(wildcard $(H264_DIR)/*.c)
H264_OBJS := $(patsubst $(H264_DIR)/%.c,$(BUILD)/h264bsd/%.o,$(H264_SRCS))
VIDEO_CFLAGS := $(HIGHX_CFLAGS) -I$(H264_DIR) -Wno-unused-but-set-variable

USER_TOOLS += $(ROOTFS_DIR)/bin/tuswm \
              $(ROOTFS_DIR)/bin/tusde \
              $(ROOTFS_DIR)/bin/hxvideo \
              $(ROOTFS_DIR)/bin/hxterm \
              $(ROOTFS_DIR)/bin/hxtsh \
              $(ROOTFS_DIR)/bin/hxfiles \
              $(ROOTFS_DIR)/bin/hxlogin \
              $(ROOTFS_DIR)/bin/tusinstall \
              $(ROOTFS_DIR)/bin/mail \
              $(ROOTFS_DIR)/bin/hxmenu \
              $(ROOTFS_DIR)/bin/hxdemo \
              $(ROOTFS_DIR)/bin/hxcube \
              $(ROOTFS_DIR)/bin/hxclock

.PHONY: all iso run run-smp test test-highx test-de test-res test-usb test-keymap test-layouts test-font test-highgl test-shell test-term \
        test-install \
        test-crypto test-ssh test-ssh-interop test-clint test-js mbedtls test-compositor test-fb test-mp4 test-video video clean clean-musl musl clang gcc

all: kernel.elf

gcc:
	$(Q)$(MAKE) CC=gcc EXTRA_CFLAGS="" EXTRA_LDFLAGS="" clean iso

clang:
	$(Q)$(MAKE) CC=clang \
                LD=x86_64-linux-gnu-ld \
                AS=x86_64-linux-gnu-as \
                LIBGCC="$(shell x86_64-linux-gnu-gcc -print-libgcc-file-name)" \
                EXTRA_CFLAGS="-target x86_64-linux-gnu" \
                EXTRA_LDFLAGS="-m elf_x86_64" \
                clean iso

musl: $(MUSL_LIB)/libc.a

# The libc is rebuilt when the TUS ABI bridge changes - otherwise an
# edited tus_syscall.c would silently test a stale libc.
$(MUSL_LIB)/libc.a: sources/musl-1.2.6/src/internal/tus_syscall.c \
                    sources/musl-1.2.6/arch/x86_64/syscall_arch.h
	$(QUIET_GEN)
	$(Q)cd $(MUSL_DIR) && CC=x86_64-linux-gnu-gcc ./configure --target=x86_64-unknown-tus \
                --disable-shared --prefix=/usr
	$(Q)$(MAKE) -C $(MUSL_DIR) -j4 CC=x86_64-linux-gnu-gcc AR=x86_64-linux-gnu-ar RANLIB=x86_64-linux-gnu-ranlib
	$(Q)$(MAKE) -C $(MUSL_DIR) install CC=x86_64-linux-gnu-gcc DESTDIR=$(CURDIR)/$(MUSL_OUT)

kernel.elf: $(KERNEL_OBJS) kernel/linker.ld
	$(QUIET_LD)
	$(Q)$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJS)

$(BUILD)/kernel/%.o: kernel/%.c
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ---- user programs (built into the rootfs staging dir) ----

# Plain freestanding test programs (no libc).
$(ROOTFS_DIR)/bin/hello: tests/hello.c
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/tests
	$(Q)$(CC) $(USER_CFLAGS) -c $< -o $(BUILD)/tests/hello.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ $(BUILD)/tests/hello.o

$(ROOTFS_DIR)/bin/enforce: tests/enforce.c
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/tests
	$(Q)$(CC) $(USER_CFLAGS) -c $< -o $(BUILD)/tests/enforce.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ $(BUILD)/tests/enforce.o

# fault: crashes on purpose, so the kernel's "kill the task, keep the
# machine" path can be tested from the shell.
$(ROOTFS_DIR)/bin/fault: tests/fault.c
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/tests
	$(Q)$(CC) $(USER_CFLAGS) -c $< -o $(BUILD)/tests/fault.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ $(BUILD)/tests/fault.o

# Programs linked against the ported musl libc (crt1.o + libc.a).
# Compiles against the musl headers (-nostdinc), like musl_hello.
$(ROOTFS_DIR)/bin/musl_hello: tests/musl_hello.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/tests
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/tests/musl_hello.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/tests/musl_hello.o $(MUSL_LINK)

# clonetest: exercises posix_spawn() -> musl's __clone(), the path
# that crashed ksh's `clear` with Invalid Opcode (raw `syscall`
# instruction in clone.s/vfork.s, unimplemented on TUS - see
# sources/musl-1.2.6/src/thread/x86_64/clone.s). Regression coverage
# for that fix without needing a full ksh93 rebuild for every check.
$(ROOTFS_DIR)/bin/clonetest: tests/test_clone_fix.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/tests
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/tests/test_clone_fix.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/tests/test_clone_fix.o $(MUSL_LINK)

# socktest: AF_UNIX sockets + poll/select, exercised from ring 3
# through the real libc entry points (socket/bind/accept/poll/select).
$(ROOTFS_DIR)/bin/socktest: tests/socktest.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/tests
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/tests/socktest.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/tests/socktest.o $(MUSL_LINK)

# kilo: a real terminal editor, unmodified, running as a ring-3 musl
# program (termios, TIOCGWINSZ, raw input, ANSI output - all provided
# by the kernel, see v0.6.0).
$(ROOTFS_DIR)/bin/kilo: sources/kilo/kilo.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/tests
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/tests/kilo.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/tests/kilo.o $(MUSL_LINK)


# ---- TUS system tools (userspace/) ----

# doas/passwd/login use crypt() from libcrypt.a; all of them link the
# full musl libc.
$(ROOTFS_DIR)/bin/doas: userspace/doas.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/doas.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/doas.o \
                -L$(MUSL_LIB) -lcrypt -lc $(MUSL_LIB)/crtn.o

$(ROOTFS_DIR)/bin/useradd: userspace/useradd.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/useradd.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/useradd.o \
                -L$(MUSL_LIB) -lcrypt -lc $(MUSL_LIB)/crtn.o

$(ROOTFS_DIR)/bin/passwd: userspace/passwd.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/passwd.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/passwd.o \
                -L$(MUSL_LIB) -lcrypt -lc $(MUSL_LIB)/crtn.o

$(ROOTFS_DIR)/bin/login: userspace/login.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/login.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/login.o \
                -L$(MUSL_LIB) -lcrypt -lc $(MUSL_LIB)/crtn.o

$(ROOTFS_DIR)/bin/grep: userspace/grep.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/grep.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/grep.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/sed: userspace/sed.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/sed.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/sed.o $(MUSL_LINK)

# ---- coreutils: real /bin binaries mirroring tsh's own built-ins
# (kernel/shell/cmd_fs.c), so ksh (and any other ring-3 shell) can use
# them via a plain PATH lookup + execve, exactly like on a real Unix.
# tsh's built-ins are untouched - this is purely additive. ----
$(ROOTFS_DIR)/bin/ls: userspace/ls.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/ls.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/ls.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/cat: userspace/cat.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/cat.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/cat.o $(MUSL_LINK)

$(BUILD)/userspace/tpm.o: userspace/tpm.c $(MBEDTLS_LIBS)
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -I$(CLINT_DIR) -c $< -o $@

$(ROOTFS_DIR)/bin/tpm: $(BUILD)/userspace/tpm.o $(CLINT_NET_OBJS) \
                       $(MBEDTLS_LIBS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/userspace/tpm.o $(CLINT_NET_OBJS) \
                $(MBEDTLS_LIBS) $(MUSL_LINK) $(LIBGCC)

$(ROOTFS_DIR)/bin/mkfs.wrf: userspace/mkfs_wrf.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -c $< -o $(BUILD)/userspace/mkfs_wrf.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/mkfs_wrf.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/mkswap: userspace/mkswap.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -c $< -o $(BUILD)/userspace/mkswap.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/mkswap.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/mkdir: userspace/mkdir.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/mkdir.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/mkdir.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/touch: userspace/touch.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/touch.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/touch.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/rm: userspace/rm.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/rm.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/rm.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/mv: userspace/mv.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/mv.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/mv.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/cp: userspace/cp.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/cp.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/cp.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/head: userspace/head.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/head.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/head.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/tail: userspace/tail.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/tail.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/tail.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/wc: userspace/wc.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/wc.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/wc.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/pwd: userspace/pwd.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/pwd.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/pwd.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/uptime: userspace/uptime.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/uptime.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/uptime.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/sleep: userspace/sleep.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/sleep.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/sleep.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/date: userspace/date.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/date.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/date.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/whoami: userspace/whoami.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/whoami.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/whoami.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/id: userspace/id.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/id.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/id.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/df: userspace/df.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/df.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/df.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/kill: userspace/kill.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/kill.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/kill.o $(MUSL_LINK)

# ps/pkill use SYS_GETPROCS (kernel/syscall/syscall.h), a TUS-only ABI
# musl does not wrap - same raw int $0x80 pattern as SYS_READDIR.
$(ROOTFS_DIR)/bin/ps: userspace/ps.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/ps.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/ps.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/pkill: userspace/pkill.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/pkill.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/pkill.o $(MUSL_LINK)

# tusSM: the service manager and its two services (errorD, bootD).
$(ROOTFS_DIR)/bin/tussm: userspace/tussm.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/tussm.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/tussm.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/errord: userspace/errord.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/errord.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/errord.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/bootd: userspace/bootd.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/bootd.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/bootd.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/clear: userspace/clear.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/clear.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/clear.o $(MUSL_LINK)

# ---- ksh (AST ksh93 port) ----
#
# ksh93's own build (libast/libdll/libshell/ksh93/builtin, driven by
# its own nmake-derived scripts, NOT this Makefile) already produced
# these static ELF binaries under sources/ksh/arch/tus.x86_64/. There
# is no real rebuild rule for them here yet - this just stages the
# already-built binaries into the rootfs image, the same as the
# bootloader files a few lines below are staged rather than compiled.
# Without this, `make clean` (which `make clang`/`make gcc` both run
# first) wipes rootfs/bin/* and ksh silently vanishes from every
# from-scratch build with no error - that gap is what broke here.
# TODO: give ksh93 a real dependency-tracked Makefile rule instead of
# this copy, so editing its sources triggers a rebuild.
$(ROOTFS_DIR)/bin/ksh: sources/ksh/arch/tus.x86_64/src/cmd/ksh93/ksh
	$(QUIET_GEN)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)cp $< $@

$(ROOTFS_DIR)/bin/shcomp: sources/ksh/arch/tus.x86_64/src/cmd/ksh93/shcomp
	$(QUIET_GEN)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)cp $< $@

$(ROOTFS_DIR)/bin/pty: sources/ksh/arch/tus.x86_64/src/cmd/builtin/pty
	$(QUIET_GEN)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)cp $< $@

# keymap includes <tusinput.h>, the SYS_INPUT ABI shared with the
# kernel. Not setuid: `doas keymap tr`.
$(ROOTFS_DIR)/bin/keymap: userspace/keymap.c include/tusinput.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -c $< -o $(BUILD)/userspace/keymap.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/keymap.o $(MUSL_LINK)

# res_set includes <tusvideo.h>, the SYS_VIDEO ABI shared verbatim with
# the kernel. Not setuid on purpose: the kernel refuses a mode change
# to a non-root caller, so the way to run it is `doas res_set`.
$(ROOTFS_DIR)/bin/res_set: userspace/res_set.c include/tusvideo.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -c $< -o $(BUILD)/userspace/res_set.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/res_set.o $(MUSL_LINK)

$(BUILD)/userspace/wav.o: userspace/wav.c userspace/wav.h $(MUSL_LIB)/libc.a
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -c $< -o $@

$(ROOTFS_DIR)/bin/wavplay: userspace/wavplay.c userspace/wav.h include/tusaudio.h \
                          $(BUILD)/userspace/wav.o $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -c $< -o $(BUILD)/userspace/wavplay.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/wavplay.o \
                $(BUILD)/userspace/wav.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/echo: userspace/echo.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/echo.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/echo.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/ping: userspace/ping.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/ping.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/ping.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/ping6: userspace/ping6.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/ping6.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/ping6.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/nc: userspace/nc.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/nc.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/nc.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/ifconfig: userspace/ifconfig.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/ifconfig.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/ifconfig.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/dhcp: userspace/dhcp.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/dhcp.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/dhcp.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/host: userspace/host.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/host.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/host.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/netstat: userspace/netstat.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/netstat.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/netstat.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/arp: userspace/arp.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/arp.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/arp.o $(MUSL_LINK)

$(ROOTFS_DIR)/bin/route: userspace/route.c userspace/tusnetutil.h include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(NET_CFLAGS) -c $< -o $(BUILD)/userspace/route.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/route.o $(MUSL_LINK)

# ---- ssh ----

$(BUILD)/userspace/tuscrypt/%.o: userspace/tuscrypt/%.c \
                                 userspace/tuscrypt/tuscrypt.h
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(SSH_CFLAGS) -c $< -o $@

$(BUILD)/userspace/ssh/%.o: userspace/ssh/%.c userspace/ssh/ssh.h \
                            userspace/ssh/sshbuf.h
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(SSH_CFLAGS) -c $< -o $@

$(ROOTFS_DIR)/bin/ssh: userspace/ssh/ssh_client.c $(SSH_LIB_OBJS) \
                       userspace/ssh/sshchan.h userspace/tusnetutil.h \
                       include/tusnet.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace/ssh
	$(Q)$(CC) $(SSH_CFLAGS) -c $< -o $(BUILD)/userspace/ssh/ssh_client.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/userspace/ssh/ssh_client.o $(SSH_LIB_OBJS) $(MUSL_LINK)

$(ROOTFS_DIR)/bin/ssh-keygen: userspace/ssh/ssh_keygen.c $(SSH_LIB_OBJS) \
                              $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace/ssh
	$(Q)$(CC) $(SSH_CFLAGS) -c $< -o $(BUILD)/userspace/ssh/ssh_keygen.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/userspace/ssh/ssh_keygen.o $(SSH_LIB_OBJS) $(MUSL_LINK)

# sshd needs crypt() (password auth against /etc/shadow, same as
# login/passwd/useradd) - not otherwise part of $(SSH_LIB_OBJS)/$(MUSL_LINK).
$(ROOTFS_DIR)/bin/sshd: userspace/ssh/sshd.c $(SSH_LIB_OBJS) \
                        userspace/ssh/sshchan.h $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace/ssh
	$(Q)$(CC) $(SSH_CFLAGS) -c $< -o $(BUILD)/userspace/ssh/sshd.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/userspace/ssh/sshd.o $(SSH_LIB_OBJS) \
                -L$(MUSL_LIB) -lcrypt -lc $(MUSL_LIB)/crtn.o

$(ROOTFS_DIR)/bin/hostname: userspace/hostname.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/hostname.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hostname.o $(MUSL_LINK)

# ---- highX window system ----

$(HIGHAPI_OBJ): userspace/highapi/highapi.c userspace/highapi/highapi.h \
                include/highx.h $(MUSL_LIB)/libc.a
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $@

# ---- tusfont: TrueType, rasterised from scratch ----
#
# Three objects, no floating point anywhere (userspace is built with
# -mgeneral-regs-only; see userspace/tusfont/tusfont.h for why fixed
# point is the right answer here rather than a workaround).
TUSFONT_SRCS := userspace/tusfont/ttf.c \
                userspace/tusfont/raster.c \
                userspace/tusfont/text.c
TUSFONT_OBJS := $(patsubst userspace/%.c,$(BUILD)/userspace/%.o,$(TUSFONT_SRCS))

# HighGL and hglui (userspace/highgl, userspace/hglui): unlike tusfont,
# these are float - a 3D pipeline and the SDF shape math the macOS-style
# UI chrome is drawn with both need it, so -mgeneral-regs-only is
# filtered back out the same way JS_CFLAGS does for Clint's JavaScript,
# and the link picks up musl's libm.
HIGHGL_CFLAGS := $(filter-out -mgeneral-regs-only,$(HIGHX_CFLAGS)) -Iuserspace/highgl
HIGHGL_SRCS := userspace/highgl/context.c userspace/highgl/pipeline.c \
              userspace/highgl/raster.c userspace/highgl/texture.c
HIGHGL_OBJS := $(patsubst userspace/%.c,$(BUILD)/userspace/%.o,$(HIGHGL_SRCS))
MUSL_LINK_M := -L$(MUSL_LIB) -lm -lc $(MUSL_LIB)/crtn.o

$(BUILD)/userspace/highgl/%.o: userspace/highgl/%.c \
                               userspace/highgl/highgl.h \
                               userspace/highgl/hgl_internal.h
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/userspace/highgl
	$(Q)$(CC) $(HIGHGL_CFLAGS) -c $< -o $@

HGLUI_CFLAGS := $(HIGHGL_CFLAGS) -Iuserspace/tusfont
HGLUI_OBJ := $(BUILD)/userspace/hglui/hglui.o

$(HGLUI_OBJ): userspace/hglui/hglui.c userspace/hglui/hglui.h \
             userspace/highgl/highgl.h userspace/tusfont/tusfont.h
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/userspace/hglui
	$(Q)$(CC) $(HGLUI_CFLAGS) -c $< -o $@

# LVGL (sources/lvgl, vendored upstream v9.6 - unmodified except for
# sources/lv_conf.h, which is TUS's, not upstream's). Float, same
# reason as HighGL/hglui: this is a real 2D compositor with real
# floating-point draw math, not a fixed-point library like tusfont.
# `-w`: 470-odd files of someone else's C, built against a different
# libc than any of its own CI configurations - warning-clean is not a
# bar third-party code compiled unmodified needs to clear, the same
# treatment sources/h264bsd already gets.
LVGL_DIR := sources/lvgl
LVGL_CFLAGS := $(filter-out -mgeneral-regs-only,$(HIGHX_CFLAGS)) -w -I$(LVGL_DIR)
LVGL_SRCS := $(shell find $(LVGL_DIR)/src -name '*.c')
LVGL_OBJS := $(patsubst $(LVGL_DIR)/src/%.c,$(BUILD)/lvgl/%.o,$(LVGL_SRCS))

$(BUILD)/lvgl/%.o: $(LVGL_DIR)/src/%.c sources/lv_conf.h
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(LVGL_CFLAGS) -c $< -o $@

LVGL_PORT_CFLAGS := $(LVGL_CFLAGS) -Iuserspace
LVGL_PORT_OBJ := $(BUILD)/userspace/lvgl_port/tus_lvgl.o

$(LVGL_PORT_OBJ): userspace/lvgl_port/tus_lvgl.c userspace/lvgl_port/tus_lvgl.h \
                 sources/lv_conf.h
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/userspace/lvgl_port
	$(Q)$(CC) $(LVGL_PORT_CFLAGS) -c $< -o $@

$(ROOTFS_DIR)/bin/lvgldemo: userspace/lvgldemo.c $(HIGHAPI_OBJ) $(LVGL_OBJS) \
                           $(LVGL_PORT_OBJ) $(MUSL_LIB)/libc.a $(MUSL_LIB)/libm.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(LVGL_PORT_CFLAGS) -c $< -o $(BUILD)/userspace/lvgldemo.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/lvgldemo.o \
                $(HIGHAPI_OBJ) $(LVGL_OBJS) $(LVGL_PORT_OBJ) $(MUSL_LINK_M)

# tusfont as an LVGL font backend (userspace/lvgl_port/tus_lvgl_font.c):
# needs LVGL's own headers/types, so it is float-enabled like the rest
# of LVGL_PORT_CFLAGS, and -Iuserspace/tusfont for tusfont.h. Links
# against TUSFONT_OBJS (fixed point, built with HIGHX_CFLAGS below) -
# mixing -mgeneral-regs-only and float-enabled objects in one binary
# is already how hglui/tusfont were linked together before the LVGL
# swap, so this is a proven pattern, not a new risk.
LVGL_FONT_CFLAGS := $(LVGL_PORT_CFLAGS) -Iuserspace/tusfont
LVGL_FONT_OBJ := $(BUILD)/userspace/lvgl_port/tus_lvgl_font.o

$(LVGL_FONT_OBJ): userspace/lvgl_port/tus_lvgl_font.c \
                 userspace/lvgl_port/tus_lvgl_font.h \
                 userspace/tusfont/tusfont.h sources/lv_conf.h
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/userspace/lvgl_port
	$(Q)$(CC) $(LVGL_FONT_CFLAGS) -c $< -o $@

$(BUILD)/userspace/tusfont/%.o: userspace/tusfont/%.c \
                                userspace/tusfont/tusfont.h \
                                userspace/tusfont/ttf_internal.h \
                                $(MUSL_LIB)/libc.a
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/userspace/tusfont
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $@

$(ROOTFS_DIR)/bin/hxfont: userspace/hxfont.c $(TUSFONT_OBJS) $(HIGHAPI_OBJ) \
                          $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $(BUILD)/userspace/hxfont.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/userspace/hxfont.o $(TUSFONT_OBJS) \
                $(HIGHAPI_OBJ) $(MUSL_LINK)

$(ROOTFS_DIR)/bin/tuswm: userspace/tuswm.c $(HIGHAPI_OBJ) $(LVGL_OBJS) \
                         $(LVGL_PORT_OBJ) $(LVGL_FONT_OBJ) $(TUSFONT_OBJS) \
                         $(MUSL_LIB)/libc.a $(MUSL_LIB)/libm.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(LVGL_FONT_CFLAGS) -c $< -o $(BUILD)/userspace/tuswm.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/tuswm.o \
                $(HIGHAPI_OBJ) $(LVGL_OBJS) $(LVGL_PORT_OBJ) $(LVGL_FONT_OBJ) \
                $(TUSFONT_OBJS) $(MUSL_LINK_M)

$(ROOTFS_DIR)/bin/tusde: userspace/tusde.c $(HIGHAPI_OBJ) $(LVGL_OBJS) \
                         $(LVGL_PORT_OBJ) $(LVGL_FONT_OBJ) $(TUSFONT_OBJS) \
                         $(MUSL_LIB)/libc.a $(MUSL_LIB)/libm.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(LVGL_FONT_CFLAGS) -c $< -o $(BUILD)/userspace/tusde.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/tusde.o \
                $(HIGHAPI_OBJ) $(LVGL_OBJS) $(LVGL_PORT_OBJ) $(LVGL_FONT_OBJ) \
                $(TUSFONT_OBJS) $(MUSL_LINK_M)

# hxwavplayer: wavplay's WAV/PCM path (userspace/wav.c) in a real
# highX window, chrome drawn with LVGL + the tusfont bridge - same
# combination tuswm.c/tusde.c use just above, so it needs the same
# float-enabled CFLAGS and link set (and must be defined after them,
# since LVGL_FONT_OBJ/LVGL_FONT_CFLAGS are simply-expanded variables -
# a prerequisite list referencing them before their own `:=` definition
# would silently expand to empty instead of erroring).
$(ROOTFS_DIR)/bin/hxwavplayer: userspace/hxwavplayer.c userspace/wav.h \
                               include/tusaudio.h $(BUILD)/userspace/wav.o \
                               $(HIGHAPI_OBJ) $(LVGL_OBJS) $(LVGL_PORT_OBJ) \
                               $(LVGL_FONT_OBJ) $(TUSFONT_OBJS) \
                               $(MUSL_LIB)/libc.a $(MUSL_LIB)/libm.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(LVGL_FONT_CFLAGS) -c $< -o $(BUILD)/userspace/hxwavplayer.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxwavplayer.o \
                $(BUILD)/userspace/wav.o $(HIGHAPI_OBJ) $(LVGL_OBJS) $(LVGL_PORT_OBJ) \
                $(LVGL_FONT_OBJ) $(TUSFONT_OBJS) $(MUSL_LINK_M)

$(ROOTFS_DIR)/bin/hxdemo: userspace/hxdemo.c $(HIGHAPI_OBJ) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $(BUILD)/userspace/hxdemo.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxdemo.o \
                $(HIGHAPI_OBJ) $(MUSL_LINK)

# hxcube: a rotating cube rendered with HighGL (float pipeline, like
# hglui/LVGL) and blitted into a highX window like hxdemo's gradient.
$(ROOTFS_DIR)/bin/hxcube: userspace/hxcube.c $(HIGHAPI_OBJ) $(HIGHGL_OBJS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHGL_CFLAGS) -Iuserspace -c $< -o $(BUILD)/userspace/hxcube.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxcube.o \
                $(HIGHAPI_OBJ) $(HIGHGL_OBJS) $(MUSL_LINK_M)

# The H.264 decoder is compiled once into build/h264bsd/ and linked
# into hxvideo; nothing else needs it.
$(BUILD)/h264bsd/%.o: $(H264_DIR)/%.c
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/h264bsd
	$(Q)$(CC) $(VIDEO_CFLAGS) -c $< -o $@

$(ROOTFS_DIR)/bin/hxvideo: userspace/hxvideo.c userspace/mp4.c userspace/mp4.h \
                           $(HIGHAPI_OBJ) $(H264_OBJS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(VIDEO_CFLAGS) -c userspace/hxvideo.c -o $(BUILD)/userspace/hxvideo.o
	$(Q)$(CC) $(VIDEO_CFLAGS) -c userspace/mp4.c -o $(BUILD)/userspace/mp4.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxvideo.o \
                $(BUILD)/userspace/mp4.o $(H264_OBJS) $(HIGHAPI_OBJ) $(MUSL_LINK)

$(ROOTFS_DIR)/bin/hxterm: userspace/hxterm.c $(HIGHAPI_OBJ) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $(BUILD)/userspace/hxterm.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxterm.o \
                $(HIGHAPI_OBJ) $(MUSL_LINK)

# hxtsh: the terminal that runs the kernel's own tsh over a terminal
# session (include/tusterm.h) instead of carrying a shell of its own.
$(ROOTFS_DIR)/bin/hxtsh: userspace/hxtsh.c $(HIGHAPI_OBJ) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $(BUILD)/userspace/hxtsh.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxtsh.o \
                $(HIGHAPI_OBJ) $(MUSL_LINK)

# hxfiles: the file manager.
$(ROOTFS_DIR)/bin/hxfiles: userspace/hxfiles.c $(HIGHAPI_OBJ) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $(BUILD)/userspace/hxfiles.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxfiles.o \
                $(HIGHAPI_OBJ) $(MUSL_LINK)

$(ROOTFS_DIR)/bin/hxmenu: userspace/hxmenu.c $(HIGHAPI_OBJ) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $(BUILD)/userspace/hxmenu.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxmenu.o \
                $(HIGHAPI_OBJ) $(MUSL_LINK)

$(ROOTFS_DIR)/bin/hxclock: userspace/hxclock.c $(HIGHAPI_OBJ) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $(BUILD)/userspace/hxclock.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxclock.o \
                $(HIGHAPI_OBJ) $(MUSL_LINK)

# tusinstall: writes an EFI system partition onto a disk and copies
# the running system into it (kernel and rootfs come from /dev/kernel
# and /dev/rootfs, so no build step has to keep a second copy).
$(ROOTFS_DIR)/bin/tusinstall: userspace/tusinstall.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -Iinclude -c $< -o $(BUILD)/userspace/tusinstall.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
            $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/tusinstall.o \
            -L$(MUSL_LIB) -lcrypt -lc $(MUSL_LIB)/crtn.o

$(ROOTFS_DIR)/bin/mail: userspace/mail.c $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) -c $< -o $(BUILD)/userspace/mail.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
            $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/mail.o $(MUSL_LINK)

# hxlogin: the greeter a desktop session starts with.
$(ROOTFS_DIR)/bin/hxlogin: userspace/hxlogin.c $(HIGHAPI_OBJ) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin $(BUILD)/userspace
	$(Q)$(CC) $(HIGHX_CFLAGS) -c $< -o $(BUILD)/userspace/hxlogin.o
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(BUILD)/userspace/hxlogin.o \
                $(HIGHAPI_OBJ) $(MUSL_LINK)

# ---- PCC: a native C compiler that runs on TUS ----
#
# sources/pcc is the Portable C Compiler, ported to TUS as an on-target
# compiler (tsh can run `cc` to compile C, same as any other program in
# /bin) rather than a second host cross-toolchain. The TUS backend lives
# in sources/pcc/os/tus/ (ccconfig.h) plus a small os_tus branch in
# cc/cc/cc.c's strlist_exec(): TUS has no fork()/vfork(), so subprocess
# spawning uses SYS_SPAWN + musl's waitpid() (already wired to TUS's
# SYS_WAITPID) instead.
#
# PCC's grammar/lexer (cgram.y/scan.l) and mip/mkext.c are compiled and
# run on the host at PCC's own configure/build time to generate
# cgram.c, scan.c, external.c and config.h - portable C with nothing
# target-specific baked in, so they are pre-generated once and
# committed under sources/pcc/os/tus/generated/ instead of requiring
# autoconf/bison/flex as build dependencies for every TUS build.
# Regenerate them (after editing cgram.y/scan.l/mkext.c, or configure.ac)
# with, from sources/pcc: `autoconf && ./configure --target=x86_64-unknown-tus
# && make -C cc/ccom` (needs bison and flex), then copy config.h and
# cc/ccom/{cgram,external}.{c,h} and cc/ccom/scan.c back into generated/.
PCC_DIR      := sources/pcc
PCC_GEN      := $(PCC_DIR)/os/tus/generated
PCC_CFLAGS   := $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) \
                -D_DEFAULT_SOURCE -DGCC_COMPAT -DPCC_DEBUG -Dos_tus -Dmach_amd64 \
                -DTARGOSVER=0 -DLIBEXECDIR=\"/bin/\" -DINCLUDEDIR=\"/usr/include/\" \
                -I$(PCC_DIR)/cc/driver -I$(PCC_GEN) -I$(PCC_DIR)/os/tus \
                -I$(PCC_DIR)/mip -I$(PCC_DIR)/arch/amd64 -I$(PCC_DIR)/common

$(BUILD)/pcc/%.o: $(PCC_DIR)/cc/cc/%.c
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(PCC_CFLAGS) -c $< -o $@

$(BUILD)/pcc/%.o: $(PCC_DIR)/cc/driver/%.c
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(PCC_CFLAGS) -c $< -o $@

$(BUILD)/pcc/%.o: $(PCC_DIR)/common/%.c
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(PCC_CFLAGS) -c $< -o $@

PCC_DRIVER_OBJS := $(BUILD)/pcc/cc.o $(BUILD)/pcc/compat.o \
                    $(BUILD)/pcc/strlist.o $(BUILD)/pcc/xalloc.o

$(ROOTFS_DIR)/bin/cc: $(PCC_DRIVER_OBJS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(PCC_DRIVER_OBJS) $(MUSL_LINK)

# cpp: the standalone preprocessor cc invokes as the pipeline's first
# stage. -DNVMPGS matches PCC's default virtual-memory page count for
# its macro-buffer allocator (see cc/cpp/cpp.h).
PCC_CPP_CFLAGS := $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) \
                   -D_DEFAULT_SOURCE -DGCC_COMPAT -DPCC_DEBUG -DNVMPGS=4 \
                   -I$(PCC_DIR)/cc/cpp -I$(PCC_GEN) -I. \
                   -I$(PCC_DIR)/mip -I$(PCC_DIR)/arch/amd64 -I$(PCC_DIR)/common

$(BUILD)/pcc/cpp/%.o: $(PCC_DIR)/cc/cpp/%.c
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(PCC_CPP_CFLAGS) -c $< -o $@

$(BUILD)/pcc/cpp/compat.o: $(PCC_DIR)/common/compat.c
	$(QUIET_CC)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(PCC_CPP_CFLAGS) -c $< -o $@

PCC_CPP_OBJS := $(BUILD)/pcc/cpp/compat.o $(BUILD)/pcc/cpp/cpp.o \
                $(BUILD)/pcc/cpp/cpc.o $(BUILD)/pcc/cpp/token.o \
                $(BUILD)/pcc/cpp/tempfile.o

$(ROOTFS_DIR)/bin/cpp: $(PCC_CPP_OBJS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(PCC_CPP_OBJS) $(MUSL_LINK)

# ccom: the compiler proper (front end + amd64 code generator in one
# pass). cgram.c/scan.c/external.c are the pre-generated grammar,
# lexer and target-constant table from PCC_GEN, not real sources.
PCC_CCOM_CFLAGS := $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) \
                    -D_DEFAULT_SOURCE -DGCC_COMPAT -DPCC_DEBUG -D_ISOC99_SOURCE \
                    -Dos_tus -Dmach_amd64 \
                    -I$(PCC_DIR)/cc/ccom -I$(PCC_GEN) \
                    -I$(PCC_DIR)/mip -I$(PCC_DIR)/arch/amd64 -I$(PCC_DIR)/os/tus \
                    -I$(PCC_DIR)/common

PCC_CCOM_SRCS := $(PCC_DIR)/cc/ccom/builtins.c $(PCC_GEN)/cgram.c \
                  $(PCC_DIR)/arch/amd64/code.c $(PCC_DIR)/mip/common.c \
                  $(PCC_DIR)/common/compat.c $(PCC_DIR)/cc/ccom/complex.c \
                  $(PCC_DIR)/cc/ccom/dwarf.c $(PCC_GEN)/external.c \
                  $(PCC_DIR)/cc/ccom/gcc_compat.c $(PCC_DIR)/cc/ccom/init.c \
                  $(PCC_DIR)/cc/ccom/inline.c $(PCC_DIR)/arch/amd64/local.c \
                  $(PCC_DIR)/arch/amd64/local2.c $(PCC_DIR)/cc/ccom/main.c \
                  $(PCC_DIR)/mip/match.c $(PCC_DIR)/cc/ccom/optim.c \
                  $(PCC_DIR)/mip/optim2.c $(PCC_DIR)/arch/amd64/order.c \
                  $(PCC_DIR)/cc/ccom/params.c $(PCC_DIR)/cc/ccom/pftn.c \
                  $(PCC_DIR)/mip/reader.c $(PCC_DIR)/common/softfloat.c \
                  $(PCC_DIR)/mip/regs.c $(PCC_GEN)/scan.c \
                  $(PCC_DIR)/cc/ccom/stabs.c $(PCC_DIR)/cc/ccom/symtabs.c \
                  $(PCC_DIR)/arch/amd64/table.c $(PCC_DIR)/cc/ccom/trees.c \
                  $(PCC_DIR)/common/unicode.c
PCC_CCOM_OBJS := $(patsubst %.c,$(BUILD)/pcc/ccom/%.o,$(notdir $(PCC_CCOM_SRCS)))

define PCC_CCOM_RULE
$(BUILD)/pcc/ccom/$(notdir $(basename $(1))).o: $(1)
	$$(QUIET_CC)
	$$(Q)mkdir -p $(BUILD)/pcc/ccom
	$$(Q)$$(CC) $$(PCC_CCOM_CFLAGS) -c $(1) -o $$@
endef
$(foreach src,$(PCC_CCOM_SRCS),$(eval $(call PCC_CCOM_RULE,$(src))))

$(ROOTFS_DIR)/bin/ccom: $(PCC_CCOM_OBJS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(PCC_CCOM_OBJS) $(MUSL_LINK)

# cc/cpp/ccom moved out of the base image, install via `tpm install pcc`

# ---- NASM: a native x86 assembler for TUS ----
#
# sources/nasm is upstream NASM, ported as a standalone assembler for
# hand-written .asm - it is not wired into PCC's cc pipeline (PCC
# emits AT&T/GNU-as syntax; NASM only ever reads its own Intel syntax,
# a different assembler entirely, not a flag away from the other).
#
# Like PCC, NASM's instruction tables and preprocessor token tables
# are generated at NASM's own build time by Perl scripts + autoconf
# feature probing (config/config.h) - portable output with nothing
# host-specific baked in, so it is pre-generated once and committed
# under sources/nasm/generated/ instead of requiring autoconf/perl as
# build dependencies for every TUS build. Regenerate after editing
# NASM's .dat/.mac sources or configure.ac with, from sources/nasm:
# `./autogen.sh && CC=x86_64-linux-gnu-gcc CFLAGS="-nostdinc
# -I$(CURDIR)/musl-out/usr/include" ./configure --with-zlib=no
# --host=x86_64-linux-gnu`, then copy config/config.h and every
# generated .c/.h under x86/, asm/, macros/, include/ back into
# generated/ (config.h needs manual fixes autoconf gets wrong for a
# musl target even when cross-configured this way: HAVE_STDBIT_H and
# HAVE_CANONICALIZE_FILE_NAME must both be undef'd - the configure
# check links successfully against glibc symbols the host linker finds
# by default, not against musl - and HAVE_SNPRINTF/HAVE_VSNPRINTF must
# both be defined - musl has both, the link-only check just came back
# negative cross-compiling this way. Leaving either undefined means
# NASM compiles its own "poor substitute" fallback from
# stdlib/vsnprintf.c instead of using musl's real one, and that
# fallback panics on the large probe size nasm_vasprintf calls it
# with).
NASM_DIR   := sources/nasm
NASM_GEN   := $(NASM_DIR)/generated
NASM_CFLAGS := $(USER_CFLAGS) -nostdinc -I$(MUSL_INC) \
               -DHAVE_CONFIG_H -I$(NASM_GEN) -I$(NASM_DIR) -I$(NASM_DIR)/include \
               -I$(NASM_DIR)/x86 -I$(NASM_DIR)/asm -I$(NASM_DIR)/output -I$(NASM_DIR)/zlib

NASM_SRCS := \
  $(NASM_DIR)/asm/error.c $(NASM_DIR)/asm/floats.c $(NASM_DIR)/asm/directiv.c \
  $(NASM_DIR)/asm/pragma.c $(NASM_DIR)/asm/assemble.c $(NASM_DIR)/asm/labels.c \
  $(NASM_DIR)/asm/parser.c $(NASM_DIR)/asm/preproc.c $(NASM_DIR)/asm/quote.c \
  $(NASM_DIR)/asm/listing.c $(NASM_DIR)/asm/eval.c $(NASM_DIR)/asm/exprlib.c \
  $(NASM_DIR)/asm/exprdump.c $(NASM_DIR)/asm/stdscan.c $(NASM_DIR)/asm/getbool.c \
  $(NASM_DIR)/asm/strfunc.c $(NASM_DIR)/asm/segalloc.c $(NASM_DIR)/asm/rdstrnum.c \
  $(NASM_DIR)/asm/srcfile.c $(NASM_DIR)/asm/uncompress.c $(NASM_DIR)/asm/warnings.c \
  $(NASM_GEN)/directbl.c $(NASM_GEN)/pptok.c $(NASM_GEN)/tokhash.c $(NASM_GEN)/macros.c \
  $(NASM_DIR)/output/outform.c $(NASM_DIR)/output/outlib.c $(NASM_DIR)/output/nulldbg.c \
  $(NASM_DIR)/output/nullout.c $(NASM_DIR)/output/outbin.c $(NASM_DIR)/output/outaout.c \
  $(NASM_DIR)/output/outcoff.c $(NASM_DIR)/output/outelf.c $(NASM_DIR)/output/outobj.c \
  $(NASM_DIR)/output/outas86.c $(NASM_DIR)/output/outdbg.c $(NASM_DIR)/output/outieee.c \
  $(NASM_DIR)/output/outmacho.c $(NASM_DIR)/output/codeview.c \
  $(NASM_DIR)/stdlib/snprintf.c $(NASM_DIR)/stdlib/vsnprintf.c \
  $(NASM_DIR)/stdlib/strlcpy.c $(NASM_DIR)/stdlib/strnlen.c \
  $(NASM_DIR)/nasmlib/ver.c $(NASM_DIR)/nasmlib/alloc.c $(NASM_DIR)/nasmlib/asprintf.c \
  $(NASM_DIR)/nasmlib/crc32b.c $(NASM_DIR)/nasmlib/crc64.c $(NASM_DIR)/nasmlib/md5c.c \
  $(NASM_DIR)/nasmlib/string.c $(NASM_DIR)/nasmlib/nctype.c $(NASM_DIR)/nasmlib/file.c \
  $(NASM_DIR)/nasmlib/fileio.c $(NASM_DIR)/nasmlib/mmap.c $(NASM_DIR)/nasmlib/realpath.c \
  $(NASM_DIR)/nasmlib/path.c $(NASM_DIR)/nasmlib/ilog2.c $(NASM_DIR)/nasmlib/numstr.c \
  $(NASM_DIR)/nasmlib/rlimit.c $(NASM_DIR)/nasmlib/zerobuf.c $(NASM_DIR)/nasmlib/bsi.c \
  $(NASM_DIR)/nasmlib/rbtree.c $(NASM_DIR)/nasmlib/hashtbl.c $(NASM_DIR)/nasmlib/raa.c \
  $(NASM_DIR)/nasmlib/saa.c $(NASM_DIR)/nasmlib/strlist.c $(NASM_DIR)/nasmlib/perfhash.c \
  $(NASM_DIR)/nasmlib/badenum.c $(NASM_DIR)/nasmlib/readnum.c \
  $(NASM_DIR)/common/common.c $(NASM_DIR)/common/errstubs.c $(NASM_DIR)/common/files.c \
  $(NASM_GEN)/insnsa.c $(NASM_GEN)/insnsb.c $(NASM_GEN)/insnsn.c $(NASM_GEN)/regs.c \
  $(NASM_GEN)/regvals.c $(NASM_GEN)/regflags.c $(NASM_GEN)/iflag.c \
  $(NASM_DIR)/zlib/adler32.c $(NASM_DIR)/zlib/crc32.c $(NASM_DIR)/zlib/infback.c \
  $(NASM_DIR)/zlib/inffast.c $(NASM_DIR)/zlib/inflate.c $(NASM_DIR)/zlib/inftrees.c \
  $(NASM_DIR)/zlib/zutil.c \
  $(NASM_DIR)/asm/nasm.c
NASM_OBJS := $(patsubst %.c,$(BUILD)/nasm/%.o,$(notdir $(NASM_SRCS)))

define NASM_RULE
$(BUILD)/nasm/$(notdir $(basename $(1))).o: $(1)
	$$(QUIET_CC)
	$$(Q)mkdir -p $(BUILD)/nasm
	$$(Q)$$(CC) $$(NASM_CFLAGS) -c $(1) -o $$@
endef
$(foreach src,$(NASM_SRCS),$(eval $(call NASM_RULE,$(src))))

$(ROOTFS_DIR)/bin/nasm: $(NASM_OBJS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(NASM_OBJS) $(MUSL_LINK)

# nasm moved out of the base image, install via `tpm install nasm`

# ---- elftoolchain: readelf and nm for TUS ----
#
# sources/elftoolchain is upstream elftoolchain (BSD-licensed ELF/DWARF
# tools). Only readelf and nm are built here - the other ~9 programs
# (ar, size, strings, objcopy/elfcopy, strip, addr2line, ...) and the
# elftoolchain `ld` linker are a separate, later port. elftoolchain's
# own `as` is a 217-line stub with no real implementation - nothing to
# port there.
#
# Like PCC and NASM, a handful of files are generated at elftoolchain's
# own build time - here via m4 macros over its ELF/DWARF constant
# tables, run through elftoolchain's own `bmake` build (not GNU make;
# `sudo apt install bmake`) - so they are pre-generated once and
# committed under sources/elftoolchain/generated/ instead of requiring
# bmake+m4 as build dependencies for every TUS build. Regenerate after
# editing any *.m4 file with, from sources/elftoolchain: `bmake` (it
# will fail at each library's final `-shared -Wl,"-soname ..."` step -
# ignore that, it's a host shared-library link this port doesn't use),
# then copy back common/sys/elfdefinitions.h and every *.c file each
# library's Makefile lists as generated from a *.m4 pair (grep
# `Makefile` in libelf/ and libdwarf/ for the `foo.c: bar.m4` rules).
#
# musl ships no sys/cdefs.h or sys/queue.h (BSD/glibc-only headers);
# generated/common/sys/ carries a minimal cdefs.h (elftoolchain only
# ever uses the two source-tagging macros from it) and an unmodified
# copy of glibc's sys/queue.h (same BSD license, standard portable
# implementation - not TUS-specific, nothing to regenerate).
#
# libelftc's demangler does real floating-point arithmetic (capacity
# growth math), so it needs actual FPU/SSE registers - like Clint's JS
# interpreter (see JS_CFLAGS above), it is built without
# -mgeneral-regs-only.
ET_DIR := sources/elftoolchain
ET_GEN := $(ET_DIR)/generated
ET_CFLAGS := $(filter-out -mgeneral-regs-only,$(USER_CFLAGS)) -nostdinc -I$(MUSL_INC) \
             -Wno-implicit-function-declaration \
             -I$(ET_DIR)/common -I$(ET_GEN)/common \
             -I$(ET_DIR)/libelf -I$(ET_GEN)/libelf \
             -I$(ET_DIR)/libdwarf -I$(ET_GEN)/libdwarf \
             -I$(ET_DIR)/libelftc

ET_LIBELF_SRCS := \
  $(ET_DIR)/libelf/elf_begin.c \
  $(ET_DIR)/libelf/elf_cntl.c \
  $(ET_DIR)/libelf/elf_data.c \
  $(ET_DIR)/libelf/elf_end.c \
  $(ET_DIR)/libelf/elf_errmsg.c \
  $(ET_DIR)/libelf/elf_errno.c \
  $(ET_DIR)/libelf/elf_fill.c \
  $(ET_DIR)/libelf/elf_flag.c \
  $(ET_DIR)/libelf/elf_getarhdr.c \
  $(ET_DIR)/libelf/elf_getarsym.c \
  $(ET_DIR)/libelf/elf_getbase.c \
  $(ET_DIR)/libelf/elf_getident.c \
  $(ET_DIR)/libelf/elf_getversion.c \
  $(ET_DIR)/libelf/elf_hash.c \
  $(ET_DIR)/libelf/elf_kind.c \
  $(ET_DIR)/libelf/elf_memory.c \
  $(ET_DIR)/libelf/elf_next.c \
  $(ET_DIR)/libelf/elf.c \
  $(ET_DIR)/libelf/elf_open.c \
  $(ET_DIR)/libelf/elf_phnum.c \
  $(ET_DIR)/libelf/elf_rand.c \
  $(ET_DIR)/libelf/elf_rawfile.c \
  $(ET_DIR)/libelf/elf_scn.c \
  $(ET_DIR)/libelf/elf_shnum.c \
  $(ET_DIR)/libelf/elf_shstrndx.c \
  $(ET_DIR)/libelf/elf_strptr.c \
  $(ET_DIR)/libelf/elf_update.c \
  $(ET_DIR)/libelf/elf_version.c \
  $(ET_DIR)/libelf/gelf_cap.c \
  $(ET_DIR)/libelf/gelf_checksum.c \
  $(ET_DIR)/libelf/gelf_dyn.c \
  $(ET_DIR)/libelf/gelf_ehdr.c \
  $(ET_DIR)/libelf/gelf_fsize.c \
  $(ET_DIR)/libelf/gelf_getclass.c \
  $(ET_DIR)/libelf/gelf_move.c \
  $(ET_DIR)/libelf/gelf_phdr.c \
  $(ET_DIR)/libelf/gelf_rela.c \
  $(ET_DIR)/libelf/gelf_rel.c \
  $(ET_DIR)/libelf/gelf_shdr.c \
  $(ET_DIR)/libelf/gelf_syminfo.c \
  $(ET_DIR)/libelf/gelf_sym.c \
  $(ET_DIR)/libelf/gelf_symshndx.c \
  $(ET_DIR)/libelf/gelf_xlate.c \
  $(ET_DIR)/libelf/libelf_align.c \
  $(ET_DIR)/libelf/libelf_allocate.c \
  $(ET_DIR)/libelf/libelf_ar.c \
  $(ET_DIR)/libelf/libelf_ar_util.c \
  $(ET_DIR)/libelf/libelf_checksum.c \
  $(ET_DIR)/libelf/libelf_data.c \
  $(ET_DIR)/libelf/libelf_ehdr.c \
  $(ET_DIR)/libelf/libelf_elfmachine.c \
  $(ET_DIR)/libelf/libelf_extended.c \
  $(ET_DIR)/libelf/libelf_memory.c \
  $(ET_DIR)/libelf/libelf_open.c \
  $(ET_DIR)/libelf/libelf_phdr.c \
  $(ET_DIR)/libelf/libelf_shdr.c \
  $(ET_DIR)/libelf/libelf_xlate.c \
  $(ET_GEN)/libelf/libelf_convert.c \
  $(ET_GEN)/libelf/libelf_fsize.c \
  $(ET_GEN)/libelf/libelf_msize.c

ET_LIBDWARF_SRCS := \
  $(ET_DIR)/libdwarf/dwarf_abbrev.c \
  $(ET_DIR)/libdwarf/dwarf_arange.c \
  $(ET_DIR)/libdwarf/dwarf_attr.c \
  $(ET_DIR)/libdwarf/dwarf_attrval.c \
  $(ET_DIR)/libdwarf/dwarf_cu.c \
  $(ET_DIR)/libdwarf/dwarf_dealloc.c \
  $(ET_DIR)/libdwarf/dwarf_die.c \
  $(ET_DIR)/libdwarf/dwarf_dump.c \
  $(ET_DIR)/libdwarf/dwarf_errmsg.c \
  $(ET_DIR)/libdwarf/dwarf_finish.c \
  $(ET_DIR)/libdwarf/dwarf_form.c \
  $(ET_DIR)/libdwarf/dwarf_frame.c \
  $(ET_DIR)/libdwarf/dwarf_init.c \
  $(ET_DIR)/libdwarf/dwarf_lineno.c \
  $(ET_DIR)/libdwarf/dwarf_loclist.c \
  $(ET_DIR)/libdwarf/dwarf_macinfo.c \
  $(ET_DIR)/libdwarf/dwarf_pro_arange.c \
  $(ET_DIR)/libdwarf/dwarf_pro_attr.c \
  $(ET_DIR)/libdwarf/dwarf_pro_die.c \
  $(ET_DIR)/libdwarf/dwarf_pro_expr.c \
  $(ET_DIR)/libdwarf/dwarf_pro_finish.c \
  $(ET_DIR)/libdwarf/dwarf_pro_frame.c \
  $(ET_DIR)/libdwarf/dwarf_pro_init.c \
  $(ET_DIR)/libdwarf/dwarf_pro_lineno.c \
  $(ET_DIR)/libdwarf/dwarf_pro_macinfo.c \
  $(ET_DIR)/libdwarf/dwarf_pro_reloc.c \
  $(ET_DIR)/libdwarf/dwarf_pro_sections.c \
  $(ET_DIR)/libdwarf/dwarf_pro_vars.c \
  $(ET_DIR)/libdwarf/dwarf_ranges.c \
  $(ET_DIR)/libdwarf/dwarf_reloc.c \
  $(ET_DIR)/libdwarf/dwarf_sections.c \
  $(ET_DIR)/libdwarf/dwarf_seterror.c \
  $(ET_DIR)/libdwarf/dwarf_str.c \
  $(ET_DIR)/libdwarf/libdwarf_abbrev.c \
  $(ET_DIR)/libdwarf/libdwarf_arange.c \
  $(ET_DIR)/libdwarf/libdwarf_attr.c \
  $(ET_DIR)/libdwarf/libdwarf_die.c \
  $(ET_DIR)/libdwarf/libdwarf_elf_access.c \
  $(ET_DIR)/libdwarf/libdwarf_elf_init.c \
  $(ET_DIR)/libdwarf/libdwarf_error.c \
  $(ET_DIR)/libdwarf/libdwarf_frame.c \
  $(ET_DIR)/libdwarf/libdwarf_info.c \
  $(ET_DIR)/libdwarf/libdwarf_init.c \
  $(ET_DIR)/libdwarf/libdwarf_lineno.c \
  $(ET_DIR)/libdwarf/libdwarf_loclist.c \
  $(ET_DIR)/libdwarf/libdwarf_loc.c \
  $(ET_DIR)/libdwarf/libdwarf_macinfo.c \
  $(ET_DIR)/libdwarf/libdwarf_nametbl.c \
  $(ET_DIR)/libdwarf/libdwarf.c \
  $(ET_DIR)/libdwarf/libdwarf_ranges.c \
  $(ET_DIR)/libdwarf/libdwarf_reloc.c \
  $(ET_DIR)/libdwarf/libdwarf_rw.c \
  $(ET_DIR)/libdwarf/libdwarf_sections.c \
  $(ET_DIR)/libdwarf/libdwarf_str.c \
  $(ET_GEN)/libdwarf/dwarf_pubnames.c \
  $(ET_GEN)/libdwarf/dwarf_pubtypes.c \
  $(ET_GEN)/libdwarf/dwarf_weaks.c \
  $(ET_GEN)/libdwarf/dwarf_funcs.c \
  $(ET_GEN)/libdwarf/dwarf_vars.c \
  $(ET_GEN)/libdwarf/dwarf_types.c \
  $(ET_GEN)/libdwarf/dwarf_pro_pubnames.c \
  $(ET_GEN)/libdwarf/dwarf_pro_weaks.c \
  $(ET_GEN)/libdwarf/dwarf_pro_funcs.c \
  $(ET_GEN)/libdwarf/dwarf_pro_types.c

ET_LIBELFTC_SRCS := \
  $(ET_DIR)/libelftc/elftc_bfdtarget.c \
  $(ET_DIR)/libelftc/elftc_copyfile.c \
  $(ET_DIR)/libelftc/elftc_demangle.c \
  $(ET_DIR)/libelftc/elftc_reloc_type_str.c \
  $(ET_DIR)/libelftc/elftc_set_timestamps.c \
  $(ET_DIR)/libelftc/elftc_string_table.c \
  $(ET_DIR)/libelftc/elftc_timestamp.c \
  $(ET_DIR)/libelftc/elftc_version.c \
  $(ET_DIR)/libelftc/libelftc_bfdtarget.c \
  $(ET_DIR)/libelftc/libelftc_dem_arm.c \
  $(ET_DIR)/libelftc/libelftc_dem_gnu2.c \
  $(ET_DIR)/libelftc/libelftc_dem_gnu3.c \
  $(ET_DIR)/libelftc/libelftc_hash.c \
  $(ET_DIR)/libelftc/libelftc_vstr.c

ET_LIBELF_OBJS   := $(patsubst %.c,$(BUILD)/elftc/libelf/%.o,$(notdir $(ET_LIBELF_SRCS)))
ET_LIBDWARF_OBJS := $(patsubst %.c,$(BUILD)/elftc/libdwarf/%.o,$(notdir $(ET_LIBDWARF_SRCS)))
ET_LIBELFTC_OBJS := $(patsubst %.c,$(BUILD)/elftc/libelftc/%.o,$(notdir $(ET_LIBELFTC_SRCS)))

define ET_RULE
$(BUILD)/elftc/$(2)/$(notdir $(basename $(1))).o: $(1)
	$$(QUIET_CC)
	$$(Q)mkdir -p $(BUILD)/elftc/$(2)
	$$(Q)$$(CC) $$(ET_CFLAGS) -c $(1) -o $$@
endef
$(foreach src,$(ET_LIBELF_SRCS),$(eval $(call ET_RULE,$(src),libelf)))
$(foreach src,$(ET_LIBDWARF_SRCS),$(eval $(call ET_RULE,$(src),libdwarf)))
$(foreach src,$(ET_LIBELFTC_SRCS),$(eval $(call ET_RULE,$(src),libelftc)))

# x86_64-linux-gnu-ar is used directly (like musl's own build above):
# archiving doesn't depend on the CC=gcc/clang choice.
$(BUILD)/elftc/libelf.a: $(ET_LIBELF_OBJS)
	$(QUIET_GEN)
	$(Q)x86_64-linux-gnu-ar rc $@ $(ET_LIBELF_OBJS)

$(BUILD)/elftc/libdwarf.a: $(ET_LIBDWARF_OBJS)
	$(QUIET_GEN)
	$(Q)x86_64-linux-gnu-ar rc $@ $(ET_LIBDWARF_OBJS)

$(BUILD)/elftc/libelftc.a: $(ET_LIBELFTC_OBJS)
	$(QUIET_GEN)
	$(Q)x86_64-linux-gnu-ar rc $@ $(ET_LIBELFTC_OBJS)

ET_LIBS := $(BUILD)/elftc/libdwarf.a $(BUILD)/elftc/libelftc.a $(BUILD)/elftc/libelf.a

$(BUILD)/elftc/readelf.o: $(ET_DIR)/readelf/readelf.c
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/elftc
	$(Q)$(CC) $(ET_CFLAGS) -c $< -o $@

$(ROOTFS_DIR)/bin/readelf: $(BUILD)/elftc/readelf.o $(ET_LIBS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/elftc/readelf.o $(ET_LIBS) $(MUSL_LINK)

$(BUILD)/elftc/nm.o: $(ET_DIR)/nm/nm.c
	$(QUIET_CC)
	$(Q)mkdir -p $(BUILD)/elftc
	$(Q)$(CC) $(ET_CFLAGS) -c $< -o $@

$(ROOTFS_DIR)/bin/nm: $(BUILD)/elftc/nm.o $(ET_LIBS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(BUILD)/elftc/nm.o $(ET_LIBS) $(MUSL_LINK)

USER_TOOLS += $(ROOTFS_DIR)/bin/readelf $(ROOTFS_DIR)/bin/nm

# ---- elftoolchain: ld (a real ELF linker) for TUS ----
#
# Unlike the utilities above, ld has its own generated files: the four
# built-in linker scripts (amd64/i386/littlemips/bigmips, turned into
# C string constants by ld/ld_script.awk from the .ld files next to
# it) and a yacc/lex grammar for parsing user-supplied linker scripts
# (ld_script_parser.y/ld_script_lexer.l). Regenerate after editing any
# of those with, from sources/elftoolchain/ld: `bmake` (needs bmake
# and, this once, real `yacc`/`lex` - already on this Pi from the NASM
# port), then copy back amd64_script.c, i386_script.c,
# littlemips_script.c, bigmips_script.c, ld_script_lexer.c,
# ld_script_parser.c and ld_script_parser.h into generated/ld/.
#
# All four architecture backends are kept, i386/mips included: TUS
# never selects anything but amd64 (LD_DEFAULT_ARCH in ld_arch.c), but
# ld_arch.c's init unconditionally registers all three, and dropping
# the other two would mean patching that upstream logic rather than
# just leaving the extra ~1500 lines of never-called code out - not
# worth the divergence for a compile-only cost.
ET_LD_CFLAGS := $(filter-out -mgeneral-regs-only,$(USER_CFLAGS)) -nostdinc -I$(MUSL_INC) \
                -Wno-implicit-function-declaration \
                -I$(ET_DIR)/ld -I$(ET_GEN)/ld \
                -I$(ET_DIR)/common -I$(ET_GEN)/common \
                -I$(ET_DIR)/libdwarf -I$(ET_GEN)/libdwarf \
                -I$(ET_DIR)/libelf -I$(ET_GEN)/libelf \
                -I$(ET_DIR)/libelftc

ET_LD_SRCS := \
  $(ET_DIR)/ld/amd64.c \
  $(ET_DIR)/ld/i386.c \
  $(ET_DIR)/ld/ld_arch.c \
  $(ET_DIR)/ld/ld_dynamic.c \
  $(ET_DIR)/ld/ld_ehframe.c \
  $(ET_DIR)/ld/ld_error.c \
  $(ET_DIR)/ld/ld_exp.c \
  $(ET_DIR)/ld/ld_file.c \
  $(ET_DIR)/ld/ld_hash.c \
  $(ET_DIR)/ld/ld_input.c \
  $(ET_DIR)/ld/ld_layout.c \
  $(ET_DIR)/ld/ld_main.c \
  $(ET_DIR)/ld/ld_options.c \
  $(ET_DIR)/ld/ld_output.c \
  $(ET_DIR)/ld/ld_path.c \
  $(ET_DIR)/ld/ld_reloc.c \
  $(ET_DIR)/ld/ld_script.c \
  $(ET_DIR)/ld/ld_strtab.c \
  $(ET_DIR)/ld/ld_symbols.c \
  $(ET_DIR)/ld/ld_symver.c \
  $(ET_DIR)/ld/mips.c \
  $(ET_GEN)/ld/amd64_script.c \
  $(ET_GEN)/ld/i386_script.c \
  $(ET_GEN)/ld/littlemips_script.c \
  $(ET_GEN)/ld/bigmips_script.c \
  $(ET_GEN)/ld/ld_script_lexer.c \
  $(ET_GEN)/ld/ld_script_parser.c

ET_LD_OBJS := $(patsubst %.c,$(BUILD)/elftc/ld/%.o,$(notdir $(ET_LD_SRCS)))

define ET_LD_RULE
$(BUILD)/elftc/ld/$(notdir $(basename $(1))).o: $(1)
	$$(QUIET_CC)
	$$(Q)mkdir -p $(BUILD)/elftc/ld
	$$(Q)$$(CC) $$(ET_LD_CFLAGS) -c $(1) -o $$@
endef
$(foreach src,$(ET_LD_SRCS),$(eval $(call ET_LD_RULE,$(src))))

$(ROOTFS_DIR)/bin/ld: $(ET_LD_OBJS) $(ET_LIBS) $(MUSL_LIB)/libc.a
	$(QUIET_LD)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)$(LD) $(USER_LDFLAGS) -o $@ \
                $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o \
                $(ET_LD_OBJS) $(ET_LIBS) $(MUSL_LINK)

USER_TOOLS += $(ROOTFS_DIR)/bin/ld

# ---- fastfetch: a system info tool for TUS ----
#
# sources/fastfetch is upstream fastfetch, built with its own CMake
# system rather than hand-listed source files like every other port
# above: it has ~150 compiled files across dozens of optional feature
# flags, and CMake already knows exactly which ones a given
# configuration needs - reproducing that by hand here would mean
# re-deriving fastfetch's own dependency graph. This mirrors how musl
# itself is built above (delegating to its own ./configure && make
# rather than a hand-written rule list), just with CMake instead of
# autoconf.
#
# TUS presents itself to fastfetch's CMakeLists.txt as CMAKE_SYSTEM_
# NAME=Linux (see sources/fastfetch/tus-compat/toolchain.cmake) -
# accurate in the sense that matters to CMake's file selection: TUS's
# musl port and syscall ABI are Linux-flavored, which is exactly what
# musl cross toolchains conventionally identify as anyway. Every
# optional external dependency (D-Bus, Wayland, X11, PulseAudio,
# libdrm, ImageMagick, SQLite, ...) is turned off below since none of
# those libraries exist for TUS; what's left is fastfetch's real
# Linux detection code, largely unmodified.
#
# That code #includes real Linux kernel/DRM uapi headers TUS has none
# of (netlink, DRM, V4L2, wireless extensions, ...). sources/fastfetch/
# tus-compat/{linux,drm}/ supplies minimal stand-ins - see the comment
# in each one for why: usually just enough structural correctness to
# compile, since the underlying subsystem doesn't exist on TUS either
# and the real ioctl()/socket() calls will correctly fail at runtime,
# which fastfetch's own modules already handle as "not detected" -
# except for the two places noted in nl80211.h and drm_mode.h where
# fastfetch's own source mixes named references with bare numeric
# literals in the same switch statement, which does require real
# kernel-correct values to avoid colliding.
#
# Regenerate/extend the compat headers the same way they were built:
# find the next missing header from a build error, grep the
# *consuming* fastfetch source file (not a real kernel header) for
# exactly which struct fields/constants/ioctls it actually references,
# and write only those - matching the style and reasoning already
# documented in every existing file under tus-compat/.
FASTFETCH_CMAKE_FLAGS := \
  -DCMAKE_TOOLCHAIN_FILE=$(CURDIR)/sources/fastfetch/tus-compat/toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DENABLE_DBUS=OFF -DENABLE_DCONF=OFF -DENABLE_DDCUTIL=OFF -DENABLE_DRM=OFF \
  -DENABLE_EET=OFF -DENABLE_EGL=OFF -DENABLE_ELF=OFF -DENABLE_GIO=OFF -DENABLE_GLX=OFF \
  -DENABLE_IMAGEMAGICK6=OFF -DENABLE_IMAGEMAGICK7=OFF -DENABLE_LIBZFS=OFF -DENABLE_LUA=OFF \
  -DENABLE_OPENCL=OFF -DENABLE_PULSE=OFF -DENABLE_QUICKJS=OFF -DENABLE_RPM=OFF \
  -DENABLE_SQLITE3=OFF -DENABLE_VADRM=OFF -DENABLE_VAX11=OFF -DENABLE_VDPAU=OFF \
  -DENABLE_VULKAN=OFF -DENABLE_WAYLAND=OFF -DENABLE_XCB_RANDR=OFF -DENABLE_XRANDR=OFF \
  -DENABLE_CHAFA=OFF -DENABLE_ZLIB=OFF -DENABLE_FREETYPE=OFF -DENABLE_THREADS=OFF \
  -DENABLE_TRACER=OFF -DENABLE_ASAN=OFF -DENABLE_LTO=OFF \
  -DENABLE_EMBEDDED_PCIIDS=OFF -DENABLE_EMBEDDED_AMDGPUIDS=OFF \
  -DENABLE_SYSTEM_YYJSON=OFF -DENABLE_WORDEXP=OFF

# Built once and committed under sources/fastfetch/prebuilt/ (static
# x86_64 ELF, musl-linked) instead of run through CMake on every build -
# fastfetch's own CMake configure/compile is the slowest single step
# in a clean `make`, and its source almost never changes here. Staged
# straight into the image below, same as the ksh93/PCC/NASM binaries.
# To rebuild after editing sources/fastfetch or tus-compat/: run the
# two commands the old rule used (kept here for that one case) -
#   cmake -S sources/fastfetch -B build/fastfetch $(FASTFETCH_CMAKE_FLAGS)
#   cmake --build build/fastfetch -j$(nproc)
# then copy build/fastfetch/{fastfetch,flashfetch} over the prebuilt/ copies.
$(ROOTFS_DIR)/bin/fastfetch: sources/fastfetch/prebuilt/fastfetch
	$(QUIET_GEN)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)cp $< $@

$(ROOTFS_DIR)/bin/flashfetch: sources/fastfetch/prebuilt/flashfetch
	$(QUIET_GEN)
	$(Q)mkdir -p $(ROOTFS_DIR)/bin
	$(Q)cp $< $@

# fastfetch/flashfetch moved out of the base image, install via `tpm install fastfetch`

# ---- rootfs image ----

$(ROOTFS_IMG): $(USER_ELFS) $(USER_TOOLS) $(ROOTFS_FILES) limine.conf \
              limine-bin/BOOTX64.EFI
	$(QUIET_GEN)
	$(Q)mkdir -p $(ROOTFS_DIR)/dev $(ROOTFS_DIR)/tmp $(ROOTFS_DIR)/etc $(ROOTFS_DIR)/bin $(ROOTFS_DIR)/lib/firmware/ath9k_htc $(ROOTFS_DIR)/etc/highx $(ROOTFS_DIR)/video $(ROOTFS_DIR)/boot $(ROOTFS_DIR)/var/mail
	# musl's headers, static lib and crt objects, staged for the
	# on-target PCC port (STDINC/DEFLIBDIRS in
	# sources/pcc/os/tus/ccconfig.h point cc/cpp/ccom at these).
	$(Q)mkdir -p $(ROOTFS_DIR)/usr/include $(ROOTFS_DIR)/usr/lib
	$(Q)cp -r $(MUSL_INC)/. $(ROOTFS_DIR)/usr/include/
	$(Q)cp $(MUSL_LIB)/crt1.o $(MUSL_LIB)/crti.o $(MUSL_LIB)/crtn.o $(MUSL_LIB)/libc.a $(ROOTFS_DIR)/usr/lib/
	# The installer needs the bootloader and its configuration as
	# FILES (the kernel and the root filesystem it takes from memory,
	# /dev/kernel and /dev/rootfs - an image cannot contain itself).
	$(Q)cp limine-bin/BOOTX64.EFI $(ROOTFS_DIR)/boot/BOOTX64.EFI
	$(Q)cp limine.conf $(ROOTFS_DIR)/boot/limine.conf
	# SUID bits must be set on the image, exactly like a real initramfs
	# build script: doas and passwd need root privileges (the kernel
	# stores the modes; ls -l shows the 's' bit).
	$(Q)chmod 755 $(ROOTFS_DIR)/bin/hello $(ROOTFS_DIR)/bin/enforce \
                  $(ROOTFS_DIR)/bin/fault \
                  $(ROOTFS_DIR)/bin/musl_hello $(ROOTFS_DIR)/bin/socktest \
                  $(ROOTFS_DIR)/bin/kilo \
                  $(ROOTFS_DIR)/bin/useradd $(ROOTFS_DIR)/bin/login \
                  $(ROOTFS_DIR)/bin/grep $(ROOTFS_DIR)/bin/sed \
                  $(ROOTFS_DIR)/bin/echo $(ROOTFS_DIR)/bin/ping \
                  $(ROOTFS_DIR)/bin/ping6 \
                  $(ROOTFS_DIR)/bin/nc \
                  $(ROOTFS_DIR)/bin/ifconfig $(ROOTFS_DIR)/bin/dhcp $(ROOTFS_DIR)/bin/netstat \
                  $(ROOTFS_DIR)/bin/arp $(ROOTFS_DIR)/bin/route \
                  $(ROOTFS_DIR)/bin/hostname $(ROOTFS_DIR)/bin/host \
                  $(ROOTFS_DIR)/bin/res_set $(ROOTFS_DIR)/bin/keymap \
                  $(ROOTFS_DIR)/bin/readelf $(ROOTFS_DIR)/bin/nm $(ROOTFS_DIR)/bin/ld \
                  $(ROOTFS_DIR)/bin/tuswm $(ROOTFS_DIR)/bin/tusde \
                  $(ROOTFS_DIR)/bin/hxdemo $(ROOTFS_DIR)/bin/hxcube $(ROOTFS_DIR)/bin/hxfont \
                  $(ROOTFS_DIR)/bin/lvgldemo \
                  $(ROOTFS_DIR)/bin/hxclock $(ROOTFS_DIR)/bin/hxterm \
                  $(ROOTFS_DIR)/bin/hxtsh $(ROOTFS_DIR)/bin/hxfiles \
                  $(ROOTFS_DIR)/bin/hxlogin $(ROOTFS_DIR)/bin/tusinstall \
                  $(ROOTFS_DIR)/bin/mail \
                  $(ROOTFS_DIR)/bin/hxmenu $(ROOTFS_DIR)/bin/hxvideo \
                  $(ROOTFS_DIR)/bin/fetch $(ROOTFS_DIR)/bin/wget \
                  $(ROOTFS_DIR)/bin/clint \
                  $(ROOTFS_DIR)/bin/wavplay $(ROOTFS_DIR)/bin/hxwavplayer \
                  $(ROOTFS_DIR)/bin/ls $(ROOTFS_DIR)/bin/cat \
                  $(ROOTFS_DIR)/bin/mkdir $(ROOTFS_DIR)/bin/touch \
                  $(ROOTFS_DIR)/bin/rm $(ROOTFS_DIR)/bin/mv \
                  $(ROOTFS_DIR)/bin/cp $(ROOTFS_DIR)/bin/head \
                  $(ROOTFS_DIR)/bin/tail $(ROOTFS_DIR)/bin/wc \
                  $(ROOTFS_DIR)/bin/pwd $(ROOTFS_DIR)/bin/uptime \
                  $(ROOTFS_DIR)/bin/sleep $(ROOTFS_DIR)/bin/date \
                  $(ROOTFS_DIR)/bin/whoami $(ROOTFS_DIR)/bin/id \
                  $(ROOTFS_DIR)/bin/df $(ROOTFS_DIR)/bin/kill \
                  $(ROOTFS_DIR)/bin/ps $(ROOTFS_DIR)/bin/pkill \
                  $(ROOTFS_DIR)/bin/clear \
                  $(ROOTFS_DIR)/bin/pty $(ROOTFS_DIR)/bin/tpm \
                  $(ROOTFS_DIR)/bin/mkfs.wrf $(ROOTFS_DIR)/bin/mkswap \
                  $(ROOTFS_DIR)/bin/tussm $(ROOTFS_DIR)/bin/errord \
                  $(ROOTFS_DIR)/bin/bootd
	$(Q)chmod 4555 $(ROOTFS_DIR)/bin/doas $(ROOTFS_DIR)/bin/passwd
	# Password hashes are root-only on every real Unix; the VFS
	# permission model actually enforces this now (see vfs_access_ok()
	# in kernel/vfs/vfs.c), so a checkout's own umask can no longer be
	# trusted to leave this file world-unreadable in the image.
	$(Q)chmod 600 $(ROOTFS_DIR)/etc/shadow
	$(Q)tar --format=ustar -C $(ROOTFS_DIR) -cf $@ .

# Header dependency tracking: rebuild objects when the headers they
# include change (without this, editing a .h silently tests a stale
# kernel).
-include $(DEPS)

# Assemble the hybrid BIOS/UEFI ISO from the limine-bin/ files.
# rootfs.img is passed to the kernel as a Limine module (see
# limine.conf: module_path) and becomes the initial root filesystem.
iso: kernel.elf $(ROOTFS_IMG)
	$(QUIET_GEN)
	$(Q)rm -rf iso_root
	$(Q)mkdir -p iso_root/boot iso_root/EFI/BOOT
	$(Q)cp kernel.elf iso_root/boot/kernel.elf
	$(Q)cp $(ROOTFS_IMG) iso_root/boot/rootfs.img
	$(Q)cp limine.conf iso_root/boot/limine.conf
	$(Q)cp limine-bin/limine-bios.sys limine-bin/limine-bios-cd.bin \
           limine-bin/limine-uefi-cd.bin iso_root/boot/
	$(Q)cp limine-bin/BOOTX64.EFI iso_root/EFI/BOOT/
	$(Q)xorriso -as mkisofs -b boot/limine-bios-cd.bin -no-emul-boot \
                -boot-load-size 4 -boot-info-table \
                --efi-boot boot/limine-uefi-cd.bin \
                -efi-boot-part --efi-boot-image --protective-msdos-label \
                iso_root -o tus.iso
	$(Q)./limine-bin/limine bios-install tus.iso || true

run: iso
	$(Q)qemu-system-x86_64 -cdrom tus.iso -m 512M -no-reboot -serial stdio

# The boot splash draws one toast per CPU; boot with several vCPUs to
# see the row of toasts (APs are parked by the kernel, see main.c).
run-smp: iso
	$(Q)qemu-system-x86_64 -cdrom tus.iso -m 512M -smp 4 -no-reboot -serial stdio

test: iso
	$(Q)python3 tests/test_boot.py

# highX window system: the automated session test (boots the ISO,
# starts highx/tusWM, drives the window manager by keyboard and checks
# the framebuffer) and the host-side compositor unit tests.
test-highx: iso
	$(Q)python3 tests/test_highx.py

test-de: iso
	$(Q)python3 tests/test_tusde.py

# res_set / SYS_VIDEO: mode changes at the console and under a live
# highX session, each checked against a QEMU screendump so a change
# that only moved the kernel's idea of the screen cannot pass.
test-res: iso
	$(Q)python3 tests/test_res.py

# USB keyboard and mouse over xHCI. Attaches a qemu-xhci controller
# with a usb-kbd and usb-mouse and checks that keystrokes and pointer
# movement really arrive - the USB driver's own key counter settles
# whether they came over USB or over the PS/2 port next to it.
test-usb: iso
	$(Q)python3 tests/test_usb.py

# Keyboard layouts. The harness calibrates itself against the machine
# (it presses physical keys and reads back what they produced), so the
# layout tables are under test rather than duplicated in the test.
test-keymap: iso
	$(Q)python3 tests/test_keymap.py

# Host tests for the layout tables themselves: every character every
# layout can produce is checked against the font the console draws
# from, so a key that types something invisible fails the build rather
# than a user.
test-layouts:
	$(Q)$(MAKE) -C tests/keymap run

# Host tests for the TrueType engine: the rasteriser against a
# brute-force model, and a fuzz pass over truncated and corrupted font
# files under AddressSanitizer.
test-font:
	$(Q)$(MAKE) -C tests/font run

# Host tests for HighGL: the rasteriser against a brute-force model,
# matrices, the perspective divide, depth, perspective-correct
# texturing, culling, blending, the hooks, misuse - and a spinning lit
# 3D triangle written to /tmp/hgl_triangle3d_*.ppm as visual proof.
test-highgl:
	$(Q)$(MAKE) -C tests/highgl run

test-compositor:
	$(Q)$(MAKE) -C tests/highx run

# Framebuffer text console: the pixels are checked against the text
# grid behind them, and the escape-sequence engine against the rules a
# terminal has to follow (deferred wrap most of all).
test-fb:
	$(Q)$(MAKE) -C tests/fb run

# MP4 demuxer + H.264 decoder, exercised on the build host against a
# real file (the one shipped in rootfs/video/).
test-mp4:
	$(Q)$(MAKE) -C tests/mp4 run

# Terminal sessions (hxtsh runs the kernel's own tsh in a window),
# the file manager and the mouse wheel - all three in one boot.
test-term: iso
	$(Q)python3 tests/test_term.py

# The greeter, the installer, and a boot of the disk it just wrote.
# The last part needs UEFI firmware for QEMU (/usr/share/OVMF); the
# test says so and skips it when there is none.
test-install: iso
	$(Q)python3 tests/test_install.py

# Shell behaviour of tsh and hxterm: quoting, redirection, pipes and
# the command history.
test-shell: iso
	$(Q)python3 tests/test_shell.py

# Clint's engine - parser, cascade, layout - on the build host. It is
# text in and boxes out, so none of it needs a boot to test.
test-clint:
	$(Q)$(MAKE) -C tests/clint run

test-js:
	$(Q)$(MAKE) -C tests/js run

# The crypto behind ssh, checked on the build host against the RFC
# test vectors.
test-crypto:
	$(Q)$(MAKE) -C tests/crypto run

# The SSH transport against the OpenSSH installed on this machine, in
# both directions. Interoperability is the part that cannot be tested
# against ourselves.
test-ssh-interop:
	$(Q)$(MAKE) -C tests/ssh run

# Boot the ISO and have the guest ssh out to an sshd on this host:
# ssh-keygen, public key authentication and a remote command, over
# the TUS network stack.
test-ssh: iso
	$(Q)python3 tests/test_ssh.py

# Boot the ISO and play the shipped MP4 with hxvideo.
test-video: iso
	$(Q)python3 tests/test_video.py

# Convert a video into something hxvideo can play and put it in the
# root filesystem: H.264 baseline (no B-frames, one reference frame),
# no audio (TUS has no sound driver) and scaled down, because every
# frame is decoded in software.
#
#   make video FILE=~/clip.mkv                 (320 px wide)
#   make video FILE=~/clip.mkv WIDTH=176 NAME=small.mp4
WIDTH ?= 320
NAME  ?= $(notdir $(basename $(FILE))).mp4
video:
	$(Q)test -n "$(FILE)" || (echo "usage: make video FILE=<input> [WIDTH=320] [NAME=out.mp4]"; false)
	$(Q)mkdir -p $(ROOTFS_DIR)/video
	$(Q)ffmpeg -y -i "$(FILE)" -an \
                -c:v libx264 -profile:v baseline -bf 0 -refs 1 -pix_fmt yuv420p \
                -vf "scale=$(WIDTH):-2" -g 30 \
                $(ROOTFS_DIR)/video/$(NAME)
	$(Q)echo "added $(ROOTFS_DIR)/video/$(NAME) - run 'make iso', then in TUS:"
	$(Q)echo "    highx hxvideo /video/$(NAME)"

clean:
	$(Q)rm -rf $(BUILD) kernel.elf tus.iso iso_root $(ROOTFS_IMG)
	$(Q)rm -f $(ROOTFS_DIR)/bin/*
	$(Q)$(MAKE) -C tests/highx clean
	$(Q)$(MAKE) -C tests/mp4 clean

# Remove the musl build too (keeps the source tree, drops obj/ lib/
# and the installed musl-out/).
clean-musl:
	$(Q)$(MAKE) -C $(MUSL_DIR) clean
	$(Q)rm -rf $(MUSL_OUT)
