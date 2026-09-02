/*
 * dhcp.h - DHCPv4 client (RFC 2131/2132)
 *
 * A single blocking call: dhcp_configure() runs DISCOVER -> OFFER ->
 * REQUEST -> ACK against whatever's on the wire and, on success,
 * overwrites g_netif's ip/netmask/gateway/dns in place - same fields
 * `ifconfig` already edits at runtime.
 */

#ifndef TUS_NET_DHCP_H
#define TUS_NET_DHCP_H

#include <stdint.h>

/* Runs the four-message exchange with a 2s wait per stage, 3 retries.
 * Returns 0 and updates g_netif on success, -1 on timeout/failure
 * (g_netif is left untouched so a prior static config still stands). */
int dhcp_configure(void);

#endif /* TUS_NET_DHCP_H */
