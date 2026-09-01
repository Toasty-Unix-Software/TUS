/*
 * test_crypto.c - tuscrypt against the published test vectors
 *
 * Runs on the host, not in TUS: the point is to check the arithmetic,
 * and the arithmetic is the same wherever it runs. Every vector here
 * comes from the document that defines the primitive - FIPS 180-4,
 * RFC 2104, RFC 8439, RFC 7748, RFC 8032 - so a passing run means the
 * implementation agrees with the standard rather than with itself.
 */

#include "tuscrypt.h"

#include <stdio.h>
#include <string.h>

static int failures;
static int checks;

static void hex_decode(const char *hex, uint8_t *out, size_t out_len) {
    for (size_t i = 0; i < out_len; i++) {
        unsigned int v;
        sscanf(hex + i * 2, "%2x", &v);
        out[i] = (uint8_t)v;
    }
}

static void check_bytes(const char *name, const uint8_t *got,
                        const char *expect_hex, size_t len) {
    uint8_t expect[512];
    hex_decode(expect_hex, expect, len);
    checks++;

    if (memcmp(got, expect, len) != 0) {
        failures++;
        printf("  [FAIL] %s\n         got      ", name);
        for (size_t i = 0; i < len; i++) printf("%02x", got[i]);
        printf("\n         expected %s\n", expect_hex);
        return;
    }
    printf("  [PASS] %s\n", name);
}

static void check_int(const char *name, long got, long expect) {
    checks++;
    if (got != expect) {
        failures++;
        printf("  [FAIL] %s (got %ld, expected %ld)\n", name, got, expect);
        return;
    }
    printf("  [PASS] %s\n", name);
}

/* ---- hashes ---- */

