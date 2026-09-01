default rel
	vphaddwd xmm3, xmm4
	vphaddwd xmm3
	vphaddwd xmm4, xmm7
	vphaddwd xmm10, xmm9
	vphaddwd xmm1, oword [eax+1]
	vphaddwd xmm0, oword [eax+64]
	vphaddwd xmm0, [0x2dc]
