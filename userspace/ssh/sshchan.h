/*
 * sshchan.h - one session channel, and the loop that pumps it
 *
 * SSH multiplexes, but neither `ssh` nor `sshd` here needs more than
 * a single session channel, so this is deliberately one channel and
 * not a table of them. What it does implement properly is flow
 * control: each end advertises a window, and sending past it is a
 * protocol violation that a real peer will drop the connection over.
 *
 * The pump is the same shape on both sides - move bytes between the
 * channel and a pair of file descriptors - so the client and the
 * server share it. On the client those descriptors are stdin and
 * stdout; on the server they are the two ends of the pty.
 */

#ifndef SSHCHAN_H
#define SSHCHAN_H

#include "ssh.h"

/* 2 MiB of window and 32 KiB packets, the values OpenSSH uses. The
 * window is large enough that a bulk transfer is not stalled waiting
 * for adjustments to make the round trip. */
#define SSH_CHAN_WINDOW     (2u * 1024 * 1024)
#define SSH_CHAN_MAX_PACKET 32768u

struct ssh_channel {
    uint32_t local_id;
    uint32_t remote_id;

    uint32_t local_window;  /* what we still let the peer send   */
    uint32_t remote_window; /* what the peer still lets us send  */
    uint32_t remote_max_packet;

    int open;
    int sent_eof;
    int got_eof;
    int got_close;

    int exit_status;
    int have_exit_status;
};

void ssh_chan_init(struct ssh_channel *c, uint32_t local_id);

void ssh_chan_send_data(struct ssh *s, struct ssh_channel *c,
                        const void *data, size_t len);
void ssh_chan_send_ext(struct ssh *s, struct ssh_channel *c, uint32_t type,
                       const void *data, size_t len);
void ssh_chan_send_eof(struct ssh *s, struct ssh_channel *c);
void ssh_chan_send_close(struct ssh *s, struct ssh_channel *c);

/* Give the peer more room once it has used up enough of its window. */
void ssh_chan_consume(struct ssh *s, struct ssh_channel *c, size_t len);

/*
 * Handle one channel message that has already been read into s->pkt
 * (with the type byte consumed). Data destined for the local end is
 * written to out_fd, and stderr data to err_fd. Returns 0 when the
 * message was handled, -1 on a protocol error.
 */
int ssh_chan_dispatch(struct ssh *s, struct ssh_channel *c, uint8_t type,
                      int out_fd, int err_fd);

/*
 * Move data between the channel and the given descriptors until the
 * channel closes. in_fd may be -1 for a half-duplex session. Returns
 * 0 on a clean end, -1 on error.
 */
int ssh_chan_pump(struct ssh *s, struct ssh_channel *c, int in_fd, int out_fd,
                  int err_fd);

#endif /* SSHCHAN_H */
