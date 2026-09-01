/*
 * usbhid.c - USB keyboard and mouse (HID boot protocol)
 *
 * See usbhid.h for the shape of the thing. The interesting part is
 * the keyboard, and the interesting part of the keyboard is that USB
 * and PS/2 disagree about what a keyboard event IS.
 *
 * A PS/2 keyboard sends an EVENT: "the A key went down", "the A key
 * came up". A USB keyboard sends STATE: eight bytes saying which
 * modifiers are held and which (up to six) keys are down right now,
 * repeated every few milliseconds whether anything changed or not.
 *
 * So this driver keeps the previous report and diffs it. A usage in
 * the new report that was not in the old one is a press; one in the
 * old that is not in the new is a release. Each becomes a scancode
 * set 1 make or break code and goes to kbd_feed_scancode(), which is
 * the PS/2 driver's decoder - so the modifier state, Caps Lock and
 * its LED, the E0-prefixed arrows and Ctrl-as-control-character are
 * all the same code for both buses, and a machine with both kinds of
 * keyboard has one keyboard's worth of state.
 *
 * The rollover byte (usage 0x01, "too many keys at once") is a report
 * that says nothing about individual keys; treating its bytes as
 * usages would type garbage, so such a report is dropped whole and
 * the previous state is left alone.
 */

#include "drivers/usbhid/usbhid.h"

#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "drivers/xhci/xhci.h"
#include "../../core/errno.h"
#include "../../core/klib.h"

/* HID class requests (bmRequestType 0x21, interface recipient). */
#define HID_REQ_SET_IDLE     0x0A
#define HID_REQ_SET_PROTOCOL 0x0B
#define HID_PROTOCOL_BOOT    0

/* Interface class / subclass / protocol for the boot devices. */
#define HID_CLASS            0x03
#define HID_SUBCLASS_BOOT    0x01
#define HID_PROTOCOL_KEYBOARD 0x01
#define HID_PROTOCOL_MOUSE    0x02

/* ---- HID usage -> scancode set 1 ----
 *
 * Indexed by HID keyboard usage (0x00..0x73). The value is the set 1
 * make code; 0 means "no key TUS has a scancode for". Codes that a
 * PS/2 keyboard sends with an E0 prefix are listed in kbd_extended[]
 * below instead, because the prefix has to be fed separately.
 *
 * The table is the standard one - HID usage 0x04 is 'a', which a PS/2
 * keyboard reports as scancode 0x1E - and is written out rather than
 * computed because there is no formula: the two layouts were designed
 * two decades apart.
 */
static const uint8_t hid_to_sc1[0x74] = {
    [0x04] = 0x1E, /* a */  [0x05] = 0x30, /* b */
    [0x06] = 0x2E, /* c */  [0x07] = 0x20, /* d */
    [0x08] = 0x12, /* e */  [0x09] = 0x21, /* f */
    [0x0A] = 0x22, /* g */  [0x0B] = 0x23, /* h */
    [0x0C] = 0x17, /* i */  [0x0D] = 0x24, /* j */
    [0x0E] = 0x25, /* k */  [0x0F] = 0x26, /* l */
    [0x10] = 0x32, /* m */  [0x11] = 0x31, /* n */
    [0x12] = 0x18, /* o */  [0x13] = 0x19, /* p */
    [0x14] = 0x10, /* q */  [0x15] = 0x13, /* r */
    [0x16] = 0x1F, /* s */  [0x17] = 0x14, /* t */
    [0x18] = 0x16, /* u */  [0x19] = 0x2F, /* v */
    [0x1A] = 0x11, /* w */  [0x1B] = 0x2D, /* x */
    [0x1C] = 0x15, /* y */  [0x1D] = 0x2C, /* z */
    [0x1E] = 0x02, /* 1 */  [0x1F] = 0x03, /* 2 */
    [0x20] = 0x04, /* 3 */  [0x21] = 0x05, /* 4 */
    [0x22] = 0x06, /* 5 */  [0x23] = 0x07, /* 6 */
    [0x24] = 0x08, /* 7 */  [0x25] = 0x09, /* 8 */
    [0x26] = 0x0A, /* 9 */  [0x27] = 0x0B, /* 0 */
    [0x28] = 0x1C, /* Enter */
    [0x29] = 0x01, /* Escape */
    [0x2A] = 0x0E, /* Backspace */
    [0x2B] = 0x0F, /* Tab */
    [0x2C] = 0x39, /* Space */
    [0x2D] = 0x0C, /* - */
    [0x2E] = 0x0D, /* = */
    [0x2F] = 0x1A, /* [ */
    [0x30] = 0x1B, /* ] */
    [0x31] = 0x2B, /* backslash */
    [0x33] = 0x27, /* ; */
    [0x34] = 0x28, /* ' */
    [0x35] = 0x29, /* ` */
    [0x36] = 0x33, /* , */
    [0x37] = 0x34, /* . */
    [0x38] = 0x35, /* / */
    [0x39] = 0x3A, /* Caps Lock */
    [0x3A] = 0x3B, [0x3B] = 0x3C, [0x3C] = 0x3D, [0x3D] = 0x3E, /* F1-F4 */
    [0x3E] = 0x3F, [0x3F] = 0x40, [0x40] = 0x41, [0x41] = 0x42, /* F5-F8 */
    [0x42] = 0x43, [0x43] = 0x44, [0x44] = 0x57, [0x45] = 0x58, /* F9-F12 */
};

