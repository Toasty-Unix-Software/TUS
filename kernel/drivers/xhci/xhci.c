/*
 * xhci.c - eXtensible Host Controller Interface driver
 *
 * See xhci.h for why this driver exists next to the EHCI one.
 *
 * THE SHAPE OF THE HARDWARE
 *
 * An xHCI controller is driven entirely through rings of 16-byte
 * descriptors called TRBs, and there are exactly three kinds:
 *
 *   command ring   driver -> controller: "enable a slot", "address
 *                  this device", "configure these endpoints"
 *   event ring     controller -> driver: every completion, every port
 *                  change. The ONLY channel the controller talks on.
 *   transfer ring  one per endpoint: the actual USB traffic
 *
 * Each ring is a circular array whose last entry is a Link TRB
 * pointing back at the start. Which entries are live is decided by a
 * one-bit *cycle state* that flips every time the producer wraps: the
 * consumer reads TRBs until it finds one whose cycle bit does not
 * match its own, and that is the end of the queue. No head and tail
 * pointers, no shared counters - which is what makes a ring safe to
 * share with a device that is reading it at the same time.
 *
 * The driver rings a doorbell to say "I put something on that ring",
 * and reads the event ring to find out what happened.
 *
 * WHAT RUNS WHERE
 *
 * xhci_init() runs at boot and does the register bring-up. Everything
 * after that - port scanning, enumeration, command completion,
 * delivering HID reports - runs in the `xhci` kernel task, one thing
 * at a time. That is the whole concurrency design: there is no lock
 * because there is no second context.
 */

#include "drivers/xhci/xhci.h"

#include "drivers/pci/pci.h"
#include "drivers/pit/pit.h"
#include "../../core/errno.h"
#include "../../core/klib.h"
#include "../../mm/dma.h"
#include "../../mm/vmm.h"
#include "../../sched/sched.h"

/* ---- capability registers (BAR0 + 0) ---- */
#define XHCI_CAPLENGTH   0x00 /* u8  */
#define XHCI_HCIVERSION  0x02 /* u16 */
#define XHCI_HCSPARAMS1  0x04
#define XHCI_HCSPARAMS2  0x08
#define XHCI_HCCPARAMS1  0x10
#define XHCI_DBOFF       0x14
#define XHCI_RTSOFF      0x18

/* ---- operational registers (BAR0 + CAPLENGTH) ---- */
#define XHCI_USBCMD      0x00
#define XHCI_USBSTS      0x04
#define XHCI_PAGESIZE    0x08
#define XHCI_DNCTRL      0x14
#define XHCI_CRCR        0x18 /* 64-bit */
#define XHCI_DCBAAP      0x30 /* 64-bit */
#define XHCI_CONFIG      0x38
#define XHCI_PORTSC(p)   (0x400 + ((p) - 1) * 0x10)

#define USBCMD_RS        (1u << 0)
#define USBCMD_HCRST     (1u << 1)
#define USBCMD_INTE      (1u << 2)

#define USBSTS_HCH       (1u << 0)
#define USBSTS_HSE       (1u << 2)
#define USBSTS_EINT      (1u << 3)
#define USBSTS_PCD       (1u << 4)
#define USBSTS_CNR       (1u << 11)

#define PORTSC_CCS       (1u << 0)  /* current connect status */
#define PORTSC_PED       (1u << 1)  /* port enabled */
#define PORTSC_PR        (1u << 4)  /* port reset */
#define PORTSC_PP        (1u << 9)  /* port power */
#define PORTSC_SPEED(v)  (((v) >> 10) & 0xF)
#define PORTSC_CSC       (1u << 17) /* connect status change */
#define PORTSC_PEC       (1u << 18)
#define PORTSC_WRC       (1u << 19)
#define PORTSC_OCC       (1u << 20)
#define PORTSC_PRC       (1u << 21) /* port reset change */
#define PORTSC_PLC       (1u << 22)
#define PORTSC_CEC       (1u << 23)
/* The change bits are write-1-to-clear, and PED is write-1-to-DISABLE.
 * Every read-modify-write of PORTSC must therefore mask both out, or
 * it silently acknowledges changes it never looked at and turns the
 * port off. This mask is the only safe base for such a write. */
#define PORTSC_RW_MASK   0x0E00C3E0u

/* ---- runtime registers (BAR0 + RTSOFF), interrupter 0 ---- */
#define XHCI_IMAN        0x20
#define XHCI_IMOD        0x24
#define XHCI_ERSTSZ      0x28
#define XHCI_ERSTBA      0x30 /* 64-bit */
#define XHCI_ERDP        0x38 /* 64-bit */
#define ERDP_EHB         (1ull << 3) /* event handler busy, write 1 to clear */

/* ---- TRB types ---- */
#define TRB_NORMAL           1
#define TRB_SETUP_STAGE      2
#define TRB_DATA_STAGE       3
#define TRB_STATUS_STAGE     4
#define TRB_LINK             6
#define TRB_ENABLE_SLOT      9
#define TRB_DISABLE_SLOT    10
#define TRB_ADDRESS_DEVICE  11
#define TRB_CONFIG_ENDPOINT 12
#define TRB_EVALUATE_CONTEXT 13
#define TRB_RESET_ENDPOINT  14
#define TRB_NOOP_COMMAND    23
#define TRB_TRANSFER_EVENT  32
#define TRB_COMMAND_COMPLETE 33
#define TRB_PORT_STATUS     34

#define TRB_TYPE(t)      ((uint32_t)(t) << 10)
#define TRB_TYPE_OF(c)   (((c) >> 10) & 0x3F)
#define TRB_CYCLE        (1u << 0)
#define TRB_TC           (1u << 1)  /* toggle cycle, on a Link TRB */
#define TRB_ENT          (1u << 1)  /* evaluate next TRB */
#define TRB_ISP          (1u << 2)  /* interrupt on short packet */
#define TRB_CHAIN        (1u << 4)
#define TRB_IOC          (1u << 5)  /* interrupt on completion */
#define TRB_IDT          (1u << 6)  /* immediate data */
#define TRB_DIR_IN       (1u << 16) /* on a Data/Status Stage TRB */

#define CC_SUCCESS       1
#define CC_SHORT_PACKET  13

/* ---- USB standard requests ---- */
#define USB_REQ_GET_DESCRIPTOR   6
#define USB_REQ_SET_CONFIGURATION 9
#define USB_DESC_DEVICE          1
#define USB_DESC_CONFIG          2

