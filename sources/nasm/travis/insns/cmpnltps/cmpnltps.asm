default rel
	cmpnltps xmm5, xmm2
	cmpnltps xmm0, xmm7
	cmpnltps xmm14, xmm8
	cmpnltps xmm0, oword [eax+1]
	cmpnltps xmm2, oword [eax+64]
	cmpnltps xmm0, [0x57b]
