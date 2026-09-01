default rel
	vpcmpb k4, xmm5, xmm0, 0xb5
	vpcmpb k5, xmm3, oword [0x43e], 0xc
	vpcmpb k3, ymm3, ymm3, 0xbb
	vpcmpb k7, ymm6, ymm6, 0xc0
	vpcmpb k1, zmm2, zmm4, 0xd2
	vpcmpb k6, zmm5, zmm3, 0x51
	vpcmpb k3, xmm15, xmm11, 0x3b
	vpcmpb k1, ymm14, ymm14, 0x74
	vpcmpb k4, zmm13, zmm13, 0xca
	vpcmpb k4, xmm29, xmm28, 0xbb
	vpcmpb k1, ymm31, ymm22, 0x48
	vpcmpb k2, zmm27, zmm29, 0xf7
	vpcmpb k2{k6}, xmm7, oword [0x7c8], 0x79
	vpcmpb k7{k1}, ymm2, yword [0x1ac], 0x2f
	vpcmpb k4{k5}, zmm2, zmm3, 0x1a
	vpcmpb k4, xmm2, oword [eax+1], 0xf7
	vpcmpb k6, xmm4, oword [eax+64], 0x87
	vpcmpb k1, ymm6, yword [eax+1], 0x5f
	vpcmpb k3, ymm1, yword [eax+64], 0x63
	vpcmpb k4, zmm1, zword [eax+1], 0xa1
	vpcmpb k6, zmm6, zword [eax+64], 0xc6
	vpcmpb k7, xmm2, [0x7dc], 0x2c
	vpcmpb k7, ymm5, [0x9e5], 0x7e
	vpcmpb k5, zmm1, [0x271], 0xfc
