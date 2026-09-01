	cmpnaxadd dword [0xbd5], ebp, eax
	cmpnaxadd dword [0xd2e], eax, ebx

%ifdef ERROR
	cmpnaxadd qword [0x8c0], rdi, rdi
	cmpnaxadd qword [0xbbb], rsi, rbp
	cmpnaxadd dword [0xb0f], r10d, r13d
	cmpnaxadd qword [0x2d9], r10, r12
	cmpnaxadd dword [0xb97], r21d, r25d
	cmpnaxadd qword [0xf6b], r26, r23
	cmpnaxadd qword [eax+1], rbx, rdx
	cmpnaxadd qword [eax+64], rbx, rcx
	cmpnaxadd [0xa73], rbx, rax
%endif
