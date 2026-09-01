default rel
	vcvttsh2usi eax, xmm6
	vcvttsh2usi ebp, xmm1
	vcvttsh2usi rdx, xmm2
	vcvttsh2usi rbx, word [0x561]
	vcvttsh2usi r8d, xmm13
	vcvttsh2usi r13, xmm10
	vcvttsh2usi r25d, xmm30
	vcvttsh2usi r25, xmm31
	vcvttsh2usi edx, xmm3, {sae}
	vcvttsh2usi rdi, xmm4, {sae}
	vcvttsh2usi eax, word [eax+1]
	vcvttsh2usi eax, word [eax+64]
	vcvttsh2usi rdx, word [eax+1]
	vcvttsh2usi rbp, word [eax+64]
	vcvttsh2usi ebx, [0x7b0]
	vcvttsh2usi rbx, [0x3ba]
