default rel
	vcvttsh2si eax, word [0xc9f]
	vcvttsh2si eax, word [0x777]
	vcvttsh2si rax, xmm0
	vcvttsh2si rcx, word [0x40f]
	vcvttsh2si r14d, xmm14
	vcvttsh2si r10, xmm12
	vcvttsh2si r28d, xmm25
	vcvttsh2si r17, xmm16
	vcvttsh2si ebx, xmm1, {sae}
	vcvttsh2si rdx, xmm4, {sae}
	vcvttsh2si ebx, word [eax+1]
	vcvttsh2si esi, word [eax+64]
	vcvttsh2si rcx, word [eax+1]
	vcvttsh2si rcx, word [eax+64]
	vcvttsh2si ebp, [0x88c]
	vcvttsh2si rdx, [0xfb3]
