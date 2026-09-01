default rel
	blsi edi, edx
	blsi ecx, edi
	blsi rbp, qword [0x6bc]
	blsi rcx, rcx
	blsi r9d, r14d
	blsi r8, r10
	blsi r20d, r31d
	blsi r19, r31
	blsi ebp, dword [eax+1]
	blsi eax, dword [eax+64]
	blsi rcx, qword [eax+1]
	blsi rbx, qword [eax+64]
	blsi esi, [0x4eb]
	blsi rcx, [0x176]
