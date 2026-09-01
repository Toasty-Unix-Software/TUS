/*
 * serial.c - 16550 UART driver implementation
 *
 * Register offsets are relative to the COM1 base port 0x3F8. The baud
 * rate is set through a divisor register that is only reachable while
 * the "DLAB" bit of the line control register is set.
 *
 * Two ways of sending a byte live here, and which one is in use is the
 * difference between a responsive console and a slow one:
 *
 *   Synchronous - wait for the transmit holding register, then write.
 *   One byte costs the time it takes to shift ten bits out of a 115200
 *   baud line: 87 microseconds. This is what the kernel uses while it
 *   boots and what the panic path goes back to, because a debug
 *   channel is worth nothing if the machine stops before the bytes
 *   leave.
 *
 *   Buffered - append to a ring and return. The UART raises IRQ4 when
 *   its transmit register empties, and the handler refills it from the
 *   ring. A write now costs a memory store, and the port drains in the
 *   background at whatever speed it can manage.
 *
 * Everything the console prints is mirrored here, so the synchronous
 * cost is not paid once but per byte of every program's output: a
 * full-screen editor redrawing its window is several hundred bytes,
 * and at a typist's speed that is more than a 115200 baud line
 * carries. The mirror is therefore best-effort: it queues, and when a
 * program outruns the wire the queue drops rather than stalling the
 * machine, and says how much it dropped once there is room again. A
 * debug log is worth having, but not at the price of deciding how
 * fast the system is allowed to run - and a short, labelled gap in a
 * log beats an editor that waits a tenth of a second per keystroke.
 */

#include "drivers/serial/serial.h"

#include "../../arch/x86_64/io.h"
#include "../../arch/x86_64/idt.h"
#include "../../arch/x86_64/pic.h"
#include "drivers/keyboard/keyboard.h"
#include "../../sched/sched.h"

#define COM1_BASE 0x3F8
#define COM1_IRQ  4

/* Divisor for 115200 baud (1.8432 MHz / 16 / 115200 = 1). */
#define BAUD_DIVISOR 1

#define REG_DATA 0 /* data register (also divisor LSB with DLAB) */
#define REG_IER  1 /* interrupt enable (also divisor MSB with DLAB) */
#define REG_IIR  2 /* interrupt identification (read) */
#define REG_FCR  2 /* FIFO control (write) */
#define REG_LCR  3 /* line control */
#define REG_MCR  4 /* modem control */
#define REG_LSR  5 /* line status */

#define LCR_DLAB      0x80 /* divisor latch access bit */
#define LCR_8N1       0x03 /* 8 data bits, no parity, 1 stop bit */
#define FCR_ENABLE    0xC7 /* enable FIFOs, clear them, 14-byte threshold */
#define MCR_DTR_RTS   0x0B /* DTR + RTS + OUT2 (needed for interrupts) */
#define IER_DATA_AVAIL 0x01 /* interrupt when a byte has arrived */
#define IER_TX_EMPTY  0x02 /* interrupt when the transmit register empties */

#define LSR_DATA_READY 0x01 /* a received byte is waiting in REG_DATA */
#define LSR_THR_EMPTY 0x20 /* transmit holding register empty */

/* A 16550's transmit FIFO. THR_EMPTY means the whole thing is free,
 * so that many bytes can be handed over in one go. */
#define TX_FIFO_DEPTH 16

/* The queue. A power of two so the indices mask instead of divide,
 * and large enough that an interactive burst - a screenful of output,
 * a few dozen editor redraws - is swallowed whole. */
#define TX_RING 65536
#define TX_MASK (TX_RING - 1)

static bool g_ready;
static bool g_async;

static char g_tx[TX_RING];
static volatile unsigned g_tx_head; /* next slot to fill */
static volatile unsigned g_tx_tail; /* next slot to send */

/* True while the UART still owes us a "transmit register empty"
 * interrupt. Without it every queued byte would poll the port, which
 * is most of what the synchronous path costs. */
static volatile bool g_tx_busy;

