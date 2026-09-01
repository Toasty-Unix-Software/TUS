default rel
	vpcmpngtub k7, xmm5, xmm5
	vpcmpngtub k5, xmm7, oword [0xaa2]
	vpcmpngtub k7, ymm5, yword [0xc11]
	vpcmpngtub k7, ymm0, ymm2
	vpcmpngtub k3, zmm0, zmm3
	vpcmpngtub k1, zmm0, zword [0x7e0]
	vpcmpngtub k5, xmm9, xmm11
	vpcmpngtub k7, ymm11, ymm8
	vpcmpngtub k5, zmm11, zmm10
	vpcmpngtub k1, xmm26, xmm31
	vpcmpngtub k4, ymm18, ymm17
	vpcmpngtub k4, zmm20, zmm24
	vpcmpngtub k5{k7}, xmm2, xmm7
	vpcmpngtub k1{k2}, ymm7, ymm6
	vpcmpngtub k5{k4}, zmm6, zmm6
	vpcmpngtub k7, xmm1, oword [eax+1]
	vpcmpngtub k6, xmm2, oword [eax+64]
	vpcmpngtub k7, ymm1, yword [eax+1]
	vpcmpngtub k3, ymm7, yword [eax+64]
	vpcmpngtub k7, zmm2, zword [eax+1]
	vpcmpngtub k1, zmm5, zword [eax+64]
	vpcmpngtub k5, xmm5, [0xa96]
	vpcmpngtub k3, ymm6, [0xa59]
	vpcmpngtub k7, zmm0, [0xcf9]
