default rel
	cmplxadd dword [0x1a1], edi, ebp
	cmplxadd dword [0xd70], edi, ecx
	cmplxadd qword [0x47b], rbp, rax
	cmplxadd qword [0x91c], rdx, rcx
	cmplxadd dword [0xe4b], r10d, r13d
	cmplxadd qword [0xa9e], r11, r13
	cmplxadd dword [0x863], r27d, r19d
	cmplxadd qword [0x746], r29, r30
	cmplxadd dword [eax+1], eax, ecx
	cmplxadd dword [eax+64], ecx, ebx
	cmplxadd qword [eax+1], rdx, rdi
	cmplxadd qword [eax+64], rbp, rbx
	cmplxadd [0x61b], edx, edi
	cmplxadd [0xee2], rax, rbp
