default rel
	vphsubbw xmm0, xmm7
	vphsubbw xmm0
	vphsubbw xmm2, xmm1
	vphsubbw xmm8, xmm10
	vphsubbw xmm4, oword [eax+1]
	vphsubbw xmm3, oword [eax+64]
	vphsubbw xmm3, [0x8c6]
