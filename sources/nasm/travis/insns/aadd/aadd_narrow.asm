	aadd dword [0xe1b], ebx
	aadd dword [0x7dc], ecx

%ifdef ERROR
	aadd qword [0xeca], rcx
	aadd qword [0x4e4], rcx
	aadd dword [0xbc9], r13d
	aadd qword [0xa2a], r10
	aadd dword [0xcad], r16d
	aadd qword [0x5c3], r17
	aadd qword [eax+1], rbx
	aadd qword [eax+64], rcx
	aadd [0x5d8], rdi
%endif
