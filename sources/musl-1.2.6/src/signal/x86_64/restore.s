	nop
.global __restore_rt
.hidden __restore_rt
.type __restore_rt,@function
__restore_rt:
	/* TUS has no `syscall` instruction wired up (the ABI is `int
	 * $0x80`/$0x81/$0x82, see kernel/syscall/syscall.h) - the real
	 * Linux restorer traps to SYS_rt_sigreturn (15) this way, TUS's
	 * traps straight to sigreturn's own dedicated IDT vector, the
	 * same reason fork() has its own (int $0x81) instead of going
	 * through the generic syscall dispatch: it needs the full
	 * register capture syscall_entry()'s narrow 7-register gate has
	 * no room for. See sigreturn_entry() in kernel/sched/sched.c. */
	int $0x82
.size __restore_rt,.-__restore_rt
