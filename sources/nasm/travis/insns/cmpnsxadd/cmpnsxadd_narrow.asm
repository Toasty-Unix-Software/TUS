	cmpnsxadd dword [0x231], ebx, edx
	cmpnsxadd dword [0x598], ebp, eax

%ifdef ERROR
	cmpnsxadd qword [0x257], rsi, rcx
	cmpnsxadd qword [0xbbe], rdx, rdx
	cmpnsxadd dword [0x791], r14d, r15d
	cmpnsxadd qword [0x669], r10, r10
	cmpnsxadd dword [0xe56], r16d, r26d
	cmpnsxadd qword [0x733], r19, r19
	cmpnsxadd qword [eax+1], rdx, rdi
	cmpnsxadd qword [eax+64], rbx, rdx
	cmpnsxadd [0x507], rdx, rbx
%endif
