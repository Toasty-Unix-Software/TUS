# Host build of Clint's engine: parser, cascade and layout.
#
# None of it touches TUS - it is text in, boxes out - so it is tested
# here rather than by booting.

CFLAGS := -std=gnu11 -Wall -Wextra -O1 -g -I../../userspace/clint

SRCS := ../../userspace/clint/html.c \
        ../../userspace/clint/css.c \
        ../../userspace/clint/layout.c \
        ../../userspace/clint/image.c \
        ../../userspace/clint/inflate.c \
        ../../userspace/clint/font.c \
        ../../userspace/clint/charset.c

dump: dump.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ dump.c $(SRCS)

run: dump
	./run_tests.sh

clean:
	rm -f dump

.PHONY: run clean
