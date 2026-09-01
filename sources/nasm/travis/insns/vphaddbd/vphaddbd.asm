default rel
	vphaddbd xmm0, oword [0xc66]
	vphaddbd xmm0
	vphaddbd xmm5, xmm3
	vphaddbd xmm10, xmm12
	vphaddbd xmm2, oword [eax+1]
	vphaddbd xmm3, oword [eax+64]
	vphaddbd xmm3, [0x730]
