	cmpnoxadd dword [0x650], ebp, ebp
	cmpnoxadd dword [0xa33], ebx, edx

%ifdef ERROR
	cmpnoxadd qword [0xcee], rbx, rax
	cmpnoxadd qword [0x251], rdx, rbx
	cmpnoxadd dword [0xecd], r15d, r14d
	cmpnoxadd qword [0x66d], r12, r9
	cmpnoxadd dword [0x690], r20d, r28d
	cmpnoxadd qword [0xe2c], r19, r21
	cmpnoxadd qword [eax+1], rax, rax
	cmpnoxadd qword [eax+64], rbx, rdx
	cmpnoxadd [0x1be], rcx, rsi
%endif
