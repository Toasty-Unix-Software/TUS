/*
 * tcp.c - TCP
 *
 * The state machine follows RFC 793: a PCB holds the send and receive
 * sequence spaces, tcp_input() validates a segment against them and
 * tcp_output() pushes whatever the peer's window allows.
 *
 * Two things are worth knowing before reading further:
 *
 *  - Retransmission runs off net_poll(), not off a timer interrupt.
 *    Every blocking path (connect, accept, send, recv) sits in
 *    net_wait(), which calls net_poll(), which calls tcp_timer(), so a
 *    connection anybody is waiting on is always being serviced. A
 *    connection nobody is waiting on has nothing to retransmit for.
 *
 *  - A segment arriving ahead of rcv_nxt is dropped rather than
 *    queued. The duplicate ACK we send makes the peer resend it in
 *    order. That costs throughput on a lossy path and costs nothing on
 *    the paths TUS actually uses.
 */

#include "tcp.h"

#include "ip.h"
#include "netif.h"

#include "../arch/x86_64/io.h"
#include "../core/errno.h"
#include "../core/klib.h"
#include "../drivers/pit/pit.h"
#include "../mm/kmalloc.h"
#include "../vfs/vfs.h"

#define TCP_PCB_MAX        64
#define TCP_EPHEMERAL_LOW  49152
#define TCP_EPHEMERAL_HIGH 65535

#define TCP_RTO_MIN_MS     200
#define TCP_RTO_MAX_MS     8000
#define TCP_RETRIES_MAX    8
#define TCP_CONNECT_TIMEOUT_MS 10000
#define TCP_TIME_WAIT_MS   5000

static struct tcp_pcb *pcbs[TCP_PCB_MAX];

/* Sequence numbers wrap, so they are compared by signed difference. */
static inline bool seq_lt(uint32_t a, uint32_t b)  { return (int32_t)(a - b) < 0; }
static inline bool seq_leq(uint32_t a, uint32_t b) { return (int32_t)(a - b) <= 0; }
static inline bool seq_gt(uint32_t a, uint32_t b)  { return (int32_t)(a - b) > 0; }
static inline bool seq_geq(uint32_t a, uint32_t b) { return (int32_t)(a - b) >= 0; }

static uint32_t tcp_isn(void) {
    /* RFC 793's clock-driven ISN, with the timestamp counter mixed in
     * so two connections made in the same millisecond differ. */
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return (uint32_t)(pit_uptime_ms() * 250000u) ^ lo ^ (hi << 13);
}

/* ---- PCB table ---- */

static void tcp_pcb_free(struct tcp_pcb *pcb) {
    for (int i = 0; i < TCP_PCB_MAX; i++) {
        if (pcbs[i] == pcb) {
            pcbs[i] = NULL;
            break;
        }
    }
    if (pcb->rx_buf) kfree(pcb->rx_buf);
    if (pcb->tx_buf) kfree(pcb->tx_buf);
    kfree(pcb);
}

struct tcp_pcb *tcp_pcb_new(void) {
    int slot = -1;
    for (int i = 0; i < TCP_PCB_MAX; i++) {
        if (pcbs[i] == NULL) { slot = i; break; }
    }
    if (slot < 0) return NULL;

    struct tcp_pcb *pcb = kmalloc(sizeof(*pcb));
    if (!pcb) return NULL;
    memset(pcb, 0, sizeof(*pcb));

    pcb->rx_buf = kmalloc(TCP_RX_BUF_SIZE);
    pcb->tx_buf = kmalloc(TCP_TX_BUF_SIZE);
    if (!pcb->rx_buf || !pcb->tx_buf) {
        if (pcb->rx_buf) kfree(pcb->rx_buf);
        if (pcb->tx_buf) kfree(pcb->tx_buf);
        kfree(pcb);
        return NULL;
    }

    pcb->in_use = true;
    pcb->state = TCP_STATE_CLOSED;
    pcb->rto_ms = TCP_RTO_MIN_MS;
    pcb->snd_wnd = TCP_MSS;

    pcbs[slot] = pcb;
    return pcb;
}

static bool tcp_port_taken(uint16_t port) {
    for (int i = 0; i < TCP_PCB_MAX; i++) {
        if (pcbs[i] && pcbs[i]->local_port == port) return true;
    }
    return false;
}

