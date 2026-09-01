default rel
	pmaxuw xmm2, oword [0xf41]
	pmaxuw xmm2, xmm7
	pmaxuw xmm8, xmm14
	pmaxuw xmm3, oword [eax+1]
	pmaxuw xmm2, oword [eax+64]
	pmaxuw xmm2, [0x281]
