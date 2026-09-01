default rel
	vpmadcswd xmm5, xmm4, xmm2, xmm4
	vpmadcswd xmm5, xmm2, xmm4
	vpmadcswd xmm7, xmm0, oword [0x2f0], xmm6
	vpmadcswd xmm14, xmm10, xmm10, xmm10
	vpmadcswd xmm0, xmm5, oword [eax+1], xmm4
	vpmadcswd xmm7, xmm0, oword [eax+64], xmm3
	vpmadcswd xmm6, xmm1, [0xb07], xmm0