static uint16_t tcp_alloc_port(void) {
    static uint16_t next = TCP_EPHEMERAL_LOW;

    for (int tries = 0; tries <= TCP_EPHEMERAL_HIGH - TCP_EPHEMERAL_LOW; tries++) {
        uint16_t port = next++;
        if (next > TCP_EPHEMERAL_HIGH) next = TCP_EPHEMERAL_LOW;
        if (!tcp_port_taken(port)) return port;
    }
    return 0;
}

/* An exact four-tuple match wins; a listener on the port is the
 * fallback, which is how a SYN reaches an accepting socket. */
static struct tcp_pcb *tcp_lookup(uint32_t src_ip, uint16_t src_port,
                                  uint16_t dst_port) {
    struct tcp_pcb *listener = NULL;

    for (int i = 0; i < TCP_PCB_MAX; i++) {
        struct tcp_pcb *pcb = pcbs[i];
        if (!pcb || pcb->local_port != dst_port) continue;

        if (pcb->state == TCP_STATE_LISTEN) {
            listener = pcb;
            continue;
        }
        if (pcb->remote_ip == src_ip && pcb->remote_port == src_port) {
            return pcb;
        }
    }
    return listener;
}

/* ---- ring helpers ---- */

static uint32_t rx_free(const struct tcp_pcb *pcb) {
    return TCP_RX_BUF_SIZE - pcb->rx_len;
}

static void rx_append(struct tcp_pcb *pcb, const uint8_t *data, uint32_t len) {
    uint32_t tail = (pcb->rx_head + pcb->rx_len) % TCP_RX_BUF_SIZE;
    uint32_t first = TCP_RX_BUF_SIZE - tail;

    if (len <= first) {
        memcpy(pcb->rx_buf + tail, data, len);
    } else {
        memcpy(pcb->rx_buf + tail, data, first);
        memcpy(pcb->rx_buf, data + first, len - first);
    }
    pcb->rx_len += len;
}

static void tx_peek(const struct tcp_pcb *pcb, uint32_t off, uint8_t *out,
                    uint32_t len) {
    uint32_t start = (pcb->tx_head + off) % TCP_TX_BUF_SIZE;
    uint32_t first = TCP_TX_BUF_SIZE - start;

    if (len <= first) {
        memcpy(out, pcb->tx_buf + start, len);
    } else {
        memcpy(out, pcb->tx_buf + start, first);
        memcpy(out + first, pcb->tx_buf, len - first);
    }
}

static void tx_append(struct tcp_pcb *pcb, const uint8_t *data, uint32_t len) {
    uint32_t tail = (pcb->tx_head + pcb->tx_len) % TCP_TX_BUF_SIZE;
    uint32_t first = TCP_TX_BUF_SIZE - tail;

    if (len <= first) {
        memcpy(pcb->tx_buf + tail, data, len);
    } else {
        memcpy(pcb->tx_buf + tail, data, first);
        memcpy(pcb->tx_buf + 0, data + first, len - first);
    }
    pcb->tx_len += len;
}

static void tx_consume(struct tcp_pcb *pcb, uint32_t len) {
    if (len > pcb->tx_len) len = pcb->tx_len;
    pcb->tx_head = (pcb->tx_head + len) % TCP_TX_BUF_SIZE;
    pcb->tx_len -= len;
}

/* ---- segment transmission ---- */

static int tcp_send_segment(struct tcp_pcb *pcb, uint8_t flags, uint32_t seq,
                            const uint8_t *data, uint16_t len, bool with_mss) {
    uint8_t hdr[sizeof(struct tcp_header) + 4];
    struct tcp_header *th = (struct tcp_header *)hdr;
    uint16_t hlen = sizeof(struct tcp_header);

    th->src_port = htons(pcb->local_port);
    th->dst_port = htons(pcb->remote_port);
    th->seq = htonl(seq);
    th->ack = htonl(pcb->rcv_nxt);
    th->flags = flags;
    th->window = htons((uint16_t)(rx_free(pcb) > 0xffff ? 0xffff : rx_free(pcb)));
    th->checksum = 0;
    th->urgent = 0;

    if (with_mss) {
        hdr[hlen + 0] = 2;               /* kind: maximum segment size */
        hdr[hlen + 1] = 4;               /* length */
        hdr[hlen + 2] = (TCP_MSS >> 8) & 0xff;
        hdr[hlen + 3] = TCP_MSS & 0xff;
        hlen += 4;
    }
    th->offset_reserved = (uint8_t)((hlen / 4) << 4);

    uint32_t src = pcb->local_ip ? pcb->local_ip : g_netif.ip;
    th->checksum = transport_checksum(src, pcb->remote_ip, IP_PROTOCOL_TCP,
                                      hdr, hlen, data, len);

    pcb->needs_ack = false;
    return ip_output(pcb->remote_ip, IP_PROTOCOL_TCP, hdr, hlen, data, len);
}