/* ---- sizes ---- */
#define CMD_RING_TRBS   64
#define EVENT_RING_TRBS 128
#define XFER_RING_TRBS  32
#define MAX_SLOTS_USED  16
#define MAX_INT_EPS     8

struct xhci_trb {
    uint32_t param_lo;
    uint32_t param_hi;
    uint32_t status;
    uint32_t control;
};

/* A ring the driver produces into (command, transfer) or consumes
 * from (event). `cycle` is the producer's cycle state for the first
 * two and the consumer's for the third. */
struct xhci_ring {
    struct dma_buf   buf;
    struct xhci_trb *trbs;
    uint32_t         count;   /* TRBs in the array, link TRB included */
    uint32_t         index;   /* next slot to write (or read) */
    uint8_t          cycle;
};

/* An interrupt IN endpoint being polled for a class driver. */
struct int_endpoint {
    bool     active;
    uint8_t  slot;
    uint8_t  dci;             /* device context index */
    uint16_t size;
    struct xhci_ring ring;
    struct dma_buf   data;    /* one report buffer, reused */
    struct xhci_device *dev;
    struct xhci_class_driver *drv;
};

/* ---- controller state ---- */

static volatile uint8_t *g_cap;   /* capability registers */
static volatile uint8_t *g_op;    /* operational registers */
static volatile uint8_t *g_rt;    /* runtime registers */
static volatile uint8_t *g_db;    /* doorbell array */

static uint32_t g_max_slots;
static uint32_t g_max_ports;
static uint32_t g_ctx_size;       /* 32 or 64 bytes per context */
static bool     g_running;

static struct dma_buf   g_dcbaa;
static struct dma_buf   g_scratchpad_array;
static struct dma_buf   g_scratchpads;
static struct xhci_ring g_cmd;
static struct xhci_ring g_event;
static struct dma_buf   g_erst;

static struct xhci_device g_devices[MAX_SLOTS_USED];
static struct dma_buf     g_dev_ctx[MAX_SLOTS_USED];   /* device context */
static struct dma_buf     g_in_ctx[MAX_SLOTS_USED];    /* input context */
static struct xhci_ring   g_ep0[MAX_SLOTS_USED];       /* EP0 transfer ring */

static struct int_endpoint g_int_eps[MAX_INT_EPS];

static struct xhci_class_driver *g_drivers[8];
static int g_driver_count;

/* One buffer for every control transfer. There is only ever one in
 * flight: all of this runs in a single task. */
static struct dma_buf g_ctrl_buf;

/* ---- register access ----
 *
 * MMIO, so every access must actually happen and must happen in the
 * order written: no caching (the mapping is VMM_NOCACHE) and no
 * reordering by the compiler (volatile). */

static inline uint32_t rd32(volatile uint8_t *base, uint32_t off) {
    return *(volatile uint32_t *)(base + off);
}

static inline void wr32(volatile uint8_t *base, uint32_t off, uint32_t v) {
    *(volatile uint32_t *)(base + off) = v;
}

/* A 64-bit register on a controller that may only decode 32 bits at a
 * time is written low half first: the high half is what commits the
 * value on the ones that care. */
static inline void wr64(volatile uint8_t *base, uint32_t off, uint64_t v) {
    *(volatile uint32_t *)(base + off) = (uint32_t)v;
    *(volatile uint32_t *)(base + off + 4) = (uint32_t)(v >> 32);
}

static inline uint64_t rd64(volatile uint8_t *base, uint32_t off) {
    uint64_t lo = *(volatile uint32_t *)(base + off);
    uint64_t hi = *(volatile uint32_t *)(base + off + 4);
    return lo | (hi << 32);
}

/* ---- rings ---- */

static int ring_init(struct xhci_ring *r, uint32_t count) {
    r->buf = dma_alloc(count * sizeof(struct xhci_trb));
    if (r->buf.virt == NULL) {
        return -ENOMEM;
    }
    r->trbs = (struct xhci_trb *)r->buf.virt;
    r->count = count;
    r->index = 0;
    r->cycle = 1;
    return 0;
}

/* Turn the last TRB into a Link back to the start. The Toggle Cycle
 * bit is what makes the producer's cycle state flip on every lap,
 * which is what tells the controller where the queue ends. Event
 * rings have no link TRB - the controller wraps them itself using the
 * segment table. */
static void ring_close(struct xhci_ring *r) {
    struct xhci_trb *link = &r->trbs[r->count - 1];
    link->param_lo = (uint32_t)r->buf.phys;
    link->param_hi = (uint32_t)(r->buf.phys >> 32);
    link->status = 0;
    link->control = TRB_TYPE(TRB_LINK) | TRB_TC;
}

/* Place one TRB and return the physical address it was written to -
 * which is how a command is later matched to its completion event,
 * since that is the only identifier the event carries. */
static uint64_t ring_push(struct xhci_ring *r, uint32_t p_lo, uint32_t p_hi,
                          uint32_t status, uint32_t control) {
    struct xhci_trb *trb = &r->trbs[r->index];
    uint64_t phys = r->buf.phys + r->index * sizeof(struct xhci_trb);

    trb->param_lo = p_lo;
    trb->param_hi = p_hi;
    trb->status = status;
    /* The cycle bit goes in last: until it matches the controller's
     * cycle state the TRB is not live, so the other three words must
     * already be visible. */
    __asm__ volatile("" ::: "memory");
    trb->control = control | (r->cycle ? TRB_CYCLE : 0);

    r->index++;
    if (r->index == r->count - 1) {
        /* The link TRB is the last entry: hand it the current cycle
         * state, then wrap and flip. */
        struct xhci_trb *link = &r->trbs[r->count - 1];
        link->control = (link->control & ~TRB_CYCLE) |
                        (r->cycle ? TRB_CYCLE : 0);
        r->index = 0;
        r->cycle ^= 1;
    }
    return phys;
}

static void doorbell(uint32_t slot, uint32_t target) {
    wr32(g_db, slot * 4, target);
    (void)rd32(g_db, slot * 4); /* post the write before we wait on it */
}

/* ---- device contexts ----
 *
 * A context is 32 or 64 bytes depending on HCCPARAMS1.CSZ, and the
 * arrays are indexed by "device context index": 0 is the slot
 * context, 1 is endpoint 0, and endpoint N is 2N (OUT) or 2N+1 (IN).
 * The input context has one extra context in front of all of them,
 * the input control context, whose add/drop bitmaps say which of the
 * following contexts the controller should look at. */

static uint32_t *ctx_at(struct dma_buf *buf, int index) {
    return (uint32_t *)((uint8_t *)buf->virt + (size_t)index * g_ctx_size);
}

