# Host tests for HighGL. The library is compiled straight in, the way
# tests/fb compiles the console: the code under test is the code that
# ships, and the model needs to reach the buffers directly.
#
# Sanitisers on: a rasteriser walks arrays by computed index, which is
# exactly where an off-by-one writes into the next scanline and looks
# like a rendering artefact rather than a bug.
CFLAGS := -std=gnu11 -Wall -Wextra -O2 -g -fsanitize=address,undefined

all: test_highgl

run: test_highgl
	./test_highgl

test_highgl: test_highgl.c \
             ../../userspace/highgl/context.c \
             ../../userspace/highgl/pipeline.c \
             ../../userspace/highgl/raster.c \
             ../../userspace/highgl/texture.c \
             ../../userspace/highgl/highgl.h \
             ../../userspace/highgl/hgl_internal.h
	$(CC) $(CFLAGS) -o $@ test_highgl.c -lm

clean:
	rm -f test_highgl

.PHONY: all run clean
