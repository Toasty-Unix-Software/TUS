/*
 * mouse.h - PS/2 mouse driver
 *
 * The second device on the 8042 controller: the auxiliary port, which
 * raises IRQ12 and speaks the classic three-byte packet (buttons and
 * sign bits, then a relative X and Y movement). The driver decodes
 * packets into events and buffers them; consumers poll.
 *
 * Movement is reported in the mouse's own convention - X to the
 * right, Y *up* - and translated once, where it is used (the highX
 * server flips Y, because screens count downwards).
 *
 * A wheel is not part of the classic protocol: the driver asks for it
 * with the IntelliMouse knock (three sample-rate commands, 200-100-80)
 * and only then does the device answer id 3 and start sending a fourth
 * byte. Devices that do not know the knock keep their three-byte
 * packets and report dz == 0 forever, so nothing has to be optional
 * above this layer.
 */

#ifndef TUS_DRIVERS_MOUSE_H
#define TUS_DRIVERS_MOUSE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Button bits in mouse_event.buttons. */
#define MOUSE_BTN_LEFT   0x1
#define MOUSE_BTN_RIGHT  0x2
#define MOUSE_BTN_MIDDLE 0x4

struct mouse_event {
    int32_t dx;      /* movement since the last packet, right positive */
    int32_t dy;      /* movement since the last packet, up positive */
    int32_t dz;      /* wheel steps since the last packet, up positive */
    uint32_t buttons; /* MOUSE_BTN_* currently held */
};

/* Probe the auxiliary port, enable reporting and install the IRQ12
 * handler. Safe to call on machines without a mouse: the controller
 * simply never answers and mouse_present() stays false. */
void mouse_init(void);

/* Feed one report from a mouse that is not on the PS/2 aux port (the
 * USB HID driver). Same convention as the PS/2 packets: X right, Y
 * up, dz in wheel steps. Marks the pointer present. */
void mouse_inject(int32_t dx, int32_t dy, int32_t dz, uint32_t buttons);

/* True when the controller acknowledged the device at init. */
bool mouse_present(void);

/* True while at least one packet is buffered. */
bool mouse_has_event(void);

/* Take the next event. Returns false when the buffer is empty (this
 * never blocks - the display server polls it). */
bool mouse_poll(struct mouse_event *out);

/* True when the device answered the IntelliMouse knock and reports a
 * wheel (diagnostics; every consumer can just read dz). */
bool mouse_has_wheel(void);

/* Packets seen since boot (diagnostics, `sysinfo`). */
uint64_t mouse_packet_count(void);

#endif /* TUS_DRIVERS_MOUSE_H */
