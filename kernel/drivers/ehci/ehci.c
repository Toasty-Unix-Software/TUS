#include "drivers/ehci/ehci.h"
#include "drivers/pci/pci.h"
#include "../../core/klib.h"
#include "../../mm/kmalloc.h"
#include "../../mm/vmm.h"

ehci_controller_t ehci_ctrl;

static ehci_qh_t *qh_pool[EHCI_MAX_DEVICES];
static ehci_qtd_t *qtd_pool[EHCI_MAX_DEVICES * 8];
static int qh_count = 0;
static int qtd_count = 0;

#define MAX_CLASS_DRIVERS 16
static usb_class_driver_t *class_drivers[MAX_CLASS_DRIVERS];
static int class_driver_count = 0;

static inline uint32_t ehci_read(uint32_t offset)
{
	return *(volatile uint32_t *)((uintptr_t)ehci_ctrl.regs + offset);
}

static inline void ehci_write(uint32_t offset, uint32_t value)
{
	*(volatile uint32_t *)((uintptr_t)ehci_ctrl.regs + offset) = value;
}

static ehci_qh_t *ehci_alloc_qh(void)
{
	if (qh_count >= EHCI_MAX_DEVICES) return NULL;

	ehci_qh_t *qh = kmalloc(sizeof(ehci_qh_t));
	if (!qh) return NULL;

	for (int i = 0; i < sizeof(ehci_qh_t); i++)
		((uint8_t *)qh)[i] = 0;

	qh_pool[qh_count++] = qh;
	return qh;
}

static ehci_qtd_t *ehci_alloc_qtd(void)
{
	if (qtd_count >= EHCI_MAX_DEVICES * 8) return NULL;

	ehci_qtd_t *qtd = kmalloc(sizeof(ehci_qtd_t));
	if (!qtd) return NULL;

	for (int i = 0; i < sizeof(ehci_qtd_t); i++)
		((uint8_t *)qtd)[i] = 0;

	qtd_pool[qtd_count++] = qtd;
	return qtd;
}

static uint32_t virt_to_phys(void *virt)
{
	return (uint32_t)(uintptr_t)virt;
}

static void ehci_wait_ms(int ms)
{
	for (int i = 0; i < ms; i++) {
		for (volatile int j = 0; j < 1000000; j++);
	}
}

static int ehci_reset_controller(void)
{
	if (!ehci_ctrl.regs) return -1;

	ehci_write(0, ehci_read(0) | EHCI_CMD_RESET);

	for (int i = 0; i < 100; i++) {
		if (!(ehci_read(0) & EHCI_CMD_RESET))
			return 0;
		for (volatile int j = 0; j < 1000000; j++);
	}
	kprintf("[ehci] reset timeout\n");
	return -1;
}

static int ehci_start_controller(void)
{
	if (!ehci_ctrl.regs) return -1;

	uint32_t cmd = ehci_read(0);
	cmd |= EHCI_CMD_RUN | EHCI_CMD_ASYNC_EN | EHCI_CMD_PERIODIC_EN | EHCI_CMD_IOCE;
	ehci_write(0, cmd);

	for (int i = 0; i < 100; i++) {
		uint32_t status = ehci_read(4);
		if (!(status & EHCI_STATUS_HCHALTED))
			return 0;
		for (volatile int j = 0; j < 1000000; j++);
	}
	kprintf("[ehci] start timeout\n");
	return -1;
}

static int ehci_wait_for_device_connection(int port)
{
	uint32_t port_reg = 0x44 + (port * 4);

	for (int i = 0; i < 500; i++) {
		uint32_t status = ehci_read(port_reg);
		if (status & EHCI_PORT_CONNECTED)
			return 0;
		for (volatile int j = 0; j < 100000; j++);
	}
	return -1;
}

static int ehci_reset_port(int port)
{
	uint32_t port_reg = 0x44 + (port * 4);
	uint32_t status = ehci_read(port_reg);

	ehci_write(port_reg, status | EHCI_PORT_RESET);

	for (int i = 0; i < 1000; i++) {
		status = ehci_read(port_reg);
		if (!(status & EHCI_PORT_RESET)) {
			if (status & EHCI_PORT_ENABLED)
				return 0;
			break;
		}
		for (volatile int j = 0; j < 10000; j++);
	}
	return -1;
}