/* Usages a PS/2 keyboard sends behind an E0 prefix. Same layout as
 * the table above; a non-zero entry means "feed 0xE0 first". */
static const uint8_t hid_extended[0x74] = {
    [0x46] = 0x37, /* Print Screen */
    [0x49] = 0x52, /* Insert */
    [0x4A] = 0x47, /* Home */
    [0x4B] = 0x49, /* Page Up */
    [0x4C] = 0x53, /* Delete */
    [0x4D] = 0x4F, /* End */
    [0x4E] = 0x51, /* Page Down */
    [0x4F] = 0x4D, /* Right */
    [0x50] = 0x4B, /* Left */
    [0x51] = 0x50, /* Down */
    [0x52] = 0x48, /* Up */
    [0x58] = 0x1C, /* Keypad Enter */
};

/* The eight modifier bits of byte 0, in order, as (extended, code)
 * pairs. Right Ctrl, right Alt and both Super keys are E0-prefixed on
 * PS/2; the left Ctrl, Shift and Alt are not. */
static const struct {
    uint8_t extended;
    uint8_t code;
} hid_modifiers[8] = {
    { 0, 0x1D }, /* left Ctrl   */
    { 0, 0x2A }, /* left Shift  */
    { 0, 0x38 }, /* left Alt    */
    { 1, 0x5B }, /* left Super  */
    { 1, 0x1D }, /* right Ctrl  */
    { 0, 0x36 }, /* right Shift */
    { 1, 0x38 }, /* right Alt   */
    { 1, 0x5C }, /* right Super */
};

/* ---- keyboard ---- */

static bool     g_kbd_present;
static uint8_t  g_prev[8];
static bool     g_have_prev;
/* Reports seen and keys decoded, so `usb` can prove a keystroke came
 * over USB rather than over the PS/2 port sitting next to it. Without
 * a number here, a test that types into QEMU and sees the shell answer
 * has shown only that SOME keyboard works. */
static uint64_t g_kbd_reports;
static uint64_t g_kbd_keys;

static void feed_key(uint8_t usage, bool make) {
    if (usage >= 0x74) {
        return;
    }
    uint8_t code = hid_to_sc1[usage];
    bool extended = false;
    if (code == 0) {
        code = hid_extended[usage];
        if (code == 0) {
            return; /* a key TUS has no scancode for */
        }
        extended = true;
    }
    if (extended) {
        kbd_feed_scancode(0xE0);
    }
    kbd_feed_scancode(make ? code : (uint8_t)(code | 0x80));
    if (make) {
        g_kbd_keys++;
    }
}

/* Was this usage in that report's six-key array? */
static bool in_report(const uint8_t *report, uint8_t usage) {
    for (int i = 2; i < 8; i++) {
        if (report[i] == usage) {
            return true;
        }
    }
    return false;
}

static void keyboard_report(struct xhci_device *dev, const uint8_t *data,
                            int len) {
    (void)dev;
    if (len < 8) {
        return;
    }

    /* Usage 0x01 in the key array is ErrorRollOver: the device is
     * saying "more keys are down than I can report", not naming keys.
     * Acting on it would press whatever 0x01 maps to, repeatedly. */
    for (int i = 2; i < 8; i++) {
        if (data[i] == 0x01) {
            return;
        }
    }

    g_kbd_reports++;

    if (!g_have_prev) {
        memset(g_prev, 0, sizeof(g_prev));
        g_have_prev = true;
    }

    /* Modifiers first: a Shift that went down in the same report as
     * the letter it shifts has to be seen first, or the letter comes
     * out lowercase. */
    uint8_t changed = (uint8_t)(data[0] ^ g_prev[0]);
    for (int bit = 0; bit < 8; bit++) {
        if ((changed & (1u << bit)) == 0) {
            continue;
        }
        bool make = (data[0] & (1u << bit)) != 0;
        if (hid_modifiers[bit].extended) {
            kbd_feed_scancode(0xE0);
        }
        kbd_feed_scancode(make ? hid_modifiers[bit].code
                               : (uint8_t)(hid_modifiers[bit].code | 0x80));
    }

    /* Releases before presses, so a key rolled over from one to the
     * next does not look held. */
    for (int i = 2; i < 8; i++) {
        uint8_t usage = g_prev[i];
        if (usage > 0x03 && !in_report(data, usage)) {
            feed_key(usage, false);
        }
    }
    for (int i = 2; i < 8; i++) {
        uint8_t usage = data[i];
        if (usage > 0x03 && !in_report(g_prev, usage)) {
            feed_key(usage, true);
        }
    }

    memcpy(g_prev, data, 8);
}

