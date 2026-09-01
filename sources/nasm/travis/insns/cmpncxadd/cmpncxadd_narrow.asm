	cmpncxadd dword [0xcbc], edi, edx
	cmpncxadd dword [0x74b], esi, esi

%ifdef ERROR
	cmpncxadd qword [0x113], rbp, rdi
	cmpncxadd qword [0x617], rbp, rax
	cmpncxadd dword [0x4a0], r11d, r10d
	cmpncxadd qword [0x702], r8, r8
	cmpncxadd dword [0xc4c], r20d, r27d
	cmpncxadd qword [0x58f], r19, r31
	cmpncxadd qword [eax+1], rax, rdx
	cmpncxadd qword [eax+64], rdi, rbp
	cmpncxadd [0x4dc], rax, rbp
%endif
