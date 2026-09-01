default rel
	addsubpd xmm6, xmm3
	addsubpd xmm5, xmm2
	addsubpd xmm8, xmm12
	addsubpd xmm6, oword [eax+1]
	addsubpd xmm4, oword [eax+64]
	addsubpd xmm0, [0x9b2]
