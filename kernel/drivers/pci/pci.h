#ifndef __PCI_H__
#define __PCI_H__

#include <stdint.h>
#include <stddef.h>

#define PCI_CONFIG_ADDR 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#define PCI_VENDOR_ID       0x00
#define PCI_DEVICE_ID       0x02
#define PCI_COMMAND         0x04
#define PCI_STATUS          0x06
#define PCI_REVISION        0x08
#define PCI_PROG_IF         0x09
#define PCI_SUBCLASS        0x0A
#define PCI_CLASS           0x0B
#define PCI_HEADER_TYPE     0x0E
#define PCI_BAR0            0x10
#define PCI_BAR1            0x14
#define PCI_BAR2            0x18
#define PCI_BAR3            0x1C
#define PCI_BAR4            0x20
#define PCI_BAR5            0x24

#define PCI_COMMAND_IO_SPACE      0x0001
#define PCI_COMMAND_MEM_SPACE     0x0002
#define PCI_COMMAND_MASTER        0x0004

#define PCI_HEADER_TYPE_DEVICE   0x00
#define PCI_HEADER_TYPE_BRIDGE   0x01
#define PCI_HEADER_TYPE_CARDBUS  0x02

typedef struct {
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_code;
    uint8_t subclass_code;
    uint8_t prog_if;
    uint32_t bar[6];
} pci_device_t;

typedef int (*pci_driver_init_t)(pci_device_t *dev);

typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_code;
    uint8_t subclass_code;
    uint8_t prog_if;
    const char *name;
    pci_driver_init_t init;
} pci_driver_t;

uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg);
void pci_config_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t value);
int pci_enumerate_devices(void);
int pci_register_driver(pci_driver_t *driver);

/* Walk a function's capability list (PCI_STATUS bit 4 gates whether
 * one even exists) looking for `cap_id`. Returns the config-space
 * byte offset of the capability header, or -1 if the function has no
 * capability list or none of that id. */
int pci_find_capability(uint8_t bus, uint8_t device, uint8_t function, uint8_t cap_id);

#define PCI_CAP_ID_MSI  0x05
#define PCI_CAP_ID_MSIX 0x11 /* found, but not programmed - see below */

/* Program a function's MSI capability (if it has one) to deliver
 * `vector` to the Local APIC named by `dest_lapic_id`, fixed delivery
 * mode - the same targeting an I/O APIC redirection entry uses - and
 * set its Enable bit. Handles both the 32-bit and 64-bit Message
 * Address capability layouts. Returns 0 on success, -1 if the
 * function has no MSI capability.
 *
 * This does NOT reroute any existing TUS driver's interrupt: enabling
 * MSI on a function makes it stop using its INTx pin (a PCI spec
 * requirement, not a TUS choice), so calling this on a device whose
 * driver still expects the legacy pin - everything in this kernel
 * today - would make it go deaf rather than switch delivery paths.
 * It is real, reusable capability-programming infrastructure for a
 * future driver written to install an ISR at `vector` and use it.
 *
 * MSI-X (capability id 0x11, PCI_CAP_ID_MSIX) is NOT handled - it is
 * a different, table-based capability (per-vector entries live in an
 * MMIO BAR, not in config space) and is what most virtual NICs
 * (virtio-net, e1000e) actually expose instead of plain MSI. Finding
 * one is possible with pci_find_capability(..., PCI_CAP_ID_MSIX) - the
 * `msi` shell command reports it separately for exactly that reason -
 * but programming it is future work. */
int pci_msi_enable(uint8_t bus, uint8_t device, uint8_t function,
                   uint32_t dest_lapic_id, uint8_t vector);

/* Undo pci_msi_enable(): clear the Enable bit. Returns 0 on success,
 * -1 if the function has no MSI capability. */
int pci_msi_disable(uint8_t bus, uint8_t device, uint8_t function);

#endif
