default rel
	cmpneqsd xmm5, xmm6
	cmpneqsd xmm3, xmm2
	cmpneqsd xmm9, xmm8
	cmpneqsd xmm2, oword [eax+1]
	cmpneqsd xmm0, oword [eax+64]
	cmpneqsd xmm5, [0xf1c]
