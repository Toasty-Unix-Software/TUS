default rel
	blcfill edx, ebp
	blcfill ebx, dword [0xe98]
	blcfill rax, rsi
	blcfill rcx, rbp
	blcfill r15d, r10d
	blcfill r8, r14
	blcfill esi, dword [eax+1]
	blcfill ebp, dword [eax+64]
	blcfill rsi, qword [eax+1]
	blcfill rbp, qword [eax+64]
	blcfill edx, [0x3cc]
	blcfill rdx, [0x452]
