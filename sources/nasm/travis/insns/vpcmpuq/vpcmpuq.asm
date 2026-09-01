default rel
	vpcmpuq k3, xmm5, xmm5, 0xb9
	vpcmpuq k2, xmm3, xmm5, 0xfd
	vpcmpuq k5, ymm3, yword [0x380], 0x75
	vpcmpuq k6, ymm0, ymm1, 0x2e
	vpcmpuq k1, zmm6, zword [0x53c], 0x64
	vpcmpuq k6, zmm5, zmm1, 0x48
	vpcmpuq k1, xmm9, xmm13, 0x89
	vpcmpuq k2, ymm9, ymm10, 0xba
	vpcmpuq k1, zmm14, zmm10, 0xc6
	vpcmpuq k3, xmm21, xmm27, 0x6
	vpcmpuq k2, ymm29, ymm19, 0x38
	vpcmpuq k1, zmm25, zmm25, 0xba
	vpcmpuq k1{k3}, xmm5, xmm3, 0x81
	vpcmpuq k4{k1}, ymm1, ymm7, 0x46
	vpcmpuq k4{k7}, zmm5, zmm3, 0x1b
	vpcmpuq k4, xmm5, oword [eax+1], 0xf6
	vpcmpuq k2, xmm3, oword [eax+64], 0x93
	vpcmpuq k3, ymm7, yword [eax+1], 0xd6
	vpcmpuq k5, ymm2, yword [eax+64], 0x82
	vpcmpuq k2, zmm0, zword [eax+1], 0xa0
	vpcmpuq k4, zmm2, zword [eax+64], 0x2a
	vpcmpuq k1, xmm2, [0xeba], 0xcd
	vpcmpuq k2, ymm0, [0xf5d], 0x2b
	vpcmpuq k2, zmm3, [0x1dd], 0xf3
