	cmpnbexadd dword [0x34d], edx, edi
	cmpnbexadd dword [0x1c2], ecx, esi

%ifdef ERROR
	cmpnbexadd qword [0x7c4], rdx, rdx
	cmpnbexadd qword [0xcce], rbx, rdi
	cmpnbexadd dword [0x9ff], r8d, r15d
	cmpnbexadd qword [0x84d], r8, r15
	cmpnbexadd dword [0xf32], r19d, r26d
	cmpnbexadd qword [0x4f2], r30, r29
	cmpnbexadd qword [eax+1], rbx, rax
	cmpnbexadd qword [eax+64], rbp, rax
	cmpnbexadd [0x2ec], rcx, rbp
%endif
