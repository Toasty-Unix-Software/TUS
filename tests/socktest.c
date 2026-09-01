/*
 * socktest.c - Unix domain socket, poll() and select() test
 *
 * A ring-3 musl program that exercises the AF_UNIX/SOCK_STREAM
 * implementation (kernel/net/socket.c) and the I/O multiplexing
 * syscalls (SYS_POLL, SYS_SELECT) end to end.
 *
 * Everything runs in ONE process, which is possible because connect()
 * completes as soon as the connection is queued on the listener's
 * backlog - exactly as on Linux - so the same task can connect to its
 * own listener and then accept it. TUS has no fork().
 *
 * Every check prints "ok: <what>" and the program ends with
 * "socktest: all good"; test_boot.py greps for those lines.
 */

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/tus.sock"

static int failures;

static void check(int cond, const char *what) {
    if (cond) {
        printf("ok: %s\n", what);
    } else {
        printf("FAIL: %s (errno %d)\n", what, errno);
        failures++;
    }
}

/* Fill a sockaddr_un for SOCK_PATH; returns the length to pass. */
static socklen_t addr_for(struct sockaddr_un *sa, const char *path) {
    memset(sa, 0, sizeof(*sa));
    sa->sun_family = AF_UNIX;
    strcpy(sa->sun_path, path);
    return (socklen_t)(sizeof(sa->sun_family) + strlen(path) + 1);
}

/* ---- 1. socketpair: an unnamed, bidirectional channel ---- */

static void test_socketpair(void) {
    int sv[2];
    char buf[32];

    check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair created");

    check(write(sv[0], "ping", 4) == 4, "socketpair write a->b");
    memset(buf, 0, sizeof(buf));
    check(read(sv[1], buf, sizeof(buf)) == 4 && memcmp(buf, "ping", 4) == 0,
          "socketpair read b sees 'ping'");

    /* The reverse direction uses the other ring buffer: a pipe could
     * not do this. */
    check(write(sv[1], "pong", 4) == 4, "socketpair write b->a");
    memset(buf, 0, sizeof(buf));
    check(read(sv[0], buf, sizeof(buf)) == 4 && memcmp(buf, "pong", 4) == 0,
          "socketpair is bidirectional (a sees 'pong')");

    /* Closing one end must give the other end EOF, not a hang. */
    close(sv[1]);
    check(read(sv[0], buf, sizeof(buf)) == 0, "closed peer reads as EOF");
    close(sv[0]);
}

/* ---- 2. bind / listen / connect / accept ---- */

static void test_named_socket(void) {
    struct sockaddr_un sa;
    socklen_t len;
    char buf[32];

    unlink(SOCK_PATH); /* a socket node outlives its socket, like UNIX */

    int srv = socket(AF_UNIX, SOCK_STREAM, 0);
    check(srv >= 0, "socket(AF_UNIX, SOCK_STREAM)");

    len = addr_for(&sa, SOCK_PATH);
    check(bind(srv, (struct sockaddr *)&sa, len) == 0, "bind " SOCK_PATH);
    check(listen(srv, 4) == 0, "listen");

    /* getsockname reports the path we bound. */
    struct sockaddr_un back;
    socklen_t blen = sizeof(back);
    memset(&back, 0, sizeof(back));
    check(getsockname(srv, (struct sockaddr *)&back, &blen) == 0 &&
              strcmp(back.sun_path, SOCK_PATH) == 0,
          "getsockname returns the bound path");

    /* Binding the same path twice must fail: the node already exists. */
    int dup_srv = socket(AF_UNIX, SOCK_STREAM, 0);
    check(bind(dup_srv, (struct sockaddr *)&sa, len) < 0 && errno == EADDRINUSE,
          "second bind to the same path fails with EADDRINUSE");
    close(dup_srv);

    /* connect() returns once queued; accept() then pops the queue. */
    int cli = socket(AF_UNIX, SOCK_STREAM, 0);
    check(connect(cli, (struct sockaddr *)&sa, len) == 0, "connect");

    int acc = accept(srv, NULL, NULL);
    check(acc >= 0, "accept");

    check(send(cli, "hello server", 12, 0) == 12, "send from client");
    memset(buf, 0, sizeof(buf));
    check(recv(acc, buf, sizeof(buf), 0) == 12 &&
              memcmp(buf, "hello server", 12) == 0,
          "server recv sees 'hello server'");

    check(send(acc, "hello client", 12, 0) == 12, "send from server");
    memset(buf, 0, sizeof(buf));
    check(recv(cli, buf, sizeof(buf), 0) == 12 &&
              memcmp(buf, "hello client", 12) == 0,
          "client recv sees 'hello client'");

    /* shutdown(SHUT_WR) is a half close: the peer reads EOF. */
    check(shutdown(acc, SHUT_WR) == 0, "shutdown(SHUT_WR)");
    check(read(cli, buf, sizeof(buf)) == 0, "half close gives the peer EOF");

    close(acc);
    close(cli);
    close(srv);

    /* The listener is gone but the node is not: connecting now must be
     * refused rather than hang. */
    int late = socket(AF_UNIX, SOCK_STREAM, 0);
    check(connect(late, (struct sockaddr *)&sa, len) < 0 &&
              errno == ECONNREFUSED,
          "connect to a stale socket path is refused");
    close(late);

    check(unlink(SOCK_PATH) == 0, "unlink removes the socket node");
}

