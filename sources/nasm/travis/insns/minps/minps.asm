default rel
	minps xmm0, xmm0
	minps xmm7, xmm1
	minps xmm14, xmm11
	minps xmm2, oword [eax+1]
	minps xmm3, oword [eax+64]
	minps xmm6, [0x817]
