default rel
	tzmsk ecx, ecx
	tzmsk eax, ebx
	tzmsk rcx, rdi
	tzmsk rdi, rbx
	tzmsk r11d, r12d
	tzmsk r9, r8
	tzmsk eax, dword [eax+1]
	tzmsk eax, dword [eax+64]
	tzmsk rbp, qword [eax+1]
	tzmsk rdx, qword [eax+64]
	tzmsk edx, [0x37f]
	tzmsk rbx, [0x928]