/* ---- 3. poll() ---- */

static void test_poll(void) {
    int sv[2];
    struct pollfd pf[2];
    char buf[32];

    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

    /* Nothing written yet: readable must NOT be reported, and the
     * zero timeout must return immediately. */
    pf[0].fd = sv[0];
    pf[0].events = POLLIN;
    pf[0].revents = 0;
    check(poll(pf, 1, 0) == 0, "poll: idle socket is not readable");

    /* An empty socket IS writable. */
    pf[0].events = POLLOUT;
    pf[0].revents = 0;
    check(poll(pf, 1, 0) == 1 && (pf[0].revents & POLLOUT),
          "poll: empty socket is writable");

    write(sv[1], "x", 1);
    pf[0].events = POLLIN;
    pf[0].revents = 0;
    check(poll(pf, 1, 0) == 1 && (pf[0].revents & POLLIN),
          "poll: socket with data is readable");
    read(sv[0], buf, sizeof(buf));

    /* A timeout on an idle fd must actually elapse and return 0. */
    pf[0].events = POLLIN;
    pf[0].revents = 0;
    check(poll(pf, 1, 150) == 0, "poll: times out on an idle socket");

    /* A closed peer reports hang-up. */
    close(sv[1]);
    pf[0].events = POLLIN;
    pf[0].revents = 0;
    check(poll(pf, 1, 0) == 1 && (pf[0].revents & (POLLIN | POLLHUP)),
          "poll: closed peer reports POLLIN/POLLHUP");
    close(sv[0]);

    /* poll() also works on pipes, which is what makes it useful for
     * multiplexing a shell pipeline. */
    int p[2];
    check(pipe(p) == 0, "pipe created");
    pf[0].fd = p[0];
    pf[0].events = POLLIN;
    pf[0].revents = 0;
    check(poll(pf, 1, 0) == 0, "poll: empty pipe is not readable");
    write(p[1], "y", 1);
    pf[0].revents = 0;
    check(poll(pf, 1, 0) == 1 && (pf[0].revents & POLLIN),
          "poll: pipe with data is readable");
    read(p[0], buf, sizeof(buf));
    close(p[1]);
    pf[0].revents = 0;
    check(poll(pf, 1, 0) == 1 && (pf[0].revents & POLLHUP),
          "poll: pipe with no writers reports POLLHUP");
    close(p[0]);

    /* A negative fd is skipped, not an error. */
    pf[0].fd = -1;
    pf[0].events = POLLIN;
    pf[0].revents = 0xff;
    check(poll(pf, 1, 0) == 0 && pf[0].revents == 0,
          "poll: negative fd is ignored");
}

/* ---- 4. select() ---- */

static void test_select(void) {
    int sv[2];
    fd_set rfds, wfds;
    struct timeval tv;
    char buf[32];

    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

    /* Idle socket, zero timeout: no fd ready. */
    FD_ZERO(&rfds);
    FD_SET(sv[0], &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    check(select(sv[0] + 1, &rfds, NULL, NULL, &tv) == 0,
          "select: idle socket is not readable");
    check(!FD_ISSET(sv[0], &rfds), "select: clears the set on timeout");

    /* Writable is reported straight away. */
    FD_ZERO(&wfds);
    FD_SET(sv[0], &wfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    check(select(sv[0] + 1, NULL, &wfds, NULL, &tv) == 1 &&
              FD_ISSET(sv[0], &wfds),
          "select: empty socket is writable");

    /* Data makes it readable. */
    write(sv[1], "hi", 2);
    FD_ZERO(&rfds);
    FD_SET(sv[0], &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    check(select(sv[0] + 1, &rfds, NULL, NULL, &tv) == 1 &&
              FD_ISSET(sv[0], &rfds),
          "select: socket with data is readable");
    read(sv[0], buf, sizeof(buf));

    /* Watching two fds at once: only the one with data comes back. */
    FD_ZERO(&rfds);
    FD_SET(sv[0], &rfds);
    FD_SET(sv[1], &rfds);
    write(sv[0], "z", 1); /* readable on sv[1], not sv[0] */
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int n = select((sv[0] > sv[1] ? sv[0] : sv[1]) + 1, &rfds, NULL, NULL, &tv);
    check(n == 1 && FD_ISSET(sv[1], &rfds) && !FD_ISSET(sv[0], &rfds),
          "select: reports only the ready fd of two");
    read(sv[1], buf, sizeof(buf));

    /* A real timeout must elapse rather than return instantly. */
    FD_ZERO(&rfds);
    FD_SET(sv[0], &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 150000;
    check(select(sv[0] + 1, &rfds, NULL, NULL, &tv) == 0,
          "select: times out on an idle socket");

    close(sv[0]);
    close(sv[1]);

    /* A closed fd in the set is an error, not a silent skip. */
    FD_ZERO(&rfds);
    FD_SET(sv[0], &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    check(select(sv[0] + 1, &rfds, NULL, NULL, &tv) < 0 && errno == EBADF,
          "select: closed fd fails with EBADF");
}

int main(void) {
    printf("socktest: AF_UNIX sockets, poll and select\n");

    test_socketpair();
    test_named_socket();
    test_poll();
    test_select();

    if (failures == 0) {
        printf("socktest: all good\n");
        return 0;
    }
    printf("socktest: %d failure(s)\n", failures);
    return 1;
}
