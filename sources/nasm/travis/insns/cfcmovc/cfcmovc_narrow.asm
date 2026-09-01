	cfcmovc word [0x52f], cx
	cfcmovc di, bx
	cfcmovc ebp, esi
	cfcmovc esi, edi
	cfcmovc ax, di
	cfcmovc dx, di

%ifdef ERROR
	cfcmovc rbp, rax
	cfcmovc rbp, rsi
	cfcmovc r14w, r9w
	cfcmovc r10d, r10d
	cfcmovc r15, r13
	cfcmovc r15w, r15w
	cfcmovc r27w, r20w
	cfcmovc r26d, r21d
	cfcmovc r24, r30
	cfcmovc r23w, r19w
	cfcmovc qword [eax+1], rsi
	cfcmovc qword [eax+64], rsi
	cfcmovc rbp, qword [eax+1]
	cfcmovc rcx, qword [eax+64]
	cfcmovc rdi, rbx, qword [eax+1]
	cfcmovc rbx, rbx, qword [eax+64]
	cfcmovc [0xfe8], rdx
	cfcmovc rdi, [0x87f]
	cfcmovc rdi, rsi, [0x3a0]
%endif
