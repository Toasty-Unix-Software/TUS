default rel
	vpcmpnltd k4, xmm7, xmm1
	vpcmpnltd k3, xmm6, xmm0
	vpcmpnltd k2, ymm0, ymm2
	vpcmpnltd k2, ymm5, ymm4
	vpcmpnltd k2, zmm2, zmm7
	vpcmpnltd k3, zmm3, zmm1
	vpcmpnltd k7, xmm12, xmm15
	vpcmpnltd k7, ymm12, ymm8
	vpcmpnltd k1, zmm10, zmm10
	vpcmpnltd k2, xmm16, xmm28
	vpcmpnltd k4, ymm18, ymm25
	vpcmpnltd k2, zmm17, zmm17
	vpcmpnltd k5{k5}, xmm3, xmm2
	vpcmpnltd k7{k3}, ymm6, yword [0x71f]
	vpcmpnltd k5{k6}, zmm0, zmm7
	vpcmpnltd k7, xmm3, oword [eax+1]
	vpcmpnltd k1, xmm2, oword [eax+64]
	vpcmpnltd k5, ymm3, yword [eax+1]
	vpcmpnltd k2, ymm2, yword [eax+64]
	vpcmpnltd k2, zmm1, zword [eax+1]
	vpcmpnltd k7, zmm4, zword [eax+64]
	vpcmpnltd k2, xmm5, [0x85d]
	vpcmpnltd k7, ymm3, [0xa30]
	vpcmpnltd k7, zmm4, [0xb33]
