default rel
	vpcmpuw k3, xmm2, xmm1, 0x1f
	vpcmpuw k3, xmm6, oword [0xd0d], 0xea
	vpcmpuw k3, ymm6, ymm6, 0x8f
	vpcmpuw k3, ymm0, ymm7, 0xd3
	vpcmpuw k6, zmm5, zword [0x287], 0x2
	vpcmpuw k2, zmm7, zword [0x53d], 0x98
	vpcmpuw k5, xmm11, xmm9, 0xb5
	vpcmpuw k2, ymm11, ymm11, 0x42
	vpcmpuw k4, zmm12, zmm14, 0xf0
	vpcmpuw k5, xmm19, xmm27, 0x8b
	vpcmpuw k1, ymm26, ymm28, 0x85
	vpcmpuw k1, zmm17, zmm22, 0x22
	vpcmpuw k6{k7}, xmm4, xmm2, 0xff
	vpcmpuw k5{k7}, ymm1, ymm6, 0x92
	vpcmpuw k7{k4}, zmm4, zmm1, 0xc6
	vpcmpuw k3, xmm4, oword [eax+1], 0xd3
	vpcmpuw k6, xmm7, oword [eax+64], 0x9d
	vpcmpuw k6, ymm0, yword [eax+1], 0xaa
	vpcmpuw k1, ymm7, yword [eax+64], 0xe8
	vpcmpuw k4, zmm0, zword [eax+1], 0xa9
	vpcmpuw k1, zmm5, zword [eax+64], 0x94
	vpcmpuw k3, xmm0, [0x73a], 0x4a
	vpcmpuw k1, ymm2, [0x7f5], 0x31
	vpcmpuw k3, zmm1, [0x647], 0x68
