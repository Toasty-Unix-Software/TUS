default rel
	cmovnl di, si
	cmovnl di, dx
	cmovnl edi, eax
	cmovnl esi, edx
	cmovnl rdx, qword [0x9d3]
	cmovnl rdi, rcx
	cmovnl di, di, word [0x436]
	cmovnl si, bx, bp
	cmovnl r8w, r11w
	cmovnl r14d, r13d
	cmovnl r12, r9
	cmovnl r12w, r13w, r8w
	cmovnl r31w, r24w
	cmovnl r21d, r18d
	cmovnl r31, r27
	cmovnl r22w, r29w, r29w
	cmovnl ax, word [eax+1]
	cmovnl di, word [eax+64]
	cmovnl ebx, dword [eax+1]
	cmovnl edx, dword [eax+64]
	cmovnl rcx, qword [eax+1]
	cmovnl rcx, qword [eax+64]
	cmovnl si, ax, word [eax+1]
	cmovnl bp, dx, word [eax+64]
	cmovnl eax, ebp, dword [eax+1]
	cmovnl ecx, ebx, dword [eax+64]
	cmovnl rcx, rbx, qword [eax+1]
	cmovnl rsi, rbx, qword [eax+64]
	cmovnl ax, [0xd4a]
	cmovnl ebx, [0x4d2]
	cmovnl rbp, [0xe1f]
	cmovnl ax, bp, [0xf50]
	cmovnl edx, edx, [0x3dd]
	cmovnl rdx, rax, [0x2cf]
