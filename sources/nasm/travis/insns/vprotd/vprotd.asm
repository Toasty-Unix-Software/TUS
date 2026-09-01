default rel
	vprotd xmm5, oword [0xd6a], xmm3
	vprotd xmm5, xmm3
	vprotd xmm6, xmm6, xmm3
	vprotd xmm2, xmm6, oword [0xddc]
	vprotd xmm2, oword [0xddc]
	vprotd xmm4, xmm4, xmm0
	vprotd xmm1, xmm6, 0xa9
	vprotd xmm1, 0xa9
	vprotd xmm3, xmm7, 0xb
	vprotd xmm14, xmm12, xmm8
	vprotd xmm12, xmm14, xmm13
	vprotd xmm10, xmm9, 0x8
	vprotd xmm4, oword [eax+1], xmm4
	vprotd xmm6, oword [eax+64], xmm7
	vprotd xmm3, xmm5, oword [eax+1]
	vprotd xmm6, xmm3, oword [eax+64]
	vprotd xmm0, oword [eax+1], 0xf1
	vprotd xmm0, oword [eax+64], 0x72
	vprotd xmm5, [0xfa5], xmm2
	vprotd xmm5, xmm6, [0xff6]
	vprotd xmm3, [0x92f], 0x76
