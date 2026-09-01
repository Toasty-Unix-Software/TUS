default rel
	cmpsxadd dword [0x544], ebp, ecx
	cmpsxadd dword [0x6ff], edx, edi
	cmpsxadd qword [0xd11], rcx, rsi
	cmpsxadd qword [0x478], rsi, rax
	cmpsxadd dword [0x28b], r14d, r14d
	cmpsxadd qword [0x47c], r12, r9
	cmpsxadd dword [0x16f], r28d, r22d
	cmpsxadd qword [0x353], r31, r29
	cmpsxadd dword [eax+1], edi, ecx
	cmpsxadd dword [eax+64], ebp, esi
	cmpsxadd qword [eax+1], rcx, rsi
	cmpsxadd qword [eax+64], rdi, rax
	cmpsxadd [0xabc], eax, ebx
	cmpsxadd [0x5d9], rbp, rbx