static int ehci_set_address(int device_addr)
{
	usb_setup_t setup;
	setup.request_type = 0x00;
	setup.request = 0x05;
	setup.value = device_addr;
	setup.index = 0;
	setup.length = 0;

	kprintf("[ehci] sending SET_ADDRESS(%d) to device 0\n", device_addr);

	int result = ehci_control_transfer(0, &setup, NULL);
	if (result < 0) {
		kprintf("[ehci] SET_ADDRESS failed\n");
		return -1;
	}

	ehci_wait_ms(10);
	return 0;
}

static int ehci_get_device_descriptor(int device_addr, usb_device_descriptor_t *desc)
{
	if (!desc) return -1;

	usb_setup_t setup;
	setup.request_type = 0x80;
	setup.request = 0x06;
	setup.value = (1 << 8);
	setup.index = 0;
	setup.length = 18;

	kprintf("[ehci] sending GET_DESCRIPTOR to device %d\n", device_addr);

	int result = ehci_control_transfer(device_addr, &setup, desc);
	if (result < 0) {
		kprintf("[ehci] GET_DESCRIPTOR failed\n");
		return -1;
	}

	return 0;
}

int ehci_bulk_transfer(int device_addr, int endpoint, int dir, void *data, int len)
{
	if (!data || len <= 0 || !ehci_ctrl.regs) return -1;

	ehci_qh_t *qh = ehci_alloc_qh();
	if (!qh) return -1;

	ehci_qtd_t *data_qtd = ehci_alloc_qtd();
	ehci_qtd_t *status_qtd = ehci_alloc_qtd();

	if (!data_qtd || !status_qtd) return -1;

	qh->ep_char = 0;
	qh->ep_char |= (device_addr & 0x7F) << 0;
	qh->ep_char |= (endpoint & 0x0F) << 8;
	qh->ep_char |= 64 << 16;
	qh->ep_char |= (1 << 27);

	data_qtd->token = EHCI_QTD_ACTIVE | (len << 16) | (dir << 8);
	data_qtd->buf_ptr[0] = virt_to_phys(data);
	data_qtd->next = virt_to_phys(status_qtd);
	data_qtd->alt_next = 1;

	status_qtd->token = EHCI_QTD_ACTIVE | (0 << 16) | (1 - dir) << 8 | EHCI_QTD_TOGGLE;
	status_qtd->buf_ptr[0] = 0;
	status_qtd->next = 1;
	status_qtd->alt_next = 1;

	qh->current_qtd = 0;
	qh->next_qtd = virt_to_phys(data_qtd) | EHCI_QTD_TOGGLE;
	qh->alt_next_qtd = 1;

	ehci_write(0x08, virt_to_phys(qh) | EHCI_QH_HEAD);

	for (int i = 0; i < 100; i++) {
		if (!(status_qtd->token & EHCI_QTD_ACTIVE)) {
			if (status_qtd->token & 0x7E) {
				kprintf("[ehci] bulk transfer error: 0x%x\n", status_qtd->token);
				return -1;
			}
			return 0;
		}
		ehci_wait_ms(1);
	}

	kprintf("[ehci] bulk transfer timeout\n");
	return -1;
}

int ehci_interrupt_transfer(int device_addr, int endpoint, void *data, int len, int interval)
{
	if (!data || len <= 0) return -1;

	ehci_qh_t *qh = ehci_alloc_qh();
	if (!qh) return -1;

	ehci_qtd_t *int_qtd = ehci_alloc_qtd();
	if (!int_qtd) return -1;

	qh->ep_char = 0;
	qh->ep_char |= (device_addr & 0x7F) << 0;
	qh->ep_char |= (endpoint & 0x0F) << 8;
	qh->ep_char |= 64 << 16;
	qh->ep_char |= (1 << 27);

	qh->ep_cap = 0;
	qh->ep_cap |= (interval & 0xFF) << 0;

	int_qtd->token = EHCI_QTD_ACTIVE | (len << 16) | 0;
	int_qtd->buf_ptr[0] = virt_to_phys(data);
	int_qtd->next = 1;
	int_qtd->alt_next = 1;

	qh->current_qtd = 0;
	qh->next_qtd = virt_to_phys(int_qtd) | EHCI_QTD_TOGGLE;
	qh->alt_next_qtd = 1;

	for (int i = 0; i < 100; i++) {
		if (!(int_qtd->token & EHCI_QTD_ACTIVE)) {
			if (int_qtd->token & 0x7E) {
				return -1;
			}
			return 0;
		}
		ehci_wait_ms(1);
	}

	return -1;
}

