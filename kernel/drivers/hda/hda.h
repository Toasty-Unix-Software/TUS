/*
 * hda.h - Intel High Definition Audio controller driver
 *
 * Brings up one PCM output path (controller -> one codec -> one DAC
 * -> one output pin) and exposes it to userspace as /dev/dsp: write()
 * raw interleaved 16-bit little-endian PCM, 48000 Hz, stereo. That
 * one fixed format is deliberate - see hda.c's top comment.
 */

#ifndef TUS_DRIVERS_HDA_H
#define TUS_DRIVERS_HDA_H

#include <stdbool.h>

#include <tusaudio.h>

#include "drivers/pci/pci.h"

/* Registers this driver's PCI class/subclass/prog-if entry, then lets
 * pci_enumerate_devices() find and init it - same pattern as every
 * other class driver in this kernel. Safe to call whether or not a
 * controller actually exists; the /dev/dsp node only appears if
 * hda_pci_init() actually finds one and brings a codec up. */
void hda_register(void);

#endif /* TUS_DRIVERS_HDA_H */
