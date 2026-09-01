/*
 * hda.c - Intel High Definition Audio controller driver
 *
 * The PDF the user placed at sources/pdfs/intelhdaudio.pdf turned out
 * to be the Intel 600-series PCH's pin/signal datasheet (Volume 1 of
 * 2) - it lists the HDA_* pins and their electrical characteristics,
 * not the controller's MMIO register set, the CORB/RIRB command ring
 * protocol or the codec verb encoding. That level of detail lives in
 * the separate Intel/Microsoft "High Definition Audio Specification"
 * (and, for the register list specifically, the same table Linux's
 * sound/pci/hda/hda_controller.h and every OSDev-wiki HDA writeup
 * reproduce) - it is a long-stable, publicly documented, effectively
 * unchanged-since-2004 register/verb ABI, not something the vendor
 * datasheet in this repo happened to carry. The offsets and verb IDs
 * below are that standard ABI, not anything specific to the 600-series
 * PCH document.
 *
 * WHAT THIS DRIVER DOES
 *
 * Brings up exactly one playback path: reset the controller, stand up
 * the CORB/RIRB command ring, find the first codec that responds,
 * walk its node graph (root -> Audio Function Group -> widgets) to
 * find one DAC and one output-capable Pin Complex, wire the DAC to
 * the pin (following the pin's real connection list - through one
 * mixer/selector hop if there is one - not just assuming index 0),
 * power both up, unmute, and program one output stream descriptor to
 * play from a ring buffer in DMA memory. /dev/dsp's write() copies
 * into that ring; a userspace player just needs to know one format.
 *
 * WHY THE FORMAT IS FIXED (16-bit, 48000 Hz, stereo)
 *
 * A stream descriptor is programmed with ONE format at a time
 * (SDnFMT) and this driver only ever programs it once, at codec
 * bring-up - there is no per-write format renegotiation, and no
 * resampling anywhere in this path. 16/48000/stereo is the format
 * every HD Audio codec is required to support (it is the mandatory
 * baseline the spec itself calls out), so it is the one format this
 * driver can assume works everywhere rather than probing capabilities
 * and hoping. wavplay.c is responsible for getting a WAV file's
 * samples into that shape (upsampling mono to stereo, upconverting
 * 8-bit to 16-bit) or refusing cleanly if it can't (a sample rate
 * other than 48000 - see wavplay.c's own comment).
 *
 * WHY THIS IS A POLLED DRIVER, NOT AN INTERRUPT-DRIVEN ONE
 *
 * No MSI, no legacy INTx handler, no stream completion interrupt.
 * /dev/dsp's write() and the DRAIN ioctl both compare the software
 * write pointer against the hardware's live position (SDnLPIB) in a
 * hlt()-and-recheck loop, the same shape as unix_sock_write()'s
 * "peer buffer full: wait for it to drain" and kernel/drivers/ec/ec.c's
 * bounded busy-poll - the 100 Hz PIT tick that already fires
 * regardless is what wakes the wait, exactly like every other hlt()
 * loop in this kernel. A real interrupt-driven path (and multiple
 * simultaneous streams, capture, per-codec amp-capability discovery
 * instead of a fixed gain) is future work - see the memory file this
 * session writes for the honest list of what is NOT done.
 */

#include "drivers/hda/hda.h"

#include "drivers/pit/pit.h"
#include "../../core/errno.h"
#include "../../core/klib.h"
#include "../../mm/dma.h"
#include "../../mm/vmm.h"
#include "../../vfs/vfs.h"
#include "../../arch/x86_64/io.h"

/* ---- controller registers (offsets from BAR0) ----
 * This table is the standard Intel HD Audio controller register set -
 * see hda.c's top comment for why it did not come from the PDF in
 * this repo. */
#define HDA_GCAP       0x00 /* u16 */
#define HDA_GCTL       0x08 /* u32 */
#define HDA_STATESTS   0x0E /* u16 */
#define HDA_INTCTL     0x18 /* u32 */
#define HDA_INTSTS     0x1C /* u32 */
#define HDA_CORBLBASE  0x40 /* u32 */
#define HDA_CORBUBASE  0x44 /* u32 */
#define HDA_CORBWP     0x48 /* u16 */
#define HDA_CORBRP     0x4A /* u16 */
#define HDA_CORBCTL    0x4C /* u8  */
#define HDA_CORBSTS    0x4D /* u8  */
#define HDA_CORBSIZE   0x4E /* u8  */
#define HDA_RIRBLBASE  0x50 /* u32 */
#define HDA_RIRBUBASE  0x54 /* u32 */
#define HDA_RIRBWP     0x58 /* u16 */
#define HDA_RINTCNT    0x5A /* u16 */
#define HDA_RIRBCTL    0x5C /* u8  */
#define HDA_RIRBSTS    0x5D /* u8  */
#define HDA_RIRBSIZE   0x5E /* u8  */
#define HDA_SD_BASE    0x80
#define HDA_SD_STRIDE  0x20

