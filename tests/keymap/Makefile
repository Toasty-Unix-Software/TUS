# Host tests for the keyboard layouts. keymap.c is compiled straight
# into the test, the way tests/fb compiles the console: the tables
# under test are the ones the kernel ships, not a copy.
CFLAGS := -std=gnu11 -Wall -Wextra -O2 -g -I../../include -I../../kernel

all: test_layouts

run: test_layouts
	./test_layouts

test_layouts: test_layouts.c ../../kernel/drivers/keymap/keymap.c \
              ../../kernel/drivers/fb/font_latin.h \
              ../../kernel/drivers/fb/font8x16.h
	$(CC) $(CFLAGS) -o $@ test_layouts.c

clean:
	rm -f test_layouts

.PHONY: all run clean
