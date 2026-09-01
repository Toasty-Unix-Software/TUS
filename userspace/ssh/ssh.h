/*
 * ssh.h - the SSH-2 transport, shared by the client and by sshd
 *
 * One algorithm per job, chosen to be the ones OpenSSH prefers anyway
 * so that a TUS peer and an OpenSSH peer meet in the middle without
 * negotiation drama:
 *
 *   key exchange    curve25519-sha256
 *   host key        ssh-ed25519
 *   cipher and MAC  chacha20-poly1305@openssh.com (an AEAD: the MAC
 *                   list is negotiated but unused)
 *   compression     none
 *
 * Everything above the transport - authentication, channels - lives
 * in the client and the server, because that is where the two roles
 * genuinely differ. What they share is this: framing packets,
 * encrypting them, and agreeing on the keys.
 */

#ifndef TUS_SSH_H
#define TUS_SSH_H

#include <stddef.h>
#include <stdint.h>

#include "sshbuf.h"

/* ---- message numbers (RFC 4253, 4252, 4254) ---- */

#define SSH_MSG_DISCONNECT                 1
#define SSH_MSG_IGNORE                     2
#define SSH_MSG_UNIMPLEMENTED               3
#define SSH_MSG_DEBUG                      4
#define SSH_MSG_SERVICE_REQUEST            5
#define SSH_MSG_SERVICE_ACCEPT             6
#define SSH_MSG_EXT_INFO                   7
#define SSH_MSG_KEXINIT                   20
#define SSH_MSG_NEWKEYS                   21
#define SSH_MSG_KEX_ECDH_INIT             30
#define SSH_MSG_KEX_ECDH_REPLY            31

#define SSH_MSG_USERAUTH_REQUEST          50
#define SSH_MSG_USERAUTH_FAILURE          51
#define SSH_MSG_USERAUTH_SUCCESS          52
#define SSH_MSG_USERAUTH_BANNER           53
#define SSH_MSG_USERAUTH_PK_OK            60

#define SSH_MSG_GLOBAL_REQUEST            80
#define SSH_MSG_REQUEST_SUCCESS           81
#define SSH_MSG_REQUEST_FAILURE           82
#define SSH_MSG_CHANNEL_OPEN              90
#define SSH_MSG_CHANNEL_OPEN_CONFIRMATION 91
#define SSH_MSG_CHANNEL_OPEN_FAILURE      92
#define SSH_MSG_CHANNEL_WINDOW_ADJUST     93
#define SSH_MSG_CHANNEL_DATA              94
#define SSH_MSG_CHANNEL_EXTENDED_DATA     95
#define SSH_MSG_CHANNEL_EOF               96
#define SSH_MSG_CHANNEL_CLOSE             97
#define SSH_MSG_CHANNEL_REQUEST           98
#define SSH_MSG_CHANNEL_SUCCESS           99
#define SSH_MSG_CHANNEL_FAILURE          100

/* Disconnect reasons we actually send. */
#define SSH_DISCONNECT_PROTOCOL_ERROR              2
#define SSH_DISCONNECT_KEY_EXCHANGE_FAILED         3
#define SSH_DISCONNECT_MAC_ERROR                   5
#define SSH_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE 14

#define SSH_EXTENDED_DATA_STDERR 1

/* The largest payload either side will accept. RFC 4253 requires at
 * least 32768; anything larger is a peer trying to make us allocate. */
#define SSH_MAX_PAYLOAD 262144

/* ---- Ed25519 keys on the wire ---- */

#define SSH_ED25519_NAME "ssh-ed25519"

struct ssh_key {
    uint8_t pub[32];
    uint8_t priv[64]; /* seed || public, OpenSSH's layout */
    int have_priv;
};

/* "ssh-ed25519" || pub, the blob that appears in host key lists,
 * authorized_keys and known_hosts. */
