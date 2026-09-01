	cmpexadd dword [0xb89], edi, ebp
	cmpexadd dword [0x6a0], edi, ebp

%ifdef ERROR
	cmpexadd qword [0x470], rsi, rbp
	cmpexadd qword [0xaa5], rsi, rdx
	cmpexadd dword [0x8b3], r12d, r12d
	cmpexadd qword [0x6a9], r12, r11
	cmpexadd dword [0x9c1], r22d, r28d
	cmpexadd qword [0x517], r25, r20
	cmpexadd qword [eax+1], rax, rsi
	cmpexadd qword [eax+64], rdx, rcx
	cmpexadd [0x94f], rbx, rbx
%endif
