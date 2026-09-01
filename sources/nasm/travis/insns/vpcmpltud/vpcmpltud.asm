default rel
	vpcmpltud k5, xmm3, oword [0x256]
	vpcmpltud k6, xmm1, oword [0xd3d]
	vpcmpltud k1, ymm0, ymm4
	vpcmpltud k4, ymm7, ymm0
	vpcmpltud k4, zmm4, zmm0
	vpcmpltud k7, zmm5, zmm5
	vpcmpltud k4, xmm10, xmm11
	vpcmpltud k7, ymm11, ymm12
	vpcmpltud k4, zmm15, zmm13
	vpcmpltud k1, xmm21, xmm29
	vpcmpltud k7, ymm30, ymm21
	vpcmpltud k1, zmm19, zmm23
	vpcmpltud k7{k7}, xmm2, xmm0
	vpcmpltud k7{k1}, ymm0, yword [0xb93]
	vpcmpltud k5{k7}, zmm6, zmm6
	vpcmpltud k1, xmm6, oword [eax+1]
	vpcmpltud k3, xmm7, oword [eax+64]
	vpcmpltud k3, ymm0, yword [eax+1]
	vpcmpltud k5, ymm5, yword [eax+64]
	vpcmpltud k3, zmm0, zword [eax+1]
	vpcmpltud k3, zmm5, zword [eax+64]
	vpcmpltud k5, xmm6, [0x659]
	vpcmpltud k7, ymm6, [0xd04]
	vpcmpltud k6, zmm7, [0xbcb]
