default rel
	vpcmpnled k6, xmm5, xmm6
	vpcmpnled k5, xmm7, xmm4
	vpcmpnled k1, ymm7, ymm3
	vpcmpnled k5, ymm5, ymm4
	vpcmpnled k1, zmm4, zmm2
	vpcmpnled k7, zmm5, zmm5
	vpcmpnled k2, xmm8, xmm10
	vpcmpnled k3, ymm13, ymm8
	vpcmpnled k6, zmm9, zmm12
	vpcmpnled k2, xmm29, xmm17
	vpcmpnled k4, ymm25, ymm19
	vpcmpnled k5, zmm23, zmm22
	vpcmpnled k6{k7}, xmm2, xmm3
	vpcmpnled k4{k5}, ymm3, ymm3
	vpcmpnled k4{k2}, zmm3, zmm7
	vpcmpnled k3, xmm3, oword [eax+1]
	vpcmpnled k1, xmm6, oword [eax+64]
	vpcmpnled k7, ymm5, yword [eax+1]
	vpcmpnled k4, ymm3, yword [eax+64]
	vpcmpnled k7, zmm1, zword [eax+1]
	vpcmpnled k5, zmm1, zword [eax+64]
	vpcmpnled k3, xmm6, [0x90b]
	vpcmpnled k7, ymm7, [0xdd4]
	vpcmpnled k3, zmm1, [0x4bf]