static uint8_t dci_of(uint8_t endpoint_address) {
    uint8_t num = endpoint_address & 0x0F;
    uint8_t in = (endpoint_address & 0x80) ? 1 : 0;
    return (uint8_t)(num * 2 + in);
}

/* The maximum packet size endpoint 0 must be given before anything is
 * known about the device. USB fixes it per speed, and getting it
 * wrong makes the first descriptor read fail in a way that looks like
 * broken hardware. */
static uint16_t ep0_packet_size(uint8_t speed) {
    switch (speed) {
    case XHCI_SPEED_LOW:   return 8;
    case XHCI_SPEED_FULL:  return 8;  /* corrected after the first read */
    case XHCI_SPEED_HIGH:  return 64;
    case XHCI_SPEED_SUPER: return 512;
    default:               return 8;
    }
}

static const char *speed_name(uint8_t speed) {
    switch (speed) {
    case XHCI_SPEED_LOW:   return "low";
    case XHCI_SPEED_FULL:  return "full";
    case XHCI_SPEED_HIGH:  return "high";
    case XHCI_SPEED_SUPER: return "super";
    default:               return "?";
    }
}

/* ---- the event ring ----
 *
 * Everything the controller has to say arrives here, and there is no
 * way to ask for only part of it. Draining therefore has to handle
 * every event type on every pass, whatever the caller was waiting
 * for: a HID report that arrives while a command is outstanding must
 * be delivered, not dropped, or the keyboard stops working the first
 * time a device is plugged in.
 *
 * `wait_trb` is the physical address of a command TRB to watch for.
 * Zero means "drain whatever is there and return".
 */

static void requeue_int_ep(struct int_endpoint *ep);

static bool event_pending(void) {
    struct xhci_trb *trb = &g_event.trbs[g_event.index];
    return ((trb->control & TRB_CYCLE) ? 1 : 0) == g_event.cycle;
}

static void event_advance(void) {
    g_event.index++;
    if (g_event.index == g_event.count) {
        g_event.index = 0;
        g_event.cycle ^= 1;
    }
}

static void event_update_dequeue(void) {
    uint64_t phys = g_event.buf.phys +
                    (uint64_t)g_event.index * sizeof(struct xhci_trb);
    /* Writing ERDP also clears the Event Handler Busy bit, which is
     * what tells the controller it may raise the next interrupt. */
    wr64(g_rt, XHCI_ERDP, phys | ERDP_EHB);
}

/* Deliver one transfer event to whoever owns the endpoint. */
static void handle_transfer_event(const struct xhci_trb *ev) {
    uint8_t slot = (uint8_t)((ev->control >> 24) & 0xFF);
    uint8_t dci = (uint8_t)((ev->control >> 16) & 0x1F);
    uint32_t cc = (ev->status >> 24) & 0xFF;
    uint32_t residue = ev->status & 0x00FFFFFF;

    for (int i = 0; i < MAX_INT_EPS; i++) {
        struct int_endpoint *ep = &g_int_eps[i];
        if (!ep->active || ep->slot != slot || ep->dci != dci) {
            continue;
        }
        if (cc == CC_SUCCESS || cc == CC_SHORT_PACKET) {
            int len = (int)ep->size - (int)residue;
            if (len > 0 && ep->drv != NULL && ep->drv->report != NULL) {
                ep->drv->report(ep->dev, (const uint8_t *)ep->data.virt, len);
            }
        }
        /* Queue the next report whatever happened: a stalled HID
         * endpoint that is never re-armed is a keyboard that stops
         * after one keystroke, and a re-arm costs one TRB. */
        requeue_int_ep(ep);
        return;
    }
}

static int event_drain(uint64_t wait_trb, struct xhci_trb *out, int timeout_ms) {
    uint64_t deadline = pit_uptime_ms() + (uint64_t)timeout_ms;
    bool found = false;
    bool moved = false;

    for (;;) {
        while (event_pending()) {
            struct xhci_trb ev = g_event.trbs[g_event.index];
            event_advance();
            moved = true;

            uint32_t type = TRB_TYPE_OF(ev.control);
            if (type == TRB_TRANSFER_EVENT) {
                handle_transfer_event(&ev);
            } else if (type == TRB_COMMAND_COMPLETE) {
                uint64_t trb_phys = (uint64_t)ev.param_lo |
                                    ((uint64_t)ev.param_hi << 32);
                if (wait_trb != 0 && trb_phys == wait_trb) {
                    if (out != NULL) {
                        *out = ev;
                    }
                    found = true;
                }
            }
            /* Port status change events need no work here: the poll
             * loop reads PORTSC itself, which is the authority. */
        }

        if (moved) {
            event_update_dequeue();
            moved = false;
        }
        if (found || wait_trb == 0) {
            return found || wait_trb == 0 ? 0 : -ETIMEDOUT;
        }
        if (pit_uptime_ms() >= deadline) {
            return -ETIMEDOUT;
        }
        timer_sleep_ms(1);
    }
}

/* Put one command on the command ring, ring doorbell 0 and wait for
 * its completion event. Returns the completion code, or a negative
 * errno. `out_slot` receives the slot id an Enable Slot command
 * allocated. */
static int command_run(uint32_t p_lo, uint32_t p_hi, uint32_t control,
                       uint8_t *out_slot) {
    uint64_t trb = ring_push(&g_cmd, p_lo, p_hi, 0, control);
    doorbell(0, 0);

    struct xhci_trb ev;
    memset(&ev, 0, sizeof(ev));
    if (event_drain(trb, &ev, 1000) != 0) {
        kprintf("[xhci] command %u timed out\n", TRB_TYPE_OF(control));
        return -ETIMEDOUT;
    }

    uint32_t cc = (ev.status >> 24) & 0xFF;
    if (out_slot != NULL) {
        *out_slot = (uint8_t)((ev.control >> 24) & 0xFF);
    }
    if (cc != CC_SUCCESS) {
        kprintf("[xhci] command %u failed, completion code %u\n",
                TRB_TYPE_OF(control), cc);
        return -EIO;
    }
    return 0;
}

/* ---- control transfers ---- */

