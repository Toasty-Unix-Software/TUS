default rel
	vpcmpnequd k7, xmm1, oword [0xd06]
	vpcmpnequd k1, xmm2, xmm0
	vpcmpnequd k6, ymm4, ymm0
	vpcmpnequd k5, ymm4, yword [0x6aa]
	vpcmpnequd k1, zmm5, zmm0
	vpcmpnequd k1, zmm0, zword [0x25d]
	vpcmpnequd k5, xmm14, xmm10
	vpcmpnequd k5, ymm11, ymm13
	vpcmpnequd k3, zmm11, zmm11
	vpcmpnequd k5, xmm17, xmm19
	vpcmpnequd k6, ymm24, ymm16
	vpcmpnequd k1, zmm19, zmm31
	vpcmpnequd k4{k6}, xmm6, xmm4
	vpcmpnequd k6{k3}, ymm1, yword [0x405]
	vpcmpnequd k1{k1}, zmm5, zmm0
	vpcmpnequd k1, xmm1, oword [eax+1]
	vpcmpnequd k7, xmm0, oword [eax+64]
	vpcmpnequd k4, ymm0, yword [eax+1]
	vpcmpnequd k1, ymm4, yword [eax+64]
	vpcmpnequd k1, zmm1, zword [eax+1]
	vpcmpnequd k1, zmm4, zword [eax+64]
	vpcmpnequd k7, xmm0, [0x196]
	vpcmpnequd k4, ymm4, [0x337]
	vpcmpnequd k1, zmm0, [0x2e8]
