default rel
	cmovo di, cx
	cmovo bp, si
	cmovo esi, edx
	cmovo esi, eax
	cmovo rcx, qword [0xaf8]
	cmovo rcx, qword [0x64c]
	cmovo si, si, word [0x925]
	cmovo ax, bx, di
	cmovo r10w, r10w
	cmovo r14d, r12d
	cmovo r14, r13
	cmovo r9w, r8w, r10w
	cmovo r16w, r24w
	cmovo r31d, r30d
	cmovo r19, r19
	cmovo r22w, r25w, r26w
	cmovo cx, word [eax+1]
	cmovo ax, word [eax+64]
	cmovo ebp, dword [eax+1]
	cmovo esi, dword [eax+64]
	cmovo rdi, qword [eax+1]
	cmovo rcx, qword [eax+64]
	cmovo cx, dx, word [eax+1]
	cmovo ax, dx, word [eax+64]
	cmovo ebp, ebx, dword [eax+1]
	cmovo esi, ecx, dword [eax+64]
	cmovo rbx, rdx, qword [eax+1]
	cmovo rdx, rbx, qword [eax+64]
	cmovo cx, [0x9bf]
	cmovo esi, [0xa83]
	cmovo rsi, [0xc69]
	cmovo ax, si, [0x55e]
	cmovo edi, esi, [0xed8]
	cmovo rbp, rdx, [0xf46]
