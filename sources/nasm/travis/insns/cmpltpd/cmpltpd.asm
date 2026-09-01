default rel
	cmpltpd xmm3, xmm1
	cmpltpd xmm4, xmm3
	cmpltpd xmm8, xmm13
	cmpltpd xmm6, oword [eax+1]
	cmpltpd xmm6, oword [eax+64]
	cmpltpd xmm0, [0x72b]