/* per-stream-descriptor offsets, relative to HDA_SD_BASE + n*HDA_SD_STRIDE */
#define SD_CTL     0x00 /* u8 low byte: SRST=1,RUN=2,IOCE=4,FEIE=8,DEIE=0x10 */
#define SD_STS     0x03 /* u8: BCIS=4,FIFOE=8,DESE=0x10 */
#define SD_LPIB    0x04 /* u32, read-only link position in buffer */
#define SD_CBL     0x08 /* u32, cyclic buffer length in bytes */
#define SD_LVI     0x0C /* u16, last valid BDL index */
#define SD_FORMAT  0x12 /* u16 */
#define SD_BDLPL   0x18 /* u32 */
#define SD_BDLPU   0x1C /* u32 */

#define GCTL_CRST  (1u << 0)

#define CORBCTL_RUN 0x02
#define RIRBCTL_RUN 0x02

#define SDCTL_SRST 0x01
#define SDCTL_RUN  0x02

/* 16-bit, 48 kHz, stereo. See the top-of-file comment for why this is
 * the only format this driver ever programs: base rate bit 0 (48k),
 * multiplier/divisor both x1, bits[6:4]=001 (16-bit), bits[3:0]=0001
 * (2 channels - 1). */
#define HDA_FORMAT_16BIT_48K_STEREO 0x0011

/* ---- codec verbs (the standard 20-bit encoding: 4-bit codec address,
 * 7-bit node id, then either a 12-bit verb + 8-bit payload, or - for
 * the handful of verbs the spec singles out - a 4-bit verb + 16-bit
 * payload). Both forms exist below; VERB_LONG/VERB_SHORT build the
 * dword each expects. */
#define VERB_LONG(codec, nid, verb12, pay8) \
    (((uint32_t)(codec) << 28) | ((uint32_t)(nid) << 20) | \
     (((uint32_t)(verb12) & 0xFFF) << 8) | ((uint32_t)(pay8) & 0xFF))
#define VERB_SHORT(codec, nid, verb4, pay16) \
    (((uint32_t)(codec) << 28) | ((uint32_t)(nid) << 20) | \
     (((uint32_t)(verb4) & 0xF) << 16) | ((uint32_t)(pay16) & 0xFFFF))

#define GET_PARAM              0xF00
#define PARAM_VENDOR_ID        0x00
#define PARAM_NODE_COUNT       0x04
#define PARAM_FUNCTION_TYPE    0x05
#define PARAM_AUDIO_WIDGET_CAP 0x09
#define PARAM_PIN_CAP          0x0C
#define PARAM_CONN_LIST_LEN    0x0E

#define SET_CONNECT_SEL        0x701
#define SET_POWER_STATE        0x705
#define SET_CHANNEL_STREAMID   0x706
#define SET_PIN_WIDGET_CONTROL 0x707
#define GET_CONNECT_LIST       0xF02

#define WIDGET_TYPE(cap)  (((cap) >> 20) & 0xF)
#define WIDGET_OUTPUT     0x0
#define WIDGET_MIXER      0x2
#define WIDGET_SELECTOR   0x3
#define WIDGET_PIN        0x4

#define PINCAP_OUT (1u << 4)

#define PINCTL_OUT_EN 0x40
#define PINCTL_HP_EN  0x80

/* ---- state ---- */

static volatile uint8_t *g_regs;
static uint8_t g_codec_addr;
static uint8_t g_dac_nid;
static uint8_t g_out_stream_idx; /* index into the stream descriptor array */

static struct dma_buf g_corb;
static struct dma_buf g_rirb;
static uint32_t g_corb_entries;
static uint32_t g_rirb_entries;
static uint16_t g_corb_wp;

