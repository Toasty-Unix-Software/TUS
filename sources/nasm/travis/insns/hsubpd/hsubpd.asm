default rel
	hsubpd xmm4, xmm0
	hsubpd xmm6, xmm6
	hsubpd xmm13, xmm14
	hsubpd xmm1, oword [eax+1]
	hsubpd xmm3, oword [eax+64]
	hsubpd xmm4, [0x2da]
