default rel
	vphaddudq xmm1, oword [0x449]
	vphaddudq xmm1
	vphaddudq xmm6, xmm2
	vphaddudq xmm11, xmm10
	vphaddudq xmm3, oword [eax+1]
	vphaddudq xmm3, oword [eax+64]
	vphaddudq xmm5, [0x611]
