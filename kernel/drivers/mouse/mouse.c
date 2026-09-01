/*
 * mouse.c - PS/2 mouse driver implementation
 *
 * The 8042 keyboard controller has two device ports; the second one
 * ("auxiliary") is where the mouse lives. Talking to it means going
 * through the controller:
 *
 *   0x64 (command)  0xA8 enable the aux port, 0x20/0x60 read/write
 *                   the configuration byte, 0xD4 "the next byte on
 *                   0x60 is for the mouse, not the keyboard"
 *   0x60 (data)     the byte itself, and the device's 0xFA ACKs
 *
 * Once reporting is enabled the device raises IRQ12 for every packet:
 * three bytes, the first carrying the buttons plus the sign and
 * overflow bits of the other two. The handler resynchronises on the
 * "always one" bit, which is what makes a lost byte cost one packet
 * instead of every packet after it.
 *
 * The wheel is an extension: a mouse that recognises the IntelliMouse
 * knock - sample rate 200, then 100, then 80 - answers device id 3 to
 * 0xF2 and from then on sends a FOURTH byte carrying the wheel
 * movement. The knock has to happen before reporting is enabled,
 * because a streaming device would be answering the sample-rate
 * commands and shipping packets at the same time. A device that does
 * not know it keeps id 0 and three-byte packets, so g_packet_bytes is
 * what the handler counts to and nothing else changes.
 */

#include "drivers/mouse/mouse.h"

#include "arch/x86_64/idt.h"
#include "arch/x86_64/io.h"
#include "arch/x86_64/pic.h"

#define PS2_DATA    0x60
#define PS2_STATUS  0x64
#define PS2_COMMAND 0x64

/* Status register bits. */
#define PS2_STATUS_OUTPUT 0x01 /* data waiting to be read */
#define PS2_STATUS_INPUT  0x02 /* the controller is still busy */
#define PS2_STATUS_AUX    0x20 /* the waiting byte came from the mouse */

/* Controller commands. */
#define PS2_CMD_ENABLE_AUX   0xA8
#define PS2_CMD_READ_CONFIG  0x20
#define PS2_CMD_WRITE_CONFIG 0x60
#define PS2_CMD_TO_MOUSE     0xD4

/* Configuration byte bits. */
#define PS2_CFG_IRQ12    0x02 /* raise IRQ12 for aux data */
#define PS2_CFG_AUX_OFF  0x20 /* aux clock disabled */

/* Device commands. */
#define MOUSE_CMD_DEFAULTS   0xF6
#define MOUSE_CMD_ENABLE     0xF4
#define MOUSE_CMD_SAMPLERATE 0xF3
#define MOUSE_CMD_GET_ID     0xF2
#define MOUSE_ACK            0xFA

/* Device id 3: the IntelliMouse (three buttons and a wheel). */
#define MOUSE_ID_WHEEL 3

#define MOUSE_IRQ    12
#define CASCADE_IRQ  2 /* the slave PIC hangs off IRQ2 of the master */

#define MOUSE_BUFFER_SIZE 128

/* ---- state ---- */

static volatile struct mouse_event g_buffer[MOUSE_BUFFER_SIZE];
static volatile int g_head;
static volatile int g_tail;

static uint8_t g_packet[4];
static int g_packet_len;
static int g_packet_bytes = 3; /* 4 once the wheel is negotiated */
static bool g_present;
static bool g_wheel;
static uint64_t g_packets;

/* ---- controller plumbing ---- */

/* Both waits are bounded: a machine without a mouse must not hang the
 * boot, it must simply end up without one. */
static bool ps2_wait_write(void) {
    for (unsigned i = 0; i < 100000; i++) {
        if ((inb(PS2_STATUS) & PS2_STATUS_INPUT) == 0) {
            return true;
        }
    }
    return false;
}

static bool ps2_wait_read(void) {
    for (unsigned i = 0; i < 100000; i++) {
        if ((inb(PS2_STATUS) & PS2_STATUS_OUTPUT) != 0) {
            return true;
        }
    }
    return false;
}

static bool ps2_command(uint8_t cmd) {
    if (!ps2_wait_write()) {
        return false;
    }
    outb(PS2_COMMAND, cmd);
    return true;
}

static bool ps2_write_data(uint8_t data) {
    if (!ps2_wait_write()) {
        return false;
    }
    outb(PS2_DATA, data);
    return true;
}

static int ps2_read_data(void) {
    if (!ps2_wait_read()) {
        return -1;
    }
    return inb(PS2_DATA);
}

/* Send one command to the mouse itself and wait for its ACK. */
static bool mouse_command(uint8_t cmd) {
    if (!ps2_command(PS2_CMD_TO_MOUSE) || !ps2_write_data(cmd)) {
        return false;
    }
    return ps2_read_data() == MOUSE_ACK;
}

/* ---- packet decoding ---- */

static void mouse_push(const struct mouse_event *ev) {
    int next = (g_head + 1) % MOUSE_BUFFER_SIZE;
    if (next != g_tail) {
        g_buffer[g_head] = *ev;
        g_head = next;
    }
}

/* The IntelliMouse knock. Returns the device id the mouse reports
 * afterwards, or -1 when it stops answering. */
