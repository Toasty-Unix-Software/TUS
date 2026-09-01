	ror byte [0x4d6], 1
	ror cl, 1
	ror dx, 1
	ror word [0x9ce], 1
	ror dword [0xac7], 1
	ror ebp, 1
	ror qword [0xd84], 1
	ror rbp, 1

%ifdef ERROR
	ror r12b, 1
	ror r11w, 1
	ror r13d, 1
	ror r14, 1
	ror r27b, 1
	ror r17w, 1
	ror r25d, 1
	ror r30, 1
	ror rdx, qword [eax+1], 0xbd
	ror rbx, qword [eax+64], 0x49
	ror rax, qword [eax+1], 1
	ror rbx, qword [eax+64], 1
	ror rsi, qword [eax+1], cl
	ror rdi, qword [eax+64], cl
	ror rdi, qword [eax+1], cx
	ror rdi, qword [eax+64], cx
	ror rbx, qword [eax+1], ecx
	ror rdx, qword [eax+64], ecx
	ror rbp, qword [eax+1], 0x76
	ror rsi, qword [eax+64], 0x5f
	ror rdi, [0x78b], 0xa5
	ror rdx, [0x5ed], 1
	ror rdi, [0x2a9], cl
	ror rbx, [0x363], cx
	ror rbp, [0x749], ecx
	ror rcx, [0xaef], 0xdd
%endif
