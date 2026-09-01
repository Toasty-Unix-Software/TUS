default rel
	cfcmovnz bx, si
	cfcmovnz word [0xf13], di
	cfcmovnz edx, edx
	cfcmovnz ecx, eax
	cfcmovnz rdx, rsi
	cfcmovnz rbp, rsi
	cfcmovnz di, bx
	cfcmovnz di, word [0x64a]
	cfcmovnz r13w, r8w
	cfcmovnz r10d, r12d
	cfcmovnz r12, r11
	cfcmovnz r12w, r14w
	cfcmovnz r20w, r19w
	cfcmovnz r22d, r17d
	cfcmovnz r31, r21
	cfcmovnz r30w, r21w
	cfcmovnz word [eax+1], bx
	cfcmovnz word [eax+64], bx
	cfcmovnz dword [eax+1], edi
	cfcmovnz dword [eax+64], edx
	cfcmovnz qword [eax+1], rdx
	cfcmovnz qword [eax+64], rbx
	cfcmovnz dx, word [eax+1]
	cfcmovnz cx, word [eax+64]
	cfcmovnz eax, dword [eax+1]
	cfcmovnz eax, dword [eax+64]
	cfcmovnz rsi, qword [eax+1]
	cfcmovnz rbx, qword [eax+64]
	cfcmovnz bp, si, word [eax+1]
	cfcmovnz cx, bp, word [eax+64]
	cfcmovnz eax, edi, dword [eax+1]
	cfcmovnz esi, edx, dword [eax+64]
	cfcmovnz rbp, rbp, qword [eax+1]
	cfcmovnz rbp, rsi, qword [eax+64]
	cfcmovnz [0xe27], bx
	cfcmovnz [0xcf2], edx
	cfcmovnz [0x1b7], rsi
	cfcmovnz ax, [0xd76]
	cfcmovnz eax, [0xc66]
	cfcmovnz rsi, [0x46b]
	cfcmovnz si, dx, [0xdde]
	cfcmovnz ebp, ebp, [0x22e]
	cfcmovnz rsi, rbp, [0xaa2]
