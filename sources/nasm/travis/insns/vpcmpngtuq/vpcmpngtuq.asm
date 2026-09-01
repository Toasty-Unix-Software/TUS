default rel
	vpcmpngtuq k7, xmm3, oword [0x4a0]
	vpcmpngtuq k3, xmm2, xmm3
	vpcmpngtuq k3, ymm2, yword [0xf72]
	vpcmpngtuq k3, ymm6, ymm1
	vpcmpngtuq k7, zmm3, zmm5
	vpcmpngtuq k2, zmm3, zmm0
	vpcmpngtuq k7, xmm12, xmm8
	vpcmpngtuq k3, ymm13, ymm10
	vpcmpngtuq k1, zmm13, zmm15
	vpcmpngtuq k1, xmm28, xmm22
	vpcmpngtuq k6, ymm23, ymm28
	vpcmpngtuq k1, zmm30, zmm20
	vpcmpngtuq k6{k3}, xmm0, oword [0xb7a]
	vpcmpngtuq k7{k3}, ymm0, ymm1
	vpcmpngtuq k7{k7}, zmm4, zmm6
	vpcmpngtuq k6, xmm3, oword [eax+1]
	vpcmpngtuq k4, xmm3, oword [eax+64]
	vpcmpngtuq k3, ymm2, yword [eax+1]
	vpcmpngtuq k3, ymm7, yword [eax+64]
	vpcmpngtuq k4, zmm5, zword [eax+1]
	vpcmpngtuq k2, zmm3, zword [eax+64]
	vpcmpngtuq k5, xmm2, [0xf98]
	vpcmpngtuq k4, ymm4, [0x695]
	vpcmpngtuq k1, zmm4, [0x4e0]
