default rel
	addpd xmm3, xmm5
	addpd xmm5, xmm2
	addpd xmm9, xmm8
	addpd xmm4, oword [eax+1]
	addpd xmm3, oword [eax+64]
	addpd xmm2, [0xa4e]