#define HDA_RING_BYTES  (128u * 1024u)
#define HDA_BDL_ENTRIES 8
#define HDA_BDL_CHUNK   (HDA_RING_BYTES / HDA_BDL_ENTRIES)

static struct dma_buf g_ring;   /* the actual PCM samples the codec DMAs from */
static struct dma_buf g_bdl;    /* its buffer descriptor list */

/* The ring is tracked as two MONOTONIC (never-wrapping) byte counters,
 * not two positions mod HDA_RING_BYTES - see stream_bytes_consumed()'s
 * comment for why a plain "(write_ptr - read_pos) % RING_BYTES"
 * distance is actually ambiguous here. g_write_total is bytes this
 * driver has ever queued; g_read_total is bytes the codec has ever
 * actually consumed, reconstructed from SDnLPIB (a raw hardware
 * position, 0..HDA_RING_BYTES-1, that free-runs and wraps forever
 * once the stream is started - it does not stop or wait at whatever
 * this driver last wrote). The ring offset for a memcpy destination
 * is always `g_write_total % HDA_RING_BYTES`. */
static uint64_t g_write_total;
static uint64_t g_read_total;
static uint32_t g_last_lpib;
static bool g_running;

static bool g_present; /* a controller and a codec were actually found */

/* ---- MMIO helpers ---- */

static inline uint32_t rd32(uint32_t off) { return *(volatile uint32_t *)(g_regs + off); }
static inline uint16_t rd16(uint32_t off) { return *(volatile uint16_t *)(g_regs + off); }
static inline uint8_t  rd8(uint32_t off)  { return *(volatile uint8_t  *)(g_regs + off); }
static inline void wr32(uint32_t off, uint32_t v) { *(volatile uint32_t *)(g_regs + off) = v; }
static inline void wr16(uint32_t off, uint16_t v) { *(volatile uint16_t *)(g_regs + off) = v; }
static inline void wr8(uint32_t off, uint8_t v)   { *(volatile uint8_t  *)(g_regs + off) = v; }

static uint32_t sd_off(uint32_t idx) { return HDA_SD_BASE + idx * HDA_SD_STRIDE; }

/* A bounded busy-poll, the same shape as kernel/drivers/ec/ec.c's
 * EC_POLL_TRIES: this all runs before sti() (during PCI enumeration),
 * so there is no timer tick to hlt() on yet - and real hardware
 * either answers in microseconds or is broken, so a spin bound is the
 * honest way to fail rather than hang the boot. */
static bool poll_until(uint32_t off, uint32_t mask, uint32_t want, int tries) {
    while (tries-- > 0) {
        if ((rd32(off) & mask) == want) {
            return true;
        }
    }
    return false;
}

/* ---- CORB/RIRB: send one verb, wait for its response ---- */

static uint32_t corb_send(uint32_t dword) {
    uint16_t next = (uint16_t)((g_corb_wp + 1) % g_corb_entries);
    ((uint32_t *)g_corb.virt)[next] = dword;

    uint16_t rirb_wp_before = rd16(HDA_RIRBWP);
    g_corb_wp = next;
    wr16(HDA_CORBWP, g_corb_wp);

    /* The response is whatever RIRB entry the write pointer lands on
     * once it moves; an unsolicited response from a DIFFERENT verb
     * racing this one is a real possibility on hardware with more
     * than one codec, but this driver only ever talks to one, so the
     * next RIRBWP advance is always this verb's answer. */
    int tries = 3000000;
    while (tries-- > 0) {
        uint16_t wp = rd16(HDA_RIRBWP);
        if (wp != rirb_wp_before) {
            uint32_t idx = wp % g_rirb_entries;
            uint32_t resp = ((uint32_t *)g_rirb.virt)[idx * 2];
            /* Standard write-1-to-clear ack of the response-interrupt
             * flag. Real hardware does not require this to keep
             * delivering responses (RIRBWP advances independently of
             * whether software has acknowledged anything) - but the
             * RINTCNT sizing above is what this emulated controller
             * actually needs; this ack is just normal practice on top
             * of that, harmless whether or not the flag is even set. */
            wr8(HDA_RIRBSTS, rd8(HDA_RIRBSTS));
            return resp;
        }
    }
    kprintf("[hda] verb 0x%08x timed out; GCTL=%08x CORBWP=%04x CORBRP=%04x "
            "RIRBWP=%04x STATESTS=%04x\n",
            dword, rd32(HDA_GCTL), rd16(HDA_CORBWP), rd16(HDA_CORBRP),
            rd16(HDA_RIRBWP), rd16(HDA_STATESTS));
    return 0xFFFFFFFFu;
}

