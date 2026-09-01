default rel
	ptest xmm7, oword [0xf74]
	ptest xmm6, xmm3
	ptest xmm13, xmm9
	ptest xmm2, oword [eax+1]
	ptest xmm3, oword [eax+64]
	ptest xmm3, [0xc05]
