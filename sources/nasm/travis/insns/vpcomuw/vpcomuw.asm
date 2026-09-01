default rel
	vpcomuw xmm6, xmm2, xmm5, 0xe5
	vpcomuw xmm6, xmm5, 0xe5
	vpcomuw xmm0, xmm4, xmm1, 0xc6
	vpcomuw xmm15, xmm8, xmm9, 0xb4
	vpcomuw xmm2, xmm2, oword [eax+1], 0xc9
	vpcomuw xmm5, xmm3, oword [eax+64], 0x58
	vpcomuw xmm6, xmm4, [0x9c0], 0xe1
