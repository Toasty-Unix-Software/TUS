default rel
	shufpd xmm1, oword [0x9ac], 0x24
	shufpd xmm0, xmm1, 0x90
	shufpd xmm15, xmm13, 0x70
	shufpd xmm6, oword [eax+1], 0xa0
	shufpd xmm4, oword [eax+64], 0xa0
	shufpd xmm0, [0xf53], 0xbd
