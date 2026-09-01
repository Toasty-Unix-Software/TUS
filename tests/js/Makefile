# Clint's JavaScript, built for the host: the interpreter, the DOM
# bindings and the regular-expression engine, over the same HTML
# parser the browser uses.

CFLAGS := -std=gnu11 -Wall -Wextra -O1 -g -I../../userspace/clint

SRCS := ../../userspace/clint/js.c \
        ../../userspace/clint/jsdom.c \
        ../../userspace/clint/jsregex.c \
        ../../userspace/clint/html.c

jsrun: jsrun.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ jsrun.c $(SRCS)

run: jsrun
	./run_tests.sh

clean:
	rm -f jsrun

.PHONY: run clean
