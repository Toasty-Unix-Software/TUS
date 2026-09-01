default rel
	vpcmpgeq k4, xmm1, xmm5
	vpcmpgeq k7, xmm4, oword [0x4b3]
	vpcmpgeq k5, ymm3, ymm7
	vpcmpgeq k6, ymm6, ymm7
	vpcmpgeq k3, zmm5, zword [0xffe]
	vpcmpgeq k7, zmm3, zmm7
	vpcmpgeq k1, xmm11, xmm13
	vpcmpgeq k7, ymm12, ymm8
	vpcmpgeq k4, zmm10, zmm9
	vpcmpgeq k5, xmm21, xmm16
	vpcmpgeq k1, ymm25, ymm19
	vpcmpgeq k4, zmm17, zmm16
	vpcmpgeq k7{k5}, xmm5, xmm0
	vpcmpgeq k6{k5}, ymm0, ymm7
	vpcmpgeq k3{k6}, zmm4, zmm2
	vpcmpgeq k2, xmm7, oword [eax+1]
	vpcmpgeq k7, xmm1, oword [eax+64]
	vpcmpgeq k3, ymm5, yword [eax+1]
	vpcmpgeq k3, ymm5, yword [eax+64]
	vpcmpgeq k2, zmm7, zword [eax+1]
	vpcmpgeq k5, zmm0, zword [eax+64]
	vpcmpgeq k3, xmm2, [0x9e2]
	vpcmpgeq k4, ymm5, [0x5c1]
	vpcmpgeq k4, zmm2, [0x4da]
