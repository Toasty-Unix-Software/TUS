default rel
	vprotb xmm3, oword [0x4bc], xmm2
	vprotb xmm3, xmm2
	vprotb xmm7, oword [0xdb1], xmm6
	vprotb xmm3, xmm3, xmm2
	vprotb xmm3, xmm2
	vprotb xmm0, xmm2, xmm3
	vprotb xmm5, oword [0xd3d], 0xd5
	vprotb xmm5, 0xd5
	vprotb xmm5, xmm7, 0x4e
	vprotb xmm9, xmm12, xmm11
	vprotb xmm14, xmm13, xmm14
	vprotb xmm11, xmm11, 0x45
	vprotb xmm1, oword [eax+1], xmm0
	vprotb xmm4, oword [eax+64], xmm4
	vprotb xmm1, xmm5, oword [eax+1]
	vprotb xmm1, xmm6, oword [eax+64]
	vprotb xmm3, oword [eax+1], 0x56
	vprotb xmm5, oword [eax+64], 0xb2
	vprotb xmm2, [0x407], xmm3
	vprotb xmm3, xmm0, [0x75f]
	vprotb xmm5, [0x7b2], 0xda
