default rel
	cmpnltpd xmm5, xmm4
	cmpnltpd xmm0, xmm4
	cmpnltpd xmm14, xmm15
	cmpnltpd xmm4, oword [eax+1]
	cmpnltpd xmm6, oword [eax+64]
	cmpnltpd xmm7, [0xc1b]
