	cmpgexadd dword [0x8b5], edx, esi
	cmpgexadd dword [0x69c], edi, eax

%ifdef ERROR
	cmpgexadd qword [0xb28], rax, rsi
	cmpgexadd qword [0x71a], rdx, rdx
	cmpgexadd dword [0x4e3], r10d, r11d
	cmpgexadd qword [0xad4], r11, r13
	cmpgexadd dword [0x3e4], r23d, r19d
	cmpgexadd qword [0xf8b], r30, r26
	cmpgexadd qword [eax+1], rbp, rdi
	cmpgexadd qword [eax+64], rdx, rdi
	cmpgexadd [0x4e3], rsi, rax
%endif
