#include "drivers/ath9k/ath9k.h"
#include "../../core/klib.h"
#include "../../mm/kmalloc.h"
#include <string.h>

struct dummy_usb_device {
	uint16_t vendor_id;
	uint16_t device_id;
	const char *name;
};

static struct dummy_usb_device test_devices[] = {
	{ 0x0cf3, 0x9018, "AR9271" },
	{ 0x0cf3, 0x9018, "AR7010" },
};

static void ath9k_test_probe_device(struct dummy_usb_device *dev)
{
	kprintf("[ath9k_test] Probing %s (%04x:%04x)...\n",
		dev->name, dev->vendor_id, dev->device_id);

	if (ath9k_probe(dev->vendor_id, dev->device_id) == 0) {
		kprintf("[ath9k_test] ✓ %s device probe successful\n", dev->name);
	} else {
		kprintf("[ath9k_test] ✗ %s device probe failed\n", dev->name);
	}
}

static void ath9k_test_firmware_loading(void)
{
	struct ath9k_hw test_hw;
	const char *fw_names[] = {
		"ath9k_htc/ar7010.fw",
		"ath9k_htc/ar9271.fw",
	};
	int i;

	kprintf("[ath9k_test] === Firmware Loading Test ===\n");

	for (i = 0; i < 2; i++) {
		memset(&test_hw, 0, sizeof(test_hw));

		kprintf("[ath9k_test] Loading %s...\n", fw_names[i]);

		if (ath9k_load_firmware(&test_hw, fw_names[i]) == 0) {
			kprintf("[ath9k_test] ✓ Firmware loaded: %s\n", fw_names[i]);
			kprintf("[ath9k_test]   Size: %zu bytes\n", test_hw.fw_size);
			kprintf("[ath9k_test]   Address: %p\n", test_hw.fw_data);

			if (test_hw.fw_size > 0 && test_hw.fw_data != NULL) {
				uint8_t *fw_bytes = (uint8_t *)test_hw.fw_data;
				kprintf("[ath9k_test]   First bytes: %02x %02x %02x %02x\n",
					fw_bytes[0], fw_bytes[1], fw_bytes[2], fw_bytes[3]);
			}

			kfree(test_hw.fw_data);
		} else {
			kprintf("[ath9k_test] ✗ Failed to load %s\n", fw_names[i]);
		}
	}
}

static void ath9k_test_dummy_device(void)
{
	int i;

	kprintf("[ath9k_test] === Dummy USB Device Test ===\n");

	for (i = 0; i < 2; i++) {
		ath9k_test_probe_device(&test_devices[i]);
	}
}

void ath9k_run_tests(void)
{
	kprintf("\n");
	kprintf("╔════════════════════════════════════════════╗\n");
	kprintf("║    ath9k-htc Driver Unit Test Suite        ║\n");
	kprintf("╚════════════════════════════════════════════╝\n");

	ath9k_test_firmware_loading();
	kprintf("\n");
	ath9k_test_dummy_device();

	kprintf("\n[ath9k_test] === Test Suite Complete ===\n\n");
}
