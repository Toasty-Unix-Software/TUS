# Host build of the SSH transport, for interoperating with OpenSSH.
#
# The same sources the TUS binaries use, compiled against the host
# libc: what is under test is the protocol, and the protocol does not
# change with the C library underneath it.

CFLAGS := -O2 -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE \
          -I../../userspace/ssh -I../../userspace/tuscrypt \
          -DTUSCRYPT_HOST_TEST -DTUS_HOST_BUILD

SSH_SRCS := ../../userspace/ssh/sshbuf.c \
            ../../userspace/ssh/sshtrans.c \
            ../../userspace/ssh/sshkey.c \
            ../../userspace/ssh/sshchan.c \
            ../../userspace/tuscrypt/hash.c \
            ../../userspace/tuscrypt/cipher.c \
            ../../userspace/tuscrypt/curve25519.c \
            ../../userspace/tuscrypt/util.c

all: kexprobe keytest tus-ssh

kexprobe: kexprobe.c $(SSH_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

keytest: keytest.c $(SSH_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

# The client itself, built for the host so it can be pointed at a
# real sshd.
tus-ssh: ../../userspace/ssh/ssh_client.c $(SSH_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	./interop.sh

clean:
	rm -f kexprobe keytest tus-ssh

.PHONY: all run clean
