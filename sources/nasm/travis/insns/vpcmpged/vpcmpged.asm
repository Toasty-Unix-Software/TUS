default rel
	vpcmpged k2, xmm2, xmm4
	vpcmpged k2, xmm5, xmm6
	vpcmpged k1, ymm0, yword [0xe7d]
	vpcmpged k3, ymm4, ymm3
	vpcmpged k4, zmm0, zword [0x996]
	vpcmpged k7, zmm4, zmm6
	vpcmpged k2, xmm14, xmm11
	vpcmpged k2, ymm14, ymm11
	vpcmpged k6, zmm8, zmm12
	vpcmpged k7, xmm18, xmm21
	vpcmpged k5, ymm20, ymm19
	vpcmpged k5, zmm18, zmm30
	vpcmpged k2{k2}, xmm7, oword [0x9b1]
	vpcmpged k2{k5}, ymm7, ymm0
	vpcmpged k4{k4}, zmm5, zmm3
	vpcmpged k3, xmm5, oword [eax+1]
	vpcmpged k7, xmm5, oword [eax+64]
	vpcmpged k4, ymm0, yword [eax+1]
	vpcmpged k5, ymm3, yword [eax+64]
	vpcmpged k6, zmm1, zword [eax+1]
	vpcmpged k5, zmm7, zword [eax+64]
	vpcmpged k5, xmm4, [0x924]
	vpcmpged k6, ymm3, [0xab5]
	vpcmpged k7, zmm4, [0x3a9]
