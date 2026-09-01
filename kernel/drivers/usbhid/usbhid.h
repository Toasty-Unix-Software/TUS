/*
 * usbhid.h - USB keyboard and mouse (HID boot protocol)
 *
 * A HID device describes itself with a report descriptor: a small
 * program saying which bits of its reports mean what. Parsing one is
 * a real piece of work, and for a keyboard or a mouse it is work
 * nobody needs to do - because USB defines a *boot protocol*, a fixed
 * report layout every keyboard and mouse must support so a PC BIOS
 * can use one before any driver exists.
 *
 *   keyboard (8 bytes)  modifiers, reserved, then six key usages
 *   mouse    (3+ bytes) buttons, then signed dx, dy (and dz)
 *
 * This driver asks for that protocol with SET_PROTOCOL(0) and reads
 * nothing else. A device that needs its report descriptor parsed - a
 * gaming keyboard's extra keys, a tablet, a joystick - is a device
 * this driver does not claim.
 *
 * WHERE THE EVENTS GO
 *
 * Nowhere new. A USB keystroke is pushed into the same ring buffer
 * the PS/2 driver fills (kbd_inject_event) and USB movement into the
 * same one the PS/2 mouse fills (mouse_inject). Everything above -
 * tsh, the tty layer, the highX display server - sees one keyboard
 * and one pointer and has no idea which bus they arrived on. That is
 * the entire integration: two functions, and no consumer changes.
 */

#ifndef TUS_DRIVERS_USBHID_H
#define TUS_DRIVERS_USBHID_H

#include <stdbool.h>
#include <stdint.h>

/* Register the keyboard and mouse class drivers with the xHCI stack.
 * Call before xhci_init(). */
void usbhid_init(void);

/* True once a USB keyboard / mouse has been claimed (diagnostics,
 * and `mouse_present()` needs to know). */
bool usbhid_keyboard_present(void);
bool usbhid_mouse_present(void);

/* Report counts, printed by the `usb` shell command. These exist to
 * settle one question a screen full of working output cannot: whether
 * a keystroke arrived over USB or over the PS/2 port. */
void usbhid_print_state(void);

#endif /* TUS_DRIVERS_USBHID_H */
