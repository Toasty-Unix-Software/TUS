default rel
	pminsd xmm0, xmm3
	pminsd xmm5, xmm6
	pminsd xmm10, xmm15
	pminsd xmm5, oword [eax+1]
	pminsd xmm6, oword [eax+64]
	pminsd xmm7, [0x89e]
