default rel
	blendpd xmm3, xmm6, 0x4b
	blendpd xmm0, xmm2, 0x6f
	blendpd xmm9, xmm13, 0x55
	blendpd xmm1, oword [eax+1], 0xcd
	blendpd xmm3, oword [eax+64], 0xbe
	blendpd xmm7, [0x176], 0x55
