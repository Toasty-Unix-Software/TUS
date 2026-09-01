	cmpnlexadd dword [0xdf4], eax, edx
	cmpnlexadd dword [0x561], eax, eax

%ifdef ERROR
	cmpnlexadd qword [0x3cf], rbx, rbp
	cmpnlexadd qword [0x3d3], rdx, rbx
	cmpnlexadd dword [0x2cc], r12d, r15d
	cmpnlexadd qword [0x83d], r9, r12
	cmpnlexadd dword [0x86f], r22d, r18d
	cmpnlexadd qword [0x30d], r30, r31
	cmpnlexadd qword [eax+1], rbx, rdx
	cmpnlexadd qword [eax+64], rbp, rax
	cmpnlexadd [0xf62], rdi, rsi
%endif
