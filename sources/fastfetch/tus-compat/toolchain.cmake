# CMake toolchain file for cross-building fastfetch against musl for
# TUS - invoked by the top-level Makefile's fastfetch rule, not run
# standalone. See the "fastfetch" section of that Makefile for why
# CMAKE_SYSTEM_NAME is Linux, and the rest of this tus-compat/ tree
# for the header stubs it needs.
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER x86_64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER x86_64-linux-gnu-g++)
set(CMAKE_AR x86_64-linux-gnu-ar)
set(CMAKE_RANLIB x86_64-linux-gnu-ranlib)

set(TUS_MUSL_INC "/home/pi/projects/TUS/musl-out/usr/include")
set(TUS_COMPAT_INC "/home/pi/projects/TUS/sources/fastfetch/tus-compat")
set(CMAKE_C_FLAGS_INIT "-m64 -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -nostdinc -isystem /usr/lib/gcc-cross/x86_64-linux-gnu/14/include -isystem ${TUS_COMPAT_INC} -isystem ${TUS_MUSL_INC} -D__linux__ -D__tus__")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_C_FLAGS_INIT}")

set(CMAKE_FIND_ROOT_PATH "/home/pi/projects/TUS/musl-out")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TUS_MUSL_LIB "/home/pi/projects/TUS/musl-out/usr/lib")
# -B redirects gcc's own automatic crt1.o/crti.o/crtn.o search to musl's,
# instead of hand-ordering -nostdlib link flags (crt1.o must precede the
# program's objects, -lc/crtn.o must follow them - gcc's own driver
# already gets this right; only the search path needs to move).
set(CMAKE_EXE_LINKER_FLAGS_INIT "-static -no-pie -B${TUS_MUSL_LIB} -L${TUS_MUSL_LIB} -Wl,-e,_start -Wl,-Ttext,0x10000000")
