default rel
	cmpneqpd xmm1, xmm1
	cmpneqpd xmm0, xmm2
	cmpneqpd xmm15, xmm8
	cmpneqpd xmm0, oword [eax+1]
	cmpneqpd xmm1, oword [eax+64]
	cmpneqpd xmm1, [0xefd]
