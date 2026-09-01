/* Stub for TUS: musl's <sys/ioctl.h> already has the real SIOCGIF*
 * numbers fastfetch uses; only SIOCETHTOOL (real Linux value) is
 * missing from it. */
#ifndef _LINUX_SOCKIOS_H
#define _LINUX_SOCKIOS_H

#define SIOCETHTOOL 0x8946

#endif
