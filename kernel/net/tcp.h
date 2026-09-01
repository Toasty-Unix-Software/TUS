/*
 * tcp.h - TCP, enough of RFC 793 to carry ssh and git
 *
 * One PCB per connection, all of them in a single table that the
 * receive path demultiplexes against. The send side keeps a ring of
 * bytes from snd_una forward: the front of the ring is data the peer
 * has not acknowledged yet (and which the retransmission timer will
 * send again), the back is data that has not gone out at all.
 *
 * What is deliberately not here: out-of-order reassembly (a segment
 * arriving ahead of rcv_nxt is dropped and the duplicate ACK makes the
 * peer resend it), window scaling, SACK and congestion control beyond
 * a fixed send window. Each of those is a throughput optimisation, not
 * a correctness requirement, and leaving them out keeps the state
 * machine small enough to read.
 */

#ifndef TUS_NET_TCP_H
#define TUS_NET_TCP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TCP_STATE_CLOSED      0
#define TCP_STATE_LISTEN      1
#define TCP_STATE_SYN_SENT    2
#define TCP_STATE_SYN_RCVD    3
#define TCP_STATE_ESTABLISHED 4
#define TCP_STATE_FIN_WAIT1   5
#define TCP_STATE_FIN_WAIT2   6
#define TCP_STATE_CLOSING     7
#define TCP_STATE_TIME_WAIT   8
#define TCP_STATE_CLOSE_WAIT  9
#define TCP_STATE_LAST_ACK    10

#define TCP_RX_BUF_SIZE 32768
#define TCP_TX_BUF_SIZE 32768
#define TCP_MSS         1460
#define TCP_BACKLOG_MAX 8

struct tcp_pcb {
    bool in_use;
    int state;

    uint32_t local_ip, remote_ip;
    uint16_t local_port, remote_port;

    /* Send sequence space (RFC 793 section 3.2). */
    uint32_t snd_una;   /* oldest sequence number not yet acknowledged */
    uint32_t snd_nxt;   /* next sequence number to send */
    uint32_t iss;
    uint16_t snd_wnd;   /* what the peer says it can take */

    /* Receive sequence space. */
    uint32_t rcv_nxt;   /* next sequence number expected */
    uint32_t irs;

    uint8_t *rx_buf;
    uint32_t rx_head, rx_len;

    /* tx_buf[tx_head] is the byte at snd_una; tx_len counts everything
     * buffered, sent or not. snd_nxt - snd_una bytes of it are in
     * flight. */
    uint8_t *tx_buf;
    uint32_t tx_head, tx_len;

    /* Retransmission: one timer for the oldest unacknowledged byte. */
    uint64_t rto_deadline_ms;
    uint32_t rto_ms;
    int retries;

    bool fin_queued;    /* the application closed; send FIN after the data */
    bool fin_sent;
    bool rx_fin;        /* peer's FIN seen and all its data delivered */
    bool reset;         /* connection was reset */
    bool orphaned;      /* no file descriptor left; free when closed */
    bool needs_ack;     /* an ACK is owed for data just received */

    uint64_t timer_deadline_ms; /* TIME_WAIT / connection-establishment */

    /* Listening sockets only. */
    int backlog_max;
    struct tcp_pcb *backlog[TCP_BACKLOG_MAX];
    int backlog_count;
    struct tcp_pcb *parent;
};

/* ---- the socket-facing API. All of it returns negative errno. ---- */

struct tcp_pcb *tcp_pcb_new(void);

long tcp_bind(struct tcp_pcb *pcb, uint32_t ip, uint16_t port);
long tcp_listen(struct tcp_pcb *pcb, int backlog);

/* Blocking three-way handshake. */
long tcp_connect(struct tcp_pcb *pcb, uint32_t dst_ip, uint16_t dst_port);

/* Blocks until a connection is established; *err is 0 with a NULL
 * return only when the socket is non-blocking and none is ready. */
struct tcp_pcb *tcp_accept(struct tcp_pcb *pcb, long *err, bool blocking);

/* Copies into the send buffer and pushes what fits on the wire.
 * Blocks while the buffer is full; returns short writes otherwise. */
long tcp_send(struct tcp_pcb *pcb, const void *buf, size_t len, bool blocking);

/* Returns 0 at end of stream (the peer's FIN), never a short read of
 * zero while data may still arrive. */
long tcp_recv(struct tcp_pcb *pcb, void *buf, size_t len, bool blocking);

/* Half-close: stop sending, keep receiving. */
long tcp_shutdown_write(struct tcp_pcb *pcb);

/* Release the application's reference; the PCB lives on until the
 * connection has finished closing. */
void tcp_close(struct tcp_pcb *pcb);

/* POLL* bits for poll()/select(). */
short tcp_poll(struct tcp_pcb *pcb);

/* ---- stack-facing ---- */

void tcp_input(uint32_t src_ip, uint32_t dst_ip,
               const uint8_t *segment, uint16_t len);

/* Retransmissions and TIME_WAIT expiry; driven from net_poll(). */
void tcp_timer(void);

/* For `netstat`: copy out one line per connection. */
struct tcp_conn_info {
    int state;
    uint32_t local_ip, remote_ip;
    uint16_t local_port, remote_port;
    uint32_t rx_queued, tx_queued;
};
int tcp_dump(struct tcp_conn_info *out, int max);
const char *tcp_state_name(int state);

#endif /* TUS_NET_TCP_H */