static int mouse_negotiate_wheel(void) {
    static const uint8_t knock[] = { 200, 100, 80 };
    for (unsigned i = 0; i < sizeof(knock) / sizeof(knock[0]); i++) {
        /* The rate byte travels the same way the command did, and is
         * acknowledged the same way, so mouse_command() sends both. */
        if (!mouse_command(MOUSE_CMD_SAMPLERATE) ||
            !mouse_command(knock[i])) {
            return -1;
        }
    }
    if (!mouse_command(MOUSE_CMD_GET_ID)) {
        return -1;
    }
    return ps2_read_data();
}

static void mouse_irq_handler(struct interrupt_frame *frame) {
    (void)frame;

    /* The IRQ says "a byte is waiting"; the status register says
     * whether it is ours. */
    uint8_t status = inb(PS2_STATUS);
    if ((status & PS2_STATUS_OUTPUT) == 0) {
        return;
    }
    uint8_t byte = inb(PS2_DATA);
    if ((status & PS2_STATUS_AUX) == 0) {
        return; /* a keyboard byte arrived on the wrong doorstep */
    }

    /* Bit 3 of the first byte is always set. If it is not, the stream
     * is out of step: drop the byte and start a new packet. */
    if (g_packet_len == 0 && (byte & 0x08) == 0) {
        return;
    }
    g_packet[g_packet_len++] = byte;
    if (g_packet_len < g_packet_bytes) {
        return;
    }
    g_packet_len = 0;
    g_packets++;

    uint8_t flags = g_packet[0];
    if ((flags & 0xC0) != 0) {
        return; /* X or Y overflowed: the movement is meaningless */
    }

    struct mouse_event ev;
    /* The sign bit lives in the flags byte, so the movement is a
     * 9-bit two's complement value spread over two bytes. */
    ev.dx = (int32_t)g_packet[1] - (int32_t)((flags << 4) & 0x100);
    ev.dy = (int32_t)g_packet[2] - (int32_t)((flags << 3) & 0x100);
    /* Byte four is a 4-bit two's complement wheel movement (the top
     * nibble belongs to the extra buttons of id 4, which TUS does not
     * ask for). The device counts a push away from the user as -1;
     * everything above this layer wants "up is positive". */
    ev.dz = 0;
    if (g_packet_bytes == 4) {
        int z = g_packet[3] & 0x0F;
        if (z & 0x08) {
            z -= 16;
        }
        ev.dz = -z;
    }
    ev.buttons = flags & (MOUSE_BTN_LEFT | MOUSE_BTN_RIGHT |
                          MOUSE_BTN_MIDDLE);
    mouse_push(&ev);
}

/* ---- public API ---- */

void mouse_init(void) {
    /* Enable the auxiliary port and let it interrupt. */
    if (!ps2_command(PS2_CMD_ENABLE_AUX)) {
        return;
    }
    if (!ps2_command(PS2_CMD_READ_CONFIG)) {
        return;
    }
    int config = ps2_read_data();
    if (config < 0) {
        return;
    }
    config |= PS2_CFG_IRQ12;
    config &= ~PS2_CFG_AUX_OFF;
    if (!ps2_command(PS2_CMD_WRITE_CONFIG) ||
        !ps2_write_data((uint8_t)config)) {
        return;
    }

    if (!mouse_command(MOUSE_CMD_DEFAULTS)) {
        return;
    }

    /* Ask for the wheel while the device is still quiet. A refusal
     * costs nothing: the packets stay three bytes long. */
    if (mouse_negotiate_wheel() == MOUSE_ID_WHEEL) {
        g_wheel = true;
        g_packet_bytes = 4;
    }

    if (!mouse_command(MOUSE_CMD_ENABLE)) {
        return;
    }

    irq_install(MOUSE_IRQ, mouse_irq_handler);
    pic_enable_irq(CASCADE_IRQ); /* without this the slave PIC is mute */
    pic_enable_irq(MOUSE_IRQ);
    g_present = true;
}

/* Feed one movement report from an input source that is not the PS/2
 * aux port - today, a USB mouse (drivers/usbhid.c).
 *
 * It goes into the same ring the PS/2 packets go into, in the same
 * convention (X right, Y UP - the display server is the one place
 * that flips Y), so the highX pointer code has no idea which bus a
 * movement came from. Reporting the device as present is part of the
 * job: highX hides the cursor when mouse_present() is false, and a
 * USB mouse on a machine with no PS/2 mouse would otherwise move an
 * arrow nobody can see. */
void mouse_inject(int32_t dx, int32_t dy, int32_t dz, uint32_t buttons) {
    struct mouse_event ev = { dx, dy, dz, buttons };
    g_present = true;
    mouse_push(&ev);
}

bool mouse_present(void) {
    return g_present;
}

bool mouse_has_wheel(void) {
    return g_wheel;
}

bool mouse_has_event(void) {
    return g_head != g_tail;
}

bool mouse_poll(struct mouse_event *out) {
    if (g_head == g_tail) {
        return false;
    }
    if (out != NULL) {
        *out = g_buffer[g_tail];
    }
    g_tail = (g_tail + 1) % MOUSE_BUFFER_SIZE;
    return true;
}

uint64_t mouse_packet_count(void) {
    return g_packets;
}
