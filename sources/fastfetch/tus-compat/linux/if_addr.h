/* Stub for TUS. Real (stable, decades-unchanged) Linux uapi values -
 * localip_linux.c only ever tests these against flags TUS's own
 * getifaddrs() never sets, so they just need to be correct-looking,
 * not exercised. */
#ifndef _LINUX_IF_ADDR_H
#define _LINUX_IF_ADDR_H

#define IFA_F_TEMPORARY  0x02
#define IFA_F_DADFAILED  0x08
#define IFA_F_OPTIMISTIC 0x10
#define IFA_F_DEPRECATED 0x20
#define IFA_F_TENTATIVE  0x40

#endif
