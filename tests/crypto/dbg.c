#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Pull the implementation in directly so the statics are reachable. */
#include "../../userspace/tuscrypt/curve25519.c"

static void dump(const char *name, const fe f) {
    uint8_t s[32];
    fe_tobytes(s, f);
    printf("%s = ", name);
    for (int i = 31; i >= 0; i--) printf("%02x", s[i]);
    printf("\n");
}

int main(void) {
    ed_init_constants();
    dump("d      ", ed_d);
    dump("sqrtm1 ", ed_sqrtm1);

    struct ge B;
    int r = ge_frombytes(&B, ed_basepoint);
    printf("ge_frombytes(basepoint) = %d\n", r);
    dump("B.X", B.X);
    dump("B.Y", B.Y);
    dump("B.Z", B.Z);
    dump("B.T", B.T);

    uint8_t back[32];
    ge_tobytes(back, &B);
    printf("recompress: ");
    for (int i = 0; i < 32; i++) printf("%02x", back[i]);
    printf("\n  expected: ");
    for (int i = 0; i < 32; i++) printf("%02x", ed_basepoint[i]);
    printf("\n");

    /* On-curve check: -x^2 + y^2 = 1 + d x^2 y^2 (with Z = 1). */
    fe x2, y2, lhs, rhs, dx2y2, one;
    fe_sq(x2, B.X);
    fe_sq(y2, B.Y);
    fe_sub(lhs, y2, x2);
    fe_mul(dx2y2, x2, y2);
    fe_mul(dx2y2, dx2y2, ed_d);
    fe_one(one);
    fe_add(rhs, one, dx2y2);
    fe_sub(lhs, lhs, rhs);
    printf("on curve: %s\n", fe_is_zero(lhs) ? "yes" : "NO");

    /* Identity behaviour of the addition law. */
    struct ge id, sum;
    ge_zero(&id);
    ge_add(&sum, &B, &id);
    uint8_t sb[32];
    ge_tobytes(sb, &sum);
    printf("B + identity == B: %s\n",
           memcmp(sb, ed_basepoint, 32) == 0 ? "yes" : "NO");

    /* Doubling versus adding to itself. */
    struct ge d1, d2;
    ge_double(&d1, &B);
    ge_add(&d2, &B, &B);
    uint8_t b1[32], b2[32];
    ge_tobytes(b1, &d1);
    ge_tobytes(b2, &d2);
    printf("2B (double) == B+B (add): %s\n",
           memcmp(b1, b2, 32) == 0 ? "yes" : "NO");
    printf("2B = ");
    for (int i = 0; i < 32; i++) printf("%02x", b1[i]);
    printf("\n");

    /* [1]B must come back as B. */
    uint8_t one_s[32];
    memset(one_s, 0, 32);
    one_s[0] = 1;
    struct ge p1;
    ge_scalarmult(&p1, one_s, &B);
    uint8_t p1b[32];
    ge_tobytes(p1b, &p1);
    printf("[1]B == B: %s\n", memcmp(p1b, ed_basepoint, 32) == 0 ? "yes" : "NO");

    /* [l]B must be the identity. */
    struct ge pl;
    ge_scalarmult(&pl, ed_l, &B);
    uint8_t plb[32], idb[32];
    ge_tobytes(plb, &pl);
    ge_zero(&id);
    ge_tobytes(idb, &id);
    printf("[l]B == identity: %s\n", memcmp(plb, idb, 32) == 0 ? "yes" : "NO");
    return 0;
}
