/*
 * keytest - check TUS key files against the ones ssh-keygen writes
 *
 * A key file format is only right if the other implementation agrees,
 * so this test never checks our writer against our reader alone:
 *
 *   read     parse a private key ssh-keygen produced and confirm the
 *            public half matches the .pub file beside it
 *   write    write a key of our own, so the script can hand it to
 *            ssh-keygen -y and compare what comes back
 *
 * Usage: keytest read <privkey> <pubfile>
 *        keytest write <privkey-out> <publine-out>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssh.h"
#include "tuscrypt.h"

static int read_first_line(const char *path, char *out, size_t out_size) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int ok = fgets(out, (int)out_size, f) != NULL;
    fclose(f);
    return ok ? 0 : -1;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "usage: keytest read|write <priv> <pub>\n");
        return 2;
    }

    if (strcmp(argv[1], "read") == 0) {
        struct ssh_key k, want;

        int r = ssh_key_read_private_file(&k, argv[2]);
        if (r != 0) {
            fprintf(stderr, "keytest: cannot read %s (%d)\n", argv[2], r);
            return 1;
        }

        char line[512];
        if (read_first_line(argv[3], line, sizeof(line)) != 0) {
            fprintf(stderr, "keytest: cannot read %s\n", argv[3]);
            return 1;
        }
        if (ssh_key_parse_pub_line(&want, line) != 0) {
            fprintf(stderr, "keytest: cannot parse the .pub line\n");
            return 1;
        }
        if (memcmp(k.pub, want.pub, 32) != 0) {
            fprintf(stderr, "keytest: the private key's public half does not "
                            "match the .pub file\n");
            return 1;
        }

        /* And the private half really signs for that public half. */
        uint8_t sig[64];
        const char msg[] = "keytest";
        ed25519_sign(sig, msg, sizeof(msg), k.pub, k.priv);
        if (ed25519_verify(sig, msg, sizeof(msg), want.pub) != 0) {
            fprintf(stderr, "keytest: the key does not sign for its own "
                            "public half\n");
            return 1;
        }
        printf("read ok\n");
        return 0;
    }

    if (strcmp(argv[1], "write") == 0) {
        struct ssh_key k;
        uint8_t seed[32];

        memset(&k, 0, sizeof(k));
        crypto_random(seed, sizeof(seed));
        ed25519_keypair(k.pub, k.priv, seed);
        k.have_priv = 1;

        if (ssh_key_write_private_file(&k, "keytest@tus", argv[2]) != 0) {
            fprintf(stderr, "keytest: cannot write %s\n", argv[2]);
            return 1;
        }

        char line[512];
        if (ssh_key_write_pub_line(&k, "keytest@tus", line, sizeof(line)) != 0) {
            fprintf(stderr, "keytest: cannot format the public line\n");
            return 1;
        }
        FILE *f = fopen(argv[3], "w");
        if (!f) {
            fprintf(stderr, "keytest: cannot write %s\n", argv[3]);
            return 1;
        }
        fputs(line, f);
        fclose(f);

        /* Reading our own file back is not proof of anything on its
         * own, but a failure here localises the bug to the writer. */
        struct ssh_key back;
        if (ssh_key_read_private_file(&back, argv[2]) != 0 ||
            memcmp(back.pub, k.pub, 32) != 0 ||
            memcmp(back.priv, k.priv, 64) != 0) {
            fprintf(stderr, "keytest: our own key file does not read back\n");
            return 1;
        }
        printf("write ok\n");
        return 0;
    }

    fprintf(stderr, "usage: keytest read|write <priv> <pub>\n");
    return 2;
}
