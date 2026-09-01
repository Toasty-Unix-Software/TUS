default rel
	vpcomb xmm2, xmm2, xmm4, 0x4f
	vpcomb xmm2, xmm4, 0x4f
	vpcomb xmm3, xmm0, oword [0x4d7], 0x7c
	vpcomb xmm11, xmm13, xmm9, 0xe0
	vpcomb xmm2, xmm0, oword [eax+1], 0x70
	vpcomb xmm0, xmm0, oword [eax+64], 0x50
	vpcomb xmm1, xmm7, [0xe2b], 0xdd
