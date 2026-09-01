default rel
	blsmsk eax, edx
	blsmsk edi, dword [0xaaa]
	blsmsk rdi, rcx
	blsmsk rdx, rdi
	blsmsk r9d, r9d
	blsmsk r14, r9
	blsmsk r27d, r19d
	blsmsk r24, r18
	blsmsk eax, dword [eax+1]
	blsmsk ecx, dword [eax+64]
	blsmsk rbp, qword [eax+1]
	blsmsk rcx, qword [eax+64]
	blsmsk eax, [0x871]
	blsmsk rax, [0xb06]
