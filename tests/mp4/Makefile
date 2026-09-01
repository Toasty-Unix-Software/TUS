# Host-side test for the MP4 demuxer and the vendored H.264 decoder.
#
#   make -C tests/mp4 run                    (uses tests/mp4/sample.mp4)
#   make -C tests/mp4 run FILE=other.mp4
#
# Both are compiled straight from the tree - userspace/mp4.c and
# sources/h264bsd/src - so this tests exactly what hxvideo runs.

CC     ?= cc
H264   := ../../sources/h264bsd/src
CFLAGS := -std=gnu11 -O2 -g -Wall -I$(H264)
SRCS   := test_mp4.c ../../userspace/mp4.c $(wildcard $(H264)/*.c)
FILE   ?= ../../rootfs/video/sample.mp4

.PHONY: run clean

run: test_mp4
	./test_mp4 $(FILE)

test_mp4: $(SRCS) ../../userspace/mp4.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	rm -f test_mp4 *.ppm
