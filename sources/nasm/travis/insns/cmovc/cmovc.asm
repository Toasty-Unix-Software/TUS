default rel
	cmovc cx, word [0x7cb]
	cmovc dx, di
	cmovc edx, edx
	cmovc esi, ecx
	cmovc rsi, rsi
	cmovc rsi, qword [0xcf7]
	cmovc cx, bx, ax
	cmovc cx, bp, di
	cmovc r11w, r8w
	cmovc r10d, r11d
	cmovc r14, r14
	cmovc r15w, r14w, r13w
	cmovc r24w, r30w
	cmovc r30d, r31d
	cmovc r19, r23
	cmovc r21w, r27w, r25w
	cmovc bp, word [eax+1]
	cmovc si, word [eax+64]
	cmovc ecx, dword [eax+1]
	cmovc edi, dword [eax+64]
	cmovc rax, qword [eax+1]
	cmovc rax, qword [eax+64]
	cmovc dx, bx, word [eax+1]
	cmovc bx, cx, word [eax+64]
	cmovc eax, edx, dword [eax+1]
	cmovc ecx, edi, dword [eax+64]
	cmovc rdx, rdi, qword [eax+1]
	cmovc rcx, rbx, qword [eax+64]
	cmovc ax, [0x451]
	cmovc esi, [0x11e]
	cmovc rsi, [0xeea]
	cmovc bx, si, [0x22e]
	cmovc ebp, edx, [0x61f]
	cmovc rcx, rdi, [0xd96]
