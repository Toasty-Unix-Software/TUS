default rel
	xorpd xmm4, xmm2
	xorpd xmm6, xmm0
	xorpd xmm13, xmm12
	xorpd xmm5, oword [eax+1]
	xorpd xmm5, oword [eax+64]
	xorpd xmm2, [0xa29]
