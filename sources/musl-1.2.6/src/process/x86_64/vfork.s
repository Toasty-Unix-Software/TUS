.global vfork
.type vfork,@function
vfork:
	# TUS has no vfork(2) (or `syscall` instruction at all - see
	# thread/x86_64/clone.s for the same issue on the __clone path).
	# tus_fork() (int $0x81) gives real, independent address spaces
	# rather than vfork's shared-until-exec semantics, but every
	# actual caller here only needs "child pid in parent, 0 in
	# child, or -errno" - fork() already satisfies that.
	int $0x81
	mov %eax,%edi
	.hidden __syscall_ret
	jmp __syscall_ret
