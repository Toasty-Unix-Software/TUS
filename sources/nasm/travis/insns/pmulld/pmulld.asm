default rel
	pmulld xmm5, oword [0x302]
	pmulld xmm5, oword [0x7e2]
	pmulld xmm12, xmm15
	pmulld xmm3, oword [eax+1]
	pmulld xmm6, oword [eax+64]
	pmulld xmm0, [0xfb3]
