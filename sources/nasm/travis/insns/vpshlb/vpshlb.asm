default rel
	vpshlb xmm3, oword [0x4ba], xmm6
	vpshlb xmm3, xmm6
	vpshlb xmm6, xmm1, xmm6
	vpshlb xmm5, xmm3, xmm4
	vpshlb xmm5, xmm4
	vpshlb xmm0, xmm4, xmm4
	vpshlb xmm12, xmm9, xmm14
	vpshlb xmm11, xmm11, xmm11
	vpshlb xmm0, oword [eax+1], xmm7
	vpshlb xmm2, oword [eax+64], xmm2
	vpshlb xmm3, xmm2, oword [eax+1]
	vpshlb xmm7, xmm5, oword [eax+64]
	vpshlb xmm4, [0xbf1], xmm5
	vpshlb xmm2, xmm1, [0x793]