/* A bare RST for a segment that matches no connection. Built by hand
 * because there is no PCB to hang it off. */
static void tcp_send_reset(uint32_t dst_ip, uint16_t local_port,
                           uint16_t remote_port, uint32_t seq, uint32_t ack,
                           bool ack_valid) {
    struct tcp_header th;

    th.src_port = htons(local_port);
    th.dst_port = htons(remote_port);
    th.offset_reserved = (sizeof(th) / 4) << 4;
    th.window = 0;
    th.checksum = 0;
    th.urgent = 0;

    if (ack_valid) {
        /* Answering a segment that carried an ACK: mirror it. */
        th.seq = htonl(ack);
        th.ack = 0;
        th.flags = TCP_FLAG_RST;
    } else {
        th.seq = 0;
        th.ack = htonl(seq);
        th.flags = TCP_FLAG_RST | TCP_FLAG_ACK;
    }

    th.checksum = transport_checksum(g_netif.ip, dst_ip, IP_PROTOCOL_TCP,
                                     &th, sizeof(th), NULL, 0);
    ip_output(dst_ip, IP_PROTOCOL_TCP, &th, sizeof(th), NULL, 0);
}

static void tcp_start_rto(struct tcp_pcb *pcb) {
    if (pcb->rto_deadline_ms == 0) {
        pcb->rto_deadline_ms = pit_uptime_ms() + pcb->rto_ms;
        pcb->retries = 0;
    }
}

/*
 * Push as much of the send buffer as the peer's window allows, in
 * segments of at most one MSS, then the FIN if the application asked
 * for one and everything before it has gone out.
 */
static void tcp_output(struct tcp_pcb *pcb) {
    if (pcb->state != TCP_STATE_ESTABLISHED &&
        pcb->state != TCP_STATE_CLOSE_WAIT &&
        pcb->state != TCP_STATE_FIN_WAIT1 &&
        pcb->state != TCP_STATE_LAST_ACK) {
        return;
    }

    uint8_t seg[TCP_MSS];

    for (;;) {
        uint32_t in_flight = pcb->snd_nxt - pcb->snd_una;
        if (in_flight >= pcb->tx_len) break;          /* nothing unsent */

        uint32_t window = pcb->snd_wnd;
        if (window == 0) {
            /* Zero window: leave the data queued. The peer's next
             * window update, or the retransmission timer's probe,
             * gets it moving again. */
            break;
        }
        if (in_flight >= window) break;

        uint32_t can_send = window - in_flight;
        uint32_t unsent = pcb->tx_len - in_flight;
        uint32_t n = unsent < can_send ? unsent : can_send;
        if (n > TCP_MSS) n = TCP_MSS;

        tx_peek(pcb, in_flight, seg, n);
        if (tcp_send_segment(pcb, TCP_FLAG_ACK | TCP_FLAG_PSH, pcb->snd_nxt,
                             seg, (uint16_t)n, false) < 0) {
            break;
        }
        pcb->snd_nxt += n;
        tcp_start_rto(pcb);
    }

    if (pcb->fin_queued && !pcb->fin_sent &&
        pcb->snd_nxt == pcb->snd_una + pcb->tx_len) {
        if (tcp_send_segment(pcb, TCP_FLAG_ACK | TCP_FLAG_FIN, pcb->snd_nxt,
                             NULL, 0, false) >= 0) {
            pcb->snd_nxt++;
            pcb->fin_sent = true;
            tcp_start_rto(pcb);
        }
    }
}

/* ---- connection setup ---- */

long tcp_bind(struct tcp_pcb *pcb, uint32_t ip, uint16_t port) {
    if (!pcb || pcb->state != TCP_STATE_CLOSED) return -EINVAL;

    if (port == 0) {
        port = tcp_alloc_port();
        if (port == 0) return -EADDRINUSE;
    } else if (tcp_port_taken(port)) {
        return -EADDRINUSE;
    }

    pcb->local_ip = ip;
    pcb->local_port = port;
    return 0;
}

