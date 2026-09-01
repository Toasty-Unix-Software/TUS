default rel
	vpcmpnequw k3, xmm5, oword [0xa3e]
	vpcmpnequw k5, xmm2, xmm0
	vpcmpnequw k5, ymm5, yword [0x3c6]
	vpcmpnequw k4, ymm0, yword [0x1fd]
	vpcmpnequw k7, zmm6, zmm1
	vpcmpnequw k2, zmm0, zmm5
	vpcmpnequw k1, xmm11, xmm9
	vpcmpnequw k3, ymm8, ymm9
	vpcmpnequw k6, zmm10, zmm11
	vpcmpnequw k6, xmm18, xmm31
	vpcmpnequw k1, ymm25, ymm31
	vpcmpnequw k6, zmm17, zmm27
	vpcmpnequw k7{k3}, xmm6, xmm2
	vpcmpnequw k3{k6}, ymm1, ymm2
	vpcmpnequw k3{k3}, zmm7, zmm2
	vpcmpnequw k5, xmm5, oword [eax+1]
	vpcmpnequw k6, xmm5, oword [eax+64]
	vpcmpnequw k7, ymm7, yword [eax+1]
	vpcmpnequw k3, ymm0, yword [eax+64]
	vpcmpnequw k3, zmm4, zword [eax+1]
	vpcmpnequw k4, zmm2, zword [eax+64]
	vpcmpnequw k6, xmm5, [0xd8a]
	vpcmpnequw k4, ymm6, [0xaa3]
	vpcmpnequw k1, zmm4, [0x987]
