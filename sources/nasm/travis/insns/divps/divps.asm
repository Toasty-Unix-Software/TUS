default rel
	divps xmm4, xmm7
	divps xmm0, xmm4
	divps xmm8, xmm15
	divps xmm7, oword [eax+1]
	divps xmm4, oword [eax+64]
	divps xmm0, [0xc2c]
