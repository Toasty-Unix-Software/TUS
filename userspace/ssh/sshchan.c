/*
 * sshchan.c - see sshchan.h
 *
 * The two rules that make a channel work, and that are easy to get
 * subtly wrong:
 *
 *   Never send more than the peer's advertised window, in pieces no
 *   larger than its maximum packet size. Both are per-channel and
 *   both shrink as data flows.
 *
 *   A channel is finished only when both ends have sent CLOSE. EOF is
 *   not the end: the peer may still have output queued, and closing
 *   on EOF is how a session loses the last of a command's output.
 */

#include "sshchan.h"

#include <errno.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>

void ssh_chan_init(struct ssh_channel *c, uint32_t local_id) {
    memset(c, 0, sizeof(*c));
    c->local_id = local_id;
    c->local_window = SSH_CHAN_WINDOW;
    c->remote_max_packet = SSH_CHAN_MAX_PACKET;
    c->exit_status = -1;
}

static void put_msg(struct ssh *s, struct sshbuf *p) {
    ssh_packet_put(s, p);
    sshbuf_free(p);
}

void ssh_chan_send_data(struct ssh *s, struct ssh_channel *c,
                        const void *data, size_t len) {
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_DATA);
    sshbuf_put_u32(&p, c->remote_id);
    sshbuf_put_string(&p, data, len);
    put_msg(s, &p);
    c->remote_window -= (uint32_t)len;
}

void ssh_chan_send_ext(struct ssh *s, struct ssh_channel *c, uint32_t type,
                       const void *data, size_t len) {
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_EXTENDED_DATA);
    sshbuf_put_u32(&p, c->remote_id);
    sshbuf_put_u32(&p, type);
    sshbuf_put_string(&p, data, len);
    put_msg(s, &p);
    c->remote_window -= (uint32_t)len;
}

void ssh_chan_send_eof(struct ssh *s, struct ssh_channel *c) {
    if (c->sent_eof) return;
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_EOF);
    sshbuf_put_u32(&p, c->remote_id);
    put_msg(s, &p);
    c->sent_eof = 1;
}

void ssh_chan_send_close(struct ssh *s, struct ssh_channel *c) {
    if (!c->open) return;
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_CLOSE);
    sshbuf_put_u32(&p, c->remote_id);
    put_msg(s, &p);
    c->open = 0;
}

void ssh_chan_consume(struct ssh *s, struct ssh_channel *c, size_t len) {
    c->local_window -= (uint32_t)len;

    /* Topping the window up only when it is half gone keeps the
     * adjustments to roughly one per megabyte instead of one per
     * packet. */
    if (c->local_window >= SSH_CHAN_WINDOW / 2) return;

    uint32_t add = SSH_CHAN_WINDOW - c->local_window;
    struct sshbuf p;
    sshbuf_init(&p);
    sshbuf_put_u8(&p, SSH_MSG_CHANNEL_WINDOW_ADJUST);
    sshbuf_put_u32(&p, c->remote_id);
    sshbuf_put_u32(&p, add);
    put_msg(s, &p);
    c->local_window += add;
}

static int write_all_fd(int fd, const uint8_t *p, size_t len) {
    while (len > 0) {
        long n = write(fd, p, len);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        p += n;
        len -= (size_t)n;
    }
    return 0;
}

