default rel
	minpd xmm1, xmm7
	minpd xmm0, oword [0x520]
	minpd xmm15, xmm13
	minpd xmm4, oword [eax+1]
	minpd xmm2, oword [eax+64]
	minpd xmm2, [0xd35]
