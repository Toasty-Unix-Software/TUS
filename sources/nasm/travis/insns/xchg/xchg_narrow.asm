	xchg ax, di
	xchg ax, cx
	xchg si, ax
	xchg si, ax

%ifdef ERROR
	xchg rax, rbx
	xchg rax, rdi
	xchg rbx, rax
	xchg rdx, rax
	xchg ax, r15w
	xchg rax, r14
	xchg r12w, ax
	xchg r12, rax
	xchg ax, r22w
	xchg rax, r28
	xchg r17w, ax
	xchg r20, rax
	xchg rax, qword [eax+1]
	xchg rax, qword [eax+64]
	xchg qword [eax+1], rax
	xchg qword [eax+64], rsi
	xchg rdi, [0xa95]
	xchg [0x927], rdx
%endif
