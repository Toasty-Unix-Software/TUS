	blsfill ecx, dword [0x4bf]
	blsfill eax, dword [0x775]

%ifdef ERROR
	blsfill rbx, rax
	blsfill rdi, rbx
	blsfill r11d, r13d
	blsfill r11, r13
	blsfill rdx, qword [eax+1]
	blsfill rcx, qword [eax+64]
	blsfill rax, [0x8e6]
%endif
