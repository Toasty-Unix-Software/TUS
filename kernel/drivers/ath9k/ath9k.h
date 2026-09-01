#ifndef __ATH9K_H__
#define __ATH9K_H__

#include <stdint.h>
#include <stddef.h>

#define ATH9K_VENDOR_ID        0x0cf3
#define AR7010_DEVICE_ID       0x9018
#define AR9271_DEVICE_ID       0x9018

struct ath9k_device {
	uint16_t device_id;
	const char *fw_file;
	size_t fw_size;
};

struct ath9k_hw {
	uint16_t vendor_id;
	uint16_t device_id;
	void *fw_data;
	size_t fw_size;
	int if_id;
};

int ath9k_probe(uint16_t vendor, uint16_t device);
void ath9k_remove(struct ath9k_hw *ah);
int ath9k_load_firmware(struct ath9k_hw *ah, const char *fw_name);
int ath9k_init_hw(struct ath9k_hw *ah);
void ath9k_init(void);
void ath9k_run_tests(void);
int ath9k_usb_probe(void *dev);

#endif
