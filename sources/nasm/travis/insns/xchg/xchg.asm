default rel
	xchg ax, di
	xchg ax, cx
	xchg rax, rbx
	xchg rax, rdi
	xchg si, ax
	xchg si, ax
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
	xchg cl, byte [eax+1]
	xchg cl, byte [eax+64]
	xchg si, word [eax+1]
	xchg si, word [eax+64]
	xchg eax, dword [eax+1]
	xchg esi, dword [eax+64]
	xchg rax, qword [eax+1]
	xchg rax, qword [eax+64]
	xchg byte [eax+1], bl
	xchg byte [eax+64], cl
	xchg word [eax+1], bx
	xchg word [eax+64], di
	xchg dword [eax+1], ebp
	xchg dword [eax+64], ecx
	xchg qword [eax+1], rax
	xchg qword [eax+64], rsi
	xchg cl, [0xd42]
	xchg bx, [0xd0d]
	xchg esi, [0x4f7]
	xchg rdi, [0xa95]
	xchg [0xa4e], bl
	xchg [0x1f6], si
	xchg [0x4f1], edx
	xchg [0x927], rdx