void ssh_key_blob(const struct ssh_key *k, struct sshbuf *out);
int ssh_key_from_blob(struct ssh_key *k, const uint8_t *blob, size_t len);

void ssh_key_sign(const struct ssh_key *k, const void *data, size_t len,
                  struct sshbuf *out);
int ssh_key_verify(const struct ssh_key *k, const uint8_t *sig, size_t sig_len,
                   const void *data, size_t len);

/* The authorized_keys / known_hosts spelling: "ssh-ed25519 <base64>". */
int ssh_key_write_pub_line(const struct ssh_key *k, const char *comment,
                           char *out, size_t out_size);
int ssh_key_parse_pub_line(struct ssh_key *k, const char *line);

/* Private keys use OpenSSH's own container, unencrypted. */
int ssh_key_write_private_file(const struct ssh_key *k, const char *comment,
                               const char *path);
int ssh_key_read_private_file(struct ssh_key *k, const char *path);

/* ---- the connection ---- */

struct ssh {
    int fd;
    int server; /* 1 when this end is sshd */

    struct sshbuf in;  /* raw bytes read but not yet parsed  */
    struct sshbuf out; /* raw bytes built but not yet written */
    struct sshbuf pkt; /* the payload of the packet just read */

    char v_local[256];
    char v_peer[256];

    uint8_t session_id[32];
    int have_session_id;

    uint32_t seq_in, seq_out;

    /* chacha20-poly1305 keys: K_2 || K_1, 64 bytes each direction. */
    uint8_t key_out[64], key_in[64];
    int encrypt_out, decrypt_in;

    /* Both ends offered kex-strict-*-v00@openssh.com, so sequence
     * numbers reset at NEWKEYS and stray messages during KEX are
     * fatal. This is the Terrapin countermeasure. */
    int strict_kex;

    int eof;             /* the peer closed the connection */
    char err[256];       /* why the last call failed       */
};

void ssh_init(struct ssh *s, int fd, int server);
void ssh_close(struct ssh *s);

/* Records the message in s->err and returns -1, so callers can
 * `return ssh_fail(s, "...")`. */
int ssh_fail(struct ssh *s, const char *fmt, ...);

/* ---- packets ---- */

/* Append a packet holding `payload` to the output buffer. */
void ssh_packet_put(struct ssh *s, const struct sshbuf *payload);

/* Write everything buffered. Returns 0 or -1. */
int ssh_flush(struct ssh *s);

/* Build, buffer and flush in one step - what the handshake uses. */
int ssh_packet_send(struct ssh *s, const struct sshbuf *payload);

/* Pull one packet out of the bytes already buffered. Returns 1 with
 * s->pkt holding the payload (read cursor past the message type,
 * which is returned in *type), 0 if more bytes are needed, -1 on a
 * protocol error. */
int ssh_packet_next(struct ssh *s, uint8_t *type);

/* Read from the socket into the input buffer. Returns the byte count,
 * 0 at end of file, -1 on error. */
long ssh_read_more(struct ssh *s);

/* Block until a whole packet arrives. Transport housekeeping
 * (IGNORE, DEBUG, UNIMPLEMENTED) is swallowed; a DISCONNECT is
 * reported as an error with the peer's reason in s->err. */
int ssh_packet_recv(struct ssh *s, uint8_t *type);

int ssh_send_disconnect(struct ssh *s, uint32_t reason, const char *text);

/* ---- handshake ---- */

/*
 * Exchange identification strings. `software` goes into the comment
 * field, e.g. "TUS_1.0".
 */
int ssh_exchange_versions(struct ssh *s, const char *software);

/*
 * Run one key exchange. The server passes its host key in `hostkey`
 * (with the private half); the client passes NULL and gets the
 * server's key back through `verify`, which returns 0 to accept.
 */
int ssh_kex(struct ssh *s, const struct ssh_key *hostkey,
            int (*verify)(void *ctx, const struct ssh_key *k), void *ctx);

#endif /* TUS_SSH_H */
