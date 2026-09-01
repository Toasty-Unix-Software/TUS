default rel
	vphaddubd xmm7, xmm4
	vphaddubd xmm7
	vphaddubd xmm2, xmm3
	vphaddubd xmm12, xmm11
	vphaddubd xmm5, oword [eax+1]
	vphaddubd xmm6, oword [eax+64]
	vphaddubd xmm7, [0xd51]
