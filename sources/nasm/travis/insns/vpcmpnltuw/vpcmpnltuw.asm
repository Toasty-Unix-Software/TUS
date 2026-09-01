default rel
	vpcmpnltuw k3, xmm4, oword [0xe37]
	vpcmpnltuw k1, xmm7, xmm6
	vpcmpnltuw k4, ymm3, ymm3
	vpcmpnltuw k3, ymm3, ymm6
	vpcmpnltuw k1, zmm5, zword [0x45a]
	vpcmpnltuw k6, zmm5, zmm2
	vpcmpnltuw k7, xmm8, xmm13
	vpcmpnltuw k6, ymm10, ymm15
	vpcmpnltuw k5, zmm14, zmm13
	vpcmpnltuw k1, xmm21, xmm30
	vpcmpnltuw k1, ymm16, ymm16
	vpcmpnltuw k7, zmm28, zmm20
	vpcmpnltuw k6{k4}, xmm3, xmm7
	vpcmpnltuw k4{k3}, ymm0, ymm7
	vpcmpnltuw k2{k3}, zmm0, zmm1
	vpcmpnltuw k1, xmm3, oword [eax+1]
	vpcmpnltuw k7, xmm6, oword [eax+64]
	vpcmpnltuw k7, ymm6, yword [eax+1]
	vpcmpnltuw k4, ymm2, yword [eax+64]
	vpcmpnltuw k2, zmm7, zword [eax+1]
	vpcmpnltuw k3, zmm3, zword [eax+64]
	vpcmpnltuw k2, xmm7, [0xa36]
	vpcmpnltuw k7, ymm2, [0x894]
	vpcmpnltuw k3, zmm4, [0xd84]
