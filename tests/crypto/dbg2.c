#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../../userspace/tuscrypt/curve25519.c"

static void show(const char *name, const struct ge *p) {
    uint8_t s[32];
    ge_tobytes(s, p);
    printf("%s = ", name);
    for (int i = 0; i < 32; i++) printf("%02x", s[i]);
    printf("\n");
}

int main(void) {
    ed_init_constants();
    struct ge B, acc, sum;
    ge_frombytes(&B, ed_basepoint);

    /* Walk the ladder by hand for the scalar 1. */
    ge_zero(&acc);
    show("acc(identity)", &acc);

    ge_double(&acc, &acc);
    show("after one double", &acc);

    ge_add(&sum, &acc, &B);
    show("identity + B", &sum);

    ge_cmov(&acc, &sum, 1);
    show("after cmov(1)", &acc);

    /* And the same through the real routine, for 1, 2 and 3. */
    for (int k = 1; k <= 3; k++) {
        uint8_t s[32];
        memset(s, 0, 32);
        s[0] = (uint8_t)k;
        struct ge r;
        ge_scalarmult(&r, s, &B);
        char name[32];
        sprintf(name, "[%d]B", k);
        show(name, &r);
    }
    return 0;
}
