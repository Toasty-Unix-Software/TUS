default rel
	sqrtps xmm5, xmm0
	sqrtps xmm7, xmm2
	sqrtps xmm9, xmm10
	sqrtps xmm5, oword [eax+1]
	sqrtps xmm4, oword [eax+64]
	sqrtps xmm1, [0xf76]
