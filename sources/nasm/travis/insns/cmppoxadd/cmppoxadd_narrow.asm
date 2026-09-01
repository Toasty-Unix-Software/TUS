	cmppoxadd dword [0xa41], esi, ebp
	cmppoxadd dword [0xae4], edi, edi

%ifdef ERROR
	cmppoxadd qword [0x814], rcx, rdx
	cmppoxadd qword [0xb4d], rcx, rsi
	cmppoxadd dword [0x66e], r8d, r13d
	cmppoxadd qword [0x514], r15, r8
	cmppoxadd dword [0x877], r16d, r20d
	cmppoxadd qword [0x487], r26, r31
	cmppoxadd qword [eax+1], rbx, rax
	cmppoxadd qword [eax+64], rcx, rsi
	cmppoxadd [0x78c], rsi, rbx
%endif
