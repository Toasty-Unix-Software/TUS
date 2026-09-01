/*
 * clear.c - clear the screen (TUS port of the classic UNIX clear).
 *
 * TUS's console and hxtsh both parse plain VT100/ANSI escapes (see
 * CLAUDE.md's "Escape sequences" note) - there is exactly one terminal
 * type on this system, so unlike real Unix's terminfo-driven `clear`,
 * writing the sequence directly is the whole implementation: home the
 * cursor, then erase the whole screen.
 */

#include <unistd.h>

int main(void) {
    write(1, "\033[H\033[2J", 7);
    return 0;
}
