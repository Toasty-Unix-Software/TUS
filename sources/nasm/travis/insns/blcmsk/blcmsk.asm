default rel
	blcmsk ebx, ecx
	blcmsk edi, esi
	blcmsk rdi, rdi
	blcmsk rbx, qword [0x756]
	blcmsk r11d, r13d
	blcmsk r10, r13
	blcmsk edx, dword [eax+1]
	blcmsk eax, dword [eax+64]
	blcmsk rbx, qword [eax+1]
	blcmsk rbx, qword [eax+64]
	blcmsk edx, [0x666]
	blcmsk rcx, [0xdf3]
