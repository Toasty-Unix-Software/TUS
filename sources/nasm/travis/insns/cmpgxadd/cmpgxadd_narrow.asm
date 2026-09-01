	cmpgxadd dword [0x6b7], eax, eax
	cmpgxadd dword [0xb6a], ebp, esi

%ifdef ERROR
	cmpgxadd qword [0x5b6], rbp, rcx
	cmpgxadd qword [0x3d0], rdi, rsi
	cmpgxadd dword [0xcb2], r15d, r13d
	cmpgxadd qword [0xa93], r10, r11
	cmpgxadd dword [0xcf6], r29d, r28d
	cmpgxadd qword [0xcf1], r20, r28
	cmpgxadd qword [eax+1], rbp, rax
	cmpgxadd qword [eax+64], rsi, rdi
	cmpgxadd [0x311], rax, rdx
%endif
