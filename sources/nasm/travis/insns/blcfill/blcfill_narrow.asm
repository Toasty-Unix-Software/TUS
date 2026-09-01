	blcfill edx, ebp
	blcfill ebx, dword [0xe98]

%ifdef ERROR
	blcfill rax, rsi
	blcfill rcx, rbp
	blcfill r15d, r10d
	blcfill r8, r14
	blcfill rsi, qword [eax+1]
	blcfill rbp, qword [eax+64]
	blcfill rdx, [0x452]
%endif