static int keyboard_probe(struct xhci_device *dev) {
    /* Boot protocol, and no idle rate: SET_IDLE(0) tells the device
     * to report only when something changes rather than every few
     * milliseconds forever. A device that refuses either request
     * still works - both are optional - so neither is fatal. */
    (void)xhci_control(dev, 0x21, HID_REQ_SET_PROTOCOL, HID_PROTOCOL_BOOT,
                       dev->if_number, NULL, 0);
    (void)xhci_control(dev, 0x21, HID_REQ_SET_IDLE, 0, dev->if_number,
                       NULL, 0);

    g_kbd_present = true;
    g_have_prev = false;
    kprintf("[usbhid] keyboard on slot %u\n", dev->slot);
    return 0;
}

/* ---- mouse ---- */

static bool     g_mouse_present;
static uint64_t g_mouse_reports;

static void mouse_report(struct xhci_device *dev, const uint8_t *data,
                         int len) {
    (void)dev;
    if (len < 3) {
        return;
    }

    /* Boot protocol: byte 0 buttons, byte 1 dx, byte 2 dy, both
     * signed. Byte 3 is the wheel on devices that have one - not part
     * of the boot report, but every real mouse appends it and a
     * device that does not simply sends three bytes. */
    uint32_t buttons = 0;
    if (data[0] & 0x01) buttons |= MOUSE_BTN_LEFT;
    if (data[0] & 0x02) buttons |= MOUSE_BTN_RIGHT;
    if (data[0] & 0x04) buttons |= MOUSE_BTN_MIDDLE;

    int32_t dx = (int8_t)data[1];
    int32_t dy = (int8_t)data[2];
    int32_t dz = (len >= 4) ? (int32_t)(int8_t)data[3] : 0;

    g_mouse_reports++;

    /* HID counts Y downward (screen convention); the TUS mouse layer
     * counts it upward (PS/2 convention) and the display server flips
     * it once. Flip here so both buses hand it the same thing. */
    mouse_inject(dx, -dy, dz, buttons);
}

static int mouse_probe(struct xhci_device *dev) {
    (void)xhci_control(dev, 0x21, HID_REQ_SET_PROTOCOL, HID_PROTOCOL_BOOT,
                       dev->if_number, NULL, 0);
    (void)xhci_control(dev, 0x21, HID_REQ_SET_IDLE, 0, dev->if_number,
                       NULL, 0);

    g_mouse_present = true;
    kprintf("[usbhid] mouse on slot %u\n", dev->slot);
    return 0;
}

/* ---- registration ---- */

static struct xhci_class_driver g_keyboard_driver = {
    .if_class = HID_CLASS,
    .if_subclass = HID_SUBCLASS_BOOT,
    .if_protocol = HID_PROTOCOL_KEYBOARD,
    .name = "usb-keyboard",
    .probe = keyboard_probe,
    .report = keyboard_report,
};

static struct xhci_class_driver g_mouse_driver = {
    .if_class = HID_CLASS,
    .if_subclass = HID_SUBCLASS_BOOT,
    .if_protocol = HID_PROTOCOL_MOUSE,
    .name = "usb-mouse",
    .probe = mouse_probe,
    .report = mouse_report,
};

void usbhid_init(void) {
    xhci_register_class_driver(&g_keyboard_driver);
    xhci_register_class_driver(&g_mouse_driver);
}

bool usbhid_keyboard_present(void) {
    return g_kbd_present;
}

bool usbhid_mouse_present(void) {
    return g_mouse_present;
}

void usbhid_print_state(void) {
    if (!g_kbd_present && !g_mouse_present) {
        return;
    }
    if (g_kbd_present) {
        kprintf("  usb-keyboard: %lu reports, %lu keys pressed\n",
                (unsigned long)g_kbd_reports, (unsigned long)g_kbd_keys);
    }
    if (g_mouse_present) {
        kprintf("  usb-mouse   : %lu reports\n",
                (unsigned long)g_mouse_reports);
    }
}
