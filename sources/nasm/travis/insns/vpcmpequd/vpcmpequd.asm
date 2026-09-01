default rel
	vpcmpequd k4, xmm3, xmm1
	vpcmpequd k6, xmm1, xmm5
	vpcmpequd k1, ymm7, ymm3
	vpcmpequd k4, ymm7, ymm7
	vpcmpequd k6, zmm4, zword [0xd84]
	vpcmpequd k7, zmm0, zword [0xf4b]
	vpcmpequd k6, xmm11, xmm8
	vpcmpequd k6, ymm12, ymm8
	vpcmpequd k3, zmm10, zmm8
	vpcmpequd k2, xmm19, xmm21
	vpcmpequd k2, ymm22, ymm30
	vpcmpequd k7, zmm28, zmm25
	vpcmpequd k4{k6}, xmm0, xmm4
	vpcmpequd k7{k6}, ymm1, ymm6
	vpcmpequd k4{k5}, zmm7, zword [0xdd0]
	vpcmpequd k6, xmm5, oword [eax+1]
	vpcmpequd k5, xmm7, oword [eax+64]
	vpcmpequd k3, ymm2, yword [eax+1]
	vpcmpequd k3, ymm0, yword [eax+64]
	vpcmpequd k7, zmm1, zword [eax+1]
	vpcmpequd k7, zmm5, zword [eax+64]
	vpcmpequd k5, xmm7, [0xc72]
	vpcmpequd k6, ymm4, [0x547]
	vpcmpequd k2, zmm2, [0x1a2]
