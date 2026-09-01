/*
 * xhci.h - eXtensible Host Controller Interface (USB 3.x) driver
 *
 * WHY xHCI AND NOT THE EHCI DRIVER THAT IS ALREADY HERE
 *
 * A USB keyboard or mouse is almost always a LOW SPEED device, and a
 * low-speed device on an EHCI controller does not talk to EHCI at
 * all: the port is handed to a companion UHCI or OHCI controller, or,
 * behind a high-speed hub, every transfer becomes a split transaction
 * through that hub's transaction translator. Supporting HID on EHCI
 * therefore means writing a second host controller driver (UHCI) or a
 * hub driver with split transaction scheduling.
 *
 * xHCI has neither problem. One controller speaks every speed from
 * low to SuperSpeed natively, there are no companions and no splits,
 * and it is what every machine built since about 2011 actually has.
 * QEMU's `-device qemu-xhci` behaves the same way, so a USB keyboard
 * can be tested rather than assumed.
 *
 * The EHCI driver stays where it is: the ath9k adapter it was written
 * for is a high-speed device, which is the case EHCI does handle.
 *
 * WHAT THIS DRIVER DOES
 *
 *   - brings the controller up (ownership handoff, reset, rings)
 *   - resets ports and enumerates the device on each one
 *   - control transfers on endpoint 0 (Setup / Data / Status TRBs)
 *   - interrupt IN transfers, which is what a HID device reports on
 *   - a kernel task polls the event ring, so hot-plug works
 *
 * What it does not do: hubs (a device plugged into a hub plugged into
 * the machine is not seen), isochronous transfers, bulk transfers,
 * SuperSpeed-specific endpoint companion descriptors, and power
 * management. Each is a real gap, and each is a device class this
 * system has no driver for anyway.
 *
 * CONCURRENCY
 *
 * Everything except the register bring-up runs in one kernel task
 * (`xhci` in the process list). Commands are issued and waited for by
 * that task, so no lock is needed and no allocation happens in
 * interrupt context. The cost is latency: input is polled at
 * XHCI_POLL_MS rather than delivered by an interrupt.
 */

#ifndef TUS_DRIVERS_XHCI_H
#define TUS_DRIVERS_XHCI_H

#include <stdbool.h>
#include <stdint.h>

/* How often the polling task drains the event ring. USB HID devices
 * report every 8-10 ms; polling faster than they report buys nothing,
 * and the event ring means a slow poll loses no reports, only time. */
#define XHCI_POLL_MS 4

/* USB speeds, as the port status register reports them. */
enum {
    XHCI_SPEED_FULL  = 1,
    XHCI_SPEED_LOW   = 2,
    XHCI_SPEED_HIGH  = 3,
    XHCI_SPEED_SUPER = 4,
};

/* A device the driver has addressed and configured. Handed to class
 * drivers, which use the slot id to talk to it. */
struct xhci_device {
    uint8_t  slot;        /* xHCI slot id, 0 = free */
    uint8_t  port;        /* root hub port number (1 based) */
    uint8_t  speed;       /* XHCI_SPEED_* */
    uint16_t vendor;
    uint16_t product;
    uint8_t  dev_class;   /* from the device descriptor */
    uint8_t  if_class;    /* from the interface descriptor */
    uint8_t  if_subclass;
    uint8_t  if_protocol;
    uint8_t  if_number;
    void    *driver_data; /* the class driver's own state */
};

/* A driver for one interface class. Registered before xhci_init();
 * `probe` is called once per matching interface, and returns 0 to
 * claim the device. */
struct xhci_class_driver {
    uint8_t     if_class;
    uint8_t     if_subclass;   /* 0xFF matches any */
    uint8_t     if_protocol;   /* 0xFF matches any */
    const char *name;
    int (*probe)(struct xhci_device *dev);
    /* Called from the polling task with one interrupt-IN report. */
    void (*report)(struct xhci_device *dev, const uint8_t *data, int len);
};

/* Register a class driver. Call before xhci_init(). */
int xhci_register_class_driver(struct xhci_class_driver *drv);

/* Find the controller on the PCI bus and bring it up. Starts the
 * polling task if a controller was found. Returns 0, or -1 when the
 * machine has no xHCI controller (which is not an error - it just
 * means no USB). */
int xhci_init(void);

/* True once a controller is running. */
bool xhci_present(void);

/* ---- what class drivers use ---- */

/* A USB control transfer on endpoint 0. `data` may be NULL for a
 * transfer with no data stage. Returns the number of bytes
 * transferred, or a negative errno. Must be called from the polling
 * task (i.e. from a probe callback). */
int xhci_control(struct xhci_device *dev, uint8_t request_type,
                 uint8_t request, uint16_t value, uint16_t index,
                 void *data, uint16_t length);

/* Start polling an interrupt IN endpoint. The driver queues a
 * transfer, and every completed report is delivered to the class
 * driver's `report` callback before the next one is queued. `size` is
 * the endpoint's maximum packet size, capped at 64. */
int xhci_start_interrupt_in(struct xhci_device *dev, uint8_t endpoint,
                            uint16_t size, uint8_t interval);

/* Print what the controller found (the `usb` shell command). */
void xhci_print_state(void);

#endif /* TUS_DRIVERS_XHCI_H */
