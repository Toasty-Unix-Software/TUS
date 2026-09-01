default rel
	vpcmpltq k2, xmm1, xmm4
	vpcmpltq k2, xmm3, xmm3
	vpcmpltq k1, ymm3, ymm1
	vpcmpltq k6, ymm2, ymm3
	vpcmpltq k2, zmm2, zmm0
	vpcmpltq k6, zmm0, zword [0x98e]
	vpcmpltq k3, xmm15, xmm14
	vpcmpltq k7, ymm14, ymm14
	vpcmpltq k5, zmm13, zmm9
	vpcmpltq k5, xmm20, xmm25
	vpcmpltq k1, ymm24, ymm30
	vpcmpltq k7, zmm28, zmm28
	vpcmpltq k3{k5}, xmm0, xmm4
	vpcmpltq k3{k1}, ymm7, ymm2
	vpcmpltq k7{k1}, zmm7, zmm5
	vpcmpltq k4, xmm2, oword [eax+1]
	vpcmpltq k4, xmm1, oword [eax+64]
	vpcmpltq k2, ymm6, yword [eax+1]
	vpcmpltq k4, ymm0, yword [eax+64]
	vpcmpltq k4, zmm6, zword [eax+1]
	vpcmpltq k4, zmm6, zword [eax+64]
	vpcmpltq k6, xmm7, [0x549]
	vpcmpltq k3, ymm1, [0x7b1]
	vpcmpltq k6, zmm1, [0x861]
