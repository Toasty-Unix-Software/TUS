	cmpzxadd dword [0xdfe], ebx, esi
	cmpzxadd dword [0xb48], edx, eax

%ifdef ERROR
	cmpzxadd qword [0x9b0], rax, rbx
	cmpzxadd qword [0x6b4], rdx, rsi
	cmpzxadd dword [0x252], r15d, r15d
	cmpzxadd qword [0x31d], r8, r15
	cmpzxadd dword [0x10a], r31d, r31d
	cmpzxadd qword [0x4e6], r27, r26
	cmpzxadd qword [eax+1], rsi, rbx
	cmpzxadd qword [eax+64], rbp, rax
	cmpzxadd [0xc9a], rax, rbp
%endif
