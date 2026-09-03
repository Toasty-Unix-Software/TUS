/*
 * tcp6.h - TCP over IPv6
 *
 * A structural duplicate of tcp.h/tcp.c's state machine (see tcp.h for
 * the RFC 793 rationale and the same deliberately-out-of-scope list:
 * no reassembly, no window scaling, no SACK, no congestion control
 * beyond a fixed window) with 16-byte addresses and the IPv6
 * pseudo-header checksum instead of IPv4's.
 *
 * This is a real, honest limitation worth stating plainly: the state
 * machine is copied rather than shared via a family-parameterised
 * core. Unifying tcp.c and tcp6.c behind one engine (addresses stored
 * as a tagged union, checksum/output calls branching on family) is
 * the right long-term shape and is left as follow-up work - it is a
 * larger, riskier refactor of the already-working IPv4 TCP than
 * duplicating its ~900 lines once and adapting the addressing.
 */

#ifndef TUS_NET_TCP6_H
#define TUS_NET_TCP6_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TCP6_STATE_CLOSED      0
#define TCP6_STATE_LISTEN      1
#define TCP6_STATE_SYN_SENT    2
#define TCP6_STATE_SYN_RCVD    3
#define TCP6_STATE_ESTABLISHED 4
#define TCP6_STATE_FIN_WAIT1   5
#define TCP6_STATE_FIN_WAIT2   6
#define TCP6_STATE_CLOSING     7
#define TCP6_STATE_TIME_WAIT   8
#define TCP6_STATE_CLOSE_WAIT  9
#define TCP6_STATE_LAST_ACK    10

#define TCP6_RX_BUF_SIZE 32768
#define TCP6_TX_BUF_SIZE 32768
#define TCP6_MSS         1440  /* IPv6 header is 20 bytes bigger than v4's */
#define TCP6_BACKLOG_MAX 8

struct tcp6_pcb {
    bool in_use;
    int state;

    uint8_t local_addr[16], remote_addr[16];
    uint16_t local_port, remote_port;

    uint32_t snd_una, snd_nxt, iss;
    uint16_t snd_wnd;

    uint32_t rcv_nxt, irs;

    uint8_t *rx_buf;
    uint32_t rx_head, rx_len;

    uint8_t *tx_buf;
    uint32_t tx_head, tx_len;

    uint64_t rto_deadline_ms;
    uint32_t rto_ms;
    int retries;

    bool fin_queued, fin_sent, rx_fin, reset, orphaned, needs_ack;

    uint64_t timer_deadline_ms;

    int backlog_max;
    struct tcp6_pcb *backlog[TCP6_BACKLOG_MAX];
    int backlog_count;
    struct tcp6_pcb *parent;
};

struct tcp6_pcb *tcp6_pcb_new(void);

long tcp6_bind(struct tcp6_pcb *pcb, const uint8_t ip[16], uint16_t port);
long tcp6_listen(struct tcp6_pcb *pcb, int backlog);
long tcp6_connect(struct tcp6_pcb *pcb, const uint8_t dst_ip[16], uint16_t dst_port);
struct tcp6_pcb *tcp6_accept(struct tcp6_pcb *pcb, long *err, bool blocking);

long tcp6_send(struct tcp6_pcb *pcb, const void *buf, size_t len, bool blocking);
long tcp6_recv(struct tcp6_pcb *pcb, void *buf, size_t len, bool blocking);
long tcp6_shutdown_write(struct tcp6_pcb *pcb);
void tcp6_close(struct tcp6_pcb *pcb);
short tcp6_poll(struct tcp6_pcb *pcb);

void tcp6_input(const uint8_t src_addr[16], const uint8_t dst_addr[16],
                const uint8_t *segment, uint16_t len);
void tcp6_timer(void);

struct tcp6_conn_info {
    int state;
    uint8_t local_addr[16], remote_addr[16];
    uint16_t local_port, remote_port;
    uint32_t rx_queued, tx_queued;
};
int tcp6_dump(struct tcp6_conn_info *out, int max);
const char *tcp6_state_name(int state);

#endif /* TUS_NET_TCP6_H */
