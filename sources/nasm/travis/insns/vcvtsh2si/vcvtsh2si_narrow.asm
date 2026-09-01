	vcvtsh2si ebx, xmm3
	vcvtsh2si ecx, xmm1

%ifdef ERROR
	vcvtsh2si rsi, xmm0
	vcvtsh2si rbx, xmm7
	vcvtsh2si r14d, xmm14
	vcvtsh2si r13, xmm8
	vcvtsh2si r20d, xmm21
	vcvtsh2si r17, xmm26
	vcvtsh2si rbp, word [eax+1]
	vcvtsh2si rbp, word [eax+64]
	vcvtsh2si rdx, [0xae8]
%endif