static uint32_t get_param(uint8_t nid, uint8_t param) {
    return corb_send(VERB_LONG(g_codec_addr, nid, GET_PARAM, param));
}

/* ---- node graph walk ---- */

/* Reads the connection list of `nid` into `out` (up to `cap` entries,
 * short form: four 8-bit NIDs packed per response dword - true for
 * every codec this driver has any hope of talking to, since a codec
 * with more than 255 wide-form node ids is not a thing QEMU or any
 * real machine implements). Returns the number of entries read. */
static uint32_t read_connection_list(uint8_t nid, uint8_t *out, uint32_t cap) {
    uint32_t len = get_param(nid, PARAM_CONN_LIST_LEN) & 0x7F;
    if (len > cap) {
        len = cap;
    }
    for (uint32_t i = 0; i < len; i += 4) {
        uint32_t resp = corb_send(VERB_LONG(g_codec_addr, nid, GET_CONNECT_LIST, i));
        for (uint32_t j = 0; j < 4 && i + j < len; j++) {
            out[i + j] = (uint8_t)((resp >> (j * 8)) & 0xFF);
        }
    }
    return len;
}

/* Point `pin_nid`'s input at `dac_nid`, through one mixer/selector hop
 * if the pin does not connect to the DAC directly - the common shape
 * for a codec that is not the absolute simplest possible one. Returns
 * true if a real path was found and selected; false means the caller
 * fell back to "select index 0 and hope", which is logged so a real
 * boot log says which one actually happened. */
static bool wire_dac_to_pin(uint8_t pin_nid, uint8_t dac_nid,
                            uint8_t widget_types[], uint8_t nid_start, uint32_t nid_count) {
    uint8_t conn[32];
    uint32_t n = read_connection_list(pin_nid, conn, 32);

    for (uint32_t i = 0; i < n; i++) {
        if (conn[i] == dac_nid) {
            corb_send(VERB_LONG(g_codec_addr, pin_nid, SET_CONNECT_SEL, i));
            return true;
        }
    }

    for (uint32_t i = 0; i < n; i++) {
        uint8_t mid = conn[i];
        if (mid < nid_start || mid >= nid_start + nid_count) {
            continue;
        }
        uint8_t t = widget_types[mid - nid_start];
        if (t != WIDGET_MIXER && t != WIDGET_SELECTOR) {
            continue;
        }
        uint8_t inner[32];
        uint32_t m = read_connection_list(mid, inner, 32);
        for (uint32_t k = 0; k < m; k++) {
            if (inner[k] == dac_nid) {
                corb_send(VERB_LONG(g_codec_addr, mid, SET_CONNECT_SEL, k));
                corb_send(VERB_LONG(g_codec_addr, pin_nid, SET_CONNECT_SEL, i));
                return true;
            }
        }
    }

    if (n > 0) {
        corb_send(VERB_LONG(g_codec_addr, pin_nid, SET_CONNECT_SEL, 0));
    }
    return false;
}

/* ---- bring-up ---- */

static bool hda_reset_controller(void) {
    wr32(HDA_GCTL, rd32(HDA_GCTL) & ~GCTL_CRST);
    if (!poll_until(HDA_GCTL, GCTL_CRST, 0, 200000)) {
        return false;
    }
    /* Hold the controller in reset for a real dwell, not just a
     * write-then-immediately-read-back of the same bit - an internal
     * reset needs actual elapsed time to propagate to every engine
     * (CORB/RIRB included), and asserting then deasserting CRST back
     * to back gives it none. */
    for (volatile int i = 0; i < 500000; i++) { }
    wr32(HDA_GCTL, rd32(HDA_GCTL) | GCTL_CRST);
    if (!poll_until(HDA_GCTL, GCTL_CRST, GCTL_CRST, 200000)) {
        return false;
    }
    /* The spec's own codec-ready window: STATESTS is not meaningful
     * until a little while after CRST comes back up. There is no
     * timer tick to hlt() on this early, so this is a plain spin. */
    for (volatile int i = 0; i < 2000000; i++) { }
    return true;
}

