	cmpngexadd dword [0x5e8], ebp, ebx
	cmpngexadd dword [0x5ab], ebp, eax

%ifdef ERROR
	cmpngexadd qword [0x89d], rsi, rdi
	cmpngexadd qword [0xe61], rbx, rdi
	cmpngexadd dword [0x5f0], r9d, r10d
	cmpngexadd qword [0x77c], r9, r10
	cmpngexadd dword [0xf7b], r23d, r24d
	cmpngexadd qword [0x9c5], r20, r17
	cmpngexadd qword [eax+1], rbx, rcx
	cmpngexadd qword [eax+64], rbx, rbx
	cmpngexadd [0xda4], rsi, rbp
%endif
