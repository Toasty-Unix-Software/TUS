/* Stub for TUS: musl's <net/if.h> already has a complete, correct
 * struct ifreq/IFNAMSIZ (fastfetch's localip_linux.c uses those, not
 * anything from the real linux/if.h). All that's actually needed from
 * here are four IFA-adjacent flag constants it references
 * unconditionally; SIOCGIFAFLAG_IN6 is deliberately left undefined -
 * it's #ifdef-guarded in localip_linux.c and skips a whole IPv6
 * extension block, avoiding needing struct in6_ifreq/IN6_IFF_* at
 * all. TUS's socket()/ioctl() never actually populate any of this at
 * runtime (no AF_NETLINK, no real network ioctls beyond what TUS's
 * own stack answers), so exact bit values don't matter - they just
 * need to be distinct. */
#ifndef _LINUX_IF_H
#define _LINUX_IF_H

#define IFF_PREFERRED  0x0800
#define IFF_DEPRECATED 0x1000
#define IFF_TEMPORARY  0x2000
#define IFF_DUPLICATE  0x4000

#endif
