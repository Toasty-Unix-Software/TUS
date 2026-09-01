default rel
	vpcmpnltud k1, xmm2, xmm0
	vpcmpnltud k2, xmm5, oword [0x445]
	vpcmpnltud k2, ymm1, yword [0xbd9]
	vpcmpnltud k3, ymm0, yword [0x2bb]
	vpcmpnltud k4, zmm6, zmm5
	vpcmpnltud k6, zmm1, zmm2
	vpcmpnltud k4, xmm15, xmm10
	vpcmpnltud k5, ymm15, ymm8
	vpcmpnltud k1, zmm10, zmm13
	vpcmpnltud k2, xmm19, xmm26
	vpcmpnltud k1, ymm31, ymm30
	vpcmpnltud k2, zmm18, zmm18
	vpcmpnltud k6{k2}, xmm6, xmm7
	vpcmpnltud k2{k7}, ymm3, ymm4
	vpcmpnltud k5{k4}, zmm6, zmm6
	vpcmpnltud k5, xmm4, oword [eax+1]
	vpcmpnltud k3, xmm0, oword [eax+64]
	vpcmpnltud k2, ymm4, yword [eax+1]
	vpcmpnltud k2, ymm7, yword [eax+64]
	vpcmpnltud k2, zmm6, zword [eax+1]
	vpcmpnltud k1, zmm7, zword [eax+64]
	vpcmpnltud k7, xmm1, [0x194]
	vpcmpnltud k4, ymm3, [0x5a8]
	vpcmpnltud k1, zmm2, [0xf7e]
