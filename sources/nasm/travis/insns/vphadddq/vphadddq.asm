default rel
	vphadddq xmm5, xmm7
	vphadddq xmm5
	vphadddq xmm6, oword [0xc1e]
	vphadddq xmm9, xmm11
	vphadddq xmm4, oword [eax+1]
	vphadddq xmm3, oword [eax+64]
	vphadddq xmm2, [0xe58]
