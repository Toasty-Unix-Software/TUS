default rel
	vpcmpngtw k3, xmm6, xmm6
	vpcmpngtw k6, xmm5, xmm0
	vpcmpngtw k4, ymm0, ymm4
	vpcmpngtw k4, ymm5, ymm4
	vpcmpngtw k7, zmm4, zword [0xf77]
	vpcmpngtw k6, zmm4, zmm3
	vpcmpngtw k6, xmm12, xmm15
	vpcmpngtw k2, ymm10, ymm11
	vpcmpngtw k3, zmm13, zmm11
	vpcmpngtw k1, xmm20, xmm18
	vpcmpngtw k7, ymm22, ymm30
	vpcmpngtw k4, zmm19, zmm17
	vpcmpngtw k2{k7}, xmm4, oword [0x7e6]
	vpcmpngtw k1{k3}, ymm2, ymm6
	vpcmpngtw k2{k6}, zmm5, zmm5
	vpcmpngtw k3, xmm4, oword [eax+1]
	vpcmpngtw k1, xmm1, oword [eax+64]
	vpcmpngtw k2, ymm5, yword [eax+1]
	vpcmpngtw k5, ymm3, yword [eax+64]
	vpcmpngtw k1, zmm0, zword [eax+1]
	vpcmpngtw k2, zmm5, zword [eax+64]
	vpcmpngtw k2, xmm3, [0xa45]
	vpcmpngtw k7, ymm1, [0x682]
	vpcmpngtw k5, zmm6, [0x9b7]