int xhci_control(struct xhci_device *dev, uint8_t request_type,
                 uint8_t request, uint16_t value, uint16_t index,
                 void *data, uint16_t length) {
    if (dev == NULL || dev->slot == 0 || dev->slot > MAX_SLOTS_USED) {
        return -EINVAL;
    }
    if (length > g_ctrl_buf.size) {
        return -EINVAL;
    }
    struct xhci_ring *ring = &g_ep0[dev->slot - 1];
    bool in = (request_type & 0x80) != 0;

    if (!in && data != NULL && length > 0) {
        memcpy(g_ctrl_buf.virt, data, length);
    }

    /* Setup stage. The eight setup bytes travel inside the TRB itself
     * (Immediate Data), which is why there is no buffer for them. */
    uint32_t setup_lo = (uint32_t)request_type |
                        ((uint32_t)request << 8) |
                        ((uint32_t)value << 16);
    uint32_t setup_hi = (uint32_t)index | ((uint32_t)length << 16);
    uint32_t trt = 0;
    if (length > 0) {
        trt = in ? 3u : 2u; /* transfer type: IN data / OUT data */
    }
    ring_push(ring, setup_lo, setup_hi, 8,
              TRB_TYPE(TRB_SETUP_STAGE) | TRB_IDT | (trt << 16));

    /* Data stage, when there is one. */
    if (length > 0) {
        ring_push(ring, (uint32_t)g_ctrl_buf.phys,
                  (uint32_t)(g_ctrl_buf.phys >> 32), length,
                  TRB_TYPE(TRB_DATA_STAGE) | (in ? TRB_DIR_IN : 0));
    }

    /* Status stage: the opposite direction to the data, and the one
     * that carries Interrupt On Completion - the transfer is not done
     * until the status handshake is. */
    uint32_t status_ctl = TRB_TYPE(TRB_STATUS_STAGE) | TRB_IOC;
    if (length == 0 || !in) {
        status_ctl |= TRB_DIR_IN;
    }
    ring_push(ring, 0, 0, 0, status_ctl);

    doorbell(dev->slot, 1); /* DCI 1 = endpoint 0 */

    /* Wait for the transfer event. Control transfers are only issued
     * from this task, so draining until one arrives is safe. */
    uint64_t deadline = pit_uptime_ms() + 1000;
    for (;;) {
        bool done = false;
        int transferred = length;

        while (event_pending()) {
            struct xhci_trb ev = g_event.trbs[g_event.index];
            event_advance();
            uint32_t type = TRB_TYPE_OF(ev.control);
            if (type == TRB_TRANSFER_EVENT) {
                uint8_t slot = (uint8_t)((ev.control >> 24) & 0xFF);
                uint8_t dci = (uint8_t)((ev.control >> 16) & 0x1F);
                if (slot == dev->slot && dci == 1) {
                    uint32_t cc = (ev.status >> 24) & 0xFF;
                    if (cc != CC_SUCCESS && cc != CC_SHORT_PACKET) {
                        event_update_dequeue();
                        return -EIO;
                    }
                    transferred = (int)length - (int)(ev.status & 0x00FFFFFF);
                    done = true;
                } else {
                    handle_transfer_event(&ev);
                }
            }
        }
        event_update_dequeue();

        if (done) {
            if (in && data != NULL && transferred > 0) {
                memcpy(data, g_ctrl_buf.virt, (size_t)transferred);
            }
            return transferred < 0 ? 0 : transferred;
        }
        if (pit_uptime_ms() >= deadline) {
            return -ETIMEDOUT;
        }
        timer_sleep_ms(1);
    }
}

/* ---- interrupt IN endpoints ---- */

static void requeue_int_ep(struct int_endpoint *ep) {
    ring_push(&ep->ring, (uint32_t)ep->data.phys,
              (uint32_t)(ep->data.phys >> 32), ep->size,
              TRB_TYPE(TRB_NORMAL) | TRB_IOC | TRB_ISP);
    doorbell(ep->slot, ep->dci);
}

int xhci_start_interrupt_in(struct xhci_device *dev, uint8_t endpoint,
                            uint16_t size, uint8_t interval) {
    if (dev == NULL || dev->slot == 0) {
        return -EINVAL;
    }
    if (size == 0 || size > 64) {
        size = 64;
    }

    struct int_endpoint *ep = NULL;
    for (int i = 0; i < MAX_INT_EPS; i++) {
        if (!g_int_eps[i].active) {
            ep = &g_int_eps[i];
            break;
        }
    }
    if (ep == NULL) {
        return -ENOSPC;
    }

    uint8_t dci = dci_of(endpoint);
    if (ring_init(&ep->ring, XFER_RING_TRBS) != 0) {
        return -ENOMEM;
    }
    ring_close(&ep->ring);

    ep->data = dma_alloc(64);
    if (ep->data.virt == NULL) {
        dma_free(&ep->ring.buf);
        return -ENOMEM;
    }

    /* Tell the controller about the endpoint: an input context whose
     * add bitmap names the slot context (bit 0) and this endpoint,
     * and whose slot context raises the "context entries" count to
     * cover it. Anything the controller is not told to add keeps
     * whatever the device context already holds. */
    struct dma_buf *in = &g_in_ctx[dev->slot - 1];
    memset(in->virt, 0, in->size);

    uint32_t *ctrl = ctx_at(in, 0);
    ctrl[1] = (1u << 0) | (1u << dci);      /* add: slot + this endpoint */

    uint32_t *slot_ctx = ctx_at(in, 1);
    uint32_t *dev_slot = ctx_at(&g_dev_ctx[dev->slot - 1], 0);
    slot_ctx[0] = dev_slot[0];
    slot_ctx[1] = dev_slot[1];
    slot_ctx[2] = dev_slot[2];
    slot_ctx[3] = dev_slot[3];
    /* Context entries live in the top five bits of word 0. */
    slot_ctx[0] = (slot_ctx[0] & 0x07FFFFFFu) | ((uint32_t)dci << 27);

    uint32_t *ep_ctx = ctx_at(in, 1 + dci);
    /* Interval is a log2 of 125 us frames for high/super speed and of
     * 1 ms frames for low/full speed. Clamp to something sane: a HID
     * device asking for 1 ms on a controller that will not schedule
     * it is a device that never reports. */
    uint32_t ival;
    if (dev->speed == XHCI_SPEED_HIGH || dev->speed == XHCI_SPEED_SUPER) {
        ival = interval ? (uint32_t)interval - 1 : 3;
        if (ival > 15) ival = 15;
    } else {
        /* bInterval is in milliseconds here; xHCI wants log2(frames)
         * with 125 us frames, so add the three-bit shift from ms. */
        uint32_t ms = interval ? interval : 8;
        uint32_t log2 = 0;
        while ((1u << (log2 + 1)) <= ms && log2 < 12) {
            log2++;
        }
        ival = log2 + 3;
    }
    ep_ctx[0] = ival << 16;
    /* EP type 7 = Interrupt IN; error count 3; max packet size. */
    ep_ctx[1] = (3u << 1) | (7u << 3) | ((uint32_t)size << 16);
    uint64_t deq = ep->ring.buf.phys | 1u; /* dequeue cycle state = 1 */
    ep_ctx[2] = (uint32_t)deq;
    ep_ctx[3] = (uint32_t)(deq >> 32);
    ep_ctx[4] = size; /* average TRB length */

    int rc = command_run((uint32_t)in->phys, (uint32_t)(in->phys >> 32),
                         TRB_TYPE(TRB_CONFIG_ENDPOINT) |
                         ((uint32_t)dev->slot << 24), NULL);
    if (rc != 0) {
        dma_free(&ep->data);
        dma_free(&ep->ring.buf);
        return rc;
    }

    ep->active = true;
    ep->slot = dev->slot;
    ep->dci = dci;
    ep->size = size;
    ep->dev = dev;
    ep->drv = (struct xhci_class_driver *)dev->driver_data;

    requeue_int_ep(ep);
    return 0;
}

