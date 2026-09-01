/* Stub for TUS: TUS has no netlink sockets, so netif_linux.c's
 * socket(AF_NETLINK, ...) always fails first and none of this ever
 * parses real routing data - see linux/netlink.h in this same compat
 * tree for why that's fine. Real, stable Linux uapi values. */
#ifndef _LINUX_RTNETLINK_H
#define _LINUX_RTNETLINK_H

#include "netlink.h"

#define RTM_NEWROUTE 24
#define RTM_GETROUTE 26

#define RT_TABLE_UNSPEC 0
#define RT_TABLE_MAIN   254

#define RTPROT_UNSPEC 0

#define RT_SCOPE_UNIVERSE 0
#define RT_SCOPE_HOST     254

#define RTN_UNSPEC 0
#define RTN_UNICAST 1
#define RTN_LOCAL   2

struct rtmsg {
    unsigned char rtm_family;
    unsigned char rtm_dst_len;
    unsigned char rtm_src_len;
    unsigned char rtm_tos;
    unsigned char rtm_table;
    unsigned char rtm_protocol;
    unsigned char rtm_scope;
    unsigned char rtm_type;
    unsigned rtm_flags;
};

#define RTA_DST      1
#define RTA_OIF      4
#define RTA_GATEWAY  5
#define RTA_PRIORITY 6
#define RTA_PREFSRC  7
#define RTA_TABLE    15

struct rtattr {
    unsigned short rta_len;
    unsigned short rta_type;
};

#define RTA_ALIGNTO 4u
#define RTA_ALIGN(len) (((len) + RTA_ALIGNTO - 1) & ~(RTA_ALIGNTO - 1))
#define RTA_OK(rta, len) \
    ((len) >= (int)sizeof(struct rtattr) && \
     (rta)->rta_len >= sizeof(struct rtattr) && \
     (int)(rta)->rta_len <= (len))
#define RTA_NEXT(rta, len) \
    ((len) -= RTA_ALIGN((rta)->rta_len), \
     (struct rtattr *)((char *)(rta) + RTA_ALIGN((rta)->rta_len)))
#define RTA_LENGTH(len) (RTA_ALIGN(sizeof(struct rtattr)) + (len))
#define RTA_PAYLOAD(rta) ((int)(rta)->rta_len - (int)RTA_ALIGN(sizeof(struct rtattr)))
#define RTA_DATA(rta) ((void *)((char *)(rta) + RTA_ALIGN(sizeof(struct rtattr))))

#define RTM_RTA(rtm) ((struct rtattr *)((char *)(rtm) + RTA_ALIGN(sizeof(struct rtmsg))))
#define RTM_PAYLOAD(nlh) NLMSG_PAYLOAD(nlh, sizeof(struct rtmsg))

#endif