long tcp_listen(struct tcp_pcb *pcb, int backlog) {
    if (!pcb) return -EINVAL;
    if (pcb->state != TCP_STATE_CLOSED) return -EINVAL;
    if (pcb->local_port == 0) return -EDESTADDRREQ;

    if (backlog < 1) backlog = 1;
    if (backlog > TCP_BACKLOG_MAX) backlog = TCP_BACKLOG_MAX;
    pcb->backlog_max = backlog;
    pcb->state = TCP_STATE_LISTEN;
    return 0;
}

long tcp_connect(struct tcp_pcb *pcb, uint32_t dst_ip, uint16_t dst_port) {
    if (!pcb) return -EINVAL;
    if (pcb->state == TCP_STATE_ESTABLISHED) return -EISCONN;
    if (pcb->state != TCP_STATE_CLOSED) return -EINVAL;
    if (!g_netif.up) return -ENETDOWN;

    if (pcb->local_port == 0) {
        pcb->local_port = tcp_alloc_port();
        if (pcb->local_port == 0) return -EADDRINUSE;
    }
    pcb->local_ip = g_netif.ip;
    pcb->remote_ip = dst_ip;
    pcb->remote_port = dst_port;

    pcb->iss = tcp_isn();
    pcb->snd_una = pcb->iss;
    pcb->snd_nxt = pcb->iss;
    pcb->rcv_nxt = 0;
    pcb->state = TCP_STATE_SYN_SENT;

    if (tcp_send_segment(pcb, TCP_FLAG_SYN, pcb->iss, NULL, 0, true) < 0) {
        pcb->state = TCP_STATE_CLOSED;
        return -EHOSTUNREACH; /* ARP found nobody for the next hop */
    }
    pcb->snd_nxt = pcb->iss + 1;
    tcp_start_rto(pcb);

    uint64_t deadline = pit_uptime_ms() + TCP_CONNECT_TIMEOUT_MS;
    while (pcb->state == TCP_STATE_SYN_SENT) {
        if (pit_uptime_ms() >= deadline) {
            pcb->state = TCP_STATE_CLOSED;
            return -ETIMEDOUT;
        }
        net_wait(deadline);
        if (pcb->reset) {
            pcb->state = TCP_STATE_CLOSED;
            return -ECONNREFUSED;
        }
    }
    return pcb->state == TCP_STATE_ESTABLISHED ? 0 : -ECONNREFUSED;
}

struct tcp_pcb *tcp_accept(struct tcp_pcb *pcb, long *err, bool blocking) {
    if (!pcb || pcb->state != TCP_STATE_LISTEN) {
        *err = -EINVAL;
        return NULL;
    }

    for (;;) {
        for (int i = 0; i < pcb->backlog_count; i++) {
            struct tcp_pcb *child = pcb->backlog[i];
            if (!child) continue;

            /* A half-open child stays queued; only a completed
             * handshake is handed to the application. */
            if (child->state == TCP_STATE_CLOSED || child->reset) {
                for (int j = i; j < pcb->backlog_count - 1; j++) {
                    pcb->backlog[j] = pcb->backlog[j + 1];
                }
                pcb->backlog_count--;
                child->parent = NULL;
                tcp_pcb_free(child);
                i--;
                continue;
            }
            if (child->state != TCP_STATE_SYN_RCVD) {
                for (int j = i; j < pcb->backlog_count - 1; j++) {
                    pcb->backlog[j] = pcb->backlog[j + 1];
                }
                pcb->backlog_count--;
                child->parent = NULL;
                *err = 0;
                return child;
            }
        }

        if (!blocking) {
            *err = -EAGAIN;
            return NULL;
        }
        net_wait(pit_uptime_ms() + 1000);
    }
}

/* ---- data transfer ---- */

