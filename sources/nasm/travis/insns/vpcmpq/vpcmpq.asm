default rel
	vpcmpq k6, xmm7, oword [0x78e], 0xc2
	vpcmpq k2, xmm4, xmm6, 0x45
	vpcmpq k2, ymm6, ymm1, 0xb9
	vpcmpq k6, ymm6, yword [0x45c], 0x38
	vpcmpq k4, zmm1, zword [0x517], 0x1a
	vpcmpq k6, zmm6, zmm3, 0xe9
	vpcmpq k7, xmm10, xmm14, 0x95
	vpcmpq k1, ymm14, ymm14, 0x90
	vpcmpq k4, zmm11, zmm8, 0xf2
	vpcmpq k2, xmm16, xmm27, 0x4a
	vpcmpq k1, ymm23, ymm28, 0x13
	vpcmpq k7, zmm28, zmm30, 0x33
	vpcmpq k1{k7}, xmm2, oword [0xd2c], 0x96
	vpcmpq k4{k6}, ymm3, ymm4, 0x81
	vpcmpq k7{k2}, zmm7, zmm1, 0x9e
	vpcmpq k3, xmm1, oword [eax+1], 0x20
	vpcmpq k6, xmm2, oword [eax+64], 0xb6
	vpcmpq k2, ymm4, yword [eax+1], 0x9f
	vpcmpq k6, ymm5, yword [eax+64], 0x5b
	vpcmpq k3, zmm6, zword [eax+1], 0xd5
	vpcmpq k5, zmm1, zword [eax+64], 0x9d
	vpcmpq k6, xmm0, [0xed4], 0x68
	vpcmpq k6, ymm1, [0xbc1], 0x94
	vpcmpq k1, zmm5, [0x2c8], 0x73
