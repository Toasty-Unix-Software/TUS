default rel
	vpcmpequw k3, xmm4, xmm4
	vpcmpequw k7, xmm0, oword [0x977]
	vpcmpequw k1, ymm2, ymm7
	vpcmpequw k7, ymm7, yword [0x1f1]
	vpcmpequw k2, zmm1, zmm6
	vpcmpequw k5, zmm4, zmm4
	vpcmpequw k3, xmm15, xmm13
	vpcmpequw k3, ymm12, ymm9
	vpcmpequw k1, zmm11, zmm12
	vpcmpequw k1, xmm29, xmm22
	vpcmpequw k6, ymm23, ymm26
	vpcmpequw k1, zmm28, zmm19
	vpcmpequw k4{k4}, xmm0, xmm5
	vpcmpequw k3{k5}, ymm0, yword [0x78b]
	vpcmpequw k7{k6}, zmm7, zmm6
	vpcmpequw k2, xmm5, oword [eax+1]
	vpcmpequw k6, xmm6, oword [eax+64]
	vpcmpequw k2, ymm1, yword [eax+1]
	vpcmpequw k4, ymm4, yword [eax+64]
	vpcmpequw k7, zmm3, zword [eax+1]
	vpcmpequw k2, zmm4, zword [eax+64]
	vpcmpequw k5, xmm2, [0x3d9]
	vpcmpequw k6, ymm7, [0xab4]
	vpcmpequw k3, zmm2, [0xe2c]
