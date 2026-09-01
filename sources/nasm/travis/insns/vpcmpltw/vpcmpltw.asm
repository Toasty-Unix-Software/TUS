default rel
	vpcmpltw k2, xmm2, xmm7
	vpcmpltw k6, xmm1, oword [0x315]
	vpcmpltw k4, ymm1, ymm4
	vpcmpltw k1, ymm4, ymm5
	vpcmpltw k5, zmm1, zmm2
	vpcmpltw k7, zmm0, zword [0xc76]
	vpcmpltw k4, xmm8, xmm14
	vpcmpltw k7, ymm13, ymm13
	vpcmpltw k7, zmm11, zmm14
	vpcmpltw k6, xmm29, xmm30
	vpcmpltw k6, ymm29, ymm19
	vpcmpltw k2, zmm23, zmm24
	vpcmpltw k3{k3}, xmm3, xmm3
	vpcmpltw k6{k2}, ymm0, ymm0
	vpcmpltw k3{k7}, zmm4, zmm3
	vpcmpltw k3, xmm3, oword [eax+1]
	vpcmpltw k4, xmm2, oword [eax+64]
	vpcmpltw k5, ymm0, yword [eax+1]
	vpcmpltw k5, ymm1, yword [eax+64]
	vpcmpltw k4, zmm2, zword [eax+1]
	vpcmpltw k3, zmm2, zword [eax+64]
	vpcmpltw k2, xmm5, [0x98a]
	vpcmpltw k3, ymm5, [0x422]
	vpcmpltw k7, zmm6, [0x140]
