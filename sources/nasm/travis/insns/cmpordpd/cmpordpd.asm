default rel
	cmpordpd xmm5, xmm0
	cmpordpd xmm7, xmm2
	cmpordpd xmm10, xmm12
	cmpordpd xmm4, oword [eax+1]
	cmpordpd xmm6, oword [eax+64]
	cmpordpd xmm1, [0xc93]
