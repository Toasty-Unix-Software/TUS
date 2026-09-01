# Host-side tests and benchmark for the framebuffer text console.
#
#   make -C tests/fb run
#
# kernel/drivers/fb/fb.c is compiled straight from the kernel tree - the
# test #includes it, so it can compare the pixels against the text
# buffer behind them.

CC      ?= cc
CFLAGS  := -std=gnu11 -Wall -Wextra -O2 -g -I../../include -I../../kernel

.PHONY: run clean

run: test_fb
	./test_fb

test_fb: test_fb.c ../../kernel/drivers/fb/fb.c ../../kernel/drivers/fb/fb.h \
         ../../kernel/drivers/fb/font8x16.h
	$(CC) $(CFLAGS) -o $@ test_fb.c

clean:
	rm -f test_fb
