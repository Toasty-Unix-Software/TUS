default rel
	vpcmpistri xmm3, xmm5, 0xa8
	vpcmpistri xmm4, xmm6, 0xf
	vpcmpistri xmm12, xmm11, 0xaf
	vpcmpistri xmm6, oword [eax+1], 0x6a
	vpcmpistri xmm1, oword [eax+64], 0xd2
	vpcmpistri xmm1, [0x696], 0x54
