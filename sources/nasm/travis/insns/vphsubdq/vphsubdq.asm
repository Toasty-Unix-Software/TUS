default rel
	vphsubdq xmm2, xmm7
	vphsubdq xmm2
	vphsubdq xmm5, xmm2
	vphsubdq xmm12, xmm11
	vphsubdq xmm5, oword [eax+1]
	vphsubdq xmm5, oword [eax+64]
	vphsubdq xmm2, [0xd3b]
