default rel
	cmpunordpd xmm1, xmm1
	cmpunordpd xmm7, xmm1
	cmpunordpd xmm11, xmm15
	cmpunordpd xmm1, oword [eax+1]
	cmpunordpd xmm3, oword [eax+64]
	cmpunordpd xmm5, [0xfba]
