/* Stub for TUS: see linux/netlink.h in this same compat tree - TUS
 * has no netlink sockets, so this never parses real data. Real,
 * stable Linux uapi values. */
#ifndef _LINUX_GENETLINK_H
#define _LINUX_GENETLINK_H

#include "netlink.h"

struct genlmsghdr {
    uint8_t cmd;
    uint8_t version;
    uint16_t reserved;
};

#define GENL_HDRLEN ((int)NLMSG_ALIGN(sizeof(struct genlmsghdr)))
#define GENL_ID_CTRL 0x10

#define CTRL_CMD_NEWFAMILY 1
#define CTRL_CMD_GETFAMILY 3

#define CTRL_ATTR_FAMILY_ID   1
#define CTRL_ATTR_FAMILY_NAME 2

#endif
