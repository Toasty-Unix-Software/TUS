default rel
	vphaddwq xmm1, oword [0x356]
	vphaddwq xmm1
	vphaddwq xmm7, xmm1
	vphaddwq xmm11, xmm15
	vphaddwq xmm2, oword [eax+1]
	vphaddwq xmm2, oword [eax+64]
	vphaddwq xmm7, [0x67b]
