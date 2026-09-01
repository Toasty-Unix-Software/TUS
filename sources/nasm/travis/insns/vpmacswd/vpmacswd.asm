default rel
	vpmacswd xmm2, xmm0, xmm3, xmm1
	vpmacswd xmm2, xmm3, xmm1
	vpmacswd xmm6, xmm2, xmm4, xmm5
	vpmacswd xmm14, xmm10, xmm8, xmm15
	vpmacswd xmm3, xmm1, oword [eax+1], xmm7
	vpmacswd xmm1, xmm6, oword [eax+64], xmm4
	vpmacswd xmm6, xmm2, [0x507], xmm0
