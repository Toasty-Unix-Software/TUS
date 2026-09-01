/*
 * PCC OS port for TUS (x86-64 hobby UNIX-like kernel).
 *
 * TUS ships only static binaries (no dynamic linker exists), so unlike
 * os/linux/ccconfig.h this defines no DYNLINKLIB/USE_MUSL bits at all —
 * cc.c defaults DYNLINKLIB to NULL when it's undefined, which is exactly
 * "don't look for a dynamic linker".
 */

#define CPPADD		{ "-D__tus__", "-D__ELF__", NULL, }

#define CRT0		"crt1.o"
#define STARTLABEL	"_start"

/* musl headers/libs staged on-target under /usr, same layout as the host
 * cross-build's MUSL_INC/MUSL_LIB in the top-level Makefile. */
#define STDINC		"/usr/include/"
#define DEFLIBDIRS	{ "/usr/lib/", 0 }

/* TUS has no PATH search and no envp (execve/spawn pass argv only), and
 * find_file()'s LIBEXECDIR-relative search harmlessly fails closed (TUS
 * has no access() syscall) and falls back to these values unchanged -
 * so the pipeline stages are just given their final on-target location
 * directly, matching every other TUS program's flat /bin layout. */
#define PREPROCESSOR	"/bin/cpp"
#define COMPILER	"/bin/ccom"

#if defined(mach_amd64)
#include "../inc/amd64.h"
#else
#error defines for arch missing
#endif