/* Bytes the queue could not take. Counted, never silently forgotten:
 * the next write that finds room says how many were lost. */
static volatile uint64_t g_dropped;

static inline bool thr_empty(void) {
    return (inb(COM1_BASE + REG_LSR) & LSR_THR_EMPTY) != 0;
}

/* Hand the UART as much of the ring as it will take. Interrupts must
 * be off; the transmit interrupt touches the same indices. */
static void tx_push(void) {
    if (!thr_empty()) {
        return; /* still shifting: the interrupt will come back to us */
    }
    int sent = 0;
    while (sent < TX_FIFO_DEPTH && g_tx_tail != g_tx_head) {
        outb(COM1_BASE + REG_DATA, (uint8_t)g_tx[g_tx_tail]);
        g_tx_tail = (g_tx_tail + 1) & TX_MASK;
        sent++;
    }
    g_tx_busy = sent > 0;
}

/* Wait for the port and push. The wait is bounded so a broken or
 * absent UART can never hang the kernel. */
static void tx_push_blocking(void) {
    for (unsigned tries = 0; tries < 100000; tries++) {
        if (thr_empty()) {
            break;
        }
    }
    tx_push();
}

/* Turn one received byte into a keyboard event and inject it into the
 * same queue the PS/2 and USB HID drivers feed - tsh, the tty layer
 * and everything downstream already knows how to read from exactly
 * one place, so a byte arriving over the wire needs nothing more
 * than to land there too. This is what makes serial input line
 * editing, history and echo "for free": tsh_key() cannot tell this
 * event apart from one a physical key produced, and every character
 * it prints in response already mirrors back out over this same
 * port (see the file header), so a real serial terminal sees its
 * own typing echoed without TUS doing anything serial-specific for
 * that either.
 *
 * Ctrl+C is the one byte that must NOT become an ordinary character:
 * a real terminal in canonical mode never delivers it as data, and
 * kernel/drivers/keyboard/keyboard.c's own Ctrl+C handling (physical Ctrl held
 * + 'c' pressed) has no way to see a byte that arrives already
 * collapsed to 0x03 - so it is special-cased here the same way, and
 * for the same reason: kill whatever exec_pipeline() is currently
 * waiting on, if anything is. */
static void serial_handle_rx_byte(uint8_t byte) {
    if (byte == 0x03) {
        sched_interrupt_foreground();
        return;
    }

    struct kbd_event ev;
    ev.type = KBD_EVENT_CHAR;
    ev.code = 0;
    ev.mods = 0;
    /* A terminal sends CR for Enter; the shell wants a newline - same
     * translation term_key() (kernel/shell/tsh.c) does for a terminal
     * window's own byte stream. */
    ev.c = (byte == '\r') ? '\n' : (char)byte;
    ev.cp = (uint32_t)(uint8_t)ev.c;
    kbd_inject_event(&ev);
}

static void serial_irq_handler(struct interrupt_frame *frame) {
    (void)frame;
    /* Reading IIR acknowledges the transmit interrupt even when there
     * is nothing left to send, which is what stops it repeating. */
    (void)inb(COM1_BASE + REG_IIR);
    while (inb(COM1_BASE + REG_LSR) & LSR_DATA_READY) {
        serial_handle_rx_byte(inb(COM1_BASE + REG_DATA));
    }
    tx_push();
    pic_send_eoi(COM1_IRQ);
}

bool serial_init(void) {
    outb(COM1_BASE + REG_IER, 0x00); /* disable all UART interrupts */

    /* Set the baud rate: enable DLAB, write the divisor, disable DLAB. */
    outb(COM1_BASE + REG_LCR, LCR_DLAB);
    outb(COM1_BASE + REG_DATA, (uint8_t)(BAUD_DIVISOR & 0xFF));
    outb(COM1_BASE + REG_IER, (uint8_t)((BAUD_DIVISOR >> 8) & 0xFF));
    outb(COM1_BASE + REG_LCR, LCR_8N1);

    outb(COM1_BASE + REG_FCR, FCR_ENABLE);
    outb(COM1_BASE + REG_MCR, MCR_DTR_RTS);

    g_ready = true;
    return true;
}

