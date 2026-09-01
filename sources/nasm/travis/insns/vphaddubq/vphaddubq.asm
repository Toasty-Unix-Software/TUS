default rel
	vphaddubq xmm1, oword [0xa5e]
	vphaddubq xmm1
	vphaddubq xmm0, xmm7
	vphaddubq xmm9, xmm15
	vphaddubq xmm0, oword [eax+1]
	vphaddubq xmm3, oword [eax+64]
	vphaddubq xmm1, [0x627]