long tcp_send(struct tcp_pcb *pcb, const void *buf, size_t len, bool blocking) {
    if (!pcb || !buf) return -EINVAL;
    if (len == 0) return 0;

    const uint8_t *p = (const uint8_t *)buf;
    size_t written = 0;

    while (written < len) {
        if (pcb->reset) return written ? (long)written : -ECONNRESET;
        if (pcb->state != TCP_STATE_ESTABLISHED &&
            pcb->state != TCP_STATE_CLOSE_WAIT) {
            return written ? (long)written : -EPIPE;
        }
        if (pcb->fin_queued) {
            return written ? (long)written : -EPIPE;
        }

        uint32_t space = TCP_TX_BUF_SIZE - pcb->tx_len;
        if (space == 0) {
            if (!blocking) {
                return written ? (long)written : -EAGAIN;
            }
            tcp_output(pcb);
            net_wait(pit_uptime_ms() + 100);
            continue;
        }

        uint32_t n = (uint32_t)(len - written);
        if (n > space) n = space;
        tx_append(pcb, p + written, n);
        written += n;

        tcp_output(pcb);
    }
    return (long)written;
}

long tcp_recv(struct tcp_pcb *pcb, void *buf, size_t len, bool blocking) {
    if (!pcb || !buf) return -EINVAL;
    if (len == 0) return 0;

    for (;;) {
        if (pcb->rx_len > 0) break;
        if (pcb->rx_fin) return 0;          /* the peer's FIN: end of stream */
        if (pcb->reset) return -ECONNRESET;
        if (pcb->state == TCP_STATE_CLOSED) return 0;
        if (pcb->state != TCP_STATE_ESTABLISHED &&
            pcb->state != TCP_STATE_FIN_WAIT1 &&
            pcb->state != TCP_STATE_FIN_WAIT2 &&
            pcb->state != TCP_STATE_CLOSE_WAIT) {
            return 0;
        }
        if (!blocking) return -EAGAIN;
        net_wait(pit_uptime_ms() + 1000);
    }

    uint32_t n = pcb->rx_len < len ? pcb->rx_len : (uint32_t)len;
    uint32_t first = TCP_RX_BUF_SIZE - pcb->rx_head;

    if (n <= first) {
        memcpy(buf, pcb->rx_buf + pcb->rx_head, n);
    } else {
        memcpy(buf, pcb->rx_buf + pcb->rx_head, first);
        memcpy((uint8_t *)buf + first, pcb->rx_buf, n - first);
    }
    pcb->rx_head = (pcb->rx_head + n) % TCP_RX_BUF_SIZE;
    pcb->rx_len -= n;

    /* Reading drains the receive buffer, which reopens our window;
     * tell the peer so it starts sending again. */
    if (pcb->state == TCP_STATE_ESTABLISHED && n > 0) {
        tcp_send_segment(pcb, TCP_FLAG_ACK, pcb->snd_nxt, NULL, 0, false);
    }
    return (long)n;
}

long tcp_shutdown_write(struct tcp_pcb *pcb) {
    if (!pcb) return -EINVAL;
    if (pcb->fin_queued) return 0;

    if (pcb->state == TCP_STATE_ESTABLISHED) {
        pcb->fin_queued = true;
        pcb->state = TCP_STATE_FIN_WAIT1;
        tcp_output(pcb);
    } else if (pcb->state == TCP_STATE_CLOSE_WAIT) {
        pcb->fin_queued = true;
        pcb->state = TCP_STATE_LAST_ACK;
        tcp_output(pcb);
    }
    return 0;
}

void tcp_close(struct tcp_pcb *pcb) {
    if (!pcb) return;

    if (pcb->state == TCP_STATE_LISTEN) {
        for (int i = 0; i < pcb->backlog_count; i++) {
            if (pcb->backlog[i]) {
                pcb->backlog[i]->parent = NULL;
                tcp_pcb_free(pcb->backlog[i]);
            }
        }
        tcp_pcb_free(pcb);
        return;
    }

    if (pcb->state == TCP_STATE_CLOSED || pcb->reset) {
        tcp_pcb_free(pcb);
        return;
    }

    tcp_shutdown_write(pcb);
    pcb->orphaned = true; /* tcp_timer() frees it once closing finishes */
}

short tcp_poll(struct tcp_pcb *pcb) {
    short r = 0;
    if (!pcb) return POLLERR;

    if (pcb->state == TCP_STATE_LISTEN) {
        for (int i = 0; i < pcb->backlog_count; i++) {
            if (pcb->backlog[i] && pcb->backlog[i]->state != TCP_STATE_SYN_RCVD) {
                r |= POLLIN;
            }
        }
        return r;
    }

    if (pcb->rx_len > 0) r |= POLLIN;
    if (pcb->rx_fin) r |= POLLIN | POLLHUP;
    if (pcb->reset) r |= POLLERR | POLLHUP;

    if ((pcb->state == TCP_STATE_ESTABLISHED ||
         pcb->state == TCP_STATE_CLOSE_WAIT) &&
        !pcb->fin_queued && pcb->tx_len < TCP_TX_BUF_SIZE) {
        r |= POLLOUT;
    }
    return r;
}

