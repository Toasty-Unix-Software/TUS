default rel
	cmovp cx, bp
	cmovp bp, ax
	cmovp ebp, esi
	cmovp ebp, ebx
	cmovp rcx, rcx
	cmovp rdi, qword [0x24f]
	cmovp bp, di, si
	cmovp ax, bp, di
	cmovp r15w, r8w
	cmovp r14d, r15d
	cmovp r9, r8
	cmovp r15w, r10w, r14w
	cmovp r21w, r17w
	cmovp r22d, r18d
	cmovp r20, r19
	cmovp r25w, r25w, r27w
	cmovp di, word [eax+1]
	cmovp si, word [eax+64]
	cmovp edx, dword [eax+1]
	cmovp ecx, dword [eax+64]
	cmovp rax, qword [eax+1]
	cmovp rcx, qword [eax+64]
	cmovp bx, ax, word [eax+1]
	cmovp si, ax, word [eax+64]
	cmovp ecx, ebp, dword [eax+1]
	cmovp eax, edx, dword [eax+64]
	cmovp rbx, rsi, qword [eax+1]
	cmovp rcx, rcx, qword [eax+64]
	cmovp si, [0xc97]
	cmovp esi, [0x49e]
	cmovp rdi, [0x207]
	cmovp cx, ax, [0xa46]
	cmovp ebp, ecx, [0x961]
	cmovp rbx, rbp, [0x8e2]
