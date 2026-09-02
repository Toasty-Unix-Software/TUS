/* test_nx.c - proves anonymous mmap() pages are really non-executable.
 *
 * Regression coverage for the NX/W^X hardening pass: sys_mmap() used
 * to ignore `prot` entirely and map every anonymous page executable.
 * This mmaps a page (no PROT_EXEC), writes a `ret` (0xc3) into it,
 * and jumps to it. On a fixed kernel this must fault and kill the
 * task - if it prints "UNSAFE" instead, NX enforcement regressed.
 */
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>

typedef void (*fn)(void);

int main(void) {
    void *p = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED) {
        printf("nxtest: mmap failed\n");
        return 1;
    }
    ((unsigned char *)p)[0] = 0xc3; /* ret */
    printf("nxtest: about to jump to a non-executable page\n");
    ((fn)p)();
    /* Only reached if the NX bit was NOT enforced. */
    printf("nxtest: UNSAFE - executed a non-executable page\n");
    return 0;
}