int ehci_register_class_driver(usb_class_driver_t *driver)
{
	if (!driver || class_driver_count >= MAX_CLASS_DRIVERS) {
		return -1;
	}

	class_drivers[class_driver_count++] = driver;
	kprintf("[ehci] registered driver for class %02x.%02x (%s)\n",
		driver->class_code, driver->subclass_code, driver->name);

	return 0;
}

int ehci_probe_device_drivers(int device_idx)
{
	if (device_idx >= ehci_ctrl.num_devices || device_idx < 0) return -1;

	usb_device_t *dev = &ehci_ctrl.devices[device_idx];
	if (!dev->descriptor || !dev->configured) return -1;

	kprintf("[ehci] probing drivers for device %d (class %02x.%02x)\n",
		dev->device_address, dev->device_class, dev->device_subclass);

	for (int i = 0; i < class_driver_count; i++) {
		usb_class_driver_t *drv = class_drivers[i];

		if ((drv->class_code == 0xFF || drv->class_code == dev->device_class) &&
			(drv->subclass_code == 0xFF || drv->subclass_code == dev->device_subclass)) {

			kprintf("[ehci] probe %s...\n", drv->name);

			if (drv->probe(dev) == 0) {
				kprintf("[ehci] %s attached to device %d\n",
					drv->name, dev->device_address);
				return 0;
			}
		}
	}

	kprintf("[ehci] no driver found for device %d\n", dev->device_address);
	return -1;
}

int ehci_enumerate_device(int port)
{
	if (port >= (int)ehci_ctrl.num_ports) return -1;

	kprintf("[ehci] enumerating device on port %d\n", port);

	if (ehci_wait_for_device_connection(port) < 0) {
		kprintf("[ehci] port %d: no device connected\n", port);
		return -1;
	}

	if (ehci_reset_port(port) < 0) {
		kprintf("[ehci] port %d: reset failed\n", port);
		return -1;
	}

	usb_device_descriptor_t *desc = kmalloc(sizeof(usb_device_descriptor_t));
	if (!desc) {
		kprintf("[ehci] failed to allocate descriptor\n");
		return -1;
	}

	int device_addr = ehci_ctrl.num_devices + 1;

	if (ehci_set_address(device_addr) < 0) {
		kfree(desc);
		return -1;
	}

	if (ehci_get_device_descriptor(device_addr, desc) < 0) {
		kfree(desc);
		return -1;
	}

	usb_device_t *dev = &ehci_ctrl.devices[ehci_ctrl.num_devices];
	dev->bus = ehci_ctrl.bus;
	dev->device_address = device_addr;
	dev->port = port;
	dev->vendor_id = desc->vendor_id;
	dev->device_id = desc->product_id;
	dev->device_class = desc->device_class;
	dev->device_subclass = desc->device_subclass;
	dev->descriptor = desc;
	dev->configured = 0;
	dev->num_endpoints = 0;

	usb_config_descriptor_t *config_desc = kmalloc(255);
	if (config_desc) {
		if (ehci_read_config_descriptor(device_addr, 0, config_desc, 255) == 0) {
			dev->config_descriptor = config_desc;
			kprintf("[ehci] config descriptor: %d interfaces\n", config_desc->num_interfaces);

			if (ehci_set_configuration(device_addr, config_desc->configuration_value) == 0) {
				dev->configured = 1;
				kprintf("[ehci] device %d configured\n", device_addr);
			}
		} else {
			kfree(config_desc);
		}
	}

	int device_idx = ehci_ctrl.num_devices;
	ehci_ctrl.num_devices++;

	kprintf("[ehci] port %d: device %d enumerated (vendor=%04x, product=%04x, class=%02x.%02x)\n",
		port, device_addr, desc->vendor_id, desc->product_id,
		desc->device_class, desc->device_subclass);

	ehci_probe_device_drivers(device_idx);

	return 0;
}

