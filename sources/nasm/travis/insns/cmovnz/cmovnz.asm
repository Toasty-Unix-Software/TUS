default rel
	cmovnz cx, cx
	cmovnz ax, word [0x582]
	cmovnz eax, edx
	cmovnz ecx, eax
	cmovnz rcx, rbx
	cmovnz rcx, rax
	cmovnz bp, cx, cx
	cmovnz ax, di, dx
	cmovnz r12w, r12w
	cmovnz r12d, r15d
	cmovnz r15, r8
	cmovnz r10w, r8w, r11w
	cmovnz r25w, r20w
	cmovnz r26d, r22d
	cmovnz r28, r17
	cmovnz r22w, r27w, r23w
	cmovnz di, word [eax+1]
	cmovnz si, word [eax+64]
	cmovnz eax, dword [eax+1]
	cmovnz ebp, dword [eax+64]
	cmovnz rsi, qword [eax+1]
	cmovnz rdx, qword [eax+64]
	cmovnz dx, bx, word [eax+1]
	cmovnz bx, cx, word [eax+64]
	cmovnz esi, eax, dword [eax+1]
	cmovnz ebp, ebp, dword [eax+64]
	cmovnz rbx, rcx, qword [eax+1]
	cmovnz rbx, rdi, qword [eax+64]
	cmovnz dx, [0x8a9]
	cmovnz esi, [0xef1]
	cmovnz rsi, [0xc75]
	cmovnz bp, ax, [0xbf7]
	cmovnz edx, ebp, [0x8ed]
	cmovnz rsi, rdi, [0x320]
