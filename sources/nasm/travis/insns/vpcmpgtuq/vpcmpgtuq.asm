default rel
	vpcmpgtuq k2, xmm4, xmm3
	vpcmpgtuq k3, xmm0, xmm3
	vpcmpgtuq k1, ymm5, ymm0
	vpcmpgtuq k6, ymm2, yword [0xb6e]
	vpcmpgtuq k3, zmm3, zword [0x4ba]
	vpcmpgtuq k1, zmm1, zmm2
	vpcmpgtuq k4, xmm12, xmm10
	vpcmpgtuq k5, ymm15, ymm14
	vpcmpgtuq k6, zmm8, zmm9
	vpcmpgtuq k3, xmm16, xmm29
	vpcmpgtuq k3, ymm30, ymm27
	vpcmpgtuq k1, zmm23, zmm27
	vpcmpgtuq k1{k1}, xmm5, oword [0xbc7]
	vpcmpgtuq k6{k4}, ymm2, yword [0x847]
	vpcmpgtuq k1{k2}, zmm2, zmm6
	vpcmpgtuq k2, xmm5, oword [eax+1]
	vpcmpgtuq k7, xmm0, oword [eax+64]
	vpcmpgtuq k1, ymm7, yword [eax+1]
	vpcmpgtuq k4, ymm0, yword [eax+64]
	vpcmpgtuq k2, zmm7, zword [eax+1]
	vpcmpgtuq k3, zmm6, zword [eax+64]
	vpcmpgtuq k1, xmm1, [0x5e1]
	vpcmpgtuq k1, ymm4, [0x19a]
	vpcmpgtuq k6, zmm2, [0xa02]