int ehci_control_transfer(int device_addr, usb_setup_t *setup, void *data)
{
	if (!setup || !ehci_ctrl.regs) return -1;

	ehci_qh_t *qh = ehci_alloc_qh();
	if (!qh) return -1;

	ehci_qtd_t *setup_qtd = ehci_alloc_qtd();
	ehci_qtd_t *data_qtd = data ? ehci_alloc_qtd() : NULL;
	ehci_qtd_t *status_qtd = ehci_alloc_qtd();

	if (!setup_qtd || !status_qtd) return -1;

	qh->ep_char = 0;
	qh->ep_char |= (device_addr & 0x7F) << 0;
	qh->ep_char |= 0 << 8;
	qh->ep_char |= 64 << 16;
	qh->ep_char |= (1 << 27);
	qh->ep_char |= EHCI_QH_CTRL_EP;

	qh->ep_cap = 0;

	uint32_t setup_phys = virt_to_phys(setup);
	uint32_t data_phys = data ? virt_to_phys(data) : 0;

	setup_qtd->token = EHCI_QTD_ACTIVE | (8 << 16) | 0 << 24;
	setup_qtd->buf_ptr[0] = setup_phys;
	setup_qtd->next = data_qtd ? virt_to_phys(data_qtd) : virt_to_phys(status_qtd);
	setup_qtd->alt_next = 1;

	if (data_qtd && setup->length > 0) {
		uint32_t token = (setup->request_type & 0x80) ? 0 : (1 << 8);
		data_qtd->token = EHCI_QTD_ACTIVE | (setup->length << 16) | token;
		data_qtd->buf_ptr[0] = data_phys;
		data_qtd->next = virt_to_phys(status_qtd);
		data_qtd->alt_next = 1;
	}

	uint32_t status_token = (setup->request_type & 0x80) ? (1 << 8) : 0;
	status_qtd->token = EHCI_QTD_ACTIVE | (0 << 16) | status_token | EHCI_QTD_TOGGLE;
	status_qtd->buf_ptr[0] = 0;
	status_qtd->next = 1;
	status_qtd->alt_next = 1;

	qh->current_qtd = 0;
	qh->next_qtd = virt_to_phys(setup_qtd) | EHCI_QTD_TOGGLE;
	qh->alt_next_qtd = 1;

	ehci_write(0x08, virt_to_phys(qh) | EHCI_QH_HEAD);

	for (int i = 0; i < 100; i++) {
		if (!(status_qtd->token & EHCI_QTD_ACTIVE)) {
			if (status_qtd->token & 0x7E) {
				kprintf("[ehci] transfer error: 0x%x\n", status_qtd->token);
				return -1;
			}
			return 0;
		}
		ehci_wait_ms(1);
	}

	kprintf("[ehci] control transfer timeout\n");
	return -1;
}

int ehci_read_device_descriptor(int port, usb_device_descriptor_t *desc)
{
	if (port >= (int)ehci_ctrl.num_ports || !desc) return -1;

	if (ehci_get_device_descriptor(0, desc) < 0) {
		kprintf("[ehci] using default descriptor for port %d\n", port);
		desc->length = 18;
		desc->descriptor_type = 1;
		desc->bcd_usb = 0x0200;
		desc->device_class = 0;
		desc->device_subclass = 0;
		desc->device_protocol = 0;
		desc->max_packet_size = 64;
		desc->vendor_id = 0x0000;
		desc->product_id = 0x0000;
		desc->bcd_device = 0x0001;
		desc->manufacturer = 0;
		desc->product = 0;
		desc->serial_number = 0;
		desc->num_configurations = 1;
	}

	return 0;
}

int ehci_read_config_descriptor(int device_addr, int config_idx, usb_config_descriptor_t *desc, int max_len)
{
	if (!desc || max_len < 9) return -1;

	usb_setup_t setup;
	setup.request_type = 0x80;
	setup.request = 0x06;
	setup.value = (2 << 8) | config_idx;
	setup.index = 0;
	setup.length = max_len > 255 ? 255 : max_len;

	kprintf("[ehci] sending GET_DESCRIPTOR(config %d) to device %d\n", config_idx, device_addr);

	int result = ehci_control_transfer(device_addr, &setup, desc);
	if (result < 0) {
		kprintf("[ehci] GET_CONFIG_DESCRIPTOR failed\n");
		return -1;
	}

	return 0;
}

