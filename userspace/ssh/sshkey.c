/*
 * sshkey.c - Ed25519 keys in the shapes SSH stores and sends them
 *
 * Three encodings for one 32-byte public key, and it matters that all
 * three are exact, because every one of them is compared byte for
 * byte against something OpenSSH produced:
 *
 *   the blob      "ssh-ed25519" || key, what travels on the wire and
 *                 what known_hosts and authorized_keys hold in base64
 *   the pub line  "ssh-ed25519 <base64 blob> <comment>"
 *   the key file  OpenSSH's own container, "openssh-key-v1", which is
 *                 written here only in its unencrypted form
 *
 * A private key file is written with mode 0600 and refused on read if
 * it is more permissive, the same rule OpenSSH enforces - a key
 * readable by everyone on the machine is not a private key.
 */

#include "ssh.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "tuscrypt.h"

void ssh_key_blob(const struct ssh_key *k, struct sshbuf *out) {
    sshbuf_put_cstring(out, SSH_ED25519_NAME);
    sshbuf_put_string(out, k->pub, sizeof(k->pub));
}

int ssh_key_from_blob(struct ssh_key *k, const uint8_t *blob, size_t len) {
    struct sshbuf b;
    const char *type;
    const uint8_t *pub;
    size_t type_len, pub_len;

    memset(k, 0, sizeof(*k));

    /* Parsing borrows the caller's bytes: the buffer is a view, and
     * sshbuf_free() must not be called on it. */
    b.data = (uint8_t *)blob;
    b.len = len;
    b.cap = len;
    b.off = 0;
    b.error = 0;

    if (sshbuf_get_cstring(&b, &type, &type_len) != 0) return -1;
    if (type_len != strlen(SSH_ED25519_NAME) ||
        memcmp(type, SSH_ED25519_NAME, type_len) != 0) {
        return -1;
    }
    if (sshbuf_get_string(&b, &pub, &pub_len) != 0) return -1;
    if (pub_len != 32) return -1;
    if (sshbuf_remaining(&b) != 0) return -1; /* trailing junk */

    memcpy(k->pub, pub, 32);
    return 0;
}

void ssh_key_sign(const struct ssh_key *k, const void *data, size_t len,
                  struct sshbuf *out) {
    uint8_t sig[ED25519_SIGNATURE_SIZE];

    ed25519_sign(sig, data, len, k->pub, k->priv);
    sshbuf_put_cstring(out, SSH_ED25519_NAME);
    sshbuf_put_string(out, sig, sizeof(sig));
}

int ssh_key_verify(const struct ssh_key *k, const uint8_t *sig, size_t sig_len,
                   const void *data, size_t len) {
    struct sshbuf b;
    const char *type;
    const uint8_t *raw;
    size_t type_len, raw_len;

    b.data = (uint8_t *)sig;
    b.len = sig_len;
    b.cap = sig_len;
    b.off = 0;
    b.error = 0;

    if (sshbuf_get_cstring(&b, &type, &type_len) != 0) return -1;
    if (type_len != strlen(SSH_ED25519_NAME) ||
        memcmp(type, SSH_ED25519_NAME, type_len) != 0) {
        return -1;
    }
    if (sshbuf_get_string(&b, &raw, &raw_len) != 0) return -1;
    if (raw_len != ED25519_SIGNATURE_SIZE) return -1;

    return ed25519_verify(raw, data, len, k->pub);
}

/* ---- the one-line public form ---- */

int ssh_key_write_pub_line(const struct ssh_key *k, const char *comment,
                           char *out, size_t out_size) {
    struct sshbuf blob;
    char b64[128];

    sshbuf_init(&blob);
    ssh_key_blob(k, &blob);
    size_t n = sshbuf_b64_encode(blob.data, blob.len, b64, sizeof(b64));
    sshbuf_free(&blob);
    if (n == 0) return -1;

    int written = snprintf(out, out_size, "%s %s%s%s\n", SSH_ED25519_NAME, b64,
                           comment && *comment ? " " : "",
                           comment && *comment ? comment : "");
    return (written < 0 || (size_t)written >= out_size) ? -1 : 0;
}

