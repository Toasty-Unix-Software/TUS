	cmpaxadd dword [0x2ec], eax, ebx
	cmpaxadd dword [0xe94], eax, esi

%ifdef ERROR
	cmpaxadd qword [0xcaa], rbx, rax
	cmpaxadd qword [0x81d], rbp, rbp
	cmpaxadd dword [0x2e2], r15d, r13d
	cmpaxadd qword [0x696], r10, r12
	cmpaxadd dword [0x9f6], r19d, r23d
	cmpaxadd qword [0x285], r24, r19
	cmpaxadd qword [eax+1], rdx, rcx
	cmpaxadd qword [eax+64], rsi, rbx
	cmpaxadd [0x19f], rbx, rdi
%endif
