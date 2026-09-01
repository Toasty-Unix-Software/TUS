default rel
	vpcmpnltub k7, xmm6, oword [0x9e9]
	vpcmpnltub k7, xmm0, xmm5
	vpcmpnltub k3, ymm5, ymm7
	vpcmpnltub k4, ymm6, yword [0xdca]
	vpcmpnltub k5, zmm4, zword [0xdbf]
	vpcmpnltub k3, zmm1, zmm6
	vpcmpnltub k2, xmm14, xmm9
	vpcmpnltub k3, ymm12, ymm13
	vpcmpnltub k1, zmm8, zmm12
	vpcmpnltub k4, xmm26, xmm19
	vpcmpnltub k6, ymm26, ymm25
	vpcmpnltub k6, zmm24, zmm27
	vpcmpnltub k6{k4}, xmm7, xmm7
	vpcmpnltub k4{k2}, ymm3, ymm1
	vpcmpnltub k2{k4}, zmm2, zmm3
	vpcmpnltub k3, xmm6, oword [eax+1]
	vpcmpnltub k5, xmm5, oword [eax+64]
	vpcmpnltub k6, ymm1, yword [eax+1]
	vpcmpnltub k6, ymm4, yword [eax+64]
	vpcmpnltub k6, zmm5, zword [eax+1]
	vpcmpnltub k5, zmm7, zword [eax+64]
	vpcmpnltub k5, xmm0, [0x972]
	vpcmpnltub k1, ymm1, [0x8b1]
	vpcmpnltub k5, zmm1, [0xf97]
