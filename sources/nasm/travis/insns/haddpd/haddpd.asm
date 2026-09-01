default rel
	haddpd xmm0, oword [0xd71]
	haddpd xmm7, xmm6
	haddpd xmm15, xmm8
	haddpd xmm0, oword [eax+1]
	haddpd xmm0, oword [eax+64]
	haddpd xmm4, [0xa4a]
