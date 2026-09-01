#!/bin/sh
#
# Run the TUS SSH implementation against the OpenSSH on this machine.
#
# Interoperability is the only property a protocol implementation
# cannot establish by testing against itself: two of your own programs
# will happily agree on the same misreading of the specification. So
# every check here has OpenSSH on one side of the connection.
#
# Nothing needs root. sshd runs in debug mode on a high port with a
# throwaway host key, which also means it serves exactly one
# connection - hence a fresh one before each client check.

WORK=$(mktemp -d)
cleanup() {
    kill $(jobs -p) 2>/dev/null
    rm -rf "$WORK"
}
trap cleanup EXIT

PORT=${PORT:-2321}
fail=0

need() {
    command -v "$1" >/dev/null 2>&1 || { echo "SKIP: no $1"; exit 0; }
}
need ssh
need ssh-keygen
need ssh-keyscan
[ -x /usr/sbin/sshd ] || { echo "SKIP: no sshd"; exit 0; }

check() {
    if [ "$2" = 0 ]; then
        echo "  [PASS] $1"
    else
        echo "  [FAIL] $1"
        fail=1
    fi
}

next_port() {
    PORT=$((PORT + 1))
}

# One sshd, one connection, our key file as the only way in.
start_sshd() {
    next_port
    /usr/sbin/sshd -d -p "$PORT" -h "$1" \
        -o "PidFile=$WORK/sshd.pid" \
        -o "AuthorizedKeysFile=$WORK/authorized_keys" \
        -o "PasswordAuthentication=no" \
        -o "StrictModes=no" > "$WORK/sshd.log" 2>&1 &
    sleep 1.2
}

# Our client trusts a host it has been told about, so tests do not
# depend on the accept-on-first-use path.
trust_host() {
    mkdir -p "$WORK/home/.ssh"
    printf '[127.0.0.1]:%s %s\n' "$PORT" \
        "$(cut -d' ' -f1,2 < "$1.pub")" >> "$WORK/home/.ssh/known_hosts"
}

tus_ssh() {
    HOME="$WORK/home" ./tus-ssh "$@"
}

blob_hex() { # base64 key blob on stdin -> hex of its trailing 32 bytes
    base64 -d 2>/dev/null | tail -c 32 | od -An -tx1 | tr -d ' \n'
}

ssh-keygen -q -t ed25519 -f "$WORK/theirs" -N '' -C 'openssh@test' || exit 1
ssh-keygen -q -t ed25519 -f "$WORK/other" -N '' || exit 1
ssh-keygen -q -t ed25519 -f "$WORK/id" -N '' || exit 1
cp "$WORK/id.pub" "$WORK/authorized_keys"

echo "key files against ssh-keygen"

./keytest read "$WORK/theirs" "$WORK/theirs.pub" >/dev/null 2>&1
check "we read a key ssh-keygen wrote" $?

./keytest write "$WORK/ours" "$WORK/ours.line" >/dev/null 2>&1
check "we write a key file" $?

if ssh-keygen -y -f "$WORK/ours" > "$WORK/ours.derived" 2>/dev/null; then
    [ "$(cut -d' ' -f1,2 < "$WORK/ours.line")" = \
      "$(cut -d' ' -f1,2 < "$WORK/ours.derived")" ]
    check "ssh-keygen reads the key file we wrote" $?
else
    check "ssh-keygen reads the key file we wrote" 1
fi

echo "TUS as the server"

next_port
./kexprobe server "$PORT" > "$WORK/server.log" 2>&1 &
sleep 1
ssh-keyscan -t ed25519 -p "$PORT" 127.0.0.1 > "$WORK/scan" 2>/dev/null
# ssh-keyscan also prints a "# host SSH-2.0-..." comment line.
scanned=$(awk '/ssh-ed25519/ {print $3; exit}' < "$WORK/scan" | blob_hex)
ours=$(awk '/our host key/ {print $4; exit}' < "$WORK/server.log")
[ -n "$scanned" ] && [ "$scanned" = "$ours" ]
check "ssh-keyscan retrieves our host key" $?

next_port
./kexprobe server "$PORT" > "$WORK/server2.log" 2>&1 &
sleep 1
ssh -p "$PORT" -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null \
    -o BatchMode=yes tester@127.0.0.1 true > "$WORK/ssh.log" 2>&1
grep -q "KEX OK" "$WORK/server2.log"
check "the OpenSSH client completes a key exchange with us" $?
grep -q "kexprobe has seen enough" "$WORK/ssh.log"
check "the OpenSSH client decrypts our first encrypted packet" $?

echo "TUS as the client: the transport"

