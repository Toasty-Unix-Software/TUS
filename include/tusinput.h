/*
 * tusinput.h - the input configuration ABI (SYS_INPUT)
 *
 * Shared verbatim by the kernel and by `keymap`, the way
 * include/tusvideo.h and include/highx.h are.
 *
 *     long input(int op, void *arg, unsigned long len);   SYS_INPUT = 55
 *
 * Today it carries one thing: which keyboard layout the machine uses.
 * The keyboard is a single piece of hardware shared by every program
 * and every user on the machine, so - like the display mode - reading
 * the setting is open to anyone and changing it is root's job. The
 * check lives in the kernel and `/bin/keymap` is NOT setuid; the way
 * to change the layout is `doas keymap tr`.
 *
 * The structure has room for the settings that belong next to it (key
 * repeat, pointer acceleration) so that adding one does not need a
 * new opcode or a new size.
 */

#ifndef TUS_INPUT_H
#define TUS_INPUT_H

#include <stdint.h>

#define TUS_INPUT_GET_KEYMAP  0 /* which layout is loaded */
#define TUS_INPUT_SET_KEYMAP  1 /* load a layout by name (root only) */
#define TUS_INPUT_LIST_KEYMAP 2 /* layout number `index`, until -ENOENT */

#define TUS_KEYMAP_NAME_MAX 16
#define TUS_KEYMAP_DESC_MAX 48

struct tus_input_keymap {
    uint32_t index;                       /* in, for LIST */
    char name[TUS_KEYMAP_NAME_MAX];       /* in for SET, out otherwise */
    char description[TUS_KEYMAP_DESC_MAX];/* out */
};

#endif /* TUS_INPUT_H */
