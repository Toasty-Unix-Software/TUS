default rel
	vpcmpgeud k1, xmm2, oword [0x85a]
	vpcmpgeud k2, xmm7, oword [0x38e]
	vpcmpgeud k7, ymm4, ymm7
	vpcmpgeud k3, ymm7, ymm0
	vpcmpgeud k1, zmm6, zmm5
	vpcmpgeud k7, zmm1, zmm4
	vpcmpgeud k2, xmm11, xmm11
	vpcmpgeud k3, ymm10, ymm10
	vpcmpgeud k7, zmm11, zmm15
	vpcmpgeud k3, xmm31, xmm23
	vpcmpgeud k2, ymm26, ymm21
	vpcmpgeud k6, zmm17, zmm20
	vpcmpgeud k1{k7}, xmm5, xmm2
	vpcmpgeud k6{k5}, ymm0, yword [0xac5]
	vpcmpgeud k2{k5}, zmm7, zmm1
	vpcmpgeud k1, xmm4, oword [eax+1]
	vpcmpgeud k3, xmm7, oword [eax+64]
	vpcmpgeud k3, ymm5, yword [eax+1]
	vpcmpgeud k3, ymm0, yword [eax+64]
	vpcmpgeud k4, zmm6, zword [eax+1]
	vpcmpgeud k4, zmm6, zword [eax+64]
	vpcmpgeud k2, xmm1, [0xf00]
	vpcmpgeud k3, ymm4, [0x573]
	vpcmpgeud k5, zmm2, [0x2e5]
