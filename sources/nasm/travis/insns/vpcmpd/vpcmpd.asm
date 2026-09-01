default rel
	vpcmpd k7, xmm1, oword [0xb83], 0x34
	vpcmpd k3, xmm3, xmm5, 0x61
	vpcmpd k3, ymm2, ymm1, 0xbb
	vpcmpd k7, ymm1, ymm2, 0xf0
	vpcmpd k6, zmm1, zmm1, 0x38
	vpcmpd k4, zmm0, zword [0xddc], 0xcf
	vpcmpd k1, xmm14, xmm9, 0x1b
	vpcmpd k7, ymm12, ymm12, 0x1e
	vpcmpd k2, zmm12, zmm10, 0xcc
	vpcmpd k7, xmm28, xmm18, 0x1
	vpcmpd k2, ymm16, ymm24, 0x9a
	vpcmpd k7, zmm19, zmm21, 0x87
	vpcmpd k2{k7}, xmm3, oword [0xddf], 0xd6
	vpcmpd k3{k5}, ymm3, ymm7, 0x40
	vpcmpd k2{k6}, zmm0, zmm7, 0x69
	vpcmpd k6, xmm4, oword [eax+1], 0xb2
	vpcmpd k1, xmm2, oword [eax+64], 0x7f
	vpcmpd k3, ymm7, yword [eax+1], 0xc1
	vpcmpd k4, ymm5, yword [eax+64], 0x51
	vpcmpd k3, zmm3, zword [eax+1], 0x4c
	vpcmpd k1, zmm3, zword [eax+64], 0xf2
	vpcmpd k3, xmm6, [0xcd8], 0xf7
	vpcmpd k7, ymm5, [0x73e], 0xde
	vpcmpd k5, zmm2, [0xe6e], 0x53
