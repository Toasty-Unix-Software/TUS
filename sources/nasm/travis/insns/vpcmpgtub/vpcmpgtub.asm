default rel
	vpcmpgtub k7, xmm0, xmm3
	vpcmpgtub k4, xmm4, xmm6
	vpcmpgtub k4, ymm4, ymm4
	vpcmpgtub k3, ymm0, ymm5
	vpcmpgtub k4, zmm1, zword [0xd4a]
	vpcmpgtub k5, zmm1, zmm3
	vpcmpgtub k4, xmm14, xmm11
	vpcmpgtub k5, ymm12, ymm9
	vpcmpgtub k1, zmm15, zmm8
	vpcmpgtub k6, xmm27, xmm25
	vpcmpgtub k4, ymm26, ymm21
	vpcmpgtub k6, zmm29, zmm27
	vpcmpgtub k2{k4}, xmm2, xmm5
	vpcmpgtub k4{k3}, ymm0, ymm1
	vpcmpgtub k3{k2}, zmm7, zword [0x4c5]
	vpcmpgtub k1, xmm4, oword [eax+1]
	vpcmpgtub k5, xmm4, oword [eax+64]
	vpcmpgtub k5, ymm6, yword [eax+1]
	vpcmpgtub k3, ymm7, yword [eax+64]
	vpcmpgtub k3, zmm4, zword [eax+1]
	vpcmpgtub k3, zmm0, zword [eax+64]
	vpcmpgtub k6, xmm2, [0x3fc]
	vpcmpgtub k6, ymm4, [0x74b]
	vpcmpgtub k6, zmm0, [0x3a4]
