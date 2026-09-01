	vcvtsh2usi edx, xmm2
	vcvtsh2usi ebx, xmm3

%ifdef ERROR
	vcvtsh2usi rcx, xmm1
	vcvtsh2usi rax, xmm6
	vcvtsh2usi r15d, xmm15
	vcvtsh2usi r12, xmm9
	vcvtsh2usi r27d, xmm25
	vcvtsh2usi r27, xmm23
	vcvtsh2usi rsi, word [eax+1]
	vcvtsh2usi rsi, word [eax+64]
	vcvtsh2usi rbp, [0x39d]
%endif
