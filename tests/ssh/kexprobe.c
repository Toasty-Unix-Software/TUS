/*
 * kexprobe - drive the TUS SSH transport against a real OpenSSH
 *
 * The point of this program is interoperability, which is the only
 * property of a protocol implementation that self-testing cannot
 * establish: two of your own programs will agree on the same mistake.
 * So it runs one half of a connection and lets OpenSSH run the other.
 *
 *   kexprobe server <port>       accept once, be sshd through NEWKEYS
 *   kexprobe client <host> <port>  connect, be ssh through NEWKEYS
 *
 * Either way it prints "KEX OK" and the session id, then disconnects
 * cleanly. Anything else is a failure with a reason.
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ssh.h"
#include "tuscrypt.h"

static void print_hex(const char *label, const uint8_t *p, size_t n) {
    printf("%s ", label);
    for (size_t i = 0; i < n; i++) printf("%02x", p[i]);
    printf("\n");
}

static int accept_any_host_key(void *ctx, const struct ssh_key *k) {
    (void)ctx;
    print_hex("server host key", k->pub, 32);
    return 0;
}

static int run(int fd, int server, const struct ssh_key *hostkey) {
    struct ssh s;
    ssh_init(&s, fd, server);

    if (ssh_exchange_versions(&s, "TUS_1.0") != 0) {
        fprintf(stderr, "kexprobe: version exchange: %s\n", s.err);
        ssh_close(&s);
        return 1;
    }
    printf("peer version %s\n", s.v_peer);

    if (ssh_kex(&s, hostkey, server ? NULL : accept_any_host_key, NULL) != 0) {
        fprintf(stderr, "kexprobe: key exchange: %s\n", s.err);
        ssh_close(&s);
        return 1;
    }

    printf("KEX OK strict=%d\n", s.strict_kex);
    print_hex("session id", s.session_id, 32);
    fflush(stdout);

    /* A disconnect after NEWKEYS is the first encrypted packet either
     * side sends, so a peer that reads it has confirmed the keys. */
    ssh_send_disconnect(&s, SSH_DISCONNECT_PROTOCOL_ERROR,
                        "kexprobe has seen enough");
    ssh_close(&s);
    return 0;
}

int main(int argc, char **argv) {
    struct ssh_key hostkey;
    uint8_t seed[32];

    if (argc < 3) {
        fprintf(stderr, "usage: kexprobe server <port> | client <host> <port>\n");
        return 2;
    }

    memset(&hostkey, 0, sizeof(hostkey));
    crypto_random(seed, sizeof(seed));
    ed25519_keypair(hostkey.pub, hostkey.priv, seed);
    hostkey.have_priv = 1;

    if (strcmp(argv[1], "server") == 0) {
        int ls = socket(AF_INET, SOCK_STREAM, 0);
        int one = 1;
        setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

        struct sockaddr_in a;
        memset(&a, 0, sizeof(a));
        a.sin_family = AF_INET;
        a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        a.sin_port = htons((uint16_t)atoi(argv[2]));
        if (bind(ls, (struct sockaddr *)&a, sizeof(a)) != 0) {
            perror("bind");
            return 1;
        }
        listen(ls, 1);
        print_hex("our host key", hostkey.pub, 32);
        printf("listening\n");
        fflush(stdout);

        int fd = accept(ls, NULL, NULL);
        if (fd < 0) {
            perror("accept");
            return 1;
        }
        close(ls);
        return run(fd, 1, &hostkey);
    }

    if (strcmp(argv[1], "client") == 0 && argc >= 4) {
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(argv[2], argv[3], &hints, &res) != 0) {
            fprintf(stderr, "kexprobe: cannot resolve %s\n", argv[2]);
            return 1;
        }
        int fd = socket(res->ai_family, res->ai_socktype, 0);
        if (connect(fd, res->ai_addr, res->ai_addrlen) != 0) {
            perror("connect");
            return 1;
        }
        freeaddrinfo(res);
        return run(fd, 0, NULL);
    }

    fprintf(stderr, "usage: kexprobe server <port> | client <host> <port>\n");
    return 2;
}
