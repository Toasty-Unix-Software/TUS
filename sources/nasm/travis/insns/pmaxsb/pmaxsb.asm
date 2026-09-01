default rel
	pmaxsb xmm3, oword [0x6ae]
	pmaxsb xmm3, xmm2
	pmaxsb xmm8, xmm13
	pmaxsb xmm0, oword [eax+1]
	pmaxsb xmm1, oword [eax+64]
	pmaxsb xmm3, [0x480]
