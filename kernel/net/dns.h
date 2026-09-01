/*
 * dns.h - a resolver in the kernel
 *
 * Every network tool needs to turn a name into an address, and none of
 * them should carry its own resolver. One query, one UDP socket, A
 * records only, with a small cache so a `git clone` that reconnects
 * does not ask twice.
 */

#ifndef TUS_NET_DNS_H
#define TUS_NET_DNS_H

#include <stdint.h>

#define DNS_MAX_ADDRS 4

/* Resolve `name` to at most DNS_MAX_ADDRS IPv4 addresses (network byte
 * order). Returns the number found, or a negative errno. A name that
 * is already dotted-quad is parsed without going near the network. */
int dns_resolve(const char *name, uint32_t *addrs, int max);

/* Parse "a.b.c.d" into a network-order address. 0 when it is not one. */
uint32_t dns_parse_ipv4(const char *s);

#endif /* TUS_NET_DNS_H */
