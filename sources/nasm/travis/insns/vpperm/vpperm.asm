default rel
	vpperm xmm0, xmm4, xmm1, oword [0xfe4]
	vpperm xmm0, xmm1, oword [0xfe4]
	vpperm xmm5, xmm7, xmm2, xmm7
	vpperm xmm7, xmm2, xmm4, xmm7
	vpperm xmm7, xmm4, xmm7
	vpperm xmm7, xmm5, oword [0x35c], xmm0
	vpperm xmm14, xmm11, xmm10, xmm12
	vpperm xmm13, xmm15, xmm8, xmm15
	vpperm xmm3, xmm2, xmm1, oword [eax+1]
	vpperm xmm7, xmm0, xmm2, oword [eax+64]
	vpperm xmm6, xmm0, oword [eax+1], xmm7
	vpperm xmm5, xmm3, oword [eax+64], xmm7
	vpperm xmm5, xmm4, xmm4, [0x8e8]
	vpperm xmm4, xmm6, [0xb0a], xmm1
