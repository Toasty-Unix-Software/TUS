default rel
	cvttsd2si ebp, xmm2
	cvttsd2si edi, qword [0xe6e]
	cvttsd2si rcx, xmm6
	cvttsd2si rax, xmm1
	cvttsd2si r11d, xmm11
	cvttsd2si r15, xmm15
	cvttsd2si ecx, qword [eax+1]
	cvttsd2si eax, qword [eax+64]
	cvttsd2si rbp, qword [eax+1]
	cvttsd2si rdi, qword [eax+64]
	cvttsd2si esi, [0x1aa]
	cvttsd2si rbp, [0xb87]
