	cmpaexadd dword [0x685], ebp, ebp
	cmpaexadd dword [0x2d2], ecx, ebx

%ifdef ERROR
	cmpaexadd qword [0x1b2], rax, rax
	cmpaexadd qword [0x572], rbx, rcx
	cmpaexadd dword [0xaa7], r14d, r9d
	cmpaexadd qword [0x452], r9, r10
	cmpaexadd dword [0xf06], r26d, r30d
	cmpaexadd qword [0x28c], r17, r27
	cmpaexadd qword [eax+1], rbp, rax
	cmpaexadd qword [eax+64], rax, rdi
	cmpaexadd [0xd44], rbx, rbx
%endif