/* ---- enumeration ---- */

static struct xhci_class_driver *match_driver(const struct xhci_device *dev) {
    for (int i = 0; i < g_driver_count; i++) {
        struct xhci_class_driver *d = g_drivers[i];
        if (d->if_class != dev->if_class) {
            continue;
        }
        if (d->if_subclass != 0xFF && d->if_subclass != dev->if_subclass) {
            continue;
        }
        if (d->if_protocol != 0xFF && d->if_protocol != dev->if_protocol) {
            continue;
        }
        return d;
    }
    return NULL;
}

/* Reset a port and wait for the controller to enable it. Returns the
 * negotiated speed, or 0. */
static uint8_t port_reset(uint32_t port) {
    uint32_t sc = rd32(g_op, XHCI_PORTSC(port));
    if ((sc & PORTSC_CCS) == 0) {
        return 0;
    }

    /* USB 3 ports train their own link and come up enabled; only USB
     * 2 ports need a reset driven by the driver. */
    if ((sc & PORTSC_PED) == 0) {
        wr32(g_op, XHCI_PORTSC(port), (sc & PORTSC_RW_MASK) | PORTSC_PR);
        for (int i = 0; i < 200; i++) {
            timer_sleep_ms(1);
            sc = rd32(g_op, XHCI_PORTSC(port));
            if (sc & PORTSC_PRC) {
                break;
            }
        }
        /* Acknowledge every change bit this reset raised. */
        wr32(g_op, XHCI_PORTSC(port),
             (sc & PORTSC_RW_MASK) | PORTSC_PRC | PORTSC_CSC | PORTSC_PEC);
        timer_sleep_ms(20); /* USB requires 10 ms of recovery; be generous */
        sc = rd32(g_op, XHCI_PORTSC(port));
    }

    if ((sc & PORTSC_PED) == 0) {
        return 0;
    }
    return (uint8_t)PORTSC_SPEED(sc);
}

