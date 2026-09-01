	cmovc cx, word [0x7cb]
	cmovc dx, di
	cmovc edx, edx
	cmovc esi, ecx
	cmovc cx, bx, ax
	cmovc cx, bp, di

%ifdef ERROR
	cmovc rsi, rsi
	cmovc rsi, qword [0xcf7]
	cmovc r11w, r8w
	cmovc r10d, r11d
	cmovc r14, r14
	cmovc r15w, r14w, r13w
	cmovc r24w, r30w
	cmovc r30d, r31d
	cmovc r19, r23
	cmovc r21w, r27w, r25w
	cmovc rax, qword [eax+1]
	cmovc rax, qword [eax+64]
	cmovc rdx, rdi, qword [eax+1]
	cmovc rcx, rbx, qword [eax+64]
	cmovc rsi, [0xeea]
	cmovc rcx, rdi, [0xd96]
%endif
