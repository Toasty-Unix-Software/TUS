default rel
	vpcmpngtd k4, xmm0, oword [0x8c7]
	vpcmpngtd k6, xmm5, xmm7
	vpcmpngtd k1, ymm5, ymm3
	vpcmpngtd k3, ymm0, ymm0
	vpcmpngtd k6, zmm7, zmm5
	vpcmpngtd k6, zmm6, zword [0x506]
	vpcmpngtd k6, xmm10, xmm9
	vpcmpngtd k7, ymm15, ymm13
	vpcmpngtd k2, zmm9, zmm14
	vpcmpngtd k2, xmm21, xmm27
	vpcmpngtd k4, ymm26, ymm20
	vpcmpngtd k4, zmm24, zmm22
	vpcmpngtd k7{k4}, xmm5, oword [0xb8d]
	vpcmpngtd k6{k6}, ymm3, ymm4
	vpcmpngtd k5{k5}, zmm4, zword [0x146]
	vpcmpngtd k3, xmm5, oword [eax+1]
	vpcmpngtd k1, xmm6, oword [eax+64]
	vpcmpngtd k2, ymm1, yword [eax+1]
	vpcmpngtd k4, ymm1, yword [eax+64]
	vpcmpngtd k4, zmm4, zword [eax+1]
	vpcmpngtd k6, zmm1, zword [eax+64]
	vpcmpngtd k5, xmm2, [0xc5e]
	vpcmpngtd k5, ymm0, [0xa5c]
	vpcmpngtd k4, zmm0, [0x853]
