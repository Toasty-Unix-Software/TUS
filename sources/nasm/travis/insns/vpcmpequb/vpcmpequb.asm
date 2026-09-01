default rel
	vpcmpequb k4, xmm7, xmm0
	vpcmpequb k7, xmm3, oword [0x3a1]
	vpcmpequb k4, ymm2, ymm6
	vpcmpequb k6, ymm4, yword [0x9fb]
	vpcmpequb k6, zmm5, zmm1
	vpcmpequb k6, zmm2, zmm6
	vpcmpequb k4, xmm11, xmm10
	vpcmpequb k7, ymm14, ymm12
	vpcmpequb k7, zmm9, zmm14
	vpcmpequb k5, xmm27, xmm21
	vpcmpequb k2, ymm20, ymm31
	vpcmpequb k2, zmm26, zmm25
	vpcmpequb k7{k7}, xmm0, xmm7
	vpcmpequb k3{k1}, ymm2, yword [0x34d]
	vpcmpequb k2{k5}, zmm5, zword [0x4ff]
	vpcmpequb k5, xmm0, oword [eax+1]
	vpcmpequb k7, xmm7, oword [eax+64]
	vpcmpequb k7, ymm1, yword [eax+1]
	vpcmpequb k3, ymm3, yword [eax+64]
	vpcmpequb k6, zmm3, zword [eax+1]
	vpcmpequb k1, zmm3, zword [eax+64]
	vpcmpequb k7, xmm3, [0x392]
	vpcmpequb k2, ymm2, [0x55d]
	vpcmpequb k7, zmm4, [0xc5e]
