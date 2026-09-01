default rel
	sqrtpd xmm7, oword [0xb6f]
	sqrtpd xmm2, oword [0x5ff]
	sqrtpd xmm14, xmm13
	sqrtpd xmm7, oword [eax+1]
	sqrtpd xmm3, oword [eax+64]
	sqrtpd xmm2, [0xb98]
