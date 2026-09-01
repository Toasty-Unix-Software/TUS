default rel
	blci edx, ebx
	blci edx, ebx
	blci rdx, rdi
	blci rbx, rsi
	blci r10d, r14d
	blci r13, r10
	blci ebx, dword [eax+1]
	blci eax, dword [eax+64]
	blci rbp, qword [eax+1]
	blci rdi, qword [eax+64]
	blci esi, [0xceb]
	blci rax, [0xd36]