static bool hda_setup_corb_rirb(void) {
    uint8_t corb_size_cap = (uint8_t)((rd8(HDA_CORBSIZE) >> 4) & 0xF);
    uint32_t corb_n = (corb_size_cap & 4) ? 256 : (corb_size_cap & 2) ? 16 : 2;
    uint8_t rirb_size_cap = (uint8_t)((rd8(HDA_RIRBSIZE) >> 4) & 0xF);
    uint32_t rirb_n = (rirb_size_cap & 4) ? 256 : (rirb_size_cap & 2) ? 16 : 2;

    g_corb = dma_alloc(corb_n * sizeof(uint32_t));
    g_rirb = dma_alloc(rirb_n * 2 * sizeof(uint32_t));
    if (g_corb.virt == NULL || g_rirb.virt == NULL) {
        return false;
    }
    g_corb_entries = corb_n;
    g_rirb_entries = rirb_n;

    /* CORB DMA must be stopped before its base/pointer registers are
     * reprogrammed, and CORBRP's reset is a real handshake (write the
     * reset bit, wait for hardware to echo it back, then clear it and
     * wait again) - unlike RIRBWP, which just self-clears. */
    wr8(HDA_CORBCTL, 0);
    wr8(HDA_RIRBCTL, 0);

    wr32(HDA_CORBLBASE, (uint32_t)(g_corb.phys & 0xFFFFFFFFu));
    wr32(HDA_CORBUBASE, (uint32_t)(g_corb.phys >> 32));
    wr32(HDA_RIRBLBASE, (uint32_t)(g_rirb.phys & 0xFFFFFFFFu));
    wr32(HDA_RIRBUBASE, (uint32_t)(g_rirb.phys >> 32));

    wr8(HDA_CORBSIZE, (uint8_t)((rd8(HDA_CORBSIZE) & 0xFC) |
        (corb_n == 256 ? 2 : corb_n == 16 ? 1 : 0)));
    wr8(HDA_RIRBSIZE, (uint8_t)((rd8(HDA_RIRBSIZE) & 0xFC) |
        (rirb_n == 256 ? 2 : rirb_n == 16 ? 1 : 0)));

    wr16(HDA_CORBRP, 0x8000);
    poll_until(HDA_CORBRP, 0x8000, 0x8000, 100000);
    wr16(HDA_CORBRP, 0x0000);
    poll_until(HDA_CORBRP, 0x8000, 0x0000, 100000);
    wr16(HDA_CORBWP, 0x0000);
    g_corb_wp = 0;

    wr16(HDA_RIRBWP, 0x8000);

    /* RINTCNT ("responses per interrupt batch") turned out not to be
     * merely an interrupt-pacing knob on this controller: with it left
     * at its post-reset value of 0, or written to exactly the ring
     * size (256 - which this register truncates to 0 in an 8-bit
     * field, the same value as leaving it alone), the controller
     * dispatches exactly ZERO commands from CORB, ever - CORBRP simply
     * never leaves 0 no matter how long this driver waits, confirmed
     * by dumping raw CORB/RIRB ring memory during bring-up: the
     * commands this driver wrote were sitting right there in CORB,
     * untouched. Writing exactly 1 unblocked dispatch but only ever
     * delivered exactly one response total, then stopped identically -
     * i.e. this counter behaves as a one-shot "total responses this
     * driver is allowed" rather than a per-batch reload. 255 (the
     * largest value that survives the 8-bit truncation) comfortably
     * covers the handful of verbs a single codec bring-up sends and
     * was verified, empirically, to actually work end to end - see
     * [[tus-hda-audio-driver]] for the full debugging trail. */
    wr16(HDA_RINTCNT, 255);

    wr8(HDA_CORBCTL, CORBCTL_RUN);
    wr8(HDA_RIRBCTL, RIRBCTL_RUN);
    return true;
}

/* Walk the codec's node graph far enough to find one DAC and one
 * output-capable pin, wire them together, power and unmute the path.
 * Returns false if no usable path exists (an input-only codec, or one
 * this walk's simplifying assumptions do not fit). */
