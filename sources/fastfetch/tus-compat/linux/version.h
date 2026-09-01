/* Stub: TUS has no real Linux kernel headers. Nothing in fastfetch's
 * source actually uses LINUX_VERSION_CODE/KERNEL_VERSION from this -
 * it is only ever #include'd, never macro-expanded - so an empty
 * header satisfies the build's existence check. */
#ifndef _LINUX_VERSION_H
#define _LINUX_VERSION_H
#endif
