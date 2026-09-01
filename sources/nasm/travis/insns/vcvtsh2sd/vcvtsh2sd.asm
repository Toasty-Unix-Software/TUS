default rel
	vcvtsh2sd xmm3, xmm7, xmm6
	vcvtsh2sd xmm3, xmm6
	vcvtsh2sd xmm2, xmm2, xmm2
	vcvtsh2sd xmm10, xmm8, xmm15
	vcvtsh2sd xmm28, xmm23, xmm18
	vcvtsh2sd xmm1, xmm3, xmm6, {sae}
	vcvtsh2sd xmm5, xmm4, word [eax+1]
	vcvtsh2sd xmm2, xmm1, word [eax+64]
	vcvtsh2sd xmm6, xmm4, [0xf50]
