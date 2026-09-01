# Host build of the tuscrypt test vectors. Nothing here is
# TUS-specific: the arithmetic is the same wherever it runs, and
# checking it on the host means a failure points at the algorithm
# rather than at the operating system underneath it.

CFLAGS := -O2 -Wall -Wextra -Wno-unused-parameter -std=c11 \
          -I../../userspace/tuscrypt -DTUSCRYPT_HOST_TEST

SRCS := test_crypto.c \
        ../../userspace/tuscrypt/hash.c \
        ../../userspace/tuscrypt/cipher.c \
        ../../userspace/tuscrypt/curve25519.c \
        ../../userspace/tuscrypt/util.c

test_crypto: $(SRCS) ../../userspace/tuscrypt/tuscrypt.h
	$(CC) $(CFLAGS) -o $@ $(SRCS)

run: test_crypto
	./test_crypto

clean:
	rm -f test_crypto

.PHONY: run clean