static int enumerate_port(uint32_t port) {
    uint8_t speed = port_reset(port);
    if (speed == 0) {
        return -ENODEV;
    }

    uint8_t slot = 0;
    if (command_run(0, 0, TRB_TYPE(TRB_ENABLE_SLOT), &slot) != 0) {
        return -EIO;
    }
    if (slot == 0 || slot > MAX_SLOTS_USED) {
        kprintf("[xhci] slot %u out of range\n", slot);
        return -ENOSPC;
    }

    struct xhci_device *dev = &g_devices[slot - 1];
    memset(dev, 0, sizeof(*dev));
    dev->slot = slot;
    dev->port = (uint8_t)port;
    dev->speed = speed;

    /* Device context: the controller writes into it, the driver only
     * reads it. Its address goes in the DCBAA slot the controller
     * just handed out. */
    g_dev_ctx[slot - 1] = dma_alloc(32 * g_ctx_size);
    g_in_ctx[slot - 1] = dma_alloc(33 * g_ctx_size);
    if (g_dev_ctx[slot - 1].virt == NULL || g_in_ctx[slot - 1].virt == NULL) {
        return -ENOMEM;
    }
    ((uint64_t *)g_dcbaa.virt)[slot] = g_dev_ctx[slot - 1].phys;

    if (ring_init(&g_ep0[slot - 1], XFER_RING_TRBS) != 0) {
        return -ENOMEM;
    }
    ring_close(&g_ep0[slot - 1]);

    /* Input context for Address Device: add the slot context and
     * endpoint 0, and nothing else. */
    struct dma_buf *in = &g_in_ctx[slot - 1];
    memset(in->virt, 0, in->size);
    uint32_t *ctrl = ctx_at(in, 0);
    ctrl[1] = (1u << 0) | (1u << 1);

    uint32_t *slot_ctx = ctx_at(in, 1);
    /* Route string 0 (a root hub port), context entries 1, speed, and
     * the root hub port number this device is on. */
    slot_ctx[0] = (1u << 27) | ((uint32_t)speed << 20);
    slot_ctx[1] = (uint32_t)port << 16;

    uint32_t *ep0 = ctx_at(in, 2);
    ep0[1] = (3u << 1) | (4u << 3) |            /* error count, control EP */
             ((uint32_t)ep0_packet_size(speed) << 16);
    uint64_t deq = g_ep0[slot - 1].buf.phys | 1u;
    ep0[2] = (uint32_t)deq;
    ep0[3] = (uint32_t)(deq >> 32);
    ep0[4] = 8;

    if (command_run((uint32_t)in->phys, (uint32_t)(in->phys >> 32),
                    TRB_TYPE(TRB_ADDRESS_DEVICE) | ((uint32_t)slot << 24),
                    NULL) != 0) {
        return -EIO;
    }

    /* Read the first eight bytes of the device descriptor. That is
     * all USB promises can be read before the real endpoint 0 packet
     * size is known - and byte 7 is that size. */
    uint8_t desc[18];
    memset(desc, 0, sizeof(desc));
    int rc = xhci_control(dev, 0x80, USB_REQ_GET_DESCRIPTOR,
                          (USB_DESC_DEVICE << 8), 0, desc, 8);
    if (rc < 8) {
        kprintf("[xhci] port %u: device descriptor read failed (%d)\n",
                port, rc);
        return -EIO;
    }

    uint16_t real_mps = desc[7];
    if (dev->speed == XHCI_SPEED_SUPER) {
        real_mps = (uint16_t)(1u << desc[7]); /* SuperSpeed reports log2 */
    }
    if (real_mps != ep0_packet_size(speed) && real_mps != 0) {
        /* Evaluate Context is the "change one thing" command: it
         * updates endpoint 0's packet size without disturbing the
         * address the device has already been given. */
        memset(in->virt, 0, in->size);
        ctrl = ctx_at(in, 0);
        ctrl[1] = (1u << 1); /* endpoint 0 only */
        ep0 = ctx_at(in, 2);
        ep0[1] = (3u << 1) | (4u << 3) | ((uint32_t)real_mps << 16);
        ep0[2] = (uint32_t)deq;
        ep0[3] = (uint32_t)(deq >> 32);
        ep0[4] = 8;
        (void)command_run((uint32_t)in->phys, (uint32_t)(in->phys >> 32),
                          TRB_TYPE(TRB_EVALUATE_CONTEXT) |
                          ((uint32_t)slot << 24), NULL);
    }

    rc = xhci_control(dev, 0x80, USB_REQ_GET_DESCRIPTOR,
                      (USB_DESC_DEVICE << 8), 0, desc, 18);
    if (rc < 18) {
        return -EIO;
    }
    dev->dev_class = desc[4];
    dev->vendor = (uint16_t)(desc[8] | (desc[9] << 8));
    dev->product = (uint16_t)(desc[10] | (desc[11] << 8));

    /* Configuration descriptor: header first for the total length,
     * then the whole thing so the interface and endpoint descriptors
     * that follow it can be walked. */
    uint8_t cfg[256];
    memset(cfg, 0, sizeof(cfg));
    rc = xhci_control(dev, 0x80, USB_REQ_GET_DESCRIPTOR,
                      (USB_DESC_CONFIG << 8), 0, cfg, 9);
    if (rc < 9) {
        return -EIO;
    }
    uint16_t total = (uint16_t)(cfg[2] | (cfg[3] << 8));
    if (total > sizeof(cfg)) {
        total = sizeof(cfg);
    }
    rc = xhci_control(dev, 0x80, USB_REQ_GET_DESCRIPTOR,
                      (USB_DESC_CONFIG << 8), 0, cfg, total);
    if (rc < 9) {
        return -EIO;
    }
    uint8_t config_value = cfg[5];

    /* Walk the descriptor chain for the first interface that some
     * registered driver wants, and the first interrupt IN endpoint
     * that follows it. */
    struct xhci_class_driver *drv = NULL;
    uint8_t ep_addr = 0, ep_interval = 0;
    uint16_t ep_size = 0;
    bool in_wanted_if = false;

    for (uint16_t off = 0; off + 2 <= total; ) {
        uint8_t len = cfg[off];
        uint8_t type = cfg[off + 1];
        if (len == 0) {
            break;
        }
        if (type == 4 && off + 9 <= total) { /* interface */
            dev->if_number = cfg[off + 2];
            dev->if_class = cfg[off + 5];
            dev->if_subclass = cfg[off + 6];
            dev->if_protocol = cfg[off + 7];
            struct xhci_class_driver *cand = match_driver(dev);
            in_wanted_if = (cand != NULL) && (drv == NULL);
            if (in_wanted_if) {
                drv = cand;
            }
        } else if (type == 5 && off + 7 <= total && in_wanted_if &&
                   ep_addr == 0) { /* endpoint */
            uint8_t addr = cfg[off + 2];
            uint8_t attrs = cfg[off + 3];
            if ((attrs & 0x03) == 0x03 && (addr & 0x80) != 0) {
                ep_addr = addr;
                ep_size = (uint16_t)(cfg[off + 4] | (cfg[off + 5] << 8));
                ep_interval = cfg[off + 6];
            }
        }
        off = (uint16_t)(off + len);
    }

    kprintf("[xhci] port %u: %s speed device %04x:%04x class %02x/%02x/%02x\n",
            port, speed_name(speed), dev->vendor, dev->product,
            dev->if_class, dev->if_subclass, dev->if_protocol);

    if (drv == NULL) {
        kprintf("[xhci]   no driver for this interface\n");
        return 0;
    }

    /* SET_CONFIGURATION before the class driver touches anything:
     * until it is sent the device answers only standard requests. */
    if (xhci_control(dev, 0x00, USB_REQ_SET_CONFIGURATION,
                     config_value, 0, NULL, 0) < 0) {
        kprintf("[xhci]   SET_CONFIGURATION failed\n");
        return -EIO;
    }

    dev->driver_data = drv;
    if (drv->probe != NULL && drv->probe(dev) != 0) {
        kprintf("[xhci]   %s declined the device\n", drv->name);
        dev->driver_data = NULL;
        return 0;
    }
    kprintf("[xhci]   claimed by %s\n", drv->name);

    if (ep_addr != 0) {
        if (xhci_start_interrupt_in(dev, ep_addr, ep_size, ep_interval) != 0) {
            kprintf("[xhci]   could not start endpoint %02x\n", ep_addr);
        }
    }
    return 0;
}

/* ---- the polling task ---- */

static uint32_t g_port_connected; /* bitmap, bit N = port N+1 has a device */

static void scan_ports(void) {
    for (uint32_t port = 1; port <= g_max_ports && port <= 32; port++) {
        uint32_t sc = rd32(g_op, XHCI_PORTSC(port));
        bool connected = (sc & PORTSC_CCS) != 0;
        bool known = (g_port_connected & (1u << (port - 1))) != 0;

        /* Acknowledge the change bits whatever they say; leaving one
         * set makes the controller repeat the event forever. */
        if (sc & (PORTSC_CSC | PORTSC_PEC | PORTSC_WRC | PORTSC_OCC |
                  PORTSC_PRC | PORTSC_PLC | PORTSC_CEC)) {
            wr32(g_op, XHCI_PORTSC(port), (sc & PORTSC_RW_MASK) |
                 (sc & (PORTSC_CSC | PORTSC_PEC | PORTSC_WRC | PORTSC_OCC |
                        PORTSC_PRC | PORTSC_PLC | PORTSC_CEC)));
        }

        if (connected && !known) {
            g_port_connected |= (1u << (port - 1));
            (void)enumerate_port(port);
        } else if (!connected && known) {
            g_port_connected &= ~(1u << (port - 1));
            kprintf("[xhci] port %u: device unplugged\n", port);
            /* Stop polling any endpoint that belonged to it. The slot
             * is left allocated: freeing it needs a Disable Slot
             * command, and a controller that has lost the device does
             * not always answer one. */
            for (int i = 0; i < MAX_INT_EPS; i++) {
                if (g_int_eps[i].active &&
                    g_int_eps[i].dev != NULL &&
                    g_int_eps[i].dev->port == port) {
                    g_int_eps[i].active = false;
                }
            }
        }
    }
}

