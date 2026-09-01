# tus_toolchain.cmake - build Mbed TLS the way TUS builds userspace
#
# Two things make this a cross build even though both sides are
# Linux-shaped: the architecture (this Pi is ARM, TUS is x86-64) and
# the C library (the musl in musl-out/, reached with -nostdinc so the
# host's headers cannot creep in).
#
# CMAKE_SYSTEM_NAME=Generic says there is no operating system to link
# a test program against, which is the truth here: a TUS binary is
# linked by hand against crt1.o and libc.a, so CMake's usual "compile
# and run a probe" would fail every check it makes.
#
# TUS_MUSL_INC and TUS_CROSS_PREFIX come from the Makefile.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

if(NOT DEFINED TUS_CROSS_PREFIX)
    set(TUS_CROSS_PREFIX "x86_64-linux-gnu-")
endif()

set(CMAKE_C_COMPILER "${TUS_CROSS_PREFIX}gcc")
set(CMAKE_AR         "${TUS_CROSS_PREFIX}ar")
set(CMAKE_RANLIB     "${TUS_CROSS_PREFIX}ranlib")

# Probes are compiled, never linked.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS_INIT
    "-m64 -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -mgeneral-regs-only -O2 -nostdinc -isystem ${TUS_MUSL_INC}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BEFORE)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
