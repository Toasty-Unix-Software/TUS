default rel
	vcvtsh2ss xmm2, xmm3, xmm5
	vcvtsh2ss xmm2, xmm5
	vcvtsh2ss xmm1, xmm6, xmm2
	vcvtsh2ss xmm12, xmm13, xmm8
	vcvtsh2ss xmm28, xmm22, xmm25
	vcvtsh2ss xmm1{k1}, xmm7, xmm2
	vcvtsh2ss xmm7{k5}{z}, xmm2, xmm4
	vcvtsh2ss xmm1, xmm2, xmm2, {sae}
	vcvtsh2ss xmm5, xmm0, word [eax+1]
	vcvtsh2ss xmm5, xmm7, word [eax+64]
	vcvtsh2ss xmm3, xmm6, [0x9f9]
