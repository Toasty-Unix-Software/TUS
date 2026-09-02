.text
.global __clone
.hidden __clone
.type   __clone,@function
__clone:
	# __clone(func, stack, flags, arg, ptid, tls, ctid)
	#          rdi   rsi    edx   rcx  r8    r9   [rsp+8]
	#
	# Upstream musl asks the Linux kernel for a real clone(2) with a
	# separate (usually CLONE_VM) child stack via the raw `syscall`
	# instruction. TUS has neither: it has no clone(2) at all, and
	# `syscall` itself is unimplemented (TUS traps through `int
	# $0x80`/`$0x81` - see arch/x86_64/syscall_arch.h), so this used
	# to fault with Invalid Opcode the moment anything (posix_spawn,
	# by way of pthread-style clone) reached it - e.g. ksh running
	# any external command.
	#
	# The only contract callers here actually rely on (posix_spawn.c,
	# pthread_create.c) is "child runs func(arg) and does not return
	# to the caller; parent gets the child's pid (or -errno)". That
	# is exactly fork()'s contract too, so emulate with tus_fork()
	# (int $0x81) and skip the manual child-stack switch real clone()
	# needs - fork() already gives the child its own full copy of
	# THIS stack, so the caller's `stack` argument goes unused.
	push %r12
	push %r13
	mov  %rdi,%r12          # save func (r12/r13 are callee-saved:
	mov  %rcx,%r13          # int $0x81 clobbers rcx/r11, not these)
	int  $0x81              # tus_fork(): eax = 0 in child, pid/-errno in parent
	test %eax,%eax
	jnz  1f
	# --- child: never returns to our caller ---
	xor  %ebp,%ebp
	mov  %r13,%rdi
	call *%r12
	mov  %eax,%edi          # exit code
	xor  %eax,%eax          # TUS_SYS_EXIT = 0 (kernel/syscall/syscall.h)
	int  $0x80
	hlt                      # unreachable
1:	# --- parent (or fork error) ---
	pop  %r13
	pop  %r12
	ret
