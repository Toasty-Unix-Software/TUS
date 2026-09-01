default rel
	pmuldq xmm7, xmm6
	pmuldq xmm0, oword [0x779]
	pmuldq xmm15, xmm11
	pmuldq xmm3, oword [eax+1]
	pmuldq xmm3, oword [eax+64]
	pmuldq xmm2, [0x6b4]