static bool hda_bring_up_codec(void) {
    uint32_t root_children = get_param(0, PARAM_NODE_COUNT);
    uint8_t fg_start = (uint8_t)((root_children >> 16) & 0xFF);
    uint8_t fg_count = (uint8_t)(root_children & 0xFF);

    uint8_t afg_nid = 0;
    for (uint8_t nid = fg_start; nid < fg_start + fg_count; nid++) {
        uint32_t type = get_param(nid, PARAM_FUNCTION_TYPE) & 0xFF;
        if (type == 0x01) {
            afg_nid = nid;
            break;
        }
    }
    if (afg_nid == 0) {
        kprintf("[hda] codec %u has no Audio Function Group\n", g_codec_addr);
        return false;
    }
    corb_send(VERB_LONG(g_codec_addr, afg_nid, SET_POWER_STATE, 0));

    uint32_t widget_children = get_param(afg_nid, PARAM_NODE_COUNT);
    uint8_t w_start = (uint8_t)((widget_children >> 16) & 0xFF);
    uint8_t w_count = (uint8_t)(widget_children & 0xFF);
    if (w_count == 0) {
        return false;
    }

    static uint8_t widget_types[256];
    uint8_t dac_nid = 0, pin_nid = 0;
    for (uint8_t nid = w_start; nid < w_start + w_count; nid++) {
        uint32_t cap = get_param(nid, PARAM_AUDIO_WIDGET_CAP);
        uint8_t t = (uint8_t)WIDGET_TYPE(cap);
        widget_types[nid - w_start] = t;
        if (t == WIDGET_OUTPUT && dac_nid == 0) {
            dac_nid = nid;
        }
        if (t == WIDGET_PIN && pin_nid == 0) {
            uint32_t pincap = get_param(nid, PARAM_PIN_CAP);
            if (pincap & PINCAP_OUT) {
                pin_nid = nid;
            }
        }
    }
    if (dac_nid == 0 || pin_nid == 0) {
        kprintf("[hda] codec %u: no DAC/output-pin pair (dac=%u pin=%u)\n",
                g_codec_addr, dac_nid, pin_nid);
        return false;
    }

    bool wired = wire_dac_to_pin(pin_nid, dac_nid, widget_types, w_start, w_count);
    kprintf("[hda] codec %u: DAC nid %u -> pin nid %u (%s)\n",
            g_codec_addr, dac_nid, pin_nid,
            wired ? "path confirmed" : "fallback: selected index 0, unconfirmed");

    corb_send(VERB_LONG(g_codec_addr, pin_nid, SET_POWER_STATE, 0));
    corb_send(VERB_LONG(g_codec_addr, dac_nid, SET_POWER_STATE, 0));
    corb_send(VERB_LONG(g_codec_addr, pin_nid, SET_PIN_WIDGET_CONTROL,
                        PINCTL_OUT_EN | PINCTL_HP_EN));

    /* Unmute, moderate gain, both channels of the DAC's output amp.
     * bit15=set output amp, bit13/12=set left/right, bit7=0 (unmuted),
     * bits6:0=gain - 0x4F is comfortably inside every codec's range
     * without querying its actual amp capability steps, which this
     * driver does not do (see the top-of-file "not done" list). */
    corb_send(VERB_SHORT(g_codec_addr, dac_nid, 0x3, 0xB04F));
    corb_send(VERB_SHORT(g_codec_addr, pin_nid, 0x3, 0xB04F));

    g_dac_nid = dac_nid;
    return true;
}

static bool hda_setup_stream(void) {
    uint32_t gcap = rd16(HDA_GCAP);
    uint32_t iss = (gcap >> 8) & 0xF;
    g_out_stream_idx = iss; /* first output stream follows all input streams */

    g_ring = dma_alloc(HDA_RING_BYTES);
    g_bdl = dma_alloc(HDA_BDL_ENTRIES * 16);
    if (g_ring.virt == NULL || g_bdl.virt == NULL) {
        return false;
    }

    struct bdl_entry { uint64_t addr; uint32_t len; uint32_t flags; };
    struct bdl_entry *bdl = (struct bdl_entry *)g_bdl.virt;
    for (int i = 0; i < HDA_BDL_ENTRIES; i++) {
        bdl[i].addr = g_ring.phys + (uint64_t)i * HDA_BDL_CHUNK;
        bdl[i].len = HDA_BDL_CHUNK;
        bdl[i].flags = 1; /* IOC - unused (this driver polls LPIB, not
                            * interrupts), harmless to leave set */
    }

    uint32_t off = sd_off(g_out_stream_idx);
    wr8(off + SD_CTL, SDCTL_SRST);
    poll_until(off + SD_CTL, SDCTL_SRST, SDCTL_SRST, 100000);
    wr8(off + SD_CTL, 0);
    poll_until(off + SD_CTL, SDCTL_SRST, 0, 100000);

    *(volatile uint32_t *)(g_regs + off + SD_BDLPL) = (uint32_t)(g_bdl.phys & 0xFFFFFFFFu);
    *(volatile uint32_t *)(g_regs + off + SD_BDLPU) = (uint32_t)(g_bdl.phys >> 32);
    *(volatile uint32_t *)(g_regs + off + SD_CBL) = HDA_RING_BYTES;
    *(volatile uint16_t *)(g_regs + off + SD_LVI) = HDA_BDL_ENTRIES - 1;
    *(volatile uint16_t *)(g_regs + off + SD_FORMAT) = HDA_FORMAT_16BIT_48K_STEREO;

    /* Stream tag 1, channel 0 - one stream, nothing to disambiguate. */
    *(volatile uint8_t *)(g_regs + off + 2) =
        (uint8_t)((*(volatile uint8_t *)(g_regs + off + 2) & 0x0F) | (1 << 4));
    corb_send(VERB_LONG(g_codec_addr, g_dac_nid, SET_CHANNEL_STREAMID, (1 << 4) | 0));
    corb_send(VERB_SHORT(g_codec_addr, g_dac_nid, 0x2, HDA_FORMAT_16BIT_48K_STEREO));

    g_write_total = 0;
    g_read_total = 0;
    g_last_lpib = 0;
    g_running = false;
    return true;
}

