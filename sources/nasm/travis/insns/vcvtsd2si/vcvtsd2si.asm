default rel
	vcvtsd2si ebp, xmm0
	vcvtsd2si ecx, xmm5
	vcvtsd2si rdx, qword [0x6ac]
	vcvtsd2si rax, qword [0xba2]
	vcvtsd2si edx, xmm1
	vcvtsd2si esi, xmm6
	vcvtsd2si rdx, qword [0x510]
	vcvtsd2si rbx, xmm0
	vcvtsd2si r9d, xmm15
	vcvtsd2si r15, xmm15
	vcvtsd2si r14d, xmm12
	vcvtsd2si r11, xmm11
	vcvtsd2si r17d, xmm28
	vcvtsd2si r27, xmm29
	vcvtsd2si r24d, xmm27
	vcvtsd2si r27, xmm24
	vcvtsd2si eax, xmm5, {rd-sae}
	vcvtsd2si rbx, xmm2, {rz-sae}
	vcvtsd2si eax, qword [eax+1]
	vcvtsd2si ebx, qword [eax+64]
	vcvtsd2si rbp, qword [eax+1]
	vcvtsd2si rdi, qword [eax+64]
	vcvtsd2si esi, qword [eax+1]
	vcvtsd2si esi, qword [eax+64]
	vcvtsd2si rdi, qword [eax+1]
	vcvtsd2si rsi, qword [eax+64]
	vcvtsd2si ecx, [0x97e]
	vcvtsd2si rcx, [0x2d0]
	vcvtsd2si ebx, [0x7b4]
	vcvtsd2si rcx, [0x975]
