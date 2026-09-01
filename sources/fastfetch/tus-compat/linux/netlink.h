/* Stub for TUS: shared netlink primitives that real Linux systems get
 * transitively (their linux/rtnetlink.h and linux/genetlink.h both
 * pull this in internally) but TUS has no linux/netlink.h at all.
 * Not included directly by fastfetch - only by this compat tree's own
 * rtnetlink.h/genetlink.h/nl80211.h, matching how the real headers are
 * structured. TUS's socket(AF_NETLINK, ...) always fails (unsupported
 * domain), so netif_linux.c/wifi_linux.c bail out before any of this
 * is used to parse real kernel data - these are the real, stable
 * Linux uapi definitions regardless, just not exercised. */
#ifndef _LINUX_NETLINK_H
#define _LINUX_NETLINK_H

#include <stdint.h>

#define NETLINK_ROUTE   0
#define NETLINK_GENERIC 16

#define NLM_F_REQUEST 0x01
#define NLM_F_ACK     0x04
#define NLM_F_ROOT    0x100
#define NLM_F_MATCH   0x200
#define NLM_F_DUMP    (NLM_F_ROOT | NLM_F_MATCH)

struct sockaddr_nl {
    unsigned short nl_family;
    unsigned short nl_pad;
    uint32_t nl_pid;
    uint32_t nl_groups;
};

struct nlmsghdr {
    uint32_t nlmsg_len;
    uint16_t nlmsg_type;
    uint16_t nlmsg_flags;
    uint32_t nlmsg_seq;
    uint32_t nlmsg_pid;
};

#define NLMSG_ALIGNTO 4u
#define NLMSG_ALIGN(len) (((len) + NLMSG_ALIGNTO - 1) & ~(NLMSG_ALIGNTO - 1))
#define NLMSG_HDRLEN ((int)NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)
#define NLMSG_DATA(nlh) ((void *)((char *)(nlh) + NLMSG_HDRLEN))
#define NLMSG_OK(nlh, len) \
    ((len) >= (int)sizeof(struct nlmsghdr) && \
     (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
     (int)(nlh)->nlmsg_len <= (len))
#define NLMSG_NEXT(nlh, len) \
    ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
     (struct nlmsghdr *)((char *)(nlh) + NLMSG_ALIGN((nlh)->nlmsg_len)))
#define NLMSG_PAYLOAD(nlh, len) ((int)(nlh)->nlmsg_len - NLMSG_LENGTH(len))

#define NLMSG_ERROR 0x2
#define NLMSG_DONE  0x3

struct nlmsgerr {
    int error;
    struct nlmsghdr msg;
};

struct nlattr {
    uint16_t nla_len;
    uint16_t nla_type;
};

#define NLA_ALIGNTO 4u
#define NLA_ALIGN(len) (((len) + NLA_ALIGNTO - 1) & ~(NLA_ALIGNTO - 1))
#define NLA_HDRLEN ((int)NLA_ALIGN(sizeof(struct nlattr)))
#define NLA_TYPE_MASK (~0)

#endif