/* ---- /dev/dsp ---- */

/* Folds SDnLPIB - a raw position that free-runs and wraps at
 * HDA_RING_BYTES for as long as the stream is RUN, whether or not
 * this driver has written anything new - into g_read_total, a
 * monotonic count of bytes the codec has actually consumed.
 *
 * The trick only works because this is called often (every write()
 * spin and every DRAIN spin, both hlt()-paced at the 100 Hz tick):
 * two consecutive LPIB samples are never more than a couple thousand
 * bytes apart in practice (192000 bytes/sec / 100 Hz ~= 1920 bytes
 * per tick), nowhere near HDA_RING_BYTES/2 - so treating the forward
 * distance since the last sample as `(lpib - g_last_lpib) %
 * HDA_RING_BYTES` is unambiguous. Sampled rarely (or with a huge gap)
 * this would misread a real wrap as a tiny backward step; it is never
 * called that way here. */
static void sample_read_total(void) {
    uint32_t lpib = *(volatile uint32_t *)(g_regs + sd_off(g_out_stream_idx) + SD_LPIB)
                    % HDA_RING_BYTES;
    g_read_total += (lpib - g_last_lpib) % HDA_RING_BYTES;
    g_last_lpib = lpib;
}

/* Bytes still sitting in the ring, not yet consumed. Clamped at 0: the
 * codec's DMA engine does not pause at "nothing new to play" the way
 * a well-behaved consumer would - once RUN, it walks the ring forever
 * at the sample rate regardless of whether this driver ever writes
 * another byte, so on any underrun (or simply once playback has
 * genuinely caught up to the last write()) g_read_total can reach or
 * even nose past g_write_total. Treating that as "positive bytes
 * still queued" (which a naive `(write_ptr - read_pos) %
 * HDA_RING_BYTES` position-only distance does - see git history for
 * the real bug this was) reads an EMPTY ring as a COMPLETELY FULL
 * one, and every future write() blocks forever waiting for space that
 * was never actually missing. */
static uint32_t stream_bytes_queued(void) {
    if (!g_running) {
        return 0;
    }
    sample_read_total();
    if (g_read_total >= g_write_total) {
        return 0;
    }
    uint64_t queued = g_write_total - g_read_total;
    return queued > HDA_RING_BYTES ? HDA_RING_BYTES : (uint32_t)queued;
}

