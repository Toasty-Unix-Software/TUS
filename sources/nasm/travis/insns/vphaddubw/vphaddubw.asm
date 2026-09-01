default rel
	vphaddubw xmm0, xmm5
	vphaddubw xmm0
	vphaddubw xmm2, xmm6
	vphaddubw xmm9, xmm10
	vphaddubw xmm6, oword [eax+1]
	vphaddubw xmm0, oword [eax+64]
	vphaddubw xmm7, [0xe78]
