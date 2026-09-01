default rel
	vpmacsswd xmm5, xmm1, oword [0x75a], xmm7
	vpmacsswd xmm5, oword [0x75a], xmm7
	vpmacsswd xmm3, xmm1, xmm0, xmm1
	vpmacsswd xmm13, xmm8, xmm13, xmm9
	vpmacsswd xmm4, xmm0, oword [eax+1], xmm4
	vpmacsswd xmm3, xmm5, oword [eax+64], xmm3
	vpmacsswd xmm3, xmm3, [0xf29], xmm5
