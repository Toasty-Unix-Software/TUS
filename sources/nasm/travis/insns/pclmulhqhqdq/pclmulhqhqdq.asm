default rel
	pclmulhqhqdq xmm2, xmm0
	pclmulhqhqdq xmm4, xmm7
	pclmulhqhqdq xmm12, xmm9
	pclmulhqhqdq xmm3, oword [eax+1]
	pclmulhqhqdq xmm7, oword [eax+64]
	pclmulhqhqdq xmm3, [0xc5e]
