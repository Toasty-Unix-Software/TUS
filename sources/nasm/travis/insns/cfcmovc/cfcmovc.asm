default rel
	cfcmovc word [0x52f], cx
	cfcmovc di, bx
	cfcmovc ebp, esi
	cfcmovc esi, edi
	cfcmovc rbp, rax
	cfcmovc rbp, rsi
	cfcmovc ax, di
	cfcmovc dx, di
	cfcmovc r14w, r9w
	cfcmovc r10d, r10d
	cfcmovc r15, r13
	cfcmovc r15w, r15w
	cfcmovc r27w, r20w
	cfcmovc r26d, r21d
	cfcmovc r24, r30
	cfcmovc r23w, r19w
	cfcmovc word [eax+1], ax
	cfcmovc word [eax+64], bx
	cfcmovc dword [eax+1], ecx
	cfcmovc dword [eax+64], ecx
	cfcmovc qword [eax+1], rsi
	cfcmovc qword [eax+64], rsi
	cfcmovc dx, word [eax+1]
	cfcmovc bx, word [eax+64]
	cfcmovc esi, dword [eax+1]
	cfcmovc eax, dword [eax+64]
	cfcmovc rbp, qword [eax+1]
	cfcmovc rcx, qword [eax+64]
	cfcmovc bp, ax, word [eax+1]
	cfcmovc cx, bx, word [eax+64]
	cfcmovc ecx, edx, dword [eax+1]
	cfcmovc esi, eax, dword [eax+64]
	cfcmovc rdi, rbx, qword [eax+1]
	cfcmovc rbx, rbx, qword [eax+64]
	cfcmovc [0x69f], cx
	cfcmovc [0x536], edx
	cfcmovc [0xfe8], rdx
	cfcmovc cx, [0xcbf]
	cfcmovc edx, [0xeb7]
	cfcmovc rdi, [0x87f]
	cfcmovc si, bx, [0x425]
	cfcmovc ebx, ebp, [0x72f]
	cfcmovc rdi, rsi, [0x3a0]
