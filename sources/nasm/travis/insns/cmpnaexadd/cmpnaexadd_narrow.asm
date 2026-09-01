	cmpnaexadd dword [0x8aa], ebx, ecx
	cmpnaexadd dword [0xeb9], edx, ecx

%ifdef ERROR
	cmpnaexadd qword [0x881], rbp, rdx
	cmpnaexadd qword [0xd1e], rsi, rcx
	cmpnaexadd dword [0x211], r15d, r8d
	cmpnaexadd qword [0x6f5], r15, r15
	cmpnaexadd dword [0x962], r19d, r18d
	cmpnaexadd qword [0xc8b], r22, r26
	cmpnaexadd qword [eax+1], rax, rsi
	cmpnaexadd qword [eax+64], rdi, rbp
	cmpnaexadd [0x98b], rsi, rbp
%endif
