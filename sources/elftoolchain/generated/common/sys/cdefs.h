/*
 * Minimal sys/cdefs.h for building elftoolchain against musl on TUS.
 * musl deliberately ships no cdefs.h (BSD/glibc-only header); the only
 * things elftoolchain's own sources actually use from it are these two
 * source-tagging macros (checked against every .c/.h in the tree), so
 * this isn't a general-purpose compat shim - just enough for that.
 */
#ifndef _SYS_CDEFS_H_
#define _SYS_CDEFS_H_

#define __FBSDID(s)
#define __RCSID(s)

#endif
