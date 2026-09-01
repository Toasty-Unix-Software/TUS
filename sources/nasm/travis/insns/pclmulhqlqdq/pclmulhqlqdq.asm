default rel
	pclmulhqlqdq xmm6, xmm2
	pclmulhqlqdq xmm4, xmm5
	pclmulhqlqdq xmm15, xmm12
	pclmulhqlqdq xmm1, oword [eax+1]
	pclmulhqlqdq xmm2, oword [eax+64]
	pclmulhqlqdq xmm7, [0xf51]
