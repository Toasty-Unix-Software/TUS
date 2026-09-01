default rel
	vpcmpnltw k2, xmm2, xmm7
	vpcmpnltw k2, xmm4, xmm1
	vpcmpnltw k6, ymm6, ymm6
	vpcmpnltw k4, ymm1, ymm4
	vpcmpnltw k6, zmm6, zmm1
	vpcmpnltw k6, zmm5, zword [0x85b]
	vpcmpnltw k1, xmm13, xmm12
	vpcmpnltw k7, ymm9, ymm9
	vpcmpnltw k1, zmm10, zmm11
	vpcmpnltw k5, xmm24, xmm22
	vpcmpnltw k7, ymm29, ymm18
	vpcmpnltw k2, zmm23, zmm29
	vpcmpnltw k7{k4}, xmm0, oword [0xecd]
	vpcmpnltw k4{k4}, ymm7, ymm6
	vpcmpnltw k4{k6}, zmm0, zmm4
	vpcmpnltw k5, xmm2, oword [eax+1]
	vpcmpnltw k4, xmm4, oword [eax+64]
	vpcmpnltw k3, ymm7, yword [eax+1]
	vpcmpnltw k6, ymm6, yword [eax+64]
	vpcmpnltw k5, zmm3, zword [eax+1]
	vpcmpnltw k3, zmm0, zword [eax+64]
	vpcmpnltw k2, xmm3, [0x4c8]
	vpcmpnltw k2, ymm2, [0xfa8]
	vpcmpnltw k2, zmm2, [0xb06]
