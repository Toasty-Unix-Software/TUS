default rel
	vpcmpgeub k7, xmm7, xmm6
	vpcmpgeub k4, xmm0, xmm5
	vpcmpgeub k5, ymm2, ymm3
	vpcmpgeub k3, ymm7, ymm7
	vpcmpgeub k4, zmm4, zmm6
	vpcmpgeub k2, zmm4, zmm2
	vpcmpgeub k5, xmm14, xmm12
	vpcmpgeub k5, ymm15, ymm15
	vpcmpgeub k6, zmm12, zmm8
	vpcmpgeub k7, xmm19, xmm20
	vpcmpgeub k6, ymm25, ymm31
	vpcmpgeub k2, zmm28, zmm27
	vpcmpgeub k7{k6}, xmm5, xmm7
	vpcmpgeub k3{k4}, ymm0, ymm3
	vpcmpgeub k6{k4}, zmm4, zmm5
	vpcmpgeub k1, xmm5, oword [eax+1]
	vpcmpgeub k6, xmm6, oword [eax+64]
	vpcmpgeub k5, ymm4, yword [eax+1]
	vpcmpgeub k2, ymm1, yword [eax+64]
	vpcmpgeub k4, zmm4, zword [eax+1]
	vpcmpgeub k7, zmm1, zword [eax+64]
	vpcmpgeub k1, xmm6, [0x28d]
	vpcmpgeub k5, ymm7, [0x856]
	vpcmpgeub k3, zmm1, [0xb52]
