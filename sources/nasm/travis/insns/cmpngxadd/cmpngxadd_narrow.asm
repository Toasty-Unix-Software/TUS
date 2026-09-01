	cmpngxadd dword [0x457], edx, eax
	cmpngxadd dword [0x10e], eax, esi

%ifdef ERROR
	cmpngxadd qword [0xb29], rax, rdi
	cmpngxadd qword [0xcd5], rdi, rdi
	cmpngxadd dword [0x651], r12d, r14d
	cmpngxadd qword [0x677], r15, r9
	cmpngxadd dword [0x695], r19d, r24d
	cmpngxadd qword [0x5e4], r27, r28
	cmpngxadd qword [eax+1], rdi, rdx
	cmpngxadd qword [eax+64], rcx, rcx
	cmpngxadd [0x25b], rbp, rsi
%endif