int ssh_key_parse_pub_line(struct ssh_key *k, const char *line) {
    while (*line == ' ' || *line == '\t') line++;

    /* known_hosts puts the host pattern first; a line that does not
     * begin with the key type has one, so step over the first field
     * until the type is what is being looked at. */
    for (int field = 0; field < 3; field++) {
        if (strncmp(line, SSH_ED25519_NAME " ", strlen(SSH_ED25519_NAME) + 1) == 0) {
            break;
        }
        const char *sp = strchr(line, ' ');
        if (!sp) return -1;
        line = sp;
        while (*line == ' ') line++;
    }
    if (strncmp(line, SSH_ED25519_NAME " ", strlen(SSH_ED25519_NAME) + 1) != 0) {
        return -1;
    }
    line += strlen(SSH_ED25519_NAME) + 1;
    while (*line == ' ') line++;

    size_t n = strcspn(line, " \t\r\n");
    uint8_t blob[128];
    long blob_len = sshbuf_b64_decode(line, n, blob, sizeof(blob));
    if (blob_len < 0) return -1;

    return ssh_key_from_blob(k, blob, (size_t)blob_len);
}

/* ---- OpenSSH's private key container ---- */

static const char PEM_BEGIN[] = "-----BEGIN OPENSSH PRIVATE KEY-----";
static const char PEM_END[] = "-----END OPENSSH PRIVATE KEY-----";
static const char OPENSSH_MAGIC[] = "openssh-key-v1";

int ssh_key_write_private_file(const struct ssh_key *k, const char *comment,
                               const char *path) {
    struct sshbuf inner, priv, pub;
    int rc = -1;

    sshbuf_init(&inner);
    sshbuf_init(&priv);
    sshbuf_init(&pub);

    ssh_key_blob(k, &pub);

    /* The two check integers are equal in a correctly decrypted key;
     * they are how a wrong passphrase is noticed. Unencrypted keys
     * carry them anyway. */
    uint32_t check;
    crypto_random(&check, sizeof(check));
    sshbuf_put_u32(&priv, check);
    sshbuf_put_u32(&priv, check);
    sshbuf_put_cstring(&priv, SSH_ED25519_NAME);
    sshbuf_put_string(&priv, k->pub, 32);
    sshbuf_put_string(&priv, k->priv, 64);
    sshbuf_put_cstring(&priv, comment ? comment : "");

    /* Pad to the cipher's block size with 1, 2, 3, ... - the "none"
     * cipher still has a nominal block size of 8. */
    for (uint8_t i = 1; priv.len % 8 != 0; i++) sshbuf_put_u8(&priv, i);

    sshbuf_put(&inner, OPENSSH_MAGIC, sizeof(OPENSSH_MAGIC)); /* with its NUL */
    sshbuf_put_cstring(&inner, "none");
    sshbuf_put_cstring(&inner, "none");
    sshbuf_put_cstring(&inner, "");
    sshbuf_put_u32(&inner, 1);
    sshbuf_put_stringb(&inner, &pub);
    sshbuf_put_stringb(&inner, &priv);
    if (inner.error) goto out;

    size_t b64_size = ((inner.len + 2) / 3) * 4 + 1;
    char *b64 = malloc(b64_size);
    if (!b64) goto out;
    size_t b64_len = sshbuf_b64_encode(inner.data, inner.len, b64, b64_size);

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) {
        free(b64);
        goto out;
    }

    FILE *f = fdopen(fd, "w");
    if (!f) {
        close(fd);
        free(b64);
        goto out;
    }
    /* OpenSSH wraps the body at 70 columns. Stepping by 70 has to be
     * bounded by the real length: the last line is short, and walking
     * past it reads whatever follows the buffer. */
    fprintf(f, "%s\n", PEM_BEGIN);
    for (size_t i = 0; i < b64_len; i += 70) {
        size_t take = b64_len - i < 70 ? b64_len - i : 70;
        fprintf(f, "%.*s\n", (int)take, b64 + i);
    }
    fprintf(f, "%s\n", PEM_END);
    rc = fclose(f) == 0 ? 0 : -1;

    crypto_wipe(b64, b64_len);
    free(b64);

out:
    crypto_wipe(priv.data, priv.cap);
    sshbuf_free(&inner);
    sshbuf_free(&priv);
    sshbuf_free(&pub);
    return rc;
}

