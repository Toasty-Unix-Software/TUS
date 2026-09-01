default rel
	vphsubwd xmm1, xmm3
	vphsubwd xmm1
	vphsubwd xmm2, xmm0
	vphsubwd xmm14, xmm15
	vphsubwd xmm2, oword [eax+1]
	vphsubwd xmm4, oword [eax+64]
	vphsubwd xmm2, [0xee9]
