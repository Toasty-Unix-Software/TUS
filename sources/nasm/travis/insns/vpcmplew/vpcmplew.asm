default rel
	vpcmplew k6, xmm6, xmm5
	vpcmplew k2, xmm1, xmm4
	vpcmplew k3, ymm3, yword [0x6ca]
	vpcmplew k3, ymm0, yword [0x7df]
	vpcmplew k5, zmm2, zword [0x6c2]
	vpcmplew k4, zmm0, zmm3
	vpcmplew k4, xmm13, xmm9
	vpcmplew k2, ymm11, ymm13
	vpcmplew k4, zmm13, zmm11
	vpcmplew k1, xmm22, xmm17
	vpcmplew k6, ymm30, ymm31
	vpcmplew k5, zmm30, zmm27
	vpcmplew k6{k7}, xmm6, xmm2
	vpcmplew k5{k7}, ymm7, ymm7
	vpcmplew k5{k1}, zmm0, zmm3
	vpcmplew k3, xmm6, oword [eax+1]
	vpcmplew k1, xmm1, oword [eax+64]
	vpcmplew k5, ymm0, yword [eax+1]
	vpcmplew k4, ymm4, yword [eax+64]
	vpcmplew k5, zmm0, zword [eax+1]
	vpcmplew k2, zmm3, zword [eax+64]
	vpcmplew k6, xmm2, [0xde7]
	vpcmplew k3, ymm6, [0x7c6]
	vpcmplew k3, zmm6, [0x187]
