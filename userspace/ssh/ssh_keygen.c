/*
 * ssh-keygen - make and inspect Ed25519 keys
 *
 *   ssh-keygen [-f file] [-C comment]   create a key pair
 *   ssh-keygen -y -f file               print the public half
 *   ssh-keygen -l -f file               print the fingerprint
 *
 * Only Ed25519, and only unencrypted: a passphrase would need a KDF
 * (bcrypt) that nothing else in TUS has a use for yet. The files it
 * writes are OpenSSH's own formats, so they can be copied to another
 * machine and used there unchanged.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ssh.h"
#include "tuscrypt.h"

static void fingerprint(const struct ssh_key *k, char *out, size_t size) {
    struct sshbuf blob;
    uint8_t digest[SHA256_DIGEST_SIZE];
    char b64[64];

    sshbuf_init(&blob);
    ssh_key_blob(k, &blob);
    sha256(blob.data, blob.len, digest);
    sshbuf_free(&blob);

    sshbuf_b64_encode(digest, sizeof(digest), b64, sizeof(b64));
    char *pad = strchr(b64, '=');
    if (pad) *pad = '\0';
    snprintf(out, size, "SHA256:%s", b64);
}

static int load(struct ssh_key *k, const char *path) {
    int r = ssh_key_read_private_file(k, path);
    if (r == 0) return 0;

    /* Failing to load may just mean this is the public half, which is
     * enough for -y and -l. */
    FILE *f = fopen(path, "r");
    if (f) {
        char line[1024];
        if (fgets(line, sizeof(line), f) && ssh_key_parse_pub_line(k, line) == 0) {
            fclose(f);
            return 0;
        }
        fclose(f);
    }

    fprintf(stderr, "ssh-keygen: cannot load %s%s\n", path,
            r == -2 ? " (permissions are too open)"
                    : r == -3 ? " (it is encrypted)" : "");
    return -1;
}

static void usage(void) {
    fprintf(stderr, "usage: ssh-keygen [-f file] [-C comment]\n"
                    "       ssh-keygen -y -f file\n"
                    "       ssh-keygen -l -f file\n");
    exit(2);
}

int main(int argc, char **argv) {
    const char *path = NULL, *comment = NULL;
    int show_public = 0, show_fingerprint = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            path = argv[++i];
        } else if (strcmp(argv[i], "-C") == 0 && i + 1 < argc) {
            comment = argv[++i];
        } else if (strcmp(argv[i], "-y") == 0) {
            show_public = 1;
        } else if (strcmp(argv[i], "-l") == 0) {
            show_fingerprint = 1;
        } else {
            usage();
        }
    }

    const char *home = getenv("HOME");
    if (!home || !*home) home = "/root";

    char default_path[512];
    snprintf(default_path, sizeof(default_path), "%s/.ssh/id_ed25519", home);
    if (!path) path = default_path;

    struct ssh_key k;
    char line[512], fp[128];

    if (show_public || show_fingerprint) {
        if (load(&k, path) != 0) return 1;
        if (show_fingerprint) {
            fingerprint(&k, fp, sizeof(fp));
            printf("256 %s %s (ED25519)\n", fp, path);
        }
        if (show_public) {
            if (ssh_key_write_pub_line(&k, NULL, line, sizeof(line)) != 0) {
                return 1;
            }
            fputs(line, stdout);
        }
        crypto_wipe(&k, sizeof(k));
        return 0;
    }

    /* Generating: never overwrite an existing key without being told
     * to. A lost private key is not recoverable. */
    if (access(path, F_OK) == 0) {
        fprintf(stderr, "ssh-keygen: %s already exists\n", path);
        return 1;
    }

    char dir[512];
    snprintf(dir, sizeof(dir), "%s", path);
    char *slash = strrchr(dir, '/');
    if (slash) {
        *slash = '\0';
        mkdir(dir, 0700);
    }

    uint8_t seed[ED25519_SEED_SIZE];
    crypto_random(seed, sizeof(seed));
    memset(&k, 0, sizeof(k));
    ed25519_keypair(k.pub, k.priv, seed);
    k.have_priv = 1;
    crypto_wipe(seed, sizeof(seed));

    char default_comment[128];
    if (!comment) {
        const char *user = getenv("USER");
        char host[64] = "tus";
        gethostname(host, sizeof(host) - 1);
        snprintf(default_comment, sizeof(default_comment), "%s@%s",
                 user && *user ? user : "root", host);
        comment = default_comment;
    }

    if (ssh_key_write_private_file(&k, comment, path) != 0) {
        fprintf(stderr, "ssh-keygen: cannot write %s\n", path);
        return 1;
    }

    char pub_path[600];
    snprintf(pub_path, sizeof(pub_path), "%s.pub", path);
    if (ssh_key_write_pub_line(&k, comment, line, sizeof(line)) != 0) {
        return 1;
    }

    FILE *f = fopen(pub_path, "w");
    if (!f) {
        fprintf(stderr, "ssh-keygen: cannot write %s\n", pub_path);
        return 1;
    }
    fputs(line, f);
    fclose(f);

    fingerprint(&k, fp, sizeof(fp));
    printf("Your identification has been saved in %s\n", path);
    printf("Your public key has been saved in %s\n", pub_path);
    printf("The key fingerprint is:\n%s %s\n", fp, comment);

    crypto_wipe(&k, sizeof(k));
    return 0;
}
