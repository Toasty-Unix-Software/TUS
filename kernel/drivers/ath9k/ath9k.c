#include "drivers/ath9k/ath9k.h"
#include "../../vfs/vfs.h"
#include "../../core/klib.h"
#include "../../mm/kmalloc.h"
#include <string.h>

#define ATH9K_MAX_DEVICES 4
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static struct ath9k_hw ath9k_devices[ATH9K_MAX_DEVICES];
static int ath9k_device_count = 0;

static struct ath9k_device ath9k_device_list[] = {
	{ AR9271_DEVICE_ID, "ath9k_htc/ar9271.fw", 0 },
	{ AR7010_DEVICE_ID, "ath9k_htc/ar7010.fw", 0 },
};

int ath9k_load_firmware(struct ath9k_hw *ah, const char *fw_name)
{
	struct vfs_node *fw_node;
	void *fw_data;
	size_t fw_size;
	char fw_path[256];
	size_t i, j;

	fw_path[0] = '/';
	fw_path[1] = 'l';
	fw_path[2] = 'i';
	fw_path[3] = 'b';
	fw_path[4] = '/';
	fw_path[5] = 'f';
	fw_path[6] = 'i';
	fw_path[7] = 'r';
	fw_path[8] = 'm';
	fw_path[9] = 'w';
	fw_path[10] = 'a';
	fw_path[11] = 'r';
	fw_path[12] = 'e';
	fw_path[13] = '/';

	for (i = 0, j = 14; fw_name[i] && j < 255; i++, j++) {
		fw_path[j] = fw_name[i];
	}
	fw_path[j] = '\0';

	fw_node = vfs_lookup(fw_path);
	if (!fw_node) {
		kprintf("[ath9k] firmware not found: %s\n", fw_path);
		return -1;
	}

	if (fw_node->type != VFS_FILE) {
		kprintf("[ath9k] %s is not a file\n", fw_path);
		return -1;
	}

	fw_size = fw_node->size;
	if (fw_size == 0) {
		kprintf("[ath9k] firmware file is empty: %s\n", fw_path);
		return -1;
	}

	if (!fw_node->data) {
		kprintf("[ath9k] firmware file data is null: %s\n", fw_path);
		return -1;
	}

	fw_data = kmalloc(fw_size);
	if (!fw_data) {
		kprintf("[ath9k] failed to allocate firmware buffer (%zu bytes)\n", fw_size);
		return -1;
	}

	memcpy(fw_data, fw_node->data, fw_size);

	ah->fw_data = fw_data;
	ah->fw_size = fw_size;

	kprintf("[ath9k] loaded firmware: %s (%zu bytes)\n", fw_name, fw_size);
	return 0;
}

int ath9k_init_hw(struct ath9k_hw *ah)
{
	if (!ah->fw_data || ah->fw_size == 0) {
		kprintf("[ath9k] firmware not loaded\n");
		return -1;
	}

	kprintf("[ath9k] initializing device (vendor=%04x device=%04x)\n",
		ah->vendor_id, ah->device_id);
	kprintf("[ath9k] firmware loaded at %p (size=%zu)\n", ah->fw_data, ah->fw_size);

	ah->if_id = -1;

	kprintf("[ath9k] device initialized successfully\n");
	return 0;
}

int ath9k_probe(uint16_t vendor, uint16_t device)
{
	struct ath9k_hw *ah;
	const char *fw_name = NULL;
	int i;

	if (ath9k_device_count >= ATH9K_MAX_DEVICES) {
		kprintf("[ath9k] max devices reached\n");
		return -1;
	}

	if (vendor != ATH9K_VENDOR_ID) {
		return -1;
	}

	for (i = 0; i < (int)ARRAY_SIZE(ath9k_device_list); i++) {
		if (ath9k_device_list[i].device_id == device) {
			fw_name = ath9k_device_list[i].fw_file;
			break;
		}
	}

	if (!fw_name) {
		kprintf("[ath9k] unknown device: %04x:%04x\n", vendor, device);
		return -1;
	}

	ah = &ath9k_devices[ath9k_device_count++];
	ah->vendor_id = vendor;
	ah->device_id = device;
	ah->fw_data = NULL;
	ah->fw_size = 0;

	kprintf("[ath9k] probing device %04x:%04x\n", vendor, device);

	if (ath9k_load_firmware(ah, fw_name) < 0) {
		kprintf("[ath9k] failed to load firmware\n");
		ath9k_device_count--;
		return -1;
	}

	if (ath9k_init_hw(ah) < 0) {
		kprintf("[ath9k] failed to initialize hardware\n");
		kfree(ah->fw_data);
		ath9k_device_count--;
		return -1;
	}

	kprintf("[ath9k] device ready (id=%d)\n", ath9k_device_count - 1);
	return 0;
}

void ath9k_remove(struct ath9k_hw *ah)
{
	if (!ah) return;

	if (ah->fw_data) {
		kfree(ah->fw_data);
		ah->fw_data = NULL;
	}

	kprintf("[ath9k] device removed\n");
}

int ath9k_usb_probe(void *dev_ptr)
{
	if (!dev_ptr) return -1;

	typedef struct {
		uint8_t bus;
		uint8_t device_address;
		uint8_t port;
		uint8_t configured;
		uint16_t vendor_id;
		uint16_t device_id;
		uint8_t device_class;
		uint8_t device_subclass;
	} usb_device_info_t;

	usb_device_info_t *usb_dev = (usb_device_info_t *)dev_ptr;

	if (usb_dev->vendor_id != ATH9K_VENDOR_ID) {
		return -1;
	}

	int result = ath9k_probe(usb_dev->vendor_id, usb_dev->device_id);
	if (result == 0) {
		kprintf("[ath9k] USB device %d attached (vendor=%04x, device=%04x)\n",
			usb_dev->device_address, usb_dev->vendor_id, usb_dev->device_id);
	}

	return result;
}

void ath9k_init(void)
{
	kprintf("[ath9k] driver loaded\n");
}