static void test_hashes(void) {
    uint8_t out[64];

    printf("SHA-1 (FIPS 180-4)\n");
    sha1("abc", 3, out);
    check_bytes("sha1(\"abc\")", out,
                "a9993e364706816aba3e25717850c26c9cd0d89d", 20);

    sha1("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56, out);
    check_bytes("sha1(56-byte string)", out,
                "84983e441c3bd26ebaae4aa1f95129e5e54670f1", 20);

    /* A million 'a's: the vector that catches a broken length field. */
    {
        struct sha1_ctx ctx;
        uint8_t block[1000];
        memset(block, 'a', sizeof(block));
        sha1_init(&ctx);
        for (int i = 0; i < 1000; i++) sha1_update(&ctx, block, sizeof(block));
        sha1_final(&ctx, out);
        check_bytes("sha1(1e6 x 'a')", out,
                    "34aa973cd4c4daa4f61eeb2bdbad27316534016f", 20);
    }

    printf("SHA-256 (FIPS 180-4)\n");
    sha256("abc", 3, out);
    check_bytes("sha256(\"abc\")", out,
                "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
                32);

    sha256("", 0, out);
    check_bytes("sha256(\"\")", out,
                "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
                32);

    sha256("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56, out);
    check_bytes("sha256(56-byte string)", out,
                "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",
                32);

    {
        struct sha256_ctx ctx;
        uint8_t block[1000];
        memset(block, 'a', sizeof(block));
        sha256_init(&ctx);
        for (int i = 0; i < 1000; i++) sha256_update(&ctx, block, sizeof(block));
        sha256_final(&ctx, out);
        check_bytes("sha256(1e6 x 'a')", out,
                    "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0",
                    32);
    }

    printf("SHA-512 (FIPS 180-4)\n");
    sha512("abc", 3, out);
    check_bytes("sha512(\"abc\")", out,
                "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
                "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f",
                64);

    sha512("", 0, out);
    check_bytes("sha512(\"\")", out,
                "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
                "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e",
                64);

    sha512("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
           "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", 112, out);
    check_bytes("sha512(112-byte string)", out,
                "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018"
                "501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909",
                64);
}

/* ---- HMAC ---- */

static void test_hmac(void) {
    uint8_t out[32];
    uint8_t key[131];

    printf("HMAC-SHA-256 (RFC 4231)\n");

    memset(key, 0x0b, 20);
    hmac_sha256(key, 20, "Hi There", 8, out);
    check_bytes("case 1", out,
                "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7",
                32);

    hmac_sha256("Jefe", 4, "what do ya want for nothing?", 28, out);
    check_bytes("case 2", out,
                "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843",
                32);

    memset(key, 0xaa, 131);
    hmac_sha256(key, 131, "Test Using Larger Than Block-Size Key - "
                          "Hash Key First", 54, out);
    check_bytes("case 6 (key longer than the block)", out,
                "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54",
                32);

    printf("HMAC-SHA-1 (RFC 2202)\n");
    {
        uint8_t out1[20];
        memset(key, 0x0b, 20);
        hmac_sha1(key, 20, "Hi There", 8, out1);
        check_bytes("case 1", out1,
                    "b617318655057264e28bc0b6fb378c8ef146be00", 20);
    }
}

/* ---- ChaCha20 and Poly1305 ---- */

static void test_chacha_poly(void) {
    printf("ChaCha20 (RFC 8439)\n");

    /* Our state lays out words 12-15 as counter-low, counter-high,
     * nonce-low, nonce-high, where the RFC uses counter plus a 96-bit
     * nonce. The two agree exactly when the RFC's first nonce word is
     * zero, which is true of both vectors below - so these check the
     * cipher itself, not a re-labelling of it. */
    {
        struct chacha20_ctx ctx;
        uint8_t key[32], nonce[8], out[64];

        memset(key, 0, sizeof(key));
        memset(nonce, 0, sizeof(nonce));
        memset(out, 0, sizeof(out));

        chacha20_init(&ctx, key, nonce, 0);
        chacha20_xor(&ctx, out, out, sizeof(out));
        check_bytes("A.1 vector 1 keystream", out,
                    "76b8e0ada0f13d90405d6ae55386bd28"
                    "bdd219b8a08ded1aa836efcc8b770dc7"
                    "da41597c5157488d7724e03fb8d84a37"
                    "6a43b8f41518a11cc387b669b2ee6586", 64);
    }

    {
        /* Section 2.4.2: the RFC's nonce 000000000000004a00000000 has
         * a zero first word, so it lands in our eight-byte nonce as
         * 000000 4a 00000000. */
        struct chacha20_ctx ctx;
        uint8_t key[32], nonce[8], cipher[114];
        const char *plain = "Ladies and Gentlemen of the class of '99: "
                            "If I could offer you only one tip for the "
                            "future, sunscreen would be it.";

        for (int i = 0; i < 32; i++) key[i] = (uint8_t)i;
        memset(nonce, 0, sizeof(nonce));
        nonce[3] = 0x4a;

        chacha20_init(&ctx, key, nonce, 1);
        chacha20_xor(&ctx, (const uint8_t *)plain, cipher, 114);
        check_bytes("2.4.2 encryption", cipher,
                    "6e2e359a2568f98041ba0728dd0d6981"
                    "e97e7aec1d4360c20a27afccfd9fae0b"
                    "f91b65c5524733ab8f593dabcd62b357"
                    "1639d624e65152ab8f530c359f0861d8"
                    "07ca0dbf500d6a6156a38e088a22b65e"
                    "52bc514d16ccf806818ce91ab7793736"
                    "5af90bbf74a35be6b40b8eedf2785e42"
                    "874d", 114);
    }

    printf("Poly1305 (RFC 8439 section 2.5.2)\n");
    {
        uint8_t key[32], tag[16];
        const char *msg = "Cryptographic Forum Research Group";

        hex_decode("85d6be7857556d337f4452fe42d506a8"
                   "0103808afb0db2fd4abff6af4149f51b", key, 32);
        poly1305(key, msg, strlen(msg), tag);
        check_bytes("tag", tag, "a8061dc1305136c6c22b8baf0c0127a9", 16);
    }

    printf("Poly1305 edge cases (RFC 8439 A.3)\n");
    {
        uint8_t key[32], tag[16], msg[64];

        /* An all-zero key must give an all-zero tag whatever the
         * message: r = 0 kills the polynomial and s = 0 adds nothing. */
        memset(key, 0, sizeof(key));
        memset(msg, 0, sizeof(msg));
        poly1305(key, msg, sizeof(msg), tag);
        check_bytes("zero key, zero message", tag,
                    "00000000000000000000000000000000", 16);

        /* r = 0, s set: the tag is s alone. */
        memset(key, 0, sizeof(key));
        hex_decode("36e5f6b5c5e06070f0efca96227a863e", key + 16, 16);
        poly1305(key, "Any submission to the IETF intended by the "
                      "Contributor for publication", 69 - 1, tag);
        check_bytes("r = 0, tag is s", tag,
                    "36e5f6b5c5e06070f0efca96227a863e", 16);
    }
}

/* ---- X25519 ---- */

static void test_x25519(void) {
    uint8_t scalar[32], point[32], out[32];

    printf("X25519 (RFC 7748 section 5.2)\n");

    hex_decode("a546e36bf0527c9d3b16154b82465edd62144c0ac1fc5a18506a2244ba449ac4",
               scalar, 32);
    hex_decode("e6db6867583030db3594c1a424b15f7c726624ec26b3353b10a903a6d0ab1c4c",
               point, 32);
    x25519(out, scalar, point);
    check_bytes("vector 1", out,
                "c3da55379de9c6908e94ea4df28d084f32eccf03491c71f754b4075577a28552",
                32);

    hex_decode("4b66e9d4d1b4673c5ad22691957d6af5c11b6421e0ea01d42ca4169e7918ba0d",
               scalar, 32);
    hex_decode("e5210f12786811d3f4b7959d0538ae2c31dbe7106fc03c3efc4cd549c715a493",
               point, 32);
    x25519(out, scalar, point);
    check_bytes("vector 2", out,
                "95cbde9476e8907d7aade45cb4b873f88b595a68799fa152e6f8f7647aac7957",
                32);

    printf("X25519 (RFC 7748 section 6.1, a full exchange)\n");
    {
        uint8_t alice_sk[32], bob_sk[32], alice_pk[32], bob_pk[32];
        uint8_t shared_a[32], shared_b[32];

        hex_decode("77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a",
                   alice_sk, 32);
        hex_decode("5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb",
                   bob_sk, 32);

        x25519_base(alice_pk, alice_sk);
        check_bytes("Alice's public key", alice_pk,
                    "8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a",
                    32);

        x25519_base(bob_pk, bob_sk);
        check_bytes("Bob's public key", bob_pk,
                    "de9edb7d7b7dc1b4d35b61c2ece435373f8343c85b78674dadfc7e146f882b4f",
                    32);

        x25519(shared_a, alice_sk, bob_pk);
        x25519(shared_b, bob_sk, alice_pk);
        check_bytes("the shared secret", shared_a,
                    "4a5d9d5ba4ce2de1728e3bf480350f25e07e21c947d19e3376f09b3c1e161742",
                    32);
        check_int("both sides agree", memcmp(shared_a, shared_b, 32), 0);
    }

    printf("X25519 refuses small-order points\n");
    {
        uint8_t zero_point[32];
        memset(zero_point, 0, sizeof(zero_point));
        memset(scalar, 0x11, sizeof(scalar));
        check_int("all-zero peer key rejected",
                  x25519(out, scalar, zero_point), -1);
    }
}

/* ---- Ed25519 ---- */

struct ed_vector {
    const char *seed;
    const char *public_key;
    const char *message;
    size_t message_len;
    const char *signature;
};

static void test_ed25519(void) {
    /* RFC 8032 section 7.1. */
    static const struct ed_vector vectors[] = {
        {
            "9d61b19deffd5a60ba844af492ec2cc44449c5697b326919703bac031cae7f60",
            "d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a",
            "", 0,
            "e5564300c360ac729086e2cc806e828a84877f1eb8e5d974d873e06522490155"
            "5fb8821590a33bacc61e39701cf9b46bd25bf5f0595bbe24655141438e7a100b"
        },
        {
            "4ccd089b28ff96da9db6c346ec114e0f5b8a319f35aba624da8cf6ed4fb8a6fb",
            "3d4017c3e843895a92b70aa74d1b7ebc9c982ccf2ec4968cc0cd55f12af4660c",
            "72", 1,
            "92a009a9f0d4cab8720e820b5f642540a2b27b5416503f8fb3762223ebdb69da"
            "085ac1e43e15996e458f3613d0f11d8c387b2eaeb4302aeeb00d291612bb0c00"
        },
        {
            "c5aa8df43f9f837bedb7442f31dcb7b166d38535076f094b85ce3a2e0b4458f7",
            "fc51cd8e6218a1a38da47ed00230f0580816ed13ba3303ac5deb911548908025",
            "af82", 2,
            "6291d657deec24024827e69c3abe01a30ce548a284743a445e3680d7db5ac3ac"
            "18ff9b538d16f290ae67f760984dc6594a7c15e9716ed28dc027beceea1ec40a"
        },
    };

    printf("Ed25519 (RFC 8032 section 7.1)\n");

    for (size_t i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        const struct ed_vector *v = &vectors[i];
        uint8_t seed[32], pk[32], sk[64], msg[64], sig[64];
        char name[64];

        hex_decode(v->seed, seed, 32);
        hex_decode(v->message, msg, v->message_len);

        ed25519_keypair(pk, sk, seed);
        sprintf(name, "vector %zu: public key", i + 1);
        check_bytes(name, pk, v->public_key, 32);

        ed25519_sign(sig, msg, v->message_len, pk, sk);
        sprintf(name, "vector %zu: signature", i + 1);
        check_bytes(name, sig, v->signature, 64);

        sprintf(name, "vector %zu: verifies", i + 1);
        check_int(name, ed25519_verify(sig, msg, v->message_len, pk), 0);

        /* A single flipped bit anywhere must break it. */
        sig[i % 64] ^= 0x01;
        sprintf(name, "vector %zu: a corrupt signature is refused", i + 1);
        check_int(name, ed25519_verify(sig, msg, v->message_len, pk) != 0, 1);
    }

    printf("Ed25519 with a longer message\n");
    {
        uint8_t seed[32], pk[32], sk[64], sig[64];
        uint8_t msg[1024];

        for (int i = 0; i < 32; i++) seed[i] = (uint8_t)(i * 7 + 1);
        for (int i = 0; i < 1024; i++) msg[i] = (uint8_t)(i * 13);

        ed25519_keypair(pk, sk, seed);
        ed25519_sign(sig, msg, sizeof(msg), pk, sk);
        check_int("1 KiB message round trip",
                  ed25519_verify(sig, msg, sizeof(msg), pk), 0);

        msg[500] ^= 0xff;
        check_int("a changed message is refused",
                  ed25519_verify(sig, msg, sizeof(msg), pk) != 0, 1);
    }
}

/* ---- AES ---- */

static void test_aes(void) {
    uint8_t key[32], in[16], out[16];

    printf("AES (FIPS 197 appendix C)\n");

    hex_decode("000102030405060708090a0b0c0d0e0f", key, 16);
    hex_decode("00112233445566778899aabbccddeeff", in, 16);
    {
        struct aes_ctx ctx;
        aes_init(&ctx, key, 128);
        aes_encrypt_block(&ctx, in, out);
        check_bytes("AES-128 block", out, "69c4e0d86a7b0430d8cdb78070b4c55a", 16);
    }

    hex_decode("000102030405060708090a0b0c0d0e0f1011121314151617", key, 24);
    {
        struct aes_ctx ctx;
        aes_init(&ctx, key, 192);
        aes_encrypt_block(&ctx, in, out);
        check_bytes("AES-192 block", out, "dda97ca4864cdfe06eaf70a0ec0d7191", 16);
    }

    hex_decode("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f",
               key, 32);
    {
        struct aes_ctx ctx;
        aes_init(&ctx, key, 256);
        aes_encrypt_block(&ctx, in, out);
        check_bytes("AES-256 block", out, "8ea2b7ca516745bfeafc49904b496089", 16);
    }

    printf("AES-CTR (NIST SP 800-38A F.5.1)\n");
    {
        struct aes_ctr_ctx ctx;
        uint8_t iv[16], plain[64], cipher[64];

        hex_decode("2b7e151628aed2a6abf7158809cf4f3c", key, 16);
        hex_decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff", iv, 16);
        hex_decode("6bc1bee22e409f96e93d7e117393172a"
                   "ae2d8a571e03ac9c9eb76fac45af8e51"
                   "30c81c46a35ce411e5fbc1191a0a52ef"
                   "f69f2445df4f9b17ad2b417be66c3710", plain, 64);

        aes_ctr_init(&ctx, key, 128, iv);
        aes_ctr_xor(&ctx, plain, cipher, 64);
        check_bytes("AES-128-CTR", cipher,
                    "874d6191b620e3261bef6864990db6ce"
                    "9806f66b7970fdff8617187bb9fffdff"
                    "5ae4df3edbd5d35e5b4f09020db03eab"
                    "1e031dda2fbe03d1792170a0f3009cee", 64);
    }
}

/* ---- constant-time helpers ---- */

static void test_helpers(void) {
    printf("helpers\n");

    uint8_t a[32], b[32];
    memset(a, 0x5a, sizeof(a));
    memset(b, 0x5a, sizeof(b));
    check_int("crypto_verify says equal", crypto_verify(a, b, 32), 0);

    b[31] ^= 1;
    check_int("crypto_verify catches the last byte",
              crypto_verify(a, b, 32) != 0, 1);

    b[31] ^= 1;
    b[0] ^= 1;
    check_int("crypto_verify catches the first byte",
              crypto_verify(a, b, 32) != 0, 1);

    memset(a, 0xff, sizeof(a));
    crypto_wipe(a, sizeof(a));
    int zeroed = 1;
    for (int i = 0; i < 32; i++) if (a[i] != 0) zeroed = 0;
    check_int("crypto_wipe clears the buffer", zeroed, 1);
}

int main(void) {
    printf("tuscrypt test vectors\n\n");

    test_hashes();
    test_hmac();
    test_aes();
    test_chacha_poly();
    test_x25519();
    test_ed25519();
    test_helpers();

    printf("\n%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
