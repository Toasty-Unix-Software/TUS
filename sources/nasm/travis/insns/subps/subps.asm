default rel
	subps xmm2, xmm4
	subps xmm0, oword [0xa14]
	subps xmm15, xmm8
	subps xmm5, oword [eax+1]
	subps xmm1, oword [eax+64]
	subps xmm3, [0x3d9]
