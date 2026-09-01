default rel
	vpcmpequq k2, xmm4, xmm1
	vpcmpequq k3, xmm0, oword [0x302]
	vpcmpequq k2, ymm3, yword [0xd69]
	vpcmpequq k4, ymm2, ymm3
	vpcmpequq k4, zmm1, zmm6
	vpcmpequq k2, zmm6, zword [0x8bc]
	vpcmpequq k5, xmm10, xmm13
	vpcmpequq k1, ymm13, ymm13
	vpcmpequq k7, zmm10, zmm8
	vpcmpequq k2, xmm25, xmm18
	vpcmpequq k5, ymm27, ymm31
	vpcmpequq k7, zmm22, zmm27
	vpcmpequq k4{k7}, xmm1, oword [0x17b]
	vpcmpequq k3{k1}, ymm0, yword [0x914]
	vpcmpequq k5{k5}, zmm6, zmm0
	vpcmpequq k2, xmm1, oword [eax+1]
	vpcmpequq k1, xmm1, oword [eax+64]
	vpcmpequq k3, ymm5, yword [eax+1]
	vpcmpequq k7, ymm4, yword [eax+64]
	vpcmpequq k6, zmm6, zword [eax+1]
	vpcmpequq k7, zmm5, zword [eax+64]
	vpcmpequq k7, xmm2, [0xbb7]
	vpcmpequq k1, ymm2, [0x266]
	vpcmpequq k2, zmm6, [0xa6c]
