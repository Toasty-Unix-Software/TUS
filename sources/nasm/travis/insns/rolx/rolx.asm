default rel
	rolx eax, dword [0x9ff], 1
	rolx ebp, edi, 1
	rolx rbx, rbx, 1
	rolx rdi, rdx, 1
	rolx r10d, r12d, 1
	rolx r8, r14, 1
	rolx r18d, r16d, 1
	rolx r18, r23, 1
	rolx ecx, dword [eax+1], 1
	rolx esi, dword [eax+64], 1
	rolx rbx, qword [eax+1], 1
	rolx rbp, qword [eax+64], 1
	rolx edx, [0x928], 1
	rolx rbp, [0xb87], 1