/* ---- receive path ---- */

static void tcp_listen_input(struct tcp_pcb *listener, uint32_t src_ip,
                             const struct tcp_header *th) {
    if ((th->flags & TCP_FLAG_SYN) == 0 || (th->flags & TCP_FLAG_ACK)) {
        return;
    }
    if (listener->backlog_count >= listener->backlog_max) {
        return; /* backlog full: drop the SYN, the peer will retry */
    }

    struct tcp_pcb *child = tcp_pcb_new();
    if (!child) return;

    child->local_ip = g_netif.ip;
    child->local_port = listener->local_port;
    child->remote_ip = src_ip;
    child->remote_port = ntohs(th->src_port);

    child->irs = ntohl(th->seq);
    child->rcv_nxt = child->irs + 1;
    child->iss = tcp_isn();
    child->snd_una = child->iss;
    child->snd_nxt = child->iss;
    child->snd_wnd = ntohs(th->window);
    child->state = TCP_STATE_SYN_RCVD;
    child->parent = listener;

    if (tcp_send_segment(child, TCP_FLAG_SYN | TCP_FLAG_ACK, child->iss,
                         NULL, 0, true) < 0) {
        tcp_pcb_free(child);
        return;
    }
    child->snd_nxt = child->iss + 1;
    tcp_start_rto(child);

    listener->backlog[listener->backlog_count++] = child;
}

static void tcp_process_ack(struct tcp_pcb *pcb, uint32_t ack) {
    if (seq_leq(ack, pcb->snd_una) || seq_gt(ack, pcb->snd_nxt)) {
        return; /* duplicate or ahead of anything we sent */
    }

    uint32_t acked = ack - pcb->snd_una;

    /* The FIN occupies one sequence number but no byte of the buffer. */
    uint32_t data_acked = acked;
    if (data_acked > pcb->tx_len) data_acked = pcb->tx_len;
    tx_consume(pcb, data_acked);

    pcb->snd_una = ack;
    pcb->retries = 0;
    pcb->rto_ms = TCP_RTO_MIN_MS;

    /* Restart the timer only while something is still in flight. */
    pcb->rto_deadline_ms = 0;
    if (pcb->snd_una != pcb->snd_nxt) {
        tcp_start_rto(pcb);
    }
}

static bool tcp_fin_acked(const struct tcp_pcb *pcb) {
    return pcb->fin_sent && pcb->snd_una == pcb->snd_nxt;
}

