	blcic edi, edx
	blcic edi, dword [0x5d5]

%ifdef ERROR
	blcic rcx, rdx
	blcic rsi, qword [0x243]
	blcic r11d, r13d
	blcic r11, r12
	blcic rdi, qword [eax+1]
	blcic rbx, qword [eax+64]
	blcic rcx, [0xea1]
%endif
