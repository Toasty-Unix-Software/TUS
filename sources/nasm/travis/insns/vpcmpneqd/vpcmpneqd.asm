default rel
	vpcmpneqd k4, xmm7, xmm7
	vpcmpneqd k5, xmm0, oword [0xa79]
	vpcmpneqd k4, ymm2, ymm2
	vpcmpneqd k3, ymm2, ymm4
	vpcmpneqd k1, zmm6, zword [0x46e]
	vpcmpneqd k2, zmm7, zmm6
	vpcmpneqd k3, xmm10, xmm8
	vpcmpneqd k5, ymm10, ymm14
	vpcmpneqd k1, zmm13, zmm13
	vpcmpneqd k7, xmm24, xmm26
	vpcmpneqd k4, ymm20, ymm18
	vpcmpneqd k2, zmm22, zmm23
	vpcmpneqd k7{k6}, xmm6, oword [0xdeb]
	vpcmpneqd k1{k5}, ymm3, ymm5
	vpcmpneqd k7{k5}, zmm2, zmm7
	vpcmpneqd k2, xmm3, oword [eax+1]
	vpcmpneqd k2, xmm1, oword [eax+64]
	vpcmpneqd k7, ymm0, yword [eax+1]
	vpcmpneqd k4, ymm7, yword [eax+64]
	vpcmpneqd k1, zmm2, zword [eax+1]
	vpcmpneqd k6, zmm3, zword [eax+64]
	vpcmpneqd k5, xmm0, [0x84d]
	vpcmpneqd k4, ymm6, [0xa77]
	vpcmpneqd k5, zmm3, [0x53b]