void tcp_input(uint32_t src_ip, uint32_t dst_ip,
               const uint8_t *segment, uint16_t len) {
    if (len < sizeof(struct tcp_header)) return;

    const struct tcp_header *th = (const struct tcp_header *)segment;
    uint16_t hlen = (uint16_t)((th->offset_reserved >> 4) * 4);
    if (hlen < sizeof(struct tcp_header) || hlen > len) return;

    if (transport_checksum(src_ip, dst_ip, IP_PROTOCOL_TCP, segment, len,
                           NULL, 0) != 0) {
        return; /* corrupt: let the peer's retransmission fix it */
    }

    uint16_t src_port = ntohs(th->src_port);
    uint16_t dst_port = ntohs(th->dst_port);
    uint32_t seq = ntohl(th->seq);
    uint32_t ack = ntohl(th->ack);
    uint8_t flags = th->flags;

    const uint8_t *data = segment + hlen;
    uint32_t data_len = (uint32_t)(len - hlen);

    struct tcp_pcb *pcb = tcp_lookup(src_ip, src_port, dst_port);

    if (!pcb) {
        if ((flags & TCP_FLAG_RST) == 0) {
            tcp_send_reset(src_ip, dst_port, src_port, seq + data_len +
                           ((flags & (TCP_FLAG_SYN | TCP_FLAG_FIN)) ? 1 : 0),
                           ack, (flags & TCP_FLAG_ACK) != 0);
        }
        return;
    }

    if (pcb->state == TCP_STATE_LISTEN) {
        tcp_listen_input(pcb, src_ip, th);
        return;
    }

    if (flags & TCP_FLAG_RST) {
        pcb->reset = true;
        pcb->state = TCP_STATE_CLOSED;
        return;
    }

    if (pcb->state == TCP_STATE_SYN_SENT) {
        if ((flags & TCP_FLAG_SYN) && (flags & TCP_FLAG_ACK)) {
            if (ack != pcb->snd_nxt) {
                tcp_send_reset(src_ip, dst_port, src_port, 0, ack, true);
                return;
            }
            pcb->irs = seq;
            pcb->rcv_nxt = seq + 1;
            pcb->snd_una = ack;
            pcb->snd_wnd = ntohs(th->window);
            pcb->rto_deadline_ms = 0;
            pcb->state = TCP_STATE_ESTABLISHED;
            tcp_send_segment(pcb, TCP_FLAG_ACK, pcb->snd_nxt, NULL, 0, false);
        }
        return;
    }

    /* Trim anything the peer is resending that we already have. */
    if (seq_lt(seq, pcb->rcv_nxt)) {
        uint32_t skip = pcb->rcv_nxt - seq;
        if (skip >= data_len) {
            data_len = 0;
        } else {
            data += skip;
            data_len -= skip;
        }
        seq = pcb->rcv_nxt;
    } else if (seq_gt(seq, pcb->rcv_nxt)) {
        /* Out of order. Acknowledge what we do have; the duplicate ACK
         * makes the peer resend from rcv_nxt. */
        tcp_send_segment(pcb, TCP_FLAG_ACK, pcb->snd_nxt, NULL, 0, false);
        return;
    }

    pcb->snd_wnd = ntohs(th->window);

    if (flags & TCP_FLAG_ACK) {
        tcp_process_ack(pcb, ack);

        if (pcb->state == TCP_STATE_SYN_RCVD) {
            if (seq_geq(ack, pcb->iss + 1)) {
                pcb->state = TCP_STATE_ESTABLISHED;
            }
        }
    }

    if (data_len > 0) {
        if (pcb->state == TCP_STATE_ESTABLISHED ||
            pcb->state == TCP_STATE_FIN_WAIT1 ||
            pcb->state == TCP_STATE_FIN_WAIT2) {
            uint32_t space = rx_free(pcb);
            uint32_t n = data_len < space ? data_len : space;
            if (n > 0) {
                rx_append(pcb, data, n);
                pcb->rcv_nxt += n;
            }
            pcb->needs_ack = true;
        }
    }

    /* The FIN is only in sequence once its preceding data is taken. */
    if ((flags & TCP_FLAG_FIN) && seq + data_len == pcb->rcv_nxt) {
        pcb->rcv_nxt++;
        pcb->rx_fin = true;
        pcb->needs_ack = true;

        switch (pcb->state) {
        case TCP_STATE_ESTABLISHED:
            pcb->state = TCP_STATE_CLOSE_WAIT;
            break;
        case TCP_STATE_FIN_WAIT1:
            pcb->state = tcp_fin_acked(pcb) ? TCP_STATE_TIME_WAIT
                                            : TCP_STATE_CLOSING;
            if (pcb->state == TCP_STATE_TIME_WAIT) {
                pcb->timer_deadline_ms = pit_uptime_ms() + TCP_TIME_WAIT_MS;
            }
            break;
        case TCP_STATE_FIN_WAIT2:
            pcb->state = TCP_STATE_TIME_WAIT;
            pcb->timer_deadline_ms = pit_uptime_ms() + TCP_TIME_WAIT_MS;
            break;
        default:
            break;
        }
    }

    /* Our own FIN finishing its journey moves the closing states on. */
    if (tcp_fin_acked(pcb)) {
        switch (pcb->state) {
        case TCP_STATE_FIN_WAIT1:
            pcb->state = TCP_STATE_FIN_WAIT2;
            break;
        case TCP_STATE_CLOSING:
            pcb->state = TCP_STATE_TIME_WAIT;
            pcb->timer_deadline_ms = pit_uptime_ms() + TCP_TIME_WAIT_MS;
            break;
        case TCP_STATE_LAST_ACK:
            pcb->state = TCP_STATE_CLOSED;
            break;
        default:
            break;
        }
    }

    if (pcb->needs_ack) {
        tcp_send_segment(pcb, TCP_FLAG_ACK, pcb->snd_nxt, NULL, 0, false);
    }
    tcp_output(pcb);
}

