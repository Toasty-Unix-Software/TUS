default rel
	vprotw xmm1, oword [0x51d], xmm5
	vprotw xmm1, xmm5
	vprotw xmm1, oword [0x30a], xmm5
	vprotw xmm0, xmm3, oword [0xc2a]
	vprotw xmm0, oword [0xc2a]
	vprotw xmm1, xmm3, xmm6
	vprotw xmm7, xmm4, 0x50
	vprotw xmm7, 0x50
	vprotw xmm6, xmm1, 0x21
	vprotw xmm12, xmm10, xmm14
	vprotw xmm13, xmm13, xmm13
	vprotw xmm12, xmm14, 0xea
	vprotw xmm4, oword [eax+1], xmm1
	vprotw xmm2, oword [eax+64], xmm7
	vprotw xmm0, xmm6, oword [eax+1]
	vprotw xmm2, xmm0, oword [eax+64]
	vprotw xmm7, oword [eax+1], 0x5e
	vprotw xmm0, oword [eax+64], 0xe1
	vprotw xmm2, [0xdd6], xmm0
	vprotw xmm5, xmm2, [0xd26]
	vprotw xmm4, [0xb08], 0x89
