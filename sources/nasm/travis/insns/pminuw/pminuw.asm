default rel
	pminuw xmm5, xmm5
	pminuw xmm5, xmm2
	pminuw xmm15, xmm8
	pminuw xmm0, oword [eax+1]
	pminuw xmm6, oword [eax+64]
	pminuw xmm6, [0x1a9]