/* ---- timers ---- */

void tcp_timer(void) {
    uint64_t now = pit_uptime_ms();
    uint8_t seg[TCP_MSS];

    for (int i = 0; i < TCP_PCB_MAX; i++) {
        struct tcp_pcb *pcb = pcbs[i];
        if (!pcb) continue;

        if (pcb->state == TCP_STATE_TIME_WAIT &&
            now >= pcb->timer_deadline_ms) {
            pcb->state = TCP_STATE_CLOSED;
        }

        if (pcb->state == TCP_STATE_CLOSED && pcb->orphaned &&
            pcb->parent == NULL) {
            tcp_pcb_free(pcb);
            continue;
        }

        if (pcb->rto_deadline_ms == 0 || now < pcb->rto_deadline_ms) continue;
        if (pcb->snd_una == pcb->snd_nxt) {
            pcb->rto_deadline_ms = 0;
            continue;
        }

        if (++pcb->retries > TCP_RETRIES_MAX) {
            pcb->reset = true;
            pcb->state = TCP_STATE_CLOSED;
            pcb->rto_deadline_ms = 0;
            continue;
        }

        /* Retransmit from snd_una: the SYN, the FIN, or the oldest
         * unacknowledged data, whichever the connection is waiting on. */
        if (pcb->state == TCP_STATE_SYN_SENT) {
            tcp_send_segment(pcb, TCP_FLAG_SYN, pcb->iss, NULL, 0, true);
        } else if (pcb->state == TCP_STATE_SYN_RCVD) {
            tcp_send_segment(pcb, TCP_FLAG_SYN | TCP_FLAG_ACK, pcb->iss,
                             NULL, 0, true);
        } else if (pcb->tx_len > 0) {
            uint32_t n = pcb->tx_len < TCP_MSS ? pcb->tx_len : TCP_MSS;
            tx_peek(pcb, 0, seg, n);
            tcp_send_segment(pcb, TCP_FLAG_ACK | TCP_FLAG_PSH, pcb->snd_una,
                             seg, (uint16_t)n, false);
        } else if (pcb->fin_sent) {
            tcp_send_segment(pcb, TCP_FLAG_ACK | TCP_FLAG_FIN, pcb->snd_una,
                             NULL, 0, false);
        }

        /* Exponential backoff, capped. */
        pcb->rto_ms = pcb->rto_ms * 2;
        if (pcb->rto_ms > TCP_RTO_MAX_MS) pcb->rto_ms = TCP_RTO_MAX_MS;
        pcb->rto_deadline_ms = now + pcb->rto_ms;
    }
}

/* ---- introspection ---- */

const char *tcp_state_name(int state) {
    switch (state) {
    case TCP_STATE_CLOSED:      return "CLOSED";
    case TCP_STATE_LISTEN:      return "LISTEN";
    case TCP_STATE_SYN_SENT:    return "SYN_SENT";
    case TCP_STATE_SYN_RCVD:    return "SYN_RCVD";
    case TCP_STATE_ESTABLISHED: return "ESTABLISHED";
    case TCP_STATE_FIN_WAIT1:   return "FIN_WAIT1";
    case TCP_STATE_FIN_WAIT2:   return "FIN_WAIT2";
    case TCP_STATE_CLOSING:     return "CLOSING";
    case TCP_STATE_TIME_WAIT:   return "TIME_WAIT";
    case TCP_STATE_CLOSE_WAIT:  return "CLOSE_WAIT";
    case TCP_STATE_LAST_ACK:    return "LAST_ACK";
    default:                    return "?";
    }
}

int tcp_dump(struct tcp_conn_info *out, int max) {
    int n = 0;
    for (int i = 0; i < TCP_PCB_MAX && n < max; i++) {
        struct tcp_pcb *pcb = pcbs[i];
        if (!pcb) continue;
        out[n].state = pcb->state;
        out[n].local_ip = pcb->local_ip;
        out[n].local_port = pcb->local_port;
        out[n].remote_ip = pcb->remote_ip;
        out[n].remote_port = pcb->remote_port;
        out[n].rx_queued = pcb->rx_len;
        out[n].tx_queued = pcb->tx_len;
        n++;
    }
    return n;
}
