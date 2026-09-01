default rel
	divsd xmm6, xmm4
	divsd xmm0, xmm1
	divsd xmm13, xmm9
	divsd xmm2, oword [eax+1]
	divsd xmm5, oword [eax+64]
	divsd xmm7, [0xa5c]
