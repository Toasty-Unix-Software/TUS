default rel
	blcs ecx, ebx
	blcs esi, edx
	blcs rbx, rsi
	blcs rbp, qword [0x9a2]
	blcs r15d, r9d
	blcs r11, r10
	blcs ebx, dword [eax+1]
	blcs ebx, dword [eax+64]
	blcs rcx, qword [eax+1]
	blcs rbx, qword [eax+64]
	blcs ecx, [0x80d]
	blcs rdx, [0xc3c]
