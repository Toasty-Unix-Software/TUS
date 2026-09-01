default rel
	cmovna bx, ax
	cmovna cx, dx
	cmovna esi, ebx
	cmovna eax, edi
	cmovna rdi, qword [0xa48]
	cmovna rbp, rax
	cmovna si, dx, bp
	cmovna cx, di, bx
	cmovna r12w, r10w
	cmovna r10d, r11d
	cmovna r13, r15
	cmovna r14w, r15w, r13w
	cmovna r24w, r17w
	cmovna r19d, r21d
	cmovna r28, r23
	cmovna r31w, r19w, r31w
	cmovna si, word [eax+1]
	cmovna bp, word [eax+64]
	cmovna eax, dword [eax+1]
	cmovna ecx, dword [eax+64]
	cmovna rax, qword [eax+1]
	cmovna rsi, qword [eax+64]
	cmovna bx, ax, word [eax+1]
	cmovna bx, ax, word [eax+64]
	cmovna ebp, ecx, dword [eax+1]
	cmovna ecx, ebp, dword [eax+64]
	cmovna rbx, rdx, qword [eax+1]
	cmovna rbp, rbp, qword [eax+64]
	cmovna cx, [0x532]
	cmovna esi, [0x1a0]
	cmovna rcx, [0x2a2]
	cmovna di, cx, [0x377]
	cmovna ebp, ecx, [0x6b7]
	cmovna rcx, rdx, [0x412]
