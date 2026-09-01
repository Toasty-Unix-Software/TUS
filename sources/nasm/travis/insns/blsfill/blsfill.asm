default rel
	blsfill ecx, dword [0x4bf]
	blsfill eax, dword [0x775]
	blsfill rbx, rax
	blsfill rdi, rbx
	blsfill r11d, r13d
	blsfill r11, r13
	blsfill ebp, dword [eax+1]
	blsfill edx, dword [eax+64]
	blsfill rdx, qword [eax+1]
	blsfill rcx, qword [eax+64]
	blsfill eax, [0x22e]
	blsfill rax, [0x8e6]
