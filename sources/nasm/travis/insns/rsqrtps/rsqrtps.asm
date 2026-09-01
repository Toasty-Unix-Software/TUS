default rel
	rsqrtps xmm1, xmm3
	rsqrtps xmm1, xmm7
	rsqrtps xmm14, xmm10
	rsqrtps xmm6, oword [eax+1]
	rsqrtps xmm1, oword [eax+64]
	rsqrtps xmm0, [0x490]
