default rel
	vphadduwq xmm1, oword [0xa17]
	vphadduwq xmm1
	vphadduwq xmm4, xmm3
	vphadduwq xmm12, xmm15
	vphadduwq xmm7, oword [eax+1]
	vphadduwq xmm3, oword [eax+64]
	vphadduwq xmm7, [0xf60]