int ssh_chan_dispatch(struct ssh *s, struct ssh_channel *c, uint8_t type,
                      int out_fd, int err_fd) {
    uint32_t id;

    switch (type) {
    case SSH_MSG_CHANNEL_DATA: {
        const uint8_t *data;
        size_t len;
        if (sshbuf_get_u32(&s->pkt, &id) != 0 ||
            sshbuf_get_string(&s->pkt, &data, &len) != 0) {
            return ssh_fail(s, "malformed CHANNEL_DATA");
        }
        if (len > c->local_window) {
            return ssh_fail(s, "peer overran its window by %zu bytes",
                            len - c->local_window);
        }
        if (out_fd >= 0 && write_all_fd(out_fd, data, len) != 0) {
            return ssh_fail(s, "write: %s", strerror(errno));
        }
        ssh_chan_consume(s, c, len);
        return 0;
    }

    case SSH_MSG_CHANNEL_EXTENDED_DATA: {
        const uint8_t *data;
        size_t len;
        uint32_t kind;
        if (sshbuf_get_u32(&s->pkt, &id) != 0 ||
            sshbuf_get_u32(&s->pkt, &kind) != 0 ||
            sshbuf_get_string(&s->pkt, &data, &len) != 0) {
            return ssh_fail(s, "malformed CHANNEL_EXTENDED_DATA");
        }
        if (len > c->local_window) {
            return ssh_fail(s, "peer overran its window");
        }
        int fd = kind == SSH_EXTENDED_DATA_STDERR ? err_fd : -1;
        if (fd >= 0 && write_all_fd(fd, data, len) != 0) {
            return ssh_fail(s, "write: %s", strerror(errno));
        }
        ssh_chan_consume(s, c, len);
        return 0;
    }

    case SSH_MSG_CHANNEL_WINDOW_ADJUST: {
        uint32_t add;
        if (sshbuf_get_u32(&s->pkt, &id) != 0 ||
            sshbuf_get_u32(&s->pkt, &add) != 0) {
            return ssh_fail(s, "malformed CHANNEL_WINDOW_ADJUST");
        }
        c->remote_window += add;
        return 0;
    }

    case SSH_MSG_CHANNEL_EOF:
        c->got_eof = 1;
        return 0;

    case SSH_MSG_CHANNEL_CLOSE:
        c->got_close = 1;
        ssh_chan_send_close(s, c);
        return ssh_flush(s);

    case SSH_MSG_CHANNEL_REQUEST: {
        const char *req;
        size_t req_len;
        int want_reply = 0;
        if (sshbuf_get_u32(&s->pkt, &id) != 0 ||
            sshbuf_get_cstring(&s->pkt, &req, &req_len) != 0 ||
            sshbuf_get_bool(&s->pkt, &want_reply) != 0) {
            return ssh_fail(s, "malformed CHANNEL_REQUEST");
        }

        if (req_len == 11 && memcmp(req, "exit-status", 11) == 0) {
            uint32_t status;
            if (sshbuf_get_u32(&s->pkt, &status) == 0) {
                c->exit_status = (int)status;
                c->have_exit_status = 1;
            }
        }
        /* Anything else - exit-signal, keepalives, an xon-xoff
         * notification - is information this end has no use for. */
        if (want_reply) {
            struct sshbuf p;
            sshbuf_init(&p);
            sshbuf_put_u8(&p, SSH_MSG_CHANNEL_FAILURE);
            sshbuf_put_u32(&p, c->remote_id);
            put_msg(s, &p);
            return ssh_flush(s);
        }
        return 0;
    }

    case SSH_MSG_GLOBAL_REQUEST: {
        int want_reply = 0;
        if (sshbuf_skip_string(&s->pkt) != 0 ||
            sshbuf_get_bool(&s->pkt, &want_reply) != 0) {
            return ssh_fail(s, "malformed GLOBAL_REQUEST");
        }
        if (want_reply) {
            struct sshbuf p;
            sshbuf_init(&p);
            sshbuf_put_u8(&p, SSH_MSG_REQUEST_FAILURE);
            put_msg(s, &p);
            return ssh_flush(s);
        }
        return 0;
    }

    case SSH_MSG_CHANNEL_SUCCESS:
    case SSH_MSG_CHANNEL_FAILURE:
        return 0;

    default:
        /* Tell the peer we did not understand, rather than going
         * quiet: a peer waiting on a reply would otherwise hang. */
        {
            struct sshbuf p;
            sshbuf_init(&p);
            sshbuf_put_u8(&p, SSH_MSG_UNIMPLEMENTED);
            sshbuf_put_u32(&p, s->seq_in - 1);
            put_msg(s, &p);
        }
        return ssh_flush(s);
    }
}

int ssh_chan_pump(struct ssh *s, struct ssh_channel *c, int in_fd, int out_fd,
                  int err_fd) {
    uint8_t buf[SSH_CHAN_MAX_PACKET];
    int local_eof = in_fd < 0;

    while (c->open || !c->got_close) {
        struct pollfd fds[2];
        int nfds = 0;

        int ssh_slot = nfds;
        fds[nfds].fd = s->fd;
        fds[nfds].events = POLLIN;
        nfds++;

        /* Only look at the local input when there is room to forward
         * what it produces; otherwise a fast writer would fill the
         * peer's window and we would have nowhere to put the bytes. */
        int in_slot = -1;
        if (!local_eof && c->remote_window > 0 && c->open) {
            in_slot = nfds;
            fds[nfds].fd = in_fd;
            fds[nfds].events = POLLIN;
            nfds++;
        }

        if (poll(fds, (nfds_t)nfds, -1) < 0) {
            if (errno == EINTR) continue;
            return ssh_fail(s, "poll: %s", strerror(errno));
        }

        if (fds[ssh_slot].revents & (POLLIN | POLLHUP | POLLERR)) {
            long n = ssh_read_more(s);
            if (n == -1) return -1;
            if (n == 0) {
                /* The peer went away without closing the channel.
                 * That is not clean, but there is nothing left to
                 * wait for. */
                break;
            }

            for (;;) {
                uint8_t type;
                int r = ssh_packet_next(s, &type);
                if (r < 0) return -1;
                if (r == 0) break;

                if (type == SSH_MSG_DISCONNECT) {
                    c->got_close = 1;
                    c->open = 0;
                    break;
                }
                if (type == SSH_MSG_IGNORE || type == SSH_MSG_DEBUG ||
                    type == SSH_MSG_UNIMPLEMENTED || type == SSH_MSG_EXT_INFO) {
                    continue;
                }
                if (ssh_chan_dispatch(s, c, type, out_fd, err_fd) != 0) {
                    return -1;
                }
            }
            if (ssh_flush(s) != 0) return -1;
        }

        if (in_slot >= 0 && (fds[in_slot].revents & (POLLIN | POLLHUP))) {
            size_t room = c->remote_window;
            if (room > c->remote_max_packet) room = c->remote_max_packet;
            if (room > sizeof(buf)) room = sizeof(buf);

            long n = read(in_fd, buf, room);
            if (n < 0 && (errno == EINTR || errno == EAGAIN)) continue;
            if (n < 0) return ssh_fail(s, "read: %s", strerror(errno));
            if (n > 0) {
                ssh_chan_send_data(s, c, buf, (size_t)n);
                if (ssh_flush(s) != 0) return -1;
            } else {
                local_eof = 1;
                ssh_chan_send_eof(s, c);
                if (ssh_flush(s) != 0) return -1;
            }
        }

        /* Once both directions are finished there is nothing to wait
         * for; send CLOSE and let the exchange complete. */
        if (c->got_eof && local_eof && c->open) {
            ssh_chan_send_close(s, c);
            if (ssh_flush(s) != 0) return -1;
        }
    }
    return 0;
}
