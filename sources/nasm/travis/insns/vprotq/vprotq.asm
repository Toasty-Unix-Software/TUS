default rel
	vprotq xmm0, oword [0x15f], xmm6
	vprotq xmm0, xmm6
	vprotq xmm0, oword [0x5f9], xmm4
	vprotq xmm2, xmm4, oword [0x403]
	vprotq xmm2, oword [0x403]
	vprotq xmm5, xmm7, oword [0xf56]
	vprotq xmm6, xmm3, 0xbc
	vprotq xmm6, 0xbc
	vprotq xmm1, oword [0x7c3], 0x4d
	vprotq xmm8, xmm14, xmm11
	vprotq xmm11, xmm12, xmm8
	vprotq xmm13, xmm8, 0x9
	vprotq xmm0, oword [eax+1], xmm3
	vprotq xmm1, oword [eax+64], xmm5
	vprotq xmm7, xmm3, oword [eax+1]
	vprotq xmm6, xmm1, oword [eax+64]
	vprotq xmm6, oword [eax+1], 0xe
	vprotq xmm7, oword [eax+64], 0x5e
	vprotq xmm0, [0xf95], xmm7
	vprotq xmm1, xmm3, [0xb72]
	vprotq xmm3, [0x616], 0x53
