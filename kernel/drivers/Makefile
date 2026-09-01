# Makefile fragment for kernel/drivers/.
# Pulls in every driver's own Makefile (one per subdirectory) and sums
# their source lists into SRCS_DRIVERS. Included by kernel/Makefile.

include kernel/drivers/ahci/Makefile
include kernel/drivers/ata/Makefile
include kernel/drivers/ath9k/Makefile
include kernel/drivers/e1000/Makefile
include kernel/drivers/ec/Makefile
include kernel/drivers/ehci/Makefile
include kernel/drivers/fb/Makefile
include kernel/drivers/hda/Makefile
include kernel/drivers/hpet/Makefile
include kernel/drivers/keyboard/Makefile
include kernel/drivers/keymap/Makefile
include kernel/drivers/mouse/Makefile
include kernel/drivers/nvme/Makefile
include kernel/drivers/pci/Makefile
include kernel/drivers/pit/Makefile
include kernel/drivers/pmtimer/Makefile
include kernel/drivers/ppm/Makefile
include kernel/drivers/rtc/Makefile
include kernel/drivers/rtl8139/Makefile
include kernel/drivers/serial/Makefile
include kernel/drivers/usbhid/Makefile
include kernel/drivers/vbe/Makefile
include kernel/drivers/xhci/Makefile

SRCS_DRIVERS := \
  $(SRCS_DRIVERS_AHCI) \
  $(SRCS_DRIVERS_ATA) \
  $(SRCS_DRIVERS_ATH9K) \
  $(SRCS_DRIVERS_E1000) \
  $(SRCS_DRIVERS_EC) \
  $(SRCS_DRIVERS_EHCI) \
  $(SRCS_DRIVERS_FB) \
  $(SRCS_DRIVERS_HDA) \
  $(SRCS_DRIVERS_HPET) \
  $(SRCS_DRIVERS_KEYBOARD) \
  $(SRCS_DRIVERS_KEYMAP) \
  $(SRCS_DRIVERS_MOUSE) \
  $(SRCS_DRIVERS_NVME) \
  $(SRCS_DRIVERS_PCI) \
  $(SRCS_DRIVERS_PIT) \
  $(SRCS_DRIVERS_PMTIMER) \
  $(SRCS_DRIVERS_PPM) \
  $(SRCS_DRIVERS_RTC) \
  $(SRCS_DRIVERS_RTL8139) \
  $(SRCS_DRIVERS_SERIAL) \
  $(SRCS_DRIVERS_USBHID) \
  $(SRCS_DRIVERS_VBE) \
  $(SRCS_DRIVERS_XHCI)

