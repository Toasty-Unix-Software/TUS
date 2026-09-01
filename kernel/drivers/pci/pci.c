#include "drivers/pci/pci.h"
#include "arch/x86_64/io.h"
#include "core/klib.h"

#define PCI_MAX_DRIVERS 32

static pci_driver_t *pci_drivers[PCI_MAX_DRIVERS];
static int pci_driver_count = 0;

uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg)
{
	uint32_t addr = 0x80000000 | ((uint32_t)bus << 16) | ((uint32_t)device << 11) |
		((uint32_t)function << 8) | (reg & 0xFC);

	outl(PCI_CONFIG_ADDR, addr);
	return inl(PCI_CONFIG_DATA);
}

void pci_config_write(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg, uint32_t value)
{
	uint32_t addr = 0x80000000 | ((uint32_t)bus << 16) | ((uint32_t)device << 11) |
		((uint32_t)function << 8) | (reg & 0xFC);

	outl(PCI_CONFIG_ADDR, addr);
	outl(PCI_CONFIG_DATA, value);
}

#define PCI_STATUS_CAPLIST 0x0010 /* PCI_STATUS bit 4 */
#define PCI_CAP_PTR        0x34

int pci_find_capability(uint8_t bus, uint8_t device, uint8_t function, uint8_t cap_id)
{
	uint16_t status = (uint16_t)(pci_config_read(bus, device, function, PCI_STATUS) >> 16);
	if (!(status & PCI_STATUS_CAPLIST)) {
		return -1;
	}

	uint8_t ptr = (uint8_t)(pci_config_read(bus, device, function, PCI_CAP_PTR) & 0xFF);
	/* The list is walked by a byte offset each capability header
	 * carries to the next one; a guard against a corrupt or looping
	 * list is cheap insurance a real device will never trigger (PCI
	 * config space is 256 bytes, so 64 capabilities is already an
	 * impossible number of them). */
	for (int guard = 0; ptr != 0 && guard < 64; guard++) {
		uint32_t header = pci_config_read(bus, device, function, ptr);
		if ((header & 0xFF) == cap_id) {
			return ptr;
		}
		ptr = (uint8_t)((header >> 8) & 0xFF);
	}
	return -1;
}

int pci_msi_enable(uint8_t bus, uint8_t device, uint8_t function,
                   uint32_t dest_lapic_id, uint8_t vector)
{
	int cap = pci_find_capability(bus, device, function, PCI_CAP_ID_MSI);
	if (cap < 0) {
		return -1;
	}
	uint8_t off = (uint8_t)cap;

	uint32_t header = pci_config_read(bus, device, function, off);
	uint16_t control = (uint16_t)(header >> 16);
	int addr64 = (control & (1u << 7)) != 0;

	/* Message address: fixed delivery to one Local APIC, physical
	 * destination mode, no redirection hint - the same targeting an
	 * I/O APIC redirection entry uses (see ioapic_set_redirection()). */
	pci_config_write(bus, device, function, off + 4, 0xFEE00000u | (dest_lapic_id << 12));

	/* Message data: vector in bits 7:0, delivery mode 0 (fixed) in
	 * bits 10:8, everything else 0 - MSI has no separate trigger-mode
	 * concept, it is inherently edge. Lives right after the address
	 * (8 bytes further if the address is 64-bit wide). */
	uint8_t data_off = (uint8_t)(off + (addr64 ? 0x0C : 0x08));
	if (addr64) {
		pci_config_write(bus, device, function, off + 8, 0); /* address upper */
	}
	uint32_t data_dword = pci_config_read(bus, device, function, data_off);
	data_dword = (data_dword & 0xFFFF0000u) | vector;
	pci_config_write(bus, device, function, data_off, data_dword);

	/* Enable bit (bit 0 of Message Control, the high 16 bits of the
	 * same dword as the capability header). */
	control |= 0x0001;
	pci_config_write(bus, device, function, off,
	                 (header & 0x0000FFFFu) | ((uint32_t)control << 16));
	return 0;
}

int pci_msi_disable(uint8_t bus, uint8_t device, uint8_t function)
{
	int cap = pci_find_capability(bus, device, function, PCI_CAP_ID_MSI);
	if (cap < 0) {
		return -1;
	}
	uint8_t off = (uint8_t)cap;
	uint32_t header = pci_config_read(bus, device, function, off);
	uint16_t control = (uint16_t)(header >> 16);
	control &= (uint16_t)~0x0001;
	pci_config_write(bus, device, function, off,
	                 (header & 0x0000FFFFu) | ((uint32_t)control << 16));
	return 0;
}

int pci_register_driver(pci_driver_t *driver)
{
	if (pci_driver_count >= PCI_MAX_DRIVERS) {
		return -1;
	}
	pci_drivers[pci_driver_count++] = driver;
	return 0;
}

int pci_enumerate_devices(void)
{
	klog("[pci] enumerating devices...\n");

	for (int bus = 0; bus < 2; bus++) {
		for (int device = 0; device < 32; device++) {
			for (int function = 0; function < 8; function++) {
				uint32_t vendor_device = pci_config_read(bus, device, function, PCI_VENDOR_ID);
				uint16_t vendor_id = vendor_device & 0xFFFF;

				if (vendor_id == 0xFFFF) {
					break;
				}

				uint16_t device_id = (vendor_device >> 16) & 0xFFFF;
				uint32_t class_info = pci_config_read(bus, device, function, PCI_REVISION);
				uint8_t class_code = (class_info >> 24) & 0xFF;
				uint8_t subclass_code = (class_info >> 16) & 0xFF;
				uint8_t prog_if = (class_info >> 8) & 0xFF;

				pci_device_t dev = {
					.bus = bus,
					.device = device,
					.function = function,
					.vendor_id = vendor_id,
					.device_id = device_id,
					.class_code = class_code,
					.subclass_code = subclass_code,
					.prog_if = prog_if,
				};

				for (int i = 0; i < 6; i++) {
					dev.bar[i] = pci_config_read(bus, device, function, PCI_BAR0 + i * 4);
				}

				/* Offset 0x3C: interrupt line (byte 0) / pin (byte 1) -
				 * no PCI_INTERRUPT_LINE define exists in pci.h yet, so
				 * read the raw config space word here rather than add
				 * one for a single call site. */
				uint32_t irq_info = pci_config_read(bus, device, function, 0x3C);
				uint8_t irq_line = irq_info & 0xFF;
				uint8_t irq_pin = (irq_info >> 8) & 0xFF;

				klog("[pci] %x:%x.%x - %x:%x (class %x.%x.%x) "
					"bar0=0x%x irq=%u pin=%u\n",
					bus, device, function, vendor_id, device_id,
					class_code, subclass_code, prog_if,
					dev.bar[0], irq_line, irq_pin);

				for (int i = 0; i < pci_driver_count; i++) {
					pci_driver_t *driver = pci_drivers[i];
					if ((driver->vendor_id == 0xFFFF || driver->vendor_id == vendor_id) &&
						(driver->device_id == 0xFFFF || driver->device_id == device_id) &&
						(driver->class_code == class_code) &&
						(driver->subclass_code == subclass_code) &&
						(driver->prog_if == prog_if)) {

						klog("[pci] matched driver: %s\n", driver->name);
						if (driver->init(&dev) == 0) {
							klog("[pci] %s initialized\n", driver->name);
						}
					}
				}

				uint8_t header_type = pci_config_read(bus, device, function, PCI_HEADER_TYPE) & 0x7F;
				if (header_type != PCI_HEADER_TYPE_DEVICE) {
					break;
				}
			}
		}
	}

	return 0;
}
