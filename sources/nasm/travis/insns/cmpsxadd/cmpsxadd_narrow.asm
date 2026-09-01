	cmpsxadd dword [0x544], ebp, ecx
	cmpsxadd dword [0x6ff], edx, edi

%ifdef ERROR
	cmpsxadd qword [0xd11], rcx, rsi
	cmpsxadd qword [0x478], rsi, rax
	cmpsxadd dword [0x28b], r14d, r14d
	cmpsxadd qword [0x47c], r12, r9
	cmpsxadd dword [0x16f], r28d, r22d
	cmpsxadd qword [0x353], r31, r29
	cmpsxadd qword [eax+1], rcx, rsi
	cmpsxadd qword [eax+64], rdi, rax
	cmpsxadd [0x5d9], rbp, rbx
%endif
