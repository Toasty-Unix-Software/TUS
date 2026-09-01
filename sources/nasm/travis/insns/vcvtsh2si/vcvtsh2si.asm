default rel
	vcvtsh2si ebx, xmm3
	vcvtsh2si ecx, xmm1
	vcvtsh2si rsi, xmm0
	vcvtsh2si rbx, xmm7
	vcvtsh2si r14d, xmm14
	vcvtsh2si r13, xmm8
	vcvtsh2si r20d, xmm21
	vcvtsh2si r17, xmm26
	vcvtsh2si ebp, xmm4, {rn-sae}
	vcvtsh2si rax, xmm1, {rd-sae}
	vcvtsh2si esi, word [eax+1]
	vcvtsh2si edi, word [eax+64]
	vcvtsh2si rbp, word [eax+1]
	vcvtsh2si rbp, word [eax+64]
	vcvtsh2si edi, [0xf2c]
	vcvtsh2si rdx, [0xae8]
