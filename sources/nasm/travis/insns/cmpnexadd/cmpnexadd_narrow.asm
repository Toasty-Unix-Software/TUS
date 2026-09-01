	cmpnexadd dword [0xab1], edi, esi
	cmpnexadd dword [0xcfe], eax, edx

%ifdef ERROR
	cmpnexadd qword [0xf26], rbp, rdx
	cmpnexadd qword [0x384], rbp, rbp
	cmpnexadd dword [0xfaf], r9d, r13d
	cmpnexadd qword [0xb41], r8, r15
	cmpnexadd dword [0x9e4], r25d, r28d
	cmpnexadd qword [0xb99], r17, r19
	cmpnexadd qword [eax+1], rsi, rdi
	cmpnexadd qword [eax+64], rdx, rsi
	cmpnexadd [0xd7d], rax, rax
%endif