start_sshd "$WORK/theirs"
./kexprobe client 127.0.0.1 "$PORT" > "$WORK/client.log" 2>&1
check "we complete a key exchange with the OpenSSH server" $?
grep -q "strict=1" "$WORK/client.log"
check "strict key exchange is negotiated" $?
theirs=$(cut -d' ' -f2 < "$WORK/theirs.pub" | blob_hex)
[ "$(awk '/server host key/ {print $4}' < "$WORK/client.log")" = "$theirs" ]
check "the host key we saw is the one sshd holds" $?

echo "TUS as the client: sessions"

start_sshd "$WORK/theirs"
trust_host "$WORK/theirs"
[ "$(tus_ssh -p "$PORT" -i "$WORK/id" pi@127.0.0.1 'echo hello' 2>/dev/null)" \
  = hello ]
check "public key authentication and a remote command" $?

start_sshd "$WORK/theirs"
trust_host "$WORK/theirs"
tus_ssh -p "$PORT" -i "$WORK/id" pi@127.0.0.1 'exit 7' >/dev/null 2>&1
[ $? = 7 ]
check "the remote exit status is our exit status" $?

start_sshd "$WORK/theirs"
trust_host "$WORK/theirs"
tus_ssh -p "$PORT" -i "$WORK/id" pi@127.0.0.1 \
    'head -c 2097152 /dev/urandom | tee /tmp/tus-interop-ref >/dev/null; \
     cat /tmp/tus-interop-ref' 2>/dev/null > "$WORK/down.bin"
[ "$(sha256sum < "$WORK/down.bin" | cut -d' ' -f1)" = \
  "$(sha256sum < /tmp/tus-interop-ref | cut -d' ' -f1)" ]
check "2 MiB downstream survives the window updates" $?
rm -f /tmp/tus-interop-ref

start_sshd "$WORK/theirs"
trust_host "$WORK/theirs"
head -c 300000 /dev/urandom > "$WORK/up.bin"
got=$(tus_ssh -p "$PORT" -i "$WORK/id" pi@127.0.0.1 'sha256sum | cut -d" " -f1' \
      < "$WORK/up.bin" 2>/dev/null | tr -d '\r\n')
[ "$got" = "$(sha256sum < "$WORK/up.bin" | cut -d' ' -f1)" ]
check "300 KiB upstream arrives intact" $?

if command -v script >/dev/null 2>&1; then
    start_sshd "$WORK/theirs"
    trust_host "$WORK/theirs"
    script -qec "HOME=$WORK/home ./tus-ssh -p $PORT -i $WORK/id -t \
        pi@127.0.0.1 'tty; echo pty-ok'" /dev/null > "$WORK/pty.log" 2>&1
    grep -q "pty-ok" "$WORK/pty.log" && grep -q "/dev/pts/" "$WORK/pty.log"
    check "a pty session gets a real terminal" $?
else
    echo "  [SKIP] pty session (no script(1))"
fi

# Input typed while the session is still starting belongs to the
# remote shell. A terminal put into raw mode with TCSAFLUSH discards
# exactly that input, and the symptom is keystrokes going missing
# under fast typing - so measure against OpenSSH rather than against
# an absolute number, which depends on the test user's prompt.
if command -v script >/dev/null 2>&1; then
    feed() { i=0; while [ $i -lt 200 ]; do printf '\n'; i=$((i + 1)); done
             sleep 6; }

    start_sshd "$WORK/theirs"
    trust_host "$WORK/theirs"
    feed | script -qec "HOME=$WORK/home ./tus-ssh -p $PORT -i $WORK/id \
        $(id -un)@127.0.0.1" /dev/null > "$WORK/typeahead.ours" 2>&1
    ours_bytes=$(wc -c < "$WORK/typeahead.ours")

    start_sshd "$WORK/theirs"
    feed | script -qec "ssh -p $PORT -i $WORK/id -o StrictHostKeyChecking=no \
        -o UserKnownHostsFile=$WORK/home/.ssh/known_hosts \
        $(id -un)@127.0.0.1" /dev/null > "$WORK/typeahead.theirs" 2>&1
    theirs_bytes=$(wc -c < "$WORK/typeahead.theirs")

    # 80% of OpenSSH's output: the login banners differ a little, the
    # 200 prompts must not.
    [ "$theirs_bytes" -gt 0 ] &&
        [ $((ours_bytes * 100 / theirs_bytes)) -ge 80 ]
    check "typed-ahead input is not discarded ($ours_bytes vs $theirs_bytes)" $?
else
    echo "  [SKIP] typed-ahead input (no script(1))"
fi

# The host key changing is the one case where continuing would be
# worse than failing, so it must fail even though the key is valid.
start_sshd "$WORK/other"
trust_host "$WORK/theirs"
tus_ssh -p "$PORT" -i "$WORK/id" pi@127.0.0.1 'echo reached' \
    > "$WORK/mismatch.log" 2>&1
rc=$?
[ "$rc" != 0 ] && ! grep -q reached "$WORK/mismatch.log"
check "a changed host key is refused" $?

exit $fail