static long hda_write(void *priv, const void *buf, size_t count, size_t pos) {
    (void)priv;
    (void)pos; /* /dev/dsp is a stream, like /dev/null - not seekable */
    if (!g_present) {
        return -ENODEV;
    }

    const uint8_t *src = (const uint8_t *)buf;
    size_t total = 0;
    while (total < count) {
        uint32_t queued = stream_bytes_queued();
        uint32_t free = HDA_RING_BYTES - queued - 1; /* leave 1 byte so full != empty */
        if (free == 0) {
            hlt(); /* the 100 Hz tick wakes us to recheck LPIB */
            continue;
        }
        uint32_t n = (uint32_t)(count - total);
        if (n > free) {
            n = free;
        }
        uint32_t write_off = (uint32_t)(g_write_total % HDA_RING_BYTES);
        uint32_t first = HDA_RING_BYTES - write_off;
        if (n > first) {
            memcpy((uint8_t *)g_ring.virt + write_off, src + total, first);
            memcpy((uint8_t *)g_ring.virt, src + total + first, n - first);
        } else {
            memcpy((uint8_t *)g_ring.virt + write_off, src + total, n);
        }
        g_write_total += n;
        total += n;

        if (!g_running) {
            uint32_t off = sd_off(g_out_stream_idx);
            wr8(off + SD_CTL, (uint8_t)(rd8(off + SD_CTL) | SDCTL_RUN));
            g_running = true;
        }
    }
    return (long)total;
}

static int hda_ioctl(void *priv, uint64_t request, void *arg) {
    (void)priv;
    (void)arg;
    if (!g_present) {
        return -ENODEV;
    }
    switch (request) {
    case TUS_AUDIO_DRAIN:
        while (stream_bytes_queued() > 0) {
            hlt();
        }
        return 0;
    case TUS_AUDIO_STOP: {
        uint32_t off = sd_off(g_out_stream_idx);
        wr8(off + SD_CTL, (uint8_t)(rd8(off + SD_CTL) & ~SDCTL_RUN));
        g_running = false;
        g_write_total = 0;
        g_read_total = 0;
        g_last_lpib = 0;
        memset(g_ring.virt, 0, g_ring.size);
        return 0;
    }
    default:
        return -ENOTTY;
    }
}

static const struct file_ops hda_ops = {
    .read = NULL,
    .write = hda_write,
    .ioctl = hda_ioctl,
    .poll = NULL,
};

/* ---- PCI glue ---- */

static int hda_pci_init(pci_device_t *dev) {
    if (dev->bar[0] & 0x1) {
        return -1; /* an I/O BAR is not an HDA register block */
    }
    uint64_t base = dev->bar[0] & 0xFFFFFFF0u;
    if (((dev->bar[0] >> 1) & 0x3) == 0x2) {
        base |= (uint64_t)dev->bar[1] << 32;
    }
    if (base == 0) {
        return -1;
    }

    uint32_t cmd = pci_config_read(dev->bus, dev->device, dev->function, PCI_COMMAND);
    pci_config_write(dev->bus, dev->device, dev->function, PCI_COMMAND,
                     cmd | PCI_COMMAND_MEM_SPACE | PCI_COMMAND_MASTER);

    g_regs = (volatile uint8_t *)vmm_map_mmio(base, 0x4000);
    kprintf("[hda] controller at %u:%u.%u, registers 0x%lx\n",
            dev->bus, dev->device, (unsigned)dev->function, (unsigned long)base);

    if (!hda_reset_controller()) {
        kprintf("[hda] controller reset timed out\n");
        return -1;
    }
    if (!hda_setup_corb_rirb()) {
        kprintf("[hda] out of memory bringing up CORB/RIRB\n");
        return -1;
    }

    uint16_t statests = rd16(HDA_STATESTS);
    if (statests == 0) {
        kprintf("[hda] no codec responded (STATESTS=0)\n");
        return -1;
    }
    for (uint8_t i = 0; i < 15; i++) {
        if (statests & (1u << i)) {
            g_codec_addr = i;
            break;
        }
    }

    uint32_t vendor = get_param(0, PARAM_VENDOR_ID);
    kprintf("[hda] codec %u vendor:device %04x:%04x\n",
            g_codec_addr, (unsigned)(vendor >> 16), (unsigned)(vendor & 0xFFFF));

    if (!hda_bring_up_codec()) {
        return -1;
    }
    if (!hda_setup_stream()) {
        kprintf("[hda] out of memory bringing up the playback stream\n");
        return -1;
    }

    g_present = true;
    vfs_create_device("/dev/dsp", &hda_ops, NULL);
    kprintf("[hda] /dev/dsp ready: 16-bit 48000 Hz stereo\n");
    return 0;
}

static pci_driver_t hda_driver = {
    .vendor_id = 0xFFFF,
    .device_id = 0xFFFF,
    .class_code = 0x04,
    .subclass_code = 0x03,
    .prog_if = 0x00,
    .name = "hda",
    .init = hda_pci_init,
};

void hda_register(void) {
    pci_register_driver(&hda_driver);
}