static void xhci_task(void *arg) {
    (void)arg;

    /* Ports are scanned once up front so devices present at boot are
     * found without waiting for a change event. */
    scan_ports();

    for (;;) {
        (void)event_drain(0, NULL, 0);

        uint32_t sts = rd32(g_op, XHCI_USBSTS);
        if (sts & USBSTS_PCD) {
            wr32(g_op, XHCI_USBSTS, USBSTS_PCD);
            scan_ports();
        }
        if (sts & USBSTS_HSE) {
            kprintf("[xhci] host system error - stopping\n");
            wr32(g_op, XHCI_USBSTS, USBSTS_HSE);
            g_running = false;
            return;
        }
        timer_sleep_ms(XHCI_POLL_MS);
    }
}

/* ---- bring-up ---- */

int xhci_register_class_driver(struct xhci_class_driver *drv) {
    if (drv == NULL || g_driver_count >= 8) {
        return -ENOSPC;
    }
    g_drivers[g_driver_count++] = drv;
    return 0;
}

bool xhci_present(void) {
    return g_running;
}

/* The firmware may still own the controller. The extended capability
 * list has a "USB Legacy Support" entry whose two semaphore bits are
 * how ownership is handed over: raise ours, wait for theirs to drop.
 * Skipping this leaves the BIOS generating SMIs on every USB event,
 * which shows up as a machine that hangs the moment a key is
 * pressed. */
static void take_ownership(void) {
    uint32_t hcc = rd32(g_cap, XHCI_HCCPARAMS1);
    uint32_t off = (hcc >> 16) & 0xFFFF;
    if (off == 0) {
        return;
    }

    volatile uint8_t *p = g_cap + off * 4;
    for (int guard = 0; guard < 64; guard++) {
        uint32_t cap = *(volatile uint32_t *)p;
        uint8_t id = cap & 0xFF;
        uint8_t next = (cap >> 8) & 0xFF;

        if (id == 1) { /* USB Legacy Support */
            if (cap & (1u << 16)) { /* BIOS owned */
                *(volatile uint32_t *)p = cap | (1u << 24); /* OS owned */
                for (int i = 0; i < 1000; i++) {
                    if ((*(volatile uint32_t *)p & (1u << 16)) == 0) {
                        break;
                    }
                    timer_sleep_ms(1);
                }
            }
            /* Turn off SMI generation whatever happened. */
            *(volatile uint32_t *)(p + 4) = 0;
            return;
        }
        if (next == 0) {
            return;
        }
        p += next * 4;
    }
}

static int scratchpad_setup(void) {
    uint32_t hcs2 = rd32(g_cap, XHCI_HCSPARAMS2);
    uint32_t count = ((hcs2 >> 27) & 0x1F) | (((hcs2 >> 21) & 0x1F) << 5);
    if (count == 0) {
        return 0;
    }

    /* The controller wants pages of its own to work in, listed in an
     * array whose address lives in DCBAA entry 0. */
    g_scratchpad_array = dma_alloc(count * sizeof(uint64_t));
    g_scratchpads = dma_alloc((size_t)count * 4096);
    if (g_scratchpad_array.virt == NULL || g_scratchpads.virt == NULL) {
        return -ENOMEM;
    }
    uint64_t *arr = (uint64_t *)g_scratchpad_array.virt;
    for (uint32_t i = 0; i < count; i++) {
        arr[i] = g_scratchpads.phys + (uint64_t)i * 4096;
    }
    ((uint64_t *)g_dcbaa.virt)[0] = g_scratchpad_array.phys;
    kprintf("[xhci] %u scratchpad buffers\n", count);
    return 0;
}

