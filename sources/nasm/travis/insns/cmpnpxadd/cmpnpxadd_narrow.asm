	cmpnpxadd dword [0x533], edx, edx
	cmpnpxadd dword [0x4ef], esi, ebp

%ifdef ERROR
	cmpnpxadd qword [0xcf8], rdi, rbp
	cmpnpxadd qword [0x3a7], rax, rsi
	cmpnpxadd dword [0xd24], r11d, r8d
	cmpnpxadd qword [0x61d], r12, r11
	cmpnpxadd dword [0xfd0], r17d, r23d
	cmpnpxadd qword [0xe5d], r22, r29
	cmpnpxadd qword [eax+1], rcx, rdi
	cmpnpxadd qword [eax+64], rbp, rsi
	cmpnpxadd [0x569], rdi, rbp
%endif
