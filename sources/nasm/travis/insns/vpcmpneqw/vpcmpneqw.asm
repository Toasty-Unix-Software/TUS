default rel
	vpcmpneqw k4, xmm0, xmm1
	vpcmpneqw k7, xmm4, xmm1
	vpcmpneqw k5, ymm4, yword [0x7f7]
	vpcmpneqw k2, ymm3, ymm2
	vpcmpneqw k1, zmm6, zword [0x346]
	vpcmpneqw k1, zmm4, zmm2
	vpcmpneqw k4, xmm12, xmm13
	vpcmpneqw k4, ymm12, ymm9
	vpcmpneqw k7, zmm11, zmm8
	vpcmpneqw k1, xmm23, xmm21
	vpcmpneqw k6, ymm31, ymm29
	vpcmpneqw k7, zmm17, zmm16
	vpcmpneqw k5{k1}, xmm2, xmm6
	vpcmpneqw k7{k4}, ymm2, ymm0
	vpcmpneqw k7{k2}, zmm5, zmm5
	vpcmpneqw k1, xmm0, oword [eax+1]
	vpcmpneqw k5, xmm0, oword [eax+64]
	vpcmpneqw k7, ymm1, yword [eax+1]
	vpcmpneqw k4, ymm0, yword [eax+64]
	vpcmpneqw k7, zmm7, zword [eax+1]
	vpcmpneqw k2, zmm6, zword [eax+64]
	vpcmpneqw k1, xmm5, [0x5f3]
	vpcmpneqw k2, ymm4, [0x989]
	vpcmpneqw k6, zmm7, [0x158]