int ssh_key_read_private_file(struct ssh_key *k, const char *path) {
    struct stat st;
    int rc = -1;

    memset(k, 0, sizeof(*k));

    FILE *f = fopen(path, "r");
    if (!f) return -1;

    /* A key that the whole machine can read is not a private key, so
     * refuse one. Where the system cannot report a mode at all - TUS
     * has no stat(2) yet - there is nothing to check, and refusing
     * every key would be worse than checking none. */
    if (fstat(fileno(f), &st) == 0 && (st.st_mode & 077) != 0) {
        fclose(f);
        return -2;
    }

    char line[256];
    char *b64 = NULL;
    size_t b64_len = 0, b64_cap = 0;
    int in_body = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, PEM_BEGIN, strlen(PEM_BEGIN)) == 0) {
            in_body = 1;
            continue;
        }
        if (strncmp(line, PEM_END, strlen(PEM_END)) == 0) break;
        if (!in_body) continue;

        size_t n = strcspn(line, "\r\n");
        if (b64_len + n + 1 > b64_cap) {
            size_t want = b64_cap ? b64_cap * 2 : 1024;
            while (want < b64_len + n + 1) want *= 2;
            char *p = realloc(b64, want);
            if (!p) goto out_b64;
            b64 = p;
            b64_cap = want;
        }
        memcpy(b64 + b64_len, line, n);
        b64_len += n;
        b64[b64_len] = '\0';
    }
    if (!b64) goto out_file;

    uint8_t *raw = malloc(b64_len);
    if (!raw) goto out_b64;
    long raw_len = sshbuf_b64_decode(b64, b64_len, raw, b64_len);
    if (raw_len < 0) goto out_raw;

    struct sshbuf b;
    b.data = raw;
    b.len = (size_t)raw_len;
    b.cap = (size_t)raw_len;
    b.off = 0;
    b.error = 0;

    char magic[sizeof(OPENSSH_MAGIC)];
    if (sshbuf_get(&b, magic, sizeof(magic)) != 0) goto out_raw;
    if (memcmp(magic, OPENSSH_MAGIC, sizeof(magic)) != 0) goto out_raw;

    const char *cipher, *kdf;
    size_t cipher_len, kdf_len;
    if (sshbuf_get_cstring(&b, &cipher, &cipher_len) != 0) goto out_raw;
    if (sshbuf_get_cstring(&b, &kdf, &kdf_len) != 0) goto out_raw;
    if (sshbuf_skip_string(&b) != 0) goto out_raw; /* kdf options */

    if (cipher_len != 4 || memcmp(cipher, "none", 4) != 0 ||
        kdf_len != 4 || memcmp(kdf, "none", 4) != 0) {
        rc = -3; /* encrypted: this reader has no passphrase support */
        goto out_raw;
    }

    uint32_t nkeys;
    if (sshbuf_get_u32(&b, &nkeys) != 0 || nkeys != 1) goto out_raw;
    if (sshbuf_skip_string(&b) != 0) goto out_raw; /* the public blob */

    const uint8_t *sect;
    size_t sect_len;
    if (sshbuf_get_string(&b, &sect, &sect_len) != 0) goto out_raw;

    struct sshbuf pb;
    pb.data = (uint8_t *)sect;
    pb.len = sect_len;
    pb.cap = sect_len;
    pb.off = 0;
    pb.error = 0;

    uint32_t c1, c2;
    if (sshbuf_get_u32(&pb, &c1) != 0 || sshbuf_get_u32(&pb, &c2) != 0) {
        goto out_raw;
    }
    if (c1 != c2) goto out_raw;

    const char *type;
    const uint8_t *pub, *priv;
    size_t type_len, pub_len, priv_len;
    if (sshbuf_get_cstring(&pb, &type, &type_len) != 0) goto out_raw;
    if (type_len != strlen(SSH_ED25519_NAME) ||
        memcmp(type, SSH_ED25519_NAME, type_len) != 0) {
        goto out_raw;
    }
    if (sshbuf_get_string(&pb, &pub, &pub_len) != 0 || pub_len != 32) goto out_raw;
    if (sshbuf_get_string(&pb, &priv, &priv_len) != 0 || priv_len != 64) goto out_raw;

    /* The stored private half is seed || public. Recomputing the pair
     * from the seed catches a corrupt or mismatched file here rather
     * than as an unexplained authentication failure later. */
    uint8_t check_pub[32], check_priv[64];
    ed25519_keypair(check_pub, check_priv, priv);
    if (memcmp(check_pub, pub, 32) != 0) {
        crypto_wipe(check_priv, sizeof(check_priv));
        goto out_raw;
    }

    memcpy(k->pub, pub, 32);
    memcpy(k->priv, priv, 64);
    k->have_priv = 1;
    crypto_wipe(check_priv, sizeof(check_priv));
    rc = 0;

out_raw:
    crypto_wipe(raw, (size_t)(raw_len > 0 ? raw_len : 0));
    free(raw);
out_b64:
    if (b64) {
        crypto_wipe(b64, b64_len);
        free(b64);
    }
out_file:
    fclose(f);
    return rc;
}