int ehci_set_configuration(int device_addr, int config_value)
{
	usb_setup_t setup;
	setup.request_type = 0x00;
	setup.request = 0x09;
	setup.value = config_value;
	setup.index = 0;
	setup.length = 0;

	kprintf("[ehci] sending SET_CONFIGURATION(%d) to device %d\n", config_value, device_addr);

	int result = ehci_control_transfer(device_addr, &setup, NULL);
	if (result < 0) {
		kprintf("[ehci] SET_CONFIGURATION failed\n");
		return -1;
	}

	ehci_wait_ms(10);
	return 0;
}

static int ehci_init_controller(void)
{
	if (ehci_reset_controller() < 0) {
		kprintf("[ehci] failed to reset controller\n");
		return -1;
	}

	if (ehci_start_controller() < 0) {
		kprintf("[ehci] failed to start controller\n");
		return -1;
	}

	ehci_ctrl.initialized = 1;
	kprintf("[ehci] controller initialized with %d ports\n", ehci_ctrl.num_ports);

	return 0;
}

int ehci_detect_devices(void)
{
	if (!ehci_ctrl.regs) {
		kprintf("[ehci] controller not initialized\n");
		return -1;
	}

	kprintf("[ehci] scanning %d ports for devices\n", ehci_ctrl.num_ports);

	for (uint32_t i = 0; i < ehci_ctrl.num_ports; i++) {
		uint32_t port_reg = 0x44 + (i * 4);
		uint32_t port_status = ehci_read(port_reg);

		if (port_status & EHCI_PORT_CONNECTED) {
			kprintf("[ehci] port %d: connected\n", i);
			if (ehci_enumerate_device(i) == 0) {
				kprintf("[ehci] port %d: device enumerated\n", i);
			}
		}
	}
	return 0;
}

int ehci_pci_init(pci_device_t *pci_dev)
{
	kprintf("[ehci] EHCI device found at %02x:%02x.%x\n",
		pci_dev->bus, pci_dev->device, pci_dev->function);

	ehci_ctrl.bus = pci_dev->bus;
	ehci_ctrl.device = pci_dev->device;
	ehci_ctrl.function = pci_dev->function;

	uint32_t bar0 = pci_dev->bar[0];
	if (bar0 == 0 || bar0 == 0xFFFFFFFF) {
		kprintf("[ehci] BAR0 not configured\n");
		return -1;
	}

	ehci_ctrl.base = (uintptr_t)(bar0 & ~0xF);
	ehci_ctrl.caps = (ehci_capability_regs_t *)ehci_ctrl.base;

	uint8_t cap_len = ehci_ctrl.caps->capability_length;
	ehci_ctrl.regs = (ehci_operational_regs_t *)((uintptr_t)ehci_ctrl.base + cap_len);

	uint32_t hcs_params = ehci_ctrl.caps->hcs_params;
	ehci_ctrl.num_ports = hcs_params & 0x0F;

	if (ehci_ctrl.num_ports == 0 || ehci_ctrl.num_ports > EHCI_MAX_PORTS) {
		ehci_ctrl.num_ports = 1;
	}

	kprintf("[ehci] capability base: %p\n", ehci_ctrl.caps);
	kprintf("[ehci] operational base: %p\n", ehci_ctrl.regs);
	kprintf("[ehci] number of ports: %d\n", ehci_ctrl.num_ports);

	if (ehci_init_controller() < 0) {
		kprintf("[ehci] controller initialization failed\n");
		return -1;
	}

	if (ehci_detect_devices() < 0) {
		kprintf("[ehci] device detection failed\n");
		return -1;
	}

	return 0;
}

static pci_driver_t ehci_driver = {
	.vendor_id = 0xFFFF,
	.device_id = 0xFFFF,
	.class_code = 0x0C,
	.subclass_code = 0x03,
	.prog_if = 0x20,
	.name = "ehci",
	.init = ehci_pci_init,
};

int ehci_init(void)
{
	kprintf("[ehci] registering EHCI PCI driver\n");
	return pci_register_driver(&ehci_driver);
}