void serial_start_async(void) {
    if (!g_ready || g_async) {
        return;
    }
    uint64_t flags = irq_save();
    g_tx_head = 0;
    g_tx_tail = 0;
    g_tx_busy = false;
    g_async = true;
    irq_install(COM1_IRQ, serial_irq_handler);
    pic_enable_irq(COM1_IRQ);
    /* Also the receive side: a byte typed into the other end of the
     * wire becomes a keyboard event from here on - see
     * serial_handle_rx_byte() above. Same timing reason as TX: this
     * needs interrupts actually running, so it waits for the same
     * call TX already waits for rather than starting at serial_init()
     * (still on the boot path, interrupts off). */
    outb(COM1_BASE + REG_IER, IER_TX_EMPTY | IER_DATA_AVAIL);
    irq_restore(flags);
}

void serial_sync(void) {
    if (!g_ready) {
        return;
    }
    uint64_t flags = irq_save();
    g_async = false;
    outb(COM1_BASE + REG_IER, 0x00);
    while (g_tx_tail != g_tx_head) {
        tx_push_blocking();
    }
    irq_restore(flags);
}

/* Write one byte the slow, certain way. */
static void tx_direct(char c) {
    /* Wait until the transmitter accepts the byte. The wait is
     * bounded so a broken port can never hang the kernel. */
    for (unsigned tries = 0; tries < 100000; tries++) {
        if (thr_empty()) {
            break;
        }
    }
    outb(COM1_BASE + REG_DATA, (uint8_t)c);
}

/* Append one byte. Interrupts must be off. False when the queue is
 * full. */
static bool tx_queue(char c) {
    unsigned next = (g_tx_head + 1) & TX_MASK;
    if (next == g_tx_tail) {
        return false;
    }
    g_tx[g_tx_head] = c;
    g_tx_head = next;
    return true;
}

static unsigned tx_free(void) {
    return TX_RING - 1 - ((g_tx_head - g_tx_tail) & TX_MASK);
}

/* Once the queue has room again, put a line in the log saying how
 * much of it was lost. A gap nobody can see is a lie; a gap with a
 * number next to it is a measurement. */
static void report_drops(void) {
    if (g_dropped == 0 || tx_free() < 48) {
        return;
    }
    uint64_t lost = g_dropped;
    g_dropped = 0;

    char digits[20];
    int nd = 0;
    do {
        digits[nd++] = (char)('0' + (lost % 10));
        lost /= 10;
    } while (lost > 0);

    const char *pre = "\r\n[serial mirror: ";
    const char *post = " bytes dropped]\r\n";
    while (*pre != '\0') {
        (void)tx_queue(*pre++);
    }
    while (nd > 0) {
        (void)tx_queue(digits[--nd]);
    }
    while (*post != '\0') {
        (void)tx_queue(*post++);
    }
}

void serial_write_n(const char *s, unsigned long n) {
    if (!g_ready || s == NULL) {
        return;
    }
    if (!g_async) {
        for (unsigned long i = 0; i < n; i++) {
            tx_direct(s[i]);
        }
        return;
    }

    /* One pass, with the queue held once: appending is a store, and
     * whether the wire keeps up is the wire's problem. */
    uint64_t flags = irq_save();
    report_drops();
    for (unsigned long i = 0; i < n; i++) {
        if (!tx_queue(s[i])) {
            g_dropped += n - i;
            break;
        }
    }
    if (!g_tx_busy) {
        tx_push(); /* the line is idle: start it going again */
    }
    irq_restore(flags);
}

void serial_putchar(char c) {
    if (!g_ready) {
        return;
    }

    if (!g_async) {
        tx_direct(c);
        return;
    }

    serial_write_n(&c, 1);
}

void serial_write(const char *s) {
    unsigned long n = 0;
    while (s[n] != '\0') {
        n++;
    }
    serial_write_n(s, n);
}
