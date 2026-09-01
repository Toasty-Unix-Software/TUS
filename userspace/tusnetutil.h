/*
 * tusnetutil.h - the bits every TUS network tool needs
 *
 * Header-only on purpose: these are six short functions, and giving
 * them their own object file would mean a link-order rule in the
 * Makefile for every tool that uses them.
 */

#ifndef TUS_NETUTIL_H
#define TUS_NETUTIL_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <tusnet.h>

/* netctl(op, arg, len) - syscall 47 through the TUS ABI directly,
 * since musl has no wrapper for a call that only TUS has. */
static inline long netctl(long op, void *arg, long len) {
    long ret;
    register long r10 __asm__("r10") = 0;
    register long r8 __asm__("r8") = 0;
    register long r9 __asm__("r9") = 0;
    register long rdi __asm__("rdi") = op;
    register long rsi __asm__("rsi") = (long)arg;
    register long rdx __asm__("rdx") = len;

    __asm__ volatile("int $0x80"
                     : "=a"(ret), "+r"(rdi), "+r"(rsi), "+r"(rdx),
                       "+r"(r10), "+r"(r8), "+r"(r9)
                     : "0"(47L)
                     : "rcx", "r11", "memory");
    return ret;
}

/* Format a network-order address into "a.b.c.d". The buffer must hold
 * at least 16 bytes. */
static inline const char *ip_str(uint32_t ip, char *buf) {
    const uint8_t *o = (const uint8_t *)&ip;
    sprintf(buf, "%u.%u.%u.%u", o[0], o[1], o[2], o[3]);
    return buf;
}

/* Parse "a.b.c.d" into a network-order address; 0 when it is not one. */
static inline uint32_t ip_parse(const char *s) {
    uint32_t out = 0;
    uint8_t *o = (uint8_t *)&out;
    int part = 0, digits = 0, v = 0;

    for (const char *p = s; ; p++) {
        if (*p >= '0' && *p <= '9') {
            v = v * 10 + (*p - '0');
            if (v > 255) return 0;
            digits++;
        } else if (*p == '.' || *p == '\0') {
            if (digits == 0 || part > 3) return 0;
            o[part++] = (uint8_t)v;
            v = 0;
            digits = 0;
            if (*p == '\0') break;
        } else {
            return 0;
        }
    }
    return part == 4 ? out : 0;
}

/* Turn a hostname or dotted quad into an address, using the kernel's
 * resolver. Returns 0 when the name cannot be resolved. */
static inline uint32_t host_resolve(const char *name) {
    uint32_t literal = ip_parse(name);
    if (literal) return literal;

    struct tus_resolve req;
    memset(&req, 0, sizeof(req));
    strncpy(req.name, name, sizeof(req.name) - 1);

    if (netctl(NETCTL_RESOLVE, &req, sizeof(req)) < 0 || req.count <= 0) {
        return 0;
    }
    return req.addr[0];
}

static inline const char *mac_str(const uint8_t *mac, char *buf) {
    sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return buf;
}

/* Count the leading one bits of a netmask, for "/24" notation. */
static inline int netmask_bits(uint32_t netmask) {
    uint32_t host_order = __builtin_bswap32(netmask);
    int bits = 0;
    while (host_order & 0x80000000u) {
        bits++;
        host_order <<= 1;
    }
    return bits;
}

#endif /* TUS_NETUTIL_H */
