	cmovp cx, bp
	cmovp bp, ax
	cmovp ebp, esi
	cmovp ebp, ebx
	cmovp bp, di, si
	cmovp ax, bp, di

%ifdef ERROR
	cmovp rcx, rcx
	cmovp rdi, qword [0x24f]
	cmovp r15w, r8w
	cmovp r14d, r15d
	cmovp r9, r8
	cmovp r15w, r10w, r14w
	cmovp r21w, r17w
	cmovp r22d, r18d
	cmovp r20, r19
	cmovp r25w, r25w, r27w
	cmovp rax, qword [eax+1]
	cmovp rcx, qword [eax+64]
	cmovp rbx, rsi, qword [eax+1]
	cmovp rcx, rcx, qword [eax+64]
	cmovp rdi, [0x207]
	cmovp rbx, rbp, [0x8e2]
%endif
