default rel
	vpcmpgtd xmm5, xmm7, xmm1
	vpcmpgtd xmm5, xmm1
	vpcmpgtd xmm2, xmm2, oword [0x566]
	vpcmpgtd ymm7, ymm7, ymm5
	vpcmpgtd ymm7, ymm5
	vpcmpgtd ymm1, ymm4, ymm2
	vpcmpgtd k4, xmm0, oword [0xcf6]
	vpcmpgtd k3, xmm3, oword [0x997]
	vpcmpgtd k4, ymm5, ymm2
	vpcmpgtd k6, ymm4, yword [0x2b5]
	vpcmpgtd xmm9, xmm10, xmm13
	vpcmpgtd ymm12, ymm9, ymm9
	vpcmpgtd k7, xmm8, xmm15
	vpcmpgtd k4, ymm9, ymm11
	vpcmpgtd k5{k5}, xmm6, oword [0x8df]
	vpcmpgtd k2{k4}, ymm6, ymm6
	vpcmpgtd k2{k4}, zmm5, zmm1
	vpcmpgtd xmm3, xmm1, oword [eax+1]
	vpcmpgtd xmm6, xmm3, oword [eax+64]
	vpcmpgtd ymm4, ymm0, yword [eax+1]
	vpcmpgtd ymm0, ymm4, yword [eax+64]
	vpcmpgtd k7, xmm4, oword [eax+1]
	vpcmpgtd k3, xmm5, oword [eax+64]
	vpcmpgtd k4, ymm3, yword [eax+1]
	vpcmpgtd k6, ymm7, yword [eax+64]
	vpcmpgtd k1, zmm4, zword [eax+1]
	vpcmpgtd k6, zmm0, zword [eax+64]
	vpcmpgtd xmm2, xmm5, [0x1ae]
	vpcmpgtd ymm1, ymm7, [0x20f]
	vpcmpgtd k2, xmm6, [0xa4e]
	vpcmpgtd k2, ymm5, [0xc18]
	vpcmpgtd k4, zmm0, [0xe9b]
