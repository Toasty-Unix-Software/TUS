	cmpnbxadd dword [0x263], esi, edi
	cmpnbxadd dword [0x572], ebp, edx

%ifdef ERROR
	cmpnbxadd qword [0xaae], rbp, rdi
	cmpnbxadd qword [0x329], rax, rbx
	cmpnbxadd dword [0x189], r13d, r10d
	cmpnbxadd qword [0xbc8], r13, r15
	cmpnbxadd dword [0x99b], r25d, r23d
	cmpnbxadd qword [0xcfc], r20, r20
	cmpnbxadd qword [eax+1], rax, rdi
	cmpnbxadd qword [eax+64], rcx, rdi
	cmpnbxadd [0x202], rcx, rsi
%endif
