default rel
	movshdup xmm5, xmm6
	movshdup xmm6, xmm1
	movshdup xmm15, xmm14
	movshdup xmm2, oword [eax+1]
	movshdup xmm6, oword [eax+64]
	movshdup xmm5, [0x1e6]
