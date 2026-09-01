	cmpnlxadd dword [0x178], ebx, ebx
	cmpnlxadd dword [0x6fb], edx, esi

%ifdef ERROR
	cmpnlxadd qword [0xfc5], rax, rax
	cmpnlxadd qword [0xdcf], rbx, rsi
	cmpnlxadd dword [0x982], r15d, r8d
	cmpnlxadd qword [0x69b], r9, r14
	cmpnlxadd dword [0x90b], r22d, r16d
	cmpnlxadd qword [0x9c6], r17, r24
	cmpnlxadd qword [eax+1], rcx, rsi
	cmpnlxadd qword [eax+64], rbx, rcx
	cmpnlxadd [0x155], rax, rsi
%endif
