default rel
	vpcmpgtuw k6, xmm0, oword [0xe75]
	vpcmpgtuw k3, xmm0, xmm3
	vpcmpgtuw k5, ymm4, yword [0xe9f]
	vpcmpgtuw k5, ymm6, ymm0
	vpcmpgtuw k3, zmm4, zmm4
	vpcmpgtuw k3, zmm6, zmm1
	vpcmpgtuw k7, xmm8, xmm12
	vpcmpgtuw k2, ymm15, ymm11
	vpcmpgtuw k7, zmm15, zmm10
	vpcmpgtuw k4, xmm31, xmm25
	vpcmpgtuw k3, ymm30, ymm21
	vpcmpgtuw k7, zmm25, zmm30
	vpcmpgtuw k2{k7}, xmm1, oword [0x59e]
	vpcmpgtuw k1{k5}, ymm5, ymm3
	vpcmpgtuw k4{k3}, zmm2, zword [0xdc3]
	vpcmpgtuw k6, xmm7, oword [eax+1]
	vpcmpgtuw k6, xmm5, oword [eax+64]
	vpcmpgtuw k5, ymm3, yword [eax+1]
	vpcmpgtuw k7, ymm1, yword [eax+64]
	vpcmpgtuw k7, zmm0, zword [eax+1]
	vpcmpgtuw k1, zmm7, zword [eax+64]
	vpcmpgtuw k2, xmm2, [0x206]
	vpcmpgtuw k5, ymm6, [0x279]
	vpcmpgtuw k1, zmm1, [0x3e3]
