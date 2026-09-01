	cmpbexadd dword [0xef3], ebp, edx
	cmpbexadd dword [0x1ba], edx, esi

%ifdef ERROR
	cmpbexadd qword [0xd38], rbx, rdx
	cmpbexadd qword [0x471], rdi, rdx
	cmpbexadd dword [0x20c], r13d, r12d
	cmpbexadd qword [0x395], r11, r9
	cmpbexadd dword [0xbfa], r25d, r20d
	cmpbexadd qword [0x7a4], r25, r19
	cmpbexadd qword [eax+1], rdx, rsi
	cmpbexadd qword [eax+64], rsi, rbx
	cmpbexadd [0x2ba], rax, rbx
%endif