static int xhci_start(uint64_t bar_phys) {
    /* 64 KiB, not the capability page: DBOFF and RTSOFF point at
     * register blocks further into the BAR (qemu-xhci puts the
     * doorbells at 0x2000), and a mapping that stops at the
     * capability registers faults the first time a doorbell is
     * rung. No controller's register block is larger than this. */
    g_cap = (volatile uint8_t *)vmm_map_mmio(bar_phys, 0x10000);
    if (g_cap == NULL) {
        kprintf("[xhci] cannot map the register block\n");
        return -ENOMEM;
    }

    /* CAPLENGTH and HCIVERSION share one dword. Read it as a dword:
     * some controllers only decode 32-bit accesses to their register
     * block and answer a narrower read with zeroes. */
    uint32_t cap0 = rd32(g_cap, 0x00);
    uint8_t caplen = (uint8_t)(cap0 & 0xFF);
    uint16_t version = (uint16_t)(cap0 >> 16);
    g_op = g_cap + caplen;
    g_rt = g_cap + (rd32(g_cap, XHCI_RTSOFF) & ~0x1Fu);
    g_db = g_cap + (rd32(g_cap, XHCI_DBOFF) & ~0x3u);

    uint32_t hcs1 = rd32(g_cap, XHCI_HCSPARAMS1);
    uint32_t hcc = rd32(g_cap, XHCI_HCCPARAMS1);
    g_max_slots = hcs1 & 0xFF;
    g_max_ports = (hcs1 >> 24) & 0xFF;
    g_ctx_size = (hcc & (1u << 2)) ? 64 : 32;

    kprintf("[xhci] version %u.%u, %u slots, %u ports, %u-byte contexts\n",
            version >> 8, version & 0xFF, g_max_slots, g_max_ports,
            g_ctx_size);

    take_ownership();

    /* Stop it, then reset it. A controller that is still running when
     * HCRST is written is a controller in an undefined state. */
    wr32(g_op, XHCI_USBCMD, rd32(g_op, XHCI_USBCMD) & ~USBCMD_RS);
    for (int i = 0; i < 100; i++) {
        if (rd32(g_op, XHCI_USBSTS) & USBSTS_HCH) {
            break;
        }
        timer_sleep_ms(1);
    }

    wr32(g_op, XHCI_USBCMD, USBCMD_HCRST);
    for (int i = 0; i < 1000; i++) {
        uint32_t cmd = rd32(g_op, XHCI_USBCMD);
        uint32_t sts = rd32(g_op, XHCI_USBSTS);
        if ((cmd & USBCMD_HCRST) == 0 && (sts & USBSTS_CNR) == 0) {
            break;
        }
        timer_sleep_ms(1);
    }
    if (rd32(g_op, XHCI_USBSTS) & USBSTS_CNR) {
        kprintf("[xhci] controller did not come back from reset\n");
        return -EIO;
    }

    if (g_max_slots > MAX_SLOTS_USED) {
        g_max_slots = MAX_SLOTS_USED;
    }
    wr32(g_op, XHCI_CONFIG, g_max_slots);

    /* Device context base address array: one 64-bit pointer per slot,
     * plus entry 0 for the scratchpad array. */
    g_dcbaa = dma_alloc((g_max_slots + 1) * sizeof(uint64_t));
    if (g_dcbaa.virt == NULL) {
        return -ENOMEM;
    }
    if (scratchpad_setup() != 0) {
        return -ENOMEM;
    }
    wr64(g_op, XHCI_DCBAAP, g_dcbaa.phys);

    /* Command ring. Bit 0 of CRCR is the ring cycle state the
     * controller starts with, and it must match ours. */
    if (ring_init(&g_cmd, CMD_RING_TRBS) != 0) {
        return -ENOMEM;
    }
    ring_close(&g_cmd);
    wr64(g_op, XHCI_CRCR, g_cmd.buf.phys | 1u);

    /* Event ring: one segment, described by a one-entry segment
     * table. No link TRB - the controller wraps it from the table. */
    if (ring_init(&g_event, EVENT_RING_TRBS) != 0) {
        return -ENOMEM;
    }
    g_erst = dma_alloc(64);
    if (g_erst.virt == NULL) {
        return -ENOMEM;
    }
    uint64_t *erst = (uint64_t *)g_erst.virt;
    erst[0] = g_event.buf.phys;
    erst[1] = EVENT_RING_TRBS; /* size in TRBs, upper 48 bits reserved */

    wr32(g_rt, XHCI_ERSTSZ, 1);
    wr64(g_rt, XHCI_ERDP, g_event.buf.phys);
    wr64(g_rt, XHCI_ERSTBA, g_erst.phys);
    /* Interrupts stay masked: the driver polls (see xhci.h). IMOD is
     * irrelevant then, but leaving it at the reset default is fine. */
    wr32(g_rt, XHCI_IMAN, 0);

    g_ctrl_buf = dma_alloc(512);
    if (g_ctrl_buf.virt == NULL) {
        return -ENOMEM;
    }

    /* Run. */
    wr32(g_op, XHCI_USBCMD, rd32(g_op, XHCI_USBCMD) | USBCMD_RS);
    for (int i = 0; i < 100; i++) {
        if ((rd32(g_op, XHCI_USBSTS) & USBSTS_HCH) == 0) {
            break;
        }
        timer_sleep_ms(1);
    }
    if (rd32(g_op, XHCI_USBSTS) & USBSTS_HCH) {
        kprintf("[xhci] controller will not start\n");
        return -EIO;
    }

    /* Power every port. Ports that are already powered ignore it. */
    for (uint32_t port = 1; port <= g_max_ports; port++) {
        uint32_t sc = rd32(g_op, XHCI_PORTSC(port));
        if ((sc & PORTSC_PP) == 0) {
            wr32(g_op, XHCI_PORTSC(port), (sc & PORTSC_RW_MASK) | PORTSC_PP);
        }
    }
    timer_sleep_ms(20);

    g_running = true;
    return 0;
}

int xhci_init(void) {
    /* Find the controller: PCI class 0x0C (serial bus), subclass 0x03
     * (USB), programming interface 0x30 (xHCI). */
    for (uint8_t bus = 0; bus < 4; bus++) {
        for (uint8_t dev = 0; dev < 32; dev++) {
            for (uint8_t fn = 0; fn < 8; fn++) {
                uint32_t id = pci_config_read(bus, dev, fn, 0x00);
                if ((id & 0xFFFF) == 0xFFFF) {
                    continue;
                }
                uint32_t cls = pci_config_read(bus, dev, fn, 0x08);
                if (((cls >> 24) & 0xFF) != 0x0C ||
                    ((cls >> 16) & 0xFF) != 0x03 ||
                    ((cls >> 8) & 0xFF) != 0x30) {
                    continue;
                }

                uint32_t bar0 = pci_config_read(bus, dev, fn, 0x10);
                if (bar0 & 0x1) {
                    continue; /* an I/O BAR is not an xHCI register block */
                }
                uint64_t base = bar0 & 0xFFFFFFF0u;
                if (((bar0 >> 1) & 0x3) == 0x2) {
                    base |= (uint64_t)pci_config_read(bus, dev, fn, 0x14) << 32;
                }
                if (base == 0) {
                    continue;
                }

                /* Memory decoding and bus mastering: without the
                 * second one the controller cannot read a single one
                 * of the rings it has just been given. */
                uint32_t cmd = pci_config_read(bus, dev, fn, 0x04);
                pci_config_write(bus, dev, fn, 0x04,
                                 cmd | PCI_COMMAND_MEM_SPACE |
                                 PCI_COMMAND_MASTER);

                kprintf("[xhci] controller at %u:%u.%u, registers 0x%lx\n",
                        bus, dev, (unsigned)fn, (unsigned long)base);

                if (xhci_start(base) != 0) {
                    return -1;
                }
                if (task_create_kernel(xhci_task, NULL, "xhci") < 0) {
                    kprintf("[xhci] cannot start the polling task\n");
                    g_running = false;
                    return -1;
                }
                return 0;
            }
        }
    }
    return -1; /* no xHCI controller: this machine simply has no USB 3 */
}

void xhci_print_state(void) {
    if (!g_running) {
        kprintf("xHCI: no controller\n");
        return;
    }
    kprintf("xHCI: %u slots, %u ports, %u-byte contexts\n",
            g_max_slots, g_max_ports, g_ctx_size);
    for (uint32_t port = 1; port <= g_max_ports; port++) {
        uint32_t sc = rd32(g_op, XHCI_PORTSC(port));
        if (sc & PORTSC_CCS) {
            kprintf("  port %-2u connected, %s speed%s\n", port,
                    speed_name((uint8_t)PORTSC_SPEED(sc)),
                    (sc & PORTSC_PED) ? ", enabled" : "");
        }
    }
    for (int i = 0; i < MAX_SLOTS_USED; i++) {
        struct xhci_device *d = &g_devices[i];
        if (d->slot != 0) {
            kprintf("  slot %-2u %04x:%04x  class %02x/%02x/%02x  %s\n",
                    d->slot, d->vendor, d->product, d->if_class,
                    d->if_subclass, d->if_protocol,
                    d->driver_data != NULL ?
                        ((struct xhci_class_driver *)d->driver_data)->name :
                        "(no driver)");
        }
    }
}
