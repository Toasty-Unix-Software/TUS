# Host tests for the TrueType engine. The library is compiled straight
# in, the way tests/fb compiles the console: the code under test is
# the code that ships, and the brute-force model needs the internals.
# -fsanitize=address is the point of the damaged-font section: a font
# file is data from outside the program, and an out-of-bounds read
# there should fail the test loudly rather than quietly return a byte
# of something else.
CFLAGS := -std=gnu11 -Wall -Wextra -O2 -g -fsanitize=address,undefined

all: test_font

run: test_font
	./test_font

test_font: test_font.c \
           ../../userspace/tusfont/ttf.c \
           ../../userspace/tusfont/raster.c \
           ../../userspace/tusfont/text.c \
           ../../userspace/tusfont/tusfont.h \
           ../../userspace/tusfont/ttf_internal.h
	$(CC) $(CFLAGS) -o $@ test_font.c

clean:
	rm -f test_font

.PHONY: all run clean
