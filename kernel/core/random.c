/*
 * random.c - ChaCha20-based CSPRNG
 *
 * The generator is a ChaCha20 keystream with a counter. Each request
 * takes bytes from the stream and then rekeys from the stream itself,
 * so a snapshot of the state cannot be walked backwards to recover
 * bytes already handed out.
 *
 * Seeding prefers RDSEED, then RDRAND. Without either - which is what
 * an old CPU or a conservative emulator gives you - the fallback times
 * a series of short loops against the PIT's tick counter and keeps the
 * low bits, the classic jitter collector. It is not as good as a
 * hardware source and it is a great deal better than a constant.
 */

#include "random.h"

#include "klib.h"
#include "../arch/x86_64/io.h"
#include "../drivers/pit/pit.h"

static uint32_t chacha_state[16];
static bool have_hardware;
static bool seeded;

#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))

#define QUARTERROUND(a, b, c, d)      \
    a += b; d ^= a; d = ROTL32(d, 16); \
    c += d; b ^= c; b = ROTL32(b, 12); \
    a += b; d ^= a; d = ROTL32(d, 8);  \
    c += d; b ^= c; b = ROTL32(b, 7)

static void chacha20_block(const uint32_t in[16], uint32_t out[16]) {
    for (int i = 0; i < 16; i++) out[i] = in[i];

    for (int round = 0; round < 10; round++) {
        QUARTERROUND(out[0], out[4], out[8],  out[12]);
        QUARTERROUND(out[1], out[5], out[9],  out[13]);
        QUARTERROUND(out[2], out[6], out[10], out[14]);
        QUARTERROUND(out[3], out[7], out[11], out[15]);
        QUARTERROUND(out[0], out[5], out[10], out[15]);
        QUARTERROUND(out[1], out[6], out[11], out[12]);
        QUARTERROUND(out[2], out[7], out[8],  out[13]);
        QUARTERROUND(out[3], out[4], out[9],  out[14]);
    }
    for (int i = 0; i < 16; i++) out[i] += in[i];
}

/* ---- entropy sources ---- */

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

static bool cpu_has(uint32_t leaf, int reg_index, uint32_t bit) {
    uint32_t regs[4];
    __asm__ volatile("cpuid"
                     : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
                     : "a"(leaf), "c"(0));
    return (regs[reg_index] & bit) != 0;
}

static bool rdseed64(uint64_t *out) {
    unsigned char ok = 0;
    uint64_t v = 0;

    for (int i = 0; i < 32; i++) {
        __asm__ volatile("rdseed %0; setc %1" : "=r"(v), "=qm"(ok));
        if (ok) { *out = v; return true; }
    }
    return false;
}

static bool rdrand64(uint64_t *out) {
    unsigned char ok = 0;
    uint64_t v = 0;

    for (int i = 0; i < 32; i++) {
        __asm__ volatile("rdrand %0; setc %1" : "=r"(v), "=qm"(ok));
        if (ok) { *out = v; return true; }
    }
    return false;
}

/*
 * Jitter: time a variable-length delay loop with the timestamp counter
 * and keep the low bits, which vary with cache state, interrupt
 * arrival and DRAM refresh. One bit per sample is a conservative
 * estimate, so 64 samples are taken per word.
 */
static uint64_t jitter_word(void) {
    uint64_t acc = 0;

    for (int bit = 0; bit < 64; bit++) {
        uint64_t start = rdtsc();
        volatile uint32_t spin = 0;
        for (int i = 0; i < 64 + (int)(start & 0x3f); i++) spin += i;
        uint64_t delta = rdtsc() - start;

        acc = (acc << 1) | (delta & 1);
        acc ^= (uint64_t)pit_uptime_ms() << (bit & 7);
    }
    return acc;
}

void random_init(void) {
    uint64_t seed[4] = {0, 0, 0, 0};

    bool has_rdseed = cpu_has(7, 1, 1u << 18);  /* CPUID.7:EBX.RDSEED */
    bool has_rdrand = cpu_has(1, 2, 1u << 30);  /* CPUID.1:ECX.RDRAND */

    for (int i = 0; i < 4; i++) {
        bool got = false;
        if (has_rdseed) got = rdseed64(&seed[i]);
        if (!got && has_rdrand) got = rdrand64(&seed[i]);
        if (got) {
            have_hardware = true;
        } else {
            seed[i] = jitter_word();
        }
        /* Mix the clock in either way: it costs nothing and a
         * hardware source that silently fails cannot fix the seed. */
        seed[i] ^= rdtsc() ^ ((uint64_t)pit_uptime_ms() << 32);
    }

    /* "expand 32-byte k" */
    chacha_state[0] = 0x61707865;
    chacha_state[1] = 0x3320646e;
    chacha_state[2] = 0x79622d32;
    chacha_state[3] = 0x6b206574;

    for (int i = 0; i < 4; i++) {
        chacha_state[4 + i * 2] = (uint32_t)seed[i];
        chacha_state[5 + i * 2] = (uint32_t)(seed[i] >> 32);
    }
    chacha_state[12] = 0;
    chacha_state[13] = (uint32_t)rdtsc();
    chacha_state[14] = (uint32_t)(rdtsc() >> 32);
    chacha_state[15] = (uint32_t)pit_uptime_ms();

    seeded = true;

    kprintf("random: seeded from %s\n",
            have_hardware ? "RDSEED/RDRAND" : "timing jitter");
}

void random_add_entropy(const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;

    /* Fold the input into the nonce words. This never reduces the
     * state's entropy, so an attacker feeding it chosen data cannot
     * weaken the generator. */
    for (size_t i = 0; i < len; i++) {
        chacha_state[13 + (i % 3)] ^= (uint32_t)p[i] << ((i % 4) * 8);
    }
    chacha_state[14] ^= (uint32_t)rdtsc();
}

void random_bytes(void *buf, size_t len) {
    if (!seeded) random_init();

    uint8_t *out = (uint8_t *)buf;
    uint32_t block[16];

    while (len > 0) {
        chacha_state[12]++;
        if (chacha_state[12] == 0) chacha_state[13]++;
        chacha20_block(chacha_state, block);

        size_t n = len < 64 ? len : 64;
        memcpy(out, block, n);
        out += n;
        len -= n;
    }

    /* Rekey from a fresh block so the bytes just returned cannot be
     * recovered from the state (forward secrecy). */
    chacha_state[12]++;
    chacha20_block(chacha_state, block);
    for (int i = 0; i < 8; i++) {
        chacha_state[4 + i] = block[i];
    }
    chacha_state[14] ^= block[8];
    chacha_state[15] ^= block[9];

    memset(block, 0, sizeof(block));
}

bool random_has_hardware(void) { return have_hardware; }
