default rel
	vpcmpnltq k4, xmm6, xmm2
	vpcmpnltq k2, xmm0, xmm6
	vpcmpnltq k7, ymm5, ymm0
	vpcmpnltq k5, ymm6, ymm4
	vpcmpnltq k4, zmm5, zword [0xc12]
	vpcmpnltq k1, zmm1, zmm7
	vpcmpnltq k7, xmm8, xmm9
	vpcmpnltq k5, ymm13, ymm9
	vpcmpnltq k6, zmm11, zmm11
	vpcmpnltq k6, xmm21, xmm30
	vpcmpnltq k7, ymm25, ymm17
	vpcmpnltq k3, zmm23, zmm25
	vpcmpnltq k2{k6}, xmm1, xmm1
	vpcmpnltq k5{k3}, ymm0, ymm0
	vpcmpnltq k7{k4}, zmm2, zword [0x821]
	vpcmpnltq k1, xmm1, oword [eax+1]
	vpcmpnltq k5, xmm2, oword [eax+64]
	vpcmpnltq k4, ymm3, yword [eax+1]
	vpcmpnltq k6, ymm4, yword [eax+64]
	vpcmpnltq k7, zmm3, zword [eax+1]
	vpcmpnltq k1, zmm0, zword [eax+64]
	vpcmpnltq k6, xmm7, [0x22f]
	vpcmpnltq k5, ymm5, [0xa10]
	vpcmpnltq k4, zmm5, [0x314]
