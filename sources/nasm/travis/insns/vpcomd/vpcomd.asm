default rel
	vpcomd xmm0, xmm2, xmm2, 0xb0
	vpcomd xmm0, xmm2, 0xb0
	vpcomd xmm0, xmm6, xmm6, 0xa
	vpcomd xmm15, xmm15, xmm15, 0x96
	vpcomd xmm1, xmm0, oword [eax+1], 0x10
	vpcomd xmm1, xmm0, oword [eax+64], 0x99
	vpcomd xmm0, xmm4, [0x1f8], 0xf7
