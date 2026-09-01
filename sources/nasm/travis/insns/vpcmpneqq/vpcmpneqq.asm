default rel
	vpcmpneqq k6, xmm5, xmm4
	vpcmpneqq k7, xmm3, xmm5
	vpcmpneqq k1, ymm5, ymm7
	vpcmpneqq k1, ymm6, yword [0x816]
	vpcmpneqq k3, zmm6, zmm3
	vpcmpneqq k2, zmm4, zmm1
	vpcmpneqq k3, xmm11, xmm13
	vpcmpneqq k5, ymm8, ymm12
	vpcmpneqq k7, zmm15, zmm10
	vpcmpneqq k5, xmm17, xmm24
	vpcmpneqq k4, ymm24, ymm27
	vpcmpneqq k5, zmm16, zmm24
	vpcmpneqq k7{k1}, xmm6, xmm1
	vpcmpneqq k4{k2}, ymm5, ymm3
	vpcmpneqq k3{k2}, zmm6, zword [0x2ed]
	vpcmpneqq k4, xmm3, oword [eax+1]
	vpcmpneqq k1, xmm2, oword [eax+64]
	vpcmpneqq k7, ymm6, yword [eax+1]
	vpcmpneqq k4, ymm6, yword [eax+64]
	vpcmpneqq k5, zmm3, zword [eax+1]
	vpcmpneqq k2, zmm3, zword [eax+64]
	vpcmpneqq k4, xmm7, [0x486]
	vpcmpneqq k5, ymm0, [0xa29]
	vpcmpneqq k7, zmm2, [0x68b]
