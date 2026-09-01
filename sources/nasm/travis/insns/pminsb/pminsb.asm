default rel
	pminsb xmm2, xmm0
	pminsb xmm4, xmm3
	pminsb xmm13, xmm13
	pminsb xmm4, oword [eax+1]
	pminsb xmm1, oword [eax+64]
	pminsb xmm3, [0xc5e]
