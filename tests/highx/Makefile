# Host-side unit tests for the highX compositor.
#
#   make -C tests/highx run
#
# The compositor is compiled straight from kernel/highx/ - no copy, no
# reimplementation - against the stubs in this directory.

CC      ?= cc
# -DHXCOMP_TRACE_WRITES turns on the screen-write counter the
# flicker test needs. It compiles to nothing in the kernel.
CFLAGS  := -std=gnu11 -Wall -Wextra -O1 -g -I../../include \
           -DHXCOMP_TRACE_WRITES

OBJS := test_compositor.o stubs.o ../../kernel/highx/compositor.o

.PHONY: run clean

run: test_compositor
	./test_compositor

test_compositor: test_compositor.c stubs.c ../../kernel/highx/compositor.c \
                 ../../kernel/highx/highx.h ../../include/highx.h stubs.h
	$(CC) $(CFLAGS) -o $@ test_compositor.c stubs.c \
		../../kernel/highx/compositor.c

clean:
	rm -f test_compositor
