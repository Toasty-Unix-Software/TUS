#ifndef __EHCI_H__
#define __EHCI_H__

#include <stdint.h>
#include <stddef.h>
#include "drivers/pci/pci.h"

#define EHCI_CLASS 0x0C
#define EHCI_SUBCLASS 0x03
#define EHCI_PROG_IF 0x20

#define EHCI_CMD_RUN           0x00000001
#define EHCI_CMD_RESET         0x00000002
#define EHCI_CMD_PERIODIC_EN   0x00000010
#define EHCI_CMD_ASYNC_EN      0x00000020
#define EHCI_CMD_IOCE          0x00000004

#define EHCI_STATUS_HCHALTED   0x00001000
#define EHCI_STATUS_RECL       0x00002000
#define EHCI_STATUS_PSS        0x00004000
#define EHCI_STATUS_ASYNCSS    0x00008000
#define EHCI_STATUS_INT        0x00000001

#define EHCI_PORT_ENABLED      0x00000004
#define EHCI_PORT_CONNECTED    0x00000001
#define EHCI_PORT_RESET        0x00000008

#define EHCI_QH_CTRL_EP        0x00008000
#define EHCI_QH_HEAD           0x00000001

#define EHCI_QTD_ACTIVE        0x00000080
#define EHCI_QTD_TOGGLE        0x80000000

#define EHCI_MAX_PORTS 16
#define EHCI_MAX_DEVICES 128

typedef struct {
	uint32_t capability_length;
	uint16_t hci_version;
	uint32_t hcs_params;
	uint32_t hcc_params;
	uint8_t hcsp_portroute[8];
} ehci_capability_regs_t;

typedef struct {
	uint32_t cmd;
	uint32_t status;
	uint32_t intr_enable;
	uint32_t frindex;
	uint32_t ctrldssegment;
	uint32_t periodic_list_base;
	uint32_t async_list_addr;
	uint32_t reserved[9];
	uint32_t config_flag;
	uint32_t port_status_ctrl[0];
} ehci_operational_regs_t;

typedef struct {
	uint32_t next;
	uint32_t alt_next;
	uint32_t token;
	uint32_t buf_ptr[5];
	uint32_t buf_ptr_ext[5];
	uint32_t reserved[2];
} __attribute__((packed)) ehci_qtd_t;

typedef struct {
	uint32_t next;
	uint32_t ep_char;
	uint32_t ep_cap;
	uint32_t current_qtd;
	uint32_t next_qtd;
	uint32_t alt_next_qtd;
	uint32_t token;
	uint32_t buf_ptr[5];
	uint32_t buf_ptr_ext[5];
	uint32_t reserved[3];
} __attribute__((packed)) ehci_qh_t;

typedef struct {
	uint8_t request_type;
	uint8_t request;
	uint16_t value;
	uint16_t index;
	uint16_t length;
} __attribute__((packed)) usb_setup_t;

typedef struct {
	uint8_t length;
	uint8_t descriptor_type;
	uint16_t bcd_usb;
	uint8_t device_class;
	uint8_t device_subclass;
	uint8_t device_protocol;
	uint8_t max_packet_size;
	uint16_t vendor_id;
	uint16_t product_id;
	uint16_t bcd_device;
	uint8_t manufacturer;
	uint8_t product;
	uint8_t serial_number;
	uint8_t num_configurations;
} __attribute__((packed)) usb_device_descriptor_t;

typedef struct {
	uint8_t length;
	uint8_t descriptor_type;
	uint16_t total_length;
	uint8_t num_interfaces;
	uint8_t configuration_value;
	uint8_t configuration;
	uint8_t attributes;
	uint8_t max_power;
} __attribute__((packed)) usb_config_descriptor_t;

typedef struct {
	uint8_t length;
	uint8_t descriptor_type;
	uint8_t interface_number;
	uint8_t alternate_setting;
	uint8_t num_endpoints;
	uint8_t interface_class;
	uint8_t interface_subclass;
	uint8_t interface_protocol;
	uint8_t interface;
} __attribute__((packed)) usb_interface_descriptor_t;

typedef struct {
	uint8_t length;
	uint8_t descriptor_type;
	uint8_t endpoint_address;
	uint8_t attributes;
	uint16_t max_packet_size;
	uint8_t interval;
} __attribute__((packed)) usb_endpoint_descriptor_t;

typedef struct {
	uint8_t endpoint_address;
	uint8_t attributes;
	uint16_t max_packet_size;
	uint8_t interval;
	int configured;
} usb_endpoint_info_t;

typedef struct {
	uint8_t bus;
	uint8_t device_address;
	uint8_t port;
	uint8_t configured;
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t device_class;
	uint8_t device_subclass;
	usb_device_descriptor_t *descriptor;
	usb_config_descriptor_t *config_descriptor;
	usb_interface_descriptor_t *interface_descriptor;
	usb_endpoint_info_t endpoints[16];
	int num_endpoints;
} usb_device_t;

typedef struct {
	uint8_t bus;
	uint8_t device;
	uint8_t function;
	uintptr_t base;
	ehci_capability_regs_t *caps;
	ehci_operational_regs_t *regs;
	uint32_t num_ports;
	int initialized;
	usb_device_t devices[EHCI_MAX_DEVICES];
	int num_devices;
	ehci_qh_t *async_qh;
	ehci_qtd_t *setup_qtd;
} ehci_controller_t;

int ehci_init(void);
int ehci_pci_init(pci_device_t *pci_dev);
int ehci_detect_devices(void);
int ehci_enumerate_device(int port);
int ehci_read_device_descriptor(int port, usb_device_descriptor_t *desc);
int ehci_read_config_descriptor(int device_addr, int config_idx, usb_config_descriptor_t *desc, int max_len);
int ehci_set_configuration(int device_addr, int config_value);
int ehci_control_transfer(int device_addr, usb_setup_t *setup, void *data);
int ehci_bulk_transfer(int device_addr, int endpoint, int dir, void *data, int len);
int ehci_interrupt_transfer(int device_addr, int endpoint, void *data, int len, int interval);

typedef int (*usb_driver_probe_t)(usb_device_t *dev);

typedef struct {
	uint8_t class_code;
	uint8_t subclass_code;
	const char *name;
	usb_driver_probe_t probe;
} usb_class_driver_t;

int ehci_register_class_driver(usb_class_driver_t *driver);
int ehci_probe_device_drivers(int device_idx);

#endif
